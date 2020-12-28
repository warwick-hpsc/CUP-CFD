/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Contains the header level definitions for the SparseMatrixCSR class
 *
 */

#ifndef CUPCFD_DATA_STRUCTURES_MATRICES_SPARSEMATRIXCSR_IPP_H
#define CUPCFD_DATA_STRUCTURES_MATRICES_SPARSEMATRIXCSR_IPP_H

#include "ArrayDrivers.h"
#include "SearchDrivers.h"
#include "ArrayDrivers.h"

#include <iostream>

namespace cupcfd
{
	namespace data_structures
	{
			// Call the base class constructor, then call the init method of the SparseMatrixCSR
		// to setup any additional properties specific to SparseMatrixCSR
		template <class I, class T>
		SparseMatrixCSR<I,T>::SparseMatrixCSR()
		: SparseMatrix<SparseMatrixCSR<I,T>,I,T>()
		{
			// Setup the initial state of the vector
			// Since there are initially nnz values, the size of A is 0, the size of JA is 0
			// and the size of IA is 1 (nnz + 1)
			// We can leave A and JA at their defaults, push zeros
			// These are the row start pointers, but they are all set to 0 since they hold no values
			for(I i = 0; i < this->m; i++) {
				this->IA.push_back(0);
			}

			// Set this to zero so the last row has the correct size of 0
			this->IA.push_back(0);
		}

		// Call the base class constructor, then call the init method of the SparseMatrixCSR
		// to setup any additional properties specific to SparseMatrixCSR
		template <class I, class T>
		SparseMatrixCSR<I,T>::SparseMatrixCSR(I m, I n)
		: SparseMatrix<SparseMatrixCSR<I,T>,I,T>(m, n)
		{
			// Setup the initial state of the vector
			// Since there are initially nnz values, the size of A is 0, the size of JA is 0
			// and the size of IA is 1 (nnz + 1)
			// We can leave A and JA at their defaults, push zeros
			// These are the row start pointers, but they are all set to 0 since they hold no values
			for(I i = 0; i < this->m; i++) {
				this->IA.push_back(0);
			}

			// Set this to zero so the last row has the correct size of 0
			this->IA.push_back(0);
		}

		// Call the base class constructor, then call the init method of the SparseMatrixCSR
		// to setup any additional properties specific to SparseMatrixCSR
		template <class I, class T>
		SparseMatrixCSR<I,T>::SparseMatrixCSR(I m, I n, I baseIndex)
		: SparseMatrix<SparseMatrixCSR<I,T>,I,T>(m, n, baseIndex)
		{
			// Setup the initial state of the vector
			// Since there are initially nnz values, the size of A is 0, the size of JA is 0
			// and the size of IA is 1 (nnz + 1)
			// We can leave A and JA at their defaults, push zeros
			// These are the row start pointers, but they are all set to 0 since they hold no values
			for(I i = 0; i < this->m; i++) {
				this->IA.push_back(0);
			}

			// Set this to zero so the last row has the correct size of 0
			this->IA.push_back(0);
		}

		template <class I, class T>
		SparseMatrixCSR<I,T>::~SparseMatrixCSR()
		{
			// Nothing to do currently, vectors will cleanup themselves on destructor call
		}

		template <class I, class T>
		inline cupcfd::error::eCodes SparseMatrixCSR<I,T>::resize(I rows, I columns) {
			cupcfd::error::eCodes status;

			// Clear the matrix
			status = this->clear();
			CHECK_ECODE(status)

			if(rows < 1) return cupcfd::error::E_MATRIX_INVALID_ROW_SIZE;
			if(columns < 1) return cupcfd::error::E_MATRIX_INVALID_COL_SIZE;

			// Resize the existing vectors to 0 (vector can resize when adding later....)
			this->A.resize(0);
			this->IA.resize(0);
			this->JA.resize(0);

			// Update Max Capacity
			this->m = rows;
			this->n = columns;
			
			// Setup the initial state of the vectors, so they are suitable for CSR lookup
			for(I i = 0; i < this->m; i++) {
				this->IA.push_back(0);
			}
			this->IA.push_back(0);

			return cupcfd::error::E_SUCCESS;
		}

		template <class I, class T>
		inline cupcfd::error::eCodes SparseMatrixCSR<I,T>::clear() {
			// Clear the vectors
			this->A.clear();
			this->IA.clear();
			this->JA.clear();

			// Reset the number of non-zero values
			this->nnz = 0;

			// Re-setup the IA vector
			for(I i = 0; i < this->m; i++) {
				this->IA.push_back(0);
			}

			// Set this to zero so the last row has the correct size of 0
			this->IA.push_back(0);

			return cupcfd::error::E_SUCCESS;
		}

		template <class I, class T>
		inline cupcfd::error::eCodes SparseMatrixCSR<I,T>::setElement(I row, I col, T val) {
			cupcfd::error::eCodes status;

			// Store in a sorted fashion to try and speed up searches a bit
			// Still a linear search, but can at least reduce the number of
			// comparisons...

			// Error Check: Within Range -- Greater than baseIndex
			if((row < this->baseIndex) || (row >= this->baseIndex + this->m)) {
				return cupcfd::error::E_MATRIX_ROW_OOB;
			}

			if((col < this->baseIndex) || (col >= this->baseIndex + this->n)) {
				return cupcfd::error::E_MATRIX_COL_OOB;
			}

			// Find the row details
			I start = this->IA[row - this->baseIndex];
			I stop = this->IA[(row - this->baseIndex) + 1];
			I nEle = stop - start;

			// We store the column index using the matrix indexing (i.e. without the offset)
			I colFoundIndex;
			status = cupcfd::utility::drivers::linearSearch(&(this->JA[start]), nEle, col, &colFoundIndex);

			if(status == cupcfd::error::E_SEARCH_NOT_FOUND) {
				// If we reached here, we did not find a matching column, so it is not a non-zero value.
				// We will need to insert a new value - this requires:
				// (a(i)) inserting the value at a suitable place in A
				// (a(ii)) inserting the column index at the same location in JA
				// (b) increasing all of the index pointers after that point in IA by 1.
				// This makes this process expensive since it involves array shifts every insertion, and
				// O(n) vector element increments, so it is recommended to build the matrix in a different
				// format and convert to CSR once built.

				// (a) Begin by finding where to insert the value - we wish to store stored.
				// ToDo: We could move this out to a utility function

				// Default to end in case we don't find a column index that is larger
				I insertIndex = stop;
				for(I index = start; index < stop; index++) {
					// We store columns using the matrix index without the offset
					if(this->JA[index] > col) {
						// Instance where stored column is greater than the desired column to insert
						// Let's insert here and stop.
						insertIndex = index;
						break;
					}
				}

				// Now we have an index, do the inserts
				typename std::vector<T>::iterator it;
				it = this->A.begin();
				this->A.insert(it + insertIndex, val);

				typename std::vector<I>::iterator it2;
				it2 = this->JA.begin();
				this->JA.insert(it2 + insertIndex, col);

				this->nnz = this->nnz + 1;

				// Insert is done, update the start pointer values of all rows after
				// this one
				for(I index = ((row - this->baseIndex) + 1); index <= this->m; index++) {
					this->IA[index] = this->IA[index] + 1;
				}

				// Done
				return cupcfd::error::E_SUCCESS;
			}
			else if(status == cupcfd::error::E_SEARCH_SUCCESS) {
				// Value already exists, so we just need to overwrite
				this->A[start + colFoundIndex] = val;
				return cupcfd::error::E_SUCCESS;
			}
			else {
				// Some form of error other than value not found has occured.
				return status;
			}

			return cupcfd::error::E_SUCCESS;
		}

		template <class I, class T>
		inline cupcfd::error::eCodes SparseMatrixCSR<I,T>::getElement(I row, I col, T * val) {
			cupcfd::error::eCodes status;

			// Data should be stored in a sorted fashion (By row, then column)
			// Search through to the point where it is no longer possible to find
			// the value if it exists (row > data rows, or columns > col in a row
			// range)

			// Error Check: Ensure we are within range
			if((row < this->baseIndex) || (row >= this->baseIndex + this->m)) {
				return cupcfd::error::E_MATRIX_ROW_OOB;
			}

			if((col < this->baseIndex) || (col >= this->baseIndex + this->n)) {
				return cupcfd::error::E_MATRIX_COL_OOB;
			}

			// Get the start and end index for this row index
			// Remember to offset for matrix indexing scheme
			I start = this->IA[row - this->baseIndex];
			I stop = this->IA[(row - this->baseIndex) + 1];
			I nEle = stop - start;

			I colFoundIndex;

			// Search through column index list for this row till we find out row
			// ToDo: If storing in sorted order is enforced, no reason why we can't use binary search over linear....
			// Storing in sorted does make insertion slower though, but building a SparseCSR element by element costly anyway
			// and a different storage format would be recommended before conversion.

			// We store the column index using the matrix indexing (i.e. without the offset)
			status = cupcfd::utility::drivers::linearSearch(&(this->JA[start]), nEle, col, &colFoundIndex);

			if(status == cupcfd::error::E_SEARCH_NOT_FOUND) {
				// If we reached here, we did not find a matching column, so it is not a non-zero value.
				*val = 0.0;
				return cupcfd::error::E_SUCCESS;
			}
			else if(status == cupcfd::error::E_SEARCH_SUCCESS) {
				// colFoundIndex should hold the offset where the non-zero value is stored since the column
				// and nnz are stored at the same indexes in their respective vectors
				*val = this->A[start + colFoundIndex];
				return cupcfd::error::E_SUCCESS;
			}
			else {
				// Some form of error other than value not found has occured.
				// Will set value to 0.0 for tidyness, but really it is undefined past this point
				// since the error has to be handled.
				*val = 0.0;
				return status;
			}
		}

		template <class I, class T>
		inline cupcfd::error::eCodes SparseMatrixCSR<I,T>::getNonZeroRowIndexes(I ** rowIndexes, I * nRowIndexes) {
			// ToDo: This could probably be moved out to a utility function

			// First pass
			// Count number of non-zero size rows
			I count = 0;
			for(I i = 0; i < this->m; i++) {
				if((this->IA[i+1] - this->IA[i]) > 0) {
					count = count + 1;
				}
			}

			// Allocate memory
			*nRowIndexes = count;
			*rowIndexes = (I *) malloc(sizeof(I) * count);

			// Second pass
			// Set the row indexes that are non-zero
			count = 0;
			for(I i = 0; i < this->m; i++) {
				if((this->IA[i+1] - this->IA[i]) > 0) {
					// We're returning matrix row indexes, so make sure we include any matrix index offset
					(*rowIndexes)[count] = i + this->baseIndex;
					count = count + 1;
				}
			}

			return cupcfd::error::E_SUCCESS;
		}

		template <class I, class T>
		inline cupcfd::error::eCodes SparseMatrixCSR<I,T>::getRowColumnIndexes(I row, I ** columnIndexes, I * nColumnIndexes) {
			cupcfd::error::eCodes status;

			if((row - this->baseIndex) >= this->m || (row - this->baseIndex) < 0) {
				// The row does not exist - error
				return cupcfd::error::E_MATRIX_ROW_OOB;
			}

			// Get the index range for this row
			I start = this->IA[row - this->baseIndex];
			I stop = this->IA[(row - this->baseIndex) + 1];
			I nEle = stop - start;

			// Allocate the array
			*nColumnIndexes = nEle;
			*columnIndexes = (I *) malloc(sizeof(I) * (*nColumnIndexes));
			// Copy to the array
			status = cupcfd::utility::drivers::copy(&(this->JA[start]), nEle, *columnIndexes, (*nColumnIndexes));
			CHECK_ECODE(status)
			// *columnIndexes = cupcfd::utility::drivers::duplicate(&(this->JA[start]), nEle);

			return cupcfd::error::E_SUCCESS;
		}

		template <class I, class T>
		inline cupcfd::error::eCodes SparseMatrixCSR<I,T>::getRowNNZValues(I row, T ** nnzValues, I * nNNZValues) {
			cupcfd::error::eCodes status;
			
			if((row - this->baseIndex) >= this->m || (row - this->baseIndex) < 0) {
				// The row does not exist - error
				return cupcfd::error::E_MATRIX_ROW_OOB;
			}

			// Get the index range for this row
			I start = this->IA[row - this->baseIndex];
			I stop = this->IA[(row - this->baseIndex) + 1];
			I nEle = stop - start;

			// Allocate the array
			*nNNZValues = nEle;
			*nnzValues = (T *) malloc(sizeof(T) * (*nNNZValues));
			// Copy to the array
			status = cupcfd::utility::drivers::copy(&(this->A[start]), nEle, *nnzValues, (*nNNZValues));
			CHECK_ECODE(status)
			// *nnzValues = cupcfd::utility::drivers::duplicate(&(this->A[start]), nEle);

			// ToDo: Should check error status here...

			// Done without error
			return cupcfd::error::E_SUCCESS;
		}
	}
}

#endif
