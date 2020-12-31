/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Declarations of Common Broadcast Operations for Parallel Communications
 */

#ifndef CUPCFD_COMM_BROADCAST_INCLUDE_H
#define CUPCFD_COMM_BROADCAST_INCLUDE_H

#include "Communicator.h"
#include "Error.h"

namespace cupcfd
{
	namespace comm
	{
		/**
		 * A broadcast routine interface to distribute a copy of a data buffer from the
		 * source process to all other processes in the communicator
		 *
		 * @param buf On the root process, this provides the data for the broadcast.
		 * On all other processes in the communicator, this is the buffer
		 * where the received broadcast data is stored.
		 * @param nBuf The size of the data buffer/number of elements to be distributed.
		 * @param sourcePID The root/source process where the data will originate from.
		 * @param myComm A communicator object that contains details about the processes
		 * involved in the communication.
		 *
		 * @tparam T The datatype of the data to be communicated. Must be either a supported
		 * MPI primitive datatype or inherit from the CustomMPIType class to support communication.
		 *
		 * @return An error status indicating the success or failure of the operation
		 * @retval E_SUCCESS Broadcast completed successfully
		 * @retval E_MPI_DATATYPE_UNREGISTERED Datatype T does not have a datatype
		 * registered with the MPI library.
		 * @retval E_MPI_ERR An MPI Error was encountered.
		 */
		template <class T>
		__attribute__((warn_unused_result))
		cupcfd::error::eCodes Broadcast(T * buf, int nBuf, int sourcePID, Communicator& myComm);

		/**
		 * A broadcast routine interface to distribute a copy of a data buffer from the
		 * source process to all other processes in the communicator
		 *
		 * @param bSend On the root process, this provides the data for the broadcast.
		 * On all other processes in the communicator, this is ignored.
		 * @param nBSend The size of the data buffer/number of elements to be distributed, in the number of elements
		 * of type T.
		 * @param bRecv The buffer where the received broadcast data will be stored.
		 * Must be large enough to hold all received data.
		 * On the root process a copy of the bSend buffer will be placed here.
		 * @param nBRecv The size of the bRecv buffer, in the number of elements of type T.
		 * @param sourcePID The root/source process where the data will originate from.
		 * @param myComm A communicator object that contains details about the processes
		 * involved in the communication.
		 *
		 * @tparam T The datatype of the data to be communicated. Must be either a supported
		 * MPI primitive datatype or inherit from the CustomMPIType class to support communication.
		 *
		 * @return An error status indicating the success or failure of the operation
		 * @retval E_SUCCESS Broadcast completed successfully
		 * @retval E_MPI_DATATYPE_UNREGISTERED Datatype T does not have a datatype
		 * registered with the MPI library.
		 * @retval E_MPI_ERR An MPI Error was encountered.
		 */
		template <class T>
		__attribute__((warn_unused_result))
		cupcfd::error::eCodes Broadcast(T * bSend, int nBSend, T * bRecv, int nBRecv, int sourcePID, Communicator& myComm);

		/**
		 * Broadcast routine interface.
		 *
		 * This routine allows us to broadcast an unknown amount of data from a source process to all other processes -
		 * e.g. when other ranks do not know how much data the root process has to broadcast.
		 *
		 * It will allocate a suitably sized recv buffer and store the received data from the source process, storing
		 * the pointer to this array in an accessible parameter. Freeing of this array is left to the callee.
		 *
		 * @tparam T The datatype of the data to be communicated. Must be either a supported
		 * MPI primitive datatype or inherit from the CustomMPIType class to support communication.
		 *
		 * @param bSend The buffer to send - This only matters on the source process
		 * @param nBSend The size of the send buffer - This only matters on the source process
		 * @param nRecv A pointer to the location where the pointer to the created recv buffer will be stored (including on sourcePID).
		 * @param nBRecv A pointer to the location where the size of nRecv, in the number of elements of type T, will be stored.
		 * @param sourcePID The root process broadcasting the data
		 * @param myComm The communicator containing all processes that wish to recv data in this broadcast
		 *
		 * @return An error status indicating the success or failure of the operation
		 * @retval E_SUCCESS Broadcast completed successfully
		 * @retval E_MPI_DATATYPE_UNREGISTERED Datatype T does not have a datatype
		 * registered with the MPI library.
		 * @retval E_MPI_ERR An MPI Error was encountered.
		 */
		template <class T>
		__attribute__((warn_unused_result))
		cupcfd::error::eCodes Broadcast(T * bSend, int nBSend, T ** bRecv, int * nBRecv, int sourcePID, Communicator& myComm);
	} // namespace comm
} // namespace cupcfd

// Include Header Level Definitions
#include "Broadcast.ipp"

#endif
