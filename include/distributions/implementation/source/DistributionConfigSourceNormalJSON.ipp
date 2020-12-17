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
 * Contains declarations for the DistributionConfigSourceNormalJSON Class
 */

#ifndef CUPCFD_DISTRIBUTIONS_DISTRIBUTION_CONFIG_SOURCE_NORMAL_JSON_IPP_H
#define CUPCFD_DISTRIBUTIONS_DISTRIBUTION_CONFIG_SOURCE_NORMAL_JSON_IPP_H

#include <fstream>

namespace cupcfd
{
	namespace distributions
	{
		// === Constructors/Deconstructors ===
	
		template <class I, class T>
		DistributionConfigSourceNormalJSON<I,T>::DistributionConfigSourceNormalJSON(std::string configFilePath, std::string * topLevel, int nTopLevel)
		:DistributionConfigSourceNormal<I,T>()
		{
			std::ifstream source(configFilePath, std::ifstream::binary);
			source >> this->configData;

			for(int i = 0; i < nTopLevel; i++)
			{
				this->configData = this->configData[topLevel[i]];
			}
			this->configData = this->configData["NormalDistribution"];
		}
		
		template <class I, class T>
		DistributionConfigSourceNormalJSON<I,T>::DistributionConfigSourceNormalJSON(Json::Value& parseJSON)
		: DistributionConfigSourceNormal<I,T>()
		{
			this->configData = parseJSON;
		}
		
		template <class I, class T>
		DistributionConfigSourceNormalJSON<I,T>::DistributionConfigSourceNormalJSON(const DistributionConfigSourceNormalJSON<I,T>& source)
		{
			*this = source;
		}
		
		template <class I, class T>
		DistributionConfigSourceNormalJSON<I,T>::~DistributionConfigSourceNormalJSON()
		{
		
		}
		
		template <class I, class T>
		void DistributionConfigSourceNormalJSON<I,T>::operator=(const DistributionConfigSourceNormalJSON<I,T>& source)
		{
			this->configData = source.configData;
		}
		
		template <class I, class T>
		DistributionConfigSourceNormalJSON<I,T> * DistributionConfigSourceNormalJSON<I,T>::clone()
		{
			return new DistributionConfigSourceNormalJSON<I,T>(*this);
		}
		
		template <class I, class T>
		cupcfd::error::eCodes DistributionConfigSourceNormalJSON<I,T>::getLBound(T * lBound)
		{
			Json::Value dataSourceType;

			// Access the correct field
			dataSourceType = this->configData["lbound"];

			// Check the value and return the appropriate ID
			if(dataSourceType == Json::Value::null)
			{
				DEBUGGABLE_ERROR; return cupcfd::error::E_CONFIG_OPT_NOT_FOUND;
			}
			else
			{
				// Note: This will result in loss of precision if storage type is less than a double
				*lBound = T(dataSourceType.asDouble());
				return cupcfd::error::E_SUCCESS;
			}

			// Found, but not a matching value
			DEBUGGABLE_ERROR; return cupcfd::error::E_CONFIG_INVALID_VALUE;
		}

		template <class I, class T>
		cupcfd::error::eCodes DistributionConfigSourceNormalJSON<I,T>::getUBound(T * uBound)
		{
			Json::Value dataSourceType;

			// Access the correct field
			dataSourceType = this->configData["ubound"];

			// Check the value and return the appropriate ID
			if(dataSourceType == Json::Value::null)
			{
				DEBUGGABLE_ERROR; return cupcfd::error::E_CONFIG_OPT_NOT_FOUND;
			}
			else
			{
				// Note: This will result in loss of precision if storage type is less than a double
				*uBound = T(dataSourceType.asDouble());
				return cupcfd::error::E_SUCCESS;
			}

			// Found, but not a matching value
			DEBUGGABLE_ERROR; return cupcfd::error::E_CONFIG_INVALID_VALUE;
		}

		template <class I, class T>
		cupcfd::error::eCodes DistributionConfigSourceNormalJSON<I,T>::getMean(T * mean)
		{
			Json::Value dataSourceType;

			// Access the correct field
			dataSourceType = this->configData["mean"];

			// Check the value and return the appropriate ID
			if(dataSourceType == Json::Value::null)
			{
				DEBUGGABLE_ERROR; return cupcfd::error::E_CONFIG_OPT_NOT_FOUND;
			}
			else
			{
				// Note: This will result in loss of precision if storage type is less than a double
				*mean = T(dataSourceType.asDouble());
				return cupcfd::error::E_SUCCESS;
			}

			// Found, but not a matching value
			DEBUGGABLE_ERROR; return cupcfd::error::E_CONFIG_INVALID_VALUE;
		}

		template <class I, class T>
		cupcfd::error::eCodes DistributionConfigSourceNormalJSON<I,T>::getStDev(T * stDev)
		{
			Json::Value dataSourceType;

			// Access the correct field
			dataSourceType = this->configData["stdev"];

			// Check the value and return the appropriate ID
			if(dataSourceType == Json::Value::null)
			{
				DEBUGGABLE_ERROR; return cupcfd::error::E_CONFIG_OPT_NOT_FOUND;
			}
			else
			{
				// Note: This will result in loss of precision if storage type is less than a double
				*stDev = T(dataSourceType.asDouble());
				return cupcfd::error::E_SUCCESS;
			}

			// Found, but not a matching value
			DEBUGGABLE_ERROR; return cupcfd::error::E_CONFIG_INVALID_VALUE;
		}
	}
}

#endif
