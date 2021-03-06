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
#include "Triangle.h"

namespace euc = cupcfd::geometry::euclidean;

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
			class Triangle2D : public Triangle<Triangle2D<T>, T, 2>
			{
				public:
					// === Constructor/Deconstructors ===

					/**
					 * Constructor - Build a triangle using the three provided points
					 * as a, b and c vertices.
					 *
					 * Edges should exist between ab, bc and ac.
					 *
					 * @tparam T The data type of the coordinate system
					 */
					Triangle2D(const euc::EuclideanPoint<T,2>& a,
							   const euc::EuclideanPoint<T,2>& b,
							   const euc::EuclideanPoint<T,2>& c);

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
					__attribute__((warn_unused_result))
					static bool isPointInsideBarycentric(
									const euc::EuclideanPoint<T,2>& a,
									const euc::EuclideanPoint<T,2>& b,
									const euc::EuclideanPoint<T,2>& c,
									const euc::EuclideanPoint<T,2>& p);

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
					__attribute__((warn_unused_result))
					bool isPointInside(const euc::EuclideanPoint<T,2>& p);

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
					//bool isPointInsideCrossProduct(euc::EuclideanPoint<T,N> p);

			};
		}
	}
}

// Include Header Level Definitions
#include "Triangle2D.ipp"

#endif
