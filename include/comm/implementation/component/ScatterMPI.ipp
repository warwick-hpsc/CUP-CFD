/*
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Contains the header level definitions for Scatter MPI Operations.
 */

#ifndef CUPCFD_COMM_MPI_SCATTER_IPP_H
#define CUPCFD_COMM_MPI_SCATTER_IPP_H

#include <cstdlib>

#include "MPIUtility.h"

namespace cupcfd
{
	namespace comm
	{
		namespace mpi
		{
			template <class T>
			cupcfd::error::eCodes ScatterMPI(T * bufferSend, int nSend, T * bufferRecv, int nRecv, int sourcePID, MPI_Comm comm) {
				T dummy;
				MPI_Datatype dTypeSend;
				MPI_Datatype dTypeRecv;
				int err;
				cupcfd::error::eCodes status;

				// Get the datatype based on the type of the dummy variable
				cupcfd::comm::mpi::getMPIType(dummy, &dTypeSend);
				
				// Send and recv types should be the same
				dTypeRecv = dTypeSend;

				// Use the MPI library for the distribution
				err = MPI_Scatter(bufferSend, nSend, dTypeSend, bufferRecv, nRecv, dTypeRecv, sourcePID, comm);
				if(err != MPI_SUCCESS) {
					return cupcfd::error::E_MPI_ERR;
				}
				
				// If received this point, presumed to be successful
				return cupcfd::error::E_SUCCESS;
			}

			template <class T>
			cupcfd::error::eCodes ScatterVMPI(T * bufferSend, int nSend, T * bufferRecv, int nRecv, int * sendCounts, int nSendCounts, int sourcePID, MPI_Comm comm) {
				int commSize, commRank;
				int err = MPI_Comm_size(comm, &commSize);
				if(err != MPI_SUCCESS) {
					return cupcfd::error::E_MPI_ERR;
				}
				err = MPI_Comm_rank(comm, &commRank);
				if(err != MPI_SUCCESS) {
					return cupcfd::error::E_MPI_ERR;
				}

				if (commRank == sourcePID) {
					if (commSize != nSendCounts) {
						return cupcfd::error::E_ARRAY_SIZE_MISMATCH;
					}
					int nSendActual = 0;
					for (int i=0; i<commSize; i++) {
						nSendActual += sendCounts[i];
					}
					if (nSendActual != nSend) {
						return cupcfd::error::E_ARRAY_SIZE_MISMATCH;
					}
				} else {
					if (nRecv != sendCounts[commRank]) {
						return cupcfd::error::E_ARRAY_SIZE_MISMATCH;
					}
				}

				T dummy;
				MPI_Datatype dTypeSend;
				MPI_Datatype dTypeRecv;
				cupcfd::error::eCodes status;
				
				int * displs = nullptr;
				
				// Get the datatype based on the type of the dummy variable
				cupcfd::comm::mpi::getMPIType(dummy, &dTypeSend);
				
				// Send and recv types should be the same
				dTypeRecv = dTypeSend;

				// bufferSend and sendCounts can only be presumed to be non-null on the source process
				// ToDo: Displacement computation could/should maybe be moved into the interface level operation, not the MPI one.
				// This would also allow us to use the stored rank/size rather than polling the mpi library.
				if(commRank == sourcePID) {
					displs = (int *) malloc(sizeof(int) * commSize);
					displs[0] = 0;
					for(int i = 1; i < commSize; i++) {
						displs[i] = displs[i-1] + sendCounts[i-1];
					}
				}

				err = MPI_Scatterv(bufferSend, sendCounts, displs, dTypeSend, bufferRecv, nRecv, dTypeRecv, sourcePID, comm);
				if(err != MPI_SUCCESS) {
					return cupcfd::error::E_MPI_ERR;
				}

				if(displs != nullptr) {
					free(displs);
				}

				// If this point is reached, presumed to be successful
				return cupcfd::error::E_SUCCESS;
			}
		} // namespace mpi
	} // namespace comm
} // namespace cupcfd

#endif
