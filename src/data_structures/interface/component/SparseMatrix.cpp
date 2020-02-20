/*
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Stores a sparse matrix for values of arbitrary row and column.
 *
 * This file is the implementation for the concrete methods of the abstract SparseMatrix class
 *
 */

#include "SparseMatrix.h"
#include "ArrayKernels.h"

#include <iostream>

namespace cupcfd
{
	namespace data_structures
	{

		/*
		// ToDo: Fix this template so that we have a third type for the nodes of the graph we can generate from
		// We keep it seperate since technically there are three types
		// I: the index type
		// T: the matrix data type
		// K: the graph node type (which is probably an int, but doesn't have to be since we lookup separate node indexes)
		template <class C, class I, class T>
		cupcfd::error::eCodes SparseMatrix<C,I,T>::populateUniformRandomGlobalIndexing(cupcfd::adjacency_list::DistributedAdjacencyList<I, int>& graph,
																						  T minVal,
																						  T maxVal)
		{
			// Error Check: Check the graph is finalized
			if(graph.finalized == false)
			{
				return cupcfd::error::E_DISTGRAPH_UNFINALIZED;
			}

			// (1) Reset the matrix

			// (2) Reinitialise the matrix to a size that could theoretically hold all of the rows in the graph.
			// ToDo: Since it it sparse, it shouldn't matter too much if we use the global values, but some structures
			// might store a piece of data per row, so it could be wasteful of memory?
			// We could set it to only use the min and max row indexes local to this process instead since in parallel setups
			// we are only storing a portion of the global matrix
			// ToDo Alternate: Make a Distributed Matrix Object that tracks the global capacity separate from the SparseMatrix object
			// which would represent the local storage

			// This should also clear the matrix before resizing (essentially a reset and reinit)

			this->resize(graph.nGNodes, graph.nGNodes);

			// Get a list of the local cells
			I nLONodes = graph.nLONodes;
			int * lONodes = (int *) malloc(sizeof(int) * nLONodes);
			graph.getLocalNodes(lONodes, nLONodes);

			// For each node, add a row of values. One value is added per neighbour, at the column index matching
			// the neighbour's global ID
			for(I i = 0; i < nLONodes; i++)
			{
				I rowGID = graph.nodeToGlobal[lONodes[i]];

				// Get the neighbouring nodes
				// ToDo: We dig into the graph object here to get the nodes, we should probably hide this by adding
				// an interface function in the distributed graph object
				I nCols;
				graph.connGraph.getAdjacentNodeCount(lONodes[i], &nCols);

				// Get neighbour nodes
				int * neighbourNodes = (int *) malloc(sizeof(int) * nCols);
				graph.connGraph.getAdjacentNodes(lONodes[i], neighbourNodes, nCols);

				// For each neighbour, add a value to the matrix on this row
				for(I j = 0; j < nCols; j++)
				{
					// Generate a random value
					// ToDo: Probably expensive to call one-by-one
					// More expensive on memory, but we could do this in one call
					// by just generating a full array of random values then iterating through
					T val;
					cupcfd::utility::kernels::randomUniform(&val, 1, minVal, maxVal);

					I colGID = graph.nodeToGlobal[neighbourNodes[j]];
					this->setElement(rowGID, colGID, val);
				}

				// Add a value for itself->itself (the diagonal)
				T val;
				cupcfd::utility::kernels::randomUniform(&val, 1, minVal, maxVal);
				this->setElement(rowGID, rowGID, val);

				// Cleanup
				free(neighbourNodes);
			}

			// Cleanup
			free(lONodes);

			return cupcfd::error::E_SUCCESS;
		}

		template <class C, class I, class T>
		cupcfd::error::eCodes SparseMatrix<C,I,T>::populateUniformRandomLocalIndexing(cupcfd::adjacency_list::DistributedAdjacencyList<I, int>& graph,
																						 T minVal,
																						 T maxVal)
		{
			// Error Check: Check the graph is finalized
			if(graph.finalized == false)
			{
				return cupcfd::error::E_DISTGRAPH_UNFINALIZED;
			}

			// (1) Reset the matrix

			// (2) Reinitialise the matrix to a size that could theoretically hold all of the rows in the graph.
			// ToDo: Since it it sparse, it shouldn't matter too much if we use the global values, but some structures
			// might store a piece of data per row, so it could be wasteful of memory?
			// We could set it to only use the min and max row indexes local to this process instead since in parallel setups
			// we are only storing a portion of the global matrix
			// ToDo Alternate: Make a Distributed Matrix Object that tracks the global capacity separate from the SparseMatrix object
			// which would represent the local storage

			// This should also clear the matrix before resizing (essentially a reset and reinit)

			this->resize(graph.nLONodes + graph.nLGhNodes, graph.nLONodes + graph.nLGhNodes);

			// Get a list of the local cells
			I nLONodes = graph.nLONodes;
			I nLGhNodes = graph.nLGhNodes;

			int * nodes = (int *) malloc(sizeof(int) * (nLONodes + nLGhNodes));
			graph.getLocalNodes(nodes, nLONodes);
			graph.getGhostNodes(nodes + nLONodes, nLGhNodes);

			// For each node, add a row of values. One value is added per neighbour, at the column index matching
			// the neighbour's global ID
			// We also add the ghost cells as rows since we need the matrix to be valid for a serial solve
			for(I i = 0; i < nLONodes + graph.nLGhNodes; i++)
			{
				// Get the local ID for the node
				I rowLID = graph.connGraph.nodeToIDX[nodes[i]];

				// Get the neighbouring nodes
				// ToDo: We dig into the graph object here to get the nodes, we should probably hide this by adding
				// an interface function in the distributed graph object
				I nCols;
				graph.connGraph.getAdjacentNodeCount(nodes[i], &nCols);

				// Get neighbour nodes
				int * neighbourNodes = (int *) malloc(sizeof(int) * nCols);
				graph.connGraph.getAdjacentNodes(nodes[i], neighbourNodes, nCols);

				// For each neighbour, add a value to the matrix on this row
				for(I j = 0; j < nCols; j++)
				{
					// Generate a random value
					// ToDo: Probably expensive to call one-by-one
					// More expensive on memory, but we could do this in one call
					// by just generating a full array of random values then iterating through
					T val;
					cupcfd::utility::kernels::randomUniform(&val, 1, minVal, maxVal);

					// Get the localID for the neighbourNode
					I colLID = graph.connGraph.nodeToIDX[neighbourNodes[j]];
					this->setElement(rowLID, colLID, val);
				}

				// Add a value for itself->itself (the diagonal)
				T val;
				cupcfd::utility::kernels::randomUniform(&val, 1, minVal, maxVal);
				this->setElement(rowLID, rowLID, val);

				// Cleanup
				free(neighbourNodes);
			}

			// Cleanup
			free(nodes);

			return cupcfd::error::E_SUCCESS;
		}

		 */
	}
}

// Explicit Instantiation
//template class cupcfd::data_structures::SparseMatrix<int, int>;
//template class cupcfd::data_structures::SparseMatrix<int, float>;
//template class cupcfd::data_structures::SparseMatrix<int, double>;
