/*
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Contains templated implementations for the wrappers
 * that interface with the MPI Broadcast routines.
 *
 */

#ifndef CUPCFD_COMM_BROADCAST_MPI_IPP_H
#define CUPCFD_COMM_BROADCAST_MPI_IPP_H

#include "MPIUtility.h"

namespace cupcfd
{
	namespace comm
	{
		namespace mpi
		{
			template <class T>
			cupcfd::error::eCodes BroadcastMPI(T * buf, int nEle, int sourcePID, MPI_Comm comm) {
				if (nEle == 0) {
					return cupcfd::error::E_NO_DATA;
				}

				cupcfd::error::eCodes status;
				int mpi_err;
				MPI_Datatype dType;
				#pragma GCC diagnostic push
				#pragma GCC diagnostic ignored "-Wuninitialized"
				T dummy;
				status = cupcfd::comm::mpi::getMPIType(dummy, &dType);
				CHECK_ECODE(status)
				#pragma GCC diagnostic pop

				// MPI Broadcast
				mpi_err = MPI_Bcast(buf, nEle, dType, sourcePID, comm);
				if(mpi_err != MPI_SUCCESS) {
					return cupcfd::error::E_MPI_ERR;
				}

				// Reached end without error
				return cupcfd::error::E_SUCCESS;
			}
		} // namespace mpi
	} // namespace comm
} // namespace cupcfd

#endif
