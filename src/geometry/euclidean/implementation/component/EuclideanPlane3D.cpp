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
#include "ArithmeticKernels.h"

// C++ Library
// Provides overloads for abs functions
#include <cmath>

#include <iostream>

namespace arth = cupcfd::utility::arithmetic::kernels;

namespace cupcfd
{
	namespace geometry
	{
		namespace euclidean
		{
			template <class T>
			EuclideanPlane3D<T>::EuclideanPlane3D(const cupcfd::geometry::euclidean::EuclideanPoint<T,3>& p1,
												  const cupcfd::geometry::euclidean::EuclideanPoint<T,3>& p2,
												  const cupcfd::geometry::euclidean::EuclideanPoint<T,3>& p3)
			{
				// Copy the points across to store as plane points
				this->p1 = p1;
				this->p2 = p2;
				this->p3 = p3;

				// Planar Equation (basically a normal computation plus a bit extra)
				this->computeScalarPlaneEquation(&(this->a), &(this->b), &(this->c), &(this->d));
			}

			template <class T>
			EuclideanPlane3D<T>::~EuclideanPlane3D()
			{
				// Nothing to do currently
			}

			template <class T>
			cupcfd::geometry::euclidean::EuclideanVector<T,3> EuclideanPlane3D<T>::getNormal()
			{
				return cupcfd::geometry::euclidean::EuclideanVector<T,3>(this->a, this->b, this->c);
			}

			template <class T>
			cupcfd::error::eCodes EuclideanPlane3D<T>::computeScalarPlaneEquation(T * a, T * b, T * c, T * d)
			{
				// a(x-x0) + b(y - y0) + c(z - z0) = 0, where (x0,y0,z0) is point in the plane and <a,b,c> is the normal vector
				// =>
				// ax - ax0 + by - by0 + cz - cz0 = 0
				// =>
				// ax + by + cz = ax0 + by0 + cz0

				// <a,b,c> = normal vector(x,y,z>
				cupcfd::geometry::euclidean::EuclideanVector<T,3> norm = EuclideanPlane3D<T>::normal(this->p1, this->p2, this->p3);

				// (x0, y0, z0) = any point: Will select point p1
				T rhs = (norm.cmp[0] * this->p1.cmp[0]) + (norm.cmp[1] * this->p1.cmp[1]) + (norm.cmp[2] * this->p1.cmp[2]);

				*a = norm.cmp[0];
				*b = norm.cmp[1];
				*c = norm.cmp[2];
				*d = 0 - rhs;

				return cupcfd::error::E_SUCCESS;
			}

			template <class T>
			bool EuclideanPlane3D<T>::isPointOnPlane(const cupcfd::geometry::euclidean::EuclideanPoint<T,3>& point)
			{
				T val = (this->a * point.cmp[0]) + (this->b * point.cmp[1]) + (this->c * point.cmp[2]) + this->d;

				return arth::isEqual(val, (T) 0);
			}

			template <class T>
			cupcfd::geometry::euclidean::EuclideanPoint<T,3> EuclideanPlane3D<T>::computeProjectedPoint(const cupcfd::geometry::euclidean::EuclideanPoint<T,3>& point)
			{
				cupcfd::geometry::euclidean::EuclideanPoint<T,3> result;

				// Pick a point on the plane and compute the vector to the point
				cupcfd::geometry::euclidean::EuclideanVector<T,3> vec = point - this->p1;

				// Compute the unit normal vector
				cupcfd::geometry::euclidean::EuclideanVector<T,3> n = this->getNormal();

				// Compute The Scalar Component of vec in the direction of the normal vector
				// https://en.wikipedia.org/wiki/Dot_product#Scalar_projection_and_first_properties
				n.normalise();
				T d = vec.dotProduct(n);

				// Translate the point p in the direction of the normal vector by the scalar distance
				// n is unit vector at this point so we can use this to get the translation vector
				cupcfd::geometry::euclidean::EuclideanVector<T,3> translate = d * n;
				result = point - translate;

				return result;
			}

			template <class T>
			bool EuclideanPlane3D<T>::isVectorParallel(const cupcfd::geometry::euclidean::EuclideanVector<T,3>& vec)
			{
				// Vector path is parallel to plane if the dot product of the vector and the plane normal is 0
				T dotP = this->getNormal().dotProduct(vec);

				return arth::isEqual(dotP, T(0.0));
			}

			template <class T>
			bool EuclideanPlane3D<T>::isVectorParallelInPlane(const cupcfd::geometry::euclidean::EuclideanVector<T,3>& vec,
															  const cupcfd::geometry::euclidean::EuclideanPoint<T,3>& l0)
			{
				// Two components:
				// (a) Is the vector parallel?
				// (b) Does the vector line on/inside the plane (check l0)

				if(!this->isVectorParallel(vec))
				{
					// Vector is not parallel
					return false;
				}

				if(!this->isPointOnPlane(l0))
				{
					// Vector has been found to be parallel, but the vector point does not lie on the plane
					return false;
				}

				// Only reachable if both the vector is parallel, and the vector point l0 lies on the plane
				return true;
			}

			template <class T>
			cupcfd::error::eCodes EuclideanPlane3D<T>::linePlaneIntersection(const cupcfd::geometry::euclidean::EuclideanVector<T,3>& l,
																			 	  const cupcfd::geometry::euclidean::EuclideanPoint<T,3>& l0,
																				  cupcfd::geometry::euclidean::EuclideanPoint<T,3>& result)
			{
				// https://en.wikipedia.org/wiki/Line-plane_intersection
				cupcfd::geometry::euclidean::EuclideanVector<T,3> normal = this->getNormal();
				T dotP = normal.dotProduct(l);

				// Could use isVectorParallel, but don't want to compute dot product twice.
				if(arth::isEqual(dotP, T(0.0)))
				{
					// Vector is parallel, so 'intersection' concept is invalid
					// (either no points intersect, or every point on the line intersects if on the plane)
					return cupcfd::error::E_EUC_VEC_PARALLEL;
				}

				// Vector is not parallel, so we can compute the intersection point
				// d = (p0 - l0) . n / (l.n)

				cupcfd::geometry::euclidean::EuclideanVector<T,3> tmp = this->p1 - l0;
				T d = tmp.dotProduct(normal);
				d = d / dotP;

				// Intersection = d * l + l0
				result = (d * l) + l0;
				return cupcfd::error::E_SUCCESS;
			}

			template <class T>
			T EuclideanPlane3D<T>::shortestDistance(const cupcfd::geometry::euclidean::EuclideanPoint<T,3>& p)
			{
				// https://en.wikipedia.org/wiki/Distance_from_a_point_to_a_plane
				// I.e. Compute the distance of this plane to the origin, and the distance of the parallel plane containing
				// point p to the origin, and determine the difference between the two.

				// Distance to origin = d from planar equation / (sqrt(a^2 + b^2+c^2)) where (a,b,c) is the norm

				T denom = (cupcfd::utility::arithmetic::kernels::sqrtWr((this->a * this->a)+(this->b*this->b)+(this->c*this->c)));

				T planeDisOrigin = this->d / denom;

				// Plane p lies on is parallel to this plane
				// Only unknown factors is d
				T pointD = 0 - ((p.cmp[0] * a) + (p.cmp[1] * b) + (p.cmp[2] * c));

				// Compute distance to origin from plane with point p, shares same normal vector/denominator since parallel
				T pointDisOrigin = pointD / denom;

				if(cupcfd::utility::arithmetic::kernels::sgn(this->d) != cupcfd::utility::arithmetic::kernels::sgn(pointD))
				{
					// Planes are different sides of the origin, so add the magnitudes of each
					return std::abs(planeDisOrigin) + std::abs(pointDisOrigin);
				}
				else
				{
					// Planes are same side of origin, so take the magnitude of the difference
					return std::abs(planeDisOrigin - pointDisOrigin);
				}
			}

			// === Static Methods ===

			template <class T>
			cupcfd::geometry::euclidean::EuclideanVector<T,3> EuclideanPlane3D<T>::normal(const cupcfd::geometry::euclidean::EuclideanPoint<T,3>& p1,
																							   const cupcfd::geometry::euclidean::EuclideanPoint<T,3>& p2,
																							   const cupcfd::geometry::euclidean::EuclideanPoint<T,3>& p3)
			{
				// Algorithm: https://www.khronos.org/opengl/wiki/Calculating_a_Surface_Normal

				// U = point 2 - point 1
				cupcfd::geometry::euclidean::EuclideanVector<T,3> u = p2 - p1;
				cupcfd::geometry::euclidean::EuclideanVector<T,3> v = p3 - p1;
				cupcfd::geometry::euclidean::EuclideanVector<T,3> normVec;

				normVec.cmp[0] = (u.cmp[1] * v.cmp[2]) - (u.cmp[2] * v.cmp[1]);
				normVec.cmp[1] = (u.cmp[2] * v.cmp[0]) - (u.cmp[0] * v.cmp[2]);
				normVec.cmp[2] = (u.cmp[0] * v.cmp[1]) - (u.cmp[1] * v.cmp[0]);

				return normVec;
			}

			template <class T>
			cupcfd::error::eCodes EuclideanPlane3D<T>::linePlaneIntersection(cupcfd::geometry::euclidean::EuclideanVector<T,3>& normal,
																				  const cupcfd::geometry::euclidean::EuclideanPoint<T,3>& p0,
																				  const cupcfd::geometry::euclidean::EuclideanVector<T,3>& l,
																				  const cupcfd::geometry::euclidean::EuclideanPoint<T,3>& l0,
																				  cupcfd::geometry::euclidean::EuclideanPoint<T,3>& result)
			{
				// https://en.wikipedia.org/wiki/Line-plane_intersection
				T dotP = normal.dotProduct(l);

				if(arth::isEqual(dotP, T(0.0)))
				{
					// Vector is parallel, so 'intersection' concept is invalid
					// (either no points intersect, or every point on the line intersects if on the plane)
					return cupcfd::error::E_EUC_VEC_PARALLEL;
				}

				// Vector is not parallel, so we can compute the intersection point
				// d = (p0 - l0) . n / (l.n)

				cupcfd::geometry::euclidean::EuclideanVector<T,3> tmp = p0 - l0;
				T d = tmp.dotProduct(normal);
				d = d / dotP;

				// Intersection = d * l + l0
				result = (d * l) + l0;
				return cupcfd::error::E_SUCCESS;
			}

			// Explicit Instantiation
			template class EuclideanPlane3D<float>;
			template class EuclideanPlane3D<double>;
		}
	}
}
