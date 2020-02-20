/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 */

#ifndef CUPCFD_COMM_REDUCE_INCLUDE_H
#define CUPCFD_COMM_REDUCE_INCLUDE_H

#include "Error.h"
#include "Communicator.h"

namespace cupcfd
{
	namespace comm
	{
		/**
		 * Performs a reduce across all ranks of the communicator and stores the result on a specified process.
		 *
		 * This variant performs add across the same index positions of each data source. E.g.:
		 *
		 * Rank 0 [a, b, c, d]
		 * Rank 1 [e, f, g, h]
		 * Rank 2 [i, j, k, l]
		 *
		 * Result: [a+e+i, b+f+j, c+g+k, d+h+l]
		 *
		 * @param bSend The buffer of data to be used as data sources for the add.
		 * Must be the same size on each participating process.
		 * @param nBSend The size of the bSend buffer in the number of elements of type T.
		 * @param bRecv The buffer to store the received results in. Only matters for the sink process.
		 * Must be of equal size to the send buffer of each participating process.
		 * @param nBRecv The size of the bRecv buffer in the number of elements of type T.
		 * @param sinkProcess The ID of the process that will receive the result.
		 * @param mpComm The communicator of all participating processes.
		 *
		 * @tparam T The datatype of the data to be communicated.
		 *
		 * @return An error status indicating the success or failure of the operation
		 * @retval E_SUCCESS Operation completed successfully.
		 * @retval E_MPI_DATATYPE_UNREGISTERED Datatype T does not have a datatype
		 * registered with the MPI library.
		 * @retval E_MPI_ERR An MPI Error was encountered.
		 */
		template <class T>
		cupcfd::error::eCodes reduceAdd(T * bSend, int nBSend, T * bRecv, int nBRecv, int sinkProcess, cupcfd::comm::Communicator& mpComm);

		/**
		 * Performs a reduce across all ranks of the communicator and stores the result on a specified process.
		 *
		 * This variant computes the product across the same index positions of each data source. E.g.:
		 *
		 * Rank 0 [a, b, c, d]
		 * Rank 1 [e, f, g, h]
		 * Rank 2 [i, j, k, l]
		 *
		 * Result: [a*e*i, b*f*j, c*g*k, d*h*l]
		 *
		 * @param bSend The buffer of data to be used as data sources for the add.
		 * Must be the same size on each participating process.
		 * @param nBSend The size of the bSend buffer in the number of elements of type T.
		 * @param bRecv The buffer to store the received results in. Only matters for the sink process.
		 * Must be of equal size to the send buffer of each participating process.
		 * @param nBRecv The size of the bRecv buffer in the number of elements of type T.
		 * @param sinkProcess The ID of the process that will receive the result.
		 * @param mpComm The communicator of all participating processes.
		 *
		 * @tparam T The datatype of the data to be communicated.
		 *
		 * @return An error status indicating the success or failure of the operation
		 * @retval E_SUCCESS Operation completed successfully.
		 * @retval E_MPI_DATATYPE_UNREGISTERED Datatype T does not have a datatype
		 * registered with the MPI library.
		 * @retval E_MPI_ERR An MPI Error was encountered.
		 */
		template <class T>
		cupcfd::error::eCodes reduceMultiply(T * bSend, int nBSend, T * bRecv, int nBRecv, int sinkProcess, cupcfd::comm::Communicator& mpComm);

		/**
		 * Performs a reduce across all ranks of the communicator and stores the result on a specified process.
		 *
		 * This variant computes the min across the same index positions of each data source. E.g.:
		 *
		 * Rank 0 [a, b, c, d]
		 * Rank 1 [e, f, g, h]
		 * Rank 2 [i, j, k, l]
		 *
		 * Result: [Min(a,e,i), Min(b,f,j), Min(c,g,k), Min(d,h,l)]
		 *
		 * @param bSend The buffer of data to be used as data sources for the add.
		 * Must be the same size on each participating process.
		 * @param nBSend The size of the bSend buffer in the number of elements of type T.
		 * @param bRecv The buffer to store the received results in. Only matters for the sink process.
		 * Must be of equal size to the send buffer of each participating process.
		 * @param nBRecv The size of the bRecv buffer in the number of elements of type T.
		 * @param sinkProcess The ID of the process that will receive the result.
		 * @param mpComm The communicator of all participating processes.
		 *
		 * @tparam T The datatype of the data to be communicated.
		 *
		 * @return An error status indicating the success or failure of the operation
		 * @retval E_SUCCESS Operation completed successfully.
		 * @retval E_MPI_DATATYPE_UNREGISTERED Datatype T does not have a datatype
		 * registered with the MPI library.
		 * @retval E_MPI_ERR An MPI Error was encountered.
		 */
		template <class T>
		cupcfd::error::eCodes reduceMin(T * bSend, int nBSend, T * bRecv, int nBRecv, int sinkProcess, cupcfd::comm::Communicator& mpComm);

		/**
		 * Performs a reduce across all ranks of the communicator and stores the result on a specified process.
		 *
		 * This variant computes the max across the same index positions of each data source. E.g.:
		 *
		 * Rank 0 [a, b, c, d]
		 * Rank 1 [e, f, g, h]
		 * Rank 2 [i, j, k, l]
		 *
		 * Result: [Max(a,e,i), Max(b,f,j), Max(c,g,k), Max(d,h,l)]
		 *
		 * @param bSend The buffer of data to be used as data sources for the add.
		 * Must be the same size on each participating process.
		 * @param nBSend The size of the bSend buffer in the number of elements of type T.
		 * @param bRecv The buffer to store the received results in. Only matters for the sink process.
		 * Must be of equal size to the send buffer of each participating process.
		 * @param nBRecv The size of the bRecv buffer in the number of elements of type T.
		 * @param sinkProcess The ID of the process that will receive the result.
		 * @param mpComm The communicator of all participating processes.
		 *
		 * @tparam T The datatype of the data to be communicated.
		 *
		 * @return An error status indicating the success or failure of the operation
		 * @retval E_SUCCESS Operation completed successfully.
		 * @retval E_MPI_DATATYPE_UNREGISTERED Datatype T does not have a datatype
		 * registered with the MPI library.
		 * @retval E_MPI_ERR An MPI Error was encountered.
		 */
		template <class T>
		cupcfd::error::eCodes reduceMax(T * bSend, int nBSend, T * bRecv, int nBRecv, int sinkProcess, cupcfd::comm::Communicator& mpComm);

		/**
		 * Performs a reduce across all ranks of the communicator and stores the result on every process.
		 *
		 * This variant performs add across the same index positions of each data source. E.g.:
		 *
		 * Rank 0 [a, b, c, d]
		 * Rank 1 [e, f, g, h]
		 * Rank 2 [i, j, k, l]
		 *
		 * Result: [a+e+i, b+f+j, c+g+k, d+h+l]
		 *
		 * @param bSend The buffer of data to be used as data sources for the add.
		 * Must be the same size on each participating process.
		 * @param nBSend The size of the bSend buffer in the number of elements of type T.
		 * @param bRecv The buffer to store the received results in.
		 * Must be of equal size to the send buffer of each participating process.
		 * @param nBRecv The size of the bRecv buffer in the number of elements of type T.
		 * @param mpComm The communicator of all participating processes.
		 *
		 * @tparam T The datatype of the data to be communicated.
		 *
		 * @return An error status indicating the success or failure of the operation
		 * @retval E_SUCCESS Operation completed successfully.
		 * @retval E_MPI_DATATYPE_UNREGISTERED Datatype T does not have a datatype
		 * registered with the MPI library.
		 * @retval E_MPI_ERR An MPI Error was encountered.
		 */
		template <class T>
		cupcfd::error::eCodes allReduceAdd(T * bSend, int nBSend, T * bRecv, int nBRecv, cupcfd::comm::Communicator& mpComm);

		/**
		 * Performs a reduce across all ranks of the communicator and stores the result on every process.
		 *
		 * This variant computes the product across the same index positions of each data source. E.g.:
		 *
		 * Rank 0 [a, b, c, d]
		 * Rank 1 [e, f, g, h]
		 * Rank 2 [i, j, k, l]
		 *
		 * Result: [a*e*i, b*f*j, c*g*k, d*h*l]
		 *
		 * @param bSend The buffer of data to be used as data sources for the add.
		 * Must be the same size on each participating process.
		 * @param nBSend The size of the bSend buffer in the number of elements of type T.
		 * @param bRecv The buffer to store the received results in.
		 * Must be of equal size to the send buffer of each participating process.
		 * @param nBRecv The size of the bRecv buffer in the number of elements of type T.
		 * @param mpComm The communicator of all participating processes.
		 *
		 * @tparam T The datatype of the data to be communicated.
		 *
		 * @return An error status indicating the success or failure of the operation
		 * @retval E_SUCCESS Operation completed successfully.
		 * @retval E_MPI_DATATYPE_UNREGISTERED Datatype T does not have a datatype
		 * registered with the MPI library.
		 * @retval E_MPI_ERR An MPI Error was encountered.
		 */
		template <class T>
		cupcfd::error::eCodes allReduceMultiply(T * bSend, int nBSend, T * bRecv, int nBRecv, cupcfd::comm::Communicator& mpComm);

		/**
		 * Performs a reduce across all ranks of the communicator and stores the result on every process.
		 *
		 * This variant computes the min across the same index positions of each data source. E.g.:
		 *
		 * Rank 0 [a, b, c, d]
		 * Rank 1 [e, f, g, h]
		 * Rank 2 [i, j, k, l]
		 *
		 * Result: [Min(a,e,i), Min(b,f,j), Min(c,g,k), Min(d,h,l)]
		 *
		 * @param bSend The buffer of data to be used as data sources for the add.
		 * Must be the same size on each participating process.
		 * @param nBSend The size of the bSend buffer in the number of elements of type T.
		 * @param bRecv The buffer to store the received results in.
		 * Must be of equal size to the send buffer of each participating process.
		 * @param nBRecv The size of the bRecv buffer in the number of elements of type T.
		 * @param mpComm The communicator of all participating processes.
		 *
		 * @tparam T The datatype of the data to be communicated.
		 *
		 * @return An error status indicating the success or failure of the operation
		 * @retval E_SUCCESS Operation completed successfully.
		 * @retval E_MPI_DATATYPE_UNREGISTERED Datatype T does not have a datatype
		 * registered with the MPI library.
		 * @retval E_MPI_ERR An MPI Error was encountered.
		 */
		template <class T>
		cupcfd::error::eCodes allReduceMin(T * bSend, int nBSend, T * bRecv, int nBRecv, cupcfd::comm::Communicator& mpComm);

		/**
		 * Performs a reduce across all ranks of the communicator and stores the result on every process.
		 *
		 * This variant computes the max across the same index positions of each data source. E.g.:
		 *
		 * Rank 0 [a, b, c, d]
		 * Rank 1 [e, f, g, h]
		 * Rank 2 [i, j, k, l]
		 *
		 * Result: [Max(a,e,i), Max(b,f,j), Max(c,g,k), Max(d,h,l)]
		 *
		 * @param bSend The buffer of data to be used as data sources for the add.
		 * Must be the same size on each participating process.
		 * @param nBSend The size of the bSend buffer in the number of elements of type T.
		 * @param bRecv The buffer to store the received results in.
		 * Must be of equal size to the send buffer of each participating process.
		 * @param nBRecv The size of the bRecv buffer in the number of elements of type T.
		 * @param mpComm The communicator of all participating processes.
		 *
		 * @tparam T The datatype of the data to be communicated.
		 *
		 * @return An error status indicating the success or failure of the operation
		 * @retval E_SUCCESS Operation completed successfully.
		 * @retval E_MPI_DATATYPE_UNREGISTERED Datatype T does not have a datatype
		 * registered with the MPI library.
		 * @retval E_MPI_ERR An MPI Error was encountered.
		 */
		template <class T>
		cupcfd::error::eCodes allReduceMax(T * bSend, int nBSend, T * bRecv, int nBRecv, cupcfd::comm::Communicator& mpComm);
	}
}

// Include Header Level Definitions
#include "Reduce.ipp"

#endif
