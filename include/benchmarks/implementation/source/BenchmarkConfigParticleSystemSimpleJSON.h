/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Class Declaration for the BenchmarkConfigParticleSystemJSON class.
 */

#ifndef CUPCFD_BENCHMARK_BENCHMARK_CONFIG_PARTICLE_SYSTEM_SIMPLE_JSON_INCLUDE_H
#define CUPCFD_BENCHMARK_BENCHMARK_CONFIG_PARTICLE_SYSTEM_SIMPLE_JSON_INCLUDE_H

// C++ Std Lib
#include <string>
#include <vector>

// Error Codes
#include "Error.h"

#include "BenchmarkConfigParticleSystemSimple.h"

#include "DistributionConfig.h"

// JsonCPP - Supplied as standalone in include/io/jsoncpp
#include "json.h"
#include "json-forwards.h"

namespace cupcfd
{
	namespace benchmark
	{
		/**
		 *
		 */
		template <class M, class I, class T, class L>
		class BenchmarkConfigParticleSystemSimpleJSON
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
				BenchmarkConfigParticleSystemSimpleJSON(Json::Value& parseJSON);

				/**
				 * Deconstructor
				 */
				~BenchmarkConfigParticleSystemSimpleJSON();

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
				cupcfd::error::eCodes getNTimesteps(I * nTimesteps);

				/**
				 *
				 */
				__attribute__((warn_unused_result))
				cupcfd::error::eCodes getDtDistributionConfig(cupcfd::distributions::DistributionConfig<I,T> ** dtDistConfig);

				/**
				 *
				 */
				__attribute__((warn_unused_result))
				cupcfd::error::eCodes getParticleSystemConfig(cupcfd::particles::ParticleSystemSimpleConfig<M,I,T,L> ** particleSystemConfig);

				/**
				 *
				 */
				__attribute__((warn_unused_result))
				cupcfd::error::eCodes buildBenchmarkConfig(BenchmarkConfigParticleSystemSimple<M,I,T,L> ** config);
		};
	}
}

// Include Header Level Definitions
#include "BenchmarkConfigParticleSystemSimpleJSON.ipp"

#endif
