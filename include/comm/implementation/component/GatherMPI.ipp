/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Contains header level definitions for MPI gather operations
 */

#ifndef CUPCFD_COMM_MPI_GATHER_IPP_H
#define CUPCFD_COMM_MPI_GATHER_IPP_H

#include "MPIUtility.h"
#include <cstdlib>

namespace cupcfd
{
	namespace comm
	{
		namespace mpi
		{
			template <class T>
			cupcfd::error::eCodes GatherMPI(T * bSend, T * bRecv, int nElePerProcess, int sinkProcess, MPI_Comm comm)
			{
				T dummy;
				MPI_Datatype dType;
				cupcfd::error::eCodes status;
				int mpiErr;
				
				status = cupcfd::comm::mpi::getMPIType(dummy, &dType);
				if(status != cupcfd::error::E_SUCCESS)
				{
					return status;
				}

				// Each process sends nElePerProcess, and the sink process receieves nElePerProcess
				// for a single receive from any one process
				mpiErr = MPI_Gather(bSend, nElePerProcess, dType, bRecv, nElePerProcess, dType, sinkProcess, comm);
				if(mpiErr != MPI_SUCCESS)
				{
					return cupcfd::error::E_MPI_ERR;
				}

				return cupcfd::error::E_SUCCESS;
			}

			template <class T>
			cupcfd::error::eCodes AllGatherMPI(T * bSend, T * bRecv, int nElePerProcess, MPI_Comm comm)
			{
				T dummy;
				MPI_Datatype dType;
				cupcfd::error::eCodes status;
				int mpiErr;
				
				status = cupcfd::comm::mpi::getMPIType(dummy, &dType);
				if(status != cupcfd::error::E_SUCCESS)
				{
					return status;
				}

				// Each process sends nElePerProcess, and each process receives nElePerProcess
				// for a single receive from any one process (nElePerProcess * process count in total)
				mpiErr = MPI_Allgather(bSend, nElePerProcess, dType, bRecv, nElePerProcess, dType, comm);
				if(mpiErr != MPI_SUCCESS)
				{
					return cupcfd::error::E_MPI_ERR;
				}
				
				return cupcfd::error::E_SUCCESS;
			}

			template <class T>
			cupcfd::error::eCodes GatherVMPI(T * bSend, int nEleSend, T * bRecv, int * bRecvCounts, int sinkPID, MPI_Comm comm)
			{
				T dummy;
				MPI_Datatype dType;
				cupcfd::error::eCodes status;
				int mpiErr;
				
				status = cupcfd::comm::mpi::getMPIType(dummy, &dType);
				if(status != cupcfd::error::E_SUCCESS)
				{
					return status;
				}

				int commSize;
				int commRank;

				mpiErr = MPI_Comm_size(comm, &commSize);
				if(mpiErr != MPI_SUCCESS)
				{
					return cupcfd::error::E_MPI_ERR;
				}
				
				mpiErr = MPI_Comm_rank(comm, &commRank);
				if(mpiErr != MPI_SUCCESS)
				{
					return cupcfd::error::E_MPI_ERR;
				}

				int * displs = nullptr;

				// Compute Displacements from expected recv counts for each process (if this is the sink process)
				if(commRank == sinkPID)
				{

					displs = (int *) malloc(sizeof(int) * commSize);
					displs[0] = 0;

					// Cumulative sum based on previous value to get displacements
					for(int i = 1; i < commSize; i++)
					{
						displs[i] = displs[i-1] + bRecvCounts[i-1];
					}
				}

				mpiErr = MPI_Gatherv(bSend, nEleSend, dType, bRecv, bRecvCounts, displs, dType, sinkPID, comm);
				if(mpiErr != MPI_SUCCESS)
				{
					return cupcfd::error::E_MPI_ERR;
				}
				
				// Cleanup
				if(displs != nullptr)
				{
					free(displs);
				}
				
				return cupcfd::error::E_SUCCESS;
			}

			template <class T>
			cupcfd::error::eCodes AllGatherVMPI(T * bSend, int nEleSend, T * bRecv, int * bRecvCounts, MPI_Comm comm)
			{
				T dummy;
				MPI_Datatype dType;
				cupcfd::error::eCodes status;
				int mpiErr;
				
				status = cupcfd::comm::mpi::getMPIType(dummy, &dType);
				if(status != cupcfd::error::E_SUCCESS)
				{
					return status;
				}

				int commSize;
				int commRank;

				mpiErr = MPI_Comm_size(comm, &commSize);
				if(mpiErr != MPI_SUCCESS)
				{
					return cupcfd::error::E_MPI_ERR;
				}
				
				mpiErr = MPI_Comm_rank(comm, &commRank);
				if(mpiErr != MPI_SUCCESS)
				{
					return cupcfd::error::E_MPI_ERR;
				}

				// Compute Displacements from expected recv counts for each process
				int * displs = (int *) malloc(sizeof(int) * commSize);
				displs[0] = 0;

				// Cumulative sum based on previous value to get displacements
				for(int i = 1; i < commSize; i++)
				{
					displs[i] = displs[i-1] + bRecvCounts[i-1];
				}

				mpiErr = MPI_Allgatherv(bSend, nEleSend, dType, bRecv, bRecvCounts, displs, dType, comm);
				if(mpiErr != MPI_SUCCESS)
				{
					return cupcfd::error::E_MPI_ERR;
				}

				// Cleanup - all ranks will have a displacement array
				free(displs);
				
				return cupcfd::error::E_SUCCESS;
			}
		} // namespace mpi
	} // namespace comm
} // namespace cupcfd

#endif
