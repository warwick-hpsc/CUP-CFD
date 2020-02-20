/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Contains declarations for the SparseMatrixSourceHDF5 class
 */

#ifndef CUPCFD_DATA_STRUCTURES_SPARSE_MATRIX_SOURCE_HDF5_INCLUDE_H
#define CUPCFD_DATA_STRUCTURES_SPARSE_MATRIX_SOURCE_HDF5_INCLUDE_H

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
		 * Load a Matrix from a HDF5 File Format.
		 * Schema for the HDF5 file is as follows:
		 * Attributes at /:
		 * base - Base of the index scheme
		 * ncols - Number of columns in the matrix
		 * nrows - Number of rows in the matrix
		 * nnz - Total number of non-zero values in the matrix
		 *
		 * The Groups of the file are split into three sections:
		 * cols, nonzero and rowrange.
		 * Their format follows that of a CSR format
		 * (but we store an entry for every row, regardless of whether
		 * it has nonzero values or not so we can track size).
		 *
		 * E.g. For a Matrix of 4x6 with a base of 0.
		 *
		 * Cols: 	[3,   5,   2,  5,	 2,	  3]
		 * NonZero: [0.1, 0.2, 0.3, 0.4, 0.5, 0.6]
		 * rowrange:[0, 2, 2, 4, 6]
		 *
		 * To Lookup the nnz values of the first row, we get the values in
		 * rowRange[0] -> rowRange[1] to get range of 0->(2-1) which gives
		 * us the NonZero values 0.1 and 0.2, and the cols 3, 5, meaning we
		 * have values at (0,3) = 0.1 and (0,5) = 0.2.
		 *
		 * Note: The values stored in columns are matrix indexes and are thus
		 * using the base index stored in the attributes.
		 * The values in rowrange are intended to be HDF5 index lookups, and thus
		 * DO NOT use the matrix base.
		 *
		 * In the event that a row has no non zero values, it should have
		 * the same value in both the rowRange[i] and rowRange[i+1]
		 * E.g. in the example above, row 1 has rowRange[1] = 2 and rowRange[2] = 2
		 * meaning there are 2-2=0 non-zero values, and thus maps to no entry in cols
		 * and nonzero.
		 *
		 */
		template <class I, class T>
		class SparseMatrixSourceHDF5 : public SparseMatrixSource<I,T>
		{
			public:
				// === Members ===

				/** File where the matrix data is stored. **/
				std::string fileName;

				// === Constructor ===

				/**
				 *
				 */
				SparseMatrixSourceHDF5(std::string fileName);

				/**
				 * Deconstructor.
				 */
				virtual ~SparseMatrixSourceHDF5();

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
#include "SparseMatrixSourceHDF5.ipp"

#endif
