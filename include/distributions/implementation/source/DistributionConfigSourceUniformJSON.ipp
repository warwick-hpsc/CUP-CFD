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
 * Contains declarations for the DistributionConfigSourceUniformJSON Class
 */

#ifndef CUPCFD_DISTRIBUTIONS_DISTRIBUTION_CONFIG_SOURCE_UNIFORM_JSON_IPP_H
#define CUPCFD_DISTRIBUTIONS_DISTRIBUTION_CONFIG_SOURCE_UNIFORM_JSON_IPP_H

#include <fstream>

namespace cupcfd
{
	namespace distributions
	{
		// === Constructors/Deconstructors ===
	
		template <class I, class T>
		DistributionConfigSourceUniformJSON<I,T>::DistributionConfigSourceUniformJSON(std::string configFilePath, std::string * topLevel, int nTopLevel)
		:DistributionConfigSourceUniform<I,T>()
		{
			std::ifstream source(configFilePath, std::ifstream::binary);
			source >> this->configData;

			if (topLevel != nullptr) {
				for(int i = 0; i < nTopLevel; i++)
				{
					this->configData = this->configData[topLevel[i]];
				}
			}
			this->configData = this->configData["UniformDistribution"];
		}
		
		template <class I, class T>
		DistributionConfigSourceUniformJSON<I,T>::DistributionConfigSourceUniformJSON(Json::Value& parseJSON)
		: DistributionConfigSourceUniform<I,T>()
		{
			this->configData = parseJSON;
		}
		
		template <class I, class T>
		DistributionConfigSourceUniformJSON<I,T>::DistributionConfigSourceUniformJSON(const DistributionConfigSourceUniformJSON<I,T>& source)
		{
			*this = source;
		}
		
		template <class I, class T>
		DistributionConfigSourceUniformJSON<I,T>::~DistributionConfigSourceUniformJSON()
		{
		
		}
		
		template <class I, class T>
		void DistributionConfigSourceUniformJSON<I,T>::operator=(const DistributionConfigSourceUniformJSON<I,T>& source)
		{
			this->configData = source.configData;
		}
		
		template <class I, class T>
		DistributionConfigSourceUniformJSON<I,T> * DistributionConfigSourceUniformJSON<I,T>::clone()
		{
			return new DistributionConfigSourceUniformJSON<I,T>(*this);
		}
		
		template <class I, class T>
		cupcfd::error::eCodes DistributionConfigSourceUniformJSON<I,T>::getLBound(T * lBound)
		{
			Json::Value dataSourceType;

			// Access the correct field
			dataSourceType = this->configData["lbound"];

			// Check the value and return the appropriate ID
			if(dataSourceType == Json::Value::null)
			{
				return cupcfd::error::E_CONFIG_OPT_NOT_FOUND;
			}
			else
			{
				// Note: This will result in loss of precision if storage type is less than a double
				*lBound = T(dataSourceType.asDouble());
				return cupcfd::error::E_SUCCESS;
			}

			// Found, but not a matching value
			return cupcfd::error::E_CONFIG_INVALID_VALUE;
		}
		
		template <class I, class T>
		cupcfd::error::eCodes DistributionConfigSourceUniformJSON<I,T>::getUBound(T * uBound)
		{
			Json::Value dataSourceType;

			// Access the correct field
			dataSourceType = this->configData["ubound"];

			// Check the value and return the appropriate ID
			if(dataSourceType == Json::Value::null)
			{
				return cupcfd::error::E_CONFIG_OPT_NOT_FOUND;
			}
			else
			{
				// Note: This will result in loss of precision if storage type is less than a double
				*uBound = T(dataSourceType.asDouble());
				return cupcfd::error::E_SUCCESS;
			}

			// Found, but not a matching value
			return cupcfd::error::E_CONFIG_INVALID_VALUE;
		}
	}
}

#endif
