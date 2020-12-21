/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Contains declarations for the VectorSourceConfigSource class
 */

#ifndef CUPCFD_DATA_STRUCTURES_VECTOR_SOURCE_CONFIG_SOURCE_INCLUDE_H
#define CUPCFD_DATA_STRUCTURES_VECTOR_SOURCE_CONFIG_SOURCE_INCLUDE_H

#include "Error.h"
#include "VectorSourceConfig.h"

namespace cupcfd
{
	namespace data_structures
	{
		/**
		 * Interface class for retrieving data from a configuration data source
		 * for SparseMatrix Source Configurations
		 */
		template <class I, class T>
		class VectorSourceConfigSource
		{
			public:
				// === Members ===

				// === Constructors/Deconstructors ===

				/**
				 * Default Constructor
				 *
				 * @tparam I The datatype of any indexing scheme
				 * @tparam T The datatype of the floating point values
				 */
				VectorSourceConfigSource();

				/**
				 * Constructor:
				 * Copy values from source to this newly constructed object
				 *
				 * @tparam I The datatype of any indexing scheme used by the distribution
				 * @tparam T The datatype of the values generated by the distribution
				 */
				VectorSourceConfigSource(const VectorSourceConfigSource<I,T>& source);

				/**
				 * Deconstructor:
				 *
				 * @tparam I The datatype of any indexing scheme used by the distribution
				 * @tparam T The datatype of the values generated by the distribution
				 */
				virtual ~VectorSourceConfigSource();

				// === Pure Virtual Methods ===

				/**
				 * Creates a new object containing a copy of the contents of this object
				 *
				 * @tparam I The datatype of any indexing scheme used by the distribution
				 * @tparam T The datatype of the values generated by the distribution
				 *
				 * @return A pointer to the newly created clone of this object.
				 */
				virtual VectorSourceConfigSource<I,T> * clone() = 0;

				/**
				 * Build a configuration for a sparse matrix source
				 *
				 * @param dist A pointer to the location where the pointer to the newly created object will be stored.
				 *
				 * @tparam I The datatype of any indexing scheme used by the distribution
				 * @tparam T The datatype of the values generated by the distribution
				 *
				 * @return An error status indicating the success or failure of the operation
				 * @retval cupcfd::error::E_SUCCESS Success
				 */
				__attribute__((warn_unused_result))
				virtual cupcfd::error::eCodes buildVectorSourceConfig(VectorSourceConfig<I,T> ** vectorSourceConfig) = 0;
		};
	}
}

// Include Header Level Definitions
#include "VectorSourceConfigSource.ipp"

#endif
