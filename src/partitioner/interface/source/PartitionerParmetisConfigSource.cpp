/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Class Implementation for the PartitionerParmetisConfigSource class.
 */

#include "PartitionerParmetisConfigSource.h"

namespace cupcfd
{
	namespace partitioner
	{
		// === Constructors/Deconstructors ===

		template <class I, class T>
		PartitionerParmetisConfigSource<I,T>::PartitionerParmetisConfigSource()
		{
			// Nothing to do currently
		}

		template <class I, class T>
		PartitionerParmetisConfigSource<I,T>::~PartitionerParmetisConfigSource()
		{
			// Nothing to do currently
		}

		// === Concrete Methods ===

		// === Pure Virtual Methods ===
		template <class I, class T>
		cupcfd::error::eCodes PartitionerParmetisConfigSource<I,T>::buildPartitionerConfig(PartitionerConfig<I,T> ** config)
		{

		}
	}
}

// Explicit Instantiation
template class cupcfd::partitioner::PartitionerParmetisConfigSource<int, int>;
template class cupcfd::partitioner::PartitionerParmetisConfigSource<int, float>;
template class cupcfd::partitioner::PartitionerParmetisConfigSource<int, double>;
