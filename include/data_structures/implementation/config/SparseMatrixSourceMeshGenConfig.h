/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Contains declarations for the SparseMatrixSourceMeshGenConfig class
 */

#ifndef CUPCFD_DATA_STRUCTURES_SPARSE_MATRIX_SOURCE_MESH_GEN_CONFIG_INCLUDE_H
#define CUPCFD_DATA_STRUCTURES_SPARSE_MATRIX_SOURCE_MESH_GEN_CONFIG_INCLUDE_H

// Library Functions/Classes
#include <string>

// Parent Class
#include "SparseMatrixSourceConfig.h"

// Error Codes
#include "Error.h"

// Sparse Matrix Source
#include "SparseMatrixSource.h"

namespace cupcfd
{
	namespace data_structures
	{
		/**
		 * Specify configuration options required for building Sparse Matrix data
		 * sources generated based on a mesh structure
		 */
		template <class I, class T>
		class SparseMatrixSourceMeshGenConfig : public SparseMatrixSourceConfig<I,T>
		{
			public:
				// === Members ===

				// === Constructor ===

				/**
				 * Constructor:
				 * Set values to the parameters provided
				 *
				 * @param filePath The file path to where the data source is located
				 * @param format The expected file format of the data source
				 */
				SparseMatrixSourceMeshGenConfig();

				/**
				 * Constructor:
				 * Copy values from source into a new object.
				 *
				 * @param source The source object to copy the values from
				 */
				SparseMatrixSourceMeshGenConfig(SparseMatrixSourceMeshGenConfig<I,T>& source);

				/**
				 * Deconstructor.
				 */
				~SparseMatrixSourceMeshGenConfig();

				// === Concrete Methods ===

				// === Overridden Inherited Methods ===

				void operator=(SparseMatrixSourceMeshGenConfig<I,T>& source);
				
				__attribute__((warn_unused_result))
				SparseMatrixSourceMeshGenConfig<I,T> * clone();

				__attribute__((warn_unused_result))
				cupcfd::error::eCodes buildSparseMatrixSource(SparseMatrixSource<I,T> ** matrixSource);
		};
	}
}

// Include Header Level Definitions
#include "SparseMatrixSourceMeshGenConfig.ipp"

#endif
