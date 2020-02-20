/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * This file contains the header level definitions for the EuclideanPlane class
 */

#ifndef CUPCFD_GEOMETRY_EUCLIDEAN_PLANE_3D_IPP_H
#define CUPCFD_GEOMETRY_EUCLIDEAN_PLANE_3D_IPP_H

namespace cupcfd
{
	namespace geometry
	{
		namespace euclidean
		{
			template <class T>
			inline void EuclideanPlane3D<T>::operator=(const EuclideanPlane3D<T>& plane)
			{
				this->p1 = plane.p1;
				this->p2 = plane.p2;
				this->p3 = plane.p3;
			}
		} // namespace euclidean
	} // namespace geometry
} // namespace cupcfd

#endif
