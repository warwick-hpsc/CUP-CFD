/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Contains the declarations for the SparseMatrixCOO class
 *
 */

#ifndef CUPCFD_DATA_STRUCTURES_MATRICES_SPARSEMATRIXCOO_INCLUDE_H
#define CUPCFD_DATA_STRUCTURES_MATRICES_SPARSEMATRIXCOO_INCLUDE_H

#include <vector>
#include "SparseMatrix.h"
#include "Error.h"

namespace cupcfd
{
	namespace data_structures
	{
		/**
		 * Stores Sparse Matrix data in a COO Format
		 *
		 * @tparam I The type of the indexing scheme
		 * @tparam T The type of the stored node data
		 */
		template <class I, class T>
		class SparseMatrixCOO : public SparseMatrix<SparseMatrixCOO<I,T>,I,T>
		{
			public:
				// === Members ===

				/** COO Row Index Data **/
				std::vector<int> row;

				/** COO Column Index Data **/
				std::vector<int> col;

				/** COO Matrix Values **/
				std::vector<T> val;

				// === Constructors/Deconstructors ===

				/**
				 * Construct a SparseMatrixCOO object.
				 * Create a default matrix with a size of 1,1 and a base index of 0
				 *
				 * @tparam I The type of the indexing scheme
				 * @tparam T The type of the stored node data
				 */
				SparseMatrixCOO();

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
				SparseMatrixCOO(I m, I n);

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
				SparseMatrixCOO(I m, I n, I baseIndex);


				/**
				 * Deconstructor for the SparseMatrixCOO object.
				 *
				 * @tparam I The type of the indexing scheme
				 * @tparam T The type of the stored node data
				 */
				~SparseMatrixCOO();

				// === CRTP Methods ===

				I getRowSize();
				I getValSize();

				inline cupcfd::error::eCodes resize(I rows, I columns);
				inline cupcfd::error::eCodes clear();
				inline cupcfd::error::eCodes setElement(I row, I col, T val);
				inline cupcfd::error::eCodes getElement(I row, I col, T * val);
				inline cupcfd::error::eCodes getNonZeroRowIndexes(I ** rowIndexes, I * nRowIndexes);
				inline cupcfd::error::eCodes getRowColumnIndexes(I row, I ** columnIndexes, I * nColumnIndexes);
				inline cupcfd::error::eCodes getRowNNZValues(I row, T ** nnzValues, I * nNNZValues);
		};
	}
}

// Header Level Definitions
#include "SparseMatrixCOO.ipp"

#endif
