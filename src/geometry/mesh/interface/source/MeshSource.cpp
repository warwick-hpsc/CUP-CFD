/*
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * A wrapper for a compressed sparse row representation of some data.
 *
 */

// C++ Library
#include <string>

// Function/Class Declaration
#include "MeshSource.h"

// Utility Functions
#include "ArrayDrivers.h"
#include "StatisticsDrivers.h"
#include "SortDrivers.h"

namespace cupcfd
{
	namespace geometry
	{
		namespace mesh
		{
			template <class I, class T, class L>
			MeshSource<I,T,L>::MeshSource()
			{
				this->gidBase = 0;
			}

			template <class I, class T, class L>
			MeshSource<I,T,L>::~MeshSource()
			{
				// Do nothing
			}

			// === Pure Methods ===
			// Empty implementations to satisfy linker

			template <class I, class T, class L>
			cupcfd::error::eCodes MeshSource<I,T,L>::getCellLabels(L * labels, I nLabels, I * indexes, I nIndexes)
			{
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			cupcfd::error::eCodes MeshSource<I,T,L>::getFaceLabels(L * labels, I nLabels, I * indexes, I nIndexes)
			{
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			cupcfd::error::eCodes MeshSource<I,T,L>::getVertexLabels(L * labels, I nLabels, I * indexes, I nIndexes)
			{
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			cupcfd::error::eCodes MeshSource<I,T,L>::getBoundaryLabels(L * labels, I nLabels, I * indexes, I nIndexes)
			{
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			cupcfd::error::eCodes MeshSource<I,T,L>::getRegionLabels(L * labels, I nLabels, I * indexes, I nIndexes)
			{
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			cupcfd::error::eCodes MeshSource<I,T,L>::getCellCount(I * cellCount)
			{
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			cupcfd::error::eCodes MeshSource<I,T,L>::getFaceCount(I * faceCount)
			{
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			cupcfd::error::eCodes MeshSource<I,T,L>::getBoundaryCount(I * boundaryCount)
			{
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			cupcfd::error::eCodes MeshSource<I,T,L>::getRegionCount(I * regionCount)
			{
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			cupcfd::error::eCodes MeshSource<I,T,L>::getVertexCount(I * vertexCount)
			{
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			cupcfd::error::eCodes MeshSource<I,T,L>::getMaxFaceCount(I * maxFaceCount)
			{
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			cupcfd::error::eCodes MeshSource<I,T,L>::getMaxVertexCount(I * maxVertexCount)
			{
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			cupcfd::error::eCodes MeshSource<I,T,L>::getCellNFaces(I * nFaces, I nNFaces, L * cellLabels, I nCellLabels)
			{
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			cupcfd::error::eCodes MeshSource<I,T,L>::getCellVolume(T * cellVol, I nCellVol, L * cellLabels, I nCellLabels)
			{
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			cupcfd::error::eCodes MeshSource<I,T,L>::getCellCenter(cupcfd::geometry::euclidean::EuclideanPoint<T,3> * cellCenter, I nCellCenter, L * cellLabels, I nCellLabels)
			{
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			cupcfd::error::eCodes MeshSource<I,T,L>::getCellFaceLabels(I * csrIndices, I nCsrIndices, I * csrData, I nCsrData, L * cellLabels, I nCellLabels)
			{
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			cupcfd::error::eCodes MeshSource<I,T,L>::getFaceIsBoundary(bool * isBoundary, I nIsBoundary, L * faceLabels, I nFaceLabels)
			{
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			cupcfd::error::eCodes MeshSource<I,T,L>::getFaceNVertices(I * nVertices, I nNVertices, L * faceLabels, I nFaceLabels)
			{
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			cupcfd::error::eCodes MeshSource<I,T,L>::getFaceBoundaryLabels(I * faceBoundaryLabels, I nFaceBoundaryLabels, L * faceLabels, I nFaceLabels)
			{
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			cupcfd::error::eCodes MeshSource<I,T,L>::getFaceCell1Labels(I * faceCell1Labels, I nFaceCell1Labels, L * faceLabels, I nFaceLabels)
			{
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			cupcfd::error::eCodes MeshSource<I,T,L>::getFaceCell2Labels(I * faceCell2Labels, I nFaceCell2Labels, L * faceLabels, I nFaceLabels)
			{
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			cupcfd::error::eCodes MeshSource<I,T,L>::getFaceArea(T * faceLambda, I nFaceLambda, L * faceLabels, I nFaceLabels)
			{
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			cupcfd::error::eCodes MeshSource<I,T,L>::getFaceLambda(T * faceLambda, I nFaceLambda, L * faceLabels, I nFaceLabels)
			{
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			cupcfd::error::eCodes MeshSource<I,T,L>::getFaceNormal(cupcfd::geometry::euclidean::EuclideanVector<T,3> * faceNormal, I nFaceNormal, L * faceLabels, I nFaceLabels)
			{
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			cupcfd::error::eCodes MeshSource<I,T,L>::getFaceCenter(cupcfd::geometry::euclidean::EuclideanPoint<T,3> * faceCenter, I nFaceCenter, L * faceLabels, I nFaceLabels)
			{
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			cupcfd::error::eCodes MeshSource<I,T,L>::getFaceVerticesLabelsCSR(I * csrIndices, I nCsrIndices, I * csrData, I nCsrData,  L * faceLabels, I nFaceLabels)
			{
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			cupcfd::error::eCodes MeshSource<I,T,L>::getVertexCoords(cupcfd::geometry::euclidean::EuclideanPoint<T,3> * vertCoords, I nVertCoords, L * vertexLabels, I nVertexLabels)
			{
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			cupcfd::error::eCodes MeshSource<I,T,L>::getBoundaryFaceLabels(I * boundaryFaceLabels, I nBoundaryFaceLabels, L * boundaryLabels, I nBoundaryLabels)
			{
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			cupcfd::error::eCodes MeshSource<I,T,L>::getBoundaryNVertices(I * nVertices, I nNVertices, L * boundaryLabels, I nBoundaryLabels)
			{
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			cupcfd::error::eCodes MeshSource<I,T,L>::getBoundaryRegionLabels(I * boundaryRegionLabels, I nBoundaryRegionLabels, L * boundaryLabels, I nBoundaryLabels)
			{
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			cupcfd::error::eCodes MeshSource<I,T,L>::getBoundaryVerticesLabelsCSR(I * csrIndices, I nCsrIndices, I * csrData, I nCsrData,  L * faceLabels, I nFaceLabels)
			{
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			cupcfd::error::eCodes MeshSource<I,T,L>::getBoundaryDistance(T * boundaryDistance, I nBoundaryDistance, L * boundaryLabels, I nBoundaryLabels)
			{
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			cupcfd::error::eCodes MeshSource<I,T,L>::getRegionName(std::string * names, I nNames, L * regionLabels, I nRegionLabels)
			{
				return cupcfd::error::E_SUCCESS;
			}

			// === Concrete Methods ===

	/*
			template <class I, class T, class L>
			cupcfd::error::eCodes MeshSource<I,T,L>::getConnectivityGraph(cupcfd::adjacency_list::AdjacencyListCSR<I, I>& graph)
			{
				cupcfd::error::eCodes status;

				// We use an empty adjacency list vector instead of the CSR version to build,
				// since there should be less array shifting and thus less of a performance hit.
				adjacency_list::AdjacencyListVector<I,I> buildGraph;

				// (1) Get Number of Cells
				I cellCount;
				status = this->getCellCount(&cellCount);

				// (2) To get the face indexes, we need to know the number of adjacent faces
				//     This is found from the sum of the adjacent face count for each cell (nFaces)
				I * nFaces = (I *) malloc(sizeof(I) * cellCount);
				status = this->getCellNFaces(nFaces, cellCount);

				I sum;
				cupcfd::utility::drivers::sum(nFaces, cellCount, &sum);

				// Free this now we no longer need it.
				free(nFaces);

				// (3) Get Face Indexes - this will store in CSR: indexes[cell] -> data[indexes[cell]] format
				I * pos = (I *) malloc(sizeof(I) * (cellCount + 1));
				I * data = (I *) malloc(sizeof(I) * sum);
				status = this->getCellFaceLabels(pos, cellCount + 1, data, sum);

				// (4) We now need to find which cells are adjacent for the graph.
				//     To do this, we need the cell1 and cell2 data from the face data,
				//	   which details the global cell indexes either side of the face.

				I faceCount;
				status = this->getFaceCount(&faceCount);

				I * faceCell1 = (I *) malloc(sizeof(I) * faceCount);
				I * faceCell2 = (I *) malloc(sizeof(I) * faceCount);
				bool * isBoundary = (bool *) malloc(sizeof(bool) * faceCount);

				status = this->getFaceCell1Labels(faceCell1, faceCount);
				status = this->getFaceCell2Labels(faceCell2, faceCount);
				status = this->getFaceIsBoundary(isBoundary, faceCount);

				// (5) Now we can start building the graph
				// (a) First, we add the nodes (since this is the full dataset, it is just
				//     a sequence form the base to the max index (in this file format, base index 1)
				// (b) For each node, look up the attached faces, and then cross-reference with the
				//     face cell1, cell2 data to find out what the other face is.

				// Add Nodes
				I base = 1;
				for(I i = 0; i < cellCount; i++)
				{
					I label = i;

					status = buildGraph.addNode(label);

					if(status != cupcfd::error::E_SUCCESS)
					{
						// Failed to add a node
						return status;
					}
				}

				// Add Edges

				for(I i = 0; i < cellCount; i++)
				{
					I cellLabel = i;

					I ptr = pos[cellLabel];
					I edgeCount = pos[cellLabel + 1] - pos[cellLabel];

					for(I j = ptr; j < ptr + edgeCount; j++)
					{
						I faceLabel = data[j];

						// Check that it is an face between two cells
						if(!(isBoundary[faceLabel]))
						{
							if(faceCell1[faceLabel] == cellLabel)
							{
								// Current cell is cell1: Direction is cell1->cell2
								status = buildGraph.addEdge(cellLabel, faceCell2[faceLabel]);
							}
							else if(faceCell2[faceLabel] == cellLabel)
							{
								// Current cell is cell1: Direction is cell2->cell1
								status = buildGraph.addEdge(cellLabel, faceCell1[faceLabel]);
							}
							else
							{
								// Mismatch between cell->face and face->cell - error
								return cupcfd::error::E_ERROR;
							}
						}
					}
				}

				// Free up remaining temporary storage
				free(faceCell1);
				free(faceCell2);
				free(isBoundary);
				free(pos);
				free(data);

				// Initialise the CSR version with data from the vector version of the adjacency_list
				graph.initAdjacencyList(buildGraph);

				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			cupcfd::error::eCodes MeshSource<I,T,L>::partitionMeshSource(cupcfd::partitioner::Partitioner<I,I>& partitionEngine,
																		cupcfd::mpi::Communicator& workComm,
																		cupcfd::adjacency_list::DistributedAdjacencyList<I, I>& resultGraph)
			{
				cupcfd::error::eCodes status;

				// Note: to avoid blocking behaviour, all ranks of either the partitioner comm or resultGraph comm must call this method,
				// even if it is not a member of both, else one of two stages will stall waiting on a blocking action.

				// Reset the partitioner
				partitionEngine.reset();

				// (a) Divide the mesh file evenly across the processes that make up the partitioner
				I nGCells;
				status = this->getCellCount(&nGCells);

				// (b) Get the connectivity graph for this 'even' distribution
				// This graph needs to be shared evenly across the partitioner comm, so use that
				// ToDo: On reflection, with this structure, all processes calling this method have access to both the partitioner
				// object and the resultGraph, so they must be the same comm, since they have to be a member of both and all members
				// have to call this method....

				I r = nGCells % resultGraph.comm.size;

				I nLCells;

				// Compute the local cell count, distribute a remainder amongst the lower ranks
				if(workComm.rank < r)
				{
					nLCells = ((I) nGCells) / ((I) resultGraph.comm.size) + 1;
				}
				else
				{
					nLCells = ((I) nGCells) / ((I) resultGraph.comm.size);
				}

				I base;
				I * naiveLocalCells = (I *) malloc(sizeof(I) * nLCells);

				if(workComm.rank < r)
				{
					base = (((I) nGCells) / ((I) resultGraph.comm.size) + 1) * resultGraph.comm.rank;
				}
				else
				{
					base = ((((I) nGCells) / ((I) resultGraph.comm.size) + 1) * r) +
							   (((I) nGCells) / ((I) resultGraph.comm.size) * (resultGraph.comm.rank - r));
				}

				// Assign local nodes naively - each rank get a range from base -> base+nLCells
				// where base is assigned such that e.g. [0->9|10->19...] to divide into segments
				for(I i = 0; i < nLCells; i++)
				{
					naiveLocalCells[i] = base + i;
				}

				// Create a temporary connectivity graph to store the graph prior to partitioning
				cupcfd::adjacency_list::DistributedAdjacencyList<I, I> * naiveGraph = new adjacency_list::DistributedAdjacencyList<I, I>(resultGraph.comm);
				this->getConnectivityGraph(*naiveGraph, naiveLocalCells, nLCells);

				// (c) The naive partition is unlikely to be the best - use the partitioner to partition this initial distributed graph
				// for the output graph
				status = partitionEngine.partition(*naiveGraph, resultGraph.comm.size);

				// (d) Distribute and retrieve the cell labels that have been allocated to this rank
				// The assignRankNodes will communicate the *nodes* from the naiveGraph that have been allocated to this rank - in this
				// case the nodes will be the cell labels. We can then use these to get a new connectivity graph with the new set of
				// local nodes rather than the naive ones.

				I * partNodes;
				I nPartNodes;
				partitionEngine.assignRankNodes(&partNodes, &nPartNodes, resultGraph.comm);

				// (e) Use this new node allocation to re-read from the source file again, and retrieve a connectivity graph with
				// the assigned nodes as local nodes instead.
				this->getConnectivityGraph(resultGraph, partNodes, nPartNodes);

				// (f) Any remaining cleanup
				free(partNodes);
				delete(naiveGraph);

				return cupcfd::error::E_SUCCESS;
			}
	*/
		}
	} // namespace geometry
} // namespace cupcfd

// Explicit Instantiation
template class cupcfd::geometry::mesh::MeshSource<int, float, int>;
template class cupcfd::geometry::mesh::MeshSource<int, double, int>;
