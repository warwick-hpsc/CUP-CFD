/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Contains declarations for the MeshSourceConfigSource class.
 */

#ifndef CUPCFD_MESH_SOURCE_MESH_SOURCE_CONFIG_SOURCE_INCLUDE_H
#define CUPCFD_MESH_SOURCE_MESH_SOURCE_CONFIG_SOURCE_INCLUDE_H

#include "Error.h"
#include "MeshSourceConfig.h"

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
			class MeshSourceConfigSource
			{
				public:
					// === Members ===


					// === Constructors/Deconstructors ===

					/**
					 * Constructor.
					 * Currently does nothing.
					 */
					MeshSourceConfigSource();

					/**
					 * Deconstructor.
					 * Currently does nothing.
					 */
					virtual ~MeshSourceConfigSource();

					// === Virtual Methods ===

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
					__attribute__((warn_unused_result))
					virtual MeshSourceConfigSource<I,T,L> * clone() = 0;

					/**
					 *
					 */
					__attribute__((warn_unused_result))
					virtual cupcfd::error::eCodes buildMeshSourceConfig(MeshSourceConfig<I,T,L> ** meshSourceConfig) = 0;
			};
		}
	}
}

#endif
