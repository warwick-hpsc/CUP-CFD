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

			// === Concrete Methods ===
			
			template <class P, class T>
			inline int Polyhedron<P,T>::getNumEdges() {
				return this->numEdges;
			}
				
			template <class P, class T>		
			inline int Polyhedron<P,T>::getNumVertices() {
				return this->numVertices;
			}		
								
			// === Interface Methods ===

			template <class P, class T>
			inline bool Polyhedron<P,T>::isPointInside(cupcfd::geometry::euclidean::EuclideanPoint<T,3>& point) {
				return static_cast<P*>(this)->isPointInside(point);
			}

			// template <class P, class T>
			// T Polyhedron<P,T>::computeVolume() {
			// 	return static_cast<P*>(this)->computeVolume();
			// }
			
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
