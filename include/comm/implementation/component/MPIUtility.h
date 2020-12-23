/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Contains the declarations for utility methods involving MPI types or operations.
 */

#ifndef CUPCFD_COMM_MPI_UTILITY_INCLUDE_H
#define CUPCFD_COMM_MPI_UTILITY_INCLUDE_H

#include "mpi.h"
#include "CustomMPIType.h"

// Error Codes
#include "Error.h"

namespace cupcfd
{
	namespace comm
	{
		namespace mpi
		{
			/**
			 * Gets the MPI Datatype for integers.
			 *
			 * @param dummy A dummy variable. Value does not matter, it merely informs the type for specialised methods.
			 * @param mpiType A pointer to where the retrieved MPI_Datatype will be stored.
			 */
			void getMPIType(int dummy, MPI_Datatype * mpiType);

			/**
			 * Gets the MPI Datatype for doubles.
			 *
			 * @param dummy A dummy variable. Value does not matter, it merely informs the type for specialised methods.
			 * @param mpiType A pointer to where the retrieved MPI_Datatype will be stored.
			 */
			void getMPIType(double dummy, MPI_Datatype * mpiType);

			/**
			 * Gets the MPI Datatype for float.
			 *
			 * @param dummy A dummy variable. Value does not matter, it merely informs the type for specialised methods.
			 * @param mpiType A pointer to where the retrieved MPI_Datatype will be stored.
			 */
			void getMPIType(float dummy, MPI_Datatype * mpiType);

			/**
			 * Gets the MPI Datatype for longs.
			 *
			 * @param dummy A dummy variable. Value does not matter, it merely informs the type for specialised methods.
			 * @param mpiType A pointer to where the retrieved MPI_Datatype will be stored.
			 */
			void getMPIType(long dummy, MPI_Datatype * mpiType);

			/**
			 * Gets the MPI Datatype for booleans.
			 *
			 * @param dummy A dummy variable. Value does not matter, it merely informs the type for specialised methods.
			 * @param mpiType A pointer to where the retrieved MPI_Datatype will be stored.
			 */
			void getMPIType(bool dummy, MPI_Datatype * mpiType);

			/**
			 * Gets the MPI Datatype for any object that inherits and implements the
			 * virtual methods for CustomMPIType (e.g. to create custom struct types).
			 *
			 * @param dummy A dummy variable. Value does not matter, it merely informs the type for specialised methods.
			 * @param mpiType A pointer to where the retrieved MPI_Datatype will be stored.
			 *
			 * @return An error status indicating the success or failure of the operation
			 * @retval E_SUCCESS Operation was completed successfully
			 * @retval cupcfd::error::E_MPI_DATATYPE_UNREGISTERED The object has not yet registered its datatype with
			 * the MPI library
			 */
			__attribute__((warn_unused_result))
			cupcfd::error::eCodes getMPIType(cupcfd::comm::mpi::CustomMPIType& customObject, MPI_Datatype * mpiType);
		} // namespace mpi
	} // namespace comm
} // namespace cupcfd

// Include Header Level Definitions
#include "MPIUtility.ipp"

#endif
