/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Contains declarations for the PartitionerConfigSource class.
 */

#ifndef CUPCFD_PARTITIONER_CONFIG_SOURCE_INCLUDE_H
#define CUPCFD_PARTITIONER_CONFIG_SOURCE_INCLUDE_H

#include "PartitionerConfig.h"
#include "Error.h"

namespace cupcfd
{
	namespace partitioner
	{
		/**
		 * Defines a generic interface for accessing partitioner configuration options from a data source.
		 */
		template <class I, class T>
		class PartitionerConfigSource
		{
			public:
				// === Members ===

				// === Constructors/Deconstructors ===

				/**
				 * Constructor:
				 * Currently does nothing
				 */
				PartitionerConfigSource();

				/**
				 * Deconstructor.
				 */
				virtual ~PartitionerConfigSource();

				// === Concrete Methods ===

				/**
				 * Build a Partitioner Configuration.
				 *
				 * Currently, since we don't provide additional options, it is just reduced down to a
				 * single config with an enum rather than multiple polymorphic types. if specialised
				 * optionsa re added per partitioner type, this will be expanded out to polymorphic types.
				 *
				 * @param config A pointer to the location where the new PartitionerConfig object will be stored
				 * @param configSource The source to read the partitioner configuration from.
				 *
				 * @return An error status indicating the success or failure of the operation
				 * @retval cupcfd::error::E_SUCCESS The method completed successfully
				 * @retval cupcfd::error::E_CONFIG_OPT_NOT_FOUND One or more required options were missing
				 * @retval cupcfd::error::E_CONFIG_INVALID_VALUE One or more options had an invalid value
				 */
				virtual cupcfd::error::eCodes buildPartitionerConfig(PartitionerConfig<I,T> ** config) = 0;
		};
	}
}

// Include Header Level Definitions
#include "PartitionerConfigSource.ipp"

#endif
