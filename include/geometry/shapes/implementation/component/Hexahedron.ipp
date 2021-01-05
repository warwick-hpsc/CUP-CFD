/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Header Level Definitions for the Hexahedron class
 *
 */

#ifndef CUPCFD_GEOMETRY_SHAPES_HEXAHEDRON_IPP_H
#define CUPCFD_GEOMETRY_SHAPES_HEXAHEDRON_IPP_H

#include "Tetrahedron.h"
#include <vector>
#include "Quadrilateral3D.h"

namespace euc = cupcfd::geometry::euclidean;
namespace shapes = cupcfd::geometry::shapes;

namespace cupcfd
{
	namespace geometry
	{
		namespace shapes
		{
			// === Constructors/Deconstructors ===

			template <class T>
			Hexahedron<T>::Hexahedron(const euc::EuclideanPoint<T,3>& tlf,
									const euc::EuclideanPoint<T,3>& trf,
									const euc::EuclideanPoint<T,3>& blf,
									const euc::EuclideanPoint<T,3>& brf,
									const euc::EuclideanPoint<T,3>& tlb,
									const euc::EuclideanPoint<T,3>& trb,
									const euc::EuclideanPoint<T,3>& blb,
									const euc::EuclideanPoint<T,3>& brb)
			: tlf(tlf), trf(trf), blf(blf), brf(brf),
			  tlb(tlb), trb(trb), blb(blb), brb(brb)
			 {
			 	this->f1 = cupcfd::geometry::shapes::Quadrilateral3D<T>(tlf, trf, brf, blf);
			 	this->f2 = cupcfd::geometry::shapes::Quadrilateral3D<T>(tlb, trb, brb, blb);
			 	this->f3 = cupcfd::geometry::shapes::Quadrilateral3D<T>(tlf, trf, trb, tlb);
			 	this->f4 = cupcfd::geometry::shapes::Quadrilateral3D<T>(blf, brf, brb, blb);
			 	this->f5 = cupcfd::geometry::shapes::Quadrilateral3D<T>(tlf, tlb, blb, blf);
			 	this->f6 = cupcfd::geometry::shapes::Quadrilateral3D<T>(trf, trb, brb, brf);

			}

			template <class T>
			Hexahedron<T>::Hexahedron(const shapes::Quadrilateral3D<T>& f1,
									const shapes::Quadrilateral3D<T>& f2,
									const shapes::Quadrilateral3D<T>& f3,
									const shapes::Quadrilateral3D<T>& f4,
									const shapes::Quadrilateral3D<T>& f5,
									const shapes::Quadrilateral3D<T>& f6)
			: f1(f1), f2(f2), f3(f3), f4(f4), f5(f5), f6(f6)
			{
				// To ensure all face normals are facing away from centroid, 
				// compute a crude centroid as arithmetic mean of quad centroids:
			}

			template <class T>
			Hexahedron<T>::~Hexahedron() {
			}

			// === Concrete Methods ===

			template <class T>
			T Hexahedron<T>::getVolume() {
				if (!this->volumeComputed) {
					this->volume = this->computeVolume();
					this->volumeComputed = true;
				}
				return this->volume;
			}

			template <class T>
			euc::EuclideanPoint<T,3> Hexahedron<T>::getCentroid() {
				if (!this->centroidComputed) {
					this->centroid = this->computeCentroid();
					this->centroidComputed = true;
				}
				return this->centroid;
			}

			template<class T>
			T Hexahedron<T>::computeVolume() {
				// General Case means we cannot assume it is a regular hexahedron.
				// Divide the hexahedron into 5 tetrahedrons, and sum their volumes.
				
				// 5 Tetrahedrons
				Tetrahedron<T> t1(trb, Triangle3D<T>(brb, brf, blb));
				Tetrahedron<T> t2(tlf, Triangle3D<T>(blf, blb, brf));
				Tetrahedron<T> t3(trb, Triangle3D<T>(tlb, blb, tlf));
				Tetrahedron<T> t4(brf, Triangle3D<T>(trf, tlf, trb));
				Tetrahedron<T> t5(trb, Triangle3D<T>(tlf, blb, brf));
				return t1.getVolume() + t2.getVolume() + t3.getVolume() + t4.getVolume() + t5.getVolume();
			}
			
			template<class T>
			euc::EuclideanPoint<T,3> Hexahedron<T>::computeCentroid() {
				// ToDo: Is this correct for hexahedrons, or just cubes?.....
				// return ((T(1)/T(8)) * (tlf + trf + blf + brf + tlb + trb + blb + brb));

				// Update: apparently, centroid of a polyhedron is volume-weighted average of its non-overlapping tetrahedrons.
				Tetrahedron<T> t1(trb, Triangle3D<T>(brb, brf, blb));
				Tetrahedron<T> t2(tlf, Triangle3D<T>(blf, blb, brf));
				Tetrahedron<T> t3(trb, Triangle3D<T>(tlb, blb, tlf));
				Tetrahedron<T> t4(brf, Triangle3D<T>(trf, tlf, trb));
				Tetrahedron<T> t5(trb, Triangle3D<T>(tlf, blb, brf));

				T totalVolume = t1.getVolume()+t2.getVolume()+t3.getVolume()+t4.getVolume()+t5.getVolume();

				return (	  (t1.getVolume() * t1.getCentroid())
							+ (t2.getVolume() * t2.getCentroid()) 
							+ (t3.getVolume() * t3.getCentroid())
							+ (t4.getVolume() * t4.getCentroid())
							+ (t5.getVolume() * t5.getCentroid()) 	) 
						/ totalVolume;
 			}

			template<class T>
			inline bool Hexahedron<T>::isPointInside(const euc::EuclideanPoint<T,3>& point) {
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
				
				double dotProd;
				euc::EuclideanVector<T,3> pointVector;
					
				// Build 6 Faces from polygons as planes
				// Ordering for each should be clockwise from inside, anti-clockwise from outside
				// This assumes that the vertices for each face are coplanar, but this is a reasonable assumption
				// since if they are not, then this is not really a hexahedral (since a Quadrilateral non coplanar = two triangular faces...)
				euc::EuclideanPlane3D<T> facePlanes[6] = 
				{
					euc::EuclideanPlane3D<T>(tlf, trf, brf), // front plane
					euc::EuclideanPlane3D<T>(trf, trb, brb), // right plane
					euc::EuclideanPlane3D<T>(trb, tlb, blb), // back plane
					euc::EuclideanPlane3D<T>(tlb, tlf, blf), // left plane
					euc::EuclideanPlane3D<T>(tlf, tlb, trb), // top plane
					euc::EuclideanPlane3D<T>(blf, brf, brb)  // bottom plane
				};	
					
				cupcfd::geometry::shapes::Quadrilateral3D<T> faces[6] = 
				{
					cupcfd::geometry::shapes::Quadrilateral3D<T>(tlf, trf, brf, blf), // front face
					cupcfd::geometry::shapes::Quadrilateral3D<T>(trf, trb, brb, brf), // right face
					cupcfd::geometry::shapes::Quadrilateral3D<T>(trb, tlb, blb, brb), // back face
					cupcfd::geometry::shapes::Quadrilateral3D<T>(tlb, tlf, blf, blb), // left face
					cupcfd::geometry::shapes::Quadrilateral3D<T>(tlf, tlb, trb, trf), // top face
					cupcfd::geometry::shapes::Quadrilateral3D<T>(blf, brf, brb, blb)  // bottom face
				};	


				// ToDo: We might not need both the planes AND the face objects.
							
				// (1) Test against each face
				for(int i = 0; i < 6; i++) {
					// ToDo: This is presuming that all type 'P' base objects have a vertices data structure
					// Could make it so Polygon3D has a pointer to a vertices array, and then leave the 
					// storage implementation to the subclasses and updating the pointer to it.
					
					// (2a) Create a vector to the first point on the face (same as plane p1)
					pointVector = facePlanes[i].p1 - point;
					
					// (2b) Are we on a face? If so this counts as inside, but the vector will be perpendicular
					// to the normal/parallel to the plane
					if(faces[i].isPointInside(point)) {
						return true;
					}
					
					// (2c) Is the point vector and the normal vector facing the same direction?
					// If not, this point is not inside the polyhedron
					dotProd = pointVector.dotProduct(facePlanes[i].getNormal());
					if(dotProd < 0) {
						// Negative means different directions - i.e. the normal vector pointing out is not the same the same direction
						// as the point vector pointing in. Therefore point not in shape
						return false;
					}
				}
				
				// If we made it this far checks were passed for all faces - therefore we are in the shape
				return true;
			}
			
			template <class T>
			inline bool Hexahedron<T>::isPointOnEdge(const euc::EuclideanPoint<T,3>& point) {							
				// ToDo: This can likely be condensed to a generic polygon method, using a getEdge method	  

				// Recreate list of edges
				std::vector<euc::EuclideanVector<T,3>> edgeVectors;
				
				bool tests[12];
				tests[0]  = isPointOnLine(tlf, trf, point);
				tests[1]  = isPointOnLine(tlf, tlb, point);
				tests[2]  = isPointOnLine(trb, tlb, point);
				tests[3]  = isPointOnLine(trb, trf, point);
				tests[4]  = isPointOnLine(blf, brf, point);
				tests[5]  = isPointOnLine(blf, blb, point);
				tests[6]  = isPointOnLine(brb, blb, point);
				tests[7]  = isPointOnLine(brb, brf, point);
				tests[8]  = isPointOnLine(tlf, blf, point);
				tests[9]  = isPointOnLine(trf, brf, point);
				tests[10] = isPointOnLine(tlb, blb, point);
				tests[11] = isPointOnLine(trb, brb, point);
				
				// Test if the point lies on each edge
				for(int i = 0; i < 12; i++) {
					if(tests[i]) {
						return true;
					}
				}
				
				return false;
			}
			
			template <class T>
			inline bool Hexahedron<T>::isPointOnVertex(const euc::EuclideanPoint<T,3>& point) {
				return (tlf == point) || (trf == point) || (tlb == point) || (trb == point) ||
					   (blf == point) || (brf == point) || (blb == point) || (brb == point);
			}
		}
	}
}

#endif
