/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Contains declarations for the abstract Partitioner class
 *
 */

#ifndef CUPCFD_PARTITIONER_INTERFACE_INCLUDE_H
#define CUPCFD_PARTITIONER_INTERFACE_INCLUDE_H

#include "DistributedAdjacencyList.h"
#include "AdjacencyList.h"
#include "Communicator.h"
#include "Error.h"

namespace cupcfd
{
	namespace partitioner
	{
		/**
		 * Abstract interface for the Partitioner classes.
		 * This provides the basic interface functions for accessing and partitioning data, the operation
		 * of which will be passed through to an underlying mechanism or library.
		 *
		 * This base class also includes the common data structures - primarily:
		 * (a) The number of partitions
		 * (b) A Node | Result Pair of Arrays. After partitioning has been run, for a given index i,
		 * the pair of (nodes[i],result[i]) should contain both the node and the partition it has been assigned to.
		 *
		 * Currently the primarily obligation of any class that implements this function is that after
		 * calling partition there is a valid partition result in this pairing. Implementing classes
		 * may wish to implement additional data stores to prepare or include additional data prior to partitioning.
		 * For example: some libraries may require connectivity data.
		 *
		 * @tparam I The type of the indexing scheme
		 * @tparam T The type of the stored array data
		 */
		template <class I, class T>
		class PartitionerInterface
		{
			public:
				// === Members ===
				// Variables common to all partitioners

				/** Array of nodes to be partitioned
				 * It is presumed that for a given rank, this may hold a full or partial set of nodes for the
				 * overall partition (e.g. due to serial vs distributed partitioners)
				 **/
				T * nodes;

				/** Size of nodes in  the number of elements of type T **/
				I nNodes;

				/** Contains the Assignment of a graph region/partition to a node of the same index position in
				 * the nodes array.
				 * It is presumed that for a given rank, this may hold a full or partial set of results for the
				   overall partition (e.g. due to serial vs distributed partitioners) **/
				I * result;

				/** Size of result in  the number of elements of type T **/
				I nResult;

				/** Number of regions to partition the overall graph into **/
				I nParts;


				/**
				 * Any process that expects to receive a partition assignment must create a partitioner object,
				 * and thus be a member of workComm.
				 *
				 * Depending on the choice of partitioner and/or configuration, not all processes in workComm
				 * may have to perform partitioning work.
				 *
				 * Likewise, if the number of partitions is set to lower than the workComm size, then not
				 * all ranks will be assigned data.
				 **/
				cupcfd::comm::Communicator workComm;

				// ===== Constructors/Deconstructors =====

				/**
				 * Create a default Partitioner object.
				 * Any internal data stores will be set to nullptr.
				 *
				 * @param workComm Communicator of processes creating the partitioner object that will
				 * perform the partitioning.
				 */
				PartitionerInterface(cupcfd::comm::Communicator& workComm);

				/**
				 * Create a partitioner object and setup the internal data structures
				 * to be ready for running the partitioner.
				 *
				 * This constructor sources the data it needs to partition from the provided graph.
				 * All process rank members of the graph's communicator must participate in the
				 * construction of this Partitioner, as well as the partitioning later (since they hold
				 * data).
				 *
				 * @param sourceGraph The graph to partition
				 * @param nParts The number of parts to partition the graph into
				 */
				PartitionerInterface(cupcfd::data_structures::DistributedAdjacencyList<I,T>& sourceGraph, int nParts);

				/**
				 * Deconstructor for a Partitioner object.
				 * This will free up any memory being used to store result or node data.
				 */
				virtual ~PartitionerInterface();

				// ========== Concrete Methods ===============

				/**
				 * Reset any data storage used for storing partitioning results
				 * This will leave the result data storage unallocated and set to nullptr.
				 *
				 * @tparam I The type of the indexing scheme
				 * @tparam T The type of the stored array data
				 *
				 * @return An error status indicating the success or failure of the operation
				 * @retval cupcfd::error::E_SUCCESS The method completed successfully
				 */
				virtual void resetResultStorage();

				/**
				 * Copy the node data in the provided array to the internal node storage
				 * of this partitioner.
				 *
				 * This will clear any existing node and result data stored.
				 *
				 * @param nodes The array of nodes to be partitioned
				 * @param nNodes The size of nodes in the number of elements of type T
				 *
				 * @tparam I The type of the indexing scheme
				 * @tparam T The type of the stored array data
				 *
				 * @return An error status indicating the success or failure of the operation
				 * @retval cupcfd::error::E_SUCCESS The method completed successfully
				 */
				virtual void setNodeStorage(T * nodes, I nNodes);

				/**
				 * Reset any data storage used for storing node data assigned to the current rank.
				 * This will leave the node data storage unallocated and set to nullptr.
				 *
				 * @tparam I The type of the indexing scheme
				 * @tparam T The type of the stored array data
				 *
				 * @return An error status indicating the success or failure of the operation
				 * @retval cupcfd::error::E_SUCCESS The method completed successfully
				 */
				virtual void resetNodeStorage();

				/**
				 * Set the number of partitions the graph space should be split into
				 *
				 * @param nParts The number of desired partitions for the partitioner results
				 *
				 * @tparam I The type of the indexing scheme
				 * @tparam T The type of the stored array data
				 *
				 * @return An error status indicating the success or failure of the operation
				 * @retval cupcfd::error::E_SUCCESS The method completed successfully
				 */
				virtual void setNParts(I nParts);

				/**
				 * Get the number of partitions the graph space should be split into
				 *
				 * @tparam I The type of the indexing scheme
				 * @tparam T The type of the stored array data
				 *
				 * @return The number of partitions this partitioner is currently using
				 */
				__attribute__((warn_unused_result))
				virtual I getNParts();

				/**
				 * Distribute the node allocations across the ranks present in the communicator.
				 * The source of the allocations is the current contents of the results array in the partitioner object,
				 * and the node allocations are done on the basis of the rank allocation = partition value for a node.
				 *
				 * ToDo: The method of distribution could be improved - not every rank that is receiving assigned nodes
				 * necessarily wants to have to create the partitioner object.
				 *
				 * @param rankNodes A pointer to the location where an array containing the nodes assigned to the current rank
				 * will be stored.
				 * @param nNodes A pointer to the location where the number of nodes assigned to this rank will be stored.
				 * @param distributeComm The communicator containing the ranks that will be assigned nodes. All ranks in the
				 * communicator must participate (this is a blocking procedure). In addition the partitioner workComm must
				 * be a subset of this communicator, since they hold the data to communicate.
				 *
				 * @tparam I The type of the indexing scheme
				 * @tparam T The type of the stored array data
				 *
				 * @return An error status indicating the success or failure of the operation
				 * @retval cupcfd::error::E_SUCCESS The method completed successfully
				 * @retval cupcfd::error::E_PARTITIONER_NO_RESULTS There are no results to distribute.
				 */
				__attribute__((warn_unused_result))
				virtual cupcfd::error::eCodes assignRankNodes(T** rankNodes,
																   I * nNodes);

				/**
				 *  Reset all internal data stores and configuration settings for the partitioner.
				 *
				 * @tparam I The type of the indexing scheme
				 * @tparam T The type of the stored array data
				 */
				virtual void reset();

				// ========== Pure Virtual Methods ===============

				/**
				 * Partition the current stored node data using the stored configuration.
				 * If a required component is not set (e.g. number of partitions, node data etc) a suitable error code
				 * will be returned.
				 *
				 * The result will be stored in the result data array of the partitioner object (potentially distributed depending
				 * on the use of a parallel or distributed partitioner).
				 *
				 * Depending on the use of a serial or parallel partitioner, this may involve redistribution
				 * of data across the ranks for partitioning and require all participating ranks to call the method.
				 *
				 * @param Number of partitions to partition the data into
				 *
				 * @tparam I The type of the indexing scheme
				 * @tparam T The type of the stored array data
				 *
				 * @return An error status indicating the success or failure of the operation
				 * @retval cupcfd::error::E_SUCCESS The method completed successfully
				 * @retval cupcfd::error::E_PARTITIONER_MISSING_NODE_DATA No node data is stored.
				 */
				__attribute__((warn_unused_result))
				virtual cupcfd::error::eCodes partition() = 0;

				/**
				 * Partition a distributed graph using the partitioner.
				 * The result will be stored in the result data array of the partitioner object (potentially distributed depending
				 * on the use of a parallel or distributed partitioner).
				 *
				 * This variant will setup the internal node data based on the node data of the provided graph, as well
				 * as any other internal constructs required, so data does not need to be setup prior to calling this function.
				 * This can be done manually, piece-by-piece but this function is provided for convenience to perform
				 * all of the required parts.
				 *
				 * Every member of Comm (i.e. every rank that created the partitioner object) must participate in this function.
				 * In addition, currently the partitioner communicator must be the same as the graph communicator (it must
				 * be known that we can get full and complete graph data for the partitioner comm members).
				 *
				 * This method will also reset and then setup any members, work arrays and result arrays as
				 * needed for the partition.
				 *
				 * @param graph The distributed graph to be partitioned
				 * @param nParts The number of partitions the graph should be split into
				 *
				 * @tparam I The type of the indexing scheme
				 * @tparam T The type of the stored array data
				 *
				 * @return An error status indicating the success or failure of the operation
				 * @retval cupcfd::error::E_SUCCESS The method completed successfully
				 */
				__attribute__((warn_unused_result))
				virtual cupcfd::error::eCodes initialise(cupcfd::data_structures::DistributedAdjacencyList<I, T>& graph, I nParts) = 0;
		};
	}
}

// Include Header Level Definitions
#include "PartitionerInterface.ipp"

#endif
