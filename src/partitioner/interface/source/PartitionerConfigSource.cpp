/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Class Implementation for the PartitionerConfigSource class.
 */

#include "PartitionerConfigSource.h"

namespace cupcfd
{
	namespace partitioner
	{
		// === Constructors/Deconstructors ===

		template <class I, class T>
		PartitionerConfigSource<I,T>::PartitionerConfigSource()
		{
			// Nothing to do currently
		}

		template <class I, class T>
		PartitionerConfigSource<I,T>::~PartitionerConfigSource()
		{
			// Nothing to do currently
		}
	}
}

// Explicit Instantiation
template class cupcfd::partitioner::PartitionerConfigSource<int, int>;
template class cupcfd::partitioner::PartitionerConfigSource<int, float>;
template class cupcfd::partitioner::PartitionerConfigSource<int, double>;
