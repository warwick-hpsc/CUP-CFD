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

		// === Concrete Functions ===

		/*
		template <class C, class I, class T>
		cupcfd::error::eCodes LinearSolverInterface<C,I,T>::setTolerances(T rTol, T eTol)
		{
			this->rTol = rTol;
			this->eTol = eTol;

			return cupcfd::error::E_SUCCESS;
		}

		template <class C, class I, class T>
		cupcfd::error::eCodes LinearSolverInterface<C,I,T>::resetTolerances()
		{
			this->rTol = 0.0;
			this->eTol = 0.0;

			return cupcfd::error::E_SUCCESS;
		}
		*/

		// === Pure Virtual Functions ===
		// These empty implementations are to satisfy the linker, since these will be overloaded
		// by children of this class

		template <class C, class I, class T>
		cupcfd::error::eCodes LinearSolverInterface<C,I,T>::reset()
		{
			return cupcfd::error::E_SUCCESS;
		}

		template <class C, class I, class T>
		cupcfd::error::eCodes LinearSolverInterface<C,I,T>::resetVectorX()
		{
			return cupcfd::error::E_SUCCESS;
		}

		template <class C, class I, class T>
		cupcfd::error::eCodes LinearSolverInterface<C,I,T>::resetVectorB()
		{
			return cupcfd::error::E_SUCCESS;
		}

		template <class C, class I, class T>
		cupcfd::error::eCodes LinearSolverInterface<C,I,T>::resetMatrixA()
		{
			return cupcfd::error::E_SUCCESS;
		}

		template <class C, class I, class T>
		cupcfd::error::eCodes LinearSolverInterface<C,I,T>::setupVectorX()
		{
			return cupcfd::error::E_SUCCESS;
		}

		template <class C, class I, class T>
		cupcfd::error::eCodes LinearSolverInterface<C,I,T>::setupVectorB()
		{
			return cupcfd::error::E_SUCCESS;
		}

		template <class C, class I, class T>
		cupcfd::error::eCodes LinearSolverInterface<C,I,T>::setupMatrixA(cupcfd::data_structures::SparseMatrix<C, I, T>& matrix)
		{
			return cupcfd::error::E_SUCCESS;
		}

		template <class C, class I, class T>
		cupcfd::error::eCodes LinearSolverInterface<C,I,T>::setup(cupcfd::data_structures::SparseMatrix<C, I, T>& matrix)
		{
			return cupcfd::error::E_SUCCESS;
		}

		template <class C, class I, class T>
		cupcfd::error::eCodes LinearSolverInterface<C,I,T>::setValuesVectorX(T scalar)
		{
			return cupcfd::error::E_SUCCESS;
		}

		template <class C, class I, class T>
		cupcfd::error::eCodes LinearSolverInterface<C,I,T>::setValuesVectorX(T * scalars, I nScalars, I * indexes, I nIndexes, I indexBase)
		{
			return cupcfd::error::E_SUCCESS;
		}

		template <class C, class I, class T>
		cupcfd::error::eCodes LinearSolverInterface<C,I,T>::setValuesVectorB(T scalar)
		{
			return cupcfd::error::E_SUCCESS;
		}

		template <class C, class I, class T>
		cupcfd::error::eCodes LinearSolverInterface<C,I,T>::setValuesVectorB(T * scalars, I nScalars, I * indexes, I nIndexes, I indexBase)
		{
			return cupcfd::error::E_SUCCESS;
		}

		template <class C, class I, class T>
		cupcfd::error::eCodes LinearSolverInterface<C,I,T>::setValuesMatrixA(cupcfd::data_structures::SparseMatrix<C, I, T>& matrix)
		{
			return cupcfd::error::E_SUCCESS;
		}

		template <class C, class I, class T>
		cupcfd::error::eCodes LinearSolverInterface<C,I,T>::getValuesVectorX(T ** result, I * nResult)
		{
			return cupcfd::error::E_SUCCESS;
		}

		template <class C, class I, class T>
		cupcfd::error::eCodes LinearSolverInterface<C,I,T>::getValuesVectorX(T ** result, I * nResult, I * indexes, I nIndexes, I indexBase)
		{
			return cupcfd::error::E_SUCCESS;
		}

		template <class C, class I, class T>
		cupcfd::error::eCodes LinearSolverInterface<C,I,T>::getValuesVectorB(T ** result, I * nResult)
		{
			return cupcfd::error::E_SUCCESS;
		}

		template <class C, class I, class T>
		cupcfd::error::eCodes LinearSolverInterface<C,I,T>::getValuesVectorB(T ** result, I * nResult, I * indexes, I nIndexes, I indexBase)
		{
			return cupcfd::error::E_SUCCESS;
		}

		template <class C, class I, class T>
		cupcfd::error::eCodes LinearSolverInterface<C,I,T>::getValuesMatrixA(cupcfd::data_structures::SparseMatrix<C, I, T>& matrix)
		{
			return cupcfd::error::E_SUCCESS;
		}

		template <class C, class I, class T>
		cupcfd::error::eCodes LinearSolverInterface<C,I,T>::clearVectorX()
		{
			return cupcfd::error::E_SUCCESS;
		}

		template <class C, class I, class T>
		cupcfd::error::eCodes LinearSolverInterface<C,I,T>::clearVectorB()
		{
			return cupcfd::error::E_SUCCESS;
		}

		template <class C, class I, class T>
		cupcfd::error::eCodes LinearSolverInterface<C,I,T>::clearMatrixA()
		{
			return cupcfd::error::E_SUCCESS;
		}

		template <class C, class I, class T>
		cupcfd::error::eCodes LinearSolverInterface<C,I,T>::solve()
		{
			return cupcfd::error::E_SUCCESS;
		}
	}
}

// Explicit Instantiation
template class cupcfd::linearsolvers::LinearSolverInterface<cupcfd::data_structures::SparseMatrixCSR<int, float>, int, float>;
template class cupcfd::linearsolvers::LinearSolverInterface<cupcfd::data_structures::SparseMatrixCOO<int, float>, int, float>;

template class cupcfd::linearsolvers::LinearSolverInterface<cupcfd::data_structures::SparseMatrixCSR<int, double>, int, double>;
template class cupcfd::linearsolvers::LinearSolverInterface<cupcfd::data_structures::SparseMatrixCOO<int, double>, int, double>;
