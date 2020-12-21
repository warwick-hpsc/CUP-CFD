/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Contains declarations for the VectorSourceConfig class
 */

#ifndef CUPCFD_DATA_STRUCTURES_VECTOR_SOURCE_CONFIG_INCLUDE_H
#define CUPCFD_DATA_STRUCTURES_VECTOR_SOURCE_CONFIG_INCLUDE_H

// Error Codes
#include "Error.h"

// Sparse Matrix Source
#include "VectorSource.h"

namespace cupcfd
{
	namespace data_structures
	{
		/**
		 * Specify configuration options required for building mesh data sources.
		 * Interface Abstract Class.
		 */
		template <class I, class T>
		class VectorSourceConfig
		{
			public:
				// === Members ===

				// === Constructor ===

				/**
				 * Constructor:
				 */
				VectorSourceConfig();

				/**
				 * Constructor:
				 * Copy values from source into a new object.
				 *
				 * @param source The source object to copy the values from
				 */
				VectorSourceConfig(VectorSourceConfig& source);

				/**
				 * Deconstructor.
				 */
				virtual ~VectorSourceConfig();

				// === Pure Virtual Methods ===

				/**
				 * Return a pointer to a clone of this object.
				 * This allows for making copies while also
				 * preserving the polymorphic type.
				 *
				 * @return A pointer to a cloned copy of this object.
				 */
				virtual VectorSourceConfig<I,T> * clone() = 0;

				/**
				 * Build a Vector Source based on the settings specified in this configuration type.
				 *
				 * @param A pointer to the location where the pointer of the newly created object will be stored.
				 */
				__attribute__((warn_unused_result))
				virtual cupcfd::error::eCodes buildVectorSource(VectorSource<I,T> ** vectorSource) = 0;
		};
	}
}

// Include Header Level Definitions
#include "SparseMatrixSourceConfig.ipp"

#endif
