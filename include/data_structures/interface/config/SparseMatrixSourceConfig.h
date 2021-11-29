/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Contains declarations for the SparseMatrixSourceConfig class
 */

#ifndef CUPCFD_DATA_STRUCTURES_SPARSE_MATRIX_SOURCE_CONFIG_INCLUDE_H
#define CUPCFD_DATA_STRUCTURES_SPARSE_MATRIX_SOURCE_CONFIG_INCLUDE_H

// Error Codes
#include "Error.h"

// Sparse Matrix Source
#include "SparseMatrixSource.h"

namespace cupcfd
{
	namespace data_structures
	{
		/**
		 * Specify configuration options required for building mesh data sources.
		 * Interface Abstract Class.
		 */
		template <class I, class T>
		class SparseMatrixSourceConfig
		{
			public:
				// === Members ===

				// === Constructor ===

				/**
				 * Constructor
				 */
				SparseMatrixSourceConfig();

				/**
				 * Deconstructor.
				 */
				virtual ~SparseMatrixSourceConfig();

				// === Pure Virtual Methods ===

				/**
				 * Return a pointer to a clone of this object.
				 * This allows for making copies while also
				 * preserving the polymorphic type.
				 *
				 * @return A pointer to a cloned copy of this object.
				 */
				__attribute__((warn_unused_result))
				virtual SparseMatrixSourceConfig<I,T> * clone() = 0;

				/**
				 * Build a Sparse Matrix Source based on the settings specified in this configuration type.
				 *
				 * @param A pointer to the location where the pointer of the newly created object will be stored.
				 */
				__attribute__((warn_unused_result))
				virtual cupcfd::error::eCodes buildSparseMatrixSource(SparseMatrixSource<I,T> ** matrixSource) = 0;
		};
	}
}

// Include Header Level Definitions
#include "SparseMatrixSourceConfig.ipp"

#endif
