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

#include "Polygon.h"

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
			 * @tparam S Triangle specialisation
			 * @tparam T Numerical type
			 * @tparam N Number of spatial dimensions
			 */
			template <class S, class T, uint N>
			class Triangle : public Polygon<Triangle<S,T,N>, T, N, 3>
			{
				public:
					// === Constructors/Deconstructors ===

					Triangle();

					Triangle(const euc::EuclideanPoint<T,N>& a,
						  	const euc::EuclideanPoint<T,N>& b,
						    const euc::EuclideanPoint<T,N>& c);

					~Triangle();

					__attribute__((warn_unused_result))
					T getArea();

					__attribute__((warn_unused_result))
					euc::EuclideanPoint<T,N> getCentroid();

					__attribute__((warn_unused_result))
					auto isPointInside(const euc::EuclideanPoint<T,N>& p);

					/**
					 * Compute the area of the triangle, using Heron's Formula
					 *
					 * @param l1 Length of edge 1/3
					 * @param l2 Length of edge 2/3
					 * @param l3 Length of edge 3/3
					 *
					 * @return Area
					 */
					__attribute__((warn_unused_result))
					static T heronsFormula(T l1, T l2, T l3);

					/**
					 * Compute the area of the triangle, using Heron's Formula
					 *
					 * @param a Triangle vertex 1/3
					 * @param b Triangle vertex 2/3
					 * @param c Triangle vertex 3/3
					 *
					 * @return Area
					 */
					__attribute__((warn_unused_result))
					static T heronsFormula(const euc::EuclideanPoint<T,N>& a,
											const euc::EuclideanPoint<T,N>& b,
											const euc::EuclideanPoint<T,N>& c);

					/**
					 * Compute the area of the triangle, using Heron's Formula
					 *
					 * @param tri Triangle
					 *
					 * @return Area
					 */
					__attribute__((warn_unused_result))
					static T heronsFormula(const Triangle<S,T,N>& tri);

				protected:
					T computeArea();

					euc::EuclideanPoint<T,N> computeCentroid();
			};
		}
	}
}

// Include Header Level Definitions
#include "Triangle.ipp"

#endif
