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
 * Contains declarations for the DistributionConfigSourceFixedJSON Class
 */

#ifndef CUPCFD_DISTRIBUTIONS_DISTRIBUTION_CONFIG_SOURCE_FIXED_JSON_IPP_H
#define CUPCFD_DISTRIBUTIONS_DISTRIBUTION_CONFIG_SOURCE_FIXED_JSON_IPP_H

// File access for reading into JSON structures
#include <fstream>
#include "DistributionConfigFixed.h"

namespace cupcfd
{
	namespace distributions
	{
		// === Constructors/Deconstructors ===
	
		template <class I, class T>
		DistributionConfigSourceFixedJSON<I,T>::DistributionConfigSourceFixedJSON(std::string configFilePath, std::string * topLevel, int nTopLevel)
		:DistributionConfigSourceFixed<I,T>()
		{
			std::ifstream source(configFilePath, std::ifstream::binary);
			source >> this->configData;

			for(int i = 0; i < nTopLevel; i++)
			{
				this->configData = this->configData[topLevel[i]];
			}
			this->configData = this->configData["FixedDistribution"];
		}
		
		template <class I, class T>
		DistributionConfigSourceFixedJSON<I,T>::DistributionConfigSourceFixedJSON(Json::Value& parseJSON)
		: DistributionConfigSourceFixed<I,T>()
		{
			this->configData = parseJSON;
		}
		
		
		template <class I, class T>
		DistributionConfigSourceFixedJSON<I,T>::DistributionConfigSourceFixedJSON(const DistributionConfigSourceFixedJSON<I,T>& source)
		:DistributionConfigSourceFixed<I,T>()
		{
			*this = source;
		}
		
		template <class I, class T>
		DistributionConfigSourceFixedJSON<I,T>::~DistributionConfigSourceFixedJSON()
		{
		
		}
		
		template <class I, class T>
		cupcfd::error::eCodes DistributionConfigSourceFixedJSON<I,T>::getFixedValue(T * value)
		{
			Json::Value dataSourceType;

			// Access the correct field
			dataSourceType = this->configData["value"];

			// Check the value and return the appropriate ID
			if(dataSourceType == Json::Value::null)
			{
				return cupcfd::error::E_CONFIG_OPT_NOT_FOUND;
			}
			else
			{
				// Note: This will result in loss of precision if storage type is less than a double
				*value = T(dataSourceType.asDouble());
				return cupcfd::error::E_SUCCESS;
			}

			// Found, but not a matching value
			return cupcfd::error::E_CONFIG_INVALID_VALUE;
		}

		template <class I, class T>
		void DistributionConfigSourceFixedJSON<I,T>::operator=(const DistributionConfigSourceFixedJSON<I,T>& source)
		{
			this->configData = source.configData;
		}
		
		template <class I, class T>
		DistributionConfigSourceFixedJSON<I,T> * DistributionConfigSourceFixedJSON<I,T>::clone()
		{
			return new DistributionConfigSourceFixedJSON<I,T>(*this);
		}
	}
}

#endif
