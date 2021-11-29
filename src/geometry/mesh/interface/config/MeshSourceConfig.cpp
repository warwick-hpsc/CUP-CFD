/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Contains definitions for the MeshSourceConfig class
 */

#include "MeshSourceConfig.h"

namespace cupcfd
{
	namespace geometry
	{
		namespace mesh
		{
			// === Constructors/Deconstructors ===

			template <class I, class T, class L>
			MeshSourceConfig<I,T,L>::MeshSourceConfig()
			{

			}

			template <class I, class T, class L>
			MeshSourceConfig<I,T,L>::~MeshSourceConfig()
			{

			}
		}
	}
}

// Explicit Instantiation
template class cupcfd::geometry::mesh::MeshSourceConfig<int, float, int>;
template class cupcfd::geometry::mesh::MeshSourceConfig<int, double, int>;

template class cupcfd::geometry::mesh::MeshSourceConfig<long, float, int>;
template class cupcfd::geometry::mesh::MeshSourceConfig<long, double, int>;
