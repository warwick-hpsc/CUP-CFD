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
			cupcfd::error::eCodes err;

			// Get the non-zero rows assigned to this rank
			I * rowIndexes;
			I nRowIndexes;
			err = matrixPtr->getNonZeroRowIndexes(&rowIndexes, &nRowIndexes);
			if (err != cupcfd::error::E_SUCCESS) {
				DEBUGGABLE_ERROR;
				throw( std::string("BenchmarkLinearSolver::runBenchmark(): ERROR: ") + cupcfd::error::eStrings[err]);
			}

			// Start tracking parameters/time for this block
			this->startBenchmarkBlock(this->benchmarkName);
			TreeTimerLogParameterInt("Repetitions", this->repetitions);

			this->recordParameters();

			for(I i = 0; i < this->repetitions; i++)
			{
				// Clear Matrix A
				this->startBenchmarkBlock("ClearMatrixA");
				this->solverSystemPtr->clearMatrixA();
				this->stopBenchmarkBlock("ClearMatrixA");

				// Clear Vector X
				this->startBenchmarkBlock("ClearVectorX");
				this->solverSystemPtr->clearVectorX();
				this->stopBenchmarkBlock("ClearVectorX");

				// Clear Vector B
				this->startBenchmarkBlock("ClearVectorB");
				this->solverSystemPtr->clearVectorB();
				this->stopBenchmarkBlock("ClearVectorB");

				// Set Vector B Values
				this->startBenchmarkBlock("SetValuesVectorB");
				this->solverSystemPtr->setValuesVectorB(&((*(this->rhsVectorPtr.get()))[0]), this->rhsVectorPtr->size(), rowIndexes, nRowIndexes, matrixPtr->baseIndex);
				this->stopBenchmarkBlock("SetValuesVectorB");

				// Set Vector X Values
				this->startBenchmarkBlock("SetValuesVectorX");
				this->solverSystemPtr->setValuesVectorX(&((*(this->solVectorPtr.get()))[0]), this->solVectorPtr->size(), rowIndexes, nRowIndexes, matrixPtr->baseIndex);
				this->stopBenchmarkBlock("SetValuesVectorX");

				// Set Matrix A Values/Build Matrix A (Structures already set inside setup)
				// Matrix must have same structures as one used during setup
				this->startBenchmarkBlock("SetValuesMatrixA");
				this->solverSystemPtr->setValuesMatrixA(*matrixPtr);
				this->stopBenchmarkBlock("SetValuesMatrixA");

				// Run Linear Solver
				this->startBenchmarkBlock("Solve");
				this->solverSystemPtr->solve();
				this->stopBenchmarkBlock("Solve");
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
