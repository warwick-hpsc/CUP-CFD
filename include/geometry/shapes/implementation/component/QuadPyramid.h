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
					/**
					 * Build a Pyramid object.
					 *
					 * @param apex The point that defines the apex of the pyramid
					 * @param quad The Quadrilateral3D base of the pyramid
					 */
					QuadPyramid(const cupcfd::geometry::euclidean::EuclideanPoint<T,3>& apex, 
								const cupcfd::geometry::shapes::Quadrilateral3D<T>& quad);

					/**
					 *
					 */
					~QuadPyramid();

					// === Concrete Methods ===

					/**
					 * Determine whether the provided point is on an edge of the polyhedron
					 *
					 * @param point The point to test
					 *
					 * @return Return true if the point is on an edge of this polyhedron
					 */
					__attribute__((warn_unused_result))
					inline bool isPointOnEdge(const cupcfd::geometry::euclidean::EuclideanPoint<T,3>& point);

					/**
					 * Determine whether the provided point is on a vertex of the polyhedron
					 *
					 * @param point The point to test
					 *
					 * @return Return true if the point is on an edge of this polyhedron
					 */
					__attribute__((warn_unused_result))
					inline bool isPointOnVertex(const cupcfd::geometry::euclidean::EuclideanPoint<T,3>& point);
			};
		}
	}
}

// Include Header Level Definitions
#include "QuadPyramid.ipp"

#endif
