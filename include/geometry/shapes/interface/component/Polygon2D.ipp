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
			template <class P, class T>
			Polygon2D<P,T>::Polygon2D() {
				this->numVertices = 0;
				this->numEdges = 0;
			}
			
			template <class P, class T>
			Polygon2D<P,T>::~Polygon2D() {
			}
			
			template <class P, class T>
			inline int Polygon2D<P,T>::getNumVertices() {
				return this->numVertices;
			}
			
			template <class P, class T>
			inline int Polygon2D<P,T>::getNumEdges() {
				return this->numEdges;
			}
			
			template <class P, class T>
			bool Polygon2D<P,T>::isPointInside(cupcfd::geometry::euclidean::EuclideanPoint<T,2>& point) {
				return static_cast<P*>(this)->isPointInside(point);
			}
			
			template <class P, class T>
			T Polygon2D<P,T>::computeArea() {
				return static_cast<P*>(this)->computeArea();
			}
		}
	}
}

#endif
