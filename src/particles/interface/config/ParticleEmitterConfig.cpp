/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Contains definitions for the ParticleEmitterConfig class.
 */

#include "ParticleEmitterConfig.h"
#include "ParticleEmitterSimple.h"

namespace cupcfd
{
	namespace particles
	{
		template <class E, class P, class I, class T>
		ParticleEmitterConfig<E,P,I,T>::ParticleEmitterConfig(const cupcfd::geometry::euclidean::EuclideanPoint<T,3>& position)
		: position(position)
		{

		}

		template <class E, class P, class I, class T>
		ParticleEmitterConfig<E,P,I,T>::ParticleEmitterConfig(ParticleEmitterConfig<E,P,I,T>& source)
		: position(source.position)
		{

		}

		template <class E, class P, class I, class T>
		ParticleEmitterConfig<E,P,I,T>::~ParticleEmitterConfig()
		{

		}

		template <class E, class P, class I, class T>
		void ParticleEmitterConfig<E,P,I,T>::operator=(const ParticleEmitterConfig<E,P,I,T>& source)
		{
			this->position = source.position;
		}

		// template <class E, class P, class I, class T>
		// ParticleEmitterConfig<E,P,I,T> * ParticleEmitterConfig<E,P,I,T>::clone()
		// {
		// 	// Pure Virtual, nothing to do here
		// }

		// template <class E, class P, class I, class T>
		// // cupcfd::error::eCodes ParticleEmitterConfig<E,P,I,T>::buildParticleEmitter(ParticleEmitter<E,P,I,T> ** emitter)
		// cupcfd::error::eCodes ParticleEmitterConfig<E,P,I,T>::buildParticleEmitter(ParticleEmitter<E,P,I,T> ** emitter, int emitterId)
		// {
		// 	// Pure Virtual, nothing to do here
		// }
	}
}

// Explicit Instantiation
using namespace cupcfd::particles;

template class cupcfd::particles::ParticleEmitterConfig<ParticleEmitterSimple<int, float>, ParticleSimple<int, float>, int, float>;
template class cupcfd::particles::ParticleEmitterConfig<ParticleEmitterSimple<int, double>, ParticleSimple<int, double>, int, double>;

