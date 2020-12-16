/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Contains declarations for the ParticleEmitterConfigSource class.
 */

#ifndef CUPCFD_PARTICLES_CONFIG_PARTICLE_EMITTER_CONFIG_SOURCE_INCLUDE_H
#define CUPCFD_PARTICLES_CONFIG_PARTICLE_EMITTER_CONFIG_SOURCE_INCLUDE_H

#include "ParticleEmitterConfig.h"

namespace cupcfd
{
	namespace particles
	{
		/**
		 * Store the common elements required for setting up any generic partitioner approach.
		 *
		 * Currently this acts as a virtual base class for polymorphism purposes.
		 *
		 * @tparam E The type of the particle emitter
		 * @tparam P The type of the particles emitted
		 * @tparam I The indexing scheme of the partitioner.
		 * @tparam T The datatype of the partitioner nodes/graph to be partitioned.
		 */
		template <class E, class P, class I, class T>
		class ParticleEmitterConfigSource
		{
			public:
				// === Members ===

				// === Constructors/Deconstructors ===

				/**
				 * Constructor.
				 * Currently does nothing.
				 */
				ParticleEmitterConfigSource();

				/**
				 * Deconstructor.
				 * Currently does nothing.
				 */
				virtual ~ParticleEmitterConfigSource();

				// === Pure Virtual Methods ===

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
				virtual ParticleEmitterConfigSource<E,P,I,T> * clone() = 0;

				/**
				 *
				 */
				virtual cupcfd::error::eCodes buildParticleEmitterConfig(ParticleEmitterConfig<E,P,I,T> ** config) = 0;
		};
	}
}

// Some definitions for the declarations at a header level
#include "ParticleEmitterConfigSource.ipp"

#endif
