/*
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Contains definitions for the ExchangePatternTwoSidedNonBlocking class.
 */

#include "mpi.h"

#include "ExchangePatternTwoSidedNonBlocking.h"
#include "ExchangeMPI.h"
#include "WaitallMPI.h"
#include <iostream>

namespace cupcfd
{
	namespace comm
	{
		template <class T>
		ExchangePatternTwoSidedNonBlocking<T>::ExchangePatternTwoSidedNonBlocking() : ExchangePattern<T>()
		{
			this->nRequests = 0;
			this->nStatuses = 0;
			this->nSendBuffer = 0;
			this->nRecvBuffer = 0;
			this->nSendCounts = 0;
			this->nRecvCounts = 0;

			this->requests = nullptr;
			this->statuses = nullptr;
			this->sendBuffer = nullptr;
			this->recvBuffer = nullptr;
			this->sendCounts = nullptr;
			this->recvCounts = nullptr;
		}

		template <class T>
		ExchangePatternTwoSidedNonBlocking<T>::~ExchangePatternTwoSidedNonBlocking() {
			if(this->requests != nullptr) {
				free(this->requests);
			}

			if(this->statuses != nullptr) {
				free(this->statuses);
			}

			if(this->sendBuffer != nullptr) {
				free(this->sendBuffer);
			}

			if(this->recvBuffer != nullptr) {
				free(this->recvBuffer);
			}

			if(this->sendCounts != nullptr) {
				free(this->sendCounts);
			}

			if(this->recvCounts != nullptr) {
				free(this->recvCounts);
			}
		}

		template <class T>
		cupcfd::error::eCodes ExchangePatternTwoSidedNonBlocking<T>::init(cupcfd::comm::Communicator& comm,
				  int * mapLocalToExchangeIDX, int nMapLocalToExchangeIDX,
				  int * exchangeIDXSend, int nExchangeIDXSend,
				  int * tRanks, int nTRanks) {
			cupcfd::error::eCodes status;
			
			// Call the parent init function - it should not have been called by the constructor (else
			//it will get called twice)
			status = this->ExchangePattern<T>::init(comm, mapLocalToExchangeIDX, nMapLocalToExchangeIDX,
											exchangeIDXSend, nExchangeIDXSend,
											tRanks, nTRanks);
			CHECK_ECODE(status)

			// On top of the base initialisation, which sets up the exchange pattern,
			// we need to set aside space for the two-sided non-blocking data, such as requests.

			// Setup the requests and statuses arrays
			this->nRequests = this->nSProc + this->nRProc;
			this->requests = (MPI_Request *) malloc(sizeof(MPI_Request) * this->nRequests);

			this->nStatuses = this->nSProc + this->nRProc;
			this->statuses = (MPI_Status *) malloc(sizeof(MPI_Status) * this->nStatuses);

			// Setup the send buffer
			// Size is dependent upon how much data we are sending across all processes
			// nSAdjncy should be equivalent to the number of indexes associated with send.
			this->nSendBuffer = this->nSAdjncy;
			this->sendBuffer = (T *) malloc(sizeof(T) * this->nSendBuffer);

			// Setup the recv buffer
			// Size is dependent upon how much data we are receiving across all processes
			// nRAdjncy should be equivalent to the number of indexes associated with receives.
			this->nRecvBuffer = this->nRAdjncy;
			this->recvBuffer = (T *) malloc(sizeof(T) * this->nRecvBuffer);

			// Setup the send counts
			// Can be found from CSR arrays - saving them in a data store to avoid
			// having to recompute.
			this->nSendCounts = this->nSProc;
			this->sendCounts = (int *) malloc(sizeof(int) * this->nSendCounts);

			for(int i = 0; i < this->nSendCounts; i++) {
				this->sendCounts[i] = this->sXAdj[i+1] - this->sXAdj[i];
			}

			// Setup the recvCounts
			// Can be found from CSR arrays - saving them in a data store to avoid
			// having to recompute.
			this->nRecvCounts = this->nRProc;
			this->recvCounts = (int *) malloc(sizeof(int) * this->nRecvCounts);

			for(int i = 0; i < this->nRecvCounts; i++) {
				this->recvCounts[i] = this->rXAdj[i+1] - this->rXAdj[i];
			}

			return cupcfd::error::E_SUCCESS;
		}

		template <class T>
		cupcfd::error::eCodes ExchangePatternTwoSidedNonBlocking<T>::packSendBuffer(T * data, int nData) {
			// The data is already grouped by process and in rank order in the pattern CSR data.
			// We can just translate the exchange IDs to elements from the data array

			for(int i = 0; i < this->nSAdjncy; i++) {
				int exchangeID = this->sAdjncy[i];
				int localID = this->exchangeToLocal[exchangeID];

				#ifdef DEBUG
					if (localID < 0 || localID >= nData) {
						return cupcfd::error::E_INVALID_INDEX;
					}
				#endif

				this->sendBuffer[i] = data[localID];
			}

			return cupcfd::error::E_SUCCESS;
		}

		template <class T>
		cupcfd::error::eCodes ExchangePatternTwoSidedNonBlocking<T>::unpackRecvBuffer(T * data, int nData) {
			// Data received should be grouped by process in the recv buffer
			// It should already be ordered in the recv buffer as per the CSR for the pattern,
			// so we can transfer across by converting the indexes

			for(int i = 0; i < this->nRecvBuffer; i++) {
				int exchangeID = this->rAdjncy[i];
				int localID = this->exchangeToLocal[exchangeID];

				#ifdef DEBUG
					if (localID < 0 || localID >= nData) {
						return cupcfd::error::E_INVALID_INDEX;
					}
				#endif

				data[localID] = this->recvBuffer[i];
			}

			return cupcfd::error::E_SUCCESS;
		}

		template <class T>
		cupcfd::error::eCodes ExchangePatternTwoSidedNonBlocking<T>::exchangeStart(T * sourceData, int nData) {
			cupcfd::error::eCodes status;

			// Pack the buffer
			status = this->packSendBuffer(sourceData, nData);
			CHECK_ECODE(status)

			// Start the exchange
			status = cupcfd::comm::mpi::ExchangeVMPIIsendIrecv(this->sendBuffer, this->nSendBuffer,
																this->sendCounts, this->nSendCounts,
																this->recvBuffer, this->nRecvBuffer,
																this->recvCounts, this->nRecvCounts,
																this->sProc, this->nSProc,
																this->rProc, this->nRProc,
																this->comm.comm,
																this->requests, this->nRequests);
			CHECK_ECODE(status)

			return cupcfd::error::E_SUCCESS;
		}

		template <class T>
		cupcfd::error::eCodes ExchangePatternTwoSidedNonBlocking<T>::exchangeStop(T * sinkData, int nData) {
			cupcfd::error::eCodes status;

			// Complete any remaining data exchange
			status = cupcfd::comm::mpi::WaitallMPI(this->requests, this->nRequests);
			CHECK_ECODE(status)

			// Unpack the buffer
			status = this->unpackRecvBuffer(sinkData, nData);
			CHECK_ECODE(status)

			return cupcfd::error::E_SUCCESS;
		}
	}
}

// Explicit Instantiation
template class cupcfd::comm::ExchangePatternTwoSidedNonBlocking<int>;
template class cupcfd::comm::ExchangePatternTwoSidedNonBlocking<float>;
template class cupcfd::comm::ExchangePatternTwoSidedNonBlocking<double>;
