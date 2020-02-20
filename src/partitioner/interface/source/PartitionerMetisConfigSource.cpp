/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Class Implementation for the PartitionerMetisConfigSource class.
 */

#include "PartitionerMetisConfigSource.h"

namespace cupcfd
{
	namespace partitioner
	{
		// === Constructors/Deconstructors ===

		template <class I, class T>
		PartitionerMetisConfigSource<I,T>::PartitionerMetisConfigSource()
		{
			// Nothing to do currently
		}

		template <class I, class T>
		PartitionerMetisConfigSource<I,T>::~PartitionerMetisConfigSource()
		{
			// Nothing to do currently
		}

		// === Concrete Methods ===

		// === Pure Virtual Methods ===
		template <class I, class T>
		cupcfd::error::eCodes PartitionerMetisConfigSource<I,T>::buildPartitionerConfig(PartitionerConfig<I,T> ** config)
		{

		}
	}
}

// Explicit Instantiation
template class cupcfd::partitioner::PartitionerMetisConfigSource<int, int>;
template class cupcfd::partitioner::PartitionerMetisConfigSource<int, float>;
template class cupcfd::partitioner::PartitionerMetisConfigSource<int, double>;
