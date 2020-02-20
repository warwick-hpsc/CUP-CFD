/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Class Definitions for the LinearSolverConfigSource class.
 */

#include "LinearSolverConfigSource.h"

#include "SparseMatrixCSR.h"
#include "SparseMatrixCOO.h"

namespace cupcfd
{
	namespace linearsolvers
	{
		// === Constructors/Deconstructors ===

		template <class C, class I, class T>
		LinearSolverConfigSource<C,I,T>::LinearSolverConfigSource()
		{

		}

		template <class C, class I, class T>
		LinearSolverConfigSource<C,I,T>::~LinearSolverConfigSource()
		{
			// Nothing to do currently
		}

		// === Concrete Methods ===

		template <class C, class I, class T>
		cupcfd::error::eCodes LinearSolverConfigSource<C,I,T>::buildLinearSolverConfig(LinearSolverConfig<C,I,T> ** linearSolverConfig)
		{
			// Pure Virtual
		}
	}
}

template class cupcfd::linearsolvers::LinearSolverConfigSource<cupcfd::data_structures::SparseMatrixCSR<int,float>, int, float>;
template class cupcfd::linearsolvers::LinearSolverConfigSource<cupcfd::data_structures::SparseMatrixCSR<int,double>, int, double>;

template class cupcfd::linearsolvers::LinearSolverConfigSource<cupcfd::data_structures::SparseMatrixCOO<int,float>, int, float>;
template class cupcfd::linearsolvers::LinearSolverConfigSource<cupcfd::data_structures::SparseMatrixCOO<int,double>, int, double>;
