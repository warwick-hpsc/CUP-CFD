/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Contains definitions for the ExchangePatternConfigSource class
 */

#include "ExchangePatternConfigSource.h"

namespace cupcfd
{
	namespace comm
	{
		ExchangePatternConfigSource::ExchangePatternConfigSource()
		{

		}

		ExchangePatternConfigSource::~ExchangePatternConfigSource()
		{

		}

		cupcfd::error::eCodes ExchangePatternConfigSource::buildExchangePatternConfig(ExchangePatternConfig ** config) {
			cupcfd::error::eCodes status;

			// Get the required data from the source
			ExchangeMethod method;
			status = this->getExchangeMethod(&method);
			CHECK_ERROR_CODE(status)
			if(status != cupcfd::error::E_SUCCESS) return status;

			// Create new object
			*config = new ExchangePatternConfig(method);

			return cupcfd::error::E_SUCCESS;
		}
	}
}
