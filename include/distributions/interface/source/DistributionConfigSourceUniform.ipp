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
		DistributionConfigSourceUniform<I,T>::DistributionConfigSourceUniform(const DistributionConfigSourceUniform<I,T>& source)
		{
		
		}
		
		template <class I, class T>
		DistributionConfigSourceUniform<I,T>::~DistributionConfigSourceUniform()
		{
		
		}
		
		template <class I, class T>
		cupcfd::error::eCodes DistributionConfigSourceUniform<I,T>::buildDistributionConfig(DistributionConfig<I,T> ** distConfig)
		{
			cupcfd::error::eCodes status;
			T lBound, uBound;
			
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
			
			*distConfig = new DistributionConfigUniform<I,T>(lBound, uBound); 
		
			return cupcfd::error::E_SUCCESS;
		}

		template <class I, class T>
		void DistributionConfigSourceUniform<I,T>::operator=(const DistributionConfigSourceUniform<I,T>& source)
		{
		
		}
		
		template <class I, class T>
		DistributionConfigSourceUniform<I,T> * DistributionConfigSourceUniform<I,T>::clone()
		{
		
		}
		
		template <class I, class T>
		cupcfd::error::eCodes DistributionConfigSourceUniform<I,T>::getLBound(T * lBound)
		{
			return cupcfd::error::E_SUCCESS;
		}
		
		template <class I, class T>
		cupcfd::error::eCodes DistributionConfigSourceUniform<I,T>::getUBound(T * lBound)
		{
			return cupcfd::error::E_SUCCESS;
		}
	}
}

#endif
