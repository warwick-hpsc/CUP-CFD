/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Declarations for Scatter Interface Operations
 */

#ifndef INCLUDE_CUPCFD_SCATTER_INCLUDE_H
#define INCLUDE_CUPCFD_SCATTER_INCLUDE_H

#include "Communicator.h"
#include "Error.h"

namespace cupcfd
{
	namespace comm
	{
		// ToDo: Order of parameters (int sProcess) is different to rest of comm interfaces.
		// Should put in same order (but will need to correct all calls to these functions)

	   /**
		 * Performs a Scatter operation across ranks to distribute a fixed-size block of data from a single process
		 * to each process.
		 *
		 * This is a top-level interface to pass through to the underlying comms operation, as well as performing
		 * various error checking.
		 *
		 * Each chunk of size chunkSize in the buffer is distributed in ascending rank order. E.g. if chunkSize = 2:
		 *
		 * bSend = [a, b, c, d, e, f, g, h]
		 *
		 * Result:
		 * Rank 0: [a, b]
		 * Rank 1: [c, d]
		 * Rank 2: [e, f]
		 * Rank 3: [g, h]
		 *
		 * @param bSend Array consisting of data to be sent. Can be safely reused at completion of function.
		 * @param nEleBSend Size of bSend array in number of elements of type T.
		 * @param bRecv Array to store received data in. Up to chunkSize elements will be overwritten.
		 * @param nEleBRecv Size of bRecv array in number of elements of type T.
		 * @param chunkSize Size of an individual chunk of data, i.e. the amount of data each process expects to receive.
		 * @param mpComm A CupCfd communicator containing data about the processes to be used.
		 * @param sProcess The id of the source process.
		 *
		 * @tparam T The datatype of the data to be communicated.
		 *
		 * @return An error status indicating the success or failure of the operation
		 * @retval cupcfd::error::E_SUCCESS Operation completed successfully.
		 * @retval cupcfd::error::E_MPI_DATATYPE_UNREGISTERED Datatype T does not have a suitable/registered datatype for communication via MPI.
		 * @retval cupcfd::error::E_MPI_ERR An MPI Error was encountered.
		*/
		template <class T>
		cupcfd::error::eCodes Scatter(T * bSend, int nEleBSend, T * bRecv, int nEleBRecv, int chunkSize, cupcfd::comm::Communicator& mpComm, int sProcess);

		/**
		 * Performs a Scatter operation across ranks to distribute a variable sized blocks of data from a single process
		 * to various processes, depending on the chunk sizes.
		 *
		 * This is a top-level interface to pass through to the underlying comms operation, as well as performing
		 * various error checking.
		 *
		 * Each chunk of size chunkSize in the buffer is distributed in ascending rank order.
		 * If the chunk size is 0, it is skipped over. E.g.:
		 *
		 * bSend = [a, b, c, d, e, f, g, h]
		 * chunkSizes[3, 1, 0, 4]
		 *
		 * Result:
		 * Rank 0: [a, b, c]
		 * Rank 1: [d]
		 * Rank 2: []
		 * Rank 3: [e, f, g, h]
		 *
		 * @param bSend Array consisting of data to be sent. Can be safely reused at completion of function.
		 * @param nEleBSend Size of bSend array in number of elements of type T.
		 * @param bRecv Array to store received data in. Should be large enough to hold the expected received data for the process.
		 * @param nEleBRecv Size of bRecv array in number of elements of type T.
		 * @param chunkSizes Array of data chunk sizes to be sent to each process, indexed by process ID in the communicator.
		 * Must be equivalent in size to the number of processes in the communicator (if no data is to be sent to a process, specify a size of 0).
		 * @param nEleChunkSizes Size of chunkSizes array in number of elements of type int.
		 * @param mpComm A Communicator containing data about the processes to be used.
		 * @param sProcess The ID of the source process sending data.
		 *
		 * @tparam T The datatype of the data to be communicated.
		 *
		 * @return An error status indicating the success or failure of the operation
		 * @retval cupcfd::error::E_SUCCESS Operation completed successfully.
		 * @retval cupcfd::error::E_MPI_DATATYPE_UNREGISTERED Datatype T does not have a suitable/registered datatype for communication via MPI.
		 * @retval cupcfd::error::E_MPI_ERR An MPI Error was encountered.
		*/
		template <class T>
		cupcfd::error::eCodes Scatter(T * bSend, int nEleBSend, T * bRecv, int nEleBRecv, int * chunkSizes, int nEleChunkSizes, cupcfd::comm::Communicator& mpComm, int sProcess);

		/**
		 * Performs a Scatter operation across ranks to distribute a variable sized blocks of data from a single process
		 * to various processes, depending on the chunk sizes.
		 *
		 * This is a top-level interface to pass through to the underlying comms operation, as well as performing
		 * various error checking.
		 *
		 * This variant has an additional overhead, but handles the setup of recv buffers when their sizes are unknown.
		 *
		 * It also manages the chunk allocation in a different manner.
		 * Each element in the send buffer is assigned a destination process. This function then handles the grouping and sorting of the data
		 * for Scatter operations, as well as notifying the processes of how much data to expect to receive.
		 * This can be in any process order and does not need to be grouped.
		 *
		 * The data should appear in the receive arrays of the destination process in the order in which they appeared in
		 * the send buffer.
		 *
		 * E.g.:
		 *
		 * bSend = [a, b, c, d, e, f, g, h]
		 * pSend[3, 3, 2, 1, 3, 0, 1, 3]
		 *
		 * Result:
		 * Rank 0: [e]
		 * Rank 1: [d,f]
		 * Rank 2: [b]
		 * Rank 3: [a,b,e,h]
		 *
		 * @param bSend Array consisting of data to be sent. Can be safely reused at completion of function.
		 * @param nEleBSend Size of bSend array in number of elements of type T.
		 * @param bRecv A pointer to where the array of received data for this process will be created.
		 * @param nEleBRecv A pointer to where the size of the created bRecv array in number of elements of type T will be stored.
		 * @param pSend The destination processes for each element in the bSend array.
		 * This should be the same size as the bSend array.
		 * @param nElePSend Size of pSend array in number of elements of type int.
		 * @param mpComm A Communicator containing data about the processes to be used.
		 * @param sProcess The ID of the source process sending data.
		 *
		 * @tparam T The datatype of the data to be communicated.
		 *
		 * @return An error status indicating the success or failure of the operation
		 * @retval cupcfd::error::E_SUCCESS Operation completed successfully.
		 * @retval cupcfd::error::E_MPI_DATATYPE_UNREGISTERED Datatype T does not have a suitable/registered datatype for communication via MPI.
		 * @retval cupcfd::error::E_MPI_ERR An MPI Error was encountered.
		*/
		template <class T>
		cupcfd::error::eCodes Scatter(T * bSend, int nEleBSend, T ** bRecv, int * nEleBRecv, int * pSend, int nElePSend, cupcfd::comm::Communicator& mpComm, int sProcess);

	} // namespace comm
} // namespace cupcfd

// Include Header Level Definitions
#include "Scatter.ipp"

#endif
