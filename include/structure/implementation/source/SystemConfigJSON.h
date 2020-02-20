/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * This file contains the declarations for the SystemConfigJSON class
 */

#ifndef CUPCFD_SYSTEM_CONFIG_INCLUDE_H
#define CUPCFD_SYSTEM_CONFIG_INCLUDE_H

#include "Error.h"

// JsonCPP - Supplied as standalone in include/io/jsoncpp
#include "json.h"
#include "json-forwards.h"

namespace cupcfd
{
	enum intDataType
	{
		INT_DATATYPE_INT
	};

	enum floatingDataType
	{
		FLOAT_DATATYPE_FLOAT,
		FLOAT_DATATYPE_DOUBLE
	};

	enum meshDataType
	{
		MESH_DATATYPE_MINIAOS,
		MESH_DATATYPE_MINISOA
	};

	/**
	 * JSON for general System Configuration options - primarily datatypes
	 * and specialisation classes that may need to be defined prior to the
	 * creation of objects.
	 */
	class SystemConfigJSON
	{
		public:
			// === Members ===

			/** Json Data Store containing fields for this JSON source **/
			Json::Value configData;

			// === Constructors/Deconstructors ===

			/**
			 * Parse the JSON record provided for fields belonging to general System Configutation JSON.
			 * The fields are presumed to start from level 0 (i.e. they are not nested inside other objects)
			 *
			 * @param parseJSON The contents of a JSON record with the appropriate fields.
			 */
			SystemConfigJSON(Json::Value& parseJSON);

			virtual ~SystemConfigJSON();

			// === Concrete Methods ===

			cupcfd::error::eCodes getIntegerDataType(intDataType * dataType);

			cupcfd::error::eCodes getFloatingPointDataType(floatingDataType * dataType);

			cupcfd::error::eCodes getMeshSpecialisationType(meshDataType * dataType);
	};
}

// Include Header Level Definitions
#include "SystemConfigJSON.ipp"

#endif
