/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Contains header level definitions for the ParticleSystemConfig class.
 */

#ifndef CUPCFD_PARTICLES_CONFIG_PARTICLE_SYSTEM_CONFIG_IPP_H
#define CUPCFD_PARTICLES_CONFIG_PARTICLE_SYSTEM_CONFIG_IPP_H

namespace cupcfd
{
	namespace particles
	{
		template <class S, class E, class P, class M, class I, class T, class L>
		ParticleSystemConfig<S,E,P,M,I,T,L>::ParticleSystemConfig()
		{

		}

		template <class S, class E, class P, class M, class I, class T, class L>
		ParticleSystemConfig<S,E,P,M,I,T,L>::ParticleSystemConfig(ParticleSystemConfig<S,E,P,M,I,T,L>& source)
		{

		}

		template <class S, class E, class P, class M, class I, class T, class L>
		ParticleSystemConfig<S,E,P,M,I,T,L>::~ParticleSystemConfig()
		{

		}

		template <class S, class E, class P, class M, class I, class T, class L>
		void ParticleSystemConfig<S,E,P,M,I,T,L>::operator=(ParticleSystemConfig<S,E,P,M,I,T,L>& source)
		{

		}

		// template <class S, class E, class P, class M, class I, class T, class L>
		// ParticleSystemConfig<S,E,P,M,I,T,L> * ParticleSystemConfig<S,E,P,M,I,T,L>::clone()
		// {

		// }

		// template <class S, class E, class P, class M, class I, class T, class L>
		// cupcfd::error::eCodes ParticleSystemConfig<S,E,P,M,I,T,L>::buildParticleSystem(ParticleSystem<S,E,P,M,I,T,L> ** system,
		// 																					std::shared_ptr<M> meshPtr)
		// {

		// }
	}
}

#endif
