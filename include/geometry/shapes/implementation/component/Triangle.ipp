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
 * Declarations for the Triangle class
 */

#ifndef CUPCFD_GEOMETRY_SHAPES_TRIANGLE_IPP_H
#define CUPCFD_GEOMETRY_SHAPES_TRIANGLE_IPP_H

#include "ArithmeticKernels.h"

namespace arth = cupcfd::utility::arithmetic::kernels;
namespace euc = cupcfd::geometry::euclidean;
			
namespace cupcfd
{
	namespace geometry
	{
		namespace shapes
		{
		
			template <class T, unsigned int N>
			T Triangle<T,N>::heronsFormula(cupcfd::geometry::euclidean::EuclideanPoint<T,N>& a,
								   	   	   cupcfd::geometry::euclidean::EuclideanPoint<T,N>& b,
										   cupcfd::geometry::euclidean::EuclideanPoint<T,N>& c)
			{
				// https://en.wikipedia.org/wiki/Heron's_formula

				cupcfd::geometry::euclidean::EuclideanVector<T,N> ab = b - a;
				cupcfd::geometry::euclidean::EuclideanVector<T,N> ac = c - a;
				cupcfd::geometry::euclidean::EuclideanVector<T,N> bc = c - b;

				T abLength = ab.length();
				T acLength = ac.length();
				T bcLength = bc.length();

				T s = (abLength + acLength + bcLength) / ((T) 2);
				return arth::sqrtWr(s * (s - abLength) * (s - bcLength) * (s - acLength));
			}

			template <class T, unsigned int N>
			T Triangle<T,N>::heronsFormula(T abLength, T acLength, T bcLength)
			{
				// https://en.wikipedia.org/wiki/Heron's_formula
				T s = (abLength + acLength + bcLength) / ((T) 2);
				return arth::sqrtWr(s * (s - abLength) * (s - bcLength) * (s - acLength));
			}
		}
	}
}

#endif
