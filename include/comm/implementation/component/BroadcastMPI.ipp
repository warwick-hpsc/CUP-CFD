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
				MPI_Datatype dType;
				// Get the datatype based on the type of the dummy variable.
				// If a CustomMPIType, this could be unregistered.
				cupcfd::comm::mpi::getMPIType(buf[0], &dType);

				// MPI Broadcast
				int err = MPI_Bcast(buf, nEle, dType, sourcePID, comm);
				if(err != MPI_SUCCESS) {
					return cupcfd::error::E_MPI_ERR;
				}

				// Reached end without error
				return cupcfd::error::E_SUCCESS;
			}
		} // namespace mpi
	} // namespace comm
} // namespace cupcfd

#endif
