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

#include "Polygon3D.h"

#include "Quadrilateral3D.h"
#include "Triangle3D.h"

namespace cupcfd
{
	namespace geometry
	{
		namespace shapes
		{
			template <class T, uint V>
			euc::EuclideanVector3D<T> Polygon3D<T,V>::computeNormal() {
				return euc::EuclideanPlane3D<T>::calculateNormal(this->vertices[0], this->vertices[1], this->vertices[2]);
			}

			template <class T, uint V>
			T Polygon3D<T,V>::computeArea() {
				T area = T(0);
				euc::EuclideanPoint<T,3> v0 = this->vertices[0];
				for (uint i2=2; i2<V; i2++) {
					const uint i1 = i2-1;
					cupcfd::geometry::shapes::Triangle3D<T> t(v0, this->vertices[i1], this->vertices[i2]);
					area += t.getArea();
				}
				return area;
			}
			
			template <class T, uint V>
			euc::EuclideanPoint<T,3> Polygon3D<T,V>::computeCentroid() {
				// Polygon centroid is area-weighted average of triangles centroids:
				euc::EuclideanPoint<T,3> centroid(T(0), T(0), T(0));
				T area = this->getArea();
				for (int i2=2; i2<V; i2++) {
					const uint i1=i2-1;
					Triangle3D<T> tri(this->vertices[0], this->vertices[i1], this->vertices[i2]);
					centroid = centroid + (tri.getCentroid() * tri.getArea());
				}
				centroid = centroid / area;

				return centroid;
			}

			template <class T, uint V>
			bool Polygon3D<T,V>::isPointInside(const euc::EuclideanPoint<T,3>& p) {
				// Does the point lie on the same plane as the Quadrilateral Points?
				euc::EuclideanPlane3D<T> plane(this->vertices[0], this->vertices[1], this->vertices[2]);
				if(!(plane.isPointOnPlane(p))) {
					return false;
				}
				
				// Does the point equal one of the points - if so it counts as inside
				for (uint i=0; i<V; i++) {
					if (p == this->vertices[i]) {
						return true;
					}
				}
				
				// If the point lies on one of the edges it counts as inside
				for (uint i1=0; i1<V; i1++) {
					const uint i2 = (i1+1)%V;
					if (isPointOnLine(this->vertices[i1], this->vertices[i2], p)) {
						return true;
					}
				}
				
				// Test the intersection of the ray ranging from the point to the centroid. 
				// If it intersects any of the edges, then the point p lies on the opposite side
				// of an edge to the centroid, and so must be outside
				euc::EuclideanPoint<T,3> centroid = this->getCentroid();
				for (uint i1=0; i1<V; i1++) {
					const uint i2 = (i1+1)%V;
					if (euc::isVectorRangeIntersection(p, centroid, this->vertices[i1], this->vertices[i2])) {
						return false;
					}
				}
				return true;
			}
			
			template <class T>
			bool isVertexOrderClockwise(const euc::EuclideanPoint<T,3>& observation, euc::EuclideanPoint<T,3> * points, int nPoints) {
				if (nPoints < 3) {
					throw(std::runtime_error("Polygon3D::isVertexOrderClockwise() called with < 3 points"));
				}
			
				// ToDo: Error Check - observation must not be coplanar with points
			
				// Draw a vector from the observation point to a point on the polygon
				euc::EuclideanVector3D<T> obsVec = observation - points[0];
				
				// Compute the normal of the polygon
				euc::EuclideanPlane3D<T> plane(points[0], points[1], points[2]);
				euc::EuclideanVector3D<T> normal = plane.getNormal();
				
				// If the points are traversed in clockwise order from the observation point, the vector will
				// point towards the observation point.
				// Compute the dot-product - if the vectors both point in the same direction (>0) then it is anti-clockwise from the observation point,
				// else it is clockwise from the observation point (since they will be in opposite directions)
				
				T dot = normal.dotProduct(obsVec);
				if(dot < 0) {
					// Anti-Clockwise
					return true;
				}
				return false;
			}

			// Explicit Instantiation
			template class Polygon3D<float, 4>;
			template class Polygon3D<double, 4>;
			template class Polygon3D<float, 3>;
			template class Polygon3D<double, 3>;

			template bool isVertexOrderClockwise(const euc::EuclideanPoint<double,3>&, euc::EuclideanPoint<double,3>*, int);
			template bool isVertexOrderClockwise(const euc::EuclideanPoint<float,3>&, euc::EuclideanPoint<float,3>*, int);

		}
	}
}
