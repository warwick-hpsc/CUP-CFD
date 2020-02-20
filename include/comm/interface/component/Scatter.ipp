/*
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Header Level Implementations for Scatter Interface Operations
 *
 */

#ifndef INCLUDE_CUPCFD_SCATTER_IPP_H
#define INCLUDE_CUPCFD_SCATTER_IPP_H

#include <cstdlib>

#include "ScatterMPI.h"

#include "SortDrivers.h"
#include "ArrayDrivers.h"

namespace cupcfd
{
	namespace comm
	{
		template <class T>
		cupcfd::error::eCodes Scatter(T * bSend, int nEleBSend, T * bRecv, int nEleBRecv, int chunkSize, cupcfd::comm::Communicator& mpComm, int sProcess)
		{
			cupcfd::error::eCodes status;
			
			// This function is essentially a wrapper for an underlying comms operation - currently this is MPI only.
			
			// ToDo: Error Checks

			// Since this function uses a fixed chunk size, we can opt for Scatter over ScatterV.
			status = cupcfd::comm::mpi::ScatterMPI(bSend, chunkSize, bRecv, nEleBRecv, sProcess, mpComm.comm);

			if(status != cupcfd::error::E_SUCCESS)
			{
				return status;
			}

			return cupcfd::error::E_SUCCESS;
		}

		template <class T>
		cupcfd::error::eCodes Scatter(T * bSend, int nEleBSend, T * bRecv, int nEleBRecv, int * chunkSizes, int nEleChunkSizes, cupcfd::comm::Communicator& mpComm, int sProcess)
		{
			cupcfd::error::eCodes status;
					
			// This function is essentially a wrapper for an underlying comms operation - currently this is MPI only.
			
			// ToDo: Error Checks

			status = cupcfd::comm::mpi::ScatterVMPI(bSend, nEleBSend, bRecv, nEleBRecv, chunkSizes, nEleChunkSizes, sProcess, mpComm.comm);

			if(status != cupcfd::error::E_SUCCESS)
			{
				return status;
			}

			return cupcfd::error::E_SUCCESS;
		}
		
		template <class T>
		cupcfd::error::eCodes Scatter(T * bSend, int nEleBSend, T ** bRecv, int * nEleBRecv, int * pSend, int nElePSend, cupcfd::comm::Communicator& mpComm, int sProcess)
		{
			cupcfd::error::eCodes status;
		
			int * sendCount;
			T * bSendCpy;
			int * pSendCpy;
			int * sortIndexes;
			int * groupID;
			int * groupSize;
			int nGroup;
			int nGroupSize;

			// (1) We need to sort/group the send buffer by destination process on the root process
			if(mpComm.rank == sProcess)
			{
				// First we can make copies of bSend and pSend so it is non-destructive
				bSendCpy = (T *) malloc(sizeof(T) * nEleBSend);
				pSendCpy = (int *) malloc(sizeof(int) * nElePSend);
				sortIndexes = (int *) malloc(sizeof(int) * nElePSend);

				cupcfd::utility::drivers::copy(bSend, nEleBSend, bSendCpy, nEleBSend);
				cupcfd::utility::drivers::copy(pSend, nElePSend, pSendCpy, nElePSend);

				// First, we need to group the process ids, so let's sort them and keep a copy of their original indexes.
				cupcfd::utility::drivers::merge_sort_index(pSend, nElePSend, sortIndexes, nElePSend);

				// Now we have a sorted order by original index, let us reorder bSendCpy to that order (pSendCpy already sorted by now)
				cupcfd::utility::drivers::sourceIndexReorder(bSendCpy, nEleBSend, sortIndexes, nElePSend);

				// Now we have a copy of the send buffer and the processes each index is going to, grouped by sorted process id.
				// Compute the size of each of these groups.
				// This driver will also allocate the pointer space for us (but we must free it in this function to avoid leaks)
				cupcfd::utility::drivers::distinctArray(pSendCpy, nElePSend, &groupID, &nGroup, &groupSize, &nGroupSize);

				// Now we need to build an array of how many elements are to be sent to each process. Not all processes are
				// guaranteed to be in the pSend list, so we need to diff the arrays to find out which are missing (and set
				// their receive count to 0).
				sendCount = (int *) malloc(sizeof(int) * mpComm.size);

				// Set process IDs and initial counts for full set of processes in communicator.
				for(int i = 0; i < mpComm.size; i++)
				{
					sendCount[i] = 0;
				}

				// For those process IDs that were listed, set their counts to what was previously calculated.
				// Those that were not in pSend will remain at the default of 0.
				for(int i = 0; i < nGroup; i++)
				{
					sendCount[groupID[i]] = groupSize[i];
				}
			}

			// (2) By this point, we should know the expected number of elements for each process in the communicator.
			//     We need to distribute these so each process can construct their receive buffers.
			//     We can use a scatter with a fixed-size chunk for this.
			//     Note: some of these pointers will be null for non-root processes, but this should be ok, since only root
			//     needs them.
			status = Scatter(sendCount, mpComm.size, nEleBRecv, 1, 1, mpComm, sProcess);

			if(status != cupcfd::error::E_SUCCESS)
			{
				return status;
			}
			
			// (3) Now that each process knows their chunk size, we can allocate space for the recv buffer.
			*bRecv = (T *) malloc(sizeof(T) * *nEleBRecv);

			// (4) We should now have everything we need for a variable-sized scatter across all processes in the communicator.
			//     We must scatter the grouped/sorted versions of the data buffer, and use the sendCount array that applies for all processes.
			//     Again some of these may be null pointers for non-root processes
			status = Scatter(bSendCpy, nEleBSend, *bRecv, *nEleBRecv, sendCount, mpComm.size, mpComm, sProcess);

			if(status != cupcfd::error::E_SUCCESS)
			{
				return status;
			}
			
			// (5) Cleanup. Note we do not free the receive buffer here, the pointer to it is passed back as part of this function since
			//     it contains the result.
			//	   Only the process doing the sending need do cleanup.
			if(mpComm.rank == sProcess)
			{
				free(bSendCpy);
				free(pSendCpy);
				free(sortIndexes);
				free(sendCount);
				free(groupID);
				free(groupSize);
			}

			return cupcfd::error::E_SUCCESS;
		}
	} // namespace mpi
} // namespace cupcfd

#endif
