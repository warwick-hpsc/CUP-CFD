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

		template <class I, class T>
		Distribution<I,T>::~Distribution()
		{

		}
	}
}

// Explicit Instantiation
template class cupcfd::distributions::Distribution<int, float>;
template class cupcfd::distributions::Distribution<int, double>;
