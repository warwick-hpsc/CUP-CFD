/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Contains definitions for the MeshSourceConfigSource class.
 */

#include "MeshSourceConfigSource.h"

namespace cupcfd
{
	namespace geometry
	{
		namespace mesh
		{
			template <class I, class T, class L>
			MeshSourceConfigSource<I,T,L>::MeshSourceConfigSource()
			{

			}

			template <class I, class T, class L>
			MeshSourceConfigSource<I,T,L>::~MeshSourceConfigSource()
			{

			}
		}
	}
}

// Explicit Instantiation
template class cupcfd::geometry::mesh::MeshSourceConfigSource<int, float, int>;
template class cupcfd::geometry::mesh::MeshSourceConfigSource<int, double, int>;
