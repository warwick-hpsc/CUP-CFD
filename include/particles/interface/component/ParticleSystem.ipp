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
 * Contains header level definitions for the ParticleSystem class
 */

#ifndef CUPCFD_PARTICLES_PARTICLE_SYSTEM_IPP_H
#define CUPCFD_PARTICLES_PARTICLE_SYSTEM_IPP_H

namespace cupcfd
{
	namespace particles
	{
		template <class S, class E, class P, class M, class I, class T, class L>
		ParticleSystem<S,E,P,M,I,T,L>::ParticleSystem(std::shared_ptr<cupcfd::geometry::mesh::UnstructuredMeshInterface<M,I,T,L>> mesh)
		: mesh(mesh)
		{

		}

		template <class S, class E, class P, class M, class I, class T, class L>
		ParticleSystem<S,E,P,M,I,T,L>::~ParticleSystem()
		{

		}

		template <class S, class E, class P, class M, class I, class T, class L>
		inline cupcfd::error::eCodes ParticleSystem<S,E,P,M,I,T,L>::addParticleEmitter(const E& emitter)
		{
			return static_cast<S*>(this)->addParticleEmitter(emitter);
		}

		template <class S, class E, class P, class M, class I, class T, class L>
		inline cupcfd::error::eCodes ParticleSystem<S,E,P,M,I,T,L>::addParticle(const P& particle)
		{
			return static_cast<S*>(this)->addParticle(particle);
		}

		template <class S, class E, class P, class M, class I, class T, class L>
		inline cupcfd::error::eCodes ParticleSystem<S,E,P,M,I,T,L>::setParticleInactive(I particleID)
		{
			return static_cast<S*>(this)->setParticleInactive(particleID);
		}

		template <class S, class E, class P, class M, class I, class T, class L>
		inline cupcfd::error::eCodes ParticleSystem<S,E,P,M,I,T,L>::removeInactiveParticles()
		{
			return static_cast<S*>(this)->removeInactiveParticles();
		}

		template <class S, class E, class P, class M, class I, class T, class L>
		cupcfd::error::eCodes ParticleSystem<S,E,P,M,I,T,L>::exchangeParticles()
		{
			return static_cast<S*>(this)->exchangeParticles();
		}

		template <class S, class E, class P, class M, class I, class T, class L>
		cupcfd::error::eCodes ParticleSystem<S,E,P,M,I,T,L>::updateSystem(T dt)
		{
			return static_cast<S*>(this)->updateSystem(dt);
		}
		
		template <class S, class E, class P, class M, class I, class T, class L>
		cupcfd::error::eCodes ParticleSystem<S,E,P,M,I,T,L>::updateSystemAtomic()
		{
			return static_cast<S*>(this)->updateSystemAtomic();
		}
		
		template <class S, class E, class P, class M, class I, class T, class L>
		I ParticleSystem<S,E,P,M,I,T,L>::getNEmitters()
		{
			return static_cast<S*>(this)->getNEmitters();
		}
		
		template <class S, class E, class P, class M, class I, class T, class L>
		I ParticleSystem<S,E,P,M,I,T,L>::getNActiveParticles()
		{
			return static_cast<S*>(this)->getNActiveParticles();
		}
		
		template <class S, class E, class P, class M, class I, class T, class L>
		I ParticleSystem<S,E,P,M,I,T,L>::getNTravelParticles()
		{
			return static_cast<S*>(this)->getNTravelParticles();
		}
		
		template <class S, class E, class P, class M, class I, class T, class L>
		I ParticleSystem<S,E,P,M,I,T,L>::getNParticles()
		{
			return static_cast<S*>(this)->getNParticles();
		}
		
		template <class S, class E, class P, class M, class I, class T, class L>
		cupcfd::error::eCodes ParticleSystem<S,E,P,M,I,T,L>::setActiveParticlesTravelTime(T travelTime)
		{
			return static_cast<S*>(this)->setActiveParticlesTravelTime(travelTime);
		}
		
		template <class S, class E, class P, class M, class I, class T, class L>
		cupcfd::error::eCodes ParticleSystem<S,E,P,M,I,T,L>::generateEmitterParticles(T dt)
		{
			return static_cast<S*>(this)->generateEmitterParticles(dt);
		}
	}	
}

#endif
