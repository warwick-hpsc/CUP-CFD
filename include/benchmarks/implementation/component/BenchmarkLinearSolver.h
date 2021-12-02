/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Contains the declarations for the BenchmarkLinearSolver class.
 */

#ifndef CUPCFD_BENCHMARK_BENCHMARK_LINEARSOLVER_INCLUDE_H
#define CUPCFD_BENCHMARK_BENCHMARK_LINEARSOLVER_INCLUDE_H

#include "Benchmark.h"

#include <memory>
#include <vector>

#include "SparseMatrix.h"
#include "LinearSolverInterface.h"
#include "src/structures.h"
#include "mpi.h"
namespace cupcfd
{
	namespace benchmark
	{
		/**
		 * @tparam C The specialisation class of the Matrix
		 * @tparam I The type of the indexing scheme
		 * @tparam T The datatype of the matrix/linear solver system
		 */
		template <class C, class I, class T>
		class BenchmarkLinearSolver : public Benchmark<I,T>
		{
			public:
				// === Members ===

				/** Shared Pointer to the Matrix to use for solving during the benchmark **/
				std::shared_ptr<cupcfd::data_structures::SparseMatrix<C,I,T>> matrixPtr;

				/**
				 * Shared Pointer to the Vector of RHS (b) Vector Values.
				 * Each entry corresponds to a row in the SparseMatrix in order
				 * (E.g. if matrixPtr contains NNZ values for rows 3, 5 and 8, then rhsVectorPtr[0] = values for row 3 in the vector,
				 * [1] = row 5 etc).
				 **/
				std::shared_ptr<std::vector<T>> rhsVectorPtr;

				/**
				 * Shared Pointer to the Vector of Solution (x) Vector Values.
				 * Each entry corresponds to a row in the SparseMatrix in order
				 * (E.g. if matrixPtr contains NNZ values for rows 3, 5 and 8, then rhsVectorPtr[0] = values for row 3 in the vector,
				 * [1] = row 5 etc).
				 **/
				std::shared_ptr<std::vector<T>> solVectorPtr;

				/** Shared Pointer to the Linear Solver System to use for solving **/
				std::shared_ptr<cupcfd::linearsolvers::LinearSolverInterface<C,I,T>> solverSystemPtr;

				// === Constructors/Deconstructors ===

				/**
				 * Constructor:
				 *
				 */
				BenchmarkLinearSolver(std::string benchmarkName, I repetitions,
									  std::shared_ptr<cupcfd::data_structures::SparseMatrix<C,I,T>> matrixPtr,
									  std::shared_ptr<std::vector<T>> rhsVectorPtr,
									  std::shared_ptr<std::vector<T>> solVectorPtr,
									  std::shared_ptr<cupcfd::linearsolvers::LinearSolverInterface<C,I,T>> solverSystemPtr);

				/**
				 *
				 */
				~BenchmarkLinearSolver();

				// === Concrete Methods ===

				// === Inherited Overridden Methods ===
				void setupBenchmark();
				void recordParameters();
				__attribute__((warn_unused_result))
				cupcfd::error::eCodes runBenchmark(MPI_Fint custom, int instance_number, struct unit units[], struct locators relative_positions[]);
		};
	}
}

// Include Header Level Definitions
#include "BenchmarkLinearSolver.ipp"

#endif
