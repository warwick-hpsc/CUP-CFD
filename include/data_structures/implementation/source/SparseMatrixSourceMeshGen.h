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
				virtual ~SparseMatrixSourceMeshGen();

				// === Concrete Methods ===

				// === Inherited Overloads ===

				cupcfd::error::eCodes getNNZ(I * nnz);

				cupcfd::error::eCodes getNRows(I * nRows);

				cupcfd::error::eCodes getNCols(I * nCols);

				cupcfd::error::eCodes getMatrixIndicesBase(I * indicesBase);

				cupcfd::error::eCodes getNNZRows(I * rowIndices, I nRowIndices);

				cupcfd::error::eCodes getRowColumnIndexes(I rowIndex, I ** columnIndexes, I * nColumnIndexes);

				cupcfd::error::eCodes getRowNNZValues(I rowIndex, T ** nnzValues, I * nNNZValues);

				// === Pure Virtual Methods ===
		};
	}
}

// Include Header Level Definitions
#include "SparseMatrixSourceMeshGen.ipp"

#endif
