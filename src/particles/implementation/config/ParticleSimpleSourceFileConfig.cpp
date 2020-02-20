/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Contains definitions for the ParticleSimpleSourceFileConfig class.
 */

#include "ParticleSimpleSourceFileConfig.h"
#include "ParticleSimpleSourceHDF5.h"
#include "ParticleSimple.h"

namespace cupcfd
{
	namespace particles
	{
		template <class I, class T>
		ParticleSimpleSourceFileConfig<I,T>::ParticleSimpleSourceFileConfig(std::string& filePath, ParticleSimpleFileFormat format)
		:ParticleSourceConfig<ParticleSimple<I,T>, I, T>(),
		 filePath(filePath),
		 format(format)
		{

		}

		template <class I, class T>
		ParticleSimpleSourceFileConfig<I,T>::ParticleSimpleSourceFileConfig(const ParticleSimpleSourceFileConfig<I,T>& source)
		{
			*this = source;
		}

		template <class I, class T>
		ParticleSimpleSourceFileConfig<I,T>::~ParticleSimpleSourceFileConfig()
		{

		}

		template <class I, class T>
		void ParticleSimpleSourceFileConfig<I,T>::operator=(const ParticleSimpleSourceFileConfig<I,T>& source)
		{
			this->filePath = source.filePath;
			this->format = source.format;
		}

		template <class I, class T>
		ParticleSimpleSourceFileConfig<I,T> * ParticleSimpleSourceFileConfig<I,T>::clone()
		{
			return new ParticleSimpleSourceFileConfig<I,T>(*this);
		}

		template <class I, class T>
		cupcfd::error::eCodes ParticleSimpleSourceFileConfig<I,T>::buildParticleSource(ParticleSource<ParticleSimple<I,T>,I,T> ** particleSource)
		{
			if(this->format == PARTICLE_SIMPLE_FILE_FORMAT_HDF5)
			{
				*particleSource = new ParticleSimpleSourceHDF5<I,T>(this->filePath);
				return cupcfd::error::E_SUCCESS;
			}

			// Shouldn't reach this part if we have a valid format
			return cupcfd::error::E_ERROR;
		}
	}
}

// Explicit Instantiation
using namespace cupcfd::particles;

template class cupcfd::particles::ParticleSimpleSourceFileConfig<int, float>;
template class cupcfd::particles::ParticleSimpleSourceFileConfig<int, double>;

