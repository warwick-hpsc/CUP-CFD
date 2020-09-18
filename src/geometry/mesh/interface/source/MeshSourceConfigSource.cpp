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
			MeshSourceConfigSource<I,T,L>::MeshSourceConfigSource(MeshSourceConfigSource<I,T,L>& source)
			{
				*this = source;
			}

			template <class I, class T, class L>
			MeshSourceConfigSource<I,T,L>::~MeshSourceConfigSource()
			{

			}

			template <class I, class T, class L>
			void MeshSourceConfigSource<I,T,L>::operator=(MeshSourceConfigSource<I,T,L>& source)
			{

			}

			// template <class I, class T, class L>
			// MeshSourceConfigSource<I,T,L> * MeshSourceConfigSource<I,T,L>::clone()
			// {

			// }

			// template <class I, class T, class L>
			// cupcfd::error::eCodes buildMeshSourceConfig(MeshSourceConfig<I,T,I> ** meshSourceConfig)
			// {

			// }
		}
	}
}

// Explicit Instantiation
template class cupcfd::geometry::mesh::MeshSourceConfigSource<int, float, int>;
template class cupcfd::geometry::mesh::MeshSourceConfigSource<int, double, int>;
