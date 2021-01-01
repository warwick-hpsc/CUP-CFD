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
#include "PolygonV2.h"

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
			class Polygon3D : public PolygonV2<Polygon3D<P,T,V>, T, 3, V>
			{
				public:
					// === Members ===

					// === Constructors/Deconstructors ===

					Polygon3D();

					template<class...Args>
					Polygon3D(Args...v);

					~Polygon3D();

					// === Concrete Methods ===

					/**
					 * Get the number of vertices in this polygon
					 *
					 * @return The number of vertices this polygon has
					 */
					// __attribute__((warn_unused_result))
					// inline int getNumVertices();

					/**
					 * Get the number of edges in this polygon
					 *
					 * @return The number of edges this polygon has
					 */
					// __attribute__((warn_unused_result))
					// inline int getNumEdges();

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
					// __attribute__((warn_unused_result))
					// inline bool isPointInside(const cupcfd::geometry::euclidean::EuclideanPoint<T,3>& point);

					// inline bool calculateIntersection(const cupcfd::geometry::euclidean::EuclideanPoint<T,3> v0, 
					// 									const cupcfd::geometry::euclidean::EuclideanVector<T,3> velocity, 
					// 									cupcfd::geometry::euclidean::EuclideanPoint<T,3>& intersect, 
					// 									T& timeToIntersect, 
					// 									bool verbose);

					/**
					 * Determine whether the provided point is inside the polygon.
					 * Edges/Vertices are treated as inside the polygon for this purposes.
					 *
					 * This method uses a ray casting technique to test whether the point is inside the polygon.
					 *
					 * @return Return whether the point exists inside this polygon
					 * @retval true The point is inside the polygon
					 * @retval false The point is outside the polygon
					 */
					//inline bool isPointInsideRayCasting(cupcfd::geometry::euclidean::EuclideanPoint<T,N>& point);

				// private:
					bool coplanar();

					bool verifyNoEdgesIntersect();

					/**
					 * Compute the area of the polygon
					 *
					 * @return Return the computed area of the polygon.
					 */
					__attribute__((warn_unused_result))
					inline T computeArea();

					__attribute__((warn_unused_result))
					// inline euc::EuclideanPoint<T,3> computeCentroid();
					euc::EuclideanPoint<T,3> computeCentroid();

					/**
					 * Compute the normal of the polygon. Direction will depend on ordering of vertices.
					 *
					 * @return Return the computed normal vector of the polygon.
					 */
					__attribute__((warn_unused_result))
					// inline cupcfd::geometry::euclidean::EuclideanVector<T,3> computeNormal();
					cupcfd::geometry::euclidean::EuclideanVector<T,3> computeNormal();
			};

			// === Non-Class, but Generic Methods ===
			// Not included as static to avoid need for setting template P (and generating an entire template)

			/**
			 * Compute the unsigned area of a polygon in 3D
			 *
			 * @param points An array of the vertices/points to compute the area for. These should be in order
			 * of connecting edges - i.e. points[0] connects to point 1, 1 to 2 etc till n-1 connects to 0.
			 * They should also be coplanar.
			 * @param nPoints Number of points in points
			 *
			 * @return The area.
			 */
			// template <class T>
			// inline T computeArea(cupcfd::geometry::euclidean::EuclideanPoint<T,3> * points, int numPoints);


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
