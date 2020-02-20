/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Contains the declarations for the AdjacencyList class.
 */

#ifndef CUPCFD_DATA_STRUCTURES_ADJACENCY_LIST_INCLUDE_H
#define CUPCFD_DATA_STRUCTURES_ADJACENCY_LIST_INCLUDE_H

#include <map>
#include "Error.h"

namespace cupcfd
{
	namespace data_structures
	{
		/**
		 * Stores graph data in an adjacency list setup.
		 *
		 * The design pattern for this uses CRTP, since it is desirable to avoid virtual overheads for
		 * getters and setters to permit a single interface access.
		 *
		 * @tparam C The type of the implementation class for the AdjacencyList.
		 * This must inherit from this class passing itself as template parameter C.
		 * @tparam I The type of the indexing scheme
		 * @tparam T The type of the graph nodes
		 */
		template<class C, class I, class T>
		class AdjacencyList
		{
			public:
				// === Members ===

				/** Stores the number of nodes in the adjacency list **/
				I nNodes;

				/** Stores the number of edges in the adjacency list **/
				I nEdges;

				// Boost has a bi-directional map for one-to-one relationships
				// However, trying to avoid hard requirement for boost beyond
				// unit testing, so will use two maps as an alternative.

				/** Map Nodes to an internal Index **/
				std::map<T,I> nodeToIDX;

				/** Map Internal Index to Nodes **/
				std::map<I,T> IDXToNode;

				// Constructors/Deconstructors

				/**
				 * Constructor:
				 * Initialises internal mapping data structures to be empty
				 * and sizes to 0.
				 */
				AdjacencyList();

				/**
				 * Deconstructor.
				 */
				~AdjacencyList();

				// === Concrete Methods ===

				/**
				 * Copy the edge and node data from source to this adjacency list
				 *
				 * @param source The adjacency list to copy from
				 *
				 * @tparam C The implementing class of the *destination* (this) adjacency list.
				 * @tparam S The implementing class of the *source* adjacency list (may differ from this
				 * adjacency list)
				 * @tparam I The type of the indexing scheme
				 * @tparam T The type of the stored node data
				 *
				 * @return Nothing
				 */
				template <class S>
				void operator=(AdjacencyList<S,I,T>& source);

				/**
				 *
				 * Get the AdjacencyList node index assigned to a node
				 *
				 * @param node The node data to find the index of
				 * @param idx A pointer to the location to be updated with the retrieved index
				 *
				 * @tparam I The type of the indexing scheme
				 * @tparam T The type of the stored node data
				 *
				 * @return An error status indicating the success or failure of the operation
				 * @retval cupcfd::error::E_SUCCESS The method completed successfully
				 * @retval cupcfd::error::E_ADJACENCY_LIST_NODE_MISSING The node was not found, so no valid index can be set
				 *
				 */
				cupcfd::error::eCodes getNodeLocalIndex(T node, I * idx);

				/**
				 * Get the node data associated with an AdjacencyList node index (each node is assigned an index
				 * when added)
				 *
				 * @param idx The index of the node to look up
				 * @param node A pointer to the location where the retrieved node data will be stored
				 *
				 * @tparam I The type of the indexing scheme
				 * @tparam T The type of the stored node data
				 *
				 * @return An error status indicating the success or failure of the operation
				 * @retval cupcfd::error::E_SUCCESS The method completed successfully
				 * @retval cupcfd::error::E_ADJACENCY_LIST_INVALID_INDEX The provided lookup index is outside of the
				 * valid range (i.e. < 0 or >= the number of nodes).
				 */
				cupcfd::error::eCodes getLocalIndexNode(I idx, T * node);

				/**
				 * Get the number of nodes stored in this adjacency list.
				 *
				 * @param nNodes A pointer to the location to be updated with the number of nodes
				 *
				 * @tparam I The type of the indexing scheme
				 * @tparam T The type of the stored node data
				 *
				 * @return An error status indicating the success or failure of the operation
				 * @retval cupcfd::error::E_SUCCESS The method completed successfully
				 */
				cupcfd::error::eCodes getNodeCount(I * nNodes);

				/**
				 * Get a copy of all nodes stored in this adjacency list.
				 *
				 * @param nodes The start location in memory of an array to copy the node data to
				 * @param nNodes The number of elements in the nodes array
				 *
				 * @tparam I The type of the indexing scheme
				 * @tparam T The type of the stored node data
				 *
				 * @return An error status indicating the success or failure of the operation
				 * @retval cupcfd::error::E_SUCCESS The method completed successfully
				 * @retval cupcfd::error::E_ARRAY_SIZE_UNDERSIZED The destination location for the node data
				 * is too small.
				 */
				cupcfd::error::eCodes getNodes(T * nodes, I nNodes);

				/**
				 * Get the number of directed edges stored in this adjacency list.
				 * Note: undirected edges count as two edges in this context, one for either direction
				 *
				 * @param nEdges A pointer to the location to be updated with the number of edges
				 *
				 * @tparam I The type of the indexing scheme
				 * @tparam T The type of the stored node data
				 *
				 * @return An error status indicating the success or failure of the operation
				 * @retval cupcfd::error::E_SUCCESS The method completed successfully
				 */
				cupcfd::error::eCodes getEdgeCount(I * nEdges);

				/**
				 * Get a copy of all edges stored in this adjacency list. This informations is
				 * returned as a pair of nodes, with nodes1[i] and nodes2[i] at index i corresponding to an
				 * edge from nodes1[i] to nodes2[i]
				 *
				 * @param nodes1 The start location in memory of an array to copy the node1 data to
				 * @param nNodes1 The number of elements in the nodes1 array - this should be at a minimum
				 * equivalent to the number of edges
				 * @param nodes2 The start location in memory of an array to copy the node2 data to
				 * @param nNodes1 The number of elements in the nodes2 array- this should be at a minimum
				 * equivalent to the number of edges
				 *
				 * @tparam I The type of the indexing scheme
				 * @tparam T The type of the stored node data
				 *
				 * @return An error status indicating the success or failure of the operation
				 * @retval cupcfd::error::E_SUCCESS The method completed successfully
				 * @retval cupcfd::error::E_ARRAY_SIZE_UNDERSIZED The destination location for at least one
				 * of the arrays is too small.
				 */
				cupcfd::error::eCodes getEdges(T * nodes1, I nNodes1,
													T * nodes2, I nNodes2);

				// ============================
				// ======= CRTP Methods =======
				// ============================
				// These methods will pass through their calls to the underlying implementation from the
				// inheriting class - must be known at compile time.

				/**
				 * Reset the adjacency list to an empty state
				 *
				 * @return An error status indicating the success or failure of the operation
				 * @retval cupcfd::error::E_SUCCESS The method completed successfully
				 */
				cupcfd::error::eCodes reset();

				/**
				 * Add a node to this AdjacencyList. By default it will have no edges.
				 *
				 * @param node The node data to add to this object.
				 *
				 * @tparam I The type of the indexing scheme
				 * @tparam T The type of the stored node data
				 *
				 * @return An error status indicating the success or failure of the operation
				 * @retval cupcfd::error::E_SUCCESS The method completed successfully
				 * @retval cupcfd::error::E_ADJACENCY_LIST_NODE_EXISTS The node cannot be added for it already exists. No changes are made.
				 */
				cupcfd::error::eCodes addNode(T node);

				/**
				 * Check whether a node already exists in the data structure
				 *
				 * @param node The node data to search for
				 * @param exists A pointer to the location where the result of this function will be written. Set to true if
				 * the node exists, or false if it does not
				 *
				 * @tparam I The type of the indexing scheme
				 * @tparam T The type of the stored node data
				 *
				 * @return An error status indicating the success or failure of the operation
				 * @retval cupcfd::error::E_SUCCESS The method completed successfully
				 */
				cupcfd::error::eCodes existsNode(T node, bool * exists);

				/**
				 * Add a directed edge to the adjacency list between two nodes from node to adjNode.
				 *
				 * @param node The source node of the edge
				 * @param adjNode The destination node of the edge
				 *
				 * @tparam I The type of the indexing scheme
				 * @tparam T The type of the stored node data
				 *
				 * @return An error status indicating the success or failure of the operation
				 * @retval cupcfd::error::E_SUCCESS The method completed successfully
				 * @retval cupcfd::error::E_ADJACENCY_LIST_EDGE_EXISTS The edge already exists - no changes were made
				 * @retval cupcfd::error::E_ADJACENCY_LIST_NODE_MISSING The source or destination node are missing from
				 * the adjacency list
				 */
				cupcfd::error::eCodes addEdge(T node, T adjNode);

				/**
				 * Checks whether an edge exists going from a source node to a destination node
				 *
				 * @param srcNode The source node
				 * @param desNode The destination node
				 * @param exists A pointer to the location where the result is stored. Updated to true if the edge exists,
				 * or false if it does not
				 *
				 * @tparam I The type of the indexing scheme
				 * @tparam T The type of the stored node data
				 *
				 * @return An error status indicating the success or failure of the operation
				 * @retval cupcfd::error::E_SUCCESS The method completed successfully
				 * @retval cupcfd::error::E_ADJACENCY_LIST_NODE_MISSING One of both of the nodes do not exist in this
				 * adjacency list
				 */
				cupcfd::error::eCodes existsEdge(T srcNode, T dstNode, bool * exists);

				/**
				 * Get the number of nodes directly reachable from a source node via a single edge transition
				 *
				 * @param node The source node
				 * @param count A pointer to the location updated with the number of adjacent reachable nodes
				 *
				 * @tparam I The type of the indexing scheme
				 * @tparam T The type of the stored node data
				 *
				 * @return An error status indicating the success or failure of the operation
				 * @retval cupcfd::error::E_SUCCESS The method completed successfully
				 * @retval cupcfd::error::E_ADJACENCY_LIST_NODE_MISSING The node provided does not exist
				 * in the adjacency list
				 */
				cupcfd::error::eCodes getAdjacentNodeCount(T node, I * count);

				/**
				 *
				 * Get the nodes directly reachable from a source node via a single edge transition
				 *
				 * @param node The source node
				 * @param adjNodes A pointer to a contiguous array where the adjacent node elements will be stored
				 * @param nAdjNodes The number of elements in the adjNodes array
				 *
				 * @tparam I The type of the indexing scheme
				 * @tparam T The type of the stored node data
				 *
				 * @return An error status indicating the success or failure of the operation
				 * @retval cupcfd::error::E_SUCCESS The method completed successfully
				 * @retval cupcfd::error::E_ADJACENCY_LIST_NODE_MISSING The node provided does not exist
				 * in the adjacency list
				 * @retval cupcfd::error::E_ARRAY_SIZE_UNDERSIZED The provided array is too small to hold
				 * the adjacent node data
				 */
				cupcfd::error::eCodes getAdjacentNodes(T node, T * adjNodes, I nAdjNodes);

		};
	} // namespace adjacency_list
} // namespace cupcfd

// Include Header Level Definitions
#include "AdjacencyList.ipp"

#endif
