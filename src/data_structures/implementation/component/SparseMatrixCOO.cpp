/*
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Contains the definitions for the SparseMatrixCOO class
 */

#include "SparseMatrixCOO.h"
#include "ArrayDrivers.h"

namespace cupcfd
{
	namespace data_structures
	{

	}
}

// Explicit Instantiation
template class cupcfd::data_structures::SparseMatrixCOO<int, int>;
template class cupcfd::data_structures::SparseMatrixCOO<int, float>;
template class cupcfd::data_structures::SparseMatrixCOO<int, double>;
