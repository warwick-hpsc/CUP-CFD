/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Contains declarations for the BenchmarkConfigParticleSystem class
 */

#ifndef CUPCFD_BENCHMARK_BENCHMARK_CONFIG_PARTICLE_SYSTEM_SIMPLE_INCLUDE_H
#define CUPCFD_BENCHMARK_BENCHMARK_CONFIG_PARTICLE_SYSTEM_SIMPLE_INCLUDE_H

#include "Benchmark.h"
#include "BenchmarkConfig.h"
#include "DistributionConfig.h"
#include "ParticleSystemSimpleConfig.h"
#include "BenchmarkParticleSystemSimple.h"

#include <string>
#include <memory>

namespace cupcfd
{
	namespace benchmark
	{
		// ToDo: This would ideally be made into a generic templated version for different Particle System types (since
		// not much changes between them other than the particle system config. However, CRTP requires we know the type
		// at compile type so we have to specify it up front before we look up JSOn or other config sources.
		// As such, we would need a Benchmark class for each particle system and then iterate through valid ones in the JSON....

		// I think a better way to do this can be found (or at the very least the templates tidied up somewhat), but this
		// will suffice for now, even if it means a copy-paste for new types.

		/**
		 *
		 */
		template <class M, class I, class T, class L>
		class BenchmarkConfigParticleSystemSimple
		{
			public:
				// === Members ===

				/** **/
				std::string benchmarkName;

				/** **/
				I repetitions;

				/** **/
				I nTimesteps;

				/** **/
				cupcfd::distributions::DistributionConfig<I,T> * dtDistConfig;

				/** **/
				cupcfd::particles::ParticleSystemSimpleConfig<M,I,T,L> * particleSystemConfig;

				// === Constructors/Deconstructors ===

				/**
				 *
				 */
				BenchmarkConfigParticleSystemSimple(std::string benchmarkName, I repetitions,
											  I nTimesteps, cupcfd::distributions::DistributionConfig<I,T>& dtDistConfig,
											  cupcfd::particles::ParticleSystemSimpleConfig<M,I,T,L>& particleSystemConfig);

				/**
				 *
				 */
				BenchmarkConfigParticleSystemSimple(const BenchmarkConfigParticleSystemSimple<M,I,T,L>& source);

				/**
				 *
				 */
				virtual ~BenchmarkConfigParticleSystemSimple();

				// === Concrete Methods ===

				// === Overloaded Inherited Methods ===

				virtual void operator=(const BenchmarkConfigParticleSystemSimple<M,I,T,L>& source);
				BenchmarkConfigParticleSystemSimple<M,I,T,L> * clone();
				cupcfd::error::eCodes buildBenchmark(BenchmarkParticleSystemSimple<M,I,T,L> ** bench,
														  std::shared_ptr<M> meshPtr);
		};
	}
}

// Include Header Level Definitions
#include "BenchmarkConfigParticleSystemSimple.ipp"

#endif
