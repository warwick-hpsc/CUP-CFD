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
 * Contains declarations for the DistributionConfigSourceFixed Class
 */

#ifndef CUPCFD_DISTRIBUTIONS_DISTRIBUTION_CONFIG_SOURCE_FIXED_INCLUDE_H
#define CUPCFD_DISTRIBUTIONS_DISTRIBUTION_CONFIG_SOURCE_FIXED_INCLUDE_H

// Error Codes
#include "Error.h"

#include "DistributionConfigSource.h"

namespace cupcfd
{
	namespace distributions
	{
		/**
		 * General Interface for retrieving information from a Distribution config data source
		 * for DistributionConfigFixed objects.
		 *
		 * @tparam The datatype of any indexing scheme used by the distribution
		 * @tparam The datatype of the values generated by the distribution
		 */
		template <class I, class T>
		class DistributionConfigSourceFixed : public DistributionConfigSource<I,T>
		{
			public:
				// === Members ===

				// === Constructors/Deconstructors ===

				/**
				 * Default Constructor
				 *
				 * @tparam I The datatype of any indexing scheme used by the distribution
				 * @tparam T The datatype of the values generated by the distribution
				 */
				DistributionConfigSourceFixed();

				/**
				 * Constructor:
				 * Copy values from source to this newly constructed object
				 *
				 * @tparam I The datatype of any indexing scheme used by the distribution
				 * @tparam T The datatype of the values generated by the distribution
				 */
				// DistributionConfigSourceFixed(const DistributionConfigSourceFixed<I,T>& source);

				/**
				 * Deconstructor:
				 *
				 * @tparam I The datatype of any indexing scheme used by the distribution
				 * @tparam T The datatype of the values generated by the distribution
				 */
				virtual ~DistributionConfigSourceFixed();

				// === Concrete Methods ===

				virtual cupcfd::error::eCodes buildDistributionConfig(DistributionConfig<I,T> ** distConfig);

				// === Pure Virtual Methods ===

				/**
				 * Get the fixed value to be generated by a DistributionFixed
				 *
				 * @param value A pointer to where the value will be stored
				 *
				 * @tparam I The datatype of any indexing scheme used by the distribution
				 * @tparam T The datatype of the values generated by the distribution
				 *
				 * @return An error status indicating the success or failure of the operation
				 * @retval cupcfd::error::E_SUCCESS Success
				 * @retval cupcfd::error::E_CONFIG_OPT_NOT_FOUND No data source was found that specifies the partitioner ID
				 * or is improperly formatted
				 * @retval cupcfd::error::E_CONFIG_INVALID_VALUE A suitable data source was found, but not with a valid value
				 */
				virtual cupcfd::error::eCodes getFixedValue(T * value) = 0;

				virtual DistributionConfigSourceFixed<I,T> * clone() = 0;
		};
	}
}

// Include Header Level Definitions
#include "DistributionConfigSourceFixed.ipp"

#endif
