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
 * Contains declarations for the ParticleSystemSimple class
 */

#ifndef CUPCFD_PARTICLES_PARTICLE_SYSTEM_SIMPLE_INCLUDE_H
#define CUPCFD_PARTICLES_PARTICLE_SYSTEM_SIMPLE_INCLUDE_H

#include "ParticleSimple.h"
#include "ParticleEmitterSimple.h"
#include "UnstructuredMeshInterface.h"

#include "ParticleSystem.h"

#include <memory>
#include <vector>

namespace cupcfd
{
	namespace particles
	{
		/**
		 * Management class for Particle and Emitters of the Simple type
		 */
		template <class M, class I, class T, class L>
		class ParticleSystemSimple : public ParticleSystem<ParticleSystemSimple<M, I, T, L>, ParticleEmitterSimple<I,T>, ParticleSimple<I,T>, M, I, T, L>
		{
			public:
				// === Members ===

				/** Particles **/
				std::vector<ParticleSimple<I,T>> particles;

				/** Particle Emitters **/
				std::vector<ParticleEmitterSimple<I,T>> emitters;

				// Cheaper to maintain a tracker than count through the list of particles every time we need this value
				/** Number of active particles in the system **/
				I nActiveParticles;


				// Cheaper to maintain a tracker than count through the list of particles every time we need this value
				/** Number of active particles with travel time remaining **/
				I nTravelParticles;

				// === Constructors/Deconstructors ===

				/**
				 * Empty Constructor
				 */
				ParticleSystemSimple(std::shared_ptr<cupcfd::geometry::mesh::UnstructuredMeshInterface<M,I,T,L>> mesh);

				/**
				 * Deconstructor
				 */
				~ParticleSystemSimple();

				// === Interface Methods ===
				I getNParticles();
				I getNActiveParticles();
				I getNTravelParticles();

				inline cupcfd::error::eCodes addParticleEmitter(const ParticleEmitterSimple<I,T>& emitter);
				inline cupcfd::error::eCodes addParticle(const ParticleSimple<I,T>& particle);
				inline cupcfd::error::eCodes setParticleInactive(I particleID);
				cupcfd::error::eCodes removeInactiveParticles();

				cupcfd::error::eCodes exchangeParticles();
				cupcfd::error::eCodes updateSystem(T dt);
				cupcfd::error::eCodes updateSystemAtomic();


				cupcfd::error::eCodes setActiveParticlesTravelTime(T travelTime);
				cupcfd::error::eCodes generateEmitterParticles(T dt);
		};
	}
}

// Include Header Level Definitions
#include "ParticleSystemSimple.ipp"

#endif
