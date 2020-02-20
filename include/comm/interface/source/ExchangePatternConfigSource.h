/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Contains declarations for the ExchangePatternConfigSource class
 */

#ifndef CUPCFD_COMM_EXCHANGEPATTERN_CONFIG_SOURCE_INCLUDE_H
#define CUPCFD_COMM_EXCHANGEPATTERN_CONFIG_SOURCE_INCLUDE_H

#include "ExchangePatternConfig.h"
#include "Error.h"

namespace cupcfd
{
	namespace comm
	{
		/**
		 * Define a set of interface methods for accessing
		 * exchange pattern configuration data from some
		 * source (such as a file).
		 */
		class ExchangePatternConfigSource
		{
			public:
				// === Members ===

				// === Constructors/Deconstructors ===

				/**
				 * Constructor.
				 *
				 * Currently this method does nothing.
				 */
				ExchangePatternConfigSource();

				/**
				 * Deconstructor
				 */
				virtual ~ExchangePatternConfigSource();

				// === Concrete Methods ===

				/**
				 * Build a ExchangePatternConfig object using the data stored in this ExchangePatternConfigSource.
				 *
				 * This method will dynamically create the appropriate polymorphic type as a new object and store the pointer.
				 *
				 * @param config A pointer to the location where the ExchangePatternConfig will be created.
				 *
				 * @return An error status indicating the success or failure of the operation
				 * @retval cupcfd::error::E_SUCCESS Success
				 * @retval cupcfd::error::E_CONFIG_OPT_NOT_FOUND An option required to build the config was found to be missing.
				 * @retval cupcfd::error::E_CONFIG_INVALID_VALUE One or more options were found to have invalid values.
				 */
				cupcfd::error::eCodes buildExchangePatternConfig(ExchangePatternConfig ** config);

				// === Pure Virtual Methods ===

				/**
				 * Retrieve the exchange method specified in the source.
				 *
				 * @param A pointer to the location that will be updated with the method identifier.
				 *
				 * @return An error status indicating the success or failure of the operation
				 * @retval cupcfd::error::E_SUCCESS Success
				 * @retval cupcfd::error::E_CONFIG_OPT_NOT_FOUND No data source was found that specifies the partitioner ID
				 * or is improperly formatted
				 * @retval cupcfd::error::E_CONFIG_INVALID_VALUE A suitable data source was found, but not with a valid value
				 */
				virtual cupcfd::error::eCodes getExchangeMethod(ExchangeMethod * method) = 0;
		};
	}
}

// Include Header Level Definitions
#include "ExchangePatternConfigSource.ipp"

#endif
