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
 * Contains header level definitions for the DistributionConfigUniform Class
 */

#ifndef CUPCFD_DISTRIBUTIONS_DISTRIBUTION_CONFIG_UNIFORM_IPP_H
#define CUPCFD_DISTRIBUTIONS_DISTRIBUTION_CONFIG_UNIFORM_IPP_H

#include "DistributionUniform.h"

namespace cupcfd
{
	namespace distributions
	{
		template <class I, class T>
		DistributionConfigUniform<I,T>::DistributionConfigUniform()
		:lbound(T(0)),
		 ubound(T(0))
		{
		
		}
		
		template <class I, class T>
		DistributionConfigUniform<I,T>::DistributionConfigUniform(T lbound, T ubound)
		:lbound(lbound),
		 ubound(ubound)
		{
		
		}
		
		template <class I, class T>
		DistributionConfigUniform<I,T>::DistributionConfigUniform(DistributionConfigUniform<I,T>& source)
		{
			*this = source;
		}
		
				
		template <class I, class T>
		DistributionConfigUniform<I,T>::~DistributionConfigUniform()
		{
		
		}
		
		template <class I, class T>
		T DistributionConfigUniform<I,T>::getLBound() {
			return this->lbound;
		}
		
		template <class I, class T>
		void DistributionConfigUniform<I,T>::setLBound(T lbound) {
			this->lbound = lbound;
		}
		
		template <class I, class T>
		T DistributionConfigUniform<I,T>::getUBound() {
			return this->ubound;
		}
		
		template <class I, class T>
		void DistributionConfigUniform<I,T>::setUBound(T ubound) {
			this->ubound = ubound;
		}
		
		template <class I, class T>
		void DistributionConfigUniform<I,T>::operator=(const DistributionConfigUniform<I,T>& source) {
			// Call Base
			DistributionConfig<I,T>::operator=(source);
			
			this->lbound = source.lbound;
			this->ubound = source.ubound;
		}
		
		template <class I, class T>
		DistributionConfigUniform<I,T> * DistributionConfigUniform<I,T>::clone() {
			return new DistributionConfigUniform(*this);
		}
		
		template <class I, class T>
		void DistributionConfigUniform<I,T>::buildDistribution(Distribution<I,T> ** dist) {
			*dist = new DistributionUniform<I,T>(this->lbound, this->ubound);
		}
	}
}

#endif
