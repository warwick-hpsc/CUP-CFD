/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Contains the declarations for Scatter MPI Operations.
 */

#ifndef CUPCFD_COMM_MPI_SCATTER_INCLUDE_H
#define CUPCFD_COMM_MPI_SCATTER_INCLUDE_H

#include "Error.h"
#include "mpi.h"

namespace cupcfd
{
	namespace comm
	{
		namespace mpi
		{
			/**
			* Runs an MPI Scatter, distributing a fixed size portion from the source to each process.
			*
			* @param bufferSend Contains all the data to be distributed by the root process - should be grouped by process (in order)
			* @param nSend The amount of data to be sent to each process (i.e. chunk size)
			* @param bufferRecv A buffer to store data received from the root process. Data is distributed in chunks in the order found in the array.
			* @param nRecv Data received from the root process
			*
			* @tparam T Accepted template types: int, double
			*
			 * @retval E_SUCCESS Operation completed successfully.
			 * @retval E_MPI_DATATYPE_UNREGISTERED Datatype T does not have a datatype
			 * registered with the MPI library.
			 * @retval E_MPI_ERR An MPI Error was encountered.
			*/
			template <class T>
			__attribute__((warn_unused_result))
			cupcfd::error::eCodes ScatterMPI(T * bufferSend, int nSend, T * bufferRecv, int nRecv, int sourcePID, MPI_Comm comm);

			/**
			* Runs an MPI ScatterV, distributing a variable size portion to each process from the source process.
			*
			* @param bufferSend Contains all the data to be distributed by the root process - should be grouped by process (in order)
			* @param nSend The amount of data to be sent to each process (i.e. chunk size)
			* @param bufferRecv A buffer to store data received from the root process. Data is distributed in chunks in the order found in the array.
			* @param nRecv Data received from the root process
			* @param sendCounts NUmber of elements to be sent for each process, indexed by process id
			* @param nDispls Size of sendCounts array - should be same as number of processes in comm
			* @tparam T Accepted template types: int, double
			*
			 * @retval E_SUCCESS Operation completed successfully.
			 * @retval E_MPI_DATATYPE_UNREGISTERED Datatype T does not have a datatype
			 * registered with the MPI library.
			 * @retval E_MPI_ERR An MPI Error was encountered.
			*/
			template <class T>
			__attribute__((warn_unused_result))
			cupcfd::error::eCodes ScatterVMPI(T * bufferSend, int nSend, T * bufferRecv, int nRecv, int * sendCounts, int nSendCounts, int sourcePID, MPI_Comm comm);
		} // namespace mpi
	} // namespace comm
} // namespace cupcfd

// Include Header Level Definitions
#include "ScatterMPI.ipp"

#endif
