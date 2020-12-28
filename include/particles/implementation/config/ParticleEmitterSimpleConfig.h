/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Contains declarations for the ParticleEmitterSimpleConfig class.
 */

#ifndef CUPCFD_PARTICLES_CONFIG_PARTICLE_EMITTER_SIMPLE_CONFIG_INCLUDE_H
#define CUPCFD_PARTICLES_CONFIG_PARTICLE_EMITTER_SIMPLE_CONFIG_INCLUDE_H

#include "ParticleEmitterConfig.h"
#include "ParticleEmitterSimple.h"

#include "DistributionConfig.h"

namespace cupcfd
{
	namespace particles
	{
		/**
		 *
		 * @tparam I The indexing scheme of the partitioner.
		 * @tparam T The datatype of the partitioner nodes/graph to be partitioned.
		 */
		template <class I, class T>
		class ParticleEmitterSimpleConfig : public ParticleEmitterConfig<ParticleEmitterSimple<I,T>, ParticleSimple<I,T>, I, T>
		{
			public:
				// === Members ===

				cupcfd::distributions::DistributionConfig<I,T> * rate;
				cupcfd::distributions::DistributionConfig<I,T> * angleXY;
				cupcfd::distributions::DistributionConfig<I,T> * angleRotation;
				cupcfd::distributions::DistributionConfig<I,T> * speed;
				cupcfd::distributions::DistributionConfig<I,T> * accelerationX;
				cupcfd::distributions::DistributionConfig<I,T> * accelerationY;
				cupcfd::distributions::DistributionConfig<I,T> * accelerationZ;
				cupcfd::distributions::DistributionConfig<I,T> * jerkX;
				cupcfd::distributions::DistributionConfig<I,T> * jerkY;
				cupcfd::distributions::DistributionConfig<I,T> * jerkZ;
				cupcfd::distributions::DistributionConfig<I,T> * decayRate;
				cupcfd::distributions::DistributionConfig<I,T> * decayThreshold;

				// === Constructors/Deconstructors ===

				/**
				 * Constructor.
				 * Currently does nothing.
				 */
				ParticleEmitterSimpleConfig(cupcfd::geometry::euclidean::EuclideanPoint<T,3>& position,
											cupcfd::distributions::DistributionConfig<I,T>& rate,
											cupcfd::distributions::DistributionConfig<I,T>& angleXY,
											cupcfd::distributions::DistributionConfig<I,T>& angleRotation,
											cupcfd::distributions::DistributionConfig<I,T>& speed,
											cupcfd::distributions::DistributionConfig<I,T>& accelerationX,
											cupcfd::distributions::DistributionConfig<I,T>& accelerationY,
											cupcfd::distributions::DistributionConfig<I,T>& accelerationZ,
											cupcfd::distributions::DistributionConfig<I,T>& jerkX,
											cupcfd::distributions::DistributionConfig<I,T>& jerkY,
											cupcfd::distributions::DistributionConfig<I,T>& jerkZ,
											cupcfd::distributions::DistributionConfig<I,T>& decayRate,
											cupcfd::distributions::DistributionConfig<I,T>& decayThreshold);

				/**
				 * Constructor.
				 * Copies data from source to this object.
				 */
				ParticleEmitterSimpleConfig(const ParticleEmitterSimpleConfig<I,T>& source);

				/**
				 * Deconstructor.
				 * Currently does nothing.
				 */
				~ParticleEmitterSimpleConfig();

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
				void operator=(const ParticleEmitterSimpleConfig<I,T>& source);

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
				ParticleEmitterSimpleConfig<I,T> * clone();

				/**
				 *
				 */
				__attribute__((warn_unused_result))
				cupcfd::error::eCodes buildParticleEmitter(ParticleEmitter<ParticleEmitterSimple<I,T>, ParticleSimple<I,T>,I,T> ** emitter, int emitterId);
		};
	}
}

// Some definitions for the declarations at a header level
#include "ParticleEmitterSimpleConfig.ipp"

#endif
