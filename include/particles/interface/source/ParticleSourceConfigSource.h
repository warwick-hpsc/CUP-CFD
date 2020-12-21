/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Contains declarations for the ParticleSourceConfigSource class.
 */

#ifndef CUPCFD_PARTICLES_CONFIG_PARTICLE_SOURCE_CONFIG_SOURCE_INCLUDE_H
#define CUPCFD_PARTICLES_CONFIG_PARTICLE_SOURCE_CONFIG_SOURCE_INCLUDE_H

#include "ParticleSourceConfig.h"

namespace cupcfd
{
	namespace particles
	{
		/**
		 * Store the common elements required for setting up any generic partitioner approach.
		 *
		 * Currently this acts as a virtual base class for polymorphism purposes.
		 *
		 * @tparam P The type of the particles
		 * @tparam I The indexing scheme of the partitioner.
		 * @tparam T The datatype of the partitioner nodes/graph to be partitioned.
		 */
		template <class P, class I, class T>
		class ParticleSourceConfigSource
		{
			public:
				// === Members ===

				// === Constructors/Deconstructors ===

				/**
				 * Constructor.
				 * Currently does nothing.
				 */
				ParticleSourceConfigSource();

				/**
				 * Constructor.
				 * Copies data from source to this object.
				 */
				ParticleSourceConfigSource(ParticleSourceConfigSource<P,I,T>& source);

				/**
				 * Deconstructor.
				 * Currently does nothing.
				 */
				virtual ~ParticleSourceConfigSource();

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
				virtual ParticleSourceConfigSource<P,I,T> * clone() = 0;

				/**
				 *
				 */
				__attribute__((warn_unused_result))
				virtual cupcfd::error::eCodes buildParticleSourceConfig(ParticleSourceConfig<P,I,T> ** config) = 0;
		};
	}
}

// Some definitions for the declarations at a header level
#include "ParticleSourceConfigSource.ipp"

#endif
