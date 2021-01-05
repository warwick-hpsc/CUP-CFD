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

#ifndef CUPCFD_GEOMETRY_SHAPES_POLYGON_3D_INCLUDE_H
#define CUPCFD_GEOMETRY_SHAPES_POLYGON_3D_INCLUDE_H

#include "EuclideanPoint.h"
#include "EuclideanVector.h"
#include "EuclideanVector3D.h"
#include "Polygon.h"

namespace euc = cupcfd::geometry::euclidean;

namespace cupcfd
{
	namespace geometry
	{
		namespace shapes
		{
			/**
			 * Top level interface for Polygon shapes in a 3D space.
			 * Declares a set of common operations and/or members.
			 *
			 * Uses a CRTP design pattern to minimise/remove virtual overheads
			 *
			 * @tparam T Numerical type
			 * @tparam V Number of vertices
			 */
			template <class T, uint V>
			class Polygon3D : public Polygon<Polygon3D<T,V>, T, 3, V>
			{
				public:
					// === Constructors/Deconstructors ===

					Polygon3D();

					template<class...Args>
					Polygon3D(Args...v);

					~Polygon3D();

					// === Concrete Methods ===

					/**
					 * Determine whether the provided point is inside the polygon.
					 * Edges/Vertices are treated as inside the polygon for this purpose.
					 *
					 * Since this is 3D, for this method points will be treated as if they are projected
					 * onto the same plane as the polygon (i.e. points that lie directly above or below
					 * the polygon will consider 'inside').
					 *
					 * @return Return true if the point exists inside this polygon
					 */
					__attribute__((warn_unused_result))
					bool isPointInside(const euc::EuclideanPoint<T,3>& point);

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
					euc::EuclideanPoint<T,3> getCentroid();

					/**
					 * Return normal of polygon, calculating if not known
					 *
					 * @return Polygon normal
					 */
					__attribute__((warn_unused_result))
					euc::EuclideanVector3D<T> getNormal();

				protected:
					/**
					 * Calculate area of polygon
					 *
					 * @return Polygon area
					 */
					__attribute__((warn_unused_result))
					T computeArea();

					/**
					 * Calculate centroid of polygon
					 *
					 * @return Polygon centroid
					 */
					__attribute__((warn_unused_result))
					euc::EuclideanPoint<T,3> computeCentroid();

					/**
					 * Compute the normal of the polygon. Direction will depend on ordering of vertices.
					 *
					 * @return Polygon normal
					 */
					__attribute__((warn_unused_result))
					euc::EuclideanVector3D<T> computeNormal();

					/**
					 * Rotate this polygon to align normal with 'v'
					 *
					 * @param v Target vector for alignment
					 */
					void alignNormalWithVector(euc::EuclideanVector3D<T>& v);

					/**
					 * Check whether all vertices are coplanar
					 *
					 * @return True if coplanar
					 */
					__attribute__((warn_unused_result))
					bool coplanar();

					/**
					 * Check whether any polygon edges intersect, indicating 
					 * vertices are incorrectly ordered. Directly-connected
					 * edges are not checked.
					 *
					 * @return True if no edges intersect
					 */
					__attribute__((warn_unused_result))
					bool verifyNoEdgesIntersect();

			};

			// === Non-Class, but Generic Methods ===
			// Not included as static to avoid need for setting template P (and generating an entire template)

			/**
			 * Compute the direction of the vertices ordering from the observation point.
			 * This can be used for testing vertex ordering - e.g. useful when determining directions of normals.
			 *
			 * @param observation The point in 3D space from which the polygon is observed for directional purposes.
			 * Must not be coplanar with the plane defined by points.
			 * @param points An array of the vertices/points to compute the area for. These should be in order
			 * of connecting edges - i.e. points[0] connects to point 1, 1 to 2 etc till n-1 connects to 0.
			 * They should also be coplanar.
			 * Must be at least three vertices.
			 * @param nPoints Number of points in points
			 *
			 * @return Whether the ordering is clockwise when viewing the polygon from the observation point
			 * @retval true The ordering is clockwise
			 * @retval false The ordering is anti-clockwise
			 */
			template <class T>
			__attribute__((warn_unused_result))
			inline bool isVertexOrderClockwise(const euc::EuclideanPoint<T,3>& observation, euc::EuclideanPoint<T,3> * points, int nPoints);
		}
	}
}

// Include Header Level Definitions
#include "Polygon3D.ipp"

#endif
