/*
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Contains declarations for the PartitionerInterface class
 */

#include "PartitionerInterface.h"
#include "ArrayDrivers.h"
#include "AllToAll.h"

namespace cupcfd
{
	namespace partitioner
	{
	// ===== Constructors/Deconstructors ====

		template <class I, class T>
		PartitionerInterface<I,T>::PartitionerInterface(cupcfd::comm::Communicator& workComm)
		: nodes(nullptr),
		  nNodes(0),
		  result(nullptr),
		  nResult(0),
		  nParts(0)
		{
			this->workComm = workComm;
		}

		template <class I, class T>
		PartitionerInterface<I,T>::PartitionerInterface(cupcfd::data_structures::DistributedAdjacencyList<I,T>& sourceGraph, int nParts)
		: nodes(nullptr),
		  nNodes(0),
		  result(nullptr),
		  nResult(0),
		  nParts(0)
		{
			cupcfd::error::eCodes status;

			this->workComm = *(sourceGraph.comm);

			// === Set Nparts ===
			// Use setter, not member initialisation - some subclasses may need to do additional things
			this->setNParts(nParts);

			// === Set the Internal Node Data ===
			// ToDo: Since we use the setter, this adds an unnecessary copy - could operate directly on pointers inside object
			I nNodes = sourceGraph.nLONodes;
			T * nodes = (T *) malloc(sizeof(T) * nNodes);

			// Make a copy of locally owned nodes from the graph
			status = sourceGraph.getLocalNodes(nodes, nNodes);
			HARD_CHECK_ECODE(status)

			// Set the nodes in the partitioner
			this->setNodeStorage(nodes, nNodes);

			// Cleanup temporary store
			free(nodes);
		}

		template <class I, class T>
		PartitionerInterface<I,T>::~PartitionerInterface()
		{
			// Cleanup the object by freeing any space currently being used to store
			// result or node data
			this->nParts = 0;
			this->resetResultStorage();
			this->resetNodeStorage();
		}

		// ========== Concrete Methods ===============
		template <class I, class T>
		void PartitionerInterface<I,T>::resetResultStorage() {
			if(this->result != nullptr) {
				free(this->result);
			}

			this->result = nullptr;
			this->nResult = 0;
		}

		template <class I, class T>
		void PartitionerInterface<I,T>::setNodeStorage(T * nodes, I nNodes) {
			// Clear existing data stores
			this->resetNodeStorage();
			this->resetResultStorage();

			this->nNodes = nNodes;
			// this->nodes = (T *) malloc(sizeof(T) * this->nNodes);
			// status = cupcfd::utility::drivers::copy(nodes, nNodes, this->nodes, this->nNodes);
			// CHECK_ECODE(status)
			// return cupcfd::error::E_SUCCESS;
			this->nodes = cupcfd::utility::drivers::duplicate(nodes, nNodes);
		}

		template <class I, class T>
		void PartitionerInterface<I,T>::resetNodeStorage() {
			if(this->nodes != nullptr) {
				free(this->nodes);
			}

			this->nodes = nullptr;
			this->nNodes = 0;
		}

		template <class I, class T>
		void PartitionerInterface<I,T>::setNParts(I nParts) {
			// Set the sub-domain value
			this->nParts = nParts;
		}

		template <class I, class T>
		I PartitionerInterface<I,T>::getNParts() {
			return this->nParts;
		}

		template <class I, class T>
		cupcfd::error::eCodes PartitionerInterface<I,T>::assignRankNodes(T** rankNodes,
																		I * nNodes) {
			cupcfd::error::eCodes status;

			// === Input Error Checks ===
			// Error Check 1: Ensure that the results array exists
			if(this->result == nullptr) {
				// Error - Results array is not allocated
				return cupcfd::error::E_PARTITIONER_NO_RESULTS;
			}

			// Error Check 2: The size of the communicator the node allocations will be distributed must be equal to or larger than the number of partitions
			// (If larger then some ranks will clearly get no allocated nodes - generate a warning in this case?)
			if(this->workComm.size < this->nParts) {
				return cupcfd::error::E_PARMETIS_UNDERSIZED_COMM;
			}

			// === Distribute the partition ===
			// Since each process may have data for every other process, this will necessitate some form of all-to-all.
			// We will leave the sorting and distribution to the all-to-all interface driver

			// Note: The partitioner will have copies of the nodes in partitioner.nodes, alongside their allocated
			// partitioner/rank in the partitioner.result aray at the same index.
			// Since in a distributed graph, we only have a copy of locally assigned nodes, we need to communicate these nodes
			// at this stage - if these nodes are large objects, this could be expensive!
			// If sourcing the nodes from another source (e.g. I/O), it may be cheaper to use graphs that are a mapping of an id to a node
			// from a source, such as a file.
			// However, lacking another meaningful way to communicate these nodes easily, we do so here with the all-to-all

			status = cupcfd::comm::AllToAll(this->nodes, this->nNodes,
											this->result, this->nResult,
											rankNodes, nNodes,
											workComm);
			CHECK_ECODE(status)

			return cupcfd::error::E_SUCCESS;
		}

		template <class I, class T>
		void PartitionerInterface<I,T>::reset() {
			resetNodeStorage();
			resetResultStorage();
		}
	}
}

// Explicit Instantiation

template class cupcfd::partitioner::PartitionerInterface<int, int>;
