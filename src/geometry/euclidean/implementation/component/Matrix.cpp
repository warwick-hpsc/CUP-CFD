/*
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * This class contains the implementation of the Matrix class.
 */

#include "Matrix.h"

#include <cmath>

namespace cupcfd
{
	namespace geometry
	{
		namespace euclidean
		{
			// Initialise Static Variables
			template <class T, unsigned int R, unsigned int C>
			Matrix<T,R,C>::Matrix() {
				for(uint i = 0; i < R; i++) {
					for(uint j = 0; j < C; j++) {
						this->mat[i][j] = T(0);
					}
				}
			}

			template <class T, unsigned int R, unsigned int C>
			EuclideanPoint<T,C> Matrix<T,R,C>::operator*(const EuclideanPoint<T,C>& v) {
				EuclideanPoint<T,C> v2;
				v2.cmp[0] = this->mat[0][0]*v.cmp[0] + this->mat[0][1]*v.cmp[1] + this->mat[0][2]*v.cmp[2];
				v2.cmp[1] = this->mat[1][0]*v.cmp[0] + this->mat[1][1]*v.cmp[1] + this->mat[1][2]*v.cmp[2];
				v2.cmp[2] = this->mat[2][0]*v.cmp[0] + this->mat[2][1]*v.cmp[1] + this->mat[2][2]*v.cmp[2];
				return v2;
			}

			template <class T, unsigned int R, unsigned int C>
			EuclideanVector<T,C> Matrix<T,R,C>::operator*(const EuclideanVector<T,C>& v) {
				EuclideanVector<T,C> v2;
				v2.cmp[0] = this->mat[0][0]*v.cmp[0] + this->mat[0][1]*v.cmp[1] + this->mat[0][2]*v.cmp[2];
				v2.cmp[1] = this->mat[1][0]*v.cmp[0] + this->mat[1][1]*v.cmp[1] + this->mat[1][2]*v.cmp[2];
				v2.cmp[2] = this->mat[2][0]*v.cmp[0] + this->mat[2][1]*v.cmp[1] + this->mat[2][2]*v.cmp[2];
				return v2;
			}

			template <class T, unsigned int R, unsigned int C>
			void Matrix<T,R,C>::print() {
				for (int i=0; i<R; i++) {
					printf("|");
					for (int j=0; j<C; j++) {
						// printf(" %.3f", this->mat[i][j]);
						printf(" %.10f", this->mat[i][j]);
					}
					printf(" |\n");
				}
			}

			// Explicit Instantiation
			// ToDo: This is kind of brittle for template usage
			// We could probably move everything to the header and avoid explicit instantiation entirely....
			template class Matrix<float, 3, 3>;
			template class Matrix<double, 3, 3>;
		}
	}
}
