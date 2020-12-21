/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Contains declarations for the ExchangePatternTwoSidedNonBlocking class.
 */

#ifndef CUPCFD_COMM_EXCHANGE_PATTERN_TWO_SIDED_NON_BLOCKING_INCLUDE_H
#define CUPCFD_COMM_EXCHANGE_PATTERN_TWO_SIDED_NON_BLOCKING_INCLUDE_H

#include "ExchangePattern.h"
#include "mpi.h"

namespace cupcfd
{
	namespace comm
	{
		// This extends the exchange pattern object to maintain statuses and requests
		// for two-sided non blocking comms (i.e. Isend/Irecvs)
		template <class T>
		class ExchangePatternTwoSidedNonBlocking : public ExchangePattern<T>
		{
			public:

				/** Non Blocking MPI - Requests **/
				MPI_Request * requests;
				int nRequests;

				/** Non Blocking MPI - Statuses **/
				MPI_Status * statuses;
				int nStatuses;

				/** Size of sendBuffer in number of elements of type T **/
				int nSendBuffer;

				/** Buffer for storing packed data to be sent when communicating **/
				T * sendBuffer;

				/** Size of recvBuffer in number of elements of type T **/
				int nRecvBuffer;

				/** Buffer for storing packed data received when communicating **/
				T * recvBuffer;

				/**
				 * Size of send messages -These can be derived from the CSR arrays
				 * but we'd prefer to only compute them once.
				 * Matched up by index to the processes in exchangeRanks.
				**/
				int * sendCounts;

				/** Size of sendCounds in number of elements **/
				int nSendCounts;

				/**
				 * Size of recv messages -These can be derived from the CSR arrays
				 * but we'd prefer to only compute them once.
				 * Matched up by index to the processes in exchangeRanks.
				**/
				int * recvCounts;

				/** Size of recvCounts in number of elements **/
				int nRecvCounts;

				/**
				 * Default Constructor:
				 * initialises internal sizes to 0 and arrays/buffers to nullptr
				 * so they can be detected as unallocated.
				 */
				ExchangePatternTwoSidedNonBlocking();

				/**
				 * Deconstructor.
				 * Cleans up internally allocated buffers/arrays.
				 */
				virtual ~ExchangePatternTwoSidedNonBlocking();

				// Inherited Methods
				__attribute__((warn_unused_result))
				cupcfd::error::eCodes init(cupcfd::comm::Communicator& comm,
						  int * mapLocalToExchangeIDX, int nMapLocalToExchangeIDX,
						  int * exchangeIDXSend, int nExchangeIDXSend,
						  int * tRanks, int nTRanks);

				__attribute__((warn_unused_result))
				cupcfd::error::eCodes packSendBuffer(T * data, int nData);
				__attribute__((warn_unused_result))
				cupcfd::error::eCodes unpackRecvBuffer(T * data, int nData);

				__attribute__((warn_unused_result))
				cupcfd::error::eCodes exchangeStart(T * sourceData, int nData);
				__attribute__((warn_unused_result))
				cupcfd::error::eCodes exchangeStop(T * sinkData, int nData);
		};
	}
}

// Include Header Level Definitions
#include "ExchangePatternTwoSidedNonBlocking.ipp"

#endif
