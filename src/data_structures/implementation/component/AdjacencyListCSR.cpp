/*
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Contains the definitions for the AdjacencyListCSR class
 */

#include <cstdlib>
#include <iostream>
#include <vector>

#include "AdjacencyListCSR.h"
#include "SearchDrivers.h"

namespace cupcfd
{
	namespace data_structures
	{
		// === Constructors/Deconstructors ===

		template <class I, class T>
		AdjacencyListCSR<I, T>::AdjacencyListCSR()
		: AdjacencyList<AdjacencyListCSR<I,T>, I,T>()
		{
			this->xadj.clear();
			this->xadj.resize(1);	// Size needs to be 1 to prevent errors on access in empty graph

			this->adjncy.clear();
			this->adjncy.resize(0);
		}

		template <class I, class T>
		AdjacencyListCSR<I, T>::~AdjacencyListCSR() {
			this->xadj.clear();
			this->xadj.resize(1);	// Size needs to be 1 to prevent errors on access in empty graph

			this->adjncy.clear();
			this->adjncy.resize(0);

			// This is the +1 index that points at the index 1 past the last index of the edgelist
			this->xadj[0] = 0;

			// Reset Maps to Empty Objects
			this->nodeToIDX = std::map<T,I>();
			this->IDXToNode = std::map<I,T>();
		}

		// === Concrete Methods ===

		// === CRTP Methods ===

		template <class I, class T>
		void AdjacencyListCSR<I,T>::reset() {
			this->nNodes = 0;
			this->nEdges = 0;

			this->xadj.clear();
			this->xadj.resize(1);

			this->adjncy.clear();
			this->adjncy.resize(0);

			// This is the +1 index that points at the index 1 past the last index of the edgelist
			this->xadj[0] = 0;

			// Reset Maps to Empty Objects
			this->nodeToIDX = std::map<T,I>();
			this->IDXToNode = std::map<I,T>();
		}

		template <class I, class T>
		cupcfd::error::eCodes AdjacencyListCSR<I, T>::addNode(T node) {
			bool nodeExists = this->existsNode(node);

			if(!nodeExists) {
				// Generate a new local index
				I nodeLocalIDX = this->nNodes;

				// Update Mappings
				this->nodeToIDX[node] = nodeLocalIDX;
				this->IDXToNode[nodeLocalIDX] = node;

				// Add node, but as yet it does not have any adjacent elements, so the index it
				// points to is the same as the one below.

				// Prepend before last element - insert the same value as the end of the current list since
				// it has no adjacencies as of yet.

				I pos = (this->nNodes + 1) - 1;
				I val = this->xadj[this->xadj.size()-1];

				auto it = this->xadj.begin();
				this->xadj.insert(it + pos, val);

				// Update Node Count
				this->nNodes = this->nNodes + 1;

				return cupcfd::error::E_SUCCESS;
			}
			else {
				return cupcfd::error::E_ADJACENCY_LIST_NODE_EXISTS;
			}
			return cupcfd::error::E_SUCCESS;
		}

		template <class I, class T>
		bool AdjacencyListCSR<I, T>::existsNode(T node) {
			if(this->nodeToIDX.find(node) != this->nodeToIDX.end()) {
				return true;
			} else {
				return false;
			}
		}

		template <class I, class T>
		cupcfd::error::eCodes AdjacencyListCSR<I, T>::existsEdge(T srcNode, T dstNode, bool * exists) {
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

			// Get Node Count Adjacent to srcNode
			I count;
			status = this->getAdjacentNodeCount(srcNode, &count);
			CHECK_ECODE(status)

			// Get Nodes Adjacent to srcNode
			T * adjNodes = (T *) malloc(sizeof(T) * count);
			status = this->getAdjacentNodes(srcNode, adjNodes, count);
			CHECK_ECODE(status)

			// Search adjacent node list for the node
			// Alternate: could seach for localIDX of node in adjncy array
			// Would avoid need for comparison operators on custom nodes.
			I index;
			status = cupcfd::utility::drivers::linearSearch(adjNodes, count, dstNode, &index);
			if (status == cupcfd::error::E_SUCCESS) {
				*exists = true;
			}
			else if (status == cupcfd::error::E_SEARCH_NOT_FOUND) {
				*exists = false;
			}
			else {
				CHECK_ECODE(status)
			}

			free(adjNodes);

			return cupcfd::error::E_SUCCESS;
		}

		template <class I, class T>
		cupcfd::error::eCodes AdjacencyListCSR<I, T>::addEdge(T node, T adjNode) {
			cupcfd::error::eCodes status;
			I nodeLocalIDX, adjNodeLocalIDX;
			bool nodeExists, edgeExists;

			// (1) Check the source node exists
			nodeExists = this->existsNode(node);
			if(!nodeExists) {
				return cupcfd::error::E_ADJACENCY_LIST_NODE_MISSING;
			}

			// (2) Check the destination node exists
			nodeExists = this->existsNode(adjNode);
			if(!nodeExists) {
				return cupcfd::error::E_ADJACENCY_LIST_NODE_MISSING;
			}

			// (3) Check that edge doesn't already exist
			status = this->existsEdge(node, adjNode, &edgeExists);
			CHECK_ECODE(status)

			// Add if edge doesn't already exist
			if(edgeExists) {
				return cupcfd::error::E_ADJACENCY_LIST_EDGE_EXISTS;
			}

			// Retrieve internal indexes for the nodes
			nodeLocalIDX = this->nodeToIDX[node];
			adjNodeLocalIDX = this->nodeToIDX[adjNode];

			I postEnd = this->xadj[nodeLocalIDX+1];

			// Insert just after the end of the current adjacency list
			this->adjncy.insert(this->adjncy.begin() + postEnd , adjNodeLocalIDX);

			// All subsequent indexes in xadj now need to be increased by 1 to reflect
			// the shift to the right

			for(I i = nodeLocalIDX + 1; i < this->nNodes + 1; i++) {
				this->xadj[i] = this->xadj[i] + 1;
			}

			// Increase number of edges
			this->nEdges = this->nEdges + 1;

			return cupcfd::error::E_SUCCESS;
		}

		template <class I, class T>
		cupcfd::error::eCodes AdjacencyListCSR<I, T>::getAdjacentNodeCount(T node, I * count) {
			bool exists = this->existsNode(node);
			*count = 0;

			if(!exists) {
				return cupcfd::error::E_ADJACENCY_LIST_NODE_MISSING;
			}

			I idx = this->nodeToIDX[node];

			*count = this->xadj[idx+1] - this->xadj[idx];
			return cupcfd::error::E_SUCCESS;
		}

		template <class I, class T>
		cupcfd::error::eCodes AdjacencyListCSR<I, T>::getAdjacentNodes(T node, T * adjNodes, I nAdjNodes) {
			cupcfd::error::eCodes status;
			I idx, ptr, count;

			// Get the number of adjacent nodes
			// Error Check: If the node does not exist it will be raised as an error here
			status = this->getAdjacentNodeCount(node, &count);
			CHECK_ECODE(status)

			// Error Check: Check the array is large enough
			if(count > nAdjNodes) {
				return cupcfd::error::E_ARRAY_SIZE_UNDERSIZED;
			}

			idx = this->nodeToIDX[node];
			ptr = this->xadj[idx];

			//ToDo: Check array size?
			for(I i = 0; i< count; i++) {
				adjNodes[i] = this->IDXToNode[this->adjncy[ptr+i]];
			}

			return cupcfd::error::E_SUCCESS;
		}
	} // namespace adjacency_list
} // namespace cupcfd

// Explicit Instantiation

template class cupcfd::data_structures::AdjacencyListCSR<int, int>;
template class cupcfd::data_structures::AdjacencyListCSR<int, float>;
template class cupcfd::data_structures::AdjacencyListCSR<int, double>;
