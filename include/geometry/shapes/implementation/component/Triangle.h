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

// #include "Polygon.h"
#include "PolygonV2.h"

namespace euc = cupcfd::geometry::euclidean;

namespace cupcfd
{
	namespace geometry
	{
		namespace shapes
		{
			/**
			 * Top level interface for Triangle shapes.
			 * Declares a set of common operations and/or members.
			 *
			 * Uses a CRTP design pattern to minimise/remove virtual overheads
			 *
			 * @tparam S The triangle specialisation
			 * @tparam T Numerical type
			 * @tparam N Number of spatial dimensions
			 * Note: We can define 2D objects in a high dimensions - e.g. a plane in a 3D space.
			 */
			template <class S, class T, uint N>
			class Triangle : public PolygonV2<Triangle<S,T,N>, T, N, 3>
			{
				public:
					// === Constructors/Deconstructors ===

					Triangle();

					Triangle(const euc::EuclideanPoint<T,N>& a,
						  	const euc::EuclideanPoint<T,N>& b,
						    const euc::EuclideanPoint<T,N>& c);

					Triangle(const Triangle<S,T,N>& source);

					~Triangle();

					bool isPointInside(const euc::EuclideanPoint<T,N>& p);

					/**
					 * Compute the area of the triangle, using Heron's Formula
					 * (https://en.wikipedia.org/wiki/Heron's_formula).
					 *
					 * @param l1 Length of edge 1/3
					 * @param l2 Length of edge 2/3
					 * @param l3 Length of edge 3/3
					 *
					 * @return Area
					 */
					static T heronsFormula(T l1, T l2, T l3);

					/**
					 * Compute the area of the triangle, using Heron's Formula
					 * (https://en.wikipedia.org/wiki/Heron's_formula).
					 *
					 * @param a Triangle vertex 1/3
					 * @param b Triangle vertex 2/3
					 * @param c Triangle vertex 3/3
					 *
					 * @return Area
					 */
					static T heronsFormula(const euc::EuclideanPoint<T,N>& a,
											const euc::EuclideanPoint<T,N>& b,
											const euc::EuclideanPoint<T,N>& c);

					/**
					 * Compute the area of the triangle, using Heron's Formula
					 * (https://en.wikipedia.org/wiki/Heron's_formula).
					 *
					 * @param tri Triangle
					 *
					 * @return Area
					 */
					static T heronsFormula(const Triangle<S,T,N>& tri);


				// protected:
					// No one else should be calling these expensive operations:

					/**
					 * Compute the area of this triangle
					 *
					 * @return Area
					 */
					T computeArea();

					/**
					 * Compute the centre point of triangle
					 *
					 * @return Centroid point
					 */
					euc::EuclideanPoint<T,N> computeCentroid();

					/**
					 * Compute the normal of triangle
					 *
					 * @return Normal vector
					 */
					euc::EuclideanVector<T,N> computeNormal();

			};
		}
	}
}

// Include Header Level Definitions
#include "Triangle.ipp"

#endif
