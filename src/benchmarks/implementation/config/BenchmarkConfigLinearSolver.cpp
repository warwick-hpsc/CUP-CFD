/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Contains definitions for the BenchmarkConfigLinearSolver class
 */

#include "BenchmarkConfigLinearSolver.h"
#include "SparseMatrixCOO.h"
#include "SparseMatrixCSR.h"
#include <memory>
#include "Communicator.h"
#include <vector>
#include <cstdlib>
#include "VectorSource.h"

namespace cupcfd
{
	namespace benchmark
	{
		// === Constructors/Deconstructors ===
		template <class C, class I, class T>
		BenchmarkConfigLinearSolver<C,I,T>::BenchmarkConfigLinearSolver(std::string benchmarkName, I repetitions,
																		BenchSolverDistribution distType,
																		cupcfd::linearsolvers::LinearSolverConfig<C,I,T>& linearSolverConfig,
																		cupcfd::data_structures::SparseMatrixSourceConfig<I,T>& matrixSourceConfig,
																		cupcfd::data_structures::VectorSourceConfig<I,T>& rhsSourceConfig,
																		cupcfd::data_structures::VectorSourceConfig<I,T>& solSourceConfig)
		: benchmarkName(benchmarkName),
		  repetitions(repetitions),
		  distType(distType)
		{
			this->linearSolverConfig = linearSolverConfig.clone();
			this->matrixSourceConfig = matrixSourceConfig.clone();
			this->rhsSourceConfig = rhsSourceConfig.clone();
			this->solSourceConfig = solSourceConfig.clone();

		}

		template <class C, class I, class T>
		BenchmarkConfigLinearSolver<C,I,T>::BenchmarkConfigLinearSolver(const BenchmarkConfigLinearSolver<C,I,T>& source)
		{
			*this = source;
		}

		template <class C, class I, class T>
		BenchmarkConfigLinearSolver<C,I,T>::~BenchmarkConfigLinearSolver()
		{
			delete(this->matrixSourceConfig);
			delete(this->linearSolverConfig);
		}

		// === Overloaded Inherited Methods ===

		template <class C, class I, class T>
		void BenchmarkConfigLinearSolver<C,I,T>::operator=(const BenchmarkConfigLinearSolver<C,I,T>& source) {
			this->benchmarkName = source.benchmarkName;
			this->repetitions = source.repetitions;
		}

		template <class C, class I, class T>
		BenchmarkConfigLinearSolver<C,I,T> * BenchmarkConfigLinearSolver<C,I,T>::clone() {
			return new BenchmarkConfigLinearSolver<C,I,T>(*this);
		}

		template <class C, class I, class T>
		cupcfd::error::eCodes BenchmarkConfigLinearSolver<C,I,T>::buildBenchmark(BenchmarkLinearSolver<C,I,T> ** bench) {
			cupcfd::error::eCodes status;
			cupcfd::data_structures::SparseMatrixSource<I,T> * matrixSource;
			cupcfd::linearsolvers::LinearSolverInterface<C,I,T> * solverSystem;

			// Build the Matrix Source
			status = this->matrixSourceConfig->buildSparseMatrixSource(&matrixSource);
			CHECK_ERROR_CODE(status)
			if(status != cupcfd::error::E_SUCCESS) {
				return status;
			}

			// Build the Matrix, distributing the data as established prior

			// ToDo: For now since not Mesh Gen Source, just evenly divide the matrix between ranks...
			// Will likely need to pass through a graph pointer at some point to get an idea of the connectivity
			// and distribution of data

			// Get Matrix Size From Source
			I rows, cols, base;

			status = matrixSource->getNRows(&rows);
			CHECK_ERROR_CODE(status)
			if(status != cupcfd::error::E_SUCCESS) {
				delete matrixSource;
				return status;
			}

			status = matrixSource->getNCols(&cols);
			CHECK_ERROR_CODE(status)
			if(status != cupcfd::error::E_SUCCESS) {
				delete matrixSource;
				return status;
			}

			status = matrixSource->getMatrixIndicesBase(&base);
			CHECK_ERROR_CODE(status)
			if(status != cupcfd::error::E_SUCCESS) {
				delete matrixSource;
				return status;
			}

			// Make the new matrix with a base of 0
			std::shared_ptr<C> matrix = std::make_shared<C>(rows, cols, 0);

			// ToDo: Could move this into SparseMatrixSource class
			if(this->distType == BENCH_SOLVER_DIST_CONCURRENT) {
				// Load Full Matrix

				// Row Indexes

				for(I i = 0; i < rows; i++) {
					I rowIndex = i + base;

					I * columnIndexes;
					I nColumnIndexes;

					T * nnzValues;
					I nNNZValues;

					matrixSource->getRowColumnIndexes(rowIndex, &columnIndexes, &nColumnIndexes);
					matrixSource->getRowNNZValues(rowIndex, &nnzValues, &nNNZValues);

					for( I j = 0; j < nColumnIndexes; j++) {
						// ToDo: Element by Element call is likely v.slow
						status = matrix->setElement(i, columnIndexes[j] - base, nnzValues[j]);
						CHECK_ERROR_CODE(status)
					}

					free(columnIndexes);
					free(nnzValues);
				}

				// Perform the initial setup for a linear solver system
				// All ranks are solving the same matrix independently
				cupcfd::comm::Communicator solverComm(MPI_COMM_SELF);
				status = this->linearSolverConfig->buildLinearSolver(&solverSystem, *matrix, solverComm);
				CHECK_ERROR_CODE(status)
				if(status != cupcfd::error::E_SUCCESS) {
					return status;
				}

			}
			else if(this->distType == BENCH_SOLVER_DIST_DISTRIBUTED) {
				// For now, we will split the rows evenly across all communicators - this may not be representative
				// of a mesh decomposition however
				cupcfd::comm::Communicator solverComm(MPI_COMM_WORLD);

				I baseSize = (rows / solverComm.size);
				if(solverComm.rank < (rows % solverComm.size)) {
					baseSize = baseSize + 1;
				}

				I startRow;

				if(solverComm.rank <= (rows % solverComm.size)) {
					startRow = ((rows / solverComm.size) + 1) * solverComm.rank;
				}
				else {
					startRow = (((rows / solverComm.size) + 1) * (rows % solverComm.size)) + ((rows / solverComm.size) * (solverComm.rank - (rows % solverComm.size)));
				}

				// Row Indexes
				for(I i = startRow; i < (startRow + baseSize); i++) {
					I rowIndex = i + base;

					I * columnIndexes;
					I nColumnIndexes;

					T * nnzValues;
					I nNNZValues;

					matrixSource->getRowColumnIndexes(rowIndex, &columnIndexes, &nColumnIndexes);
					matrixSource->getRowNNZValues(rowIndex, &nnzValues, &nNNZValues);

					for( I j = 0; j < nColumnIndexes; j++) {
						// ToDo: Element by Element call is likely v.slow
						status = matrix->setElement(i, columnIndexes[j] - base, nnzValues[j]);
						CHECK_ERROR_CODE(status)
					}

					free(columnIndexes);
					free(nnzValues);
				}

				// Perform the initial setup for a linear solver system
				// Matrix should be distributed (i.e. each rank has different rows)
				// This requires a parallel solver, and thus a non-serial communicator.

				status = this->linearSolverConfig->buildLinearSolver(&solverSystem, *matrix, solverComm);
				CHECK_ERROR_CODE(status)
				if(status != cupcfd::error::E_SUCCESS) {
					return status;
				}
			}
			else {
				return cupcfd::error::E_ERROR;
			}

			// Make RHS Vector Source
			cupcfd::data_structures::VectorSource<I,T> * rhsSource;
			status = this->rhsSourceConfig->buildVectorSource(&rhsSource);
			CHECK_ERROR_CODE(status)
			if(status != cupcfd::error::E_SUCCESS) {
				return status;
			}

			// Make Sol Vector Source
			cupcfd::data_structures::VectorSource<I,T> * solSource;
			status = this->rhsSourceConfig->buildVectorSource(&solSource);
			CHECK_ERROR_CODE(status)
			if(status != cupcfd::error::E_SUCCESS) {
				return status;
			}

			// Get the non-zero rows assigned to this rank
			I * rowIndexes;
			I nRowIndexes;
			status = matrix->getNonZeroRowIndexes(&rowIndexes, &nRowIndexes);
			CHECK_ERROR_CODE(status)

			// Setup a data array
			T nData = nRowIndexes;
			T * data = (T *) malloc(sizeof(T) * nData);

			// Populate RHS Vector
			std::shared_ptr<std::vector<T>> rhsVectorPtr = std::make_shared<std::vector<T>>();
			rhsSource->getData(data, nData, rowIndexes, nRowIndexes, matrix->baseIndex);
			for(I i = 0; i < nRowIndexes; i++) {
				rhsVectorPtr->push_back(data[i]);
			}

			// Populate Sol Vector
			std::shared_ptr<std::vector<T>> solVectorPtr = std::make_shared<std::vector<T>>();
			solSource->getData(data, nData, rowIndexes, nRowIndexes, matrix->baseIndex);
			for(I i = 0; i < nRowIndexes; i++) {
				solVectorPtr->push_back(data[i]);
			}

			free(rhsSource);
			free(solSource);
			free(data);
			free(rowIndexes);

			// Build the benchmark - we leave the setup of the matrix/vectors in the linear solver system
			// to the benchmark, since we might wish to benchmark the time it takes.
			// Convert raw pointer to shared pointer (this is the only occurence currently)
			std::shared_ptr<cupcfd::linearsolvers::LinearSolverInterface<C,I,T>> solverSystemSharedPtr(solverSystem);

			// Build Benchmark Object
			*bench = new BenchmarkLinearSolver<C,I,T>(this->benchmarkName, this->repetitions, matrix, rhsVectorPtr, solVectorPtr, solverSystemSharedPtr);

			delete matrixSource;

			return cupcfd::error::E_SUCCESS;
		}
	}
}

// Explicit Instantiation
template class cupcfd::benchmark::BenchmarkConfigLinearSolver<cupcfd::data_structures::SparseMatrixCOO<int,float>, int, float>;
template class cupcfd::benchmark::BenchmarkConfigLinearSolver<cupcfd::data_structures::SparseMatrixCOO<int,double>, int, double>;

template class cupcfd::benchmark::BenchmarkConfigLinearSolver<cupcfd::data_structures::SparseMatrixCSR<int,float>, int, float>;
template class cupcfd::benchmark::BenchmarkConfigLinearSolver<cupcfd::data_structures::SparseMatrixCSR<int,double>, int, double>;
