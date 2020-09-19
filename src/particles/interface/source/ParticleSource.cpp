/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Contains definitions for the ParticleSource class
 */

// Class Declarations
#include "ParticleSource.h"
#include "ParticleSimple.h"

namespace cupcfd
{
	namespace particles
	{
		template <class P, class I, class T>
		ParticleSource<P,I,T>::ParticleSource()
		{

		}

		template <class P, class I, class T>
		ParticleSource<P,I,T>::~ParticleSource()
		{
			// Currently Nothing to Clean Up.
		}

		// === Pure Virtual ===

		// template <class P, class I, class T>
		// cupcfd::error::eCodes ParticleSource<P,I,T>::getNParticles(I * nParticles)
		// {
		// }

		// template <class P, class I, class T>
		// cupcfd::error::eCodes ParticleSource<P,I,T>::getParticles(Particle<P,I,T> *** particleData, I * nParticles, I * indexes, I nIndexes, I indexBase)
		// {
		// }
	}
}

// Explicit Instantiation
template class cupcfd::particles::ParticleSource<cupcfd::particles::ParticleSimple<int,float>, int, float>;
template class cupcfd::particles::ParticleSource<cupcfd::particles::ParticleSimple<int,double>, int, double>;

