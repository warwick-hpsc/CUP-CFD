/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Contains the declarations for the Benchmark class.
 */

#ifndef CUPCFD_BENCHMARK_BENCHMARK_INCLUDE_H
#define CUPCFD_BENCHMARK_BENCHMARK_INCLUDE_H

#include <string>

#include "Error.h"

namespace cupcfd
{
	namespace benchmark
	{
		/**
		 * Top-level benchmark abstraction.
		 *
		 * Contains the generic benchmark actions, such as starting
		 * timers, stopping timers and virtual interfaces for expected
		 * functions that polymorphic types should implement, such
		 * as for registering/tracking parameters etc.
		 *
		 * @tparam I The type of the indexing scheme
		 * @tparam T The type of the computation data
		 */
		template <class I, class T>
		class Benchmark
		{
			public:
				// === Members ===

				/** A name/ID to attribute to the benchmark **/
				std::string benchmarkName;

				/** Number of repetitions per timer block **/
				I repetitions;

				// === Constructors/Deconstructors ===

				/**
				 *
				 */
				Benchmark(std::string benchmarkName, I repetitions);

				/**
				 *
				 */
				virtual ~Benchmark();

				// === Concrete Methods ===

				/**
				 * Start the benchmark block, including any timing
				 * etc.
				 *
				 * @tparam I The type of the indexing scheme
				 * @tparam T The type of the computation data
				 *
				 * @return Nothing
				 */
				void startBenchmarkBlock(std::string blockName);

				/**
				 * Stop the benchmark block, including any timing etc.
				 *
				 * @tparam I The type of the indexing scheme
				 * @tparam T The type of the computation data
				 */
				void stopBenchmarkBlock(std::string blockName);

				// === Pure Virtual Methods ===

				/**
				 * Setup any benchmark generated data that isn't provided from outside
				 * sources via a constructed (e.g. dummy data)
				 *
				 * @tparam I The type of the indexing scheme
				 * @tparam T The type of the computation data
				 *
				 * @return Nothing
				 */
				virtual void setupBenchmark() = 0;

				/**
				 * Run the benchmark operations specific to a benchmark
				 * implementation (e.g. kernels)
				 *
				 * @tparam I The type of the indexing scheme
				 * @tparam T The type of the computation data
				 *
				 * @return error code
				 */
				__attribute__((warn_unused_result))
				virtual cupcfd::error::eCodes runBenchmark(int testvar) = 0;
		};
	}
}

// Include Header Level Definitions
#include "Benchmark.ipp"

#endif
