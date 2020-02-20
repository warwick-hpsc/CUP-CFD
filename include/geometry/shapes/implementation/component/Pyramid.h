/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Declarations for the Pyramid class
 *
 */

#ifndef CUPCFD_GEOMETRY_SHAPES_PYRAMID_INCLUDE_H
#define CUPCFD_GEOMETRY_SHAPES_PYRAMID_INCLUDE_H

#include "Polygon3D.h"
#include "Polyhedron.h"
#include "EuclideanPoint.h"
#include "Quadrilateral3D.h"

namespace cupcfd
{
	namespace geometry
	{
		namespace shapes
		{
			/**
			 * Class to store geometry information and access/compute various properties
			 * of Pyramid shapes
			 *
			 * @tparam P Data type of the Polygon that makes up the pyramid base
			 * @tparam T Datatype of the geometry
			 */
			template <class P, class T>
			class Pyramid : public Polyhedron<Pyramid<P,T>,T>
			{
				public:
					// === Members ===

					/**
					 * The point that defines the apex of the pyramid/the vertex point
					 * shared by all triangular faces
					 */
					cupcfd::geometry::euclidean::EuclideanPoint<T,3> apex;

					/**
					 * The polygon base that makes up the bottom face of the pyramid
					 */
					// Note: We could make this a Polygon type and enforce the interface restriction,
					// but we would have to make it a pointer since the compiler would not know how much
					// space to allocate, and ideally would prefer to avoid having a pointer to elsewhere
					// in a potential data structure
					P base;

					// === Constructors/Deconstructors ===

					/**
					 * Build a Pyramid object.
					 *
					 * @param apex The point that defines the apex of the pyramid/the vertex point
					 * shared by all triangular faces
					 * @param base The polygon base that makes up the bottom face of the pyramid.
					 * Points in the base object should be stored in anti-clockwise order when viewed from
					 * outside the object/clockwise when viewed from inside the object.
					 * Not following this order will cause issues with certain functionality (e.g. isPointInside)
					 * since it will affect the directions of computed normals.
					 *
					 * @tparam P Data type of the Polygon that makes up the pyramid base
					 * @tparam T Datatype of the geometry
					 */
					Pyramid(const cupcfd::geometry::euclidean::EuclideanPoint<T,3>& apex, const P& base);

					/**
					 *
					 */
					virtual ~Pyramid();

					// === Static Methods ===

					// === Concrete Methods ===

					/**
					 * Determine whether the provided point is inside the polyhedron.
					 * Edges/Faces/Vertices are treated as inside the polygon for this purpose.
					 *
					 * @tparam P Data type of the Polygon that makes up the pyramid base
					 * @tparam T Datatype of the geometry
					 *
					 * @return Return whether the point exists inside this polyhedron
					 * @retval true The point is inside the polyhedron
					 * @retval false The point is outside the polyhedron
					 */
					inline bool isPointInside(const cupcfd::geometry::euclidean::EuclideanPoint<T,3>& point);

					/**
					 * Compute the volume of the pyramid
					 *
					 * @tparam P Data type of the Polygon that makes up the pyramid base
					 * @tparam T Datatype of the geometry
					 *
					 * @return The computed volume
					 */
					T computeVolume();

					/**
					 * Compute the centroid of the pyramid
					 *
					 * @tparam P Data type of the Polygon that makes up the pyramid base
					 * @tparam T Datatype of the geometry
					 *
					 * @return The computed centroid
					 */
					cupcfd::geometry::euclidean::EuclideanPoint<T,3> computeCentroid();
			};
		}
	}
}

// Include Header Level Definitions
#include "Pyramid.ipp"

#endif
