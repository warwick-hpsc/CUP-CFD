/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Contains Declarations for the CustomMPIType class
 */

#ifndef CUPCFD_COMM_MPI_CUSTOM_MPI_TYPE_INCLUDE_H
#define CUPCFD_COMM_MPI_CUSTOM_MPI_TYPE_INCLUDE_H

// External Libraries
#include "mpi.h"

// Error Codes
#include "Error.h"

namespace cupcfd
{
	namespace comm
	{
		namespace mpi
		{
			/**
			 * Abstract Class for managing custom MPI Types.
			 *
			 * Types should be registered before use. This class is intended
			 * to leave storage to each specific class that inherits from this class,
			 * while providing a common interface that MPI operations can use.
			 *
			 * ToDo: Ideally storage should be in a fixed static variable (since registration
			 * only needs to occur once). However, we cannot store in this class, since the
			 * same static variable would be inherited by all classes.
			 * Storage will instead be left to each individual subclass to manage.
			 * The same principles apply to the registration/deregistration methods -
			 * they should be static, but if made static they would not be inheritable AND
			 * unique to the inheriting classes, so left as non-static for now.
			 *
			 */
			class CustomMPIType
			{
				public:
					// === Members ===

					// === Constructors/Deconstructors ===

					/**
					 * Constructor:
					 * Currently does nothing
					 */
					CustomMPIType();

					/**
					 * Deconstructor:
					 * Currently does nothing
					 */
					virtual ~CustomMPIType();

					// === Concrete Methods ===

					// === Pure Virtual Methods ===

					/**
					 * Return the MPI Datatype for the custom registered datatype
					 *
					 * @param A pointer to the location where the datatype will be stored
					 *
					 * @return An error status indicating the success or failure of the operation
					 * @retval cupcfd::error::E_SUCCESS Success
					 * @retval cupcfd::error::E_MPI_DATATYPE_UNREGISTERED The object has not yet registered its datatype with
					 * the MPI library
					 */
					__attribute__((warn_unused_result))
					virtual cupcfd::error::eCodes getMPIType(MPI_Datatype * dType) = 0;

					/**
					 * Register the custom MPI Datatype needed for the exchange of the current structure.
					 * This should update the state of the datatype to registered for this class.
					 *
					 * @return An error status indicating the success or failure of the operation
					 * @retval cupcfd::error::E_SUCCESS Success
					 * @retval cupcfd::error::E_MPI_DATATYPE_REGISTERED The datatype is already registered
					 * @retval cupcfd::error::E_MPI_ERR An MPI Error was encountered
					 */
					__attribute__((warn_unused_result))
					virtual cupcfd::error::eCodes registerMPIType() = 0;

					/**
					 * Deregister the custom MPI Datatype associated with this class.
					 * This should update the state of the datatype to unregistered for this class.
					 *
					 * @return An error status indicating the success or failure of the operation
					 * @retval cupcfd::error::E_SUCCESS Success
					 * @retval cupcfd::error::E_MPI_DATATYPE_UNREGISTERED The datatype is not registered
					 * @retval cupcfd::error::E_MPI_ERR An MPI Error was encountered
					 */
					__attribute__((warn_unused_result))
					virtual cupcfd::error::eCodes deregisterMPIType() = 0;

					/**
					 * Check whether the datatype for this class has been registered yet with the MPI library.
					 *
					 * @return A boolean indicating whether the datatype has been registered.
					 * @retval true The datatype has been registered.
					 * @retval false The datatype has not been registered.
					 */
					virtual bool isRegistered() = 0;
			};
		}
	}
}

// Include Header Level Definitions
#include "CustomMPIType.ipp"

#endif
