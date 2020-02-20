/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Contains header level definitions for the ParticleSystemConfigSource class.
 */

#ifndef CUPCFD_PARTICLES_CONFIG_PARTICLE_SYSTEM_CONFIG_SOURCE_IPP_H
#define CUPCFD_PARTICLES_CONFIG_PARTICLE_SYSTEM_CONFIG_SOURCE_IPP_H

namespace cupcfd
{
	namespace particles
	{
		template <class S, class E, class P, class M, class I, class T, class L>
		ParticleSystemConfigSource<S,E,P,M,I,T,L>::ParticleSystemConfigSource()
		{

		}

		template <class S, class E, class P, class M, class I, class T, class L>
		ParticleSystemConfigSource<S,E,P,M,I,T,L>::ParticleSystemConfigSource(ParticleSystemConfigSource<S,E,P,M,I,T,L>& source)
		{

		}

		template <class S, class E, class P, class M, class I, class T, class L>
		ParticleSystemConfigSource<S,E,P,M,I,T,L>::~ParticleSystemConfigSource()
		{

		}

		template <class S, class E, class P, class M, class I, class T, class L>
		void ParticleSystemConfigSource<S,E,P,M,I,T,L>::operator=(ParticleSystemConfigSource<S,E,P,M,I,T,L>& source)
		{

		}

		template <class S, class E, class P, class M, class I, class T, class L>
		ParticleSystemConfigSource<S,E,P,M,I,T,L> * ParticleSystemConfigSource<S,E,P,M,I,T,L>::clone()
		{

		}

		template <class S, class E, class P, class M, class I, class T, class L>
		cupcfd::error::eCodes ParticleSystemConfigSource<S,E,P,M,I,T,L>::buildParticleSystemConfig(ParticleSystemConfig<S,E,P,M,I,T,L> ** config)
		{

		}
	}
}

#endif
