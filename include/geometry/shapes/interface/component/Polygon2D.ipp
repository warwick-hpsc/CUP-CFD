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
			template <class T, uint V>
			Polygon2D<T,V>::Polygon2D() {
			}

			template <class T, uint V>
			Polygon2D<T,V>::Polygon2D(euc::EuclideanPoint<T,2> vertices[V]) {
				for (int i=0; i<V; i++) {
					this->vertices[i] = vertices[i];
				}
			}
			
			template <class T, uint V>
			Polygon2D<T,V>::~Polygon2D() {
			}

			template <class T, uint V>
			T Polygon2D<T,V>::getArea() {
				if (!this->areaComputed) {
					this->area = this->computeArea();
					this->areaComputed = true;
				}
				return this->area;
			}

			template <class T, uint V>
			euc::EuclideanPoint<T,2> Polygon2D<T,V>::computeCentroid() {
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
			template <class T, uint V>
			euc::EuclideanPoint<T,2> Polygon2D<T,V>::getCentroid() {
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
