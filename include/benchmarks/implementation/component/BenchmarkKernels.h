/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Contains the declarations for the BenchmarkKernels class.
 */

#ifndef CUPCFD_BENCHMARK_BENCHMARK_KERNELS_INCLUDE_H
#define CUPCFD_BENCHMARK_BENCHMARK_KERNELS_INCLUDE_H

#include "Benchmark.h"
#include <memory>
#include "UnstructuredMeshInterface.h"
#include "EuclideanVector.h"

namespace cupcfd
{
	namespace benchmark
	{
		/**
		 * Benchmark Kernels.
		 *
		 * This benchmark will benchmark the duration of running various
		 * different kernels for a given mesh.
		 *
		 * Any required stateful data is generated.
		 */
		template <class M, class I, class T, class L>
		class BenchmarkKernels : public Benchmark<I,T>
		{
			public:
				// === Members ===

				std::shared_ptr<cupcfd::geometry::mesh::UnstructuredMeshInterface<M,I,T,L>> meshPtr;

				// === Constructors/Deconstructors ===

				/**
				 *
				 */
				BenchmarkKernels(std::string benchmarkName,
											 std::shared_ptr<cupcfd::geometry::mesh::UnstructuredMeshInterface<M,I,T,L>> meshPtr,
											 I repetitions);

				/**
				 *
				 */
				~BenchmarkKernels();

				// === Concrete Methods ===

				/**
				 *
				 */
				__attribute__((warn_unused_result))
				cupcfd::error::eCodes GradientPhiGaussDolfynBenchmark();

				__attribute__((warn_unused_result))
				cupcfd::error::eCodes FluxMassDolfynFaceLoopBenchmark();

				__attribute__((warn_unused_result))
				cupcfd::error::eCodes FluxMassDolfynBoundaryLoop1Benchmark();

				__attribute__((warn_unused_result))
				cupcfd::error::eCodes FluxMassDolfynBoundaryLoop2Benchmark();

				__attribute__((warn_unused_result))
				cupcfd::error::eCodes FluxMassDolfynBoundaryLoop3Benchmark();

				__attribute__((warn_unused_result))
				cupcfd::error::eCodes FluxMassDolfynBoundaryLoop4Benchmark();

				__attribute__((warn_unused_result))
				cupcfd::error::eCodes FluxMassDolfynBoundaryLoop5Benchmark();

				__attribute__((warn_unused_result))
				cupcfd::error::eCodes FluxMassDolfynRegionLoopBenchmark();

				__attribute__((warn_unused_result))
				cupcfd::error::eCodes FluxScalarDolfynFaceLoopBenchmark();

				__attribute__((warn_unused_result))
				cupcfd::error::eCodes FluxUVWDolfynFaceLoop1Benchmark();

				void FluxUVWDolfynRegionLoop1Benchmark();

				void FluxUVWDolfynBndsLoop1Benchmark();

				__attribute__((warn_unused_result))
				cupcfd::error::eCodes calculateViscosityDolfynCellLoop1Benchmark();

				void calculateViscosityDolfynRegionLoopBenchmark();

				__attribute__((warn_unused_result))
				cupcfd::error::eCodes calculateViscosityDolfynBoundaryLoopBenchmark();

				__attribute__((warn_unused_result))
				cupcfd::error::eCodes calculateViscosityDolfynCellLoop2Benchmark();

				// === Overridden Inherited Methods ===

				void setupBenchmark();

				void recordParameters();

				__attribute__((warn_unused_result))
				cupcfd::error::eCodes runBenchmark(int testvar);
		};
	}
}

// Include Header Level Definitions
#include "BenchmarkKernels.ipp"

#endif
