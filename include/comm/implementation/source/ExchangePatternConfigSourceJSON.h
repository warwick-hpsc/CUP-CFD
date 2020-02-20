/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Class Declaration for the ExchangePatternConfigSourceJSON class.
 */

#ifndef CUPCFD_COMM_EXCHANGEPATTERN_CONFIG_SOURCE_JSON_INCLUDE_H
#define CUPCFD_COMM_EXCHANGEPATTERN_CONFIG_SOURCE_JSON_INCLUDE_H

// C++ Std Lib
#include <string>
#include <vector>

// Error Codes
#include "Error.h"

// Abstract interface
#include "ExchangePatternConfigSource.h"

// JsonCPP - Supplied as standalone in include/io/jsoncpp
#include "json.h"
#include "json-forwards.h"

namespace cupcfd
{
	namespace comm
	{
		/**
		 * Defines a interface for accessing ExchangePattern configuration
		 * options from a JSON data source.
		 *
		 * === Fields ===
		 *
		 * Required:
		 * Method: String - Valid Entries are "NBOneSided", "NBTwoSided".
		 * Chooses between one sided and two sided non-blocking communications for
		 * performing the exchange
		 *
		 * Optional:
		 * None
		 */
		class ExchangePatternConfigSourceJSON : public ExchangePatternConfigSource
		{
			public:
				// === Members ===

				/** Json Data Store containing fields for this JSON source **/
				Json::Value configData;

				// === Constructors/Deconstructors ===

				/**
				 * Parse the JSON record provided for fields belonging to Configuration for ExchangePatterns JSON.
				 * The fields are presumed to start from level 0 (i.e. they are not nested inside other objects)
				 *
				 * @param parseJSON The contents of a JSON record with the appropriate fields.
				 */
				ExchangePatternConfigSourceJSON(Json::Value& parseJSON);

				/**
				 * Deconstructor
				 */
				virtual ~ExchangePatternConfigSourceJSON();

				// === Inherited Overloaded Methods ===
				cupcfd::error::eCodes getExchangeMethod(ExchangeMethod * method);

				// === Concrete Methods ===

				/**
				 *
				 */
				cupcfd::error::eCodes buildExchangePatternConfig(ExchangePatternConfig ** patternConfig);

				// === Pure Virtual Methods ===
		};
	}
}

// Include Header Level Definitions
#include "ExchangePatternConfigSourceJSON.ipp"

#endif
