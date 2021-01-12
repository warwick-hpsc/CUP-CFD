/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Contains the header level definitions for the LineSegment3D class.
 */

#ifndef CUPCFD_GEOMETRY_LINE_SEGMENT3D_IPP_H
#define CUPCFD_GEOMETRY_LINE_SEGMENT3D_IPP_H

namespace cupcfd
{
	namespace geometry
	{
		namespace euclidean
		{
			template <class T>
			LineSegment3D<T>::LineSegment3D()
			: v(), source()
			{

			}

			template <class T>
			LineSegment3D<T>::LineSegment3D(const EuclideanVector<T,3>& vector, const EuclideanPoint<T,3>& source)
			: v(vector), source(source)
			{

			}

			template <class T>
			LineSegment3D<T>::~LineSegment3D()
			{

			}

		} // namespace euclidean
	} // namespace geometry
} // namespace cupcfd

#endif
