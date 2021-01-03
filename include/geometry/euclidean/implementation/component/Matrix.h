/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Contains the declarations for the EuclideanVector3D class.
 */

#ifndef CUPCFD_GEOMETRY_MATRIX_INCLUDE_H
#define CUPCFD_GEOMETRY_MATRIX_INCLUDE_H

#include "EuclideanVector.h"

namespace cupcfd
{
	namespace geometry
	{
		namespace euclidean
		{
			/**
			 * Stores data and operators for matrics in a euclidean space.
			 *
			 * This classes is also suitable for parallel communication via
			 * the MPI interfaces.
			 *
			 * @tparam T The data type of the coordinate system
			 * @tparam R Number of rows
			 * @tparam C Number of columns
			 */
			template <class T, unsigned int R, unsigned int C>
			class Matrix
			{
				public:
					Matrix();

					T mat[R][C];

					EuclideanPoint<T,C> operator*(const EuclideanPoint<T,C>& v);
					EuclideanVector<T,C> operator*(const EuclideanVector<T,C>& v);

					void print();
			};

		} // namespace euclidean
	} // namespace geometry
} // namespace cupcfd

// Include Header Level Definitions
// ToDo: Would like to move friend functions into this file if the compiler errors are fixed.
#include "Matrix.ipp"


#endif
