/*
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
 * Definitions for the ParticleEmitterSimple Class
 */

#include "EuclideanVector3D.h"

#include "ParticleEmitterSimple.h"
#include "Polyhedron.h"
#include "Distribution.h"
#include <vector>
#include "ArithmeticKernels.h"
#include <cstdlib>

namespace arth = cupcfd::utility::arithmetic::kernels;

namespace cupcfd
{
	namespace particles
	{
		template <class I, class T>
		ParticleEmitterSimple<I,T>::ParticleEmitterSimple(I localCellID, I globalCellID, I rank, 
														  int emitterId, 
														  cupcfd::geometry::euclidean::EuclideanPoint<T,3>& position,
														  cupcfd::distributions::Distribution<I,T> * rate,
														  cupcfd::distributions::Distribution<I,T> * angleXY,
														  cupcfd::distributions::Distribution<I,T> * angleRotation,
														  cupcfd::distributions::Distribution<I,T> * speed,
														  cupcfd::distributions::Distribution<I,T> * accelerationX,
														  cupcfd::distributions::Distribution<I,T> * accelerationY,
														  cupcfd::distributions::Distribution<I,T> * accelerationZ,
														  cupcfd::distributions::Distribution<I,T> * jerkX,
														  cupcfd::distributions::Distribution<I,T> * jerkY,
														  cupcfd::distributions::Distribution<I,T> * jerkZ,
														  cupcfd::distributions::Distribution<I,T> * decayRate,
														  cupcfd::distributions::Distribution<I,T> * decayThreshold)
		: ParticleEmitter<ParticleEmitterSimple<I,T>,ParticleSimple<I,T>,I,T>(localCellID, globalCellID, rank, position),
		  nextParticleTime(T(-1))
		{
			this->rate = rate->clone();
			this->angleXY = angleXY->clone();
			this->angleRotation = angleRotation->clone();
			this->speed = speed->clone();
			this->accelerationX = accelerationX->clone();
			this->accelerationY = accelerationY->clone();
			this->accelerationZ = accelerationZ->clone();
			this->jerkX = jerkX->clone();
			this->jerkY = jerkY->clone();
			this->jerkZ = jerkZ->clone();
			this->decayRate = decayRate->clone();
			this->decayThreshold = decayThreshold->clone();

			this->nextParticleID = I(0);
			this->id = emitterId;
		}

		template <class I, class T>
		ParticleEmitterSimple<I,T>::ParticleEmitterSimple(const ParticleEmitterSimple<I,T>& source)
		: ParticleEmitter<ParticleEmitterSimple<I,T>,ParticleSimple<I,T>,I,T>(source.localCellID, source.globalCellID, source.rank, source.position),
		  nextParticleTime(source.nextParticleTime)
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

			this->nextParticleID = source.nextParticleID;
			this->id = source.id;
		}

		template <class I, class T>
		ParticleEmitterSimple<I,T>::~ParticleEmitterSimple()
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
		cupcfd::error::eCodes ParticleEmitterSimple<I,T>::generateParticles(ParticleSimple<I,T> ** particles, I * nParticles, T dt) {
			// tCurrent is the current time in the dt period, relative to 0
			// tInc is the amount of time that must pass before the next particle is generated.
			T tCurrent, tInc;

			// Stores the relative times, from 0->dt, when each particle is generated
			std::vector<T> times;

			// Set tCurrent to be the time of the next particle generation
			if(arth::isEqual(this->nextParticleTime, T(-1))) {
				// Generate the next interval
				this->rate->getValues(&tInc, 1);
				tCurrent = tInc;
			} else {
				// Set the next internal to be the last unused interval
				// This presumes that tStart picks straight up from the end of the prior interval though
				// Error Check?
				tCurrent = this->nextParticleTime;
			}

			// Loop till the time of the next particle is after the end of the dt time period.
			// I ptr = 0;
			while(tCurrent < dt) {
				// Store the times of the particles to be generated
				times.push_back(tCurrent);

				// Get the time of the next particle
				this->rate->getValues(&tInc, 1);
				tCurrent += tInc;

				// Track number of loops
				// ptr++;
			}

			// We have exceeded the range, store the unused time for use in the next time period
			// Since this time is relative to the time period 0->dt->tCurrent, we reduce it by
			// dt so that it is tCurrent-dt time into the next time period, and can be used as
			// a pregenerated next particle time.
			// If this still exceeds the next dt time period, it will be reduced again (and no particles
			// would be generated) until enough dt time periods have passed (though dt could be different
			// amounts with every call)
			this->nextParticleTime = tCurrent - dt;

			// Create the particle storage and the particles
			*nParticles = times.size();
			*particles = (ParticleSimple<I,T> *) malloc(sizeof(ParticleSimple<I,T>) * (*nParticles));

			// Create and setup particles
			// Generate accelerations
			T * accelerationX = (T *) malloc(sizeof(T) * (*nParticles));
			T * accelerationY = (T *) malloc(sizeof(T) * (*nParticles));
			T * accelerationZ = (T *) malloc(sizeof(T) * (*nParticles));
			T * jerkX = (T *) malloc(sizeof(T) * (*nParticles));
			T * jerkY = (T *) malloc(sizeof(T) * (*nParticles));
			T * jerkZ = (T *) malloc(sizeof(T) * (*nParticles));
			T * speed = (T *) malloc(sizeof(T) * (*nParticles));
			T * angleXY = (T *) malloc(sizeof(T) * (*nParticles));
			T * angleRotation = (T *) malloc(sizeof(T) * (*nParticles));
			T * decayRate = (T *) malloc(sizeof(T) * (*nParticles));
			T * decayThreshold = (T *) malloc(sizeof(T) * (*nParticles));

			this->accelerationX->getValues(accelerationX, *nParticles);
			this->accelerationY->getValues(accelerationY, *nParticles);
			this->accelerationZ->getValues(accelerationZ, *nParticles);
			this->jerkX->getValues(jerkX, *nParticles);
			this->jerkY->getValues(jerkY, *nParticles);
			this->jerkZ->getValues(jerkZ, *nParticles);
			this->speed->getValues(speed, *nParticles);
			this->angleXY->getValues(angleXY, *nParticles);
			this->angleRotation->getValues(angleRotation, *nParticles);
			this->decayRate->getValues(decayRate, *nParticles);
			this->decayThreshold->getValues(decayThreshold, *nParticles);

			// Generate each particle
			for(I i = 0; i < *nParticles; i++) {
				cupcfd::geometry::euclidean::EuclideanVector3D<T> velocity(T(1),T(0),T(0));
				cupcfd::geometry::euclidean::EuclideanVector3D<T> acceleration(accelerationX[i],accelerationY[i],accelerationZ[i]);
				cupcfd::geometry::euclidean::EuclideanVector3D<T> jerk(jerkX[i],jerkY[i],jerkZ[i]);

				// Build some velocity vectors from the speed + angles
				// Angle unit vector on XY plane
				velocity.rotateZAxisRadian(angleXY[i]);

				// Rotate Vector into/out of Z plane
				velocity.rotateYAxisRadian(angleRotation[i]);

				// Adjust the length of the vector to match the provided speed
				T length = velocity.length();
				if (length == T(0)) {
					return cupcfd::error::E_GEOMETRY_LOGIC_ERROR;
				}
				velocity = (speed[i]/length) * velocity;

				(*particles)[i] = ParticleSimple<I,T>(
					this->position,
					velocity,
					acceleration,
					jerk,
					(this->nextParticleID << 8) + this->id,
					this->globalCellID,
					this->rank,
					decayThreshold[i],
					decayRate[i],
					dt-times[i]);

				this->nextParticleID++;
				if (this->nextParticleID == std::numeric_limits<I>::max()) {
					// Assume that by this point in simulation, that the first particles 
					// to be emitted have left the system.
					this->nextParticleID = I(0);
				}
			}

			free(accelerationX);
			free(accelerationY);
			free(accelerationZ);
			free(jerkX);
			free(jerkY);
			free(jerkZ);
			free(speed);
			free(angleXY);
			free(angleRotation);
			free(decayRate);
			free(decayThreshold);

			return cupcfd::error::E_SUCCESS;
		}
	}
}

// Explicit Instantiation
template class cupcfd::particles::ParticleEmitterSimple<int, float>;
template class cupcfd::particles::ParticleEmitterSimple<int, double>;
