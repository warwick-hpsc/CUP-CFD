/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Contains the header level definitions for the AdjacencyList class.
 */

#ifndef CUPCFD_DATA_STRUCTURES_ADJACENCY_LIST_IPP_H
#define CUPCFD_DATA_STRUCTURES_ADJACENCY_LIST_IPP_H

#include "AdjacencyList.h"

#include <cstdlib>

namespace cupcfd
{
	namespace data_structures
	{
		template <class C, class I, class T>
		template <class S>
		void AdjacencyList<C,I,T>::operator=(AdjacencyList<S,I,T>& source)
		{
			// Specialised versions could be more optimised if they are
			// aware of internal data structures.
			
			// However, this generic version will serve for the interface version.
						
			// Call the reset function 
			this->reset();
			
			// Retrieve a list of nodes to add
			I nNodes;
			
			source.getNodeCount(&nNodes);
			
			T * nodes = (T *) malloc(sizeof(T) * nNodes);
			source.getNodes(nodes, nNodes);
			
			// Retrieve a list of edges to add
			I nEdges;
			
			source.getEdgeCount(&nEdges);
			
			T * edgeNode1 = (T *) malloc(sizeof(T) * nEdges);
			T * edgeNode2 = (T *) malloc(sizeof(T) * nEdges);
			source.getEdges(edgeNode1, nEdges, edgeNode2, nEdges);
			
			// Add the nodes
			for(I i = 0; i < nNodes; i++)
			{
				this->addNode(nodes[i]);
			}
			
			// Add the edges
			for(I i = 0; i < nEdges; i++)
			{
				this->addEdge(edgeNode1[i], edgeNode2[i]);
			}
			
			// Cleanup
			free(nodes);
			free(edgeNode1);
			free(edgeNode2);
		}
	}
}

#endif
