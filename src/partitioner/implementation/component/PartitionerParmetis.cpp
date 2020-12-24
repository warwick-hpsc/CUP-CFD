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

#include "PartitionerParmetis.h"
#include "AllToAll.h"

namespace cupcfd
{
	namespace partitioner
	{
		template <class I, class T>
		PartitionerParmetis<I,T>::PartitionerParmetis(cupcfd::comm::Communicator& workComm)
		: PartitionerInterface<I,T>(workComm),
		  nTpwgts(0),
		  nXAdj(0),
		  nAdjncy(0),
		  nVtxdist(0),
		  nVwgt(0),
		  nAdjwgt(0),
		  nUbvec(0),
		  nCon(0)
		{
			// This is a precaution, since if the pointers are undetermined they may not be null
			// (which would cause issues with free)
			this->xadj = nullptr;
			this->adjncy = nullptr;
			this->vtxdist = nullptr;

			this->vwgt = nullptr;
			this->adjwgt = nullptr;
			this->tpwgts = nullptr;
			this->ubvec = nullptr;

			this->result = nullptr;

			// Set to default parmetis options
			this->options[0] = 0;

			// Set to base-0 indexes
			this->numflag = 0;
		}

		template <class I, class T>
		PartitionerParmetis<I,T>::PartitionerParmetis(cupcfd::data_structures::DistributedAdjacencyList<I,T>& sourceGraph, int nParts, int nCon)
		: xadj(nullptr),
		  adjncy(nullptr),
		  vtxdist(nullptr),
		  vwgt(nullptr),
		  adjwgt(nullptr),
		  tpwgts(nullptr),
		  ubvec(nullptr),
		  nTpwgts(0),
		  nXAdj(0),
		  nAdjncy(0),
		  nVtxdist(0),
		  nVwgt(0),
		  nAdjwgt(0),
		  nUbvec(0),
		  nCon(nCon),
		  numflag(0),
		  PartitionerInterface<I,T>(sourceGraph, nParts)
		{
			cupcfd::error::eCodes status;

			// Set the options of Parmetis
			this->options[0] = 0;

			// Reset the parmetis data arrays
			this->resetWorkArrays();
			this->resetVertexEdgeWeights();
			this->resetSubdomainWeights();
			this->resetVertexImbalanceWeights();

			// Now we have base items set up, and Parmetis specific items set to null/uninitialised,
			// start setting up the Parmetis data structures

			// // === Set NCon Size ===
			// status = this->setNCon(1);
			// if (status != cupcfd::error::E_SUCCESS) {
			// 	throw(std::runtime_error(std::string("PartitionerParmetis: CONSTRUCTOR: setNCon() failed")));
			// }

			// === Set VertexImbalanceWeightArray ===
			status = this->setVertexImbalanceWeightArrays();
			HARD_CHECK_ECODE(status)

			// setSubdomainWeightArrays cannot be set till nCon and nParts set so do so here.
			status = this->setSubdomainWeightArrays();
			HARD_CHECK_ECODE(status)

			// === Setup Work Arrays ===
			status = this->setWorkArrays(sourceGraph);
			if (status != cupcfd::error::E_SUCCESS) {
				if (status == cupcfd::error::E_DISTGRAPH_UNFINALIZED) {
					// TODO: setWorkArrays() is returning E_DISTGRAPH_UNFINALIZED. Why?! Should finalized flag be true? If not, 
					//       why is it operating on an unfinalized graph?!
					//       Ignore error for now, as app was running before.
				} else {
					HARD_CHECK_ECODE(status)
				}
			}

			// Done
		}

		template <class I, class T>
		PartitionerParmetis<I,T>::~PartitionerParmetis() {
			// === Reset the partitioner components specific to the Parmetis variant ===

			// Set nCon to 0 (i.e. not set)
			this->nCon = 0;

			// Set to default parmetis options
			this->options[0] = 0;

			// Set to base-0 indexes
			this->numflag = 0;

			// Reset the parmetis data arrays
			this->resetWorkArrays();
			this->resetVertexEdgeWeights();
			this->resetSubdomainWeights();
			this->resetVertexImbalanceWeights();
		}

		template <class I, class T>
		void PartitionerParmetis<I,T>::resetWorkArrays() {
			// Free allocated arrays
			if(this->xadj != nullptr) {
				free(this->xadj);
			}

			if(this->adjncy != nullptr) {
				free(this->adjncy);
			}

			if(this->vtxdist != nullptr) {
				free(this->vtxdist);
			}

			// Reset to nullptr to indicate they are inactive
			this->xadj = nullptr;
			this->nXAdj = 0;

			this->adjncy = nullptr;
			this->nAdjncy = 0;

			this->vtxdist = nullptr;
			this->nVtxdist = 0;
		}

		template <class I, class T>
		void PartitionerParmetis<I,T>::setNParts(I nParts) {
			// Set the sub-domain value
			this->nParts = nParts;

			// Reset in case arrays exist - they will need to be recreated/resized
			// ubvec does not use nparts so no need to reset it.
			this->resetSubdomainWeights();
		}

		template <class I, class T>
		void PartitionerParmetis<I,T>::setNCon(I nCon) {
			// Set the number of weights that each vertex has (also number of balance constraints)
			this->nCon = nCon;

			// Reset in case arrays exist - they will need to be recreated/resized

			this->resetSubdomainWeights();
			this->resetVertexImbalanceWeights();
		}

		template <class I, class T>
		cupcfd::error::eCodes PartitionerParmetis<I,T>::setWorkArrays(cupcfd::data_structures::DistributedAdjacencyList<I, T>& distGraph) {
			cupcfd::error::eCodes status;

			// Error Checks:
			// Check that the provided graph has been finalised, so that it has all the data we need
			if(distGraph.finalized == false) {
				return cupcfd::error::E_DISTGRAPH_UNFINALIZED;
			}

			// Error Check:
			// Check that the number of local nodes in the partition graph is not zero (we need some work to operate with)
			if(distGraph.nLONodes == 0) {
				return cupcfd::error::E_DISTGRAPH_NO_LOCAL_NODES;
			}

			// (a) Clear any exiting work arrays
			this->resetWorkArrays();

			// (b) Setup node data workspace
			// Allocate suitable space - will be assigning locally owned nodes for this rank from the graph
			this->nNodes = distGraph.nLONodes;
			this->nodes = (T *) malloc(sizeof(T) * this->nNodes);


			// Make a copy of locally owned nodes from the graph
			status = distGraph.getLocalNodes(this->nodes, this->nNodes);
			CHECK_ECODE(status)

			// (c) Setup xadj indirect node->edges array
			// Allocate suitable space, should always be one more than there are nodes
			this->nXAdj = this->nNodes + 1;
			this->xadj = (idx_t *) malloc(sizeof(idx_t) * this->nXAdj);


			// First position always points to zero in the adjncy array
			// ToDo: This is assuming there is always at least one locally owned node.
			this->xadj[0] = 0;

			// Retrieve the number of adjacency nodes for each node
			I nEdges = 0;

			for(I i = 0; i < this->nNodes; i++) {
				I adjCount;
				status = distGraph.connGraph.getAdjacentNodeCount(this->nodes[i], &adjCount);
				CHECK_ECODE(status)
				nEdges = nEdges + adjCount;

				// Set next index along, this should leave the final position being set at one past the final index of adjncy
				this->xadj[i+1] = nEdges;
			}

			// (d) Setup the adjncy array that stores edges for the nodes
			// Allocate storage space
			this->nAdjncy = nEdges;
			this->adjncy = (idx_t *) malloc(sizeof(idx_t) * this->nAdjncy);


			// Copy edge data across - note that for Parmetis, we must use global index references for nodes.
			// The distributed graph should have a suitably constructed index system already that we can reuse,
			// where locally owned nodes are assigned a global index sequentially across the ranks
			// Loop over nodes
			I ptr = 0;
			for(I i = 0; i < this->nNodes; i++) {
				I adjCount = this->xadj[i+1] - this->xadj[i];

				// Get nodes adjacent to current node
				T * scratch = (T *) malloc(sizeof(T) * adjCount);
				status = distGraph.connGraph.getAdjacentNodes(this->nodes[i], scratch, adjCount);
				CHECK_ECODE(status)

				// Loop over each adjacent node
				for(I j = 0; j < adjCount; j++) {
					// Retrieve and store the adjacent nodes global index
					this->adjncy[ptr] = distGraph.nodeToGlobal[scratch[j]];
					ptr = ptr + 1;
				}

				free(scratch);
			}

			// (e) Setup the communicator and vtxdist array that stores the number of nodes that have been assigned to each rank of the partitioner
			// The distributed graph should already have such data stored

			// Allocate the workspace
			this->workComm = *(distGraph.comm);

			this->nVtxdist = this->workComm.size + 1;
			this->vtxdist = (idx_t *) malloc(sizeof(idx_t) * this->nVtxdist);

			this->vtxdist[0] = 0;
			for(I i = 1; i < this->nVtxdist; i++) {
				this->vtxdist[i] = this->vtxdist[i-1] + distGraph.processNodeCounts[i-1];
			}

			return cupcfd::error::E_SUCCESS;
		}

		template <class I, class T>
		void PartitionerParmetis<I,T>::resetVertexEdgeWeights() {
			if(this->vwgt != nullptr) {
				free(this->vwgt);
			}

			if(this->adjwgt != nullptr) {
				free(this->adjwgt);
			}

			this->vwgt = nullptr;
			this->nVwgt = 0;

			this->adjwgt = nullptr;
			this->nAdjwgt = 0;

			// Set to 0 to indicate neither vertex or edges are weighted
			// ToDo: Reviewing later - I declared this as local - not used??
			// idx_t wgtflag = 0;
		}

		template <class I, class T>
		void PartitionerParmetis<I,T>::resetSubdomainWeights() {
			if(this->tpwgts == nullptr) {
				free(this->tpwgts);
			}

			this->tpwgts = nullptr;
			this->nTpwgts = 0;
		}

		template <class I, class T>
		void PartitionerParmetis<I,T>::resetVertexImbalanceWeights() {
			if(this->ubvec == nullptr) {
				free(this->ubvec);
			}

			this->ubvec = nullptr;
			this->nUbvec = 0;
		}

		template <class I, class T>
		cupcfd::error::eCodes PartitionerParmetis<I,T>::setWeightArrays() {
			// ToDo: Currently weights for vertex, edges are not set via this method since they are
			// fixed.
			return cupcfd::error::E_PARMETIS_ERROR;
		}

		template <class I, class T>
		cupcfd::error::eCodes PartitionerParmetis<I,T>::setSubdomainWeightArrays() {
			// Error Check 1: Check nCon has been set
			if(this->nCon < 1) {
				// Error Case - Cannot allocate suitably sized array
				return cupcfd::error::E_PARMETIS_INVALID_NCON;
			}

			// Error Check 2: Check nparts has been set
			if(this->nParts < 1) {
				// Error Case - Cannot allocate suitably sized array
				// Technically partitioning requires a minimum nParts of 2...
				return cupcfd::error::E_PARMETIS_INVALID_NPARTS;
			}

			// Reset the subdomain weights to free memory in case it is already allocated
			this->resetSubdomainWeights();

			this->nTpwgts = this->nCon * this->nParts;
			this->tpwgts = (real_t *) malloc(sizeof(real_t) * this->nTpwgts);
			for(I i = 0; i < this->nTpwgts; i++) {
				this->tpwgts[i] = 1.0/this->nParts;
			}

			return cupcfd::error::E_SUCCESS;
		}

		template <class I, class T>
		cupcfd::error::eCodes PartitionerParmetis<I,T>::setVertexImbalanceWeightArrays() {
			// Error Check 1: Check nCon has been set
			if(this->nCon < 1) {
				// Error Case - Cannot allocate suitably sized array
				return cupcfd::error::E_PARMETIS_INVALID_NCON;
			}

			// Reset to free memory in case it is already allocated
			this->resetVertexImbalanceWeights();

			this->nUbvec = this->nCon;
			this->ubvec = (real_t *) malloc(sizeof(real_t) * this->nUbvec);
			for(I i = 0; i < this->nUbvec; i++) {
				this->ubvec[i] = 1.05;
			}

			return cupcfd::error::E_SUCCESS;
		}

		template <class I, class T>
		cupcfd::error::eCodes PartitionerParmetis<I,T>::partition() {
			// === Input Error Checks ===
			// Error Check 1 - Work arrays all exist
			if(this->xadj == nullptr) {
				return cupcfd::error::E_PARMETIS_INVALID_WORK_ARRAY;
			}

			if(this->adjncy == nullptr) {
				return cupcfd::error::E_PARMETIS_INVALID_WORK_ARRAY;
			}

			if(this->vtxdist == nullptr) {
				return cupcfd::error::E_PARMETIS_INVALID_WORK_ARRAY;
			}

			if(this->nCon < 1) {
				return cupcfd::error::E_PARMETIS_INVALID_NCON;
			}

			// Error Check 2 - Number of parts has been set to something greater than 1
			if(this->nParts <= 1) {
				// Error - Can't partition into one or fewer parts
				return cupcfd::error::E_PARMETIS_INVALID_NPARTS;
			}

			if(this->tpwgts == nullptr) {
				return cupcfd::error::E_PARMETIS_INVALID_SUBDOMAIN_WEIGHT_ARRAYS;
			}

			if(this->ubvec == nullptr) {
				return cupcfd::error::E_PARMETIS_INVALID_VERTEX_IMBALANCE_WEIGHT_ARRAYS;
			}

			// Error Check 3 - Communicator is set to something other than SELF/size greater than 1
			// (i.e. should be parallel)
			// ToDo: Does this need to be the case? Parmetis may work with 1 process - check

			// Reset the results array since we are about to overwrite it
			this->resetResultStorage();

			// Error Check 4: Nodes Array Exists

			// Allocate the result array to store the parmetis result
			this->result = (I *) malloc(sizeof(I) * this->nNodes);
			this->nResult = this->nNodes;

			// Some arrays can be set to NULL - i.e. vwgt, adjwgt
			// In case they are set to nullptr, convert to NULL as a precaution for C calls
			idx_t * vwgt = this->vwgt;
			idx_t * adjwgt = this->adjwgt;

			if(vwgt == nullptr) {
				vwgt = NULL;
			}

			if(adjwgt == nullptr) {
				adjwgt = NULL;
			}

			// === Run Partitioner ===
			// This is an external function call to the PARMETIS library - it computes the partition.
			int ret = ParMETIS_V3_PartKway(this->vtxdist,
										   this->xadj,
										   this->adjncy,
										   vwgt,
										   adjwgt,
										   &this->wgtflag,
										   &this->numflag,
										   &this->nCon,
										   &this->nParts,
										   this->tpwgts,
										   this->ubvec,
										   this->options,
										   &this->edgecut,
										   this->result,
										   &(this->workComm.comm)
										   );

			// N.B.Parmetis manual states this is METIS_ERROR (i.e. not PARMETIS_ERROR)
			if(ret == METIS_ERROR) {
				return cupcfd::error::E_PARMETIS_LIBRARY_ERROR;
			}

			return cupcfd::error::E_SUCCESS;
		}

		template <class I, class T>
		cupcfd::error::eCodes PartitionerParmetis<I,T>::initialise(cupcfd::data_structures::DistributedAdjacencyList<I, T>& graph, I nParts) {
			cupcfd::error::eCodes status;

			// === Set NCon Size ===
			this->setNCon(1);

			// === Set Partition Size ===
			this->setNParts(nParts);

			// === Set VertexImbalanceWeightArray ===
			status = this->setVertexImbalanceWeightArrays();
			CHECK_ECODE(status)

			// setSubdomainWeightArrays cannot be set till nCon and nParts set so do so here.
			status = this->setSubdomainWeightArrays();
			CHECK_ECODE(status)

			// === Setup Work Arrays ===
			status = this->setWorkArrays(graph);
			CHECK_ECODE(status)

			return cupcfd::error::E_SUCCESS;
		}

		template <class I, class T>
		void PartitionerParmetis<I,T>::reset() {
			// Base class reset
			this->PartitionerInterface<I,T>::reset();

			this->options[0] = 0;
			this->numflag = 0;
			this->setNParts(0);
			this->setNCon(0);
			this->resetWorkArrays();
			this->resetVertexEdgeWeights();
			this->resetSubdomainWeights();
			this->resetVertexImbalanceWeights();
		}
	}
}

// Explicit Instantiation

template class cupcfd::partitioner::PartitionerParmetis<int,int>;

