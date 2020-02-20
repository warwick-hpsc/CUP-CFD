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
			Polygon2D<P,T>::Polygon2D()
			{
				this->nVertices = 0;
				this->nEdges = 0;
			}
			
			template <class P, class T>
			Polygon2D<P,T>::~Polygon2D()
			{
			
			}
			
			template <class P, class T>
			inline int Polygon2D<P,T>::getNVertices()
			{
				return this->nVertices;
			}
			
			template <class P, class T>
			inline int Polygon2D<P,T>::getNEdges()
			{
				return this->nEdges;
			}
			
			template <class P, class T>
			bool Polygon2D<P,T>::isPointInside(cupcfd::geometry::euclidean::EuclideanPoint<T,2>& point)
			{
				return static_cast<P*>(this)->isPointInside(point);
			}
			
			template <class P, class T>
			T Polygon2D<P,T>::computeArea()
			{
				return static_cast<P*>(this)->computeArea();
			}
		}
	}
}

#endif
