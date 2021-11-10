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
		
		template <class I, class T>
		DistributionConfigSourceNormal<I,T>::~DistributionConfigSourceNormal()
		{
		
		}
		
		// === Concrete Methods ===
		
		template <class I, class T>
		cupcfd::error::eCodes DistributionConfigSourceNormal<I,T>::buildDistributionConfig(DistributionConfig<I,T> ** distConfig) {
			cupcfd::error::eCodes status;
			
			T lBound, uBound, mean, stDev;
			
			status = this->getLBound(&lBound);
			CHECK_ECODE(status)
			
			status = this->getUBound(&uBound);
			CHECK_ECODE(status)
					
			status = this->getMean(&mean);
			CHECK_ECODE(status)
			
			status = this->getStDev(&stDev);
			CHECK_ECODE(status)

			*distConfig = new DistributionConfigNormal<I,T>(lBound, uBound, mean, stDev);
			
			return cupcfd::error::E_SUCCESS;
		}
	}
}

#endif
