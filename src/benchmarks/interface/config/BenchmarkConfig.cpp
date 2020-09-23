/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Contains definitions for the BenchmarkConfig class
 */

#include "BenchmarkConfig.h"

namespace cupcfd
{
	namespace benchmark
	{
		// === Constructors/Deconstructors ===
		template <class I, class T>
		BenchmarkConfig<I,T>::BenchmarkConfig(const std::string benchmarkName, const I repetitions)
		: benchmarkName(benchmarkName),
		  repetitions(repetitions)
		{

		}

		template <class I, class T>
		BenchmarkConfig<I,T>::BenchmarkConfig(const BenchmarkConfig<I,T>& source)
		: benchmarkName(source.benchmarkName),
		  repetitions(source.repetitions)
		{

		}

		template <class I, class T>
		BenchmarkConfig<I,T>::~BenchmarkConfig()
		{

		}

		// === Concrete Methods ===

		// === Pure Virtual Methods ===

		template <class I, class T>
		void BenchmarkConfig<I,T>::operator=(const BenchmarkConfig& source)
		{
			this->benchmarkName = source.benchmarkName;
			this->repetitions = source.repetitions;
		}

		// template <class I, class T>
		// BenchmarkConfig<I,T> * BenchmarkConfig<I,T>::clone()
		// {
		// 	// Nothing to do here
		// 	return nullptr;
		// }

		// template <class I, class T>
		// cupcfd::error::eCodes BenchmarkConfig<I,T>::buildBenchmark(Benchmark<I,T> ** bench)
		// {
		// 	// Nothing to do here
		// 	return cupcfd::error::E_SUCCESS;
		// }
	}
}
