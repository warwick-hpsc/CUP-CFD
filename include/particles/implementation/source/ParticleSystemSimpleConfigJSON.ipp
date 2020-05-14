/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Contains header level definitions for the ParticleSystemSimpleConfigJSON class.
 */

#ifndef CUPCFD_PARTICLES_CONFIG_PARTICLE_SYSTEM_SIMPLE_CONFIG_SOURCE_JSON_IPP_H
#define CUPCFD_PARTICLES_CONFIG_PARTICLE_SYSTEM_SIMPLE_CONFIG_SOURCE_JSON_IPP_H

#include <fstream>
#include "ParticleEmitterSimpleConfigJSON.h"
#include "ParticleSimpleSourceFileConfigJSON.h"

namespace cupcfd
{
	namespace particles
	{	
		template <class M, class I, class T, class L>
		ParticleSystemSimpleConfigJSON<M,I,T,L>::ParticleSystemSimpleConfigJSON(Json::Value& configData)
		:configData(configData)
		{
		
		}

		template <class M, class I, class T, class L>
		ParticleSystemSimpleConfigJSON<M,I,T,L>::ParticleSystemSimpleConfigJSON(ParticleSystemSimpleConfigJSON<M,I,T,L>& source)
		{
			*this = source;
		}

		template <class M, class I, class T, class L>
		ParticleSystemSimpleConfigJSON<M,I,T,L>::~ParticleSystemSimpleConfigJSON()
		{

		}

		template <class M, class I, class T, class L>
		void ParticleSystemSimpleConfigJSON<M,I,T,L>::operator=(ParticleSystemSimpleConfigJSON<M,I,T,L>& source)
		{
			this->configData = source.configData;
		}

		template <class M, class I, class T, class L>
		ParticleSystemSimpleConfigJSON<M,I,T,L> * ParticleSystemSimpleConfigJSON<M,I,T,L>::clone()
		{
			return new ParticleSystemSimpleConfigJSON<M,I,T,L>(*this);
		}
		
		template <class M, class I, class T, class L>
		cupcfd::error::eCodes ParticleSystemSimpleConfigJSON<M,I,T,L>::getParticleEmitterConfigs(std::vector<ParticleEmitterConfig<ParticleEmitterSimple<I,T>, ParticleSimple<I,T>, I, T> *>& configs)
		{
			cupcfd::error::eCodes status;
		
			// If there any emitter configs, expect in array format.
			// However, it is not required that there are any emitters (but none will be added to the system if this is the case)

			if(this->configData.isMember("Emitters"))
			{
				for(I i = 0; i < this->configData["Emitters"].size(); i++)
				{
					// Only accept ParticleEmitterSimple emitters for now
					if(this->configData["Emitters"][i].isMember("ParticleEmitterSimple"))
					{
						ParticleEmitterSimpleConfigJSON<I,T> emitterConfigJSON(this->configData["Emitters"][i]["ParticleEmitterSimple"]);
						ParticleEmitterConfig<ParticleEmitterSimple<I,T>, ParticleSimple<I,T>, I, T> * emitterConfig;
						
						// Error out if we have a incorrect ParticleEmitterSimple JSON format
						status = emitterConfigJSON.buildParticleEmitterConfig(&emitterConfig);
						if(status != cupcfd::error::E_SUCCESS)
						{
							std::cout << "ERROR: buildParticleEmitterConfig() failed" << std::endl;
							return status;
						}
						
						configs.push_back(emitterConfig);
					}
				}
			}
			else
			{
				return cupcfd::error::E_CONFIG_OPT_NOT_FOUND;
			}
			
			return cupcfd::error::E_SUCCESS;
		}
		
		template <class M, class I, class T, class L>
		cupcfd::error::eCodes ParticleSystemSimpleConfigJSON<M,I,T,L>::getParticleSourceConfig(ParticleSourceConfig<ParticleSimple<I,T>, I, T> ** particleSourceConfig)
		{
			cupcfd::error::eCodes status;
			
			if(this->configData.isMember("ParticleSourceSimple"))
			{
				// Try each type of source
			
				ParticleSimpleSourceFileConfigJSON<I,T> particleSourceConfigJSON(this->configData["ParticleSourceSimple"]);
				status = particleSourceConfigJSON.buildParticleSourceConfig(particleSourceConfig);
				if(status != cupcfd::error::E_SUCCESS)
				{
					// No more types to try, so return with error
					std::cout << "ERROR: buildParticleSourceConfig() failed" << std::endl;
					return status;
				}
			}
			else
			{
				return cupcfd::error::E_CONFIG_OPT_NOT_FOUND;
			}
			
			return cupcfd::error::E_SUCCESS;
		}

		template <class M, class I, class T, class L>
		cupcfd::error::eCodes ParticleSystemSimpleConfigJSON<M,I,T,L>::buildParticleSystemConfig(ParticleSystemConfig<ParticleSystemSimple<M,I,T,L>, ParticleEmitterSimple<I,T>, ParticleSimple<I,T>, M,I,T,L> ** config)
		{
			cupcfd::error::eCodes status;
			std::vector<ParticleEmitterConfig<ParticleEmitterSimple<I,T>, ParticleSimple<I,T>, I, T> *> emitterConfigs;
			ParticleSourceConfig<ParticleSimple<I,T>, I, T> * particleSourceConfig;
			
			// Retrieve the emitter configs specified by the JSON
			// Required
			status = this->getParticleEmitterConfigs(emitterConfigs);
			if(status != cupcfd::error::E_SUCCESS)
			{
				std::cout << "ERROR: getParticleEmitterConfigs() failed" << std::endl;
				return status;
			}
			
			// Get a particle source (optional)
			status = getParticleSourceConfig(&particleSourceConfig);
			if(status != cupcfd::error::E_SUCCESS)
			{
				particleSourceConfig = nullptr;
			}

			// Pass the emitter configs + particle source to a new Particle System
			*config = new ParticleSystemSimpleConfig<M,I,T,L>(emitterConfigs, particleSourceConfig);
			
			// Cleanup the vector of pointers - likely a nicer way to do object passing but templates + inheritance 
			// causing headaches when using plain vector of objects...
			
			for(I i = 0; i < emitterConfigs.size(); i++)
			{
				delete(emitterConfigs[i]);
			}
			
			if(particleSourceConfig != nullptr)
			{
				delete(particleSourceConfig);
			}
			
			return cupcfd::error::E_SUCCESS;
		}
	}
}

#endif
