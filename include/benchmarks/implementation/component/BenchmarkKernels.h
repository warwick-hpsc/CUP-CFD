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
				cupcfd::error::eCodes GradientPhiGaussDolfynBenchmark();

				cupcfd::error::eCodes FluxMassDolfynFaceLoopBenchmark();

				cupcfd::error::eCodes FluxMassDolfynBoundaryLoop1Benchmark();

				cupcfd::error::eCodes FluxMassDolfynBoundaryLoop2Benchmark();

				cupcfd::error::eCodes FluxMassDolfynBoundaryLoop3Benchmark();

				cupcfd::error::eCodes FluxMassDolfynBoundaryLoop4Benchmark();

				cupcfd::error::eCodes FluxMassDolfynBoundaryLoop5Benchmark();

				cupcfd::error::eCodes FluxMassDolfynRegionLoopBenchmark();

				cupcfd::error::eCodes FluxScalarDolfynFaceLoopBenchmark();

				cupcfd::error::eCodes FluxUVWDolfynFaceLoop1Benchmark();

				cupcfd::error::eCodes FluxUVWDolfynRegionLoop1Benchmark();

				cupcfd::error::eCodes FluxUVWDolfynBndsLoop1Benchmark();

				cupcfd::error::eCodes calculateViscosityDolfynCellLoop1Benchmark();

				cupcfd::error::eCodes calculateViscosityDolfynRegionLoopBenchmark();

				cupcfd::error::eCodes calculateViscosityDolfynBoundaryLoopBenchmark();

				cupcfd::error::eCodes calculateViscosityDolfynCellLoop2Benchmark();

				// === Overridden Inherited Methods ===

				void setupBenchmark();

				void recordParameters();

				void runBenchmark();
		};
	}
}

// Include Header Level Definitions
#include "BenchmarkKernels.ipp"

#endif
