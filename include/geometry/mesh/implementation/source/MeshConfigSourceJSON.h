/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Contains declarations for the MeshConfigSourceJSON class.
 */

#ifndef CUPCFD_MESH_MESH_CONFIG_SOURCE_JSON_INCLUDE_H
#define CUPCFD_MESH_MESH_CONFIG_SOURCE_JSON_INCLUDE_H

#include "MeshConfig.h"
#include "PartitionerConfig.h"
#include "MeshSourceConfig.h"

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
			template <class I, class T, class L>
			class MeshConfigSourceJSON
			{
				public:
					// === Members ===

					/** Path to JSON File **/
					std::string configFilePath;

					/**
					 * Define the nested fields to look under for the JSON data in the source file.
					 * The field "FixedDistribution" is always appended to this.
					 *
					 * E.g. If the contents are "FieldA", "FieldB", then
					 * any JSON data lookup for "FieldC" in other methods is done
					 * using [FieldA][FieldB][FixedDistribution] as the root (i.e. the data lookup would
					 * be [FieldA][FieldB][FixedDistribution][FieldC]
					 */
					std::vector<std::string> topLevel;

					/**
					 * Json Storage of Data
					 */
					Json::Value configData;

					// === Constructors/Deconstructors ===

					/**
					 * Constructor.
					 * Currently does nothing.
					 */
					MeshConfigSourceJSON(std::string configFilePath, std::string * topLevel, int nTopLevel);

					/**
					 * Parse the JSON record provided for values belonging to a FixedDistribution entry
					 *
					 * @param parseJSON The contents of a JSON record with the appropriate fields
					 */
					MeshConfigSourceJSON(Json::Value& parseJSON);

					/**
					 * Constructor.
					 * Copies data from source to this object.
					 */
					MeshConfigSourceJSON(MeshConfigSourceJSON<I,T,L>& source);

					/**
					 * Deconstructor.
					 * Currently does nothing.
					 */
					virtual ~MeshConfigSourceJSON();

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
					virtual void operator=(MeshConfigSourceJSON<I,T,L>& source);

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
					MeshConfigSourceJSON<I,T,L> * clone();


					/**
					 * Retrieve and create a dynamically allocated Partitioner Configuration
					 */
					__attribute__((warn_unused_result))
					cupcfd::error::eCodes getPartitionerConfig(cupcfd::partitioner::PartitionerConfig<I,I> ** partConfig);

					/**
					 * Retrieve and create a dynamically allocated MeshSource Configuration
					 */
					__attribute__((warn_unused_result))
					// MeshSourceConfig<I,T,L> * getMeshSourceConfig();
					cupcfd::error::eCodes getMeshSourceConfig(MeshSourceConfig<I,T,L>* config);

					/**
					 *
					 */
					__attribute__((warn_unused_result))
					cupcfd::error::eCodes buildMeshConfig(MeshConfig<I,T,L> ** config);
			};
		}
	}
}

#endif
