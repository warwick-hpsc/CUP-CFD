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

#include "PartitionerMetis.h"
#include "ArrayDrivers.h"
#include "SearchDrivers.h"
#include "StatisticsDrivers.h"
#include "Scatter.h"
#include "Gather.h"
#include "Broadcast.h"
#include "SortDrivers.h"

namespace cupcfd
{
	namespace partitioner
	{
		template <class I, class T>
		PartitionerMetis<I,T>::PartitionerMetis(cupcfd::comm::Communicator workComm)
		: PartitionerInterface<I,T>(workComm)
		{
			// Set nCon to 0 (i.e. not set)
			this->nCon = 0;

			// Set to default options
			this->options[0] = 0;

			// Set to base-0 indexes
			this->numflag = 0;

			this->xadj = nullptr;
			this->nXAdj = 0;

			this->adjncy = nullptr;
			this->nNodes = 0;
		}

		template <class I, class T>
		PartitionerMetis<I,T>::PartitionerMetis(cupcfd::data_structures::DistributedAdjacencyList<I,T>& sourceGraph, int nParts, int nCon)
		: PartitionerInterface<I,T>(*(sourceGraph.comm)),
		  xadj(nullptr),
		  adjncy(nullptr),
		  nCon(0),
		  numflag(0)
		{
			cupcfd::error::eCodes status;

			// Note: The constructor used from the base class leaves most values uninitialised or set to null.
			// We can't use the base version that sets up the node arrays, since it will do it on all processes
			// whereas we only want it on root.
			// It does however set the workComm for us

			// Set the options of Metis
			this->options[0] = 0;

			// === Set Nparts ===
			// Use setter, not member initialisation - some subclasses may need to do additional things
			this->setNParts(nParts);

			// === Set NCon ===
			this->setNCon(nCon);

			// === Set the Internal Node Data ===

			// Build a serial graph from a distributed graph
			// This graph will exist on the root process of the distributed graph communicator
			cupcfd::data_structures::AdjacencyListCSR<I,T> * rootGraph = nullptr;

			if(this->workComm.root) {
				rootGraph = new cupcfd::data_structures::AdjacencyListCSR<I,T>();
			}

			// Convert the distributed graph to a serial graph on the root process
			status = sourceGraph.buildSerialAdjacencyList(rootGraph, this->workComm.root_rank);
			HARD_CHECK_ECODE(status)

			// Setup the internal data structures on the root process
			// This includes:
			// (a) The node storage
			// (b) The work storage
			if(this->workComm.root) {
				I nNodes = rootGraph->getNodeCount();

				//std::shared_ptr<std::array<T,nNodes>> nodes;
				T * nodes = (T *) malloc(sizeof(T) * nNodes);

				// Make a copy of locally owned nodes from the graph and store them in the partitioner
				// ToDo: Since we use the setter, this adds an unnecessary copy - could operate directly on pointers inside object
				status = rootGraph->getNodes(nodes, nNodes);
				HARD_CHECK_ECODE(status)
				this->setNodeStorage(nodes, nNodes);

				// Setup the work arrays
				status = this->setWorkArrays(*rootGraph);
				HARD_CHECK_ECODE(status)

				// Cleanup temporary store
				free(nodes);

				// Don't need the serial graph anymore
				delete rootGraph;
			}
		}

		template <class I, class T>
		PartitionerMetis<I,T>::~PartitionerMetis() {
			// Node, Results Arrays will be reset by base class
			this->resetWorkArrays();
		}

		template <class I, class T>
		void PartitionerMetis<I,T>::resetWorkArrays() {
			// Free allocated arrays
			if(this->xadj != nullptr) {
				free(this->xadj);
			}

			if(this->adjncy != nullptr) {
				free(this->adjncy);
			}

			// Reset to nullptr to indicate they are inactive
			this->xadj = nullptr;
			this->nXAdj = 0;

			this->adjncy = nullptr;
			this->nAdjncy = 0;

			// No nodes currently in work arrays
			this->nNodes = 0;
		}

		template <class I, class T>
		void PartitionerMetis<I,T>::setNCon(I nCon) {
			// Set the number of weights that each vertex has (also number of balance constraints)
			this->nCon = nCon;
		}

		template <class I, class T>
		cupcfd::error::eCodes PartitionerMetis<I,T>::setWorkArrays(cupcfd::data_structures::AdjacencyListCSR<I,T>& graph) {
			// AdjacencyListCSR has no features for distributed nodes, so it can only refer to nodes that are
			// stored within the AdjacencyListCSR, and so we can treat it as serial data

			cupcfd::error::eCodes status;

			if(graph.nNodes == 0) {
				//std::cout << "Error in Metis setWorkArrays, partition graph has zero nodes\n";
			}

			// (a) Clear any exiting work arrays
			this->resetWorkArrays();

			// (b) Setup node data workspace
			// Allocate suitable space
			this->nNodes = graph.nNodes;
			this->nodes = (T *) malloc(sizeof(T) * this->nNodes);

			// Make a copy of locally owned nodes from the graph
			status = graph.getNodes(this->nodes, this->nNodes);
			CHECK_ECODE(status)

			// Make a copy of the graph CSR
			this->nXAdj = graph.xadj.size();
			this->xadj = (idx_t *) malloc(sizeof(idx_t) * this->nXAdj);

			this->nAdjncy = graph.adjncy.size();
			this->adjncy = (idx_t *) malloc(sizeof(idx_t) * this->nAdjncy);

			//cupcfd::utility::drivers::copy(&(graph.xadj[0]), graph.xadj.size(), this->xadj, this->nXAdj);
			//cupcfd::utility::drivers::copy(&(graph.adjncy[0]), graph.adjncy.size(), this->adjncy, this->nAdjncy);

			// ToDo: Fix the copy template for these and switch back to using the array drivers
			I size;
			size = cupcfd::utility::drivers::safeConvertSizeT<I>(graph.xadj.size());
			for(I i = 0; i < size; i++) {
				this->xadj[i] = graph.xadj[i];
			}

			size = cupcfd::utility::drivers::safeConvertSizeT<I>(graph.adjncy.size());
			for(I i = 0; i < size; i++) {
				this->adjncy[i] = graph.adjncy[i];
			}

			return cupcfd::error::E_SUCCESS;
		}

		template <class I, class T>
		cupcfd::error::eCodes PartitionerMetis<I,T>::assignRankNodes(T** rankNodes,
														 I * nNodes) {
			// === Notify all processes in the comm if no results are available ===

			cupcfd::error::eCodes status;

			// bool hasResults = true;
			// if(this->workComm.root)
			// {
			// 	if(this->result == nullptr)
			// 	{
			// 		// Error - Results array is not allocated
			// 		hasResults = false;
			// 	}
			// }
			// if(noResults)
			// {
			// 	return cupcfd::error::E_PARTITIONER_NO_RESULTS;
			// }


			// === Distribute the results ===
			// This scatter function will not only distribute the results, but handle the grouping by partition
			status = cupcfd::comm::Scatter(this->nodes,
											this->nNodes,
											rankNodes,
											nNodes,
											this->result,
											this->nResult,
											this->workComm,
											this->workComm.root_rank);
			CHECK_ECODE(status)

			return cupcfd::error::E_SUCCESS;
		}

		template <class I, class T>
		cupcfd::error::eCodes PartitionerMetis<I,T>::partition() {
			// Only Root does serial partitioning
			if(this->workComm.root) {
				// === Input Error Checks ===
				// Error Check 1 - Work arrays all exist
				if(this->xadj == nullptr) {
					return cupcfd::error::E_PARTITIONER_INVALID_WORK_ARRAY;
				}

				if(this->adjncy == nullptr) {
					return cupcfd::error::E_PARTITIONER_INVALID_WORK_ARRAY;
				}

				// Error Check 2 - Nodes array exists

				// Error Check 3 - Number of parts is acceptable value
				if(this->nParts < 1) {
					// Error - Can't partition into zero or fewer parts (will treat 1 as a straight copy of input)
					return cupcfd::error::E_PARTITIONER_NPARTS_UNSET;
				}

				// Error Check 4 - Check Ncon is acceptable value


				// Reset the results array since we are about to overwrite it
				this->resetResultStorage();

				// Begin Partition
				if(this->nParts == 1) {
					// Every node is assigned to partition 0
					this->result = (I *) malloc(sizeof(I) * this->nNodes);
					this->nResult = this->nNodes;

					for(I i = 0; i < this->nNodes; i++) {
						this->result[i] = this->nodes[i];
					}

					return cupcfd::error::E_SUCCESS;
				}

				// Allocate the result array to store the parmetis result
				this->result = (I *) malloc(sizeof(I) * this->nNodes);
				this->nResult = this->nNodes;

				// This is an external function call to the METIS library - it computes the partition.
				// Currently does not bother with weightings
				int ret = METIS_PartGraphKway(&(this->nNodes),
											  &(this->nCon),
											  this->xadj,
											  this->adjncy,
											  NULL,
											  NULL,
											  NULL,
											  &(this->nParts),
											  NULL,
											  NULL,
											  NULL,
											  &(this->objval),
											  this->result);
				if (ret != METIS_OK) {
					return cupcfd::error::E_METIS_ERROR;
				}
			}

			return cupcfd::error::E_SUCCESS;
		}

		template <class I, class T>
		cupcfd::error::eCodes PartitionerMetis<I,T>::initialise(cupcfd::data_structures::DistributedAdjacencyList<I, T>& graph __attribute__((unused)), I nParts __attribute__((unused))) {
			// // Set the number of parts
			// this->setNParts(nParts);

			// // Build a serial graph from a distributed graph
			// cupcfd::data_structures::AdjacencyListCSR<I,T> * rootGraph = nullptr;

			// // Setup the internal data structures on
			// if(this->workComm.root) {
			// 	// Setup the work arrays - graph should exist on root
			// 	this->setWorkArrays(*rootGraph);

			// 	// Cleanup
			// 	delete rootGraph;
			// }

			// return cupcfd::error::E_SUCCESS;

			// Above code is nonense.
			return cupcfd::error::E_NOT_IMPLEMENTED;
		}

		template <class I, class T>
		void PartitionerMetis<I,T>::reset() {
			// Base class reset
			this->PartitionerInterface<I,T>::reset();

			// Reset Members
			this->nCon = 0;
			this->nParts = 0;
			this->options[0] = 0;
			this->numflag = 0;

			// Reset Work Arrays
			this->resetWorkArrays();
		}

	}
}

// Explicit Instantiation
template class cupcfd::partitioner::PartitionerMetis<int,int>;
