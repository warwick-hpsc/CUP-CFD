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
#include "EuclideanVector3D.h"
#include "Error.h"

namespace euc = cupcfd::geometry::euclidean;

namespace cupcfd
{
	namespace geometry
	{
		namespace euclidean
		{
			/**
			 * This class stores three points that represent a plane in a 3D euclidean space, along
			 * with associated methods that might be used to compute characteristics of a 3D euclidean plane.
			 *
			 * @tparam T The type of the coordinate system
			 */
			template <class T>
			class EuclideanPlane3D
			{
				public:
					// === Members ==

					/** First distinct member point of plane **/
					euc::EuclideanPoint<T,3> p1;

					/** Second distinct member point of plane **/
					euc::EuclideanPoint<T,3> p2;

					/** Third distinct member point of plane **/
					euc::EuclideanPoint<T,3> p3;

					/** Normal **/
					// euc::EuclideanVector<T,3> normal;

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
					 */
					EuclideanPlane3D(const euc::EuclideanPoint<T,3>& p1,
									 const euc::EuclideanPoint<T,3>& p2,
									 const euc::EuclideanPoint<T,3>& p3);

					/**
					 * Deconstructor
					 */
					~EuclideanPlane3D();

					// === Concrete Methods ===

					// void reverseVertexOrdering();

					/**
					 * Copy the points from the provided plane to this object
					 *
					 * @param plane The plane to copy from
					 */
					inline void operator=(const EuclideanPlane3D<T>& plane);

					/**
					 * Compare two EuclideanPlane3D objects to see if they are equivalent.
					 * Important: This does not test whether they are the same *plane*, as two objects could
					 * have different points defined and yet lie on the same plane. This only tests
					 * whether the objects have the same three points defined.
					 *
					 * @param p1 The first source plane
					 * @param p2 The second source plane
					 *
					 * @return True if the two plane objects are composed of same vertices
					 */
					__attribute__((warn_unused_result))
					friend inline bool operator==(const EuclideanPlane3D<T>& p1, const EuclideanPlane3D<T>& p2) {
						// ToDo: Do we want a tolerance comparison here? There will probably be comparisons of
						// floating point values....
						return (p1.p1 == p2.p1 && p1.p2 == p2.p2 && p1.p3 == p2.p3);
					}

					/**
					 * Compare two planes to see if the first is less than the second, 
					 * for sorting purposes.
					 *
					 * @param p1 The first source point
					 * @param p2 The second source point
					 *
					 * @return True if p1 vertices numerically less than p2
					 */
					__attribute__((warn_unused_result))
					friend inline bool operator<(const EuclideanPlane3D<T>& p1, const EuclideanPlane3D<T>& p2) {
						if(p1.p1 < p2.p1) {
							return true;
						}

						if(p1.p1 == p2.p1) {
							if(p1.p2 < p2.p2) {
								return true;
							}

							if(p1.p2 == p2.p2) {
								if(p1.p3 < p2.p3) {
									return true;
								}
							}
						}

						return false;
					}

					/**
					 * Compare two EuclideanPlane3D objects to see if they are not equivalent.
					 * Important: This does not test whether they are the same *plane*, as two objects could
					 * have different points defined and yet lie on the same plane. This only tests
					 * whether the objects have the same or different three points defined.
					 *
					 * @param p1 The first source plane
					 * @param p2 The second source plane
					 *
					 * @return False if the two plane objects are composed of same vertices
					 */
					__attribute__((warn_unused_result))
					friend inline bool operator!=(const EuclideanPlane3D<T>& p1, const EuclideanPlane3D<T>& p2) {
						return !(p1 == p2);
					}

					/**
					 * Compare two planes to see if the first is less than the second, 
					 * for sorting purposes.
					 *
					 * @param p1 The first source point
					 * @param p2 The second source point
					 *
					 * @return True if p1 vertices less-than-or-equal p2
					 */
					__attribute__((warn_unused_result))
					friend inline bool operator<=(const EuclideanPlane3D<T>& p1, const EuclideanPlane3D<T>& p2) {
						return (p1 < p2) || (p1 == p2);
					}

					/**
					 * Compare two planes to see if the first is less than the second, 
					 * for sorting purposes.
					 *
					 * @param p1 The first source point
					 * @param p2 The second source point
					 *
					 * @return True if p1 vertices numerically greater than p2
					 */
					__attribute__((warn_unused_result))
					friend inline bool operator>(const EuclideanPlane3D<T>& p1, const EuclideanPlane3D<T>& p2) {
						return !( (p1==p2) || (p1<p2) );
					}


					/**
					 * Compare two planes to see if the first is less than the second, 
					 * for sorting purposes.
					 *
					 * @param p1 The first source point
					 * @param p2 The second source point
					 *
					 * @return True if p1 vertices greater-than-or-equal p2
					 */
					__attribute__((warn_unused_result))
					friend inline bool operator>=(const EuclideanPlane3D<T>& p1, const EuclideanPlane3D<T>& p2) {
						return !( p1<p2 );
					}

					/**
					 * Return a copy of the plane's normal vector. Direction is determined by
					 * the ordering of the original provided vertices that defined the plane.
					 * (I.e. clockwise or anticlockwise)
					 *
					 * @return The normal vector of the plane
					 */
					__attribute__((warn_unused_result))
					euc::EuclideanVector3D<T> getNormal();

					/**
					 * Compute the equation of the plane ax + by + cz + d = 0
					 *
					 * @param a A pointer to the location where the a coefficient will be stored
					 * @param b A pointer to the location where the b coefficient will be stored
					 * @param c A pointer to the location where the c coefficient will be stored
					 * @param d A pointer to the location where the d coefficient will be stored
					 *
					 * @return An error status indicating the success or failure of the operation
					 * @retval cupcfd::error::E_SUCCESS The method completed successfully
					 */
					void computeScalarPlaneEquation(T * a, T * b, T * c, T * d);


					/**
					 * Determine whether a point lies on the plane defined by this object.
					 *
					 * @param point The point to test
					 *
					 * @return A boolean indicating whether the point lies on the plane
					 * @retval true The point lies on the plane
					 * @retval false The point does not lie on the plane
					 */
					__attribute__((warn_unused_result))
					bool isPointOnPlane(const euc::EuclideanPoint<T,3>& point);


					/**
					 * Compute the point that lies on the plane when projected from a provided point along
					 * the plane's normal.
					 *
					 * @param point The point to be projected (may be above, below or on the plane)
					 *
					 * @return The point that lies on the plane, projected from the provided point along
					 * the normal
					 */
					__attribute__((warn_unused_result))
					euc::EuclideanPoint<T,3> computeProjectedPoint(const euc::EuclideanPoint<T,3>& point);

					/**
					 * Test whether a vector is parallel to this plane
					 *
					 * @param vec The vector to test against
					 *
					 * @return A boolean indicating whether the vector path is parallel
					 * @retval true The vector is parallel to this plane
					 * @retval false The vector path is not parallel to this plane
					 */
					__attribute__((warn_unused_result))
					bool isVectorParallel(const euc::EuclideanVector<T,3>& vec);

					/**
					 * Test whether the vector is parallel to the plane and exists inside the plane.
					 *
					 * @param vec The vector to test against
					 * @param l0 A point on the vector line
					 *
					 * @return A boolean indicating whether the vector is parallel to the plane and in the plane.
					 * @retval true The vector is parallel to the plane and the line lies exactly on/inside the plane
					 * @retval false The vector is not parallel OR the line does not lie on the plane
					 */
					__attribute__((warn_unused_result))
					bool isVectorParallelInPlane(const euc::EuclideanVector<T,3>& vec,
												 const euc::EuclideanPoint<T,3>& l0);

					/**
					 * Find the intersection point of a line and a plane, as per https://en.wikipedia.org/wiki/Line-plane_intersection
					 * This is only applicable when the vector is not parallel to the plane.
					 *
					 * @param l The vector to test intersection with the plane
					 * @param l0 A point not on the plane that the vector intersects (i.e. a point on the line following the vector)
					 * @param result A referenced point object that will be updated to where the vector intersects the plane
					 *
					 * @return An error status indicating the success or failure of the operation
					 * @retval cupcfd::error::E_SUCCESS The method completed successfully
					 * @retval cupcfd::error::E_EUC_VEC_PARALLEL The line is parallel with the plane and will never intersect
					 * (or will intersect every point if it lies on the plane)
					 */
					__attribute__((warn_unused_result))
					cupcfd::error::eCodes linePlaneIntersection(const euc::EuclideanVector<T,3>& l,
																const euc::EuclideanPoint<T,3>& l0,
																euc::EuclideanPoint<T,3>& result);

					/**
					 * Compute the shortest distance (i.e. perpendicular distance) to a point from this plane.
					 *
					 * @param p The point to compute the distance to
					 *
					 * @return The shortest distance to the point from the plane.
					 */
					__attribute__((warn_unused_result))
					T shortestDistance(const euc::EuclideanPoint<T,3>& p);

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
					 * @return A vector containing the values for the normal vector of this plane
					 */
					__attribute__((warn_unused_result))
					static euc::EuclideanVector3D<T> calculateNormal(const euc::EuclideanPoint<T,3>& p1,
																	const euc::EuclideanPoint<T,3>& p2,
																	const euc::EuclideanPoint<T,3>& p3);

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
					 * @return An error status indicating the success or failure of the operation
					 * @retval cupcfd::error::E_SUCCESS The method completed successfully
					 * @retval cupcfd::error::E_EUC_VEC_PARALLEL The line is parallel with the plane and will never intersect
					 * (or will intersect every point if it lies on the plane)
					 */
					__attribute__((warn_unused_result))
					static cupcfd::error::eCodes linePlaneIntersection(euc::EuclideanVector<T,3>& normal,
																		const euc::EuclideanPoint<T,3>& p0,
																		const euc::EuclideanVector<T,3>& l,
																		const euc::EuclideanPoint<T,3>& l0,
																		euc::EuclideanPoint<T,3>& result);
				protected:

				private:

			};
		} // namespace euclidean
	} // namespace geometry
} // namespace cupcfd

// Include Header Level Definitions
#include "EuclideanPlane3D.ipp"

#endif
