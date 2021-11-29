/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Contains the class definitions for the PartitionerParmetisConfig class.
 */

// Base Class
#include "PartitionerParmetisConfig.h"

// Partitioner Specialisation Class
#include "PartitionerParmetis.h"

// Communicator
#include "Communicator.h"

namespace cupcfd
{
	namespace partitioner
	{
		// === Constructors/Deconstructors ===

		template <class I, class T>
		PartitionerParmetisConfig<I,T>::PartitionerParmetisConfig()
		:PartitionerConfig<I,T>()
		{
			// Nothing to do currently
		}

		template <class I, class T>
		PartitionerParmetisConfig<I,T>::~PartitionerParmetisConfig()
		{
			// Nothing to do currently
		}

		template <class I, class T>
		PartitionerParmetisConfig<I,T> * PartitionerParmetisConfig<I,T>::clone()
		{
			return new PartitionerParmetisConfig();
		}

		template <class I, class T>
		cupcfd::error::eCodes PartitionerParmetisConfig<I,T>::buildPartitioner(PartitionerInterface<I,T> ** part)
		{
			// Currently hard-coded to use clone of MPI_COMM_WORLD
			cupcfd::comm::Communicator workComm(MPI_COMM_WORLD);

			*part = new PartitionerParmetis<I,T>(workComm);

			return cupcfd::error::E_SUCCESS;
		}

		template <class I, class T>
		cupcfd::error::eCodes PartitionerParmetisConfig<I,T>::buildPartitioner(PartitionerInterface<I,T> ** part,
																					cupcfd::data_structures::DistributedAdjacencyList<I,T>& partGraph)
		{
			*part = new PartitionerParmetis<I,T>(partGraph, partGraph.comm->size, 1);

			return cupcfd::error::E_SUCCESS;
		}
	}
}

// Explicit Instantiation
template class cupcfd::partitioner::PartitionerParmetisConfig<int, int>;

