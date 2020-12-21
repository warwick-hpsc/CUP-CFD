/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Contains declarations for the ParticleEmitterConfig class.
 */

#ifndef CUPCFD_PARTICLES_CONFIG_PARTICLE_EMITTER_CONFIG_INCLUDE_H
#define CUPCFD_PARTICLES_CONFIG_PARTICLE_EMITTER_CONFIG_INCLUDE_H

#include "ParticleEmitter.h"
#include "UnstructuredMeshInterface.h"

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
		class ParticleEmitterConfig
		{
			public:
				// === Members ===

				/** Euclidean Position of the emitter **/
				cupcfd::geometry::euclidean::EuclideanPoint<T,3> position;

				// === Constructors/Deconstructors ===

				/**
				 * Constructor.
				 * Currently does nothing.
				 */
				ParticleEmitterConfig(const cupcfd::geometry::euclidean::EuclideanPoint<T,3>& position);

				/**
				 * Constructor.
				 * Copies data from source to this object.
				 */
				ParticleEmitterConfig(ParticleEmitterConfig<E,P,I,T>& source);

				/**
				 * Deconstructor.
				 * Currently does nothing.
				 */
				virtual ~ParticleEmitterConfig();

				// === Concrete Methods ===

				// === Virtual Methods ===

				/**
				 * Deep copy from source to this config
				 *
				 * @param source The source configuration to copy from.
				 *
				 * @tparam I The indexing scheme of the partitioner.
				 * @tparam T The datatype of the partitioner nodes.
				 *
				 * @return Nothing.
				 */
				virtual void operator=(const ParticleEmitterConfig<E,P,I,T>& source);

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
				virtual ParticleEmitterConfig<E,P,I,T> * clone() = 0;

				/**
				 * Build a Particle Emitter Object.
				 * Note: Since there is no reference to a mesh here, the localCellID, globalCellID and rank
				 * are set to -1, since their correct values cannot be determined.
				 *
				 * These should be set post operation for correct functionality when working with a mesh.
				 */
				__attribute__((warn_unused_result))
				virtual cupcfd::error::eCodes buildParticleEmitter(ParticleEmitter<E,P,I,T> ** emitter, int emitterId) = 0;
		};
	}
}

// Some definitions for the declarations at a header level
#include "ParticleEmitterConfig.ipp"

#endif
