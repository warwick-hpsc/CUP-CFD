/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Definitions for the Triangle2D class
 */

#include "Triangle2D.h"

namespace arth = cupcfd::utility::arithmetic::kernels;
namespace euc = cupcfd::geometry::euclidean;

namespace cupcfd
{
	namespace geometry
	{
		namespace shapes
		{

		}
	}
}

// Explicit Instantiation

using namespace cupcfd::geometry::shapes;

template class Triangle2D<float>;
template class Triangle2D<double>;
