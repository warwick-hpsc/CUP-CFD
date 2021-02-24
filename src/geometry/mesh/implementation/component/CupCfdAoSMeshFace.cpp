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

#include "CupCfdAoSMeshFace.h"

namespace cupcfd
{
	namespace geometry
	{
		namespace mesh
		{
			template <class I, class T>
			CupCfdAoSMeshFace<I,T>::CupCfdAoSMeshFace()
			{
				this->bndID = (I) -1;
				this->cell1ID = (I) -1;
				this->cell2ID = (I) -1;
				this->verticesID[0] = (I) -1;
				this->verticesID[1] = (I) -1;
				this->verticesID[2] = (I) -1;
				this->verticesID[3] = (I) -1;
				this->lambda = (T) 0;
				this->rlencos = (T) 0;
				this->area = (T) 0;
				this->center = (T) 0;
				this->norm = (T) 0;
				this->xpac = (T) 0;
				this->xnac = (T) 0;
			}

			template <class I, class T>
			CupCfdAoSMeshFace<I,T>::~CupCfdAoSMeshFace()
			{

			}

			// Explicit Instantiation
			template class CupCfdAoSMeshFace<int, float>;
			template class CupCfdAoSMeshFace<int, double>;
		}
	}
}
