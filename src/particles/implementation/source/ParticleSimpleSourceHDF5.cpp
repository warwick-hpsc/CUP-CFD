/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Contains definitions for the ParticleSimpleSourceHDF5 class
 */

// Class Declarations
#include "ParticleSimpleSourceHDF5.h"

// C++ Library Function/Objects
#include <vector>

// HDF5 I/O Access
#include "HDF5Record.h"
#include "HDF5Access.h"

#include <iostream>

namespace cupcfd
{
	namespace particles
	{
		// This HDF5 file is formatted as follows:
		// Attributes:
		// Group | Name | Description
		// "/"	 | "nparticles" | The number of vector values stored in this file

		// DataSets:
		// Particle data is distributed across multiple datasets, but each dataset
		// shares the indexing scheme (i.e. the data element at index 0 of one dataset
		// belongs to the same particles as the data element at index 0 of another dataset)

		// Note, we don't store in-flight data (time remaining, inflight position).
		// It is presumed that the particle data is from the very start of very end of a
		// time iteration.

		// Group | Name | Description
		// "/particles"	 | "pos" | The 3D position of the point
		// "/particles"	 | "velocity" | The 3D velocity vector
		// "/particles"	 | "acceleration" | The 3D acceleration vector
		// "/particles"	 | "jerk" | The 3D jerk vector
		// "/particles"	 | "decayLevel" | The particles decay level
		// "/particles"	 | "decayRate" | The particles decay rate

		template <class I, class T>
		ParticleSimpleSourceHDF5<I,T>::ParticleSimpleSourceHDF5(std::string fileName, int sourceId)
		: ParticleSource<ParticleSimple<I,T>,I,T>(),
		  fileName(fileName),
		  id(sourceId)
		{

		}

		template <class I, class T>
		ParticleSimpleSourceHDF5<I, T>::~ParticleSimpleSourceHDF5()
		{
			// Currently Nothing to Clean Up.
		}

		template <class I, class T>
		cupcfd::error::eCodes ParticleSimpleSourceHDF5<I,T>::getNParticles(I * nParticles)
		{
			cupcfd::error::eCodes status;

			// This format will store it's attributes at the root level
			cupcfd::io::hdf5::HDF5Record record("/", "nparticles",true);
			cupcfd::io::hdf5::HDF5Access access(this->fileName, record);
			access.readData(nParticles);

			return cupcfd::error::E_SUCCESS;
		}

		template <class I, class T>
		cupcfd::error::eCodes ParticleSimpleSourceHDF5<I,T>::getParticles(Particle<ParticleSimple<I,T>,I,T> *** particleData, I * nParticles, I * indexes, I nIndexes, I indexBase)
		{
			cupcfd::error::eCodes status;

			// ToDo: Don't really like that this is a m*n function where m and n could both get large.
			// We could store the rank, but if the decomposition changes (e.g. different number of processes) between runs, it invalidates
			// the ranks and possibly the global cell IDs if the decomposition changes from the partitioner.

			// "/particles"	 | "pos" | The 3D position of the point
			// "/particles"	 | "velocity" | The 3D velocity vector
			// "/particles"	 | "acceleration" | The 3D acceleration vector
			// "/particles"	 | "jerk" | The 3D jerk vector
			// "/particles"	 | "decaylevel" | The particles decay level
			// "/particles"	 | "decayrate" | The particles decay rate


			// Setup Access
			cupcfd::io::hdf5::HDF5Record recordPos("/particles", "pos" , false);
			cupcfd::io::hdf5::HDF5Access accessPos(this->fileName, recordPos);
			cupcfd::io::hdf5::HDF5Properties propertiesPosX(accessPos);
			cupcfd::io::hdf5::HDF5Properties propertiesPosY(accessPos);
			cupcfd::io::hdf5::HDF5Properties propertiesPosZ(accessPos);

			cupcfd::io::hdf5::HDF5Record recordVel("/particles", "velocity" , false);
			cupcfd::io::hdf5::HDF5Access accessVel(this->fileName, recordVel);
			cupcfd::io::hdf5::HDF5Properties propertiesVelX(accessVel);
			cupcfd::io::hdf5::HDF5Properties propertiesVelY(accessVel);
			cupcfd::io::hdf5::HDF5Properties propertiesVelZ(accessVel);

			cupcfd::io::hdf5::HDF5Record recordAccel("/particles", "acceleration" , false);
			cupcfd::io::hdf5::HDF5Access accessAccel(this->fileName, recordAccel);
			cupcfd::io::hdf5::HDF5Properties propertiesAccelX(accessAccel);
			cupcfd::io::hdf5::HDF5Properties propertiesAccelY(accessAccel);
			cupcfd::io::hdf5::HDF5Properties propertiesAccelZ(accessAccel);

			cupcfd::io::hdf5::HDF5Record recordJerk("/particles", "jerk" , false);
			cupcfd::io::hdf5::HDF5Access accessJerk(this->fileName, recordJerk);
			cupcfd::io::hdf5::HDF5Properties propertiesJerkX(accessJerk);
			cupcfd::io::hdf5::HDF5Properties propertiesJerkY(accessJerk);
			cupcfd::io::hdf5::HDF5Properties propertiesJerkZ(accessJerk);

			cupcfd::io::hdf5::HDF5Record recordDecayLevel("/particles", "decayLevel" , false);
			cupcfd::io::hdf5::HDF5Access accessDecayLevel(this->fileName, recordDecayLevel);
			cupcfd::io::hdf5::HDF5Properties propertiesDecayLevel(accessDecayLevel);

			cupcfd::io::hdf5::HDF5Record recordDecayRate("/particles", "decayRate" , false);
			cupcfd::io::hdf5::HDF5Access accessDecayRate(this->fileName, recordDecayRate);
			cupcfd::io::hdf5::HDF5Properties propertiesDecayRate(accessDecayRate);

			for(I i = 0; i < nIndexes; i++)
			{
				propertiesPosX.addIndex(indexes[i] - indexBase, 0);
				propertiesPosY.addIndex(indexes[i] - indexBase, 1);
				propertiesPosZ.addIndex(indexes[i] - indexBase, 2);

				propertiesVelX.addIndex(indexes[i] - indexBase, 0);
				propertiesVelY.addIndex(indexes[i] - indexBase, 1);
				propertiesVelZ.addIndex(indexes[i] - indexBase, 2);

				propertiesAccelX.addIndex(indexes[i] - indexBase, 0);
				propertiesAccelY.addIndex(indexes[i] - indexBase, 1);
				propertiesAccelZ.addIndex(indexes[i] - indexBase, 2);

				propertiesJerkX.addIndex(indexes[i] - indexBase, 0);
				propertiesJerkY.addIndex(indexes[i] - indexBase, 1);
				propertiesJerkZ.addIndex(indexes[i] - indexBase, 2);

				propertiesDecayLevel.addIndex(indexes[i] - indexBase);
				propertiesDecayRate.addIndex(indexes[i] - indexBase);
			}

			// Read in particle positional data
			T * posX = (T *) malloc(sizeof(T) * nIndexes);
			T * posY = (T *) malloc(sizeof(T) * nIndexes);
			T * posZ = (T *) malloc(sizeof(T) * nIndexes);

			T * velX = (T *) malloc(sizeof(T) * nIndexes);
			T * velY = (T *) malloc(sizeof(T) * nIndexes);
			T * velZ = (T *) malloc(sizeof(T) * nIndexes);

			T * accelX = (T *) malloc(sizeof(T) * nIndexes);
			T * accelY = (T *) malloc(sizeof(T) * nIndexes);
			T * accelZ = (T *) malloc(sizeof(T) * nIndexes);

			T * jerkX = (T *) malloc(sizeof(T) * nIndexes);
			T * jerkY = (T *) malloc(sizeof(T) * nIndexes);
			T * jerkZ = (T *) malloc(sizeof(T) * nIndexes);

			T * decayLevel = (T *) malloc(sizeof(T) * nIndexes);
			T * decayRate = (T *) malloc(sizeof(T) * nIndexes);

			// Read Data
			accessPos.readData(posX, propertiesPosX);
			accessPos.readData(posY, propertiesPosY);
			accessPos.readData(posZ, propertiesPosZ);

			accessVel.readData(velX, propertiesVelX);
			accessVel.readData(velY, propertiesVelY);
			accessVel.readData(velZ, propertiesVelZ);

			accessAccel.readData(accelX, propertiesAccelX);
			accessAccel.readData(accelY, propertiesAccelY);
			accessAccel.readData(accelZ, propertiesAccelZ);

			accessJerk.readData(jerkX, propertiesJerkX);
			accessJerk.readData(jerkY, propertiesJerkY);
			accessJerk.readData(jerkZ, propertiesJerkZ);

			accessDecayLevel.readData(decayLevel, propertiesDecayLevel);
			accessDecayRate.readData(decayRate, propertiesDecayRate);

			*nParticles = nIndexes;

			*particleData = (Particle<ParticleSimple<I,T>,I,T> **) malloc(sizeof(Particle<ParticleSimple<I,T>,I,T> *) * (*nParticles));

			for(I i = 0; i < nIndexes; i++)
			{
				cupcfd::geometry::euclidean::EuclideanPoint<T,3> pos(posX[i], posY[i], posZ[i]);
				cupcfd::geometry::euclidean::EuclideanVector<T,3> velocity(velX[i], velY[i], velZ[i]);
				cupcfd::geometry::euclidean::EuclideanVector<T,3> acceleration(accelX[i], accelY[i], accelZ[i]);
				cupcfd::geometry::euclidean::EuclideanVector<T,3> jerk(jerkX[i], jerkY[i], jerkZ[i]);

				// I particleId = i;
				I particleId = (i << 8) + this->id;
				I cellGlobalID = -1;
				I rank = -1;

				// ToDo: Not a big fan of passing array of pointers since it's not contiguous, but these should only be used for builders and
				// then discarded anyway.
				// Ideally though we can tidy up the templates/inheritance types a bit so that an array of pointers might not be necessary.
				(*particleData)[i] = new ParticleSimple<I,T>(
					pos, 
					velocity, 
					acceleration, 
					jerk, 
					particleId, 
					cellGlobalID, 
					rank, 
					decayLevel[i], 
					decayRate[i], 
					T(0));

			}

			free(posX);
			free(posY);
			free(posZ);
			free(velX);
			free(velY);
			free(velZ);
			free(accelX);
			free(accelY);
			free(accelZ);
			free(jerkX);
			free(jerkY);
			free(jerkZ);
			free(decayLevel);
			free(decayRate);

			return cupcfd::error::E_SUCCESS;
		}
	}
}

// Explicit Instantiation
template class cupcfd::particles::ParticleSimpleSourceHDF5<int, float>;
template class cupcfd::particles::ParticleSimpleSourceHDF5<int, double>;

