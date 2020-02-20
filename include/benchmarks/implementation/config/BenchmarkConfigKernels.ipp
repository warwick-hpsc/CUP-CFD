/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Contains header level definitions for the BenchmarkConfigKernels class
 */

#ifndef CUPCFD_BENCHMARK_BENCHMARK_CONFIG_KERNELS_IPP_H
#define CUPCFD_BENCHMARK_BENCHMARK_CONFIG_KERNELS_IPP_H

namespace cupcfd
{
	namespace benchmark
	{
		template <class I, class T>
		template <class M, class L>
		cupcfd::error::eCodes BenchmarkConfigKernels<I,T>::buildBenchmark(BenchmarkKernels<M,I,T,L> ** bench,
												  std::shared_ptr<M> meshPtr)
		{
			*bench = new BenchmarkKernels<M,I,T,L>(this->benchmarkName, meshPtr, this->repetitions);

			return cupcfd::error::E_SUCCESS;
		}
	}
}

#endif
