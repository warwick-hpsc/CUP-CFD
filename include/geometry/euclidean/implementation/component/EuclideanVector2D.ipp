/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Contains the header level definitions for the EuclideanVector2D class.
 */

#ifndef CUPCFD_GEOMETRY_EUCLIDEAN_VECTOR2D_IPP_H
#define CUPCFD_GEOMETRY_EUCLIDEAN_VECTOR2D_IPP_H

namespace cupcfd
{
	namespace geometry
	{
		namespace euclidean
		{
			template <class T>
			EuclideanVector2D<T>::EuclideanVector2D() {
				this->cmp[0] = T(0);
				this->cmp[1] = T(0);
			}

			template <class T>
			EuclideanVector2D<T>::EuclideanVector2D(T a, T b) {
				this->cmp[0] = a;
				this->cmp[1] = b;
			}

			template <class T>
			EuclideanVector2D<T>::EuclideanVector2D(const EuclideanVector<T,2>& v) {
				this->cmp[0] = v.cmp[0];
				this->cmp[1] = v.cmp[1];
			}

			template <class T>
			bool isVectorRangeIntersection(const EuclideanPoint<T,2>& p1, const EuclideanPoint<T,2>& p2,
											const EuclideanPoint<T,2>& p3, const EuclideanPoint<T,2>& p4)
			{
				// Needed to store result, but we discard it
				EuclideanPoint<T,2> intersectPoint(T(0.0), T(0.0));
				cupcfd::error::eCodes status = computeVectorRangeIntersection(p1, p2, p3, p4, intersectPoint);
				
				if(status == cupcfd::error::E_SUCCESS) {
					return true;
				}
				else {
					return false;
				}
			}

			template <class T>
			inline cupcfd::error::eCodes computeVectorRangeIntersection(const EuclideanPoint<T,2>& p1, const EuclideanPoint<T,2>& p2,
											 	  	  	  	  	  	  	const EuclideanPoint<T,2>& p3, const EuclideanPoint<T,2>& p4,
																		EuclideanPoint<T,2>& intersectPoint)
			{
				// First, handle edge cases:

				if((p1 == p2) && (p2 == p3) && (p3 == p4)) {					
					// All same point, therefore technically is intersecting at the point
					intersectPoint = p1;
					return cupcfd::error::E_SUCCESS;
				}
				
				if((p1 == p2) && (p3 == p4)) {
					// Both vectors are actually single points, but different coordinates
					return cupcfd::error::E_GEOMETRY_NO_INTERSECT;
				}
				
				if((p1 == p2)) {
					// First range is a point, second one is not since previous checks failed
					// Therefore can only intersect if the point *is* the intersection point

					// Test whether the point lies on the line
					bool isIntersect = isPointOnLine(p3, p4, p1);
					if(isIntersect) {
						intersectPoint = p1;
						return cupcfd::error::E_SUCCESS;
					}
					else {
						#ifdef DEBUG
							std::cout << "ERROR: isIntersect is false" << std::endl;
						#endif
						return cupcfd::error::E_GEOMETRY_NO_INTERSECT;
					}
				}
			
				if((p3 == p4)) {
					// Second range is a point, first one is not since previous checks failed
					// Therefore can only intersect if the point *is* the intersection point
					#ifdef DEBUG
						std::cout << "Second range is a point, first one is not since previous checks failed. Therefore can only intersect if the point *is* the intersection point" << std::endl;
					#endif

					// Test whether the point lies on the line
					bool isIntersect = isPointOnLine(p1, p2, p3);
					
					if(isIntersect) {
						intersectPoint = p3;
						return cupcfd::error::E_SUCCESS;
					}
					else {
						#ifdef DEBUG
							std::cout << "ERROR: isIntersect is false" << std::endl;
						#endif
						return cupcfd::error::E_GEOMETRY_NO_INTERSECT;
					}
				}

				// Now, calculate intersection point
				// https://en.wikipedia.org/wiki/Line%E2%80%93line_intersection#In_two_dimensions
				T x1 = p1.cmp[0];
				T y1 = p1.cmp[1];
				T x2 = p2.cmp[0];
				T y2 = p2.cmp[1];
				T x3 = p3.cmp[0];
				T y3 = p3.cmp[1];
				T x4 = p4.cmp[0];
				T y4 = p4.cmp[1];
				// intersectPoint.cmp[0]=	( (x1*y2 - y1*x2)*(x3-x4) - (x1-x2)*(x3*y4 - y3*x4) ) /
				// 						( (x1-x2)*(y3-y4) - (y1-y2)*(x3-x4) );
				// intersectPoint.cmp[1]=	( (x1*y2 - y1*x2)*(y3-y4) - (y1-y2)*(x3*y4 - y3*x4) ) /
				// 						( (x1-x2)*(y3-y4) - (y1-y2)*(x3-x4) );

				// Better yet, calculate intersection point as a scalar multiple of vector p1->p2:
				T t=	( (x1-x3)*(y3-y4) - (y1-y3)*(x3-x4) ) /
						( (x1-x2)*(y3-y4) - (y1-y2)*(x3-x4) );
				
				if (t >= T(0) && t <= T(1)) {
					return cupcfd::error::E_SUCCESS;
				} else {
					return cupcfd::error::E_GEOMETRY_NO_INTERSECT;
				}
			}			

			template <class T>
			bool isPointOnLine(const EuclideanPoint<T,2>& x1, const EuclideanPoint<T,2>& x2,
								const EuclideanPoint<T,2>& p)
			{
				// Check p != x1
				if(p == x1) {
					return true;
				}
			
				// Create a vector from x1 to p
				EuclideanVector2D<T> x1p = p - x1;
				
				// Create a vector from x1 to x2
				EuclideanVector2D<T> x1x2 = x2 - x1;
				
				// If the length of x1 to p is
				// (a) The same direction(sign + parallel vector)
				// (b) A length less than or equal to x1->x2
				// Then the point must lie on the line
				
				// First Check: Make sure it's parallel
				if(!(x1p.isParallel(x1x2))) {
					return false;
				}
				
				// Since we know it's parallel, check whether the direction is the same or is 180
				// by finding the sign of the scalar multiplier (u = r * v) for vectors u,v and scalar r
				// Search for first non-zero value to compute the scalar
				// Since we know it's parallel, they sohuld be non-zero for both
				T scalar;
				bool scalarFound = false;
				for(uint i = 0; i < 3; i++) {
					if(!(arth::isEqual(x1p.cmp[i], T(0)))) {
						scalar = x1p.cmp[i] / x1x2.cmp[i];
						scalarFound = true;
						break;
					}
				}
				if (!scalarFound) {
					HARD_CHECK_ECODE(cupcfd::error::E_GEOMETRY_LOGIC_ERROR)
				}
				
				if(scalar < 0) {
					// Directions at 180 to one another so cannot be on line
					return false;
				}
				
				// Compute Lengths
				T x1p_length  = x1p.length();
				T x1x2_length = x1x2.length();

				// Last check, know lines are colinear (parallel + same origin)
				// Check the length of the x1p vector is less so it must fit on the range
				// of the line
				if(x1p_length > x1x2_length) {
					std::cout << "ERROR: isPointOnLine(): point not on line (within endpoints)" << std::endl;
					return false;
				}
				
				// All checks passed
				return true;
			}
		}
	}
}

template class euc::EuclideanVector2D<float>;
template class euc::EuclideanVector2D<double>;

#endif
