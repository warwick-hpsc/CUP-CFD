/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Contains declarations for the ParticleSourceConfig class
 */

#ifndef CUPCFD_PARTICLES_INTERFACE_CONFIG_PARTICLE_SOURCE_CONFIG_INCLUDE_H
#define CUPCFD_PARTICLES_INTERFACE_CONFIG_PARTICLE_SOURCE_CONFIG_INCLUDE_H

// Error Codes
#include "Error.h"

#include "ParticleSource.h"

namespace cupcfd
{
	namespace particles
	{
		/**
		 * Specify configuration options required for building mesh data sources.
		 * Interface Abstract Class.
		 */
		template <class P, class I, class T>
		class ParticleSourceConfig
		{
			public:
				// === Members ===

				// === Constructor ===

				/**
				 * Constructor:
				 */
				ParticleSourceConfig();

				/**
				 * Deconstructor.
				 */
				virtual ~ParticleSourceConfig();

				// === Pure Virtual Methods ===

				/**
				 * Return a pointer to a clone of this object.
				 * This allows for making copies while also
				 * preserving the polymorphic type.
				 *
				 * @return A pointer to a cloned copy of this object.
				 */
				__attribute__((warn_unused_result))
				virtual ParticleSourceConfig<P,I,T> * clone() = 0;

				/**
				 * Build a Particle Source based on the settings specified in this configuration type.
				 *
				 * @param particleSource A pointer to the location where the pointer of the newly created object will be stored.
				 * @param sourceId ID for this source, unique across all emitters and sources in system; used to create unique particle IDs
				 */
				__attribute__((warn_unused_result))
				virtual cupcfd::error::eCodes buildParticleSource(ParticleSource<P,I,T> ** particleSource, int sourceId) = 0;
		};
	}
}

// Include Header Level Definitions
#include "ParticleSourceConfig.ipp"

#endif
