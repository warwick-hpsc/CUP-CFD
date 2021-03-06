/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Contains declarations for the ParticleSystemConfig class.
 */

#ifndef CUPCFD_PARTICLES_CONFIG_PARTICLE_SYSTEM_CONFIG_INCLUDE_H
#define CUPCFD_PARTICLES_CONFIG_PARTICLE_SYSTEM_CONFIG_INCLUDE_H

#include "ParticleSystem.h"
#include <memory>

namespace cupcfd
{
	namespace particles
	{
		/**
		 * Store the common elements required for setting up any generic partitioner approach.
		 *
		 * Currently this acts as a virtual base class for polymorphism purposes.
		 *
		 * @tparam S Specialisation type of the Particle System
		 * @tparam E Specialisation type of the Particle Emitters
		 * @tparam P Specialisation type of the Particles
		 * @tparam M Specialisation Type of the Mesh
		 * @tparam I Type of the indexing scheme
		 * @tparam T Type of the mesh spatial volume
		 * @tparam L Label type of the mesh
		 */
		template <class S, class E, class P, class M, class I, class T, class L>
		class ParticleSystemConfig
		{
			public:
				// === Members ===

				// === Constructors/Deconstructors ===

				/**
				 * Constructor.
				 * Currently does nothing.
				 */
				ParticleSystemConfig();

				/**
				 * Deconstructor.
				 * Currently does nothing.
				 */
				virtual ~ParticleSystemConfig();

				// === Virtual Methods ===

				/**
				 * Return a pointer to a clone of this object.
				 * This allows for making copies while also
				 * preserving the polymorphic type.
				 *
				 * @tparam I The indexing scheme of the partitioner.
				 * @tparam T The datatype of the partitioner nodes.
				 *
				 * @return A pointer to a cloned copy of this object.
				 */
				__attribute__((warn_unused_result))
				virtual ParticleSystemConfig<S,E,P,M,I,T,L> * clone() = 0;

				/**
				 *
				 */
				__attribute__((warn_unused_result))
				virtual cupcfd::error::eCodes buildParticleSystem(ParticleSystem<S,E,P,M,I,T,L> ** system,
																	std::shared_ptr<M> meshPtr) = 0;
		};
	}
}

// Some definitions for the declarations at a header level
#include "ParticleSystemConfig.ipp"

#endif
