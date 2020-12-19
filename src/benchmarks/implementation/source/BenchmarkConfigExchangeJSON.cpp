/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Class Definition for the BenchmarkConfigExchangeJSON class.
 */

// Header for this class
#include "BenchmarkConfigExchangeJSON.h"

#include "ExchangePatternConfigSourceJSON.h"

// File access for reading into JSON structures
#include <fstream>

namespace cupcfd
{
	namespace benchmark
	{
		// === Constructors/Deconstructors ===

		template <class I, class T>
		BenchmarkConfigExchangeJSON<I,T>::BenchmarkConfigExchangeJSON(Json::Value& parseJSON)
		{
			this->configData = parseJSON;
		}

		template <class I, class T>
		BenchmarkConfigExchangeJSON<I,T>::~BenchmarkConfigExchangeJSON()
		{
			// Nothing to do currently
		}

		template <class I, class T>
		cupcfd::error::eCodes BenchmarkConfigExchangeJSON<I,T>::getBenchmarkName(std::string& benchmarkName) {
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
		cupcfd::error::eCodes BenchmarkConfigExchangeJSON<I,T>::getBenchmarkRepetitions(I * repetitions) {
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

			return cupcfd::error::E_CONFIG_OPT_NOT_FOUND;
		}

		template <class I, class T>
		cupcfd::error::eCodes BenchmarkConfigExchangeJSON<I,T>::getExchangePatternConfig(cupcfd::comm::ExchangePatternConfig ** patternConfig) {
			cupcfd::error::eCodes status;

			if(this->configData.isMember("ExchangePattern")) {
				cupcfd::comm::ExchangePatternConfigSourceJSON patterConfigSource(this->configData["ExchangePattern"]);
				status = patterConfigSource.buildExchangePatternConfig(patternConfig);
				return status;
			}

			return cupcfd::error::E_CONFIG_INVALID_VALUE;
		}

		template <class I, class T>
		cupcfd::error::eCodes BenchmarkConfigExchangeJSON<I,T>::buildBenchmarkConfig(BenchmarkConfigExchange<I,T> ** config) {

			cupcfd::error::eCodes status;
			std::string benchmarkName;
			I repetitions;
			cupcfd::comm::ExchangePatternConfig * patternConfig;

			status = this->getBenchmarkName(benchmarkName);
			CHECK_ERROR_CODE(status)
			if(status != cupcfd::error::E_SUCCESS) {
				return status;
			}

			status = this->getBenchmarkRepetitions(&repetitions);
			CHECK_ERROR_CODE(status)
			if(status != cupcfd::error::E_SUCCESS) {
				return status;
			}

			status = this->getExchangePatternConfig(&patternConfig);
			CHECK_ERROR_CODE(status)
			if(status != cupcfd::error::E_SUCCESS) {
				return status;
			}

			*config = new BenchmarkConfigExchange<I,T>(benchmarkName, repetitions, *patternConfig);

			delete patternConfig;

			return cupcfd::error::E_SUCCESS;

		}
	}
}

// Explicit Instantiation
template class cupcfd::benchmark::BenchmarkConfigExchangeJSON<int, float>;
template class cupcfd::benchmark::BenchmarkConfigExchangeJSON<int, double>;
