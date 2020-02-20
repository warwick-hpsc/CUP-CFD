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

#include "CupCfdAoSMeshBoundary.h"

namespace cupcfd
{
	namespace geometry
	{
		namespace mesh
		{
			template <class I, class T>
			CupCfdAoSMeshBoundary<I,T>::CupCfdAoSMeshBoundary()
			{
				this->faceID = (I) -1;
				this->verticesID[0] = (I) -1;
				this->verticesID[1] = (I) -1;
				this->verticesID[2] = (I) -1;
				this->verticesID[3] = (I) -1;
				this->regionID = (I) -1;

				this->distance = (T) 0;
				this->yplus = (T) 0;
				this->uplus = (T) 0;
				this->shear = (T) 0; // Sets all components to 0.0
				this->q = (T) 0;
				this->h = (T) 0;
				this->t = (T) 0;
			}

			template <class I, class T>
			CupCfdAoSMeshBoundary<I,T>::~CupCfdAoSMeshBoundary()
			{
				// Nothing allocated on heap, so nothing to do.
			}
		}
	}
}

// Explicit instantiation
using namespace cupcfd::geometry::mesh;

template class CupCfdAoSMeshBoundary<int, float>;
template class CupCfdAoSMeshBoundary<int, double>;
