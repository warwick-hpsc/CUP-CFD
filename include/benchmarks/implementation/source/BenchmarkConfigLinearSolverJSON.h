/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Class Declaration for the BenchmarkConfigLinearSolverJSON class.
 */

#ifndef CUPCFD_BENCHMARK_BENCHMARK_CONFIG_LINEAR_SOLVER_JSON_INCLUDE_H
#define CUPCFD_BENCHMARK_BENCHMARK_CONFIG_LINEAR_SOLVER_JSON_INCLUDE_H

// C++ Std Lib
#include <string>
#include <vector>

// Error Codes
#include "Error.h"

#include "BenchmarkConfigLinearSolver.h"

#include "LinearSolverConfig.h"
#include "SparseMatrixSourceConfig.h"
#include "VectorSourceConfig.h"

// JsonCPP - Supplied as standalone in include/io/jsoncpp
#include "json.h"
#include "json-forwards.h"

namespace cupcfd
{
	namespace benchmark
	{
		/**
		 * Defines a interface for accessing Linear Solver Benchmark configuration options from a JSON data source.
		 *
		 * === Fields ===
		 *
		 * Required:
		 * BenchmarkName: String. Defines the benchmark name - also used for the overall benchmark timer identifier so
		 * should be unique amongst benchmarks
		 *
		 * Repetitions: Integer. Defines the number of times to run the benchmark
		 *
		 * LinearSolver: Contains a JSON record for a linear solver to use for benchmarking.
		 * Accepted record field names:
		 * "LinearSolverPETSc" - Field name for a record that contains all fields needed to define a
		 * LinearSolverConfigPETScJSON record. (See LinearSolverConfigPETScJSON.h")
		 *
		 * SparseMatrix: Contains a JSON record for a SparseMatrix source to use with the linear solver for benchmarking.
		 * Accepted record field names:
		 * "SparseMatrixFile" - Field name for a record that contains all fields needed to defines a
		 * SparseMatrixSourceFileConfigJSON record (See SparseMatrixSourceFileConfigJSON.h)
		 *
		 * RHSVector: Contains a JSON record for a Vector source to use as the RHS Vector (b) with the linear solver for benchmarking.
		 * ToDo: Would like to make this optional, such that if this field is not provided, the RHS vector is treated as if all zeroes.
		 *
		 * SolVector: Contains a JSON record for a Vector source to use as the initial Solution vector (x) with the linear solver
		 * for benchmarking.
		 * ToDo: Would like to make this optional, such that if this field is not provided, the RHS vector is treated as if all zeroes.
		 *
		 * DataDistribution: String. Defines how the matrix should be distributed across participating processes.
		 * Accepted Values:
		 * "Concurrent" : The full matrix is loaded and solved independently but concurrently on every rank - This is intended
		 * for benchmarking multiple serial solves occurring simultaneously.
		 * If a single serial solve is desired for a benchmark, it is recommended to run the benchmark with this option and only
		 * one MPI process in MPI_COMM_WORLD (i.e. a serial run).
		 *
		 * "Distributed": The matrix is divided and distributed across every participating rank for a parallel linear solve.
		 *
		 * Optional Fields:
		 * None
		 *
		 * Example JSON entry:
		 *
		 * {
		 * "BenchmarkName" : "LinearSolverTest",
		 * "Repetitions"	: 10,
		 * "LinearSolver"  : {
		 * 		"LinearSolverPETSc" : {
		 *			"Algorithm" : "CGAMG",
		 *			"eTol"	: 1e-6,
		 *			"rTol"	: 1e-6
		 *		}
		 *	},
		 * "SparseMatrix"  : {
		 *	 	"SparseMatrixFile" : {
		 *			"FilePath" : "../Path/To/File/file.h5",
		 *			"FileFormat" : "HDF5"
		 *	 	}
		 *	},
		 * 	"DataDistribution"  : "Concurrent"
		 * 	}
		 *
		 */
		template <class C, class I, class T>
		class BenchmarkConfigLinearSolverJSON
		{
			public:
				// === Members ===

				/** Json Data Store containing fields for this JSON source **/
				Json::Value configData;

				// === Constructors/Deconstructors ===

				/**
				 * Parse the JSON record provided for values belonging to a FixedDistribution entry
				 *
				 * @param parseJSON The contents of a JSON record with the appropriate fields
				 */
				BenchmarkConfigLinearSolverJSON(Json::Value& parseJSON);

				/**
				 * Deconstructor
				 */
				~BenchmarkConfigLinearSolverJSON();

				// === Concrete Methods ===

				/**
				 *
				 */
				__attribute__((warn_unused_result))
				cupcfd::error::eCodes getBenchmarkName(std::string& benchmarkName);

				/**
				 *
				 */
				__attribute__((warn_unused_result))
				cupcfd::error::eCodes getBenchmarkRepetitions(I * repetitions);

				/**
				 *
				 */
				__attribute__((warn_unused_result))
				cupcfd::error::eCodes getLinearSolverConfig(cupcfd::linearsolvers::LinearSolverConfig<C,I,T> ** solverSystemConfig);

				/**
				 *
				 */
				__attribute__((warn_unused_result))
				cupcfd::error::eCodes getSparseMatrixSourceConfig(cupcfd::data_structures::SparseMatrixSourceConfig<I,T> ** matrixSourceConfig);

				/**
				 *
				 */
				__attribute__((warn_unused_result))
				cupcfd::error::eCodes getRHSVectorSourceConfig(cupcfd::data_structures::VectorSourceConfig<I,T> ** rhsSourceConfig);

				/**
				 *
				 */
				__attribute__((warn_unused_result))
				cupcfd::error::eCodes getSolutionVectorSourceConfig(cupcfd::data_structures::VectorSourceConfig<I,T> ** solSourceConfig);

				/**
				 *
				 */
				__attribute__((warn_unused_result))
				cupcfd::error::eCodes getBenchSolverDistribution(BenchSolverDistribution * solverDist);

				/**
				 *
				 */
				__attribute__((warn_unused_result))
				cupcfd::error::eCodes buildBenchmarkConfig(BenchmarkConfigLinearSolver<C,I,T> ** config);
		};
	}
}

// Include Header Level Definitions
#include "BenchmarkConfigLinearSolverJSON.ipp"

#endif
