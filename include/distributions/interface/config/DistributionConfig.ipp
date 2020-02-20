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
 * Contains header level definitions for the DistributionConfig Class
 */

#ifndef CUPCFD_DISTRIBUTIONS_DISTRIBUTION_CONFIG_IPP_H
#define CUPCFD_DISTRIBUTIONS_DISTRIBUTION_CONFIG_IPP_H

namespace cupcfd
{
	namespace distributions
	{
		// === Constructors/Deconstructors ===
		
		template <class I, class T>
		DistributionConfig<I,T>::DistributionConfig()
		{
		
		}
			
		template <class I, class T>	
		DistributionConfig<I,T>::DistributionConfig(DistributionConfig<I,T>& source)
		{
			// Nothing to copy currently
		}
				
		template <class I, class T>
		DistributionConfig<I,T>::~DistributionConfig()
		{
		
		}

		// === Concrete Methods ===
		
		template <class I, class T>
		void DistributionConfig<I,T>::operator=(const DistributionConfig<I,T>& source)
		{

		}
		
		// === Pure Virtual Methods ===
		// Here to satisfy linker
		
		template <class I, class T>
		DistributionConfig<I,T> * DistributionConfig<I,T>::clone()
		{
			return nullptr;
		}
		
		template <class I, class T>
		cupcfd::error::eCodes DistributionConfig<I,T>::buildDistribution(Distribution<I,T> ** dist)
		{
			return cupcfd::error::E_SUCCESS;
		}
	}
}

#endif
