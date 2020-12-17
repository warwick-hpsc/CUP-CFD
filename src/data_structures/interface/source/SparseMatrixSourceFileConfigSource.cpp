/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Contains definitions for the SparseMatrixSourceFileConfigSource class
 */

#include "SparseMatrixSourceFileConfigSource.h"

namespace cupcfd
{
	namespace data_structures
	{
		template <class I, class T>
		SparseMatrixSourceFileConfigSource<I,T>::SparseMatrixSourceFileConfigSource()
		:SparseMatrixSourceConfigSource<I,T>()
		{

		}

		template <class I, class T>
		SparseMatrixSourceFileConfigSource<I,T>::~SparseMatrixSourceFileConfigSource()
		{

		}
	}
}

// Explicit Instantiation
template class cupcfd::data_structures::SparseMatrixSourceFileConfigSource<int, float>;
template class cupcfd::data_structures::SparseMatrixSourceFileConfigSource<int, double>;
template class cupcfd::data_structures::SparseMatrixSourceFileConfigSource<long, float>;
template class cupcfd::data_structures::SparseMatrixSourceFileConfigSource<long, double>;
