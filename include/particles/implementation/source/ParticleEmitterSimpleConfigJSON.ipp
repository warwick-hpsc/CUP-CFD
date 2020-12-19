/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Contains header level definitions for the ParticleEmitterSimpleConfigJSON class.
 */

#ifndef CUPCFD_PARTICLES_CONFIG_PARTICLE_EMITTER_SIMPLE_CONFIG_SOURCE_JSON_IPP_H
#define CUPCFD_PARTICLES_CONFIG_PARTICLE_EMITTER_SIMPLE_CONFIG_SOURCE_JSON_IPP_H

#include "DistributionConfigSourceFixed.h"
#include "DistributionConfigSourceNormal.h"
#include "DistributionConfigSourceUniform.h"

#include "DistributionConfigSourceFixedJSON.h"
#include "DistributionConfigSourceNormalJSON.h"
#include "DistributionConfigSourceUniformJSON.h"

#include "ParticleEmitterSimpleConfig.h"

#include <fstream>

namespace cupcfd
{
	namespace particles
	{	
		template <class I, class T>
		ParticleEmitterSimpleConfigJSON<I,T>::ParticleEmitterSimpleConfigJSON(Json::Value& parseJSON)
		{
			this->configData = parseJSON;
		}

		template <class I, class T>
		ParticleEmitterSimpleConfigJSON<I,T>::ParticleEmitterSimpleConfigJSON(ParticleEmitterSimpleConfigJSON<I,T>& source)
		{
			*this = source;
		}

		template <class I, class T>
		ParticleEmitterSimpleConfigJSON<I,T>::~ParticleEmitterSimpleConfigJSON()
		{

		}

		template <class I, class T>
		void ParticleEmitterSimpleConfigJSON<I,T>::operator=(ParticleEmitterSimpleConfigJSON<I,T>& source) {
			this->configData = source.configData;
		}

		template <class I, class T>
		ParticleEmitterSimpleConfigJSON<I,T> * ParticleEmitterSimpleConfigJSON<I,T>::clone() {
			return new ParticleEmitterSimpleConfigJSON<I,T>(*this);
		}
		
		template <class I, class T>
		cupcfd::error::eCodes ParticleEmitterSimpleConfigJSON<I,T>::getEmitterPosition(cupcfd::geometry::euclidean::EuclideanPoint<T,3>& pos) {
			T posX, posY, posZ;
		
			if(this->configData.isMember("Position")) {
				Json::Value dataSourceType;
	
				// Need all three dimensional components of the position
	
				if(this->configData["Position"].isMember("X")) {
					// Access the X field
					dataSourceType = this->configData["Position"]["X"];
					posX = T(dataSourceType.asDouble());
				}
				else {
					return cupcfd::error::E_CONFIG_OPT_NOT_FOUND;
				}
				
				if(this->configData["Position"].isMember("Y")) {
					// Access the Y field
					dataSourceType = this->configData["Position"]["Y"];
					posY = T(dataSourceType.asDouble());
				}
				else {
					return cupcfd::error::E_CONFIG_OPT_NOT_FOUND;
				}	
				
				if(this->configData["Position"].isMember("Z")) {
					// Access the Z field
					dataSourceType = this->configData["Position"]["Z"];
					posZ = T(dataSourceType.asDouble());
				}
				else {
					return cupcfd::error::E_CONFIG_OPT_NOT_FOUND;
				}
				
				// ToDo: Check they are valid doubles?
				
				pos.cmp[0] = posX;
				pos.cmp[1] = posY;
				pos.cmp[2] = posZ;
				
				return cupcfd::error::E_SUCCESS;
			}

			return cupcfd::error::E_CONFIG_OPT_NOT_FOUND;
		}
		
		template <class I, class T>
		cupcfd::error::eCodes ParticleEmitterSimpleConfigJSON<I,T>::getRateDistributionConfig(cupcfd::distributions::DistributionConfig<I,T> ** distConfig) {
			return this->getParticleEmitterDistributionConfig("Rate", distConfig);
		}

		template <class I, class T>
		cupcfd::error::eCodes ParticleEmitterSimpleConfigJSON<I,T>::getAngleXYDistributionConfig(cupcfd::distributions::DistributionConfig<I,T> ** distConfig) {
			return this->getParticleEmitterDistributionConfig("AngleXY", distConfig);
		}

		template <class I, class T>
		cupcfd::error::eCodes ParticleEmitterSimpleConfigJSON<I,T>::getAngleRotationDistributionConfig(cupcfd::distributions::DistributionConfig<I,T> ** distConfig) {
			return this->getParticleEmitterDistributionConfig("AngleRotation", distConfig);
		}

		template <class I, class T>
		cupcfd::error::eCodes ParticleEmitterSimpleConfigJSON<I,T>::getSpeedDistributionConfig(cupcfd::distributions::DistributionConfig<I,T> ** distConfig) {
			return this->getParticleEmitterDistributionConfig("Speed", distConfig);
		}

		template <class I, class T>
		cupcfd::error::eCodes ParticleEmitterSimpleConfigJSON<I,T>::getAccelerationXDistributionConfig(cupcfd::distributions::DistributionConfig<I,T> ** distConfig) {
			return this->getParticleEmitterDistributionConfig("AccelerationX", distConfig);
		}

		template <class I, class T>
		cupcfd::error::eCodes ParticleEmitterSimpleConfigJSON<I,T>::getAccelerationYDistributionConfig(cupcfd::distributions::DistributionConfig<I,T> ** distConfig) {
			return this->getParticleEmitterDistributionConfig("AccelerationY", distConfig);
		}

		template <class I, class T>
		cupcfd::error::eCodes ParticleEmitterSimpleConfigJSON<I,T>::getAccelerationZDistributionConfig(cupcfd::distributions::DistributionConfig<I,T> ** distConfig) {
			return this->getParticleEmitterDistributionConfig("AccelerationZ", distConfig);
		}

		template <class I, class T>
		cupcfd::error::eCodes ParticleEmitterSimpleConfigJSON<I,T>::getJerkXDistributionConfig(cupcfd::distributions::DistributionConfig<I,T> ** distConfig) {
			return this->getParticleEmitterDistributionConfig("JerkX", distConfig);
		}

		template <class I, class T>
		cupcfd::error::eCodes ParticleEmitterSimpleConfigJSON<I,T>::getJerkYDistributionConfig(cupcfd::distributions::DistributionConfig<I,T> ** distConfig) {
			return this->getParticleEmitterDistributionConfig("JerkY", distConfig);
		}

		template <class I, class T>
		cupcfd::error::eCodes ParticleEmitterSimpleConfigJSON<I,T>::getJerkZDistributionConfig(cupcfd::distributions::DistributionConfig<I,T> ** distConfig) {
			return this->getParticleEmitterDistributionConfig("JerkZ", distConfig);
		}

		template <class I, class T>
		cupcfd::error::eCodes ParticleEmitterSimpleConfigJSON<I,T>::getDecayRateDistributionConfig(cupcfd::distributions::DistributionConfig<I,T> ** distConfig) {
			return this->getParticleEmitterDistributionConfig("DecayRate", distConfig);
		}

		template <class I, class T>
		cupcfd::error::eCodes ParticleEmitterSimpleConfigJSON<I,T>::getDecayThresholdDistributionConfig(cupcfd::distributions::DistributionConfig<I,T> ** distConfig) {
			return this->getParticleEmitterDistributionConfig("DecayThreshold", distConfig);
		}

		template <class I, class T>
		cupcfd::error::eCodes ParticleEmitterSimpleConfigJSON<I,T>::getParticleEmitterDistributionConfig(std::string fieldName, cupcfd::distributions::DistributionConfig<I,T> ** distConfig) {
			// Iterate through JSON formats for different potential distribution types
			// Test each to see if a distribution can be built - if it can a viable format has been found.
				
			if(this->configData.isMember(fieldName)) {
				if(this->configData[fieldName].isMember("FixedDistribution")) {
					cupcfd::distributions::DistributionConfigSourceFixedJSON<I,T> test1(this->configData[fieldName]["FixedDistribution"]);
					return test1.buildDistributionConfig(distConfig);
				}
				
				if(this->configData[fieldName].isMember("NormalDistribution")) {
					cupcfd::distributions::DistributionConfigSourceNormalJSON<I,T> test1(this->configData[fieldName]["NormalDistribution"]);
					return test1.buildDistributionConfig(distConfig);
				}
				
				if(this->configData[fieldName].isMember("UniformDistribution")) {
					cupcfd::distributions::DistributionConfigSourceNormalJSON<I,T> test1(this->configData[fieldName]["UniformDistribution"]);
					return test1.buildDistributionConfig(distConfig);
				}

				// No matching field names were found
				return cupcfd::error::E_CONFIG_OPT_NOT_FOUND;
			}
			
			// No viable JSON found for a distribution
			std::cout << "ERROR: Field '" << fieldName << "' not found in ParticleEmitterSimple's json" << std::endl;
			return cupcfd::error::E_CONFIG_OPT_NOT_FOUND;
		}

		template <class I, class T>
		cupcfd::error::eCodes ParticleEmitterSimpleConfigJSON<I,T>::buildParticleEmitterConfig(ParticleEmitterConfig<ParticleEmitterSimple<I,T>, ParticleSimple<I,T>, I, T> ** config) {
			cupcfd::error::eCodes status;
			
			// Position
			cupcfd::geometry::euclidean::EuclideanPoint<T,3> pos;
			
			// Get each of the distributions
			cupcfd::distributions::DistributionConfig<I,T> * rate;
			cupcfd::distributions::DistributionConfig<I,T> * angleXY;
			cupcfd::distributions::DistributionConfig<I,T> * angleRotation;
			cupcfd::distributions::DistributionConfig<I,T> * speed;
			cupcfd::distributions::DistributionConfig<I,T> * accelerationX;
			cupcfd::distributions::DistributionConfig<I,T> * accelerationY;
			cupcfd::distributions::DistributionConfig<I,T> * accelerationZ;
			cupcfd::distributions::DistributionConfig<I,T> * jerkX;
			cupcfd::distributions::DistributionConfig<I,T> * jerkY;
			cupcfd::distributions::DistributionConfig<I,T> * jerkZ;
			cupcfd::distributions::DistributionConfig<I,T> * decayRate;
			cupcfd::distributions::DistributionConfig<I,T> * decayThreshold;
			
			// Position
			status = this->getEmitterPosition(pos);
			CHECK_ERROR_CODE(status)
			if(status != cupcfd::error::E_SUCCESS) return status;
			
			// Rate
			status = this->getRateDistributionConfig(&rate);
			CHECK_ERROR_CODE(status)
			if(status != cupcfd::error::E_SUCCESS) return status;
			
			// Angle XY
			status = this->getAngleXYDistributionConfig(&angleXY);
			CHECK_ERROR_CODE(status)
			if(status != cupcfd::error::E_SUCCESS) return status;
			
			// Angle Rotation
			status = this->getAngleRotationDistributionConfig(&angleRotation);
			CHECK_ERROR_CODE(status)
			if(status != cupcfd::error::E_SUCCESS) return status;
			
			// Speed
			status = this->getSpeedDistributionConfig(&speed);
			CHECK_ERROR_CODE(status)
			if(status != cupcfd::error::E_SUCCESS) return status;
			
			// Acceleration X
			status = this->getAccelerationXDistributionConfig(&accelerationX);
			CHECK_ERROR_CODE(status)
			if(status != cupcfd::error::E_SUCCESS) return status;
			
			// Acceleration Y
			status = this->getAccelerationYDistributionConfig(&accelerationY);
			CHECK_ERROR_CODE(status)
			if(status != cupcfd::error::E_SUCCESS) return status;
			
			// Acceleration Z
			status = this->getAccelerationZDistributionConfig(&accelerationZ);
			CHECK_ERROR_CODE(status)
			if(status != cupcfd::error::E_SUCCESS) return status;
			
			// Jerk X
			status = this->getJerkXDistributionConfig(&jerkX);
			CHECK_ERROR_CODE(status)
			if(status != cupcfd::error::E_SUCCESS) return status;
			
			// Jerk Y
			status = this->getJerkYDistributionConfig(&jerkY);
			CHECK_ERROR_CODE(status)
			if(status != cupcfd::error::E_SUCCESS) return status;
			
			// Jerk Z
			status = this->getJerkZDistributionConfig(&jerkZ);
			CHECK_ERROR_CODE(status)
			if(status != cupcfd::error::E_SUCCESS) return status;
			
			// Decay Rate
			status = this->getDecayRateDistributionConfig(&decayRate);
			CHECK_ERROR_CODE(status)
			if(status != cupcfd::error::E_SUCCESS) return status;
			
			// Decay Threshold
			status = this->getDecayThresholdDistributionConfig(&decayThreshold);
			CHECK_ERROR_CODE(status)
			if(status != cupcfd::error::E_SUCCESS) return status;
			
			*config = new ParticleEmitterSimpleConfig<I,T>(pos, *rate, *angleXY, *angleRotation, *speed, *accelerationX, *accelerationY, *accelerationZ,
														   *jerkX, *jerkY, *jerkZ, *decayRate, *decayThreshold);
														   
			delete(rate);
			delete(angleXY);
			delete(angleRotation);
			delete(speed);
			delete(accelerationX);
			delete(accelerationY);
			delete(accelerationZ);
			delete(jerkX);
			delete(jerkY);
			delete(jerkZ);
			delete(decayRate);
			delete(decayThreshold);
																									 
			return cupcfd::error::E_SUCCESS;
		}
	}
}

#endif
