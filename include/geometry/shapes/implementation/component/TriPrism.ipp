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
 * Header Level Definitions for TriPrism class
 */

#ifndef CUPCFD_GEOMETRY_SHAPES_TRIPRISM_IPP_H
#define CUPCFD_GEOMETRY_SHAPES_TRIPRISM_IPP_H

#include "TriPrism.h"
#include "EuclideanPlane3D.h"
#include "Triangle3D.h"

namespace euc = cupcfd::geometry::euclidean;

namespace cupcfd
{
	namespace geometry
	{
		namespace shapes
		{
			// === Constructors/Deconstructors ===

			template <class T>
			TriPrism<T>::TriPrism(const shapes::Triangle3D<T>& top, const shapes::Triangle3D<T>& bottom)
			: top(top), bottom(bottom)
			{
				/*
				// Flip either face if its normal points inwards:
				T dp = this->top.getNormal().dotProduct(this->top.getCentroid() - this->bottom.getCentroid());
				// T dp = this->top.normal.dotProduct(this->top.centroid - this->bottom.centroid);
				if (dp < T(0.0)) {
					this->top.reverseVertexOrdering();
				}
				dp = this->bottom.getNormal().dotProduct(this->bottom.getCentroid() - this->top.getCentroid());
				// dp = this->bottom.normal.dotProduct(this->bottom.centroid - this->top.centroid);
				if (dp < T(0.0)) {
					this->bottom.reverseVertexOrdering();
				}
				*/

				// // this->centroid = this->computeCentroid();
				// this->centroid =  T(1)/T(7) * (this->top.vertices[0]    + this->top.vertices[1]    + this->top.vertices[2] + 
				// 							this->bottom.vertices[0] + this->bottom.vertices[1] + this->bottom.vertices[2]);
				// this->volume = (this->top.centroid - this->bottom.centroid).length() * this->base.area;
			}

			template <class T>
			TriPrism<T>::~TriPrism()
			{
			}

			// === Concrete Methods ===

			template <class T>
			inline bool TriPrism<T>::isPointInside(euc::EuclideanPoint<T,3>& point) {
				// ToDo: This algorithm could be moved up to a more general level in Polyhedron.
				// However, we would need to either (a) find a way to export the vertices (overhead of extra copies)
				// or (b) store a general vertex member list (such as an array) at the Polyhedron level - however
				//since the size would be variable, this would be an array pointer, not a fixed size, and so every
				//Polyhedron object would just point to a different fragmented arbitrary location in memory - would be bad
				//for creating contiguous arrays of Polyhedrons (not that I would recommend AoS as an approach).
				
				// ToDo: Cost/benefit - if this is used frequently may be cheaper to store the faces rather than
				// reconstruct them - however, this will increase the cost of the constructors....
				
				// Algorithm: Construct a vector from the point to each face
				// Assuming the normal vector direction is pointing out, if the dot-product of the face normal 
				// vector and the point->face vector is positive, they are both in the same direction, and
				// thus the point is 'inside' for that face.
				// Repeat for all faces, and if 'inside' all of them, the point is inside the polygon
				
				double dotProd;
				euc::EuclideanVector<T,3> pointVector;
					
				// Build 5 Faces from polygons as planes
				// Ordering for each should be clockwise from inside, anti-clockwise from outside
				// This assumes that the vertices for each face are coplanar, but this is a reasonable assumption
				// since if they are not, then this is not really a triprism (since a Quadrilateral non coplanar = two triangular faces...)
				euc::EuclideanPlane3D<T> facePlanes[5] = 
				{
					euc::EuclideanPlane3D<T>(this->top.vertices[0],    this->top.vertices[1],    this->top.vertices[2]),
					euc::EuclideanPlane3D<T>(this->bottom.vertices[0], this->bottom.vertices[1], this->bottom.vertices[2]),
					euc::EuclideanPlane3D<T>(this->top.vertices[0],    this->top.vertices[2],    this->bottom.vertices[2]),
					euc::EuclideanPlane3D<T>(this->top.vertices[2],    this->top.vertices[1],    this->bottom.vertices[1]),
					euc::EuclideanPlane3D<T>(this->top.vertices[1],    this->top.vertices[0],    this->bottom.vertices[0])
				};

				// // Ensure each face plane normal points outwards:
				// for (int i=0; i<5; i++) {
				// 	dotProd = facePlanes[i].getNormal().dotProduct(facePlanes[i].p1 - this->computeCentroid());
				// 	// dotProd = facePlanes[i].normal.dotProduct(facePlanes[i].p1 - this->centroid);
				// 	if (dotProd < T(0)) {
				// 		facePlanes[i].reverseVertexOrdering();
				// 	}
				// }
					 
				// (1) Test against each face
				for(int i = 0; i < 5; i++) {
					// ToDo: This is presuming that all type 'P' base objects have a vertices data structure
					// Could make it so Polygon3D has a pointer to a vertices array, and then leave the 
					// storage implementation to the subclasses and updating the pointer to it.
					
					// (2a) Create a vector to the first point on the face (same as plane p1)
					pointVector = point - facePlanes[i].p1;
					
					// (2b) Are we on a face? If so this counts as inside, but the vector will be perpendicular
					// to the normal/parallel to the plane
					if(facePlanes[i].isVectorParallelInPlane(pointVector, point)) {
						return true;
					}
					
					// (2c) Is the point vector and the normal vector facing the same direction?
					// If not, this point is not inside the polyhedron
					dotProd = facePlanes[i].getNormal().dotProduct(pointVector);
					// dotProd = facePlanes[i].normal.dotProduct(pointVector);
					if(dotProd > T(0)) {
						return false;
					}
				}
				
				// If we made it this far then checks were passed for all faces - therefore we are in the shape
				return true;
			}

			template <class T>
			T TriPrism<T>::getVolume() {
				if (!this->volumeComputed) {
					this->volume = this->computeVolume();
					this->volumeComputed = true;
				}
				return this->volume;
			}

			template <class T>
			euc::EuclideanPoint<T,3> TriPrism<T>::getCentroid() {
				if (!this->centroidComputed) {
					this->centroid = this->computeCentroid();
					this->centroidComputed = true;
				}
				return this->centroid;
			}

			template <class T>
			T TriPrism<T>::computeVolume() {
				T height = (this->top.getCentroid() - this->bottom.getCentroid()).length();
				T volume = height * this->bottom.getArea();
				return volume;
			}
			
			template <class T>
			euc::EuclideanPoint<T,3> TriPrism<T>::computeCentroid() {
				// https://en.wikipedia.org/wiki/Centroid#Of_a_tetrahedron_and_n-dimensional_simplex
				return T(1)/T(7) * (this->top.vertices[0]    + this->top.vertices[1]    + this->top.vertices[2] + 
									this->bottom.vertices[0] + this->bottom.vertices[1] + this->bottom.vertices[2]);
			}
			
			template <class T>
			inline bool TriPrism<T>::isPointOnEdge(const euc::EuclideanPoint<T,3>& point) {
				for (int v1=0; v1<3; v1++) {
					const int v2=(v1+1)%3;
					if (isPointOnLine(this->top.vertices[v1], this->top.vertices[v2], point)) {
						return true;
					}
					if (isPointOnLine(this->bottom.vertices[v1], this->bottom.vertices[v2], point)) {
						return true;
					}
				}
				
				return false;
			}
			
			template <class T>
			inline bool TriPrism<T>::isPointOnVertex(const euc::EuclideanPoint<T,3>& point) {
				for (int i=0; i<3; i++) {
					if ( (this->top.vertices[i] == point) || (this->bottom.vertices[i] == point) ) {
						return true;
					}
				}
				return false;
			}
		}
	}
}

#endif
