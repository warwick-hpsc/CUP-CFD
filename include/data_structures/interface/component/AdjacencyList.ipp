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
		void AdjacencyList<C,I,T>::operator=(AdjacencyList<S,I,T>& source) {
			// Specialised versions could be more optimised if they are
			// aware of internal data structures.
			
			// However, this generic version will serve for the interface version.

			cupcfd::error::eCodes status;

			status = this->reset();
			CHECK_ERROR_CODE(status)
			
			// Retrieve a list of nodes to add
			I nNodes;
			status = source.getNodeCount(&nNodes);
			CHECK_ERROR_CODE(status)
			
			T * nodes = (T *) malloc(sizeof(T) * nNodes);
			status = source.getNodes(nodes, nNodes);
			CHECK_ERROR_CODE(status)
			
			// Retrieve a list of edges to add
			I nEdges;
			status = source.getEdgeCount(&nEdges);
			CHECK_ERROR_CODE(status)
			
			T * edgeNode1 = (T *) malloc(sizeof(T) * nEdges);
			T * edgeNode2 = (T *) malloc(sizeof(T) * nEdges);
			status = source.getEdges(edgeNode1, nEdges, edgeNode2, nEdges);
			CHECK_ERROR_CODE(status)
			
			for(I i = 0; i < nNodes; i++) {
				status = this->addNode(nodes[i]);
				CHECK_ERROR_CODE(status)
			}
			for(I i = 0; i < nEdges; i++) {
				status = this->addEdge(edgeNode1[i], edgeNode2[i]);
				CHECK_ERROR_CODE(status)
			}
			
			// Cleanup
			free(nodes);
			free(edgeNode1);
			free(edgeNode2);
		}
	}
}

#endif
