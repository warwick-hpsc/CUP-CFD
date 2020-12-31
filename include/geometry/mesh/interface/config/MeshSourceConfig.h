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

#ifndef CUPCFD_CONFIG_MESH_SOURCE_CONFIG_INCLUDE_H
#define CUPCFD_CONFIG_MESH_SOURCE_CONFIG_INCLUDE_H

// Mesh Sources
#include "MeshSource.h"

// Error Codes
#include "Error.h"

namespace cupcfd
{
	namespace geometry
	{
		namespace mesh
		{
			/**
			 * Details the configuration required to build a
			 * Mesh Data Source
			 */
			template <class I, class T, class L>
			class MeshSourceConfig
			{
				public:
					// === Members ===

					// === Constructors/Deconstructors ===

					/**
					 *
					 */
					MeshSourceConfig();

					/**
					 *
					 */
					MeshSourceConfig(MeshSourceConfig<I,T,L>& source);

					/**
					 *
					 */
					virtual ~MeshSourceConfig();

					// === Pure Virtual Methods ===

					/**
					 * Returns a cloned copy of this object
					 *
					 * @return A pointer to a new MeshConfig object
					 * with copied values from this object.
					 */
					virtual MeshSourceConfig<I,T,L> * clone() = 0;

					/**
					 * Build a mesh source based on the details specified in the configuration file.
					 *
					 * @param A pointer to where the newly created MeshSource object pointer will be stored.
					 *
					 * @tparam I The type of the indexing system
					 * @tparam T The type of the mesh structural data
					 *
					 * @return An error status indicating the success or failure of the operation
					 * @return cupcfd::error::E_SUCCESS Success
					 */
					__attribute__((warn_unused_result))
					virtual cupcfd::error::eCodes buildMeshSource(MeshSource<I,T,L> ** source) = 0;
			};
		}
	}
}

// Include Header Level Definitions
#include "MeshSourceConfig.ipp"

#endif
