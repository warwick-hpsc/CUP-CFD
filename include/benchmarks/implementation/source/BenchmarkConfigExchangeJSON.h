/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Class Declaration for the BenchmarkConfigExchangeJSON class.
 */

#ifndef CUPCFD_BENCHMARK_BENCHMARK_CONFIG_EXCHANGE_JSON_INCLUDE_H
#define CUPCFD_BENCHMARK_BENCHMARK_CONFIG_EXCHANGE_JSON_INCLUDE_H

// C++ Std Lib
#include <string>
#include <vector>

// Error Codes
#include "Error.h"

#include "BenchmarkConfigExchange.h"

#include "ExchangePatternConfig.h"

// JsonCPP - Supplied as standalone in include/io/jsoncpp
#include "json.h"
#include "json-forwards.h"

namespace cupcfd
{
	namespace benchmark
	{
		/**
		 * Defines a interface for accessing FVM Kernel Benchmark configuration
		 * options from a JSON data source.
		 *
		 * === Fields ===
		 *
		 * Required:
		 * BenchmarkName: String. Defines the benchmark name - also used for the overall benchmark timer identifier so
		 * should be unique amongst benchmarks
		 *
		 * Repetitions: Integer. Defines the number of times to run the benchmark
		 *
		 * ExchangePattern: A nested JSON record that matches the pattern defined in ExchangePatternConfigSourceJSON
		 * This selects which exchange method to use for the benchmark.
		 *
		 *
		 * Optional:
		 * None
		 *
		 * No configuration is provided for the exchange pattern (i.e. sizes, processes) since it is currently defined by
		 * the Mesh being used for the benchmark run. Rather than define a nested mesh JSON (and thus have to rebuild the mesh)
		 * it is assumed that the same top-level mesh configuration is used for all benchmarks.
		 *
		 */
		template <class I, class T>
		class BenchmarkConfigExchangeJSON
		{
			public:
				// === Members ===

				/** Json Data Store containing fields for this JSON source **/
				Json::Value configData;

				// === Constructors/Deconstructors ===

				/**
				 * Parse the JSON record provided for fields belonging to Configuration for BenchmarkExchange JSON.
				 * The fields are presumed to start from level 0 (i.e. they are not nested inside other objects)
				 *
				 * @param parseJSON The contents of a JSON record with the appropriate fields.
				 */
				BenchmarkConfigExchangeJSON(Json::Value& parseJSON);

				/**
				 * Deconstructor
				 */
				~BenchmarkConfigExchangeJSON();

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
				cupcfd::error::eCodes getExchangePatternConfig(cupcfd::comm::ExchangePatternConfig ** patternConfig);


				// === Overloaded Methods ===
				__attribute__((warn_unused_result))
				cupcfd::error::eCodes buildBenchmarkConfig(BenchmarkConfigExchange<I,T> ** config);

				// === Pure Virtual Methods ===
		};
	}
}

// Include Header Level Definitions
#include "BenchmarkConfigExchangeJSON.ipp"

#endif
