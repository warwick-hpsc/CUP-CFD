/*
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 */
#ifndef CUPCFD_COMM_EXCHANGE_MPI_IPP_H
#define CUPCFD_COMM_EXCHANGE_MPI_IPP_H

#include "MPIUtility.h"
#include <cstdlib>

namespace cupcfd
{
	namespace comm
	{
		namespace mpi
		{
			template <class T>
			cupcfd::error::eCodes ExchangeMPIIsendIrecv(T * sendBuffer, int nSendBuffer,
															 T * recvBuffer, int nRecvBuffer,
															 int * tRanks, int nTRanks,
															 int elePerRank,
															 MPI_Comm comm,
															 MPI_Request ** requests, int * nRequests) {
				if (nSendBuffer < (nTRanks*elePerRank)) {
					return cupcfd::error::E_ARRAY_SIZE_UNDERSIZED;
				}
				if (nRecvBuffer < (nTRanks*elePerRank)) {
					return cupcfd::error::E_ARRAY_SIZE_UNDERSIZED;
				}

				int err;
				T dummy;
				MPI_Datatype dType;
				int offset;
				cupcfd::error::eCodes status;

				// Get the datatype based on the type of the dummy variable
				status = cupcfd::comm::mpi::getMPIType(dummy, &dType);
				CHECK_ERROR_CODE(status)
				if(status != cupcfd::error::E_SUCCESS) return status;
				
				int tag = 78;

				*nRequests = nTRanks * 2;
				*requests = (MPI_Request *) malloc(sizeof(MPI_Request) * (*nRequests));

				// Initiate the irecvs with all neighbour ranks specified
				offset = 0;
				for(int i = 0; i < nTRanks; i++) {
					err = MPI_Irecv(recvBuffer + offset, elePerRank, dType, tRanks[i], tag, comm, (*requests) + i);
					offset += elePerRank;
					
					if(err != MPI_SUCCESS) {
						return cupcfd::error::E_MPI_ERR;
					}
				}

				// Initiate the isends with all the neighbour ranks specified
				offset = 0;
				for(int i = 0; i < nTRanks; i++) {
					err = MPI_Isend(sendBuffer + offset, elePerRank, dType, tRanks[i], tag, comm, (*requests) + nTRanks + i);
					offset += elePerRank;
					
					if(err != MPI_SUCCESS) {
						return cupcfd::error::E_MPI_ERR;
					}
				}

				return cupcfd::error::E_SUCCESS;
			}


			template <class T>
			cupcfd::error::eCodes ExchangeVMPIIsendIrecv(T * sendBuffer, int nSendBuffer,
															 int * sendCount, int nSendCount,
															 T * recvBuffer, int nRecvBuffer,
															 int * recvCount, int nRecvCount,
															 int * sRanks, int nSRanks,
															 int * rRanks, int nRRanks,
															 MPI_Comm comm,
															 MPI_Request ** requests, int * nRequests) {
				if (nSendCount < nSRanks) {
					return cupcfd::error::E_ARRAY_SIZE_UNDERSIZED;
				}
				int sendSize = 0;
				for(int i = 0; i < nSRanks; i++) {
					if(sendCount[i] > 0) {
						sendSize += sendCount[i];
					}
				}
				if (nSendBuffer < sendSize) {
					return cupcfd::error::E_ARRAY_SIZE_UNDERSIZED;
				}

				if (nRecvCount < nRRanks) {
					return cupcfd::error::E_ARRAY_SIZE_UNDERSIZED;
				}
				int recvSize = 0;
				for(int i = 0; i < nRRanks; i++) {
					if(recvCount[i] > 0) {
						recvSize += recvCount[i];
					}
				}
				if (nRecvBuffer < recvSize) {
					return cupcfd::error::E_ARRAY_SIZE_UNDERSIZED;
				}

				int err;
				int offset;
				int reqPtr;
				T dummy;
				MPI_Datatype dType;
				cupcfd::error::eCodes status;

				// Get the datatype based on the type of the dummy variable
				status = cupcfd::comm::mpi::getMPIType(dummy, &dType);
				CHECK_ERROR_CODE(status)
				if(status != cupcfd::error::E_SUCCESS) return status;
				
				int tag = 79;

				*nRequests = 0;
				for(int i = 0; i < nRRanks; i++) {
					if(recvCount[i] > 0) {
						(*nRequests)++;
					}
				}
				
				for(int i = 0; i < nSRanks; i++) {
					if(sendCount[i] > 0) {
						(*nRequests)++;
					}
				}

				*requests = (MPI_Request *) malloc(sizeof(MPI_Request) * (*nRequests));


				// Init the irecvs

				offset = 0;
				reqPtr = 0;
				for(int i = 0; i < nRRanks; i++) {
					if(recvCount[i] > 0) {
						err = MPI_Irecv(recvBuffer + offset, recvCount[i], dType, rRanks[i], tag, comm, (*requests) + reqPtr);
						offset = offset + recvCount[i];
						reqPtr++;
						
						if(err != MPI_SUCCESS) {
							return cupcfd::error::E_MPI_ERR;
						}
					}
				}

				// Initiate the isends
				offset = 0;
				for(int i = 0; i < nSRanks; i++) {
					if(sendCount[i] > 0) {					
						err = MPI_Isend(sendBuffer + offset, sendCount[i], dType, sRanks[i], tag, comm, (*requests) + reqPtr);
						offset += sendCount[i];
						reqPtr++;
						
						if(err != MPI_SUCCESS) {
							return cupcfd::error::E_MPI_ERR;
						}
					}
				}

				return cupcfd::error::E_SUCCESS;
			}



			template <class T>
			cupcfd::error::eCodes ExchangeVMPIIsendIrecv(T * sendBuffer, int nSendBuffer,
															 int * sendCount, int nSendCount,
															 T * recvBuffer, int nRecvBuffer,
															 int * recvCount, int nRecvCount,
															 int * sRanks, int nSRanks,
															 int * rRanks, int nRRanks,
															 MPI_Comm comm,
															 MPI_Request * requests, int nRequests) {
				if (nSendCount < nSRanks) {
					return cupcfd::error::E_ARRAY_SIZE_UNDERSIZED;
				}
				int sendSize = 0;
				int sendCountActual = 0;
				for(int i = 0; i < nSRanks; i++) {
					if(sendCount[i] > 0) {
						sendSize += sendCount[i];
						sendCountActual++;
					}
				}
				if (nSendBuffer < sendSize) {
					return cupcfd::error::E_ARRAY_SIZE_UNDERSIZED;
				}
				if (nRequests < sendCountActual) {
					return cupcfd::error::E_ARRAY_SIZE_UNDERSIZED;
				}

				if (nRecvCount < nRRanks) {
					return cupcfd::error::E_ARRAY_SIZE_UNDERSIZED;
				}
				int recvSize = 0;
				int recvCountActual = 0;
				for(int i = 0; i < nRRanks; i++) {
					if(recvCount[i] > 0) {
						recvSize += recvCount[i];
						recvCountActual++;
					}
				}
				if (nRecvBuffer < recvSize) {
					return cupcfd::error::E_ARRAY_SIZE_UNDERSIZED;
				}
				if (nRequests < recvCountActual) {
					return cupcfd::error::E_ARRAY_SIZE_UNDERSIZED;
				}

				int err;
				int offset;
				int reqPtr;
				T dummy;
				MPI_Datatype dType;
				cupcfd::error::eCodes status;

				// Get the datatype based on the type of the dummy variable
				status = cupcfd::comm::mpi::getMPIType(dummy, &dType);
				CHECK_ERROR_CODE(status)
				if(status != cupcfd::error::E_SUCCESS) return status;

				int tag = 79;

				// Init the irecvs

				offset = 0;
				reqPtr = 0;

				int rank;
				MPI_Comm_rank(comm, &rank);
				
				for(int i = 0; i < nRRanks; i++) {			
					if(recvCount[i] > 0) {
						err = MPI_Irecv(recvBuffer + offset, recvCount[i], dType, rRanks[i], tag, comm, requests + reqPtr);

						offset += recvCount[i];
						reqPtr++;
						
						if(err != MPI_SUCCESS) {
							return cupcfd::error::E_MPI_ERR;
						}
					}
				}

				// Initiate the isends
				offset = 0;
				for(int i = 0; i < nSRanks; i++) {
					if(sendCount[i] > 0) {
						err = MPI_Isend(sendBuffer + offset, sendCount[i], dType, sRanks[i], tag, comm, requests + reqPtr);

						offset += sendCount[i];
						reqPtr++;
						
						if(err != MPI_SUCCESS) {
							return cupcfd::error::E_MPI_ERR;
						}
					}
				}

				return cupcfd::error::E_SUCCESS;
			}


/*
			template <class T>
			cupcfd::error::eCodes ExchangeVMPIPut(T * sendBuffer, int nSendBuffer,
													  int * sendCount, int nSendCount,
													  T * recvBuffer, int nRecvBuffer,
													  int * recvCount, int nRecvCount,
													  int * tRanks, int nTRanks,
													  MPI_Comm comm,
													  MPI_Request ** requests, int * nRequests) {
				int err;
				int offset;
				int reqPtr;

				T dummy;
				MPI_Datatype dType;
				cupcfd::error::eCodes status;

				// Get the datatype based on the type of the dummy variable
				status = cupcfd::comm::mpi::getMPIType(dummy, &dType);
				CHECK_ERROR_CODE(status)
				if(status != cupcfd::error::E_SUCCESS) return status;
				
				int tag = 79;

				*nRequests = 0;
				for(int i = 0; i < nTRanks; i++) {
					if(recvCount[i] > 0) {
						*nRequests = *nRequests + 1;
					}

					if(sendCount[i] > 0) {
						*nRequests = *nRequests + 1;
					}
				}

				*requests = (MPI_Request *) malloc(sizeof(MPI_Request) * (*nRequests));


				// Init the irecvs

				offset = 0;
				reqPtr = 0;
				for(int i = 0; i < nTRanks; i++) {
					if(recvCount[i] > 0) {
						MPI_Irecv(recvBuffer + offset, recvCount[i], dType, tRanks[i], tag, comm, (*requests) + reqPtr);
						offset = offset + recvCount[i];
						reqPtr = reqPtr + 1;
					}
				}

				// Initiate the isends
				offset = 0;
				for(int i = 0; i < nTRanks; i++) {
					if(sendCount[i] > 0) {
						MPI_Isend(sendBuffer + offset, sendCount[i], dType, tRanks[i], tag, comm, (*requests) + reqPtr);
						offset = offset + sendCount[i];
						reqPtr = reqPtr + 1;
					}
				}
				
				return cupcfd::error::E_SUCCESS;
			}
			*/
		}
	}
}

#endif
