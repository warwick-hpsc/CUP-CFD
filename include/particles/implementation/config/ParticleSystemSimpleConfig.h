/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Contains declarations for the ParticleSystemSimpleConfig class.
 */

#ifndef CUPCFD_PARTICLES_CONFIG_PARTICLE_SYSTEM_SIMPLE_CONFIG_INCLUDE_H
#define CUPCFD_PARTICLES_CONFIG_PARTICLE_SYSTEM_SIMPLE_CONFIG_INCLUDE_H

#include "ParticleSystemConfig.h"
#include "ParticleSystemSimple.h"
#include "ParticleEmitterSimpleConfig.h"
#include "ParticleSourceConfig.h"

#include <vector>

namespace cupcfd
{
	namespace particles
	{
		/**
		 * Store the common elements required for setting up any generic partitioner approach.
		 *
		 * Currently this acts as a virtual base class for polymorphism purposes.
		 *
		 * @tparam M Specialisation Type of the Mesh
		 * @tparam I Type of the indexing scheme
		 * @tparam T Type of the mesh spatial volume
		 * @tparam L Label type of the mesh
		 */
		template <class M, class I, class T, class L>
		class ParticleSystemSimpleConfig : public ParticleSystemConfig<ParticleSystemSimple<M,I,T,L>, ParticleEmitterSimple<I,T>, ParticleSimple<I,T>, M,I,T,L>
		{
			public:
				// === Members ===

				std::vector<ParticleEmitterConfig<ParticleEmitterSimple<I,T>, ParticleSimple<I,T>,I,T> *> emitterConfigs;

				ParticleSourceConfig<ParticleSimple<I,T>, I, T> * particleSourceConfig;

				// === Constructors/Deconstructors ===

				/**
				 * Constructor.
				 * Stores the provided configurations for later setup of a ParticleSystemSimple.
				 *
				 * @param emitterConfigs A vector of configurations for emitters to add to the system. If there are no emitters, pass an empty vector.
				 * @param particleSourceConfig A configuration for a data source to load particles from for the initial system state. If no such source
				 * exists, pass nullptr as the value.
				 */
				ParticleSystemSimpleConfig(std::vector<ParticleEmitterConfig<ParticleEmitterSimple<I,T>, ParticleSimple<I,T>,I,T> *>& emitterConfigs,
										   ParticleSourceConfig<ParticleSimple<I,T>, I, T> * particleSourceConfig);

				/**
				 * Constructor.
				 * Copies data from source to this object.
				 */
				ParticleSystemSimpleConfig(ParticleSystemSimpleConfig<M,I,T,L>& source);

				/**
				 * Deconstructor.
				 * Currently does nothing.
				 */
				~ParticleSystemSimpleConfig();

				// === Concrete Methods ===

				I getNEmitterConfigs();

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
				void operator=(ParticleSystemSimpleConfig<M,I,T,L>& source);

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
				ParticleSystemSimpleConfig<M,I,T,L> * clone();

				/**
				 *
				 */
				__attribute__((warn_unused_result))
				cupcfd::error::eCodes buildParticleSystem(ParticleSystem<ParticleSystemSimple<M, I, T, L>, ParticleEmitterSimple<I,T>, ParticleSimple<I,T>, M, I, T, L> ** system,
															std::shared_ptr<M> meshPtr);

			private:
				int numParticleSourcesOrEmitters = 0;
		};
	}
}

// Some definitions for the declarations at a header level
#include "ParticleSystemSimpleConfig.ipp"

#endif
