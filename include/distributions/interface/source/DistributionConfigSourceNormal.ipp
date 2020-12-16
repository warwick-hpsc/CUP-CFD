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
 * Contains declarations for the DistributionConfigSourceNormalClass
 */

#ifndef CUPCFD_DISTRIBUTIONS_DISTRIBUTION_CONFIG_SOURCE_NORMAL_IPP_H
#define CUPCFD_DISTRIBUTIONS_DISTRIBUTION_CONFIG_SOURCE_NORMAL_IPP_H

#include "DistributionConfigNormal.h"

namespace cupcfd
{
	namespace distributions
	{
		// === Constructors/Deconstructors ===
	
		template <class I, class T>
		DistributionConfigSourceNormal<I,T>::DistributionConfigSourceNormal()
		{
		
		}
		
		// template <class I, class T>
		// DistributionConfigSourceNormal<I,T>::DistributionConfigSourceNormal(const DistributionConfigSourceNormal<I,T>& source)
		// {
		
		// }
		
		template <class I, class T>
		DistributionConfigSourceNormal<I,T>::~DistributionConfigSourceNormal()
		{
		
		}
		
		// === Concrete Methods ===
		
		template <class I, class T>
		cupcfd::error::eCodes DistributionConfigSourceNormal<I,T>::buildDistributionConfig(DistributionConfig<I,T> ** distConfig)
		{
			cupcfd::error::eCodes status;
			
			T lBound, uBound, mean, stDev;
			
			status = this->getLBound(&lBound);
			if(status != cupcfd::error::E_SUCCESS)
			{
				return status;
			}
			
			status = this->getUBound(&uBound);
			if(status != cupcfd::error::E_SUCCESS)
			{
				return status;
			}
					
			status = this->getMean(&mean);
			if(status != cupcfd::error::E_SUCCESS)
			{
				return status;
			}
			
			status = this->getStDev(&stDev);
			if(status != cupcfd::error::E_SUCCESS)
			{
				return status;
			}
			
			*distConfig = new DistributionConfigNormal<I,T>(lBound, uBound, mean, stDev);
			
			return cupcfd::error::E_SUCCESS;
		}
		
		// === Pure Virtual Methods ===
		// Definitions here to satisfy linker
		
		// template <class I, class T>
		// DistributionConfigSourceNormal<I,T> * DistributionConfigSourceNormal<I,T>::clone()
		// {
		// 	// Pure Virtual
		// }
		
		// template <class I, class T>
		// cupcfd::error::eCodes DistributionConfigSourceNormal<I,T>::getLBound(T * lBound)
		// {
		// 	// Pure Virtual
		// }

		// template <class I, class T>
		// cupcfd::error::eCodes DistributionConfigSourceNormal<I,T>::getUBound(T * uBound)
		// {
		// 	// Pure Virtual
		// }

		// template <class I, class T>
		// cupcfd::error::eCodes DistributionConfigSourceNormal<I,T>::getMean(T * mean)
		// {
		// 	// Pure Virtual
		// }

		// template <class I, class T>
		// cupcfd::error::eCodes DistributionConfigSourceNormal<I,T>::getStDev(T * stDev)
		// {
		// 	// Pure Virtual
		// }
	}
}

#endif
