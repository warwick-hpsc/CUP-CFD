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

#include <unistd.h>

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
			cupcfd::error::eCodes status;

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

			return cupcfd::error::E_SUCCESS;
		}
		
		template <class M, class I, class T, class L>
		cupcfd::error::eCodes ParticleSystemSimple<M,I,T,L>::addParticleEmitter(const ParticleEmitterSimple<I,T>& emitter)
		{
			// ToDo: Check Local, Global and Rank in emitter are correct
		
			this->emitters.push_back(emitter);

			return cupcfd::error::E_SUCCESS;
		}
		
		template <class M, class I, class T, class L>
		inline cupcfd::error::eCodes ParticleSystemSimple<M,I,T,L>::setParticleInactive(I particleID)
		{
			cupcfd::error::eCodes status;
			
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

			return cupcfd::error::E_SUCCESS;
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
			
			// for(I i = 0; i < loopCount; i++)
			// {
			// 	if(this->particles[i].getInactive())
			// 	{
			// 		this->particles.erase(this->particles.begin() + i);
					
			// 		// Correct indexes and loop count to account for removed particle
			// 		i = i - 1;
			// 		loopCount = loopCount - 1;
			// 	}
			// }

			for (I i=loopCount-1; i>=0; i--) {
				if (this->particles[i].getInactive())
					this->particles.erase(this->particles.begin() + i);
			}

			return cupcfd::error::E_SUCCESS;
		}
		
		
		
		template <class M, class I, class T, class L>
		cupcfd::error::eCodes ParticleSystemSimple<M,I,T,L>::exchangeParticles()
		{
			cupcfd::error::eCodes status;

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
				// rankIDs[i] = this->particles[i].getRank();
			}
			
			// Proxy behaviour - sorts and copies
			status = cupcfd::utility::drivers::merge_sort_index(rankIDs, nParticles, rankIDIndexes, nParticles);
			if (status != cupcfd::error::E_SUCCESS) {
				std::cout << "ERROR: merge_sort_index() failed" << std::endl;
				return status;
			}

			status = cupcfd::utility::drivers::sourceIndexReorder(&(this->particles[0]), nParticles, rankIDIndexes, nParticles);
			if (status != cupcfd::error::E_SUCCESS) {
				std::cout << "ERROR: sourceIndexReorder() failed" << std::endl;
				return status;
			}
			
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
			status = cupcfd::utility::drivers::merge_sort(neighbourRanks, nNeighbours);
			if (status != cupcfd::error::E_SUCCESS) {
				std::cout << "ERROR: merge_sort() failed" << std::endl;
				return status;
			}
			
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
				// if(this->particles[i].getRank() != this->mesh->cellConnGraph->comm->rank)
				// {
				// 	I index = neighbourIDMapping[this->particles[i].getRank()];
					neighbourCount[index] = neighbourCount[index] + 1;
				}
			}

			// Exchange expected particle counts with neighbours
			I * recvBuffer = (I *) malloc(sizeof(I) * nNeighbours);
			MPI_Request * requests;
			I nRequests;
			
			status = cupcfd::comm::mpi::ExchangeMPIIsendIrecv(neighbourCount, nNeighbours,recvBuffer, nNeighbours, 
								  neighbourRanks, nNeighbours,
								  1, this->mesh->cellConnGraph->comm->comm,
								  &requests, &nRequests);
			if (status != cupcfd::error::E_SUCCESS) {
				std::cout << "ERROR: ExchangeMPIIsendIrecv() failed" << std::endl;
				return status;
			}
			
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
				// if(this->particles[i].getRank() != this->mesh->cellConnGraph->comm->rank)
				{
					particleSendBuffer[ptr] = this->particles[i];
					ptr = ptr + 1; 
				}
			}

			// if (totalSendCount > 0) {
			// 	std::cout << "Sending " << totalSendCount << " particles to a neighbour" << std::endl;
			// }
			
			// if (totalSendCount > 0) {
			// 	std::cout << "Sending " << totalSendCount << " particles to a neighbour" << std::endl;
			// 	if (totalSendCount < 5) {
			// 		std::cout << "> IDs: ";
			// 		std::cout << particleSendBuffer[0].particleID;
			// 		for(I i = 1; i < totalSendCount; i++) {
			// 			std::cout << ", " << particleSendBuffer[i].particleID;
			// 		}
			// 		std::cout << std::endl;

			// 		std::cout << "> Cell IDs: ";
			// 		std::cout << particleSendBuffer[0].getCellGlobalID() << " (last = " << particleSendBuffer[0].lastCellMoveGlobalID << ")";
			// 		for(I i = 1; i < totalSendCount; i++) {
			// 			std::cout << ", " << particleSendBuffer[i].getCellGlobalID() << " (last = " << particleSendBuffer[i].lastCellMoveGlobalID << ")";
			// 		}
			// 		std::cout << std::endl;
			// 	}
			// }
			
			status = ExchangeVMPIIsendIrecv(particleSendBuffer, totalSendCount, neighbourCount, nNeighbours,
								   particleRecvBuffer, totalRecvCount, recvBuffer, nNeighbours,
								   neighbourRanks, nNeighbours,
								   neighbourRanks, nNeighbours,
								   this->mesh->cellConnGraph->comm->comm,
								   &requests, &nRequests);
			if (status != cupcfd::error::E_SUCCESS) {
				std::cout << "ERROR: ExchangeVMPIIsendIrecv() failed" << std::endl;
				return status;
			}
								   
			statuses = (MPI_Status *) malloc(sizeof(MPI_Status) * nRequests);
			MPI_Waitall(nRequests, requests, statuses);
			free(statuses);

			// if (totalRecvCount > 0) {
			// 	std::cout << "Received " << totalRecvCount << " particles from a neighbour" << std::endl;
			// }
						
			// if (totalRecvCount > 0) {
			// 	std::cout << "Received " << totalRecvCount << " particles from a neighbour" << std::endl;
			// 	if (totalRecvCount < 5) {
			// 		std::cout << "> IDs: ";
			// 		std::cout << particleRecvBuffer[0].particleID;
			// 		for(I i = 1; i < totalRecvCount; i++) {
			// 			std::cout << ", " << particleRecvBuffer[i].particleID;
			// 		}
			// 		std::cout << std::endl;
					
			// 		std::cout << "> Cell IDs: ";
			// 		std::cout << particleRecvBuffer[0].getCellGlobalID() << " (last = " << particleRecvBuffer[0].lastCellMoveGlobalID << ")";
			// 		for(I i = 1; i < totalRecvCount; i++) {
			// 			std::cout << ", " << particleRecvBuffer[i].getCellGlobalID() << " (last = " << particleRecvBuffer[i].lastCellMoveGlobalID << ")";
			// 		}
			// 		std::cout << std::endl;
			// 	}
			// }

			// Add any particles we received to the system
			for(I i = 0; i < totalRecvCount; i++)
			{
				if (particleRecvBuffer[i].lastCellGlobalID == I(-1)) {
					std::cout << "ERROR: Exchanged particle " << particleRecvBuffer[i].particleID << " has forgotten its cell travel history" << std::endl;
					throw std::exception();
				}

				// When sending particles across process boundaries, particle will forget 
				// which face it entered cell through. So need to manually identify and set here:
				I cellGlobalID = particleRecvBuffer[i].getCellGlobalID();
				I cellNode = this->mesh->cellConnGraph->globalToNode[cellGlobalID];
				I cellLocalID;
				status = this->mesh->cellConnGraph->connGraph.getNodeLocalIndex(cellNode, &cellLocalID);
				if (status != cupcfd::error::E_SUCCESS) {
					std::cout << "ERROR: getNodeLocalIndex() failed" << std::endl;
					throw std::exception();
				}
				I cellNumFaces;
				status = this->mesh->getCellNFaces(cellLocalID, &cellNumFaces);
				if (status != cupcfd::error::E_SUCCESS) {
					std::cout << "ERROR: getCellNFaces() failed" << std::endl;
					throw std::exception();
				}

				I lastCellGlobalID = particleRecvBuffer[i].lastCellGlobalID;
				I lastCellNode = this->mesh->cellConnGraph->globalToNode[lastCellGlobalID];
				I lastCellLocalID;
				status = this->mesh->cellConnGraph->connGraph.getNodeLocalIndex(lastCellNode, &lastCellLocalID);
				if (status != cupcfd::error::E_SUCCESS) {
					std::cout << "ERROR: getNodeLocalIndex() failed" << std::endl;
					throw std::exception();
				}
				I lastCellNumFaces;
				status = this->mesh->getCellNFaces(lastCellLocalID, &lastCellNumFaces);
				if (status != cupcfd::error::E_SUCCESS) {
					std::cout << "ERROR: getCellNFaces() failed" << std::endl;
					throw std::exception();
				}

				I sharedFaceLocalID;
				bool sharedFaceFound = false;
				for (I fi1=0; fi1<cellNumFaces; fi1++) {
					I f1 = this->mesh->getCellFaceID(cellLocalID, fi1);
					for (I fi2=0; fi2<lastCellNumFaces; fi2++) {
						I f2 = this->mesh->getCellFaceID(lastCellLocalID, fi2);

						if (f1 == f2) {
							sharedFaceFound = true;
							sharedFaceLocalID = f1;
							break;
						}
					}
					if (sharedFaceFound) break;
				}
				if (!sharedFaceFound) {
					std::cout << "ERROR: When manually detecting cell entry face of particle " << particleRecvBuffer[i].particleID << " after being sent across MPI boundary, detected failed" << std::endl;
					throw std::exception();
				}
				particleRecvBuffer[i].setCellEntryFaceLocalID(sharedFaceLocalID);
				if (particleRecvBuffer[i].particleID == 1) {
					std::cout << "Manually updating particle " << particleRecvBuffer[i].particleID << " to have entry face local ID " << sharedFaceLocalID << std::endl;
				}
				// End of entry face identification.


				status = this->addParticle(particleRecvBuffer[i]);
				if (status != cupcfd::error::E_SUCCESS) {
					std::cout << "ERROR: addParticle() failed" << std::endl;
					return status;
				}
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
				// if((this->particles[i].getRank() != this->mesh->cellConnGraph->comm->rank) && (!(this->particles[i].getInactive())))
				{
					status = this->setParticleInactive(i);
					if (status != cupcfd::error::E_SUCCESS) {
						std::cout << "ERROR: setParticleInactive() failed" << std::endl;
						return status;
					}
				}
			}

			return cupcfd::error::E_SUCCESS;
		}
		
		template <class M, class I, class T, class L>
		cupcfd::error::eCodes ParticleSystemSimple<M,I,T,L>::updateSystem(T dt)
		{
			useconds_t verbose_sleep_period = 100;

			// std::cout << "UPDATE SYSTEM" << std::endl;

			// ToDo: Notes
			// If a particle is not going off-rank, we could theoretically keep performing atomic updates for those
			// particles till they run out of travel time in this dt period, or they go off-rank.
			// This could reduce the number of exchange stages/group them into fewer, larger comms, as
			// well as the number of allreduces. 
			// Would this affect synchronised behaviours for certain particle types though?
		
			cupcfd::error::eCodes status;

			// (1a) Ensure that the travelTime for all existing active particles is set to the time period dt
			status = this->setActiveParticlesTravelTime(dt);
			if (status != cupcfd::error::E_SUCCESS) {
				std::cout << "ERROR: setActiveParticlesTravelTime() failed" << std::endl;
				return status;
			}
			
			// (1b) Set all particles inflight positions to be equal to their current positions
			// ToDo: Move this to a function
			for(I i = 0; i < particles.size(); i++)
			{
				particles[i].inflightPos = particles[i].pos;
			}

			// (2) Generate any new particles from the emitter, and add them to the system (with the
			// appropriate time remaining in this period depending on when they were generated)
			status = this->generateEmitterParticles(dt);
			if (status != cupcfd::error::E_SUCCESS) {
				std::cout << "ERROR: generateEmitterParticles() failed" << std::endl;
				return status;
			}

			// std::cout << particles.size() << " particles in system" << std::endl;

			// Keep looping as long as there exists a particle anywhere in the system that is still going (since we could
			// receive one on any iteration, even if we don't on this one)
			I nGlobalTravelParticles = 0;
			I tmp = this->getNTravelParticles();
			status = cupcfd::comm::allReduceAdd(&tmp, 1, &nGlobalTravelParticles, 1, *(this->mesh->cellConnGraph->comm));
			if (status != cupcfd::error::E_SUCCESS) {
				std::cout << "ERROR: allReduceAdd() failed" << std::endl;
				return status;
			}

			// Check for particles with same ID:
			if (particles.size() > 0) {
				for (I i=0; i<particles.size(); i++) {
					for (I j=i+1; j<particles.size(); j++) {
						if (particles[i].particleID == particles[j].particleID) {
							std::cout << "ERROR: Detected duplicate of particle ID " << particles[i].particleID << std::endl;
							std::cout << "       > original at idx=" << i << std::endl;
							std::cout << "       > duplicate at idx=" << j << std::endl;
							return cupcfd::error::E_ERROR;
						}
					}
				}
			}

			// bool have_bugged_particle = false;
			// I bugged_particle_idx = 0;
			// ParticleSimple<I,T> bugged_particle_copy;
			// if (particles.size() > 0) {
			// 	for (I i=0; i<particles.size(); i++) {
			// 		if (particles[i].particleID == 4801) {
			// 			if (have_bugged_particle) {
			// 				std::cout << "ERROR: Multiple particles have ID 4801" << std::endl;
			// 				throw std::exception();
			// 			}
			// 			have_bugged_particle = true;
			// 			bugged_particle_idx = i;
			// 			bugged_particle_copy = ParticleSimple<I,T>(particles[i]);
			// 		}
			// 	}
			// }

			// bool verbose = false;
			// // if (nGlobalParticles == 138) {
			// if (nGlobalParticles == 138 && have_bugged_particle) {
			// 	// Particle 86 on emitter 0 becomes a problem at this point in simulation.
			// 	verbose = true;
			// }

			int nGlobalParticles = nGlobalTravelParticles;
			bool have_bugged_particle = false;
			I bugged_particle_idx = 0;
			ParticleSimple<I,T> bugged_particle_copy;

			int num_passes = 0;
			bool first_pass = true;
			bool verbose = false;
			while(nGlobalTravelParticles > 0)
			{
				if (first_pass) {
					// std::cout << "  " << nGlobalTravelParticles << " global travellers" << std::endl;
					// std::cout << "  " << nGlobalTravelParticles << " global travellers, " << this->particles.size() << " local particles" << std::endl;
					std::cout << "Num travelling particles: global = " << nGlobalTravelParticles << ", local = " << this->getNTravelParticles() << std::endl;
					usleep(verbose_sleep_period);
				}

				have_bugged_particle = false;
				if (particles.size() > 0) {
					for (I i=0; i<particles.size(); i++) {
						if (particles[i].particleID == 1) {
							if (have_bugged_particle) {
								std::cout << "ERROR: Multiple particles have ID 1" << std::endl;
								throw std::exception();
							}
							have_bugged_particle = true;
							bugged_particle_idx = i;
							bugged_particle_copy = ParticleSimple<I,T>(particles[i]);
						}
					}
				}
				// // if (nGlobalParticles == 138 && have_bugged_particle) {
				// if (nGlobalParticles == 138 && have_bugged_particle && this->mesh->cellConnGraph->comm->rank==0) {
				// 	// The error occurs just after bugged particle is transferred from rank 1 -> 0
				// 	verbose = true;
				// }

				// Advance particles by at most one cell
				status = this->updateSystemAtomic(verbose);
				if (status != cupcfd::error::E_SUCCESS) {
					std::cout << "ERROR: updateSystemAtomic() failed" << std::endl;
					return status;
				}
								
				// No further changes should need to be made to the data stored inside a particle, and they should have ranks
				// representing the process they should be located on. 
				// Now we need to organise the particles within the system.
				// Particles that are 'dead' will have no further processing, and should be removed or handled to be treated as such
				// Particles that are to be communicated to other processes should be grouped up ready for comms
				// Particles that do not have to go off-node, but have travel time remaining, should be separated from those that have
				// no move travel time to speed up subsequent atomic steps in updating the system state (since those with no further travel
				// time should be skipped)
				
				// Remove Dead Particles (They have no further effect on the system and we don't want to exchange dead particles)
				status = this->removeInactiveParticles();
				if (status != cupcfd::error::E_SUCCESS) {
					std::cout << "ERROR: removeInactiveParticles() failed" << std::endl;
					return status;
				}
				
				// We can now perform an exchange to identify how many (if any)
				// particles will go off-rank in this atomic update, and transfer them ready for another round of atomic updates.
				status = this->exchangeParticles();
				if (status != cupcfd::error::E_SUCCESS) {
					std::cout << "ERROR: exchangeParticles() failed" << std::endl;
					return status;
				}
				
				// Cleanup any sent particles that should now be marked as inactive after being sent to another rank
				status = this->removeInactiveParticles();
				if (status != cupcfd::error::E_SUCCESS) {
					std::cout << "ERROR: removeInactiveParticles() failed" << std::endl;
					return status;
				}
				
				// Count how many are actively moving overall (to keep the loop going if needed)
				tmp = this->getNTravelParticles();
				status = cupcfd::comm::allReduceAdd(&tmp, 1, &nGlobalTravelParticles, 1, *(this->mesh->cellConnGraph->comm));
				if (status != cupcfd::error::E_SUCCESS) {
					std::cout << "ERROR: allReduceAdd() failed" << std::endl;
					return status;
				}

				// Verify that 'tmp' reflects reality:
				I trueNumTravellingParticles = 0;
				for (I i=0; i<this->particles.size(); i++) {
					if (this->particles[i].getTravelTime() > T(0)) {
						trueNumTravellingParticles++;
					}
				}
				if (trueNumTravellingParticles != tmp) {
					std::cout << "ERROR: Bug detected in stack-based tracking of #travelling particles. Stack claims " << tmp << " but actual is " << trueNumTravellingParticles << std::endl;
					return cupcfd::error::E_ERROR;
				}

				if (verbose) {
					std::cout << "Num travelling particles: global = " << nGlobalTravelParticles << ", local = " << this->getNTravelParticles() << std::endl;
				}


				if (have_bugged_particle) {
					auto bg = this->particles[bugged_particle_idx];
					if (bg.getTravelTime() > T(0)) {

						if (first_pass) {
							if (bg.getInFlightPos() == bugged_particle_copy.getInFlightPos()) {
								std::cout << "ERROR: particle " << bg.particleID << " has not moved in first pass of update" << std::endl;
								return cupcfd::error::E_ERROR;
							}
						}

						if (nGlobalTravelParticles == 1) {
							if ( (bg.getInFlightPos()  == bugged_particle_copy.getInFlightPos()) && 
							     (bg.getPos()          == bugged_particle_copy.getPos()) && 
							     (bg.getVelocity()     == bugged_particle_copy.getVelocity()) && 
								 (bg.getCellGlobalID() == bugged_particle_copy.getCellGlobalID()) )
							{
								std::cout << "ERROR: particle " << bg.particleID << " has not changed but it is only particle with travel time" << std::endl;
								return cupcfd::error::E_ERROR;
							}
						}
					}
				}

				num_passes++;
				int max_passes = nGlobalParticles * 50;
				// int max_passes = nGlobalParticles * 2;
				if (num_passes > max_passes) {
					std::cout << "ERROR: more than " << max_passes << " passes in update of system with just " << nGlobalParticles << " particles, that indicates an infinite loop bug" << std::endl;

					std::cout << "       " << nGlobalTravelParticles << " global particles still travelling:" << std::endl;
					for (I i = 0; i < particles.size(); i++) {
						if (this->particles[i].getTravelTime() > T(0)) {
							std::cout << "       > P " << particles[i].particleID << " is still travelling" << std::endl;
							// std::cout << "       > P " << particles[i].getParticleID() << " is still travelling" << std::endl;
							// std::cout << "       > > position: "; this->particles[i].getPos().print(); std::cout << std::endl;
							// std::cout << "       > > in-flight pos: "; this->particles[i].getInFlightPos().print(); std::cout << std::endl;
							// std::cout << "       > > travel time: " << this->particles[i].getTravelTime() << std::endl;
							// std::cout << "       > > active?: " << this->particles[i].getInactive() << std::endl;
							particles[i].print();
						}
					}

					return cupcfd::error::E_ERROR;
					// verbose = true;
				}

				first_pass = false;
			}

			return cupcfd::error::E_SUCCESS;
		}
		
		template <class M, class I, class T, class L>
		cupcfd::error::eCodes ParticleSystemSimple<M,I,T,L>::updateSystemAtomic(bool verbosePermitted)
		{
			cupcfd::error::eCodes status;

			useconds_t verbose_sleep_period = 100;

			// Loop over all particles in vector
			// ToDo: This approach also loops over particles that are inactive or active but have no further travel time.
			// Would be faster if inactive particles are removed
			for(I i = 0; i < this->particles.size(); i++)
			{
				// bool verbose = false;
				// bool verbose = (particles[i].getParticleID() == 8601) && verbosePermitted;
				// bool verbose = (particles[i].particleID == 8601) && verbosePermitted;
				// bool verbose = (particles[i].particleID == 4801) && verbosePermitted;
				// bool verbose = (particles[i].particleID == 101);
				bool verbose = (particles[i].particleID == 1);

				// Manually validate particle state:
				if (this->particles[i].getCellEntryFaceLocalID() == I(-1) && (this->particles[i].lastCellGlobalID != I(-1))) {
					std::cout << "ERROR: particle " << this->particles[i].particleID << " has history of cell movement but cellEntryFaceLocalID is -1" << std::endl;
					throw std::exception();
				}

				if (verbose) {
					// std::cout << "> Performing particle update" << std::endl;
					std::cout << "> Updating particle " << particles[i].particleID << std::endl;
					// std::cout << "> Updating particle " << particles[i].getParticleID() << std::endl;
					usleep(verbose_sleep_period);
				}

				// Can't guarantee there are no inactive particles so have this check guard here.
				// If we can guarantee it we can remove it however (don't really want a branch inside a loop)
				if(!(this->particles[i].getInactive()))
				{
					T stepDt;			// How much time this particle moves by in its current cell
					I localFaceID;		// The mesh local ID of the face that the particle ends up at in its current cell
					I localCellID;		// The local (not global) mesh cell ID that the particle is currently in
					I cellNode;			// The node value in the connectivity graph of the cell that the particle is currently in
										// (Needed to map between local ID and global ID since this label is arbitrary when building)

					I cellGlobalID = this->particles[i].getCellGlobalID();
				
					// Get the Local Cell ID since ParticleSimple only stores the Mesh Global Cell ID
					// cellNode = this->mesh->cellConnGraph->globalToNode[this->particles[i].cellGlobalID];
					cellNode = this->mesh->cellConnGraph->globalToNode[cellGlobalID];
					status = this->mesh->cellConnGraph->connGraph.getNodeLocalIndex(cellNode, &localCellID);
					if (status != cupcfd::error::E_SUCCESS) {
						std::cout << "ERROR: getNodeLocalIndex() failed" << std::endl;
						return status;
					}

					// if (verbose) {
					// 	this->particles[i].print();
					// 	usleep(verbose_sleep_period);
					// }
					
					// Note: For particles with no further travel time, the following steps must not change the state
					// of the particle and system.
					
					// Perform an atomic positional update for every particle in the system, advancing them by at most one cell
					// This will also update the travel time
					auto pos_before_mov = cupcfd::geometry::euclidean::EuclideanPoint<T,3>(this->particles[i].getInFlightPos());
					// status = this->particles[i].updatePositionAtomic(*(this->mesh), &stepDt, &localFaceID);
					// status = this->particles[i].updatePositionAtomic(*(this->mesh), &stepDt, &localFaceID, false);
					status = this->particles[i].updatePositionAtomic(*(this->mesh), &stepDt, &localFaceID, verbose);
					// status = this->particles[i].updatePositionAtomic(*(this->mesh), &stepDt, &localFaceID, true);
					// try {
					// 	status = this->particles[i].updatePositionAtomic(*(this->mesh), &stepDt, &localFaceID, verbose);
					// } catch (...) {
					// 	std::cout << "Caught exception, repeating operation with vebose enabled" << std::endl;
					// 	// Repeat with verbose enabled
					// 	status = this->particles[i].updatePositionAtomic(*(this->mesh), &stepDt, &localFaceID, true);
					// }
					if (status != cupcfd::error::E_SUCCESS) {
						std::cout << "ERROR: updatePositionAtomic() failed" << std::endl;
						return status;
					}
					auto pos_after_mov = this->particles[i].getInFlightPos();
					// if (verbose) {
					// 	// std::cout << "  > post-flight state:" << std::endl;
					// 	// bool movement = false;
					// 	// for(int i=0; i<3; i++) {
					// 	// 	if(!(cupcfd::utility::arithmetic::kernels::isEqual(pos_before_mov.cmp[i], pos_after_mov.cmp[i]))) {
					// 	// 		movement = true;
					// 	// 	}
					// 	// }
					// 	this->particles[i].print();
					// 	usleep(verbose_sleep_period);
					// }
	
					// Perform an atomic velocity update for every particle in the system, reflecting the time they have advanced by
					status = this->particles[i].updateVelocityAtomic(*(this->mesh), localCellID, stepDt);
					if (status != cupcfd::error::E_SUCCESS) {
						std::cout << "ERROR: updateVelocityAtomic() failed" << std::endl;
						return status;
					}
					
					// Perform a state update, updating both particle state (e.g. change in properties specific to a particle,
					// such as mass) and updating cell source values resulting from changes in the particle.
					status = this->particles[i].updateStateAtomic(*(this->mesh), localCellID, stepDt);
					if (status != cupcfd::error::E_SUCCESS) {
						std::cout << "ERROR: updateStateAtomic() failed" << std::endl;
						return status;
					}

					// Check that it reached a face after moving
					if(!(localFaceID == -1) && stepDt > T(0))
					{
						// Perform a face update - any cells that are trying to exit a cell are currently positioned at a face (boundary
						// or non-boundary). This step updates their cell ID (if they are leaving a cell), their rank (if they are
						// going into a position owned by a different process in the mesh), and/or any other quantities that might be
						// applicable depending upon the face (e.g. changing velocity direction in the event of a reflective boundary)

						// if (verbose) {
						// 	std::cout << "  > > performing face update for particle " << particles[i].getParticleID() << std::endl;
						// }
						
						// Is the face a boundary
						if(!this->mesh->getFaceIsBoundary(localFaceID))
						{
							// I globalCellIdBefore = this->particles[i].cellGlobalID;
							I globalCellIdBefore = this->particles[i].getCellGlobalID();
							// I globalCellIdBefore = cellGlobalID;
							status = this->particles[i].updateNonBoundaryFace(*(this->mesh), localFaceID);
							// I globalCellIdAfter = this->particles[i].cellGlobalID;
							I globalCellIdAfter = this->particles[i].getCellGlobalID();
							if (verbose) {
								std::cout << "  > > moved from cell " << globalCellIdBefore << " --> " << globalCellIdAfter << std::endl;
							}
							if (status != cupcfd::error::E_SUCCESS) {
								std::cout << "ERROR: updateNonBoundaryFace() failed" << std::endl;
								return status;
							}
						}
						else
						{
							// I globalCellIdBefore = this->particles[i].cellGlobalID;
							I globalCellIdBefore = this->particles[i].getCellGlobalID();
							I boundaryID = this->mesh->getFaceBoundaryID(localFaceID);
							I regionID = this->mesh->getBoundaryRegionID(boundaryID);
							cupcfd::geometry::mesh::RType boundaryType = this->mesh->getRegionType(regionID);
							
							switch(boundaryType)
							{
								case(cupcfd::geometry::mesh::RTYPE_WALL):
									status = this->particles[i].updateBoundaryFaceWall(*(this->mesh), localCellID, localFaceID);
									break;
								
								case(cupcfd::geometry::mesh::RTYPE_INLET):
									status = this->particles[i].updateBoundaryFaceInlet(*(this->mesh), localCellID, localFaceID);
									break;
																					
								case(cupcfd::geometry::mesh::RTYPE_OUTLET):
									status = this->particles[i].updateBoundaryFaceOutlet(*(this->mesh), localCellID, localFaceID);
									break;
														
								case(cupcfd::geometry::mesh::RTYPE_SYMP):
									status = this->particles[i].updateBoundaryFaceSymp(*(this->mesh), localCellID, localFaceID);
									break;
																					
								// Unexpected case - treat like a wall?
								case(cupcfd::geometry::mesh::RTYPE_DEFAULT):
									status = this->particles[i].updateBoundaryFaceWall(*(this->mesh), localCellID, localFaceID);
									break;
																					
								// Unexpected case - treat like a wall?
								default:
									status = this->particles[i].updateBoundaryFaceWall(*(this->mesh), localCellID, localFaceID);
									break;										
							}
							// I globalCellIdAfter = this->particles[i].cellGlobalID;
							I globalCellIdAfter = this->particles[i].getCellGlobalID();
							if (verbose) {
								// std::cout << "  > > > P " << i << " reflecting in boundary cell " << globalCellIdAfter << std::endl;
								std::cout << "  > reflecting in bnd cell " << globalCellIdAfter << std::endl;
							}

							if (status != cupcfd::error::E_SUCCESS) {
								std::cout << "ERROR: updateBoundaryFace*() failed" << std::endl;
								return status;
							}
						}

						if (verbose) {
							usleep(verbose_sleep_period);
						}
					}
										
					// If this particle has no further travel time (but did move this step) then decrease the number
					// of travelling particles
					if(!(this->particles[i].getTravelTime() > T(0)) && stepDt > T(0))
					{
						if (verbose) {
							std::cout << "  > P " << particles[i].particleID << " has no travel time left" << std::endl;
							// std::cout << "  > > P " << particles[i].getParticleID() << " has no travel time left" << std::endl;
							usleep(verbose_sleep_period);
						}
						this->nTravelParticles = this->nTravelParticles - 1;

						if (this->nTravelParticles < 0) {
							std::cout << "ERROR: nTravelParticles has dropped below 0" << std::endl;
							throw std::exception();
						}
					}
				}
				else {
					std::cout << "ERROR: Attempting to update an inactive particle" << std::endl;
					return cupcfd::error::E_ERROR;
				}
			}
				
			return cupcfd::error::E_SUCCESS;
		}
		
		template <class M, class I, class T, class L>
		I ParticleSystemSimple<M,I,T,L>::getNEmitters()
		{
			return this->emitters.size();
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
			cupcfd::error::eCodes status;

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

			return cupcfd::error::E_SUCCESS;
		}
		
		template <class M, class I, class T, class L>
		cupcfd::error::eCodes ParticleSystemSimple<M,I,T,L>::generateEmitterParticles(T dt)
		{
			cupcfd::error::eCodes status;

			for(I i = 0; i < this->emitters.size(); i++)
			{
				ParticleSimple<I,T> * newParticles;
				I nNewParticles = 0;
				
				status = this->emitters[i].generateParticles(&newParticles, &nNewParticles, dt);
				if (status != cupcfd::error::E_SUCCESS) {
					std::cout << "ERROR: generateParticles() failed" << std::endl;
					return status;
				}
				
				for(I j = 0; j < nNewParticles; j++)
				{
					// Check that new particle does not already exist:
					for (I k=0; k<this->particles.size(); k++) {
						if (this->particles[k].particleID == newParticles[j].particleID) {
							std::cout << "ERROR: Particle with ID " << newParticles[j].particleID << " already in system" << std::endl;
							return cupcfd::error::E_ERROR;
						}
					}

					this->addParticle(newParticles[j]);
				}
				
				free(newParticles);
			}

			return cupcfd::error::E_SUCCESS;
		}
	}
}

#endif
