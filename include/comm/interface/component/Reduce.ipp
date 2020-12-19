/*
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Contains header level declarations for Reduce Interface Operations
 */

#ifndef CUPCFD_COMM_REDUCE_IPP_H
#define CUPCFD_COMM_REDUCE_IPP_H

#include "ReduceMPI.h"

namespace cupcfd
{
	namespace comm
	{
		template <class T>
		cupcfd::error::eCodes reduceAdd(T * bSend, int nBSend, T * bRecv, int nBRecv, int sinkProcess, cupcfd::comm::Communicator& mpComm) {
			cupcfd::error::eCodes status;
			
			// ToDo: Error Checks - E.g. Null Ptrs, Sizes
			
			status = cupcfd::comm::mpi::reduceMPISum(bSend, nBSend, bRecv, nBRecv, sinkProcess, mpComm.comm);
			CHECK_ERROR_CODE(status)
			return status;
		}

		template <class T>
		cupcfd::error::eCodes reduceMultiply(T * bSend, int nBSend, T * bRecv, int nBRecv, int sinkProcess, cupcfd::comm::Communicator& mpComm) {		
			cupcfd::error::eCodes status;
			
			// ToDo: Error Checks - E.g. Null Ptrs, Sizes
		
			status = cupcfd::comm::mpi::reduceMPIProduct(bSend, nBSend, bRecv, nBRecv, sinkProcess, mpComm.comm);
			CHECK_ERROR_CODE(status)
			return status;
		}

		template <class T>
		cupcfd::error::eCodes reduceMin(T * bSend, int nBSend, T * bRecv, int nBRecv, int sinkProcess, cupcfd::comm::Communicator& mpComm) {
			cupcfd::error::eCodes status;
			
			// ToDo: Error Checks - E.g. Null Ptrs, Sizes
			
			status = cupcfd::comm::mpi::reduceMPIMin(bSend, nBSend, bRecv, nBRecv, sinkProcess, mpComm.comm);
			CHECK_ERROR_CODE(status)
			return status;
		}

		template <class T>
		cupcfd::error::eCodes reduceMax(T * bSend, int nBSend, T * bRecv, int nBRecv, int sinkProcess, cupcfd::comm::Communicator& mpComm) {
			cupcfd::error::eCodes status;
			
			// ToDo: Error Checks - E.g. Null Ptrs, Sizes
			
			status = cupcfd::comm::mpi::reduceMPIMax(bSend, nBSend, bRecv, nBRecv, sinkProcess, mpComm.comm);
			CHECK_ERROR_CODE(status)
			return status;
		}

		template <class T>
		cupcfd::error::eCodes allReduceAdd(T * bSend, int nBSend, T * bRecv, int nBRecv, cupcfd::comm::Communicator& mpComm) {
			cupcfd::error::eCodes status;
			
			// ToDo: Error Checks - E.g. Null Ptrs, Sizes
			
			status = cupcfd::comm::mpi::allReduceMPISum(bSend, nBSend, bRecv, nBRecv, mpComm.comm);
			CHECK_ERROR_CODE(status)
			return status;
		}

		template <class T>
		cupcfd::error::eCodes allReduceMultiply(T * bSend, int nBSend, T * bRecv, int nBRecv, cupcfd::comm::Communicator& mpComm) {
			cupcfd::error::eCodes status;
			
			// ToDo: Error Checks - E.g. Null Ptrs, Sizes
			
			status = cupcfd::comm::mpi::allReduceMPIProduct(bSend, nBSend, bRecv, nBRecv, mpComm.comm);
			CHECK_ERROR_CODE(status)
			return status;
		}

		template <class T>
		cupcfd::error::eCodes allReduceMin(T * bSend, int nBSend, T * bRecv, int nBRecv, cupcfd::comm::Communicator& mpComm) {
			cupcfd::error::eCodes status;
			
			// ToDo: Error Checks - E.g. Null Ptrs, Sizes
			
			status = cupcfd::comm::mpi::allReduceMPIMin(bSend, nBSend, bRecv, nBRecv, mpComm.comm);
			CHECK_ERROR_CODE(status)
			return status;
		}

		template <class T>
		cupcfd::error::eCodes allReduceMax(T * bSend, int nBSend, T * bRecv, int nBRecv, cupcfd::comm::Communicator& mpComm) {
			cupcfd::error::eCodes status;
			
			// ToDo: Error Checks - E.g. Null Ptrs, Sizes
			
			status = cupcfd::comm::mpi::allReduceMPIMax(bSend, nBSend, bRecv, nBRecv, mpComm.comm);
			CHECK_ERROR_CODE(status)
			return status;
		}
	} // namespace comm
} // namespace cupcfd

#endif
