/*
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Contains the definitions for the ExchangePattern Class.
 */

#include "ExchangePattern.h"

#include <cstdlib>
#include "AllToAll.h"

#include "ArrayDrivers.h"
#include "SortDrivers.h"
#include "StatisticsDrivers.h"
#include "Communicator.h"
#include <vector>
#include "Broadcast.h"

namespace cupcfd
{
	namespace comm
	{
		template <class T>
		ExchangePattern<T>::ExchangePattern() {
			this->nSProc = 0;
			this->nSXAdj = 0;
			this->nSAdjncy = 0;
			this->nRProc = 0;
			this->nRXAdj = 0;
			this->nRAdjncy = 0;

			this->sProc = nullptr;
			this->sXAdj = nullptr;
			this->sAdjncy = nullptr;
			this->rProc = nullptr;
			this->rXAdj = nullptr;
			this->rAdjncy = nullptr;
		}

		template <class T>
		ExchangePattern<T>::~ExchangePattern() {
			// Cleanup Components
			// (a) Communicator will clean up itself

			// (b) Maps will clean up themselves

			// (c) Free any allocated arrays
			if(this->sProc != nullptr) {
				free(this->sProc);
			}

			if(this->sXAdj != nullptr) {
				free(this->sXAdj);
			}

			if(this->sAdjncy != nullptr) {
				free(this->sAdjncy);
			}

			if(this->rProc != nullptr) {
				free(this->rProc);
			}

			if(this->rXAdj != nullptr) {
				free(this->rXAdj);
			}

			if(this->rAdjncy != nullptr) {
				free(this->rAdjncy);
			}
		}

		template <class T>
		cupcfd::error::eCodes ExchangePattern<T>::init(cupcfd::comm::Communicator& comm,
									  int * mapLocalToExchangeIDX, int nMapLocalToExchangeIDX,
									  int * exchangeIDXSend, int nExchangeIDXSend,
									  int * tRanks, int nTRanks) {
			/*
			 * This init function sets up the exchange data for patterns that are intended to be reused.
			 * The input comprises of three things:
			 * (1) mapLocalToExchangeIDX - This is the essentially an array of global identifiers for a data array.
			 * I.e. if we have 5 elements then [10, 20, 6, 3, 2] means that when given a data array, the elements map
			 * such that dataArray[0] -> Global ID 10, dataArray[20]-> Global ID 20 etc etc.
			 * (2) exchangeIDXSend is an array of the Global IDs on this rank that we will be sending during an exchange
			 * - e.g. if we have an array of [20, 6] then this rank will be sending the data elements that are associated
			 * with 20 and 6, or indexes 1 and 2 in the data array as known from mapLocalToExchangeIDX.
			 * (3) This tells us which rank(s) we are sending the data to, matching up pairwise with indexes in exchangeIDXSend.
			 * I.e. if exchangeIDXSend = [20, 6], and tRanks[1, 3], then the data with global ID 20 is going to rank 1,
			 * and the data with global ID 6 is going to rank 3.
			 */

			cupcfd::error::eCodes status;

			// (1) Store a copy of the mapping of local index to 'exchange' indexes and vice-versa
			//  The 'exchange' index map be a reuse of mappings from elsewhere - e.g. a connectivity graph, as long
			//  as it is global across processes. This does not mean it has to be unique across processes (quite the
			//  contrary, it is expected to be reused, since received data will be copied to a position of the same
			//  exchange index as per a normal exchange system such as for ghost nodes. It is necessary so that we
			//  know where to place received data however.

			// Error Check: Check mappings are unique


			// Copy the communicator
			this->comm = comm;

			// Copy and store in maps
			for(int i = 0; i < nMapLocalToExchangeIDX; i++) {
				this->localToExchange[i] = mapLocalToExchangeIDX[i];
				this->exchangeToLocal[mapLocalToExchangeIDX[i]] = i;
			}


			// (2) Store a copy of the local indexes to send data for and their corresponding ranks
			// Since we don't enforce ordering at input, it makes sense to group these elements and order
			// the groups by target rank to send to.
			// Technically the ordering within the groups of elements does not matter, but it must not change once
			// all processes have exchange pattern information - each process must know which received elements maps
			// to which exchange index for unpacking purposes, and this is set at the next stage.

			// Error Check? Provided indexes must exist on this process
			// Error Check? No more than one process should be sending any particular exchange ID across the comm

			// === Sort elements of exchangeIDXSend by process ===
			// Scratch space setup
			int * sortIndexes = (int *) malloc(sizeof(int) * nTRanks);


			int nCopyExchangeIDXSend = nExchangeIDXSend;


			// Copy the input data arrays of the target ranks and the matching exchange index to be sent
			// int * copyTRanks = (int *)  malloc(sizeof(int) * nTRanks);
			// status = cupcfd::utility::drivers::copy(tRanks, nTRanks, copyTRanks, nTRanks);
			// CHECK_ECODE(status)
			// int * copyExchangeIDXSend = (int *)  malloc(sizeof(int) * nExchangeIDXSend);
			// status = cupcfd::utility::drivers::copy(exchangeIDXSend, nExchangeIDXSend, copyExchangeIDXSend, nCopyExchangeIDXSend);
			// CHECK_ECODE(status)
			int* copyTRanks = cupcfd::utility::drivers::duplicate(tRanks, nTRanks);
			int* copyExchangeIDXSend = cupcfd::utility::drivers::duplicate(exchangeIDXSend, nExchangeIDXSend);

			// Sort the copied ranks array.
			// SortIndexes is an array of the original index positions in matching sorted order - we will use this to reshuffle
			// copyExchangeIDXSend so that they still match pairwise.
			status = cupcfd::utility::drivers::merge_sort_index(copyTRanks, nTRanks, sortIndexes, nTRanks);
			CHECK_ECODE(status)

			// Reorder the data elements of copyExchangeIDXSend to be pairwise matching with copyTRanks so we don't lose
			// the original association
			status = cupcfd::utility::drivers::sourceIndexReorder(copyExchangeIDXSend, nCopyExchangeIDXSend, sortIndexes, nTRanks);
			CHECK_ECODE(status)

			// === Get array of distinct processes ===
			// Number of distinct elements - how many ranks total will we be sending to
			status = cupcfd::utility::drivers::distinctCount(copyTRanks, nTRanks, &this->nSProc);
			CHECK_ECODE(status)

			// Storage for distinct processes
			this->sProc = (int *) malloc(sizeof(int) * this->nSProc);

			// Store for number of repeated instances of each processes (i.e. how many elements we are sending to each)
			int nDupCount = this->nSProc;
			int * dupCount = (int *) malloc(sizeof(int) * nDupCount);

			// Get the distinct array and duplicate counts
			// this->sProc: An array that contains only the process ranks we will send to, each rank will be unique
			// dupCount: Track a count of how many times a rank in sProc appeared in the original copyTRanks array. This will
			// let us know how many elements we are sending to that matching rank.
			status = cupcfd::utility::drivers::distinctArray(copyTRanks, nTRanks, this->sProc, this->nSProc, dupCount, nDupCount);
			CHECK_ECODE(status)

			// === Store in pattern as a CSR ===
			this->nSXAdj = this->nSProc + 1;
			this->sXAdj = (int *) malloc(sizeof(int) * this->nSXAdj);

			this->nSAdjncy = nExchangeIDXSend;
			this->sAdjncy = (int *) malloc(sizeof(int) * this->nSAdjncy);

			// First entry is always 0
			this->sXAdj[0] = 0;

			// Loop over each rank to send to
			for(int i = 0; i < this->nSProc; i++) {
				// Loop over each ranks data and place into CSR
				for(int j = 0; j < dupCount[i]; j++) {
					// Basically copying the sorted array over since it's already grouped....
					this->sAdjncy[this->sXAdj[i] + j] = copyExchangeIDXSend[this->sXAdj[i] + j];
				}
				this->sXAdj[i+1] = this->sXAdj[i] + dupCount[i];
			}

			// (3) Processes must communicate with one another to identify how much data they will receive, and from who.
			// Store in a similar fashion to exchangeIDXSend so unpacking purposes at exchange stages.

			// Perform an all-to-all of the 'exchange' indexes we will be sending to the relevant ranks
			// ToDo: This is expensive - either set up exchanges infrequently and reuse, or find an alternate approach
			// This will be the same order in which elements are sorted when performing exchange.
			// First, let each process know how many indexes we are sending.
			int * sendCount = (int *) malloc(sizeof(int) * comm.size);
			int * recvCount = (int *) malloc(sizeof(int) * comm.size);

			for(int i = 0; i < comm.size; i++) {
				sendCount[i] = 0;
			}

			for(int i = 0; i < this->nSProc; i++) {
				sendCount[this->sProc[i]] = this->sXAdj[i+1] - this->sXAdj[i];
			}

			status = cupcfd::comm::AllToAll(sendCount, comm.size, recvCount, comm.size, 1, comm);
			CHECK_ECODE(status)

			// Now we know how many elements we expect to receive from each process, perform an all-to-all of the 'exchange'
			// indexes (not local indexes)


			// Setup the recv storage
			cupcfd::utility::drivers::sum(recvCount, comm.size, &this->nRAdjncy);
			this->rAdjncy = (int *) malloc(sizeof(int) * this->nRAdjncy);

			// Check for non-zero entries, and store as a process we receive from
			// First pass to count non-zero entries
			this->nRProc = 0;
			for(int i = 0; i < comm.size; i++) {
				if(recvCount[i] != 0) {
					this->nRProc = this->nRProc + 1;
				}
			}

			this->rProc = (int *) malloc(sizeof(int) * this->nRProc);

			this->nRXAdj = this->nRProc + 1;
			this->rXAdj = (int *) malloc(sizeof(int) * this->nRXAdj);

			// Second pass to store the non-zero ranks (could do in one pass with a vector?)
			int ptr = 0;
			this->rXAdj[0] = 0;

			for(int i = 0; i < comm.size; i++) {
				if(recvCount[i] != 0) {
					this->rProc[ptr] = i;
					this->rXAdj[ptr+1] = this->rXAdj[ptr] + recvCount[i];
					ptr = ptr + 1;
				}
			}

			// Perform the all-to-all to get a copy of all the exchange indexes from various processes, as well as sending
			// our own that we will communicate during exchanges.

			status = cupcfd::comm::AllToAll(this->sAdjncy, this->nSAdjncy, sendCount, comm.size,
											this->rAdjncy, this->nRAdjncy, recvCount, comm.size,
											comm);
			CHECK_ECODE(status)

			// Error Check: For every exchange index element we are expecting to receive, have we declared a mapping to
			// a local index on this rank? If not, we do not know where to store it.

			free(dupCount);
			free(copyTRanks);
			free(copyExchangeIDXSend);
			free(sortIndexes);
			free(sendCount);
			free(recvCount);

			return cupcfd::error::E_SUCCESS;
		}
	}
}

// Explicit Instantiation
template class cupcfd::comm::ExchangePattern<int>;
template class cupcfd::comm::ExchangePattern<float>;
template class cupcfd::comm::ExchangePattern<double>;
