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
#include "EuclideanPlane3D.h"

namespace arth = cupcfd::utility::arithmetic::kernels;
namespace euc = cupcfd::geometry::euclidean;
			
namespace cupcfd
{
	namespace geometry
	{
		namespace shapes
		{
			// === Constructors/Deconstructors ===

			template <class S, class T, uint N>
			Triangle<S,T,N>::Triangle() {
			}

			template <class S, class T, uint N>
			Triangle<S,T,N>::Triangle(const euc::EuclideanPoint<T,N>& a,
								  	const euc::EuclideanPoint<T,N>& b,
								    const euc::EuclideanPoint<T,N>& c)
			{
				this->vertices[0] = a;
				this->vertices[1] = b;
				this->vertices[2] = c;
			}

			template <class S, class T, uint N>
			Triangle<S,T,N>::~Triangle() {
			}

			template <class S, class T, uint N>
			T Triangle<S,T,N>::heronsFormula(T abLength, T acLength, T bcLength)
			{
				// Heron's formula: Calculate area purely from edge lengths
				T s = (abLength + acLength + bcLength) / T(2);
				return arth::sqrtWr(s * (s - abLength) * (s - bcLength) * (s - acLength));
			}
		
			template <class S, class T, uint N>
			T Triangle<S,T,N>::heronsFormula(const cupcfd::geometry::euclidean::EuclideanPoint<T,N>& a,
										   	const cupcfd::geometry::euclidean::EuclideanPoint<T,N>& b,
											const cupcfd::geometry::euclidean::EuclideanPoint<T,N>& c)
			{
				cupcfd::geometry::euclidean::EuclideanVector<T,N> ab = b - a;
				cupcfd::geometry::euclidean::EuclideanVector<T,N> ac = c - a;
				cupcfd::geometry::euclidean::EuclideanVector<T,N> bc = c - b;
				return Triangle<S,T,N>::heronsFormula((b-a).length(), (c-a).length(), (c-b).length());
			}
			
			template <class S, class T, uint N>
			T Triangle<S,T,N>::heronsFormula(const Triangle<S,T,N>& tri) {
				return Triangle<S,T,N>::heronsFormula(tri.vertices[0], tri.vertices[1], tri.vertices[2]);
			}
		}
	}
}

#endif
