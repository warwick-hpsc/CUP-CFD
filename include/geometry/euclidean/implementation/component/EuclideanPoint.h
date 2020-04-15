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
					 *
					 * @tparam T The data type of the coordinate system
					 * @tparam N The dimension of the coordinate space (e.g. 2 for 2D, 3 for 3D...)
					 *
					 */
					EuclideanPoint();


					EuclideanPoint(const EuclideanPoint &point);

					/**
					 * Constructor - Sets dimensional coordinates of the point to those provided
					 *
					 * @tparam T The data type of the coordinate system
					 * @tparam N The dimension of the coordinate space (e.g. 2 for 2D, 3 for 3D...)
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
					 * @tparam T The data type of the coordinate system
					 * @tparam N The dimension of the coordinate space (e.g. 2 for 2D, 3 for 3D...)
					 *
					 * @return Returns nothing
					 */
					inline void operator=(const EuclideanPoint<T,N>& source);

					/**
					 * Set both values of the coordinate to a fixed scalar
					 *
					 * @param source The scalar to set the values to
					 *
					 * @tparam T The data type of the coordinate system
					 * @tparam N The dimension of the coordinate space (e.g. 2 for 2D, 3 for 3D...)
					 *
					 * @return Returns nothing
					 */
					inline void operator=(const T scalar);

					/**
					 * Compare two points to see if they are equivalent (i.e. they have the same coordinates)
					 *
					 * @param point1 The first source point
					 * @param point2 The second source point
					 *
					 * @tparam T The data type of the coordinate system
					 * @tparam N The dimension of the coordinate space (e.g. 2 for 2D, 3 for 3D...)
					 *
					 * @return A boolean indicating whether the two points are equal - this is determined
					 * by whether all of the cartesian locations are equivalent
					 * @retval true The points are equal
					 * @retval false The points are not equal
					 */
					friend inline bool operator==(const EuclideanPoint<T,N>& point1, const EuclideanPoint<T,N>& point2)
					{
						for(int i = 0; i < N; i++)
						{
							if(!(cupcfd::utility::arithmetic::kernels::isEqual(point1.cmp[i], point2.cmp[i])))
							{
								return false;
							}
						}

						return true;
					}

					/**
					 * Compare two points to see if they are not equivalent (they have different coordinates in one
					 * or more dimensions)
					 *
					 * @param point1 The first source point
					 * @param point2 The second source point
					 *
					 * @tparam T The data type of the coordinate system
					 * @tparam N The dimension of the coordinate space (e.g. 2 for 2D, 3 for 3D...)
					 *
					 * @return A boolean indicating whether the two points are not equal - this is determined
					 * by whether all of the cartesian locations are equivalent
					 * @retval true The points are not equal
					 * @retval false The points are equal
					 */
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
					 * @tparam T The data type of the coordinate system
					 * @tparam N The dimension of the coordinate space (e.g. 2 for 2D, 3 for 3D...)
					 *
					 * @return A boolean indicating whether point1 is less than point2.
					 * @retval true point1 is less than to point 2
					 * @retval false point1 is not less than to point 2
					 */
					friend inline bool operator<(const EuclideanPoint<T,N>& point1, const EuclideanPoint<T,N>& point2)
					{
						// ToDo: Tolerance checks?

						for(int i = N-1; i >= 0; i--)
						{
							if(point1.cmp[i] < point2.cmp[i])
							{
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
					 * @tparam T The data type of the coordinate system
					 * @tparam N The dimension of the coordinate space (e.g. 2 for 2D, 3 for 3D...)
					 *
					 * @return A boolean indicating whether point1 is less than point2.
					 * @retval true point1 is less than or equal to point 2
					 * @retval false point1 is not less than or equal to point 2
					 */
					friend inline bool operator<=(const EuclideanPoint<T,N>& point1, const EuclideanPoint<T,N>& point2)
					{
						// ToDo: Tolerance checks?
						for(int i = N-1; i >= 0; i--)
						{
							if(point1.cmp[i] < point2.cmp[i])
							{
								return true;
							}

							if(point1.cmp[i] > point2.cmp[i])
							{
								return false;
							}
						}

						// Only reached if all are equal
						return true;
					}


					/**
					 * Compare two points to see if the first point is greater than the second, determined by if the
					 * coordinates in each dimension are greater, in priority order of y, x
					 *
					 * @param point1 The first source point
					 * @param point2 The second source point
					 *
					 * @tparam T The data type of the coordinate system
					 * @tparam N The dimension of the coordinate space (e.g. 2 for 2D, 3 for 3D...)
					 *
					 * @return A boolean indicating whether point1 is greater than point2.
					 * @retval true point1 is greater than point 2
					 * @retval false point1 is not greater than point 2
					 */
					friend inline bool operator>(const EuclideanPoint<T,N>& point1, const EuclideanPoint<T,N>& point2)
					{
						// ToDo: Tolerance checks?

						for(int i = N-1; i >= 0; i--)
						{
							if(point1.cmp[i] > point2.cmp[i])
							{
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
					 * @tparam T The data type of the coordinate system
					 * @tparam N The dimension of the coordinate space (e.g. 2 for 2D, 3 for 3D...)
					 *
					 * @return A boolean indicating whether point1 is greater than or equal to point2.
					 * @retval true point1 is greater than or equal to point 2
					 * @retval false point1 is not greater than or equal to point 2
					 */
					friend inline bool operator>=(const EuclideanPoint<T,N>& point1, const EuclideanPoint<T,N>& point2)
					{
						// ToDo: Tolerance checks?
						for(int i = N-1; i >= 0; i--)
						{
							if(point1.cmp[i] > point2.cmp[i])
							{
								return true;
							}

							if(point1.cmp[i] < point2.cmp[i])
							{
								return false;
							}
						}

						// Only reached if all are equal
						return true;
					}

					/**
					 * Sum two points
					 *
					 * @param point1 The first point
					 * @param point2 The second point
					 *
					 * @tparam T The data type of the coordinate system
					 * @tparam N The dimension of the coordinate space (e.g. 2 for 2D, 3 for 3D...)
					 *
					 * @return The sum of thw two points
					 */
					friend inline EuclideanPoint<T,N> operator+(const EuclideanPoint<T,N>& point1, const EuclideanPoint<T,N>& point2)
					{
						EuclideanPoint<T,N> result;

						for(int i = 0; i < N; i++)
						{
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
					 * @tparam T The data type of the coordinate system
					 * @tparam N The dimension of the coordinate space (e.g. 2 for 2D, 3 for 3D...)
					 *
					 * @return The point reached after following the vector from the origin point
					 */
					friend inline EuclideanPoint<T,N> operator+(const EuclideanPoint<T,N>& point, const cupcfd::geometry::euclidean::EuclideanVector<T,N>& vector)
					{
						EuclideanPoint<T,N> result;

						for(int i = 0; i < N; i++)
						{
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
					 * @tparam T The data type of the coordinate system
					 * @tparam N The dimension of the coordinate space (e.g. 2 for 2D, 3 for 3D...)
					 *
					 * @return The point reached after following the vector from the origin point
					 */
					friend inline EuclideanPoint<T,N> operator+(const cupcfd::geometry::euclidean::EuclideanVector<T,N>& vector, const EuclideanPoint<T,N>& point)
					{
						EuclideanPoint<T,N> result;

						for(int i = 0; i < N; i++)
						{
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
					 * @tparam T The data type of the coordinate system
					 * @tparam N The dimension of the coordinate space (e.g. 2 for 2D, 3 for 3D...)
					 *
					 * @return The shifted point
					 */
					friend inline EuclideanPoint<T,N> operator+(const EuclideanPoint<T,N>& point, T scalar)
					{
						EuclideanPoint<T,N> result;

						for(int i = 0; i < N; i++)
						{
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
					 * @tparam T The data type of the coordinate system
					 * @tparam N The dimension of the coordinate space (e.g. 2 for 2D, 3 for 3D...)
					 *
					 * @return The shifted point
					 */
					friend inline EuclideanPoint<T,N> operator+(T scalar, const EuclideanPoint<T,N>& point)
					{
						EuclideanPoint<T,N> result;

						for(int i = 0; i < N; i++)
						{
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
					 * @tparam T The data type of the coordinate system
					 * @tparam N The dimension of the coordinate space (e.g. 2 for 2D, 3 for 3D...)
					 *
					 * @return The vector for moving from origin to dest
					 */
					friend inline cupcfd::geometry::euclidean::EuclideanVector<T,N> operator-(const EuclideanPoint<T,N>& end, const EuclideanPoint<T,N>& start)
					{
						cupcfd::geometry::euclidean::EuclideanVector<T,N> result;

						for(int i = 0; i < N; i++)
						{
							result.cmp[i] = end.cmp[i] - start.cmp[i];
						}

						return result;
					}

					/**
					 * Compute the point reached by travelling in the reverse direction along a vector
					 *
					 * @param end The point at the end of the vector
					 *
					 * @tparam T The data type of the coordinate system
					 * @tparam N The dimension of the coordinate space (e.g. 2 for 2D, 3 for 3D...)
					 *
					 * @return The point at the start of the vector
					 */
					friend inline cupcfd::geometry::euclidean::EuclideanPoint<T,N> operator-(const EuclideanPoint<T,N>& end, const EuclideanVector<T,N>& vec)
					{
						cupcfd::geometry::euclidean::EuclideanPoint<T,N> result;

						for(int i = 0; i < N; i++)
						{
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
					 * @tparam T The data type of the coordinate system
					 * @tparam N The dimension of the coordinate space (e.g. 2 for 2D, 3 for 3D...)
					 *
					 * @return The point of the new location
					 */
					friend inline EuclideanPoint<T,N> operator*(const EuclideanPoint<T,N>& point, T scalar)
					{
						EuclideanPoint<T,N> result;

						for(int i = 0; i < N; i++)
						{
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
					 * @tparam T The data type of the coordinate system
					 * @tparam N The dimension of the coordinate space (e.g. 2 for 2D, 3 for 3D...)
					 *
					 * @return The point of the new location
					 */
					friend inline EuclideanPoint<T,N> operator*(T scalar, const EuclideanPoint<T,N>& point)
					{
						EuclideanPoint<T,N> result;

						for(int i = 0; i < N; i++)
						{
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
					 * @tparam T The data type of the coordinate system
					 * @tparam N The dimension of the coordinate space (e.g. 2 for 2D, 3 for 3D...)
					 *
					 * @return The point of the new location
					 */
					friend inline EuclideanPoint<T,N> operator/(const EuclideanPoint<T,N>& point, T scalar)
					{
						EuclideanPoint<T,N> result;

						for(int i = 0; i < N; i++)
						{
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
					 * @tparam T The data type of the coordinate system
					 * @tparam N The dimension of the coordinate space (e.g. 2 for 2D, 3 for 3D...)
					 *
					 * @return The point of the new location
					 */
					friend inline EuclideanPoint<T,N> operator/(T scalar, const EuclideanPoint<T,N>& point)
					{
						EuclideanPoint<T,N> result;

						for(int i = 0; i < N; i++)
						{
							result.cmp[i] = scalar / point.cmp[i];
						}

						return result;
					}

					// === Inherited Overloads ===

					inline cupcfd::error::eCodes getMPIType(MPI_Datatype * dType);

					inline MPI_Datatype getMPIType();

					cupcfd::error::eCodes registerMPIType();

					cupcfd::error::eCodes deregisterMPIType();

					inline bool isRegistered();

					void print();
			};
		} // namespace euclidean
	} // namespace geometry
} // namespace cupcfd

// Include Header Level Definitions
// ToDo: Would like to move friend functions into here as well, although will need to fix compiler errors.
#include "EuclideanPoint.ipp"

#endif
