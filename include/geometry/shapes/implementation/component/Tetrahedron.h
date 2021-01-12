/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Declarations for the Tetrahedron class
 *
 */

#ifndef CUPCFD_GEOMETRY_SHAPES_TETRAHEDRON_INCLUDE_H
#define CUPCFD_GEOMETRY_SHAPES_TETRAHEDRON_INCLUDE_H

#include "Polygon3D.h"
#include "Polyhedron.h"
#include "EuclideanPoint.h"
#include "Triangle3D.h"
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
			 * @tparam T Numerical type
			 */
			template <class T>
			class Tetrahedron : public Pyramid<Triangle3D<T>,T>
			{
				public:
					// === Members ===

					// === Constructors/Deconstructors ===

					/**
					 * Default constructor
					 */
					Tetrahedron();

					/**
					 * Build a Tetrahedron object.
					 *
					 * @param apex The point that defines the apex of pyramid
					 * @param base The Triangle3D that defines the base of pyramid
					 */
					Tetrahedron(const euc::EuclideanPoint<T,3>& apex,
								const shapes::Triangle3D<T>& base);

					/**
					 *
					 */
					~Tetrahedron();

					/**
					 * Pretty-print to STDOUT
					 */
					void print();
			};
		}
	}
}

// Include Header Level Definitions
#include "Tetrahedron.ipp"

#endif
