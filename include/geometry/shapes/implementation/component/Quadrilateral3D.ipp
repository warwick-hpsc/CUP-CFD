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
				this->vertices[0] = T(0.0);
				this->vertices[1] = T(0.0);
				this->vertices[2] = T(0.0);
				this->vertices[3] = T(0.0);
				
				this->vertices[2].cmp[0] = 1.0;
				this->vertices[3].cmp[0] = 1.0;
				this->vertices[1].cmp[1] = 1.0;
				this->vertices[2].cmp[1] = 1.0;

				/*
				euc::EuclideanPoint<T,3> a(T(0.0), T(0.0), T(0.0));
				euc::EuclideanPoint<T,3> b(T(0.0), T(1.0), T(0.0));
				euc::EuclideanPoint<T,3> c(T(1.0), T(1.0), T(0.0));
				euc::EuclideanPoint<T,3> d(T(1.0), T(0.0), T(0.0));

				this->vertices[0] = a;
				this->vertices[1] = b;
				this->vertices[2] = c;
				this->vertices[3] = d;
				*/

				// this->centroid = euc::EuclideanPoint<T,3>(T(0.4), T(0.5), T(0.5));
				// this->normal = euc::EuclideanPlane3D<T>::calculateNormal(a, b, c);
				// this->area = T(1);
			}

			template <class T>
			Quadrilateral3D<T>::Quadrilateral3D(const euc::EuclideanPoint<T,3>& a,
											    const euc::EuclideanPoint<T,3>& b,
											    const euc::EuclideanPoint<T,3>& c,
											    const euc::EuclideanPoint<T,3>& d)
			: Polygon3D<Quadrilateral3D<T>, T, 4>(a, b, c, d)
			{
			}

			template <class T>
			Quadrilateral3D<T>::Quadrilateral3D(const Quadrilateral3D<T>& source) {
				*this = source;
			}

			template <class T>
			Quadrilateral3D<T>::~Quadrilateral3D() {
			}

			// === Overloaded Inherited Methods ===
			
			template <class T>
			Quadrilateral3D<T> * Quadrilateral3D<T>::clone() {
				return new Quadrilateral3D(*this);
			}

			// === Static Methods ===

			template <class T>
			T Quadrilateral3D<T>::triangularAreaSum(const euc::EuclideanPoint<T,3>& a,
												    const euc::EuclideanPoint<T,3>& b,
													const euc::EuclideanPoint<T,3>& c,
													const euc::EuclideanPoint<T,3>& d) {
				// Decompose the Quadrilateral into Non-Overlapping Triangles
				Triangle3D<T> tri1 = Triangle3D<T>(a, b, c);
				Triangle3D<T> tri2 = Triangle3D<T>(a, d, c);

				// Compute the area of each triangle and sum them
				T area = tri1.computeArea();
				area = area + tri2.computeArea();

				return area;
			}
		}
	}
}

#endif
