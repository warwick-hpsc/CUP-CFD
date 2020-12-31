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

#ifndef CUPCFD_PARTITIONER_METIS_CONFIG_SOURCE_INCLUDE_H
#define CUPCFD_PARTITIONER_METIS_CONFIG_SOURCE_INCLUDE_H

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
		class PartitionerMetisConfigSource
		{
			public:
				// === Members ===

				// === Constructors/Deconstructors ===

				/**
				 * Constructor:
				 * Currently does nothing
				 */
				PartitionerMetisConfigSource();

				/**
				 * Deconstructor.
				 */
				virtual ~PartitionerMetisConfigSource();

				// === Concrete Methods ===

				__attribute__((warn_unused_result))
				virtual cupcfd::error::eCodes buildPartitionerConfig(PartitionerConfig<I,T> ** config) = 0;
		};
	}
}

// Include Header Level Definitions
#include "PartitionerMetisConfigSource.ipp"

#endif
