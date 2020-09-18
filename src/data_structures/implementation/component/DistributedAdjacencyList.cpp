/*
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Contains definitions for the DistributedAdjacencyList Class.
 */

#include "DistributedAdjacencyList.h"
#include <iostream>

#include "Broadcast.h"
#include "Barrier.h"
#include "CommError.h"
#include "Reduce.h"
#include "Gather.h"

#include "ArrayDrivers.h"
#include "SortDrivers.h"
#include "AdjacencyListCSR.h"

#include <iostream>

namespace cupcfd
{
	namespace data_structures
	{
		template <class I, class T>
		DistributedAdjacencyList<I, T>::DistributedAdjacencyList(cupcfd::comm::Communicator& comm)
		{
			// std::cout << "DistributedAdjacencyList CONSTRUCTOR CALLED" << std::endl;

			this->comm = comm.clone();

			this->processNodeCounts = nullptr;
			cupcfd::error::eCodes status = this->reset();
			if (status != cupcfd::error::E_SUCCESS)
			{
				// std::cout << "DistributedAdjacencyList<I, T>::reset() FAILED" << std::endl;
				int ierr = -1;
				MPI_Abort(MPI_COMM_WORLD, ierr);
			} else {
				// std::cout << "DistributedAdjacencyList<I, T>::reset() SUCCESS" << std::endl;
			}

			// ToDo: Should put a barrier in here to ensure that every participating process has created the graph.

			// ToDo: Should distribute a unique random graph identifier to establish whether the same graph
			// object is in use across processes.

			// std::cout << "DistributedAdjacencyList CONSTRUCTOR COMPLETE" << std::endl;
		}

		template <class I, class T>
		DistributedAdjacencyList<I, T>::~DistributedAdjacencyList()
		{
			if(this->processNodeCounts != nullptr)
			{
				free(this->processNodeCounts);
			}
			this->processNodeCounts = nullptr;

			// Vectors will cleanup themselves

			// Clear up dynamically allocated comm
			delete this->comm;
		}

		template <class I, class T>
		cupcfd::error::eCodes DistributedAdjacencyList<I, T>::reset()
		{
			// std::cout << "DistributedAdjacencyList<I, T>::reset() CALLED" << std::endl;

			// Clear Data - Note we do not reset the MPI Communicator.
			// A new object should be created if using a new communicator.

			this->buildGraph = cupcfd::data_structures::AdjacencyListVector<I, T>();
			this->connGraph = cupcfd::data_structures::AdjacencyListCSR<I, T>();

			if(this->processNodeCounts != nullptr)
			{
				free(this->processNodeCounts);
				this->processNodeCounts = nullptr;
			}

			// Reset Variables
			this->nLONodes = 0;
			this->nLGhNodes = 0;
			this->nLEdges = 0;

			this->nGNodes = 0;
			this->nGGhNodes = 0;
			this->nGEdges = 0;

			this->finalized = false;

			this->nodeDistType = std::map<T, nodeType>();
			this->nodeOwner = std::map<T, I>();

			// ToDo: Should some form of blocking barrier here be placed here to enforce consistency across processes?

			// std::cout << "DistributedAdjacencyList<I, T>::reset() COMPLETE" << std::endl;

			return cupcfd::error::eCodes::E_SUCCESS;
		}

		template <class I, class T>
		cupcfd::error::eCodes DistributedAdjacencyList<I, T>::operator=(DistributedAdjacencyList<I, T>& source)
		{
			// Deep copy the contents from source to destination.
			// This involves copying
			// (a) Top level variables
			// (b) Any adjacencyCSR graphs (build, conn)
			// (c) Any vectors
			// (d) Any Maps
			// (e) Any Arrays

			// ToDo: Error Check: They must not be the same object

			// Reset the destination graph
			this->reset();

			// === Variables ===
			this->nLONodes = source.nLONodes;
			this->nLGhNodes = source.nLGhNodes;
			this->nLEdges = source.nLEdges;
			this->nGNodes = source.nGNodes;
			this->nGGhNodes = source.nGGhNodes;
			this->nGEdges = source.nGEdges;
			this->finalized = source.finalized;

			// Copy the data, not the pointer)
			*(this->comm) = *(source.comm);

			// This will trigger a deep copy
			this->buildGraph = source.buildGraph;

			// ToDo: Danger here of not being a deep copy if Type T is defined to be a pointer!
			this->nodeDistType = source.nodeDistType;
			this->nodeOwner = source.nodeOwner;

			this->neighbourRanks = source.neighbourRanks;

			this->sendRank = source.sendRank;
			this->sendGlobalIDsXAdj = source.sendGlobalIDsXAdj;
			this->sendGlobalIDsAdjncy = source.sendGlobalIDsAdjncy;

			this->recvRank = source.recvRank;
			this->recvGlobalIDsXAdj = source.recvGlobalIDsXAdj;
			this->recvGlobalIDsAdjncy = source.recvGlobalIDsAdjncy;

			this->nodeToGlobal = source.nodeToGlobal;
			this->globalToNode = source.globalToNode;

			this->processNodeCounts = (I *) malloc(sizeof(I) * this->comm->size);
			cupcfd::utility::drivers::copy(source.processNodeCounts, source.comm->size, this->processNodeCounts, this->comm->size);

			return cupcfd::error::E_SUCCESS;
		}

		template <class I, class T>
		cupcfd::error::eCodes DistributedAdjacencyList<I, T>::existsLocalNode( T node, bool * found)
		{
			*found = false;

			auto find = this->nodeDistType.find(node);
			if(find == this->nodeDistType.end())
			{
				// Did not find the node, but function ran to completion.
				return cupcfd::error::E_SUCCESS;
			}
			else
			{
				if(find->second == cupcfd::data_structures::LOCAL)
				{
					*found = true;
				}
			}

			return cupcfd::error::E_SUCCESS;
		}

		template <class I, class T>
		cupcfd::error::eCodes DistributedAdjacencyList<I, T>::existsGhostNode(T node, bool * found)
		{
			*found = false;

			auto find = this->nodeDistType.find(node);
			if(find == this->nodeDistType.end())
			{
				// Did not find the node, but function ran to completion.
				return cupcfd::error::E_SUCCESS;
			}
			else
			{
				if(find->second == cupcfd::data_structures::GHOST)
				{
					*found = true;
				}
			}

			return cupcfd::error::E_SUCCESS;
		}

		template <class I, class T>
		cupcfd::error::eCodes DistributedAdjacencyList<I, T>::existsNode(T node, bool * found)
		{
			*found = false;
			this->existsLocalNode(node, found);

			if(*found == true)
			{
				// Node has been found so we can stop here.
				return cupcfd::error::E_SUCCESS;
			}

			this->existsGhostNode(node, found);

			// There are no more types to search through, so 'found' is the final correct value, whatever that might be.
			return cupcfd::error::E_SUCCESS;
		}

		template <class I, class T>
		cupcfd::error::eCodes DistributedAdjacencyList<I, T>::addLocalNode(T node)
		{
			// Check node does not exist already as a local node.
			bool found;

			// Check node does not exist already.
			this->existsNode(node, &found);

			if(found == true)
			{
				return cupcfd::error::E_ADJACENCY_LIST_NODE_EXISTS;
			}

			// If it does not exist as either, add it as a local node
			this->nodeDistType[node] = LOCAL;

			// Store the node in the local adjacency_list structure
			this->buildGraph.addNode(node);

			// Increment node counter for local nodes
			this->nLONodes++;

			return cupcfd::error::E_SUCCESS;
		}

		template <class I, class T>
		cupcfd::error::eCodes DistributedAdjacencyList<I, T>::addGhostNode(T node)
		{
			// Check node does not exist already as a local node.
			bool found;

			// Check node does not exist already.
			this->existsNode(node, &found);

			if(found == true)
			{
				return cupcfd::error::E_ADJACENCY_LIST_NODE_EXISTS;
			}

			// If it does not exist as either, add it as a local node
			this->nodeDistType[node] = GHOST;

			// Store the node in the local adjacency_list structure
			this->buildGraph.addNode(node);

			// Increment node counter for ghost nodes
			this->nLGhNodes++;

			return cupcfd::error::E_SUCCESS;
		}

		template <class I, class T>
		cupcfd::error::eCodes DistributedAdjacencyList<I, T>::addNode(T node)
		{
			// Since the node type is unspecified, we default to adding it as a ghost node.
			return this->addGhostNode(node);
		}

		template <class I, class T>
		cupcfd::error::eCodes DistributedAdjacencyList<I, T>::existsEdge(T src, T dst, bool * found)
		{
			bool srcExists = false;
			bool dstExists = false;

			// ToDo: Is it safe to use buildgraph here???
			// This might be cleared after finalize!
			// Check that the source node exists
			this->buildGraph.existsNode(src, &srcExists);
			if(!srcExists)
			{
				return cupcfd::error::E_ADJACENCY_LIST_NODE_MISSING;
			}

			// Check that the destination node exists
			this->buildGraph.existsNode(dst, &dstExists);
			if(!dstExists)
			{
				return cupcfd::error::E_ADJACENCY_LIST_NODE_MISSING;
			}

			// Both nodes exist
			// Pass through the task of checking to the underlying graph structure
			//ToDo: Once return type of adjacency lists are fixed, return the err code.
			this->buildGraph.existsEdge(src, dst, found);

			return cupcfd::error::E_SUCCESS;
		}

		template <class I, class T>
		cupcfd::error::eCodes DistributedAdjacencyList<I, T>::addEdge(T src, T dst)
		{
			cupcfd::error::eCodes status;
			bool srcExists;
			bool dstExists;
			bool edgeExists;

			// Behaviour for this function is to add a node as a ghost node if it is missing, rather
			// than to return an error code
			this->existsNode(src, &srcExists);
			this->existsNode(dst, &dstExists);

			// ToDo: Currently add node as ghost node if missing, but is an error code preferable?
			if(!srcExists)
			{
				//return E_ADJACENCY_LIST_NODE_MISSING;
				this->addGhostNode(src);
			}

			if(!dstExists)
			{
				//return E_ADJACENCY_LIST_NODE_MISSING;
				this->addGhostNode(dst);
			}

			// Error Check built into addEdge
			// If edge exists it will return the status code
			status = this->buildGraph.addEdge(src, dst);

			if(status != cupcfd::error::E_SUCCESS)
			{
				return status;
			}

			return cupcfd::error::E_SUCCESS;
		}

		template <class I, class T>
		cupcfd::error::eCodes DistributedAdjacencyList<I, T>::addUndirectedEdge(T src, T dst)
		{
			cupcfd::error::eCodes err;

			err = this->addEdge(src, dst);
			if(err != cupcfd::error::E_SUCCESS)
			{
				return err;
			}

			err = this->addEdge(dst, src);
			if(err != cupcfd::error::E_SUCCESS)
			{
				return err;
			}

			return cupcfd::error::E_SUCCESS;
		}

		template <class I, class T>
		cupcfd::error::eCodes DistributedAdjacencyList<I, T>::finalize()
		{
			std::cout << "DistributedAdjacencyList::finalize() CALLED" << std::endl;

			cupcfd::error::eCodes err;

			// =========================================================================================================
			// (1) Setup a barrier to ensure that every process registered as involved with the graph (comm) has
			//     called this finalize step.
			// =========================================================================================================
			err = cupcfd::comm::Barrier(*(this->comm));

			// =========================================================================================================
			// (2) Distribute the graph unique identifier to ensure everyone is modifying the same graph
			// =========================================================================================================

			// =========================================================================================================
			// (3) Update the global stats component of the graph
			// =========================================================================================================

			// (3a) Allreduce on number of local ghost cells
			cupcfd::comm::allReduceAdd(&(this->nLGhNodes), 1, &(this->nGGhNodes), 1, *(this->comm));

			// (3b) Allreduce on number of owned cells to total number of owned cells (assuming everything is correct)
			cupcfd::comm::allReduceAdd(&(this->nLONodes), 1, &(this->nGNodes), 1, *(this->comm));

			// =========================================================================================================
			// (4) Verification/Data Gathering Stage
			// =========================================================================================================

			// (a) Every cell registered as a ghost cell is owned by a process
			// (b) Every cell registered as a locally owned cell is owned by exactly one process
			// (c) For each ghost cell a process has, it needs to store which process owns that cell

			// ===== Build an array of ghost cell global identifiers on this process =====
			// Allocate space based on local ghost node count.
			T * ghostNodes = (T *) malloc(sizeof(T) * this->nLGhNodes);

			// Populate the array
			this->getGhostNodes(ghostNodes, this->nLGhNodes);

			// ===== Build an array of local owned cell global identifiers on this process =====
			// Allocate space based on local owned node count.
			T * localNodes = (T *) malloc(sizeof(T) * this->nLONodes);

			// Populate the array
			this->getLocalNodes(localNodes, this->nLONodes);

			// A global indexing system needs to built that associates a global id with a node, but in such a way that any
			// process can determine who they need to communicate with based on the global id
			// One such way is to assign sequential global ids to each node within a process, such that process 1 starts with
			// an index one higher than the process before etc.
			// To do this, we can apply a similar principle to CSR data structures to determine the global ID, but only if
			// each process knows how many nodes each other process has (so it can be computed locally).
			// ToDo: Memory scalability issues with more processes?

			// Essentially an all-to-all process.
			// ToDo: Implement using an all-to-all. Since interface for all-to-all is not in place, quickly implement using
			// a gather/broadcast (will be less efficient since this is all-to-one and then one-to-all

			// this will only be freed on reset/deconstructor
			this->processNodeCounts = (I *) malloc(sizeof(I) * this->comm->size);
			err = cupcfd::comm::Gather(&(this->nLONodes), 1, this->processNodeCounts, this->comm->size, 1, 0, *(this->comm));
			err = cupcfd::comm::Broadcast(this->processNodeCounts, this->comm->size, 0, *(this->comm));

			// Assign global ids to this processes' nodes. Done on a sequential basis across processes
			// Assumes ranks will always start at 0 and be sequential in number
			I base = 0;

			for(I i = 0; i < this->comm->rank; i++)
			{
				// Sum the number of nodes up to process rank - 1
				base = base + this->processNodeCounts[i];
			}

			// === Assign locally owned graph nodes a global id ===
			// The base starting index is dependant on how many nodes came before us in the process ranking
			// I.e. we keep counting sequential across the processes in order.
			// E.g.
			// Process:    0      1      2      3
			// Node Count  10     12     14     10
			// Range	   0-9    10-21  22-35  36-45
			// Ghost node global ids will be assigned by the rank that owns them, and we must retrieve this later.
			// Note that this will assign ids in sorted order due to the use of the getLocalNodes returned array.

			// Store the Global Index Range in the graph
			this->globalOwnedRangeMin = base;
			this->globalOwnedRangeMax = base + this->nLONodes - 1;

			// Perhaps it should do this by order of nodes in adjacency_list to improve cache behaviour if looping over them later.
			for(I i = 0; i < this->nLONodes; i++)
			{
				this->nodeToGlobal[localNodes[i]] = base + i;
				this->globalToNode[base + i] = localNodes[i];
			}

			// Technically we want an all-to-allv but for now we will take this approach
			// ToDo: Replace serialised scatters with all-to-allv so MPI can potentially optimise

			I sendAdjncyPtr = 0;


			for(I i = 0; i < this->comm->size; i++)
			{
				// ToDo: Should probably be a barrier here

				// Serialised Scatter - i.e. each process takes it in turn to
				// (a) Notify the processes of what ghost cells it has
				// (b) Ask which/any ghost cells the remote processes own as local cells
				// (c) Store the response from each process
				// (d) Count to verify that exactly one process has claimed ownership of each and every ghost cell on this process

				// ToDo: Ideally we would like to rely on communicating global indexes, rather than nodes (since a node could be quite a large object depending
				//		 on future extensions to the template). However, we build the graph in the context of nodes. At this stage while we know
				//		 the global ids of the locally owned nodes, we do not know what global ids were assigned to ghost nodes (since this was done on an unknown remote process,
				//		 we do not yet know its owner)


				// (a) Broadcast the ghost cell data from process i in the serial loop to all other processes so they know what cells we're looking for information on.
				T * recvGhostData;
				I nRecvGhostData;
				cupcfd::error::eCodes err = cupcfd::comm::Broadcast(ghostNodes, this->nLGhNodes, &recvGhostData, &nRecvGhostData, i, *(this->comm));
				if(err != cupcfd::error::E_SUCCESS)
				{
					return err;
				}

				// (b)/(c) Identify which cells in the ghost cell list are in the local cell list via a set intersect (uses equality on nodes, precaution if extended in future
				//		   with custom operator)
				T * intersect;
				I nIntersect;
				cupcfd::utility::drivers::intersectArray(localNodes, this->nLONodes, recvGhostData, nRecvGhostData, &intersect, &nIntersect);

				// (b)/(c) Gather the results from each process - i.e. the cells that each process has identified from the ghost cell list as ones they 'own'
				// ToDo: This could possibly be made more efficient by only communicating bits (1/0) for if a cell is owned - but then each process would have to communicate
				// a bit for every cell rather than just the ones they own.

				// Storage for gather data - will be created by gather driver (but only on sink process, should be null on others).
				// Must be initialised to NULL or nullptr for the driver
				T * ownership = nullptr;
				I nOwnership = 0;

				T * countOwned = nullptr;
				I nCountOwned = 0;

				// Communicate the nodes which we own that have been requested as a ghost node by the root process
				cupcfd::comm::GatherV(intersect, nIntersect, &ownership, &nOwnership, &countOwned, &nCountOwned, i, *(this->comm));

				// Also communicate the global IDs for the nodes that we own that have been requested as a ghost node by the root process
				// Build array of globalIDs
				I * intersectGID = (I *) malloc(sizeof(I) * nIntersect);	// Each process has some ghost global id data to send
				I * ghostGID = (I *) malloc(sizeof(I) * this->nLGhNodes);	// Need storage on process i to store ghost node global ids from all process
																				// Could also use size of ownership?

				for(I j = 0; j < nIntersect; j++)
				{
					intersectGID[j] = this->nodeToGlobal[intersect[j]];
				}

				// Can reuse countOwned, since the values should be the same as identified in the previous gatherV.
				cupcfd::comm::GatherV(intersectGID, nIntersect, ghostGID, this->nLGhNodes, countOwned, nCountOwned, i, *(this->comm));

				// While we're here, make a note of the globalIDs requested from this process for the sink process i.
				// We'll need this information later to send data during exchanges
				// Since we loop over the ranks in order, this should also means the ranks will be stored in a sorted fashion.

				if(nIntersect > 0 && this->comm->rank != i)
				{
					this->sendRank.push_back(i);
					this->sendGlobalIDsXAdj.push_back(sendAdjncyPtr);

					for(I j = 0; j < nIntersect; j++)
					{
						this->sendGlobalIDsAdjncy.push_back(intersectGID[j]);
						sendAdjncyPtr = sendAdjncyPtr + 1;
					}
				}

				// Now the sink/root process should have data of which processes claim ownership over it's ghost nodes, grouped by process (regions indexed by nNodesOwned)
				// Update local information to store this
				// ToDo: Should this be in a kernel of some sort
				if(this->comm->rank == i)
				{
					// Error Checks
					// Check 1: Number of ghost cells stored locally should be same as total sum of ghost cells claimed by other processes
					if(nOwnership != this->nLGhNodes)
					{
						// Error
						//std::cout << "Error: On Rank " << this->comm.rank << "\n";
						//std::cout << "Error: Rank " << i << ", Mismatch in local ghost cells and remotely claimed cells\n";
						//std::cout << "This rank has " << this->nLGhNodes << " registered ghost cells,";
						//std::cout << "the number of ghost cells on this rank claimed by other processes is " << nOwnership << "\n";
						return cupcfd::error::E_ADJACENCY_LIST_NODE_CLAIM_MISMATCH;
					}

					// Check 2: Every ghost cell should be claimed by another process only once - cannot have two processes both claiming to own
					// a local cell
					// ToDo: This error check requires a count of each node id - i.e. group + count - distinctArray can do this.

					I * processID = (I *) malloc(sizeof(I) * nOwnership);

					I ptr = 0;
					// Loop over regions
					for(I k = 0; k < nCountOwned; k++)
					{
						// Loop over number of elements claimed by this region/process
						for(I j = 0; j < countOwned[k]; j++)
						{
							processID[ptr] = k;
							ptr = ptr + 1;
						}
					}

					if(ptr != nOwnership)
					{
						// Error - fewer process ids were assigned that reported number of claimed nodes
						// ToDo: Suitable Error Code
						return cupcfd::error::E_ERROR;
					}

					// Can now pair claimed nodes with the process that claims them
					// Update the local map to store this information

					std::vector<I> neighbourRanksTmp;

					for(I j = 0; j < nOwnership; j++)
					{
						this->nodeOwner[ownership[j]] = processID[j];
						neighbourRanksTmp.push_back(processID[j]);
					}

					if(neighbourRanksTmp.size() > 0)
					{
						// Reduce the neighbour list down to distinct ranks
						I * distinctRanks;
						I nDistinctRanks;

						I tmpSize = neighbourRanksTmp.size();

						cupcfd::utility::drivers::distinctArray(&neighbourRanksTmp[0], tmpSize, &distinctRanks, &nDistinctRanks);

						for(I k = 0; k < nDistinctRanks; k++)
						{
							this->neighbourRanks.push_back(distinctRanks[k]);
						}

						free(distinctRanks);

						// Sort the rank order
						cupcfd::utility::drivers::merge_sort(&(this->neighbourRanks[0]), this->neighbourRanks.size());
					}

					// Can also assigned global IDs
					for(I j = 0; j < nOwnership; j++)
					{
						this->nodeToGlobal[ownership[j]] = ghostGID[j];
						this->globalToNode[ghostGID[j]] = ownership[j];
					}

					// Cleanup
					free(processID);
				}



				// The broadcast driver will always allocate these, even on root
				free(recvGhostData);
				free(intersect);
				free(intersectGID);
				free(ghostGID);

				// These are only allocated by the gather driver on the sink process
				if(ownership != NULL && ownership != nullptr)
				{
					free(ownership);
				}

				if(countOwned != NULL && countOwned != nullptr)
				{
					free(countOwned);
				}

				// ToDo: Edge Check: If an edge exists on one process, it should exist on the other process
				// (even if uni-directional, since it should be aware of a neighbouring node)
			}
			this->sendGlobalIDsXAdj.push_back(sendAdjncyPtr);

			// =========================================================================================================
			// Convert the buildGraph to a CSR version (the connGraph) for more efficient lookup
			// since no more nodes should be added.
			// =========================================================================================================
			// Use Deep Copy Operator
			this->connGraph = this->buildGraph;
			std::cout << "DistributedAdjacencyList::finalize() ABORTING EARLY" << std::endl;
			return cupcfd::error::E_ERROR;

			// =========================================================================================================
			// ToDo: Should now be able to reset the buildGraph to free some space.
			// =========================================================================================================

			// Store in CSR format a list of the send/recv global ids for ghost nodes
			// These will be needed for exchanging cell data.

			// Loop over the neighbours, and store the globalIDs of cells to be sent, sorted by global id
			// The nodes that need to be sent are those associated with ghost cells
			// For each ghost cell, build a CSR of any adjacent local nodes

			// === Loop over the neighbours, and store the globalIDs of cells to be received, sorted by global id ===
			// Get the ghost GIDs

			std::cout << "DistributedAdjacencyList::finalize() ABORTING EARLY" << std::endl;
			return cupcfd::error::E_ERROR;
			T * ghosts = (T *) malloc(sizeof(T) * this->nLGhNodes);
			T * ghostsGIDs = (T *) malloc(sizeof(T) * this->nLGhNodes);
			T * ghostrank = (T *) malloc(sizeof(T) * this->nLGhNodes);
			std::cout << "DistributedAdjacencyList::finalize() ABORTING EARLY" << std::endl;
			return cupcfd::error::E_ERROR;
			this->getGhostNodes(ghosts, this->nLGhNodes);


			for(I i = 0; i < this->nLGhNodes; i++)
			{
				ghostsGIDs[i] = this->nodeToGlobal[ghosts[i]];
				ghostrank[i] = this->nodeOwner[ghosts[i]];
			}

			// Now group the global IDs by neighbour rank, by sorting the ranks and then reordering the ghostGIDs
			I * indexes = (I *) malloc(sizeof(I) * this->nLGhNodes);
			cupcfd::utility::drivers::merge_sort_index(ghostrank, this->nLGhNodes, indexes, this->nLGhNodes);
			cupcfd::utility::drivers::destIndexReorder(ghostsGIDs, this->nLGhNodes, indexes, this->nLGhNodes);

			// Loop over neighbour ranks, this should already have been sorted
			I ptr = 0;
			I j = 0;

			for(I i = 0; i < this->neighbourRanks.size(); i++)
			{
				this->recvRank.push_back(this->neighbourRanks[i]);
				this->recvGlobalIDsXAdj.push_back(ptr);

				I rankCellCount = 0;
				while((j < this->nLGhNodes) && (ghostrank[j] == this->neighbourRanks[i]))
				{
					this->recvGlobalIDsAdjncy.push_back(ghostsGIDs[j]);
					rankCellCount = rankCellCount + 1;
					j = j + 1;
				}
				ptr = ptr + rankCellCount;
			}
			this->recvGlobalIDsXAdj.push_back(j);

			// Sort the ghost global ids within their groups
			for(I i = 0; i < this->recvGlobalIDsXAdj.size()-1; i++)
			{
				I start = this->recvGlobalIDsXAdj[i];
				I count = this->recvGlobalIDsXAdj[i+1] - this->recvGlobalIDsXAdj[i];
				cupcfd::utility::drivers::merge_sort(&(this->recvGlobalIDsAdjncy[start]), count);
			}

			// Sort the send global ids within their groups
			for(I i = 0; i < this->sendGlobalIDsXAdj.size()-1; i++)
			{
				I start = this->sendGlobalIDsXAdj[i];
				I count = this->sendGlobalIDsXAdj[i+1] - this->sendGlobalIDsXAdj[i];
				cupcfd::utility::drivers::merge_sort(&(this->sendGlobalIDsAdjncy[start]), count);
			}

			free(ghosts);
			free(ghostsGIDs);
			free(ghostrank);
			free(indexes);

			// =========================================================================================================
			// Cleanup
			// =========================================================================================================
			free(ghostNodes);
			free(localNodes);

			this->finalized = true;

			// Sort the nodes so local nodes have lower local indexes than ghost nodes
			this->sortNodesByLocal();

			std::cout << "DistributedAdjacencyList::finalize() COMPLETE" << std::endl;

			return cupcfd::error::E_SUCCESS;
		}

		template <class I, class T>
		cupcfd::error::eCodes DistributedAdjacencyList<I, T>::sortNodesByLocal()
		{
			// Kernels may wish to only loop over local nodes. We wish to maintain index parity with any data arrays
			// between the locally assigned index for a node, and it's data.
			// As such, we want the graph data to be sorted by local nodes first, else we'd have to check for every
			// node.

			// We can maintain nodeType, nodeOwner, nodeToGlobal and globalToNode as they are, since they may by node.
			// Node to Local Mapping is maintained by the connectivity graph in AdjacencyListCSR.
			// The easiest approach is thus to rebuild a new AdjacencyListCSR in order of local nodes, followed by
			// ghost nodes

			// Error Check: We should only do this on finalized graphs - it's premature to sort before then

			// (a) Get the list of nodes that are local and add each node to the new adjacency list
			// 	   Order them by their globalID - this will make data exchange easier later on, since we will
			//	   know what order to expect node data in.

			cupcfd::error::eCodes status;

			// New List
			cupcfd::data_structures::AdjacencyListVector<I, T> sourceList;

			T * localNodes = (T *) malloc(sizeof(T) * this->nLONodes);
			this->getLocalNodes(localNodes, this->nLONodes);

			for(I i = 0; i < this->nLONodes; i++)
			{
				sourceList.addNode(localNodes[i]);
			}

			// (b) Get the list of nodes that are ghosts and add each node to the new adjacency list
			T * ghostNodes = (T *) malloc(sizeof(T) * this->nLGhNodes);
			this->getGhostNodes(ghostNodes, this->nLGhNodes);

			for(I i = 0; i < this->nLGhNodes; i++)
			{
				sourceList.addNode(ghostNodes[i]);
			}

			// (c) For each node, add the edges that were previously stored. There should be no new nodes at this stage.
			for(I i = 0; i < this->nLONodes; i++)
			{
				I count;
				this->connGraph.getAdjacentNodeCount(localNodes[i], &count);

				T * adjNodes = (T *) malloc(sizeof(T) * count);

				this->connGraph.getAdjacentNodes(localNodes[i], adjNodes, count);

				for(I j = 0; j < count; j++)
				{
					// No need to add in both directions - if it exists in both directions it will be added when processing
					// the adjacent node.
					status = sourceList.addEdge(localNodes[i], adjNodes[j]);

					if(status != cupcfd::error::E_SUCCESS)
					{
						return status;
					}
				}
				free(adjNodes);
			}

			for(I i = 0; i < this->nLGhNodes; i++)
			{
				I count;
				this->connGraph.getAdjacentNodeCount(ghostNodes[i], &count);

				T * adjNodes = (T *) malloc(sizeof(T) * count);

				this->connGraph.getAdjacentNodes(ghostNodes[i], adjNodes, count);

				for(I j = 0; j < count; j++)
				{
					// No need to add in both directions - if it exists in both directions it will be added when processing
					// the adjacent node.
					status = sourceList.addEdge(ghostNodes[i], adjNodes[j]);

					if(status != cupcfd::error::E_SUCCESS)
					{
						return status;
					}
				}
				free(adjNodes);
			}

			// (d) Rebuild complete - use a deep copy to reset the list and copy the data across
			this->connGraph = sourceList;

			// Cleanup
			free(ghostNodes);
			free(localNodes);

			return cupcfd::error::E_SUCCESS;
		}

		template <class I, class T>
		cupcfd::error::eCodes DistributedAdjacencyList<I, T>::getGhostNodes(T * nodes, I nNodes)
		{
			auto it = this->nodeDistType.begin();
			I ptr = 0;

			while(it != this->nodeDistType.end())
			{
				if(it->second == GHOST)
				{
					nodes[ptr] = it->first;
					ptr++;
				}
				it++;
			}

			return cupcfd::error::E_SUCCESS;
		}

		template <class I, class T>
		cupcfd::error::eCodes DistributedAdjacencyList<I, T>::getLocalNodes(T * nodes, I nNodes)
		{
			auto it = this->nodeDistType.begin();
			I ptr = 0;

			while(it != this->nodeDistType.end())
			{
				if(it->second == LOCAL)
				{
					nodes[ptr] = it->first;
					ptr++;
				}
				it++;
			}

			return cupcfd::error::E_SUCCESS;
		}
	} // namespace data_structures
} // namespace cupcfd

// Explicit Instantiation
template class cupcfd::data_structures::DistributedAdjacencyList<int, int>;
