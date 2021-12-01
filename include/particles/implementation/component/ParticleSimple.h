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
 * Contains declarations for the ParticleSimple Class
 *
 */

#ifndef CUPCFD_PARTICLES_PARTICLE_SIMPLE_INCLUDE_H
#define CUPCFD_PARTICLES_PARTICLE_SIMPLE_INCLUDE_H

// CRTP Base Interface Class
#include "Particle.h"

// Error Codes
#include "Error.h"

#include "EuclideanPoint.h"
#include "EuclideanVector.h"
#include "EuclideanPlane3D.h"

#include "UnstructuredMeshInterface.h"

// Define padding size for particles if unspecified
#ifndef PARTICLE_PADDING
#define PARTICLE_PADDING 0
#endif

namespace cupcfd
{
	namespace particles
	{
		/**
		 * This defines a basic particle that has minimal interaction with the
		 * mesh/finite-volume system as a whole. It does not account for mass, fluid flow
		 * or other particles, focusing upon the provided velocity, acceleration and jerk levels
		 * to determine its travel patterns. It simply provides a object that can be transported
		 * around the mesh.
		 *
		 * A decay rate can be set to ensure that particles are removed from the system.
		 */
		template <class I, class T>
		class ParticleSimple : public Particle<ParticleSimple<I,T>, I, T>
		{
			public:
				// === Members ===

				/** Storage for the MPI Type returned upon registration of this class with the MPI library **/
				static MPI_Datatype mpiType;

				/** Track whether the datatype has been registered with the MPI library**/
				static bool mpiDataTypeReg;

				/**
				 * Acceleration - Rate of change of velocity
				 * Units used should be same as those used for the mesh size and time
				 */
				cupcfd::geometry::euclidean::EuclideanVector<T,3> acceleration;


				/**
				 * Jerk - Rate of change of acceleration
				 * Units used should be same as those used for the mesh size and time
				 */
				cupcfd::geometry::euclidean::EuclideanVector<T,3> jerk;

				bool inactive;

				/**
				 * 'Decay' Level
				 * Means by which we can let a particle decay and disappear from the system.
				 * This variable is set to a non-zero value, and approaches zero depending
				 * on the rate of decay. Once it reaches zero, the particle is removed from
				 * the system.
				 */
				T decayLevel;

				/**
				 * 'Decay' rate.
				 * The rate at which the particle decays per unit time.
				 * This is only a magnitude, not a direction, since the decay level can only
				 * approach zero or remain static in this context.
				 */
				T decayRate;

				/**
				 * Padding.
				 * Used to pad byte count of particle for experimenting with data exchange sizes
				 * etc.
				 */
				char padding[PARTICLE_PADDING];

				// === Constructor/Deconstructors ===

                /**
                 * Setup a particle with default values.
                 *
                 * All values default to zero (or positional/velocity equivalents)
                 * apart from cellID and rank which are set to -1.
                 *
                 * Warning: This particle will likely have to be corrected before use
                 * and is provided for the benefit of creating arrays of particles etc.
                 *
                 * @tparam I The type of the indexing scheme
                 * @tparam T The type of the stored node data
                 */
                ParticleSimple();

				/**
				 * Setup a particle with values set to the parameters
				 *
				 * Parameters not specified here are set to appropriate values
				 * according to the particle state.
				 *
				 * @tparam I The type of the indexing scheme
				 * @tparam T The type of the stored node data
				 */
				ParticleSimple(cupcfd::geometry::euclidean::EuclideanPoint<T,3>& pos,
						 cupcfd::geometry::euclidean::EuclideanVector<T,3>& velocity,
						 cupcfd::geometry::euclidean::EuclideanVector<T,3>& acceleration,
						 cupcfd::geometry::euclidean::EuclideanVector<T,3>& jerk,
						 I id, 
						 I cellGlobalID, I rank,
						 T decayLevel, T decayRate,
						 T travelDt);

				/**
				 * Constructor:
				 * Setup a particle with a copy of the values stored in source.
				 */
				ParticleSimple(const ParticleSimple<I,T>& source);

				/**
				 * Destroy a Particle object
				 *
				 * @tparam I The type of the indexing scheme
				 * @tparam T The type of the stored node data
				 */
				~ParticleSimple();

				// === Concrete Methods ===

				/**
				 * Copy the values from a source particle to this one.
				 *
				 * @param source The source to copy from
				 *
				 * @tparam I The type of the indexing scheme
				 * @tparam T The type of the stored node data
				 *
				 * @return Returns nothing
				 */
				inline void operator=(const ParticleSimple<I,T>& source);

				/**
				 * Set the particle acceleration
				 *
				 * @param The new particle acceleration to use
				 *
				 * @tparam I The type of the indexing scheme
				 * @tparam T The type of the stored node data
				 *
				 * @return Nothing
				 */
				inline void setAcceleration(cupcfd::geometry::euclidean::EuclideanVector<T,3>& acceleration);

				/**
				 * Get the current particle acceleration
				 *
				 * @tparam I The type of the indexing scheme
				 * @tparam T The type of the stored node data
				 *
				 * @return The particle acceleration
				 */
				__attribute__((warn_unused_result))
				inline cupcfd::geometry::euclidean::EuclideanVector<T,3> getAcceleration();

				/**
				 * Set the particle jerk
				 *
				 * @param The new particle jerk to use
				 *
				 * @tparam I The type of the indexing scheme
				 * @tparam T The type of the stored node data
				 *
				 * @return Nothing
				 */
				inline void setJerk(cupcfd::geometry::euclidean::EuclideanVector<T,3>& jerk);

				/**
				 * Get the current particle jerk
				 *
				 * @tparam I The type of the indexing scheme
				 * @tparam T The type of the stored node data
				 *
				 * @return The particle jerk
				 */
				__attribute__((warn_unused_result))
				inline cupcfd::geometry::euclidean::EuclideanVector<T,3> getJerk();

				/**
				 * Set the decay level of the particle.
				 * Zero represents a decayed particle. A positive value represents
				 * the particle still exists to some capacity.
				 *
				 * @param decayLevel The decay level of the particle.
				 *
				 * @tparam I The type of the indexing scheme
				 * @tparam T The type of the stored node data
				 *
				 * @return Nothing
				 */
				inline void setDecayLevel(T decayLevel);
				/**
				 * Get the decay level of the particle.
				 * Zero represents a decayed particle. A positive value represents
				 * the particle still exists to some capacity.
				 *
				 * @tparam I The type of the indexing scheme
				 * @tparam T The type of the stored node data
				 *
				 * @return The particle's decay level
				 */
				__attribute__((warn_unused_result))
				inline T getDecayLevel();

				/**
				 * Set the decay rate of the particle.
				 * Represents the rate per unit time towards which the particle
				 * proceeds towards full decay.
				 *
				 * @param decayRate The decay level of the particle per unit time.
				 *
				 * @tparam I The type of the indexing scheme
				 * @tparam T The type of the stored node data
				 *
				 * @return Nothing
				 */
				inline void setDecayRate(T decayRate);

				/**
				 * Get the decay rate of the particle.
				 *
				 * @tparam I The type of the indexing scheme
				 * @tparam T The type of the stored node data
				 *
				 * @return The particle's decay rate per unit time
				 */
				__attribute__((warn_unused_result))
				inline T getDecayRate();

				inline void setInactive();
				__attribute__((warn_unused_result))
				inline bool getInactive() const;

				/**
				 * Update the particle velocity to the velocity that is reached after applying the currently
				 * set acceleration across the specified time period
				 *
				 * @param dt The time period of acceleration. This should be in the same time units
				 * as the acceleration/velocity of the particle
				 *
				 * @return An error status indicating the success or failure of the operation
				 * @retval cupcfd::error::E_SUCCESS The method completed successfully
				 */
				__attribute__((warn_unused_result))
				inline cupcfd::error::eCodes updateVelocity(T dt);

				/**
				 * Update the particle acceleration to the acceleration that is reached after applying
				 * the jerk across the specified time period.
				 *
				 * @param dt The time period of jerk. This should be in the same time units
				 * as the acceleration/velocity of the particle
				 *
				 * @return An error status indicating the success or failure of the operation
				 * @retval cupcfd::error::E_SUCCESS The method completed successfully
				 */
				__attribute__((warn_unused_result))
				inline cupcfd::error::eCodes updateAcceleration(T dt);

				/**
				 * Update the particle decay level after the specified time period.
				 *
				 * @param dt The time period of day.
				 *
				 * @return An error status indicating the success or failure of the operation
				 * @retval cupcfd::error::E_SUCCESS The method completed successfully
				 */
				__attribute__((warn_unused_result))
				inline cupcfd::error::eCodes updateDecayLevel(T dt);

				/**
				 * Update the velocity of the particle after travelling through the identified cell for a period of dT.
				 *
				 * The means by which the velocity might be influenced is left to the particle specialisation (e.g. acceleration)
				 *
				 * @param mesh The object containing the mesh data
				 * @param cellLocalID The Local ID of the cell in the mesh that the particle travelled through
				 * @param dt The time that elapsed while traversing the cell
				 *
				 * @return An error status indicating the success or failure of the operation
				 * @retval cupcfd::error::E_SUCCESS The method completed successfully and the particle
				 * has no further travel time.
				 */
				template <class M, class L>
				__attribute__((warn_unused_result))
				cupcfd::error::eCodes updateVelocityAtomic(cupcfd::geometry::mesh::UnstructuredMeshInterface<M,I,T,L>& mesh,
																I cellLocalID,
																T dt);
				/**
				 * Update the state of non-positional properties (e.g. physical properties that a particle might have like mass, or source
				 * properties of a cell) after travelling through the identified cell for a period of dT.
				 *
				 * @param mesh The object containing the mesh data
				 * @param cellLocalID The Local ID of the cell in the mesh that the particle travelled through
				 * @param dt The time that elapsed while traversing the cell
				 *
				 * @return An error status indicating the success or failure of the operation
				 * @retval cupcfd::error::E_SUCCESS The method completed successfully and the particle
				 * has no further travel time.
				 */
				template <class M, class L>
				__attribute__((warn_unused_result))
				cupcfd::error::eCodes updateStateAtomic(cupcfd::geometry::mesh::UnstructuredMeshInterface<M,I,T,L>& mesh,
																    I cellLocalID,
																    T dt);

				/**
				 * Update properties of the particle that has been positioned at a cell face for further updates
				 *
				 * @param mesh The object containing the mesh data
				 * @param cellLocalID The Local ID of the cell in the mesh that the particle travelled through
				 * @param dt The time that elapsed while traversing the cell
				 * @param faceLocalID The local ID of the face in the mesh that the particle is currently at
				 *
				 * @return An error status indicating the success or failure of the operation
				 * @retval cupcfd::error::E_SUCCESS The method completed successfully and the particle
				 * has no further travel time.
				 */
				template <class M, class L>
				__attribute__((warn_unused_result))
				cupcfd::error::eCodes updateNonBoundaryFace(cupcfd::geometry::mesh::UnstructuredMeshInterface<M,I,T,L>& mesh,
																 I faceLocalID);


				/**
				 * Update properties of the particle to handle encountering a boundary wall condition while
				 * it has been positioned at a cell face for further updates.
				 *
				 * For a ParticleSimple cell, this mirrors the velocity, acceleration and jerk vectors against
				 * the normal of the face in question. This presumes that the cell is located at the boundary,
				 * and that the velocity is facing the correct direction for a reflection to be valid (i.e. the velocity
				 * is moving into the boundary face so it should be reflected).
				 *
				 * @param mesh The object containing the mesh data
				 * @param cellLocalID The Local ID of the cell in the mesh that the particle travelled through
				 * @param dt The time that elapsed while traversing the cell
				 * @param faceLocalID The local ID of the face in the mesh that the particle is currently at
				 *
				 * @return An error status indicating the success or failure of the operation
				 * @retval cupcfd::error::E_SUCCESS The method completed successfully and the particle
				 * has no further travel time.
				 */
				template <class M, class L>
				__attribute__((warn_unused_result))
				cupcfd::error::eCodes updateBoundaryFaceWall(cupcfd::geometry::mesh::UnstructuredMeshInterface<M,I,T,L>& mesh,
																		  I cellLocalID, I faceLocalID);

				/**
				 * Update properties of the particle to handle encountering a boundary symp condition while
				 * it has been positioned at a cell face for further updates
				 *
				 * For a ParticleSimple cell, 'correct' behaviour is not currently implemented, instead this
				 * behaves in the same manner as for a wall boundary.
				 *
				 * @param mesh The object containing the mesh data
				 * @param cellLocalID The Local ID of the cell in the mesh that the particle travelled through
				 * @param dt The time that elapsed while traversing the cell
				 * @param faceLocalID The local ID of the face in the mesh that the particle is currently at
				 *
				 * @return An error status indicating the success or failure of the operation
				 * @retval cupcfd::error::E_SUCCESS The method completed successfully and the particle
				 * has no further travel time.
				 */
				template <class M, class L>
				__attribute__((warn_unused_result))
				cupcfd::error::eCodes updateBoundaryFaceSymp(cupcfd::geometry::mesh::UnstructuredMeshInterface<M,I,T,L>& mesh,
						  	  	  	  	  	  	  	  	  	  	  	  	  I cellLocalID, I faceLocalID);

				/**
				 * Update properties of the particle to handle encountering a boundary inlet condition while
				 * it has been positioned at a cell face for further updates
				 *
				 * For a ParticleSimple cell, 'correct' behaviour is not currently implemented, instead this
				 * behaves in the same manner as for a wall boundary.
				 *
				 * @param mesh The object containing the mesh data
				 * @param cellLocalID The Local ID of the cell in the mesh that the particle travelled through
				 * @param dt The time that elapsed while traversing the cell
				 * @param faceLocalID The local ID of the face in the mesh that the particle is currently at
				 *
				 * @return An error status indicating the success or failure of the operation
				 * @retval cupcfd::error::E_SUCCESS The method completed successfully and the particle
				 * has no further travel time.
				 */
				template <class M, class L>
				__attribute__((warn_unused_result))
				cupcfd::error::eCodes updateBoundaryFaceInlet(cupcfd::geometry::mesh::UnstructuredMeshInterface<M,I,T,L>& mesh,
						  	  	  	  	  	  	  	  	  	  	  	  	   I cellLocalID, I faceLocalID);

				/**
				 * Update properties of the particle to handle encountering a boundary outlet condition while
				 * it has been positioned at a cell face for further updates
				 *
				 * For a ParticleSimple cell, 'correct' behaviour is not currently implemented, instead this
				 * behaves in the same manner as for a wall boundary.
				 *
				 * @param mesh The object containing the mesh data
				 * @param cellLocalID The Local ID of the cell in the mesh that the particle travelled through
				 * @param dt The time that elapsed while traversing the cell
				 * @param faceLocalID The local ID of the face in the mesh that the particle is currently at
				 *
				 * @return An error status indicating the success or failure of the operation
				 * @retval cupcfd::error::E_SUCCESS The method completed successfully and the particle
				 * has no further travel time.
				 */
				template <class M, class L>
				__attribute__((warn_unused_result))
				cupcfd::error::eCodes updateBoundaryFaceOutlet(cupcfd::geometry::mesh::UnstructuredMeshInterface<M,I,T,L>& mesh,
						  	  	  	  	  	  	  	  	  	  	  	I cellLocalID, I faceLocalID);

				// === Pure Virtual Methods ===

				// === Inherited Overloaded Methods ===

				__attribute__((warn_unused_result))
				inline cupcfd::error::eCodes getMPIType(MPI_Datatype * dType);

				__attribute__((warn_unused_result))
				cupcfd::error::eCodes registerMPIType();

				__attribute__((warn_unused_result))
				cupcfd::error::eCodes deregisterMPIType();

				__attribute__((warn_unused_result))
				inline bool isRegistered();
		};
	}
}

// Include Header Level Definitions
#include "ParticleSimple.ipp"

// Explicit instantiation declarations of class static variables:
extern template MPI_Datatype cupcfd::particles::ParticleSimple<int, float>::mpiType;
extern template bool cupcfd::particles::ParticleSimple<int, float>::mpiDataTypeReg;
extern template MPI_Datatype cupcfd::particles::ParticleSimple<int, double>::mpiType;
extern template bool cupcfd::particles::ParticleSimple<int, double>::mpiDataTypeReg;

#endif
