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
 * Contains Header Level Definitions for the Particle Class
 */

#ifndef CUPCFD_PARTICLES_PARTICLE_IPP_H
#define CUPCFD_PARTICLES_PARTICLE_IPP_H

#include <iostream>
#include <unistd.h>

namespace cupcfd
{
	namespace particles
	{		
		template <class P, class I, class T>
		Particle<P, I, T>::Particle()
		:CustomMPIType(),
		 cellGlobalID(-1),
		 lastCellMoveGlobalID(-1),
		 rank(-1),
		 travelDt(T(0))
		{
		
		}
	
		template <class P, class I, class T>
		Particle<P, I, T>::Particle(cupcfd::geometry::euclidean::EuclideanVector<T,3> velocity,
						 			cupcfd::geometry::euclidean::EuclideanPoint<T,3> pos,
						 			I cellGlobalID, I rank, T travelDt)
		:CustomMPIType(),
		 velocity(velocity),
		 pos(pos),
		 inflightPos(pos),
		 cellGlobalID(cellGlobalID),
		 lastCellMoveGlobalID(cellGlobalID),
		 rank(rank),
		 travelDt(travelDt)
		{

		}

		template <class P, class I, class T>
		Particle<P, I, T>::~Particle()
		{
		
		}

		template <class P, class I, class T>
		inline void Particle<P, I, T>::operator=(Particle& source)
		{
			static_cast<P*>(this)->operator=(source);
		}

		template <class P, class I, class T>
		inline void Particle<P, I, T>::setPos(cupcfd::geometry::euclidean::EuclideanPoint<T,3>& pos)
		{
			this->pos = pos;
		}

		template <class P, class I, class T>
		inline cupcfd::geometry::euclidean::EuclideanPoint<T,3> Particle<P, I, T>::getPos()
		{
			return this->pos;
		}

		template <class P, class I, class T>
		inline void Particle<P, I, T>::setInFlightPos(cupcfd::geometry::euclidean::EuclideanPoint<T,3>& inflightPos)
		{
			this->inflightPos = inflightPos;
		}

		template <class P, class I, class T>
		inline cupcfd::geometry::euclidean::EuclideanPoint<T,3> Particle<P, I, T>::getInFlightPos()
		{
			return this->inflightPos;
		}

		template <class P, class I, class T>
		inline void Particle<P, I, T>::setVelocity(cupcfd::geometry::euclidean::EuclideanVector<T,3>& velocity)
		{
			this->velocity = velocity;
		}

		template <class P, class I, class T>
		inline cupcfd::geometry::euclidean::EuclideanVector<T,3> Particle<P, I, T>::getVelocity()
		{
			return this->velocity;
		}

		template <class P, class I, class T>
		inline void Particle<P, I, T>::setCellGlobalID(I cellGlobalID)
		{
			// std::cout << "Request received to move particle ID " << this->particleID << " from cell " << this->cellGlobalID << " --> cell " << cellGlobalID << std::endl;
			// usleep(100*1000);

			if (this->cellGlobalID != this->lastCellMoveGlobalID) {
				std::cout << "ERROR: Attempting to update a cell of particle " << this->particleID << ", but its cellGlobalID=" << this->cellGlobalID << " != lastCellMoveGlobalID=" << this->lastCellMoveGlobalID << std::endl;
				throw std::exception();
			}
			if (this->cellGlobalID == cellGlobalID) {
				// std::cout << "ERROR: Attempting to update a cell of particle " << this->particleID << " to " << cellGlobalID << " but it is already in that cell" << std::endl;
				std::cout << "ERROR: Attempting to update a particle " << this->particleID << " to be in cell " << cellGlobalID << " but it is already in that cell" << std::endl;
				throw std::exception();
			}

			// // if (this->particleID == 8601) {
			// 	// std::cout << "Particle ID " << this->particleID << " being moved from cell " << this->cellGlobalID << " --> cell " << cellGlobalID << std::endl;
			// 	std::cout << "> request granted" << std::endl;
			// 	usleep(100*1000);
			// // }
			this->cellGlobalID = cellGlobalID;
			this->lastCellMoveGlobalID = cellGlobalID;
		}

		template <class P, class I, class T>
		inline I Particle<P, I, T>::getCellGlobalID() const
		{
			return this->cellGlobalID;
		}

		// template <class P, class I, class T>
		// inline I Particle<P, I, T>::getRank() const
		// {
		// 	return this->rank;
		// }

		// template <class P, class I, class T>
		// inline I Particle<P, I, T>::getParticleID() const
		// {
		// 	return this->particleID;
		// }

		template <class P, class I, class T>
		inline void Particle<P, I, T>::setTravelTime(T dt)
		{
			this->travelDt = dt;
		}

		template <class P, class I, class T>
		inline T Particle<P, I, T>::getTravelTime() const
		{
			return this->travelDt;
		}
		
		// template <class P, class I, class T>
		// inline void Particle<P, I, T>::setInactive()
		// {
		// 	static_cast<P*>(this)->setInactive(inactive);
		// }
		
		template <class P, class I, class T>
		inline bool Particle<P, I, T>::getInactive() const
		{
			return static_cast<P*>(this)->getInactive();
		}
		

		template <class P, class I, class T>
		template <class M, class L>
		cupcfd::error::eCodes Particle<P, I, T>::updatePositionAtomic(cupcfd::geometry::mesh::UnstructuredMeshInterface<M,I,T,L>& mesh, T * dt, I * exitFaceLocalID, bool print_info)
		{
			// ToDo: Warning: I'm not certain how this code currently handles intersection points directly through vertices well (assigns first face it finds?)
		
			cupcfd::error::eCodes status;
		
			// Note: We are treating velocity as if it cannot change within one atomic traversal of a cell.
			// Velocity can be updated via the updateVelocityAtomic function, but more fine-grained applications of acceleration
			// would need a fined-grained mesh.
		
			// ToDo: Error Check - Particle cannot advance if it has reached a rank transition (i.e. the current rank
			// does not match the particle rank because it needs to be transferred to that rank)
		
			// Check - if the particle has no remaining travel time, then don't change anything
			if(arth::isEqual(this->getTravelTime(), T(0)))
			{				
				*exitFaceLocalID = -1;
				*dt = T(0);
				if (print_info) {
					std::cout << "  > > > no travel time left" << std::endl;
				}
				return cupcfd::error::E_SUCCESS;
			}
		
			// Get Cell Local ID - ToDo: Could store this inside cell - storage overhead vs graph lookup overhead
			I node = mesh.cellConnGraph->globalToNode[this->cellGlobalID];
			I localCellID;
			status = mesh.cellConnGraph->connGraph.getNodeLocalIndex(node, &localCellID);
			if (status != cupcfd::error::E_SUCCESS) {
				std::cout << "ERROR: getNodeLocalIndex() failed" << std::endl;
				return status;
			}

			// Identify which face the exiting vector intersects with
			
			I nFaces;
			status = mesh.getCellNFaces(localCellID, &nFaces);
			if (status != cupcfd::error::E_SUCCESS) {
				std::cout << "ERROR: getCellNFaces() failed" << std::endl;
				return status;
			}

			// Store the local ID of the face we are exiting by
			I exitFaceID = -1;
			
			// Store the travel time to the exiting face
			T travelTime;
			
			// Store the intersection point of that face
			cupcfd::geometry::euclidean::EuclideanPoint<T,3> intersection;
			cupcfd::geometry::euclidean::EuclideanPoint<T,3> exitIntersection;
			T exitTravelTime;

			I intersectionCount = 0;

			// Loop over the faces of the cell
			bool face_was_found = false;
			// if (print_info) {
			// 	std::cout << "  > > > searching for exit face" << std::endl;
			// }
			for(I i = 0; i < nFaces; i++)
			{
				// Retrieve the faces of the cell
				I localFaceID = mesh.getCellFaceID(localCellID, i);
				
				// Break the faces up into triangles and compute the intersection with the plane of each triangle and determine the time to reach
				// Get the vertices/spatial coordinates for each face
				I nFaceVertices = mesh.getFaceNVertices(localFaceID);
				
				// Get the first vertex
				I faceVertex0ID = mesh.getFaceVertex(localFaceID, 0);
				I faceVertex1ID;
				I faceVertex2ID;
				
				// Loop over the triangles of the face
				for(I j = 0; j < (nFaceVertices-2); j++)
				{
					faceVertex1ID = mesh.getFaceVertex(localFaceID, j+1);
					faceVertex2ID = mesh.getFaceVertex(localFaceID, j+2);
				
					// Test for intersection on the triangle formed by the plane with the travel vector
					cupcfd::geometry::euclidean::EuclideanPlane3D<T> plane(mesh.getVertexPos(faceVertex0ID), 
																				mesh.getVertexPos(faceVertex1ID), 
																				mesh.getVertexPos(faceVertex2ID));
					
					status = plane.linePlaneIntersection(this->velocity, this->inflightPos, intersection);
					
					// Verify that the travel vector (velocity) is not parallel to or fully in the face.
					if(status == cupcfd::error::E_EUC_VEC_PARALLEL)
					{
						// Either will never intersect, or is intersecting at all times and not exiting into the
						// neighbouring cell via this face.
						
						// Skip any actions for this face - will not intersect						
					}
					else
					{
						// Verify that the intersection point occurs within the triangle of the face, rather than just the plane
						// Could also use static method directly rather than create an object
						cupcfd::geometry::shapes::Triangle3D<T> shape(mesh.getVertexPos(faceVertex0ID), 
																	  mesh.getVertexPos(faceVertex1ID), 
																	  mesh.getVertexPos(faceVertex2ID));
							   
						if(!shape.isPointInside(intersection))
						{							
							// Progress onto next face instead
							continue;
						}
						face_was_found = true;
							
						intersectionCount = intersectionCount + 1;
															
						// Intersection point is inside a triangular face on this shape - Find the time it takes to intersect
						// Inflight pos -> intersection is a scalar multiple of the velocity vector from Inflightpos
						// Could use time = distance/speed, but we lose direction (don't want to travel in direction
						// of negative time)
						// We know from inflight pos goes through intersection along vector, so just compute scalar for time
						// ToDo: Probably just a more elegant way of doing this (really we just want negative distance or speed)
						
						// Distance
						T distance;
						status = (intersection - this->inflightPos).length(&distance);
						if (status != cupcfd::error::E_SUCCESS) {
							std::cout << "ERROR: length() failed" << std::endl;
							return status;
						}
						
						// Speed
						T speed;
						status = this->velocity.length(&speed);
						if (status != cupcfd::error::E_SUCCESS) {
							std::cout << "ERROR: length() failed" << std::endl;
							return status;
						}
						
						// Time
						T travelTime = distance / speed;

						if(this->inflightPos + (this->velocity * travelTime) != intersection)
						{
							// Travelling in wrong direction to velocity, set time to negative
							travelTime = T(-1) * travelTime;
						}
						
						// Theoretically, if the time/distance is not zero (i.e. we aren't checking the face of the cell we
						// just left) and the cell is not concave (the cell type for which we don't
						// currently support since it needs to be greater than a hexahedron), then if it is inside the triangle
						// this is the only face we can exit by?
						
						// ToDo: Think there might be a danger of 'bouncing' back and forth if exactly on a vertex - check 
						
						if((!arth::isEqual(travelTime, 0.0)) && travelTime > 0.0)
						{
							// if (print_info) {
							// 	std::cout << "  > > > > found exit face" << std::endl;
							// }
							// Exiting the cell via this face if time permits			
							exitFaceID = localFaceID;
							exitIntersection = intersection;
							exitTravelTime = travelTime;							
							break;
						}
						
						// We can only exit via one face - if it is not set yet and the travel time is 0
						// we are currently sitting on a face/vertex etc.
						// Set it as a fallback - if we exit via no other face then the face we are sitting on
						// must be the one we leave by, else it will be overwritten by the correct face (and
						// then the loop is exited via a break)
												
						if(arth::isEqual(travelTime, 0.0) && (exitFaceID == -1))
						{
							// if (print_info) {
							// 	std::cout << "> did not find exit face, using current as fallback" << std::endl;
							// }
							exitFaceID = localFaceID;
							exitIntersection = intersection;
							exitTravelTime = 0.0;
						}
					}
				}
			}
			if (!face_was_found) {
				std::cout << "ERROR: Failed to find face that particle will intersect" << std::endl;
				return cupcfd::error::E_ERROR;
			}
			
			// Theoretically should either have to leave via one of the faces or stay inside the cell, assuming
			// it is a closed polyhedron. Therefore, at this point there should be at least a travelTime,
			// and potentially a exitFaceID.
			// If there is not, then it is possible an invalid cell ID is set for the particle
			// ToDo: Error Check for this?
			
			if(intersectionCount == 0)
			{			
				// Error - No Face was found for exiting (assuming there would be travel time to reach it)
				// This would suggest the particle is in the wrong cell for its position
				return cupcfd::error::E_ERROR;
			}

			// Verify with the travel time remaining whether it will actually exit the cell
			// (a) Does not exit cell
			if(exitTravelTime > this->travelDt)
			{
				if (print_info) {
					std::cout << "  > > > does not exit cell" << std::endl;
				}
				// Not enough travel time on the particle left to reach face
				// Particle does not exit cell
				*exitFaceLocalID = -1;
				*dt = this->travelDt;
				
				// Update the particle position to within this cell, and set remaining travel time to 0
				this->inflightPos = this->inflightPos + (this->velocity * this->travelDt);				
				this->pos = this->inflightPos;
				this->travelDt = 0.0;
				return cupcfd::error::E_SUCCESS;
			}
			else
			{
				// if (print_info) {
				// 	// std::cout << "  > > > does exit cell, or stops on face" << std::endl;
				// 	std::cout << "  > > > does exit cell after " << exitTravelTime << " seconds" << std::endl;
				// }

				// Stops either exactly on or exits via face.

				// std::cout << "  particle exiting on/through face" << std::endl;
				// for(I i = 0; i < nFaces; i++)
				// {
				// 	I localFaceID = mesh.getCellFaceID(localCellID, i);
				// 	I nFaceVertices = mesh.getFaceNVertices(localFaceID);
				// 	I faceVertex0ID = mesh.getFaceVertex(localFaceID, 0);
				// 	I faceVertex1ID, faceVertex2ID;
				// 	for(I j = 0; j < (nFaceVertices-2); j++)
				// 	{
				// 		faceVertex1ID = mesh.getFaceVertex(localFaceID, j+1);
				// 		faceVertex2ID = mesh.getFaceVertex(localFaceID, j+2);
					
				// 		auto v0 = mesh.getVertexPos(faceVertex0ID);
				// 		auto v1 = mesh.getVertexPos(faceVertex1ID);
				// 		auto v2 = mesh.getVertexPos(faceVertex2ID);

				// 		if (intersection == v0 || intersection == v1 || intersection == v2) {
				// 			std::cout << "  ... and through a face vertex!" << std::endl;
				// 		}
				// 	}
				// }

				// In either scenario, we move the particle to the face (where it can be assigned to the next cell when its state is updated)
				*exitFaceLocalID = exitFaceID;
				*dt = exitTravelTime;
								
				// Intersection was set when we set the exitFaceID
				this->inflightPos = exitIntersection;
					
				// Can reuse last computed distance/speed/travel time since the prior loop exited on the valid value
				this->travelDt = this->travelDt - exitTravelTime;

				// Boundary conditions will be handed by other functions
				
				return cupcfd::error::E_SUCCESS;
			}			
		}

		// template <class P, class I, class T>
		// template <class M, class L>
		// cupcfd::error::eCodes Particle<P, I, T>::updateVelocityAtomic(cupcfd::geometry::mesh::UnstructuredMeshInterface<M,I,T,L>& mesh, I cellLocalID, T dt)
		// {
		// 	static_cast<P*>(this)->updateVelocityAtomic(mesh, cellLocalID, faceLocalID);
		// }
		
		// template <class P, class I, class T>
		// template <class M, class L>
		// cupcfd::error::eCodes Particle<P, I, T>::updateStateAtomic(cupcfd::geometry::mesh::UnstructuredMeshInterface<M,I,T,L>& mesh, I cellLocalID, T dt)
		// {
		// 	static_cast<P*>(this)->updateStateAtomic(mesh, cellLocalID, faceLocalID);
		// }														    

		// template <class P, class I, class T>
		// template <class M, class L> 
		// cupcfd::error::eCodes Particle<P, I, T>::updateNonBoundaryFace(cupcfd::geometry::mesh::UnstructuredMeshInterface<M,I,T,L>& mesh, I faceLocalID)
		// {
		// 	static_cast<P*>(this)->updateBoundaryFaceWall(mesh, cellLocalID, faceLocalID);
		// }
		
		template <class P, class I, class T>
		template <class M, class L> 
		cupcfd::error::eCodes Particle<P, I, T>::updateBoundaryFaceWall(cupcfd::geometry::mesh::UnstructuredMeshInterface<M,I,T,L>& mesh, I cellLocalID, I faceLocalID)
		{
			static_cast<P*>(this)->updateBoundaryFaceWall(mesh, cellLocalID, faceLocalID);
		}
																		  
		template <class P, class I, class T>
		template <class M, class L> 
		cupcfd::error::eCodes Particle<P, I, T>::updateBoundaryFaceSymp(cupcfd::geometry::mesh::UnstructuredMeshInterface<M,I,T,L>& mesh, I cellLocalID, I faceLocalID)
		{
			static_cast<P*>(this)->updateBoundaryFaceSymp(mesh, cellLocalID, faceLocalID);
		}																		  
																		  
		template <class P, class I, class T>
		template <class M, class L> 
		cupcfd::error::eCodes Particle<P, I, T>::updateBoundaryFaceInlet(cupcfd::geometry::mesh::UnstructuredMeshInterface<M,I,T,L>& mesh, I cellLocalID, I faceLocalID)
		{
			static_cast<P*>(this)->updateBoundaryFaceInlet(mesh, cellLocalID, faceLocalID);
		}
		
		template <class P, class I, class T>
		template <class M, class L> 
		cupcfd::error::eCodes Particle<P, I, T>::updateBoundaryFaceOutlet(cupcfd::geometry::mesh::UnstructuredMeshInterface<M,I,T,L>& mesh, I cellLocalID, I faceLocalID)
		{
			static_cast<P*>(this)->updateBoundaryFaceOutlet(mesh, cellLocalID, faceLocalID);
		}	
																						
		template <class P, class I, class T>
		inline cupcfd::error::eCodes Particle<P, I, T>::getMPIType(MPI_Datatype * dType)
		{
			return static_cast<P*>(this)->getMPIType(dType);
		}
		
		template <class P, class I, class T>
		inline MPI_Datatype Particle<P, I, T>::getMPIType()
		{
			return static_cast<P*>(this)->getMPIType();
		}
		
		template <class P, class I, class T>
		cupcfd::error::eCodes Particle<P, I, T>::registerMPIType()
		{
			return static_cast<P*>(this)->registerMPIType();
		}
		
		template <class P, class I, class T>
		cupcfd::error::eCodes Particle<P, I, T>::deregisterMPIType()
		{
			return static_cast<P*>(this)->deregisterMPIType();
		}
		
		template <class P, class I, class T>
		inline bool Particle<P, I, T>::isRegistered()
		{
			return static_cast<P*>(this)->isRegistered();
		}
	}
}

#endif
