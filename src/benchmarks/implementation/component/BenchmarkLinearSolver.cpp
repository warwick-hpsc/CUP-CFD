/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Contains the definitions for the BenchmarkLinearSolver class.
 */

#include "BenchmarkLinearSolver.h"
#include "SparseMatrixCOO.h"
#include "SparseMatrixCSR.h"

#include "tt_interface_c.h"

#include <cstdlib>

#include <iostream>

namespace cupcfd
{
	namespace benchmark
	{
		template <class C, class I, class T>
		BenchmarkLinearSolver<C,I,T>::BenchmarkLinearSolver(std::string benchmarkName, I repetitions,
														  std::shared_ptr<cupcfd::data_structures::SparseMatrix<C,I,T>> matrixPtr,
														  std::shared_ptr<std::vector<T>> rhsVectorPtr,
														  std::shared_ptr<std::vector<T>> solVectorPtr,
														  std::shared_ptr<cupcfd::linearsolvers::LinearSolverInterface<C,I,T>> solverSystemPtr)
		: Benchmark<I,T>(benchmarkName, repetitions),
		  matrixPtr(matrixPtr),
		  rhsVectorPtr(rhsVectorPtr),
		  solVectorPtr(solVectorPtr),
		  solverSystemPtr(solverSystemPtr)
		{

		}

		template <class C, class I, class T>
		BenchmarkLinearSolver<C,I,T>::~BenchmarkLinearSolver()
		{
			// Shared Pointer will cleanup after itself as object is destroyed
		}

		template <class C, class I, class T>
		void BenchmarkLinearSolver<C,I,T>::setupBenchmark()
		{
			// Nothing to do here currently
		}

		template <class C, class I, class T>
		void BenchmarkLinearSolver<C,I,T>::recordParameters()
		{

		}

		template <class C, class I, class T>
		void BenchmarkLinearSolver<C,I,T>::runBenchmark()
		{

			// Get the non-zero rows assigned to this rank
			I * rowIndexes;
			I nRowIndexes;
			matrixPtr->getNonZeroRowIndexes(&rowIndexes, &nRowIndexes);

			// Start tracking parameters/time for this block
			this->startBenchmarkBlock(this->benchmarkName);
			TreeTimerLogParameterInt("Repetitions", this->repetitions);

			this->recordParameters();

			for(I i = 0; i < this->repetitions; i++)
			{
				// Clear Matrix A
				TreeTimerEnterLibraryCall("ClearMatrixA");
				this->solverSystemPtr->clearMatrixA();
				TreeTimerExit("ClearMatrixA");

				// Clear Vector X
				TreeTimerEnterLibraryCall("ClearVectorX");
				this->solverSystemPtr->clearVectorX();
				TreeTimerExit("ClearVectorX");

				// Clear Vector B
				TreeTimerEnterLibraryCall("ClearVectorB");
				this->solverSystemPtr->clearVectorB();
				TreeTimerExit("ClearVectorB");

				// Set Vector B Values
				TreeTimerEnterLibraryCall("SetValuesVectorB");
				this->solverSystemPtr->setValuesVectorB(&((*(this->rhsVectorPtr.get()))[0]), this->rhsVectorPtr->size(), rowIndexes, nRowIndexes, matrixPtr->baseIndex);
				TreeTimerExit("SetValuesVectorB");

				// Set Vector X Values
				TreeTimerEnterLibraryCall("SetValuesVectorX");
				this->solverSystemPtr->setValuesVectorX(&((*(this->solVectorPtr.get()))[0]), this->solVectorPtr->size(), rowIndexes, nRowIndexes, matrixPtr->baseIndex);
				TreeTimerExit("SetValuesVectorX");

				// Set Matrix A Values/Build Matrix A (Structures already set inside setup)
				// Matrix must have same structures as one used during setup
				TreeTimerEnterLibraryCall("SetValuesMatrixA");
				this->solverSystemPtr->setValuesMatrixA(*matrixPtr);
				TreeTimerExit("SetValuesMatrixA");

				// Run Linear Solver
				TreeTimerEnterLibraryCall("Solve");
				this->solverSystemPtr->solve();
				TreeTimerExit("Solve");
			}

			// Stop tracking parameters/time for this block
			this->stopBenchmarkBlock(this->benchmarkName);

			// Cleanup
			free(rowIndexes);
		}
	}
}

// Explicit Instantiation

template class cupcfd::benchmark::BenchmarkLinearSolver<cupcfd::data_structures::SparseMatrixCOO<int,float>, int, float>;
template class cupcfd::benchmark::BenchmarkLinearSolver<cupcfd::data_structures::SparseMatrixCOO<int,double>, int, double>;

template class cupcfd::benchmark::BenchmarkLinearSolver<cupcfd::data_structures::SparseMatrixCSR<int,float>, int, float>;
template class cupcfd::benchmark::BenchmarkLinearSolver<cupcfd::data_structures::SparseMatrixCSR<int,double>, int, double>;
