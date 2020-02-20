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

#ifndef CUPCFD_GEOMETRY_SHAPES_TRIANGLE_INCLUDE_H
#define CUPCFD_GEOMETRY_SHAPES_TRIANGLE_INCLUDE_H

#include "EuclideanPoint.h"
#include "EuclideanVector.h"

namespace cupcfd
{
	namespace geometry
	{
		namespace shapes
		{
			/**
			 * This class is primarily for generic Triangle functions that are dimension
			 * agnostic (i.e. can be used by both 2D and 3D), stored as static methods.
			 *
			 */
			template <class T, unsigned int N>
			class Triangle
			{
				public:
					/**
					 * Compute the area of the triangle, using Heron's Formula
					 * (https://en.wikipedia.org/wiki/Heron's_formula).
					 *
					 * @param a Position of Point a of the triangle
					 * @param b Position of Point b of the triangle
					 * @param c Position of Point c of the triangle
					 *
					 * @tparam T The data type of the coordinate system
					 * @tparam N The dimension of the space the triangle is in (e.g. 2 for 2D, 3 for 3D...)
					 *
					 * @return The area of the triangle
					 */
					static T heronsFormula(cupcfd::geometry::euclidean::EuclideanPoint<T,N>& a,
										   cupcfd::geometry::euclidean::EuclideanPoint<T,N>& b,
										   cupcfd::geometry::euclidean::EuclideanPoint<T,N>& c);

					/**
					 * Compute the area of the triangle, using Heron's Formula
					 * (https://en.wikipedia.org/wiki/Heron's_formula).
					 *
					 * @param abLength Length of edge from Point a to Point b
					 * @param acLength Length of edge from Point a to Point c
					 * @param bcLength Length of edge from Point b to Point c
					 *
					 * @tparam T The data type of the coordinate system
					 * @tparam N The dimension of the space the triangle is in (e.g. 2 for 2D, 3 for 3D...)
					 *
					 * @return The area of the triangle
					 */
					static T heronsFormula(T abLength, T acLength, T bcLength);
			};
		}
	}
}

// Include Header Level Definitions
#include "Triangle.ipp"

#endif
