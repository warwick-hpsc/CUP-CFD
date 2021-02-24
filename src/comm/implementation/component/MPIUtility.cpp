/*
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Contains the definitions for utility methods involving MPI types or operations.
 */

#include "MPIUtility.h"

namespace cupcfd
{
	namespace comm
	{
		namespace mpi
		{
			cupcfd::error::eCodes getMPIType(int dummy __attribute__((unused)), MPI_Datatype * mpiType) {
				*mpiType = MPI_INT;
				return cupcfd::error::E_SUCCESS;
			}

			cupcfd::error::eCodes getMPIType(double dummy __attribute__((unused)), MPI_Datatype * mpiType) {
				*mpiType = MPI_DOUBLE;
				return cupcfd::error::E_SUCCESS;
			}

			cupcfd::error::eCodes getMPIType(float dummy __attribute__((unused)), MPI_Datatype * mpiType) {
				*mpiType = MPI_FLOAT;
				return cupcfd::error::E_SUCCESS;
			}

			cupcfd::error::eCodes getMPIType(long dummy __attribute__((unused)), MPI_Datatype * mpiType) {
				*mpiType = MPI_LONG;
				return cupcfd::error::E_SUCCESS;
			}

			cupcfd::error::eCodes getMPIType(bool dummy __attribute__((unused)), MPI_Datatype * mpiType) {
				// ToDo: C MPI doesn't have a bool datatype
				*mpiType = MPI::BOOL;
				return cupcfd::error::E_SUCCESS;
			}

			cupcfd::error::eCodes getMPIType(cupcfd::comm::mpi::CustomMPIType& customObject, MPI_Datatype * mpiType) {
				// Sets mpiType and returns the error code from a CustomMPIType class
				// Passes though operation to the CustomMPIType class to use its definition of the method.
				return customObject.getMPIType(mpiType);
			}
		} // namespace mpi
	} // namespace comm
} // namespace cupcfd
