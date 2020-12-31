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

#ifndef CUPCFD_GEOMETRY_SHAPES_POLYGON_INCLUDE_H
#define CUPCFD_GEOMETRY_SHAPES_POLYGON_INCLUDE_H

#include "EuclideanPoint.h"
#include "EuclideanVector.h"

namespace euc = cupcfd::geometry::euclidean;

namespace cupcfd
{
	namespace geometry
	{
		namespace shapes
		{
			/**
			 * Top level interface for Polygon shapes.
			 * Declares a set of common operations and/or members.
			 *
			 * Uses a CRTP design pattern to minimise/remove virtual overheads
			 *
			 * @tparam S The polygon specialisation
			 * @tparam T Numerical type
			 * @tparam N Number of spatial dimensions
			 */
			template <class S, class T, uint N>
			class Polygon
			{
				public:
					// === Members ===

					int numVertices;
					int numEdges;
					T area;
					euc::EuclideanPoint<T,N> centroid;
					euc::EuclideanVector<T,N> normal;

					// === Constructors/Deconstructors ===

					Polygon();

					virtual ~Polygon() = 0;

					// === Concrete Methods ===

					/**
					 * Get the number of vertices in this Polygon
					 *
					 * @return The number of vertices this Polygon has
					 */
					// virtual int getNumVertices() = 0;

					/**
					 * Get the number of edges in this Polygon
					 *
					 * @return The number of edges this Polygon has
					 */
					// virtual int getNumEdges() = 0;

					/**
					 * Determine whether the provided point is inside the Polygon.
					 * Edges/Vertices are treated as inside the Polygon for this purpose.
					 *
					 * @return Return whether the point exists inside this Polygon
					 * @retval true The point is inside the Polygon
					 * @retval false The point is outside the Polygon
					 */
					bool isPointInside(euc::EuclideanPoint<T,N>& point);

					/**
					 * Determine whether the provided point is inside the Polygon.
					 * Edges/Vertices are treated as inside the Polygon for this purposes.
					 *
					 * This method uses a ray casting technique to test whether the point is inside the Polygon.
					 *
					 * @return Return whether the point exists inside this Polygon
					 * @retval true The point is inside the Polygon
					 * @retval false The point is outside the Polygon
					 */
					//bool isPointInsideRayCasting(cupcfd::geometry::euclidean::EuclideanPoint<T,N>& point);

					/**
					 * Compute the area of the Polygon
					 *
					 * @return Return the computed area of the Polygon.
					 */
					T computeArea();
			};

		}
	}
}

// Include Header Level Definitions
#include "Polygon.ipp"

#endif
