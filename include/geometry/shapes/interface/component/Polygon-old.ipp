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
 */

#ifndef CUPCFD_GEOMETRY_SHAPES_POLYGON_IPP_H
#define CUPCFD_GEOMETRY_SHAPES_POLYGON_IPP_H

namespace euc = cupcfd::geometry::euclidean;

namespace cupcfd
{
	namespace geometry
	{
		namespace shapes
		{
			template <class S, class T, uint N>
			Polygon<S,T,N>::Polygon() {
			}
			
			template <class S, class T, uint N>
			bool Polygon<S,T,N>::isPointInside(euc::EuclideanPoint<T,N>& point) {
				return static_cast<S*>(this)->isPointInside(point);
			}
			
			template <class S, class T, uint N>
			T Polygon<S,T,N>::computeArea() {
				return static_cast<S*>(this)->computeArea();
			}
		}
	}
}

#endif
