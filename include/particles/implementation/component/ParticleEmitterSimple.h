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
 * Contains declarations for the ParticleEmitterSimple Class
 *
 */

#ifndef CUPCFD_PARTICLES_PARTICLE_EMITTER_SIMPLE_INCLUDE_H
#define CUPCFD_PARTICLES_PARTICLE_EMITTER_SIMPLE_INCLUDE_H

#include "ParticleEmitter.h"
#include <memory>
#include "ParticleSimple.h"

namespace cupcfd
{
	namespace particles
	{
		/**
		 *
		 */
		template <class I, class T>
		class ParticleEmitterSimple : public ParticleEmitter<ParticleEmitterSimple<I,T>, ParticleSimple<I,T>, I, T>
		{
			public:
				// Members

				// ToDo: Do we want these to be shared pointers?
				// Might prefer clones so they can't be changed from outside the class....

				/** Distribution that governs the rate of particle generation **/
				cupcfd::distributions::Distribution<I,T> * rate;

				/**
				 * Distribution that governs the angle of generated particles in the XY plane -
				 * i.e. the 'up/down' angle
				 * Measured in radians from -pi to pi from the positive x-axis
				 **/
				cupcfd::distributions::Distribution<I,T> * angleXY;

				/**
				 * Distribution that governs the angle of generated particles in the XZ plane -
				 * i.e. the 'rotation' (the degree of rotation of the XY angle/vector into the Z plane about the Y axis)
				 * Measured in radians from -pi to pi from the positive x axis
				 **/
				cupcfd::distributions::Distribution<I,T> * angleRotation;

				/**
				 * Distribution that governs the speed (velocity magnitude) of generated particles
				 * Treated as spatial unit/second
				 **/
				cupcfd::distributions::Distribution<I,T> * speed;

				/**
				 * Distribution that governs the acceleration of generated particles in the X direction
				 **/
				cupcfd::distributions::Distribution<I,T> * accelerationX;

				/**
				 * Distribution that governs the acceleration of generated particles in the Y direction
				 **/
				cupcfd::distributions::Distribution<I,T> * accelerationY;

				/**
				 * Distribution that governs the acceleration of generated particles in the Z direction
				 **/
				cupcfd::distributions::Distribution<I,T> * accelerationZ;

				/**
				 * Distribution that governs the jerk of generated particles in the X direction
				 **/
				cupcfd::distributions::Distribution<I,T> * jerkX;

				/**
				 * Distribution that governs the jerk of generated particles in the X direction
				 **/
				cupcfd::distributions::Distribution<I,T> * jerkY;

				/**
				 * Distribution that governs the jerk of generated particles in the X direction
				 **/
				cupcfd::distributions::Distribution<I,T> * jerkZ;

				/**
				 * Distribution that governs the decay rate of generated particles
				 **/
				cupcfd::distributions::Distribution<I,T> * decayRate;

				/**
				 * Distribution that governs the threshold point at which particles are considered 'decayed' and removed from the system
				 */
				cupcfd::distributions::Distribution<I,T> * decayThreshold;

				/**
				 * When generating particles, we will eventually exceed the dt period, and so the next 'time' is
				 * not used. We store that time here to use for the next round of particle generation so that
				 * it is not lost, in case it skews distributions.
				 * A value of -1 indicates it is not set.
				 * **/
				T nextParticleTime;

				// Constructors/Deconstructors

				/**
				 * Create an emitter of simple (massless, point) particles. This emitter acts as a point source.
				 *
				 * The properties of the emitted particles are determined by distributions. If a fixed value
				 * is desired, a fixed distribution can be used.
				 *
				 * @param localCellID The local ID of the cell the emitter is in
				 * @param globalCellID The global ID of the cell the emitter is in
				 * @param rank The process rank that holds the globalCellID for the intended mesh (Particles will be generated
				 * with this rank ID)
				 * @param emitterId ID for this emitter, unique across all emitters and sources in system; used to create unique particle IDs
				 * @param position The spatial position of the emitter
				 * @param rate The distribution to use for the rate of particle emission (dt between particles)
				 * @param angleXY The distribution to use for the initial angle of the emitted particles velocity in the XY plane (should be in radians)
				 * @param angleXZ The distribution to use for the initial angle of the emitted particles velocity in the XY plane (should be in radians)
				 * @param speed The distribution to use for the initial magnitude of the emitted particles velocity
				 * @param accelerationX The distribution to use for the initial acceleration of the emitted particle in the X dimension
				 * @param accelerationY The distribution to use for the initial acceleration of the emitted particle in the Y dimension
				 * @param accelerationZ The distribution to use for the initial acceleration of the emitted particle in the Z dimension
				 * @param jerkX The distribution to use for the initial jerk of the emitted particle
				 * @param jerkY The distribution to use for the initial jerk of the emitted particle
				 * @param jerkZ The distribution to use for the initial jerk of the emitted particle
				 * @param decay The distribution to use for the decay of the emitted particle
				 */
				ParticleEmitterSimple(I localCellID, I globalCellID, I rank, 
									  int emitterId, 
									  cupcfd::geometry::euclidean::EuclideanPoint<T,3>& position,
									  cupcfd::distributions::Distribution<I,T> * rate,
									  cupcfd::distributions::Distribution<I,T> * angleXY,
									  cupcfd::distributions::Distribution<I,T> * angleRotation,
									  cupcfd::distributions::Distribution<I,T> * speed,
									  cupcfd::distributions::Distribution<I,T> * accelerationX,
									  cupcfd::distributions::Distribution<I,T> * accelerationY,
									  cupcfd::distributions::Distribution<I,T> * accelerationZ,
									  cupcfd::distributions::Distribution<I,T> * jerkX,
									  cupcfd::distributions::Distribution<I,T> * jerkY,
									  cupcfd::distributions::Distribution<I,T> * jerkZ,
									  cupcfd::distributions::Distribution<I,T> * decayRate,
									  cupcfd::distributions::Distribution<I,T> * decayThreshold);


				/**
				 * Copy constructor
				 */
				ParticleEmitterSimple(const ParticleEmitterSimple<I,T>& source);

				/**
				 *
				 */
				~ParticleEmitterSimple();

				// Overridden inherited methods

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
				 */
				cupcfd::error::eCodes generateParticles(ParticleSimple<I, T> ** particles, I * nParticles, T dt);

				// Concrete Methods
			
			private:
				int id;
		};
	}
}

// Include Header Level Definitions
#include "ParticleEmitterSimple.h"

#endif
