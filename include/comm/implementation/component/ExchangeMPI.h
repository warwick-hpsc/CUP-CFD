/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Contains declarations for MPI Exchange Operations
 */

#ifndef CUPCFD_COMM_EXCHANGE_MPI_INCLUDE_H
#define CUPCFD_COMM_EXCHANGE_MPI_INCLUDE_H

#include "Error.h"
#include "mpi.h"

namespace cupcfd
{
	namespace comm
	{
		namespace mpi
		{
			/**
			 * Initialise the exchange of data between a set of processes.
			 *
			 * Every participating process (i.e. those listed in tRanks on any process)
			 * must call this function.
			 *
			 * This variant exchanges a fixed amount of data to each and every process specified
			 * in tRanks, and expects to receive the same amount of data from those same ranks in return.
			 *
			 * This variant is non-blocking, initialising but not blocking/waiting on the result
			 * of the exchange. This can be done using the MPI Waitall wrapper.
			 *
			 * This variant is two-sided, using Isend/Irecvs to perform its communications.
			 *
			 * The data buffers are split into chunks of size elePerRank elements. These chunks are
			 * assigned to ranks in order of the ranks listed in tRanks in both the send and recv buffers.
			 * As such the ranks in tRanks do not have to be in ascending order,
			 * but changing their order will impact which data blocks are sent/received.
			 *
			 * E.g.:
			 *
			 * Rank 0:
			 * 	SendBuffer: [a,b,c,d]
			 * 	TRanks: [1, 2]
			 *
			 * Rank 1:
			 * 	SendBuffer: [e,f,g,h]
			 * 	TRanks: [2, 0]
			 *
			 * Rank 2:
			 *  SendBuffer: [i,j,k,l]
			 *  TRanks: [0, 1]
			 *
			 * Result
			 *  Rank 0 RecvBuffer: [g,h,i,j]
			 *  Rank 1 RecvBuffer: [k,l,a,b]		// Note a,b is after k,l due to the order of processes in tRanks
			 *  Rank 2 RecvBuffer: [c,d,e,f]
			 *
			 * @param sendBuffer Array of all data blocks to send.
			 * @param nSendBuffer Number of elements in sendBuffer of type T.
			 * @param recvBuffer Array of all received data blocks.
			 * @param nRecvBuffer Number of elements in recvBuffer of type T.
			 * @param tRanks Array of ranks to send to/recieve from, in order of how they should be
			 * assigned to the data blocks in the send/recv buffers.
			 * @param nTRanks Number of elements in tRanks
			 * @param elePerRank Number of elements of type T to be sent/received per rank (equivalent exchange)
			 * @param comm MPI Communicator
			 * @param requests A pointer to a location where a requests array will be created and requests stored
			 *
			 * @tparam T The datatype of the data to be communicated. May be a supported primitive type or a class
			 * that inherits from CustomMPIType.
			 *
			 * @retval E_SUCCESS Broadcast completed successfully.
			 * @retval E_MPI_DATATYPE_UNREGISTERED Datatype T does not have a datatype
			 * registered with the MPI library.
			 * @retval E_MPI_ERR An MPI Error was encountered.
			 */
			template <class T>
			__attribute__((warn_unused_result))
			cupcfd::error::eCodes ExchangeMPIIsendIrecv(T * sendBuffer, int nSendBuffer,
														T * recvBuffer, int nRecvBuffer,
														int * tRanks, int nTRanks,
														int elePerRank,
														MPI_Comm comm,
														MPI_Request ** requests, int * nRequests);


			/**
			 * Initialise the exchange of data between a set of processes.
			 *
			 * Every participating process (i.e. those listed in tRanks on any process)
			 * must call this function.
			 *
			 * This variant exchanges a variable amount of data to each and every process specified
			 * in tRanks - the send and recv amounts do not have to match, but if a process is listed
			 * in tRanks then an entry in both sendCount and recvCount must be made for that rank at the same
			 * index in sendCount and recvCount as it is in tRanks.
			 *
			 * This variant is non-blocking, initialising but not blocking/waiting on the result
			 * of the exchange. This can be done using the MPI Waitall wrapper.
			 *
			 * This variant is two-sided, using Isend/Irecvs to perform its communications.
			 *
			 * The send buffers are split into chunks based on the counts in sendCount.
			 * The recv buffers are split into chunks based on the counts in recvCount.
			 *
			 * These chunks are assigned to ranks in order of the ranks listed in tRanks in both the send and recv buffers.
			 * As such the ranks in tRanks do not have to be in ascending order but changing their order will impact
			 * which data blocks are sent/recieved.
			 *
			 * This variant will create the requests array of an appropriate size. The size may be less than tRanks * 2, since
			 * empty messages (i.e. those with a send -> recv amount of 0) should be skipped, resulting in fewer than
			 * tRank sends and tRank recvs.
			 *
			 * Example:
			 *
			 * Rank 0:
			 * 	SendBuffer: [a,b,c,d]
			 * 	TRanks: [1, 2, 3]
			 * 	SendCount: [1, 3, 0]
			 * 	RecvCount: [2, 1, 4]
			 *
			 * Rank 1:
			 * 	SendBuffer: [e,f]
			 * 	TRanks: [0, 3]
			 * 	SendCount: [2, 0]
			 * 	RecvCount: [1, 1]
			 *
			 * Rank 2:
			 *  SendBuffer: [i]
			 *  TRanks: [1]
			 *  SendCount: [1]
			 *  RecvCount: [3]
			 *
			 * Rank 3:
			 *  SendBuffer: [m,n,o,p,q]
			 *  TRanks: [1, 0]
			 *  SendCount: [1, 4]
			 *  RecvCount: [0, 0]
			 *
			 * Result
			 *  Rank 0 RecvBuffer: [e,f,i,n,o,p,q]
			 *  Rank 1 RecvBuffer: [a,m]
			 *  Rank 2 RecvBuffer: [b,c,d]
			 *  Rank 3 RecvBuffer: []
			 *
			 * @param sendBuffer Packed buffer of send data
			 * @param nSendBuffer Number of elements in sendBuffer of type T
			 * @param sendCount Array containing number of elements to send to rank of corresponding index in tRanks
			 * @param nSendCount Number of elements in sendCount. Must be the same as nTRanks.
			 * @param recvBuffer Packed buffer for receiving data
			 * @param nRecvBuffer Number of elements in recvBuffer
			 * @param recvCount Array containing number of elements to recv from rank of corresponding index in tRanks
			 * @param nRecvCount Number of elements in recvCount.  Must be the same as nTRanks.
			 * @param sRanks Array of ranks to send data to
			 * @param nSRanks Number of elements in sRanks
			 * @param rRanks Array of ranks to receive data from
			 * @param nRRanks Number of elements in rRanks
			 * @param comm MPI Communicator
			 * @param requests A pointer to a location where a requests array will be created and requests stored
			 * @param nRequests A pointer to the location where the size of the requests array in the number
			 * of elements will be stored. This shold be created to be adequately sized.
			 *
			 * @tparam T The datatype of the data to be communicated. May be a supported primitive type or a class
			 * that inherits from CustomMPIType.
			 *
			 * @retval E_SUCCESS Broadcast completed successfully.
			 * @retval E_MPI_DATATYPE_UNREGISTERED Datatype T does not have a datatype
			 * registered with the MPI library.
			 * @retval E_MPI_ERR An MPI Error was encountered.
			 */
			template <class T>
			__attribute__((warn_unused_result))
			cupcfd::error::eCodes ExchangeVMPIIsendIrecv(T * sendBuffer, int nSendBuffer,
														int * sendCount, int nSendCount,
														T * recvBuffer, int nRecvBuffer,
														int * recvCount, int nRecvCount,
														int * sRanks, int nSRanks,
														int * rRanks, int nRRanks,
														MPI_Comm comm,
														MPI_Request ** requests, int * nRequests);


			/**
			 * Initialise the exchange of data between a set of processes.
			 *
			 * Every participating process (i.e. the collective union of tRanks from all processes)
			 * must call this function.
			 *
			 * This variant exchanges a variable amount of data to each and every process specified
			 * in tRanks - the send and recv amounts do not have to match, but if a process is listed
			 * in tRanks then an entry in both sendCount and recvCount must be made for that rank at the same
			 * index in sendCount and recvCount as it is in tRanks.
			 *
			 * This variant is non-blocking, initialising but not blocking/waiting on the result
			 * of the exchange. This can be done using the MPI Waitall wrapper.
			 *
			 * This variant is two-sided, using Isend/Irecvs to perform its communications.
			 *
			 * The send buffers are split into chunks based on the counts in sendCount.
			 * The recv buffers are split into chunks based on the counts in recvCount.
			 *
			 * These chunks are assigned to ranks in order of the ranks listed in tRanks in both the send and recv buffers.
			 * As such the ranks in tRanks do not have to be in ascending order but changing their order will impact
			 * which data blocks are sent/recieved.
			 *
			 * This variant expects requests arrays to already be created.
			 *
			 * Example:
			 *
			 * Rank 0:
			 * 	SendBuffer: [a,b,c,d]
			 * 	TRanks: [1, 2, 3]
			 * 	SendCount: [1, 3, 0]
			 * 	RecvCount: [2, 1, 4]
			 *
			 * Rank 1:
			 * 	SendBuffer: [e,f]
			 * 	TRanks: [0, 3]
			 * 	SendCount: [2, 0]
			 * 	RecvCount: [1, 1]
			 *
			 * Rank 2:
			 *  SendBuffer: [i]
			 *  TRanks: [1]
			 *  SendCount: [1]
			 *  RecvCount: [3]
			 *
			 * Rank 3:
			 *  SendBuffer: [m,n,o,p,q]
			 *  TRanks: [1, 0]
			 *  SendCount: [1, 4]
			 *  RecvCount: [0, 0]
			 *
			 * Result
			 *  Rank 0 RecvBuffer: [e,f,i,n,o,p,q]
			 *  Rank 1 RecvBuffer: [a,m]
			 *  Rank 2 RecvBuffer: [b,c,d]
			 *  Rank 3 RecvBuffer: []
			 *
			 * @param sendBuffer Packed buffer of send data
			 * @param nSendBuffer Number of elements in sendBuffer of type T
			 * @param sendCount Array containing number of elements to send to rank of corresponding index in tRanks
			 * @param nSendCount Number of elements in sendCount. Must be the same as nTRanks.
			 * @param recvBuffer Packed buffer for receiving data
			 * @param nRecvBuffer Number of elements in recvBuffer
			 * @param recvCount Array containing number of elements to recv from rank of corresponding index in tRanks
			 * @param nRecvCount Number of elements in recvCount.  Must be the same as nTRanks.
			 * @param sRanks Array of ranks to send data to
			 * @param nSRanks Number of elements in sRanks
			 * @param rRanks Array of ranks to receive data from
			 * @param nRRanks Number of elements in rRanks
			 * @param comm MPI Communicator
			 * @param requests The requests array for storing non-blocking requests.
			 * @param nRequests The size of the requests array. Must be large enough to hold all requests.
			 *
			 * @tparam T The datatype of the data to be communicated. May be a supported primitive type or a class
			 * that inherits from CustomMPIType.
			 *
			 * @retval E_SUCCESS Broadcast completed successfully.
			 * @retval E_MPI_DATATYPE_UNREGISTERED Datatype T does not have a datatype
			 * registered with the MPI library.
			 * @retval E_MPI_ERR An MPI Error was encountered.
			 */
			template <class T>
			__attribute__((warn_unused_result))
			cupcfd::error::eCodes ExchangeVMPIIsendIrecv(T * sendBuffer, int nSendBuffer,
														int * sendCount, int nSendCount,
														T * recvBuffer, int nRecvBuffer,
														int * recvCount, int nRecvCount,
														int * sRanks, int nSRanks,
														int * rRanks, int nRRanks,
														MPI_Comm comm,
														MPI_Request * requests, int nRequests);
		}
	}
}

// Include Header Level Definitions
#include "ExchangeMPI.ipp"

#endif
