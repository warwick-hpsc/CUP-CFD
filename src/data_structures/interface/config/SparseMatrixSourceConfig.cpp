/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Contains definitions for the SparseMatrixSourceConfig class
 */

#include "SparseMatrixSourceConfig.h"

namespace cupcfd
{
	namespace data_structures
	{
		// === Constructors/Deconstructors ===

		template <class I, class T>
		SparseMatrixSourceConfig<I,T>::SparseMatrixSourceConfig()
		{
			// Nothing to do
		}

		// template <class I, class T>
		// SparseMatrixSourceConfig<I,T>::SparseMatrixSourceConfig(SparseMatrixSourceConfig& source)
		// {
		// 	*this = source;
		// }

		template <class I, class T>
		SparseMatrixSourceConfig<I,T>::~SparseMatrixSourceConfig()
		{
			// Nothing to do
		}

		// === Concrete Methods ===

		// template <class I, class T>
		// void SparseMatrixSourceConfig<I,T>::operator=(SparseMatrixSourceConfig<I,T>& source)
		// {
		// 	// Nothing to do
		// }

		// // === Pure Virtual Methods ===
		// // Here to satisfy linker

		// template <class I, class T>
		// SparseMatrixSourceConfig<I,T> * SparseMatrixSourceConfig<I,T>::clone()
		// {
		// 	return nullptr;
		// }

		// template <class I, class T>
		// cupcfd::error::eCodes SparseMatrixSourceConfig<I,T>::buildSparseMatrixSource(SparseMatrixSource<I,T> ** matrixSource)
		// {
		// 	return cupcfd::error::E_SUCCESS;
		// }
	}
}

// Explicit Instantiation
using namespace cupcfd::data_structures;

template class cupcfd::data_structures::SparseMatrixSourceConfig<int, float>;
template class cupcfd::data_structures::SparseMatrixSourceConfig<int, double>;
