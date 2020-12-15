/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Contains Header Level Definitions for MPI Reduce Operations
 */

#include "mpi.h"
#include "ReduceMPI.h"
#include "MPIUtility.h"

// ToDo: The behaviour for CustomMPITypes will not work - possibly need to extend CustomMPIType
// to include custom operators for these functions....

namespace cupcfd
{
	namespace comm
	{
		namespace mpi
		{
			template <class T>
			cupcfd::error::eCodes reduceMPI(T * bSend, int nBSend, T * bRecv, int nBRecv, MPI_Op op, int sProcess, MPI_Comm comm)
			{
				if (nBSend != nBRecv) {
					return cupcfd::error::E_ARRAY_SIZE_MISMATCH;
				}
				
				T dummy;
				MPI_Datatype dType;
				cupcfd::error::eCodes status;
				
				status = cupcfd::comm::mpi::getMPIType(dummy, &dType);	// Retrieve the MPI Datatype for T.
				if(status != cupcfd::error::E_SUCCESS)
				{
					return status;
				}
				
				// Call the Reduce operation
				int err = MPI_Reduce(bSend, bRecv, nBSend, dType, op, sProcess, comm);
				if(err != MPI_SUCCESS)
				{
					return cupcfd::error::E_MPI_ERR;
				}
				return cupcfd::error::E_SUCCESS;
			}

			template <class T>
			cupcfd::error::eCodes reduceMPISum(T * bSend, int nBSend, T * bRecv, int nBRecv, int sProcess, MPI_Comm comm)
			{
				// Passthrough work to generic function with SUM operation.
				// Pass back error code returned by that function.
				return reduceMPI(bSend, nBSend, bRecv, nBRecv, MPI_SUM, sProcess, comm);
			}

			template <class T>
			cupcfd::error::eCodes reduceMPIMin(T * bSend, int nBSend, T * bRecv, int nBRecv, int sProcess, MPI_Comm comm)
			{
				// Passthrough work to generic function with SUM operation.
				// Pass back error code returned by that function.
				return reduceMPI(bSend, nBSend, bRecv, nBRecv, MPI_MIN, sProcess, comm);
			}

			template <class T>
			cupcfd::error::eCodes reduceMPIMax(T * bSend, int nBSend, T * bRecv, int nBRecv, int sProcess, MPI_Comm comm)
			{
				// Passthrough work to generic function with SUM operation.
				// Pass back error code returned by that function.
				return reduceMPI(bSend, nBSend, bRecv, nBRecv, MPI_MAX, sProcess, comm);
			}

			template <class T>
			cupcfd::error::eCodes reduceMPIProduct(T * bSend, int nBSend, T * bRecv, int nBRecv, int sProcess, MPI_Comm comm)
			{
				// Passthrough work to generic function with SUM operation.
				// Pass back error code returned by that function.
				return reduceMPI(bSend, nBSend, bRecv, nBRecv, MPI_PROD, sProcess, comm);
			}

			template <class T>
			cupcfd::error::eCodes allReduceMPI(T * bSend, int nBSend, T * bRecv, int nBRecv, MPI_Op op, MPI_Comm comm)
			{
				if (nBSend != nBRecv) {
					return cupcfd::error::E_ARRAY_SIZE_MISMATCH;
				}

				T dummy;
				MPI_Datatype dType;
				cupcfd::error::eCodes status;
				
				status = cupcfd::comm::mpi::getMPIType(dummy, &dType);	// Retrieve the MPI Datatype for T.
				if(status != cupcfd::error::E_SUCCESS)
				{
					return status;
				}

				// Call the AllReduce operation
				int err = MPI_Allreduce(bSend, bRecv, nBSend, dType, op, comm);
				if(err != MPI_SUCCESS)
				{
					return cupcfd::error::E_MPI_ERR;
				}

				return cupcfd::error::E_SUCCESS;
			}

			template <class T>
			cupcfd::error::eCodes allReduceMPISum(T * bSend, int nBSend, T * bRecv, int nBRecv, MPI_Comm comm)
			{
				// Passthrough work to generic function with SUM operation.
				// Pass back error code returned by that function.
				return allReduceMPI(bSend, nBSend, bRecv, nBRecv, MPI_SUM, comm);
			}

			template <class T>
			cupcfd::error::eCodes allReduceMPIMin(T * bSend, int nBSend, T * bRecv, int nBRecv, MPI_Comm comm)
			{
				// Passthrough work to generic function with SUM operation.
				// Pass back error code returned by that function.
				return allReduceMPI(bSend, nBSend, bRecv, nBRecv, MPI_MIN, comm);
			}

			template <class T>
			cupcfd::error::eCodes allReduceMPIMax(T * bSend, int nBSend, T * bRecv, int nBRecv, MPI_Comm comm)
			{
				// Passthrough work to generic function with SUM operation.
				// Pass back error code returned by that function.
				return allReduceMPI(bSend, nBSend, bRecv, nBRecv, MPI_MAX, comm);
			}

			template <class T>
			cupcfd::error::eCodes allReduceMPIProduct(T * bSend, int nBSend, T * bRecv, int nBRecv, MPI_Comm comm)
			{
				// Passthrough work to generic function with SUM operation.
				// Pass back error code returned by that function.
				return allReduceMPI(bSend, nBSend, bRecv, nBRecv, MPI_PROD, comm);
			}
		}
	}
}
