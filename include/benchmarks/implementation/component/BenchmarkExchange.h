/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Contains the declarations for the BenchmarkExchange class.
 */

#ifndef CUPCFD_BENCHMARK_BENCHMARK_EXCHANGE_INCLUDE_H
#define CUPCFD_BENCHMARK_BENCHMARK_EXCHANGE_INCLUDE_H

#include "ExchangePattern.h"
#include "Benchmark.h"
#include <memory>

namespace cupcfd
{
	namespace benchmark
	{
		/**
		 * Sets up and runs the benchmark for ExchangePatterns
		 */
		template <class I, class T>
		class BenchmarkExchange : public Benchmark<I,T>
		{
			public:
				// === Members ===

				/**
				 * A shared pointer to the pattern to be used as the basis for the exchange benchmark.
				 * This will define which processes communicate with one another, and how much data
				 * is communicated.
				 *
				 * A share pointer is used to avoid copy overhead, since an exchange pattern can
				 * store indexes and buffer spaces.
				 **/
				std::shared_ptr<cupcfd::comm::ExchangePattern<T>> patternPtr;

				// === Constructors/Deconstructors ===

				/**
				 * Constructor:
				 *
				 * Sets up the benchmark for the provided exchange pattern
				 */
				BenchmarkExchange(std::string benchmarkName, I repetitions, std::shared_ptr<cupcfd::comm::ExchangePattern<T>> patternPtr);

				/**
				 *
				 */
				~BenchmarkExchange();

				// === Concrete Methods ===

				/**
				 * Benchmark the time to run the data exchange for the pattern
				 * associated with this benchmark.
				 */
				__attribute__((warn_unused_result))
				cupcfd::error::eCodes ExchangeBenchmark();

				// === Inherited Overridden Methods ===
				void setupBenchmark();
				void recordParameters();
				__attribute__((warn_unused_result))
				cupcfd::error::eCodes runBenchmark();
		};
	}
}

// Include Header Level Definitions
#include "BenchmarkExchange.ipp"

#endif
