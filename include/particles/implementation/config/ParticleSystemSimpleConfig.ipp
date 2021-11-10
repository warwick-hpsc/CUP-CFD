/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Contains header level definitions for the ParticleSystemSimpleConfig class.
 */

#ifndef CUPCFD_PARTICLES_CONFIG_PARTICLE_SYSTEM_SIMPLE_CONFIG_IPP_H
#define CUPCFD_PARTICLES_CONFIG_PARTICLE_SYSTEM_SIMPLE_CONFIG_IPP_H

#include "ParticleSimpleSourceFileConfig.h"

namespace cupcfd
{
	namespace particles
	{
		template <class M, class I, class T, class L>
		ParticleSystemSimpleConfig<M,I,T,L>::ParticleSystemSimpleConfig(std::vector<ParticleEmitterConfig<ParticleEmitterSimple<I,T>, ParticleSimple<I,T>,I,T> *>& emitterConfigs,
																	    ParticleSourceConfig<ParticleSimple<I,T>, I, T> * particleSourceConfig)
		: ParticleSystemConfig<ParticleSystemSimple<M,I,T,L>, ParticleEmitterSimple<I,T>, ParticleSimple<I,T>, M,I,T,L>()
		{
			I size = cupcfd::utility::drivers::safeConvertSizeT<I>(emitterConfigs.size());
			for(I i = 0; i < size; i++) {
				this->emitterConfigs.push_back(emitterConfigs[i]->clone());
			}
			
			if(particleSourceConfig == nullptr) {			
				this->particleSourceConfig = nullptr;
			}
			else {	
				this->particleSourceConfig = particleSourceConfig->clone();
			}

			this->numParticleSourcesOrEmitters = 0;
		}

		template <class M, class I, class T, class L>
		ParticleSystemSimpleConfig<M,I,T,L>::ParticleSystemSimpleConfig(ParticleSystemSimpleConfig<M,I,T,L>& source)
		{
			*this = source;
		}

		template <class M, class I, class T, class L>
		ParticleSystemSimpleConfig<M,I,T,L>::~ParticleSystemSimpleConfig() {
			I size = cupcfd::utility::drivers::safeConvertSizeT<I>(this->emitterConfigs.size());
			for(I i = 0; i < size; i++) {
				delete this->emitterConfigs[i];
			}
			
			if(this->particleSourceConfig != nullptr) {
				delete this->particleSourceConfig;
			}
		}

		template <class M, class I, class T, class L>
		void ParticleSystemSimpleConfig<M,I,T,L>::operator=(ParticleSystemSimpleConfig<M,I,T,L>& source) {
			I size = cupcfd::utility::drivers::safeConvertSizeT<I>(source.emitterConfigs.size());
			for(I i = 0; i < size; i++) {
				this->emitterConfigs.push_back(source.emitterConfigs[i]->clone());
			}
			
			if (source.particleSourceConfig != nullptr) {
				this->particleSourceConfig = source.particleSourceConfig->clone();
			} else {
				this->particleSourceConfig = nullptr;
			}
		}

		template <class M, class I, class T, class L>
		ParticleSystemSimpleConfig<M,I,T,L> * ParticleSystemSimpleConfig<M,I,T,L>::clone() {
			return new ParticleSystemSimpleConfig<M,I,T,L>(*this);
		}

		template <class M, class I, class T, class L>
		cupcfd::error::eCodes ParticleSystemSimpleConfig<M,I,T,L>::buildParticleSystem(ParticleSystem<ParticleSystemSimple<M, I, T, L>, ParticleEmitterSimple<I,T>, ParticleSimple<I,T>, M, I, T, L> ** system,
																							std::shared_ptr<M> meshPtr) {
			cupcfd::error::eCodes status;
		
			// Build the initial system
			*system = new ParticleSystemSimple<M,I,T,L>(meshPtr);
			
			// For each emitter, check whether it belongs to a cell on this rank in the mesh. If it does, add it to the systemI size;
			I size = cupcfd::utility::drivers::safeConvertSizeT<I>(this->emitterConfigs.size());
			for(I i = 0; i < size; i++) {
				// Search through the local cells on this rank for the position specified in the emitter configuration
				bool onRank = false;
				
				I localCellID;
				I globalCellID;
				I rank = meshPtr->cellConnGraph->comm->rank;
				
				status = meshPtr->findCellID(this->emitterConfigs[i]->position, &localCellID, &globalCellID);
			
				// ToDo: If exactly on a local - ghost face/edge/vertex, then it could potentially be assigned to more than one rank.
				// We do not want this duplication.
				// Approaches? - Could do a count allreduce
			
				if(status == cupcfd::error::E_SUCCESS) {
					onRank = true;
				}
			
				// If found, build the emitter and set the rank, localCellID and globalCellID
				if(onRank) {
					ParticleEmitter<ParticleEmitterSimple<I,T>, ParticleSimple<I,T>, I, T> * emitter;
					status = this->emitterConfigs[i]->buildParticleEmitter(&emitter, this->numParticleSourcesOrEmitters+1);
					this->numParticleSourcesOrEmitters++;
					CHECK_ECODE(status)

					emitter->localCellID = localCellID;
					emitter->globalCellID = globalCellID;
					emitter->rank = rank;

					// ToDo: Shouldn't really typecast this here, though it should hold up for now.
					// Need to fix templates around these types since the CRTP interface passes around more generic types,
					// and since we're not returning directly I think this makes it more difficult to handle them, 
					// when we really want very concrete types for e.g. adding Particles.
					// We could do away with the interface as one approach, resolving the issue....
					status = (*system)->addParticleEmitter( *(static_cast<ParticleEmitterSimple<I,T> *>(emitter)));
					CHECK_ECODE(status)
				
					delete emitter;
				}
				
			}
			
			// Once Particle Sources are done, repeat the same process for adding particles when we add a particle source to this method
			// ToDo: Since we have to check whether every particle is in any of this ranks cells, this could get very expensive (m * n where both
			// m and n could be large). Storing rank data doesn't work if the decomposition could change between loads - consider alternatives?
			
			if(this->particleSourceConfig != nullptr) {
				// Configuration exists - Build Particle Source Object
				ParticleSource<ParticleSimple<I,T>,I,T> * particleSource;

				status = this->particleSourceConfig->buildParticleSource(&particleSource, this->numParticleSourcesOrEmitters+1);
				this->numParticleSourcesOrEmitters++;
				CHECK_ECODE(status)
				
				// Retrieve Particle Data From Source
				I nIndexes;
				I nParticles;
				
				status = particleSource->getNParticles(&nIndexes);
				CHECK_ECODE(status)
				
				I * indexes = (I *) malloc(sizeof(I) * nIndexes);
				
				for(I i = 0; i < nIndexes; i++) {
					indexes[i] = i;
				}
				
				Particle<ParticleSimple<I,T>,I,T> ** particles;
				status = particleSource->getParticles(&particles, &nParticles, indexes, nIndexes, 0);
				CHECK_ECODE(status)

				// Add particles to the system, but only if they exist on this ranks mesh partition
				for(I i = 0; i < nParticles; i++) {
					ParticleSimple<I,T> p = *(static_cast<ParticleSimple<I,T> *>(particles[i]));

					// Find the cell this particle exists in on this rank, if it exists
					I localCellID;
					I globalCellID;
					
					
					cupcfd::geometry::euclidean::EuclideanPoint<T,3> pos = p.getPos();
					
					status = meshPtr->findCellID(pos, &localCellID, &globalCellID);
					
					if(status == cupcfd::error::E_SUCCESS) {
						ParticleSimple<I,T> allocatedParticle = 
							ParticleSimple<I,T>(
								p.pos,
								p.velocity,
								p.acceleration,
								p.jerk,
								p.getParticleID(),
								globalCellID,
								meshPtr->cellConnGraph->comm->rank,
								p.decayLevel,
								p.decayRate,
								p.travelDt);
						allocatedParticle.inflightPos = p.pos;
						status = (*system)->addParticle(allocatedParticle);

						CHECK_ECODE(status)
					}
				}
				
				// Cleanup
				// Destroy particle data retrieved from object
				for( I i = 0; i < nParticles; i++) {
					delete(particles[i]);
				}
				
				// Destroy particle source object
				delete(particleSource);
			}		
			
			// Done
			return cupcfd::error::E_SUCCESS;
		}
	}
}

#endif
