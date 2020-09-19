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
 * Header Level Definitions for the Quadrilateral3D class
 */

#ifndef CUPCFD_GEOMETRY_SHAPES_QUADRILATERAL_3D_IPP_H
#define CUPCFD_GEOMETRY_SHAPES_QUADRILATERAL_3D_IPP_H

#include "Triangle3D.h"
#include "Quadrilateral3D.h"
#include "EuclideanPlane3D.h"

#include <iostream>

namespace cupcfd
{
	namespace geometry
	{
		namespace shapes
		{
			// === Constructors/Deconstructors ===

			template <class T>
			Quadrilateral3D<T>::Quadrilateral3D(const cupcfd::geometry::euclidean::EuclideanPoint<T,3>& a,
											    const cupcfd::geometry::euclidean::EuclideanPoint<T,3>& b,
											    const cupcfd::geometry::euclidean::EuclideanPoint<T,3>& c,
											    const cupcfd::geometry::euclidean::EuclideanPoint<T,3>& d)
			{
				this->nVertices = 4;
				this->nEdges = 4;

				this->vertices[0] = a;
				this->vertices[1] = b;
				this->vertices[2] = c;
				this->vertices[3] = d;
			}

			template <class T>
			Quadrilateral3D<T>::Quadrilateral3D(const Quadrilateral3D<T>& source)
			{
				*this = source;
			}

			template <class T>
			Quadrilateral3D<T>::~Quadrilateral3D()
			{

			}

			// === Overloaded Inherited Methods ===
			
			template <class T>
			inline void Quadrilateral3D<T>::operator=(const Quadrilateral3D<T>& source)
			{
				this->nVertices = source.nVertices;
				this->nEdges = source.nEdges;
				this->vertices[0] = source.vertices[0];
				this->vertices[1] = source.vertices[1];
				this->vertices[2] = source.vertices[2];
				this->vertices[3] = source.vertices[3];
			}
			
			template <class T>
			Quadrilateral3D<T> * Quadrilateral3D<T>::clone()
			{
				return new Quadrilateral3D(*this);
			}

			// === Static Methods ===

			template <class T>
			T Quadrilateral3D<T>::triangularAreaSum(const cupcfd::geometry::euclidean::EuclideanPoint<T,3>& a,
												    const cupcfd::geometry::euclidean::EuclideanPoint<T,3>& b,
													const cupcfd::geometry::euclidean::EuclideanPoint<T,3>& c,
													const cupcfd::geometry::euclidean::EuclideanPoint<T,3>& d)
			{
				// Decompose the Quadrilateral into Non-Overlapping Triangles
				Triangle3D<T> tri1 = Triangle3D<T>(a, b, c);
				Triangle3D<T> tri2 = Triangle3D<T>(a, d, c);

				// Compute the area of each triangle and sum them
				T area = tri1.computeArea();
				area = area + tri2.computeArea();

				return area;
			}

			template <class T>
			cupcfd::geometry::euclidean::EuclideanPoint<T,3> Quadrilateral3D<T>::computeCentroidBiMedianIntersection(
																 const cupcfd::geometry::euclidean::EuclideanPoint<T,3>& a,
																 const cupcfd::geometry::euclidean::EuclideanPoint<T,3>& b,
																 const cupcfd::geometry::euclidean::EuclideanPoint<T,3>& c,
																 const cupcfd::geometry::euclidean::EuclideanPoint<T,3>& d)
			{
				// https://en.wikipedia.org/wiki/Quadrilateral#Bimedians
				// ToDo: Possible alternate function: Is just taking the arithmetic mean of each component a viable option?
				// Would be much cheaper
				
				// Compute the bimedians - halfway points along the four edges
				cupcfd::geometry::euclidean::EuclideanVector<T,3> ab = b-a;
				cupcfd::geometry::euclidean::EuclideanVector<T,3> bc = c-b;
				cupcfd::geometry::euclidean::EuclideanVector<T,3> cd = d-c;
				cupcfd::geometry::euclidean::EuclideanVector<T,3> da = a-d;
				
				cupcfd::geometry::euclidean::EuclideanPoint<T,3> abBiMedian = a + (ab/T(2));
				cupcfd::geometry::euclidean::EuclideanPoint<T,3> bcBiMedian = b + (bc/T(2));
				cupcfd::geometry::euclidean::EuclideanPoint<T,3> cdBiMedian = c + (cd/T(2));
				cupcfd::geometry::euclidean::EuclideanPoint<T,3> daBiMedian = d + (da/T(2));
				
				// Compute the intersection of the bimedians on opposite sides - this is the centroid
				cupcfd::geometry::euclidean::EuclideanPoint<T,3> intersectPoint;
				cupcfd::error::eCodes status = computeVectorRangeIntersection(abBiMedian, cdBiMedian, bcBiMedian, daBiMedian, intersectPoint);
				if (status != cupcfd::error::E_SUCCESS) {
					throw(std::runtime_error("Quadrilateral3D: computeCentroidBiMedianIntersection: computeVectorRangeIntersection() failed"));
				}
				return intersectPoint;
			}
							
			template <class T>									 
			inline bool Quadrilateral3D<T>::isPointInsideCentroid(const cupcfd::geometry::euclidean::EuclideanPoint<T,3>& a,
														     	  const cupcfd::geometry::euclidean::EuclideanPoint<T,3>& b,
															 	  const cupcfd::geometry::euclidean::EuclideanPoint<T,3>& c,
															 	  const cupcfd::geometry::euclidean::EuclideanPoint<T,3>& d,
															 	  const cupcfd::geometry::euclidean::EuclideanPoint<T,3>& p)
			{
				// Does the point lie on the same plane as the Quadrilateral Points?
				// This assumes that d is coplanar with a,b,c
				cupcfd::geometry::euclidean::EuclideanPlane3D<T> plane(a,b,c);
				
				if(!(plane.isPointOnPlane(p)))
				{
					return false;
				}
				
				// Does the point equal one of the points - if so it counts as inside
				if(p == a || p == b || p == c || p == d)
				{
					return true;
				}
				
				// If the point lies on one of the edges it counts as inside
				if(isPointOnLine(a, b, p) || isPointOnLine(b, c, p) || isPointOnLine(c, d, p) || isPointOnLine(a, d, p))
				{
					return true;
				}
				
				// Compute the centroid
				cupcfd::geometry::euclidean::EuclideanPoint<T,3> centroid = Quadrilateral3D<T>::computeCentroidBiMedianIntersection(a, b, c, d);
				
				// Test the intersection of the ray ranging from the point to the centroid
				// if it intersects any of the faces, then the point p lies on the opposite side
				// of an edge to the centroid, and so must be outside
				
				bool abIntersect, bcIntersect, cdIntersect, adIntersect;

				abIntersect = cupcfd::geometry::euclidean::isVectorRangeIntersection(p, centroid, a, b);
				bcIntersect = cupcfd::geometry::euclidean::isVectorRangeIntersection(p, centroid, b, c);
				cdIntersect = cupcfd::geometry::euclidean::isVectorRangeIntersection(p, centroid, c, d);
				adIntersect = cupcfd::geometry::euclidean::isVectorRangeIntersection(p, centroid, a, d);
				
				if(abIntersect || bcIntersect || cdIntersect || adIntersect)
				{
					// One of the edges was intersected, so must be outside
					return false;
				}
				
				// No intersections detected
				return true;
			}
															 												 
			template <class T>
			T Quadrilateral3D<T>::validQuadrilateralPoints(cupcfd::geometry::euclidean::EuclideanPoint<T,3>& a,
														    cupcfd::geometry::euclidean::EuclideanPoint<T,3>& b,
														    cupcfd::geometry::euclidean::EuclideanPoint<T,3>& c,
														    cupcfd::geometry::euclidean::EuclideanPoint<T,3>& d)
			{			
				// Check 1: Do any of the vectors ab, bc, cd and da intersect with one another?
				// If so this is not a valid ordering of points for the quadrilateral definition (e.g, hourglass vs square)
				
				// Check by creating two planes now we know the point orderings are correct, and see if those two
				// triangles are coplanar
				
				return false;
			}
																								 
			// === Concrete Methods ===

			template <class T>
			inline bool Quadrilateral3D<T>::isPointInside(const cupcfd::geometry::euclidean::EuclideanPoint<T,3>& point)
			{
				// Pass through to the static method
				return Quadrilateral3D<T>::isPointInsideCentroid(this->vertices[0], this->vertices[1], this->vertices[2], this->vertices[3], point);
			}

			template <class T>
			T Quadrilateral3D<T>::computeArea()
			{
				// Pass through to the static method
				return this->triangularAreaSum(this->vertices[0], this->vertices[1], this->vertices[2], this->vertices[3]);
			}
			
			template <class T>
			cupcfd::geometry::euclidean::EuclideanVector<T,3> Quadrilateral3D<T>::computeNormal()
			{
				// Pass through to the static method
				return cupcfd::geometry::euclidean::EuclideanPlane3D<T>::normal(this->vertices[0], this->vertices[1], this->vertices[2]);
			}
			
			template <class T>
			inline cupcfd::geometry::euclidean::EuclideanPoint<T,3> Quadrilateral3D<T>::computeCentroid()
			{
				// Pass through to the static method
				return Quadrilateral3D<T>::computeCentroidBiMedianIntersection(this->vertices[0], this->vertices[1], this->vertices[2], this->vertices[3]);
			}
		}
	}
}

#endif
