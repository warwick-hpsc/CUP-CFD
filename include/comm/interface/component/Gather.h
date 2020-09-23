/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Declarations for Gather Interface Operations
 */

#ifndef CUPCFD_COMM_GATHER_INCLUDE_H
#define CUPCFD_COMM_GATHER_INCLUDE_H

#include "Communicator.h"
#include "Error.h"

namespace cupcfd
{
	namespace comm
	{
		/**
		 * Performs a Gather operation across ranks to gather a fixed-size block of data from each process
		 * onto a single sink process from all processes in a MPI Communicator.
		 *
		 * This is a top-level interface to pass through to the underlying comms operation, as well as performing
		 * various error checking.
		 *
		 * @param bSend The buffer containing data to be sent, including on the sink process.
		 * Must contain at least nElePerProcess elements of type T.
		 * @param nBSend The number of elements of type T in the send buffer bSend.
		 * @param bRecv The buffer for receiving data on the sink process. Does not matter for senders
		 * @param nBRecv The size of the bRecv buffer in the number of elements of type T.
		 * @param nElePerProcess The number of data elements of type T to send from each process.
		 * @param sinkProcess The rank ID of the process that will be receiving the data.
		 * @param comm The communicator for the participating processes
		 *
		 * @tparam T The datatype of the data to be communicated. Must be either a supported
		 * MPI primitive datatype or inherit from the CustomMPIType class to support communication.
		 *
		 * @return An error status indicating the success or failure of the operation
		 * @retval cupcfd::error::E_SUCCESS Operation completed successfully.
		 * @retval cupcfd::error::E_NULL_PTR A required buffer was found to be null (e.g. the recv buffer)
		 * @retval cupcfd::error::E_ARRAY_SIZE_UNDERSIZED The recv buffer is not large enough to hold all received data.
		 * @retval cupcfd::error::E_MPI_DATATYPE_UNREGISTERED Datatype T does not have a suitable/registered datatype for communication via MPI.
		 * @retval cupcfd::error::E_MPI_ERR An MPI Error was encountered.
		 */
		template <class T>
		// cupcfd::error::eCodes Gather(T * bSend, int nBSend, T * bRecv, int nBRecv, int nElePerProcess, int sinkProcess, cupcfd::comm::Communicator& mpComm);
		cupcfd::error::eCodes Gather(T * bSend, T * bRecv, int nBRecv, int nElePerProcess, int sinkProcess, cupcfd::comm::Communicator& mpComm);

		/**
		 * Performs a AllGather operation across ranks to gather a fixed-size block of data from each process,
		 * distributing that result to every process in the communicator.
		 *
		 * This is a top-level interface to pass through to the underlying comms operation, as well as performing
		 * various error checking.
		 *
		 * @param bSend The buffer containing data to be sent, including on the sink process.
		 * Must contain at least nElePerProcess elements of type T.
		 * @param nBSend The number of elements of type T in the send buffer bSend.
		 * @param bRecv The buffer for receiving data.
		 * @param nBRecv The size of the bRecv buffer in the number of elements of type T.
		 * Must be large enough to hold nElePerProcess * number of process ranks elements.
		 * @param sinkProcess The rank ID of the process that will be receiving the data.
		 * @param comm The communicator for the participating processes
		 *
		 * @tparam T The datatype of the data to be communicated. Must be either a supported
		 * MPI primitive datatype or inherit from the CustomMPIType class to support communication.
		 *
		 * @return An error status indicating the success or failure of the operation
		 * @retval cupcfd::error::E_SUCCESS Operation completed successfully.
		 * @retval cupcfd::error::E_NULL_PTR A required buffer was found to be null (e.g. the recv buffer)
		 * @retval cupcfd::error::E_ARRAY_SIZE_UNDERSIZED The recv buffer is not large enough to hold all received data.
		 * @retval cupcfd::error::E_MPI_DATATYPE_UNREGISTERED Datatype T does not have a suitable/registered datatype for communication via MPI.
		 * @retval cupcfd::error::E_MPI_ERR An MPI Error was encountered.
		 */
		template <class T>
		// cupcfd::error::eCodes AllGather(T * bSend, int nBSend, T * bRecv, int nBRecv, int nElePerProcess, cupcfd::comm::Communicator& mpComm);
		cupcfd::error::eCodes AllGather(T * bSend, T * bRecv, int nBRecv, int nElePerProcess, cupcfd::comm::Communicator& mpComm);

		/**
		 * Performs a Gather operation across ranks to gather a variable sized block of data from each process
		 * onto a single sink process from all processes in a Communicator.
		 *
		 * This is a top-level interface to pass through to the underlying comms operation, as well as performing
		 * various error checking.
		 *
		 * @param bSend The buffer containing data to be sent, including on the sink process.
		 * Must contain at least nElePerProcess elements of type T.
		 * @param nBSend The number of elements of type T in the send buffer bSend.
		 * @param bRecv The buffer for receiving data on the sink process. Does not matter for senders
		 * @param nBRecv The size of the bRecv buffer in the number of elements of type T.
		 * @param bRecvCounts An array of the number of elements to be sent from each process.
		 * Must have an entry for each process in the communicator, indexed by rank ID from 0.
		 * @param nBRecvCounts The size of the bRecvCounts array, in the number of elements of type int.
		 * @param sinkPID The rank ID of the process that will be receiving the data.
		 * @param comm The communicator for the participating processes
		 *
		 * @tparam T The datatype of the data to be communicated. Must be either a supported
		 * MPI primitive datatype or inherit from the CustomMPIType class to support communication.
		 *
		 * @return An error status indicating the success or failure of the operation
		 * @retval cupcfd::error::E_SUCCESS Operation completed successfully.
		 * @retval cupcfd::error::E_NULL_PTR A required buffer was found to be null (e.g. the recv buffer)
		 * @retval cupcfd::error::E_MPI_DATATYPE_UNREGISTERED Datatype T does not have a suitable/registered datatype for communication via MPI.
		 * @retval cupcfd::error::E_MPI_ERR An MPI Error was encountered.
		 */
		template <class T>
		// cupcfd::error::eCodes GatherV(T * bSend, int nBSend, T * bRecv, int nBRecv, int * bRecvCounts, int nBRecvCounts, int sinkPID, cupcfd::comm::Communicator& mpComm);
		cupcfd::error::eCodes GatherV(T * bSend, int nBSend, T * bRecv, int * bRecvCounts, int sinkProcess, cupcfd::comm::Communicator& mpComm);

		/**
		 * Performs a Gather operation across ranks to gather a variable sized block of data from each process
		 * onto a single sink process from all processes in a Communicator.
		 *
		 * This is a top-level interface to pass through to the underlying comms operation, as well as performing
		 * various error checking.
		 *
		 * This variant does not expect the sink process to know how much data is sent by each process.
		 * Instead, it communicates the buffer send size of each rank to determine the values of bRecvCounts
		 * and creates/allocates a suitably sized recv buffer accordingly.
		 *
		 * Note: This process involves an extra communication step since a gather is necessary to retrieve
		 * the send sizes for each rank.
		 *
		 * @param bSend The buffer containing data to be sent, including on the sink process.
		 * Must contain at least nElePerProcess elements of type T.
		 * @param nBSend The number of elements of type T in the send buffer bSend.
		 * @param bRecv A pointer to where a buffer for received data will be created. The location pointed at must be nullptr.
		 * This will remain nullptr upon completion if this process is not the sink process.
		 * @param nBRecv A pointer to where the size of the bRecv buffer will be stored.
		 * @param bRecvCounts A pointer to where an array of the number of elements sent from each process will be created. The location pointed at must be nullptr.
		 * This will remain nullptr upon completion if this process is not the sink process.
		 * @param nBRecvCounts A pointer to where the size of the bRecvCounts array, in the number of elements of type int will be stored.
		 * @param sinkPID The rank ID of the process that will be receiving the data.
		 * @param comm The communicator for the participating processes
		 *
		 * @tparam T The datatype of the data to be communicated. Must be either a supported
		 * MPI primitive datatype or inherit from the CustomMPIType class to support communication.
		 *
		 * @return An error status indicating the success or failure of the operation
		 * @retval cupcfd::error::E_SUCCESS Operation completed successfully.
		 * @retval cupcfd::error::E_NULL_PTR A required buffer was found to be null.
		 * @retval cupcfd::error::E_NOT_NULL_PTR The location pointed at by bRecv (i.e. *bRecv) was not nullptr on the sink process.
		 * @retval cupcfd::error::E_MPI_DATATYPE_UNREGISTERED Datatype T does not have a suitable/registered datatype for communication via MPI.
		 * @retval cupcfd::error::E_MPI_ERR An MPI Error was encountered.
		 */
		template <class T>
		cupcfd::error::eCodes GatherV(T * bSend, int nBSend, T ** bRecv, int * nBRecv, int ** bRecvCounts, int * nBRecvCounts, int sinkPID, cupcfd::comm::Communicator& mpComm);

		/**
		 * Performs a AllGather operation across ranks to gather a variable sized block of data from each process
		 * to all processes in a Communicator.
		 *
		 * This is a top-level interface to pass through to the underlying comms operation, as well as performing
		 * various error checking.
		 *
		 * @param bSend The buffer containing data to be sent, including on the sink process.
		 * Must contain at least nElePerProcess elements of type T.
		 * @param nBSend The number of elements of type T in the send buffer bSend.
		 * @param bRecv The buffer for receiving data. Since this is all gather it must hold data received from all processes, on every process.
		 * @param nBRecv The size of the bRecv buffer in the number of elements of type T.
		 * @param bRecvCounts An array of the number of elements expected to be sent/receive from each process.
		 * Must have an entry for each process in the communicator, indexed by rank ID from 0.
		 * @param nBRecvCounts The size of the bRecvCounts array, in the number of elements of type int.
		 * @param comm The communicator for the participating processes
		 *
		 * @tparam T The datatype of the data to be communicated. Must be either a supported
		 * MPI primitive datatype or inherit from the CustomMPIType class to support communication.
		 *
		 * @return An error status indicating the success or failure of the operation
		 * @retval cupcfd::error::E_SUCCESS Operation completed successfully.
		 * @retval cupcfd::error::E_MPI_DATATYPE_UNREGISTERED Datatype T does not have a suitable/registered datatype for communication via MPI.
		 * @retval cupcfd::error::E_MPI_ERR An MPI Error was encountered.
		 */
		template <class T>
		// cupcfd::error::eCodes AllGatherV(T * bSend, int nBSend, T * bRecv, int nBRecv, int * bRecvCounts, int nBRecvCounts, cupcfd::comm::Communicator& mpComm);
		cupcfd::error::eCodes AllGatherV(T * bSend, int nBSend, T * bRecv, int * bRecvCounts, cupcfd::comm::Communicator& mpComm);

		/**
		 * Performs a AllGather operation across ranks to gather a variable sized block of data from each process
		 * to all processes in a Communicator.
		 *
		 * This variant handles cases where the number of expected bytes to be sent by each process is unknown, performing
		 * an extra communication stage to determine this information. It also handles the creation of suitably sized arrays.
		 * Since this requires additional communication, if the expected sizes *are* known then do not use this variant, since
		 * it will incur an uncessary expense.
		 *
		 * This is a top-level interface to pass through to the underlying comms operation, as well as performing
		 * various error checking.
		 *
		 * @param bSend The buffer containing data to be sent, including on the sink process.
		 * Must contain at least nElePerProcess elements of type T.
		 * @param nBSend The number of elements of type T in the send buffer bSend.
		 * @param bRecv A pointer to where a buffer for received data will be created. The location pointed at must be nullptr.
		 * @param nBRecv A pointer to where the size of the bRecv buffer will be stored.
		 * @param bRecvCounts A pointer to where an array of the number of elements sent from each process will be created. The location pointed at must be nullptr.
		 * @param nBRecvCounts A pointer to where the size of the bRecvCounts array, in the number of elements of type int will be stored.
		 * @param comm The communicator for the participating processes
		 *
		 * @tparam T The datatype of the data to be communicated. Must be either a supported
		 * MPI primitive datatype or inherit from the CustomMPIType class to support communication.
		 *
		 * @return An error status indicating the success or failure of the operation
		 * @retval cupcfd::error::E_SUCCESS Operation completed successfully.
		 * @retval cupcfd::error::E_MPI_DATATYPE_UNREGISTERED Datatype T does not have a suitable/registered datatype for communication via MPI.
		 * @retval cupcfd::error::E_MPI_ERR An MPI Error was encountered.
		 */
		template <class T>
		cupcfd::error::eCodes AllGatherV(T * bSend, int nBSend, T ** bRecv, int * nBRecv, int ** bRecvCounts, int * nBRecvCounts, cupcfd::comm::Communicator& mpComm);

	} // namespace comm
} // namespace cupcfd

// Include Header Level Definitions
#include "Gather.ipp"

#endif
