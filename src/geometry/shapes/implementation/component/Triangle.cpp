/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Definitions for the Triangle class
 */

#include "Triangle.h"

namespace arth = cupcfd::utility::arithmetic::kernels;
namespace euc = cupcfd::geometry::euclidean;

namespace cupcfd
{
	namespace geometry
	{
		namespace shapes
		{
			// Explicit Instantiation
			template class Triangle<float,2>;
			template class Triangle<double,2>;

			template class Triangle<float,3>;
			template class Triangle<double,3>;
		}
	}
}
