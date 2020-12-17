/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Contains header level definitions for Barrier MPI operations.
 */

#ifndef CUPCFD_COMM_MPI_BARRIER_IPP_H
#define CUPCFD_COMM_MPI_BARRIER_IPP_H

namespace cupcfd
{
	namespace comm
	{
		namespace mpi
		{
			cupcfd::error::eCodes BarrierMPI(MPI_Comm comm)
			{
				// This method has little variance - it is basically just a wrapper for now
				int err = MPI_Barrier(comm);
				if(err != MPI_SUCCESS)
				{
					DEBUGGABLE_ERROR; return cupcfd::error::E_MPI_ERR;
				}

				return cupcfd::error::E_SUCCESS;
			}
		}
	}
}

#endif
