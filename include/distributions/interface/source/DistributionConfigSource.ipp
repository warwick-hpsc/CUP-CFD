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
 * Contains declarations for the DistributionConfig Class
 */

#ifndef CUPCFD_DISTRIBUTIONS_DISTRIBUTION_CONFIG_SOURCE_IPP_H
#define CUPCFD_DISTRIBUTIONS_DISTRIBUTION_CONFIG_SOURCE_IPP_H

namespace cupcfd
{
	namespace distributions
	{
		// === Constructors/Deconstructors ===
	
		template <class I, class T>
		DistributionConfigSource<I,T>::DistributionConfigSource()
		{
		
		}
		
		template <class I, class T>
		DistributionConfigSource<I,T>::DistributionConfigSource(const DistributionConfigSource<I,T>& source)
		{
		
		}
		
		template <class I, class T>
		DistributionConfigSource<I,T>::~DistributionConfigSource()
		{
		
		}
		
		// === Pure Virtual Methods ===
		// Definitions here to satisfy linker
		
		template <class I, class T>
		void DistributionConfigSource<I,T>::operator=(const DistributionConfigSource<I,T>& source)
		{
		
		}
		
		template <class I, class T>
		DistributionConfigSource<I,T> * DistributionConfigSource<I,T>::clone()
		{
		
		}
		
		template <class I, class T>
		cupcfd::error::eCodes DistributionConfigSource<I,T>::buildDistributionConfig(DistributionConfig<I,T> ** distConfig)
		{
			return cupcfd::error::E_SUCCESS;
		}
	}
}

#endif
