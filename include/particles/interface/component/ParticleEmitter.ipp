/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Description
 *
 * Contains header level definitions for the ParticleEmitter class
 */

#ifndef CUPCFD_PARTICLES_PARTICLE_EMITTER_IPP_H
#define CUPCFD_PARTICLES_PARTICLE_EMITTER_IPP_H

namespace cupcfd
{
	namespace particles
	{
		static int numEmitters = 0;

		template <class E, class P, class I, class T>
		ParticleEmitter<E,P,I,T>::ParticleEmitter(const I localCellID, const I globalCellID, 
												  const I rank, const cupcfd::geometry::euclidean::EuclideanPoint<T,3>& position)
		: position(position),
		  globalCellID(globalCellID),
		  localCellID(localCellID),
		  emitterID(numEmitters),
		  rank(rank),
		  nextParticleID(0)
		{
			numEmitters++;
		}

		template <class E, class P, class I, class T>
		ParticleEmitter<E,P,I,T>::~ParticleEmitter()
		{

		}
		
		template <class E, class P, class I, class T>
		inline void ParticleEmitter<E,P,I,T>::operator=(const ParticleEmitter<E,P,I,T>& source)
		{
			static_cast<P*>(this)->operator=(source);
		}
		
		template <class E, class P, class I, class T>
		cupcfd::error::eCodes ParticleEmitter<E,P,I,T>::generateParticles(Particle<P,I,T> ** particles, I * nParticles, T dt)
		{
			return static_cast<P*>(this)->generateParticles(particles, nParticles, dt);
		}
	}
}

#endif
