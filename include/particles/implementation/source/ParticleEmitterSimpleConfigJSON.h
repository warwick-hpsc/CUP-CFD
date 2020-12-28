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

#ifndef CUPCFD_PARTICLES_CONFIG_PARTICLE_EMITTER_SIMPLE_CONFIG_SOURCE_JSON_INCLUDE_H
#define CUPCFD_PARTICLES_CONFIG_PARTICLE_EMITTER_SIMPLE_CONFIG_SOURCE_JSON_INCLUDE_H

#include "ParticleEmitterConfigSource.h"
#include "ParticleEmitterSimple.h"
#include "ParticleSimple.h"
#include "ParticleEmitterSimpleConfig.h"

#include "DistributionConfig.h"

// JsonCPP - Supplied as standalone in include/io/jsoncpp
#include "json.h"
#include "json-forwards.h"

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
		class ParticleEmitterSimpleConfigJSON : public ParticleEmitterConfigSource<ParticleEmitterSimple<I,T>, ParticleSimple<I,T>, I, T>
		{
			public:
				// === Members ===

				/** Json Data Store containing fields for this JSON source **/
				Json::Value configData;

				// === Constructors/Deconstructors ===

				/**
				 * Parse the JSON record provided for values belonging to a ParticleEmitterSimple entry
				 *
				 * @param parseJSON The contents of a "ParticleEmitterSimple" JSON record (not the level above.)
				 */
				ParticleEmitterSimpleConfigJSON(Json::Value& parseJSON);

				/**
				 * Constructor.
				 * Copies data from source to this object.
				 */
				ParticleEmitterSimpleConfigJSON(ParticleEmitterSimpleConfigJSON<I,T>& source);

				/**
				 * Deconstructor.
				 * Currently does nothing.
				 */
				~ParticleEmitterSimpleConfigJSON();

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
				void operator=(ParticleEmitterSimpleConfigJSON<I,T>& source);

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
				ParticleEmitterSimpleConfigJSON<I,T> * clone();

				__attribute__((warn_unused_result))
				cupcfd::error::eCodes getEmitterPosition(cupcfd::geometry::euclidean::EuclideanPoint<T,3>& pos);

				__attribute__((warn_unused_result))
				cupcfd::error::eCodes getRateDistributionConfig(cupcfd::distributions::DistributionConfig<I,T> ** distConfig);

				__attribute__((warn_unused_result))
				cupcfd::error::eCodes getAngleXYDistributionConfig(cupcfd::distributions::DistributionConfig<I,T> ** distConfig);

				__attribute__((warn_unused_result))
				cupcfd::error::eCodes getAngleRotationDistributionConfig(cupcfd::distributions::DistributionConfig<I,T> ** distConfig);

				__attribute__((warn_unused_result))
				cupcfd::error::eCodes getSpeedDistributionConfig(cupcfd::distributions::DistributionConfig<I,T> ** distConfig);

				__attribute__((warn_unused_result))
				cupcfd::error::eCodes getAccelerationXDistributionConfig(cupcfd::distributions::DistributionConfig<I,T> ** distConfig);

				__attribute__((warn_unused_result))
				cupcfd::error::eCodes getAccelerationYDistributionConfig(cupcfd::distributions::DistributionConfig<I,T> ** distConfig);

				__attribute__((warn_unused_result))
				cupcfd::error::eCodes getAccelerationZDistributionConfig(cupcfd::distributions::DistributionConfig<I,T> ** distConfig);

				__attribute__((warn_unused_result))
				cupcfd::error::eCodes getJerkXDistributionConfig(cupcfd::distributions::DistributionConfig<I,T> ** distConfig);

				__attribute__((warn_unused_result))
				cupcfd::error::eCodes getJerkYDistributionConfig(cupcfd::distributions::DistributionConfig<I,T> ** distConfig);

				__attribute__((warn_unused_result))
				cupcfd::error::eCodes getJerkZDistributionConfig(cupcfd::distributions::DistributionConfig<I,T> ** distConfig);

				__attribute__((warn_unused_result))
				cupcfd::error::eCodes getDecayRateDistributionConfig(cupcfd::distributions::DistributionConfig<I,T> ** distConfig);

				__attribute__((warn_unused_result))
				cupcfd::error::eCodes getDecayThresholdDistributionConfig(cupcfd::distributions::DistributionConfig<I,T> ** distConfig);

				/**
				 * Utility function to build a distribution config from a json format (will iterate through potential
				 * formats till matching one is found)
				 */
				__attribute__((warn_unused_result))
				cupcfd::error::eCodes getParticleEmitterDistributionConfig(std::string fieldName,
																				cupcfd::distributions::DistributionConfig<I,T> ** distConfig);


				/**
				 *
				 */
				__attribute__((warn_unused_result))
				cupcfd::error::eCodes buildParticleEmitterConfig(ParticleEmitterConfig<ParticleEmitterSimple<I,T>, ParticleSimple<I,T>, I, T> ** config);
		};
	}
}

// Some definitions for the declarations at a header level
#include "ParticleEmitterSimpleConfigJSON.ipp"

#endif
