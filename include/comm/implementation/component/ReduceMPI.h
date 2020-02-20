/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Contains declarations for MPI Reduce Operations
 */

#ifndef CUPCFD_COMM_MPI_REDUCE_INCLUDE_H
#define CUPCFD_COMM_MPI_REDUCE_INCLUDE_H

#include "mpi.h"
#include "Error.h"

namespace cupcfd
{
	namespace comm
	{
		namespace mpi
		{
			/**
			 * Wrapper for performing MPI Reduce.
			 *
			 * @param bSend The buffer to be sent from this process.
			 * @param nBSend The size of the bSend buffer in the number of elements of type T.
			 * @param bRecv The buffer where the result is stored.
			 * @param nBRecv The size of the bRecv buffer in the number of elements of type T.
			 * @param op The MPI Reduce Operator to use.
			 * @param sProcess The process that will receive the result.
			 * @param comm The MPI communicator detailing which processes are participating.
			 *
			 * @tparam T The datatype of the data to be communicated.
			 * ToDo/Note: CustomMPITypes are currently not supported since the mechanism
			 * for defining the MPI Operator is not in place.
			 *
			 * @retval E_SUCCESS Operation completed successfully.
			 * @retval E_MPI_DATATYPE_UNREGISTERED Datatype T does not have a datatype
			 * registered with the MPI library.
			 * @retval E_MPI_ERR An MPI Error was encountered.
			 */
			template <class T>
			cupcfd::error::eCodes reduceMPI(T * bSend, int nBSend, T * bRecv, int nBRecv, MPI_Op op, int sProcess, MPI_Comm comm);

			/**
			 * Wrapper for performing MPI Reduce using the Sum Operation.
			 *
			 * @param bSend The buffer to be sent from this process.
			 * @param nBSend The size of the bSend buffer in the number of elements of type T.
			 * @param bRecv The buffer where the result is stored.
			 * @param nBRecv The size of the bRecv buffer in the number of elements of type T.
			 * @param sProcess The process that will receive the result.
			 * @param comm The MPI communicator detailing which processes are participating.
			 *
			 * @tparam T The datatype of the data to be communicated.
			 * ToDo/Note: CustomMPITypes are currently not supported since the mechanism
			 * for defining the MPI Operator is not in place.
			 *
			 * @retval E_SUCCESS Operation completed successfully.
			 * @retval E_MPI_DATATYPE_UNREGISTERED Datatype T does not have a datatype
			 * registered with the MPI library.
			 * @retval E_MPI_ERR An MPI Error was encountered.
			 */
			template <class T>
			cupcfd::error::eCodes reduceMPISum(T * bSend, int nBSend, T * bRecv, int nBRecv, int sProcess, MPI_Comm comm);

			/**
			 * Wrapper for performing MPI Reduce using the Min Operation.
			 *
			 * @param bSend The buffer to be sent from this process.
			 * @param nBSend The size of the bSend buffer in the number of elements of type T.
			 * @param bRecv The buffer where the result is stored.
			 * @param nBRecv The size of the bRecv buffer in the number of elements of type T.
			 * @param sProcess The process that will receive the result.
			 * @param comm The MPI communicator detailing which processes are participating.
			 *
			 * @tparam T The datatype of the data to be communicated.
			 * ToDo/Note: CustomMPITypes are currently not supported since the mechanism
			 * for defining the MPI Operator is not in place.
			 *
			 * @retval E_SUCCESS Operation completed successfully.
			 * @retval E_MPI_DATATYPE_UNREGISTERED Datatype T does not have a datatype
			 * registered with the MPI library.
			 * @retval E_MPI_ERR An MPI Error was encountered.
			 */
			template <class T>
			cupcfd::error::eCodes reduceMPIMin(T * bSend, int nBSend, T * bRecv, int nBRecv, int sProcess, MPI_Comm comm);

			/**
			 * Wrapper for performing MPI Reduce using the Max Operation.
			 *
			 * @param bSend The buffer to be sent from this process.
			 * @param nBSend The size of the bSend buffer in the number of elements of type T.
			 * @param bRecv The buffer where the result is stored.
			 * @param nBRecv The size of the bRecv buffer in the number of elements of type T.
			 * @param sProcess The process that will receive the result.
			 * @param comm The MPI communicator detailing which processes are participating.
			 *
			 * @tparam T The datatype of the data to be communicated.
			 * ToDo/Note: CustomMPITypes are currently not supported since the mechanism
			 * for defining the MPI Operator is not in place.
			 *
			 * @retval E_SUCCESS Operation completed successfully.
			 * @retval E_MPI_DATATYPE_UNREGISTERED Datatype T does not have a datatype
			 * registered with the MPI library.
			 * @retval E_MPI_ERR An MPI Error was encountered.
			 */
			template <class T>
			cupcfd::error::eCodes reduceMPIMax(T * bSend, int nBSend, T * bRecv, int nBRecv, int sProcess, MPI_Comm comm);

			/**
			 * Wrapper for performing MPI Reduce using the Product Operation.
			 *
			 * @param bSend The buffer to be sent from this process.
			 * @param nBSend The size of the bSend buffer in the number of elements of type T.
			 * @param bRecv The buffer where the result is stored.
			 * @param nBRecv The size of the bRecv buffer in the number of elements of type T.
			 * @param sProcess The process that will receive the result.
			 * @param comm The MPI communicator detailing which processes are participating.
			 *
			 * @tparam T The datatype of the data to be communicated.
			 * ToDo/Note: CustomMPITypes are currently not supported since the mechanism
			 * for defining the MPI Operator is not in place.
			 *
			 * @retval E_SUCCESS Operation completed successfully.
			 * @retval E_MPI_DATATYPE_UNREGISTERED Datatype T does not have a datatype
			 * registered with the MPI library.
			 * @retval E_MPI_ERR An MPI Error was encountered.
			 */
			template <class T>
			cupcfd::error::eCodes reduceMPIProduct(T * bSend, int nBSend, T * bRecv, int nBRecv, int sProcess, MPI_Comm comm);

			/**
			 * Wrapper for performing MPI All Reduce.
			 *
			 * @param bSend The buffer to be sent from this process.
			 * @param nBSend The size of the bSend buffer in the number of elements of type T.
			 * @param bRecv The buffer where the result is stored.
			 * @param nBRecv The size of the bRecv buffer in the number of elements of type T.
			 * @param comm The MPI communicator detailing which processes are participating.
			 *
			 * @tparam T The datatype of the data to be communicated.
			 * ToDo/Note: CustomMPITypes are currently not supported since the mechanism
			 * for defining the MPI Operator is not in place.
			 *
			 * @retval E_SUCCESS Operation completed successfully.
			 * @retval E_MPI_DATATYPE_UNREGISTERED Datatype T does not have a datatype
			 * registered with the MPI library.
			 * @retval E_MPI_ERR An MPI Error was encountered.
			 */
			template <class T>
			cupcfd::error::eCodes allReduceMPI(T * bSend, int nBSend, T * bRecv, int nBRecv, MPI_Op op, MPI_Comm comm);

			/**
			 * Wrapper for performing MPI All Reduce using the Sum operation.
			 *
			 * @param bSend The buffer to be sent from this process.
			 * @param nBSend The size of the bSend buffer in the number of elements of type T.
			 * @param bRecv The buffer where the result is stored.
			 * @param nBRecv The size of the bRecv buffer in the number of elements of type T.
			 * @param comm The MPI communicator detailing which processes are participating.
			 *
			 * @tparam T The datatype of the data to be communicated.
			 * ToDo/Note: CustomMPITypes are currently not supported since the mechanism
			 * for defining the MPI Operator is not in place.
			 *
			 * @retval E_SUCCESS Operation completed successfully.
			 * @retval E_MPI_DATATYPE_UNREGISTERED Datatype T does not have a datatype
			 * registered with the MPI library.
			 * @retval E_MPI_ERR An MPI Error was encountered.
			 */
			template <class T>
			cupcfd::error::eCodes allReduceMPISum(T * bSend, int nBSend, T * bRecv, int nBRecv, MPI_Comm comm);

			/**
			 * Wrapper for performing MPI All Reduce using the Min operation.
			 *
			 * @param bSend The buffer to be sent from this process.
			 * @param nBSend The size of the bSend buffer in the number of elements of type T.
			 * @param bRecv The buffer where the result is stored.
			 * @param nBRecv The size of the bRecv buffer in the number of elements of type T.
			 * @param comm The MPI communicator detailing which processes are participating.
			 *
			 * @tparam T The datatype of the data to be communicated.
			 * ToDo/Note: CustomMPITypes are currently not supported since the mechanism
			 * for defining the MPI Operator is not in place.
			 *
			 * @retval E_SUCCESS Operation completed successfully.
			 * @retval E_MPI_DATATYPE_UNREGISTERED Datatype T does not have a datatype
			 * registered with the MPI library.
			 * @retval E_MPI_ERR An MPI Error was encountered.
			 */
			template <class T>
			cupcfd::error::eCodes allReduceMPIMin(T * bSend, int nBSend, T * bRecv, int nBRecv, MPI_Comm comm);

			/**
			 * Wrapper for performing MPI All Reduce using the Max operation.
			 *
			 * @param bSend The buffer to be sent from this process.
			 * @param nBSend The size of the bSend buffer in the number of elements of type T.
			 * @param bRecv The buffer where the result is stored.
			 * @param nBRecv The size of the bRecv buffer in the number of elements of type T.
			 * @param comm The MPI communicator detailing which processes are participating.
			 *
			 * @tparam T The datatype of the data to be communicated.
			 * ToDo/Note: CustomMPITypes are currently not supported since the mechanism
			 * for defining the MPI Operator is not in place.
			 *
			 * @retval E_SUCCESS Operation completed successfully.
			 * @retval E_MPI_DATATYPE_UNREGISTERED Datatype T does not have a datatype
			 * registered with the MPI library.
			 * @retval E_MPI_ERR An MPI Error was encountered.
			 */
			template <class T>
			cupcfd::error::eCodes allReduceMPIMax(T * bSend, int nBSend, T * bRecv, int nBRecv, MPI_Comm comm);

			/**
			 * Wrapper for performing MPI All Reduce using the Product operation.
			 *
			 * @param bSend The buffer to be sent from this process.
			 * @param nBSend The size of the bSend buffer in the number of elements of type T.
			 * @param bRecv The buffer where the result is stored.
			 * @param nBRecv The size of the bRecv buffer in the number of elements of type T.
			 * @param comm The MPI communicator detailing which processes are participating.
			 *
			 * @tparam T The datatype of the data to be communicated.
			 * ToDo/Note: CustomMPITypes are currently not supported since the mechanism
			 * for defining the MPI Operator is not in place.
			 *
			 * @retval E_SUCCESS Operation completed successfully.
			 * @retval E_MPI_DATATYPE_UNREGISTERED Datatype T does not have a datatype
			 * registered with the MPI library.
			 * @retval E_MPI_ERR An MPI Error was encountered.
			 */
			template <class T>
			cupcfd::error::eCodes allReduceMPIProduct(T * bSend, int nBSend, T * bRecv, int nBRecv, MPI_Comm comm);
		}
	}
}

// Include header level definitions
#include "ReduceMPI.ipp"

#endif
