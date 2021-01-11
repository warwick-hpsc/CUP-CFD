/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Declarations for the Hexahedron class
 *
 */

#ifndef CUPCFD_GEOMETRY_SHAPES_HEXAHEDRON_INCLUDE_H
#define CUPCFD_GEOMETRY_SHAPES_HEXAHEDRON_INCLUDE_H

#include "Polygon3D.h"
#include "Polyhedron.h"
#include "EuclideanPoint.h"
#include "Quadrilateral3D.h"
#include "Tetrahedron.h"

namespace euc = cupcfd::geometry::euclidean;
namespace shapes = cupcfd::geometry::shapes;

namespace cupcfd
{
	namespace geometry
	{
		namespace shapes
		{
			/**
			 * Stores geometry data for a Hexahedron shape.
			 *
			 * Does not presume that opposite faces are parallel, or equivalent.
			 *
			 * @tparam T The numerical type
			 */
			template <class T>
			class Hexahedron : public Polyhedron<Hexahedron<T>,T>
			{
				public:
					// === Members ===

					shapes::Quadrilateral3D<T> faces[6];

					// === Constructors/Deconstructors ===

					/**
					 * Construct a hexahedron from 6 appropriately-connected quadrilaterals. 
					 * Ordering is not important, constructor will determine orientation.
					 * Any faces with inward-pointing normals will be inverted.
					 * It will also find face opposite f0, and shift vertex ordering 
					 * so that vertex 0 of both faces are connected by an edge.
					 */
					Hexahedron(const shapes::Quadrilateral3D<T>& f0,
							   const shapes::Quadrilateral3D<T>& f1,
							   const shapes::Quadrilateral3D<T>& f2,
							   const shapes::Quadrilateral3D<T>& f3,
							   const shapes::Quadrilateral3D<T>& f4,
							   const shapes::Quadrilateral3D<T>& f5);

					/**
					 *
					 */
					~Hexahedron();

					// === Concrete Methods ===

					/**
					 * Check that no two faces used to construct Hexahedron 
					 * have same vertices.
					 *
					 * @return True if are faces are distinct, False if 
					 * any two faces have same vertices.
					 */
					bool verifyFacesDistinct();

					/**
					 * Return volume of hexahedron, calculating if not known
					 *
					 * @return Hexahedron volume
					 */
					__attribute__((warn_unused_result))
					T getVolume();

					/**
					 * Return centroid of hexahedron, calculating if not known
					 *
					 * @return Hexahedron centroid
					 */
					__attribute__((warn_unused_result))
					euc::EuclideanPoint<T,3> getCentroid();

					/**
					 * Determine whether the provided point is inside the polyhedron.
					 * Edges/Faces/Vertices are treated as inside the polygon for this purpose.
					 *
					 * @param point The point to test
					 *
					 * @return Return true if the point exists inside this polyhedron
					 */
					__attribute__((warn_unused_result))
					bool isPointInside(const euc::EuclideanPoint<T,3>& point);

					/**
					 * Determine whether the provided point is on an edge of the polyhedron
					 *
					 * @param point The point to test
					 *
					 * @return Return true if the point is on an edge of this polyhedron
					 */
					__attribute__((warn_unused_result))
					bool isPointOnEdge(const euc::EuclideanPoint<T,3>& point);

					/**
					 * Determine whether the provided point is on a vertex of the polyhedron
					 *
					 * @param point The point to test
					 *
					 * @return Return true if the point is on an edge of this polyhedron
					 */
					__attribute__((warn_unused_result))
					inline bool isPointOnVertex(const euc::EuclideanPoint<T,3>& point);

				protected:
					// Stores Data as Points - Can reconstruct faces if needed

					uint top_face_idx = 99;
					uint bottom_face_idx = 99;

					/**
					 * Decomposition of hexahedron into 5 tetrahedrons
					 */
					shapes::Tetrahedron<T> tetra[5];

					void detectOpposingFaces();

					void orientFaceNormals();

					/**
					 * Compute the volume of this polyhedron
					 *
					 * @return The computed volume
					 */
					__attribute__((warn_unused_result))
					T computeVolume();

					/**
					 * Compute the centroid of the Hexahedral
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
#include "Hexahedron.ipp"

#endif
