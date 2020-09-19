/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Contains definitions for the SparseMatrixSourceMeshGen class
 */

// Class Declarations
#include "SparseMatrixSourceMeshGen.h"

// C++ Library Function/Objects
#include <vector>

namespace cupcfd
{
	namespace data_structures
	{
		template <class I, class T>
		SparseMatrixSourceMeshGen<I, T>::SparseMatrixSourceMeshGen()
		: SparseMatrixSource<I,T>()
		{

		}

		template <class I, class T>
		SparseMatrixSourceMeshGen<I, T>::~SparseMatrixSourceMeshGen()
		{
			// Currently Nothing to Clean Up.
		}

		// === Pure Virtual Methods ===
		// Empty implementations to satisy the linker.

		// template <class I, class T>
		// cupcfd::error::eCodes SparseMatrixSourceMeshGen<I, T>::getNNZ(I * nnz)
		// {
		// 	cupcfd::error::eCodes status;

		// 	return cupcfd::error::E_SUCCESS;
		// }

		// template <class I, class T>
		// cupcfd::error::eCodes SparseMatrixSourceMeshGen<I, T>::getNRows(I * nRows)
		// {
		// 	cupcfd::error::eCodes status;

		// 	return cupcfd::error::E_SUCCESS;
		// }

		// template <class I, class T>
		// cupcfd::error::eCodes SparseMatrixSourceMeshGen<I, T>::getNCols(I * nCols)
		// {
		// 	cupcfd::error::eCodes status;

		// 	return cupcfd::error::E_SUCCESS;
		// }

		// template <class I, class T>
		// cupcfd::error::eCodes SparseMatrixSourceMeshGen<I, T>::getMatrixIndicesBase(I * indicesBase)
		// {
		// 	cupcfd::error::eCodes status;

		// 	return cupcfd::error::E_SUCCESS;
		// }

		// template <class I, class T>
		// cupcfd::error::eCodes SparseMatrixSourceMeshGen<I, T>::getNNZRows(I * rowIndices, I nRowIndices)
		// {
		// 	cupcfd::error::eCodes status;

		// 	return cupcfd::error::E_SUCCESS;
		// }

		// template <class I, class T>
		// cupcfd::error::eCodes SparseMatrixSourceMeshGen<I, T>::getRowColumnIndexes(I rowIndex, I ** columnIndexes, I * nColumnIndexes)
		// {
		// 	cupcfd::error::eCodes status;

		// 	return cupcfd::error::E_SUCCESS;
		// }

		// template <class I, class T>
		// cupcfd::error::eCodes SparseMatrixSourceMeshGen<I,T>::getRowNNZValues(I rowIndex, T ** nnzValues, I * nNNZValues)
		// {
		// 	cupcfd::error::eCodes status;

		// 	return cupcfd::error::E_SUCCESS;
		// }
	}
}

// Explicit Instantiation
template class cupcfd::data_structures::SparseMatrixSourceMeshGen<int, float>;
template class cupcfd::data_structures::SparseMatrixSourceMeshGen<int, double>;

