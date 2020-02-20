/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * A wrapper for a compressed sparse row representation of some data.
 *
 */

#ifndef INCLUDE_CUPCFD_COMM_ERROR_H
#define INCLUDE_CUPCFD_COMM_ERROR_H

namespace cupcfd
{
	namespace comm
	{
		enum eCodes {E_SCATTER_SUCCESS,
					 E_BROADCAST_MPI_ERR,
					 E_BROADCAST_SUCCESS,
					 E_GATHER_MPI_ERR,
					 E_GATHER_SUCCESS,
					 E_GATHER_NULL_PTR,
					 E_GATHER_UNDERSIZED_RECV_BUFFER,
					 E_BARRIER_SUCCESS,
					 E_REDUCE_SUCCESS,
					 E_GATHER_NOT_NULL_PTR,
					 E_ALLTOALL_MPI_ERR,
					 E_ALLTOALL_MPI_SUCCESS,
					 E_ALLTOALL_SUCCESS,
					 E_ALLTOALL_UNDERSIZED_SEND_BUFFER,
					 E_ALLTOALL_UNDERSIZED_RECV_BUFFER,
					 E_EXCHANGE_MPI_SUCCESS,
					 E_EXCHANGE_MPI_ERR,
					 E_WAITALL_MPI_SUCCESS,
					 E_WAITALL_MPI_ERR,
		};
	}
}

#endif
