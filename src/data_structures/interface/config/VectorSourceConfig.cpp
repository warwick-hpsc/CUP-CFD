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
		VectorSourceConfig<I,T>::~VectorSourceConfig()
		{
			// Nothing to do
		}
	}
}

// Explicit Instantiation
using namespace cupcfd::data_structures;

template class cupcfd::data_structures::VectorSourceConfig<int, float>;
template class cupcfd::data_structures::VectorSourceConfig<int, double>;
