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

#ifndef CUPCFD_GEOMETRY_SHAPES_Polygon3D_3D_IPP_H
#define CUPCFD_GEOMETRY_SHAPES_Polygon3D_3D_IPP_H

#include "EuclideanPlane3D.h"

namespace cupcfd
{
	namespace geometry
	{
		namespace shapes
		{
			template <class P, class T>
			Polygon3D<P,T>::Polygon3D()
			{
				this->nVertices = 0;
				this->nEdges = 0;
			}
			
			template <class P, class T>
			Polygon3D<P,T>::~Polygon3D()
			{
			
			}
			
			template <class P, class T>
			inline int Polygon3D<P,T>::getNVertices()
			{
				return this->nVertices;
			}
			
			template <class P, class T>
			inline int Polygon3D<P,T>::getNEdges()
			{
				return this->nEdges;
			}
			
			template <class P, class T>
			bool Polygon3D<P,T>::isPointInside(const cupcfd::geometry::euclidean::EuclideanPoint<T,3>& point)
			{
				return static_cast<P*>(this)->isPointInside(point);
			}
			
			template <class P, class T>
			T Polygon3D<P,T>::computeArea()
			{
				return static_cast<P*>(this)->computeArea();
			}
			
			template <class P, class T>
			inline cupcfd::geometry::euclidean::EuclideanVector<T,3> Polygon3D<P,T>::computeNormal()
			{
				return static_cast<P*>(this)->computeNormal();
			}
			
			template <class T>
			inline T computeArea(cupcfd::geometry::euclidean::EuclideanPoint<T,3> * points, int nPoints)
			{
				// (Typo is intentional)
				//https://math.stackexchange.com/questions/3207981/caculate-area-of-polygon-in-3d
				
				cupcfd::geometry::euclidean::EuclideanVector<T,3> partialVector(T(0),T(0),T(0));
				
				for(int i = 1; i < (nPoints-1); i++)
				{
					partialVector = partialVector + (0.5 * cupcfd::geometry::euclidean::crossProduct(points[i] - points[0], points[i+1] - points[0]));	
				}
				
				// Compute norm/length - this will also ignore any direction (magnitude only)
				T length;
				partialVector.length(&length);
				return length;
			}
			
			template <class T>
			inline bool isVertexOrderClockwise(const cupcfd::geometry::euclidean::EuclideanPoint<T,3>& observation, cupcfd::geometry::euclidean::EuclideanPoint<T,3> * points, int nPoints)
			{
				if (nPoints < 3) {
					throw(std::runtime_error("Polygon3D::isVertexOrderClockwise() called with < 3 points"));
				}
			
				// ToDo: Error Check - observation must not be coplanar with points
			
				// Draw a vector from the observation point to a point on the polygon
				cupcfd::geometry::euclidean::EuclideanVector<T,3> obsVec = observation - points[0];
				
				// Compute the normal of the polygon
				cupcfd::geometry::euclidean::EuclideanPlane3D<T> plane(points[0], points[1], points[2]);
				cupcfd::geometry::euclidean::EuclideanVector<T,3> normal = plane.getNormal();
				
				// If the points are traversed in clockwise order from the observation point, the vector will
				// point towards the observation point.
				// Compute the dot-product - if the vectors both point in the same direction (>0) then it is anti-clockwise from the observation point,
				// else it is clockwise from the observation point (since they will be in opposite directions)
				
				T dot = normal.dotProduct(obsVec);
				
				if(dot < 0)
				{
					// Anti-Clockwise
					return true;
				}
				
				return false;
			}
		}
	}
}

#endif
