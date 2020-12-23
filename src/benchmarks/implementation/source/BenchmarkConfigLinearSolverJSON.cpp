/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Class Definition for the BenchmarkConfigLinearSolverJSON class.
 */

// Header for this class
#include "BenchmarkConfigLinearSolverJSON.h"

// File access for reading into JSON structures
#include <fstream>

#include "SparseMatrixCOO.h"
#include "SparseMatrixCSR.h"

#include "LinearSolverConfigPETScJSON.h"

#include "SparseMatrixSourceFileConfigJSON.h"
#include "VectorSourceFileConfigJSON.h"

namespace cupcfd
{
	namespace benchmark
	{
		// === Constructors/Deconstructors ===

		template <class C, class I, class T>
		BenchmarkConfigLinearSolverJSON<C,I,T>::BenchmarkConfigLinearSolverJSON(Json::Value& parseJSON)
		{
			this->configData = parseJSON;
		}

		template <class C, class I, class T>
		BenchmarkConfigLinearSolverJSON<C,I,T>::~BenchmarkConfigLinearSolverJSON()
		{
			// Nothing to do currently
		}

		template <class C, class I, class T>
		cupcfd::error::eCodes BenchmarkConfigLinearSolverJSON<C,I,T>::getBenchmarkName(std::string& benchmarkName) {
			if(this->configData.isMember("BenchmarkName")) {
				const Json::Value dataSourceType = this->configData["BenchmarkName"];

				if(dataSourceType == Json::Value::null) {
					return cupcfd::error::E_CONFIG_OPT_NOT_FOUND;
				}
				else {
					benchmarkName = dataSourceType.asString();
					return cupcfd::error::E_SUCCESS;
				}

				// Found, but not a matching value
				return cupcfd::error::E_CONFIG_INVALID_VALUE;
			}

			// Field not found
			return cupcfd::error::E_CONFIG_OPT_NOT_FOUND;
		}

		template <class C, class I, class T>
		cupcfd::error::eCodes BenchmarkConfigLinearSolverJSON<C,I,T>::getBenchmarkRepetitions(I * repetitions) {
			if(this->configData.isMember("Repetitions")) {
				const Json::Value dataSourceType = this->configData["Repetitions"];

				if(dataSourceType == Json::Value::null) {
					return cupcfd::error::E_CONFIG_OPT_NOT_FOUND;
				}
				else {
					*repetitions = dataSourceType.asLargestInt();
					return cupcfd::error::E_SUCCESS;
				}

				// Found, but not a matching value
				return cupcfd::error::E_CONFIG_INVALID_VALUE;
			}

			// Field not found
			return cupcfd::error::E_CONFIG_OPT_NOT_FOUND;
		}

		template <class C, class I, class T>
		cupcfd::error::eCodes BenchmarkConfigLinearSolverJSON<C,I,T>::getLinearSolverConfig(cupcfd::linearsolvers::LinearSolverConfig<C,I,T> ** solverSystemConfig) {
			cupcfd::error::eCodes status;

			if(this->configData.isMember("LinearSolver")) {
				// Try each of the potential Linear Solver Configuration Sources in Turn till a valid one is found

				// Option 1 - PETSc Linear Solver
				if(this->configData["LinearSolver"].isMember("LinearSolverPETSc")) {
					cupcfd::linearsolvers::LinearSolverConfigPETScJSON<C,I,T> solverConfig(this->configData["LinearSolver"]["LinearSolverPETSc"]);
					status = solverConfig.buildLinearSolverConfig(solverSystemConfig);
					return status;
				}

				// Field not found
				return cupcfd::error::E_CONFIG_OPT_NOT_FOUND;
			}

			// Field not found
			return cupcfd::error::E_CONFIG_OPT_NOT_FOUND;
		}

		template <class C, class I, class T>
		cupcfd::error::eCodes BenchmarkConfigLinearSolverJSON<C,I,T>::getSparseMatrixSourceConfig(cupcfd::data_structures::SparseMatrixSourceConfig<I,T> ** matrixSourceConfig) {
			cupcfd::error::eCodes status;

			if(this->configData.isMember("SparseMatrix")) {
				// Option 1 - Sparse Matrix from a File
				// ToDo: Option 2: Add Matrix Gen From a MeshGenConfig
				if(this->configData["SparseMatrix"].isMember("SparseMatrixFile")) {
					cupcfd::data_structures::SparseMatrixSourceFileConfigJSON<I,T> configSource1(this->configData["SparseMatrix"]["SparseMatrixFile"]);
					status = configSource1.buildSparseMatrixSourceConfig(matrixSourceConfig);
					return status;
				}
				else {
					return cupcfd::error::E_CONFIG_OPT_NOT_FOUND;
				}
			}

			// Field not found
			return cupcfd::error::E_CONFIG_OPT_NOT_FOUND;
		}

		template <class C, class I, class T>
		cupcfd::error::eCodes BenchmarkConfigLinearSolverJSON<C,I,T>::getRHSVectorSourceConfig(cupcfd::data_structures::VectorSourceConfig<I,T> ** rhsSourceConfig) {
			cupcfd::error::eCodes status;

			if(this->configData.isMember("RHSVector")) {
				// Option 1 - Sparse Matrix from a File
				// ToDo: Option 2: Add Matrix Gen From a MeshGenConfig
				if(this->configData["RHSVector"].isMember("VectorFile")) {
					cupcfd::data_structures::VectorSourceFileConfigJSON<I,T> configSource1(this->configData["RHSVector"]["VectorFile"]);
					status = configSource1.buildVectorSourceConfig(rhsSourceConfig);
					return status;
				}
				else {
					return cupcfd::error::E_CONFIG_OPT_NOT_FOUND;
				}
			}

			// Field not found
			return cupcfd::error::E_CONFIG_OPT_NOT_FOUND;
		}

		template <class C, class I, class T>
		cupcfd::error::eCodes BenchmarkConfigLinearSolverJSON<C,I,T>::getSolutionVectorSourceConfig(cupcfd::data_structures::VectorSourceConfig<I,T> ** solSourceConfig) {
			cupcfd::error::eCodes status;

			if(this->configData.isMember("SolVector")) {
				// Option 1 - Sparse Matrix from a File
				// ToDo: Option 2: Add Matrix Gen From a MeshGenConfig
				if(this->configData["SolVector"].isMember("VectorFile")) {
					cupcfd::data_structures::VectorSourceFileConfigJSON<I,T> configSource1(this->configData["SolVector"]["VectorFile"]);
					status = configSource1.buildVectorSourceConfig(solSourceConfig);
					return status;
				}
				else {
					return cupcfd::error::E_CONFIG_OPT_NOT_FOUND;
				}
			}

			// Field not found
			return cupcfd::error::E_CONFIG_OPT_NOT_FOUND;
		}

		template <class C, class I, class T>
		cupcfd::error::eCodes BenchmarkConfigLinearSolverJSON<C,I,T>::getBenchSolverDistribution(BenchSolverDistribution * solverDist) {
			Json::Value dataSourceType;

			if(this->configData.isMember("DataDistribution")) {
				// Access the correct field
				dataSourceType = this->configData["DataDistribution"];

				// Check the value and return the appropriate ID
				if(dataSourceType == Json::Value::null) {
					return cupcfd::error::E_CONFIG_OPT_NOT_FOUND;
				}
				else if(dataSourceType == "Concurrent") {
					*solverDist = BENCH_SOLVER_DIST_CONCURRENT;
					return cupcfd::error::E_SUCCESS;
				}
				else if(dataSourceType == "Distributed") {
					*solverDist = BENCH_SOLVER_DIST_DISTRIBUTED;
					return cupcfd::error::E_SUCCESS;
				}

				// Found, but not a matching value
				return cupcfd::error::E_CONFIG_INVALID_VALUE;
			}

			// No option found
			return cupcfd::error::E_CONFIG_INVALID_VALUE;
		}

		template <class C, class I, class T>
		cupcfd::error::eCodes BenchmarkConfigLinearSolverJSON<C,I,T>::buildBenchmarkConfig(BenchmarkConfigLinearSolver<C,I,T> ** config) {
			cupcfd::error::eCodes status;

			// Get Benchmark Name
			std::string benchmarkName;
			status = this->getBenchmarkName(benchmarkName);
			CHECK_ECODE(status)

			// Get Repetitions
			I repetitions;
			status = this->getBenchmarkRepetitions(&repetitions);
			CHECK_ECODE(status)

			// Get Distribution Type
			BenchSolverDistribution distType;
			status = this->getBenchSolverDistribution(&distType);
			CHECK_ECODE(status)

			// Matrix Source Config
			cupcfd::data_structures::SparseMatrixSourceConfig<I,T> * matrixSourceConfig;
			status = this->getSparseMatrixSourceConfig(&matrixSourceConfig);
			CHECK_ECODE(status)

			// RHS Vector Source Config
			cupcfd::data_structures::VectorSourceConfig<I,T> * rhsSourceConfig;
			status = this->getRHSVectorSourceConfig(&rhsSourceConfig);
			CHECK_ECODE(status)

			// Sol Vector Source Config
			cupcfd::data_structures::VectorSourceConfig<I,T> * solSourceConfig;
			status = this->getSolutionVectorSourceConfig(&solSourceConfig);
			CHECK_ECODE(status)

			// Linear Solver Config
			cupcfd::linearsolvers::LinearSolverConfig<C,I,T> * linearSolverConfig;
			status = this->getLinearSolverConfig(&linearSolverConfig);
			CHECK_ECODE(status)

			*config = new BenchmarkConfigLinearSolver<C,I,T>(benchmarkName, repetitions, distType, *linearSolverConfig,
															 *matrixSourceConfig, *rhsSourceConfig, *solSourceConfig);

			delete(matrixSourceConfig);

			return cupcfd::error::E_SUCCESS;

		}
	}
}

// Explicit Instantiation
template class cupcfd::benchmark::BenchmarkConfigLinearSolverJSON<cupcfd::data_structures::SparseMatrixCOO<int,float>,int, float>;
template class cupcfd::benchmark::BenchmarkConfigLinearSolverJSON<cupcfd::data_structures::SparseMatrixCOO<int,double>,int, double>;
template class cupcfd::benchmark::BenchmarkConfigLinearSolverJSON<cupcfd::data_structures::SparseMatrixCSR<int,float>,int, float>;
template class cupcfd::benchmark::BenchmarkConfigLinearSolverJSON<cupcfd::data_structures::SparseMatrixCSR<int,double>,int, double>;
