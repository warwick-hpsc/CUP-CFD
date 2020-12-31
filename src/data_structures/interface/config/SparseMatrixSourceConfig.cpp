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

		template <class I, class T>
		SparseMatrixSourceConfig<I,T>::~SparseMatrixSourceConfig()
		{
			// Nothing to do
		}
	}
}

// Explicit Instantiation
using namespace cupcfd::data_structures;

template class cupcfd::data_structures::SparseMatrixSourceConfig<int, float>;
template class cupcfd::data_structures::SparseMatrixSourceConfig<int, double>;
