/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Declarations for the QuadPyramid class
 *
 */

#ifndef CUPCFD_GEOMETRY_SHAPES_QUADPYRAMID_INCLUDE_H
#define CUPCFD_GEOMETRY_SHAPES_QUADPYRAMID_INCLUDE_H

#include "Polygon3D.h"
#include "Polyhedron.h"
#include "EuclideanPoint.h"
#include "Quadrilateral3D.h"
#include "Pyramid.h"

namespace cupcfd
{
	namespace geometry
	{
		namespace shapes
		{
			/**
			 * Class to store geometry information and access/compute various properties
			 * of Tetrahedrons
			 *
			 * @tparam T Datatype of the geometry
			 */
			template <class T>
			class QuadPyramid : public Pyramid<Quadrilateral3D<T>,T>
			{
				public:
					// === Members ===

					// === Constructors/Deconstructors ===

					/**
					 * Build a Pyramid object.
					 *
					 * @param apex The point that defines the apex of the pyramid/the vertex point
					 * shared by all triangular faces
					 * @param fl The 'front' left point of the triangular base
					 * @param fr The 'front' right point of the triangular base
					 * @param bl The 'back' left point of the triangular base
					 * @param br The 'back' right point of the triangular base
					 *
					 * @tparam T Datatype of the geometry
					 */
					QuadPyramid(const cupcfd::geometry::euclidean::EuclideanPoint<T,3>& apex,
							    const cupcfd::geometry::euclidean::EuclideanPoint<T,3>& fl,
								const cupcfd::geometry::euclidean::EuclideanPoint<T,3>& fr,
								const cupcfd::geometry::euclidean::EuclideanPoint<T,3>& bl,
								const cupcfd::geometry::euclidean::EuclideanPoint<T,3>& br);

					/**
					 *
					 */
					~QuadPyramid();

					// === Static Methods ===

					// === Concrete Methods ===

					/**
					 * Determine whether the provided point is on an edge of the polyhedron
					 *
					 * @param point The point to test
					 *
					 * @tparam P The implementation type of the polygon
					 * @tparam T The type of the spatial domain
					 * @tparam N The dimension of the spatial domain that the shape exists in
					 *
					 * @return Return whether the point is on an edge of this polyhedron
					 * @retval true The point is on an edge of the polyhedron
					 * @retval false The point is not on an edge of the polyhedron
					 */
					inline bool isPointOnEdge(const cupcfd::geometry::euclidean::EuclideanPoint<T,3>& point);

					/**
					 * Determine whether the provided point is on a vertex of the polyhedron
					 *
					 * @param point The point to test
					 *
					 * @tparam P The implementation type of the polygon
					 * @tparam T The type of the spatial domain
					 * @tparam N The dimension of the spatial domain that the shape exists in
					 *
					 * @return Return whether the point is on an edge of this polyhedron
					 * @retval true The point is on an edge of the polyhedron
					 * @retval false The point is not on an edge of the polyhedron
					 */
					inline bool isPointOnVertex(const cupcfd::geometry::euclidean::EuclideanPoint<T,3>& point);
			};
		}
	}
}

// Include Header Level Definitions
#include "QuadPyramid.ipp"

#endif
