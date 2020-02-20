/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Contains header level definitions for the BenchmarkConfigExchange class
 */

#ifndef CUPCFD_BENCHMARK_BENCHMARK_CONFIG_EXCHANGE_IPP_H
#define CUPCFD_BENCHMARK_BENCHMARK_CONFIG_EXCHANGE_IPP_H

#include "ExchangePattern.h"
#include <memory>

namespace cupcfd
{
	namespace benchmark
	{
		template <class I, class T>
		template <class N>
		cupcfd::error::eCodes BenchmarkConfigExchange<I,T>::buildBenchmark(BenchmarkExchange<I,T> ** bench,
																				cupcfd::data_structures::DistributedAdjacencyList<I, N>& graph)
		{
			cupcfd::error::eCodes status;
			
			// Build an Exchange Pattern from the Exchange Pattern Config + the Mesh Graph
			// This will give a Exchange Pattern for the Mesh Halo Exchange, using whichever Exchange variant (e.g. one or two sided) defined in the
			// ExchangeConfig
			cupcfd::comm::ExchangePattern<T> * pattern;
			
			status = this->patternConfig.buildExchangePattern(&pattern, graph);

			if(status != cupcfd::error::E_SUCCESS)
			{
				return status;
			}
			
			// Shared pointer will take responsibility for cleaning up the pattern pointer.
			std::shared_ptr<cupcfd::comm::ExchangePattern<T>> patternPtr(pattern);

			// Build the Exchange Benchmark
			*bench = new BenchmarkExchange<I,T>(this->benchmarkName, this->repetitions, patternPtr);

			return cupcfd::error::E_SUCCESS;
		}
	}
}

#endif
