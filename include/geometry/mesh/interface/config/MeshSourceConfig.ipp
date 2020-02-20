/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Definition of some functions of the MeshSourceFileConfig class at a header level
 */

#ifndef CUPCFD_CONFIG_MESH_SOURCE_CONFIG_IPP_H
#define CUPCFD_CONFIG_MESH_SOURCE_CONFIG_IPP_H

namespace cupcfd
{
	namespace geometry
	{
		namespace mesh
		{
			// === Concrete Methods ===
			
			// === Pure Virtual Methods ===
			// Here to satisfy the linker
			
			template <class I, class T, class L>
			cupcfd::error::eCodes MeshSourceConfig<I,T,L>::buildMeshSource(MeshSource<I,T,L> ** source)
			{
				// Nothing to do here.
			}

		}
	}
}

#endif
