/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Contains the header level definitions for the SparseMatrixCOO class
 *
 */

#ifndef CUPCFD_DATA_STRUCTURES_MATRICES_SPARSEMATRIXCOO_IPP_H
#define CUPCFD_DATA_STRUCTURES_MATRICES_SPARSEMATRIXCOO_IPP_H

#include "ArrayDrivers.h"
#include <iostream>

namespace cupcfd
{
	namespace data_structures
	{
			template <class I, class T>
		SparseMatrixCOO<I,T>::SparseMatrixCOO()
		: SparseMatrix<SparseMatrixCOO<I,T>,I,T>()
		{
			// Nothing to do beyond parent constructor
		}

		template <class I, class T>
		SparseMatrixCOO<I,T>::SparseMatrixCOO(I m, I n)
		: SparseMatrix<SparseMatrixCOO<I,T>,I,T>(m, n)
		{
			// Nothing to do beyond parent constructor
		}

		template <class I, class T>
		SparseMatrixCOO<I,T>::SparseMatrixCOO(I m, I n, I baseIndex)
		: SparseMatrix<SparseMatrixCOO<I,T>,I,T>(m, n, baseIndex)
		{
			// Nothing to do beyond parent constructor
		}

		template <class I, class T>
		SparseMatrixCOO<I,T>::~SparseMatrixCOO()
		{
			// Nothing to do currently, vectors will cleanup themselves on destructor call
		}

		template <class I, class T>
		I SparseMatrixCOO<I,T>::getRowSize()
		{
			std::size_t s = this->row.size();
			#ifdef DEBUG
				// Need to cast size_t to I. First, check that no data is lost:
				if (s > (std::size_t)std::numeric_limits<I>::max()) {
					std::string msg("row size ");
					msg += s;
					msg += std::string(" exceeds max value of type ");
					msg += typeid(I).name();
					throw(std::runtime_error(msg));
				}
			#endif
			return (I)s;
		}
		template <class I, class T>
		I SparseMatrixCOO<I,T>::getValSize()
		{
			std::size_t s = this->val.size();
			#ifdef DEBUG
				// Need to cast size_t to I. First, check that no data is lost:
				if (s > (std::size_t)std::numeric_limits<I>::max()) {
					std::string msg("val size ");
					msg += s;
					msg += std::string(" exceeds max value of type ");
					msg += typeid(I).name();
					throw(std::runtime_error(msg));
				}
			#endif
			return (I)s;
		}

		template <class I, class T>
		inline cupcfd::error::eCodes SparseMatrixCOO<I,T>::resize(I rows, I columns)
		{
			// Clear the matrix
			this->clear();

			if(rows < 1)
			{
				return cupcfd::error::E_MATRIX_INVALID_ROW_SIZE;
			}

			if(columns < 1)
			{
				return cupcfd::error::E_MATRIX_INVALID_COL_SIZE;
			}

			// Resize the existing vectors to 0 since no data (vector can resize when adding later....)
			this->row.resize(0);
			this->col.resize(0);
			this->val.resize(0);

			// Update the stored matrix max capacity
			this->m = rows;
			this->n = columns;

			return cupcfd::error::E_SUCCESS;
		}

		template <class I, class T>
		inline cupcfd::error::eCodes SparseMatrixCOO<I,T>::clear()
		{
			// Delete the entries from row, col and val in the object
			this->row.clear();
			this->col.clear();
			this->val.clear();

			// Reset the number of non-zero values
			this->nnz = 0;

			return cupcfd::error::E_SUCCESS;
		}

		template <class I, class T>
		inline cupcfd::error::eCodes SparseMatrixCOO<I,T>::setElement(I row, I col, T val)
		{
			// Store in a sorted fashion to try and speed up searches a bit
			// Still a linear search, but can at least reduce the number of
			// comparisons...
			// ToDo: Since it is stored in sorted order, should be able to use
			// any of a number of better search methods...

			// Error Check: Within Range -- Greater than baseIndex
			if((row < this->baseIndex) || (row >= this->baseIndex + this->m))
			{
				return cupcfd::error::E_MATRIX_ROW_OOB;
			}

			if((col < this->baseIndex) || (col >= this->baseIndex + this->n))
			{
				return cupcfd::error::E_MATRIX_COL_OOB;
			}

			// === Find the location of an existing value if it exists ===
			bool exists = false;
			I index = 0;
			I i;

			for(i = 0; i < this->getValSize(); i++)
			{
				// We don't need to compute the offset of row and col from the baseIndex here
				// since we store the values directly

				if(this->row[i] == row)
				{
					if(this->col[i] == col)
					{
						exists = true;
						index = i;
						break;
					}

					if(this->col[i] > col)
					{
						// Past the point where a valid column for the correct row will be found in a sorted data store
						exists = false;
						index = i;
						break;
					}
				}

				if(this->row[i] > row)
				{
					// We're past the point where a valid entry can be found
					exists = false;
					index = i;
					break;
				}
			}

			if(i == (I)this->getValSize())
			{
				// We got to the end of the list without finding a match
				// Insert at the end (i.e. one greater than the last element)
				exists = false;
				index = this->getValSize();
			}

			// === Insert/Replace as appropriate

			if(exists)
			{
				// Note: We still leave the entry even if overwriting with a zero value
				// This is because it will leave the position registered as a non-zero position.
				// ToDo: Add a seperate deleteElement function to remove the element entirely from
				// the non-zero structure.

				// Overwrite value
				this->val[index] = val;
				return cupcfd::error::E_SUCCESS;
			}
			else
			{
				// Add a new value
				// index should be set to the first valid location past the possible entries
				// (i.e. a suitable insert point to keep data sorted)

				typename std::vector<I>::iterator it1 = this->row.begin();
				this->row.insert(it1 + index, row);

				typename std::vector<I>::iterator it2 = this->col.begin();
				this->col.insert(it2 + index, col);

				typename std::vector<T>::iterator it3 = this->val.begin();
				this->val.insert(it3 + index, val);

				// Since we are adding a new value, increase the non-zero counter
				this->nnz = this->nnz + 1;

				return cupcfd::error::E_SUCCESS;
			}
		}

		template <class I, class T>
		inline cupcfd::error::eCodes SparseMatrixCOO<I,T>::getElement(I row, I col, T * val)
		{
			// Data should be stored in a sorted fashion (By row, then column)
			// Search through to the point where it is no longer possible to find
			// the value if it exists (row > data rows, or columns > col in a row
			// range)

			// Error Check: Ensure we are within range
			if((row < this->baseIndex) || (row >= this->baseIndex + this->m))
			{
				return cupcfd::error::E_MATRIX_ROW_OOB;
			}

			if((col < this->baseIndex) || (col >= this->baseIndex + this->n))
			{
				return cupcfd::error::E_MATRIX_COL_OOB;
			}

			// === Find the location of an existing value if it exists ===
			bool exists = false;
			I index = 0;
			I i;

			for(i = 0; i < this->getValSize(); i++)
			{
				// We don't need to compute the offset of row and col from the baseIndex here
				// since we store the values directly

				if(this->row[i] == row)
				{
					if(this->col[i] == col)
					{
						exists = true;
						index = i;
						break;
					}

					if(this->col[i] > col)
					{
						// Past the point where a valid column for the correct row will be found in a sorted data store
						exists = false;
						index = i;
						break;
					}
				}

				if(this->row[i] > row)
				{
					// We're past the point where a valid entry can be found
					exists = false;
					index = i;
					break;
				}
			}

			if(i == this->getValSize())
			{
				// We got to the end of the list without finding a match
				// Insert at the end (i.e. one greater than the last element)
				exists = false;
				index = this->getValSize();
			}

			// Pass value back via reference
			if(exists)
			{
				*val = this->val[index];
				return cupcfd::error::E_SUCCESS;
			}
			else
			{
				// No value was found - this makes it a zero value
				*val = 0;
				return cupcfd::error::E_SUCCESS;
			}
		}

		template <class I, class T>
		inline cupcfd::error::eCodes SparseMatrixCOO<I,T>::getNonZeroRowIndexes(I ** rowIndexes, I * nRowIndexes)
		{
			// Non Zero Rows are basically any row which has an entry for a value in the matrix
			// We can pass through this work to a utility function that gets the distinct row indexes in the data store, since
			// if an entry exists then there is a non-zero value stored for that row.

			// ToDo: Should return error status of utility function instead - can do this once
			// error codes are ported across to same enum.

			// This method also allocates the array
			I tmpSize = this->row.size();
			cupcfd::utility::drivers::distinctArray(&(this->row[0]), tmpSize, rowIndexes, nRowIndexes);

			return cupcfd::error::E_SUCCESS;
		}

		template <class I, class T>
		inline cupcfd::error::eCodes SparseMatrixCOO<I,T>::getRowColumnIndexes(I row, I ** columnIndexes, I * nColumnIndexes)
		{
			// Find the Region where the row starts/ends
			// ToDo: This search component is getting reused throughout the class in parts
			// We can probably move it out into a generic function
			I startIndex = -1;
			I stopIndex = -1;
			I i;

			for(i = 0; i < this->getValSize(); i++)
			{
				// We don't need to compute the offset of row and col from the baseIndex here
				// since we store the values directly

				if(this->row[i] == row && startIndex == -1)
				{
					startIndex = i;
					stopIndex = i;
				}
				else if(this->row[i] == row && startIndex != -1)
				{
					stopIndex = i;
				}
				else if(this->row[i] > row)
				{
					break;
				}
			}

			if(startIndex == -1)
			{
				// The row does not exist - error
				return cupcfd::error::E_MATRIX_ROW_OOB;
			}

			*nColumnIndexes = (stopIndex - startIndex) + 1;

			// Allocate the space
			*columnIndexes = (I *) malloc(sizeof(I) * *nColumnIndexes);

			// Copy the column indexes across
			cupcfd::utility::drivers::copy(&(this->col[startIndex]), *nColumnIndexes, *columnIndexes, *nColumnIndexes);

			// Done without error
			return cupcfd::error::E_SUCCESS;
		}

		template <class I, class T>
		inline cupcfd::error::eCodes SparseMatrixCOO<I,T>::getRowNNZValues(I row, T ** nnzValues, I * nNNZValues)
		{
			// Find the Region where the row starts/ends
			// ToDo: This search component is getting reused throughout the class in parts
			// We can probably move it out into a generic function
			I startIndex = -1;
			I stopIndex = -1;
			I i;

			for(i = 0; i < this->getValSize(); i++)
			{
				// We don't need to compute the offset of row and col from the baseIndex here
				// since we store the values directly

				if(this->row[i] == row && startIndex == -1)
				{
					startIndex = i;
					stopIndex = i;
				}
				else if(this->row[i] == row && startIndex != -1)
				{
					stopIndex = i;
				}
				else if(this->row[i] > row)
				{
					break;
				}
			}

			if(startIndex == -1)
			{
				// The row does not exist - error
				return cupcfd::error::E_MATRIX_ROW_OOB;
			}

			*nNNZValues = (stopIndex - startIndex) + 1;

			// Allocate the space
			*nnzValues = (T *) malloc(sizeof(T) * *nNNZValues);

			// Copy the column indexes across
			cupcfd::utility::drivers::copy(&(this->val[startIndex]), *nNNZValues, *nnzValues, *nNNZValues);

			// Done without error
			return cupcfd::error::E_SUCCESS;
		}
	}
}

#endif
