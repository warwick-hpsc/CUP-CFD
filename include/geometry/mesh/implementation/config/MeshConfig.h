/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Class Declaration for the MeshConfig class.
 */

#ifndef CUPCFD_CONFIG_MESH_CONFIG_INCLUDE_H
#define CUPCFD_CONFIG_MESH_CONFIG_INCLUDE_H

// Error Codes
#include "Error.h"

// Partitioner Configuration Objects
#include "PartitionerConfig.h"
#include "MeshSourceConfig.h"
#include "UnstructuredMeshInterface.h"

namespace cupcfd
{
	namespace geometry
	{
		namespace mesh
		{
			/**
			 * Specify the format that the mesh should be built into.
			 */
			enum MeshFormat
			{
				MESH_FORMAT_CUPCFD_AOS
			};

			/**
			 * Stores the top-level mesh source configuration parameters common to all
			 * mesh sources.
			 * This currently includes the type of mesh source (e.g. file or generation)
			 * and the type of partitioner to use.
			 */
			template <class I, class T, class L>
			class MeshConfig
			{
				public:
					// === Members ===

					/** Stores the configuration for partitioning the mesh. **/
					// Note this uses the same type as the indexing for node labels
					cupcfd::partitioner::PartitionerConfig<I,I> * partConfig;

					/** Stores the mesh data source configuration**/
					MeshSourceConfig<I,T,L> * meshSourceConfig;

					// === Constructor/Deconstructor ===

					/**
					 * Constructor
					 * Sets values/configuration to those provided.
					 *
					 * @param partConfig Partitioner Configuration
					 */
					MeshConfig(cupcfd::partitioner::PartitionerConfig<I,I>& partConfig,
							   MeshSourceConfig<I,T,L>& meshSourceConfig);

					/**
					 * Constructor.
					 * Initialise this configuration with a deep copy of the provided configuration.
					 *
					 * @param source The configuration to copy from.
					 */
					MeshConfig(MeshConfig<I,T,L>& source);

					/**
					 * Deconstructor
					 */
					~MeshConfig();

					// === Concrete Methods ===

					/**
					 * Get a cloned copy of the partitioner configuration
					 *
					 * @return The partitioner configuration stored for this mesh configuration.
					 */
					inline cupcfd::partitioner::PartitionerConfig<I,I> * getPartitionerConfig();

					/**
					 * Set the partitioner configuration.
					 *
					 * @param partConfig The partitioner configuration to set (makes a copy)
					 *
					 * @return Nothing
					 */
					inline void setPartitionerConfig(cupcfd::partitioner::PartitionerConfig<I,I>& partConfig);

					/**
					 * Get a cloned copy of the mesh source configuration
					 *
					 * @return The mesh source configuration stored for this mesh configuration.
					 */
					inline MeshSourceConfig<I,T,L> * getMeshSourceConfig();

					/**
					 * Set the mesh source configuration.
					 *
					 * @param partConfig The mesh source configuration to set (makes a copy)
					 *
					 * @return Nothing
					 */
					inline void setMeshSourceConfig(MeshSourceConfig<I,T,L>& meshSourceConfig);

					/**
					 * Deep copy from source to this configuration
					 *
					 * @param source The source configuration to copy from.
					 *
					 * @return Nothing.
					 */
					void operator=(const MeshConfig<I,T,L>& source);

					/**
					 * Returns a cloned copy of this object
					 *
					 * @return A pointer to a new MeshConfig object
					 * with copied values from this object.
					 */
					MeshConfig<I,T,L> * clone();

					/**
					 * Construct a new Mesh object for all ranks in the communicator using the configuration provided.
					 * It will be populated with data from sources specified in the configuration, and partitioned
					 * using the partitioner specified in the config.
					 *
					 * ToDo: This should probably be moved out into a static function with additional parameters
					 * for partitioners etc.
					 *
					 * @param mesh A pointer to the location where the newly created mesh pointer should be stored.
					 * @param comm The communicator containing all ranks that will store part of the mesh. These ranks
					 * will also be used for any partitioning.
					 *
					 * @tparam M The type of the implementing class for the UnstructuredMeshInterface type.
					 * @tparam I The type of the indexing scheme/ cell labels
					 * @tparam T The type of the stored array data
					 *
					 * @return An error status indicating the success or failure of the operation
					 * @retval cupcfd::error::E_ARRAY_SUCCESS Success
					 */
					template <class M>
					__attribute__((warn_unused_result))
					cupcfd::error::eCodes buildUnstructuredMesh(M ** mesh,
																cupcfd::comm::Communicator& comm);
			};
		}
	}
}

// Include select class definitions at a header level
#include "MeshConfig.ipp"

#endif
