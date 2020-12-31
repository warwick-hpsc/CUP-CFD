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

				// this->area = Triangle<S,T,N>::heronsFormula(a,b,c);
				// this->centroid = (a+b+c)/T(3);
				// this->normal = euc::EuclideanPlane3D<T>::calculateNormal(a, b, c);
			}

			template <class S, class T, uint N>
			Triangle<S,T,N>::Triangle(const Triangle<S,T,N>& source) {
				this->vertices[0] = source.vertices[0];
				this->vertices[1] = source.vertices[1];
				this->vertices[2] = source.vertices[2];

				// this->area = source.area;
				// this->centroid = source.centroid;
				// this->normal = source.normal;
			}

			template <class S, class T, uint N>
			Triangle<S,T,N>::~Triangle() {
			}

			template <class S, class T, uint N>
			bool Triangle<S,T,N>::isPointInside(const euc::EuclideanPoint<T,N>& point) {
				return static_cast<S*>(this)->isPointInside(point);
			}
		
			template <class S, class T, uint N>
			T Triangle<S,T,N>::computeArea() {
				T area = Triangle<S,T,N>::heronsFormula(*this);
				if (area == T(0.0)) {
					HARD_CHECK_ECODE(cupcfd::error::E_GEOMETRY_ZERO_AREA)
				}
				if (std::isnan(area) || std::isnan(-area)) {
					HARD_CHECK_ECODE(cupcfd::error::E_GEOMETRY_NAN_AREA)
				}
				return area;
			}
		
			template <class S, class T, uint N>
			euc::EuclideanPoint<T,N> Triangle<S,T,N>::computeCentroid() {
				// Centroid located at arithmetic mean of vertices
				return ((this->vertices[0] + this->vertices[1] + this->vertices[2]) / T(3.0));
			}

			template <class S, class T, uint N>
			euc::EuclideanVector<T,N> Triangle<S,T,N>::computeNormal() {
				return euc::EuclideanPlane3D<T>::calculateNormal(this->vertices[0], this->vertices[1], this->vertices[2]);
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
