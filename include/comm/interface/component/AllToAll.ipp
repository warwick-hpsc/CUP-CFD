/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Header Level Definitions for All To All Interface Operations
 */

#ifndef CUPCFD_COMM_ALLTOALL_IPP_H
#define CUPCFD_COMM_ALLTOALL_IPP_H

#include <cstdlib>
#include "AllToAllMPI.h"
#include "StatisticsDrivers.h"

#include "ArrayDrivers.h"
#include "SortDrivers.h"

namespace cupcfd
{
	namespace comm
	{
		template <class T>
		cupcfd::error::eCodes AllToAll(T * sendBuffer, int nSendBuffer,
										   T * recvBuffer, int nRecvBuffer,
										   int chunkSize,
										   cupcfd::comm::Communicator& mpComm)
		{
			#ifdef DEBUG
				// Error Check 1: Is the send buffer a suitable size?
				if(nSendBuffer < (mpComm.size * chunkSize)) {
					DEBUGGABLE_ERROR; return cupcfd::error::E_ARRAY_SIZE_UNDERSIZED;
				}

				// Error Check 2: Is the recv buffer a suitable size?
				if(nRecvBuffer < (mpComm.size * chunkSize)) {
					DEBUGGABLE_ERROR; return cupcfd::error::E_ARRAY_SIZE_UNDERSIZED;
				}

				// Error Check 3: Comm Size is at least 1
				if (mpComm.size <= 1) {
					DEBUGGABLE_ERROR; return cupcfd::error::E_MPI_ERR;
				}
			#endif

			// Process with a suitable communication call
			return cupcfd::comm::mpi::AllToAllMPI(sendBuffer, chunkSize, recvBuffer, chunkSize, mpComm.comm);
		}

		template <class T>
		cupcfd::error::eCodes AllToAll(T * sendBuffer, int nSendBuffer, int * sendCounts, int nSendCounts,
											T * recvBuffer, int nRecvBuffer, int * recvCounts, int nRecvCounts,
											cupcfd::comm::Communicator& mpComm)
		{
			// === Pass through to suitable communicator library ===

			#ifdef DEBUG
				// Error Checks
				// Error Check 1: Communicator is at least of size 1
				if (mpComm.size <= 1) {
					DEBUGGABLE_ERROR; return cupcfd::error::E_MPI_ERR;
				}
				if (nSendCounts != mpComm.size) {
					DEBUGGABLE_ERROR; return cupcfd::error::E_ARRAY_SIZE_MISMATCH;
				}
				if (nRecvCounts != mpComm.size) {
					DEBUGGABLE_ERROR; return cupcfd::error::E_ARRAY_SIZE_MISMATCH;
				}

				// Error Check 2: Send Buffer is of a suitable size
				int nSendActual = 0;
				for (int i=0; i<mpComm.size; i++) {
					nSendActual += sendCounts[i];
				}
				if (nSendActual != nSendBuffer) {
					DEBUGGABLE_ERROR; return cupcfd::error::E_ARRAY_SIZE_MISMATCH;
				}

				// Error Check 3: Recv Buffer is of a suitable size
				int nRecvActual = 0;
				for (int i=0; i<mpComm.size; i++) {
					nRecvActual += recvCounts[i];
				}
				if (nRecvActual != nRecvBuffer) {
					DEBUGGABLE_ERROR; return cupcfd::error::E_ARRAY_SIZE_MISMATCH;
				}
			#endif

			// Use MPI communicator library
			// ToDo: Displacements are computed in the MPI wrapper function, should it be moved up a level to here?
			return cupcfd::comm::mpi::AllToAllVMPI(sendBuffer, sendCounts, recvBuffer, recvCounts, mpComm.comm);
		}

		template <class T>
		cupcfd::error::eCodes AllToAll(T * sendBuffer, int nSendBuffer,
											int * sendCounts, int nSendCounts,
											int * sDispls, int nSDispls,
											T * recvBuffer, int nRecvBuffer,
											int * recvCounts, int nRecvCounts,
											int * rDispls, int nRDispls,
											cupcfd::comm::Communicator& mpComm)
		{
			// === Pass through to suitable communicator library ===

			#ifdef DEBUG
				// Error Checks
				// Error Check 1: Communicator is at least of size 1
				if (mpComm.size <= 1) {
					DEBUGGABLE_ERROR; return cupcfd::error::E_MPI_ERR;
				}
				if (nSendCounts != mpComm.size) {
					DEBUGGABLE_ERROR; return cupcfd::error::E_ARRAY_SIZE_MISMATCH;
				}
				if (nRecvCounts != mpComm.size) {
					DEBUGGABLE_ERROR; return cupcfd::error::E_ARRAY_SIZE_MISMATCH;
				}

				// Error Check 2: Send Buffer is of a suitable size
				if (nSendCounts != nSDispls) {
					DEBUGGABLE_ERROR; return cupcfd::error::E_ARRAY_SIZE_MISMATCH;
				}
				int nSendActual = 0;
				for (int i=0; i<mpComm.size; i++) {
					nSendActual += sendCounts[i];
				}
				if (nSendActual != nSendBuffer) {
					DEBUGGABLE_ERROR; return cupcfd::error::E_ARRAY_SIZE_MISMATCH;
				}
				for (int i=0; i<mpComm.size-1; i++) {
					int relativeDisp = sDispls[i+1] - sDispls[i];
					if (relativeDisp != sendCounts[i]) {
						DEBUGGABLE_ERROR; return cupcfd::error::E_ARRAY_SIZE_MISMATCH;
					}
				}

				// Error Check 3: Recv Buffer is of a suitable size
				if (nRecvCounts != nRDispls) {
					DEBUGGABLE_ERROR; return cupcfd::error::E_ARRAY_SIZE_MISMATCH;
				}
				int nRecvActual = 0;
				for (int i=0; i<mpComm.size; i++) {
					nRecvActual += recvCounts[i];
				}
				if (nRecvActual != nRecvBuffer) {
					DEBUGGABLE_ERROR; return cupcfd::error::E_ARRAY_SIZE_MISMATCH;
				}
				for (int i=0; i<mpComm.size-1; i++) {
					int relativeDisp = rDispls[i+1] - rDispls[i];
					if (relativeDisp != recvCounts[i]) {
						DEBUGGABLE_ERROR; return cupcfd::error::E_ARRAY_SIZE_MISMATCH;
					}
				}
			#endif

			// Use MPI communicator library
			return cupcfd::comm::mpi::AllToAllVMPI(sendBuffer, sendCounts, sDispls, recvBuffer, recvCounts, rDispls, mpComm.comm);
		}

		// Note: this method uses a stable sort, thus the order in which elements appear in the
		// sendbuffer is maintained *for the same process target* - i.e. for an array of the form
		// data(process), the following:
		// 3(0), 4(0), 5(0), 6(1), 1(0)
		// will communicate [3,4,5,1] to process 0 and [6] to process 1.
		// This will presumably matter if the index order has additional properties, the elements in the
		// send buffer are grouped by process id, but are not sorted within the send buffer itself.

		template <class T>
		cupcfd::error::eCodes AllToAll(T * sendBuffer, int nSendBuffer,
											int * processIDs, int nProcessIDs,
											T ** recvBuffer, int * nRecvBuffer,
											cupcfd::comm::Communicator& mpComm)
		{
			// === AllToAllV with unknown recv counts ===
			// In this method, we know the data we wish to send, but we do not know how much data we will recieve.
			// Determine the amount of data we will recieve prior to AllToAll, also accounting for factors such as
			// ungrouped data.

			T * localSendBuffer = nullptr;
			int nLocalSendBuffer = 0;
			int * localProcessIDs = nullptr;
			int nLocalProcessIDs = 0;

			int * groupID = nullptr;
			int nGroupID = 0;

			int * groupSize = nullptr;
			// int nGroupSize = 0;

			int * sendCounts = nullptr;
			int nSendCounts = 0;

			int * recvCounts = nullptr;
			int nRecvCounts = 0;

			// (a)  Check whether the processIDs array is already sorted by ascending rank id.
			// 		If not, sort both the sendBuffer and processIDs array such that they are grouped by processID
			//		without loosing the association between a element of data in sendBuffer and it's corresponding
			//		target rank

			// 		ToDo: A very similar stage is used in other comms methods (e.g Scatter) - should consider
			//		moving a driver function to MPIUtility for determining message sizes.

			//		Check whether the processID array is sorted
			bool sorted;
			cupcfd::utility::drivers::is_sorted(processIDs, nProcessIDs, & sorted);

			// 		Use original buffers or sort copies depending on is_sorted outcome
			if(sorted)
			{
				// Set pointers to use the original arrays, since we don't need to modify them
				localSendBuffer = sendBuffer;
				nLocalSendBuffer = nSendBuffer;
				localProcessIDs = processIDs;
				nLocalProcessIDs = nProcessIDs;
			}
			else
			{
				// We need to sort the send and process ID arrays, but we don't want to
				// destructively modify them, so we must make a copy.

				nLocalSendBuffer = nSendBuffer;
				nLocalProcessIDs = nProcessIDs;

				int * sortIndexes = (int *) malloc(sizeof(int) * nLocalProcessIDs);

				localSendBuffer = (T *) malloc(sizeof(T) * nLocalSendBuffer);
				localProcessIDs = (int *) malloc(sizeof(int) * nLocalProcessIDs);
				sortIndexes = (int *) malloc(sizeof(int) * nLocalProcessIDs);

				cupcfd::utility::drivers::copy(sendBuffer, nSendBuffer, localSendBuffer, nLocalSendBuffer);
				cupcfd::utility::drivers::copy(processIDs, nProcessIDs, localProcessIDs, nLocalProcessIDs);

				// Group the process ids,keeping a copy of their original indexes.
				cupcfd::utility::drivers::merge_sort_index(localProcessIDs, nLocalProcessIDs, sortIndexes, nLocalProcessIDs);

				// Now we have a sorted order by original index, let us reorder sendBuffer to that order
				cupcfd::utility::drivers::sourceIndexReorder(localSendBuffer, nLocalSendBuffer, sortIndexes, nLocalProcessIDs);

				free(sortIndexes);
			}

			// (b) Now we have grouped the sendBuffer, determine how much data we are going to send to each process - other processes
			//	   will need this information

			// This driver will allocate memory for the group id/size arrays (but we must free it in this function to avoid leaks)
			cupcfd::utility::drivers::distinctArray(localProcessIDs, nLocalProcessIDs, &groupID, &nGroupID, &groupSize);

			nSendCounts = mpComm.size;
			nRecvCounts = mpComm.size;

			sendCounts = (int *) malloc(sizeof(int) * nSendCounts);
			recvCounts = (int *) malloc(sizeof(int) * nRecvCounts);

			for(int i = 0; i < nSendCounts; i++)
			{
				sendCounts[i] = 0;
			}

			for(int i = 0; i < nRecvCounts; i++)
			{
				recvCounts[i] = 0;
			}

			// For those process IDs that were listed, set their counts to what was previously calculated.
			// Those that were not in pSend will remain at the default of 0.
			for(int i = 0; i < nGroupID; i++)
			{
				sendCounts[groupID[i]] = groupSize[i];
			}

			// (c) Once the amount of received data from each process is known, we can construct the recvBuffer
			//     and update the size of the recvBuffer

			// 	   It is necessary to send out send counts to every process, and receive counts from every process
			//	   We can do this with a fixed-size chunk AllToAll.

			AllToAll(sendCounts, nSendCounts, recvCounts, nRecvCounts, 1, mpComm);

			// Total number of received elements
			cupcfd::utility::drivers::sum(recvCounts, nRecvCounts, nRecvBuffer);

			// Construct the receive buffer
			*recvBuffer = (T *) malloc(sizeof(T) * (*nRecvBuffer));

			// (d) Perform the communication - we will use AllToAllV

			AllToAll(localSendBuffer, nLocalSendBuffer,
					 sendCounts, nSendCounts,
					 *recvBuffer, *nRecvBuffer,
					 recvCounts, nRecvCounts,
					 mpComm);

			// (e) Cleanup
			// Only free pointers if they were allocated memory (i.e. different address to method input buffers)
			if(localSendBuffer != nullptr && localSendBuffer != sendBuffer)
			{
				free(localSendBuffer);
			}

			if(localProcessIDs != nullptr && localProcessIDs != processIDs)
			{
				free(localProcessIDs);
			}

			if(groupID != nullptr)
			{
				free(groupID);
			}

			if(groupSize != nullptr)
			{
				free(groupSize);
			}

			if(sendCounts != nullptr)
			{
				free(sendCounts);
			}

			if(recvCounts != nullptr)
			{
				free(recvCounts);
			}

			return cupcfd::error::E_SUCCESS;
		}
	}
}

#endif
