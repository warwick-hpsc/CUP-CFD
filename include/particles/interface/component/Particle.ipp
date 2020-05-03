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
		 cellEntryFaceLocalID(-1),
		 lastCellGlobalID(-1),
		 lastLastCellGlobalID(-1),
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
		 cellEntryFaceLocalID(-1),
		 lastCellGlobalID(-1),
		 lastLastCellGlobalID(-1),
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
		// inline void Particle<P, I, T>::setCellGlobalID(I cellGlobalID)
		inline void Particle<P, I, T>::setCellGlobalID(I cellGlobalID, I cellEntryFaceLocalID)
		{
			// std::cout << "Request received to move particle ID " << this->particleID << " from cell " << this->cellGlobalID << " --> cell " << cellGlobalID << std::endl;
			// std::cout << "Request received to move particle ID " << this->particleID << " from cell " << this->cellGlobalID << " --> cell " << cellGlobalID << " through face " << cellEntryFaceLocalID << std::endl;
			// usleep(100*1000);

			if (this->cellGlobalID == cellGlobalID) {
				// std::cout << "ERROR: Attempting to update a cell of particle " << this->particleID << " to " << cellGlobalID << " but it is already in that cell" << std::endl;
				std::cout << "ERROR: Attempting to update a particle " << this->particleID << " to be in cell " << cellGlobalID << " but it is already in that cell" << std::endl;
				throw std::exception();
			}

			if ( (cellGlobalID == this->lastLastCellGlobalID) || (cellGlobalID == this->lastCellGlobalID) ) {
				std::cout << "ERROR: Attempting to move particle " << this->particleID << " to cell " << cellGlobalID << " but it was there recently (recent history is " << this->lastLastCellGlobalID << " -> " << this->lastCellGlobalID << " -> " << this->cellGlobalID << ")" << std::endl;
				throw std::exception();
			}

			this->lastLastCellGlobalID = this->lastCellGlobalID;
			this->lastCellGlobalID = this->cellGlobalID;
			this->cellGlobalID = cellGlobalID;

			if (cellEntryFaceLocalID == I(-1)) {
				std::cout << "ERROR: Particle::setCellGlobalID() called with invalid value of 'cellEntryFaceLocalID'" << std::endl;
				throw std::exception();
			}
			this->cellEntryFaceLocalID = cellEntryFaceLocalID;
		}

		template <class P, class I, class T>
		inline void Particle<P, I, T>::setCellEntryFaceLocalID(I cellEntryFaceLocalID)
		{
			this->cellEntryFaceLocalID = cellEntryFaceLocalID;
		}

		template <class P, class I, class T>
		inline I Particle<P, I, T>::getCellGlobalID() const
		{
			return this->cellGlobalID;
		}

		template <class P, class I, class T>
		inline I Particle<P, I, T>::getCellEntryFaceLocalID() const
		{
			return this->cellEntryFaceLocalID;
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
		}

		

		template <class P, class I, class T>
		template <class M, class L>
		cupcfd::error::eCodes Particle<P, I, T>::updatePositionAtomic(cupcfd::geometry::mesh::UnstructuredMeshInterface<M,I,T,L>& mesh, T * dt, I * exitFaceLocalID, bool verbose)
		{
			// ToDo: Warning: I'm not certain how this code currently handles intersection points directly through vertices well (assigns first face it finds?)
		
			cupcfd::error::eCodes status;

			// useconds_t verbose_sleep_period = 500*1000;
			useconds_t verbose_sleep_period = 100;

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
					usleep(verbose_sleep_period);
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
			// cupcfd::geometry::euclidean::EuclideanVector<T,3> intersectionDistance;
			T timeToIntersect = T(-1);
			bool intersectionOnEdge;
			cupcfd::geometry::euclidean::EuclideanPoint<T,3> exitIntersection;
			T exitTravelTime = T(-1);
			T exitDistance= T(-1);

			I intersectionCount = 0;

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

			// if (verbose) {
			// 	this->print();
			// }

			// Loop over the faces of the cell
			bool face_was_found = false;
			// if (verbose) {
			// 	std::cout << "  > searching for exit face of particle " << this->particleID << std::endl;
			// 	usleep(verbose_sleep_period);
			// }
			I num_faces_contacting_particle_on_edge = I(0);
			I num_faces_contacting_particle_within_tri = I(0);
			for(I i = 0; i < nFaces; i++)
			{
				// Retrieve the faces of the cell
				I localFaceID = mesh.getCellFaceID(localCellID, i);

				bool face_verbose = false;
				// bool face_verbose = verbose;
				// bool face_verbose = verbose && (this->cellGlobalID == 1202);
				// bool face_verbose = verbose && (this->cellGlobalID == 77);
				// bool face_verbose = verbose && ( (this->cellGlobalID == 77) || (this->cellGlobalID == 78) );
				// bool face_verbose = verbose && (this->cellGlobalID == 1063);
				// bool face_verbose = verbose && (this->cellGlobalID == 1202);
				// bool face_verbose = verbose && (this->cellGlobalID == 5);
				// bool face_verbose = verbose && ( (this->cellGlobalID == 326) || (this->cellGlobalID == 392) || (this->cellGlobalID == 1063) );

				if (localFaceID == this->cellEntryFaceLocalID) {
					if (face_verbose) {
						std::cout << "    > skipping face ID " << localFaceID << " as particle " << this->particleID << " entered cell through it" << std::endl;
					}
					continue;
				}
				
				// Break the faces up into triangles and compute the intersection with the plane of each triangle and determine the time to reach
				// Get the vertices/spatial coordinates for each face
				I nFaceVertices = mesh.getFaceNVertices(localFaceID);
				
				if (face_verbose) {
					std::cout << "    > checking face ID=" << localFaceID << ", #vertices=" << nFaceVertices << " (" << i+1 << " / " << nFaces << ")" << std::endl;
					usleep(verbose_sleep_period);
				}

				// Get the first vertex
				I faceVertex0ID = mesh.getFaceVertex(localFaceID, 0);

				// T max_face_edge_length;
				// for(I j1 = 0; j1 < nFaceVertices; j1++) {
				// 	for(I j2 = j1+1; j2 < nFaceVertices; j2++) {
				// 		faceVertex1ID = mesh.getFaceVertex(localFaceID, j1);
				// 		faceVertex2ID = mesh.getFaceVertex(localFaceID, j2);

				// 		T face_edge_length;
				// 		( mesh.getVertexPos(faceVertex1ID) - mesh.getVertexPos(faceVertex2ID) ).length(&face_edge_length);

				// 		if (j1==0 && j2==1) {
				// 			max_face_edge_length = face_edge_length;
				// 		}
				// 		else if (max_face_edge_length < face_edge_length) {
				// 			max_face_edge_length = face_edge_length;
				// 		}
				// 	}
				// }
				
				// Loop over the triangles of the face
				// // for(I j = 0; j < (nFaceVertices-2); j++)
				// for(I j = 1; j < (nFaceVertices-1); j++)
				// {
				// 	// faceVertex1ID = mesh.getFaceVertex(localFaceID, j+1);
				// 	// faceVertex2ID = mesh.getFaceVertex(localFaceID, j+2);
				// 	faceVertex1ID = mesh.getFaceVertex(localFaceID, j);
				// 	faceVertex2ID = mesh.getFaceVertex(localFaceID, j);

				for(I j = 1; j < (nFaceVertices-1); j++)
				{
					I faceVertex1ID = mesh.getFaceVertex(localFaceID, j);
					I faceVertex2ID = mesh.getFaceVertex(localFaceID, j+1);

					// if (face_verbose) {
					// 	// std::cout << "      > vertex " << j+1 << " / " << (nFaceVertices-2)+1 << std::endl;
					// 	std::cout << "      > vertex " << j+1 << " / " << nFaceVertices << std::endl;
					// 	usleep(verbose_sleep_period);
					// }

					if (face_verbose) {
						std::cout << "      > checking tri formed by vertices: " << faceVertex0ID << ", " << faceVertex1ID << ", " << faceVertex2ID << ", " << std::endl;
						usleep(verbose_sleep_period);
					}
				
					// // Test for intersection on the triangle formed by the plane with the travel vector
					// cupcfd::geometry::euclidean::EuclideanPlane3D<T> plane(mesh.getVertexPos(faceVertex0ID), 
					// 														mesh.getVertexPos(faceVertex1ID), 
					// 														mesh.getVertexPos(faceVertex2ID));
					cupcfd::geometry::euclidean::EuclideanPoint<T,3> faceVertex0 = mesh.getVertexPos(faceVertex0ID);
					cupcfd::geometry::euclidean::EuclideanPoint<T,3> faceVertex1 = mesh.getVertexPos(faceVertex1ID);
					cupcfd::geometry::euclidean::EuclideanPoint<T,3> faceVertex2 = mesh.getVertexPos(faceVertex2ID);
					// if (face_verbose) {
					// 	std::cout << "        > vertices coordinates: " << std::endl;
					// 	std::cout << "          > " ; faceVertex0.print(); std::cout << std::endl;
					// 	std::cout << "          > " ; faceVertex1.print(); std::cout << std::endl;
					// 	std::cout << "          > " ; faceVertex2.print(); std::cout << std::endl;
					// 	usleep(verbose_sleep_period);
					// }
					cupcfd::geometry::euclidean::EuclideanPlane3D<T> plane(faceVertex0,faceVertex1, faceVertex2);
					
					// status = plane.linePlaneIntersection(this->velocity, this->inflightPos, intersection);
					if (plane.isVectorParallel(this->velocity)) {
						status = cupcfd::error::E_EUC_VEC_PARALLEL;
					} else {
						status = cupcfd::error::E_SUCCESS;
					}
					// Verify that the travel vector (velocity) is not parallel to or fully in the face.
					if(status == cupcfd::error::E_EUC_VEC_PARALLEL)
					{
						// Either will never intersect, or is intersecting at all times and not exiting into the
						// neighbouring cell via this face.
						
						// Skip any actions for this face - will not intersect

						if (face_verbose) {
							std::cout << "        > travel is parallel to tri" << std::endl;
							usleep(verbose_sleep_period);
						}
					}
					else
					{
						// if (face_verbose) {
						// 	std::cout << "        > intersection point (old-method) is "; intersection.print() ; std::cout << std::endl;
						// 	usleep(verbose_sleep_period);
						// }

						// Verify that the intersection point occurs within the triangle of the face, rather than just the plane
						// Could also use static method directly rather than create an object
						cupcfd::geometry::shapes::Triangle3D<T> shape(mesh.getVertexPos(faceVertex0ID), 
																	  mesh.getVertexPos(faceVertex1ID), 
																	  mesh.getVertexPos(faceVertex2ID));
							   
						// if(!shape.isPointInside(intersection))
						// {
						// 	if (face_verbose) {
						// 		std::cout << "        > intersection is outside tri shape" << std::endl;
						// 		usleep(verbose_sleep_period);
						// 	}

						// 	// Progress onto next triangle instead
						// 	continue;
						// }
						// if (!shape.intersection(this->inflightPos, this->velocity, intersection)) {
						if (!shape.calculateIntersection(this->inflightPos, this->velocity, intersection, timeToIntersect, intersectionOnEdge, face_verbose)) {
							// if (face_verbose) {
							// 	std::cout << "        > intersection is outside tri shape" << std::endl;
							// 	usleep(verbose_sleep_period);
							// }

							// Progress onto next triangle instead
							continue;
						}
						if (face_verbose) {
							std::cout << "        > intersection point (new-method) is "; intersection.print() ; std::cout << std::endl;
							std::cout << "        > vertices coordinates: " << std::endl;
							std::cout << "          > " ; faceVertex0.print(); std::cout << std::endl;
							std::cout << "          > " ; faceVertex1.print(); std::cout << std::endl;
							std::cout << "          > " ; faceVertex2.print(); std::cout << std::endl;
							usleep(verbose_sleep_period);
							// return cupcfd::error::E_ERROR;
						}
							
						intersectionCount = intersectionCount + 1;
															
						// // Intersection point is inside a triangular face on this shape - Find the time it takes to intersect
						// // Inflight pos -> intersection is a scalar multiple of the velocity vector from Inflightpos
						// // Could use time = distance/speed, but we lose direction (don't want to travel in direction
						// // of negative time)
						// // We know from inflight pos goes through intersection along vector, so just compute scalar for time
						// // ToDo: Probably just a more elegant way of doing this (really we just want negative distance or speed)
						
						// Distance
						T distance;
						// status = (intersection - this->inflightPos).length(&distance);
						// if (status != cupcfd::error::E_SUCCESS) {
						// 	std::cout << "ERROR: length() failed" << std::endl;
						// 	return status;
						// }

						// Speed
						T speed;
						status = this->velocity.length(&speed);
						if (status != cupcfd::error::E_SUCCESS) {
							std::cout << "ERROR: length() failed" << std::endl;
							return status;
						}

						distance = timeToIntersect * speed;

						// if (face_verbose) {
						// 	std::cout << "        > intersection distance is " << distance << std::endl;
						// 	std::cout << "        > speed is " << speed << std::endl;
						// }
						
						// // Time
						// T timeToIntersect = distance / speed;

						// if (this->inflightPos + (this->velocity * timeToIntersect) != intersection)
						// {
						// 	// Travelling in wrong direction to velocity, set time to negative
						// 	timeToIntersect = T(-1) * timeToIntersect;
						// 	if (face_verbose) {
						// 		std::cout << "        > particle travelling away from tri, making time negative" << std::endl;
						// 	}
						// }
						
						// Theoretically, if the time/distance is not zero (i.e. we aren't checking the face of the cell we
						// just left) and the cell is not concave (the cell type for which we don't
						// currently support since it needs to be greater than a hexahedron), then if it is inside the triangle
						// this is the only face we can exit by?
						
						// ToDo: Think there might be a danger of 'bouncing' back and forth if exactly on a vertex - check 
						
						// if ( (!arth::isEqual(timeToIntersect, 0.0)) && timeToIntersect > 0.0)
						// if (timeToIntersect >= CUPCFD_ZERO_COMP_TOL_D)
						if (timeToIntersect >= T(0))
						// T tolerance;
						// cupcfd::geometry::euclidean::EuclideanVector<T,3> faceEdge = mesh.getVertexPos(faceVertex2ID) - mesh.getVertexPos(faceVertex1ID);
						// faceEdge.length(&tolerance);
						// tolerance = fabs(tolerance);
						// // tolerance *= 0.001;
						// tolerance *= 1e-10;
						// if (timeToIntersect > tolerance)
						{
							if (face_verbose) {
								std::cout << "        > will intersect this tri after non-zero travel time " << timeToIntersect << " seconds" << std::endl;
								// std::cout << "          > intersection point is "; intersection.print() ; std::cout << std::endl;
								usleep(verbose_sleep_period);
							}

							if (timeToIntersect == T(0.0)) {
								if (intersectionOnEdge) {
									num_faces_contacting_particle_on_edge++;
								} else {
									num_faces_contacting_particle_within_tri++;
								}
							}

							// if (face_was_found) {
							// 	// Only select this face if nearer than the previously-found face
							// 	if (timeToIntersect < exitTravelTime) {
							// 		if (face_verbose) {
							// 			std::cout << "        > this face is nearer than previously-found face" << std::endl;
							// 		}
							// 		exitFaceID = localFaceID;
							// 		exitIntersection = intersection;
							// 		exitTravelTime = timeToIntersect;
							// 		exitDistance = distance;
							// 		face_was_found = true;
							// 		break;
							// 	}
							// } else {
							// 	exitFaceID = localFaceID;
							// 	exitIntersection = intersection;
							// 	exitTravelTime = timeToIntersect;
							// 	exitDistance = distance;
							// 	face_was_found = true;
							// }

							if (!face_was_found || (timeToIntersect < exitTravelTime)) {
								if (face_verbose) {
									if (timeToIntersect < exitTravelTime) {
										std::cout << "        > this face is nearer than previously-found face" << std::endl;
									} else {
										std::cout << "        > selecting this face" << std::endl;
									}
								}

								if (face_was_found && timeToIntersect==T(0) && exitTravelTime==T(0) && num_faces_contacting_particle_within_tri==2) {
									// There are two face triangles which this particle is apparently directly resting on, away from their edges.
									// This can only happen if two faces are directly overlapping in the same plane!
									std::cout << "ERROR: Particle " << this->particleID << " is classified as making direct contact with at least 2 face triangles, when just 1 is expected" << std::endl;
									throw std::exception();
								}

								exitFaceID = localFaceID;
								exitIntersection = intersection;
								exitTravelTime = timeToIntersect;
								exitDistance = distance;
								face_was_found = true;
							}
						}
						// else {
						// 	if (face_verbose) {
						// 		std::cout << "> > > > travel time to face is " << timeToIntersect << std::endl;
						// 		usleep(verbose_sleep_period);
						// 	}
						// }

						// if (timeToIntersect < 0.0) {
						// 	if (face_verbose) {
						// 		std::cout << "        > travelling away from tri, " << timeToIntersect*(-1) << " seconds away" << std::endl;
						// 		usleep(verbose_sleep_period);
						// 	}
						// }
						
						// We can only exit via one face - if it is not set yet and the travel time is 0
						// we are currently sitting on a face/vertex etc.
						// Set it as a fallback - if we exit via no other face then the face we are sitting on
						// must be the one we leave by, else it will be overwritten by the correct face (and
						// then the loop is exited via a break)
												
						// // I think the following logic is faulty. For a start, 'intersection' has no relation to 
						// // 'localFaceID'.
						// std::cout << "ERROR: Failed to find cell face that particle " << this->particleID << " will exit through" << std::endl;
						// return cupcfd::error::E_ERROR;
						// // throw std::exception();
						// if(arth::isEqual(timeToIntersect, 0.0) && (exitFaceID == -1))
						// {
						// 	if (face_verbose) {
						// 		std::cout << "> did not find exit face, using current as fallback" << std::endl;
						// 	}
						// 	exitFaceID = localFaceID;
						// 	exitIntersection = intersection;
						// 	exitTravelTime = 0.0;
						// 	face_was_found = true;
						// }
					}
				}

				// if (face_was_found) {
				// 	break;
				// }
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

			// if (verbose) {
			// 	std::cout << "    > selected exit face ID is " << exitFaceID << std::endl;
			// }


			if (exitDistance > max_inter_vertex_distance) {
				std::cout << "ERROR: Particle " << this->particleID << " distance to selected face intersection " << exitDistance << " is greater than max inter-vertex distance " << max_inter_vertex_distance << std::endl;
				throw std::exception();
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
					std::cout << "    > exits cell in this timestep through face " << exitFaceID << " after " << exitTravelTime << " seconds" << std::endl;
					usleep(verbose_sleep_period);
				}

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
