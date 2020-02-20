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

#include "CupCfdAoSMeshRegion.h"

namespace cupcfd
{
	namespace geometry
	{
		namespace mesh
		{
			template <class I, class T>
			CupCfdAoSMeshRegion<I,T>::CupCfdAoSMeshRegion()
			{
				this->rType = RTYPE_DEFAULT;
				this->std = false;
				this->flux = false;
				this->adiab = false;
				this->ylog = (T) 0;
				this->elog = (T) 0;
				this->density = (T) 0;
				this->turbKE = (T) 0;
				this->turbDiss = (T) 0;
				this->splvl = (T) 0;
				this->den = (T) 0;
				this->r = (T) 0;
				this->t = (T) 0;

				this->forceTangent = (T) 0;
				this->uvw = (T) 0;
				this->regionName = "Default Region";
			}

			template <class I, class T>
			CupCfdAoSMeshRegion<I,T>::~CupCfdAoSMeshRegion()
			{

			}
		}
	}
}

// Explicit Instantiation
using namespace cupcfd::geometry::mesh;

template class CupCfdAoSMeshRegion<int, float>;
template class CupCfdAoSMeshRegion<int, double>;
