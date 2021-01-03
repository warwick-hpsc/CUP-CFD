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

#ifndef CUPCFD_GEOMETRY_SHAPES_POLYGON_3D_INCLUDE_H
#define CUPCFD_GEOMETRY_SHAPES_POLYGON_3D_INCLUDE_H

#include "EuclideanPoint.h"
#include "EuclideanVector.h"
#include "EuclideanVector3D.h"
#include "Polygon.h"

namespace cupcfd
{
	namespace geometry
	{
		namespace shapes
		{
			/**
			 * Top level interface for Polygon shapes in a 3D space.
			 * Declares a set of common operations and/or members.
			 *
			 * Uses a CRTP design pattern to minimise/remove virtual overheads
			 *
			 * @tparam P The polygon specialisation
			 * @tparam T Numerical type
			 * @tparam V Number of vertices
			 */
			template <class P, class T, uint V>
			class Polygon3D : public Polygon<Polygon3D<P,T,V>, T, 3, V>
			{
				public:
					// === Constructors/Deconstructors ===

					Polygon3D();

					template<class...Args>
					Polygon3D(Args...v);

					~Polygon3D();

					// === Concrete Methods ===

					/**
					 * Determine whether the provided point is inside the polygon.
					 * Edges/Vertices are treated as inside the polygon for this purpose.
					 *
					 * Since this is 3D, for this method points will be treated as if they are projected
					 * onto the same plane as the polygon (i.e. points that lie directly above or below
					 * the polygon will consider 'inside').
					 *
					 * This is a generic interface should derived classes prefer to implement additional
					 * methods.
					 *
					 * @return Return true if the point exists inside this polygon
					 */
					__attribute__((warn_unused_result))
					bool isPointInside(const cupcfd::geometry::euclidean::EuclideanPoint<T,3>& point);

					// inline bool calculateIntersection(const cupcfd::geometry::euclidean::EuclideanPoint<T,3> v0, 
					// 									const cupcfd::geometry::euclidean::EuclideanVector<T,3> velocity, 
					// 									cupcfd::geometry::euclidean::EuclideanPoint<T,3>& intersect, 
					// 									T& timeToIntersect, 
					// 									bool verbose);

					__attribute__((warn_unused_result))
					T getArea();

					__attribute__((warn_unused_result))
					euc::EuclideanPoint<T,3> getCentroid();

					__attribute__((warn_unused_result))
					euc::EuclideanVector<T,3> getNormal();

				protected:
					// area, centroid, and normal calculations are costly, and not always required, 
					// so do not compute in a constructor. Instead, compute ONCE in a getter
					bool areaComputed = false;
					bool centroidComputed = false;
					bool normalComputed = false;

					/**
					 * Compute the area of the polygon
					 *
					 * @return Return the computed area of the polygon.
					 */
					__attribute__((warn_unused_result))
					T computeArea();

					__attribute__((warn_unused_result))
					euc::EuclideanPoint<T,3> computeCentroid();

					/**
					 * Compute the normal of the polygon. Direction will depend on ordering of vertices.
					 *
					 * @return Return the computed normal vector of the polygon.
					 */
					__attribute__((warn_unused_result))
					cupcfd::geometry::euclidean::EuclideanVector3D<T> computeNormal();

					bool coplanar();

					bool verifyNoEdgesIntersect();

			};

			// === Non-Class, but Generic Methods ===
			// Not included as static to avoid need for setting template P (and generating an entire template)

			/**
			 * Compute the direction of the vertices ordering from the observation point.
			 * This can be used for testing vertex ordering - e.g. useful when determining directions of normals.
			 *
			 * @param observation The point in 3D space from which the polygon is observed for directional purposes.
			 * Must not be coplanar with the plane defined by points.
			 * @param points An array of the vertices/points to compute the area for. These should be in order
			 * of connecting edges - i.e. points[0] connects to point 1, 1 to 2 etc till n-1 connects to 0.
			 * They should also be coplanar.
			 * Must be at least three vertices.
			 * @param nPoints Number of points in points
			 *
			 * @return Whether the ordering is clockwise when viewing the polygon from the observation point
			 * @retval true The ordering is clockwise
			 * @retval false The ordering is anti-clockwise
			 */
			template <class T>
			__attribute__((warn_unused_result))
			inline bool isVertexOrderClockwise(const cupcfd::geometry::euclidean::EuclideanPoint<T,3>& observation, cupcfd::geometry::euclidean::EuclideanPoint<T,3> * points, int nPoints);
		}
	}
}

// Include Header Level Definitions
#include "Polygon3D.ipp"

#endif
