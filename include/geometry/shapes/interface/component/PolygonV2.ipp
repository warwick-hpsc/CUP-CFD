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

#ifndef CUPCFD_GEOMETRY_SHAPES_POLYGONV2_IPP_H
#define CUPCFD_GEOMETRY_SHAPES_POLYGONV2_IPP_H

#include "EuclideanPlane3D.h"
#include "EuclideanVector.h"

namespace euc = cupcfd::geometry::euclidean;

namespace cupcfd
{
	namespace geometry
	{
		namespace shapes
		{
			template <class S, class T, uint N, uint V>
			PolygonV2<S,T,N,V>::PolygonV2() {
			}

			template <class S, class T, uint N, uint V>
			template<class...Args>
			PolygonV2<S,T,N,V>::PolygonV2(Args...v)
			: vertices{ (v)... }
			{
				static_assert(sizeof...(Args) == V, "PolygonV2 constructor dimension does not match number of parameters");
			}

			template <class S, class T, uint N, uint V>
			PolygonV2<S,T,N,V>::~PolygonV2() {
			}

			// template <class S, class T, uint N, uint V>
			// PolygonV2<S,T,N,V>::~PolygonV2() {
			// }
			
			// template <class S, class T, uint N, uint V>
			// inline int PolygonV2<S,T,N,V>::getNumVertices() {
			// 	return this->numVertices;
			// }
			
			// template <class S, class T, uint N, uint V>
			// inline int PolygonV2<S,T,N,V>::getNumEdges() {
			// 	return this->numEdges;
			// }
			
			// template <class S, class T, uint N, uint V>
			// void PolygonV2<S,T,N,V>::reverseVertexOrdering() {
			// 	euc::EuclideanPoint<T,N> v2[V];
			// 	for (uint i=0; i<V; i++) {
			// 		uint i2 = (V-1)-i;
			// 		v2[i2] = this->vertices[i];
			// 	}
			// 	for (uint i=0; i<V; i++) {
			// 		this->vertices[i] = v2[i];
			// 	}
			// 	// this->normal *= T(-1);
			// }
			
			// template <class S, class T, uint N, uint V>
			// bool PolygonV2<S,T,N,V>::isPointInside(euc::EuclideanPoint<T,N>& point) {
			// 	return static_cast<S*>(this)->isPointInside(point);
			// }

			// template <class S, class T, uint N, uint V>
			// T PolygonV2<S,T,N,V>::computeCentroid() {
			// 	return static_cast<S*>(this)->computeCentroid();
			// }

			// template <class S, class T, uint N, uint V>
			// euc::EuclideanVector<T,N> PolygonV2<S,T,N,V>::computeNormal() {
			// 	// Pass through to the static method
			// 	return euc::EuclideanPlane3D<T>::calculateNormal(this->vertices[0], this->vertices[1], this->vertices[2]);
			// }
			
			// template <class S, class T, uint N, uint V>
			// T PolygonV2<S,T,N,V>::computeArea() {
			// 	return static_cast<S*>(this)->computeArea();
			// }
		}
	}
}

#endif
