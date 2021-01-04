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

#ifndef CUPCFD_GEOMETRY_SHAPES_POLYGON_2D_INCLUDE_H
#define CUPCFD_GEOMETRY_SHAPES_POLYGON_2D_INCLUDE_H

#include "EuclideanPoint.h"
#include "EuclideanVector.h"
#include "Polygon.h"

namespace euc = cupcfd::geometry::euclidean;

namespace cupcfd
{
	namespace geometry
	{
		namespace shapes
		{
			/**
			 * Top level interface for Polygon shapes in a 2D space.
			 * Declares a set of common operations and/or members.
			 *
			 * Uses a CRTP design pattern to minimise/remove virtual overheads
			 *
			 * @tparam P The polygon specialisation
			 * @tparam T Numerical type
			 * @tparam V Number of vertices
			 */
			template <class P, class T, uint V>
			class Polygon2D : public Polygon<Polygon2D<P,T,V>, T, 2, V>
			{
				public:
					// === Constructors/Deconstructors ===

					Polygon2D();

					~Polygon2D();

					// === Concrete Methods ===

					/**
					 * Determine whether the provided point is inside the Polygon2D.
					 * Edges/Vertices are treated as inside the Polygon2D for this purpose.
					 *
					 * @return Return true if the point exists inside this Polygon2D
					 */
					__attribute__((warn_unused_result))
					inline bool isPointInside(cupcfd::geometry::euclidean::EuclideanPoint<T,2>& point);

					/**
					 * Determine whether the provided point is inside the Polygon2D.
					 * Edges/Vertices are treated as inside the Polygon2D for this purposes.
					 *
					 * This method uses a ray casting technique to test whether the point is inside the Polygon2D.
					 *
					 * @return Return true if the point exists inside this Polygon2D
					 */
					//inline bool isPointInsideRayCasting(cupcfd::geometry::euclidean::EuclideanPoint<T,N>& point);

					__attribute__((warn_unused_result))
					T getArea();

					__attribute__((warn_unused_result))
					euc::EuclideanPoint<T,2> getCentroid();

				protected:
					/**
					 * Compute the area of the Polygon2D
					 *
					 * @return Return the computed area of the Polygon2D.
					 */
					__attribute__((warn_unused_result))
					T computeArea();

					__attribute__((warn_unused_result))
					euc::EuclideanPoint<T,2> computeCentroid();
			};

		}
	}
}

// Include Header Level Definitions
#include "Polygon2D.ipp"

#endif
