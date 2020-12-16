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
					 * Constructor.
					 * Copies data from source to this object.
					 */
					MeshSourceConfigSource(MeshSourceConfigSource<I,T,L>& source);

					/**
					 * Deconstructor.
					 * Currently does nothing.
					 */
					virtual ~MeshSourceConfigSource();

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
					// virtual void operator=(MeshSourceConfigSource<I,T,L>& source);

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
					virtual MeshSourceConfigSource<I,T,L> * clone() = 0;

					/**
					 *
					 */
					virtual cupcfd::error::eCodes buildMeshSourceConfig(MeshSourceConfig<I,T,L> ** meshSourceConfig) = 0;
			};
		}
	}
}

#endif
