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

#ifndef CUPCFD_GEOMETRY_SHAPES_TRIANGLE_3D_INCLUDE_H
#define CUPCFD_GEOMETRY_SHAPES_TRIANGLE_3D_INCLUDE_H

#include "EuclideanPoint.h"
#include "EuclideanVector.h"
#include "Polygon3D.h"

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
			class Triangle3D : public Polygon3D<Triangle3D<T>,T>
			{
				public:
					// === Members ===

					/** Vertices that make up the 3D Triangle **/
					cupcfd::geometry::euclidean::EuclideanPoint<T,3> vertices[3];

					// === Constructor/Deconstructors ===

					/**
					 * Constructor - Build a triangle using the three provided points
					 * as a, b and c vertices.
					 *
					 * Edges should exist between ab, bc and ac.
					 *
					 * @tparam T The data type of the coordinate system
					 */
					Triangle3D(const cupcfd::geometry::euclidean::EuclideanPoint<T,3>& a,
							   const cupcfd::geometry::euclidean::EuclideanPoint<T,3>& b,
							   const cupcfd::geometry::euclidean::EuclideanPoint<T,3>& c);

					/**
					 * Copy Constructor
					 *
					 * @param source The object to copy the values from
					 *
					 * @tparam T The data type of the coordinate system
					 * @tparam N The dimension of the euclidean space the Triangle3D exists in.
					 */
					Triangle3D(const Triangle3D<T>& source);

					/**
					 * Deconstructor
					 *
					 * @tparam T The data type of the coordinate system
					 */
					~Triangle3D();

					// === Static Methods ===

					/**
					 * Compute the center of three points
					 *
					 * @param a Point 1 (3D)
					 * @param b Point 2 (3D)
					 * @param c Point 3 (3D)
					 *
					 * @return The point at the center of the three points
					 */
					static inline cupcfd::geometry::euclidean::EuclideanPoint<T,3> computeCentroid(const cupcfd::geometry::euclidean::EuclideanPoint<T,3>& a,
																								 	    const cupcfd::geometry::euclidean::EuclideanPoint<T,3>& b,
																										const cupcfd::geometry::euclidean::EuclideanPoint<T,3>& c);

					/**
					 * Test whether a point is inside by tracing a vector from the point to the centroid.
					 * If it crosses any of the edges then it is outside the triangle.
					 *
					 * Points on an edge or vertex are considered inside the polygon.
					 *
					 * Points not on the same plane as the polygon are considered outside the polygon.
					 *
					 * @param a Point 1 of a Triangle
					 * @param b Point 2 of a Triangle
					 * @param c Point 3 of a triangle
					 * @param p The point to test the position of
					 *
					 * @return Whether the point is inside the polygon made up by the three points
					 * @retval true The point is inside or on an edge/vertex
					 * @retval false The point is not inside the polygon
					 */
					static inline bool isPointInsideCentroid(const cupcfd::geometry::euclidean::EuclideanPoint<T,3>& a,
														     const cupcfd::geometry::euclidean::EuclideanPoint<T,3>& b,
															 const cupcfd::geometry::euclidean::EuclideanPoint<T,3>& c,
															 const cupcfd::geometry::euclidean::EuclideanPoint<T,3>& p);

					// === Concrete Methods ===

					/**
					 * Determine whether a point lies within the three points of the triangle.
					 * Triangle edges/vertices are treated as inside the triangle for this purpose.
					 *
					 * Points that do not lie on the same plane as the triangle will not be treated as
					 * inside, even if directly above or below.
					 *
					 * @param p The point to evaluate the position of
					 *
					 * @tparam T The data type of the coordinate system
					 *
					 * @return A boolean indicating whether the point lies inside the triangle vertices
					 * @retval true The point lies inside the triangle (or on one of the edges/vertices)
					 * @retval false The point does not lie inside the triangle
					 */
					bool isPointInside(const cupcfd::geometry::euclidean::EuclideanPoint<T,3>& p);

					/**
					 * Compute the area of the Triangle
					 *
					 * @tparam P The implementation type of the Triangle
					 * @tparam T The type of the spatial domain
					 *
					 * @return Return the computed area of the Triangle.
					 */
					T computeArea();

					/**
					 * Compute the normal of the triangle.
					 *
					 * If the dimensionality is 2, then this is treated as a 3D polygon with a z component of 0.
					 *
					 * @tparam P The implementation type of the triangle
					 * @tparam T The type of the spatial domain
					 * @tparam N The dimension of the spatial domain that the shape exists in
					 *
					 * @return Return the computed normal vector of the triangle.
					 */
					inline cupcfd::geometry::euclidean::EuclideanVector<T,3> computeNormal();

					/**
					 * Compute the center of three points that make up this triangle
					 *
					 * @return The point at the center of the three points
					 */
					inline cupcfd::geometry::euclidean::EuclideanPoint<T,3> computeCentroid();
			};
		}
	}
}

// Include Header Level Definitions
#include "Triangle3D.ipp"

#endif
