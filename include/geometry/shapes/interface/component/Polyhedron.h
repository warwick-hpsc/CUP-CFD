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

namespace euc = cupcfd::geometry::euclidean;

namespace cupcfd
{
	namespace geometry
	{
		namespace shapes
		{
			/**
			 * Top level interface for Polyhedron shapes.
			 * Fix dimension to 3D (not interested in 4D and above, cannot be 2D)
			 * Declares a set of common operations and/or members.
			 *
			 * Uses a CRTP design pattern to minimise/remove virtual overheads
			 *
			 * @tparam S The polyhedron specialisation
			 * @tparam T Numerical type
			 */

			enum PolyhedronType 
			{
				POLYHEDRON_TETRAHEDRON,
				POLYHEDRON_QUADPYRAMID,
				POLYHEDRON_TRIPRISM,
				POLYHEDRON_HEXAHEDRON,
				POLYHEDRON_UNKNOWN
			};

			template <class S, class T>
			class Polyhedron
			{
				public:
					// === Constructors/Deconstructors ===

					Polyhedron();

					~Polyhedron();

					// === Concrete Methods ===

					// === Interface Methods ===

					/**
					 * Return volume of polyhedron, calculating if not known
					 *
					 * @return Polyhedron volume
					 */
					__attribute__((warn_unused_result))
					auto getVolume();

					/**
					 * Return centroid of polyhedron, calculating if not known
					 *
					 * @return Polyhedron centroid
					 */
					__attribute__((warn_unused_result))
					auto getCentroid();

					/**
					 * Determine whether the provided point is inside the polyhedron.
					 * Edges/Faces/Vertices are treated as inside the polygon for this purpose.
					 *
					 * @return Return true if point exists inside this polyhedron
					 */
					__attribute__((warn_unused_result))
					auto isPointInside(euc::EuclideanPoint<T,3>& point);

				protected:
					// This class contains all data needed/used by derived classes, 
					// derived classes just implement new methods. 
					// This makes implementing operator=() much easier.
					T volume;
					bool volumeComputed = false;

					euc::EuclideanPoint<T,3> centroid;
					bool centroidComputed = false;
			};

			// These methods are equivalent to static, but we'll place them here since they're generic
			// enough we don't want to go through the hassle of creating a templated class just to call them

			/**
			 * Get an identifier for what type of polyhedron has the specified
			 * number of vertices and faces
			 *
			 * @return An indentifier for the type of Polyhedron
			 */
			__attribute__((warn_unused_result))
			inline PolyhedronType findPolyhedronType(int nVertices, int nFaces);
		}
	}
}

// Include Header Definitions
#include "Polyhedron.ipp"

#endif
