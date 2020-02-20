/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Contains declarations for the DistributedAdjacencyList Class
 */

#ifndef CUPCFD_DATA_STRUCTURES_DISTRIBUTED_ADJACENCY_LIST_INCLUDE_H
#define CUPCFD_DATA_STRUCTURES_DISTRIBUTED_ADJACENCY_LIST_INCLUDE_H

#include <mpi.h>
#include <map>
#include <vector>
#include <unordered_map>
#include <algorithm>

#include "Communicator.h"
#include "AdjacencyList.h"
#include "AdjacencyListCSR.h"
#include "AdjacencyListVector.h"
#include "Error.h"
#include "ExchangePattern.h"
#include "ExchangePatternTwoSidedNonBlocking.h"

namespace cupcfd
{
	namespace data_structures
	{
		enum nodeType {LOCAL, GHOST};

		/**
		 * Stores node and edge data of a graph in a distributed fashion across multiple ranks.
		 *
		 * This class stores the parallel relationships of nodes and edges.
		 * When a node is added, it can be assigned as a local or ghost node. Each unique node
		 * can be added locally to one process, and as a ghost node to multiple processes.
		 * Upon finalizing the graph, the rank relationships are stored internally, including who
		 * are the owners of the various ghost nodes and a suitable connectivity graph is built.
		 *
		 * In addition, every node is assigned both a global ID and a local ID, such that it can be
		 * used to track the location of data for data exchanges by replicating the same index scheme
		 * in local arrays.
		 *
		 * In a finalized graph, the nodes are ordered such that local nodes have a lower localID than
		 * than any ghost nodes - i.e. the localID range is divided into [Locally Owned Nodes] | [Ghost Owned Nodes].
		 * This is to help faciliate looping over local or ghost nodes by range.
		 *
		 * In non-finalized graphs, this localID ordering is unknown/likely dependent on order of node addition,
		 * but in non-finalized graphs any retrieved node data should be treated as unreliable/incorrect/subject to
		 * change - functions that should depend on the graph being finalizsed should have a finalize check that
		 * errors if the graph is not finalized.
		*/
		template <class I, class T>
		class DistributedAdjacencyList
		{
			public:
				// === Members ===

				/** Number of Locally Owned Nodes **/
				I nLONodes;

				/** Number of Local Ghost Nodes **/
				I nLGhNodes;

				/** Number of Local Edges **/
				I nLEdges;

				/** Number of Global (Distributed) Non-Ghost Nodes **/
				I nGNodes;

				/** Number of Global (Distributed) Ghost Nodes **/
				I nGGhNodes;

				/** Number of Global Edges **/
				I nGEdges;

				/** Track whether the Distributed List is finalized (can no longer add nodes/edges) **/
				bool finalized;

				/** Processes the adjacency list is spread across **/
				cupcfd::comm::Communicator * comm;

				/** AdjacencyList used for initial construction (faster to build). Discarded at finalisation. **/
				cupcfd::data_structures::AdjacencyListVector<I, T> buildGraph;

				/** Final Local Connectivity Graph **/
				cupcfd::data_structures::AdjacencyListCSR<I, T> connGraph;

				// ToDo: Make these unordered_maps for performance improvement
				// May require changing the unit tests since the order from get functions would no longer be guaranteed.

				/** Map node to a local/ghost type **/
				std::map<T, nodeType> nodeDistType;

				/** Map node to the process that owns it **/
				std::map<T, I> nodeOwner;

				/** Track the neighbouring process ranks **/
				std::vector<I> neighbourRanks;

				/** Store Global IDs of nodes we will need to send - ranks **/
				std::vector<I>sendRank;

				/** Store Global IDs of nodes we will need to send - CSR xadj **/
				std::vector<I>sendGlobalIDsXAdj;

				/** Store Global IDs of nodes we will need to send - CSR adjncy **/
				std::vector<I>sendGlobalIDsAdjncy;

				/** Store Global IDs of nodes we will need to receive from - ranks **/
				std::vector<I>recvRank;

				/** Store Global IDs of nodes we will need to receive from - CSR xadj **/
				std::vector<I>recvGlobalIDsXAdj;

				/** Store Global IDs of nodes we will need to receive from - CSR adjncy **/
				std::vector<I>recvGlobalIDsAdjncy;

				// ToDo: Since connGraph already maps a node to a local index, we could use another CSR to associate the same index with a global id
				// rather than use a map.

				/** Minimum global index of a locally owned node (global index of a ghost cell might be lower) **/
				I globalOwnedRangeMin;

				/** Maximum global index of a locally owned node (global index of a ghost cell might be higher) **/
				I globalOwnedRangeMax;

				/*
				 * LocalIDX is managed by the buildGraph/connGraph, but we may need a uniform global ID for each node separate
				 * from the node itself (to accomodate different types, to enforce sequential numbering on a process to make later processes easier etc)
				 * These maps store the relationships between node and global id so we can dereference from one to another.
				 * This also allows us to build a distributed graph around nodes, but manage communications with global ids after the finalization stage.
				 */

				/** Map a node (not the localID) to a global id. **/
				std::map<T, I> nodeToGlobal;

				/** Map a global id to a node (not the localID). **/
				std::map<I, T> globalToNode;

				/** Store the number of graph nodes on a process - only valids after finalizing. **/
				I * processNodeCounts;

				// === Constructors/Deconstructors ===

				/**
				 * Constructor.
				 * Setup the initial empty graph, ready to add nodes and edges
				 *
				 * @param comm The communicator containing the ranks that can hold
				 * distributed data
				 *
				 * @tparam I The type of the indexing scheme
				 * @tparam T The type of the stored node data
				 */
				DistributedAdjacencyList(cupcfd::comm::Communicator& comm);

				/**
				 * Deconstructor.
				 *
				 * @tparam I The type of the indexing scheme
				 * @tparam T The type of the stored node data
				 */
				virtual ~DistributedAdjacencyList();

				// === Concrete Methods ===

				/**
				 * Resets the list to a default state containing no data.
				 * Note: this does not reset the communicator used.
				 *
				 * @tparam I The type of the indexing scheme
				 * @tparam T The type of the stored node data
				 *
				 * @return An error status indicating the success or failure of the operation
				 * @retval cupcfd::error::E_SUCCESS The method completed successfully
				 */
				cupcfd::error::eCodes reset();

				/**
				 * Deep copy from the source graph to the destination graph, overwriting it's contents
				 *
				 * @param source The source graph to copy from
				 *
				 * @tparam I The type of the indexing scheme
				 * @tparam T The type of the stored node data
				 *
				 * @return An error status indicating the success or failure of the operation
				 * @retval cupcfd::error::E_SUCCESS The method completed successfully
				 */
				virtual cupcfd::error::eCodes operator=(DistributedAdjacencyList<I, T>& source);

				/**
				 * Tests whether the node is known to exist within the portion of the list stored on
				 * this process, and that it exists as a locally owned node.
				 *
				 * @param node The node to search for
				 * @param found A pointer to the location updated with the result,
				 * true if the node is found and it is a locally owned node, otherwise false
				 *
				 * @tparam I The type of the indexing scheme
				 * @tparam T The type of the stored node data
				 *
				 * @return An error status indicating the success or failure of the operation
				 * @retval cupcfd::error::E_SUCCESS The method completed successfully
				 */
				cupcfd::error::eCodes existsLocalNode( T node, bool * found);

				/**
				 * Tests whether the node is known to exist within the portion of the list stored on
				 * this process, and that it exists as a ghost (not locally owned) node.
				 *
				 * @param node The node to search for
				 * @param found A pointer to the location updated with the result,
				 * true if the node is found and it is a ghost node, otherwise false
				 *
				 * @tparam I The type of the indexing scheme
				 * @tparam T The type of the stored node data
				 *
				 * @return An error status indicating the success or failure of the operation
				 * @retval cupcfd::error::E_SUCCESS The method completed successfully
				 */
				cupcfd::error::eCodes existsGhostNode(T node, bool * found);

				/**
				 * Tests where the node exists within the portion of the list stored on this process, irrespective of what type it is.
				 *
				 * @param node The node to search for
				 * @param found A pointer to the location updated with the result,
				 * true if the node is found, otherwise false
				 *
				 * @tparam I The type of the indexing scheme
				 * @tparam T The type of the stored node data
				 *
				 * @return An error status indicating the success or failure of the operation
				 * @retval cupcfd::error::E_SUCCESS The method completed successfully
				 */
				cupcfd::error::eCodes existsNode(T node, bool * found);

				/**
				 * Add a node to the distributed list and register it as a locally owned node -
				 * i.e. it is owned by the process that is calling the add function
				 *
				 * @param node The node to add to the list
				 *
				 * @tparam I The type of the indexing scheme
				 * @tparam T The type of the stored node data
				 *
				 * @return An error status indicating the success or failure of the operation
				 * @retval cupcfd::error::E_SUCCESS The method completed successfully
				 * @retval cupcfd::error::E_ADJACENCY_LIST_NODE_EXISTS Node already exists in the list (as ghost or local), nothing was changed.
				 */
				cupcfd::error::eCodes addLocalNode(T node);

				/**
				 * Add a node to the distributed list and register it as a ghost node -
				 * i.e. it is not owned by the process that is calling the add function, but this process is made aware of it existing
				 * within the graph somewhere.
				 *
				 * @param node The node to add to the list
				 *
				 * @tparam I The type of the indexing scheme
				 * @tparam T The type of the stored node data
				 *
				 * @return An error status indicating the success or failure of the operation
				 * @retval cupcfd::error::E_SUCCESS The method completed successfully
				 * @retval cupcfd::error::E_ADJACENCY_LIST_NODE_EXISTS Node already exists in the list (as ghost or local), nothing was changed.
				 */
				cupcfd::error::eCodes addGhostNode(T node);

				/**
				 * Add a node to the distributed list - since the type is not specified, it defaults to a ghost node.
				 *
				 * @param node The node to add to the list
				 *
				 * @tparam I The type of the indexing scheme
				 * @tparam T The type of the stored node data
				 *
				 * @return An error status indicating the success or failure of the operation
				 * @retval cupcfd::error::E_SUCCESS The method completed successfully
				 * @retval cupcfd::error::E_ADJACENCY_LIST_NODE_EXISTS Node already exists in the list (as ghost or local), nothing was changed.
				 */
				cupcfd::error::eCodes addNode(T node);

				/**
				 * Checks whether an edge exists from src to dst (direction matters)
				 *
				 * @param src The source node of the edge
				 * @param dst The destination node of the edge
				 * @param found A pointer to the location that will be updated with true if the edge exists in the graph, false if it does not
				 *
				 * @tparam I The type of the indexing scheme
				 * @tparam T The type of the stored node data
				 *
				 * @return An error status indicating the success or failure of the operation
				 * @retval cupcfd::error::E_SUCCESS The method completed successfully
				 * @retval cupcfd::error::E_ADJACENCY_LIST_NODE_MISSING At least one of the src or dst nodes was not found in the graph
				 */
				cupcfd::error::eCodes existsEdge(T src, T dst, bool * found);

				/**
				 * Adds a directed edge from the 'src' node to the 'dst' node.
				 * If a node is missing, they are added as ghost nodes.
				 * ToDo: It may be preferable to have an error code if a node is missing
				 *
				 * @param src The source node the edge goes from
				 * @param dst The destination node the edge goes to
				 *
				 * @tparam I The type of the indexing scheme
				 * @tparam T The type of the stored node data
				 *
				 * @return An error status indicating the success or failure of the operation
				 * @retval cupcfd::error::E_SUCCESS The method completed successfully
				 */
				cupcfd::error::eCodes addEdge(T src, T dst);


				/**
				 * Adds an undirected edge between src and dst - i.e. it adds a directed edge in both directions.
				 * If a node is missing, they are added as ghost nodes.
				 * ToDo: It may be preferable to have an error code if a node is missing
				 *
				 * @param src The source node the edge goes from
				 * @param dst The destination node the edge goes to
				 *
				 * @tparam I The type of the indexing scheme
				 * @tparam T The type of the stored node data
				 *
				 * @return An error status indicating the success or failure of the operation
				 * @retval cupcfd::error::E_SUCCESS The method completed successfully
				 */
				cupcfd::error::eCodes addUndirectedEdge(T src, T dst);

				/**
				 * Finalizes the distributed graph. After this it will no longer accept modification
				 * (e.g. adding nodes), but it will ensure that any connectivity data required from other processes is obtained.
				 * This information includes knowing which process own the nodes registered as ghost nodes on this process,
				 * and will communicate that this process owns certain nodes to the relevant processes.
				 *
				 * @tparam I The type of the indexing scheme
				 * @tparam T The type of the stored node data
				 *
				 * @return An error status indicating the success or failure of the operation
				 * @retval cupcfd::error::E_SUCCESS The method completed successfully
				 */
				cupcfd::error::eCodes finalize();

				/**
				 * Sorts the node data to have local nodes appear first by local index
				 *
				 * @tparam I The type of the indexing scheme
				 * @tparam T The type of the stored node data
				 *
				 * @return An error status indicating the success or failure of the operation
				 * @retval cupcfd::error::E_SUCCESS The method completed successfully
				 */
				cupcfd::error::eCodes sortNodesByLocal();

				//template <class T>
				//cupcfd::adjacency_list::eCodes getNodeOwner(DistributedAdjacencyList<I, T>& list, T node, int * process);

				/**
				 * Retrieve the node data of the ghost nodes on this rank
				 *
				 * @param nodes The array to store the ghost node data
				 * @param nNodes The size of nodes in the number of elements of type T
				 *
				 * @tparam I The type of the indexing scheme
				 * @tparam T The type of the stored node data
				 *
				 * @return An error status indicating the success or failure of the operation
				 * @retval cupcfd::error::E_SUCCESS The method completed successfully
				 */
				cupcfd::error::eCodes getGhostNodes(T * nodes, I nNodes);

				/**
				 * Retrieve the node data of the locally owned nodes on this rank
				 *
				 * @param nodes The array to store the ghost node data
				 * @param nNodes The size of nodes in the number of elements of type T
				 *
				 * @tparam I The type of the indexing scheme
				 * @tparam T The type of the stored node data
				 *
				 * @return An error status indicating the success or failure of the operation
				 * @retval cupcfd::error::E_SUCCESS The method completed successfully
				 */
				cupcfd::error::eCodes getLocalNodes(T * nodes, I nNodes);

				/**
				 * Populate the serial adjacency list with data from this distributed graph..
				 * The data from the distributed list is gathered onto the specified rank and added to the serial adjacency list.
				 *
				 * All ranks that are members of this graph's communicator must participate, but if they are not the rank which data
				 * is being gathered onto (i.e. where the adjacency list is being populated), they can pass nullptr as destGraph, since they
				 * may not have a valid destGraph object.
				 *
				 * @param destGraph The serial adjacency list to populate with data. May be nullptr if this rank is not the rank specified as
				 * holding the serial adjacency list.
				 * @param rank The rank which holds the serial adjacency list to be populated. Must be a member of this graph's communicator
				 *
				 * @tparam C The implementation type of the serial adjacency list
				 * @tparam I The type of the indexing scheme
				 * @tparam T The type of the stored node data
				 *
				 * @return An error status indicating the success or failure of the operation
				 * @retval cupcfd::error::E_SUCCESS The method completed successfully
				 */
				template <class C>
				cupcfd::error::eCodes buildSerialAdjacencyList(AdjacencyList<C,I,T> * destGraph, I rank);

				/**
				 * Builds an ExchangePattern object, and sets up the exchange pattern
				 * based on the exchange of ghost cell data between the distributed regions of the graph.
				 * Note: This requires the graph to be finalized, since it requires full knowledge of the allocation
				 * of local and ghost nodes.
				 *
				 * For the exchange pattern generated from this function, the order of the data in the exchange
				 * pattern buffer (for when providing data arrays) is expected to match up with the order of the nodes
				 * by localID in the graph
				 *
				 * This should correspond to the ascending order of the local indexes of the nodes in the graph - i.e.
				 * E.g. For a setup of:
				 *
				 * Nodes   : 4, 6, 7, 2, 3
				 * LocalID : 0, 1, 2, 3, 4
				 *
				 * The data buffer of [a, b, c, d, e] means that a is the data for node 4 (localID 0),
				 * b is the data for node 6 (localID 1) etc.
				 *
				 * GlobalIDs are handled behind the scenes as part of the exchange pattern setup and internal data.
				 *
				 * @param pattern A pointer to where the pointer for the newly created ExchangePattern will be stored.
				 * @param list The distributed graph to source the connectivity data from for ghost data.
				 *
				 * @tparam I The type of the indexing scheme
				 * @tparam T The type of the stored node data
				 * @tparam D The type of the data to be exchanged by the pattern
				 *
				 * @return An error status indicating the success or failure of the operation
				 * @retval cupcfd::error::E_SUCCESS The method completed successfully
				 */
				template <class D>
				cupcfd::error::eCodes buildExchangePattern(cupcfd::comm::ExchangePatternTwoSidedNonBlocking<D>** pattern);
		};
	} // namespace data_structures
} // namespace cupcfd

// Include Header Level Definitions
#include "DistributedAdjacencyList.ipp"


#endif
