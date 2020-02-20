/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Header level definitions for the BenchmarkParticleSystem class.
 */

#ifndef CUPCFD_BENCHMARK_BENCHMARK_PARTICLESYSTEM_SIMPLE_IPP_H
#define CUPCFD_BENCHMARK_BENCHMARK_PARTICLESYSTEM_SIMPLE_IPP_H

#include "tt_interface_c.h"

namespace cupcfd
{
	namespace benchmark
	{
		template <class M, class I, class T, class L>
		BenchmarkParticleSystemSimple<M,I,T,L>::BenchmarkParticleSystemSimple(std::string benchmarkName, I repetitions,
																		I nTimesteps,
																		cupcfd::distributions::Distribution<I,T>& dtDist,
																		std::shared_ptr<cupcfd::particles::ParticleSystemSimple<M,I,T,L>> particleSystemPtr)
		: Benchmark<I,T>(benchmarkName, repetitions),
		  particleSystemPtr(particleSystemPtr),
		  nTimesteps(nTimesteps)
		{
			this->dtDist = dtDist.clone();
		}

		template <class M, class I, class T, class L>
		BenchmarkParticleSystemSimple<M,I,T,L>::~BenchmarkParticleSystemSimple()
		{
			delete this->dtDist;
		}

		template <class M, class I, class T, class L>
		void BenchmarkParticleSystemSimple<M,I,T,L>::setupBenchmark()
		{
			// Nothing to do currently
		}

		template <class M, class I, class T, class L>
		void BenchmarkParticleSystemSimple<M,I,T,L>::recordParameters()
		{
			// Nothing to do currently
		}

		template <class M, class I, class T, class L>
		void BenchmarkParticleSystemSimple<M,I,T,L>::runBenchmark()
		{
			// ToDo: Increasing number of repetitions is currently just
			// a multiplier for the number of timesteps.
			// Need to add a means to reset the Particle System!

			this->startBenchmarkBlock(this->benchmarkName);
			TreeTimerLogParameterInt("Repetitions", this->repetitions);
			this->recordParameters();

			for(I i = 0; i < this->repetitions; i++)
			{
				for(I j = 0; j < this->nTimesteps; j++)
				{
					// Generate time for next timestep
					T timestep;
					this->dtDist->getValues(&timestep, 1);

					// Advance Particle System by one timestep
					this->startBenchmarkBlock("UpdateParticleTimestep");
					particleSystemPtr->updateSystem(timestep);
					this->stopBenchmarkBlock("UpdateParticleTimestep");
				}
			}

			this->stopBenchmarkBlock(this->benchmarkName);
		}
	}
}

#endif
