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

namespace cupcfd
{
	namespace geometry
	{
		namespace shapes
		{
			/**
			 * Top level interface for 2D Polygon2D shapes.
			 * Declares a set of common operations and/or members.
			 *
			 * Uses a CRTP design pattern to minimise/remove virtual overheads
			 *
			 * @tparam P The implementation type of the Polygon2D
			 * @tparam T The type of the spatial domain
			 * @tparam N The dimension of the spatial domain that the shape exists in.
			 * Note: We can define 2D objects in a high dimensions - e.g. a plane in a 3D space.
			 */
			template <class P, class T>
			class Polygon2D
			{
				public:
					// === Members ===

					/** Number of vertices **/
					int nVertices;

					/** Number of edges **/
					int nEdges;

					// === Constructors/Deconstructors ===

					/**
					 *
					 */
					Polygon2D();

					/**
					 *
					 */
					~Polygon2D();

					// === Concrete Methods ===

					/**
					 * Get the number of vertices in this Polygon2D
					 *
					 * @tparam P The implementation type of the Polygon2D
					 * @tparam T The type of the spatial domain
					 * @tparam N The dimension of the spatial domain that the shape exists in
					 *
					 * @return The number of vertices this Polygon2D has
					 */
					inline int getNVertices();

					/**
					 * Get the number of edges in this Polygon2D
					 *
					 * @tparam P The implementation type of the Polygon2D
					 * @tparam T The type of the spatial domain
					 * @tparam N The dimension of the spatial domain that the shape exists in
					 *
					 * @return The number of edges this Polygon2D has
					 */
					inline int getNEdges();

					/**
					 * Determine whether the provided point is inside the Polygon2D.
					 * Edges/Vertices are treated as inside the Polygon2D for this purpose.
					 *
					 * @tparam P The implementation type of the Polygon2D
					 * @tparam T The type of the spatial domain
					 * @tparam N The dimension of the spatial domain that the shape exists in
					 *
					 * @return Return whether the point exists inside this Polygon2D
					 * @retval true The point is inside the Polygon2D
					 * @retval false The point is outside the Polygon2D
					 */
					inline bool isPointInside(cupcfd::geometry::euclidean::EuclideanPoint<T,2>& point);

					/**
					 * Determine whether the provided point is inside the Polygon2D.
					 * Edges/Vertices are treated as inside the Polygon2D for this purposes.
					 *
					 * This method uses a ray casting technique to test whether the point is inside the Polygon2D.
					 *
					 * @tparam P The implementation type of the Polygon2D
					 * @tparam T The type of the spatial domain
					 * @tparam N The dimension of the spatial domain that the shape exists in
					 *
					 * @return Return whether the point exists inside this Polygon2D
					 * @retval true The point is inside the Polygon2D
					 * @retval false The point is outside the Polygon2D
					 */
					//inline bool isPointInsideRayCasting(cupcfd::geometry::euclidean::EuclideanPoint<T,N>& point);

					/**
					 * Compute the area of the Polygon2D
					 *
					 * @tparam P The implementation type of the Polygon2D
					 * @tparam T The type of the spatial domain
					 * @tparam N The dimension of the spatial domain that the shape exists in
					 *
					 * @return Return the computed area of the Polygon2D.
					 */
					inline T computeArea();
			};

		}
	}
}

// Include Header Level Definitions
#include "Polygon2D.ipp"

#endif
