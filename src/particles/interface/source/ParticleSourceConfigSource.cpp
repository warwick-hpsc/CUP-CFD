/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Contains definitions for the ParticleSourceConfigSource class.
 */

#include "ParticleSourceConfigSource.h"
#include "ParticleSimple.h"

namespace cupcfd
{
	namespace particles
	{
		template <class P, class I, class T>
		ParticleSourceConfigSource<P,I,T>::ParticleSourceConfigSource()
		{

		}

		template <class P, class I, class T>
		ParticleSourceConfigSource<P,I,T>::~ParticleSourceConfigSource()
		{

		}
	}
}

// Explicit Instantiation
using namespace cupcfd::particles;

template class cupcfd::particles::ParticleSourceConfigSource<ParticleSimple<int, float>, int, float>;
template class cupcfd::particles::ParticleSourceConfigSource<ParticleSimple<int, double>, int, double>;

