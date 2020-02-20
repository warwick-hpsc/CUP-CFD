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
 * Contains definitions for the DistributionUniform Class
 */

#include "DistributionUniform.h"

namespace cupcfd
{
	namespace distributions
	{
		// === Constructor/Deconstructors ===

		template <class I, class T>
		DistributionUniform<I,T>::DistributionUniform(T lbound, T ubound)
		:Distribution<I,T>()
		{
			std::random_device seedSource;

			this->dist = new std::uniform_real_distribution<T>(lbound, ubound);
			this->rEngine = new std::default_random_engine(seedSource());
		}

		template <class I, class T>
		DistributionUniform<I,T>::DistributionUniform(DistributionUniform<I,T>& source)
		{
			this->dist = source.dist;
			this->rEngine = source.rEngine;
		}

		template <class I, class T>
		DistributionUniform<I,T>::~DistributionUniform()
		{
			// Cleanup distribution object
			delete this->dist;
			delete this->rEngine;
		}

		// === Concrete Methods ===

		// === Overloaded Inherited Methods ===

		template <class I, class T>
		cupcfd::error::eCodes DistributionUniform<I,T>::getValues(T * values, I nValues)
		{
			for(I i = 0; i < nValues; i++)
			{
				values[i] = (*(this->dist))(*(this->rEngine));
			}

			return cupcfd::error::E_SUCCESS;
		}

		template <class I, class T>
		DistributionUniform<I,T> * DistributionUniform<I,T>::clone()
		{
			// Pure Virtual - Shouldn't be used
			return new DistributionUniform(*this);
		}

		// === Pure Virtual Methods ===
	}
}

// Explicit Instantiation
template class cupcfd::distributions::DistributionUniform<int, float>;
template class cupcfd::distributions::DistributionUniform<int, double>;
