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

#ifndef CUPCFD_GEOMETRY_EUCLIDEAN_VECTOR3D_INCLUDE_H
#define CUPCFD_GEOMETRY_EUCLIDEAN_VECTOR3D_INCLUDE_H

#include "Error.h"
#include "MPIUtility.h"
#include "EuclideanPoint.h"
#include "Matrix.h"

namespace cupcfd
{
	namespace geometry
	{
		namespace euclidean
		{
			/**
			 * Stores data and operators for 3D vectors in a euclidean space.
			 *
			 * This classes is also suitable for parallel communication via
			 * the MPI interfaces.
			 *
			 * @tparam T The data type of the coordinate system
			 */
			template <class T>
			class EuclideanVector3D : public EuclideanVector<T,3>
			{
				public:
					EuclideanVector3D();
					EuclideanVector3D(T a, T b, T c);
					EuclideanVector3D(const EuclideanVector<T,3>& v);

					operator EuclideanVector<T,3>() const  {
						EuclideanVector<T,3> base;
						for (uint i=0; i<3; i++) {
							base.cmp[i] = this->cmp[i];
						}
						return base;
					}

					/**
					 * Compute the vector that is the cross product of this vector and another vector.
					 * This is currently only defined for 3D Vectors, so is not a class function
					 *
					 * @param vec The vector to compute the cross product against
					 *
					 * @return The cross-product vector
					 */
					__attribute__((warn_unused_result))
					EuclideanVector3D<T> crossProduct(const EuclideanVector3D<T>& vec);

					/**
					 * Rotate vector by specified radians about the X axis.
					 *
					 * @param radian The number of radians to rotate.
					 */
					void rotateXAxisRadian(T radian);

					/**
					 * Rotate vector by specified radians about the Y axis.
					 *
					 * @param radian The number of radians to rotate.
					 */
					void rotateYAxisRadian(T radian);

					/**
					 * Rotate vector by specified radians about the Z axis
					 *
					 * @param radian The number of radians to rotate.
					 */
					void rotateZAxisRadian(T radian);

			};


			template <class T>
			__attribute__((warn_unused_result))
			EuclideanVector3D<T> crossProduct(const EuclideanVector<T,3>& vec1, const EuclideanVector<T,3>& vec2);

			/**
			 * Compute whether two vectors intersect with one another between the provided points in 3D space.
			 *
			 * If the vectors are colinear and overlap in the point range (partially or fully), this is considered as intersecting.
			 *
			 * If the vectors are not coplanar, they are not considered as intersecting (they must meet in all three dimensions)
			 *
			 * If the lines would intersect, but the intersection point is not in both of the ranges of x1->x2 and x3->x4, they
			 * are not considered as intersecting
			 *
			 * @param x1 The origin point of the first vector
			 * @param x2 The destination point of the first vector
			 * @param x3 The origin point of the second vector
			 * @param x4 The destination point of the second vector
			 *
			 * @return Return true if the vectors intersect
			 */
			template <class T>
			__attribute__((warn_unused_result))
			bool isVectorRangeIntersection(const EuclideanPoint<T,3>& x1, const EuclideanPoint<T,3>& x2,
											const EuclideanPoint<T,3>& x3, const EuclideanPoint<T,3>& x4);
	
			/**
			 * Compute the intersect of two lines defined by a range in 3D space, if it exists
			 *
			 * If the vectors are colinear and overlap in the point range (partially or fully), this is considered as intersecting.
			 *
			 * If the vectors are not coplanar, they are not considered as intersecting (they must meet in all three dimensions)
			 *
			 * If the lines would intersect, but the intersection point is not in both of the ranges of x1->x2 and x3->x4, they
			 * are not considered as intersecting
			 *
			 * @param x1 The origin point of the first vector
			 * @param x2 The destination point of the first vector
			 * @param x3 The origin point of the second vector
			 * @param x4 The destination point of the second vector
			 * @param intersectPoint A reference to the point object that will be overwritten with the result, if it exists
			 *
			 * @return An error status indicating the success or failure of the operation
			 * @retval cupcfd::error::E_SUCCESS The method completed successfully
			 * @retval cupcfd::error::E_GEOMETRY_NO_INTERSECT No intersection exists
			 */
			template <class T>
			__attribute__((warn_unused_result))
			cupcfd::error::eCodes computeVectorRangeIntersection(const EuclideanPoint<T,3>& x1, const EuclideanPoint<T,3>& x2, 
																const EuclideanPoint<T,3>& x3, const EuclideanPoint<T,3>& x4, 
																EuclideanPoint<T,3>& intersectPoint);

			/**
			 * Test whether a point lies on a 3D line delinated by two other points
			 *
			 * @param x1 The first point of the line
			 * @param x2 The second point of the line
			 * @param p The point to test the position of
			 *
			 * @return Return true if the vectors intersect
			 */
			template <class T>
			__attribute__((warn_unused_result))
			bool isPointOnLine(const EuclideanPoint<T,3>& x1, const EuclideanPoint<T,3>& x2,
								const EuclideanPoint<T,3>& p);

			template <class T>
			static Matrix<T,3,3> calculateRotationMatrix(
								const EuclideanVector3D<T>& source, 
								const EuclideanVector3D<T>& target);

		} // namespace euclidean
	} // namespace geometry
} // namespace cupcfd

// Include Header Level Definitions
// ToDo: Would like to move friend functions into this file if the compiler errors are fixed.
#include "EuclideanVector3D.ipp"


#endif
