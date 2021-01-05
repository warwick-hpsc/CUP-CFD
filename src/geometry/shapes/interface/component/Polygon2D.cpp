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
 */

#include "Polygon2D.h"
#include "Triangle2D.h"
#include "EuclideanVector2D.h"

namespace cupcfd
{
	namespace geometry
	{
		namespace shapes
		{
			template <class T, uint V>
			T Polygon2D<T,V>::computeArea() {
				T area = T(0);
				euc::EuclideanPoint<T,2> v0 = this->vertices[0];
				for (int i2=2; i2<V; i2++) {
					const uint i1 = i2-1;
					cupcfd::geometry::shapes::Triangle2D<T> t(v0, this->vertices[i1], this->vertices[i2]);
					area += t.getArea();
				}
				return area;
			}

			template <class T, uint V>
			bool Polygon2D<T,V>::isPointInside(const cupcfd::geometry::euclidean::EuclideanPoint<T,2>& p) {
				// Does the point equal one of the points - if so it counts as inside
				for (int i=0; i<V; i++) {
					if (p == this->vertices[i]) {
						return true;
					}
				}
				
				// If the point lies on one of the edges it counts as inside
				for (int i2=1; i2<V; i2++) {
					const int i1 = i2-1;
					if (isPointOnLine(this->vertices[i1], this->vertices[i2], p)) {
						return true;
					}
				}

				// Test the intersection of the ray ranging from the point to the centroid. 
				// If it intersects any of the edges, then the point p lies on the opposite side
				// of an edge to the centroid, and so must be outside
				euc::EuclideanPoint<T,2> centroid = this->computeCentroid();
				for (int i2=1; i2<V; i2++) {
					const int i1 = i2-1;
					if (euc::isVectorRangeIntersection(p, centroid, this->vertices[i1], this->vertices[i2])) {
						return false;
					}
				}
				return true;
			}

			// Explicit Instantiation
			template class Polygon2D<float, 4>;
			template class Polygon2D<double, 4>;
			template class Polygon2D<float, 3>;
			template class Polygon2D<double, 3>;
		}
	}
}
