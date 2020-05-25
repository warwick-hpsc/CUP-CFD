/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Contains declarations for the ParticleSimpleSourceConfig class.
 */

#ifndef CUPCFD_PARTICLES_CONFIG_PARTICLE_SIMPLE_SOURCE_FILE_CONFIG_INCLUDE_H
#define CUPCFD_PARTICLES_CONFIG_PARTICLE_SIMPLE_SOURCE_FILE_CONFIG_INCLUDE_H

#include "ParticleSourceConfig.h"
#include "ParticleSimple.h"

#include "Error.h"

namespace cupcfd
{
	namespace particles
	{
		enum ParticleSimpleFileFormat
		{
			PARTICLE_SIMPLE_FILE_FORMAT_HDF5
		};

		/**
		 *
		 * @tparam I The indexing scheme of the partitioner.
		 * @tparam T The datatype of the partitioner nodes/graph to be partitioned.
		 */
		template <class I, class T>
		class ParticleSimpleSourceFileConfig : public ParticleSourceConfig<ParticleSimple<I,T>,I,T>
		{
			public:
				// === Members ===

				/** Path to the file where the sparse matrix data is stored **/
				std::string filePath;

				/** Format of the Sparse Matrix file **/
				ParticleSimpleFileFormat format;

				// === Constructors/Deconstructors ===

				/**
				 * Constructor.
				 */
				ParticleSimpleSourceFileConfig(std::string& filePath, ParticleSimpleFileFormat format);

				/**
				 * Constructor.
				 * Copies data from source to this object.
				 */
				ParticleSimpleSourceFileConfig(const ParticleSimpleSourceFileConfig<I,T>& source);

				/**
				 * Deconstructor.
				 * Currently does nothing.
				 */
				virtual ~ParticleSimpleSourceFileConfig();

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
				virtual void operator=(const ParticleSimpleSourceFileConfig<I,T>& source);

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
				virtual ParticleSimpleSourceFileConfig<I,T> * clone();

				/**
				 *
				 */
				cupcfd::error::eCodes buildParticleSource(ParticleSource<ParticleSimple<I,T>,I,T> ** particleSource, int sourceId);
		};
	}
}

// Some definitions for the declarations at a header level
#include "ParticleSimpleSourceFileConfig.ipp"

#endif
