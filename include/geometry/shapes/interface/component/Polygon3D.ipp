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
#include "Triangle3D.h"

namespace cupcfd
{
	namespace geometry
	{
		namespace shapes
		{
			template <class P, class T, uint V>
			Polygon3D<P,T,V>::Polygon3D() {
			}

			template <class P, class T, uint V>
			template<class...Args>
			Polygon3D<P,T,V>::Polygon3D(Args...v)
			: Polygon<Polygon3D<P,T,V>,T,3,V>::Polygon( (v)... )
			{
				static_assert(sizeof...(Args) == V, "Polygon constructor dimension does not match number of parameters");
				if (!this->coplanar()) {
					HARD_CHECK_ECODE(cupcfd::error::E_GEOMETRY_LOGIC_ERROR);
				}

				// ToDo: Verify that no edges intersect
				if (!this->verifyNoEdgesIntersect()) {
					HARD_CHECK_ECODE(cupcfd::error::E_GEOMETRY_LOGIC_ERROR);
				}
			}
			
			template <class P, class T, uint V>
			Polygon3D<P,T,V>::~Polygon3D() {
			}
			
			// template <class P, class T, uint V>
			// bool Polygon3D<P,T,V>::isPointInside(const cupcfd::geometry::euclidean::EuclideanPoint<T,3>& point) {
			// 	return static_cast<P*>(this)->isPointInside(point);
			// }

			template <class P, class T, uint V>
			bool Polygon3D<P,T,V>::coplanar() {
				// Calculate plane normal from first three vertices:
				// euc::EuclideanVector<T,3> normal = euc::EuclideanPlane3D<T>::calculateNormal(this->vertices[0], this->vertices[1], this->vertices[2]);
				euc::EuclideanVector3D<T> normal = this->computeNormal();
				// Check if any other vertex is NOT coplanar:
				for (int i=3; i<V; i++){
					euc::EuclideanVector3D<T> v = this->vertices[i] - this->vertices[0];
					T dp = v.dotProduct(normal);
					if (dp != T(0)) {
						printf("NOT COPLANAR! (dp = %.2e)\n", dp);
						return false;
					}
				}
				return true;
			}
			
			template <class P, class T, uint V>
			T Polygon3D<P,T,V>::computeArea() {
				T area = T(0);
				euc::EuclideanPoint<T,3> v0 = this->vertices[0];
				for (int i2=2; i2<V; i2++) {
					const uint i1 = i2-1;
					shapes::Triangle3D<T> t(v0, this->vertices[i1], this->vertices[i2]);
					area += t.computeArea();
				}
				return area;
			}
			
			template <class P, class T, uint V>
			inline cupcfd::geometry::euclidean::EuclideanVector3D<T> Polygon3D<P,T,V>::computeNormal() {
				// return static_cast<P*>(this)->computeNormal();
				return euc::EuclideanPlane3D<T>::calculateNormal(this->vertices[0], this->vertices[1], this->vertices[2]);
			}

			template <class P, class T, uint V>
			inline euc::EuclideanPoint<T,3> Polygon3D<P,T,V>::computeCentroid() {
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

			// Probably an actual geometric term for 'polygon edges do not intersect':
			// template <class S, class T, uint N, uint V>
			template <class P, class T, uint V>
			bool Polygon3D<P,T,V>::verifyNoEdgesIntersect() {
				// floating-point round errors could prevent detection of any true
				// 3D intersection point. Best bet is to rotate the polygon so
				// normal becomes Z-axis, then only working in 2D and will 
				// definitely find intersection point (unless parallel)

				// Gather the calculation inputs:
				euc::EuclideanPoint<T,3> centroid = this->computeCentroid();
				euc::EuclideanVector3D<T> normal = this->computeNormal();
				normal.normalise();
				euc::EuclideanVector3D<T> zAxis(T(0), T(0), T(1));

				// Begin:
				euc::EuclideanVector3D<T> axis = normal.crossProduct(zAxis);
				T cosine = normal.dotProduct(zAxis);
				T sine = axis.length();
				axis.normalise();
				T oneMinusCosine = T(1) - cosine;
				if (cosine != T(0) || sine != T(0)) {
					// Construct rotation matrix
					// https://en.wikipedia.org/wiki/Transformation_matrix#Rotation_2
					T rotMatrix[3][3];
					T x = zAxis.cmp[0];
					T y = zAxis.cmp[1];
					T z = zAxis.cmp[2];
					rotMatrix[0][0] = x*x*oneMinusCosine + cosine;
					rotMatrix[0][1] = y*x*oneMinusCosine - sine*z;
					rotMatrix[0][2] = z*x*oneMinusCosine + sine*y;

					rotMatrix[1][0] = x*y*oneMinusCosine + sine*z;
					rotMatrix[1][1] = y*y*oneMinusCosine + cosine;
					rotMatrix[1][2] = z*y*oneMinusCosine - sine*x;

					rotMatrix[2][0] = x*z*oneMinusCosine - sine*y;
					rotMatrix[2][1] = y*z*oneMinusCosine + sine*x;
					rotMatrix[2][2] = z*z*oneMinusCosine + cosine;

					euc::EuclideanVector3D<T> normalRot(T(0));
					normalRot.cmp[0] = rotMatrix[0][0]*x + rotMatrix[0][1]*y + rotMatrix[0][2]*z;
					normalRot.cmp[1] = rotMatrix[1][0]*x + rotMatrix[1][1]*y + rotMatrix[1][2]*z;
					normalRot.cmp[2] = rotMatrix[2][0]*x + rotMatrix[2][1]*y + rotMatrix[2][2]*z;

					if (!(normalRot == zAxis)) {
						printf("Vector: ");
						normal.print(); std::cout << std::endl;
						printf("Target: ");
						zAxis.print(); std::cout << std::endl;
						printf("Axis:");
						axis.print(); std::cout << std::endl;
						printf("cosine: %.2f\n", cosine);
						printf("sine: %.2f\n", sine);

						printf("rotMatrix:\n");
						printf("| %.2f  %.2f  %.2f |\n", rotMatrix[0][0], rotMatrix[0][1], rotMatrix[0][2]);
						printf("| %.2f  %.2f  %.2f |\n", rotMatrix[1][0], rotMatrix[1][1], rotMatrix[1][2]);
						printf("| %.2f  %.2f  %.2f |\n", rotMatrix[2][0], rotMatrix[2][1], rotMatrix[2][2]);
						printf("\n");

						printf("Vector rotated: ");
						normalRot.print(); std::cout << std::endl;

						HARD_CHECK_ECODE(cupcfd::error::E_ERROR);
					}
				}
				return true;
			}
			
			// template <class T>
			// inline T computeArea(cupcfd::geometry::euclidean::EuclideanPoint<T,3> * points, int nPoints) {
			// 	// (Typo is intentional)
			// 	//https://math.stackexchange.com/questions/3207981/caculate-area-of-polygon-in-3d
				
			// 	cupcfd::geometry::euclidean::EuclideanVector<T,3> partialVector(T(0),T(0),T(0));
				
			// 	for(int i = 1; i < (nPoints-1); i++) {
			// 		partialVector += (0.5 * cupcfd::geometry::euclidean::crossProduct(points[i] - points[0], points[i+1] - points[0]));	
			// 	}
				
			// 	// Compute norm/length - this will also ignore any direction (magnitude only)
			// 	T length;
			// 	partialVector.length(&length);
			// 	return length;
			// }
			
			template <class T>
			inline bool isVertexOrderClockwise(const cupcfd::geometry::euclidean::EuclideanPoint<T,3>& observation, cupcfd::geometry::euclidean::EuclideanPoint<T,3> * points, int nPoints) {
				if (nPoints < 3) {
					throw(std::runtime_error("Polygon3D::isVertexOrderClockwise() called with < 3 points"));
				}
			
				// ToDo: Error Check - observation must not be coplanar with points
			
				// Draw a vector from the observation point to a point on the polygon
				cupcfd::geometry::euclidean::EuclideanVector3D<T> obsVec = observation - points[0];
				
				// Compute the normal of the polygon
				cupcfd::geometry::euclidean::EuclideanPlane3D<T> plane(points[0], points[1], points[2]);
				cupcfd::geometry::euclidean::EuclideanVector3D<T> normal = plane.getNormal();
				
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
