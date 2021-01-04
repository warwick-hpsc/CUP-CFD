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

#include "Polygon3D.h"

#include "Quadrilateral3D.h"
#include "Triangle3D.h"

namespace cupcfd
{
	namespace geometry
	{
		namespace shapes
		{
			template <class P, class T, uint V>
			T Polygon3D<P,T,V>::computeArea() {
				T area = T(0);
				euc::EuclideanPoint<T,3> v0 = this->vertices[0];
				for (int i2=2; i2<V; i2++) {
					const uint i1 = i2-1;
					cupcfd::geometry::shapes::Triangle3D<T> t(v0, this->vertices[i1], this->vertices[i2]);
					area += t.getArea();
				}
				return area;
			}

			// Explicit Instantiation
			template class Polygon3D<Quadrilateral3D<float>, float, 4>;
			template class Polygon3D<Quadrilateral3D<double>, double, 4>;

			template class Polygon3D<Triangle3D<float>, float, 3>;
			template class Polygon3D<Triangle3D<double>, double, 3>;
		}
	}
}
