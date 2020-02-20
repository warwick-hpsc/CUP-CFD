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
				 * Constructor:
				 * Copy values from source into a new object.
				 *
				 * @param source The source object to copy the values from
				 */
				SparseMatrixSourceConfig(SparseMatrixSourceConfig& source);

				/**
				 * Deconstructor.
				 */
				virtual ~SparseMatrixSourceConfig();

				// === Concrete Methods ===

				/**
				 * Deep copy from source to this config
				 *
				 * @param source The source configuration to copy from.
				 *
				 * @return Nothing.
				 */
				virtual void operator=(SparseMatrixSourceConfig<I,T>& source);

				// === Pure Virtual Methods ===

				/**
				 * Return a pointer to a clone of this object.
				 * This allows for making copies while also
				 * preserving the polymorphic type.
				 *
				 * @return A pointer to a cloned copy of this object.
				 */
				virtual SparseMatrixSourceConfig<I,T> * clone() = 0;

				/**
				 * Build a Sparse Matrix Source based on the settings specified in this configuration type.
				 *
				 * @param A pointer to the location where the pointer of the newly created object will be stored.
				 */
				virtual cupcfd::error::eCodes buildSparseMatrixSource(SparseMatrixSource<I,T> ** matrixSource) = 0;
		};
	}
}

// Include Header Level Definitions
#include "SparseMatrixSourceConfig.ipp"

#endif
