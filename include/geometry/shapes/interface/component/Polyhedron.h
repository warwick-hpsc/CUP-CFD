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
 * Declarations for the Polyhedron class
 */

#ifndef CUPCFD_GEOMETRY_SHAPES_POLYHEDRON_INCLUDE_H
#define CUPCFD_GEOMETRY_SHAPES_POLYHEDRON_INCLUDE_H

#include "EuclideanPoint.h"

namespace cupcfd
{
	namespace geometry
	{
		namespace shapes
		{
			/**
			 * Top level interface for Polyhedron shapes.
			 * Declares a set of common operations and/or members.
			 *
			 * Uses a CRTP design pattern to minimise/remove virtual overheads
			 *
			 * @tparam P The implementation type of the polygon
			 * @tparam T The type of the spatial domain
			 */

			enum PolyhedronType
			{
				POLYHEDRON_TETRAHEDRON,
				POLYHEDRON_QUADPYRAMID,
				POLYHEDRON_TRIPRISM,
				POLYHEDRON_HEXAHEDRON,
				POLYHEDRON_UNKNOWN
			};

			// For now, we will hard code all Polyhedrons to be 3D (not interested in 4D and above, cannot be 2D)
			template <class P, class T>
			class Polyhedron
			{
				public:
					// === Members ===

					/** Number of edges in polyhedron **/
					int nEdges;

					/** Number of vertices in polyhedron **/
					int nVertices;

					/** Number of faces in the polyhedron **/
					int nFaces;

					// === Constructors/Deconstructors ===

					/**
					 * Default Constructor
					 */
					Polyhedron();

					/**
					 * Deconstructor
					 */
					~Polyhedron();

					// === Static Methods ===

					// === Concrete Methods ===

					/**
					 * Get the number of edges in this polyhedron.
					 *
					 * @tparam P The implementation type of the polygon
					 * @tparam T The type of the spatial domain
					 *
					 * @return The number of edges.
					 */
					inline int getNEdges();

					/**
					 * Get the number of vertices in this polyhedron.
					 *
					 * @tparam P The implementation type of the polygon
					 * @tparam T The type of the spatial domain
					 *
					 * @return The number of vertices.
					 */
					inline int getNVertices();

					// === Interface Methods ===

					/**
					 * Determine whether the provided point is inside the polyhedron.
					 * Edges/Faces/Vertices are treated as inside the polygon for this purpose.
					 *
					 * @tparam P The implementation type of the polygon
					 * @tparam T The type of the spatial domain
					 *
					 * @return Return whether the point exists inside this polyhedron
					 * @retval true The point is inside the polyhedron
					 * @retval false The point is outside the polyhedron
					 */
					inline bool isPointInside(cupcfd::geometry::euclidean::EuclideanPoint<T,3>& point);

					/**
					 * Compute the volume of this polyhedron
					 *
					 * @tparam P The implementation type of the polygon
					 * @tparam T The type of the spatial domain
					 *
					 * @return The computed volume
					 */
					inline T computeVolume();
			};

			// These methods are equivalent to static, but we'll place them here since they're generic
			// enough we don't want to go through the hassle of creating a templated class just to call them

			/**
			 * Get an identifier for what type of polyhedron has the specified
			 * number of vertices and faces
			 *
			 * @param nVertices Number of vertices
			 * @param nFaces Number of faces
			 *
			 * @return An indentifier for the type of Polyhedron
			 */
			inline PolyhedronType findPolyhedronType(int nVertices, int nFaces);
		}
	}
}

// Include Header Definitions
#include "Polyhedron.ipp"

#endif
