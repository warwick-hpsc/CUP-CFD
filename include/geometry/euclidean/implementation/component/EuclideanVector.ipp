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

#ifndef CUPCFD_GEOMETRY_EUCLIDEAN_VECTOR_IPP_H
#define CUPCFD_GEOMETRY_EUCLIDEAN_VECTOR_IPP_H

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
			template <class T, unsigned int N>
			inline void EuclideanVector<T,N>::operator=(const EuclideanVector<T,N>& source) {
				for(uint i = 0; i < N; i++) {
					this->cmp[i] = source.cmp[i];
				}
			}

			template <class T, unsigned int N>
			inline void EuclideanVector<T,N>::operator=(const T scalar) {
				for(uint i = 0; i < N; i++) {
					this->cmp[i] = scalar;
				}
			}

			template <class T, unsigned int N>
			inline void EuclideanVector<T,N>::operator+=(const EuclideanVector<T,N>& source) {
				for(uint i = 0; i < N; i++) {
					this->cmp[i] += source.cmp[i];
				}
			}

			template <class T, unsigned int N>
			inline void EuclideanVector<T,N>::operator+=(const T scalar) {
				for(uint i = 0; i < N; i++) {
					this->cmp[i] += scalar;
				}
			}

			template <class T, unsigned int N>
			inline void EuclideanVector<T,N>::operator*=(const EuclideanVector<T,N>& source) {
				for(uint i = 0; i < N; i++) {
					this->cmp[i] *= source.cmp[i];
				}
			}

			template <class T, unsigned int N>
			inline void EuclideanVector<T,N>::operator*=(const T scalar) {
				for(uint i = 0; i < N; i++) {
					this->cmp[i] *= scalar;
				}
			}

			template <class T, unsigned int N>
			inline bool EuclideanVector<T,N>::operator==(const EuclideanVector<T,N>& cmpVec) {
				for(uint i = 0; i < N; i++) {
					if(!(arth::isEqual(this->cmp[i], cmpVec.cmp[i]))) {
						return false;
					}
				}
				
				return true;
			}

			template <class T, unsigned int N>	
			EuclideanVector<T,N> EuclideanVector<T,N>::computeOrthagonalVector() {
				// Orthagonal Vector has a Dot Product of 0 with this Vector
				// I.e. (u.x * v.x) + (u.y * v.y) + .... (u.n-1 * v.n-1) == 0
				// Since we know u, pick 1 for all v components except one, and solve
				// Need to ensure that the 'unknown' in v does not have a zero component in u, else it is not
				// part of the equation and we'd end up with 1 + 1 = 0 or something incorrect
				
				// Set all components to 1 (this will temporarily give the wrong result)
				T sum = T(0);

				for(uint i = 0; i < N; i++) {
					sum = this->cmp[i] + sum;
				}
				
				// Find a non zero component
				uint index;
				bool found = false;
				for(uint i = 0; i < N; i++) {
					if(!(arth::isEqual(this->cmp[i], (T) 0))) {
						index = i;
						found = true;
						break;
					}
				}
				if (!found) {
					HARD_CHECK_ECODE(cupcfd::error::E_GEOMETRY_LOGIC_ERROR)
				}
				
				// Now the non-zero component is known, subtract that part from the result and set it back to 'unknown' coefficient,
				// then solve so that the component * its coefficient cancels out the rest to give a sum of zero.
				// I.e. sum + coefficient * cmp[index] == 0
				sum -= this->cmp[index];
				
				T coefficient = (sum * T(-1)) / this->cmp[index];
				
				// Create and return the vector
				EuclideanVector<T,N> result(T(1));
				result.cmp[index] = coefficient;
				
				return result;
			}
			
			// ToDo: Need a better way of handling case where vectors are uniformally zero
			template <class T, unsigned int N>
			inline bool EuclideanVector<T,N>::isParallel(EuclideanVector<T,N>& cmpVec) {
				// If components are equal, then vector will always be parallel (colinear)
				if(*this == cmpVec) {
					return true;
				}
			
				// If parallel, then vec1 = vec2 * scalar since only the magnitude/sign should differ
				// not the angle of direction
				
				// Need to find a comparison scalar at first non-zero value of comparison vector
				// to avoid division by zero
				T cmpScalar;
				bool found = false;
				
				for(uint i = 0; i < N; i++) {
					if(!(arth::isEqual(cmpVec.cmp[i], T(0)))) {
						cmpScalar = this->cmp[i] / cmpVec.cmp[i];
						found = true;
						break;
					}
				}

				if(!found) {
					// All components of cmpVec are zero....
					// Technically this means both vectors are zero, which makes them both parallel technically?
					// ToDo: Want a better way to handle this - make an error code variant of the function?
					return true;
				}

				// Int's are sufficient for vector dimensions, no need to template it
				for(uint i = 0; i < N; i++) {
					// Four outcomes
					// (a) Both components are zero - Continue
					// (b) Scalar is the same - Continue
					// (c) Scalar is not the same - Not Parallel
					// (d) One component is zero, the other is not - Not Parallel
				
					if(arth::isEqual(cmpVec.cmp[i], T(0))) {
						if(!(arth::isEqual(this->cmp[i], T(0)))) {
							return false;
						}
						else {
							// Skip rest, we don't want division by zero and both components are zero
							continue;
						}
					}
				
					// Scalars should be the same for all other dimensional components, if not then not parallel
					T scalar = this->cmp[i] / cmpVec.cmp[i];
					
					if(!(arth::isEqual(scalar, cmpScalar))) {
						return false;
					}
				}
				
				// All scalars matched (or both components are zero), so must be parallel
				return true;
			}
			
			template <class T, unsigned int N>
			bool colinearPoints(EuclideanPoint<T,N> a, EuclideanPoint<T,N> b, EuclideanPoint<T,N> c) {
				// Three points are colinear if ab is parallel to ac (since they are parallel and have a common shared point)
				
				EuclideanVector<T,N> ab = b - a;
				EuclideanVector<T,N> bc = c - b;
				
				if(ab.isParallel(bc)) {
					return true;
				}
				
				return false;
			}

			template <class T, unsigned int N>
			inline void EuclideanVector<T,N>::print() const {
				if (N > 0) {
					std::cout << "[ " << this->cmp[0];
				    for(uint i = 1; i < N; i++) {
					    std::cout << ", " << this->cmp[i];
				    }
				    std::cout << " ]";
				}
			}
			
			
			

			template <class T, unsigned int N>
			inline cupcfd::error::eCodes EuclideanVector<T,N>::getMPIType(MPI_Datatype * dType) {
				if(!(this->isRegistered())) {
					return cupcfd::error::E_MPI_DATATYPE_UNREGISTERED;
				}
			
				*dType = EuclideanVector<T,N>::mpiType;

				return cupcfd::error::E_SUCCESS;
			}
			
			template <class T, unsigned int N>
			inline bool EuclideanVector<T,N>::isRegistered() {
				return EuclideanVector<T,N>::mpiDataTypeReg;
			}
				
		} // namespace euclidean
	} // namespace geometry
} // namespace cupcfd

// Include Header Level Definitions
#include "EuclideanVector.ipp"

#endif
