/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Class Implementation for the PartitionerNaiveConfigSource class.
 */

#include "PartitionerNaiveConfigSource.h"

namespace cupcfd
{
	namespace partitioner
	{
		// === Constructors/Deconstructors ===

		template <class I, class T>
		PartitionerNaiveConfigSource<I,T>::PartitionerNaiveConfigSource()
		{
			// Nothing to do currently
		}

		template <class I, class T>
		PartitionerNaiveConfigSource<I,T>::~PartitionerNaiveConfigSource()
		{
			// Nothing to do currently
		}
	}
}

// Explicit Instantiation
template class cupcfd::partitioner::PartitionerNaiveConfigSource<int, int>;
template class cupcfd::partitioner::PartitionerNaiveConfigSource<int, float>;
template class cupcfd::partitioner::PartitionerNaiveConfigSource<int, double>;
