/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Contains the definitions for the BenchmarkExchange class.
 */

#include "BenchmarkExchange.h"

#include "tt_interface_c.h"
#include <cstdlib>

namespace cupcfd
{
	namespace benchmark
	{
		template <class I, class T>
		BenchmarkExchange<I,T>::BenchmarkExchange(std::string benchmarkName, I repetitions, std::shared_ptr<cupcfd::comm::ExchangePattern<T>> patternPtr)
		: Benchmark<I,T>(benchmarkName, repetitions),
		  patternPtr(patternPtr)
		{

		}

		template <class I, class T>
		BenchmarkExchange<I,T>::~BenchmarkExchange()
		{

		}

		template <class I, class T>
		cupcfd::error::eCodes BenchmarkExchange<I,T>::ExchangeBenchmark()
		{
			return cupcfd::error::E_SUCCESS;
		}

		template <class I, class T>
		void BenchmarkExchange<I,T>::setupBenchmark()
		{
			// Nothing to currently do here
		}

		template <class I, class T>
		void BenchmarkExchange<I,T>::recordParameters()
		{

		}

		template <class I, class T>
		void BenchmarkExchange<I,T>::runBenchmark()
		{
			// Expected size of data array is determined at init time (this is array of data both local and to be sent)
			I dataSize = patternPtr->localToExchange.size();

			// ToDo: We would likely want to test vector exchanges too - how to generate a random vector value in a generic way
			// that is also suitable for primitive types?
			T * data = (T *) malloc(sizeof(T) * dataSize);

			for(I i = 0; i < dataSize; i++)
			{
				// The actual contents of the data array do not matter.
				data[i] = T(i)/T(23);
			}

			this->startBenchmarkBlock(this->benchmarkName);
			TreeTimerLogParameterInt("Repetitions", this->repetitions);

			for(I i = 0; i < this->repetitions; i++)
			{
				this->recordParameters();

				this->startBenchmarkBlock("PackBuffer");
				patternPtr->packSendBuffer(data, dataSize);
				this->stopBenchmarkBlock("PackBuffer");

				this->startBenchmarkBlock("Exchange");
				patternPtr->exchangeStart(data, dataSize);
				patternPtr->exchangeStop(data, dataSize);
				this->stopBenchmarkBlock("Exchange");

				this->startBenchmarkBlock("UnpackBuffer");
				patternPtr->unpackRecvBuffer(data, dataSize);
				this->stopBenchmarkBlock("UnpackBuffer");
			}

			this->stopBenchmarkBlock(this->benchmarkName);

			free(data);
		}
	}
}

// Explicit Instantiation
template class cupcfd::benchmark::BenchmarkExchange<int, float>;
template class cupcfd::benchmark::BenchmarkExchange<int, double>;

