/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Contains declarations for the ParticleSource class
 */

#ifndef CUPCFD_PARTICLES_INTERFACE_SOURCE_PARTICLE_SOURCE_INCLUDE_H
#define CUPCFD_PARTICLES_INTERFACE_SOURCE_PARTICLE_SOURCE_INCLUDE_H

#include "Particle.h"

#include "Error.h"

namespace cupcfd
{
	namespace particles
	{
		/**
		 * This class defines an interface for accessing/loading Particle data
		 * from some as yet undefined source (e.g. a file).
		 *
		 * @tparam I The type of the indexing scheme
		 * @tparam T The type of the stored node data
		 */
		template <class P, class I, class T>
		class ParticleSource
		{
			public:
				// === Members ===

				// === Constructor ===

				/**
				 * Constructor:
				 * Default - current sets nothing.
				 *
				 * @tparam I The type of the indexing scheme
				 * @tparam T The type of the stored node data
				 */
				ParticleSource();

				/**
				 * Deconstructor
				 *
				 * @tparam I The type of the indexing scheme
				 * @tparam T The type of the stored node data
				 */
				virtual ~ParticleSource();

				// === Concrete Methods ===

				// === Pure Virtual Methods ===

				/**
				 * Get the number of particles stored in the data source
				 *
				 * @param vecSize A pointer to the location updated with the number of elements
				 *
				 * @return An error code indicating the success or failure of the function
				 * @retval cupcfd::error::E_SUCCESS The function completed successfully
				 */
				virtual cupcfd::error::eCodes getNParticles(I * nParticles) = 0;

				/**
				 * Get the particles that exist on this rank for the provided mesh.
				 *
				 * Note: Due to the lack of a mesh at this stage, the values of the cell global ID and rank for
				 * the particles should be treated as undetermined. It is expected that the callee will process
				 * these particles alongside a mesh to establish these values, if valid.
				 *
				 * @param particleData A pointer to where a pointer for a newly created array will be stored,
				 * This array will contain pointer to particles loaded from the source that belong to this rank for the
				 * given mesh.
				 * Cleanup of the object pointers inside the array is left to the callee.
				 * @param nParticles A pointer to where the number of retrieved particles will be stored
				 * @param indexes An array of the indexes to retrieve data for from the data source
				 * @param nIndexes The size of the indexes array in the number of elements of type I
				 * @param indexBase The base index used for the indexes provided in the indexes array (e.g. 0 for zero-indexed etc).
				 * These will be shifted if the data source requires it.
				 *
				 * @return An error code indicating the success or failure of the function
				 * @retval cupcfd::error::E_SUCCESS The function completed successfully
				 */
				virtual cupcfd::error::eCodes getParticles(Particle<P,I,T> *** particleData, I * nParticles, I * indexes, I nIndexes, I indexBase) = 0;
		};
	}
}

// Include Header Level Definitions
#include "ParticleSource.ipp"

#endif
