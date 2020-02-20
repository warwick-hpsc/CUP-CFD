/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Contains Definitions for the PartitionerConfig class.
 */

#include "PartitionerConfig.h"

namespace cupcfd
{
	namespace partitioner
	{
		// === Constructors/Deconstructors ===

		template <class I, class T>
		PartitionerConfig<I,T>::PartitionerConfig()
		{
		}

		template <class I, class T>
		PartitionerConfig<I,T>::PartitionerConfig(PartitionerConfig& source)
		{
			*this = source;
		}

		template <class I, class T>
		PartitionerConfig<I,T>::~PartitionerConfig()
		{

		}

		// === Concrete Methods ===

		template <class I, class T>
		void PartitionerConfig<I,T>::operator=(PartitionerConfig<I,T>& source)
		{

		}

		template <class I, class T>
		PartitionerConfig<I,T> * PartitionerConfig<I,T>::clone()
		{
			// Shouldn't be called as a pure virtual method
			return nullptr;
		}

		template <class I, class T>
		cupcfd::error::eCodes PartitionerConfig<I,T>::buildPartitioner(PartitionerInterface<I,T> ** part)
		{
			return cupcfd::error::E_SUCCESS;
		}

		template <class I, class T>
		cupcfd::error::eCodes PartitionerConfig<I,T>::buildPartitioner(PartitionerInterface<I,T> ** part,
																			cupcfd::data_structures::DistributedAdjacencyList<I,T>& partGraph)
		{
			return cupcfd::error::E_SUCCESS;
		}
	}
}

// Explicit Instantiation
template class cupcfd::partitioner::PartitionerConfig<int, int>;
