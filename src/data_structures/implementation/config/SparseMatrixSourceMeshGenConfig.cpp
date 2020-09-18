/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Contains definitions for the SparseMatrixSourceMeshGenConfig class
 */

#include "SparseMatrixSourceMeshGenConfig.h"
#include "SparseMatrixSourceHDF5.h"

namespace cupcfd
{
	namespace data_structures
	{
		template <class I, class T>
		SparseMatrixSourceMeshGenConfig<I,T>::SparseMatrixSourceMeshGenConfig()
		: SparseMatrixSourceConfig<I,T>()
		{
			// Nothing to do beyond intialiser list currently.
		}

		template <class I, class T>
		SparseMatrixSourceMeshGenConfig<I,T>::SparseMatrixSourceMeshGenConfig(SparseMatrixSourceMeshGenConfig<I,T>& source)
		{
			*this = source;
		}

		template <class I, class T>
		SparseMatrixSourceMeshGenConfig<I,T>::~SparseMatrixSourceMeshGenConfig()
		{
			// Nothing to do beyond intialiser list currently.
		}

		template <class I, class T>
		void SparseMatrixSourceMeshGenConfig<I,T>::operator=(SparseMatrixSourceMeshGenConfig<I,T>& source)
		{

		}

		template <class I, class T>
		SparseMatrixSourceMeshGenConfig<I,T> * SparseMatrixSourceMeshGenConfig<I,T>::clone()
		{
			return new SparseMatrixSourceMeshGenConfig<I,T>(*this);
		}

		template <class I, class T>
		cupcfd::error::eCodes SparseMatrixSourceMeshGenConfig<I,T>::buildSparseMatrixSource(SparseMatrixSource<I,T> ** matrixSource)
		{
			// Not implemented.
			return cupcfd::error::E_ERROR;
		}
	}
}

// Explicit Instantiation
template class cupcfd::data_structures::SparseMatrixSourceMeshGenConfig<int, float>;
template class cupcfd::data_structures::SparseMatrixSourceMeshGenConfig<int, double>;
