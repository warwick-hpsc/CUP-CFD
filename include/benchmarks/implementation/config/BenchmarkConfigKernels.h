/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Contains declarations for the BenchmarkConfigKernels class
 */

#ifndef CUPCFD_BENCHMARK_BENCHMARK_CONFIG_KERNELS_INCLUDE_H
#define CUPCFD_BENCHMARK_BENCHMARK_CONFIG_KERNELS_INCLUDE_H

#include "Benchmark.h"
#include "BenchmarkConfig.h"

#include <memory>
#include "UnstructuredMeshInterface.h"
#include "BenchmarkKernels.h"
#include "CupCfdAoSMesh.h"

#include "ExchangePatternConfig.h"

namespace cupcfd
{
	namespace benchmark
	{
		/**
		 *
		 */
		template <class I, class T>
		class BenchmarkConfigKernels
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
				BenchmarkConfigKernels(const std::string benchmarkName, const I repetitions);

				/**
				 *
				 */
				BenchmarkConfigKernels(const BenchmarkConfigKernels<I,T>& source);

				/**
				 *
				 */
				~BenchmarkConfigKernels();

				// === Concrete Methods ===

				// === Overloaded Inherited Methods ===

				void operator=(const BenchmarkConfigKernels<I,T>& source);
				
				__attribute__((warn_unused_result))
				BenchmarkConfigKernels<I,T> * clone();

				template <class M, class L>
				__attribute__((warn_unused_result))
				cupcfd::error::eCodes buildBenchmark(BenchmarkKernels<M,I,T,L> ** bench,
													std::shared_ptr<M> meshPtr);
		};
	}
}

// Include Header Level Definitions
#include "BenchmarkConfigKernels.ipp"

#endif
