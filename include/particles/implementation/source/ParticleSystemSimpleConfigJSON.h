/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Contains declarations for the ParticleSystemSimpleConfigJSON class.
 */

#ifndef CUPCFD_PARTICLES_CONFIG_PARTICLE_SYSTEM_SIMPLE_CONFIG_SOURCE_JSON_INCLUDE_H
#define CUPCFD_PARTICLES_CONFIG_PARTICLE_SYSTEM_SIMPLE_CONFIG_SOURCE_JSON_INCLUDE_H

#include "ParticleSystemConfigSource.h"
#include "ParticleSystemSimpleConfig.h"
#include "ParticleSourceConfig.h"

// JsonCPP - Supplied as standalone in include/io/jsoncpp
#include "json.h"
#include "json-forwards.h"

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
		class ParticleSystemSimpleConfigJSON : public ParticleSystemConfigSource<ParticleSystemSimple<M,I,T,L>, ParticleEmitterSimple<I,T>, ParticleSimple<I,T>, M,I,T,L>
		{
			public:
				// === Members ===

				/**
				 * Json Storage of Data
				 */
				Json::Value configData;

				// === Constructors/Deconstructors ===

				/**
				 *
				 */
				ParticleSystemSimpleConfigJSON(Json::Value& configData);

				/**
				 * Constructor.
				 * Copies data from source to this object.
				 */
				ParticleSystemSimpleConfigJSON(ParticleSystemSimpleConfigJSON<M,I,T,L>& source);

				/**
				 * Deconstructor.
				 * Currently does nothing.
				 */
				virtual ~ParticleSystemSimpleConfigJSON();

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
				virtual void operator=(ParticleSystemSimpleConfigJSON<M,I,T,L>& source);

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
				ParticleSystemSimpleConfigJSON<M,I,T,L> * clone();

				/**
				 *
				 */
				__attribute__((warn_unused_result))
				cupcfd::error::eCodes getParticleEmitterConfigs(std::vector<ParticleEmitterConfig<ParticleEmitterSimple<I,T>, ParticleSimple<I,T>, I, T> *>& configs);

				/**
				 *
				 */
				__attribute__((warn_unused_result))
				cupcfd::error::eCodes getParticleSourceConfig(ParticleSourceConfig<ParticleSimple<I,T>, I, T> ** particleSourceConfig);

				/**
				 *
				 */
				__attribute__((warn_unused_result))
				cupcfd::error::eCodes buildParticleSystemConfig(ParticleSystemConfig<ParticleSystemSimple<M,I,T,L>, ParticleEmitterSimple<I,T>, ParticleSimple<I,T>, M,I,T,L> ** config);
		};
	}
}

// Some definitions for the declarations at a header level
#include "ParticleSystemSimpleConfigJSON.ipp"

#endif
