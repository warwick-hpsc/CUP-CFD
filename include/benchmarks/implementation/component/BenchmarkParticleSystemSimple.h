/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Contains the declarations for the BenchmarkParticleSystemSimple class.
 */

#ifndef CUPCFD_BENCHMARK_BENCHMARK_PARTICLESYSTEM_SIMPLE_INCLUDE_H
#define CUPCFD_BENCHMARK_BENCHMARK_PARTICLESYSTEM_SIMPLE_INCLUDE_H

#include "Benchmark.h"
#include <memory>
#include "ParticleSystemSimple.h"
#include <string>
#include "Distribution.h"
#include "src/structures.h"
#include "mpi.h"

namespace cupcfd
{
	namespace benchmark
	{
		/**
		 *
		 */
		template <class M, class I, class T, class L>
		class BenchmarkParticleSystemSimple : public Benchmark<I,T>
		{
			public:
				// === Members ===

				/** Shared Pointer to the Particle System to Benchmark **/
				std::shared_ptr<cupcfd::particles::ParticleSystemSimple<M,I,T,L>> particleSystemPtr;

				/** Number of timesteps **/
				I nTimesteps;

				/** Distribution of dt for each timestep **/
				cupcfd::distributions::Distribution<I,T> * dtDist;

				// === Constructors/Deconstructors ===

				/**
				 * Constructor:
				 *
				 */
				BenchmarkParticleSystemSimple(std::string benchmarkName, I repetitions,
											I nTimesteps,
											cupcfd::distributions::Distribution<I,T>& dtDist,
											std::shared_ptr<cupcfd::particles::ParticleSystemSimple<M,I,T,L>> particleSystemPtr);

				/**
				 *
				 */
				~BenchmarkParticleSystemSimple();

				// === Concrete Methods ===

				// === Inherited Overridden Methods ===
				void setupBenchmark();
				void recordParameters();
				__attribute__((warn_unused_result))
				cupcfd::error::eCodes runBenchmark(MPI_Fint custom, int instance_number, struct unit units[], struct locators relative_positions[]);
		};
	}
}

// Include Header Level Definitions
#include "BenchmarkParticleSystemSimple.ipp"

#endif
