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

#ifndef CUPCFD_GEOMETRY_SHAPES_POLYGON3D_3D_IPP_H
#define CUPCFD_GEOMETRY_SHAPES_POLYGON3D_3D_IPP_H

#include "EuclideanPlane3D.h"
#include "EuclideanVector2D.h"
#include "EuclideanVector3D.h"
#include "Matrix.h"
#include "Triangle3D.h"
#include "Polygon2D.h"

namespace euc = cupcfd::geometry::euclidean;

namespace cupcfd
{
	namespace geometry
	{
		namespace shapes
		{
			template <class T, uint V>
			Polygon3D<T,V>::Polygon3D() {
			}

			template <class T, uint V>
			template<class...Args>
			Polygon3D<T,V>::Polygon3D(Args...v)
			: Polygon<Polygon3D<T,V>,T,3,V>::Polygon( (v)... )
			{
				static_assert(sizeof...(Args) == V, "Polygon constructor dimension does not match number of parameters");

				if (this->numVertices > 3) {
					if (!this->coplanar()) {
						HARD_CHECK_ECODE(cupcfd::error::E_GEOMETRY_LOGIC_ERROR);
					}

					if (!this->verifyNoEdgesIntersect()) {
						HARD_CHECK_ECODE(cupcfd::error::E_GEOMETRY_LOGIC_ERROR);
					}
				}
			}
			
			template <class T, uint V>
			Polygon3D<T,V>::~Polygon3D() {
			}
			
			template <class T, uint V>
			euc::EuclideanPoint<T,3> Polygon3D<T,V>::computeCentroid() {
				// https://en.wikipedia.org/wiki/Centroid#Of_a_polygon
				// Necessary assumption: the vertices forming this 3D polygon are co-planar.
				// Entering this process without knowing what plane the polygon lies on, so 
				// a little more work required. 
				// Process: calculate centroids of three projections (using Wiki method):
				//  1. XY plane
				//  2. YZ plane
				//  3. YZ plane
				// 'unwind' the projection to get the polygon centroid

				T area = this->getArea();

				// 1. calculate centroid of XY projection
				euc::EuclideanPoint<T,3> cXY(T(0), T(0), T(0));
				for (uint i=0; i<V; i++) {
					uint i2=(i+1)%V;
					euc::EuclideanPoint<T,3> v1 = this->vertices[i];
					euc::EuclideanPoint<T,3> v2 = this->vertices[i2];
					cXY.cmp[0] += (v1.cmp[0] + v2.cmp[0]) * ( v1.cmp[0]*v2.cmp[1] - v2.cmp[0] * v1.cmp[1] );
					cXY.cmp[1] += (v1.cmp[1] + v2.cmp[1]) * ( v1.cmp[0]*v2.cmp[1] - v2.cmp[0] * v1.cmp[1] );
				}
				cXY *= T(1) / (T(6) * area);

				// 2. calculate centroid of XZ projection
				euc::EuclideanPoint<T,3> cXZ(T(0), T(0), T(0));
				for (uint i=0; i<V; i++) {
					uint i2=(i+1)%V;
					euc::EuclideanPoint<T,3> v1 = this->vertices[i];
					euc::EuclideanPoint<T,3> v2 = this->vertices[i2];
					cXZ.cmp[0] += (v1.cmp[0] + v2.cmp[0]) * ( v1.cmp[0]*v2.cmp[2] - v2.cmp[0] * v1.cmp[2] );
					cXZ.cmp[2] += (v1.cmp[2] + v2.cmp[2]) * ( v1.cmp[0]*v2.cmp[2] - v2.cmp[0] * v1.cmp[2] );
				}
				cXZ *= T(1) / (T(6) * area);

				// 3. calculate centroid of YZ projection
				euc::EuclideanPoint<T,3> cYZ(T(0), T(0), T(0));
				for (uint i=0; i<V; i++) {
					uint i2=(i+1)%V;
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

				return centroid;
			}

			template <class T, uint V>
			T Polygon3D<T,V>::getArea() {
				if (!this->areaComputed) {
					this->area = this->computeArea();
					this->areaComputed = true;
				}
				return this->area;
			}

			template <class T, uint V>
			euc::EuclideanPoint<T,3> Polygon3D<T,V>::getCentroid() {
				if (!this->centroidComputed) {
					this->centroid = this->computeCentroid();
					this->centroidComputed = true;
				}
				return this->centroid;
			}

			template <class T, uint V>
			euc::EuclideanVector3D<T> Polygon3D<T,V>::computeNormal() {
				return euc::EuclideanPlane3D<T>::calculateNormal(this->vertices[0], this->vertices[1], this->vertices[2]);
			}

			template <class T, uint V>
			euc::EuclideanVector3D<T> {olygon3D<T,V>::getNormal() {
				if (!this->normalComputed) {
					this->normal = this->computeNormal();
					this->normalComputed = true;
				}
				return this->normal;
			}

			template <class T, uint V>
			bool Polygon3D<T,V>::isPointInside(const euc::EuclideanPoint<T,3>& p) {
				// Does the point lie on the same plane as the Quadrilateral Points?
				euc::EuclideanPlane3D<T> plane(this->vertices[0], this->vertices[1], this->vertices[2]);
				if(!(plane.isPointOnPlane(p))) {
					return false;
				}
				
				// Does the point equal one of the points - if so it counts as inside
				for (int i=0; i<V; i++) {
					if (p == this->vertices[i]) {
						return true;
					}
				}
				
				// If the point lies on one of the edges it counts as inside
				for (int i2=1; i2<V; i2++) {
					const int i1 = i2-1;
					if (isPointOnLine(this->vertices[i1], this->vertices[i2], p)) {
						return true;
					}
				}
				
				// // Test the intersection of the ray ranging from the point to the centroid. 
				// // If it intersects any of the edges, then the point p lies on the opposite side
				// // of an edge to the centroid, and so must be outside
				// euc::EuclideanPoint<T,3> centroid = this->computeCentroid();
				// for (int i2=1; i2<V; i2++) {
				// 	const int i1 = i2-1;
				// 	if (euc::isVectorRangeIntersection(p, centroid, this->vertices[i1], this->vertices[i2])) {
				// 		return false;
				// 	}
				// }
				// return true;

				// Alternative method: rotate into 2D plane, because 
				// I don't trust floating-point rounding errors to not break 3D method:
				Polygon3D<T,V> polyCopy(*this);
				euc::EuclideanPoint<T,3> pCopy(p);

				// ToDo: potential optimisation: store rotation matrix and 'poly2d', instead 
				//       of computing every time.
				euc::EuclideanVector3D<T> zAxis(T(0), T(0), T(1));
				euc::Matrix<T,3,3> rotMatrix2;
				rotMatrix2 = euc::calculateRotationMatrix(polyCopy.getNormal(), zAxis);
				// Apply rotation:
				for (int i=0; i<V; i++) {
					polyCopy.vertices[i] = rotMatrix2 * polyCopy.vertices[i];
				}
				pCopy = rotMatrix2 * pCopy;

				// Switch to 2D X-Y plane
				euc::EuclideanPoint<T,2> vertices2D[V];
				for (int i=0; i<V; i++) {
					vertices2D[i] = euc::EuclideanPoint<T,2>(polyCopy.vertices[i].cmp[0], polyCopy.vertices[i].cmp[1]);
				}
				Polygon2D<T,V> poly2d(vertices2D);
				euc::EuclideanPoint<T,2> p2d(pCopy.cmp[0], pCopy.cmp[1]);
				return poly2d.isPointInside(p2d);
			}

			template <class T, uint V>
			bool Polygon3D<T,V>::coplanar() {
				// Calculate plane normal from first three vertices:
				euc::EuclideanVector3D<T> normal = this->computeNormal();
				// Check if any other vertex is NOT coplanar:
				for (int i=3; i<V; i++){
					euc::EuclideanVector3D<T> v = this->vertices[i] - this->vertices[0];
					T dp = v.dotProduct(normal);
					if (dp != T(0)) {
						return false;
					}
				}
				return true;
			}

			template <class T, uint V>
			void Polygon3D<T,V>::alignNormalWithVector(euc::EuclideanVector3D<T>& v) {
				// Record original vertices:
				euc::EuclideanPoint<T,3> vertices[V];
				for (int i=0; i<V; i++) {
					vertices[i] = this->vertices[i];
				}

				// Calculate rotation matrix:
				euc::EuclideanVector3D<T> normal = euc::EuclideanPlane3D<T>::calculateNormal(this->vertices[0], this->vertices[1], this->vertices[2]);
				normal.normalise();
				euc::Matrix<T,3,3> rotMatrix2;
				rotMatrix2 = euc::calculateRotationMatrix(normal, v);

				// Apply rotation:
				for (int i=0; i<V; i++) {
					this->vertices[i] = rotMatrix2 * this->vertices[i];
				}

				euc::EuclideanVector3D<T> normalRotated = rotMatrix2 * normal;
				normalRotated.normalise();

				// Verify that rotation aligned polygon normal with Z-axis:
				euc::EuclideanVector3D<T> normalActual = euc::EuclideanPlane3D<T>::calculateNormal(this->vertices[0], this->vertices[1], this->vertices[2]);
				normalActual.normalise();
				T cosine = v.dotProduct(normalActual);
				if (cosine < T(0.9999)) {
					// 'cosine' should be 1.0 exactly, but allow a tiny tolerance for floating-point

					printf("Polygon rotation failed to align its normal with specified vector\n");

					printf("Polygon vertices before:\n");
					for (int i=0; i<V; i++) {
						printf("V %d: [ %.3f %.3f %.3f ]\n", i, vertices[i].cmp[0], vertices[i].cmp[1], vertices[i].cmp[2]);
					}
					printf("Normal before: ");
					normal.print(); std::cout << std::endl;
					
					printf("Polygon vertices after:\n");
					for (int i=0; i<V; i++) {
						printf("V %d: [ %.3f %.3f %.3f ]\n", i, this->vertices[i].cmp[0], this->vertices[i].cmp[1], this->vertices[i].cmp[2]);
					}
					printf("Normal after: ");
					normalActual.print(); std::cout << std::endl;
					// printf("Normal rotated (should match above): ");
					// normalRotated.print(); std::cout << std::endl;
					printf("Cosine between normal and Z-axis = %.4f (%.1f degrees)\n", cosine, acos(cosine)/3.14157*180.0);

					printf("Target vector: ");
					v.print(); std::cout << std::endl;

					printf("Rotation matrix: \n");
					rotMatrix2.print(); std::cout << std::endl;

					printf("\n");

					HARD_CHECK_ECODE(cupcfd::error::E_ERROR);
				}
			}
			
			// Probably an actual geometric term for 'polygon edges do not intersect':
			// template <class S, class T, uint N, uint V>
			template <class T, uint V>
			bool Polygon3D<T,V>::verifyNoEdgesIntersect() {
				// floating-point rounding errors could prevent detection of any true
				// 3D intersection point. Best bet is to rotate the polygon so
				// normal becomes Z-axis (can do this because coplanar). 
				// Then only working in 2D so will definitely find 
				// intersection point.
				euc::EuclideanVector3D<T> zAxis(T(0), T(0), T(1));
				Polygon3D<T,V> copy(*this);
				copy.alignNormalWithVector(zAxis);
				euc::EuclideanPoint<T,3> vertices[V];
				for (int i=0; i<V; i++) {
					vertices[i] = copy.vertices[i];
				}
				
				// Now have polygon rotated into 2D X-Y plane, scan for any 
				// crossing edges. 
				// Permute through all possible pairs of edges, except directly adjacent.
				// Maybe there is a better way to scan, but 
				// this check is only performed once so does it matter?
				for (uint e1=1; e1<V; e1++) {
					euc::EuclideanPoint<T,2> e1a(vertices[e1-1].cmp[0], vertices[e1-1].cmp[1]);
					euc::EuclideanPoint<T,2> e1b(vertices[e1  ].cmp[0], vertices[e1  ].cmp[1]);

					for (uint e2=e1+2; e2<V; e2++) {
						euc::EuclideanPoint<T,2> e2a(vertices[e2-1].cmp[0], vertices[e2-1].cmp[1]);
						euc::EuclideanPoint<T,2> e2b(vertices[e2  ].cmp[0], vertices[e2  ].cmp[1]);

						euc::EuclideanPoint<T,2> intersectPoint(T(0.0), T(0.0));
						cupcfd::error::eCodes status = euc::computeVectorRangeIntersection(e1a, e1b, e2a, e2b, intersectPoint);
						if (status == cupcfd::error::E_SUCCESS) {
							printf("Intersection detected between polygon edges!\n");

							printf("Polygon vertices:\n");
							for (int i=0; i<V; i++) {
								printf("V %d: [ %.2f %.2f %.2f ]\n", i, vertices[i].cmp[0], vertices[i].cmp[1], vertices[i].cmp[2]);
							}

							printf("Intersecting edges:\n");
							e1a.print(); printf(" --> "); e1b.print(); printf("\n");
							e2a.print(); printf(" --> "); e2b.print(); printf("\n");

							printf("Intersection point:\n");
							intersectPoint.print();

							return false;
						}
					}
				}

				return true;
			}
			
			template <class T>
			inline bool isVertexOrderClockwise(const euc::EuclideanPoint<T,3>& observation, euc::EuclideanPoint<T,3> * points, int nPoints) {
				if (nPoints < 3) {
					throw(std::runtime_error("Polygon3D::isVertexOrderClockwise() called with < 3 points"));
				}
			
				// ToDo: Error Check - observation must not be coplanar with points
			
				// Draw a vector from the observation point to a point on the polygon
				euc::EuclideanVector3D<T> obsVec = observation - points[0];
				
				// Compute the normal of the polygon
				euc::EuclideanPlane3D<T> plane(points[0], points[1], points[2]);
				euc::EuclideanVector3D<T> normal = plane.getNormal();
				
				// If the points are traversed in clockwise order from the observation point, the vector will
				// point towards the observation point.
				// Compute the dot-product - if the vectors both point in the same direction (>0) then it is anti-clockwise from the observation point,
				// else it is clockwise from the observation point (since they will be in opposite directions)
				
				T dot = normal.dotProduct(obsVec);
				
				if(dot < 0) {
					// Anti-Clockwise
					return true;
				}
				
				return false;
			}
		}
	}
}

#endif
