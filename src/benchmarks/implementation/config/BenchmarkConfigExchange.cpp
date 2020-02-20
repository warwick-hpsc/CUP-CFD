/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Contains definitions for the BenchmarkConfigExchange class
 */

#include "BenchmarkConfigExchange.h"

namespace cupcfd
{
	namespace benchmark
	{
		// === Constructors/Deconstructors ===

		template <class I, class T>
		BenchmarkConfigExchange<I,T>::BenchmarkConfigExchange(std::string benchmarkName, I repetitions, cupcfd::comm::ExchangePatternConfig& patternConfig)
		: benchmarkName(benchmarkName),
		  repetitions(repetitions),
		  patternConfig(patternConfig)
		{

		}

		template <class I, class T>
		BenchmarkConfigExchange<I,T>::BenchmarkConfigExchange(const BenchmarkConfigExchange<I,T>& source)
		{
			*this = source;
		}

		template <class I, class T>
		BenchmarkConfigExchange<I,T>::~BenchmarkConfigExchange()
		{

		}

		// === Overloaded Inherited Methods ===

		template <class I, class T>
		void BenchmarkConfigExchange<I,T>::operator=(const BenchmarkConfigExchange<I,T>& source)
		{
			this->benchmarkName = source.benchmarkName;
			this->repetitions = source.repetitions;
			this->patternConfig = source.patternConfig;
		}

		template <class I, class T>
		BenchmarkConfigExchange<I,T> * BenchmarkConfigExchange<I,T>::clone()
		{
			return new BenchmarkConfigExchange<I,T>(*this);
		}
	}
}

// Explicit Instantiation
template class cupcfd::benchmark::BenchmarkConfigExchange<int,float>;
template class cupcfd::benchmark::BenchmarkConfigExchange<int,double>;
