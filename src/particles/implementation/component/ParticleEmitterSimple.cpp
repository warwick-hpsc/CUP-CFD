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

			this->numParticlesEmitted = 0;
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

			this->numParticlesEmitted = 0;
		}

		template <class I, class T>
		ParticleEmitterSimple<I,T>::~ParticleEmitterSimple()
		{
			free(this->rate);
			free(this->angleXY);
			free(this->angleRotation);
			free(this->speed);
			free(this->accelerationX);
			free(this->accelerationY);
			free(this->accelerationZ);
			free(this->jerkX);
			free(this->jerkY);
			free(this->jerkZ);
			free(this->decayRate);
			free(this->decayThreshold);
		}

		template <class I, class T>
		cupcfd::error::eCodes ParticleEmitterSimple<I,T>::generateParticles(ParticleSimple<I,T> ** particles, I * nParticles, T dt)
		{
			// std::cout << "ParticleEmitterSimple<I,T>::generateParticles() called" << std::endl;
			// usleep(500*1000);

			cupcfd::error::eCodes status;

			// tCurrent is the current time in the dt period, relative to 0
			// tInc is the amount of time that must pass before the next particle is generated.
			T tCurrent, tInc;
			I ptr;

			// Stores the relative times, from 0->dt, when each particle is generated
			std::vector<T> times;

			// Set tCurrent to be the time of the next particle generation
			if(arth::isEqual(this->nextParticleTime, T(-1)))
			{
				// Generate the next interval
				this->rate->getValues(&tInc, 1);
				tCurrent = tCurrent + tInc;
			}
			else
			{
				// Set the next internal to be the last unused interval
				// This presumes that tStart picks straight up from the end of the prior interval though
				// Error Check?
				tCurrent = this->nextParticleTime;
			}

			ptr = 0;

			// Loop till the time of the next particle is after the end of the dt time period.
			while(tCurrent < dt)
			{
				// Store the times of the particles to be generated
				times.push_back(tCurrent);

				// Get the time of the next particle
				this->rate->getValues(&tInc, 1);
				tCurrent = tCurrent + tInc;

				// Track number of loops
				ptr = ptr + 1;
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
			for(I i = 0; i < *nParticles; i++)
			{
				cupcfd::geometry::euclidean::EuclideanVector<T,3> velocity(T(1),T(0),T(0));
				cupcfd::geometry::euclidean::EuclideanVector<T,3> acceleration(accelerationX[i],accelerationY[i],accelerationZ[i]);
				cupcfd::geometry::euclidean::EuclideanVector<T,3> jerk(jerkX[i],jerkY[i],jerkZ[i]);

				// Build some velocity vectors from the speed + angles
				// Angle unit vector on XY plane
				rotateZAxisRadian(angleXY[i], velocity);

				// Rotate Vector into/out of Z plane
				rotateYAxisRadian(angleRotation[i], velocity);

				// Adjust the length of the vector to match the provided speed
				T length;
				velocity.length(&length);
				velocity = (speed[i]/length) * velocity;

				// (*particles)[i].setPos(this->position);
				// (*particles)[i].setInFlightPos(this->position);
				// (*particles)[i].setVelocity(velocity);
				// (*particles)[i].setAcceleration(acceleration);
				// (*particles)[i].setJerk(jerk);
				// (*particles)[i].setCellGlobalID(this->globalCellID);

				// // Remaining travel time depends on when it was generated in this dt period.
				// // E.g. if dt is 10, and it was generated at 1.2, then it only has 8.8 remaining
				// // in this time period.
				// (*particles)[i].setTravelTime(dt-times[i]);
				// (*particles)[i].setDecayLevel(decayThreshold[i]);
				// (*particles)[i].setDecayRate(decayRate[i]);
				// (*particles)[i].rank = this->rank;				// ToDo: Should be setRank method

				(*particles)[i] = ParticleSimple<I,T>(
					this->position,
					velocity,
					acceleration,
					jerk,
					this->globalCellID,
					this->rank,
					decayThreshold[i],
					decayRate[i],
					dt-times[i]);

				// (*particles)[i].particleID = this->numParticlesEmitted;
				(*particles)[i].particleID = this->emitterID + 100*this->numParticlesEmitted;

				// if ((*particles)[i].particleID == 8601) {
				// 	std::cout << "particle " << (*particles)[i].particleID << " emitted at POS: ";
				// 	(*particles)[i].getPos().print(); std::cout << std::endl;
				// }

				this->numParticlesEmitted++;
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
