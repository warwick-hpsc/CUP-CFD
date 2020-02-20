/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Contains declarations for the PartitionerMetisConfigSource class.
 */

#ifndef CUPCFD_PARTITIONER_NAIVE_CONFIG_SOURCE_INCLUDE_H
#define CUPCFD_PARTITIONER_NAIVE_CONFIG_SOURCE_INCLUDE_H

#include "PartitionerConfig.h"
#include "Error.h"

namespace cupcfd
{
	namespace partitioner
	{
		/**
		 * Defines a interface for accessing Metis Partitioner Configuration settings
		 * from a source
		 */
		template <class I, class T>
		class PartitionerNaiveConfigSource
		{
			public:
				// === Members ===

				// === Constructors/Deconstructors ===

				/**
				 * Constructor:
				 * Currently does nothing
				 */
				PartitionerNaiveConfigSource();

				/**
				 * Deconstructor.
				 */
				virtual ~PartitionerNaiveConfigSource();

				// === Concrete Methods ===

				virtual cupcfd::error::eCodes buildPartitionerConfig(PartitionerConfig<I,T> ** config) = 0;
		};
	}
}

// Include Header Level Definitions
#include "PartitionerNaiveConfigSource.ipp"

#endif
