/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Contains declarations for the ParticleSystemConfigSource class.
 */

#ifndef CUPCFD_PARTICLES_CONFIG_PARTICLE_SYSTEM_CONFIG_SOURCE_INCLUDE_H
#define CUPCFD_PARTICLES_CONFIG_PARTICLE_SYSTEM_CONFIG_SOURCE_INCLUDE_H

#include "ParticleSystemConfig.h"

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
		class ParticleSystemConfigSource
		{
			public:
				// === Members ===

				// === Constructors/Deconstructors ===

				/**
				 * Constructor.
				 * Currently does nothing.
				 */
				ParticleSystemConfigSource();

				/**
				 * Constructor.
				 * Copies data from source to this object.
				 */
				ParticleSystemConfigSource(ParticleSystemConfigSource<S,E,P,M,I,T,L>& source);

				/**
				 * Deconstructor.
				 * Currently does nothing.
				 */
				virtual ~ParticleSystemConfigSource();

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
				virtual ParticleSystemConfigSource<S,E,P,M,I,T,L> * clone() = 0;

				/**
				 *
				 */
				__attribute__((warn_unused_result))
				virtual cupcfd::error::eCodes buildParticleSystemConfig(ParticleSystemConfig<S,E,P,M,I,T,L> ** config) = 0;
		};
	}
}

// Some definitions for the declarations at a header level
#include "ParticleSystemConfigSource.ipp"

#endif
