/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Contains definitions for the SparseMatrixSourceConfigSource class
 */

#include "SparseMatrixSourceConfigSource.h"

namespace cupcfd
{
	namespace data_structures
	{
		template <class I, class T>
		SparseMatrixSourceConfigSource<I,T>::SparseMatrixSourceConfigSource()
		{

		}

		template <class I, class T>
		SparseMatrixSourceConfigSource<I,T>::SparseMatrixSourceConfigSource(const SparseMatrixSourceConfigSource<I,T>& source)
		{

		}

		template <class I, class T>
		SparseMatrixSourceConfigSource<I,T>::~SparseMatrixSourceConfigSource()
		{

		}

		template <class I, class T>
		void SparseMatrixSourceConfigSource<I,T>::operator=(const SparseMatrixSourceConfigSource<I,T>& source)
		{

		}

		template <class I, class T>
		SparseMatrixSourceConfigSource<I,T> * SparseMatrixSourceConfigSource<I,T>::clone()
		{

		}

		template <class I, class T>
		cupcfd::error::eCodes SparseMatrixSourceConfigSource<I,T>::buildSparseMatrixSourceConfig(SparseMatrixSourceConfig<I,T> ** matrixSourceConfig)
		{

		}
	}
}

// Explicit Instantiation
template class cupcfd::data_structures::SparseMatrixSourceConfigSource<int, float>;
template class cupcfd::data_structures::SparseMatrixSourceConfigSource<int, double>;
template class cupcfd::data_structures::SparseMatrixSourceConfigSource<long, float>;
template class cupcfd::data_structures::SparseMatrixSourceConfigSource<long, double>;
