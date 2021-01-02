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
#include "Triangle.h"

namespace euc = cupcfd::geometry::euclidean;

namespace cupcfd
{
	namespace geometry
	{
		namespace shapes
		{
			/**
			 * Class for storing 3D triangle definition data and operations in a
			 * 3-dimensional space.
			 * 
			 * @tparam T Numerical type
			 */
			template <class T>
			class Triangle3D : public Triangle<Triangle3D<T>, T, 3>
			{
				public:
					// === Constructor/Deconstructors ===

					/**
					 * Constructor - Build a triangle using the three provided points
					 * as vertices.
					 */
					Triangle3D(const euc::EuclideanPoint<T,3>& a,
							   const euc::EuclideanPoint<T,3>& b,
							   const euc::EuclideanPoint<T,3>& c);

					/**
					 * Copy Constructor
					 *
					 * @param source The triangle to copy values from
					 */
					Triangle3D(const Triangle3D<T>& source);

					/**
					 * Deconstructor
					 */
					~Triangle3D();

					// === Concrete Methods ===

					/**
					 * Determine whether a point lies within the three points of the triangle.
					 * Triangle edges/vertices are treated as inside the triangle for this purpose.
					 *
					 * Points that do not lie on the same plane as the triangle will not be treated as
					 * inside, even if directly above or below.
					 *
					 * @return True if the point lies inside the triangle vertices
					 */
					__attribute__((warn_unused_result))
					bool isPointInside(const euc::EuclideanPoint<T,3>& p);

					/**
					 * Calculate intersection of a moving point with this triangle: intersection point, 
					 * time to intersect, and whether it intersects the edge.
					 *
					 * @param v0 The point position
					 * @param velocity The point velocity
					 * @param intersection Calculated intersection point
					 * @param timeToIntersect Time to intersect
					 * @param onEdge Is intersection point on triangle edge
					 * @param verbose Enable debugging print statements.
					 *
					 * @return True if the ray intersects
					 */
					__attribute__((warn_unused_result))
					bool calculateIntersection(const euc::EuclideanPoint<T,3> v0, 
                                                const euc::EuclideanVector<T,3> velocity, 
												euc::EuclideanPoint<T,3>& intersection, 
												T& timeToIntersect, 
												bool* onEdge,
												bool verbose) const;

				// protected:
					// static T computeAreaV2(Triangle3D<T>& tri);

					/**
					 * Compute the area of the Triangle
					 *
					 * @return Return the computed area of the Triangle.
					 */
					// T computeArea();

					/**
					 * Compute the normal of the triangle.
					 *
					 * If the dimensionality is 2, then this is treated as a 3D polygon with a z component of 0.
					 *
					 * @return Return the computed normal vector of the triangle.
					 */
					euc::EuclideanVector3D<T> computeNormal();

					/**
					 * Compute the center of three points that make up this triangle
					 *
					 * @return The point at the center of the three points
					 */
					// inline euc::EuclideanPoint<T,3> computeCentroid();
			};
		}
	}
}

// Include Header Level Definitions
#include "Triangle3D.ipp"

#endif
