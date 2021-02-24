/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * This file contains the declarations for the N-dimensional EuclideanPoint class
 */

#ifndef CUPCFD_GEOMETRY_EUCLIDEAN_POINT_INCLUDE_H
#define CUPCFD_GEOMETRY_EUCLIDEAN_POINT_INCLUDE_H

#include "EuclideanVector.h"
#include "ArithmeticKernels.h"

namespace cupcfd
{
	namespace geometry
	{
		namespace euclidean
		{
			// Forward Declaration
			template <class T, unsigned int N>
			class EuclideanVector;

			/**
			 * Stores N-Dimensional Position Data in a Euclidean Space for a
			 * templated datatype.
			 *
			 * @tparam T The data type of the coordinate system
			 * @tparam N The dimension of the coordinate space (e.g. 2 for 2D, 3 for 3D...)
			 */
			template <class T, unsigned int N>
			class EuclideanPoint : public cupcfd::comm::mpi::CustomMPIType
			{
				public:

					// === Static Variables ===

					/** Storage for the MPI Type returned upon registration of this class with the MPI library **/
					static MPI_Datatype mpiType;

					/** Track whether the datatype has been registered with the MPI library**/
					static bool mpiDataTypeReg;

					// === Members ===

					/** Dimensional Components of EuclideanPoint.
					 * Each represents the distance from the origin 0 in
					 * the i'th dimension out of N dimensions.
					 * E.g. For a 3D Point, X-Dimension = 0, Y Dimension = 1, Z Dimension = 2
					 */
					T cmp[N];

					// === Constructors/Deconstructors

					/**
					 * Constructor - Sets the dimensional coordinates of the point to zero.
					 */
					EuclideanPoint();

					/**
					 * Constructor - Duplicate another EuclideanPoint
					 * @param point The point to duplicate
					 *
					 */
					EuclideanPoint(const EuclideanPoint &point);

					/**
					 * Constructor - Sets dimensional coordinates of the point to those provided
					 *
					 * @param cmp An array of distances from the origin 0 in each of the dimensions
					 */
					template<class...Args>
					EuclideanPoint(Args...src)
					: cmp{ (src)... }
					{
						static_assert(sizeof...(Args) == N, "EuclideanPoint constructor dimension does not match number of parameters");
					}

					// === Concrete Methods ===

					/**
					 * Copy the coordinate values from one 2D Euclidean point to another
					 *
					 * @param source The point to copy the values from
					 *
					 * @return Returns nothing
					 */
					inline void operator=(const EuclideanPoint<T,N>& source);

					/**
					 * Set both values of the coordinate to a fixed scalar
					 *
					 * @param source The scalar to set the values to
					 *
					 * @return Returns nothing
					 */
					inline void operator=(const T scalar);

					inline void operator*=(T scalar);

					/**
					 * Compare two points to see if they are equivalent (i.e. they have the same coordinates)
					 *
					 * @param point1 The first source point
					 * @param point2 The second source point
					 *
					 * @return True if the two points are equal - this is determined
					 * by whether all of the cartesian locations are equivalent
					 */
					__attribute__((warn_unused_result))
					friend inline bool operator==(const EuclideanPoint<T,N>& point1, const EuclideanPoint<T,N>& point2)
					{
						for(uint i = 0; i < N; i++) {
							if(!(cupcfd::utility::arithmetic::kernels::isEqual(point1.cmp[i], point2.cmp[i]))) {
								return false;
							}
						}

						return true;
					}

					__attribute__((warn_unused_result))
					inline bool approximateEquals(const EuclideanPoint<T,N>& p, float pct_tolerance);

					/**
					 * Compare two points to see if they are not equivalent (they have different coordinates in one
					 * or more dimensions)
					 *
					 * @param point1 The first source point
					 * @param point2 The second source point
					 *
					 * @return True if the two points are not equal - this is determined
					 * by whether all of the cartesian locations are equivalent
					 */
					__attribute__((warn_unused_result))
					friend inline bool operator!=(const EuclideanPoint<T,N>& point1, const EuclideanPoint<T,N>& point2)
					{
						return !(point1 == point2);
					}

					/**
					 * Compare two points to see if the first point is less than the second, determined by if the
					 * coordinates in each dimension are less, in priority order of dimension N, N-1, N-2..... 0
					 *
					 * @param point1 The first source point
					 * @param point2 The second source point
					 *
					 * @return True if point1 is less than point2.
					 */
					__attribute__((warn_unused_result))
					friend inline bool operator<(const EuclideanPoint<T,N>& point1, const EuclideanPoint<T,N>& point2)
					{
						// ToDo: Tolerance checks?

						for (uint i=N; i>0; i--) {
							if(point1.cmp[i-1] < point2.cmp[i-1]) {
								return true;
							}
						}
						return false;
					}

					/**
					 * Compare two points to see if the first point is less than or equal to the second, determined by if the
					 * coordinates in each dimension are less or equal, in priority order of y, x
					 *
					 * @param point1 The first source point
					 * @param point2 The second source point
					 *
					 * @return True if point1 is less than point2.
					 */
					__attribute__((warn_unused_result))
					friend inline bool operator<=(const EuclideanPoint<T,N>& point1, const EuclideanPoint<T,N>& point2)
					{
						// ToDo: Tolerance checks?
						for (uint i=N; i>0; i--) {
							if(point1.cmp[i-1] < point2.cmp[i-1]) {
								return true;
							}
							if(point1.cmp[i-1] > point2.cmp[i-1]) {
								return false;
							}
						}
						return true;
					}


					/**
					 * Compare two points to see if the first point is greater than the second, determined by if the
					 * coordinates in each dimension are greater, in priority order of y, x
					 *
					 * @param point1 The first source point
					 * @param point2 The second source point
					 *
					 * @return True if point1 is greater than point2.
					 */
					__attribute__((warn_unused_result))
					friend inline bool operator>(const EuclideanPoint<T,N>& point1, const EuclideanPoint<T,N>& point2)
					{
						// ToDo: Tolerance checks?

						for (uint i=N; i>0; i--) {
							if(point1.cmp[i-1] > point2.cmp[i-1]) {
								return true;
							}
						}
						return false;
					}


					/**
					 * Compare two points to see if the first point is greater than or equal to the second, determined by if the
					 * coordinates in each dimension are greater or equal, in priority order of y, x
					 *
					 * @param point1 The first source point
					 * @param point2 The second source point
					 *
					 * @return True if point1 is greater than or equal to point2.
					 */
					__attribute__((warn_unused_result))
					friend inline bool operator>=(const EuclideanPoint<T,N>& point1, const EuclideanPoint<T,N>& point2)
					{
						// ToDo: Tolerance checks?
						for (uint i=N; i>0; i--) {
							if(point1.cmp[i-1] > point2.cmp[i-1]) {
								return true;
							}
							if(point1.cmp[i-1] < point2.cmp[i-1]) {
								return false;
							}
						}
						return true;
					}

					/**
					 * Sum two points
					 *
					 * @param point1 The first point
					 * @param point2 The second point
					 *
					 * @return The sum of thw two points
					 */
					__attribute__((warn_unused_result))
					friend inline EuclideanPoint<T,N> operator+(const EuclideanPoint<T,N>& point1, const EuclideanPoint<T,N>& point2)
					{
						EuclideanPoint<T,N> result;

						for(uint i = 0; i < N; i++) {
							result.cmp[i] = point1.cmp[i] + point2.cmp[i];
						}

						return result;
					}

					/**
					 * Compute the point at the location reached by following the vector from the point
					 *
					 * @param point The origin point
					 * @param vector The vector to follow
					 *
					 * @return The point reached after following the vector from the origin point
					 */
					__attribute__((warn_unused_result))
					friend inline EuclideanPoint<T,N> operator+(const EuclideanPoint<T,N>& point, const EuclideanVector<T,N>& vector)
					{
						EuclideanPoint<T,N> result;

						for(uint i = 0; i < N; i++) {
							result.cmp[i] = point.cmp[i] + vector.cmp[i];
						}

						return result;
					}


					/**
					 * Compute the point at the location reached by following the vector from the point
					 *
					 * @param vector The vector to follow
					 * @param point The origin point
					 *
					 * @return The point reached after following the vector from the origin point
					 */
					__attribute__((warn_unused_result))
					friend inline EuclideanPoint<T,N> operator+(const EuclideanVector<T,N>& vector, const EuclideanPoint<T,N>& point)
					{
						EuclideanPoint<T,N> result;

						for(uint i = 0; i < N; i++) {
							result.cmp[i] = vector.cmp[i] + point.cmp[i];
						}

						return result;
					}

					/**
					 * Compute the point shift by a scalar in all three dimensions (i.e. a shift)
					 *
					 * @param point The origin point
					 * @param scalar The scalar to shift the point by in all dimensions
					 *
					 * @return The shifted point
					 */
					__attribute__((warn_unused_result))
					friend inline EuclideanPoint<T,N> operator+(const EuclideanPoint<T,N>& point, T scalar)
					{
						EuclideanPoint<T,N> result;

						for(uint i = 0; i < N; i++) {
							result.cmp[i] = point.cmp[i] + scalar;
						}

						return result;
					}

					/**
					 * Compute the point shift by a scalar in all three dimensions (i.e. a shift)
					 *
					 * @param point The origin point
					 * @param scalar The scalar to shift the point by in all dimensions
					 *
					 * @return The shifted point
					 */
					__attribute__((warn_unused_result))
					friend inline EuclideanPoint<T,N> operator+(T scalar, const EuclideanPoint<T,N>& point)
					{
						EuclideanPoint<T,N> result;

						for(uint i = 0; i < N; i++) {
							result.cmp[i] = scalar + point.cmp[i];
						}

						return result;
					}

					/**
					 * Compute the the traversal vector for reaching a destination point from a start point to an end point
					 *
					 * @param start The origin position
					 * @param end The destination position
					 *
					 * @return The vector for moving from origin to dest
					 */
					__attribute__((warn_unused_result))
					friend inline EuclideanVector<T,N> operator-(const EuclideanPoint<T,N>& end, const EuclideanPoint<T,N>& start)
					{
						EuclideanVector<T,N> result;

						for(uint i = 0; i < N; i++) {
							result.cmp[i] = end.cmp[i] - start.cmp[i];
						}

						return result;
					}

					/**
					 * Compute the point reached by travelling in the reverse direction along a vector
					 *
					 * @param end The point at the end of the vector
					 *
					 * @return The point at the start of the vector
					 */
					__attribute__((warn_unused_result))
					friend inline EuclideanPoint<T,N> operator-(const EuclideanPoint<T,N>& end, const EuclideanVector<T,N>& vec)
					{
						EuclideanPoint<T,N> result;

						for(uint i = 0; i < N; i++) {
							result.cmp[i] = end.cmp[i] - vec.cmp[i];
						}

						return result;
					}

					/**
					 * Move a point by a scalar multiplier
					 *
					 * @param point The origin point
					 * @param scalar The scalar factor
					 *
					 * @return The point of the new location
					 */
					__attribute__((warn_unused_result))
					friend inline EuclideanPoint<T,N> operator*(const EuclideanPoint<T,N>& point, T scalar)
					{
						EuclideanPoint<T,N> result;

						for(uint i = 0; i < N; i++) {
							result.cmp[i] = point.cmp[i] * scalar;
						}

						return result;
					}

					/**
					 * Move a point by a scalar multiplier
					 *
					 * @param point The origin point
					 * @param scalar The scalar factor
					 *
					 * @return The point of the new location
					 */
					__attribute__((warn_unused_result))
					friend inline EuclideanPoint<T,N> operator*(T scalar, const EuclideanPoint<T,N>& point)
					{
						EuclideanPoint<T,N> result;

						for(uint i = 0; i < N; i++) {
							result.cmp[i] = scalar * point.cmp[i];
						}

						return result;
					}

					/**
					 * Divide a point by a scalar
					 *
					 * @param point The origin point
					 * @param scalar The scalar factor
					 *
					 * @return The point of the new location
					 */
					__attribute__((warn_unused_result))
					friend inline EuclideanPoint<T,N> operator/(const EuclideanPoint<T,N>& point, T scalar)
					{
						EuclideanPoint<T,N> result;

						for(uint i = 0; i < N; i++) {
							result.cmp[i] = point.cmp[i] / scalar;
						}

						return result;
					}

					/**
					 * Divide a scalar value by a point
					 *
					 * @param point The origin point
					 * @param scalar The scalar factor
					 *
					 * @return The point of the new location
					 */
					__attribute__((warn_unused_result))
					friend inline EuclideanPoint<T,N> operator/(T scalar, const EuclideanPoint<T,N>& point)
					{
						EuclideanPoint<T,N> result;

						for(uint i = 0; i < N; i++) {
							result.cmp[i] = scalar / point.cmp[i];
						}

						return result;
					}

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
		} // namespace euclidean
	} // namespace geometry
} // namespace cupcfd

// Include Header Level Definitions
// ToDo: Would like to move friend functions into here as well, although will need to fix compiler errors.
#include "EuclideanPoint.ipp"

namespace euc = cupcfd::geometry::euclidean;

// Explicit instantiation declarations of class static variables:
extern template MPI_Datatype euc::EuclideanPoint<float,3>::mpiType;
extern template bool euc::EuclideanPoint<float,3>::mpiDataTypeReg;
extern template MPI_Datatype euc::EuclideanPoint<float,2>::mpiType;
extern template bool euc::EuclideanPoint<float,2>::mpiDataTypeReg;

extern template MPI_Datatype euc::EuclideanPoint<double,3>::mpiType;
extern template bool euc::EuclideanPoint<double,3>::mpiDataTypeReg;
extern template MPI_Datatype euc::EuclideanPoint<double,2>::mpiType;
extern template bool euc::EuclideanPoint<double,2>::mpiDataTypeReg;

#endif
