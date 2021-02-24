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
#include "EuclideanPoint.h"

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
			: Polygon<Triangle<S,T,N>,T,N,3>(a, b, c)
			{
			}

			template <class S, class T, uint N>
			Triangle<S,T,N>::~Triangle() {
			}

			template <class S, class T, uint N>
			auto Triangle<S,T,N>::isPointInside(const euc::EuclideanPoint<T,N>& point) {
				return static_cast<S*>(this)->isPointInside(point);
			}

			template <class S, class T, uint N>
			T Triangle<S,T,N>::computeArea() {
				return Triangle<S,T,N>::heronsFormula(this->vertices[0], this->vertices[1], this->vertices[2]);
			}

			template <class S, class T, uint N>
			T Triangle<S,T,N>::getArea() {
				if (!this->areaComputed) {
					this->area = this->computeArea();
					this->areaComputed = true;
				}
				return this->area;
			}

			template <class S, class T, uint N>
			euc::EuclideanPoint<T,N> Triangle<S,T,N>::computeCentroid() {
				// Centroid located at arithmetic mean of three vertices:
				return ((this->vertices[0]+this->vertices[1]+this->vertices[2]) / T(3.0));
			}

			template <class S, class T, uint N>
			euc::EuclideanPoint<T,N> Triangle<S,T,N>::getCentroid() {
				if (!this->centroidComputed) {
					this->centroid = this->computeCentroid();
					this->centroidComputed = true;
				}
				return this->centroid;
			}

			template <class S, class T, uint N>
			T Triangle<S,T,N>::heronsFormula(T abLength, T acLength, T bcLength)
			{
				// Heron's formula: Calculate area purely from edge lengths
				T s = (abLength + acLength + bcLength) / T(2);
				return arth::sqrtWr( s*(s-abLength) * (s-bcLength) * (s-acLength) );
			}
		
			template <class S, class T, uint N>
			T Triangle<S,T,N>::heronsFormula(const euc::EuclideanPoint<T,N>& a,
										   	const euc::EuclideanPoint<T,N>& b,
											const euc::EuclideanPoint<T,N>& c)
			{
				euc::EuclideanVector<T,N> ab = b - a;
				euc::EuclideanVector<T,N> ac = c - a;
				euc::EuclideanVector<T,N> bc = c - b;
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
