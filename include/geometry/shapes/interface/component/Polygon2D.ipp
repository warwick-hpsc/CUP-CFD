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

#ifndef CUPCFD_GEOMETRY_SHAPES_POLYGON_2D_IPP_H
#define CUPCFD_GEOMETRY_SHAPES_POLYGON_2D_IPP_H

namespace cupcfd
{
	namespace geometry
	{
		namespace shapes
		{
			template <class P, class T, uint V>
			Polygon2D<P,T,V>::Polygon2D() {
			}
			
			template <class P, class T, uint V>
			Polygon2D<P,T,V>::~Polygon2D() {
			}
			
			template <class P, class T, uint V>
			bool Polygon2D<P,T,V>::isPointInside(cupcfd::geometry::euclidean::EuclideanPoint<T,2>& point) {
				return static_cast<P*>(this)->isPointInside(point);
			}
			
			template <class P, class T, uint V>
			T Polygon2D<P,T,V>::computeArea() {
				return static_cast<P*>(this)->computeArea();
			}
			template <class P, class T, uint V>
			T Polygon2D<P,T,V>::getArea() {
				if (!this->areaComputed) {
					this->area = this->computeArea();
					this->areaComputed = true;
				}
				return this->area;
			}

			template <class P, class T, uint V>
			euc::EuclideanPoint<T,2> Polygon2D<P,T,V>::computeCentroid() {
				// https://en.wikipedia.org/wiki/Centroid#Of_a_polygon

				euc::EuclideanPoint<T,2> centroid(T(0), T(0));
				for (uint i=0; i<V; i++) {
					uint i2=(i+1)%V;
					euc::EuclideanPoint<T,2> v1 = this->vertices[i];
					euc::EuclideanPoint<T,2> v2 = this->vertices[i2];
					centroid.cmp[0] += (v1.cmp[0] + v2.cmp[0]) * ( v1.cmp[0]*v2.cmp[1] - v2.cmp[0] * v1.cmp[1] );
					centroid.cmp[1] += (v1.cmp[1] + v2.cmp[1]) * ( v1.cmp[0]*v2.cmp[1] - v2.cmp[0] * v1.cmp[1] );
				}
				centroid *= T(1) / (T(6) * this->getArea());

				return centroid;
			}
			template <class P, class T, uint V>
			euc::EuclideanPoint<T,2> Polygon2D<P,T,V>::getCentroid() {
				if (!this->centroidComputed) {
					this->centroid = this->computeCentroid();
					this->centroidComputed = true;
				}
				return this->centroid;
			}
		}
	}
}

#endif
