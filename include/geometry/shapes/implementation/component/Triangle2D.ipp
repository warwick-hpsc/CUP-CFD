/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Header Level Definitions for the Triangle class
 *
 */

#ifndef CUPCFD_GEOMETRY_SHAPES_TRIANGLE_2D_IPP_H
#define CUPCFD_GEOMETRY_SHAPES_TRIANGLE_2D_IPP_H

#include "ArithmeticKernels.h"
#include "Triangle.h"

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
			Triangle2D<T>::Triangle2D(const euc::EuclideanPoint<T,2>& a,
								      const euc::EuclideanPoint<T,2>& b,
								      const euc::EuclideanPoint<T,2>& c)
			// :Polygon2D<Triangle2D<T>,T>() // Polygon2D constructor does nothing
			: Triangle<Triangle2D<T>,T,2>(a,b,c)
			{
				// this->numVertices = 3;
				// this->numEdges = 3;

				// this->vertices[0] = a;
				// this->vertices[1] = b;
				// this->vertices[2] = c;

				// this->centroid = computeCentroid();
				// this->area = computeArea();
			}
			
			template <class T>
			Triangle2D<T>::Triangle2D(const Triangle2D& source)
			// :Polygon2D<Triangle2D<T>,T>() // Polygon2D constructor does nothing
			: Triangle<Triangle2D<T>,T,2>(source)
			{
				// this->numVertices = 3;
				// this->numEdges = 3;
			
				// this->vertices[0] = source.vertices[0];
				// this->vertices[1] = source.vertices[1];
				// this->vertices[2] = source.vertices[2];

				// this->centroid = source.centroid;
				// this->area = source.area;
			}

			template <class T>
			Triangle2D<T>::~Triangle2D()
			{
				// Nothing to do currently
			}

			// === Static Methods ===

			template <class T>
			bool Triangle2D<T>::isPointInsideBarycentric(
										const cupcfd::geometry::euclidean::EuclideanPoint<T,2>& a,
										const cupcfd::geometry::euclidean::EuclideanPoint<T,2>& b,
										const cupcfd::geometry::euclidean::EuclideanPoint<T,2>& c,
										const cupcfd::geometry::euclidean::EuclideanPoint<T,2>& p) {
				// Barycentric coordinates method
				// www.drdobbs.com/database/triangle-intersection-tests/184404201
				// https://www.youtube.com/watch?v=HYAgJN3x4GA

				// Compute w1 and w2 weightings to multiply vectors by to reach p.

				// Precaution: If c.cmp[1] = a.cmp[1] then we have a divide by zero.
				// Temporary Work-around: If they are equal, rotate/relabel the points. If a.cmp[1] == b.cmp[1] == c.cmp[1] then this is not
				// a triangle, and by rotating we end up with an a.cmp[1] and c.cmp[1] that are not equal.
				// Since w1 and w2 are defined by the transition from a to b and c, it shouldn't matter which point we use
				// since if we change the points, we also change the vectors.

				euc::EuclideanPoint<T,2> p1;
				euc::EuclideanPoint<T,2> p2;
				euc::EuclideanPoint<T,2> p3;

				if(a.cmp[1] != c.cmp[1]) {
					p1 = a;
					p2 = b;
					p3 = c;
				} else {
					p1 = b;
					p2 = c;
					p3 = a;
				}

				T w1top = (p1.cmp[0] * (p3.cmp[1] - p1.cmp[1])) + ((p.cmp[1] - p1.cmp[1]) * (p3.cmp[0] - p1.cmp[0])) - (p.cmp[0] * (p3.cmp[1] - p1.cmp[1]));
				T w1bottom = ((p2.cmp[1] - p1.cmp[1]) * (p3.cmp[0] - p1.cmp[0])) - ((p2.cmp[0] - p1.cmp[0]) * (p3.cmp[1] - p1.cmp[1]));
				T w1 = w1top / w1bottom;

				// If w1 < 0, outside triangle
				if(w1 < 0) {
					return false;
				}

				T w2 = p.cmp[1] - p1.cmp[1] - (w1 * (p2.cmp[1] - p1.cmp[1]));
				w2 = w2 / (p3.cmp[1] - p1.cmp[1]);

				// If w2 < 0, outside triangle
				if(w2 < 0) {
					return false;
				}

				// If w1 + w2 > 1, outside triangle
				if(w1 + w2 > 1) {
					return false;
				}

				// Criteria met
				return true;
			}
			
			// template <class T>
			// inline cupcfd::geometry::euclidean::EuclideanPoint<T,2> Triangle2D<T>::computeCentroid(cupcfd::geometry::euclidean::EuclideanPoint<T,2>& a,
			// 																							cupcfd::geometry::euclidean::EuclideanPoint<T,2>& b,
			// 																							cupcfd::geometry::euclidean::EuclideanPoint<T,2>& c) {
			// 	// ToDo: This could be moved up into a generic polygon method....
			// 	// https://en.wikipedia.org/wiki/Centroid
			// 	// https://en.wikipedia.org/wiki/Median_(triangle)
			
			// 	// Centroid located at arithmetic mean of three points
			// 	return ((a + b + c) / T(3.0));
			// }

			// === Concrete Methods ===

			template <class T>
			bool Triangle2D<T>::isPointInside(const euc::EuclideanPoint<T,2>& p) {
				// Currently defaults to barycentric method
				return this->isPointInsideBarycentric(this->vertices[0], this->vertices[1], this->vertices[2], p);
			}

			// template <class T>
			// T Triangle2D<T>::computeArea() {
			// 	// return Triangle<Triangle2D<T>,T,2>::heronsFormula(this->vertices[0], this->vertices[1], this->vertices[2]);
			// 	Triangle<Triangle2D<T>,T,2>* t = (Triangle<Triangle2D<T>,T,2>*)this;
			// 	return Triangle<Triangle2D<T>,T,2>::heronsFormula(*t);
			// }
			
			// template <class T>
			// inline cupcfd::geometry::euclidean::EuclideanPoint<T,2> Triangle2D<T>::computeCentroid() {
			// 	return Triangle2D<T>::computeCentroid(this->vertices[0], this->vertices[1], this->vertices[2]);
			// }
		}
	}
}

#endif
