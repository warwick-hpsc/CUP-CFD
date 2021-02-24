/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Contains the header level definitions for Barrier Interface operations.
 */

#ifndef CUPCFD_COMM_BARRIER_IPP_H
#define CUPCFD_COMM_BARRIER_IPP_H

#include "BarrierMPI.h"

namespace cupcfd
{
	namespace comm
	{
		cupcfd::error::eCodes Barrier(cupcfd::comm::Communicator& mpComm) {
			// Currently just a simple wrapper to using the MPI operation
			return cupcfd::comm::mpi::BarrierMPI(mpComm.comm);
		}
	} // namespace comm
} // namespace cupcfd

#endif
