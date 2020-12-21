/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Contains the declarations for Barrier Interface operations.
 */

#ifndef CUPCFD_COMM_BARRIER_INCLUDE_H
#define CUPCFD_COMM_BARRIER_INCLUDE_H

#include "Communicator.h"
#include "Error.h"

namespace cupcfd
{
	namespace comm
	{
		/**
		 * Interface for performing a barrier on all processes in mpComm.
		 *
		 * @param mpComm The communicator containing the ranks involved in the barrier.
		 */
		__attribute__((warn_unused_result))
		cupcfd::error::eCodes Barrier(cupcfd::comm::Communicator& mpComm);

	} // namespace comm
} // namespace cupcfd

// Include Header Level Definitions
#include "Barrier.ipp"

#endif
