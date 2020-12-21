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
 * Contains declarations for the DistributionConfig Class
 */

#ifndef CUPCFD_DISTRIBUTIONS_DISTRIBUTION_CONFIG_INCLUDE_H
#define CUPCFD_DISTRIBUTIONS_DISTRIBUTION_CONFIG_INCLUDE_H

// Error Codes
#include "Error.h"

// Distribution (For Builder)
#include "Distribution.h"

namespace cupcfd
{
	namespace distributions
	{
		/**
		 * Stores the general configuration components of distributions.
		 * Also acts as a base class for polymorphism.
		 *
		 * @tparam The datatype of any indexing scheme used by the distribution
		 * @tparam The datatype of the values generated by the distribution
		 */
		template <class I, class T>
		class DistributionConfig
		{
			public:
				// === Members ===

				// === Constructors/Deconstructors ===

				/**
				 * Constructor:
				 * Initialises all values to zero.
				 *
				 * @tparam The datatype of any indexing scheme used by the distribution
				 * @tparam The datatype of the values generated by the distribution
				 */
				DistributionConfig();

				/**
				 * Deconstructor:
				 * Currently does nothing.
				 *
				 * @tparam The datatype of any indexing scheme used by the distribution
				 * @tparam The datatype of the values generated by the distribution
				 */
				virtual ~DistributionConfig();

				// === Pure Virtual Methods ===

				/**
				 * Creates a new object containing a copy of the contents of this object
				 *
				 * @tparam The datatype of any indexing scheme used by the distribution
				 * @tparam The datatype of the values generated by the distribution
				 *
				 * @return A pointer to the newly created clone of this object.
				 */
				virtual DistributionConfig<I,T> * clone() = 0;

				/**
				 * Build a distribution object that is ready to generate values
				 *
				 * @param dist A pointer to the location where the pointer to the newly created
				 * distribution will be stored.
				 *
				 * @tparam The datatype of any indexing scheme used by the distribution
				 * @tparam The datatype of the values generated by the distribution
				 *
				 * @return An error status indicating the success or failure of the operation
				 * @retval cupcfd::error::E_SUCCESS Success
				 */
				__attribute__((warn_unused_result))
				virtual cupcfd::error::eCodes buildDistribution(Distribution<I,T> ** dist) = 0;
		};
	}
}

// Include Header Level Definitions
#include "DistributionConfig.ipp"

#endif
