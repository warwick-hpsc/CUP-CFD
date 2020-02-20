/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Contains definitions for the ParticleSourceConfig class
 */

#include "ParticleSourceConfig.h"
#include "ParticleSimple.h"

namespace cupcfd
{
	namespace particles
	{
		// === Constructors/Deconstructors ===

		template <class P, class I, class T>
		ParticleSourceConfig<P,I,T>::ParticleSourceConfig()
		{
			// Nothing to do
		}

		template <class P, class I, class T>
		ParticleSourceConfig<P,I,T>::ParticleSourceConfig(ParticleSourceConfig& source)
		{
			*this = source;
		}

		template <class P, class I, class T>
		ParticleSourceConfig<P,I,T>::~ParticleSourceConfig()
		{
			// Nothing to do
		}

		// === Concrete Methods ===

		template <class P, class I, class T>
		void ParticleSourceConfig<P,I,T>::operator=(ParticleSourceConfig<P,I,T>& source)
		{
			// Nothing to do
		}

		// === Pure Virtual Methods ===
		// Here to satisfy linker

		template <class P, class I, class T>
		ParticleSourceConfig<P,I,T> * ParticleSourceConfig<P,I,T>::clone()
		{
			return nullptr;
		}

		template <class P, class I, class T>
		cupcfd::error::eCodes ParticleSourceConfig<P,I,T>::buildParticleSource(ParticleSource<P,I,T> ** particleSource)
		{
			return cupcfd::error::E_SUCCESS;
		}
	}
}

// Explicit Instantiation
using namespace cupcfd::particles;

template class cupcfd::particles::ParticleSourceConfig<cupcfd::particles::ParticleSimple<int,float>,int, float>;
template class cupcfd::particles::ParticleSourceConfig<cupcfd::particles::ParticleSimple<int,double>, int, double>;
