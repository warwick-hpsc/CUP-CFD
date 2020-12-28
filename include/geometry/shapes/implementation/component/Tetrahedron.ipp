/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Header Level Definitions for the Tetrahedron class
 */

#ifndef CUPCFD_GEOMETRY_SHAPES_TETRAHEDRON_IPP_H
#define CUPCFD_GEOMETRY_SHAPES_TETRAHEDRON_IPP_H

namespace cupcfd
{
	namespace geometry
	{
		namespace shapes
		{
			// === Constructors/Deconstructors ===

			template <class T>
			Tetrahedron<T>::Tetrahedron(cupcfd::geometry::euclidean::EuclideanPoint<T,3>& apex,
										cupcfd::geometry::euclidean::EuclideanPoint<T,3>& f,
										cupcfd::geometry::euclidean::EuclideanPoint<T,3>& bl,
										cupcfd::geometry::euclidean::EuclideanPoint<T,3>& br)
			: Pyramid<Triangle3D<T>,T>(apex, Triangle3D<T>(f, br, bl))
			{
				// Note in the initialiser list that we define the base in anti-clockwise order
				// when viewed from the outside/clockwise when viewed from inside the polyhedron
				// This is so normals for the base when using the vertices in order point outwards.
			
				this->numEdges = 6;
				this->numVertices = 4;
				this->numFaces = 4;
			}
			
			template <class T>
			Tetrahedron<T>::~Tetrahedron()
			{
			}

			// === Static Methods ===

			// === Concrete Methods ===
			
			template <class T>
			inline bool Tetrahedron<T>::isPointOnEdge(const cupcfd::geometry::euclidean::EuclideanPoint<T,3>& point) {							
				// ToDo: This can likely be condensed to a generic polygon method, using a getEdge method	  
				
				bool tests[6];
				tests[0] = isPointOnLine(this->apex, this->base.vertices[0], point);
				tests[1] = isPointOnLine(this->apex, this->base.vertices[1], point);
				tests[2] = isPointOnLine(this->apex, this->base.vertices[2], point);
				tests[3] = isPointOnLine(this->base.vertices[0], this->base.vertices[1], point);
				tests[4] = isPointOnLine(this->base.vertices[1], this->base.vertices[2], point);
				tests[5] = isPointOnLine(this->base.vertices[2], this->base.vertices[0], point);
				
				// Test if the point lies on each edge
				for(int i = 0; i < 6; i++) {
					if(tests[i]) {
						return true;
					}
				}
				
				return false;
			}
			
			template <class T>
			inline bool Tetrahedron<T>::isPointOnVertex(const cupcfd::geometry::euclidean::EuclideanPoint<T,3>& point) {
				return (this->apex == point) || (this->base.vertices[0] == point) || (this->base.vertices[1] == point) || (this->base.vertices[2] == point);
			}
			
			template<class T>
			cupcfd::geometry::euclidean::EuclideanPoint<T,3> Tetrahedron<T>::computeCentroid() {
				// Override Pyramid's function, because for Tetrahedron is much easier:

				// Arithmetic mean of four vertices
				return (this->base.vertices[0] + this->base.vertices[1] + this->base.vertices[2] + this->base.vertices[3]) / T(4);
 			}
		}
	}
}

#endif
