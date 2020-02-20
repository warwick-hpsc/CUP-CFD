/*
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 */

#include "CupCfdAoSMeshVertex.h"

namespace cupcfd
{
	namespace geometry
	{
		namespace mesh
		{
			template <class T>
			CupCfdAoSMeshVertex<T>::CupCfdAoSMeshVertex()
			{
				// === Default values setup ===

				// Set vertex position to (0.0, 0.0, 0.0)
				this->pos = (T) 0.0;
			}

			template <class T>
			CupCfdAoSMeshVertex<T>::~CupCfdAoSMeshVertex()
			{

			}
		}
	}
}

// Explicit instantiation
using namespace cupcfd::geometry::mesh;

template class CupCfdAoSMeshVertex<float>;
template class CupCfdAoSMeshVertex<double>;
