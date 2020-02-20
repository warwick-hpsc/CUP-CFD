/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Contains Header Level Definitions for Gather Interface Operations
 */

#ifndef CUPCFD_COMM_GATHER_IPP_H
#define CUPCFD_COMM_GATHER_IPP_H

#include "GatherMPI.h"
#include "StatisticsDrivers.h"

#include <iostream>

namespace cupcfd
{
	namespace comm
	{
		template <class T>
		cupcfd::error::eCodes Gather(T * bSend, int nBSend, T * bRecv, int nBRecv, int nElePerProcess, int sinkProcess, cupcfd::comm::Communicator& mpComm)
		{
			cupcfd::error::eCodes status;
		
			// Error Check: bRecv cannot be null (sink process only)
			if((bRecv == nullptr || bRecv == NULL) && mpComm.rank == sinkProcess)
			{
				return cupcfd::error::E_NULL_PTR;
			}

			// Error Check: Verify recv buffer size is appropriate (sink process only)
			// Naturally, we're trusting the callee to accurately report the buffer size
			if((nBRecv < (nElePerProcess * mpComm.size)) && mpComm.rank == sinkProcess)
			{
				return cupcfd::error::E_ARRAY_SIZE_UNDERSIZED;
			}

			// Call Suitable Communication Library Driver (in this case, MPI)
			status = cupcfd::comm::mpi::GatherMPI(bSend, bRecv, nElePerProcess, sinkProcess, mpComm.comm);
			
			if(status != cupcfd::error::E_SUCCESS)
			{
				return status;
			}

			return cupcfd::error::E_SUCCESS;
		}

		template <class T>
		cupcfd::error::eCodes AllGather(T * bSend, int nBSend, T * bRecv, int nBRecv, int nElePerProcess, cupcfd::comm::Communicator& mpComm)
		{
			cupcfd::error::eCodes status;
					
			// Error Check: bRecv cannot be null
			if(bRecv == nullptr || bRecv == NULL)
			{
				return cupcfd::error::E_NULL_PTR;
			}
			
			// Error Check: Verify recv buffer size is appropriate (sink process only)
			// Naturally, we're trusting the callee to accurately report the buffer size
			if(nBRecv < (nElePerProcess * mpComm.size))
			{
				return cupcfd::error::E_ARRAY_SIZE_UNDERSIZED;
			}

			// Call Suitable Communication Library Driver (in this case, MPI)
			status = cupcfd::comm::mpi::AllGatherMPI(bSend, bRecv, nElePerProcess, mpComm.comm);

			if(status != cupcfd::error::E_SUCCESS)
			{
				return status;
			}

			return cupcfd::error::E_SUCCESS;
		}

		// ===============================================================================================================

		template <class T>
		cupcfd::error::eCodes GatherV(T * bSend, int nBSend, T * bRecv, int nBRecv, int * bRecvCounts, int nBRecvCounts, int sinkProcess, cupcfd::comm::Communicator& mpComm)
		{
			cupcfd::error::eCodes status;
		
			// Error Check: bRecv cannot be null (sink process only)
			if((bRecv == nullptr || bRecv == NULL) && mpComm.rank == sinkProcess)
			{
				return cupcfd::error::E_NULL_PTR;
			}

			// Call Suitable Communication Library Driver (in this case, MPI)
			status = cupcfd::comm::mpi::GatherVMPI(bSend, nBSend, bRecv, bRecvCounts, sinkProcess, mpComm.comm);

			if(status != cupcfd::error::E_SUCCESS)
			{
				return status;
			}
			
			return cupcfd::error::E_SUCCESS;
		}

		// ===============================================================================================================

		template <class T>
		cupcfd::error::eCodes GatherV(T * bSend, int nBSend, T ** bRecv, int * nBRecv, int ** bRecvCounts, int * nBRecvCounts, int sinkPID, cupcfd::comm::Communicator& mpComm)
		{
			// The bulk of the gather works is passed through to a different gather driver.
			// However, this driver is responsible for setting up the receive buffer on the sink process before the full gather can go ahead.
			// This requires:
			// (a) Data gathering for how many elements the sinkPID will receieve from each process (i.e. setup bRecvCounts)
			// (b) Constructing a suitably sized array for receiving once the total number of elements is known.
			// Both of these can be achieved by the knowledge of how large each send array is from each process. However,
			// since the array sizes are variable and on remote processes, we must obtain this data first via a single element gather

			// Note: We only need to know the total number of elements to setup the receiving buffer.
			// This could be achieved via a reduce. However, we would lose knowledge of how many elements came from which process, as reported
			// in bRecvCounts, which is needed for the final gather.

			cupcfd::error::eCodes status;

			// Error Checks:
			// We expect *bRecv, *bRecvCounts to be NULL or nullptr on all processes.
			if(*bRecv != NULL && *bRecv != nullptr)
			{
				return cupcfd::error::E_NOT_NULL_PTR;
			}

			if(*bRecvCounts != NULL && *bRecvCounts != nullptr)
			{
				return cupcfd::error::E_NOT_NULL_PTR;
			}

			// Buffers for receiving will only be setup on the sink process
			// They will remain NULL/nullptr on other processes

			// First, setup the array to store the send buffer sizes
			// It must be the same size as the number of processes

			if(mpComm.rank == sinkPID)
			{
				*nBRecvCounts = mpComm.size;
				*bRecvCounts = (int *) malloc(sizeof(int) * *nBRecvCounts);
			}

			// Gather on the send buffer sizes (single-element fixed-size gather)
			Gather(&nBSend, 1, *bRecvCounts, *nBRecvCounts, 1, sinkPID, mpComm);

			// Now bRecvCounts is known, we can setup the receive buffer
			if(mpComm.rank == sinkPID)
			{
				// Get the total element count
				cupcfd::utility::drivers::sum(*bRecvCounts, *nBRecvCounts, nBRecv);

				// Allocate memory for the array
				*bRecv = (T *) malloc(sizeof(T) * *nBRecv);
			}

			// Setup complete, pass final gather work onto next driver
			status = GatherV(bSend, nBSend, *bRecv, *nBRecv, *bRecvCounts, *nBRecvCounts, sinkPID, mpComm);
			
			if(status != cupcfd::error::E_SUCCESS)
			{
				return status;
			}
			
			return cupcfd::error::E_SUCCESS;
		}

		// ===============================================================================================================

		template <class T>
		cupcfd::error::eCodes AllGatherV(T * bSend, int nBSend, T * bRecv, int nBRecv, int * bRecvCounts, int nBRecvCounts, cupcfd::comm::Communicator& mpComm)
		{
			cupcfd::error::eCodes status;
		
			// Call Suitable Communication Library Driver (in this case, MPI)
			status = cupcfd::comm::mpi::AllGatherVMPI(bSend, nBSend, bRecv, bRecvCounts, mpComm.comm);

			if(status != cupcfd::error::E_SUCCESS)
			{
				return status;
			}
			
			return cupcfd::error::E_SUCCESS;
		}	
		
		// ===============================================================================================================
		
		template <class T>
		cupcfd::error::eCodes AllGatherV(T * bSend, int nBSend, T ** bRecv, int * nBRecv, int ** bRecvCounts, int * nBRecvCounts, cupcfd::comm::Communicator& mpComm)
		{
			// The bulk of the allgather works is passed through to a different allgather driver.
			// However, this driver is responsible for setting up the receive buffer on the sink process before the full gather can go ahead.
			// This requires:
			// (a) Data gathering for how many elements the process will receieve from each process (i.e. setup bRecvCounts)
			// (b) Constructing a suitably sized array for receiving once the total number of elements is known.
			// Both of these can be achieved by the knowledge of how large each send array is from each process. However,
			// since the array sizes are variable and on remote processes, we must obtain this data first via a single element gather

			// Note: We only need to know the total number of elements to setup the receiving buffer.
			// This could be achieved via a reduce. However, we would lose knowledge of how many elements came from which process, as reported
			// in bRecvCounts, which is needed for the final gather.

			// ToDo: This functionality is essentially the same as in the gather unknown size variant, and so could be moved out into
			// a separate utility function (other than we need to allgather rather than gather the sizes at the end)

			cupcfd::error::eCodes status;

			// Error Checks:
			// We expect *bRecv, *bRecvCounts to be NULL or nullptr on all processes.
			if(*bRecv != NULL && *bRecv != nullptr)
			{
				return cupcfd::error::E_NOT_NULL_PTR;
			}

			if(*bRecvCounts != NULL && *bRecvCounts != nullptr)
			{
				return cupcfd::error::E_NOT_NULL_PTR;
			}

			// Buffers for receiving will only be setup on the sink process
			// They will remain NULL/nullptr on other processes

			// First, setup the array to store the send buffer sizes
			// It must be the same size as the number of processes
			*nBRecvCounts = mpComm.size;
			*bRecvCounts = (int *) malloc(sizeof(int) * *nBRecvCounts);

			// Gather on the send buffer sizes (single-element fixed-size gather)
			AllGather(&nBSend, 1, *bRecvCounts, *nBRecvCounts, 1, mpComm);

			// Now bRecvCounts is known, we can setup the receive buffer
			// Get the total element count
			cupcfd::utility::drivers::sum(*bRecvCounts, *nBRecvCounts, nBRecv);

			// Allocate memory for the array
			*bRecv = (T *) malloc(sizeof(T) * *nBRecv);
			
			// Setup complete, pass final allgather work onto next driver
			status = AllGatherV(bSend, nBSend, *bRecv, *nBRecv, *bRecvCounts, *nBRecvCounts, mpComm);
			
			if(status != cupcfd::error::E_SUCCESS)
			{
				return status;
			}
			
			return cupcfd::error::E_SUCCESS;
		}
	}
}

#endif
