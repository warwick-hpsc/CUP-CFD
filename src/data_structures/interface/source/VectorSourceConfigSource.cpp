/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Contains definitions for the VectorSourceConfigSource class
 */

#include "VectorSourceConfigSource.h"

namespace cupcfd
{
	namespace data_structures
	{
		template <class I, class T>
		VectorSourceConfigSource<I,T>::VectorSourceConfigSource()
		{

		}

		// template <class I, class T>
		// VectorSourceConfigSource<I,T>::VectorSourceConfigSource(const VectorSourceConfigSource<I,T>& source)
		// {

		// }

		template <class I, class T>
		VectorSourceConfigSource<I,T>::~VectorSourceConfigSource()
		{

		}

		// template <class I, class T>
		// void VectorSourceConfigSource<I,T>::operator=(const VectorSourceConfigSource<I,T>& source)
		// {

		// }

		// template <class I, class T>
		// VectorSourceConfigSource<I,T> * VectorSourceConfigSource<I,T>::clone()
		// {

		// }

		// template <class I, class T>
		// cupcfd::error::eCodes VectorSourceConfigSource<I,T>::buildVectorSourceConfig(VectorSourceConfig<I,T> ** vectorSourceConfig)
		// {

		// }
	}
}

// Explicit Instantiation
template class cupcfd::data_structures::VectorSourceConfigSource<int, float>;
template class cupcfd::data_structures::VectorSourceConfigSource<int, double>;
template class cupcfd::data_structures::VectorSourceConfigSource<long, float>;
template class cupcfd::data_structures::VectorSourceConfigSource<long, double>;
