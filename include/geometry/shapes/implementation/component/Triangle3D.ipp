/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Header Level Definitions for the Triangle3D class
 *
 */

#ifndef CUPCFD_GEOMETRY_SHAPES_TRIANGLE_3D_IPP_H
#define CUPCFD_GEOMETRY_SHAPES_TRIANGLE_3D_IPP_H

#include "ArithmeticKernels.h"
#include "EuclideanPlane3D.h"
#include "EuclideanVector3D.h"
#include "Triangle2D.h"
#include "Triangle.h"

#include <iostream>

namespace arth = cupcfd::utility::arithmetic::kernels;
namespace euc = cupcfd::geometry::euclidean;

namespace cupcfd
{
	namespace geometry
	{
		namespace shapes
		{
			// === Constructors/Deconstructors ===

			template <class T>
			Triangle3D<T>::Triangle3D(const euc::EuclideanPoint<T,3>& a,
								  	  const euc::EuclideanPoint<T,3>& b,
								      const euc::EuclideanPoint<T,3>& c)
			: Triangle<Triangle3D<T>,T,3>(a, b, c), triAsPolygon3D(a, b, c)
			{
			}
			
			template <class T>
			Triangle3D<T>::Triangle3D(const Triangle3D<T>& source)
			: Triangle<Triangle3D<T>,T,3>(source)
			{
				this->triAsPolygon3D = source.triAsPolygon3D;
			}

			template <class T>
			Triangle3D<T>::~Triangle3D()
			{
				// Nothing to do currently
			}

			// === Concrete Methods ===

			template <class T>
			euc::EuclideanVector3D<T> Triangle3D<T>::getNormal() {
				return this->triAsPolygon3D.getNormal();
			}

			template <class T>
			bool Triangle3D<T>::isPointInside(const euc::EuclideanPoint<T,3>& p) {
				// return this->triAsPolygon3D.isPointInside(p);

				// Alternative method: rotate into 2D plane, because 
				// I don't trust floating-point rounding errors to not break 3D method:

				// Does the point lie on the same plane as the Triangle?
				euc::EuclideanPlane3D<T> plane(this->vertices[0], this->vertices[1], this->vertices[2]);
				if(!(plane.isPointOnPlane(p))) {
					return false;
				}

				Triangle3D<T> triCopy(*this);
				euc::EuclideanPoint<T,3> pCopy(p);

				// ToDo: potential optimisation: store rotation matrix and 'tri2d', instead 
				//       of computing every time.
				euc::EuclideanVector3D<T> zAxis(T(0), T(0), T(1));
				euc::Matrix<T,3,3> rotMatrix2;
				rotMatrix2 = euc::calculateRotationMatrix(triCopy.getNormal(), zAxis);
				// Apply rotation:
				for (int i=0; i<3; i++) {
					triCopy.vertices[i] = rotMatrix2 * triCopy.vertices[i];
				}
				pCopy = rotMatrix2 * pCopy;

				// Switch to 2D
				euc::EuclideanPoint<T,2> v0(triCopy.vertices[0].cmp[0], triCopy.vertices[0].cmp[1]);
				euc::EuclideanPoint<T,2> v1(triCopy.vertices[1].cmp[0], triCopy.vertices[1].cmp[1]);
				euc::EuclideanPoint<T,2> v2(triCopy.vertices[2].cmp[0], triCopy.vertices[2].cmp[1]);
				Triangle2D<T> tri2d(v0, v1, v2);
				euc::EuclideanPoint<T,2> p2d(pCopy.cmp[0], pCopy.cmp[1]);
				return tri2d.isPointInside(p2d);
			}

			template <class T>
			bool Triangle3D<T>::calculateIntersection(const euc::EuclideanPoint<T,3> v0, const euc::EuclideanVector<T,3> velocity, 
														euc::EuclideanPoint<T,3>& intersection, 
														T& timeToIntersect, 
														bool* onEdge,
														bool verbose) const
			{
				// http://www.lighthouse3d.com/tutorials/maths/ray-triangle-intersection
				euc::EuclideanVector3D<T> e1 = this->vertices[1] - this->vertices[0];
				euc::EuclideanVector3D<T> e2 = this->vertices[2] - this->vertices[0];
				
				euc::EuclideanVector3D<T> vel(velocity);
				euc::EuclideanVector3D<T> h = vel.crossProduct(e2);
			
				T a = e1.dotProduct(h);

				T f = T(1)/a;

				euc::EuclideanVector3D<T> s = v0 - this->vertices[0];

				T u = f * s.dotProduct(h);

				euc::EuclideanVector3D<T> q = s.crossProduct(e1);

				T v = f * q.dotProduct(velocity);

				T t = f * q.dotProduct(e2);
				if (t == -T(0)) {
					t = T(0);
				}

				intersection = (T(1)-u-v)*this->vertices[0] + u*this->vertices[1] + v*this->vertices[2];

				if (u < T(0.0) || u > T(1.0)) {
					return false;
				}
				if (v < T(0.0) || (u+v) > T(1.0)) {
					return false;
				}

				if (verbose) {
					std::cout << "          > u=" << u << ", v=" << v << ", t=" <<t << std::endl;
				}
				timeToIntersect = t;

				if (u == T(0.0) || u == T(1.0) || v == T(0.0) || v == T(1.0)) {
					*onEdge = true;
				} else {
					*onEdge = false;
				}

				return true;
			}
		}
	}
}

#endif
