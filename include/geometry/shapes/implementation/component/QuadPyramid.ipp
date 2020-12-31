/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Header Level Definitions for the QuadPyramid class
 */

#ifndef CUPCFD_GEOMETRY_SHAPES_QUADPYRAMID_IPP_H
#define CUPCFD_GEOMETRY_SHAPES_QUADPYRAMID_IPP_H

namespace cupcfd
{
	namespace geometry
	{
		namespace shapes
		{
			// === Constructors/Deconstructors ===

			template <class T>
			QuadPyramid<T>::QuadPyramid(const cupcfd::geometry::euclidean::EuclideanPoint<T,3>& apex, 
										const cupcfd::geometry::shapes::Quadrilateral3D<T>& base)
			 : Pyramid<Quadrilateral3D<T>,T>(apex, base)
			{
			}
			
			template <class T>
			QuadPyramid<T>::~QuadPyramid()
			{
			}

			// === Static Methods ===

			// === Concrete Methods ===
			
			template <class T>
			inline bool QuadPyramid<T>::isPointOnEdge(const cupcfd::geometry::euclidean::EuclideanPoint<T,3>& point) {							
				// ToDo: This can likely be condensed to a generic polygon method, using a getEdge method	  
				
				bool tests[8];
				tests[0] = isPointOnLine(this->apex,             this->base.vertices[0], point);
				tests[1] = isPointOnLine(this->apex,             this->base.vertices[1], point);
				tests[2] = isPointOnLine(this->apex,             this->base.vertices[2], point);
				tests[3] = isPointOnLine(this->apex,             this->base.vertices[3], point);
				tests[4] = isPointOnLine(this->base.vertices[0], this->base.vertices[1], point);
				tests[5] = isPointOnLine(this->base.vertices[1], this->base.vertices[2], point);
				tests[6] = isPointOnLine(this->base.vertices[2], this->base.vertices[3], point);
				tests[7] = isPointOnLine(this->base.vertices[3], this->base.vertices[0], point);
				
				// Test if the point lies on each edge
				for(int i = 0; i < 8; i++) {
					if(tests[i]) {
						return true;
					}
				}
				return false;
			}
			
			template <class T>
			inline bool QuadPyramid<T>::isPointOnVertex(const cupcfd::geometry::euclidean::EuclideanPoint<T,3>& point) {
				return (this->apex == point) || (this->base.vertices[0] == point) || (this->base.vertices[1] == point) || 
					   (this->base.vertices[2] == point) || (this->base.vertices[3] == point);
			}
		}
	}
}

#endif
