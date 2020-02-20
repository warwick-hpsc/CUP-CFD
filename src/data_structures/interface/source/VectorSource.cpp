/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Contains definitions for the VectorSource class
 */

// Class Declarations
#include "VectorSource.h"

// C++ Library Function/Objects
#include <vector>

namespace cupcfd
{
	namespace data_structures
	{
		template <class I, class T>
		VectorSource<I, T>::VectorSource()
		{

		}

		template <class I, class T>
		VectorSource<I, T>::~VectorSource()
		{
			// Currently Nothing to Clean Up.
		}

		// === Pure Virtual ===

		template <class I, class T>
		cupcfd::error::eCodes VectorSource<I,T>::getSize(I * vecSize)
		{
			cupcfd::error::eCodes status;
			return cupcfd::error::E_SUCCESS;
		}

		template <class I, class T>
		cupcfd::error::eCodes VectorSource<I,T>::getData(T * data, I nData, I * indexes, I nIndexes, I indexBase)
		{
			cupcfd::error::eCodes status;
			return cupcfd::error::E_SUCCESS;
		}
	}
}

// Explicit Instantiation
template class cupcfd::data_structures::VectorSource<int, float>;
template class cupcfd::data_structures::VectorSource<int, double>;

