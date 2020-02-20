/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Contains definitions for the VectorSourceConfig class
 */

#include "VectorSourceConfig.h"

namespace cupcfd
{
	namespace data_structures
	{
		// === Constructors/Deconstructors ===

		template <class I, class T>
		VectorSourceConfig<I,T>::VectorSourceConfig()
		{
			// Nothing to do
		}

		template <class I, class T>
		VectorSourceConfig<I,T>::VectorSourceConfig(VectorSourceConfig& source)
		{
			*this = source;
		}

		template <class I, class T>
		VectorSourceConfig<I,T>::~VectorSourceConfig()
		{
			// Nothing to do
		}

		// === Concrete Methods ===

		template <class I, class T>
		void VectorSourceConfig<I,T>::operator=(VectorSourceConfig<I,T>& source)
		{
			// Nothing to do
		}

		// === Pure Virtual Methods ===
		// Here to satisfy linker

		template <class I, class T>
		VectorSourceConfig<I,T> * VectorSourceConfig<I,T>::clone()
		{
			return nullptr;
		}

		template <class I, class T>
		cupcfd::error::eCodes VectorSourceConfig<I,T>::buildVectorSource(VectorSource<I,T> ** vectorSource)
		{
			return cupcfd::error::E_SUCCESS;
		}
	}
}

// Explicit Instantiation
using namespace cupcfd::data_structures;

template class cupcfd::data_structures::VectorSourceConfig<int, float>;
template class cupcfd::data_structures::VectorSourceConfig<int, double>;
