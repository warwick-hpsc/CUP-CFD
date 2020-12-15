/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Contains the class declaration for the PartitionerParmetisConfig class.
 */

#ifndef CUPCFD_PARTITIONER_METIS_CONFIG_INCLUDE_H
#define CUPCFD_PARTITIONER_METIS_CONFIG_INCLUDE_H

#include "PartitionerConfig.h"

namespace cupcfd
{
	namespace partitioner
	{
		/**
		 * Stores configuration details and builder functions for PartitionerParmetis classes
		 */
		template <class I, class T>
		class PartitionerParmetisConfig : public PartitionerConfig<I,T>
		{
			public:
				// === Members ===

				// === Constructors/Deconstructors ===

				/**
				 * Constructor:
				 * Set up default naive configuration options
				 *
				 * @tparam I The indexing scheme of the partitioner.
				 * @tparam T The datatype of the partitioner nodes/graph to be partitioned.
				 */
				PartitionerParmetisConfig();
				/**
				 * Constructor:
				 * Copies the configuration from the source to this newly created object
				 *
				 * @tparam I The indexing scheme of the partitioner.
				 * @tparam T The datatype of the partitioner nodes/graph to be partitioned.
				 */
				PartitionerParmetisConfig(PartitionerParmetisConfig<I,T>& source);

				/**
				 * Deconstructor.
				 *
				 * Destroys any dynamically allocated memory and/or objects.
				 *
				 * @tparam I The indexing scheme of the partitioner.
				 * @tparam T The datatype of the partitioner nodes/graph to be partitioned.
				 */
				~PartitionerParmetisConfig();

				// === Concrete Methods ===

				// === Overridden Inherited Methods ===
				PartitionerParmetisConfig<I,T> * clone();
				cupcfd::error::eCodes buildPartitioner(PartitionerInterface<I,T> ** part);
				cupcfd::error::eCodes buildPartitioner(PartitionerInterface<I,T> ** part,
															cupcfd::data_structures::DistributedAdjacencyList<I,T>& partGraph);
		};
	}
}

// Include Header Level Definitions
#include "PartitionerParmetisConfig.ipp"

#endif
