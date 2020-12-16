/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Description
 *
 * Contains definitions for the Distribution Class
 */

#include "Distribution.h"

namespace cupcfd
{
	namespace distributions
	{
		// === Constructor/Deconstructors ===

		template <class I, class T>
		Distribution<I,T>::Distribution()
		{

		}

		// template <class I, class T>
		// Distribution<I,T>::Distribution(Distribution<I,T>& source)
		// {
		// 	// Nothing to do currently
		// }

		// template <class I, class T>
		// Distribution<I,T>::~Distribution()
		// {

		// }

		// // === Concrete Methods ===

		// // === Pure Virtual Methods ===
		// // Here to satisfy linker
		// template <class I, class T>
		// cupcfd::error::eCodes Distribution<I,T>::getValues(T * values, I nValues)
		// {
		// 	return cupcfd::error::E_SUCCESS;
		// }

		// template <class I, class T>
		// Distribution<I,T> * clone()
		// {
		// 	// Pure Virtual - Shouldn't be used
		// 	return nullptr;
		// }
	}
}

// Explicit Instantiation
template class cupcfd::distributions::Distribution<int, float>;
template class cupcfd::distributions::Distribution<int, double>;
