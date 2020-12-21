/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Contains declarations for the BenchmarkConfig class
 */

#ifndef CUPCFD_BENCHMARK_BENCHMARK_CONFIG_INCLUDE_H
#define CUPCFD_BENCHMARK_BENCHMARK_CONFIG_INCLUDE_H

#include "Benchmark.h"
#include "Error.h"
#include <string>

namespace cupcfd
{
	namespace benchmark
	{
		/**
		 * This class defines the top-level interface/components
		 * for configuration of a benchmark.
		 */
		template <class I, class T>
		class BenchmarkConfig
		{
			public:
				// === Members ===

				/** Benchmark Name **/
				std::string benchmarkName;

				/** Number of repetitions per benchmark time/run **/
				I repetitions;

				// === Constructors/Deconstructors ===

				/**
				 *
				 */
				BenchmarkConfig(const std::string benchmarkName, const I repetitions);

				/**
				 *
				 */
				BenchmarkConfig(const BenchmarkConfig<I,T>& source);

				/**
				 *
				 */
				virtual ~BenchmarkConfig();

				// === Concrete Methods ===

				// === Pure Virtual Methods ===

				/**
				 * Deep copy from source to this config
				 *
				 * @param source The source configuration to copy from.
				 *
				 * @return Nothing.
				 */
				virtual void operator=(const BenchmarkConfig<I,T>& source);

				/**
				 * Return a pointer to a clone of this object.
				 * This allows for making copies while also
				 * preserving the polymorphic type.
				 *
				 * @return A pointer to a cloned copy of this object.
				 */
				virtual BenchmarkConfig<I,T> * clone() = 0;

				/**
				 *
				 */
				__attribute__((warn_unused_result))
				cupcfd::error::eCodes buildBenchmark(Benchmark<I,T> ** bench);
		};
	}
}

// Include Header Level Definitions
#include "BenchmarkConfig.ipp"

#endif
