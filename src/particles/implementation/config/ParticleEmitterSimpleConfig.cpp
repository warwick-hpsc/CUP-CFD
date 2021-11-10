/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Contains definitions for the ParticleEmitterSimpleConfig class.
 */

#include "ParticleEmitterSimpleConfig.h"
#include "Distribution.h"
#include "ParticleEmitterSimple.h"

namespace cupcfd
{
	namespace particles
	{
		template <class I, class T>
		ParticleEmitterSimpleConfig<I,T>::ParticleEmitterSimpleConfig(cupcfd::geometry::euclidean::EuclideanPoint<T,3>& position,
																	  cupcfd::distributions::DistributionConfig<I,T>& rate,
																	  cupcfd::distributions::DistributionConfig<I,T>& angleXY,
																	  cupcfd::distributions::DistributionConfig<I,T>& angleRotation,
																	  cupcfd::distributions::DistributionConfig<I,T>& speed,
																	  cupcfd::distributions::DistributionConfig<I,T>& accelerationX,
																	  cupcfd::distributions::DistributionConfig<I,T>& accelerationY,
																	  cupcfd::distributions::DistributionConfig<I,T>& accelerationZ,
																	  cupcfd::distributions::DistributionConfig<I,T>& jerkX,
																	  cupcfd::distributions::DistributionConfig<I,T>& jerkY,
																	  cupcfd::distributions::DistributionConfig<I,T>& jerkZ,
																	  cupcfd::distributions::DistributionConfig<I,T>& decayRate,
																	  cupcfd::distributions::DistributionConfig<I,T>& decayThreshold)
		:ParticleEmitterConfig<ParticleEmitterSimple<I,T>, ParticleSimple<I,T>, I, T>(position)
		{
			this->rate = rate.clone();
			this->angleXY = angleXY.clone();
			this->angleRotation = angleRotation.clone();
			this->speed = speed.clone();
			this->accelerationX = accelerationX.clone();
			this->accelerationY = accelerationY.clone();
			this->accelerationZ = accelerationZ.clone();
			this->jerkX = jerkX.clone();
			this->jerkY = jerkY.clone();
			this->jerkZ = jerkZ.clone();
			this->decayRate = decayRate.clone();
			this->decayThreshold = decayThreshold.clone();
		}

		template <class I, class T>
		ParticleEmitterSimpleConfig<I,T>::ParticleEmitterSimpleConfig(const ParticleEmitterSimpleConfig<I,T>& source)
		:ParticleEmitterConfig<ParticleEmitterSimple<I,T>, ParticleSimple<I,T>, I, T>(source.position)
		{
			this->rate = source.rate->clone();
			this->angleXY = source.angleXY->clone();
			this->angleRotation = source.angleRotation->clone();
			this->speed = source.speed->clone();
			this->accelerationX = source.accelerationX->clone();
			this->accelerationY = source.accelerationY->clone();
			this->accelerationZ = source.accelerationZ->clone();
			this->jerkX = source.jerkX->clone();
			this->jerkY = source.jerkY->clone();
			this->jerkZ = source.jerkZ->clone();
			this->decayRate = source.decayRate->clone();
			this->decayThreshold = source.decayThreshold->clone();
		}

		template <class I, class T>
		ParticleEmitterSimpleConfig<I,T>::~ParticleEmitterSimpleConfig()
		{
			delete this->rate;
			delete this->angleXY;
			delete this->angleRotation;
			delete this->speed;
			delete this->accelerationX;
			delete this->accelerationY;
			delete this->accelerationZ;
			delete this->jerkX;
			delete this->jerkY;
			delete this->jerkZ;
			delete this->decayRate;
			delete this->decayThreshold;
		}

		template <class I, class T>
		void ParticleEmitterSimpleConfig<I,T>::operator=(const ParticleEmitterSimpleConfig<I,T>& source)
		{
			// Call Base
			ParticleEmitterConfig<ParticleEmitterSimple<I,T>, ParticleSimple<I,T>, I, T>::operator=(source);

			this->rate = source.rate->clone();
			this->angleXY = source.angleXY->clone();
			this->angleRotation = source.angleRotation->clone();
			this->speed = source.speed->clone();
			this->accelerationX = source.accelerationX->clone();
			this->accelerationY = source.accelerationY->clone();
			this->accelerationZ = source.accelerationZ->clone();
			this->jerkX = source.jerkX->clone();
			this->jerkY = source.jerkY->clone();
			this->jerkZ = source.jerkZ->clone();
			this->decayRate = source.decayRate->clone();
			this->decayThreshold = source.decayThreshold->clone();
		}

		template <class I, class T>
		ParticleEmitterSimpleConfig<I,T> * ParticleEmitterSimpleConfig<I,T>::clone()
		{
			return new ParticleEmitterSimpleConfig<I,T>(*this);
		}

		template <class I, class T>
		// cupcfd::error::eCodes ParticleEmitterSimpleConfig<I,T>::buildParticleEmitter(ParticleEmitter<ParticleEmitterSimple<I,T>, ParticleSimple<I,T>,I,T> ** emitter)
		cupcfd::error::eCodes ParticleEmitterSimpleConfig<I,T>::buildParticleEmitter(ParticleEmitter<ParticleEmitterSimple<I,T>, ParticleSimple<I,T>,I,T> ** emitter, int emitterId)
		{
			// Position is initially unknown until we have a mesh to search though
			I localCellID = -1;
			I globalCellID = -1;
			I rank = -1;

			cupcfd::distributions::Distribution<I,T> * rate;
			cupcfd::distributions::Distribution<I,T> * angleXY;
			cupcfd::distributions::Distribution<I,T> * angleRotation;
			cupcfd::distributions::Distribution<I,T> * speed;
			cupcfd::distributions::Distribution<I,T> * accelerationX;
			cupcfd::distributions::Distribution<I,T> * accelerationY;
			cupcfd::distributions::Distribution<I,T> * accelerationZ;
			cupcfd::distributions::Distribution<I,T> * jerkX;
			cupcfd::distributions::Distribution<I,T> * jerkY;
			cupcfd::distributions::Distribution<I,T> * jerkZ;
			cupcfd::distributions::Distribution<I,T> * decayRate;
			cupcfd::distributions::Distribution<I,T> * decayThreshold;

			this->rate->buildDistribution(&rate);
			this->angleXY->buildDistribution(&angleXY);
			this->angleRotation->buildDistribution(&angleRotation);
			this->speed->buildDistribution(&speed);
			this->accelerationX->buildDistribution(&accelerationX);
			this->accelerationY->buildDistribution(&accelerationY);
			this->accelerationZ->buildDistribution(&accelerationZ);
			this->jerkX->buildDistribution(&jerkX);
			this->jerkY->buildDistribution(&jerkY);
			this->jerkZ->buildDistribution(&jerkZ);
			this->decayRate->buildDistribution(&decayRate);
			this->decayThreshold->buildDistribution(&decayThreshold);

			// Create the Particle Emitter
			*emitter = new ParticleEmitterSimple<I,T>(localCellID, globalCellID, rank, 
												   emitterId, 
												   this->position,
												   rate, angleXY, angleRotation, speed,
												   accelerationX, accelerationY, accelerationZ,
												   jerkX, jerkY, jerkZ, decayRate, decayThreshold);

			delete rate;
			delete angleXY;
			delete angleRotation;
			delete speed;
			delete accelerationX;
			delete accelerationY;
			delete accelerationZ;
			delete jerkX;
			delete jerkY;
			delete jerkZ;
			delete decayRate;
			delete decayThreshold;

			return cupcfd::error::E_SUCCESS;
		}
	}
}

// Explicit Instantiation
using namespace cupcfd::particles;

template class cupcfd::particles::ParticleEmitterSimpleConfig<int, float>;
template class cupcfd::particles::ParticleEmitterSimpleConfig<int, double>;

