/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Contains the header level definitions for the EuclideanVector class.
 */

#ifndef CUPCFD_GEOMETRY_EUCLIDEAN_VECTOR3D_IPP_H
#define CUPCFD_GEOMETRY_EUCLIDEAN_VECTOR3D_IPP_H

#include <iostream>
#include "ArithmeticKernels.h"
#include <cmath>

// ToDo: Need to check for dangers of division by 0....

namespace arth = cupcfd::utility::arithmetic::kernels;

namespace cupcfd
{
	namespace geometry
	{
		namespace euclidean
		{
			template <class T>
			EuclideanVector3D<T>::EuclideanVector3D() {
				this->cmp[0] = T(0);
				this->cmp[1] = T(0);
				this->cmp[2] = T(0);
			}

			template <class T>
			EuclideanVector3D<T>::EuclideanVector3D(T a, T b, T c) {
				this->cmp[0] = a;
				this->cmp[1] = b;
				this->cmp[2] = c;
			}

			template <class T>
			EuclideanVector3D<T>::EuclideanVector3D(const EuclideanVector<T,3>& v) {
				this->cmp[0] = v.cmp[0];
				this->cmp[1] = v.cmp[1];
				this->cmp[2] = v.cmp[2];
			}

			template <class T>
			EuclideanVector3D<T> EuclideanVector3D<T>::crossProduct(const EuclideanVector3D<T>& vec) {
				EuclideanVector3D<T> result;

				result.cmp[0] = (this->cmp[1] * vec.cmp[2]) - (this->cmp[2] * vec.cmp[1]);
				result.cmp[1] = (this->cmp[2] * vec.cmp[0]) - (this->cmp[0] * vec.cmp[2]);
				result.cmp[2] = (this->cmp[0] * vec.cmp[1]) - (this->cmp[1] * vec.cmp[0]);

				return result;
			}

			template <class T>
			EuclideanVector3D<T> EuclideanVector3D<T>::crossProduct(const EuclideanVector<T,3>& vec) {
				EuclideanVector3D<T> v1(vec);
				EuclideanVector3D<T> result = this->crossProduct(v1);
				// EuclideanVector<T,3> res2;
				// res2.cmp[0] = result.cmp[0];
				// res2.cmp[1] = result.cmp[1];
				// res2.cmp[2] = result.cmp[2];
				// return res2;
				return result;
			}

			template <class T>
			EuclideanVector3D<T> crossProduct(const EuclideanVector<T,3>& vec1, const EuclideanVector<T,3>& vec2) {
				EuclideanVector3D<T> v1(vec1);
				EuclideanVector3D<T> v2(vec2);
				return v1.crossProduct(v2);
			}

			// Note: This is only going to work for floating point vectors due to the type restraints
			// (if we rotate an integer vector we're inevitably going to end up with a floating point vector)
			// Perhaps should just enforce the requirement that vectors should always be floating point.
			template <class T>
			void EuclideanVector3D<T>::rotateXAxisRadian(T radian) {
				T y = this->cmp[1];
				T z = this->cmp[2];
				
				// Rotation Matrix
				// X is unchanged
				
				// Y
				this->cmp[1] = (y * cos(radian)) - (z * sin(radian));
				
				// Z
				this->cmp[2] = (y * sin(radian)) + (z * cos(radian));
			}
			
			template <class T>
			void EuclideanVector3D<T>::rotateYAxisRadian(T radian) {
				T x = this->cmp[0];
				T z = this->cmp[2];
				
				// Rotation Matrix
				// X
				this->cmp[0] = (x * cos(radian)) + (z * sin(radian));
				
				// Y is unchanged
				
				// Z
				this->cmp[2] = (z * cos(radian)) - (x * sin(radian));
			}			
			
			template <class T>
			void EuclideanVector3D<T>::rotateZAxisRadian(T radian) {
				T x = this->cmp[0];
				T y = this->cmp[1];
				
				// Rotation Matrix
				// X
				this->cmp[0] = (x * cos(radian)) - (y * sin(radian));

				// Y				
				this->cmp[1] = (x * sin(radian)) + (y * cos(radian));
				
				// Z is unchanged		
			}
			
			template <class T>
			bool isVectorRangeIntersection(const EuclideanPoint<T,3>& x1, const EuclideanPoint<T,3>& x2,
												const EuclideanPoint<T,3>& x3, const EuclideanPoint<T,3>& x4)
			{
				// Needed to store result, but we discard it
				EuclideanPoint<T,3> intersectPoint(T(0.0), T(0.0), T(0.0));
				cupcfd::error::eCodes status = computeVectorRangeIntersection(x1, x2, x3, x4, intersectPoint);
				
				if(status == cupcfd::error::E_SUCCESS) {
					return true;
				}
				else {
					return false;
				}
			}

			template <class T>
			inline cupcfd::error::eCodes computeVectorRangeIntersection(const EuclideanPoint<T,3>& x1, const EuclideanPoint<T,3>& x2,
											 	  	  	  	  	  	  	const EuclideanPoint<T,3>& x3, const EuclideanPoint<T,3>& x4,
																		EuclideanPoint<T,3>& intersectPoint)
			{
				// ToDo: Do we want an extra Error Code for Colinear setups (since callee might want to change
				// behaviour of their code if it is)
				// Alternate: Add a separate function for testing for colinear
					
				// mathworld.wolfram.com/Line-LineIntersection.html
				// Goldman, R. "Intersection of Two Lines in Three-Space", Graphics Gems 1, Academic Press Pg. 304, 1990 
				// https://stackoverflow.com/questions/563198/how-do-you-detect-where-two-line-segments-intersect
			
				// Test there is a non-zero length for both vectors
				if((x1 == x2) && (x2 == x3) && (x3 == x4)) {					
					// All same point, therefore technically is intersecting at the point
					intersectPoint = x1;
					return cupcfd::error::E_SUCCESS;
				}
				
				if((x1 == x2) && (x3 == x4)) {
					// Both single points, but not same point since the previous check failed
					// Therefore must not be overlapping
					#ifdef DEBUG
						std::cout << "Both single points, but not same point since the previous check failed. Therefore must not be overlapping" << std::endl;
					#endif
					return cupcfd::error::E_GEOMETRY_NO_INTERSECT;
				}
				
				if((x1 == x2)) {
					// First range is a point, second one is not since previous checks failed
					// Therefore can only intersect if the point *is* the intersection point
					#ifdef DEBUG
						std::cout << "First range is a point, second one is not since previous checks failed. Therefore can only intersect if the point *is* the intersection point" << std::endl;
					#endif

					// Test whether the point lies on the line
					bool isIntersect = cupcfd::geometry::euclidean::isPointOnLine(x3, x4, x1);
					if(isIntersect) {
						intersectPoint = x1;
						return cupcfd::error::E_SUCCESS;
					}
					else {
						#ifdef DEBUG
							std::cout << "ERROR: isIntersect is false" << std::endl;
						#endif
						return cupcfd::error::E_GEOMETRY_NO_INTERSECT;
					}
				}
			
				if((x3 == x4)) {
					// Second range is a point, first one is not since previous checks failed
					// Therefore can only intersect if the point *is* the intersection point
					#ifdef DEBUG
						std::cout << "Second range is a point, first one is not since previous checks failed. Therefore can only intersect if the point *is* the intersection point" << std::endl;
					#endif

					// Test whether the point lies on the line
					bool isIntersect = cupcfd::geometry::euclidean::isPointOnLine(x1, x2, x3);
					
					if(isIntersect) {
						intersectPoint = x3;
						return cupcfd::error::E_SUCCESS;
					}
					else {
						#ifdef DEBUG
							std::cout << "ERROR: isIntersect is false" << std::endl;
						#endif
						return cupcfd::error::E_GEOMETRY_NO_INTERSECT;
					}
				}
			
				// Are all four points coplanar?
				// if they meet at the X,Y coordinates, and are coplanar, then other dimensions must also match
				EuclideanVector3D<T> tmp1 = x3 - x1;
				EuclideanVector3D<T> tmp2 = x2 - x1;
				EuclideanVector3D<T> tmp3 = x4 - x3;
				EuclideanVector3D<T> tmp4 = tmp2.crossProduct(tmp3);

				T coplanarCheck = tmp1.dotProduct(tmp4);
				if(!(arth::isEqual(coplanarCheck, (T) 0))) {
					// Not coplanar points
					#ifdef DEBUG
						std::cout << "not coplanar" << std::endl;
					#endif
					return cupcfd::error::E_GEOMETRY_NO_INTERSECT;
				}

				// Simultanious Equation Solving
				// If the points cross, then there exists a point on vec a = x2-x1 and vec b = (x4-x3)
				// where they meet.
				// The scalar multiplier from x1 and x3 to reach this point is r and s respectively.
				// Therefore we can make the following equations
				
				// x1_x + r.a_x = x3_x + s.b_x
				// x1_y + r.a_y = x3_y + s.b_y
				// x1_z + r.a_z = x3_z + s.b_z
				
				// Or Rearranging
				//r = (x3_x + s.b_x - x1_x) / a_x
				//r = (x3_y + s.b_y - x1_y) / a_y
				//r = (x3_z + s.b_z - x1_z) / a_z
				//s = (x1_x + r.a_x - x3_x) / b_x
				//s = (x1_y + r.a_y - x3_y) / b_y
				//s = (x1_z + r.a_z - x3_z) / b_z
				
				// Substituting and rearranging, we can get
				// (a) a_y.(x3_x + s.b_x - x1_x) = a_x.(x3_y + s.b_y - x1_y)
				
				// (x3_x * a_y) + (s * b_x * a_y) - (x1_x * a_y) = (a_x * x3_y) + (a_x * s * b_y) - (a_x * x1_y)
				
				// (x3_x * a_y) - (x1_x * a_y) + (a_x * x1_y) - (a_x * x3_y) =  (a_x * s * b_y) - (s * b_x * a_y)
				// (x3_x * a_y) - (x1_x * a_y) + (a_x * x1_y) - (a_x * x3_y) =  s((a_x * b_y) - (b_x * a_y))
				

				
				// (b) (x3_x + s.b_x - x1_x) / a_x = (x3_z + s.b_z - x1_z) / a_z
				// a_z * (x3_x + s.b_x - x1_x) = a_x * (x3_z + s.b_z - x1_z)
				
				// (a_z * x3_x) + (a_z * s * b_x) - (a_z * x1_x) = (a_x * x3_z) + (a_x * s * b_z) - (a_x * x1_z)
				// (a_z * x3_x) - (a_z * x1_x) - (a_x * x3_z) + (a_x * x1_z) = (a_x * s * b_z) - (a_z * s * b_x)
				// (a_z * x3_x) - (a_z * x1_x) - (a_x * x3_z) + (a_x * x1_z) = s((a_x * b_z) - (a_z * b_x))
				
				// (c) (x3_y + s.b_y - x1_y) / a_y = (x3_z + s.b_z - x1_z) / a_z
				// a_z * (x3_y + s.b_y - x1_y) = a_y * (x3_z + s.b_z - x1_z)
				
				// (a_z * x3_y) + (a_z * s * b_y) - (a_z * x1_y) = (a_y * x3_z) + (a_y * s * b_z) - (a_y * x1_z)
				// (a_z * x3_y) - (a_z * x1_y) - (a_y * x3_z) + (a_y * x1_z) = (a_y * s * b_z) - (a_z * s * b_y)
				// (a_z * x3_y) - (a_z * x1_y) - (a_y * x3_z) + (a_y * x1_z) = s * ((a_y * b_z) - (a_z * b_y))
				
				
				// Equations we can solve
				// s = ((x3_x * a_y) - (x1_x * a_y) + (x1_y * a_x) - (x3_y * a_x)) / ((a_x * b_y) - (b_x * a_y))
				// s = ((x3_x * a_z) - (x1_x * a_z) + (x1_z * a_x) - (x3_z * a_x)) / ((a_x * b_z) - (b_x * a_z))
				// s = ((x3_y * a_z) - (x1_y * a_z) + (x1_z * a_y) - (x3_z * a_y)) / ((a_y * b_z) - (b_y * a_z))
				
				// r = ((x1_x * b_y) - (x3_x * b_y) + (x3_y * b_x) - (x1_y * b_x)) / ((b_x * a_y) - (a_x * b_y))
				// r = ((x1_x * b_z) - (x3_x * b_z) + (x3_z * b_x) - (x1_z * b_x)) / ((b_x * a_z) - (a_x * b_z))
				// r = ((x1_y * b_z) - (x3_y * b_z) + (x3_z * b_y) - (x1_z * b_y)) / ((b_y * a_z) - (a_y * b_z))			
				
				// Challenges: Division by Zero, Parallel/Colinear vectors
				
				EuclideanVector3D<T> a = x2 - x1;
				EuclideanVector3D<T> b = x4 - x3;
				
				// Can test for parallel/colinear
				// If Parallel/Colinear then....
				if(a.isParallel(b)) {
					// Parallel, but colinear?
					if(colinearPoints(x1, x2, x3) && colinearPoints(x1, x2, x4)) {
						// Ranges are colinear - Do they overlap?
						// Since we know they lie on same line, we can do direct < or > operators
						// without worring about different values in different dimensions (they'll
						// all be greater or all be less than etc)
						
						if(((x1 >= x3) && (x1 <= x4)) || ((x1 >= x4) && (x1 <= x3))) {
							intersectPoint = x1;
							return cupcfd::error::E_SUCCESS;
						}
						else if(((x2 >= x3) && (x2 <= x4)) || ((x2 >= x4) && (x2 <= x3))) {
							intersectPoint = x2;
							return cupcfd::error::E_SUCCESS;
						}
						else {
							// Colinear, but not overlapping since neither the min nor max
							// of the first range lies in the second range
							#ifdef DEBUG
								std::cout << "colinear, but not overlapping" << std::endl;
							#endif
							return cupcfd::error::E_GEOMETRY_NO_INTERSECT;
						}
					}
					else {
						// Parallel, but not colinear so they never meet
						#ifdef DEBUG
							std::cout << "parallel, not not colinear" << std::endl;
						#endif
						return cupcfd::error::E_GEOMETRY_NO_INTERSECT;
					}
					
				}
				
				// If not parallel/colinear, then....
				// Trying carefully to avoid division by zero!
				// ToDo: Likely a much nicer way to check this in vector math (cross product etc)
				
				// Compute denominators till we find one that isn't zero....
				T r, s;
				
				bool rSolve = false;
				bool sSolve = false;
				
				if(rSolve == false && sSolve == false) {
					T denom1 = ((a.cmp[0] * b.cmp[1]) - (b.cmp[0] * a.cmp[1]));
					
					if(!(arth::isEqual(denom1, T(0.0)))) {
						s = ((x3.cmp[0] * a.cmp[1]) - (x1.cmp[0] * a.cmp[1]) + (x1.cmp[1] * a.cmp[0]) - (x3.cmp[1] * a.cmp[0])) / denom1;
						sSolve = true;
					}
				}			

				if(rSolve == false && sSolve == false) {
					T denom2 = ((a.cmp[0] * b.cmp[2]) - (b.cmp[0] * a.cmp[2]));
				
					if(!(arth::isEqual(denom2, T(0.0)))) {
						s = ((x3.cmp[0] * a.cmp[2]) - (x1.cmp[0] * a.cmp[2]) + (x1.cmp[2] * a.cmp[0]) - (x3.cmp[2] * a.cmp[0])) / denom2;
						sSolve = true;
					}
				}
				
				if(rSolve == false && sSolve == false) {
					T denom3 = ((a.cmp[1] * b.cmp[2]) - (b.cmp[1] * a.cmp[2]));
					
					if(!(arth::isEqual(denom3, T(0.0)))) {
						s = ((x3.cmp[1] * a.cmp[2]) - (x1.cmp[1] * a.cmp[2]) + (x1.cmp[2] * a.cmp[1]) - (x3.cmp[2] * a.cmp[1])) / denom3;
						sSolve = true;
					}
				}
				
				if(rSolve == false && sSolve == false) {
					T denom4 = ((b.cmp[0] * a.cmp[1]) - (a.cmp[0] * b.cmp[1]));
					
					if(!(arth::isEqual(denom4, T(0.0)))) {
						r = ((x1.cmp[0] * b.cmp[1]) - (x3.cmp[0] * b.cmp[1]) + (x3.cmp[1] * b.cmp[0]) - (x1.cmp[1] * b.cmp[0])) / denom4;
						rSolve = true;
					}
				}
				
				if(rSolve == false && sSolve == false) {
					T denom5 = ((b.cmp[0] * a.cmp[2]) - (a.cmp[0] * b.cmp[2]));
					
					if(!(arth::isEqual(denom5, T(0.0)))) {
						r = ((x1.cmp[0] * b.cmp[2]) - (x3.cmp[0] * b.cmp[2]) + (x3.cmp[2] * b.cmp[0]) - (x1.cmp[2] * b.cmp[0])) / denom5;
						rSolve = true;
					}
				}
				
				if(rSolve == false && sSolve == false) {
					T denom6 = ((b.cmp[1] * a.cmp[2]) - (a.cmp[1] * b.cmp[2]));
					
					if(!(arth::isEqual(denom6, T(0.0)))) {
						r = ((x1.cmp[1] * b.cmp[2]) - (x3.cmp[1] * b.cmp[2]) + (x3.cmp[2] * b.cmp[1]) - (x1.cmp[2] * b.cmp[1])) / denom6;
						rSolve = true;
					}
				}
				
				if(sSolve) {
					if(!(arth::isEqual(a.cmp[0], T(0.0)))) {
						//r = (x3_x + s.b_x - x1_x) / a_x
						r = (x3.cmp[0] + (s * b.cmp[0]) - x1.cmp[0]) / a.cmp[0];
					}
					else if(!(arth::isEqual(a.cmp[1], T(0.0)))) {
						//r = (x3_y + s.b_y - x1_y) / a_y
						r = (x3.cmp[1] + (s * b.cmp[1]) - x1.cmp[1]) / a.cmp[1];
					}
					else if(!(arth::isEqual(a.cmp[2], T(0.0)))) {
						//r = (x3_z + s.b_z - x1_z) / a_z
						r = (x3.cmp[2] + (s * b.cmp[2]) - x1.cmp[2]) / a.cmp[2];
					}
				}
				else if(rSolve) {
					if(!(arth::isEqual(b.cmp[0], T(0.0)))) {
						//s = (x1_x + r.a_x - x3_x) / b_x
						s = (x1.cmp[0] + (r * a.cmp[0]) - x3.cmp[0]) / b.cmp[0];
					}
					else if(!(arth::isEqual(b.cmp[1], T(0.0)))) {
						//s = (x1_y + r.a_y - x3_y) / b_y
						s = (x1.cmp[1] + (r * a.cmp[1]) - x3.cmp[1]) / b.cmp[1];
					}
					else if(!(arth::isEqual(b.cmp[2], T(0.0)))) {
						//s = (x1_z + r.a_z - x3_z) / b_z
						s = (x1.cmp[2] + (r * a.cmp[2]) - x3.cmp[2]) / b.cmp[2];
					}
				}
				else {
					// Both vectors were all zeroes - should never happen (catch above at point == point)
				}

				// Both r and s are known - check it holds for all three equations...
				// x1_x + r.a_x = x3_x + s.b_x
				// x1_y + r.a_y = x3_y + s.b_y
				// x1_z + r.a_z = x3_z + s.b_z
				
				if((r < 0) || (s < 0) || (r > 1) || (s > 1)) {
					// Have to travel more than the length of the range to reach the intersection point
					#ifdef DEBUG
						std::cout << "Have to travel more than the length of the range to reach the intersection point" << std::endl;
					#endif
					return cupcfd::error::E_GEOMETRY_NO_INTERSECT;
				}
				
				// Equation 1
				T eqn1Lhs = x1.cmp[0] + (r * a.cmp[0]);
				T eqn1Rhs = x3.cmp[0] + (s * b.cmp[0]);
				
				T eqn2Lhs = x1.cmp[1] + (r * a.cmp[1]);
				T eqn2Rhs = x3.cmp[1] + (s * b.cmp[1]);	
				
				T eqn3Lhs = x1.cmp[2] + (r * a.cmp[2]);
				T eqn3Rhs = x3.cmp[2] + (s * b.cmp[2]);								
				
				if(arth::isEqual(eqn1Lhs, eqn1Rhs) && arth::isEqual(eqn2Lhs, eqn2Rhs) && arth::isEqual(eqn3Lhs, eqn3Rhs)) {
					intersectPoint = x1 + r * a;
					return cupcfd::error::E_SUCCESS;
				}
				else {
					#ifdef DEBUG
						std::cout << "At end of routine, no intersection" << std::endl;
					#endif
					return cupcfd::error::E_GEOMETRY_NO_INTERSECT;
				}


				/*
				else {
					// If Both Vectors are all zeroes, then that means they would have been caught by the
					// point check above.
					
					// If Both Vectors have two zero components, then they're either parallel/colinear
					// (which is caught), not coplanar (which is caught) or they are orthagonal
					(e.g. (1,0,0) and (0,0,1).
				}		
				*/
			}			

			template <class T>
			bool isPointOnLine(const EuclideanPoint<T,3>& x1, const EuclideanPoint<T,3>& x2,
									  const EuclideanPoint<T,3>& p)
			{
				// Check p != x1
				if(p == x1) {
					return true;
				}
			
				// Create a vector from x1 to p
				EuclideanVector3D<T> x1p = p - x1;
				
				// Create a vector from x1 to x2
				EuclideanVector3D<T> x1x2 = x2 - x1;
				
				// If the length of x1 to p is
				// (a) The same direction(sign + parallel vector)
				// (b) A length less than or equal to x1->x2
				// Then the point must lie on the line
				
				// First Check: Make sure it's parallel
				if(!(x1p.isParallel(x1x2))) {
					// std::cout << "ERROR: isPointOnLine(): parallel check failed" << std::endl;
					// HARD_CHECK_ECODE(cupcfd::error::E_GEOMETRY_LOGIC_ERROR)
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

template class cupcfd::geometry::euclidean::EuclideanVector3D<float>;

template class cupcfd::geometry::euclidean::EuclideanVector3D<double>;
// template cupcfd::geometry::euclidean::EuclideanVector3D<double>::EuclideanVector3D(const cupcfd::geometry::euclidean::EuclideanVector<T,3>& v);
// template cupcfd::geometry::euclidean::EuclideanVector3D<double>::rotateYAxisRadian(double radians);

#endif
