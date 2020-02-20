/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Class Declaration for the PartitionerParmetisConfigSourceJSON class.
 */

#ifndef CUPCFD_PARTITIONER_PARMETIS_CONFIG_SOURCE_JSON_INCLUDE_H
#define CUPCFD_PARTITIONER_PARMETIS_CONFIG_SOURCE_JSON_INCLUDE_H

// C++ Std Lib
#include <string>
#include <vector>

// Error Codes
#include "Error.h"

// Abstract interface
#include "PartitionerParmetisConfigSource.h"

// JsonCPP - Supplied as standalone in include/io/jsoncpp
#include "json.h"
#include "json-forwards.h"

namespace cupcfd
{
	namespace partitioner
	{
	/**
	 * Defines a interface for accessing parmetis partitioner configuration
	 * options from a JSON data source.
	 *
	 * === Fields ===
	 *
	 * Required:
	 * None
	 *
	 * Optional:
	 * None
	 *
	 * Currently no fields are defined for configuring partitioners, but
	 * a distinction in classes is made to provide different object builders and
	 * for future expansion if desired.
	 */
		template <class I, class T>
		class PartitionerParmetisConfigSourceJSON : public PartitionerParmetisConfigSource<I,T>
		{
			public:
				// === Members ===

				/** Json Data Store containing fields for this JSON source **/
				Json::Value configData;

				// === Constructors/Deconstructors ===

				/**
				 * Parse the JSON record provided for fields belonging to PartionerMetis JSON.
				 * The fields are presumed to start from level 0 (i.e. they are not nested inside other objects)
				 *
				 * @param parseJSON The contents of a JSON record with the appropriate fields.
				 */
				PartitionerParmetisConfigSourceJSON(Json::Value& parseJSON);

				/**
				 * Deconstructor
				 */
				virtual ~PartitionerParmetisConfigSourceJSON();

				// === Concrete Methods ===

				// === Overloaded Methods ===
				cupcfd::error::eCodes buildPartitionerConfig(PartitionerConfig<I,T> ** config);

				// === Pure Virtual Methods ===
		};
	}
}

// Include Header Level Definitions
#include "PartitionerParmetisConfigSourceJSON.ipp"

#endif
