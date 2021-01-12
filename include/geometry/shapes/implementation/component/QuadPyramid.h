/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Declarations for the QuadPyramid class
 *
 */

#ifndef CUPCFD_GEOMETRY_SHAPES_QUADPYRAMID_INCLUDE_H
#define CUPCFD_GEOMETRY_SHAPES_QUADPYRAMID_INCLUDE_H

#include "Polygon3D.h"
#include "Polyhedron.h"
#include "EuclideanPoint.h"
#include "Quadrilateral3D.h"
#include "Pyramid.h"

namespace euc = cupcfd::geometry::euclidean;
namespace shapes = cupcfd::geometry::shapes;

namespace cupcfd
{
	namespace geometry
	{
		namespace shapes
		{
			/**
			 * Class to store geometry information and access/compute various properties
			 * of Tetrahedrons
			 *
			 * @tparam T Datatype of the geometry
			 */
			template <class T>
			class QuadPyramid : public Pyramid<Quadrilateral3D<T>,T>
			{
				public:
					/**
					 * Build a Pyramid object.
					 *
					 * @param apex The point that defines the apex of the pyramid
					 * @param quad The Quadrilateral3D base of the pyramid
					 */
					QuadPyramid(const euc::EuclideanPoint<T,3>& apex, 
								const shapes::Quadrilateral3D<T>& quad);

					/**
					 *
					 */
					~QuadPyramid();
			};
		}
	}
}

// Include Header Level Definitions
#include "QuadPyramid.ipp"

#endif
