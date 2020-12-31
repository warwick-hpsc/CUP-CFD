/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Contains the declarations for MPI Waitall Operations
 */

#ifndef CUPCFD_COMM_MPI_WAITALL_INCLUDE_H
#define CUPCFD_COMM_MPI_WAITALL_INCLUDE_H

#include "Error.h"
#include "mpi.h"

namespace cupcfd
{
	namespace comm
	{
		namespace mpi
		{
			/**
			 * This is a wrapper for the MPI Waitall Operation
			 *
			 * @param requests The array of Active MPI Requests to Check
			 * @param nRequests The number of elements (requests) in the requests array.
			 *
			 * @retval E_SUCCESS Operation completed successfully.
			 * @retval E_MPI_ERR An MPI Error was encountered.
			 */
			__attribute__((warn_unused_result))
			cupcfd::error::eCodes WaitallMPI(MPI_Request * requests, int nRequests);
		}
	}
}

// Include Header Level Definitions
#include "WaitallMPI.ipp"

#endif
