/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Contains definitions for the BenchmarkConfigParticleSystemSimple class
 */

#include "BenchmarkConfigParticleSystemSimple.h"
#include "CupCfdAoSMesh.h"
#include "CupCfdSoAMesh.h"
#include <memory>

namespace cupcfd
{
	namespace benchmark
	{
		// === Constructors/Deconstructors ===
		template <class M, class I, class T, class L>
		BenchmarkConfigParticleSystemSimple<M,I,T,L>::BenchmarkConfigParticleSystemSimple(std::string benchmarkName, I repetitions,
																		  I nTimesteps, cupcfd::distributions::DistributionConfig<I,T>& dtDistConfig,
																		  cupcfd::particles::ParticleSystemSimpleConfig<M,I,T,L>& particleSystemConfig)
		: benchmarkName(benchmarkName),
		  repetitions(repetitions),
		  nTimesteps(nTimesteps)
		{
			this->dtDistConfig = dtDistConfig.clone();
			this->particleSystemConfig = particleSystemConfig.clone();
		}

		template <class M, class I, class T, class L>
		BenchmarkConfigParticleSystemSimple<M,I,T,L>::BenchmarkConfigParticleSystemSimple(const BenchmarkConfigParticleSystemSimple<M,I,T,L>& source)
		{
			*this = source;
		}

		template <class M, class I, class T, class L>
		BenchmarkConfigParticleSystemSimple<M,I,T,L>::~BenchmarkConfigParticleSystemSimple()
		{
			delete this->dtDistConfig;
			delete this->particleSystemConfig;
		}

		// === Concrete Methods ===

		// === Overloaded Inherited Methods ===

		template <class M, class I, class T, class L>
		void BenchmarkConfigParticleSystemSimple<M,I,T,L>::operator=(const BenchmarkConfigParticleSystemSimple<M,I,T,L>& source)
		{
			this->benchmarkName = source.benchmarkName;
			this->repetitions = source.repetitions;
			this->nTimesteps = source.nTimesteps;
			this->dtDistConfig = source.dtDistConfig->clone();
			this->particleSystemConfig = source.particleSystemConfig->clone();
		}

		template <class M, class I, class T, class L>
		BenchmarkConfigParticleSystemSimple<M,I,T,L> * BenchmarkConfigParticleSystemSimple<M,I,T,L>::clone()
		{
			return new BenchmarkConfigParticleSystemSimple<M,I,T,L>(*this);
		}

		template <class M, class I, class T, class L>
		cupcfd::error::eCodes BenchmarkConfigParticleSystemSimple<M,I,T,L>::buildBenchmark(BenchmarkParticleSystemSimple<M,I,T,L> ** bench,
																								std::shared_ptr<M> meshPtr)
		{
			cupcfd::error::eCodes status;
			cupcfd::distributions::Distribution<I,T> * dtDist;

			cupcfd::particles::ParticleSystem<cupcfd::particles::ParticleSystemSimple<M, I, T, L>, cupcfd::particles::ParticleEmitterSimple<I,T>, cupcfd::particles::ParticleSimple<I,T>, M, I, T, L> * tmp;

			// Build the dt Distribution
			status = this->dtDistConfig->buildDistribution(&dtDist);
			if(status != cupcfd::error::E_SUCCESS)
			{
				return status;
			}

			// Build the Particle System;
			status = this->particleSystemConfig->buildParticleSystem(&tmp, meshPtr);
			if(status != cupcfd::error::E_SUCCESS)
			{
				return status;
			}

			std::shared_ptr<cupcfd::particles::ParticleSystemSimple<M,I,T,L>> particleSystemPtr(static_cast<cupcfd::particles::ParticleSystemSimple<M,I,T,L> *>(tmp));

			*bench = new BenchmarkParticleSystemSimple<M,I,T,L>(this->benchmarkName, this->repetitions, this->nTimesteps, *dtDist, particleSystemPtr);

			// Don't free 'tmp', as the shared pointer has taken over management of it
			delete dtDist;

			return cupcfd::error::E_SUCCESS;
		}
	}
}

// Explicit Instantiation
template class cupcfd::benchmark::BenchmarkConfigParticleSystemSimple<cupcfd::geometry::mesh::CupCfdAoSMesh<int,float,int>,int,float,int>;
template class cupcfd::benchmark::BenchmarkConfigParticleSystemSimple<cupcfd::geometry::mesh::CupCfdAoSMesh<int,double,int>,int,double,int>;

template class cupcfd::benchmark::BenchmarkConfigParticleSystemSimple<cupcfd::geometry::mesh::CupCfdSoAMesh<int,float,int>,int,float,int>;
template class cupcfd::benchmark::BenchmarkConfigParticleSystemSimple<cupcfd::geometry::mesh::CupCfdSoAMesh<int,double,int>,int,double,int>;
