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
 * Contains declarations for the ParticleEmitter class
 */

#ifndef CUPCFD_PARTICLES_PARTICLE_EMITTER_INCLUDE_H
#define CUPCFD_PARTICLES_PARTICLE_EMITTER_INCLUDE_H

#include "EuclideanPoint.h"
#include "EuclideanVector.h"
#include "EuclideanPlane3D.h"
#include "Particle.h"
#include "Distribution.h"
#include "UnstructuredMeshInterface.h"

namespace cupcfd
{
	namespace particles
	{
		/**
		 * Defines interface methods for basic particle emitter operations
		 * such as particle generation etc.
		 *
		 * Design uses CRTP to permit the addition of further ParticleEmitter types.
		 */
		template <class E, class P, class I, class T>
		class ParticleEmitter
		{
			public:
				// === Members ===

				/**
				 * Point from where the particles are emitted
				 */
				cupcfd::geometry::euclidean::EuclideanPoint<T,3> position;

				/**
				 * Associate this emitter with a global cell from a mesh
				 */
				I globalCellID;

				/**
				 * Associate this emitter with a local cell from a mesh
				 */
				I localCellID;

				/**
				 * Unique identifier for this emitter
				 */
				I emitterID;

				/**
				 * Associate this emitter with a certain rank
				 */
				I rank;

				/**
				 * ID to be given to next particle emitted
				 */
				I nextParticleID;

				// === Constructors/Deconstructors ===

				/**
				 * It is left to the callee to ensure that the localCellID, globalCellID and rank
				 * are accurate according to the mesh the emitter may be used with.
				 */
				ParticleEmitter(const I localCellID,
								const I globalCellID,
								const I rank,
								const cupcfd::geometry::euclidean::EuclideanPoint<T,3>& position);

				/**
				 *
				 */
				~ParticleEmitter();

				// === Concrete Methods ===

				/**
				 * Deep copy the values from source to this object.
				 *
				 * @param source The source to copy from.
				 *
				 * @tparam I The type of the indexing scheme
				 * @tparam T The type of the stored node data
				 *
				 * @return Nothing
				 */
				inline void operator=(const ParticleEmitter<E,P,I,T>& source);

				/**
				 * Generate particles for the given time period.
				 *
				 * The number of particles generated is based on the time change provided (dt), and the rate of
				 * particle generation based on the distribution of the emitter.
				 *
				 * If no particles have been generated prior, then tthe time of the first particle is dependent on
				 * the distribution.
				 *
				 * If particles have been generated before, then a 'pending time' to the next particle has been stored, and
				 * that much time will elapse in the new dt period before a particle is generated, then resuming
				 * the distribution generation of particles. This is so that values from the distribution are not
				 * skipped if they exceed the current dt period.
				 *
				 * Particles are assigned a remaining travel time based on the amount of time left in the dt time period
				 * from when they were generated (e.g. if generated at 1.2 out of 10 seconds, they have 8.8 seconds remaining).
				 *
				 * Particles are not advanced in time after generation, i.e. they are all fixed at the emitter position, with
				 * varying amounts of time remaining. Their movement is left to a ParticleSystem object.
				 *
				 * @param particles A pointer to the location where the particles array will be created
				 * @param nParticles A pointer to where the number of particles in the created array will be stored
				 * @param dt The delta time change
				 *
				 * @tparam I The type of the indexing scheme
				 * @tparam T The type of the stored node data
				 *
				 * @return An error status indicating the success or failure of the operation
				 * @retval cupcfd::error::E_SUCCESS The method completed successfully
				 */
				__attribute__((warn_unused_result))
				cupcfd::error::eCodes generateParticles(Particle<P,I,T> ** particles, I * nParticles, T dt);

				// === Pure Virtual Methods ===
		};
	}
}

// Include Header Level Definitions
#include "ParticleEmitter.ipp"

#endif
