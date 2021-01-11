/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Contains the header level definitions for the EuclideanVector3D class.
 */

#ifndef CUPCFD_GEOMETRY_EUCLIDEAN_VECTOR3D_IPP_H
#define CUPCFD_GEOMETRY_EUCLIDEAN_VECTOR3D_IPP_H

#include "ArithmeticKernels.h"
#include "Matrix.h"
#include "LineSegment3D.h"

#include <iostream>
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
			static Matrix<T,3,3> calculateRotationMatrix(
								const EuclideanVector3D<T>& source, 
								const EuclideanVector3D<T>& target) {
				EuclideanVector3D<T> v1(source);
				v1.normalise();
				EuclideanVector3D<T> v2(target);
				v2.normalise();

				// https://en.wikipedia.org/wiki/Transformation_matrix#Rotation_2
				T cosine = v1.dotProduct(v2);
				EuclideanVector3D<T> axis;
				T sine;
				if (cosine == T(-1) || cosine == T(1)) {
					// Cross Product will fail to find a rotation axis, because there
					// are an infinite number. Fortunately can handle this edge case, 
					// just use any perpendicular vector as axis:
					v2.cmp[0] = v1.cmp[1];
					v2.cmp[1] = v1.cmp[2];
					v2.cmp[2] = v1.cmp[0];
					axis = v1.crossProduct(v2);
					axis.normalise();
					sine = T(0);
				} else {
					axis = v1.crossProduct(v2);
					sine = axis.length();
					axis.normalise();
				}
				T oneMinusCosine = T(1) - cosine;

				Matrix<T,3,3> m;
				T x = axis.cmp[0];
				T y = axis.cmp[1];
				T z = axis.cmp[2];
				m.mat[0][0] = x*x*oneMinusCosine + cosine;
				m.mat[0][1] = y*x*oneMinusCosine - sine*z;
				m.mat[0][2] = z*x*oneMinusCosine + sine*y;
				m.mat[1][0] = x*y*oneMinusCosine + sine*z;
				m.mat[1][1] = y*y*oneMinusCosine + cosine;
				m.mat[1][2] = z*y*oneMinusCosine - sine*x;
				m.mat[2][0] = x*z*oneMinusCosine - sine*y;
				m.mat[2][1] = y*z*oneMinusCosine + sine*x;
				m.mat[2][2] = z*z*oneMinusCosine + cosine;

				return m;
			}
		}
	}
}

#endif
