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
 * Contains declarations for the DistributionNormal Class
 */

#ifndef CUPCFD_DISTRIBUTIONS_NORMAL_INCLUDE_H
#define CUPCFD_DISTRIBUTIONS_NORMAL_INCLUDE_H

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
		class DistributionNormal : public Distribution<I,T>
		{
			public:
				// === Members ===

				/** C++ Library Distribution this class is a wrapper for **/
				std::normal_distribution<T> * dist;
				std::default_random_engine * rEngine;

				/** Lower bound for accepted generated values **/
				T lbound;

				/** Upper bound for accepted generated values **/
				T ubound;

				/** Mean **/
				T mean;

				/** Standard Deviation **/
				T stdev;

				// === Constructors/Deconstructors

				/**
				 * Constructor:
				 * Setup a normal distribution for generating numbers.
				 * The range of potential values is capped by a lower
				 * and upper bound.
				 *
				 * @param mean Mean of the distribution
				 * @param stdev Standard Deviation of the distribution
				 * @param lbound Inclusive lower bound of the range of generated numbers.
				 * @param ubound Non-inclusive upper bound of the range of generated numbers.
				 */
				DistributionNormal(T mean, T stdev, T lbound, T ubound);

				/**
				 * Copy Constructor
				 *
				 */
				DistributionNormal(DistributionNormal<I,T>& source);

				/**
				 * Deconstructor
				 */
				virtual ~DistributionNormal();

				// === Concrete Methods ===

				// === Overloaded Inherited Methods ===

				__attribute__((warn_unused_result))
				cupcfd::error::eCodes getValues(T * values, I nValues);

				DistributionNormal<I,T> * clone();

				// === Pure Virtual Methods ===
		};
	}
}

// Include Header Level Definitions
#include "DistributionNormal.ipp"

#endif
