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
			 */
			template <class T>
			class Quadrilateral3D : public Polygon3D<Quadrilateral3D<T>, T>
			{
				public:
					// === Members ===

					/** Vertices that make up the 3D Quadrilateral **/
					cupcfd::geometry::euclidean::EuclideanPoint<T,3> vertices[4];

					// === Constructors/Deconstructors ===

					/**
					 * Default constructor.
					 *
					 * Creates a quadrilateral that is a square of 0.0->1.0 in the XY Plane, vertices
					 * in clockwise order from the origin.
					 *
					 * @tparam T The data type of the coordinate system
					 * @tparam N The dimension of the euclidean space the quadrilateral exists in.
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
					 * @param a Point a of the quadrilateral
					 * @param b Point b of the quadrilateral
					 * @param c Point c of the quadrilateral
					 * @param d Point d of the quadrilateral
					 *
					 * @tparam T The data type of the coordinate system
					 * @tparam N The dimension of the euclidean space the quadrilateral exists in.
					 */
					Quadrilateral3D(const cupcfd::geometry::euclidean::EuclideanPoint<T,3>& a,
									const cupcfd::geometry::euclidean::EuclideanPoint<T,3>& b,
									const cupcfd::geometry::euclidean::EuclideanPoint<T,3>& c,
									const cupcfd::geometry::euclidean::EuclideanPoint<T,3>& d);

					/**
					 * Construct a Quadrilateral that has a copy of the values stored
					 * in the source object.
					 *
					 * @param source The object to copy the values from
					 *
					 * @tparam T The data type of the coordinate system
					 * @tparam N The dimension of the euclidean space the quadrilateral exists in.
					 */
					Quadrilateral3D(const Quadrilateral3D<T>& source);


					/**
					 * Deconstructor
					 */
					~Quadrilateral3D();

					// === Inherited Overloaded Methods ===

					/**
					 * Copys the data from the source to this object
					 *
					 * @param plane The object to copy from
					 *
					 * @tparam T The type of the coordinate system
					 */
					inline void operator=(const Quadrilateral3D<T>& source);

					/**
					 * Returns a cloned copy of this object
					 *
					 * @return A pointer to a new MeshConfig object
					 * with copied values from this object.
					 */
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
					 * @param a Vertex a of the quadrilateral
					 * @param b Vertex b of the quadrilateral
					 * @param c Vertex c of the quadrilateral
					 * @param d Vertex d of the quadrilateral
					 *
					 * @tparam T The data type of the coordinate system
					 * @tparam N The dimension of the euclidean space the quadrilateral exists in.
					 *
					 * @return The area of the quadrilateral
					 */
					static T triangularAreaSum(const cupcfd::geometry::euclidean::EuclideanPoint<T,3>& a,
											   const cupcfd::geometry::euclidean::EuclideanPoint<T,3>& b,
											   const cupcfd::geometry::euclidean::EuclideanPoint<T,3>& c,
											   const cupcfd::geometry::euclidean::EuclideanPoint<T,3>& d);

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
					 * @param a Vertex a of the quadrilateral
					 * @param b Vertex b of the quadrilateral
					 * @param c Vertex c of the quadrilateral
					 * @param d Vertex d of the quadrilateral
					 *
					 * @tparam T The data type of the coordinate system
					 *
					 * @return A boolean indicating whether the vertices provided make a valid quadrilateral
					 * @retval true The four vertices produce a valid quadrilateral
					 * @retval false The four vertices as per the positional definition for edges do not
					 * produce a valid quadrilateral that encloses a single area.
					 */
					// static T validQuadrilateralPoints(cupcfd::geometry::euclidean::EuclideanPoint<T,3>& a,
					// 						    cupcfd::geometry::euclidean::EuclideanPoint<T,3>& b,
					// 						    cupcfd::geometry::euclidean::EuclideanPoint<T,3>& c,
					// 						    cupcfd::geometry::euclidean::EuclideanPoint<T,3>& d);


					/**
					 * Compute the centroid of 4 points in a 3D space using the intersection of the bimedians
					 * of the quadrilateral.
					 *
					 * It is assumed that the 4 points provided make up a quadrilateral when traversed in order - i.e.
					 * edges exist between ab, bc, cd and ad.
					 *
					 * Whether they are clockwise or anticlockwise in order does not matter for this function.
					 *
					 * @tparam T The data type of the coordinate system
					 *
					 * @return The computed centroid position
					 */
					static cupcfd::geometry::euclidean::EuclideanPoint<T,3> computeCentroidBiMedianIntersection(
																 const cupcfd::geometry::euclidean::EuclideanPoint<T,3>& a,
																 const cupcfd::geometry::euclidean::EuclideanPoint<T,3>& b,
																 const cupcfd::geometry::euclidean::EuclideanPoint<T,3>& c,
																 const cupcfd::geometry::euclidean::EuclideanPoint<T,3>& d);

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
					 * @return Whether the point is inside the Quadrilateral made up by the three points
					 * @retval true The point is inside or on an edge/vertex
					 * @retval false The point is not inside the Quadrilateral
					 */
					static inline bool isPointInsideCentroid(const cupcfd::geometry::euclidean::EuclideanPoint<T,3>& a,
														     const cupcfd::geometry::euclidean::EuclideanPoint<T,3>& b,
															 const cupcfd::geometry::euclidean::EuclideanPoint<T,3>& c,
															 const cupcfd::geometry::euclidean::EuclideanPoint<T,3>& d,
															 const cupcfd::geometry::euclidean::EuclideanPoint<T,3>& p);

					// === Concrete Methods ===

					/**
					 * Determine whether the provided point is inside the Quadrilateral.
					 * Edges/Vertices are treated as inside the polygon for this purpose.
					 *
					 * @tparam T The type of the spatial domain
					 * @tparam N The dimension of the spatial domain that the shape exists in
					 *
					 * @return Return whether the point exists inside this Quadrilateral
					 * @retval true The point is inside the Quadrilateral
					 * @retval false The point is outside the Quadrilateral
					 */
					inline bool isPointInside(const cupcfd::geometry::euclidean::EuclideanPoint<T,3>& point);

					/**
					 * Compute the area of the Quadrilateral
					 *
					 * @tparam T The type of the spatial domain
					 *
					 * @return Return the computed area of the Quadrilateral.
					 */
					T computeArea();

					/**
					 * Compute the normal of the 3D Quadrilateral.
					 * Direction is determined by the ordering of the first three vertices.
					 *
					 * @tparam T The type of the spatial domain
					 *
					 * @return Return the computed normal vector of the Quadrilateral.
					 */
					inline cupcfd::geometry::euclidean::EuclideanVector<T,3> computeNormal();

					/**
					 * Compute the center of the four points that make up this Quadrilateral
					 *
					 * @return The point at the center of the four points
					 */
					inline cupcfd::geometry::euclidean::EuclideanPoint<T,3> computeCentroid();
			};
		}
	}
}

// Include Header Level Definitions
#include "Quadrilateral3D.ipp"

#endif

