/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Contains the definitions for the LinearSolverConfigPETSc class
 */

#include "LinearSolverConfigPETSc.h"
#include "Communicator.h"

#include "SparseMatrixCSR.h"
#include "SparseMatrixCOO.h"

#include <iostream>

namespace cupcfd
{
	namespace linearsolvers
	{
		template <class C, class I, class T>
		LinearSolverConfigPETSc<C,I,T>::LinearSolverConfigPETSc(PETScAlgorithm solverAlg, T eTol, T rTol)
		: LinearSolverConfig<C,I,T>(),
		  solverAlg(solverAlg),
		  rTol(rTol),
		  eTol(eTol)
		{

		}

		template <class C, class I, class T>
		LinearSolverConfigPETSc<C,I,T>::LinearSolverConfigPETSc(const LinearSolverConfigPETSc<C,I,T>& source)
		{
			*this = source;
		}

		template <class C, class I, class T>
		LinearSolverConfigPETSc<C,I,T>::~LinearSolverConfigPETSc()
		{

		}

		template <class C, class I, class T>
		void LinearSolverConfigPETSc<C,I,T>::operator=(const LinearSolverConfigPETSc<C,I,T>& source)
		{
			this->eTol = source.eTol;
			this->rTol = source.rTol;
			this->solverAlg = source.solverAlg;
		}

		template <class C, class I, class T>
		LinearSolverConfigPETSc<C,I,T> * LinearSolverConfigPETSc<C,I,T>::clone()
		{
			return new LinearSolverConfigPETSc<C,I,T>(*this);
		}

		template <class C, class I, class T>
		cupcfd::error::eCodes LinearSolverConfigPETSc<C,I,T>::buildLinearSolver(LinearSolverInterface<C,I,T> ** solverSystem,
																					 cupcfd::data_structures::SparseMatrix<C,I,T>& matrix,
																					 cupcfd::comm::Communicator& solverComm)
		{
			// Create the PETSc Linear Solver Object
			*solverSystem = new LinearSolverPETSc<C,I,T>(solverComm, this->solverAlg, this->rTol, this->eTol, matrix);

			return cupcfd::error::E_SUCCESS;
		}
	}
}

// Explicit Instantiation
template class cupcfd::linearsolvers::LinearSolverConfigPETSc<cupcfd::data_structures::SparseMatrixCSR<int,float>, int, float>;
template class cupcfd::linearsolvers::LinearSolverConfigPETSc<cupcfd::data_structures::SparseMatrixCSR<int,double>, int, double>;

template class cupcfd::linearsolvers::LinearSolverConfigPETSc<cupcfd::data_structures::SparseMatrixCOO<int,float>, int, float>;
template class cupcfd::linearsolvers::LinearSolverConfigPETSc<cupcfd::data_structures::SparseMatrixCOO<int,double>, int, double>;
