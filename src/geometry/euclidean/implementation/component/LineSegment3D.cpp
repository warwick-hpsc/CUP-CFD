/*
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * This class contains the implementation of the LineSegment3D class.
 */

#include "LineSegment3D.h"

#include "EuclideanPlane3D.h"

namespace cupcfd
{
	namespace geometry
	{
		namespace euclidean
		{
			template <class T>
			LineSegment3D<T>::LineSegment3D(const EuclideanPoint<T,3>& source, const EuclideanPoint<T,3>& finish)
			: source(source)
			{
				this->v = finish - source;
				this->length = this->v.length();
			}

			template <class T>
			void LineSegment3D<T>::print() const
			{
				printf("Line segment: start = "); this->source.print(); 
				printf(", end = "); (this->source + this->v).print(); 
				printf(", length = %.2e", this->length); 
				printf("\n");
			}

			template <class T>
			bool LineSegment3D<T>::isLineColinear(const LineSegment3D<T>& line) const
			{
				if (this->length == T(0) || line.length == T(0)) {
					return false;
				}
				if (this->v.isParallel(line.v)) {
					if (this->v.isParallel(line.source - this->source)) {
						return true;
					}
				}

				return false;
			}

			template <class T>
			bool LineSegment3D<T>::isPointOnLine(const EuclideanPoint<T,3>& p) const
			{
				if (p == this->source || p == (this->source+this->v)) {
					return true;
				}

				EuclideanVector<T,3> x2p = p - this->source;
				if (!this->v.isParallel(x2p)) {
					return false;
				}
				// Point is on line's vector, now just to check it falls on line segment:
				
				// Check whether the direction is the same or is 180
				// by finding the sign of the scalar multiplier (u = r * v) for vectors u,v and scalar r
				// Search for first non-zero value to compute the scalar
				// Should be non-zero for both
				T scalar;
				bool scalarFound = false;
				for(uint i = 0; i < 3; i++) {
					if(!(arth::isEqual(x2p.cmp[i], T(0)))) {
						scalar = x2p.cmp[i] / this->v.cmp[i];
						scalarFound = true;
						break;
					}
				}
				if (!scalarFound) {
					HARD_CHECK_ECODE(cupcfd::error::E_GEOMETRY_LOGIC_ERROR)
				}

				return (scalar >= T(0) && scalar <= T(1));
			}

			template <class T>
			cupcfd::error::eCodes LineSegment3D<T>::lineIntersection(const LineSegment3D<T>& line2, EuclideanPoint<T,3>& intersectPoint) {
				bool debug_print = false;

				if (debug_print) printf("LineSegment3D::lineIntersection(): Calculating intersection between two lines:\n");
				if (debug_print) this->print();
				if (debug_print) line2.print();

				EuclideanPoint<T,3> x1 = this->source;
				EuclideanPoint<T,3> x2 = this->source + this->v;
				EuclideanPoint<T,3> x3 = line2.source;
				EuclideanPoint<T,3> x4 = line2.source + line2.v;

				if (this->length == T(0) && line2.length == T(0)) {
					if (this->source == line2.source) {
						if (debug_print) printf("LineSegment3D::lineIntersection(): both 'lines' are co-located points\n");
						intersectPoint = this->source;
						return cupcfd::error::E_SUCCESS;
					}
				}
				if (this->length == T(0)) {
					if (line2.isPointOnLine(this->source)) {
						if (debug_print) printf("LineSegment3D::lineIntersection(): 'this' is a point on line2\n");
						intersectPoint = this->source;
						return cupcfd::error::E_SUCCESS;
					}
				}
				if (line2.length == T(0)) {
					if (this->isPointOnLine(line2.source)) {
						if (debug_print) printf("LineSegment3D::lineIntersection(): 'line2' is a point on this\n");
						intersectPoint = line2.source;
						return cupcfd::error::E_SUCCESS;
					}
				}

				if (this->isLineColinear(line2)) {
					if (debug_print) printf("LineSegment3D::lineIntersection(): lines are colinear, scanning for an endpoint on both\n");
					if (x3 >= x1 && x3 <= x2) {
						intersectPoint = x3;
						return cupcfd::error::E_SUCCESS;
					}
					if (x4 >= x1 && x4 <= x2) {
						intersectPoint = x4;
						return cupcfd::error::E_SUCCESS;
					}
					if (x1 >= x3 && x1 <= x4) {
						intersectPoint = x1;
						return cupcfd::error::E_SUCCESS;
					}
					if (x2 >= x3 && x2 <= x4) {
						intersectPoint = x2;
						return cupcfd::error::E_SUCCESS;
					}
					return cupcfd::error::E_GEOMETRY_NO_INTERSECT;
				}

				EuclideanPlane3D<T> plane(x1, x2, x3);
				if (!plane.isPointOnPlane(x4)) {
					return cupcfd::error::E_GEOMETRY_NO_INTERSECT;
				}

				// https://mathworld.wolfram.com/Line-LineIntersection.html
				EuclideanVector3D<T> c = x3-x1;
				T sn = (c.crossProduct(line2.v)).dotProduct(this->v.crossProduct(line2.v));
				T sd = this->v.crossProduct(line2.v).length();
				T s = sn / (sd*sd);
				if (debug_print) printf("LineSegment3D::lineIntersection(): sd = %.2e\n", sd);
				if (debug_print) printf("LineSegment3D::lineIntersection(): sn = %.2e\n", sn);
				if (debug_print) printf("LineSegment3D::lineIntersection(): s = %.2e\n", s);

				if (s >= T(0) && s <= T(1)) {
					intersectPoint = x1 + this->v*s;
					if (debug_print) {
						printf("LineSegment3D::lineIntersection(): intersectPoint: "); intersectPoint.print(); printf("\n");
					}
					// if (!this->isPointOnLine(intersectPoint)) {
					// 	return cupcfd::error::E_GEOMETRY_NO_INTERSECT;
					// }

					EuclideanVector3D<T> vi = intersectPoint-x3;
					if (debug_print) {
						printf("LineSegment3D::lineIntersection(): vi:      "); vi.print(); printf("\n");
						printf("LineSegment3D::lineIntersection(): line2.v: "); line2.v.print(); printf("\n");
					}
					if (line2.isPointOnLine(intersectPoint)) {
						if (debug_print) printf("LineSegment3D::lineIntersection(): intersectPoint is on line2\n");
						return cupcfd::error::E_SUCCESS;
					}
				}

				return cupcfd::error::E_GEOMETRY_NO_INTERSECT;
			}

			// Explicit instantiation:
			template class LineSegment3D<double>;
			template class LineSegment3D<float>;
		}
	}
}
