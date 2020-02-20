/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * This file contains the declarations for the EuclideanPlane class
 */

#ifndef CUPCFD_GEOMETRY_EUCLIDEAN_PLANE_3D_INCLUDE_H
#define CUPCFD_GEOMETRY_EUCLIDEAN_PLANE_3D_INCLUDE_H

#include "EuclideanPoint.h"
#include "EuclideanVector.h"
#include "Error.h"

namespace cupcfd
{
	namespace geometry
	{
		namespace euclidean
		{
			/**
			 * This class stores three points that represent a plane in a 3D euclidean space, along
			 * with associated methods that might be used to compute characteristics of a 3D euclidean plane.
			 */
			template <class T>
			class EuclideanPlane3D
			{
				public:
					// === Members ==

					/** First distinct member point of plane **/
					cupcfd::geometry::euclidean::EuclideanPoint<T,3> p1;

					/** Second distinct member point of plane **/
					cupcfd::geometry::euclidean::EuclideanPoint<T,3> p2;

					/** Third distinct member point of plane **/
					cupcfd::geometry::euclidean::EuclideanPoint<T,3> p3;

					/** Plane Scalar Equation ax + by + cz + d = 0, a coefficient **/
					T a;

					/** Plane Scalar Equation ax + by + cz + d = 0, b coefficient **/
					T b;

					/** Plane Scalar Equation ax + by + cz + d = 0, c coefficient **/
					T c;

					/** Plane Scalar Equation ax + by + cz + d = 0, d coefficient **/
					T d;

					// ToDo: We could also compute and store the norm for future use, but if we do not
					// plan to use the norm, this is an unnecessary expense, so omitted for now

					// === Constructors/Deconstructors ===

					/**
					 * Constructor to build a plane. Takes and stores three distinct points that define the plane
					 *
					 * @param p1 Point 1 on the plane
					 * @param p2 Point 2 on the plane
					 * @param p3 Point 3 on the plane
					 *
					 * @tparam T The type of the coordinate system
					 */
					EuclideanPlane3D(const cupcfd::geometry::euclidean::EuclideanPoint<T,3>& p1,
									 const cupcfd::geometry::euclidean::EuclideanPoint<T,3>& p2,
									 const cupcfd::geometry::euclidean::EuclideanPoint<T,3>& p3);

					/**
					 * Deconstructor
					 */
					virtual ~EuclideanPlane3D();

					// === Concrete Methods ===

					/**
					 * Copy the points from the provided plane to this object
					 *
					 * @param plane The plane to copy from
					 *
					 * @tparam T The type of the coordinate system
					 */
					inline void operator=(const EuclideanPlane3D<T>& plane);

					/**
					 * Compare two EuclideanPlane3D objects to see if they are equivalent.
					 * Important: This does not test whether they are the same *plane*, as two objects could
					 * have different points defined and yet lie on the same plane. This only tests
					 * whether the objects have the same three points defined.
					 *
					 * @param plane1 The first source plane
					 * @param plane2 The second source plane
					 *
					 * @tparam T The type of the coordinate system
					 *
					 * @return A boolean indicating whether the two plane objects have the same
					 * three points defined
					 * @retval true The three stored planar points are equal
					 * @retval false The three stored planar points are not equal
					 */
					friend inline bool operator==(const EuclideanPlane3D<T>& plane1, const EuclideanPlane3D<T>& plane2)
					{
						// ToDo: Do we want a tolerance comparison here? There will probably be comparisons of
						// floating point values....
						if(plane1.p1 == plane2.p1 && plane1.p2 == plane2.p2 && plane1.p3 == plane2.p3)
						{
							return true;
						}

						return false;
					}

					/**
					 * Compare two EuclideanPlane3D objects to see if they are not equivalent.
					 * Important: This does not test whether they are the same *plane*, as two objects could
					 * have different points defined and yet lie on the same plane. This only tests
					 * whether the objects have the same or different three points defined.
					 *
					 * @param plane1 The first source plane
					 * @param plane2 The second source plane
					 *
					 * @tparam T The type of the coordinate system
					 *
					 * @return A boolean indicating whether the two plane objects have the same
					 * three points defined
					 * @retval true The three stored planar points are not equal
					 * @retval false The three stored planar points are equal
					 */
					friend inline bool operator!=(const EuclideanPlane3D<T>& plane1, const EuclideanPlane3D<T>& plane2)
					{
						// ToDo: Do we want a tolerance comparison here? There will probably be comparisons of
						// floating point values....
						if(plane1.p1 == plane2.p1 && plane1.p2 == plane2.p2 && plane1.p3 == plane2.p3)
						{
							return false;
						}

						return true;
					}


					/**
					 * Compare two planes to see if the first is less than the second.
					 * Such a thing does not technically exist, since points could be stored
					 * in any order in the plane, and each has different values.
					 * However, certain functions may insist on returning sorted arrays, so
					 * we implement this to facilitate them.
					 * We compare be evaluating p1, then p2, then p3. I.e. if p1 of plane1 is less,
					 * the entire plane is less. If equal, then compare p2 etc.
					 *
					 * @param plane1 The first source point
					 * @param plane2 The second source point
					 *
					 * @tparam T The type of the coordinate system
					 *
					 * @return A boolean indicating whether plane1 is less than plane2.
					 * @retval true plane1 is less than to plane2
					 * @retval false plane1 is not less than to plane2
					 */
					friend inline bool operator<(const EuclideanPlane3D<T>& plane1, const EuclideanPlane3D<T>& plane2)
					{
						if(plane1.p1 < plane2.p1)
						{
							return true;
						}

						if(plane1.p1 == plane2.p1)
						{
							if(plane1.p2 < plane2.p2)
							{
								return true;
							}

							if(plane1.p2 == plane2.p2)
							{
								if(plane1.p3 < plane2.p3)
								{
									return true;
								}
							}
						}

						return false;
					}

					/**
					 * Compare two planes to see if the first is less than or equal to the second.
					 * Such a thing does not technically exist, since points could be stored
					 * in any order in the plane, and each has different values.
					 * However, certain functions may insist on returning sorted arrays, so
					 * we implement this to facilitate them.
					 * We compare be evaluating p1, then p2, then p3. I.e. if p1 of plane1 is less,
					 * the entire plane is less. If equal, then compare p2 etc.
					 *
					 * @param plane1 The first source point
					 * @param plane2 The second source point
					 *
					 * @tparam T The type of the coordinate system
					 *
					 * @return A boolean indicating whether plane1 is less than plane2.
					 * @retval true plane1 is less than or equal to plane2
					 * @retval false plane1 is not less than or equal to plane2
					 */
					friend inline bool operator<=(const EuclideanPlane3D<T>& plane1, const EuclideanPlane3D<T>& plane2)
					{
						if(plane1.p1 < plane2.p1)
						{
							return true;
						}

						if(plane1.p1 == plane2.p1)
						{
							if(plane1.p2 < plane2.p2)
							{
								return true;
							}

							if(plane1.p2 == plane2.p2)
							{
								if(plane1.p3 <= plane2.p3)
								{
									return true;
								}
							}
						}

						return false;
					}

					/**
					 * Compare two planes to see if the first is greater than the second.
					 * Such a thing does not technically exist, since points could be stored
					 * in any order in the plane, and each has different values.
					 * However, certain functions may insist on returning sorted arrays, so
					 * we implement this to facilitate them.
					 * We compare be evaluating p1, then p2, then p3. I.e. if p1 of plane1 is greater,
					 * the entire plane is greater. If equal, then compare p2 etc.
					 *
					 * @param plane1 The first source point
					 * @param plane2 The second source point
					 *
					 * @tparam T The type of the coordinate system
					 *
					 * @return A boolean indicating whether plane1 is greater than plane2.
					 * @retval true plane1 is greater than to plane2
					 * @retval false plane1 is not greater than to plane2
					 */
					friend inline bool operator>(const EuclideanPlane3D<T>& plane1, const EuclideanPlane3D<T>& plane2)
					{
						if(plane1.p1 > plane2.p1)
						{
							return true;
						}

						if(plane1.p1 == plane2.p1)
						{
							if(plane1.p2 > plane2.p2)
							{
								return true;
							}

							if(plane1.p2 == plane2.p2)
							{
								if(plane1.p3 > plane2.p3)
								{
									return true;
								}
							}
						}

						return false;
					}


					/**
					 * Compare two planes to see if the first is greater than or equal to the second.
					 * Such a thing does not technically exist, since points could be stored
					 * in any order in the plane, and each has different values.
					 * However, certain functions may insist on returning sorted arrays, so
					 * we implement this to facilitate them.
					 * We compare be evaluating p1, then p2, then p3. I.e. if p1 of plane1 is greater,
					 * the entire plane is greater. If equal, then compare p2 etc.
					 *
					 * @param plane1 The first source point
					 * @param plane2 The second source point
					 *
					 * @tparam T The type of the coordinate system
					 *
					 * @return A boolean indicating whether plane1 is greater than plane2.
					 * @retval true plane1 is greater than or equal to plane2
					 * @retval false plane1 is not greater than or equal to plane2
					 */
					friend inline bool operator>=(const EuclideanPlane3D<T>& plane1, const EuclideanPlane3D<T>& plane2)
					{
						if(plane1.p1 > plane2.p1)
						{
							return true;
						}

						if(plane1.p1 == plane2.p1)
						{
							if(plane1.p2 > plane2.p2)
							{
								return true;
							}

							if(plane1.p2 == plane2.p2)
							{
								if(plane1.p3 >= plane2.p3)
								{
									return true;
								}
							}
						}

						return false;
					}

					/**
					 * Return a copy of the plane's normal vector. Direction is determined by
					 * the ordering of the original provided vertices that defined the plane.
					 * (I.e. clockwise or anticlockwise)
					 *
					 * @return The normal vector of the plane
					 */
					cupcfd::geometry::euclidean::EuclideanVector<T,3> getNormal();

					/**
					 * Compute the equation of the plane ax + by + cz + d = 0
					 *
					 * @param a A pointer to the location where the a coefficient will be stored
					 * @param b A pointer to the location where the b coefficient will be stored
					 * @param c A pointer to the location where the c coefficient will be stored
					 * @param d A pointer to the location where the d coefficient will be stored
					 *
					 * @tparam T The type of the coordinate system
					 *
					 * @return An error status indicating the success or failure of the operation
					 * @retval cupcfd::error::E_SUCCESS The method completed successfully
					 */
					cupcfd::error::eCodes computeScalarPlaneEquation(T * a, T * b, T * c, T * d);


					/**
					 * Determine whether a point lies on the plane defined by this object.
					 *
					 * @param point The point to test
					 *
					 * @tparam T The type of the coordinate system
					 *
					 * @return A boolean indicating whether the point lies on the plane
					 * @retval true The point lies on the plane
					 * @retval false The point does not lie on the plane
					 */
					bool isPointOnPlane(const cupcfd::geometry::euclidean::EuclideanPoint<T,3>& point);


					/**
					 * Compute the point that lies on the plane when projected from a provided point along
					 * the plane's normal.
					 *
					 * @param point The point to be projected (may be above, below or on the plane)
					 *
					 * @tparam T The type of the coordinate system
					 *
					 * @return The point that lies on the plane, projected from the provided point along
					 * the normal
					 */
					cupcfd::geometry::euclidean::EuclideanPoint<T,3> computeProjectedPoint(const cupcfd::geometry::euclidean::EuclideanPoint<T,3>& point);

					/**
					 * Test whether a vector is parallel to this plane
					 *
					 * @param vec The vector to test against
					 *
					 * @tparam T The type of the coordinate system
					 *
					 * @return A boolean indicating whether the vector path is parallel
					 * @retval true The vector is parallel to this plane
					 * @retval false The vector path is not parallel to this plane
					 */
					bool isVectorParallel(const cupcfd::geometry::euclidean::EuclideanVector<T,3>& vec);

					/**
					 * Test whether the vector is parallel to the plane and exists inside the plane.
					 *
					 * @param vec The vector to test against
					 * @param l0 A point on the vector line
					 *
					 * @tparam T The type of the coordinate system
					 *
					 * @return A boolean indicating whether the vector is parallel to the plane and in the plane.
					 * @retval true The vector is parallel to the plane and the line lies exactly on/inside the plane
					 * @retval false The vector is not parallel OR the line does not lie on the plane
					 */
					bool isVectorParallelInPlane(const cupcfd::geometry::euclidean::EuclideanVector<T,3>& vec,
												 const cupcfd::geometry::euclidean::EuclideanPoint<T,3>& l0);

					/**
					 * Find the intersection point of a line and a plane, as per https://en.wikipedia.org/wiki/Line-plane_intersection
					 * This is only applicable when the vector is not parallel to the plane.
					 *
					 * @param l The vector to test intersection with the plane
					 * @param l0 A point not on the plane that the vector intersects (i.e. a point on the line following the vector)
					 * @param result A referenced point object that will be updated to where the vector intersects the plane
					 *
					 * @tparam T The type of the coordinate system
					 *
					 * @return An error status indicating the success or failure of the operation
					 * @retval cupcfd::error::E_SUCCESS The method completed successfully
					 * @retval cupcfd::error::E_EUC_VEC_PARALLEL The line is parallel with the plane and will never intersect
					 * (or will intersect every point if it lies on the plane)
					 */
					cupcfd::error::eCodes linePlaneIntersection(const cupcfd::geometry::euclidean::EuclideanVector<T,3>& l,
																	 const cupcfd::geometry::euclidean::EuclideanPoint<T,3>& l0,
																	 cupcfd::geometry::euclidean::EuclideanPoint<T,3>& result);

					/**
					 * Compute the shortest distance (i.e. perpendicular distance) to a point from this plane.
					 *
					 * @param p The point to compute the distance to
					 *
					 * @tparam T The type of the coordinate system
					 *
					 * @return The shortest distance to the point from the plane.
					 */
					T shortestDistance(const cupcfd::geometry::euclidean::EuclideanPoint<T,3>& p);

					// === Static Methods ===

					/**
					 * Compute the normal vector from three points that lie on the plane
					 *
					 * Direction is determined by the ordering of the vertices (I.e. clockwise or anticlockwise).
					 *
					 * @param p1 Point 1 on the plane
					 * @param p2 Point 2 on the plane
					 * @param p3 Point 3 on the plane
					 *
					 * @tparam T The type of the coordinate system
					 *
					 * @return A vector containing the values for the normal vector of this plane
					 */
					static cupcfd::geometry::euclidean::EuclideanVector<T,3> normal(const cupcfd::geometry::euclidean::EuclideanPoint<T,3>& p1,
																						 const cupcfd::geometry::euclidean::EuclideanPoint<T,3>& p2,
																						 const cupcfd::geometry::euclidean::EuclideanPoint<T,3>& p3);

					/**
					 * Find the intersection point of a line and a plane, as per https://en.wikipedia.org/wiki/Line-plane_intersection
					 * This is only applicable when the vector is not parallel to the plane.
					 *
					 * @param normal The normal of the intersection plane
					 * @param p0 A point that lies on the plane
					 * @param l The vector to test intersection with the plane
					 * @param l0 A point not on the plane that the vector intersects (i.e. a point on the line following the vector)
					 * @param result A referenced point object that will be updated to where the vector intersects the plane
					 *
					 * @tparam T The type of the coordinate system
					 *
					 * @return An error status indicating the success or failure of the operation
					 * @retval cupcfd::error::E_SUCCESS The method completed successfully
					 * @retval cupcfd::error::E_EUC_VEC_PARALLEL The line is parallel with the plane and will never intersect
					 * (or will intersect every point if it lies on the plane)
					 */
					static cupcfd::error::eCodes linePlaneIntersection(cupcfd::geometry::euclidean::EuclideanVector<T,3>& normal,
																			const cupcfd::geometry::euclidean::EuclideanPoint<T,3>& p0,
																			const cupcfd::geometry::euclidean::EuclideanVector<T,3>& l,
																			const cupcfd::geometry::euclidean::EuclideanPoint<T,3>& l0,
																			cupcfd::geometry::euclidean::EuclideanPoint<T,3>& result);
				protected:

				private:

			};
		} // namespace euclidean
	} // namespace geometry
} // namespace cupcfd

// Include Header Level Definitions
#include "EuclideanPlane3D.ipp"

#endif
