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
 * Contains declarations for the DistributionConfigSourceUniform Class
 */

#ifndef CUPCFD_DISTRIBUTIONS_DISTRIBUTION_CONFIG_SOURCE_UNIFORM_IPP_H
#define CUPCFD_DISTRIBUTIONS_DISTRIBUTION_CONFIG_SOURCE_UNIFORM_IPP_H

#include "DistributionConfigUniform.h"

namespace cupcfd
{
	namespace distributions
	{
		// === Constructors/Deconstructors ===
	
		template <class I, class T>
		DistributionConfigSourceUniform<I,T>::DistributionConfigSourceUniform()
		{
		
		}
		
		template <class I, class T>
		DistributionConfigSourceUniform<I,T>::~DistributionConfigSourceUniform()
		{
		
		}
		
		// === Concrete Methods ===
		
		template <class I, class T>
		cupcfd::error::eCodes DistributionConfigSourceUniform<I,T>::buildDistributionConfig(DistributionConfig<I,T> ** distConfig) {
			cupcfd::error::eCodes status;
			
			T lBound, uBound;
			
			status = this->getLBound(&lBound);
			CHECK_ECODE(status)
			
			status = this->getUBound(&uBound);
			CHECK_ECODE(status)
					
			*distConfig = new DistributionConfigUniform<I,T>(lBound, uBound);
			
			return cupcfd::error::E_SUCCESS;
		}
	}
}

#endif
