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
		ParticleSourceConfigSource<P,I,T>::ParticleSourceConfigSource(ParticleSourceConfigSource<P,I,T>& source)
		{

		}

		template <class P, class I, class T>
		ParticleSourceConfigSource<P,I,T>::~ParticleSourceConfigSource()
		{

		}

		template <class P, class I, class T>
		void ParticleSourceConfigSource<P,I,T>::operator=(ParticleSourceConfigSource<P,I,T>& source)
		{
			// Nothing to do here currently
		}

		template <class P, class I, class T>
		ParticleSourceConfigSource<P,I,T> * ParticleSourceConfigSource<P,I,T>::clone()
		{
			// Pure Virtual Method
		}

		template <class P, class I, class T>
		cupcfd::error::eCodes ParticleSourceConfigSource<P,I,T>::buildParticleSourceConfig(ParticleSourceConfig<P,I,T> ** config)
		{
			// Pure Virtual Method
			// Nothing to do here currently
		}
	}
}

// Explicit Instantiation
using namespace cupcfd::particles;

template class cupcfd::particles::ParticleSourceConfigSource<ParticleSimple<int, float>, int, float>;
template class cupcfd::particles::ParticleSourceConfigSource<ParticleSimple<int, double>, int, double>;

