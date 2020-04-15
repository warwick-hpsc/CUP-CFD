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
 * Contains Header Level Definitions for the ParticleSimple Class
 *
 */

#ifndef CUPCFD_PARTICLES_PARTICLE_SIMPLE_IPP_H
#define CUPCFD_PARTICLES_PARTICLE_SIMPLE_IPP_H

#include "Triangle3D.h"
#include "ArithmeticKernels.h"

#include <unistd.h>

namespace arth = cupcfd::utility::arithmetic::kernels;

namespace cupcfd
{
	namespace particles
	{
		// === Concrete Methods ===
		
		template <class I, class T>
		inline void ParticleSimple<I,T>::operator=(const ParticleSimple<I,T>& source)
		{
			this->pos = source.pos;
			this->inflightPos = source.inflightPos;
			this->velocity = source.velocity;
			this->acceleration = source.acceleration;
			this->jerk = source.jerk;
			this->rank = source.rank;
			this->travelDt = source.travelDt;
			this->decayLevel = source.decayLevel;
			this->decayRate = source.decayRate;

			this->cellGlobalID = source.cellGlobalID;
			
			// Padding doesn't matter for setting initial values, 
			// they're expected to be garbage

			this->particleID = source.particleID;
		}
		
		template <class I, class T>
		inline void ParticleSimple<I,T>::setAcceleration(cupcfd::geometry::euclidean::EuclideanVector<T,3>& acceleration)
		{
			this->acceleration = acceleration;
		}

		template <class I, class T>
		inline cupcfd::geometry::euclidean::EuclideanVector<T,3> ParticleSimple<I,T>::getAcceleration()
		{
			return this->acceleration;
		}

		template <class I, class T>
		inline void ParticleSimple<I,T>::setJerk(cupcfd::geometry::euclidean::EuclideanVector<T,3>& jerk)
		{
			this->jerk = jerk;
		}

		template <class I, class T>
		inline cupcfd::geometry::euclidean::EuclideanVector<T,3> ParticleSimple<I,T>::getJerk()
		{
			return this->jerk;
		}
		
		template <class I, class T>
		inline void ParticleSimple<I,T>::setDecayLevel(T decayLevel)
		{
				this->decayLevel = decayLevel;
		}
		
		template <class I, class T>
		inline T ParticleSimple<I,T>::getDecayLevel()
		{
			return this->decayLevel;
		}
		
		template <class I, class T>
		inline void ParticleSimple<I,T>::setDecayRate(T decayRate)
		{
			this->decayRate = decayRate;
		}
		
		template <class I, class T>
		inline T ParticleSimple<I,T>::getDecayRate()
		{
			return this->decayRate;
		}
		
		template <class I, class T>
		inline void ParticleSimple<I,T>::setInactive()
		{
			this->decayLevel = T(0);
		}
		
		template <class I, class T>
		inline bool ParticleSimple<I,T>::getInactive() const
		{
			if(this->decayLevel > T(0))
			{
				return false;
			}
			
			return true;
		}
				

		template <class I, class T>
		inline cupcfd::error::eCodes ParticleSimple<I,T>::updateVelocity(T dt)
		{
			// Change in velocity = acceleration * change in time
			cupcfd::geometry::euclidean::EuclideanVector<T,3> dV = this->acceleration * dt;

			// Velocity = Start Velocity + Change in Velocity
			this->velocity = this->velocity + dV;

			return cupcfd::error::E_SUCCESS;
		}
		
		template <class I, class T>
		inline cupcfd::error::eCodes ParticleSimple<I,T>::updateAcceleration(T dt)
		{
			// Change in acceleration = jerk * change in time
			cupcfd::geometry::euclidean::EuclideanVector<T,3> dA = this->jerk * dt;

			// Velocity = Start Velocity + Change in Velocity
			this->acceleration = this->acceleration + dA;

			return cupcfd::error::E_SUCCESS;
		}
		
		template <class I, class T>
		inline cupcfd::error::eCodes ParticleSimple<I,T>::updateDecayLevel(T dt)
		{
			// Change in decay = rate of decay * change in time
			T dR = this->decayRate * dt;
			
			// ToDo: Should probably put a abs on decay here, but need to match appropriate one for template type....
			this->decayLevel = this->decayLevel - dR;
			
			return cupcfd::error::E_SUCCESS;
		}	

		template <class I, class T>
		template <class M, class L>
		cupcfd::error::eCodes ParticleSimple<I,T>::updateVelocityAtomic(cupcfd::geometry::mesh::UnstructuredMeshInterface<M,I,T,L>& mesh, I cellLocalID, T dt)
		{
			// This particular particle is primarily for testing particle transport
			// It doesn't really acknowledge the impact of mesh cells/state or other particles.
			
			// Update the velocity using the current acceleration
			this->velocity = this->velocity + (this->acceleration * dt);
			
			// Update the acceleration using the jerk value
			this->acceleration = this->acceleration + (this->jerk * dt);

			return cupcfd::error::E_SUCCESS;
		}
		
		template <class I, class T>
		template <class M, class L>
		cupcfd::error::eCodes ParticleSimple<I,T>::updateStateAtomic(cupcfd::geometry::mesh::UnstructuredMeshInterface<M,I,T,L>& mesh, I cellLocalID, T dt)
		{
			return cupcfd::error::E_SUCCESS;
		}														    

		template <class I, class T>
		template <class M, class L> 
		cupcfd::error::eCodes ParticleSimple<I,T>::updateNonBoundaryFace(cupcfd::geometry::mesh::UnstructuredMeshInterface<M,I,T,L>& mesh, I faceLocalID)
		{
			cupcfd::error::eCodes status;

			// If we are sitting at the non-boundary face, all that remains is to set the correct new cell ID and/or identify if we are going off rank
		
			// ToDo: Error Check: The local face ID should be face accessible from the current cellGlobalID set for the particle
		
			// Retrieve Cell1 and Cell2 ID of the Face, we need to know which we are not currently in (i.e.
			// the direction we're crossing the face in)
			I cell1LocalID = mesh.getFaceCell1ID(faceLocalID);
			I cell2LocalID = mesh.getFaceCell2ID(faceLocalID);

			if (cell1LocalID == cell2LocalID) {
				std::cout << "ERROR: getFaceCell1ID() == getFaceCell12D() for face ID " << faceLocalID << std::endl;
				return cupcfd::error::E_ERROR;
			}
					
			// Get the Global IDs
			I node1, node2;
			status = mesh.cellConnGraph->connGraph.getLocalIndexNode(cell1LocalID, &node1);
			if (status != cupcfd::error::E_SUCCESS) {
				std::cout << "ERROR: getLocalIndexNode() failed" << std::endl;
				return status;
			}
			status = mesh.cellConnGraph->connGraph.getLocalIndexNode(cell2LocalID, &node2);
			if (status != cupcfd::error::E_SUCCESS) {
				std::cout << "ERROR: getLocalIndexNode() failed" << std::endl;
				return status;
			}
					
			I cell1GlobalID = mesh.cellConnGraph->nodeToGlobal[node1];
			I cell2GlobalID = mesh.cellConnGraph->nodeToGlobal[node2];

			if(this->cellGlobalID == cell1GlobalID)
			{
				this->cellGlobalID = cell2GlobalID;

				if (this->particleID == 86) {
					std::cout << " > moved to cell " << cell1GlobalID << " --> " << cell2GlobalID << std::endl;
					usleep(100*1000);
				}
			}
			else
			{
				this->cellGlobalID = cell1GlobalID;

				if (this->particleID == 86) {
					std::cout << " > moved to cell " << cell2GlobalID << " --> " << cell1GlobalID << std::endl;
					usleep(100*1000);
				}
			}

			// I cellGlobalID = this->getCellGlobalID();
			// if(cellGlobalID == cell1GlobalID)
			// {
			// 	this->setCellGlobalID(cell2GlobalID);

			// 	if (this->particleID == 86) {
			// 		std::cout << " > moved to cell " << cell1GlobalID << " --> " << cell2GlobalID << std::endl;
			// 		usleep(100*1000);
			// 	}
			// }
			// else
			// {
			// 	this->setCellGlobalID(cell1GlobalID);

			// 	if (this->particleID == 86) {
			// 		std::cout << " > moved to cell " << cell2GlobalID << " --> " << cell1GlobalID << std::endl;
			// 		usleep(100*1000);
			// 	}
			// }
			// cellGlobalID = this->getCellGlobalID();

			// Update the Target Rank if we are crossing into a ghost cell
			
			T node = mesh.cellConnGraph->globalToNode[this->cellGlobalID];
			bool isGhost;
			status = mesh.cellConnGraph->existsGhostNode(node, &isGhost);
			if (status != cupcfd::error::E_SUCCESS) {
				std::cout << "ERROR: getLocalIndexNode() failed" << std::endl;
				return status;
			}
			
			// Check it exists as a ghost node
			if(isGhost)
			{
				// Update rank to be the rank that owns the ghost node
				this->rank = mesh.cellConnGraph->nodeOwner[node];
			}

			return cupcfd::error::E_SUCCESS;
		}
		
		template <class I, class T>
		template <class M, class L> 
		cupcfd::error::eCodes ParticleSimple<I,T>::updateBoundaryFaceWall(cupcfd::geometry::mesh::UnstructuredMeshInterface<M,I,T,L>& mesh, I cellLocalID, I faceLocalID)
		{
			cupcfd::error::eCodes status;
			
			// Treat walls as reflective surfaces
			// Reflect the velocity vector
			
			// (1) Get the face and normalise it
			// Get Normal
			cupcfd::geometry::euclidean::EuclideanVector<T,3> normal = mesh.getFaceNorm(faceLocalID);
			
			// Check which direction it is facing
			I cell1ID = mesh.getFaceCell1ID(faceLocalID);
			
			// If cell1ID does not match the cell we traversed, then we are in cell 2 and the normal is facing into the cell
			// Make sure it faces inwards
			if(cell1ID == cellLocalID)
			{
				normal = T(-1) * normal;
			}
			
			// Normalise it
			status = normal.normalise();
			if (status != cupcfd::error::E_SUCCESS) {
				std::cout << "ERROR: normalise() failed" << std::endl;
				return status;
			}
				
			// (2) Mirror the velocity
			this->velocity = this->velocity - (2 * (this->velocity.dotProduct(normal)) * normal);
			
			// (3) Mirror other properties that might be specific to this particle specialisation
			this->acceleration = this->acceleration - (2 * (this->acceleration.dotProduct(normal)) * normal);
			this->jerk = this->jerk - (2 * (this->jerk.dotProduct(normal)) * normal);
			
			// Since we reflect, we do not change cell or rank

			return cupcfd::error::E_SUCCESS;
		}
																		  
		template <class I, class T>
		template <class M, class L> 
		cupcfd::error::eCodes ParticleSimple<I,T>::updateBoundaryFaceSymp(cupcfd::geometry::mesh::UnstructuredMeshInterface<M,I,T,L>& mesh, I cellLocalID, I faceLocalID)
		{
			// ToDo: This isn't correct behaviour typically, but for now we will just treat these the same as walls
			return this->updateBoundaryFaceWall(mesh, cellLocalID, faceLocalID);
		}																		  
																		  
		template <class I, class T>
		template <class M, class L> 
		cupcfd::error::eCodes ParticleSimple<I,T>::updateBoundaryFaceInlet(cupcfd::geometry::mesh::UnstructuredMeshInterface<M,I,T,L>& mesh, I cellLocalID, I faceLocalID)
		{
			// ToDo: This isn't correct behaviour typically, but for now we will just treat these the same as walls 
			return this->updateBoundaryFaceWall(mesh, cellLocalID, faceLocalID);
		}
		
		template <class I, class T>
		template <class M, class L> 
		cupcfd::error::eCodes ParticleSimple<I,T>::updateBoundaryFaceOutlet(cupcfd::geometry::mesh::UnstructuredMeshInterface<M,I,T,L>& mesh, I cellLocalID, I faceLocalID)
		{
			// ToDo: This isn't correct behaviour typically, but for now we will just treat these the same as walls
			return this->updateBoundaryFaceWall(mesh, cellLocalID, faceLocalID); 
		}	
		
		
		template <class I, class T>
		inline cupcfd::error::eCodes ParticleSimple<I,T>::getMPIType(MPI_Datatype * dType)
		{
			if(!(this->isRegistered()))
			{
				return cupcfd::error::E_MPI_DATATYPE_UNREGISTERED;
			}
		
			*dType = ParticleSimple<I,T>::mpiType;

			return cupcfd::error::E_SUCCESS;
		}
		
		template <class I, class T>
		inline MPI_Datatype ParticleSimple<I,T>::getMPIType()
		{
			return ParticleSimple<I,T>::mpiType;
		}
		
		template <class I, class T>
		inline bool ParticleSimple<I,T>::isRegistered()
		{
			return ParticleSimple<I,T>::mpiDataTypeReg;
		}
		
		template <class I, class T>
		cupcfd::error::eCodes ParticleSimple<I,T>::registerMPIType()
		{
			cupcfd::error::eCodes status;
			
			// Error Check - Only Register if currently unregistered
			if(this->isRegistered())
			{
				return cupcfd::error::E_MPI_DATATYPE_REGISTERED;
			}

			int mpiErr;

			// Only need one block since all of same type
			int count = 1;

			// Keep as blocks of size 1 incase of compiler rearranging members
			int blocklengths[11] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, PARTICLE_PADDING};
			MPI_Aint displ[11];
			MPI_Datatype structTypes[11];

			// === Set types ===
			MPI_Datatype componentType;
						
			// Block 1: Vectors
			status = cupcfd::comm::mpi::getMPIType(this->pos, &componentType);
			if (status != cupcfd::error::E_SUCCESS) return status;
			structTypes[0] = componentType;

			status = cupcfd::comm::mpi::getMPIType(this->inflightPos, &componentType);
			if (status != cupcfd::error::E_SUCCESS) return status;
			structTypes[1] = componentType;
			
			// Block 2: Points
			status = cupcfd::comm::mpi::getMPIType(this->velocity, &componentType);
			if (status != cupcfd::error::E_SUCCESS) return status;
			structTypes[2] = componentType;

			status = cupcfd::comm::mpi::getMPIType(this->acceleration, &componentType);
			if (status != cupcfd::error::E_SUCCESS) return status;
			structTypes[3] = componentType;

			status = cupcfd::comm::mpi::getMPIType(this->velocity, &componentType);
			if (status != cupcfd::error::E_SUCCESS) return status;
			structTypes[4] = componentType;
			
			// Block 3: Type I
			status = cupcfd::comm::mpi::getMPIType(this->cellGlobalID, &componentType);
			if (status != cupcfd::error::E_SUCCESS) return status;
			structTypes[5] = componentType;

			status = cupcfd::comm::mpi::getMPIType(this->rank, &componentType);
			if (status != cupcfd::error::E_SUCCESS) return status;
			structTypes[6] = componentType;
			
			// Block 4: Type T
			status = cupcfd::comm::mpi::getMPIType(this->travelDt, &componentType);
			if (status != cupcfd::error::E_SUCCESS) return status;
			structTypes[7] = componentType;

			status = cupcfd::comm::mpi::getMPIType(this->decayLevel, &componentType);
			if (status != cupcfd::error::E_SUCCESS) return status;
			structTypes[8] = componentType;

			status = cupcfd::comm::mpi::getMPIType(this->decayRate, &componentType);
			if (status != cupcfd::error::E_SUCCESS) return status;
			structTypes[9] = componentType;
			
			// Block 5: Booleans (Padding)
			status = cupcfd::comm::mpi::getMPIType(this->padding, &componentType);
			if (status != cupcfd::error::E_SUCCESS) return status;
			structTypes[10] = componentType;			
			
			// Displacements
			displ[0] = (MPI_Aint) offsetof(class ParticleSimple, pos);
			displ[1] = (MPI_Aint) offsetof(class ParticleSimple, inflightPos);
			displ[2] = (MPI_Aint) offsetof(class ParticleSimple, velocity);
			displ[3] = (MPI_Aint) offsetof(class ParticleSimple, acceleration);		
			displ[4] = (MPI_Aint) offsetof(class ParticleSimple, jerk);	
			displ[5] = (MPI_Aint) offsetof(class ParticleSimple, cellGlobalID);
			displ[6] = (MPI_Aint) offsetof(class ParticleSimple, rank);	
			displ[7] = (MPI_Aint) offsetof(class ParticleSimple, travelDt);	
			displ[8] = (MPI_Aint) offsetof(class ParticleSimple, decayLevel);	
			displ[9] = (MPI_Aint) offsetof(class ParticleSimple, decayRate);	
			displ[10] = (MPI_Aint) offsetof(class ParticleSimple, padding);				
			
			
			MPI_Datatype vecType;
			MPI_Datatype vecTypeResized;

			mpiErr = MPI_Type_create_struct(11, blocklengths, displ, structTypes, &vecType);

			if(mpiErr != MPI_SUCCESS)
			{
				return cupcfd::error::E_MPI_ERR;
			}

			mpiErr = MPI_Type_commit(&vecType);

			if(mpiErr != MPI_SUCCESS)
			{
				return cupcfd::error::E_MPI_ERR;
			}

			mpiErr = MPI_Type_create_resized(vecType, displ[0], (MPI_Aint) sizeof(class ParticleSimple<I,T>), &vecTypeResized);
			if(mpiErr != MPI_SUCCESS)
			{
				return cupcfd::error::E_MPI_ERR;
			}

			mpiErr = MPI_Type_commit(&vecTypeResized);
			if(mpiErr != MPI_SUCCESS)
			{
				return cupcfd::error::E_MPI_ERR;
			}

			mpiErr = MPI_Type_commit(&vecTypeResized);
			if(mpiErr != MPI_SUCCESS)
			{
				return cupcfd::error::E_MPI_ERR;
			}

			// Store statically so the get method can retrieve it later
			ParticleSimple<I,T>::mpiType = vecTypeResized;

			// Cleanup - Don't need the unresized type
			mpiErr = MPI_Type_free(&vecType);
			if(mpiErr != MPI_SUCCESS)
			{
				return cupcfd::error::E_MPI_ERR;
			}

			ParticleSimple<I,T>::mpiDataTypeReg = true;

			return cupcfd::error::E_SUCCESS;
		}

		template <class I, class T>
		cupcfd::error::eCodes ParticleSimple<I,T>::deregisterMPIType()
		{
			int mpiErr;

			// Error Check - Only Deregister if currently registered
			if(!this->isRegistered())
			{
				return cupcfd::error::E_MPI_DATATYPE_UNREGISTERED;
			}

			mpiErr = MPI_Type_free(&(ParticleSimple<I,T>::mpiType));
			if(mpiErr != MPI_SUCCESS)
			{
				return cupcfd::error::E_MPI_ERR;
			}

			ParticleSimple<I,T>::mpiDataTypeReg = false;
			return cupcfd::error::E_SUCCESS;
		}
	}
}

#endif
