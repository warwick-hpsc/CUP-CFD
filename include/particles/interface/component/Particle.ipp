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
		 particleID(-1),
		 cellGlobalID(-1),
		 cellEntryFaceLocalID(-1),
		 lastCellGlobalID(-1),
		 lastLastCellGlobalID(-1),
		 rank(-1),
		 lastRank(-1),
		 travelDt(T(0))
		{
		
		}
	
		template <class P, class I, class T>
		Particle<P, I, T>::Particle(cupcfd::geometry::euclidean::EuclideanVector<T,3> velocity,
						 			cupcfd::geometry::euclidean::EuclideanPoint<T,3> pos,
						 			I id, 
						 			I cellGlobalID, I rank, T travelDt)
		:CustomMPIType(),
		 velocity(velocity),
		 pos(pos),
		 inflightPos(pos),
		 particleID(id),
		 cellGlobalID(cellGlobalID),
		 cellEntryFaceLocalID(-1),
		 lastCellGlobalID(-1),
		 lastLastCellGlobalID(-1),
		 rank(rank),
		 lastRank(-1),
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
		inline I Particle<P, I, T>::getRank() {
			return this->rank;
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
		inline cupcfd::error::eCodes Particle<P, I, T>::safelySetCellGlobalID(I cellGlobalID, I cellEntryFaceLocalID)
		{
			if (this->cellGlobalID == cellGlobalID) {
				std::cout << "ERROR: Attempting to update a particle " << this->particleID << " to be in cell " << cellGlobalID << " but it is already in that cell" << std::endl;
				return cupcfd::error::E_ERROR;
			}

			if ( (cellGlobalID == this->lastLastCellGlobalID) || (cellGlobalID == this->lastCellGlobalID) ) {
				std::cout << "ERROR: Attempting to move particle " << this->particleID << " to cell " << cellGlobalID << " but it was there recently (recent history is " << this->lastLastCellGlobalID << " -> " << this->lastCellGlobalID << " -> " << this->cellGlobalID << ")" << std::endl;
				return cupcfd::error::E_ERROR;
			}

			this->lastLastCellGlobalID = this->lastCellGlobalID;
			this->lastCellGlobalID = this->cellGlobalID;
			this->cellGlobalID = cellGlobalID;

			if (cellEntryFaceLocalID == I(-1)) {
				std::cout << "ERROR: Particle::safelySetCellGlobalID() called with invalid value of 'cellEntryFaceLocalID'" << std::endl;
				return cupcfd::error::E_ERROR;
			}
			this->cellEntryFaceLocalID = cellEntryFaceLocalID;

			return cupcfd::error::E_SUCCESS;
		}

		template <class P, class I, class T>
		inline I Particle<P, I, T>::getCellGlobalID() const
		{
			return this->cellGlobalID;
		}

		template <class P, class I, class T>
		inline I Particle<P, I, T>::getLastCellGlobalID() const
		{
			return this->lastCellGlobalID;
		}

		template <class P, class I, class T>
		inline I Particle<P, I, T>::getCellEntryFaceLocalID() const
		{
			return this->cellEntryFaceLocalID;
		}

		template <class P, class I, class T>
		inline I Particle<P, I, T>::getParticleID() const
		{
			return this->particleID;
		}

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
		
		template <class P, class I, class T>
		inline bool Particle<P, I, T>::getInactive() const
		{
			return static_cast<P*>(this)->getInactive();
		}

		template <class P, class I, class T>
		inline bool Particle<P, I, T>::stateValid() const
		{
			if ((this->lastCellGlobalID != I(-1)) && (this->cellEntryFaceLocalID == I(-1))) {
				std::cout << "ERROR: particle " << this->particleID << " has history of cell movement but cellEntryFaceLocalID is -1" << std::endl;
				return false;
			}

			return true;
		}

		template <class P, class I, class T>
		inline void Particle<P, I, T>::print() const
		{
			std::cout << "  > Particle " << this->particleID << " state:" << std::endl;
			if (this->cellEntryFaceLocalID != -1) {
				std::cout << "    > Cell ID: " << cellGlobalID << " , entered through local face ID " << this->cellEntryFaceLocalID << std::endl;
			} else{
				std::cout << "    > Cell ID: " << cellGlobalID << std::endl;
			}
			std::cout << "    > POS: "; this->pos.print(); std::cout << std::endl;
			std::cout << "    > In-flight POS: "; this->inflightPos.print(); std::cout << std::endl;
			std::cout << "    > Velocity: "; this->velocity.print(); std::cout << std::endl;
			std::cout << "    > Travel time left: " << this->travelDt << std::endl;
			std::cout << "    > Cell movement history: " << this->lastLastCellGlobalID << " -> " << this->lastCellGlobalID << " -> " << this->cellGlobalID << std::endl;
			if (this->lastRank != -1) {
				std::cout << "    > Rank history: " << this->lastRank << " -> " << this->rank << std::endl;
			}
		}

		
		template <class P, class I, class T>
		template <class M, class L>
		cupcfd::error::eCodes Particle<P, I, T>::calculateFaceIntersection(cupcfd::geometry::mesh::UnstructuredMeshInterface<M,I,T,L>& mesh,
																			I faceID, 
																			bool verbose,
																			bool& doesIntersect, 
																			cupcfd::geometry::euclidean::EuclideanPoint<T,3>& intersection, 
																			bool& intersectionOnEdge,
																			T& timeToIntersect)
		{
			// Ensure false by default:
			doesIntersect = false;
			intersectionOnEdge = false;
			timeToIntersect = T(-1);

			cupcfd::geometry::euclidean::EuclideanPoint<T,3> v0 = this->inflightPos;
			cupcfd::geometry::euclidean::EuclideanVector<T,3> velocity = this->velocity;

			// Break the faces up into triangles and compute the intersection with the plane of each triangle and determine the time to reach
			// Get the vertices/spatial coordinates for each face
			I nFaceVertices = mesh.getFaceNVertices(faceID);

			// Counters to ensure that face is being split into non-overlapping triangles:
			I num_faces_contacting_particle_on_edge = I(0);
			I num_faces_contacting_particle_within_tri = I(0);

			I faceVertex0ID = mesh.getFaceVertex(faceID, 0);
			for(I j = 1; j < (nFaceVertices-1); j++)
			{
				I faceVertex1ID = mesh.getFaceVertex(faceID, j);
				I faceVertex2ID = mesh.getFaceVertex(faceID, j+1);

				cupcfd::geometry::euclidean::EuclideanPoint<T,3> faceVertex0 = mesh.getVertexPos(faceVertex0ID);
				cupcfd::geometry::euclidean::EuclideanPoint<T,3> faceVertex1 = mesh.getVertexPos(faceVertex1ID);
				cupcfd::geometry::euclidean::EuclideanPoint<T,3> faceVertex2 = mesh.getVertexPos(faceVertex2ID);

				cupcfd::geometry::euclidean::EuclideanPlane3D<T> plane(faceVertex0, faceVertex1, faceVertex2);

				if (j==1) {
					// On first triangle, check if velocity is parallel to face:
					if (plane.isVectorParallel(this->velocity)) {
						return cupcfd::error::E_SUCCESS;
					}
				}

				cupcfd::geometry::shapes::Triangle3D<T> shape(mesh.getVertexPos(faceVertex0ID), 
															  mesh.getVertexPos(faceVertex1ID), 
															  mesh.getVertexPos(faceVertex2ID));

				bool doesIntersectTriangle;
				bool doesIntersectTriangleOnEdge;
				cupcfd::geometry::euclidean::EuclideanPoint<T,3> triIntersection;
				T timeToIntersectTriangle;
				doesIntersectTriangle = shape.calculateIntersection(v0, velocity, triIntersection, timeToIntersectTriangle, doesIntersectTriangleOnEdge, verbose);
				
				if (doesIntersectTriangle) {
					if (doesIntersect) {
						std::cout << "ERROR: calculateFaceIntersection() has detected multiple face intersections for particle " << this->particleID << std::endl;
						return cupcfd::error::E_ERROR;
					}

					if (timeToIntersect == T(0.0)) {
						if (doesIntersectTriangleOnEdge) {
							num_faces_contacting_particle_on_edge++;
						} else {
							num_faces_contacting_particle_within_tri++;
						}
					}
					
					doesIntersect = true;
					intersection = triIntersection;
					intersectionOnEdge = doesIntersectTriangleOnEdge;
					timeToIntersect = timeToIntersectTriangle;
				}
			}

			if (num_faces_contacting_particle_within_tri > 1) {
				std::cout << "ERROR: Particle " << this->particleID << " of cell " << this->cellGlobalID << " is directly resting on " << num_faces_contacting_particle_within_tri << " triangles" << std::endl;
				std::cout << "       Only " << num_faces_contacting_particle_on_edge << " of these have the particle on a triangle edge, indicating that triangles are overlapping" << std::endl;
				return cupcfd::error::E_ERROR;
			}

			return cupcfd::error::E_SUCCESS;
		}

		template <class P, class I, class T>
		template <class M, class L>
		cupcfd::error::eCodes Particle<P, I, T>::updatePositionAtomic(cupcfd::geometry::mesh::UnstructuredMeshInterface<M,I,T,L>& mesh, T * dt, I * exitFaceLocalID, bool verbose)
		{
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
				if (verbose) {
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

			// ****************************************************** //
			// Identify which face the exiting vector intersects with //
			// ****************************************************** //
			I nFaces;
			status = mesh.getCellNFaces(localCellID, &nFaces);
			if (status != cupcfd::error::E_SUCCESS) {
				std::cout << "ERROR: getCellNFaces() failed" << std::endl;
				return status;
			}

			// Store the local ID of the face we are exiting by
			I exitFaceID = -1;
			
			// Store the intersection point of that face
			cupcfd::geometry::euclidean::EuclideanPoint<T,3> exitIntersection;
			T exitTravelTime = T(-1);
			T exitDistance= T(-1);

			I intersectionCount = 0;

			// Calculate maximum distance across cell, to provide an upper bound on 
			// valid values for distance-to-intersection:
			T max_inter_vertex_distance = T(-1);
			for(I f1 = 0; f1 < nFaces; f1++) {
				I localFaceID1 = mesh.getCellFaceID(localCellID, f1);
				I nFaceVertices1 = mesh.getFaceNVertices(localFaceID1);
				for(I j1 = 0; j1 < nFaceVertices1; j1++) {
					I faceVertexID1 = mesh.getFaceVertex(localFaceID1, j1);

					for (I f2 = 0; f2 < nFaces; f2++) {
						I localFaceID2 = mesh.getCellFaceID(localCellID, f2);
						I nFaceVertices2 = mesh.getFaceNVertices(localFaceID2);
						for(I j2 = j1+1; j2 < nFaceVertices2; j2++) {
							I faceVertexID2 = mesh.getFaceVertex(localFaceID2, j2);

							if (faceVertexID1 != faceVertexID2) {
								T face_edge_length;
								( mesh.getVertexPos(faceVertexID1) - mesh.getVertexPos(faceVertexID2) ).length(&face_edge_length);

								if (max_inter_vertex_distance == T(-1)) {
									max_inter_vertex_distance = face_edge_length;
								}
								else if (max_inter_vertex_distance < face_edge_length) {
									max_inter_vertex_distance = face_edge_length;
								}
							}
						}
					}
				}
			}

			// Loop over the faces of the cell
			bool face_was_found = false;
			I num_faces_contacting_particle_on_edge = I(0);
			I num_faces_contacting_particle_within_tri = I(0);
			for(I i = 0; i < nFaces; i++)
			{
				// Retrieve the faces of the cell
				I localFaceID = mesh.getCellFaceID(localCellID, i);

				bool doesIntersect;
				cupcfd::geometry::euclidean::EuclideanPoint<T,3> intersection;
				bool intersectionOnEdge;
				T timeToIntersect = T(-1);
				status = calculateFaceIntersection(	mesh, 
													localFaceID, 
													false, 
													doesIntersect, 
													intersection, 
													intersectionOnEdge,
													timeToIntersect);
				if (status != cupcfd::error::E_SUCCESS) {
					std::cout << "ERROR: calculateFaceIntersection() failed" << std::endl;
					return status;
				}
				if (doesIntersect) {
					if (timeToIntersect >= T(0)) {
						intersectionCount++;

						if (timeToIntersect == T(0.0)) {
							if (intersectionOnEdge) {
								num_faces_contacting_particle_on_edge++;
							} else {
								num_faces_contacting_particle_within_tri++;
							}
						}

						if (localFaceID == this->cellEntryFaceLocalID) {
							continue;
						}

						T speed;
						status = this->velocity.length(&speed);
						if (status != cupcfd::error::E_SUCCESS) {
							std::cout << "ERROR: length() failed" << std::endl;
							return status;
						}
						T distance = timeToIntersect * speed;

						exitFaceID = localFaceID;
						exitIntersection = intersection;
						exitTravelTime = timeToIntersect;
						exitDistance = distance;
						face_was_found = true;
					}
				}
			}

			if (!face_was_found) {
				std::cout << "ERROR: Failed to find face of cell " << this->cellGlobalID << " that particle " << this->particleID << "  will intersect" << std::endl;
				return cupcfd::error::E_ERROR;
			}

			if (exitTravelTime < T(0)) {
				std::cout << "ERROR: Selected exit face " << exitFaceID << " of cell " << this->cellGlobalID << " will be reached by particle " << this->particleID << " in negative time " << exitTravelTime << ", should be positive time" << std::endl;
				return cupcfd::error::E_ERROR;
			}

			if (num_faces_contacting_particle_within_tri > 1) {
				std::cout << "ERROR: Particle " << this->particleID << " of cell " << this->cellGlobalID << " is directly resting on " << num_faces_contacting_particle_within_tri << " triangles" << std::endl;
				std::cout << "       Only " << num_faces_contacting_particle_on_edge << " of these have the particle on a triangle edge, indicating that triangles are overlapping" << std::endl;
				return cupcfd::error::E_ERROR;
			}

			if (exitDistance > max_inter_vertex_distance) {
				std::cout << "ERROR: Particle " << this->particleID << " distance to selected face intersection " << exitDistance << " is greater than max inter-vertex distance " << max_inter_vertex_distance << std::endl;
				return cupcfd::error::E_ERROR;
			}

			
			// Theoretically should either have to leave via one of the faces or stay inside the cell, assuming
			// it is a closed polyhedron. Therefore, at this point there should be at least a timeToIntersect,
			// and potentially a exitFaceID.
			// If there is not, then it is possible an invalid cell ID is set for the particle
			// ToDo: Error Check for this?
			
			if(intersectionCount == 0)
			{			
				// Error - No Face was found for exiting (assuming there would be travel time to reach it)
				// This would suggest the particle is in the wrong cell for its position
				std::cout << "ERROR: No exit face found for particle " << this->particleID << std::endl;
				return cupcfd::error::E_ERROR;
			}

			// Verify with the travel time remaining whether it will actually exit the cell
			// (a) Does not exit cell
			if(exitTravelTime > this->travelDt)
			{
				if (verbose) {
					std::cout << "    > does not exit cell in this timestep, will travel for " << this->travelDt << std::endl;
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
				if (verbose) {
					std::cout << "    > exits cell in this timestep through local-face-ID " << exitFaceID << " after " << exitTravelTime << " seconds" << std::endl;
				}

				// Stops either exactly on or exits via face.
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

		template <class P, class I, class T>
		template <class M, class L> 
		cupcfd::error::eCodes Particle<P, I, T>::updateBoundaryFaceWall(cupcfd::geometry::mesh::UnstructuredMeshInterface<M,I,T,L>& mesh, I cellLocalID, I faceLocalID)
		{
			return static_cast<P*>(this)->updateBoundaryFaceWall(mesh, cellLocalID, faceLocalID);
		}
																		  
		template <class P, class I, class T>
		template <class M, class L> 
		cupcfd::error::eCodes Particle<P, I, T>::updateBoundaryFaceSymp(cupcfd::geometry::mesh::UnstructuredMeshInterface<M,I,T,L>& mesh, I cellLocalID, I faceLocalID)
		{
			return static_cast<P*>(this)->updateBoundaryFaceSymp(mesh, cellLocalID, faceLocalID);
		}																		  
																		  
		template <class P, class I, class T>
		template <class M, class L> 
		cupcfd::error::eCodes Particle<P, I, T>::updateBoundaryFaceInlet(cupcfd::geometry::mesh::UnstructuredMeshInterface<M,I,T,L>& mesh, I cellLocalID, I faceLocalID)
		{
			return static_cast<P*>(this)->updateBoundaryFaceInlet(mesh, cellLocalID, faceLocalID);
		}
		
		template <class P, class I, class T>
		template <class M, class L> 
		cupcfd::error::eCodes Particle<P, I, T>::updateBoundaryFaceOutlet(cupcfd::geometry::mesh::UnstructuredMeshInterface<M,I,T,L>& mesh, I cellLocalID, I faceLocalID)
		{
			return static_cast<P*>(this)->updateBoundaryFaceOutlet(mesh, cellLocalID, faceLocalID);
		}	

		template <class P, class I, class T>
		template <class M, class L>
		cupcfd::error::eCodes Particle<P, I, T>::redetectEntryFaceID(cupcfd::geometry::mesh::UnstructuredMeshInterface<M,I,T,L>& mesh)
		{
			cupcfd::error::eCodes status;

			bool verbose = false;
			
			if (verbose) {
				std::cout << "> Redetecting entry face ID of particle " << this->particleID << " between cells " << this->lastCellGlobalID << " -> " << this->getCellGlobalID() << std::endl;
				this->print();
			}

			I cellGlobalID = this->getCellGlobalID();
			I cellNode = mesh.cellConnGraph->globalToNode[cellGlobalID];
			I cellLocalID;
			status = mesh.cellConnGraph->connGraph.getNodeLocalIndex(cellNode, &cellLocalID);
			if (status != cupcfd::error::E_SUCCESS) {
				std::cout << "ERROR: getNodeLocalIndex() failed" << std::endl;
				return status;
			}
			I cellNumFaces;
			status = mesh.getCellNFaces(cellLocalID, &cellNumFaces);
			if (status != cupcfd::error::E_SUCCESS) {
				std::cout << "ERROR: getCellNFaces() failed" << std::endl;
				return status;
			}

			I lastCellGlobalID = this->lastCellGlobalID;
			I lastCellNode = mesh.cellConnGraph->globalToNode[lastCellGlobalID];
			I lastCellLocalID;
			status = mesh.cellConnGraph->connGraph.getNodeLocalIndex(lastCellNode, &lastCellLocalID);
			if (status != cupcfd::error::E_SUCCESS) {
				std::cout << "ERROR: getNodeLocalIndex() failed" << std::endl;
				return status;
			}
			I lastCellNumFaces;
			status = mesh.getCellNFaces(lastCellLocalID, &lastCellNumFaces);
			if (status != cupcfd::error::E_SUCCESS) {
				std::cout << "ERROR: getCellNFaces() failed" << std::endl;
				return status;
			}

			I entryFaceLocalID;
			bool entryFaceFound = false;
			T entryFaceDistance = T(0);
			T speed;
			this->velocity.length(&speed);
			for (I fi1=0; fi1<cellNumFaces; fi1++) {
				I f1 = mesh.getCellFaceID(cellLocalID, fi1);
				for (I fi2=0; fi2<lastCellNumFaces; fi2++) {
					I f2 = mesh.getCellFaceID(lastCellLocalID, fi2);

					if (f1 == f2) {
						if (verbose) {
							std::cout << "  > Analysing face " << f1 << std::endl;
						}
						bool doesIntersect;
						cupcfd::geometry::euclidean::EuclideanPoint<T,3> intersection;
						bool intersectionOnEdge;
						T timeToIntersect = T(-1);
						status = calculateFaceIntersection(	mesh, 
															f1, 
															verbose, 
															doesIntersect, 
															intersection, 
															intersectionOnEdge,
															timeToIntersect);
						if (status != cupcfd::error::E_SUCCESS) {
							std::cout << "ERROR: calculateFaceIntersection() failed" << std::endl;
							return status;
						}
						if (doesIntersect) {
							if (!entryFaceFound) {
								entryFaceFound = true;
								entryFaceLocalID = f1;
								entryFaceDistance = speed * timeToIntersect;
							}

							else {
								// Select nearest face:
								T thisFaceDistance = speed * timeToIntersect;
								if (thisFaceDistance < entryFaceDistance) {
									entryFaceLocalID = f1;
									entryFaceDistance = speed * timeToIntersect;
								}
							}
						}
					}
				}
			}
			if (!entryFaceFound) {
				std::cout << "ERROR: Redetection of cell entry face of particle " << this->particleID << " failed" << std::endl;
				return cupcfd::error::E_ERROR;
			}
			this->cellEntryFaceLocalID = entryFaceLocalID;

			return cupcfd::error::E_SUCCESS;
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
