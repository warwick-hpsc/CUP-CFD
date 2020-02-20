/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Contains definitions for the SparseMatrixSourceHDF5 class
 */

// Class Declarations
#include "SparseMatrixSourceHDF5.h"

// C++ Library Function/Objects
#include <vector>

// HDF5 I/O Access
#include "HDF5Record.h"
#include "HDF5Access.h"

#include <iostream>

namespace cupcfd
{
	namespace data_structures
	{
		// This HDF5 file is formatted as follows:
		// Attributes:
		// Group | Name | Description
		// "/"	 | "nnz" | The number of non-zero values stored in this file
		// "/"	 | "nrows" | The number of rows in the matrix
		// "/"	 | "ncols" | The number of columns in the matrix
		// "/"   | "base"  | The base index of the matrix data (i.e. all indices in this file use this values as a base)

		// DataSets:
		// Group | Name | Description
		// "/"	 | "nonzero" | The array of non-zero values - size nnz
		// "/"	 | "cols" 	 | The array of columns of each non-zero value - size nnz
		// "/"	 | "rowrange"  | The array of HDF5 indexes for the start of each row (i.e. like CSR), including rows with no values
		//						 The sizes is nRows + 1. Note this is always zero-indexed.

		// Note: All NNZ values must be stored sorted by row, then column order

		template <class I, class T>
		SparseMatrixSourceHDF5<I, T>::SparseMatrixSourceHDF5(std::string fileName)
		: fileName(fileName)
		{

		}

		template <class I, class T>
		SparseMatrixSourceHDF5<I, T>::~SparseMatrixSourceHDF5()
		{
			// Currently Nothing to Clean Up.
		}

		// === Pure Virtual Methods ===
		// Empty implementations to satisy the linker.

		template <class I, class T>
		cupcfd::error::eCodes SparseMatrixSourceHDF5<I, T>::getNNZ(I * nnz)
		{
			cupcfd::error::eCodes status;

			// This format will store it's attributes at the root level
			cupcfd::io::hdf5::HDF5Record record("/", "nnz",true);
			cupcfd::io::hdf5::HDF5Access access(this->fileName, record);
			access.readData(nnz);

			return cupcfd::error::E_SUCCESS;
		}

		template <class I, class T>
		cupcfd::error::eCodes SparseMatrixSourceHDF5<I, T>::getNRows(I * nRows)
		{
			cupcfd::error::eCodes status;

			// This format will store it's attributes at the root level
			cupcfd::io::hdf5::HDF5Record record("/", "nrows",true);
			cupcfd::io::hdf5::HDF5Access access(this->fileName, record);
			access.readData(nRows);

			return cupcfd::error::E_SUCCESS;
		}

		template <class I, class T>
		cupcfd::error::eCodes SparseMatrixSourceHDF5<I, T>::getNCols(I * nCols)
		{
			cupcfd::error::eCodes status;

			// This format will store it's attributes at the root level
			cupcfd::io::hdf5::HDF5Record record("/", "ncols",true);
			cupcfd::io::hdf5::HDF5Access access(this->fileName, record);
			access.readData(nCols);

			return cupcfd::error::E_SUCCESS;
		}

		template <class I, class T>
		cupcfd::error::eCodes SparseMatrixSourceHDF5<I, T>::getMatrixIndicesBase(I * indicesBase)
		{
			cupcfd::error::eCodes status;

			// This format will store it's attributes at the root level
			cupcfd::io::hdf5::HDF5Record record("/", "base",true);
			cupcfd::io::hdf5::HDF5Access access(this->fileName, record);
			access.readData(indicesBase);

			return cupcfd::error::E_SUCCESS;
		}

		template <class I, class T>
		cupcfd::error::eCodes SparseMatrixSourceHDF5<I, T>::getNNZRows(I * rowIndices, I nRowIndices)
		{
			cupcfd::error::eCodes status;

			// Get the number of rows
			I nRows;
			status = this->getNRows(&nRows);

			if(status != cupcfd::error::E_SUCCESS)
			{
				return status;
			}

			// Error Check: Size of the rowIndices array should be the same as the number of rows
			if(nRowIndices != nRows)
			{
				return cupcfd::error::E_ARRAY_MISMATCH_SIZE;
			}

			// Get the row ranges from the file - number of entries should be stored rows + 1
			I * rowRanges = (I *) malloc(sizeof(I) * (nRows + 1));

			// Setup Access
			cupcfd::io::hdf5::HDF5Record record("/", "rowrange" , false);
			cupcfd::io::hdf5::HDF5Access access(this->fileName, record);

			// Read Data
			access.readData(rowRanges);

			// Compute the sizes from the range difference
			for(I index = 0; index < nRows; index++)
			{
				rowIndices[index] = rowRanges[index + 1] - rowRanges[index];
			}

			// Cleanup
			free(rowRanges);

			return cupcfd::error::E_SUCCESS;
		}

		template <class I, class T>
		cupcfd::error::eCodes SparseMatrixSourceHDF5<I, T>::getRowColumnIndexes(I rowIndex, I ** columnIndexes, I * nColumnIndexes)
		{
			cupcfd::error::eCodes status;

			// Error Check: is rowIndex < number of rows?

			// === Get the row range from the file - start + end ===
			I rowRange[2];

			// Setup Access
			cupcfd::io::hdf5::HDF5Record record("/", "rowrange" , false);
			cupcfd::io::hdf5::HDF5Access access(this->fileName, record);
			cupcfd::io::hdf5::HDF5Properties properties(access);

			// Select Indexes
			// Get Matrix Base Index
			I base;
			status = this->getMatrixIndicesBase(&base);

			// Labels are zero-based, HDF5 Indexes are base-0
			properties.addIndex(rowIndex - base);
			properties.addIndex((rowIndex + 1) - base);

			// Get the range
			access.readData(rowRange, properties);
			*nColumnIndexes = rowRange[1] - rowRange[0];

			// Setup the data store array
			// Technically should never be negative, but may as well catch this case
			if(*nColumnIndexes < 1)
			{
				// Function still expected to return a non-null pointer.
				*columnIndexes = (I *) malloc(sizeof(I) * 0);
			}
			else
			{
				*columnIndexes = (I *) malloc(sizeof(I) * (*nColumnIndexes));
			}

			// Read Data if the range says it exists
			if(*nColumnIndexes > 0)
			{
				// Read the column data
				cupcfd::io::hdf5::HDF5Record record2("/", "cols" , false);
				cupcfd::io::hdf5::HDF5Access access2(this->fileName, record2);
				cupcfd::io::hdf5::HDF5Properties properties2(access2);

				for(I i = 0; i < *nColumnIndexes; i++)
				{
					// The indexes from rowrange are HDF5 indexes, so no need to remove the base.
					properties2.addIndex(rowRange[0] + i);
				}

				// Read the column data
				if(*nColumnIndexes > 0)
				{
					access2.readData(*columnIndexes, properties2);
				}
			}

			return cupcfd::error::E_SUCCESS;
		}

		template <class I, class T>
		cupcfd::error::eCodes SparseMatrixSourceHDF5<I,T>::getRowNNZValues(I rowIndex, T ** nnzValues, I * nNNZValues)
		{
			cupcfd::error::eCodes status;

			// Error Check: is rowIndex < number of rows and greater or equal to the base?

			// === Get the row range from the file - start + end ===

			I rowRange[2];

			// Setup Access
			cupcfd::io::hdf5::HDF5Record record("/", "rowrange" , false);
			cupcfd::io::hdf5::HDF5Access access(this->fileName, record);
			cupcfd::io::hdf5::HDF5Properties properties(access);



			// Select Indexes
			// Get Matrix Base Index
			I base;
			status = this->getMatrixIndicesBase(&base);

			// HDF5 Indexes are base-0
			properties.addIndex(rowIndex - base);
			properties.addIndex((rowIndex + 1) - base);

			// Get the range
			access.readData(rowRange, properties);
			*nNNZValues = rowRange[1] - rowRange[0];

			// Setup the data store array
			*nnzValues = (T *) malloc(sizeof(T) * (*nNNZValues));

			// Read the column data
			cupcfd::io::hdf5::HDF5Record record2("/", "nonzero" , false);
			cupcfd::io::hdf5::HDF5Access access2(this->fileName, record2);
			cupcfd::io::hdf5::HDF5Properties properties2(access2);

			for(I i = 0; i < *nNNZValues; i++)
			{
				// The indexes from rowrange are HDF5 indexes, so no need to remove the base.
				properties2.addIndex(rowRange[0] + i);
			}

			// Read the column data
			if(*nNNZValues > 0)
			{
				access2.readData(*nnzValues, properties2);
			}

			return cupcfd::error::E_SUCCESS;
		}
	}
}

// Explicit Instantiation
template class cupcfd::data_structures::SparseMatrixSourceHDF5<int, float>;
template class cupcfd::data_structures::SparseMatrixSourceHDF5<int, double>;

