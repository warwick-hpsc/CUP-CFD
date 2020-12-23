/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Contains the declarations for the MeshSource Class.
 */

#ifndef CUPCFD_GEOMETRY_MESH_SOURCE_IPP_H
#define CUPCFD_GEOMETRY_MESH_SOURCE_IPP_H

namespace cupcfd
{
	namespace geometry
	{
		namespace mesh
		{
			template <class I, class T, class L>
			cupcfd::error::eCodes MeshSource<I,T,L>::buildDistributedAdjacencyList(cupcfd::data_structures::DistributedAdjacencyList<I,I> ** graph,
																					cupcfd::comm::Communicator& comm,
																					L * cellLabels, I nCellLabels)
			{
				// This function builds a connectivity graph.
				// The cell labels provided are the ones allocated to this process rank.

				// ToDo: Do we want to add an error check here to ensure that the provided cellLabels
				// are valid for the source?

				cupcfd::error::eCodes status;

				// (1) Get Number of Cells
				// In this case, the number of cells we are interested in is the portion stated by nCellLabels
				I cellCount = nCellLabels;

				*graph = new cupcfd::data_structures::DistributedAdjacencyList<I,I>(comm);

				// Only add cells if the number is greater than 0
				if(cellCount > 0) {
					// Let us ensure that the cells are added in a sorted order
					I * sortedCellLabels = (I *) malloc(sizeof(I) * nCellLabels);
					cupcfd::utility::drivers::merge_sort(cellLabels, sortedCellLabels, nCellLabels);

					// (2) To get the face indexes, we need to know the number of adjacent faces
					//     This is found from the sum of the adjacent face count for each cell listed (nFaces)
					I * nFaces = (I *) malloc(sizeof(I) * cellCount);
					status = this->getCellNFaces(nFaces, cellCount, sortedCellLabels, nCellLabels);
					CHECK_ECODE(status)

					I numEdges;
					cupcfd::utility::drivers::sum(nFaces, cellCount, &numEdges);

					// Free this now we no longerbuildDistributedAdjacencyList need it.
					free(nFaces);

					// (3) Get Face Indexes, but only for the cells we are interested in.
					// This should give us them in a CSR format
					I * pos = (I *) malloc(sizeof(I) * (cellCount + 1));
					I * data = (I *) malloc(sizeof(I) * numEdges);
					status = this->getCellFaceLabels(pos, cellCount + 1, data, numEdges, sortedCellLabels, nCellLabels);
					CHECK_ECODE(status)

					// (4) We now need to find which cells are adjacent for the graph.
					//     To do this, we need the cell1 and cell2 data from the face data,
					//	   which details the global cell indexes either side of the face.

					// We only need the face data for the faces attached to the requested cell data
					// However, using the data array, there will be duplicates.
					// We could remove them at this stage, but then we would have to update indexes so
					// for the time being we will just load duplicates.
					I * faceCell1 = (I *) malloc(sizeof(I) * numEdges);
					I * faceCell2 = (I *) malloc(sizeof(I) * numEdges);
					bool * faceIsBoundary = (bool *) malloc(sizeof(bool) * numEdges);

					status = this->getFaceCell1Labels(faceCell1, numEdges, data, numEdges);
					CHECK_ECODE(status)

					status = this->getFaceCell2Labels(faceCell2, numEdges, data, numEdges);
					CHECK_ECODE(status)

					status = this->getFaceIsBoundary(faceIsBoundary, numEdges, data, numEdges);
					CHECK_ECODE(status)

					// (5) Now we can start building the graph
					// (a) First, we add the nodes (since this is the full dataset, it is just
					//     a sequence from the base to the max index (in this file format, base index 1)
					// (b) For each node, look up the attached faces, and then cross-reference with the
					//     face cell1, cell2 data to find out what the other face is.

					// Add Local Nodes
					for(I i = 0; i < cellCount; i++) {
						status = (*graph)->addLocalNode(sortedCellLabels[i]);
						CHECK_ECODE(status)
					}

					// Add Edges
					// Since we stored duplicate face data, the position of the face data in the faceCell1, faceCell2
					// arrays is the same as the position in the 'data' array for cell face indexes
					// I.e. we can loop over the faces instead of cells and just keep adding edges (at most we would try
					// to add the same undirected edge twice)
					// If we removed duplicate data to only store every face once, then we would have to store face indexes
					// alongside and do a suitable search/lookup mechanism for every cell - this would use less memory but
					// makes for more complex lookups. Since this is a distributed function, the extra memory overhead
					// shouldn't be too much of an issue).

					for(I i = 0; i < numEdges; i++) {
						if(!(faceIsBoundary[i])) {
							// Shouldn't matter if edge already exists, will just cause an error to be thrown without adding
							// the edge again.
							// If a node/cell is missing (i.e. not added as a local), the functionality of this method
							// will add the node as a ghost node.
							// ToDo: If this changes in the future we will need to revisit this.
							status = (*graph)->addUndirectedEdge(faceCell1[i], faceCell2[i]);
							CHECK_ECODE(status)
						}
					}

					// Free up remaining temporary storage
					free(faceCell1);
					free(faceCell2);
					free(faceIsBoundary);
					free(pos);
					free(data);
				}

				// Finalize the Distributed Adjacency Graph so everyone is aware of their neighbours
				status = (*graph)->finalize();
				CHECK_ECODE(status)

				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			cupcfd::error::eCodes MeshSource<I,T,L>::buildDistributedAdjacencyList(cupcfd::data_structures::DistributedAdjacencyList<I,I> ** graph,
																					cupcfd::comm::Communicator& comm)
			{
				cupcfd::error::eCodes status;

				// Note: to avoid blocking behaviour, all ranks of comm must call this method.

				// (a) Divide the mesh file evenly across the processes that make up the partitioner
				// This likely won't make for a good edge allocation, but this function makes no promises
				// about the setup.
				I nGCells, nLCells, r;

				status = this->getCellCount(&nGCells);
				CHECK_ECODE(status)

				// (b) Get the connectivity graph for this 'even' distribution
				r = nGCells % comm.size;

				// Compute the local cell count, distribute a remainder amongst the lower ranks
				if(comm.rank < r) {
					nLCells = ((I) nGCells) / ((I) comm.size) + 1;
				}
				else {
					nLCells = ((I) nGCells) / ((I) comm.size);
				}

				I base;
				I * naiveLocalCells = (I *) malloc(sizeof(I) * nLCells);

				if(comm.rank < r) {
					base = (((I) nGCells) / ((I) comm.size) + 1) * comm.rank;
				}
				else {
					base = ((((I) nGCells) / ((I) comm.size) + 1) * r) +
							   (((I) nGCells) / ((I) comm.size) * (comm.rank - r));
				}

				// Assign local nodes naively - each rank get a range from base -> base+nLCells
				// where base is assigned such that e.g. [0->9|10->19...] to divide into segments
				for(I i = 0; i < nLCells; i++) {
					naiveLocalCells[i] = base + i;
				}

				// Convert these cell indices into cell labels from the source
				I * cellLabels = (I *) malloc(sizeof(I) * nLCells);
				status = this->getCellLabels(cellLabels, nLCells, naiveLocalCells, nLCells);
				CHECK_ECODE(status)

				// Build the connectivity graph using the cell allocation built above
				status = this->buildDistributedAdjacencyList(graph, comm, cellLabels, nLCells);
				CHECK_ECODE(status)

				free(naiveLocalCells);
				free(cellLabels);

				return cupcfd::error::E_SUCCESS;
			}
		}
	}
}

#endif
