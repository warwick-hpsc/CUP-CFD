/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Contains declarations for wrappers that interface with MPI Broadcast routines.
 */

#ifndef CUPCFD_COMM_BROADCAST_MPI_INCLUDE_H
#define CUPCFD_COMM_BROADCAST_MPI_INCLUDE_H

#include "mpi.h"
#include "Error.h"

namespace cupcfd
{
	namespace comm
	{
		namespace mpi
		{
			/**
			 * Calls the MPI routine responsible for broadcasting a buffer to all
			 * processes in a MPI Communicator.
			 * This is a wrapper that also handles passing the correct MPI datatype.
			 *
			 * @param buf On the root process, this provides the data for the broadcast.
			 * 			  On all other processes in the communicator, this is the buffer
			 * 			  where the received broadcast data is stored.
			 * @param nEle The number of elements to be broadcast from the buffer.
			 * @param sourcePID The root process broadcasting the data to others.
			 * @param comm The MPI communicator detailing which processes are participating.
			 *
			 * @tparam T The datatype of the data to be communicated.
			 * Can be a supported primitive type or a CustomMPIType.
			 *
			 * @return An error status indicating the success or failure of the operation
			 * @retval E_SUCCESS Broadcast completed successfully.
			 * @retval E_MPI_DATATYPE_UNREGISTERED Datatype T does not have a datatype
			 * registered with the MPI library.
			 * @retval E_MPI_ERR An MPI Error was encountered.
			 */
			template <class T>
			cupcfd::error::eCodes BroadcastMPI(T * buf, int nEle, int sourcePID, MPI_Comm comm);
		} // namespace mpi
	} // namespace comm
} // namespace cupcfd

#include "BroadcastMPI.ipp"

#endif
