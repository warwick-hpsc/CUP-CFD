/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Contains declarations for All To All MPI Operations.
 */

#ifndef CUPCFD_COMM_MPI_ALLTOALL_INCLUDE_H
#define CUPCFD_COMM_MPI_ALLTOALL_INCLUDE_H

// Provides access to MPI functionality
#include "mpi.h"

// Provides Error Codes
#include "Error.h"

namespace cupcfd
{
	namespace comm
	{
		namespace mpi
		{
			/**
			 * Wrapper for performing an All-to-All operation using MPI.
			 * This All-to-All communicates a fixed number of elements to each MPI rank
			 * in the communicator.
			 *
			 * @param sendbuf The buffer containing the data to be sent
			 * @param sendcount The number of elements to send to each process
			 * @param recvbuf The buffer to store any received data in.
			 * @param recvcount The number of elements expected to recieve from any individual process.
			 * @param comm The MPI communicator to use.
			 *
			 * @tparam The datatype of the data to be communicated. This must be of a primative type
			 * or a class that extends the CustomMPIType class.
			 *
			 * @return An error status indicating the success or failure of the operation
			 * @retval cupcfd::error::E_SUCCESS Operation was successful
			 * @retval cupcfd::error::E_MPI_ERR An MPI error was encountered
			 */
			template <class T>
			__attribute__((warn_unused_result))
			cupcfd::error::eCodes AllToAllMPI(T * sendbuf, int sendcount,
												T * recvbuf, int recvcount,
												MPI_Comm comm);

			/**
			 * Wrapper for performing an All-to-All operation using MPI.
			 * This All-to-All communicates a variable number of elements to each MPI rank
			 * in the communicator.
			 *
			 * This function will compute the displacements based on the send/recv sizes.
			 *
			 * @param sendbuf The array containing the data to be sent
			 * @param sendcounts The array containing the number of elements to be sent to each rank.
			 * @param recvbuf The buffer to store any received data in.
			 * @param recvcounts The array containing the number of elements expected to recv from each rank.
			 * @param comm The MPI communicator to use.
			 *
			 * @tparam The datatype of the data to be communicated. This must be of a primative type
			 * or a class that extends the CustomMPIType class.
			 *
			 * @return An error status indicating the success or failure of the operation
			 * @retval cupcfd::error::E_SUCCESS Operation was successful
			 * @retval cupcfd::error::E_MPI_ERR An MPI error was encountered
			 */
			template <class T>
			__attribute__((warn_unused_result))
			cupcfd::error::eCodes AllToAllVMPI(T * sendbuf, int * sendcounts,
												T * recvbuf, int *recvcounts,
												MPI_Comm comm);

			/**
			 * Wrapper for performing an All-to-All operation using MPI.
			 * This All-to-All communicates a variable number of elements to each MPI rank
			 * in the communicator.
			 *
			 * @param sendbuf The array containing the data to be sent
			 * @param sendcounts The array containing the number of elements to be sent to each rank.
			 * @param sdispls The array of displacements, indexed by process, indicating the index in sendbuf
			 * where the block of data to be sent to that process starts.
			 * @param recvbuf The buffer to store any received data in.
			 * @param recvcounts The array containing the number of elements expected to recv from each rank.
			 * @param rdispls The array of displacements, indexed by process, indicating the index in recvbuf
			 * where the block of data received from a process begins.
			 * @param comm The MPI communicator to use.
			 *
			 * @tparam The datatype of the data to be communicated. This must be of a primative type
			 * or a class that extends the CustomMPIType class.
			 *
			 * @return An error status indicating the success or failure of the operation
			 * @retval cupcfd::error::E_SUCCESS Operation was successful
			 * @retval cupcfd::error::E_MPI_ERR An MPI error was encountered
			 */
			template <class T>
			__attribute__((warn_unused_result))
			cupcfd::error::eCodes AllToAllVMPI(T * sendbuf, int * sendcounts, int *sdispls,
												T * recvbuf, int *recvcounts, int *rdispls,
												MPI_Comm comm);
		}
	}
}

// Include Header Level Definitions
#include "AllToAllMPI.ipp"

#endif
