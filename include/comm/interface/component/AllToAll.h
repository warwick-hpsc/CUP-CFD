/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Declarations for All To All Interface Operations
 */

#ifndef CUPCFD_COMM_ALLTOALL_INCLUDE_H
#define CUPCFD_COMM_ALLTOALL_INCLUDE_H

#include "Communicator.h"
#include "Error.h"

namespace cupcfd
{
	namespace comm
	{
		/*
		 * All-to-all for all processes in mpComm.
		 *
		 * Every process sends chunkSize elements of type T to every other process,
		 * and expects chunkSize elements of type T from each process in turn.
		 *
		 * This is a top-level interface to pass through to the underlying comms operation, as well as performing
		 * various error checking.
		 *
		 * @param sendBuffer The buffer containing the data to be sent to all ranks, including itself.
		 * @param nSendBuffer The size of sendBuffer - should be equal to chunkSize * number of ranks in mpComm
		 * @param recvBuffer The buffer containing all data received from the all-to-all -
		 * @param nRecvBuffer The size of recvBuffer - should be equal to chunkSize * number of ranks in mpComm
		 * @param mpComm The communicator containing the ranks involved in the all-to-all.
		 *
		 * @tparam T The datatype of the data to be communicated. Must be a supported primitive type
		 * or an object that implements CustomMPIType.
		 *
		 * @retval cupcfd::error::E_SUCCESS Operation completed successfully.
		 * @retval cupcfd::error::E_MPI_DATATYPE_UNREGISTERED Datatype T does not have a suitable/registered datatype for communication via MPI.
		 * @retval cupcfd::error::E_MPI_ERR An MPI Error was encountered.
		 * @retval cupcfd::error::E_ARRAY_SIZE_UNDERSIZED One of the required arrays (send/recv) is insufficiently sized.
		 */
		template <class T>
		cupcfd::error::eCodes AllToAll(T * sendBuffer, int nSendBuffer,
										   T * recvBuffer, int nRecvBuffer,
										   int chunkSize,
										   cupcfd::comm::Communicator& mpComm);

		/*
		 * All-to-all for all processes in mpComm.
		 *
		 * Every process sends a variable number of elements of type T to every other process,
		 * and expects a variable number of elements of type T from each process in turn.
		 *
		 * This is a top-level interface to pass through to the underlying comms operation, as well as performing
		 * various error checking.
		 *
		 * This function computes any displacements based on ascending process order for the sendCount/recvCount
		 * sizes.
		 *
		 * @param sendBuffer The buffer containing the data to be sent to all ranks, including itself.
		 * @param nSendBuffer The size of sendBuffer - should be equal to sum(sendCounts)
		 * @param sendCounts An array of the number of elements to be sent to each process.
		 * Data is assigned in order to destination processes from sendBuffer in chunks matching the sizes specified.
		 * @param nSendCounts The size of nSendBounds in the number of elements of type int.
		 * Must be the same size as the number of processes in mpComm.
		 * @param recvBuffer The buffer containing all data received from the all-to-all -
		 * @param nRecvBuffer The size of recvBuffer - should be equal to chunkSize * number of ranks in mpComm
		 * @param recvCounts An array of the number of elements received from each process.
		 * Data is ordered in ascending order of sending process in chunks matching the sizes specified.
		 * @param nRecvCounts The size of recvCounts in the number of elements of type int.
		 * Must be the same size as the number of processes in mpComm.
		 * @param mpComm The communicator containing the ranks involved in the all-to-all.
		 *
		 * @tparam T The datatype of the data to be communicated. Must be a supported primitive type
		 * or an object that implements CustomMPIType.
		 *
		 * @retval cupcfd::error::E_SUCCESS Operation completed successfully.
		 * @retval cupcfd::error::E_MPI_DATATYPE_UNREGISTERED Datatype T does not have a suitable/registered datatype for communication via MPI.
		 * @retval cupcfd::error::E_MPI_ERR An MPI Error was encountered.
		 */
		template <class T>
		cupcfd::error::eCodes AllToAll(T * sendBuffer, int nSendBuffer, int * sendCounts, int nSendCounts,
											T * recvBuffer, int nRecvBuffer, int * recvCounts, int nRecvCounts,
											cupcfd::comm::Communicator& mpComm);

		/*
		 * All-to-all for all processes in mpComm.
		 *
		 * Every process sends a variable number of elements of type T to every other process,
		 * and expects a variable number of elements of type T from each process in turn.
		 *
		 * This is a top-level interface to pass through to the underlying comms operation, as well as performing
		 * various error checking.
		 *
		 * @param sendBuffer The buffer containing the data to be sent to all ranks, including itself.
		 * @param nSendBuffer The size of sendBuffer - should be equal to sum(sendCounts)
		 * @param sendCounts An array of the number of elements to be sent to each process.
		 * Data is assigned in order to destination processes from sendBuffer in chunks matching the sizes specified.
		 * @param nSendCounts The size of nSendBounds in the number of elements of type int.
		 * Must be the same size as the number of processes in mpComm.
		 * @param sDispls An array of displacements, indicating where the data chunk to be sent to a process ID
		 * of the corresponding index in the array begins.
		 * @param nSDispls The size of sDispls in the number of elements of type int.
		 * Must be the same size as the number of processes in mpComm.
		 * @param recvBuffer The buffer containing all data received from the all-to-all -
		 * @param nRecvBuffer The size of recvBuffer - should be equal to chunkSize * number of ranks in mpComm
		 * @param recvCounts An array of the number of elements received from each process.
		 * Data is ordered in ascending order of sending process in chunks matching the sizes specified.
		 * @param nRecvCounts The size of recvCounts in the number of elements of type int.
		 * Must be the same size as the number of processes in mpComm.
		 * @param rDispls An array of displacements, indicating where the data chunk received from a process ID
		 * of the corresponding index in the array begins.
		 * @param nRDispls The size of rDispls in the number of elements of type int.
		 * Must be the same size as the number of processes in mpComm.
		 * @param mpComm The communicator containing the ranks involved in the all-to-all.
		 *
		 * @tparam T The datatype of the data to be communicated. Must be a supported primitive type
		 * or an object that implements CustomMPIType.
		 *
		 * @retval cupcfd::error::E_SUCCESS Operation completed successfully.
		 * @retval cupcfd::error::E_MPI_DATATYPE_UNREGISTERED Datatype T does not have a suitable/registered datatype for communication via MPI.
		 * @retval cupcfd::error::E_MPI_ERR An MPI Error was encountered.
		 */
		template <class T>
		cupcfd::error::eCodes AllToAll(T * sendBuffer, int nSendBuffer,
											int * sendCounts, int nSendCounts,
											int * sDispls, int nSDispls,
											T * recvBuffer, int nRecvBuffer,
											int * recvCounts, int nRecvCounts,
											int * rDispls, int nRDispls,
											cupcfd::comm::Communicator& mpComm);

		/**
		 * All-to-all for all processes in mpComm.
		 *
		 * Every process sends a variable number of elements of type T to every other process,
		 * and expects a variable number of elements of type T from each process in turn.
		 *
		 * This is a top-level interface to pass through to the underlying comms operation, as well as performing
		 * various error checking.
		 *
		 * This variant does not assume that data is grouped by process. Rather, each element in the sendBuffer
		 * has a destination process specified, and this function will group, sort and compute the send/recv
		 * sizes for an all-to-all. This will incur an extra overhead in result - in particuar it must
		 * use communications to tell other processes how much data to expect to receive.
		 * As such, do not use this function is such information is already known by the processes.
		 *
		 * The data does not need to be ordered by destination process. The received data will be
		 * ordered/grouped by processID of the sending process, but within that group should retain the same order
		 * as in that processes sending buffer if more than one element is sent.
		 *
		 * E.g.:
		 *
		 * Rank 0:
		 * 	Buffer: 	 [i, b, c]
		 *  DestProcess: [2, 2, 1]
		 *
		 * Rank 1:
		 * 	Buffer: 	 [e, f, g, h, a]
		 *  DestProcess: [2, 0, 1, 0, 2]
		 *
		 * Rank 2:
		 * 	Buffer: 	 [j, k]
		 *  DestProcess: [0, 0]
		 *
		 *  Result:
		 *
		 *  Rank 0:
		 *  	[f, h, j, k]
		 *
		 *  Rank 1:
		 *  	[c, g]
		 *
		 *  Rank 2:
		 *  	[i, b, e, a]
		 *
		 *
		 * @param sendBuffer An array of data elements to be sent from this rank. It does not have to contain
		 * data for all processes, not does it need to be sorted.
		 * @param nSendBuffer The size of the sendBuffer array in the number of elements of type T.
		 * @param processIDs An array of rank targets. Each element in this array is the rank destination for the
		 * data element of the same array index in sendBuffer.
		 * @param nProcessIDs The size of the processIDs array in the number of elements of type int.
		 * @param recvBuffer A pointer to a space where the recvBuffer will be allocated, and populated with data received
		 * from across all ranks. The data will be ordered/grouped by process, but the data within each group
		 * will be in the same order as in the sendBuffer of the process it was received from.
		 * Only data that is received is allocated space.
		 * @param nRecvBuffer A pointer to a location where the size of the recvBuffer will be stored -
		 * this will be the number of received elements - processes for which no data is received will have no entries at all.
		 * @param mpComm The communicator containing all processes participating in the all-to-all.
		 *
		 * @tparam T The datatype of the data to be communicated. Must be a supported primitive type
		 * or an object that implements CustomMPIType.
		 *
		 * @retval cupcfd::error::E_SUCCESS Operation completed successfully.
		 * @retval cupcfd::error::E_MPI_DATATYPE_UNREGISTERED Datatype T does not have a suitable/registered datatype for communication via MPI.
		 * @retval cupcfd::error::E_MPI_ERR An MPI Error was encountered.
		 */
		template <class T>
		cupcfd::error::eCodes AllToAll(T * sendBuffer, int nSendBuffer,
											int * processIDs, int nProcessIDs,
											T ** recvBuffer, int * nRecvBuffer,
											cupcfd::comm::Communicator& mpComm);

	}
}

// Include Header Level Definitions
#include "AllToAll.ipp"

#endif
