/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Header Level Definitions for the QuadPyramid class
 */

#ifndef CUPCFD_GEOMETRY_SHAPES_QUADPYRAMID_IPP_H
#define CUPCFD_GEOMETRY_SHAPES_QUADPYRAMID_IPP_H

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
			QuadPyramid<T>::QuadPyramid(const euc::EuclideanPoint<T,3>& apex, 
										const shapes::Quadrilateral3D<T>& base)
			 : Pyramid<Quadrilateral3D<T>,T>(apex, base)
			{
			}
			
			template <class T>
			QuadPyramid<T>::~QuadPyramid()
			{
			}
		}
	}
}

#endif
