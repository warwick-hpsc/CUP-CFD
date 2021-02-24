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

#include "CupCfdAoSMeshCell.h"

namespace cupcfd
{
	namespace geometry
	{
		namespace mesh
		{
			template <class I, class T>
			CupCfdAoSMeshCell<I,T>::CupCfdAoSMeshCell()
			{
				this->vol = (T) 0;
				this->center = (T) 0;	// Sets all components to zero
			}

			template <class I, class T>
			CupCfdAoSMeshCell<I,T>::~CupCfdAoSMeshCell()
			{

			}

			// Explicit Instantiation
			template class CupCfdAoSMeshCell<int, float>;
			template class CupCfdAoSMeshCell<int, double>;
		}
	}
}
