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
 * Declarations for the Triangle class
 */

#ifndef CUPCFD_GEOMETRY_SHAPES_TRIANGLE_2D_INCLUDE_H
#define CUPCFD_GEOMETRY_SHAPES_TRIANGLE_2D_INCLUDE_H

#include "EuclideanPoint.h"
#include "EuclideanVector.h"
#include "Polygon2D.h"

namespace cupcfd
{
	namespace geometry
	{
		namespace shapes
		{
			/**
			 * Class for storing 2D triangle definition data and operations in a
			 * N-dimensional space.
			 */
			template <class T>
			class Triangle2D : public Polygon2D<Triangle2D<T>,T>
			{
				public:
					// === Members ===

					/** Position of vertices from origin(0,0) **/
					cupcfd::geometry::euclidean::EuclideanPoint<T,2> vertices[3];

					// === Constructor/Deconstructors ===

					/**
					 * Constructor - Build a triangle using the three provided points
					 * as a, b and c vertices.
					 *
					 * Edges should exist between ab, bc and ac.
					 *
					 * @tparam T The data type of the coordinate system
					 */
					Triangle2D(const cupcfd::geometry::euclidean::EuclideanPoint<T,2>& a,
							   const cupcfd::geometry::euclidean::EuclideanPoint<T,2>& b,
							   const cupcfd::geometry::euclidean::EuclideanPoint<T,2>& c);

					/**
					 * Copy Constructor
					 *
					 * @tparam T The data type of the coordinate system
					 */
					Triangle2D(const Triangle2D& source);

					/**
					 * Deconstructor
					 *
					 * @tparam T The data type of the coordinate system
					 */
					~Triangle2D();

					// === Static Methods ===

					/**
					 * Uses the barycentric method to determine whether a point lies inside the area of a
					 * 2D triangle defined by three points.
					 *
					 * www.drdobbs.com/database/triangle-intersection-tests/184404201
					 * https://www.youtube.com/watch?v=HYAgJN3x4GA
					 *
					 * This method can be used for triangles in a higher dimensionality space, but the triangle
					 * points would need to be rotated such that they all lie on a XY plane, and their XY
					 * dimensional components used instead.
					 *
					 * @param a Position of Point a of a triangle
					 * @param b Position of Point b of a triangle
					 * @param c Position of Point c of a triangle
					 * @param p The point to evaluate the position of
					 *
					 * @tparam T The data type of the coordinate system
					 *
					 * @return A boolean indicating whether the point lies inside the triangle vertices
					 * @retval true The point lies inside the triangle (or on one of the edges/vertices)
					 * @retval false The point does not lie inside the triangle
					 */
					static bool isPointInsideBarycentric(cupcfd::geometry::euclidean::EuclideanPoint<T,2>& a,
												    cupcfd::geometry::euclidean::EuclideanPoint<T,2>& b,
												    cupcfd::geometry::euclidean::EuclideanPoint<T,2>& c,
													cupcfd::geometry::euclidean::EuclideanPoint<T,2> p);

					/**
					 * Compute the center of three points
					 *
					 * @param a Point 1 (2D)
					 * @param b Point 2 (2D)
					 * @param c Point 3 (2D)
					 *
					 * @return The point at the center of the three points
					 */
					static inline cupcfd::geometry::euclidean::EuclideanPoint<T,2> computeCentroid(cupcfd::geometry::euclidean::EuclideanPoint<T,2>& a,
																								 cupcfd::geometry::euclidean::EuclideanPoint<T,2>& b,
																								 cupcfd::geometry::euclidean::EuclideanPoint<T,2>& c);

					// === Concrete Methods ===

					/**
					 * Determine whether a point lies within the three points of the triangle.
					 * Triangle edges/vertices are treated as inside the triangle for this purpose.
					 *
					 * If the dimensionality of the triangle is greater than 2D (e.g. 3D), then the point is
					 * projected to the same plane as the triangle for the purposes of this test
					 *
					 * @param p The point to evaluate the position of
					 *
					 * @tparam T The data type of the coordinate system
					 * @tparam N The dimension of the space the triangle is in (e.g. 2 for 2D, 3 for 3D...)
					 *
					 * @return A boolean indicating whether the point lies inside the triangle vertices
					 * @retval true The point lies inside the triangle (or on one of the edges/vertices)
					 * @retval false The point does not lie inside the triangle
					 */
					bool isPointInside(cupcfd::geometry::euclidean::EuclideanPoint<T,2> p);

					/**
					 * Determine whether a point lies within the three points of the triangle.
					 * Triangle edges/vertices are treated as inside the triangle for this purpose.
					 *
					 * Uses the direction of vector cross-products.
					 *
					 * @param p The point to evaluate the position of
					 *
					 * @tparam T The data type of the coordinate system
					 * @tparam N The dimension of the space the triangle is in (e.g. 2 for 2D, 3 for 3D...)
					 *
					 * @return A boolean indicating whether the point lies inside the triangle vertices
					 * @retval true The point lies inside the triangle (or on one of the edges/vertices)
					 * @retval false The point does not lie inside the triangle
					 */
					//bool isPointInsideCrossProduct(cupcfd::geometry::euclidean::EuclideanPoint<T,N> p);

				protected:
					T computeArea();

					/**
					 * Compute the center of three points that make up this triangle
					 *
					 * @return The point at the center of the three points
					 */
					inline cupcfd::geometry::euclidean::EuclideanPoint<T,2> computeCentroid();
			};
		}
	}
}

// Include Header Level Definitions
#include "Triangle2D.ipp"

#endif
