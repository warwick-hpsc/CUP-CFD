/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Contains declarations for the BenchmarkConfigLinearSolver class
 */

#ifndef CUPCFD_BENCHMARK_BENCHMARK_CONFIG_LINEAR_Solver_INCLUDE_H
#define CUPCFD_BENCHMARK_BENCHMARK_CONFIG_LINEAR_Solver_INCLUDE_H

#include "Benchmark.h"
#include "BenchmarkConfig.h"
#include "SparseMatrixSourceConfig.h"
#include "LinearSolverConfig.h"
#include "BenchmarkLinearSolver.h"
#include "VectorSourceConfig.h"

namespace cupcfd
{
	namespace benchmark
	{
		enum BenchSolverDistribution {
			BENCH_SOLVER_DIST_CONCURRENT,		// Each rank Loads a Full Copy of the matrix and solves it concurrently (testing contention)
			BENCH_SOLVER_DIST_DISTRIBUTED		// Each rank receives part of the matrix and the system is solved in parallel
		};

		/**
		 *
		 */
		template <class C, class I, class T>
		class BenchmarkConfigLinearSolver
		{
			public:
				// === Members ===

				/** Benchmark Name **/
				std::string benchmarkName;

				/** Number of repetitions per benchmark time/run **/
				I repetitions;

				/** Indicator of how to distribute the Matrix **/
				BenchSolverDistribution distType;

				/** Configuration detailing where to find a data source for a Matrix **/
				cupcfd::data_structures::SparseMatrixSourceConfig<I,T> * matrixSourceConfig;

				/** Configuration detailing where to find a data source for the RHS Vector **/
				cupcfd::data_structures::VectorSourceConfig<I,T> * rhsSourceConfig;

				/** Configuration detailing where to find a data source for the Solution Vector **/
				cupcfd::data_structures::VectorSourceConfig<I,T> * solSourceConfig;

				/** Configuration detailing which Linear Solver System to use **/
				cupcfd::linearsolvers::LinearSolverConfig<C,I,T> * linearSolverConfig;

				// === Constructors/Deconstructors ===

				/**
				 *
				 */
				BenchmarkConfigLinearSolver(std::string benchmarkName, I repetitions,
											 BenchSolverDistribution distType,
											 cupcfd::linearsolvers::LinearSolverConfig<C,I,T>& linearSolverConfig,
											 cupcfd::data_structures::SparseMatrixSourceConfig<I,T>& matrixSourceConfig,
											 cupcfd::data_structures::VectorSourceConfig<I,T>& rhsSourceConfig,
											 cupcfd::data_structures::VectorSourceConfig<I,T>& solSourceConfig);

				/**
				 *
				 */
				BenchmarkConfigLinearSolver(const BenchmarkConfigLinearSolver<C,I,T>& source);

				/**
				 *
				 */
				virtual ~BenchmarkConfigLinearSolver();

				// === Concrete Methods ===

				// === Overloaded Inherited Methods ===

				virtual void operator=(const BenchmarkConfigLinearSolver<C,I,T>& source);
				BenchmarkConfigLinearSolver<C,I,T> * clone();
				__attribute__((warn_unused_result))
				cupcfd::error::eCodes buildBenchmark(BenchmarkLinearSolver<C,I,T> ** bench);
		};
	}
}

// Include Header Level Definitions
#include "BenchmarkConfigLinearSolver.ipp"

#endif
