/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Contains declarations for the MeshSourceStructGenConfigJSON class.
 */

#ifndef CUPCFD_MESH_MESH_SOURCE_STRUCT_GEN_CONFIG_SOURCE_JSON_INCLUDE_H
#define CUPCFD_MESH_MESH_SOURCE_STRUCT_GEN_CONFIG_SOURCE_JSON_INCLUDE_H

#include "Error.h"
#include "MeshSourceStructGenConfig.h"
#include "MeshSourceConfigSource.h"

// JsonCPP - Supplied as standalone in include/io/jsoncpp
#include "json.h"
#include "json-forwards.h"

// ToDo: Don't really want to include template L here, since we won't know the label type till
// after we read the configuration, but CRTP means we need to know this at compile time.....
// For now our sources all uses integer labels so we will just hard-code those when using this class,
// but should revisit.

namespace cupcfd
{
	namespace geometry
	{
		namespace mesh
		{
			/**
			 *
			 */
			template <class I, class T>
			class MeshSourceStructGenConfigJSON : public MeshSourceConfigSource<I,T,I>
			{
				public:
					// === Members ===

					std::vector<std::string> topLevel;

					/** Json Data Store containing fields for this JSON source **/
					Json::Value configData;

					// === Constructors/Deconstructors ===

					MeshSourceStructGenConfigJSON(std::string configFilePath, std::string * topLevel, int nTopLevel);

					/**
					 * Parse the JSON record provided for fields belonging to Configuration for MeshSourceStructGen JSON.
					 * The fields are presumed to start from level 0 (i.e. they are not nested inside other objects)
					 *
					 * @param parseJSON The contents of a JSON record with the appropriate fields.
					 */
					MeshSourceStructGenConfigJSON(Json::Value& parseJSON);

					/**
					 * Constructor.
					 * Copies data from source to this object.
					 */
					MeshSourceStructGenConfigJSON(MeshSourceStructGenConfigJSON<I,T>& source);

					/**
					 * Deconstructor.
					 * Currently does nothing.
					 */
					virtual ~MeshSourceStructGenConfigJSON();

					// === Concrete Methods ===

					// === Virtual Methods ===

					/**
					 * Deep copy from source to this config
					 *
					 * @param source The source configuration to copy from.
					 *
					 * @tparam I The indexing scheme of the partitioner.
					 * @tparam T The datatype of the partitioner nodes.
					 *
					 * @return Nothing.
					 */
					virtual void operator=(MeshSourceStructGenConfigJSON<I,T>& source);

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
					MeshSourceStructGenConfigJSON<I,T> * clone();


					__attribute__((warn_unused_result))
					cupcfd::error::eCodes getMeshCellSizeX(I * cellX);

					__attribute__((warn_unused_result))
					cupcfd::error::eCodes getMeshCellSizeY(I * cellY);

					__attribute__((warn_unused_result))
					cupcfd::error::eCodes getMeshCellSizeZ(I * cellZ);

					__attribute__((warn_unused_result))
					cupcfd::error::eCodes getMeshSpatialXMin(T * sMinX);

					__attribute__((warn_unused_result))
					cupcfd::error::eCodes getMeshSpatialYMin(T * sMinY);

					__attribute__((warn_unused_result))
					cupcfd::error::eCodes getMeshSpatialZMin(T * sMinZ);

					__attribute__((warn_unused_result))
					cupcfd::error::eCodes getMeshSpatialXMax(T * sMaxX);

					__attribute__((warn_unused_result))
					cupcfd::error::eCodes getMeshSpatialYMax(T * sMaxY);

					__attribute__((warn_unused_result))
					cupcfd::error::eCodes getMeshSpatialZMax(T * sMaxZ);

					/**
					 *
					 */
					__attribute__((warn_unused_result))
					cupcfd::error::eCodes buildMeshSourceConfig(MeshSourceConfig<I,T,I> ** meshSourceConfig);
			};
		}
	}
}

#endif
