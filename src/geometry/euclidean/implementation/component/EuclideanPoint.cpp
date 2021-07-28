/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * This file contains the declarations for the EuclideanPoint class
 */

#include "EuclideanPoint.h"
#include "Error.h"

namespace cupcfd
{
	namespace geometry
	{
		namespace euclidean
		{
			// ToDo: Would rather do away with this and make it fully in the header,
			// but need to research briefly if it is possible to initialise static variables there....

			// Initialise Static Variables
			template <class T, unsigned int N>
			bool EuclideanPoint<T,N>::mpiDataTypeReg = false;

			template <class T, unsigned int N>
			MPI_Datatype EuclideanPoint<T,N>::mpiType = reinterpret_cast<MPI_Datatype>(-1);
		}
	}
}

// Explicit Instantiation
template class cupcfd::geometry::euclidean::EuclideanPoint<int, 2>;
template class cupcfd::geometry::euclidean::EuclideanPoint<float, 2>;
template class cupcfd::geometry::euclidean::EuclideanPoint<double, 2>;

template class cupcfd::geometry::euclidean::EuclideanPoint<int, 3>;
template class cupcfd::geometry::euclidean::EuclideanPoint<float, 3>;
template class cupcfd::geometry::euclidean::EuclideanPoint<double, 3>;
