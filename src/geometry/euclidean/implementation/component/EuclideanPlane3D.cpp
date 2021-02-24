/*
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * This file contains the implementation of the EuclideanPlane class
 */

#include "EuclideanPlane3D.h"
#include "EuclideanVector.h"
#include "EuclideanVector3D.h"
#include "ArithmeticKernels.h"

// C++ Library
// Provides overloads for abs functions
#include <cmath>

#include <iostream>

namespace arth = cupcfd::utility::arithmetic::kernels;
namespace euc = cupcfd::geometry::euclidean;

namespace cupcfd
{
	namespace geometry
	{
		namespace euclidean
		{
			template <class T>
			EuclideanPlane3D<T>::EuclideanPlane3D(const euc::EuclideanPoint<T,3>& p1,
												  const euc::EuclideanPoint<T,3>& p2,
												  const euc::EuclideanPoint<T,3>& p3) 
			{
				// Copy the points across to store as plane points
				this->p1 = p1;
				this->p2 = p2;
				this->p3 = p3;

				// Useful to store a point vector:
				this->v2 = p2 - p1;
				this->v2unit = this->v2;
				this->v2unit.normalise();
				this->v2Length = this->v2.length();

				this->normal = EuclideanPlane3D<T>::calculateNormal(p1, p2, p3);
				this->normalUnit = this->normal;
				this->normalUnit.normalise();
				this->computedNormal = true;

				// Planar Equation (basically a normal computation plus a bit extra)
				this->computeScalarPlaneEquation(&(this->a), &(this->b), &(this->c), &(this->d));
			}

			template <class T>
			EuclideanPlane3D<T>::~EuclideanPlane3D() {
				// Nothing to do currently
			}

			// template <class T>
			// void EuclideanPlane3D<T>::reverseVertexOrdering() {
			// 	euc::EuclideanPoint<T,3> tmp = this->p1;
			// 	this->p1 = this->p3;
			// 	this->p3 = tmp;

			// 	// this->normal *= T(-1);
			// }

			template <class T>
			euc::EuclideanVector3D<T> EuclideanPlane3D<T>::getNormal() {
				if (!this->computedNormal) {
					this->normal = EuclideanPlane3D<T>::calculateNormal(this->p1, this->p2, this->p3);
					this->normalUnit = this->normal;
					this->normalUnit.normalise();
					this->computedNormal = true;
				}
				return this->normal;
			}

			template <class T>
			void EuclideanPlane3D<T>::computeScalarPlaneEquation(T * a, T * b, T * c, T * d) {
				// a(x-x0) + b(y - y0) + c(z - z0) = 0, where (x0,y0,z0) is point in the plane and <a,b,c> is the normal vector
				// =>
				// ax - ax0 + by - by0 + cz - cz0 = 0
				// =>
				// ax + by + cz = ax0 + by0 + cz0

				// <a,b,c> = normal vector(x,y,z>

				// (x0, y0, z0) = any point: Will select point p1
				T rhs = (this->normal.cmp[0] * this->p1.cmp[0]) + (this->normal.cmp[1] * this->p1.cmp[1]) + (this->normal.cmp[2] * this->p1.cmp[2]);

				*a = this->normal.cmp[0];
				*b = this->normal.cmp[1];
				*c = this->normal.cmp[2];
				*d = 0 - rhs;
			}

			template <class T>
			bool EuclideanPlane3D<T>::isPointOnPlane(const euc::EuclideanPoint<T,3>& point) {
				EuclideanVector3D<T> u = point - this->p1;
				this->dotP = u.dotProduct(this->normalUnit);
				if (arth::isEqual(this->dotP, T(0))) {
					return true;
				}

				// Floating-point rounding errors means tolerance is required.
				// Tolerance should be relative to size of polygon represented 
				// by plane, otherwise we are just stabbing in the dark (or need 
				// to calculate the expected rounding error).
				// Thus use distance from plane, not angle, as metric - tolerate a
				/// relatively-negligible distance from plane:
				T tolerance_pct = T(1e-10);
				T tolerableDistance = tolerance_pct * this->v2Length;
				T distance = std::fabs(this->dotP);
				return (distance < tolerableDistance);
			}

			template <class T>
			euc::EuclideanPoint<T,3> EuclideanPlane3D<T>::computeProjectedPoint(const euc::EuclideanPoint<T,3>& point) {
				euc::EuclideanPoint<T,3> projection;
				
				euc::EuclideanVector3D<T> v = point - this->p1;
				euc::EuclideanVector3D<T> n = this->normalUnit;
				this->dotP = v.dotProduct(n);
				projection = point - (this->dotP*n);

				return projection;
			}

			template <class T>
			bool EuclideanPlane3D<T>::isVectorParallel(const euc::EuclideanVector<T,3>& vec) {
				// Vector path is parallel to plane if the dot product of the vector and the plane normal is 0
				this->dotP = this->normalUnit.dotProduct(vec);
				if (arth::isEqual(this->dotP, T(0))) {
					return true;
				}

				// Floating-point rounding errors means tolerance is required.
				// Tolerance should be relative to size of polygon represented 
				// by plane, otherwise we are just stabbing in the dark (or need 
				// to calculate the expected rounding error).
				// Thus use distance from plane, not angle, as metric - tolerate a
				/// relatively-negligible distance from plane:
				T tolerance_pct = T(1e-10);
				T tolerableDistance = tolerance_pct * this->v2Length;
				T distance = std::fabs(this->dotP);
				return (distance < tolerableDistance);
			}

			template <class T>
			bool EuclideanPlane3D<T>::isVectorParallelInPlane(const euc::EuclideanVector<T,3>& l,
															  const euc::EuclideanPoint<T,3>& l0)
			{
				// Two components:
				// (a) Is the vector parallel?
				// (b) Does the vector line on/inside the plane (check l0)

				if(!this->isVectorParallel(l)) {
					return false;
				}
				else if(!this->isPointOnPlane(l0)) {
					return false;
				}
				return true;
			}

			template <class T>
			cupcfd::error::eCodes EuclideanPlane3D<T>::linePlaneIntersection(const euc::EuclideanVector<T,3>& l,
																		 	const euc::EuclideanPoint<T,3>& l0,
																			euc::EuclideanPoint<T,3>& result) {
				if(this->isVectorParallel(l)) {
					// Vector is parallel, so 'intersection' concept is invalid
					// (either no points intersect, or every point on the line intersects if on the plane)
					return cupcfd::error::E_EUC_VEC_PARALLEL;
				}

				// Vector is not parallel, so we can compute the intersection point
				// d = ( (p0 - l0) . n ) / (l.n)

				euc::EuclideanVector3D<T> l0_to_p1 = this->p1 - l0;
				// Re-use dot-product from isVectorParallel() call:0.0
				T d = l0_to_p1.dotProduct(this->normalUnit) / this->dotP;

				// Intersection = d * l + l0
				result = (d * l) + l0;
				return cupcfd::error::E_SUCCESS;
			}

			template <class T>
			T EuclideanPlane3D<T>::shortestDistance(const euc::EuclideanPoint<T,3>& p) {
				EuclideanVector3D<T> v(p - this->p1);
				this->dotP = v.dotProduct(this->normalUnit);
				return std::abs(this->dotP);
			}

			// === Static Methods ===

			template <class T>
			euc::EuclideanVector3D<T> EuclideanPlane3D<T>::calculateNormal(const euc::EuclideanPoint<T,3>& p1,
																			const euc::EuclideanPoint<T,3>& p2,
																			const euc::EuclideanPoint<T,3>& p3) {
				euc::EuclideanVector3D<T> u = p2-p1;
				euc::EuclideanVector3D<T> v = p3-p1;
				euc::EuclideanVector3D<T> normVec = u.crossProduct(v);
				return normVec;
			}

			// Explicit Instantiation
			template class EuclideanPlane3D<float>;
			template class EuclideanPlane3D<double>;
		}
	}
}
