/*
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Contains definitions for the AdjacencyListVector class.
 */

#include <iostream>
#include <algorithm>

#include "AdjacencyListVector.h"
#include "SearchDrivers.h"

namespace cupcfd
{
	namespace data_structures
	{
		// === Constructors/Deconstructors ===

		template <class I, class T>
		AdjacencyListVector<I, T>::AdjacencyListVector()
		: AdjacencyList<AdjacencyListVector<I,T>, I,T>()
		{
			this->nNodes = 0;
			this->nEdges = 0;

			// Reset Maps to Empty Objects
			this->nodeToIDX = std::map<T,I>();
			this->IDXToNode = std::map<I,T>();

			// Reset the vector object
			this->adjacencies.clear();
			this->adjacencies.resize(0);
		}

		template <class I, class T>
		AdjacencyListVector<I, T>::~AdjacencyListVector() {
		   // Reset node/edge counts
			this->nNodes = 0;
			this->nEdges = 0;

			// Reset Maps to Empty Objects
			this->nodeToIDX = std::map<T,I>();
			this->IDXToNode = std::map<I,T>();

			// Reset the vector object
			this->adjacencies.clear();
			this->adjacencies.resize(0);
		}

		// === CRTP Methods ===

		template <class I, class T>
		void AdjacencyListVector<I,T>::reset() {
		   // Reset node/edge counts
			this->nNodes = 0;
			this->nEdges = 0;

			// Reset Maps to Empty Objects
			this->nodeToIDX = std::map<T,I>();
			this->IDXToNode = std::map<I,T>();

			// Reset the vector object
			this->adjacencies.clear();
			this->adjacencies.resize(0);
		}

		template <class I, class T>
		cupcfd::error::eCodes AdjacencyListVector<I,T>::addNode(T node) {
			bool nodeExists = this->existsNode(node);
			if(!nodeExists) {
				// Generate a new local index
				I nodeLocalIDX = this->nNodes;

				this->nNodes += 1;

				// Update Mappings
				this->nodeToIDX[node] = nodeLocalIDX;
				this->IDXToNode[nodeLocalIDX] = node;

				// Add a new empty vector to the adjacencies list
				this->adjacencies.push_back(std::vector<I>());

				return cupcfd::error::E_SUCCESS;
			}
			else {
				return cupcfd::error::E_ADJACENCY_LIST_NODE_EXISTS;
			}
		}

		template <class I, class T>
		bool AdjacencyListVector<I, T>::existsNode(T node) {
			if(this->nodeToIDX.find(node) != this->nodeToIDX.end()) {
				return true;
			}
			else {
				return false;
			}
		}

		template <class I, class T>
		cupcfd::error::eCodes AdjacencyListVector<I, T>::addEdge(T node, T adjNode) {
			cupcfd::error::eCodes status;
			I nodeLocalIDX, adjNodeLocalIDX;

			// (1) Check the source node exists
			bool nodeExists = this->existsNode(node);
			if(!nodeExists) return cupcfd::error::E_ADJACENCY_LIST_NODE_MISSING;

			// (2) Check the destination node exists
			nodeExists = this->existsNode(adjNode);
			if(!nodeExists) return cupcfd::error::E_ADJACENCY_LIST_NODE_MISSING;

			// (3) Check that edge doesn't already exist
			bool edgeExists;
			status = this->existsEdge(node, adjNode, &edgeExists);
			CHECK_ECODE(status)
			// Add if edge doesn't already exist
			if(edgeExists) return cupcfd::error::E_ADJACENCY_LIST_EDGE_EXISTS;

			// Retrieve internal indexes for the nodes
			nodeLocalIDX = this->nodeToIDX[node];
			adjNodeLocalIDX = this->nodeToIDX[adjNode];

			// Add the edge to the graph
			this->adjacencies[nodeLocalIDX].push_back(adjNodeLocalIDX);
			this->nEdges = this->nEdges + 1;

			return cupcfd::error::E_SUCCESS;
		}

		template <class I, class T>
		cupcfd::error::eCodes AdjacencyListVector<I, T>::existsEdge(T srcNode, T dstNode, bool * exists) {
			cupcfd::error::eCodes status;

			bool nodeExists = this->existsNode(srcNode);
			if(!nodeExists) {
				*exists = false;
				return cupcfd::error::E_ADJACENCY_LIST_NODE_MISSING;
			}

			nodeExists = this->existsNode(dstNode);
			if(!nodeExists) {
				*exists = false;
				return cupcfd::error::E_ADJACENCY_LIST_NODE_MISSING;
			}

			// Retrieve internal indexes for the nodes
			I nodeLocalIDX = this->nodeToIDX[srcNode];
			I adjNodeLocalIDX = this->nodeToIDX[dstNode];

			// Search edge list for destination node and return existance (or lack thereof)
			I edgeIdx;
			I numAdj = cupcfd::utility::drivers::safeConvertSizeT<I>(this->adjacencies[nodeLocalIDX].size());
			status = cupcfd::utility::drivers::linearSearch(&(this->adjacencies[nodeLocalIDX][0]), numAdj, adjNodeLocalIDX, &edgeIdx);
			if (status == cupcfd::error::E_SUCCESS) {
				*exists = true;
			}
			else if (status == cupcfd::error::E_SEARCH_NOT_FOUND) {
				*exists = false;
			}
			else {
				CHECK_ECODE(status)
			}

			return cupcfd::error::E_SUCCESS;
		}

		template <class I, class T>
		cupcfd::error::eCodes AdjacencyListVector<I, T>::getAdjacentNodeCount(T node, I * count) {
			bool exists = this->existsNode(node);

			if(!exists) return cupcfd::error::E_ADJACENCY_LIST_NODE_MISSING;

			I idx = this->nodeToIDX[node];
			*count = this->adjacencies[idx].size();
			return cupcfd::error::E_SUCCESS;
		}

		template <class I, class T>
		cupcfd::error::eCodes AdjacencyListVector<I, T>::getAdjacentNodes(T node, T * adjNodes, I nAdjNodes) {
			cupcfd::error::eCodes status;
			I idx, count;

			// Get the number of adjacent nodes
			// Error Check: If the node does not exist it will be raised as an error here
			status = this->getAdjacentNodeCount(node, &count);
			CHECK_ECODE(status)

			// Error Check: Check the array is large enough
			if(count > nAdjNodes) return cupcfd::error::E_ARRAY_SIZE_UNDERSIZED;

			idx = this->nodeToIDX[node];
			std::vector<I> adjNodesVec = this->adjacencies[idx];

			for(std::size_t i = 0; i< adjNodesVec.size(); i++) {
				adjNodes[i] = this->IDXToNode[adjNodesVec[i]];
			}

			return cupcfd::error::E_SUCCESS;
		}
	}	// namespace data_structures
} // namespace cupcfd

// Explicit Instantiation
template class cupcfd::data_structures::AdjacencyListVector<int, int>;
template class cupcfd::data_structures::AdjacencyListVector<int, float>;
template class cupcfd::data_structures::AdjacencyListVector<int, double>;
