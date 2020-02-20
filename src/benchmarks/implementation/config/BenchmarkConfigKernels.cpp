/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Contains definitions for the BenchmarkConfigKernels class
 */

#include "BenchmarkConfigKernels.h"

#include "BenchmarkKernels.h"

namespace cupcfd
{
	namespace benchmark
	{
		// === Constructors/Deconstructors ===

		template <class I, class T>
		BenchmarkConfigKernels<I,T>::BenchmarkConfigKernels(const std::string benchmarkName, const I repetitions)
		: benchmarkName(benchmarkName),
		  repetitions(repetitions)
		{

		}

		template <class I, class T>
		BenchmarkConfigKernels<I,T>::BenchmarkConfigKernels(const BenchmarkConfigKernels<I,T>& source)
		{
			*this = source;
		}

		template <class I, class T>
		BenchmarkConfigKernels<I,T>::~BenchmarkConfigKernels()
		{

		}

		// === Concrete Methods ===

		// === Overloaded Inherited Methods ===

		template <class I, class T>
		void BenchmarkConfigKernels<I,T>::operator=(const BenchmarkConfigKernels<I,T>& source)
		{
			this->benchmarkName = source.benchmarkName;
			this->repetitions = source.repetitions;
		}

		template <class I, class T>
		BenchmarkConfigKernels<I,T> * BenchmarkConfigKernels<I,T>::clone()
		{
			return new BenchmarkConfigKernels<I,T>(*this);
		}
	}
}

// Explicit Instantiation
template class cupcfd::benchmark::BenchmarkConfigKernels<int, float>;
template class cupcfd::benchmark::BenchmarkConfigKernels<int, double>;
