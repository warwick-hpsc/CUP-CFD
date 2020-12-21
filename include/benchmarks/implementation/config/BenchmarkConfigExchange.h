/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Contains declarations for the BenchmarkConfigExchange class
 */

#ifndef CUPCFD_BENCHMARK_BENCHMARK_CONFIG_EXCHANGE_INCLUDE_H
#define CUPCFD_BENCHMARK_BENCHMARK_CONFIG_EXCHANGE_INCLUDE_H

#include "Benchmark.h"
#include "BenchmarkConfig.h"
#include "ExchangePatternConfig.h"
#include "BenchmarkExchange.h"

#include "CupCfdAoSMesh.h"
#include <memory>

namespace cupcfd
{
	namespace benchmark
	{
		/**
		 *
		 */
		template <class I, class T>
		class BenchmarkConfigExchange
		{
			public:
				// === Members ===

				/** Benchmark Name **/
				std::string benchmarkName;

				/** Number of repetitions per benchmark time/run **/
				I repetitions;

				/** Exchange Pattern Configuration for the Pattern to build **/
				cupcfd::comm::ExchangePatternConfig patternConfig;

				// === Constructors/Deconstructors ===

				/**
				 *
				 */
				BenchmarkConfigExchange(std::string benchmarkName, I repetitions, cupcfd::comm::ExchangePatternConfig& patternConfig);

				/**
				 *
				 */
				BenchmarkConfigExchange(const BenchmarkConfigExchange<I,T>& source);

				/**
				 *
				 */
				virtual ~BenchmarkConfigExchange();

				// === Concrete Methods ===

				// === Overloaded Inherited Methods ===

				virtual void operator=(const BenchmarkConfigExchange<I,T>& source);
				BenchmarkConfigExchange<I,T> * clone();

				template <class N>
				__attribute__((warn_unused_result))
				cupcfd::error::eCodes buildBenchmark(BenchmarkExchange<I,T> ** bench,
													cupcfd::data_structures::DistributedAdjacencyList<I, N>& graph);
		};
	}
}

// Include Header Level Definitions
#include "BenchmarkConfigExchange.ipp"

#endif
