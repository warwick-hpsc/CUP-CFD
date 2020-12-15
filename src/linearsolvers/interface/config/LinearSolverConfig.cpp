/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Contains the definitions for the LinearSolverConfig class
 */

#include "LinearSolverConfig.h"
#include "Communicator.h"

#include "SparseMatrixCSR.h"
#include "SparseMatrixCOO.h"

namespace cupcfd
{
	namespace linearsolvers
	{
		template <class C, class I, class T>
		LinearSolverConfig<C,I,T>::LinearSolverConfig()
		{

		}

		template <class C, class I, class T>
		LinearSolverConfig<C,I,T>::LinearSolverConfig(const LinearSolverConfig<C,I,T>& source)
		{
			*this = source;
		}

		template <class C, class I, class T>
		LinearSolverConfig<C,I,T>::~LinearSolverConfig()
		{

		}

		// template <class C, class I, class T>
		// LinearSolverConfig<C,I,T> * LinearSolverConfig<C,I,T>::clone()
		// {
		// 	// Pure Virtual Class, Left to inheriting classes (plus we can't return an abstract object)
		// }

		// template <class C, class I, class T>
		// cupcfd::error::eCodes LinearSolverConfig<C,I,T>::buildLinearSolver(LinearSolverInterface<C,I,T> ** solverSystem,
		// 																		cupcfd::data_structures::SparseMatrix<C,I,T>& matrix,
		// 																		cupcfd::comm::Communicator& solverComm)
		// {

		// }
	}
}

// Explicit Instantiation
template class cupcfd::linearsolvers::LinearSolverConfig<cupcfd::data_structures::SparseMatrixCSR<int,float>, int, float>;
template class cupcfd::linearsolvers::LinearSolverConfig<cupcfd::data_structures::SparseMatrixCSR<int,double>, int, double>;

template class cupcfd::linearsolvers::LinearSolverConfig<cupcfd::data_structures::SparseMatrixCOO<int,float>, int, float>;
template class cupcfd::linearsolvers::LinearSolverConfig<cupcfd::data_structures::SparseMatrixCOO<int,double>, int, double>;
