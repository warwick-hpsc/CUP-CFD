/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Header Level Definitions for the Tetrahedron class
 */

#ifndef CUPCFD_GEOMETRY_SHAPES_TETRAHEDRON_IPP_H
#define CUPCFD_GEOMETRY_SHAPES_TETRAHEDRON_IPP_H

namespace euc = cupcfd::geometry::euclidean;
namespace shapes = cupcfd::geometry::shapes;

namespace cupcfd
{
	namespace geometry
	{
		namespace shapes
		{
			// === Constructors/Deconstructors ===

			template <class T>
			Tetrahedron<T>::Tetrahedron(const euc::EuclideanPoint<T,3>& apex,
										const shapes::Triangle3D<T>& base)
			: Pyramid<Triangle3D<T>,T>(apex, base)
			{
			}
			
			template <class T>
			Tetrahedron<T>::~Tetrahedron()
			{
			}
		}
	}
}

#endif
