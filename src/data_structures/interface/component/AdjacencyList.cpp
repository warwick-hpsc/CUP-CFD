/*
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Contains the definitions for the AdjacencyList class
 */

// Interface Class
#include "AdjacencyList.h"

// Implementing Classes
#include "AdjacencyListVector.h"
#include "AdjacencyListCSR.h"

#include <iostream>

namespace cupcfd
{
	namespace data_structures
	{
		// === Constructors/Deconstructors

		template <class C, class I, class T>
		AdjacencyList<C,I,T>::AdjacencyList() {
			this->nNodes = 0;
			this->nEdges = 0;

			// Vectors should be empty by default
		}

		template <class C, class I, class T>
		AdjacencyList<C,I,T>::~AdjacencyList() {
			// Vectors should cleanup after themselves
		}

		// === Concrete Methods ===

		template <class C, class I, class T>
		cupcfd::error::eCodes AdjacencyList<C,I,T>::getNodeLocalIndex(T node, I * idx) {
			cupcfd::error::eCodes status;
			bool exists;

			// Error Check - Does the node exist
			status = this->existsNode(node, &exists);
			CHECK_ERROR_CODE(status)

			if(!exists) {
				return cupcfd::error::E_ADJACENCY_LIST_NODE_MISSING;
			}
			*idx = this->nodeToIDX[node];
			return cupcfd::error::E_SUCCESS;
		}

		template <class C, class I, class T>
		cupcfd::error::eCodes AdjacencyList<C,I,T>::getLocalIndexNode(I idx, T * node) {
			// Error Checking - Check Index is valid by ensuring it is within the node count
			if(idx < 0 || idx >= nNodes) {
				return cupcfd::error::E_ADJACENCY_LIST_INVALID_INDEX;
			}

			*node = this->IDXToNode[idx];
			return cupcfd::error::E_SUCCESS;
		}

		template <class C, class I, class T>
		cupcfd::error::eCodes AdjacencyList<C,I,T>::getNodeCount(I * nNodes) {
			*nNodes = this->nNodes;

			return cupcfd::error::E_SUCCESS;
		}

		template <class C, class I, class T>
		cupcfd::error::eCodes AdjacencyList<C,I,T>::getNodes(T * nodes, I nNodes) {
			// Error check on size of nodes array

			I nodeCount;
			cupcfd::error::eCodes status;
			status = this->getNodeCount(&nodeCount);
			CHECK_ERROR_CODE(status)

			if(nNodes < nodeCount) {
				// The destination array does not have elements to hold all of the node data
				return cupcfd::error::E_ARRAY_SIZE_UNDERSIZED;
			}

			// Begin the copy - since we are retrieving from a map 
			// we don't use the utility copy operation here.
			for(I i = 0; i < this->nNodes; i++) {
				nodes[i] = this->IDXToNode[i];
			}

			return cupcfd::error::E_SUCCESS;
		}

		template <class C, class I, class T>
		cupcfd::error::eCodes AdjacencyList<C,I,T>::getEdgeCount(I * nEdges) {
			*nEdges = this->nEdges;

			return cupcfd::error::E_SUCCESS;
		}

		template <class C, class I, class T>
		cupcfd::error::eCodes AdjacencyList<C,I,T>::getEdges(T * nodes1, I nNodes1,
																 T * nodes2, I nNodes2) {
			cupcfd::error::eCodes status;

			I nNodes;
			status = this->getNodeCount(&nNodes);
			CHECK_ERROR_CODE(status)

			I nEdges;
			status = this->getEdgeCount(&nEdges);
			CHECK_ERROR_CODE(status)

			// Error Check - Are the results arrays sufficiently large?
			if(nNodes1 < nEdges) {
				return cupcfd::error::E_ARRAY_SIZE_UNDERSIZED;
			}

			if(nNodes2 < nEdges) {
				return cupcfd::error::E_ARRAY_SIZE_UNDERSIZED;
			}

			// Get a copy of the nodes
			T * nodes = (T *) malloc(sizeof(T) * nNodes);
			status = this->getNodes(nodes, nNodes);
			CHECK_ERROR_CODE(status)

			I ptr = 0;
			// For each node, get a copy of the adjacent nodes
			for(I i = 0; i < nNodes; i++) {
				I nAdjNodes;
				status = this->getAdjacentNodeCount(nodes[i], &nAdjNodes);
				CHECK_ERROR_CODE(status)

				T * adjNodes = (T *) malloc(sizeof(T) * nAdjNodes);
				status = this->getAdjacentNodes(nodes[i], adjNodes, nAdjNodes);
				CHECK_ERROR_CODE(status)

				// Copy to results array
				for(I j = 0; j < nAdjNodes; j++) {
					nodes1[ptr] = nodes[i];
					nodes2[ptr] = adjNodes[j];
					ptr++;
				}

				free(adjNodes);
			}

			free(nodes);

			return cupcfd::error::E_SUCCESS;
		}

		// === CRTP Methods ===

		template <class C, class I, class T>
		cupcfd::error::eCodes AdjacencyList<C,I,T>::reset() {
			return static_cast<C*>(this)->reset();
		}

		template <class C, class I, class T>
		cupcfd::error::eCodes AdjacencyList<C,I,T>::addNode(T node) {
			return static_cast<C*>(this)->addNode(node);
		}

		template <class C, class I, class T>
		cupcfd::error::eCodes AdjacencyList<C,I,T>::existsNode(T node, bool * exists) {
			return static_cast<C*>(this)->existsNode(node, exists);
		}

		template <class C, class I, class T>
		cupcfd::error::eCodes AdjacencyList<C,I,T>::addEdge(T node, T adjNode) {
			return static_cast<C*>(this)->addEdge(node, adjNode);
		}

		template <class C, class I, class T>
		cupcfd::error::eCodes AdjacencyList<C,I,T>::existsEdge(T srcNode, T dstNode, bool * exists) {
			return static_cast<C*>(this)->existsEdge(srcNode, dstNode, exists);
		}

		template <class C, class I, class T>
		cupcfd::error::eCodes AdjacencyList<C,I,T>::getAdjacentNodeCount(T node, I * count) {
			return static_cast<C*>(this)->getAdjacentNodeCount(node, count);
		}

		template <class C, class I, class T>
		cupcfd::error::eCodes AdjacencyList<C,I,T>::getAdjacentNodes(T node, T * adjNodes, I nAdjNodes) {
			return static_cast<C*>(this)->getAdjacentNodes(node, adjNodes, nAdjNodes);
		}
	}
}

// Explicit Instantiation
using namespace cupcfd::data_structures;

template class cupcfd::data_structures::AdjacencyList<AdjacencyListCSR<int, int>, int, int>;
template class cupcfd::data_structures::AdjacencyList<AdjacencyListCSR<int, float>,int, float>;
template class cupcfd::data_structures::AdjacencyList<AdjacencyListCSR<int, double>,int, double>;

template class cupcfd::data_structures::AdjacencyList<AdjacencyListVector<int, int>, int, int>;
template class cupcfd::data_structures::AdjacencyList<AdjacencyListVector<int, float>,int, float>;
template class cupcfd::data_structures::AdjacencyList<AdjacencyListVector<int, double>,int, double>;
