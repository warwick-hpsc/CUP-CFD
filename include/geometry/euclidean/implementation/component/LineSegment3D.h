/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Contains the declarations for the LineSegment3D class.
 */

#ifndef CUPCFD_GEOMETRY_LINE_SEGMENT3D_INCLUDE_H
#define CUPCFD_GEOMETRY_LINE_SEGMENT3D_INCLUDE_H

#include "EuclideanPoint.h"
#include "EuclideanVector3D.h"

namespace cupcfd
{
	namespace geometry
	{
		namespace euclidean
		{
			/**
			 * Stores data and operations for line segments in a euclidean space.
			 *
			 * @tparam T Numerical type
			 */
			template <class T>
			class LineSegment3D
			{
				public:
					EuclideanVector3D<T> v;
					EuclideanPoint<T,3> source;
					T length;

					/**
					 * Default constructor. Sets all values to zero.
					 */
					LineSegment3D();

					/**
					 * Construct from two points
					 *
					 * @param source Source point
					 * @param finish Finish point
					 */
					LineSegment3D(const EuclideanPoint<T,3>& source, const EuclideanPoint<T,3>& finish);

					/**
					 * Construct from source point and length vector
					 *
					 * @param vector Vector to endpoint
					 * @param source Source point
					 */
					LineSegment3D(const EuclideanVector<T,3>& vector, const EuclideanPoint<T,3>& source);

					/**
					 * Default Deconstructor
					 */
					~LineSegment3D();

					// === Concrete Methods ===

					/**
					 * Pretty-print to STDOUT
					 */
					void print() const;

					/**
					 * Calculate intersection point between this and another line
					 *
					 * @param line2 The other line
					 * @param intersectPoint Calculated intersection point
					 *
					 * @return Error code
					 * @retval cupcfd::error::E_SUCCESS Intersection found
					 * @retval cupcfd::error::E_GEOMETRY_NO_INTERSECT No intersection
					 */
					cupcfd::error::eCodes lineIntersection(const LineSegment3D& line2, EuclideanPoint<T,3>& intersectPoint);

					/**
					 * Check if a point is on this line
					 *
					 * @param p The point
					 *
					 * @return True if point on line
					 */
					bool isPointOnLine(const EuclideanPoint<T,3>& p) const;

					/**
					 * Is another line colinear with this?
					 *
					 * @param l The other line
					 *
					 * @return True if colinear
					 */
					bool isLineColinear(const LineSegment3D<T>& l) const;
			};
		}
	}
}

#include "LineSegment3D.ipp"

#endif
