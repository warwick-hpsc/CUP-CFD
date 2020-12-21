/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Contains declarations for MPI Barrier operations
 */

#ifndef CUPCFD_COMM_MPI_BARRIER_INCLUDE_H
#define CUPCFD_COMM_MPI_BARRIER_INCLUDE_H

#include "mpi.h"
#include "Error.h"

namespace cupcfd
{
	namespace comm
	{
		namespace mpi
		{
			/**
			 * Simple wrapper for MPI Barrier operation.
			 * Blocks on all processes in the communicator till
			 * all processes have called the method.
			 *
			 * @param comm The MPI communicator to use.
			 *
			 * @retval E_SUCCESS Operation completed successfully.
			 * @retval E_MPI_ERR An MPI Error was encountered.
			 */
			__attribute__((warn_unused_result))
			cupcfd::error::eCodes BarrierMPI(MPI_Comm comm);
		}
	}
}

// Include Header Level Definitions
#include "BarrierMPI.ipp"

#endif
