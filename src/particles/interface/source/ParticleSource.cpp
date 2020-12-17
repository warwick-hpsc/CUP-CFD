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
	}
}

// Explicit Instantiation
template class cupcfd::particles::ParticleSource<cupcfd::particles::ParticleSimple<int,float>, int, float>;
template class cupcfd::particles::ParticleSource<cupcfd::particles::ParticleSimple<int,double>, int, double>;

