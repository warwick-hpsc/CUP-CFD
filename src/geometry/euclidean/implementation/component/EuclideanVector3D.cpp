/*
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * This class contains the implementation of the EuclideanVector3D class.
 */

#include "EuclideanPoint.h"
#include "LineSegment3D.h"

namespace cupcfd
{
	namespace geometry
	{
		namespace euclidean
		{

			template <class T>
			bool isPointOnLine(const EuclideanPoint<T,3>& x1, const EuclideanPoint<T,3>& x2,
									  const EuclideanPoint<T,3>& p)
			{
				LineSegment3D<T> line(x1, x2);
				return line.isPointOnLine(p);
			}

			template <class T>
			bool isVectorRangeIntersection(const EuclideanPoint<T,3>& x1, const EuclideanPoint<T,3>& x2,
											const EuclideanPoint<T,3>& x3, const EuclideanPoint<T,3>& x4)
			{
				// Needed to store result, but we discard it
				EuclideanPoint<T,3> intersectPoint(T(0.0), T(0.0), T(0.0));
				cupcfd::error::eCodes status = computeVectorRangeIntersection(x1, x2, x3, x4, intersectPoint);
				
				if(status == cupcfd::error::E_SUCCESS) {
					return true;
				}
				else {
					return false;
				}
			}

			template <class T>
			cupcfd::error::eCodes computeVectorRangeIntersection(const EuclideanPoint<T,3>& x1, const EuclideanPoint<T,3>& x2,
										 	  	  	  	  	  	  	const EuclideanPoint<T,3>& x3, const EuclideanPoint<T,3>& x4,
																	EuclideanPoint<T,3>& intersectPoint)
			{
				LineSegment3D<T> line1(x1, x2);
				LineSegment3D<T> line2(x3, x4);
				return line1.lineIntersection(line2, intersectPoint);
			}

			// Explicit Instantiation
			template bool isPointOnLine(const EuclideanPoint<double,3>&, const EuclideanPoint<double,3>&, const EuclideanPoint<double,3>&);
			template bool isPointOnLine(const EuclideanPoint<float,3>&, const EuclideanPoint<float,3>&, const EuclideanPoint<float,3>&);

			template bool isVectorRangeIntersection(const EuclideanPoint<double,3>&, const EuclideanPoint<double,3>&,
													const EuclideanPoint<double,3>&, const EuclideanPoint<double,3>&);
			template bool isVectorRangeIntersection(const EuclideanPoint<float,3>&, const EuclideanPoint<float,3>&,
													const EuclideanPoint<float,3>&, const EuclideanPoint<float,3>&);
		}
	}
}
