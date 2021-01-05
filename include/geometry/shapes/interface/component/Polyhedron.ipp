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
 */

#ifndef CUPCFD_GEOMETRY_SHAPES_POLYHEDRON_IPP_H
#define CUPCFD_GEOMETRY_SHAPES_POLYHEDRON_IPP_H

namespace euc = cupcfd::geometry::euclidean;

namespace cupcfd
{
	namespace geometry
	{
		namespace shapes
		{
			// === Constructors/Deconstructors ===

			template <class P, class T>
			Polyhedron<P,T>::Polyhedron() {

			}

			template <class P, class T>
			Polyhedron<P,T>::~Polyhedron() {

			}

			// === Interface Methods ===

			template <class P, class T>
			auto Polyhedron<P,T>::getVolume() {
				return static_cast<P*>(this)->getVolume();
			}

			template <class P, class T>
			auto Polyhedron<P,T>::getCentroid() {
				return static_cast<P*>(this)->getCentroid();
			}

			template <class P, class T>
			auto Polyhedron<P,T>::isPointInside(euc::EuclideanPoint<T,3>& point) {
				return static_cast<P*>(this)->isPointInside(point);
			}

			// Namespace non-class methods

			inline PolyhedronType findPolyhedronType(int nVertices, int nFaces) {
				// https://en.wikipedia.org/wiki/Types_of_mesh
			
				// Determine the type of cell
				if(nVertices == 4 && nFaces == 4) {
					// Tetrahedron
					return POLYHEDRON_TETRAHEDRON;
				}
				else if(nVertices == 5 && nFaces == 5) {
					// Quad Pyramid
					return POLYHEDRON_QUADPYRAMID;
				}
				else if(nVertices == 6 && nFaces == 5) {
					// TriPrism
					return POLYHEDRON_TRIPRISM;
				}
				else if(nVertices == 8 && nFaces == 6) {
					// Hexahedron
					return POLYHEDRON_HEXAHEDRON;
				}
				else {
					// Unknown Type
					return POLYHEDRON_UNKNOWN;
				}
			}
		}
	}
}

#endif
