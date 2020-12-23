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

#include "ExchangePatternOneSidedNonBlocking.h"
#include "MPIUtility.h"
#include "AllToAll.h"
#include "Communicator.h"

namespace cupcfd
{
	namespace comm
	{
		template <class T>
		ExchangePatternOneSidedNonBlocking<T>::ExchangePatternOneSidedNonBlocking()
		: ExchangePattern<T>()
		{
			// Set counts to zero
			this->nSendBuffer = 0;
			this->nSendCounts = 0;
			this->nRecvCounts = 0;
			this->nTargetDispls = 0;

			// Set array pointers to nullptr so it is known that they are
			// not yet allocated
			this->sendBuffer = nullptr;
			this->sendCounts = nullptr;
			this->recvCounts = nullptr;
			this->targetDispls = nullptr;
		}

		template <class T>
		ExchangePatternOneSidedNonBlocking<T>::~ExchangePatternOneSidedNonBlocking() {
			if(this->sendBuffer != nullptr) {
				free(this->sendBuffer);
			}

			if(this->sendCounts != nullptr) {
				free(this->sendCounts);
			}

			if(this->recvCounts != nullptr) {
				free(this->recvCounts);
			}

			if(this->targetDispls != nullptr) {
				free(this->targetDispls);
			}
		}

		template <class T>
		cupcfd::error::eCodes ExchangePatternOneSidedNonBlocking<T>::init(cupcfd::comm::Communicator& comm,
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

			// Setup the send buffer
			// Size is dependent upon how much data we are sending across all processes
			this->nSendBuffer = this->nSAdjncy;
			this->sendBuffer = (T *) malloc(sizeof(T) * this->nSendBuffer);


			// Setup the send counts
			this->nSendCounts = this->nSProc;
			this->sendCounts = (int *) malloc(sizeof(int) * this->nSendCounts);

			for(int i = 0; i < this->nSendCounts; i++) {
				this->sendCounts[i] = this->sXAdj[i+1] - this->sXAdj[i];
			}

			// Setup the recvCounts
			this->nRecvCounts = this->nRProc;
			this->recvCounts = (int *) malloc(sizeof(int) * this->nRecvCounts);

			for(int i = 0; i < this->nRecvCounts; i++) {
				this->recvCounts[i] = this->rXAdj[i+1] - this->rXAdj[i];
			}


			// Allocate a memory space for the MPI Windows to recv data to.
			// The parent init method will have setup the necessary data we need to know - i.e.
			// how big the window for receiving data must be
			this->nWinData = this->nRAdjncy;
			this->winData = (T *) malloc(sizeof(T) * this->nWinData);

			// Create the window - this is a collective call across all processes in the communicator, so
			// all must participate
			MPI_Info_create(&this->info);
			MPI_Win_create(this->winData, this->nWinData * sizeof(T), sizeof(T), this->info, comm.comm, &this->win);


			// We need to know the displacement in the windows on the destination process for each of our sent messages
			// While the receiving process knows how much data it will receive in total, and where to place it,
			// we only know how much data we are sending to it.
			// Thus, we need to do one final comm stage to find out where each of the processes we are sending to expects
			// this rank to place the data.

			// Create an array to store the displacement window values for each process we are sending to
			this->nTargetDispls = this->nSProc;
			this->targetDispls = (int *) malloc(sizeof(int) * this->nTargetDispls);

			// Exchange the rXAdj values (the positions) with each process listed in rProc (the processes we are
			// receiving data from)
			// ToDo: Don't really want to use an AllToAllV here, but is quickest and simplest approach. Gather
			// would be more suitable, but then we need communicators restricted to exchange neighbours only,
			// else we have to repeat across all ranks and it just becomes an AllToAllV again...
			// One approach is an all-to-allv only on a restricted rank list, but then we may miss some neighours
			// on other processes.
			// ToDo: Feels like best approach is a simple one sided exchange - revisit

			// rXAdj - The computed offsets for the received data
			// rProc - The processes we are receiving from - we need to let these know the offsets

			int nTmpBuffer;
			int * tmpBuffer;

			// Note: this all-to-all only stores received elements, ordered by rank.
			// If we do not communicate with a rank (sProc), we will have no data for it.
			cupcfd::comm::AllToAll(this->rXAdj, this->nRXAdj - 1, this->rProc, this->nRProc,
										&tmpBuffer, &nTmpBuffer, comm);


			// This rank should now have in the tmpBuffer displacement values from every process it sends to.
			// Copy these over to the displacement array
			// We should have received one entry for every process we send to.
			for(int i = 0; i < this->nSProc; i++) {
				this->targetDispls[i] = tmpBuffer[i];
			}

			// Copy finished, free space
			free(tmpBuffer);

			// === Finally, set up the MPI Groups so we don't block collectively across the entire comm just for neighbours ===
			// Copy the communicator group
			MPI_Group tmpGroup;
			MPI_Comm_group(this->comm.comm, &tmpGroup);
			MPI_Group_incl(tmpGroup, this->nSProc, this->sProc, &this->sendGroup);
			MPI_Group_incl(tmpGroup, this->nRProc, this->rProc, &this->recvGroup);

			return cupcfd::error::E_SUCCESS;
		}

		template <class T>
		cupcfd::error::eCodes ExchangePatternOneSidedNonBlocking<T>::packSendBuffer(T * data, int nData) {
			// Pack from the data array into a send buffer

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
		cupcfd::error::eCodes ExchangePatternOneSidedNonBlocking<T>::unpackRecvBuffer(T * data, int nData) {
			// Unpack from window into the data array

			// Data received should be grouped by process in the recv buffer
			// It should already be ordered in the recv buffer as per the CSR for the pattern,
			// so we can transfer across by converting the indexes

			for(int i = 0; i < this->nWinData; i++) {
				int exchangeID = this->rAdjncy[i];
				int localID = this->exchangeToLocal[exchangeID];

				#ifdef DEBUG
					if (localID < 0 || localID >= nData) {
						return cupcfd::error::E_INVALID_INDEX;
					}
				#endif

				data[localID] = this->winData[i];
			}

			return cupcfd::error::E_SUCCESS;
		}

		template <class T>
		cupcfd::error::eCodes ExchangePatternOneSidedNonBlocking<T>::exchangeStart(T * sourceData, int nData) {
			// Pack the send buffer
			this->packSendBuffer(sourceData, nData);

			// Start Epochs for Send/Recv of data
			// MPI_Win_fence is also an option, but since it is a collective across
			// the communicator, it is likely too expensive for what is a neighbour
			// communication

			// Start the epoch
			//MPI_Win_fence(0, this->win);
			MPI_Win_post(this->recvGroup, 0, this->win);
			MPI_Win_start(this->sendGroup, 0, this->win);

			// Call all MPI_Puts for this process to send within this epoch
			for(int i = 0; i < this->nSProc; i++) {
				// Send Data Location in buffer
				// = start of buffer + offset given by CSR for entry 'i'
				T * sendData = this->sendBuffer + this->sXAdj[i];

				// Send Count is difference in two offsets
				int sendCount = this->sXAdj[i+1] - this->sXAdj[i];

				// ToDo - Should separate this into the ExchangeMPI drivers section?
				// This class is more responsible for storing the window and buffers
				// that are passed through.
				// Get MPI DataType
				MPI_Datatype dType;
				T dummy;
				cupcfd::comm::mpi::getMPIType(dummy, &dType);

				MPI_Put(sendData, sendCount, dType,
						this->sProc[i], this->targetDispls[i], sendCount, dType,
						this->win);
			}

			return cupcfd::error::E_SUCCESS;
		}

		template <class T>
		cupcfd::error::eCodes ExchangePatternOneSidedNonBlocking<T>::exchangeStop(T * sinkData, int nData) {
			// End this epoch for data retrieval.
			// Avoid use of Win_fence due to collective nature
			// MPI_Win_fence(MPI_MODE_NOPUT, this->win);

			MPI_Win_complete(this->win);
			MPI_Win_wait(this->win);

			// Unpack the recv buffer/window
			this->unpackRecvBuffer(sinkData, nData);

			return cupcfd::error::E_SUCCESS;
		}
	}
}

// Explicit Instantiation
template class cupcfd::comm::ExchangePatternOneSidedNonBlocking<int>;
template class cupcfd::comm::ExchangePatternOneSidedNonBlocking<float>;
template class cupcfd::comm::ExchangePatternOneSidedNonBlocking<double>;
