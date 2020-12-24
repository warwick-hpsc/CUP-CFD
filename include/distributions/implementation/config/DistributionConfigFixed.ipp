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
 * Contains header level definitions for the DistributionConfigFixed Class
 */

#ifndef CUPCFD_DISTRIBUTIONS_DISTRIBUTION_CONFIG_FIXED_IPP_H
#define CUPCFD_DISTRIBUTIONS_DISTRIBUTION_CONFIG_FIXED_IPP_H

#include "DistributionFixed.h"

namespace cupcfd
{
	namespace distributions
	{
		template <class I, class T>
		DistributionConfigFixed<I,T>::DistributionConfigFixed()
		:fixedValue(T(0))
		{
		
		}
		
		template <class I, class T>
		DistributionConfigFixed<I,T>::DistributionConfigFixed(T fixedValue)
		:fixedValue(fixedValue)
		{
		
		}
		
		template <class I, class T>
		DistributionConfigFixed<I,T>::DistributionConfigFixed(DistributionConfigFixed<I,T>& source)
		{
			// Passthrough to copy operator
			*this = source;
		}
		
				
		template <class I, class T>
		DistributionConfigFixed<I,T>::~DistributionConfigFixed()
		{
		
		}
		
		template <class I, class T>
		T DistributionConfigFixed<I,T>::getFixedValue() {
			return this->fixedValue;
		}
		
		template <class I, class T>
		void DistributionConfigFixed<I,T>::setFixedValue(T fixedValue) {
			this->fixedValue = fixedValue;
		}
	
		template <class I, class T>
		void DistributionConfigFixed<I,T>::operator=(const DistributionConfigFixed<I,T>& source) {
			// Call Base
			DistributionConfig<I,T>::operator=(source);
			
			this->fixedValue = source.fixedValue;
		}
		
		template <class I, class T>
		DistributionConfigFixed<I,T> * DistributionConfigFixed<I,T>::clone() {
			return new DistributionConfigFixed(*this);
		}
		
		template <class I, class T>
		void DistributionConfigFixed<I,T>::buildDistribution(Distribution<I,T> ** dist) {
			*dist = new DistributionFixed<I,T>(this->fixedValue);
		}
	}
}

#endif
