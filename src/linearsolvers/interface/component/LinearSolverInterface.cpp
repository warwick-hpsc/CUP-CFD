/*
 * @file
 * @author Unviersity of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Contains the definitions for the LinearSolverInterface class
 */

#include "LinearSolverInterface.h"
#include "SparseMatrixCSR.h"
#include "SparseMatrixCOO.h"

namespace cupcfd
{
	namespace linearsolvers
	{
		// Constructor/Deconstructors
		template <class C, class I, class T>
		LinearSolverInterface<C,I,T>::LinearSolverInterface(cupcfd::comm::Communicator comm, I mGlobal, I nGlobal)
		:comm(comm),
		 mGlobal(mGlobal),
		 nGlobal(nGlobal)
		{

		}

		template <class C, class I, class T>
		LinearSolverInterface<C,I,T>::~LinearSolverInterface()
		{

		}
	}
}

// Explicit Instantiation
template class cupcfd::linearsolvers::LinearSolverInterface<cupcfd::data_structures::SparseMatrixCSR<int, float>, int, float>;
template class cupcfd::linearsolvers::LinearSolverInterface<cupcfd::data_structures::SparseMatrixCOO<int, float>, int, float>;

template class cupcfd::linearsolvers::LinearSolverInterface<cupcfd::data_structures::SparseMatrixCSR<int, double>, int, double>;
template class cupcfd::linearsolvers::LinearSolverInterface<cupcfd::data_structures::SparseMatrixCOO<int, double>, int, double>;
