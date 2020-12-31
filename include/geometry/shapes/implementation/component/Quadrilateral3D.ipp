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

namespace euc = cupcfd::geometry::euclidean;

namespace cupcfd
{
	namespace geometry
	{
		namespace shapes
		{
			// === Constructors/Deconstructors ===

			template <class T>
			Quadrilateral3D<T>::Quadrilateral3D() {
				this->vertices[0] = T(0.0);
				this->vertices[1] = T(0.0);
				this->vertices[2] = T(0.0);
				this->vertices[3] = T(0.0);
				
				this->vertices[2].cmp[0] = 1.0;
				this->vertices[3].cmp[0] = 1.0;
				// this->vertices[3].cmp[1] = 1.0;
				// this->vertices[4].cmp[1] = 1.0;
				this->vertices[1].cmp[1] = 1.0;
				this->vertices[2].cmp[1] = 1.0;

				/*
				euc::EuclideanPoint<T,3> a(T(0.0), T(0.0), T(0.0));
				euc::EuclideanPoint<T,3> b(T(0.0), T(1.0), T(0.0));
				euc::EuclideanPoint<T,3> c(T(1.0), T(1.0), T(0.0));
				euc::EuclideanPoint<T,3> d(T(1.0), T(0.0), T(0.0));

				this->vertices[0] = a;
				this->vertices[1] = b;
				this->vertices[2] = c;
				this->vertices[3] = d;
				*/

				// this->centroid = euc::EuclideanPoint<T,3>(T(0.4), T(0.5), T(0.5));
				// this->normal = euc::EuclideanPlane3D<T>::calculateNormal(a, b, c);
				// this->area = T(1);
			}

			template <class T>
			Quadrilateral3D<T>::Quadrilateral3D(const euc::EuclideanPoint<T,3>& a,
											    const euc::EuclideanPoint<T,3>& b,
											    const euc::EuclideanPoint<T,3>& c,
											    const euc::EuclideanPoint<T,3>& d) {
				this->vertices[0] = a;
				this->vertices[1] = b;
				this->vertices[2] = c;
				this->vertices[3] = d;

				// this->centroid = Quadrilateral3D<T>::computeCentroidBiMedianIntersection(a, b, c, d);
				// this->normal = euc::EuclideanPlane3D<T>::calculateNormal(a, b, c);
				// this->area = Quadrilateral3D<T>::triangularAreaSum(a, b, c, d);
			}

			template <class T>
			Quadrilateral3D<T>::Quadrilateral3D(const Quadrilateral3D<T>& source) {
				*this = source;
			}

			template <class T>
			Quadrilateral3D<T>::~Quadrilateral3D() {
			}

			// === Overloaded Inherited Methods ===
			
			template <class T>
			inline void Quadrilateral3D<T>::operator=(const Quadrilateral3D<T>& source) {
				this->vertices[0] = source.vertices[0];
				this->vertices[1] = source.vertices[1];
				this->vertices[2] = source.vertices[2];
				this->vertices[3] = source.vertices[3];
				// this->area        = source.area;
				// this->normal      = source.normal;
				// this->centroid    = source.centroid;
			}
			
			template <class T>
			Quadrilateral3D<T> * Quadrilateral3D<T>::clone() {
				return new Quadrilateral3D(*this);
			}

			// === Static Methods ===

			template <class T>
			T Quadrilateral3D<T>::triangularAreaSum(const euc::EuclideanPoint<T,3>& a,
												    const euc::EuclideanPoint<T,3>& b,
													const euc::EuclideanPoint<T,3>& c,
													const euc::EuclideanPoint<T,3>& d) {
				// Decompose the Quadrilateral into Non-Overlapping Triangles
				Triangle3D<T> tri1 = Triangle3D<T>(a, b, c);
				Triangle3D<T> tri2 = Triangle3D<T>(a, d, c);

				// Compute the area of each triangle and sum them
				T area = tri1.computeArea();
				area = area + tri2.computeArea();

				return area;
			}

			// template <class T>
			// euc::EuclideanPoint<T,3> Quadrilateral3D<T>::computeCentroidBiMedianIntersection(
			// 													const euc::EuclideanPoint<T,3>& a,
			// 													const euc::EuclideanPoint<T,3>& b,
			// 													const euc::EuclideanPoint<T,3>& c,
			// 													const euc::EuclideanPoint<T,3>& d) {
			// 	// https://en.wikipedia.org/wiki/Quadrilateral#Bimedians
			// 	// ToDo: Possible alternate function: Is just taking the arithmetic mean of each component a viable option?
			// 	// Would be much cheaper
				
			// 	// Compute the bimedians - halfway points along the four edges
			// 	euc::EuclideanVector<T,3> ab = b-a;
			// 	euc::EuclideanVector<T,3> bc = c-b;
			// 	euc::EuclideanVector<T,3> cd = d-c;
			// 	euc::EuclideanVector<T,3> da = a-d;
				
			// 	euc::EuclideanPoint<T,3> abMidpoint = a + (ab/T(2));
			// 	euc::EuclideanPoint<T,3> bcMidpoint = b + (bc/T(2));
			// 	euc::EuclideanPoint<T,3> cdMidpoint = c + (cd/T(2));
			// 	euc::EuclideanPoint<T,3> daMidpoint = d + (da/T(2));
				
			// 	// Compute the intersection of the midpoints on opposite sides - this is the centroid
			// 	euc::EuclideanPoint<T,3> intersectPoint;
			// 	cupcfd::error::eCodes status = computeVectorRangeIntersection(abMidpoint, cdMidpoint, bcMidpoint, daMidpoint, intersectPoint);
			// 	if (status != cupcfd::error::E_SUCCESS) {
			// 		HARD_CHECK_ECODE(status)
			// 	}
			// 	return intersectPoint;
			// }
							
			template <class T>									 
			inline bool Quadrilateral3D<T>::isPointInsideCentroid(const euc::EuclideanPoint<T,3>& a,
														     	  const euc::EuclideanPoint<T,3>& b,
															 	  const euc::EuclideanPoint<T,3>& c,
															 	  const euc::EuclideanPoint<T,3>& d,
															 	  const euc::EuclideanPoint<T,3>& p) {
				// Does the point lie on the same plane as the Quadrilateral Points?
				// This assumes that d is coplanar with a,b,c
				euc::EuclideanPlane3D<T> plane(a,b,c);
				
				if(!(plane.isPointOnPlane(p))) {
					return false;
				}
				
				// Does the point equal one of the points - if so it counts as inside
				if(p == a || p == b || p == c || p == d) {
					return true;
				}
				
				// If the point lies on one of the edges it counts as inside
				if(isPointOnLine(a, b, p) || isPointOnLine(b, c, p) || isPointOnLine(c, d, p) || isPointOnLine(a, d, p)) {
					return true;
				}
				
				// Compute the centroid
				// euc::EuclideanPoint<T,3> centroid = Quadrilateral3D<T>::computeCentroidBiMedianIntersection(a, b, c, d);
				Quadrilateral3D<T> quad(a,b,c,d);
				euc::EuclideanPoint<T,3> centroid = quad.computeCentroid();
				
				// Test the intersection of the ray ranging from the point to the centroid
				// if it intersects any of the faces, then the point p lies on the opposite side
				// of an edge to the centroid, and so must be outside
				
				bool abIntersect, bcIntersect, cdIntersect, adIntersect;

				abIntersect = euc::isVectorRangeIntersection(p, centroid, a, b);
				bcIntersect = euc::isVectorRangeIntersection(p, centroid, b, c);
				cdIntersect = euc::isVectorRangeIntersection(p, centroid, c, d);
				adIntersect = euc::isVectorRangeIntersection(p, centroid, a, d);
				
				if(abIntersect || bcIntersect || cdIntersect || adIntersect) {
					// One of the edges was intersected, so must be outside
					return false;
				}
				
				// No intersections detected
				return true;
			}
															 												 
			// template <class T>
			// T Quadrilateral3D<T>::validQuadrilateralPoints(euc::EuclideanPoint<T,3>& a,
			// 											    euc::EuclideanPoint<T,3>& b,
			// 											    euc::EuclideanPoint<T,3>& c,
			// 											    euc::EuclideanPoint<T,3>& d)
			// {			
			// 	// Check 1: Do any of the vectors ab, bc, cd and da intersect with one another?
			// 	// If so this is not a valid ordering of points for the quadrilateral definition (e.g, hourglass vs square)
				
			// 	// Check by creating two planes now we know the point orderings are correct, and see if those two
			// 	// triangles are coplanar
				
			// 	return false;
			// }
																								 
			// === Concrete Methods ===

			template <class T>
			inline bool Quadrilateral3D<T>::isPointInside(const euc::EuclideanPoint<T,3>& point) {
				// Pass through to the static method
				return Quadrilateral3D<T>::isPointInsideCentroid(this->vertices[0], this->vertices[1], this->vertices[2], this->vertices[3], point);
			}

			template <class T>
			T Quadrilateral3D<T>::computeArea() {
				// Pass through to the static method
				return this->triangularAreaSum(this->vertices[0], this->vertices[1], this->vertices[2], this->vertices[3]);
			}
			
			template <class T>
			euc::EuclideanVector<T,3> Quadrilateral3D<T>::computeNormal() {
				// Pass through to the static method
				return euc::EuclideanPlane3D<T>::calculateNormal(this->vertices[0], this->vertices[1], this->vertices[2]);
			}
			
			template <class T>
			inline euc::EuclideanPoint<T,3> Quadrilateral3D<T>::computeCentroid() {
				// Pass through to the static method
				// return Quadrilateral3D<T>::computeCentroidBiMedianIntersection(this->vertices[0], this->vertices[1], this->vertices[2], this->vertices[3]);

				// computeCentroidBiMedianIntersection() is incredibly convoluted, and also wrong/broken.
				// Better method below:

				// https://en.wikipedia.org/wiki/Centroid#Of_a_polygon
				// Necessary assumption: the vertices forming this 3D polygon are co-planar.
				// Entering this process without knowing what plane the polygon lies on, so 
				// a little more work required. 
				// Process: calculate centroids of three projections (using Wiki method):
				//  1. XY plane
				//  2. YZ plane
				//  3. YZ plane
				// 'unwind' the projection to get the polygon centroid

				T area = this->computeArea();

				// 1. calculate centroid of XY projection
				euc::EuclideanPoint<T,3> cXY(T(0), T(0), T(0));
				for (uint i=0; i<4; i++) {
					uint i2=(i+1)%4;
					euc::EuclideanPoint<T,3> v1 = this->vertices[i];
					euc::EuclideanPoint<T,3> v2 = this->vertices[i2];
					cXY.cmp[0] += (v1.cmp[0] + v2.cmp[0]) * ( v1.cmp[0]*v2.cmp[1] - v2.cmp[0] * v1.cmp[1] );
					cXY.cmp[1] += (v1.cmp[1] + v2.cmp[1]) * ( v1.cmp[0]*v2.cmp[1] - v2.cmp[0] * v1.cmp[1] );
				}
				cXY *= T(1) / (T(6) * area);

				// 2. calculate centroid of XZ projection
				euc::EuclideanPoint<T,3> cXZ(T(0), T(0), T(0));
				for (uint i=0; i<4; i++) {
					uint i2=(i+1)%4;
					euc::EuclideanPoint<T,3> v1 = this->vertices[i];
					euc::EuclideanPoint<T,3> v2 = this->vertices[i2];
					cXZ.cmp[0] += (v1.cmp[0] + v2.cmp[0]) * ( v1.cmp[0]*v2.cmp[2] - v2.cmp[0] * v1.cmp[2] );
					cXZ.cmp[2] += (v1.cmp[2] + v2.cmp[2]) * ( v1.cmp[0]*v2.cmp[2] - v2.cmp[0] * v1.cmp[2] );
				}
				cXZ *= T(1) / (T(6) * area);

				// 3. calculate centroid of YZ projection
				euc::EuclideanPoint<T,3> cYZ(T(0), T(0), T(0));
				for (uint i=0; i<4; i++) {
					uint i2=(i+1)%4;
					euc::EuclideanPoint<T,3> v1 = this->vertices[i];
					euc::EuclideanPoint<T,3> v2 = this->vertices[i2];
					cYZ.cmp[1] += (v1.cmp[1] + v2.cmp[1]) * ( v1.cmp[1]*v2.cmp[2] - v2.cmp[1] * v1.cmp[2] );
					cYZ.cmp[2] += (v1.cmp[2] + v2.cmp[2]) * ( v1.cmp[1]*v2.cmp[2] - v2.cmp[1] * v1.cmp[2] );
				}
				cYZ *= T(1) / (T(6) * area);

				// 4. 'unwind' projections:
				euc::EuclideanPoint<T,3> centroid;
				for (uint i=0; i<3; i++) {
					if (cXY.cmp[i] != T(0)) {
						centroid.cmp[i] = cXY.cmp[i];
					}
					else if (cXZ.cmp[i] != T(0)) {
						centroid.cmp[i] = cXZ.cmp[i];
					}
					else {
						centroid.cmp[i] = cYZ.cmp[i];
					}
				}

				// std::cout << "vertices:" << std::endl;
				// for (uint i=0; i<4; i++) {
				// 	printf("  %.4f, %.4f, %.4f\n", this->vertices[i].cmp[0], this->vertices[i].cmp[1], this->vertices[i].cmp[2]);
				// }
				// std::cout << "cXY:" << std::endl;
				// // printf("  %.4f, %.4f, %.4f\n", cXY.cmp[0], cXY.cmp[1], cXY.cmp[2]);
				// printf("  %.4e, %.4e, %.4e\n", cXY.cmp[0], cXY.cmp[1], cXY.cmp[2]);
				// std::cout << "cXZ:" << std::endl;
				// // printf("  %.4f, %.4f, %.4f\n", cXZ.cmp[0], cXZ.cmp[1], cXZ.cmp[2]);
				// printf("  %.4e, %.4e, %.4e\n", cXZ.cmp[0], cXZ.cmp[1], cXZ.cmp[2]);
				// std::cout << "cYZ:" << std::endl;
				// printf("  %.4f, %.4f, %.4f\n", cYZ.cmp[0], cYZ.cmp[1], cYZ.cmp[2]);
				// // printf("  %.4e, %.4e, %.4e\n", cYZ.cmp[0], cYZ.cmp[1], cYZ.cmp[2]);
				// std::cout << "centroid:" << std::endl;
				// printf("  %.4f, %.4f, %.4f\n", centroid.cmp[0], centroid.cmp[1], centroid.cmp[2]);
				// // printf("  %.4e, %.4e, %.4e\n", cYZ.cmp[0], cYZ.cmp[1], cYZ.cmp[2]);

				// // ToDo: finish logic
				// HARD_CHECK_ECODE(cupcfd::error::E_ERROR)
				
				return centroid;
			}
		}
	}
}

#endif
