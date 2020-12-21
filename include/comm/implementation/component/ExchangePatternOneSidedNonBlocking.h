/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Contains declarations for the ExchangePatternOneSidedNonBlocking class.
 */

#ifndef CUPCFD_COMM_EXCHANGE_PATTERN_ONE_SIDED_NON_BLOCKING_INCLUDE_H
#define CUPCFD_COMM_EXCHANGE_PATTERN_ONE_SIDED_NON_BLOCKING_INCLUDE_H

#include "ExchangePattern.h"
#include "mpi.h"

namespace cupcfd
{
	namespace comm
	{
		template <class T>
		class ExchangePatternOneSidedNonBlocking : public ExchangePattern<T>
		{
			public:
				// Variables
				T * winData;
				int nWinData;

				MPI_Info info;
				MPI_Win win;	// The MPI Window for receiving data

				/** MPI Groups for sending processes **/
				MPI_Group sendGroup;

				/** MPI Groups for receiving processes **/
				MPI_Group recvGroup;

				/** Size of sendBuffer in number of elements of type T **/
				int nSendBuffer;

				/** Buffer for storing packed data to be sent when communicating **/
				T * sendBuffer;

				/**
				 * Size of send messages -These can be derived from the CSR arrays
				 * but we'd prefer to only compute them once
				**/
				int * sendCounts;

				/** Size of sendCounds in number of elements **/
				int nSendCounts;

				/**
				 * Size of recv messages -These can be derived from the CSR arrays
				 * but we'd prefer to only compute them once
				**/
				int * recvCounts;

				/** Size of recvCounts in number of elements **/
				int nRecvCounts;

				/**
				 * Store the displacement of the data on the target process.
				 * This is computed on the target process (since only they know all of the
				 * processes they receive from).
				 */
				int * targetDispls;

				/** Size of targetDispls in number of elements **/
				int nTargetDispls;

				// Constructors/Deconstructors

				/**
				 * Default constructor:
				 * Sets up internal sizes to 0 and buffers/arrays to nullptr
				 * so they can be detected as unallocated.
				 */
				ExchangePatternOneSidedNonBlocking();

				/**
				 * Deconstructor:
				 * Clears up any allocated buffers/arrays
				 */
				virtual ~ExchangePatternOneSidedNonBlocking();

				// Other Methods
				__attribute__((warn_unused_result))
				virtual cupcfd::error::eCodes init(cupcfd::comm::Communicator& comm,
						  int * mapLocalToExchangeIDX, int nMapLocalToExchangeIDX,
						  int * exchangeIDXSend, int nExchangeIDXSend,
						  int * tRanks, int nTRanks);

				__attribute__((warn_unused_result))
				virtual cupcfd::error::eCodes packSendBuffer(T * data, int nData);
				__attribute__((warn_unused_result))
				virtual cupcfd::error::eCodes unpackRecvBuffer(T * data, int nData);

				__attribute__((warn_unused_result))
				virtual cupcfd::error::eCodes exchangeStart(T * sourceData, int nData);
				__attribute__((warn_unused_result))
				virtual cupcfd::error::eCodes exchangeStop(T * sinkData, int nData);
		};
	}
}

// Include Header Level Definitions
#include "ExchangePatternOneSidedNonBlocking.ipp"

#endif
