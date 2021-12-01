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
 * Definitions for the ParticleSimple Class
 */

#include "ParticleSimple.h"
#include "EuclideanPlane3D.h"

namespace cupcfd
{
	namespace particles
	{
		// Initialise Static Variables
		template <class I, class T>
		bool ParticleSimple<I,T>::mpiDataTypeReg = false;

		template <class I, class T>
		MPI_Datatype ParticleSimple<I,T>::mpiType = reinterpret_cast<MPI_Datatype>(-1);

		// === Constructors/Deconstructors

        template <class I, class T>
        ParticleSimple<I,T>::ParticleSimple()
        : Particle<ParticleSimple,I,T>(cupcfd::geometry::euclidean::EuclideanVector<T,3>(T(0),T(0),T(0)),
        							   cupcfd::geometry::euclidean::EuclideanPoint<T,3>(T(0),T(0),T(0)),
									   -1, -1, -1, T(0)),
          inactive(false),
          decayLevel(0.0),
          decayRate(0.0)
        {
            // Vectors and Points should default to 0 by their default constructor
        }

		template <class I, class T>
		ParticleSimple<I,T>::ParticleSimple(cupcfd::geometry::euclidean::EuclideanPoint<T,3>& pos,
							    cupcfd::geometry::euclidean::EuclideanVector<T,3>& velocity,
								cupcfd::geometry::euclidean::EuclideanVector<T,3>& acceleration,
								cupcfd::geometry::euclidean::EuclideanVector<T,3>& jerk,
								I id, 
								I cellGlobalID, I rank,
								T decayLevel, T decayRate,
								T travelDt)
		: Particle<ParticleSimple<I,T>,I,T>(velocity, pos, id, cellGlobalID, rank, travelDt),
		  inactive(false),
		  acceleration(acceleration),
		  jerk(jerk),
		  decayLevel(decayLevel),
		  decayRate(decayRate)
		{
			// Unset Vectors and Points should default to 0 by their default constructor
		}

		template <class I, class T>
		ParticleSimple<I,T>::ParticleSimple(const ParticleSimple<I,T>& source)
		: Particle<ParticleSimple<I,T>,I,T>()
		{
			// Passthrough to copy operator
			*this = source;
		}

		template <class I, class T>
		ParticleSimple<I,T>::~ParticleSimple()
		{

		}
	}
}

// Explicit Instantiation
template class cupcfd::particles::ParticleSimple<int, float>;
template class cupcfd::particles::ParticleSimple<int, double>;
