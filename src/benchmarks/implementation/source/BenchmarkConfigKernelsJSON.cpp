/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Class Definition for the BenchmarkConfigKernelsJSON class.
 */

// Header for this class
#include "BenchmarkConfigKernelsJSON.h"
#include "CupCfdAoSMesh.h"

// File access for reading into JSON structures
#include <fstream>

namespace cupcfd
{
	namespace benchmark
	{
		// === Constructors/Deconstructors ===

		template <class I, class T>
		BenchmarkConfigKernelsJSON<I,T>::BenchmarkConfigKernelsJSON(Json::Value& parseJSON)
		{
			this->configData = parseJSON;
		}

		template <class I, class T>
		BenchmarkConfigKernelsJSON<I,T>::~BenchmarkConfigKernelsJSON()
		{
			// Nothing to do currently
		}

		template <class I, class T>
		cupcfd::error::eCodes BenchmarkConfigKernelsJSON<I,T>::getBenchmarkName(std::string& benchmarkName) {
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

		template <class I, class T>
		cupcfd::error::eCodes BenchmarkConfigKernelsJSON<I,T>::getBenchmarkRepetitions(I * repetitions) {
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

		template <class I, class T>
		cupcfd::error::eCodes BenchmarkConfigKernelsJSON<I,T>::buildBenchmarkConfig(BenchmarkConfigKernels<I,T> ** config) {
			cupcfd::error::eCodes status;
			std::string benchmarkName;
			I repetitions;

			status = this->getBenchmarkName(benchmarkName);
			CHECK_ECODE(status)

			status = this->getBenchmarkRepetitions(&repetitions);
			CHECK_ECODE(status)

			*config = new BenchmarkConfigKernels<I,T>(benchmarkName, repetitions);
			return cupcfd::error::E_SUCCESS;
		}
	}
}

// Explicit Instantiation
template class cupcfd::benchmark::BenchmarkConfigKernelsJSON<int, float>;
template class cupcfd::benchmark::BenchmarkConfigKernelsJSON<int, double>;
