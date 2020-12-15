/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Contains the class definitions for the PartitionerNaiveConfig class.
 */

// Base Class
#include "PartitionerNaiveConfig.h"

// Partitioner Specialisation Class
#include "PartitionerNaive.h"

// Communicator
#include "Communicator.h"

namespace cupcfd
{
	namespace partitioner
	{
		// === Constructors/Deconstructors ===

		template <class I, class T>
		PartitionerNaiveConfig<I,T>::PartitionerNaiveConfig()
		:PartitionerConfig<I,T>()
		{
			// Nothing to do currently
		}

		template <class I, class T>
		PartitionerNaiveConfig<I,T>::PartitionerNaiveConfig(PartitionerNaiveConfig<I,T>& source)
		{
			// Nothing to do currently
		}

		template <class I, class T>
		PartitionerNaiveConfig<I,T>::~PartitionerNaiveConfig()
		{
			// Nothing to do currently
		}

		template <class I, class T>
		PartitionerNaiveConfig<I,T> * PartitionerNaiveConfig<I,T>::clone()
		{
			return new PartitionerNaiveConfig(*this);
		}

		template <class I, class T>
		cupcfd::error::eCodes PartitionerNaiveConfig<I,T>::buildPartitioner(PartitionerInterface<I,T> ** part)
		{
			// Currently hard-coded to use clone of MPI_COMM_WORLD
			cupcfd::comm::Communicator workComm(MPI_COMM_WORLD);

			*part = new PartitionerNaive<I,T>(workComm);

			return cupcfd::error::E_SUCCESS;
		}

		template <class I, class T>
		cupcfd::error::eCodes PartitionerNaiveConfig<I,T>::buildPartitioner(PartitionerInterface<I,T> ** part,
																				 cupcfd::data_structures::DistributedAdjacencyList<I,T>& partGraph)
		{
			*part = new PartitionerNaive<I,T>(partGraph, partGraph.comm->size);

			return cupcfd::error::E_SUCCESS;
		}
	}
}

// Explicit Instantiation
template class cupcfd::partitioner::PartitionerNaiveConfig<int, int>;

