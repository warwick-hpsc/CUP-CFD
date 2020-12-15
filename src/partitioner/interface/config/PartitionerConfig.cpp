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
	}
}

// Explicit Instantiation
template class cupcfd::partitioner::PartitionerConfig<int, int>;
