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
	}
}

// Explicit Instantiation
template class cupcfd::data_structures::SparseMatrixSource<int, float>;
template class cupcfd::data_structures::SparseMatrixSource<int, double>;
template class cupcfd::data_structures::SparseMatrixSource<long, float>;
template class cupcfd::data_structures::SparseMatrixSource<long, double>;
