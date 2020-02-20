/*
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Contains the definitions for the SparseMatrixCSR class.
 *
 */

#include "SparseMatrixCSR.h"

namespace cupcfd
{
	namespace data_structures
	{

	}
}

// Explicit Instantiation
template class cupcfd::data_structures::SparseMatrixCSR<int, int>;
template class cupcfd::data_structures::SparseMatrixCSR<int, float>;
template class cupcfd::data_structures::SparseMatrixCSR<int, double>;
