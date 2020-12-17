/*
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Contains header level definitions for MPI Waitall Operations
 *
 */

#ifndef CUPCFD_COMM_MPI_WAITALL_IPP_H
#define CUPCFD_COMM_MPI_WAITALL_IPP_H

#include <cstdlib>

namespace cupcfd
{
	namespace comm
	{
		namespace mpi
		{
			cupcfd::error::eCodes WaitallMPI(MPI_Request * requests, int nRequests)
			{
				int err;

				MPI_Status * statuses = (MPI_Status *) malloc(sizeof(MPI_Status) * nRequests);

				err = MPI_Waitall(nRequests, requests, statuses);
				if(err != MPI_SUCCESS)
				{
					DEBUGGABLE_ERROR; return cupcfd::error::E_MPI_ERR;
				}
		
				free(statuses);

				return cupcfd::error::E_SUCCESS;
			}
		}
	}
}

#endif
