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
 * Contains declarations for the ParticleBase Class
 */

#ifndef CUPCFD_PARTICLES_PARTICLE_INCLUDE_H
#define CUPCFD_PARTICLES_PARTICLE_INCLUDE_H

// Error Codes
#include "Error.h"

// Communicator Details
#include "Communicator.h"

// Euclidean Space Data Structures
#include "EuclideanPoint.h"
#include "EuclideanVector.h"

#include "UnstructuredMeshInterface.h"

namespace cupcfd
{
	namespace particles
	{
		/**
		 * This class is intended as an interface for specialised particle classes.
		 *
		 * It defines the minimum required function declarations for implementing
		 * a particle that can move and interact with a mesh. This interface
		 * lets us declare the methods that can be used for interacting with particles,
		 * while leaving how the particles fulfill those methods to the specialised
		 * versions.
		 *
		 * The first design pattern for this uses a template method pattern for any
		 * concrete methods that are common and unchanging across all particles
		 * but refer to specialised functions implemented by specialised classes.
		 *
		 * The second design pattern for this is CRTP (Curiously Recurring Template
		 * Pattern) to allow us to refer to specialised methods without incurring the
		 * penalty of using virtual methods. The primary downside of this is that
		 * we must know the types we wish to use at compile time (hence the lack of
		 * virtual overhead).
		 *
		 * It is also a requirement that all subclasses implement the CustomMPIType
		 * functions such that particles can be exchanged between processes.
		 *
		 * @tparam P A specialised particle class that inherits from this interface with itself
		 * as the parameter P (e.g. : Particle<ParticleSpecialised>) at the class declaration.
		 * It must implement all methods called on the P parameter in this class (else template
		 * error will ensure at compile)
		 * @tparam I The type of the particle's indexing scheme
		 * @tparam T The type of the particle/mesh structural data (e.g. spatial position)
		 */
		template <class P, class I, class T>
		class Particle : public cupcfd::comm::mpi::CustomMPIType
		{
			public:
				// === Members ===

				/**
				 * Velocity
				 * Units used should be same as those used for the mesh size and time
				 **/
				cupcfd::geometry::euclidean::EuclideanVector<T,3> velocity;

				/** Position of the particle when not in a in-flight traversal operation
				 * I.e Used as the start position, and updated at the end **/
				cupcfd::geometry::euclidean::EuclideanPoint<T,3> pos;

				/**
				 * 'In-flight' position when moving the particle
				 * Necessary for tracking particle in motion if having to traverse cells/MPI boundaries
				 **/
				cupcfd::geometry::euclidean::EuclideanPoint<T,3> inflightPos;

				/** Stores the remaining time to travel for 'in-flight' particles **/
				T travelDt;

				// === Constructors/Deconstructors ===

				/**
				 * Empty Default Constructor
				 */
				Particle();

				/**
				 * Constructor: Set up any elements common to all specialised particles.
				 */
				Particle(cupcfd::geometry::euclidean::EuclideanVector<T,3> velocity,
						 cupcfd::geometry::euclidean::EuclideanPoint<T,3> pos,
						 I id, 
						 I cellGlobalID, I rank, T travelDt);

				/**
				 *
				 */
				virtual ~Particle();

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
				inline void operator=(Particle& source);
				/**
				 * Set the position of the particle
				 *
				 * @param pos The new particle position
				 *
				 * @return Nothing
				 */
				inline void setPos(cupcfd::geometry::euclidean::EuclideanPoint<T,3>& pos);

				/**
				 * Get the position of the particle
				 *
				 * @return The position of the particle
				 */
				inline cupcfd::geometry::euclidean::EuclideanPoint<T,3> getPos();

				/**
				 * Set the current in-flight position
				 *
				 * @param inflightPos The new in-flight position
				 *
				 * @return Nothing
				 */
				inline void setInFlightPos(cupcfd::geometry::euclidean::EuclideanPoint<T,3>& inflightPos);

				/**
				 * Get the current in-flight position for the particle
				 *
				 * @return The current in-flight position of the particle
				 */
				__attribute__((warn_unused_result))
				inline cupcfd::geometry::euclidean::EuclideanPoint<T,3> getInFlightPos();

				/**
				 * Set the particle velocity
				 *
				 * @param velocity The new particle velocity
				 *
				 * @return Nothing
				 */
				inline void setVelocity(cupcfd::geometry::euclidean::EuclideanVector<T,3>& velocity);

				/**
				 * Get the particle velocity
				 *
				 * @return The particle velocity
				 */
				__attribute__((warn_unused_result))
				inline cupcfd::geometry::euclidean::EuclideanVector<T,3> getVelocity();

				/**
				 * Get the owner rank of the particle
				 *
				 * @return The owner rank of the particle
				 */
				__attribute__((warn_unused_result))
				inline I getRank();

				/**
				 * Get the cell ID of the particle
				 *
				 * @return The cell ID of the particle
				 */
				__attribute__((warn_unused_result))
				inline I getCellGlobalID() const;

				/**
				 * Get the ID of cell that particle was in previous to current cell
				 *
				 * @return Cell ID
				 */
				__attribute__((warn_unused_result))
				inline I getLastCellGlobalID() const;

				/**
				 * Get the ID of face through which particle entered current cell
				 *
				 * @return Face ID
				 */
				__attribute__((warn_unused_result))
				inline I getCellEntryFaceLocalID() const;

				/**
				 * Get unique identifier of this particle
				 *
				 * @return Particle ID
				 **/
				__attribute__((warn_unused_result))
				inline I getParticleID() const;

				/**
				 * Set the remaining travel time of the particle within the current time period.
				 *
				 * @param dt The remaining time to set the particle travel time to
				 *
				 * @return Nothing
				 */
				inline void setTravelTime(T dt);

				/**
				 * Get the remaining travel time of the particle within the current time period.
				 *
				 * @return The remaining travel time of the particle
				 */
				__attribute__((warn_unused_result))
				inline T getTravelTime() const;

				/**
				 * Set a particle to be 'inactive'/dead to the system
				 * Particles that are set to be inactive should not be restored to be
				 * active, hence no boolean parameter is provided.
				 *
				 * @param active A boolean to indicate whether the particle is Inactive or not
				 * True is inactive, false is active.
				 *
				 * @return Nothing
				 */
				inline void setInactive();

				/**
				 * Get whether the particle is inactive or not.
				 *
				 * @return Whether the particle is inactive or not
				 * @retval true The particle is inactive
				 * @retval false The particle is active
				 */
				__attribute__((warn_unused_result))
				inline bool getInactive() const;

				/**
				 * Check that particle attributes do not contradict
				 *
				 * @return Whether the particle is in an invalid state
				 * @retval true No contradictions
				 * @retval false Contradiction detected
				 */
				__attribute__((warn_unused_result))
				inline bool stateValid() const;

				/**
				 * Print variable values of this particle, for debugging
				 */
				inline void print() const;

				/**
				 * Using the currently set velocity, move the particle until it either reaches the
				 * face of a cell, or runs out of travel time before exiting the cell.
				 *
				 * This will update the particles 'inflight' position, as well as reduce the remaining
				 * travel time for the particle.
				 *
				 * It will not update the Cell ID of the particle, it will leave it unchanged with the
				 * particle located on the face. However, the localFaceID will be stored in the pointer
				 * provided, so it can be updated after the fact via whichever means are preferred.
				 * If the particle does not exit the cell, or the particle does not move (travel time of 0)
				 * then the localFaceID is set to -1.
				 *
				 * The time taken (i.e. the travel time reduction) is also stored in a provided pointer.
				 *
				 * @param mesh The object containing the mesh data
				 * @param dt A pointer to the location that will be updated with the amount of time that elapsed while
				 * moving the particle through the cell.
				 * @param exitFaceLocalID A pointer to the location that will be updated with local ID of the face
				 * the particle is exiting via/positioned on.
				 *
				 * @tparam I The type of the indexing scheme
				 * @tparam T The type of the stored node data
				 * @tparam M The implementation type of the UnstructuredMeshInterface
				 * @tparam L The label type of cells, faces etc in the mesh (different from local, global IDs)
				 *
				 * @return An error status indicating the success or failure of the operation
				 * @retval cupcfd::error::E_SUCCESS The method completed successfully and the particle
				 * has no further travel time.
				 */
				template <class M, class L>
				__attribute__((warn_unused_result))
				cupcfd::error::eCodes updatePositionAtomic(cupcfd::geometry::mesh::UnstructuredMeshInterface<M,I,T,L>& mesh,
															T * dt,
															I * exitFaceLocalID, bool verbose);
				
				/**
				 * Calculate intersection of this particle with specified face. 
				 *
				 * @param mesh The object containing the mesh data
				 * @param faceID The ID of the face to analyse
				 * 
				 * 
				 * @return An error status indicating the success or failure of the operation
				 * @retval cupcfd::error::E_SUCCESS The method completed successfully and the particle
				 * has no further travel time.
				 */
				template <class M, class L>
				__attribute__((warn_unused_result))
				cupcfd::error::eCodes calculateFaceIntersection(cupcfd::geometry::mesh::UnstructuredMeshInterface<M,I,T,L>& mesh,
																I faceID, 
																bool verbose, 
																bool& doesIntersect, 
																cupcfd::geometry::euclidean::EuclideanPoint<T,3>& intersection, 
																bool& intersectionOnEdge,
																T& timeToIntersect);

				/**
				 * Detect entry face ID of current cell.
				 *
				 * Particle must maintain a record of which face it entered current cell through. But face ID is local to rank. 
				 * Thus, when transferred between MPI ranks, ID must be redetected. Implementing this was deemed easier than 
				 * (a) adding concept of global face IDs and (b) mapping of local to glocal face IDs.
				 *
				 * @param mesh The object containing the mesh data
				 *
				 * @return An error status indicating the success or failure of the operation
				 * @retval cupcfd::error::E_SUCCESS The method completed successfully and the particle
				 * has no further travel time.
				**/
				template <class M, class L>
				__attribute__((warn_unused_result))
				cupcfd::error::eCodes redetectEntryFaceID(cupcfd::geometry::mesh::UnstructuredMeshInterface<M,I,T,L>& mesh);

				/**
				 * Update the velocity of the particle after travelling through the identified cell for a period of dT.
				 *
				 * The means by which the velocity might be influenced is left to the particle specialisation (e.g. acceleration)
				 *
				 * @param mesh The object containing the mesh data
				 * @param cellLocalID The local ID of the cell in the mesh that the particle travelled through
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
				 * it has been positioned at a cell face for further updates
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



				// === Inherited Overloaded Methods ===

				// These are the methods required for MPI Communication of the particle.
				// All specialised particles must implement these methods

				__attribute__((warn_unused_result))
				virtual inline cupcfd::error::eCodes getMPIType(MPI_Datatype * dType);
				__attribute__((warn_unused_result))
				virtual cupcfd::error::eCodes registerMPIType();
				__attribute__((warn_unused_result))
				virtual cupcfd::error::eCodes deregisterMPIType();
				__attribute__((warn_unused_result))
				virtual inline bool isRegistered();

			protected:
				/**
				 * Unique particle identifier, to assist debugging
				 **/
				I particleID;

				/**
				 * Stores the particles rank. If equal to current rank, it is not exchanged.
				 * If it does not equal the current rank, it is destined for another rank and
				 * must be exchanged at the next exchange stage (else we will not know what to
				 * do with it during the next update position stage).
				 */
				I rank;

				/**
				 * Previous rank that owned this particle, to assist debugging
				 **/
				I lastRank;

				/**
				 * Set the cell ID of the particle, with safety checks to prevent infinite 
				 * back-and-forth
				 *
				 * @param cellGlobalID The cellID to set the particle cellID to
				 * @param cellEntryFaceLocalID ID of the face through which particle is entering cell
				 *
				 * @return Nothing
				 */
				__attribute__((warn_unused_result))
				inline cupcfd::error::eCodes safelySetCellGlobalID(I cellGlobalID, I cellEntryFaceLocalID);

				/**
				 * Global ID of the cell in a mesh this particle is currently located in.
				 * If a particle desires an exchange, this should be the destination
				 * global cell ID.
				 *
				 * We use Global Over Local here since we can translate it to the local ID via a mesh,
				 * and local IDs do not translate between processes when communicating.
				 **/
				I cellGlobalID;

				/**
				 * Global ID of cell that particle was in before current. Used for debugging/verifying particle movement
				 **/
				I lastCellGlobalID;

				/**
				 * Global ID of cell that particle was in before last. Used for debugging/verifying particle movement
				 **/
				I lastLastCellGlobalID;

				/**
				 * Local ID of face used to enter cell
				 **/
				I cellEntryFaceLocalID;

			private:

		};
	}
}

// Include Header Level Definitions
#include "Particle.ipp"

#endif
