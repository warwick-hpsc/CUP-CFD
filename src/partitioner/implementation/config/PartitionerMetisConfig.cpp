/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Contains the class definitions for the PartitionerMetisConfig class.
 */

// Base Class
#include "PartitionerMetisConfig.h"

// Partitioner Specialisation Class
#include "PartitionerMetis.h"

// Communicator
#include "Communicator.h"

namespace cupcfd
{
	namespace partitioner
	{
		// === Constructors/Deconstructors ===

		template <class I, class T>
		PartitionerMetisConfig<I,T>::PartitionerMetisConfig()
		:PartitionerConfig<I,T>()
		{
			// Nothing to do currently
		}

		template <class I, class T>
		PartitionerMetisConfig<I,T>::PartitionerMetisConfig(PartitionerMetisConfig<I,T>& source)
		{
			// Nothing to do currently
		}

		template <class I, class T>
		PartitionerMetisConfig<I,T>::~PartitionerMetisConfig()
		{
			// Nothing to do currently
		}

		template <class I, class T>
		void PartitionerMetisConfig<I,T>::operator=(PartitionerMetisConfig<I,T>& source)
		{
			// Nothing to do currently
		}

		template <class I, class T>
		PartitionerMetisConfig<I,T> * PartitionerMetisConfig<I,T>::clone()
		{
			return new PartitionerMetisConfig(*this);
		}

		template <class I, class T>
		cupcfd::error::eCodes PartitionerMetisConfig<I,T>::buildPartitioner(PartitionerInterface<I,T> ** part)
		{
			// Currently hard-coded to use clone of MPI_COMM_WORLD
			cupcfd::comm::Communicator workComm(MPI_COMM_WORLD);

			*part = new PartitionerMetis<I,T>(workComm);

			return cupcfd::error::E_SUCCESS;
		}

		template <class I, class T>
		cupcfd::error::eCodes PartitionerMetisConfig<I,T>::buildPartitioner(PartitionerInterface<I,T> ** part,
																				 cupcfd::data_structures::DistributedAdjacencyList<I,T>& partGraph)
		{
			*part = new PartitionerMetis<I,T>(partGraph, partGraph.comm->size, 1);

			return cupcfd::error::E_SUCCESS;
		}
	}
}

// Explicit Instantiation
template class cupcfd::partitioner::PartitionerMetisConfig<int, int>;

