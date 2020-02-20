/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Contains definitions for the SparseMatrixSource class
 */

#include "SparseMatrixSource.h"

namespace cupcfd
{
	namespace data_structures
	{
		template <class I, class T>
		SparseMatrixSource<I, T>::SparseMatrixSource()
		{
			// Currently Nothing to Set.
		}

		template <class I, class T>
		SparseMatrixSource<I, T>::~SparseMatrixSource()
		{
			// Currently Nothing to Clean Up.
		}

		// === Pure Virtual Methods ===
		// Empty implementations to satisy the linker.

		template <class I, class T>
		cupcfd::error::eCodes SparseMatrixSource<I, T>::getNNZ(I * nnz)
		{
			return cupcfd::error::E_SUCCESS;
		}

		template <class I, class T>
		cupcfd::error::eCodes SparseMatrixSource<I, T>::getNRows(I * nRows)
		{
			return cupcfd::error::E_SUCCESS;
		}

		template <class I, class T>
		cupcfd::error::eCodes SparseMatrixSource<I, T>::getNCols(I * nCols)
		{
			return cupcfd::error::E_SUCCESS;
		}

		template <class I, class T>
		cupcfd::error::eCodes SparseMatrixSource<I, T>::getMatrixIndicesBase(I * indicesBase)
		{
			return cupcfd::error::E_SUCCESS;
		}

		template <class I, class T>
		cupcfd::error::eCodes SparseMatrixSource<I, T>::getNNZRows(I * rowIndices, I nRowIndices)
		{
			return cupcfd::error::E_SUCCESS;
		}

		template <class I, class T>
		cupcfd::error::eCodes SparseMatrixSource<I, T>::getRowColumnIndexes(I rowIndex, I ** columnIndexes, I * nColumnIndexes)
		{
			return cupcfd::error::E_SUCCESS;
		}

		template <class I, class T>
		cupcfd::error::eCodes SparseMatrixSource<I, T>::getRowNNZValues(I rowIndex, T ** nnzValues, I * nNNZValues)
		{
			return cupcfd::error::E_SUCCESS;
		}
	}
}

// Explicit Instantiation
template class cupcfd::data_structures::SparseMatrixSource<int, float>;
template class cupcfd::data_structures::SparseMatrixSource<int, double>;
template class cupcfd::data_structures::SparseMatrixSource<long, float>;
template class cupcfd::data_structures::SparseMatrixSource<long, double>;
