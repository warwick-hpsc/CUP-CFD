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

#ifndef CUPCFD_GEOMETRY_SHAPES_POLYGON_IPP_H
#define CUPCFD_GEOMETRY_SHAPES_POLYGON_IPP_H

#include "EuclideanVector.h"
#include "Triangle.h"

namespace euc = cupcfd::geometry::euclidean;
namespace shapes = cupcfd::geometry::shapes;

namespace cupcfd
{
	namespace geometry
	{
		namespace shapes
		{
			template <class S, class T, uint N, uint V>
			Polygon<S,T,N,V>::Polygon() {
			}

			template <class S, class T, uint N, uint V>
			template<class...Args>
			Polygon<S,T,N,V>::Polygon(Args...v)
			: vertices{ (v)... }
			{
				static_assert(sizeof...(Args) == V, "Polygon constructor dimension does not match number of parameters");
			}

			template <class S, class T, uint N, uint V>
			Polygon<S,T,N,V>::~Polygon() {
			}

			// template <class S, class T, uint N, uint V>
			// Polygon<S,T,N,V>::~Polygon() {
			// }

			template <class S, class T, uint N, uint V>
			void Polygon<S,T,N,V>::operator=(const Polygon<S,T,N,V>& source) {
				for (uint i=0; i<V; i++) {
					this->vertices[i] = source.vertices[i];
				}
				this->area     = source.area;
				this->centroid = source.centroid;
				this->normal   = source.normal;
				this->areaComputed     = source.areaComputed;
				this->centroidComputed = source.centroidComputed;
				this->normalComputed   = source.normalComputed;
			}
			
			template <class S, class T, uint N, uint V>
			void Polygon<S,T,N,V>::reverseVertexOrdering() {
				euc::EuclideanPoint<T,N> v2[V];
				for (uint i=0; i<V; i++) {
					uint i2 = (V-1)-i;
					v2[i2] = this->vertices[i];
				}
				for (uint i=0; i<V; i++) {
					this->vertices[i] = v2[i];
				}
				this->normal *= T(-1);
			}
			
			template <class S, class T, uint N, uint V>
			auto Polygon<S,T,N,V>::isPointInside(const euc::EuclideanPoint<T,N>& point) {
				return static_cast<S*>(this)->isPointInside(point);
			}

			template <class S, class T, uint N, uint V>
			auto Polygon<S,T,N,V>::getArea() {
				return static_cast<S*>(this)->getArea();
			}

			template <class S, class T, uint N, uint V>
			auto Polygon<S,T,N,V>::getCentroid() {
				return static_cast<S*>(this)->getCentroid();
			}
		}
	}
}

#endif
