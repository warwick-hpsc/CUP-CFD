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

#include "Benchmark.h"
#include "tt_interface_c.h"

namespace cupcfd
{
	namespace benchmark
	{
		template <class I, class T>
		Benchmark<I,T>::Benchmark(std::string benchmarkName, I repetitions)
		:benchmarkName(benchmarkName),
		 repetitions(repetitions)
		{

		}

		template <class I, class T>
		Benchmark<I,T>::~Benchmark()
		{

		}

		template <class I, class T>
		void Benchmark<I,T>::startBenchmarkBlock(std::string blockName)
		{
			TT_EnterMethod(blockName.c_str());
		}

		template <class I, class T>
		void Benchmark<I,T>::stopBenchmarkBlock(std::string blockName)
		{
			TT_Exit(blockName.c_str());
		}


		// === Pure Virtual Methods ===
		// Here to satisfy linker

		template <class I, class T>
		void Benchmark<I,T>::setupBenchmark()
		{

		}

		template <class I, class T>
		void Benchmark<I,T>::runBenchmark()
		{

		}
	}
}

// Explicit Instantiation
template class cupcfd::benchmark::Benchmark<int, float>;
template class cupcfd::benchmark::Benchmark<int, double>;
