/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Class Definition for the ExchangePatternConfigSourceJSON class.
 */

// Header for this class
#include "ExchangePatternConfigSourceJSON.h"

// File access for reading into JSON structures
#include <fstream>
#include <iostream>

namespace cupcfd
{
	namespace comm
	{
		// === Constructors/Deconstructors ===

		ExchangePatternConfigSourceJSON::ExchangePatternConfigSourceJSON(Json::Value& parseJSON)
		:ExchangePatternConfigSource()
		{
			this->configData = parseJSON;
		}

		ExchangePatternConfigSourceJSON::~ExchangePatternConfigSourceJSON()
		{
			// Nothing to do currently
		}

		// === Concrete Methods ===

		cupcfd::error::eCodes ExchangePatternConfigSourceJSON::getExchangeMethod(ExchangeMethod * method)
		{
			Json::Value dataSourceType;

			// Access the correct field
			dataSourceType = this->configData["Method"];

			// Check the value and return the appropriate ID
			if(dataSourceType == Json::Value::null)
			{
				DEBUGGABLE_ERROR; return cupcfd::error::E_CONFIG_OPT_NOT_FOUND;
			}
			else if(dataSourceType == "NBOneSided")
			{
				*method = EXCHANGE_NONBLOCKING_ONE_SIDED;
				return cupcfd::error::E_SUCCESS;
			}
			else if(dataSourceType == "NBTwoSided")
			{
				*method = EXCHANGE_NONBLOCKING_TWO_SIDED;
				return cupcfd::error::E_SUCCESS;
			}

			// Found, but not a matching value
			DEBUGGABLE_ERROR; return cupcfd::error::E_CONFIG_INVALID_VALUE;
		}

		cupcfd::error::eCodes ExchangePatternConfigSourceJSON::buildExchangePatternConfig(ExchangePatternConfig ** patternConfig)
		{
			cupcfd::error::eCodes status;
			ExchangeMethod method;

			status = this->getExchangeMethod(&method);
			if(status != cupcfd::error::E_SUCCESS)
			{
				return status;
			}

			*patternConfig = new ExchangePatternConfig(method);

			return cupcfd::error::E_SUCCESS;
		}
	}
}
