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
			 * @tparam N Spatial dimension
			 * @tparam V Number of vertices
			 */
			template <class S, class T, uint N, uint V>
			class Polygon
			{
				public:
					// === Members ===

					const int numVertices = V;
					euc::EuclideanPoint<T,N> vertices[V];
					T area;
					euc::EuclideanPoint<T,N> centroid;
					euc::EuclideanVector<T,N> normal;
					
					// bool initialised = false;

					// === Constructors/Deconstructors ===

					Polygon();

					template<class...Args>
					Polygon(Args...v);

					virtual ~Polygon();

					// === Concrete Methods ===

					/**
					 * Copys the data from the source to this object
					 *
					 * @param source The Polygon to copy from
					 */
					inline void operator=(const Polygon<S,T,N,V>& source);

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

					// __attribute__((warn_unused_result))
					// // bool coplanar();
					// virtual bool coplanar() = 0;

					// __attribute__((warn_unused_result))
					// // bool verifyNoEdgesIntersect();
					// virtual bool verifyNoEdgesIntersect() = 0;

				// protected:
					/**
					 * Compute the area of the Polygon
					 *
					 * @return Return the computed area of the Polygon.
					 */
					__attribute__((warn_unused_result))
					T computeArea();

					/**
					 * Compute the centre point of triangle
					 *
					 * @return Centroid point
					 */
					// __attribute__((warn_unused_result))
					// virtual euc::EuclideanPoint<T,N> computeCentroid() = 0;
					// virtual euc::EuclideanPoint<T,N> computeCentroid();
					// euc::EuclideanPoint<T,N> computeCentroid();

					/**
					 * Compute the normal of triangle
					 *
					 * @return Normal vector
					 */
					// __attribute__((warn_unused_result))
					// virtual euc::EuclideanVector<T,N> computeNormal() = 0;
					// virtual euc::EuclideanVector<T,N> computeNormal();

			};

		}
	}
}

// Include Header Level Definitions
#include "Polygon.ipp"

#endif
