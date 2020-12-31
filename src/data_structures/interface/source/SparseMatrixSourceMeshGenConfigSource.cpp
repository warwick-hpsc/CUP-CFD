/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Contains definitions for the SparseMatrixSourceMeshGenConfigSource class
 */

#include "SparseMatrixSourceMeshGenConfigSource.h"

namespace cupcfd
{
	namespace data_structures
	{
		template <class I, class T>
		SparseMatrixSourceMeshGenConfigSource<I,T>::SparseMatrixSourceMeshGenConfigSource()
		:SparseMatrixSourceConfigSource<I,T>()
		{

		}

		template <class I, class T>
		SparseMatrixSourceMeshGenConfigSource<I,T>::~SparseMatrixSourceMeshGenConfigSource()
		{

		}
	}
}

// Explicit Instantiation
template class cupcfd::data_structures::SparseMatrixSourceMeshGenConfigSource<int, float>;
template class cupcfd::data_structures::SparseMatrixSourceMeshGenConfigSource<int, double>;
template class cupcfd::data_structures::SparseMatrixSourceMeshGenConfigSource<long, float>;
template class cupcfd::data_structures::SparseMatrixSourceMeshGenConfigSource<long, double>;
