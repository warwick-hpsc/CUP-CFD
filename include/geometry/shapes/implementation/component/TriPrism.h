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
			 */
			template <class T>
			class TriPrism : public Polyhedron<TriPrism<T>,T>
			{
				public:
					// === Members ===

					/**
					 * Vertices of the Polyhedron.
					 * Stored in the following order:
					 * 0: Top Front Vertex
					 * 1: Top Left Back Vertex
					 * 2: Top Right Back Vertex
					 * 3: Bottom Front Vertex
					 * 4: Bottom Left Back Vertex
					 * 5: Bottom Right Back Vertex
					 **/
					cupcfd::geometry::euclidean::EuclideanPoint<T,3> verticesStore[6];

					// === Constructors/Deconstructors ===

					/**
					 * Construct a Triprism from the provided vertices
					 *
					 * @param tf Top Front Vertex
					 * @param tlb Top Left Back Vertex
					 * @param trb Top Right Back Vertex
					 * @param bf Bottom Front Vertex
					 * @param blb Bottom Left Back Vertex
					 * @param brb Bottom Right Back Vertex
					 *
					 */
					TriPrism(const cupcfd::geometry::euclidean::EuclideanPoint<T,3>& tf,
						   	 const cupcfd::geometry::euclidean::EuclideanPoint<T,3>& tlb,
						   	 const cupcfd::geometry::euclidean::EuclideanPoint<T,3>& trb,
						   	 const cupcfd::geometry::euclidean::EuclideanPoint<T,3>& bf,
						   	 const cupcfd::geometry::euclidean::EuclideanPoint<T,3>& blb,
						   	 const cupcfd::geometry::euclidean::EuclideanPoint<T,3>& brb);

					/**
					 *
					 */
					virtual ~TriPrism();

					// === Static Methods ===

					// === Concrete Methods ===

					/**
					 * Determine whether the provided point is inside the polyhedron.
					 * Edges/Faces/Vertices are treated as inside the polygon for this purpose.
					 *
					 * @tparam T The type of the spatial domain
					 * @tparam N The dimension of the spatial domain that the shape exists in
					 *
					 * @return Return whether the point exists inside this polyhedron
					 * @retval true The point is inside the polyhedron
					 * @retval false The point is outside the polyhedron
					 */
					inline bool isPointInside(cupcfd::geometry::euclidean::EuclideanPoint<T,3>& point);

					/**
					 * Compute the volume of this polyhedron
					 *
					 * @tparam T The type of the spatial domain
					 * @tparam N The dimension of the spatial domain that the shape exists in
					 *
					 * @return The computed volume
					 */
					T computeVolume();

					/**
					 * Compute the centroid of the polyhedron
					 *
					 * @tparam T Datatype of the geometry
					 *
					 * @return The computed centroid
					 */
					cupcfd::geometry::euclidean::EuclideanPoint<T,3> computeCentroid();

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
#include "TriPrism.ipp"

#endif
