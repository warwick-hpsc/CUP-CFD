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
#include "EuclideanVector2D.h"
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
			template <class T, uint V>
			class Polygon2D : public Polygon<Polygon2D<T,V>, T, 2, V>
			{
				public:
					// === Constructors/Deconstructors ===

					Polygon2D();

					Polygon2D(euc::EuclideanPoint<T,2> vertices[V]);

					~Polygon2D();

					// === Concrete Methods ===

					/**
					 * Check whether any polygon edges intersect, indicating 
					 * vertices are incorrectly ordered. Directly-connected
					 * edges are not checked.
					 *
					 * @return True if no edges intersect
					 */
					__attribute__((warn_unused_result))
					bool verifyNoEdgesIntersect();

					/**
					 * Determine whether the provided point is inside the Polygon2D.
					 * Edges/Vertices are treated as inside the Polygon2D for this purpose.
					 *
					 * @return Return true if the point exists inside this Polygon2D
					 */
					__attribute__((warn_unused_result))
					bool isPointInside(const euc::EuclideanPoint<T,2>& point);

					/**
					 * Determine whether the provided point is inside the Polygon2D.
					 * Edges/Vertices are treated as inside the Polygon2D for this purposes.
					 *
					 * This method uses a ray casting technique to test whether the point is inside the Polygon2D.
					 *
					 * @return Return true if the point exists inside this Polygon2D
					 */
					//inline bool isPointInsideRayCasting(euc::EuclideanPoint<T,N>& point);

					/**
					 * Return area of polygon, calculating if not known
					 *
					 * @return Polygon area
					 */
					__attribute__((warn_unused_result))
					T getArea();

					/**
					 * Return centroid of polygon, calculating if not known
					 *
					 * @return Polygon centroid
					 */
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

					/**
					 * Compute the centroid of the Polygon2D
					 *
					 * @return Return the computed centroid of the Polygon2D.
					 */
					__attribute__((warn_unused_result))
					euc::EuclideanPoint<T,2> computeCentroid();
			};

		}
	}
}

// Include Header Level Definitions
#include "Polygon2D.ipp"

#endif
