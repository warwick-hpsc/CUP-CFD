/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Header Level Definitions for the Pyramid class
 */

#ifndef CUPCFD_GEOMETRY_SHAPES_PYRAMID_IPP_H
#define CUPCFD_GEOMETRY_SHAPES_PYRAMID_IPP_H

#include <iostream>

namespace cupcfd
{
	namespace geometry
	{
		namespace shapes
		{
			// === Constructors/Deconstructors ===

			template <class P, class T>
			Pyramid<P,T>::Pyramid(const cupcfd::geometry::euclidean::EuclideanPoint<T,3>& apex,
								  const P& base)
			:apex(apex), base(base)
			{

			}

			template <class P, class T>
			Pyramid<P,T>::~Pyramid()
			{

			}

			// === Static Methods ===


			// === Concrete Methods ===

			template <class P, class T>
			inline bool Pyramid<P,T>::isPointInside(const cupcfd::geometry::euclidean::EuclideanPoint<T,3>& point)
			{
				// ToDo: This algorithm could be moved up to a more general level in Polyhedron.
				// However, we would need to either (a) find a way to export the vertices (overhead of extra copies)
				// or (b) store a general vertex member list (such as an array) at the Polyhedron level - however
				//since the size would be variable, this would be an array pointer, not a fixed size, and so every
				//Polyhedron object would just point to a different fragmented arbitrary location in memory - would be bad
				//for creating contiguous arrays of Polyhedrons (not that I would recommend AoS as an approach).
				
				// ToDo: Const/benefit - if this is used frequently may be cheaper to store the faces rather than
				// reconstruct them - however, this will increase the cost of the constructors....
				
				// Algorithm: Construct a vector from the point to each face
				// Assuming the normal vector direction is pointing out, if the dot-product of the face normal 
				// vector and the point->face vector is positive, they are both in the same direction, and
				// thus the point is 'inside' for that face.
				// Repeat for all faces, and if 'inside' all of them, the point is inside the polygon
				
				T dotProd;
				cupcfd::geometry::euclidean::EuclideanVector<T,3> pointVector;
				
				// (1) Test against base. We can reuse the base, but need to validate whether it is in clockwise or anticlockwise order
				// ToDo: For now we will presume it is in anticlockwise order when viewed from outside, since we can control this via the
				// Tetrahedron/QuadPyramid constructors, such that the normal should point outwards.
				// (Alternative is use weighted polyhedron area computation to check)
				// ToDo: This is also assuming that the base is convex. This is fine as long as we are working with Triangular/Quadrilateral
				// Bases, but any higher and this would need to be tested.
				
				// Polygon must always have at least three vertices
				cupcfd::geometry::euclidean::EuclideanPlane3D<T> basePlane(base.vertices[0], base.vertices[1], base.vertices[2]);

				// Create Point Vector
				pointVector = base.vertices[0] - point;
				
				// Test if exactly on base plane and inside base
				if(this->base.isPointInside(point))
				{
					return true;
				}
				
				dotProd = pointVector.dotProduct(basePlane.getNormal());
				if(dotProd < 0)
				{
					// Negative means different directions - i.e. the normal vector pointing out is not the same the same direction
					// as the point vector pointing in. Therefore point not in shape
					return false;
				}
					
				// (2) Test against other faces
				for(int i = 0; i < this->base.nVertices; i++)
				{
					// ToDo: This is presuming that all type 'P' base objects have a vertices data structure
					// Could make it so Polygon3D has a pointer to a vertices array, and then leave the 
					// storage implementation to the subclasses and updating the pointer to it.
					
					// Construct a plane using anticlockwise ordering when viewed from outside/ Clockwise from inside (assuming face is coplanar)
					// This is done presuming the base vertices are already in anticlockwise order when viewed from outside/clockwise from inside
					// so we just need to traverse them in the correct order from the apex point.
					cupcfd::geometry::euclidean::EuclideanPlane3D<T> facePlane(this->apex, base.vertices[(i+1)%this->base.nVertices], base.vertices[i]);
					
					cupcfd::geometry::shapes::Triangle3D<T> face(this->apex, base.vertices[(i+1)%this->base.nVertices], base.vertices[i]);
					
					// (2a) Create a vector to the first point on the face
					pointVector = base.vertices[i] - point;
					
					// (2b) Are we on a face? If so this counts as inside, but the vector will be perpendicular
					// to the normal/parallel to the plane
					if(face.isPointInside(point))
					{
						return true;
					}
					
					// (2c) Is the point vector and the normal vector facing the same direction?
					// If not, this point is not inside the polyhedron
					dotProd = pointVector.dotProduct(facePlane.getNormal());

					if(dotProd < 0)
					{
						// Negative means different directions - i.e. the normal vector pointing out is not the same the same direction
						// as the point vector pointing in. Therefore point not in shape
						return false;
					}
				}
				
				// If we made it this far checks were passed for all faces - therefore we are in the shape
				return true;
			}

			template <class P, class T>
			T Pyramid<P,T>::computeVolume()
			{
				// === Volume = 1/3 Base Area * Height ===
				
				// (1) Compute Area of Base
				T area = base.computeArea();
				
				// (2) Compute Height as shortest distance from base plane
				cupcfd::geometry::euclidean::EuclideanPlane3D<T> plane(this->base.vertices[0], this->base.vertices[1], this->base.vertices[2]);
				T height = plane.shortestDistance(apex);
				
				// (3) Compute and return volume
				return (T(1.0)/T(3.0)) * area * height;
			}
			
			template <class P, class T>
			cupcfd::geometry::euclidean::EuclideanPoint<T,3> Pyramid<P,T>::computeCentroid()
			{
				// https://en.wikipedia.org/wiki/Centroid/#Of_a_cone_or_pyramid
				// Centroid = 1/4 the distance from the base centroid to the apex
				
				cupcfd::geometry::euclidean::EuclideanPoint<T,3> baseCentroid = this->base.computeCentroid();
				cupcfd::geometry::euclidean::EuclideanVector<T,3> baseCentroidToApex = this->apex - baseCentroid;
		
				return baseCentroid + (T(0.25) * baseCentroidToApex);
			}
		}
	}
}

#endif
