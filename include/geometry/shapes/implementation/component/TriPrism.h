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
 * Declarations for the TriPrism class
 */

#ifndef CUPCFD_GEOMETRY_SHAPES_TRIPRISM_INCLUDE_H
#define CUPCFD_GEOMETRY_SHAPES_TRIPRISM_INCLUDE_H

#include "Polygon3D.h"
#include "Polyhedron.h"
#include "Triangle3D.h"

namespace shapes = cupcfd::geometry::shapes;

namespace cupcfd
{
	namespace geometry
	{
		namespace shapes
		{
			/**
			 * Class for storing TriPrism data.
			 *
			 * ToDo: Currently presumes that the non-triangular faces
			 * have normals in the same plane and does not handle truncated prisms
			 * (i.e. the top and bottom triangular faces are parallel)
			 *
			 * @tparam T Numerical type
			 *
			 */
			template <class T>
			class TriPrism : public Polyhedron<TriPrism<T>,T>
			{
				public:
					// === Members ===

					/** Top and bottom faces **/
					shapes::Triangle3D<T> top;
					shapes::Triangle3D<T> bottom;

					// === Constructors/Deconstructors ===

					/**
					 * Construct a Triprism from the provided vertices
					 *
					 * @param top Top Triangle
					 * @param bottom Bottom Triangle
					 *
					 */
					TriPrism(const shapes::Triangle3D<T>& top, const shapes::Triangle3D<T>& bottom);

					/**
					 *
					 */
					~TriPrism();

					// === Concrete Methods ===

					/**
					 * Determine whether the provided point is inside the polyhedron.
					 * Edges/Faces/Vertices are treated as inside the polygon for this purpose.
					 *
					 * @return Return true if the point exists inside this polyhedron
					 */
					__attribute__((warn_unused_result))
					inline bool isPointInside(cupcfd::geometry::euclidean::EuclideanPoint<T,3>& point);

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

				// protected:

					/**
					 * Compute the volume of this polyhedron
					 *
					 * @return The computed volume
					 */
					__attribute__((warn_unused_result))
					T computeVolume();

					/**
					 * Compute the centroid of the polyhedron
					 *
					 * @return The computed centroid
					 */
					__attribute__((warn_unused_result))
					cupcfd::geometry::euclidean::EuclideanPoint<T,3> computeCentroid();

			};
		}
	}
}

// Include Header Level Definitions
#include "TriPrism.ipp"

#endif
