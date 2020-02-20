/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Contains definitions for the ExchangePatternConfig class
 */

#include "ExchangePatternConfig.h"

namespace cupcfd
{
	namespace comm
	{
		ExchangePatternConfig::ExchangePatternConfig()
		{
			this->method = EXCHANGE_NONBLOCKING_TWO_SIDED;
		}

		ExchangePatternConfig::ExchangePatternConfig(ExchangeMethod method)
		{
			this->method = method;
		}

		ExchangePatternConfig::ExchangePatternConfig(ExchangePatternConfig& config)
		{
			*this = config;
		}

		ExchangePatternConfig::~ExchangePatternConfig()
		{
			// Nothing to do currently.
		}
	}
}
