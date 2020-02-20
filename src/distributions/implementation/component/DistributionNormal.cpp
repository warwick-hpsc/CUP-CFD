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
		 mean(mean),
		 stdev(stdev),
		 lbound(lbound),
		 ubound(ubound)
		{
			std::random_device seedSource;

			this->dist = new std::normal_distribution<T>(this->mean, this->stdev);
			this->rEngine = new std::default_random_engine(seedSource());
		}

		template <class I, class T>
		DistributionNormal<I,T>::DistributionNormal(DistributionNormal<I,T>& source)
		{
			this->mean = source.mean;
			this->stdev = source.stdev;
			this->lbound = source.lbound;
			this->ubound = source.ubound;
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
		cupcfd::error::eCodes DistributionNormal<I,T>::getValues(T * values, I nValues)
		{
			for(I i = 0; i < nValues; i++)
			{
				// Generate Initial Value
				T val = (*(this->dist))(*(this->rEngine));

				// Loop until we get a number that is within the specified range.
				// ToDo: Danger of inifinite loop if a bad range + mean/stdev is provided?
				// Might want to add a warning/error code/loop limit of some kind.
				while ((val < lbound) || (val >= ubound))
				{
					val = (*(this->dist))(*(this->rEngine));
				}

				values[i] = val;
			}

			return cupcfd::error::E_SUCCESS;
		}

		template <class I, class T>
		DistributionNormal<I,T> * DistributionNormal<I,T>::clone()
		{
			// Pure Virtual - Shouldn't be used
			return new DistributionNormal(*this);
		}

		// === Pure Virtual Methods ===
	}
}

// Explicit Instantiation
template class cupcfd::distributions::DistributionNormal<int, float>;
template class cupcfd::distributions::DistributionNormal<int, double>;
