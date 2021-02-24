/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Description
 *
 * Header Level Definitions for the Quadrilateral3D class
 */

#ifndef CUPCFD_GEOMETRY_SHAPES_QUADRILATERAL_3D_IPP_H
#define CUPCFD_GEOMETRY_SHAPES_QUADRILATERAL_3D_IPP_H

#include "Triangle3D.h"
#include "Quadrilateral3D.h"
#include "EuclideanPlane3D.h"

#include <iostream>

namespace euc = cupcfd::geometry::euclidean;

namespace cupcfd
{
	namespace geometry
	{
		namespace shapes
		{
			// === Constructors/Deconstructors ===

			template <class T>
			Quadrilateral3D<T>::Quadrilateral3D()
			{
				euc::EuclideanPoint<T,3> a(T(0), T(0), T(0));
				euc::EuclideanPoint<T,3> b(T(0), T(1), T(0));
				euc::EuclideanPoint<T,3> c(T(1), T(1), T(0));
				euc::EuclideanPoint<T,3> d(T(1), T(0), T(0));

				this->vertices[0] = a;
				this->vertices[1] = b;
				this->vertices[2] = c;
				this->vertices[3] = d;
			}

			template <class T>
			Quadrilateral3D<T>::Quadrilateral3D(const euc::EuclideanPoint<T,3>& a,
											    const euc::EuclideanPoint<T,3>& b,
											    const euc::EuclideanPoint<T,3>& c,
											    const euc::EuclideanPoint<T,3>& d)
			: Polygon3D<T, 4>(a, b, c, d)
			{
			}

			template <class T>
			Quadrilateral3D<T>::~Quadrilateral3D() {
			}

			// === Overloaded Inherited Methods ===
			
			template <class T>
			Quadrilateral3D<T> * Quadrilateral3D<T>::clone() {
				return new Quadrilateral3D(*this);
			}
		}
	}
}

#endif
