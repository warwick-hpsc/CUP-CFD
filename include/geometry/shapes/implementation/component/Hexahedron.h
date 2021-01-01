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

					// Stores Data as Points - Can reconstruct faces if needed

					// ToDo: Might restructure things to be more consistent in naming
					// to follow a generic polyhedron
					// Could makes these a vertices array and enforce ordering internally

					/** Top-Left Front Vertex **/
					cupcfd::geometry::euclidean::EuclideanPoint<T,3> tlf;

					/** Top-Right Front Vertex **/
					cupcfd::geometry::euclidean::EuclideanPoint<T,3> trf;

					/** Bottom-Left Front Vertex **/
					cupcfd::geometry::euclidean::EuclideanPoint<T,3> blf;

					/** Bottom-Right Front Vertex **/
					cupcfd::geometry::euclidean::EuclideanPoint<T,3> brf;

					/** Top-Left Back Vertex **/
					cupcfd::geometry::euclidean::EuclideanPoint<T,3> tlb;

					/** Top-Right Back Vertex **/
					cupcfd::geometry::euclidean::EuclideanPoint<T,3> trb;

					/** Bottom-Left Back Vertex **/
					cupcfd::geometry::euclidean::EuclideanPoint<T,3> blb;

					/** Bottom-Right Back Vertex **/
					cupcfd::geometry::euclidean::EuclideanPoint<T,3> brb;

					cupcfd::geometry::shapes::Quadrilateral3D<T> f1;
					cupcfd::geometry::shapes::Quadrilateral3D<T> f2;
					cupcfd::geometry::shapes::Quadrilateral3D<T> f3;
					cupcfd::geometry::shapes::Quadrilateral3D<T> f4;
					cupcfd::geometry::shapes::Quadrilateral3D<T> f5;
					cupcfd::geometry::shapes::Quadrilateral3D<T> f6;


					// === Constructors/Deconstructors ===

					/**
					 * Construct a hexahedron from the provided points.
					 *
					 * Positional information/ordering of the vertex is important since
					 * it can be used for certain computation - e.g. direction of computed normals
					 *
					 * @param tlf Top-left Front Vertex
					 * @param trf Top-right Front Vertex
					 * @param blf Bottom-left Front Vertex
					 * @param brf Bottom-right Front Vertex
					 * @param tlb Top-left Back Vertex
					 * @param trb Top-right Back Vertex
					 * @param blb Bottom-left Back Vertex
					 * @param brb Bottom-right Back Vertex
					 */
					Hexahedron(const cupcfd::geometry::euclidean::EuclideanPoint<T,3>& tlf,
							   const cupcfd::geometry::euclidean::EuclideanPoint<T,3>& trf,
							   const cupcfd::geometry::euclidean::EuclideanPoint<T,3>& blf,
							   const cupcfd::geometry::euclidean::EuclideanPoint<T,3>& brf,
							   const cupcfd::geometry::euclidean::EuclideanPoint<T,3>& tlb,
							   const cupcfd::geometry::euclidean::EuclideanPoint<T,3>& trb,
							   const cupcfd::geometry::euclidean::EuclideanPoint<T,3>& blb,
							   const cupcfd::geometry::euclidean::EuclideanPoint<T,3>& brb);

					Hexahedron(const cupcfd::geometry::shapes::Quadrilateral3D<T>& f1,
							   const cupcfd::geometry::shapes::Quadrilateral3D<T>& f2,
							   const cupcfd::geometry::shapes::Quadrilateral3D<T>& f3,
							   const cupcfd::geometry::shapes::Quadrilateral3D<T>& f4,
							   const cupcfd::geometry::shapes::Quadrilateral3D<T>& f5,
							   const cupcfd::geometry::shapes::Quadrilateral3D<T>& f6);


					/**
					 *
					 */
					~Hexahedron();


					// === Static Methods ===

					// === Concrete Methods ===

					/**
					 * Determine whether the provided point is inside the polyhedron.
					 * Edges/Faces/Vertices are treated as inside the polygon for this purpose.
					 *
					 * @param point The point to test
					 *
					 * @return Return true if the point exists inside this polyhedron
					 */
					__attribute__((warn_unused_result))
					inline bool isPointInside(const cupcfd::geometry::euclidean::EuclideanPoint<T,3>& point);

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
					 * Compute the centroid of the Hexahedral
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
#include "Hexahedron.ipp"

#endif
