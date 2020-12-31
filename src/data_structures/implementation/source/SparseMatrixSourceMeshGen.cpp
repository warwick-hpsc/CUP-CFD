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
	}
}

// Explicit Instantiation
template class cupcfd::data_structures::SparseMatrixSourceMeshGen<int, float>;
template class cupcfd::data_structures::SparseMatrixSourceMeshGen<int, double>;

