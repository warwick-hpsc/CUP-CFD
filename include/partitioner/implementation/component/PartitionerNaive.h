/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Contains the declaration for the PartitionerNaive class
 */

#ifndef CUPCFD_PARTITIONER_NAIVE_INCLUDE_H
#define CUPCFD_PARTITIONER_NAIVE_INCLUDE_H

#include "PartitionerInterface.h"

namespace cupcfd
{
	namespace partitioner
	{
		/**
		 * Partitioner interface for using a Naive approach to partitioning.
		 *
		 * This partitioner is parallel aware (although not efficient) and
		 * so can partition distributed data.
		 *
		 * It partitions based on node ordering from rank 0 -> rank n-1, assigning a node an id
		 * to share them as evenly as possible across the ranks.
		 * This ordering is technically arbitrary, but is typically ordered by rank, and sorted by node
		 * value on rank (due to the use of graphs).
		 *
		 * Remainders are split evenly as possible amongst the first n ranks, where n is the remainder
		 * by increasing their allocation by 1.
		 *
		 * The actual value of the nodes, or their graph edges/connections do not matter.
		 *
		 * @tparam I The type of the indexing scheme
		 * @tparam T The type of the stored array data
		 */
		template <class I, class T>
		class PartitionerNaive : public cupcfd::partitioner::PartitionerInterface<I,T>
		{
			public:
				// === Members ===

				// === Methods ===
				// Constructor/Deconstructors

				/**
				 * Create a default Partitioner object.
				 * Initially this leaves the many work arrays and result arrays unset.
				 * they can be set in one of two ways - directly, or via the use of a function
				 * like partition that will setup the various member values and arrays where needed.
				 *
				 * @param workComm Communicator of all processes creating a partitioning object that
				 * could potentially receive partition assignments.
				 */
				PartitionerNaive(cupcfd::comm::Communicator& workComm);

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
				PartitionerNaive(cupcfd::data_structures::DistributedAdjacencyList<I,T>& sourceGraph, int nParts);

				/**
				 * Deconstructor for a PartitionerMetis object.
				 * This will free up any memory being used to store result or node data.
				 */
				~PartitionerNaive();

				// === Inherited Overrides ===
				__attribute__((warn_unused_result))
				cupcfd::error::eCodes reset();
				__attribute__((warn_unused_result))
				cupcfd::error::eCodes partition();
				__attribute__((warn_unused_result))
				cupcfd::error::eCodes initialise(cupcfd::data_structures::DistributedAdjacencyList<I, T>& graph, I nParts);
		};
	}
}

// Include Header Level Definitions
#include "PartitionerNaive.ipp"

#endif
