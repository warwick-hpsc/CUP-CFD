/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Contains definitions for the VectorSourceFileConfig class
 */

#include "VectorSourceFileConfig.h"
#include "VectorSourceHDF5.h"

namespace cupcfd
{
	namespace data_structures
	{
		template <class I, class T>
		VectorSourceFileConfig<I,T>::VectorSourceFileConfig(std::string& filePath, VectorFileFormat format)
		: VectorSourceConfig<I,T>(),
		  filePath(filePath),
		  format(format)
		{
			// Nothing to do beyond intialiser list currently.
		}

		template <class I, class T>
		VectorSourceFileConfig<I,T>::VectorSourceFileConfig(VectorSourceFileConfig<I,T>& source)
		{
			*this = source;
		}

		template <class I, class T>
		VectorSourceFileConfig<I,T>::~VectorSourceFileConfig()
		{
			// Nothing to do beyond intialiser list currently.
		}

		template <class I, class T>
		void VectorSourceFileConfig<I,T>::operator=(VectorSourceFileConfig<I,T>& source)
		{
			this->filePath = source.filePath;
			this->format = source.format;
		}

		template <class I, class T>
		VectorSourceFileConfig<I,T> * VectorSourceFileConfig<I,T>::clone()
		{
			return new VectorSourceFileConfig<I,T>(*this);
		}

		template <class I, class T>
		cupcfd::error::eCodes VectorSourceFileConfig<I,T>::buildVectorSource(VectorSource<I,T> ** vectorSource)
		{
			if(this->format == VECTOR_FILE_FORMAT_HDF5)
			{
				*vectorSource = new VectorSourceHDF5<I,T>(this->filePath);
				return cupcfd::error::E_SUCCESS;
			}

			// Shouldn't reach this part if we have a valid format
			DEBUGGABLE_ERROR; return cupcfd::error::E_ERROR;
		}
	}
}

// Explicit Instantiation
using namespace cupcfd::data_structures;

template class cupcfd::data_structures::VectorSourceFileConfig<int, float>;
template class cupcfd::data_structures::VectorSourceFileConfig<int, double>;
