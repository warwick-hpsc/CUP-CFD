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
			 * @tparam T Numerical type
			 * @tparam N Number of spatial dimensions
			 */
			template <class T>
			class Quadrilateral3D : public Polygon3D<T, 4>
			{
				public:
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
			};
		}
	}
}

// Include Header Level Definitions
#include "Quadrilateral3D.ipp"

#endif

