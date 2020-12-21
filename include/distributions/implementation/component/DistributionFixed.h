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
 * Contains declarations for the DistributionFixed Class
 */

#ifndef CUPCFD_DISTRIBUTIONS_FIXED_INCLUDE_H
#define CUPCFD_DISTRIBUTIONS_FIXED_INCLUDE_H

// C++ Library
#include <random>

// Base Class
#include "Distribution.h"

// Error Codes
#include "Error.h"

namespace cupcfd
{
	namespace distributions
	{
		/**
		 * An interface class to retrieve random data elements
		 * from a distribution.
		 *
		 * @tparam I The type of the index scheme.
		 * @tparam T The type of the values generated by the distribution.
		 */
		template <class I, class T>
		class DistributionFixed : public Distribution<I,T>
		{
			public:
				// === Members ===

				T fixedValue;

				// === Constructors/Deconstructors

				/**
				 * Constructor:
				 * Setup a distribution that always returns the same value
				 *
				 * @param fixedValue The fixed value this distribution always generates
				 */
				DistributionFixed(T fixedValue);

				/**
				 * Copy Constructor
				 *
				 */
				DistributionFixed(DistributionFixed<I,T>& source);

				/**
				 * Deconstructor
				 */
				virtual ~DistributionFixed();

				// === Concrete Methods ===

				// === Overloaded Inherited Methods ===

				__attribute__((warn_unused_result))
				cupcfd::error::eCodes getValues(T * values, I nValues);

				DistributionFixed<I,T> * clone();

				// === Pure Virtual Methods ===
		};
	}
}

// Include Header Level Definitions
#include "DistributionFixed.ipp"

#endif
