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
			 * @tparam P The implementation type of the polygon
			 * @tparam T The type of the spatial domain
			 */
			template <class P, class T>
			class Polygon3D
			{
				public:
					// === Members ===

					int numVertices;
					int numEdges;
					T area;
					cupcfd::geometry::euclidean::EuclideanPoint<T,3> centroid;
					cupcfd::geometry::euclidean::EuclideanVector<T,3> normal;

					// === Constructors/Deconstructors ===

					Polygon3D();

					~Polygon3D();

					// === Concrete Methods ===

					/**
					 * Get the number of vertices in this polygon
					 *
					 * @tparam P The implementation type of the polygon
					 * @tparam T The type of the spatial domain
					 *
					 * @return The number of vertices this polygon has
					 */
					inline int getNumVertices();

					/**
					 * Get the number of edges in this polygon
					 *
					 * @tparam P The implementation type of the polygon
					 * @tparam T The type of the spatial domain
					 *
					 * @return The number of edges this polygon has
					 */
					inline int getNumEdges();

					/**
					 * Determine whether the provided point is inside the polygon.
					 * Edges/Vertices are treated as inside the polygon for this purpose.
					 *
					 * Since this is 3D, for this method points will be treated as if they are projected
					 * onto the same plane as the polygon (i.e. points that lie directly above or below
					 * the polygon will consider 'inside').
					 *
					 *
					 * This is a generic interface should derived classes prefer to implement additional
					 * methods.
					 *
					 * @tparam P The implementation type of the polygon
					 * @tparam T The type of the spatial domain
					 *
					 * @return Return true if the point exists inside this polygon
					 */
					inline bool isPointInside(const cupcfd::geometry::euclidean::EuclideanPoint<T,3>& point);

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
					 * @tparam P The implementation type of the polygon
					 * @tparam T The type of the spatial domain
					 *
					 * @return Return whether the point exists inside this polygon
					 * @retval true The point is inside the polygon
					 * @retval false The point is outside the polygon
					 */
					//inline bool isPointInsideRayCasting(cupcfd::geometry::euclidean::EuclideanPoint<T,N>& point);

				private:
					/**
					 * Compute the area of the polygon
					 *
					 * @tparam P The implementation type of the polygon
					 * @tparam T The type of the spatial domain
					 *
					 * @return Return the computed area of the polygon.
					 */
					inline T computeArea();

					/**
					 * Compute the normal of the polygon. Direction will depend on ordering of vertices.
					 *
					 * @tparam P The implementation type of the polygon
					 * @tparam T The type of the spatial domain
					 *
					 * @return Return the computed normal vector of the polygon.
					 */
					inline cupcfd::geometry::euclidean::EuclideanVector<T,3> computeNormal();
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
			 * @tparam T The type of the spatial domain
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
			 * @tparam T The type of the spatial domain
			 *
			 * @return Whether the ordering is clockwise when viewing the polygon from the observation point
			 * @retval true The ordering is clockwise
			 * @retval false The ordering is anti-clockwise
			 */
			template <class T>
			inline bool isVertexOrderClockwise(const cupcfd::geometry::euclidean::EuclideanPoint<T,3>& observation, cupcfd::geometry::euclidean::EuclideanPoint<T,3> * points, int nPoints);
		}
	}
}

// Include Header Level Definitions
#include "Polygon3D.ipp"

#endif
