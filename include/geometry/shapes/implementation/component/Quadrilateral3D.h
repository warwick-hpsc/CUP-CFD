/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Description
 *
 * Declarations for the Quadrilateral3D class
 */

#ifndef CUPCFD_GEOMETRY_SHAPES_QUADRILATERAL_3D_INCLUDE_H
#define CUPCFD_GEOMETRY_SHAPES_QUADRILATERAL_3D_INCLUDE_H

#include "EuclideanPoint.h"
#include "EuclideanVector.h"
#include "Polygon3D.h"

namespace euc = cupcfd::geometry::euclidean;

namespace cupcfd
{
	namespace geometry
	{
		namespace shapes
		{
			/**
			 * Class for storing 2D quadrilateral definition data and operations in a
			 * N-dimensional space.
			 *
			 * Uses CRTP Design.
			 *
			 * @tparam T The data type of the coordinate system
			 * @tparam N The dimension of the euclidean space the quadrilateral exists in.
			 */
			template <class T>
			class Quadrilateral3D : public Polygon3D<Quadrilateral3D<T>, T, 4>
			{
				public:
					// === Members ===

					/** Vertices that make up the 3D Quadrilateral **/

					// === Constructors/Deconstructors ===

					/**
					 * Default constructor.
					 *
					 * Creates a quadrilateral that is a square of 0.0->1.0 in the XY Plane, vertices
					 * in clockwise order from the origin.
					 */
					Quadrilateral3D();

					/**
					 * Construct a quadrilateral defined by four points a,b,c and d where edges exist between
					 * (i) a and b
					 * (ii) b and c
					 * (iii) c and d
					 * (iv) d and a
					 *
					 * The 4 points are expected to be coplanar.
					 *
					 * @param a Point a
					 * @param b Point b
					 * @param c Point c
					 * @param d Point d
					 */
					Quadrilateral3D(const euc::EuclideanPoint<T,3>& a,
									const euc::EuclideanPoint<T,3>& b,
									const euc::EuclideanPoint<T,3>& c,
									const euc::EuclideanPoint<T,3>& d);

					/**
					 * Construct a Quadrilateral that has a copy of the values stored
					 * in the source object.
					 *
					 * @param source The Quadrilateral3D to copy the values from
					 */
					Quadrilateral3D(const Quadrilateral3D<T>& source);


					/**
					 * Deconstructor
					 */
					~Quadrilateral3D();

					// === Inherited Overloaded Methods ===

					/**
					 * Returns a cloned copy of this object
					 *
					 * @return A pointer to a new Quadrilateral3D object, with values copied from this object.
					 */
					__attribute__((warn_unused_result))
					Quadrilateral3D<T> * clone();

					// === Static Methods ===

					/**
					 * Compute the area of a quadrilateral by using the sum of the triangular areas
					 * that make up a quadrilateral
					 * defined by four vertices a,b,c and d where edges exist between
					 * (i) a and b
					 * (ii) b and c
					 * (iii) c and d
					 * (iv) d and a
					 *
					 * @param a Vertex a
					 * @param b Vertex b
					 * @param c Vertex c
					 * @param d Vertex d
					 *
					 * @return The area of the quadrilateral
					 */
					__attribute__((warn_unused_result))
					static T triangularAreaSum(const euc::EuclideanPoint<T,3>& a,
											   const euc::EuclideanPoint<T,3>& b,
											   const euc::EuclideanPoint<T,3>& c,
											   const euc::EuclideanPoint<T,3>& d);

					/**
					 * ToDo: This method is not yet implemented
					 * Test that the four vertices provided make up a valid quadrilateral - i .e. the edges
					 * between the points do not intersect with one another (other than at the vertices)
					 * given the following definition:
					 *
					 * The quadrilateral is defined by four points a,b,c and d where edges exist between
					 * (i) a and b
					 * (ii) b and c
					 * (iii) c and d
					 * (iv) d and a
					 *
					 * or alternatively, that these vertices by their labels and the edge layout above only
					 * enclose a single area.
					 *
					 * This function also enforces that the 4 points must be coplanar.
					 *
					 * This is primarily intended for debugging purposes, to ensure that the points are
					 * appropriate for quadrilateral operations
					 *
					 * @param a Vertex a
					 * @param b Vertex b
					 * @param c Vertex c
					 * @param d Vertex d
					 *
					 * @return A true if the vertices provided make a valid quadrilateral
					 */
					// static T validQuadrilateralPoints(euc::EuclideanPoint<T,3>& a,
					// 						    euc::EuclideanPoint<T,3>& b,
					// 						    euc::EuclideanPoint<T,3>& c,
					// 						    euc::EuclideanPoint<T,3>& d);


					/**
					 * Compute the centroid of 4 points in a 3D space using the intersection of the bimedians
					 * of the quadrilateral.
					 *
					 * It is assumed that the 4 points provided make up a quadrilateral when traversed in order - i.e.
					 * edges exist between ab, bc, cd and ad.
					 *
					 * Whether they are clockwise or anticlockwise in order does not matter for this function.
					 *
					 * @return The computed centroid position
					 */
					// static euc::EuclideanPoint<T,3> computeCentroidBiMedianIntersection(
					// 											const euc::EuclideanPoint<T,3>& a,
					// 											const euc::EuclideanPoint<T,3>& b,
					// 											const euc::EuclideanPoint<T,3>& c,
					// 											const euc::EuclideanPoint<T,3>& d);

					/**
					 * Test whether a point is inside by tracing a vector from the point to the centroid.
					 * If it crosses any of the edges then it is outside the Quadrilateral.
					 *
					 * Points on an edge or vertex are considered inside the Quadrilateral.
					 *
					 * Points not on the same plane as the Quadrilateral are considered outside the Quadrilateral.
					 *
					 * The points provided should be in order of connectivity via edges (i.e. edges exist between
					 * ab, bc, cd and ad). They also must be coplanar (if not coplanar, then it is recommended
					 * that two checks be done using the test for Triangle3D instead).
					 *
					 * @param a Point 1 of a Quadrilateral
					 * @param b Point 2 of a Quadrilateral
					 * @param c Point 3 of a Quadrilateral
					 * @param d Point 4 of a Quadrilateral
					 * @param p The point to test the position of
					 *
					 * @return Return true if the point is inside the Quadrilateral made up by the three points
					 */
					__attribute__((warn_unused_result))
					static inline bool isPointInsideCentroid(const euc::EuclideanPoint<T,3>& a,
														    const euc::EuclideanPoint<T,3>& b,
															const euc::EuclideanPoint<T,3>& c,
															const euc::EuclideanPoint<T,3>& d,
															const euc::EuclideanPoint<T,3>& p);

					// === Concrete Methods ===

					/**
					 * Determine whether the provided point is inside the Quadrilateral.
					 * Edges/Vertices are treated as inside the polygon for this purpose.
					 *
					 * @return Return true if the point exists inside this Quadrilateral
					 */
					__attribute__((warn_unused_result))
					inline bool isPointInside(const euc::EuclideanPoint<T,3>& point);

				// protected:

					/**
					 * Compute the normal of the 3D Quadrilateral.
					 * Direction is determined by the ordering of the first three vertices.
					 *
					 * @return Return the computed normal vector of the Quadrilateral.
					 */
					// __attribute__((warn_unused_result))
					// inline euc::EuclideanVector<T,3> computeNormal();

					/**
					 * Compute the center of the four points that make up this Quadrilateral
					 *
					 * @return The point at the center of the four points
					 */
					// __attribute__((warn_unused_result))
					// inline euc::EuclideanPoint<T,3> computeCentroid();
			};
		}
	}
}

// Include Header Level Definitions
#include "Quadrilateral3D.ipp"

#endif

