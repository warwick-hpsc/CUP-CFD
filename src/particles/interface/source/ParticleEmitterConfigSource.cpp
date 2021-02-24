/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Contains definitions for the ParticleEmitterConfigSource class.
 */

#include "ParticleEmitterConfigSource.h"
#include "ParticleEmitterSimple.h"
#include "ParticleSimple.h"

namespace cupcfd
{
	namespace particles
	{
		template <class E, class P, class I, class T>
		ParticleEmitterConfigSource<E,P,I,T>::ParticleEmitterConfigSource()
		{

		}

		template <class E, class P, class I, class T>
		ParticleEmitterConfigSource<E,P,I,T>::~ParticleEmitterConfigSource()
		{

		}
	}
}

// Explicit Instantiation
using namespace cupcfd::particles;

template class cupcfd::particles::ParticleEmitterConfigSource<ParticleEmitterSimple<int, float>, ParticleSimple<int, float>, int, float>;
template class cupcfd::particles::ParticleEmitterConfigSource<ParticleEmitterSimple<int, double>, ParticleSimple<int, double>, int, double>;

