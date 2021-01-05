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

#include "Polygon.h"
#include "Polyhedron.h"
#include "EuclideanPoint.h"
#include "Quadrilateral3D.h"

namespace euc = cupcfd::geometry::euclidean;

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
			 * @tparam P Polygon specialisation
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
					euc::EuclideanPoint<T,3> apex;

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
					 * @param apex The point that defines the apex of the pyramid
					 * @param base The polygon base of the pyramid.
					 */
					Pyramid(const euc::EuclideanPoint<T,3>& apex, const P& base);

					/**
					 *
					 */
					virtual ~Pyramid();

					// === Concrete Methods ===

					/**
					 * Determine whether the provided point is on an edge of the Pyramid
					 *
					 * @param point The point to test
					 *
					 * @return Return true if the point is on an edge of this Pyramid
					 */
					__attribute__((warn_unused_result))
					bool isPointOnEdge(const euc::EuclideanPoint<T,3>& point);

					/**
					 * Determine whether the provided point is on a vertex of the Pyramid
					 *
					 * @param point The point to test
					 *
					 * @return Return true if the point is on an edge of this Pyramid
					 */
					__attribute__((warn_unused_result))
					bool isPointOnVertex(const euc::EuclideanPoint<T,3>& point);

					/**
					 * Determine whether the provided point is inside the Pyramid.
					 * Edges/Faces/Vertices are treated as inside the polygon for this purpose.
					 *
					 * @return Return true if the point exists inside this Pyramid
					 */
					__attribute__((warn_unused_result))
					bool isPointInside(const euc::EuclideanPoint<T,3>& point);

					/**
					 * Return volume of Pyramid, calculating if not known
					 *
					 * @return Pyramid volume
					 */
					T getVolume();

					/**
					 * Return centroid of Pyramid, calculating if not known
					 *
					 * @return Pyramid centroid
					 */
					euc::EuclideanPoint<T,3> getCentroid();

				protected:
					/**
					 * Compute the volume of the pyramid
					 *
					 * @return The computed volume
					 */
					__attribute__((warn_unused_result))
					T computeVolume();

					/**
					 * Compute the centroid of the pyramid
					 *
					 * @return The computed centroid
					 */
					__attribute__((warn_unused_result))
					euc::EuclideanPoint<T,3> computeCentroid();
			};
		}
	}
}

// Include Header Level Definitions
#include "Pyramid.ipp"

#endif
