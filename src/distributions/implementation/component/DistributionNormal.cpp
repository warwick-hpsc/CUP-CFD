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
 * Contains definitions for the DistributionNormal Class
 */

#include "DistributionNormal.h"

namespace cupcfd
{
	namespace distributions
	{
		// === Constructor/Deconstructors ===

		template <class I, class T>
		DistributionNormal<I,T>::DistributionNormal(T mean, T stdev, T lbound, T ubound)
		: Distribution<I,T>(),
		 lbound(lbound),
		 ubound(ubound),
		 mean(mean),
		 stdev(stdev)
		{
			if (lbound > mean) {
				throw(std::runtime_error("DistributionNormal constructor: lbound must not exceed mean"));
			}
			if (ubound < mean) {
				throw(std::runtime_error("DistributionNormal constructor: ubound must not be less than mean"));
			}
			if (lbound == ubound) {
				throw(std::runtime_error("DistributionNormal constructor: lbound must differ to ubound"));
			}

			std::random_device seedSource;

			this->dist = new std::normal_distribution<T>(this->mean, this->stdev);
			this->rEngine = new std::default_random_engine(seedSource());
		}

		template <class I, class T>
		DistributionNormal<I,T>::DistributionNormal(DistributionNormal<I,T>& source)
		: Distribution<I,T>(source)
		{
			this->lbound = source.lbound;
			this->ubound = source.ubound;
			this->mean = source.mean;
			this->stdev = source.stdev;
		}

		template <class I, class T>
		DistributionNormal<I,T>::~DistributionNormal()
		{
			delete this->dist;
			delete this->rEngine;
		}

		// === Concrete Methods ===

		// === Overloaded Inherited Methods ===

		template <class I, class T>
		void DistributionNormal<I,T>::getValues(T * values, I nValues) {
			for(I i = 0; i < nValues; i++) {
				// Loop until we get a number that is within bounds.
				T val = (*(this->dist))(*(this->rEngine));
				while ((val < lbound) || (val >= ubound)) {
					val = (*(this->dist))(*(this->rEngine));
				}
				values[i] = val;
			}
		}

		template <class I, class T>
		DistributionNormal<I,T> * DistributionNormal<I,T>::clone() {
			// Pure Virtual - Shouldn't be used
			return new DistributionNormal(*this);
		}

		// === Pure Virtual Methods ===
	}
}

// Explicit Instantiation
template class cupcfd::distributions::DistributionNormal<int, float>;
template class cupcfd::distributions::DistributionNormal<int, double>;
