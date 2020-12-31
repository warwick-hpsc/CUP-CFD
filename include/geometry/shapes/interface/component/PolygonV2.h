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

#ifndef CUPCFD_GEOMETRY_SHAPES_POLYGONV2_INCLUDE_H
#define CUPCFD_GEOMETRY_SHAPES_POLYGONV2_INCLUDE_H

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
			 * @tparam N Spatial dimension
			 * @tparam V Number of vertices
			 */
			template <class S, class T, uint N, uint V>
			class PolygonV2
			{
				public:
					// === Members ===

					const int numVertices = V;
					euc::EuclideanPoint<T,N> vertices[V];
					// T area;
					// euc::EuclideanPoint<T,N> centroid;
					// euc::EuclideanVector<T,N> normal;
					
					// bool initialised = false;

					// === Constructors/Deconstructors ===

					PolygonV2();

					virtual ~PolygonV2();

					// === Concrete Methods ===

					// cupcfd::error::eCodes initialise();

					/**
					 * Reverse vertex ordering, and flip normal
					 */
					// void reverseVertexOrdering();

					// ToDo: replace pure-virtual methods below with CRTP calls using static_cast

					/**
					 * Determine whether the provided point is inside the Polygon.
					 * Edges/Vertices are treated as inside the Polygon for this purpose.
					 *
					 * @return Return true if the point exists inside this Polygon
					 */
					__attribute__((warn_unused_result))
					virtual bool isPointInside(const euc::EuclideanPoint<T,N>& point) = 0;

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

				protected:
					/**
					 * Compute the area of the Polygon
					 *
					 * @return Return the computed area of the Polygon.
					 */
					__attribute__((warn_unused_result))
					virtual T computeArea() = 0;

					/**
					 * Compute the centre point of triangle
					 *
					 * @return Centroid point
					 */
					__attribute__((warn_unused_result))
					virtual euc::EuclideanPoint<T,N> computeCentroid() = 0;

					/**
					 * Compute the normal of triangle
					 *
					 * @return Normal vector
					 */
					__attribute__((warn_unused_result))
					virtual euc::EuclideanVector<T,N> computeNormal() = 0;

			};

		}
	}
}

// Include Header Level Definitions
#include "PolygonV2.ipp"

#endif
