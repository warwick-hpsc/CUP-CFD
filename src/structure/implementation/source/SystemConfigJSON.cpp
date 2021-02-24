/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * This file contains the definitions for the SystemConfigJSON class
 */

#include "SystemConfigJSON.h"

#include <fstream>

namespace cupcfd
{
	SystemConfigJSON::SystemConfigJSON(Json::Value& parseJSON)
	{
		this->configData = parseJSON;
	}

	SystemConfigJSON::~SystemConfigJSON()
	{

	}

	cupcfd::error::eCodes SystemConfigJSON::getIntegerDataType(intDataType * dataType)
	{
		if(this->configData.isMember("IntDataType"))
		{
			const Json::Value dataSourceType = this->configData["IntDataType"];

			if(dataSourceType == Json::Value::null)
			{
				return cupcfd::error::E_CONFIG_OPT_NOT_FOUND;
			}
			else if(dataSourceType.asString() == "int")
			{
				*dataType = INT_DATATYPE_INT;
				return cupcfd::error::E_SUCCESS;
			}

			// Found, but not a matching value

			return cupcfd::error::E_CONFIG_INVALID_VALUE;
		}

		return cupcfd::error::E_CONFIG_OPT_NOT_FOUND;
	}

	cupcfd::error::eCodes SystemConfigJSON::getFloatingPointDataType(floatingDataType * dataType)
	{
		if(this->configData.isMember("FloatDataType"))
		{
			const Json::Value dataSourceType = this->configData["FloatDataType"];

			if(dataSourceType == Json::Value::null)
			{
				return cupcfd::error::E_CONFIG_OPT_NOT_FOUND;
			}
			else if(dataSourceType.asString() == "float")
			{
				*dataType = FLOAT_DATATYPE_FLOAT;
				return cupcfd::error::E_SUCCESS;
			}
			else if(dataSourceType.asString() == "double")
			{
				*dataType = FLOAT_DATATYPE_DOUBLE;
				return cupcfd::error::E_SUCCESS;
			}

			// Found, but not a matching value

			return cupcfd::error::E_CONFIG_INVALID_VALUE;
		}

		return cupcfd::error::E_CONFIG_OPT_NOT_FOUND;
	}

	cupcfd::error::eCodes SystemConfigJSON::getMeshSpecialisationType(meshDataType * dataType)
	{
		if(this->configData.isMember("MeshDataType"))
		{
			const Json::Value dataSourceType = this->configData["MeshDataType"];

			if(dataSourceType == Json::Value::null)
			{
				return cupcfd::error::E_CONFIG_OPT_NOT_FOUND;
			}
			else if(dataSourceType.asString() == "MiniAoS")
			{
				*dataType = MESH_DATATYPE_MINIAOS;
				return cupcfd::error::E_SUCCESS;
			}
			else if(dataSourceType.asString() == "MiniSoA")
			{
				*dataType = MESH_DATATYPE_MINISOA;
				return cupcfd::error::E_SUCCESS;
			}
			// Found, but not a matching value

			return cupcfd::error::E_CONFIG_INVALID_VALUE;
		}

		return cupcfd::error::E_CONFIG_OPT_NOT_FOUND;
	}
}
