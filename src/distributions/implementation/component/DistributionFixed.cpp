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
 * Contains definitions for the DistributionFixed Class
 */

#include "DistributionFixed.h"

namespace cupcfd
{
	namespace distributions
	{
		// === Constructor/Deconstructors ===

		template <class I, class T>
		DistributionFixed<I,T>::DistributionFixed(T fixedValue)
		: Distribution<I,T>(),
		 fixedValue(fixedValue)
		{

		}

		template <class I, class T>
		DistributionFixed<I,T>::DistributionFixed(DistributionFixed<I,T>& source)
		{
			this->fixedValue = source.fixedValue;
		}

		template <class I, class T>
		DistributionFixed<I,T>::~DistributionFixed()
		{

		}

		// === Concrete Methods ===

		// === Overloaded Inherited Methods ===

		template <class I, class T>
		void DistributionFixed<I,T>::getValues(T * values, I nValues) {
			for(I i = 0; i < nValues; i++) {
				values[i] = this->fixedValue;
			}
		}

		template <class I, class T>
		DistributionFixed<I,T> * DistributionFixed<I,T>::clone() {
			// Pure Virtual - Shouldn't be used
			return new DistributionFixed(*this);
		}
	}
}

// Explicit Instantiation
template class cupcfd::distributions::DistributionFixed<int, float>;
template class cupcfd::distributions::DistributionFixed<int, double>;
