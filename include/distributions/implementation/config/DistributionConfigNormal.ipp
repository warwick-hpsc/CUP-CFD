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
 * Contains header level definitions for the DistributionConfigNormal Class
 */

#ifndef CUPCFD_DISTRIBUTIONS_DISTRIBUTION_CONFIG_NORMAL_IPP_H
#define CUPCFD_DISTRIBUTIONS_DISTRIBUTION_CONFIG_NORMAL_IPP_H

#include "DistributionNormal.h"

namespace cupcfd
{
	namespace distributions
	{
		template <class I, class T>
		DistributionConfigNormal<I,T>::DistributionConfigNormal()
		:lbound(T(0)),
		 ubound(T(0)),
		 mean(T(0)),
		 stdev(T(0))
		{
		
		}
		
		template <class I, class T>
		DistributionConfigNormal<I,T>::DistributionConfigNormal(T lbound, T ubound, T mean, T stdev)
		:lbound(lbound),
		 ubound(ubound),
		 mean(mean),
		 stdev(stdev)
		{
		
		}
		
		template <class I, class T>
		DistributionConfigNormal<I,T>::DistributionConfigNormal(DistributionConfigNormal<I,T>& source)
		{
			// Passthrough to copy operator
			*this = source;
		}
		
				
		template <class I, class T>
		DistributionConfigNormal<I,T>::~DistributionConfigNormal()
		{
		
		}
		
		template <class I, class T>
		T DistributionConfigNormal<I,T>::getLBound()
		{
			return this->lbound;
		}
		
		template <class I, class T>
		void DistributionConfigNormal<I,T>::setLBound(T lbound)
		{
			this->lbound = lbound;
		}
		
		template <class I, class T>
		T DistributionConfigNormal<I,T>::getUBound()
		{
			return this->ubound;
		}
		
		template <class I, class T>
		void DistributionConfigNormal<I,T>::setUBound(T ubound)
		{
			this->ubound = ubound;
		}
		
		template <class I, class T>
		T DistributionConfigNormal<I,T>::getMean()
		{
			return this->mean;
		}
		
		template <class I, class T>
		void DistributionConfigNormal<I,T>::setMean(T mean)
		{
			this->mean = mean;
		}
		
		template <class I, class T>
		T DistributionConfigNormal<I,T>::getStdev()
		{
			return this->stdev;
		}
		
		template <class I, class T>
		void DistributionConfigNormal<I,T>::setStdev(T stdev)
		{
			this->stdev = stdev;
		}
		
		template <class I, class T>
		void DistributionConfigNormal<I,T>::operator=(const DistributionConfigNormal<I,T>& source)
		{
			// Call Base
			DistributionConfig<I,T>::operator=(source);
			
			this->lbound = source.lbound;
			this->ubound = source.ubound;
			this->mean = source.mean;
			this->stdev = source.stdev;
		}
		
		template <class I, class T>
		DistributionConfigNormal<I,T> * DistributionConfigNormal<I,T>::clone()
		{
			return new DistributionConfigNormal(*this);
		}
		
		template <class I, class T>
		cupcfd::error::eCodes DistributionConfigNormal<I,T>::buildDistribution(Distribution<I,T> ** dist)
		{
			*dist = new DistributionNormal<I,T>(this->mean, this->stdev, this->lbound, this->ubound);
			return cupcfd::error::E_SUCCESS;
		}
	}
}

#endif
