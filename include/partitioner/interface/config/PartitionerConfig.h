/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Contains declarations for the PartitionerConfig class.
 */

#ifndef CUPCFD_CONFIG_PARTITIONER_CONFIG_INCLUDE_H
#define CUPCFD_CONFIG_PARTITIONER_CONFIG_INCLUDE_H

#include "DistributedAdjacencyList.h"
#include "PartitionerInterface.h"
#include "PartitionerMethodConfig.h"

namespace cupcfd
{
	namespace partitioner
	{
		/**
		 * Store the common elements required for setting up any generic partitioner approach.
		 *
		 * Currently this acts as a virtual base class for polymorphism purposes.
		 *
		 * @tparam I The indexing scheme of the partitioner.
		 * @tparam T The datatype of the partitioner nodes/graph to be partitioned.
		 */
		template <class I, class T>
		class PartitionerConfig
		{
			public:
				// === Members ===

				// === Constructors/Deconstructors ===

				/**
				 * Constructor.
				 * Currently does nothing.
				 */
				PartitionerConfig();

				/**
				 * Deconstructor.
				 * Currently does nothing.
				 */
				virtual ~PartitionerConfig();

				// === Concrete Methods ===

				// === Virtual Methods ===

				/**
				 * Return a pointer to a clone of this object.
				 * This allows for making copies while also
				 * preserving the polymorphic type.
				 *
				 * @tparam I The indexing scheme of the partitioner.
				 * @tparam T The datatype of the partitioner nodes.
				 *
				 * @return A pointer to a cloned copy of this object.
				 */
				__attribute__((warn_unused_result))
				virtual PartitionerConfig * clone() = 0;

				/**
				 * Create a suitable partitioner object given the current configuration.
				 * The polymorphic type will depend upon the partitioner method configuration
				 *
				 * This does not setup the internal data structures - that is reserved for the initialize function
				 * of partitioner objects at a time when the data source for partitioning is known.
				 *
				 * @param part A pointer to the location where a pointer to the newly created partitioner object
				 * will be stored.
				 *
				 * @tparam I The indexing scheme of the partitioner.
				 * @tparam T The datatype of the partitioner nodes.
				 *
				 * @return An error status indicating the success or failure of the operation.
				 * @retval cupcfd::error::E_SUCCESS The method completed successfully.
				 */
				__attribute__((warn_unused_result))
				virtual cupcfd::error::eCodes buildPartitioner(PartitionerInterface<I,T> ** part) = 0;

				/**
				 * Build a partitioner ready to partition the provided graph
				 *
				 * @param part A pointer to the location where a pointer to the newly created partitioner object
				 * will be stored.
				 * @param partGraph The graph to be partitioned
				 *
				 * @tparam I The indexing scheme of the partitioner.
				 * @tparam T The datatype of the partitioner nodes.
				 *
				 * @return An error status indicating the success or failure of the operation.
				 * @retval cupcfd::error::E_SUCCESS The method completed successfully.
				 */
				__attribute__((warn_unused_result))
				virtual cupcfd::error::eCodes buildPartitioner(PartitionerInterface<I,T> ** part,
																cupcfd::data_structures::DistributedAdjacencyList<I,T>& partGraph) = 0;
		};
	}
}

// Some definitions for the declarations at a header level
#include "PartitionerConfig.ipp"

#endif
