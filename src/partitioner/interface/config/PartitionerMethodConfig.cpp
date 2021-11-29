/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Contains Definitions for the PartitionerMethodConfig class.
 */

#include "PartitionerMethodConfig.h"

namespace cupcfd
{
	namespace partitioner
	{
		// === Constructors/Deconstructors ===

		template <class I, class T>
		PartitionerMethodConfig<I,T>::PartitionerMethodConfig()
		{
			// Nothing to do
		}

		template <class I, class T>
		PartitionerMethodConfig<I,T>::~PartitionerMethodConfig()
		{
			// Nothing to do
		}

		// === Pure Virtual Methods ===
		// Here to satisfy linker or implement base class components

		template <class I, class T>
		cupcfd::error::eCodes PartitionerMethodConfig<I,T>::buildPartitioner(PartitionerInterface<I,T> ** part)
		{
			return cupcfd::error::E_SUCCESS;
		}

		template <class I, class T>
		cupcfd::error::eCodes PartitionerMethodConfig<I,T>::buildPartitioner(PartitionerInterface<I,T> ** part,
																				  cupcfd::data_structures::DistributedAdjacencyList<I,T>& partGraph)
		{
			return cupcfd::error::E_SUCCESS;
		}
	}
}

// Explicit Instantiation
template class cupcfd::partitioner::PartitionerMethodConfig<int, int>;
