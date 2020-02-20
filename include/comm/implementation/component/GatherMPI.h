/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Contains declarations for MPI gather operations
 */

#ifndef CUPCFD_COMM_MPI_GATHER_INCLUDE_H
#define CUPCFD_COMM_MPI_GATHER_INCLUDE_H

#include "mpi.h"
#include "Error.h"

namespace cupcfd
{
	namespace comm
	{
		namespace mpi
		{
			/**
			 * Calls the MPI routine responsible for gathering a fixed-size block of data from each process
			 * onto a single sink process from all processes in a MPI Communicator.
			 * This is a wrapper that also handles passing the correct MPI datatype for primitive types
			 * and any object that implements CustomMPIType.
			 *
			 * @param bSend The buffer containing data to be sent, including on the sink process.
			 * Must contain at least nElePerProcess elements of type T.
			 * @param bRecv The buffer for receiving data on the sink process. Does not matter for senders
			 * @param nElePerProcess The number of data elements of type T to send from each process.
			 * @param sinkProcess The rank ID of the process that will be receiving the data.
			 * @param comm The MPI communicator detailing which processes are participating.
			 *
			 * @tparam T The datatype of the data to be communicated.
			 *
			 * @retval E_SUCCESS Operation completed successfully.
			 * @retval E_MPI_DATATYPE_UNREGISTERED Datatype T does not have a datatype
			 * registered with the MPI library.
			 * @retval E_MPI_ERR An MPI Error was encountered.
			 */
			template <class T>
			cupcfd::error::eCodes GatherMPI(T * bSend, T * bRecv, int nElePerProcess, int sinkProcess, MPI_Comm comm);

			/**
			 * Calls the MPI routine responsible for gathering a fixed-size block of data from each process
			 * and distributing the gathered data to all processes in a MPI Communicator.
			 * This is a wrapper that also handles passing the correct MPI datatype for primitive types
			 * and any object that implements CustomMPIType.
			 *
			 * @param bSend The buffer containing data to be sent, including on the sink process.
			 * Must contain at least nElePerProcess elements of type T.
			 * @param bRecv The buffer for receiving the gathered data.
			 * @param nElePerProcess The number of data elements of type T being sent by each process.
			 * @param comm The MPI communicator detailing which processes are participating.
			 *
			 * @tparam T The datatype of the data to be communicated.
			 *
			 * @retval E_SUCCESS Operation completed successfully.
			 * @retval E_MPI_DATATYPE_UNREGISTERED Datatype T does not have a datatype
			 * registered with the MPI library.
			 * @retval E_MPI_ERR An MPI Error was encountered.
			 */
			template <class T>
			cupcfd::error::eCodes AllGatherMPI(T * bSend, T * bRecv, int nElePerProcess, MPI_Comm comm);

			/**
			 * Calls the MPI routine responsible for gathering a variable sized block of data from each process
			 * onto a single sink process from all processes in a MPI Communicator.
			 * This is a wrapper that also handles passing the correct MPI datatype for primitive types
			 * and any object that implements CustomMPIType.
			 *
			 * @param bSend The buffer containing data to be sent, including on the sink process.
			 * Must be at least nEleSend elements of type T in size.
			 * @param nEleSend The number of data elements of  type T being sent by this process from bSend.
			 * @param bRecv The buffer for receiving data on the sink process. Does not matter for senders.
			 * @param bRecvCounts An array of the number of elements expected from each process.
			 * Must have an entry for each process in the communicator, indexed by rank ID from 0.
			 * @param sinkPID The rank ID of the process that will be receiving the data.
			 * @param comm The MPI communicator detailing which processes are participating.
			 *
			 * @tparam T The datatype of the data to be communicated.
			 *
			 * @retval E_SUCCESS Operation completed successfully.
			 * @retval E_MPI_DATATYPE_UNREGISTERED Datatype T does not have a datatype
			 * registered with the MPI library.
			 * @retval E_MPI_ERR An MPI Error was encountered.
			 */
			template <class T>
			cupcfd::error::eCodes GatherVMPI(T * bSend, int nEleSend, T * bRecv, int * bRecvCounts, int sinkPID, MPI_Comm comm);

			/**
			 * Calls the MPI routine responsible for gathering a variable sized block of data from each process
			 * and distributing the gathered data to all processes in a MPI Communicator.
			 * This is a wrapper that also handles passing the correct MPI datatype for primitive types
			 * and any object that implements CustomMPIType.
			 *
			 * @param bSend The buffer containing data to be sent, including on the sink process.
			 * Must be at least nEleSend elements of type T in size.
			 * @param nEleSend The number of data elements of  type T being sent by this process from bSend.
			 * @param bRecv The buffer for receiving data on the sink process.
			 * Must be large enough to hold all received data.
			 * @param bRecvCounts An array of the number of elements expected from each process.
			 * Must have an entry for each process in the communicator, indexed by rank ID from 0.
			 * @param comm The MPI communicator detailing which processes are participating.
			 *
			 * @tparam T The datatype of the data to be communicated.
			 *
			 * @retval E_SUCCESS Operation completed successfully.
			 * @retval E_MPI_DATATYPE_UNREGISTERED Datatype T does not have a datatype
			 * registered with the MPI library.
			 * @retval E_MPI_ERR An MPI Error was encountered.
			 */
			template <class T>
			cupcfd::error::eCodes AllGatherVMPI(T * bSend, int nEleSend, T * bRecv, int * bRecvCounts, MPI_Comm comm);
		} // namespace mpi
	} // namespace comm
} // namespace cupcfd

// Include Header Level Definitions
#include "GatherMPI.ipp"

#endif
