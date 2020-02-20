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
 * Contains header level definitions for the ParticleSystemSimple class
 */

#ifndef CUPCFD_PARTICLES_PARTICLE_SYSTEM_SIMPLE_IPP_H
#define CUPCFD_PARTICLES_PARTICLE_SYSTEM_SIMPLE_IPP_H

#include "ArithmeticKernels.h"
#include "SortDrivers.h"
#include "ExchangeMPI.h"
#include "Reduce.h"

namespace arth = cupcfd::utility::arithmetic::kernels;

namespace cupcfd
{
	namespace particles
	{
		template <class M, class I, class T, class L>
		ParticleSystemSimple<M,I,T,L>::ParticleSystemSimple(std::shared_ptr<cupcfd::geometry::mesh::UnstructuredMeshInterface<M,I,T,L>> mesh)
		: ParticleSystem<ParticleSystemSimple<M, I, T, L>, ParticleEmitterSimple<I,T>, ParticleSimple<I,T>, M, I, T, L>(mesh),
		  nActiveParticles(0),
		  nTravelParticles(0)
		{
		
		}
		
		template <class M, class I, class T, class L>
		ParticleSystemSimple<M,I,T,L>::~ParticleSystemSimple()
		{
		
		}
		
		template <class M, class I, class T, class L>
		inline cupcfd::error::eCodes ParticleSystemSimple<M,I,T,L>::addParticle(const ParticleSimple<I,T>& particle)
		{			
			// If the particle is not already marked as inactive (would be a bit weird to add an inactive particle),
			// increment the active counter
			bool isInactive = particle.getInactive();
			
			// Note: We do not find the cell ID here, since it is an expensive operation and for emitter generated
			// particles the cell is already known. For other particles, it should be computed elsewhere
			// and correctly set before calling this function.
			
			if(!isInactive)
			{
				this->particles.push_back(particle);
				
				// We don't set the inflight position equal to the position here, it sohuld be set prior to an add.
				// If this is a particle in flight, then pos is the original start position, we don't want to lose our current position.
				//this->particles[this->particles.size()-1].inflightPos = particle.pos;
				
				this->nActiveParticles = this->nActiveParticles + 1;
				
				// If the particle has a travel time assigned to it, increase the number of active, travelling particles
				if(particle.getTravelTime() > T(0))
				{
					this->nTravelParticles = this->nTravelParticles + 1;
				}
			}
		}
		
		template <class M, class I, class T, class L>
		cupcfd::error::eCodes ParticleSystemSimple<M,I,T,L>::addParticleEmitter(const ParticleEmitterSimple<I,T>& emitter)
		{
			// ToDo: Check Local, Global and Rank in emitter are correct
		
			this->emitters.push_back(emitter);
		}
		
		template <class M, class I, class T, class L>
		inline cupcfd::error::eCodes ParticleSystemSimple<M,I,T,L>::setParticleInactive(I particleID)
		{
			// particleID for this scheme is the index in the vector
			
			// Check particle is not already inactive
			if(!(this->particles[particleID].getInactive()))
			{
				this->particles[particleID].setInactive();
				this->nActiveParticles = this->nActiveParticles - 1;
				
				// If it had a travel time, it would also have been counted as a travelling particle,
				// so reduce this counter also
				if(this->particles[particleID].getTravelTime() > T(0))
				{
					this->nTravelParticles = this->nTravelParticles - 1;
				}
			}
		}
		
		template <class M, class I, class T, class L>
		cupcfd::error::eCodes ParticleSystemSimple<M,I,T,L>::removeInactiveParticles()
		{
			// This is far too expensive for a vector, but will temporarily suffice for
			// testing functionality
		
			// We should not need to update active/travel counters, particles removed here
			// are already inactive and thus counters should have already been decreased.
		
			// Go through the system and mark any inactive particles
			I loopCount = this->particles.size();
			
			for(I i = 0; i < loopCount; i++)
			{
				if(this->particles[i].getInactive())
				{
					this->particles.erase(this->particles.begin() + i);
					
					// Correct indexes and loop count to account for removed particle
					i = i - 1;
					loopCount = loopCount - 1;
				}
			}
		}
		
		
		
		template <class M, class I, class T, class L>
		cupcfd::error::eCodes ParticleSystemSimple<M,I,T,L>::exchangeParticles()
		{
			// We could build an Exchange Pattern based on the positions of the Particles we wish to exchange.
			// However, particles are not like a halo, since the amounts can vary even if the neighbours do not,
			// so it would have to be rebuilt. Since the current builder uses an All-To-All (since it is completly
			// unware of which ranks hold which Global ID data) that would be far, far too expensive.
			// At some point, a ExchangePattern builder that uses partial information should be made (e.g. restricts
			// information requests to known neighbours only - we don't need to ask all ranks if they own 'index 1'
			// if we know it must be on a neighbour). Till such a time, we will do the index location management stuff manually
			// in this function and then fall back onto the MPI Exchange Functions.
			
			// (1) Identify the indexes of any particles in the particle list that are intended to leave this rank
			// Group particles by Rank ID to make this easier
			
			I nParticles = this->particles.size();
			I * rankIDs = (I *) malloc(sizeof(I) * nParticles);
			I * rankIDIndexes = (I *) malloc(sizeof(I) * nParticles);

			for(I i = 0; i < this->particles.size(); i++)
			{
				rankIDs[i] = this->particles[i].rank;
			}
			
			// Proxy behaviour - sorts and copies
			cupcfd::utility::drivers::merge_sort_index(rankIDs, nParticles, rankIDIndexes, nParticles);
			cupcfd::utility::drivers::sourceIndexReorder(&(this->particles[0]), nParticles, rankIDIndexes, nParticles);
			
			// (2) Pass-through the particles and their destination ranks to an exchange function
			// No point in creating an exchange pattern object, since there is an overhead to doing so and it will not be reused
			// between atomic steps since the state will change causing the previous one to no longer be correct
			
			// ToDo: Would like the option to select between different exchange methods (i.e. one-sided vs two-sided), but
			// for now we can leave it as fixed for proxy tests
			
			// Count number of elements to send to each neighbour
			// NeighbourRanks in the comm graph holds the neighbours of this rank
			I nNeighbours = this->mesh->cellConnGraph->neighbourRanks.size();
			I * neighbourCount = (I *) malloc(sizeof(I) * nNeighbours);
			
			I * neighbourRanks = (I *) malloc(sizeof(I) * nNeighbours);
			for(I i = 0; i < nNeighbours; i++)
			{
				neighbourRanks[i] = this->mesh->cellConnGraph->neighbourRanks[i];
			}
			cupcfd::utility::drivers::merge_sort(neighbourRanks, nNeighbours);
			
			std::map<I,I> neighbourIDMapping;
			
			for(I i = 0; i < nNeighbours; i++)
			{
				neighbourIDMapping[neighbourRanks[i]] = i;
				neighbourCount[i] = I(0);
			}
			

			for(I i = 0; i < nParticles; i++)
			{
				if(this->particles[i].rank != this->mesh->cellConnGraph->comm->rank)
				{
					I index = neighbourIDMapping[this->particles[i].rank];
					neighbourCount[index] = neighbourCount[index] + 1;
				}
			}

			// Exchange expected particle counts with neighbours
			I * recvBuffer = (I *) malloc(sizeof(I) * nNeighbours);
			MPI_Request * requests;
			I nRequests;
			
			cupcfd::comm::mpi::ExchangeMPIIsendIrecv(neighbourCount, nNeighbours,recvBuffer, nNeighbours, 
								  neighbourRanks, nNeighbours,
								  1, this->mesh->cellConnGraph->comm->comm,
								  &requests, &nRequests);
			
			// ToDo: Getting multiple definitions error when using WaitallMPI.h header.
			// Presumably a missing header guard somewhere, but I can't seem to find it for now....
			// Do it directly instead
			MPI_Status * statuses = (MPI_Status *) malloc(sizeof(MPI_Status) * nRequests);
			MPI_Waitall(nRequests, requests, statuses);
			free(statuses);
			free(requests);
			

			// Now counts are known, we can do the proper exchange of particles with suitably sized buffers
			I totalSendCount = 0;
			I totalRecvCount = 0;
			
			for(I i = 0; i < nNeighbours; i++)
			{
				totalSendCount = totalSendCount + neighbourCount[i];
				totalRecvCount = totalRecvCount + recvBuffer[i];
			}
			
			ParticleSimple<I,T> * particleSendBuffer = (ParticleSimple<I,T> *) malloc(sizeof(ParticleSimple<I,T>) * totalSendCount);
			ParticleSimple<I,T> * particleRecvBuffer = (ParticleSimple<I,T> *) malloc(sizeof(ParticleSimple<I,T>) * totalRecvCount);
			

			// Copy particles we are sending to the send buffer
			// We are expecting the particles vectors to be in rank sorted order, and the destination ranks should be in rank order
			// so we should be able to just copy one after the other
			
			I ptr = 0;
			for(I i = 0; i < nParticles; i++)
			{
				if(this->particles[i].rank != this->mesh->cellConnGraph->comm->rank)
				{
					particleSendBuffer[ptr] = this->particles[i];
					ptr = ptr + 1; 
				}
			}
			
			cupcfd::error::eCodes status;
			status = ExchangeVMPIIsendIrecv(particleSendBuffer, totalSendCount, neighbourCount, nNeighbours,
								   particleRecvBuffer, totalRecvCount, recvBuffer, nNeighbours,
								   neighbourRanks, nNeighbours,
								   neighbourRanks, nNeighbours,
								   this->mesh->cellConnGraph->comm->comm,
								   &requests, &nRequests);
								   
			statuses = (MPI_Status *) malloc(sizeof(MPI_Status) * nRequests);
			MPI_Waitall(nRequests, requests, statuses);
			free(statuses);				   
							   
			// Add any particles we received to the system
			for(I i = 0; i < totalRecvCount; i++)
			{
				this->addParticle(particleRecvBuffer[i]);
			}
			
			// Tidyup Stage
			// Free temporary buffers
						   
			free(rankIDs);
			free(rankIDIndexes);
			free(neighbourCount);
			free(recvBuffer);
			free(particleSendBuffer);
			free(particleRecvBuffer);
			free(neighbourRanks);
			free(requests);

			// Mark any particles we have sent to other processes as inactive - they should effectively be ignored/queued up for removal 
			for(I i = 0; i < this->particles.size(); i++)
			{
				// Gone off-rank and was previously marked as active
				if((this->particles[i].rank != this->mesh->cellConnGraph->comm->rank) && (!(this->particles[i].getInactive())))
				{
					this->setParticleInactive(i);
				}
			}
		}
		
		template <class M, class I, class T, class L>
		cupcfd::error::eCodes ParticleSystemSimple<M,I,T,L>::updateSystem(T dt)
		{
			// ToDo: Notes
			// If a particle is not going off-rank, we could theoretically keep performing atomic updates for those
			// particles till they run out of travel time in this dt period, or they go off-rank.
			// This could reduce the number of exchange stages/group them into fewer, larger comms, as
			// well as the number of allreduces. 
			// Would this affect synchronised behaviours for certain particle types though?
		
			// (1a) Ensure that the travelTime for all existing active particles is set to the time period dt
			this->setActiveParticlesTravelTime(dt);
			
			// (1b) Set all particles inflight positions to be equal to their current positions
			// ToDo: Move this to a function
			for(I i = 0; i < particles.size(); i++)
			{
				particles[i].inflightPos = particles[i].pos;
			}
			
			// (2) Generate any new particles from the emitter, and add them to the system (with the
			// appropriate time remaining in this period depending on when they were generated)
			this->generateEmitterParticles(dt);

			// Keep looping as long as there exists a particle anywhere in the system that is still going (since we could
			// receive one on any iteration, even if we don't on this one)
			I nGlobalTravelParticles = 0;
			I tmp = this->getNTravelParticles();
			cupcfd::comm::allReduceAdd(&tmp, 1, &nGlobalTravelParticles, 1, *(this->mesh->cellConnGraph->comm));
			
			while(nGlobalTravelParticles > 0)
			{									
				// Advance particles by at most one cell
				this->updateSystemAtomic();
								
				// No further changes should need to be made to the data stored inside a particle, and they should have ranks
				// representing the process they should be located on. 
				// Now we need to organise the particles within the system.
				// Particles that are 'dead' will have no further processing, and should be removed or handled to be treated as such
				// Particles that are to be communicated to other processes should be grouped up ready for comms
				// Particles that do not have to go off-node, but have travel time remaining, should be separated from those that have
				// no move travel time to speed up subsequent atomic steps in updating the system state (since those with no further travel
				// time should be skipped)
				
				// Remove Dead Particles (They have no further effect on the system and we don't want to exchange dead particles)
				this->removeInactiveParticles();
				
				// We can now perform an exchange to identify how many (if any)
				// particles will go off-rank in this atomic update, and transfer them ready for another round of atomic updates.
				this->exchangeParticles();
				
				// Cleanup any sent particles that should now be marked as inactive after being sent to another rank
				this->removeInactiveParticles();
				
				// Count how many are actively moving overall (to keep the loop going if needed)
				tmp = this->getNTravelParticles();
				cupcfd::comm::allReduceAdd(&tmp, 1, &nGlobalTravelParticles, 1, *(this->mesh->cellConnGraph->comm));
			}
		}
		
		template <class M, class I, class T, class L>
		cupcfd::error::eCodes ParticleSystemSimple<M,I,T,L>::updateSystemAtomic()
		{
			// Loop over all particles in vector
			// ToDo: This approach also loops over particles that are inactive or active but have no further travel time.
			// Would be faster if inactive particles are removed
			for(I i = 0; i < this->particles.size(); i++)
			{
				// Can't guarantee there are no inactive particles so have this check guard here.
				// If we can guarantee it we can remove it however (don't really want a branch inside a loop)
				if(!(this->particles[i].getInactive()))
				{
					T stepDt;			// How much time this particle moves by in its current cell
					I localFaceID;		// The mesh local ID of the face that the particle ends up at in its current cell
					I localCellID;		// The local (not global) mesh cell ID that the particle is currently in
					I cellNode;			// The node value in the connectivity graph of the cell that the particle is currently in
										// (Needed to map between local ID and global ID since this label is arbitrary when building)
				
					// Get the Local Cell ID since ParticleSimple only stores the Mesh Global Cell ID
					cellNode = this->mesh->cellConnGraph->globalToNode[this->particles[i].cellGlobalID];
					this->mesh->cellConnGraph->connGraph.getNodeLocalIndex(cellNode, &localCellID);
					
					// Note: For particles with no further travel time, the following steps must not change the state
					// of the particle and system.
					
					// Perform an atomic positional update for every particle in the system, advancing them by at most one cell
					// This will also update the travel time
					this->particles[i].updatePositionAtomic(*(this->mesh), &stepDt, &localFaceID);
	
					// Perform an atomic velocity update for every particle in the system, reflecting the time they have advanced by
					this->particles[i].updateVelocityAtomic(*(this->mesh), localCellID, stepDt);
					
					// Perform a state update, updating both particle state (e.g. change in properties specific to a particle,
					// such as mass) and updating cell source values resulting from changes in the particle.
					this->particles[i].updateStateAtomic(*(this->mesh), localCellID, stepDt);
					
					// Check that it reached a face after moving
					if(!(localFaceID == -1) && stepDt > T(0))
					{
						// Perform a face update - any cells that are trying to exit a cell are currently positioned at a face (boundary
						// or non-boundary). This step updates their cell ID (if they are leaving a cell), their rank (if they are
						// going into a position owned by a different process in the mesh), and/or any other quantities that might be
						// applicable depending upon the face (e.g. changing velocity direction in the event of a reflective boundary)
						
						// Is the face a boundary
						if(!this->mesh->getFaceIsBoundary(localFaceID))
						{
							this->particles[i].updateNonBoundaryFace(*(this->mesh), localFaceID);
						}
						else
						{
							I boundaryID = this->mesh->getFaceBoundaryID(localFaceID);
							I regionID = this->mesh->getBoundaryRegionID(boundaryID);
							cupcfd::geometry::mesh::RType boundaryType = this->mesh->getRegionType(regionID);
							
							switch(boundaryType)
							{
								case(cupcfd::geometry::mesh::RTYPE_WALL):		this->particles[i].updateBoundaryFaceWall(*(this->mesh), localCellID, localFaceID);
																					break;
								
								case(cupcfd::geometry::mesh::RTYPE_INLET):		this->particles[i].updateBoundaryFaceInlet(*(this->mesh), localCellID, localFaceID);
																					break;
																					
								case(cupcfd::geometry::mesh::RTYPE_OUTLET):	this->particles[i].updateBoundaryFaceOutlet(*(this->mesh), localCellID, localFaceID);
																					break;
														
								case(cupcfd::geometry::mesh::RTYPE_SYMP):		this->particles[i].updateBoundaryFaceSymp(*(this->mesh), localCellID, localFaceID);
																					break;
																					
								// Unexpected case - treat like a wall?
								case(cupcfd::geometry::mesh::RTYPE_DEFAULT):	this->particles[i].updateBoundaryFaceWall(*(this->mesh), localCellID, localFaceID);
																					break;
																					
								// Unexpected case - treat like a wall?
								default:											this->particles[i].updateBoundaryFaceWall(*(this->mesh), localCellID, localFaceID);
																					break;										
							}
						}
					}
										
					// If this particle has no further travel time (but did move this step) then decrease the number
					// of travelling particles
					if(!(this->particles[i].getTravelTime() > T(0)) && stepDt > T(0))
					{
						this->nTravelParticles = this->nTravelParticles - 1;
					}
				}
			}
				
			return cupcfd::error::E_SUCCESS;
		}
		
		template <class M, class I, class T, class L>
		I ParticleSystemSimple<M,I,T,L>::getNParticles()
		{
			return this->particles.size();
		}
		
		template <class M, class I, class T, class L>
		I ParticleSystemSimple<M,I,T,L>::getNActiveParticles()
		{
			return this->nActiveParticles;
		}
		
		template <class M, class I, class T, class L>
		I ParticleSystemSimple<M,I,T,L>::getNTravelParticles()
		{
			return this->nTravelParticles;
		}
		
		template <class M, class I, class T, class L>
		cupcfd::error::eCodes ParticleSystemSimple<M,I,T,L>::setActiveParticlesTravelTime(T travelTime)
		{
			if(!(travelTime > T(0)))
			{
				// Negative or zero travel time - set all particles travel time to zero and non-travelling
				for(I i = 0; i < this->particles.size(); i++)
				{
					this->particles[i].setTravelTime(T(0));
				}
				
				this->nTravelParticles = 0;
			}
			else
			{
				for(I i = 0; i < this->particles.size(); i++)
				{
					if(!this->particles[i].getInactive())
					{
						// Increase travel particle count if it wasn't already counted as travelling
						if(!(this->particles[i].getTravelTime() > T(0)))
						{
							this->nTravelParticles = this->nTravelParticles + 1;
						}
						
						// Set the travel time
						this->particles[i].setTravelTime(travelTime);
					}
				}
			}
		}
		
		template <class M, class I, class T, class L>
		cupcfd::error::eCodes ParticleSystemSimple<M,I,T,L>::generateEmitterParticles(T dt)
		{
			for(I i = 0; i < this->emitters.size(); i++)
			{
				ParticleSimple<I,T> * newParticles;
				I nNewParticles = 0;
				
				this->emitters[i].generateParticles(&newParticles, &nNewParticles, dt);
				
				for(I j = 0; j < nNewParticles; j++)
				{
					this->addParticle(newParticles[j]);
				}
				
				free(newParticles);
			}
		}
	}
}

#endif
