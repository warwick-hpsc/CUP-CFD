/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Contains declarations for the PartitionerParmetisConfigSource class.
 */

#ifndef CUPCFD_PARTITIONER_PARMETIS_CONFIG_SOURCE_INCLUDE_H
#define CUPCFD_PARTITIONER_PARMETIS_CONFIG_SOURCE_INCLUDE_H

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
		class PartitionerParmetisConfigSource
		{
			public:
				// === Members ===

				// === Constructors/Deconstructors ===

				/**
				 * Constructor:
				 * Currently does nothing
				 */
				PartitionerParmetisConfigSource();

				/**
				 * Deconstructor.
				 */
				virtual ~PartitionerParmetisConfigSource();

				// === Concrete Methods ===

				__attribute__((warn_unused_result))
				virtual cupcfd::error::eCodes buildPartitionerConfig(PartitionerConfig<I,T> ** config) = 0;
		};
	}
}

// Include Header Level Definitions
#include "PartitionerParmetisConfigSource.ipp"

#endif
