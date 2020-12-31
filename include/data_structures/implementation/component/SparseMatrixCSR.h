/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Contains the declarations for the SparseMatrixCSR class
 *
 */

#ifndef CUPCFD_DATA_STRUCTURES_MATRICES_SPARSEMATRIXCSR_INCLUDE_H
#define CUPCFD_DATA_STRUCTURES_MATRICES_SPARSEMATRIXCSR_INCLUDE_H

#include <vector>
#include "SparseMatrix.h"
#include "Error.h"

namespace cupcfd
{
	namespace data_structures
	{
		/**
		 * Stores Sparse Matrix data in a CSR Format
		 *
		 * @tparam I The type of the indexing scheme
		 * @tparam T The type of the stored node data
		 */
		template <class I, class T>
		class SparseMatrixCSR : public SparseMatrix<SparseMatrixCSR<I,T>,I,T>
		{
			public:
				// === Members ===

				// CSR Constructs

				/** CSR Construct - Matrix Values **/
				std::vector<T> A;

				/** CSR Construct - Index Pointers into JA for Rows **/
				std::vector<I> IA;

				/** CSR Construct - Column Indexes **/
				std::vector<I> JA;

				// === Constructors/Deconstructors ===

				/**
				 * Construct a SparseMatrixCSR object.
				 * Create a default matrix with a size of 1,1 and a base index of 0
				 *
				 * @tparam I The type of the indexing scheme
				 * @tparam T The type of the stored node data
				 */
				SparseMatrixCSR();

				/**
				 * Create a sparse matrix COO object of a specific size and
				 * a base index of 0
				 *
				 * @param m Number of rows
				 * @param n Number of columns
				 *
				 * @tparam I The type of the indexing scheme
				 * @tparam T The type of the stored node data
				 */
				SparseMatrixCSR(I m, I n);

				/**
				 * Create a sparse matrix COO object of a specific size and
				 * base index
				 *
				 * @param m Number of rows
				 * @param n Number of columns
				 * @param baseIndex The base index to use for the matrix indexing scheme
				 *
				 * @tparam I The type of the indexing scheme
				 * @tparam T The type of the stored node data
				 */
				SparseMatrixCSR(I m, I n, I baseIndex);


				/**
				 * Deconstructor for the SparseMatrixCSR object.
				 *
				 * @tparam I The type of the indexing scheme
				 * @tparam T The type of the stored node data
				 */
				~SparseMatrixCSR();

				// === CRTP Methods ===

				__attribute__((warn_unused_result))
				cupcfd::error::eCodes resize(I rows, I columns);

				__attribute__((warn_unused_result))
				cupcfd::error::eCodes clear();

				__attribute__((warn_unused_result))
				cupcfd::error::eCodes setElement(I row, I col, T val);

				__attribute__((warn_unused_result))
				cupcfd::error::eCodes getElement(I row, I col, T * val);

				__attribute__((warn_unused_result))
				cupcfd::error::eCodes getNonZeroRowIndexes(I ** rowIndexes, I * nRowIndexes);

				__attribute__((warn_unused_result))
				cupcfd::error::eCodes getRowColumnIndexes(I row, I ** columnIndexes, I * nColumnIndexes);

				__attribute__((warn_unused_result))
				cupcfd::error::eCodes getRowNNZValues(I row, T ** nnzValues, I * nNNZValues);
		};
	}
}

// Include Header Level Definitions
#include "SparseMatrixCSR.ipp"

#endif
