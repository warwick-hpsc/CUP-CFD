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

namespace cupcfd
{
	namespace geometry
	{
		namespace shapes
		{
			// === Constructors/Deconstructors ===

			template <class T>
			Hexahedron<T>::Hexahedron(const cupcfd::geometry::euclidean::EuclideanPoint<T,3>& tlf,
									  const cupcfd::geometry::euclidean::EuclideanPoint<T,3>& trf,
									  const cupcfd::geometry::euclidean::EuclideanPoint<T,3>& blf,
									  const cupcfd::geometry::euclidean::EuclideanPoint<T,3>& brf,
									  const cupcfd::geometry::euclidean::EuclideanPoint<T,3>& tlb,
									  const cupcfd::geometry::euclidean::EuclideanPoint<T,3>& trb,
									  const cupcfd::geometry::euclidean::EuclideanPoint<T,3>& blb,
									  const cupcfd::geometry::euclidean::EuclideanPoint<T,3>& brb)
			: tlf(tlf), trf(trf), blf(blf), brf(brf),
			  tlb(tlb), trb(trb), blb(blb), brb(brb)
			{
				this->nEdges = 12;
				this->nVertices = 8;
				this->nFaces = 6;
			}

			template <class T>
			Hexahedron<T>::~Hexahedron()
			{

			}

			// === Static Methods ===


			// === Concrete Methods ===

			template<class T>
			inline bool Hexahedron<T>::isPointInside(const cupcfd::geometry::euclidean::EuclideanPoint<T,3>& point)
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
				
				double dotProd;
				cupcfd::geometry::euclidean::EuclideanVector<T,3> pointVector;
					
				// Build 6 Faces from polygons as planes
				// Ordering for each should be clockwise from inside, anti-clockwise from outside
				// This assumes that the vertices for each face are coplanar, but this is a reasonable assumption
				// since if they are not, then this is not really a hexahedral (since a Quadrilateral non coplanar = two triangular faces...)
				cupcfd::geometry::euclidean::EuclideanPlane3D<T> facePlanes[6] = 
				{
					cupcfd::geometry::euclidean::EuclideanPlane3D<T>(tlf, trf, brf),
					cupcfd::geometry::euclidean::EuclideanPlane3D<T>(trf, trb, brb),
					cupcfd::geometry::euclidean::EuclideanPlane3D<T>(trb, tlb, blb),
					cupcfd::geometry::euclidean::EuclideanPlane3D<T>(tlb, tlf, blf),
					cupcfd::geometry::euclidean::EuclideanPlane3D<T>(tlf, tlb, trb),
					cupcfd::geometry::euclidean::EuclideanPlane3D<T>(blf, brf, brb)
				};	
					
				cupcfd::geometry::shapes::Quadrilateral3D<T> faces[6] = 
				{
					cupcfd::geometry::shapes::Quadrilateral3D<T>(tlf, trf, brf, blf),
					cupcfd::geometry::shapes::Quadrilateral3D<T>(trf, trb, brb, brf),
					cupcfd::geometry::shapes::Quadrilateral3D<T>(trb, tlb, blb, brb),
					cupcfd::geometry::shapes::Quadrilateral3D<T>(tlb, tlf, blf, blb),
					cupcfd::geometry::shapes::Quadrilateral3D<T>(tlf, tlb, trb, trf),
					cupcfd::geometry::shapes::Quadrilateral3D<T>(blf, brf, brb, blb)
				};	
					
							
				// ToDo: We might not need both the planes AND the face objects.
							
				// (1) Test against each face
				for(int i = 0; i < 6; i++)
				{
					// ToDo: This is presuming that all type 'P' base objects have a vertices data structure
					// Could make it so Polygon3D has a pointer to a vertices array, and then leave the 
					// storage implementation to the subclasses and updating the pointer to it.
					
					// (2a) Create a vector to the first point on the face (same as plane p1)
					pointVector = facePlanes[i].p1 - point;
					
					// (2b) Are we on a face? If so this counts as inside, but the vector will be perpendicular
					// to the normal/parallel to the plane
					if(faces[i].isPointInside(point))
					{
						return true;
					}
					
					// (2c) Is the point vector and the normal vector facing the same direction?
					// If not, this point is not inside the polyhedron
					dotProd = pointVector.dotProduct(facePlanes[i].getNormal());

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
			
			template <class T>
			inline bool Hexahedron<T>::isPointOnEdge(const cupcfd::geometry::euclidean::EuclideanPoint<T,3>& point)
			{							
				// ToDo: This can likely be condensed to a generic polygon method, using a getEdge method	  
									  
				// Recreate list of edges
				std::vector<cupcfd::geometry::euclidean::EuclideanVector<T,3>> edgeVectors;
				
				bool tests[12];
				tests[0] = isPointOnLine(tlf, trf, point);
				tests[1] = isPointOnLine(tlf, tlb, point);
				tests[2] = isPointOnLine(trb, tlb, point);
				tests[3] = isPointOnLine(trb, trf, point);
				tests[4] = isPointOnLine(blf, brf, point);
				tests[5] = isPointOnLine(blf, blb, point);
				tests[6] = isPointOnLine(brb, blb, point);
				tests[7] = isPointOnLine(brb, brf, point);
				tests[8] = isPointOnLine(tlf, blf, point);
				tests[9] = isPointOnLine(trf, brf, point);
				tests[10] = isPointOnLine(tlb, blb, point);
				tests[11] = isPointOnLine(trb, brb, point);
				
				// Test if the point lies on each edge
				for(int i = 0; i < 12; i++)
				{
					if(tests[i])
					{
						return true;
					}
				}
				
				return false;
			}
			
			template <class T>
			inline bool Hexahedron<T>::isPointOnVertex(const cupcfd::geometry::euclidean::EuclideanPoint<T,3>& point)
			{
				return (tlf == point) || (trf == point) || (tlb == point) || (trb == point) ||
					   (blf == point) || (brf == point) || (blb == point) || (brb == point);
			}

			template<class T>
			T Hexahedron<T>::computeVolume()
			{
				// General Case means we cannot assume it is a regular hexahedron
				// Divide the hexahedron into tetrahedrons (triangular pyramids), and compute the volumes of
				// those, then sum
				
				// 5 Tetrahedrons
				Tetrahedron<T> t1(trb, brb, brf, blb); 
				Tetrahedron<T> t2(tlf, blf, blb, brf);
				Tetrahedron<T> t3(trb, tlb, blb, tlf);
				Tetrahedron<T> t4(brf, trf, tlf, trb);
				Tetrahedron<T> t5(trb, tlf, blb, brf);
				
				return t1.computeVolume() + t2.computeVolume() + t3.computeVolume() + t4.computeVolume() + t5.computeVolume();
			}
			
			template<class T>
			cupcfd::geometry::euclidean::EuclideanPoint<T,3> Hexahedron<T>::computeCentroid()
			{
				// ToDo: Is this correct for hexahedrons, or just cubes?.....
				return ((T(1)/T(8)) * (tlf + trf + blf + brf + tlb + trb + blb + brb));
			}
		}
	}
}

#endif
