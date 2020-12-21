/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Contains declarations for the VectorSourceHDF5 class
 */

#ifndef CUPCFD_PARTICLES_IMPLEMENTATION_SOURCE_PARTICLE_SIMPLE_SOURCE_HDF5_INCLUDE_H
#define CUPCFD_PARTICLES_IMPLEMENTATION_SOURCE_PARTICLE_SIMPLE_SOURCE_HDF5_INCLUDE_H

#include "ParticleSource.h"
#include "ParticleSimple.h"

// Library Functions/Objects
#include <string>

// Error Codes
#include "Error.h"

namespace cupcfd
{
	namespace particles
	{

		/**
		 * Load vector data from a HDF5 source.
		 *
		 * @tparam I The type of the indexing scheme
		 * @tparam T The type of the vector data
		 */
		template <class I, class T>
		class ParticleSimpleSourceHDF5 : public ParticleSource<ParticleSimple<I,T>,I,T>
		{
			public:
				// === Members ===

				/** File where the data is stored. **/
				std::string fileName;

				// === Constructor ===

				/**
				 * Constructor.
				 *
				 * Setup class for reading data from the file at the provided location
				 *
				 * @param fileName Location
				 * @param sourceId ID for this source, unique across all emitters and sources in system; used to create unique particle IDs
				 */
				ParticleSimpleSourceHDF5(std::string fileName, int sourceId);

				/**
				 * Deconstructor.
				 */
				virtual ~ParticleSimpleSourceHDF5();

				// === Concrete Methods ===

				// === Inherited Overloads ===

				__attribute__((warn_unused_result))
				cupcfd::error::eCodes getNParticles(I * nParticles);

				__attribute__((warn_unused_result))
				cupcfd::error::eCodes getParticles(Particle<ParticleSimple<I,T>,I,T> *** particleData, I * nParticles, I * indexes, I nIndexes, I indexBase);

				// === Pure Virtual Methods ===
			
			private:
				int id;
		};
	}
}

// Include Header Level Definitions
#include "ParticleSimpleSourceHDF5.ipp"

#endif
