/*
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Contains the header level definitions for All To All MPI Operations.
 */

#ifndef CUPCFD_COMM_MPI_ALLTOALL_IPP_H
#define CUPCFD_COMM_MPI_ALLTOALL_IPP_H

// Library Operations
#include <cstdlib>

// Provides access to MPI utility functions such as retrieving MPI types
#include "MPIUtility.h"

namespace cupcfd
{
	namespace comm
	{
		namespace mpi
		{
			template <class T>
			cupcfd::error::eCodes AllToAllMPI(T * sendbuf, int sendcount,
												  T * recvbuf, int recvcount,
												  MPI_Comm comm) {
				// MPI Error Status
				int err;

				// Create a dummy variable of type T so we can select the correct
				// specialisation of getMPIType
				T dummy;

				// Retrieve the MPI Datatype for T
				MPI_Datatype dType;
				cupcfd::comm::mpi::getMPIType(dummy, &dType);

				// MPI Call
				err = MPI_Alltoall(sendbuf, sendcount, dType,
								   recvbuf, recvcount, dType,
								   comm);

				// Error Status Check
				if(err != MPI_SUCCESS) {
					return cupcfd::error::E_MPI_ERR;
				}

				return cupcfd::error::E_SUCCESS;
			}

			template <class T>
			cupcfd::error::eCodes AllToAllVMPI(T * sendbuf, int * sendcounts,
												   T * recvbuf, int *recvcounts,
												   MPI_Comm comm) {
				int err;
				int commSize;
				cupcfd::error::eCodes status;

				err = MPI_Comm_size(comm, &commSize);
				if(err != MPI_SUCCESS) {
					return cupcfd::error::E_MPI_ERR;
				}
				
				// Compute Displacements from send/recv counts
				int * sdispls = (int *) malloc(sizeof(int) * commSize);
				int * rdispls = (int *) malloc(sizeof(int) * commSize);

				sdispls[0] = 0;
				rdispls[0] = 0;

				for(int i = 1; i < commSize; i++) {
					sdispls[i] = sdispls[i-1] + sendcounts[i-1];
					rdispls[i] = rdispls[i-1] + recvcounts[i-1];
				}

				status = AllToAllVMPI(sendbuf, sendcounts, sdispls,
								   recvbuf, recvcounts, rdispls,
								   comm);
				CHECK_ECODE(status)			   
				
				// Cleanup
				free(sdispls);
				free(rdispls);

				return cupcfd::error::E_SUCCESS;
			}

			template <class T>
			cupcfd::error::eCodes AllToAllVMPI(T * sendbuf, int * sendcounts, int *sdispls,
												   T * recvbuf, int *recvcounts, int *rdispls,
												   MPI_Comm comm) {
				int err;
				T dummy;
				MPI_Datatype dType;
				
				// Retrieve the MPI Datatype for T
				cupcfd::comm::mpi::getMPIType(dummy, &dType);

				err = MPI_Alltoallv(sendbuf, sendcounts, sdispls, dType,
							  recvbuf, recvcounts, rdispls, dType,
							  comm);

				if(err != MPI_SUCCESS) {
					return cupcfd::error::E_MPI_ERR;
				}

				return cupcfd::error::E_SUCCESS;
			}
		}
	}
}

#endif
