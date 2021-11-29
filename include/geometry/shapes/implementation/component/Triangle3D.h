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
 * Declarations for the Triangle3D class
 */

#ifndef CUPCFD_GEOMETRY_SHAPES_TRIANGLE_3D_INCLUDE_H
#define CUPCFD_GEOMETRY_SHAPES_TRIANGLE_3D_INCLUDE_H

#include "EuclideanPoint.h"
#include "EuclideanVector.h"
#include "Polygon3D.h"
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
					/**
					 * Because Triangle3D has no inheritance path back to
					 * Polygon3D, which contains useful methods, maintain 
					 * a Polygon3D member variable to get to those methods:
					 * Polygon3D<Triangle3D<T>, T, 3> triAsPolygon3D;
					 */
					Polygon3D<T, 3> triAsPolygon3D;

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

					void operator=(const Triangle3D<T>& t);

					/**
					 * Return normal of triangle, calculating if not known
					 *
					 * @return Triangle normal
					 */
					__attribute__((warn_unused_result))
					euc::EuclideanVector3D<T> getNormal();

					/**
					 * Determine whether the provided point is inside the Triangle.
					 * Edges/Vertices are treated as inside the Triangle for this purpose.
					 *
					 * @return Return true if the point exists inside this Triangle
					 */
					__attribute__((warn_unused_result))
					bool isPointInside(const euc::EuclideanPoint<T,3>& point);

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
			};
		}
	}
}

// Include Header Level Definitions
#include "Triangle3D.ipp"

#endif
