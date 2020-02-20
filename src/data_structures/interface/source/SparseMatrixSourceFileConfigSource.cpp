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
		SparseMatrixSourceFileConfigSource<I,T>::SparseMatrixSourceFileConfigSource(const SparseMatrixSourceFileConfigSource<I,T>& source)
		{

		}

		template <class I, class T>
		SparseMatrixSourceFileConfigSource<I,T>::~SparseMatrixSourceFileConfigSource()
		{

		}

		template <class I, class T>
		void SparseMatrixSourceFileConfigSource<I,T>::operator=(const SparseMatrixSourceFileConfigSource<I,T>& source)
		{

		}

		template <class I, class T>
		SparseMatrixSourceFileConfigSource<I,T> * SparseMatrixSourceFileConfigSource<I,T>::clone()
		{

		}

		template <class I, class T>
		cupcfd::error::eCodes SparseMatrixSourceFileConfigSource<I,T>::buildSparseMatrixSourceConfig(SparseMatrixSourceConfig<I,T> ** matrixSourceConfig)
		{

		}

		template <class I, class T>
		cupcfd::error::eCodes SparseMatrixSourceFileConfigSource<I,T>::getFilePath(std::string& filePath)
		{

		}

		template <class I, class T>
		cupcfd::error::eCodes SparseMatrixSourceFileConfigSource<I,T>::getFileFormat(SparseMatrixFileFormat * format)
		{

		}
	}
}

// Explicit Instantiation
template class cupcfd::data_structures::SparseMatrixSourceFileConfigSource<int, float>;
template class cupcfd::data_structures::SparseMatrixSourceFileConfigSource<int, double>;
template class cupcfd::data_structures::SparseMatrixSourceFileConfigSource<long, float>;
template class cupcfd::data_structures::SparseMatrixSourceFileConfigSource<long, double>;
