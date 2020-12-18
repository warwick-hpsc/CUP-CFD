/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Contains declarations for the SparseMatrixSourceMeshGen class
 */

#ifndef CUPCFD_DATA_STRUCTURES_SPARSE_MATRIX_SOURCE_MESH_GEN_INCLUDE_H
#define CUPCFD_DATA_STRUCTURES_SPARSE_MATRIX_SOURCE_MESH_GEN_INCLUDE_H

// Library Functions/Objects
#include <string>

// Parent Class
#include "SparseMatrixSource.h"

// Error Codes
#include "Error.h"

namespace cupcfd
{
	namespace data_structures
	{
		/**
		 * Generate a matrix that uses a provided mesh as the basis for its structure
		 */
		template <class I, class T>
		class SparseMatrixSourceMeshGen : public SparseMatrixSource<I,T>
		{
			public:
				// === Members ===

				// === Constructor ===

				/**
				 *
				 */
				SparseMatrixSourceMeshGen();

				/**
				 * Deconstructor.
				 */
				~SparseMatrixSourceMeshGen();

				// === Concrete Methods ===

				// === Inherited Overloads ===

				__attribute__((warn_unused_result))
				cupcfd::error::eCodes getNNZ(I * nnz);

				__attribute__((warn_unused_result))
				cupcfd::error::eCodes getNRows(I * nRows);

				__attribute__((warn_unused_result))
				cupcfd::error::eCodes getNCols(I * nCols);

				__attribute__((warn_unused_result))
				cupcfd::error::eCodes getMatrixIndicesBase(I * indicesBase);

				__attribute__((warn_unused_result))
				cupcfd::error::eCodes getNNZRows(I * rowIndices, I nRowIndices);

				__attribute__((warn_unused_result))
				cupcfd::error::eCodes getRowColumnIndexes(I rowIndex, I ** columnIndexes, I * nColumnIndexes);

				__attribute__((warn_unused_result))
				cupcfd::error::eCodes getRowNNZValues(I rowIndex, T ** nnzValues, I * nNNZValues);

				// === Pure Virtual Methods ===
		};
	}
}

// Include Header Level Definitions
#include "SparseMatrixSourceMeshGen.ipp"

#endif
