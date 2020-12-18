/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Contains header level definitions for the DistributedAdjacencyList Class
 */
 
#ifndef CUPCFD_DATA_STRUCTURES_DISTRIBUTED_ADJACENCY_LIST_IPP_H
#define CUPCFD_DATA_STRUCTURES_DISTRIBUTED_ADJACENCY_LIST_IPP_H

#include "Gather.h"
#include "ArrayDrivers.h"
#include <iostream>

namespace cupcfd
{
	namespace data_structures
	{
		template <class I, class T>
		template <class C>
		cupcfd::error::eCodes DistributedAdjacencyList<I,T>::buildSerialAdjacencyList(AdjacencyList<C,I,T> * destGraph, I rank) {
			cupcfd::error::eCodes status;
			// === Error Checks ===
			// (1) Are all members of the sourceGraph communicator present - use a barrier
			// ToDo: Multiple Definition error? May be missing an include guard somewhere
			//status = Barrier(cupcfd::comm::Communicator& mpComm);

			// (2) Check that the sourceGraph is finalised
			if(this->finalized == false) {
				DEBUGGABLE_ERROR; return cupcfd::error::E_ERROR;
			}
			
			// (3) Check that destGraph is not null on the specified rank
			if((this->comm->rank == rank) && (destGraph == nullptr)) {
				DEBUGGABLE_ERROR; return cupcfd::error::E_ERROR;
			}
			
			// === Data Gathering ===

			// (1) Each rank must send data about its locally owned nodes to rank.
			// Ghost node data is not needed - if the distributed graph is finalised, then it must be the case
			// that the full node set is covered by the global set of locally owned nodes.
			
			I nLocalNodes = this->nLONodes;
		 	T * localNodes = (T *) malloc(sizeof(T) * nLocalNodes);

			status = this->getLocalNodes(localNodes, nLocalNodes);
			CHECK_ERROR_CODE(status)

			I nRecvNodes;
			T * recvNodes = nullptr;
			
			I nRecvNodeProcCount;
			T * recvNodeProcCount = nullptr;

			status = cupcfd::comm::GatherV(localNodes, nLocalNodes, 
											&recvNodes, &nRecvNodes, 
											&recvNodeProcCount, &nRecvNodeProcCount, 
											rank, *(this->comm));
			CHECK_ERROR_CODE(status)				   
			if(status != cupcfd::error::E_SUCCESS) {
				return status;
			}
			
			// (2) Each rank must send data about the edges it has. This should include in both directions.
			// It will need two stages, one to gather node1 in the edge, and a second to get node 2 in the edge.

			I nEdges;
			status = this->connGraph.getEdgeCount(&nEdges);
			CHECK_ERROR_CODE(status)
			if(status != cupcfd::error::E_SUCCESS) {
				return status;
			}
			
			T * edgeNode1 = (T *) malloc(sizeof(T) * nEdges);
			T * edgeNode2 = (T *) malloc(sizeof(T) * nEdges);
			
			status = this->connGraph.getEdges(edgeNode1, nEdges, edgeNode2, nEdges);
			CHECK_ERROR_CODE(status)
			if(status != cupcfd::error::E_SUCCESS) {
				return status;
			}

			I nRecvEdge1;
			T * recvEdge1 = nullptr;
			
			I nRecvEdge1ProcCount;
			T * recvEdge1ProcCount = nullptr;

			I nRecvEdge2;
			T * recvEdge2 = nullptr;
			
			I nRecvEdge2ProcCount;
			T * recvEdge2ProcCount = nullptr;
			
			status = cupcfd::comm::GatherV(edgeNode1, nEdges, 
										   &recvEdge1, &nRecvEdge1, 
										   &recvEdge1ProcCount, &nRecvEdge1ProcCount, 
										   rank, *(this->comm));
			CHECK_ERROR_CODE(status)
			if(status != cupcfd::error::E_SUCCESS) {
				return status;
			}
							   
			status = cupcfd::comm::GatherV(edgeNode2, nEdges, 
										   &recvEdge2, &nRecvEdge2, 
										   &recvEdge2ProcCount, &nRecvEdge2ProcCount, 
										   rank, *(this->comm));								   
			CHECK_ERROR_CODE(status)
			if(status != cupcfd::error::E_SUCCESS) {
				return status;
			}
			
			// === Graph Reconstruction ===
			
			if(this->comm->rank == rank) {			
				// (1) We can now reconstruct the graph by adding the nodes to the serial graph
				for(I i = 0; i < nRecvNodes; i++) {
					status = destGraph->addNode(recvNodes[i]);
					CHECK_ERROR_CODE(status)
				}
	
				// (2) Now we can add the edges
				// If the edge already exists (e.g. overlap with ghost nodes),
				// nothing should change.
				for(I i = 0; i < nRecvEdge1; i++) {
					status = destGraph->addEdge(recvEdge1[i], recvEdge2[i]);
					CHECK_ERROR_CODE(status)
				}
				
				free(recvNodes);
				free(recvNodeProcCount);
				free(recvEdge1);
				free(recvEdge2);
				free(recvEdge1ProcCount);
				free(recvEdge2ProcCount);
			}
			
			// === Cleanup ===
			// Free any locally created temporary stores
			free(localNodes);
			free(edgeNode1);
			free(edgeNode2);

			// Done
			return cupcfd::error::E_SUCCESS;
		}
		
		template <class I, class T>
		template <class D>
		cupcfd::error::eCodes DistributedAdjacencyList<I,T>::buildExchangePattern(cupcfd::comm::ExchangePatternTwoSidedNonBlocking<D>** pattern) {
			*pattern = new cupcfd::comm::ExchangePatternTwoSidedNonBlocking<D>();

			// Items needed to initialise the exchange pattern
			// (a) Communicator (taken from graph)
			// (b) Mapping of Local to Exchange IDs - this will be the global ID mapping for any local or ghost cells
			// store on this process.
			// This is important since the mapping order must be the same as for any data arrays, (i.e. cell '5' has
			// to be in the same place in this mapping and in the data array) so it needs to be consistent.
			// (c) Number of elements in mapping array - see above
			// (d)/(e)Pairwise linked array that contain a data element to send, by global id, and the process it is going to
			// This is a pair of a ghost nodes global id and the process that owns that ghost node locally.

			// Mapping
			I nMapLocalToExchangeIDX = this->connGraph.nNodes;
			I * mapLocalToExchangeIDX = (I *) malloc(sizeof(I) * nMapLocalToExchangeIDX);

			for(I i = 0; i < nMapLocalToExchangeIDX; i++) {
				I localID = i;
				I globalID = this->nodeToGlobal[this->connGraph.IDXToNode[localID]];
				mapLocalToExchangeIDX[i] = globalID;
			}

			// Compile arrays of the data elements we are going to send, and to which processes.
			// This is basically the list of ghost cells and their target processes

			// ToDo: For now we can reuse the send/recv CSRs that the adjacency list builds
			// May wish to move that data/computation out to this drivers, since it is only needed for
			// exchange patterns, so it could be wrapped into an exchange pattern object in graph instead.

			I nTRanks = this->sendGlobalIDsAdjncy.size();
			I * tRanks = (I *) malloc(sizeof(I) * nTRanks);

			I size = cupcfd::utility::drivers::safeConvertSizeT<I>(this->sendGlobalIDsXAdj.size());
			for(I i = 0; i < size; i++) {
				for(I j = 0; j < this->sendGlobalIDsXAdj[i+1] - this->sendGlobalIDsXAdj[i]; j++) {
					tRanks[this->sendGlobalIDsXAdj[i] + j] = this->sendRank[i];
				}
			}


			(*pattern)->init(*(this->comm),
							 mapLocalToExchangeIDX, nMapLocalToExchangeIDX,
							 &(this->sendGlobalIDsAdjncy[0]), this->sendGlobalIDsAdjncy.size(),
							 tRanks, nTRanks);

			free(mapLocalToExchangeIDX);
			free(tRanks);

			return cupcfd::error::E_SUCCESS;
		}
	}
}

#endif
