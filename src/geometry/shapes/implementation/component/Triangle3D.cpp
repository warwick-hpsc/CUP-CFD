/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Definitions for the Triangle3D class
 */

#include "Triangle3D.h"

namespace arth = cupcfd::utility::arithmetic::kernels;
namespace euc = cupcfd::geometry::euclidean;

namespace cupcfd
{
	namespace geometry
	{
		namespace shapes
		{
			// Explicit Instantiation
			template class Triangle3D<float>;
			template class Triangle3D<double>;
		}
	}
}
