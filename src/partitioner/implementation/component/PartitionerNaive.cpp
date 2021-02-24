/*
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Description
 *
 */

#include <iostream>

#include "PartitionerNaive.h"
#include "ArrayDrivers.h"
#include "SearchDrivers.h"
#include "StatisticsDrivers.h"
#include "Gather.h"
#include "AllToAll.h"

#include "DistributedAdjacencyList.h"
#include <cmath>

namespace cupcfd
{
	namespace partitioner
	{
		template <class I, class T>
		PartitionerNaive<I,T>::PartitionerNaive(cupcfd::comm::Communicator& workComm)
		: PartitionerInterface<I,T>(workComm)
		{

		}

		template <class I, class T>
		PartitionerNaive<I,T>::PartitionerNaive(cupcfd::data_structures::DistributedAdjacencyList<I,T>& sourceGraph, int nParts)
		:PartitionerInterface<I,T>(sourceGraph, nParts)
		{
			// Nothing to do beyond base class
		}

		template <class I, class T>
		PartitionerNaive<I,T>::~PartitionerNaive()
		{
			// Nothing to do in this subclass.
		}

		// template <class I, class T>
		// cupcfd::error::eCodes PartitionerNaive<I,T>::reset() {
		// 	// Nothing to reset beyond the base class components
		// 	this->PartitionerInterface<I,T>::reset();

		// 	return cupcfd::error::E_SUCCESS;
		// }

		template <class I, class T>
		cupcfd::error::eCodes PartitionerNaive<I,T>::partition() {
			// The naive partition approach simply divides the total number of nodes
			// stored across the various node stores in the communicator for the partitioner.

			// As such, it can just use whatever is set in nodes directly rather than have an additional
			// store for transformed data.

			// This naturally means that it does not account for any edge data (hence naive!)

			cupcfd::error::eCodes status;

			// Error Check: Check that nParts is set
			if(this->getNParts() == 0) {
				return cupcfd::error::E_PARTITIONER_NPARTS_UNSET;
			}

			// Error Check: Ensure that there is data in the node store.
			if(this->nodes == nullptr) {
				return cupcfd::error::E_PARTITIONER_MISSING_NODE_DATA;
			}

			// AllGather - Get the number of nodes on each rank
			I * rankNNodes = (I *) malloc(sizeof(I) * this->workComm.size);
			status = cupcfd::comm::AllGather(&this->nNodes, 1, rankNNodes, this->workComm.size, 1, this->workComm);
			CHECK_ECODE(status)

			// Since this is naive, doesn't really matter *which* nodes we are assigned - they don't have to be sequential
			// As such, just compute the assigned partition based on which range it falls into

			I commSize = this->workComm.size;
			I commRank = this->workComm.rank;

			// Sum Total Number of Nodes
			// ToDo: This is going to cost more with increasing process count
			I gNodes;
			cupcfd::utility::drivers::sum(rankNNodes, commSize, &gNodes);

			// Compute Sizes of Partitions
			I partitionSize = gNodes/ commSize;
			I partitionerRemainder = gNodes % commSize;

			// Sum number of nodes prior to this rank
			I nPriorNodes;
			cupcfd::utility::drivers::sum(rankNNodes, commRank, &nPriorNodes);

			// Allocate space for storing the result
			this->result = (I *) malloc(sizeof(I) * this->nNodes);
			this->nResult = this->nNodes;

			// Assign partition sizes
			// If there is a remainder (uneven division) add 1 to size of each partition until
			// remainder is used up.

			// Prior Nodes is the number of nodes assigned to a group before this rank.
			// Since the number of nodes on each rank may be even, the rank assignment is based
			// on where the current count of nodes globally lies on this rank, in order, so they
			// can be placed in the correct partition group.

			// If this partition also has a remainder, then 1 more extra cell must be assigned to it.
			for(I i = 0; i < this->nNodes; i++) {
				// Zero-Indexed

				// Compute the group assignment for a node base on its count position when globally
				// ordering over the nodes in rank order.
				I id = nPriorNodes + i;
				I group = (id / partitionSize);

				// ToDo: Should just use min
				// Add 1 to remainder for each prior group
				I check;
				if(group < partitionerRemainder) {
					check = group;
				}
				else {
					check = partitionerRemainder;
				}

				// Adjust group assignment if within remainder limits
				if((group > 0) && (id < ((group * partitionSize) + check))) {
					group = group - 1;
				}

				this->result[i] = group;
			}

			// Should now have a pairwise set of data for node: result - Finished.

			return cupcfd::error::E_SUCCESS;
		}

		template <class I, class T>
		cupcfd::error::eCodes PartitionerNaive<I,T>::initialise(cupcfd::data_structures::DistributedAdjacencyList<I, T>& graph, I nParts) {
			cupcfd::error::eCodes status;

			this->setNParts(nParts);

			// === Set the node data ===
			// ToDo: Since we use the setter, this adds an unnecessary copy - could operate directly on pointers inside object
			I nNodes = graph.nLONodes;
			T * nodes = (T *) malloc(sizeof(T) * nNodes);

			// Make a copy of locally owned nodes from the graph
			status = graph.getLocalNodes(nodes, nNodes);
			CHECK_ECODE(status)

			// Set the nodes in the partitioner
			this->setNodeStorage(nodes, nNodes);

			// Cleanup temporary store
			free(nodes);

			// Naive Partitioner is now ready for partitioning.

			return cupcfd::error::E_SUCCESS;
		}
	}
}

// Explicit Instantiation
template class cupcfd::partitioner::PartitionerNaive<int,int>;
