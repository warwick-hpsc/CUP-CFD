/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Contains declarations for the SparseMatrixSource class
 */

#ifndef CUPCFD_DATA_STRUCTURES_SPARSE_MATRIX_SOURCE_INCLUDE_H
#define CUPCFD_DATA_STRUCTURES_SPARSE_MATRIX_SOURCE_INCLUDE_H

#include "Error.h"
#include "SparseMatrix.h"

namespace cupcfd
{
	namespace data_structures
	{
		/**
		 * This class defines an interface for accessing matrix data from some
		 * as yet undefined source (e.g. a file)
		 *
		 * @tparam I The type of the indexing scheme
		 * @tparam T The type of the stored node data
		 */
		template <class I, class T>
		class SparseMatrixSource
		{
			public:
				// === Members ===

				// === Constructor ===

				/**
				 * Constructor:
				 * Default - current sets nothing.
				 *
				 * @tparam I The type of the indexing scheme
				 * @tparam T The type of the stored node data
				 */
				SparseMatrixSource();

				/**
				 * Deconstructor
				 *
				 * @tparam I The type of the indexing scheme
				 * @tparam T The type of the stored node data
				 */
				virtual ~SparseMatrixSource();

				// === Concrete Methods ===

				// === Pure Virtual Methods ===

				/**
				 * Retrieve the number of non zero values stored in the matrix source.
				 *
				 * @param nnz A pointer to the location that will be updated with the number of non-zero values
				 *
				 * @tparam I The type of the indexing scheme
				 * @tparam T The type of the stored node data
				 *
				 * @return An error status indicating the success or failure of the operation
				 * @retval cupcfd::error::E_SUCCESS The method completed successfully
				 */
				__attribute__((warn_unused_result))
				virtual cupcfd::error::eCodes getNNZ(I * nnz) = 0;

				/**
				 * Retrieve the number of rows in the matrix.
				 *
				 * @param nRows A pointer to the location that will be updated with the number of rows.
				 *
				 * @tparam I The type of the indexing scheme
				 * @tparam T The type of the stored node data
				 *
				 * @return An error status indicating the success or failure of the operation
				 * @retval cupcfd::error::E_SUCCESS The method completed successfully
				 */
				__attribute__((warn_unused_result))
				virtual cupcfd::error::eCodes getNRows(I * nRows) = 0;

				/**
				 * Retrieve the number of columns in the matrix.
				 *
				 * @param nCols A pointer to the location that will be updated with the number of columns.
				 *
				 * @tparam I The type of the indexing scheme
				 * @tparam T The type of the stored node data
				 *
				 * @return An error status indicating the success or failure of the operation
				 * @retval cupcfd::error::E_SUCCESS The method completed successfully
				 */
				__attribute__((warn_unused_result))
				virtual cupcfd::error::eCodes getNCols(I * nCols) = 0;

				/**
				 * Retrieve the base used for the matrix indices stored in the source (e.g. whether they begin at 0 or 1)
				 *
				 * @param indicesBase A pointer to the location that will be updated with the base index
				 * of the matrix stored in this source.
				 *
				 * @tparam I The type of the indexing scheme
				 * @tparam T The type of the stored node data
				 *
				 * @return An error status indicating the success or failure of the operation
				 * @retval cupcfd::error::E_SUCCESS The method completed successfully
				 */
				__attribute__((warn_unused_result))
				virtual cupcfd::error::eCodes getMatrixIndicesBase(I * indicesBase) = 0;

				/**
				 * Get the number of non-zero values for every row in the matrix.
				 *
				 * @param rowIndices A pointer to the location which the number of non-zeroes will be stored.
				 * @param nRowIndices The size of the rowIndices array - must be equal to the number of rows
				 * in the matrix source
				 *
				 * @tparam I The type of the indexing scheme
				 * @tparam T The type of the stored node data
				 *
				 * @return An error status indicating the success or failure of the operation
				 * @retval cupcfd::error::E_SUCCESS The method completed successfully
				 */
				__attribute__((warn_unused_result))
				virtual cupcfd::error::eCodes getNNZRows(I * rowIndices, I nRowIndices) = 0;

				/**
				 * Retrieve the matrix column indexes that have non-zero values set for the specified row.
				 * They are in sorted order - i.e. ascending column indexes
				 *
				 * @param rowIndex The index of the row to get the column indexes for - this should use the same
				 * base as specified in the source.
				 * @param columnIndexes A pointer to the location where an array containing the column indexes will be created.
				 * These should be sorted in column order.
				 * @param nColumnIndexes A pointer to the location where the number of column indexes retrieved will be stored
				 *
				 * @tparam I The type of the indexing scheme
				 * @tparam T The type of the stored node data
				 *
				 * @return An error status indicating the success or failure of the operation
				 * @retval cupcfd::error::E_SUCCESS The method completed successfully
				 * @retval cupcfd::error::E_MATRIX_ROW_OOB rowIndex is out of bounds for an acceptable row index (from the matrix base index)
				 */
				__attribute__((warn_unused_result))
				virtual cupcfd::error::eCodes getRowColumnIndexes(I rowIndex, I ** columnIndexes, I * nColumnIndexes) = 0;

				/**
				 * Return the matrix non-zero values that have values set for the specified row
				 * They are in the order they appear in across the row - i.e. ascending column index.
				 *
				 * @param rowIndex The index of the row to get the non-zero values from - this should use the same
				 * base as specified in the source.
				 * @param nnzValues A pointer to the location where an array containing the non-zero values for the row will be created.
				 * These should be sorted by column order.
				 * @param nNNZValues A pointer to the location where the number of non-zero values retrieved will be stored
				 *
				 * @tparam I The type of the indexing scheme
				 * @tparam T The type of the stored node data
				 *
				 * @return An error status indicating the success or failure of the operation
				 * @retval cupcfd::error::E_SUCCESS The method completed successfully
				 * @retval cupcfd::error::E_MATRIX_ROW_OOB rowIndex is out of bounds for an acceptable row index (from the matrix base index)
				 */
				__attribute__((warn_unused_result))
				virtual cupcfd::error::eCodes getRowNNZValues(I rowIndex, T ** nnzValues, I * nNNZValues) = 0;
		};
	}
}

// Include Header Level Definitions
#include "SparseMatrixSource.ipp"

#endif
