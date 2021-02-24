/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Contains definitions for the CustomMPIType class
 */

#include "CustomMPIType.h"

namespace cupcfd
{
	namespace comm
	{
		namespace mpi
		{
			// === Constructors/Deconstructors ===

			CustomMPIType::CustomMPIType()
			{

			}

			CustomMPIType::~CustomMPIType()
			{

			}

			// === Pure Virtual Methods ===
			// Here to satisfy linker

			// cupcfd::error::eCodes CustomMPIType::getMPIType(MPI_Datatype * dType)
			// {
			// 	return cupcfd::error::E_SUCCESS;
			// }

			// MPI_Datatype CustomMPIType::getMPIType()
			// {
			// 	MPI_Datatype garbage;

			// 	return garbage;
			// }

			// cupcfd::error::eCodes CustomMPIType::registerMPIType()
			// {
			// 	return cupcfd::error::E_SUCCESS;
			// }

			// cupcfd::error::eCodes deregisterMPIType()
			// {
			// 	return cupcfd::error::E_SUCCESS;
			// }
		}
	}
}
