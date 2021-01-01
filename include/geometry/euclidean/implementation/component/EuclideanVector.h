/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Contains the declarations for the EuclideanVector class.
 */

#ifndef CUPCFD_GEOMETRY_EUCLIDEAN_VECTOR_INCLUDE_H
#define CUPCFD_GEOMETRY_EUCLIDEAN_VECTOR_INCLUDE_H

#include "Error.h"
#include "MPIUtility.h"
#include "EuclideanPoint.h"

namespace cupcfd
{
	namespace geometry
	{
		namespace euclidean
		{
			// Forward Declaration
			template <class T, unsigned int N>
			class EuclideanPoint;

			/**
			 * Stores data and operators for vectors in a euclidean space.
			 *
			 * This classes is also suitable for parallel communication via
			 * the MPI interfaces.
			 *
			 * @tparam T The data type of the coordinate system
			 * @tparam N The dimension of the coordinate space (e.g. 2 for 2D, 3 for 3D...)
			 */
			template <class T, unsigned int N>
			class EuclideanVector : public cupcfd::comm::mpi::CustomMPIType
			{
				// Asserts
				static_assert(N > 1, "Cannot have zero or negative dimension vectors");

				public:
					// === Static Variables ===

					/** Storage for the MPI Type returned upon registration of this class with the MPI library **/
					static MPI_Datatype mpiType;

					/** Track whether the datatype has been registered with the MPI library**/
					static bool mpiDataTypeReg;

					// === Members ===

					/** N-Dimensional Components of Vector **/
					T cmp[N];

					// === Constructors/Deconstructors ===

					/**
					 * Default constructor. Sets all vector values to zero.
					 */
					EuclideanVector();

					/**
					 * Constructor. Sets all vector values to the scalar.
					 */
					EuclideanVector(T scalar);

					/**
					 * Constructor. Sets the components to matching values of the provided params
					 *
					 * @param src A list of parameters to initialise the vector components
					 */
					template<class...Args>
					EuclideanVector(Args...src);

					/**
					 * Default Deconstructor
					 */
					~EuclideanVector();

					// === Static Methods ===

					// === Concrete Methods ===

					/**
					 * Deep copies the x and y values from a source vector into this vector object
					 *
					 * @param source The source vector to copy from
					 *
					 * @return Nothing
					 */
					inline void operator=(const EuclideanVector<T,N>& source);

					/**
					 * Sets each component of this vector to the scalar value
					 *
					 * @param scalar The value to set each vector component to
					 *
					 * @return Nothing
					 */
					inline void operator=(const T scalar);

					/**
					 * Element-wise add source vector onto this vector
					 *
					 * @param source The source vector to add
					 *
					 * @return Nothing
					 */
					inline void operator+=(const EuclideanVector<T,N>& source);

					/**
					 * Adds the scalar value to each component of this vector
					 *
					 * @param scalar The value to add to all three components
					 *
					 * @return Nothing
					 */
					inline void operator+=(const T scalar);

					/**
					 * Element-wise multiply source vector with this vector, overwriting this vector
					 *
					 * @param source The source vector to multiply with
					 *
					 * @return Nothing
					 */
					inline void operator*=(const EuclideanVector<T,N>& source);

					/**
					 * Multiply the scalar value with each component of this vector, overwriting this vector
					 *
					 * @param scalar The value to multiply with all three components
					 *
					 * @return Nothing
					 */
					inline void operator*=(const T scalar);

					/**
					 * Tests whether each dimensional component of the two vectors is equal,
					 * within a small threshold to accounting for floating point.
					 *
					 * Note: this only matches on exact values, it does NOT test for colinear vectors,
					 * parallel vectors, same direction etc etc.
					 *
					 * @param cmpVec This vector to compare against
					 *
					 * @return Return true if the vectors have equal components
					 */
					inline bool operator==(const EuclideanVector<T,N>& cmpVec);

					/**
					 * Add the two vectors together to obtain the vector that is the combination of the two
					 *
					 * @param vec1 The first source vector to add
					 * @param vec2 The second source vector to add
					 *
					 * @return The combined vector
					 */
					__attribute__((warn_unused_result))
					friend inline EuclideanVector<T,N> operator+(const EuclideanVector<T,N>& vec1, const EuclideanVector<T,N>& vec2)
					{
						EuclideanVector<T,N> result;

						for(uint i = 0; i < N; i++)
						{
							result.cmp[i] = vec1.cmp[i] + vec2.cmp[i];
						}

						return result;
					}

					/**
					 * Add the scalar value to both components of the vector
					 *
					 * @param vec The source vector
					 * @param scalar The scalar to add
					 *
					 * @return The vector with the scalar added
					 */
					__attribute__((warn_unused_result))
					friend inline EuclideanVector<T,N> operator+(const EuclideanVector<T,N>& vec, T scalar)
					{
						EuclideanVector<T,N> result;

						for(uint i = 0; i < N; i++)
						{
							result.cmp[i] = vec.cmp[i] + scalar;
						}

						return result;
					}

					/**
					 * Add the scalar value to both components of the vector
					 *
					 * @param vec The source vector
					 * @param scalar The scalar to add
					 *
					 * @return The vector with the scalar added
					 */
					__attribute__((warn_unused_result))
					friend inline EuclideanVector<T,N> operator+(T scalar, const EuclideanVector<T,N>& vec)
					{
						EuclideanVector<T,N> result;

						for(uint i = 0; i < N; i++)
						{
							result.cmp[i] = scalar + vec.cmp[i];
						}

						return result;
					}

					/**
					 * Subtract the second vector from the first to get the combined vector that describes this displacement
					 *
					 * @param vec1 The vector to subtract from
					 * @param vec2 The vector to subtract
					 *
					 * @return The displacement vector that is vec1 - vec2
					 */
					__attribute__((warn_unused_result))
					friend inline EuclideanVector<T,N> operator-(const EuclideanVector<T,N>& vec1, const EuclideanVector<T,N>& vec2)
					{
						EuclideanVector<T,N> result;

						for(uint i = 0; i < N; i++)
						{
							result.cmp[i] = vec1.cmp[i] - vec2.cmp[i];
						}

						return result;
					}

					/**
					 * Subtract the scalar value from the components of the vector to get a new vector
					 *
					 * @param vec The source vector
					 * @param scalar The scalar to subtract
					 *
					 * @return The vector with the scalar subtracted
					 */
					__attribute__((warn_unused_result))
					friend inline EuclideanVector<T,N> operator-(const EuclideanVector<T,N>& vec, T scalar)
					{
						EuclideanVector<T,N> result;

						for(uint i = 0; i < N; i++)
						{
							result.cmp[i] = vec.cmp[i] - scalar;
						}

						return result;
					}

					/**
					 * Subtract components of the vector from the scalar value to get a new vector
					 *
					 * @param vec The source vector
					 * @param scalar The scalar to subtract
					 *
					 * @return The vector with the scalar subtracted
					 */
					__attribute__((warn_unused_result))
					friend inline EuclideanVector<T,N> operator-(T scalar, const EuclideanVector<T,N>& vec)
					{
						EuclideanVector<T,N> result;

						for(uint i = 0; i < N; i++)
						{
							result.cmp[i] = scalar - vec.cmp[i];
						}

						return result;
					}

					/**
					 * Multiply the scalar value against all components of the vector to get a new vector
					 *
					 * @param vec The source vector
					 * @param scalar The scalar to multiply by
					 *
					 * @return The vector scaled up by the scalar multiplier
					 */
					friend inline EuclideanVector<T,N> operator*(const EuclideanVector<T,N>& vec, T scalar)
					{
						EuclideanVector<T,N> result;

						for(uint i = 0; i < N; i++)
						{
							result.cmp[i] = vec.cmp[i] * scalar;
						}

						return result;
					}

					/**
					 * Multiply the scalar value against all components of the vector to get a new vector
					 *
					 * @param vec The source vector
					 * @param scalar The scalar to multiply by
					 *
					 * @return The vector scaled up by the scalar multiplier
					 */
					__attribute__((warn_unused_result))
					friend inline EuclideanVector<T,N> operator*(T scalar, const EuclideanVector<T,N>& vec)
					{
						EuclideanVector<T,N> result;

						for(uint i = 0; i < N; i++)
						{
							result.cmp[i] = scalar * vec.cmp[i];
						}

						return result;
					}

					/**
					 * Divide all components of the vector by the scalar to get a new vector
					 *
					 * @param vec The source vector
					 * @param scalar The scalar to divide by
					 *
					 * @return The vector divided up by the scalar
					 */
					__attribute__((warn_unused_result))
					friend inline EuclideanVector<T,N> operator/(const EuclideanVector<T,N>& vec, T scalar)
					{
						EuclideanVector<T,N> result;

						for(uint i = 0; i < N; i++)
						{
							result.cmp[i] = vec.cmp[i] / scalar;
						}

						return result;
					}

					/**
					 * Divide the scalar by all components of the vector to get a new vector
					 *
					 * @param vec The source vector
					 * @param scalar The scalar to divide by
					 *
					 * @return The vector divided up by the scalar
					 */
					__attribute__((warn_unused_result))
					friend inline EuclideanVector<T,N> operator/(T scalar, const EuclideanVector<T,N>& vec)
					{
						EuclideanVector<T,N> result;

						for(uint i = 0; i < N; i++)
						{
							result.cmp[i] = scalar / vec.cmp[i];
						}

						return result;
					}

					/**
					 * Compute the dot-product of this vector and another vector
					 *
					 * @param vec The vector to compute the dot-product against
					 * @param scalar A pointer to the location where the dot-product result will be stored
					 *
					 * @return An error status indicating the success or failure of the operation
					 * @retval cupcfd::error::E_SUCCESS The method completed successfully
					 */
					// __attribute__((warn_unused_result))
					// cupcfd::error::eCodes dotProduct(const EuclideanVector<T,N>& vec, T * scalar);

					/**
					 * Compute the dot-product of this vector and another vector
					 *
					 * @param vec The vector to compute the dot-product against
					 *
					 * @return The dot-product
					 */
					__attribute__((warn_unused_result))
					T dotProduct(const EuclideanVector<T,N>& vec);


					// ToDo: Specify float/double explicitly since length will always be floating-point, even
					// for integer systems
					// Can we add an extra template type for this to reduce code repetition?

					// ToDo: Could rename this from length to norm
					/**
					 * Compute the distance covered by this vector
					 *
					 * @param length A pointer to the location updated by the function to contain the distance of the vector.
					 *
					 * @return length
					 */
					__attribute__((warn_unused_result))
					double length();

					/**
					 * Compute the normalised (length 1 in same direction) vector of an input vector.
					 * This method updates the vector in place, meaning that the result are only valid
					 * for floating-type vectors (since the value will lie between 0 and 1)
					 */
					void normalise();

					/**
					 * Compute the normalised (length 1 in same direction) vector of an input vector.
					 * This method updates the provided vector, meaning that the result are only valid
					 * for floating-type vectors (since the value will lie between 0 and 1)
					 *
					 * @param result A referenced vector object that will have its values modified to the
					 * normalised vector
					 */
					void normalise(cupcfd::geometry::euclidean::EuclideanVector<T,N>& result);


					/**
					 * Compute a orthagonal vector to this vector.
					 * Orthagonal vector direction is to the 'right' of this vector.
					 *
					 * @return An orthagonal vector.
					 */
					__attribute__((warn_unused_result))
					cupcfd::geometry::euclidean::EuclideanVector<T,N> computeOrthagonalVector();


					/**
					 * Test if a vector is parallel with this vector
					 *
					 * @param cmpVec The vector to compare against
					 *
					 * @return Return true if the vector is parallel
					 */
					__attribute__((warn_unused_result))
					inline bool isParallel(EuclideanVector<T,N>& b);

					/**
					 * Print point data to stdout, useful for debugging
					 */
					void print() const;


					// === Inherited Overloads ===

					__attribute__((warn_unused_result))
					inline cupcfd::error::eCodes getMPIType(MPI_Datatype * dType);

					__attribute__((warn_unused_result))
					cupcfd::error::eCodes registerMPIType();

					__attribute__((warn_unused_result))
					cupcfd::error::eCodes deregisterMPIType();

					__attribute__((warn_unused_result))
					inline bool isRegistered();
			};

			// ToDo: Would prefer to have this as a member of the vector class, but this function makes no sense
			// for 2D vectors, and we don't use 4D vectors or above, so we will have a specialised function instead

			/**
			 * Test if three points are colinear
			 *
			 * @param a First point
			 * @param b Second point
			 * @param c Third point
			 *
			 * @return Return true if they are colinear
			 */
			template <class T, unsigned int N>
			__attribute__((warn_unused_result))
			bool colinearPoints(EuclideanPoint<T,N> a, EuclideanPoint<T,N> b, EuclideanPoint<T,N> c);

			/**
			 * Compute the vector that is the cross product of this vector and another vector.
			 * This is currently only defined for 3D Vectors, so is not a class function
			 *
			 * @param vec The vector to compute the cross product against
			 *
			 * @return The cross-product vector
			 */
			// template <class T>
			// __attribute__((warn_unused_result))
			// EuclideanVector<T,3> crossProduct(const EuclideanVector<T,3>& vec1, const EuclideanVector<T,3>& vec2);

			/**
			 * Rotate a 3D vector some amount of radians about the X axis.
			 * Vector is updated in-place.
			 *
			 * @param radian The number of radians to rotate.
			 * Positive: If Facing from negative to positive on X-Axis, goes anti-clockwise
			 * Negative: If Facing from negative to positive on X-Axis, goes clockwise
			 * @param vec The vector to rotate
			 *
			 * @return Nothing
			 */
			// template <class T>
			// inline void rotateXAxisRadian(T radian, EuclideanVector<T,3>& vec);

			/**
			 * Rotate a 3D vector some amount of radians about the Y axis.
			 * Vector is updated in-place.
			 *
			 * @param radian The number of radians to rotate.
			 * Positive: If Facing from negative to positive on Y-Axis, goes anti-clockwise
			 * Negative: If Facing from negative to positive on Y-Axis, goes clockwise
			 * @param vec The vector to rotate
			 *
			 * @return Nothing
			 */
			// template <class T>
			// inline void rotateYAxisRadian(T radian, EuclideanVector<T,3>& vec);

			/**
			 * Rotate a 3D vector some amount of radians about the Z axis
			 * Vector is updated in-place.
			 *
			 * @param radian The number of radians to rotate.
			 * Positive: If Facing from negative to positive on Z-Axis, goes anti-clockwise
			 * Negative: If Facing from negative to positive on Z-Axis, goes clockwise
			 * @param vec The vector to rotate
			 *
			 * @return Nothing
			 */
			// template <class T>
			// inline void rotateZAxisRadian(T radian,  EuclideanVector<T,3>& vec);

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
			// template <class T>
			// __attribute__((warn_unused_result))
			// inline bool isVectorRangeIntersection(const EuclideanPoint<T,3>& x1, const EuclideanPoint<T,3>& x2,
			// 								 	const EuclideanPoint<T,3>& x3, const EuclideanPoint<T,3>& x4);


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
			// template <class T>
			// __attribute__((warn_unused_result))
			// inline cupcfd::error::eCodes computeVectorRangeIntersection(const EuclideanPoint<T,3>& x1, const EuclideanPoint<T,3>& x2, 
			// 															const EuclideanPoint<T,3>& x3, const EuclideanPoint<T,3>& x4, 
			// 															EuclideanPoint<T,3>& intersectPoint);

			/**
			 * Test whether a point lies on a 3D line delinated by two other points
			 *
			 * @param x1 The first point of the line
			 * @param x2 The second point of the line
			 * @param p The point to test the position of
			 *
			 * @return Return true if the vectors intersect
			 */
			// template <class T>
			// __attribute__((warn_unused_result))
			// inline bool isPointOnLine(const EuclideanPoint<T,3>& x1, const EuclideanPoint<T,3>& x2,
			// 						  const EuclideanPoint<T,3>& p);

		} // namespace euclidean
	} // namespace geometry
} // namespace cupcfd

// Include Header Level Definitions
// ToDo: Would like to move friend functions into this file if the compiler errors are fixed.
#include "EuclideanVector.ipp"

// Explicit instantiation declarations of class static variables:
extern template MPI_Datatype cupcfd::geometry::euclidean::EuclideanVector<float,3>::mpiType;
extern template bool cupcfd::geometry::euclidean::EuclideanVector<float,3>::mpiDataTypeReg;
extern template MPI_Datatype cupcfd::geometry::euclidean::EuclideanVector<float,2>::mpiType;
extern template bool cupcfd::geometry::euclidean::EuclideanVector<float,2>::mpiDataTypeReg;

extern template MPI_Datatype cupcfd::geometry::euclidean::EuclideanVector<double,3>::mpiType;
extern template bool cupcfd::geometry::euclidean::EuclideanVector<double,3>::mpiDataTypeReg;
extern template MPI_Datatype cupcfd::geometry::euclidean::EuclideanVector<double,2>::mpiType;
extern template bool cupcfd::geometry::euclidean::EuclideanVector<double,2>::mpiDataTypeReg;

#endif
