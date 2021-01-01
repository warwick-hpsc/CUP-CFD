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

#include "EuclideanVector3D.h"

namespace cupcfd
{
	namespace geometry
	{
		namespace euclidean
		{
			// template <class T>
			// EuclideanVector3D<T>::EuclideanVector3D(T a, T b, T c)
			// {
			// 	this->cmp[0] = a;
			// 	this->cmp[1] = b;
			// 	this->cmp[2] = c;
			// }

			// template <class T>
			// EuclideanVector3D<T>::EuclideanVector3D(const EuclideanVector<T,3>& v) {
			// 	this->cmp[0] = v.cmp[0];
			// 	this->cmp[1] = v.cmp[1];
			// 	this->cmp[2] = v.cmp[2];
			// }

			// template <class T>
			// EuclideanVector3D<T> crossProduct(const EuclideanVector<T,3>& vec1, const EuclideanVector<T,3>& vec2) {
			// 	EuclideanVector3D<T> v1(vec1);
			// 	EuclideanVector3D<T> v2(vec2);
			// 	return v1.crossProduct(v2);
			// }

			// Explicit Instantiation
			// ToDo: This is kind of brittle for template usage
			// We could probably move everything to the header and avoid explicit instantiation entirely....
			// template class EuclideanVector3D<float>;
			// template EuclideanVector3D<float>::EuclideanVector3D(float x, float y, float z);

			// template class EuclideanVector3D<double>;
			// template EuclideanVector3D<double>::EuclideanVector3D(double x, double y, double z);

			// template EuclideanVector3D<float> crossProduct<float>(const EuclideanVector<float,3>& vec1, const EuclideanVector<float,3>& vec2);
			// template EuclideanVector3D<double> crossProduct<double>(const EuclideanVector<double,3>& vec1, const EuclideanVector<double,3>& vec2);
		}
	}
}

// template class cupcfd::geometry::euclidean::EuclideanVector3D<float>;
// template cupcfd::geometry::euclidean::EuclideanVector3D<float>::EuclideanVector3D(float x, float y, float z);

// template class cupcfd::geometry::euclidean::EuclideanVector3D<double>;
// template cupcfd::geometry::euclidean::EuclideanVector3D<double>::EuclideanVector3D(double x, double y, double z);

// template cupcfd::geometry::euclidean::EuclideanVector3D<float> crossProduct<float>(const EuclideanVector<float,3>& vec1, const EuclideanVector<float,3>& vec2);
// template cupcfd::geometry::euclidean::EuclideanVector3D<double> crossProduct<double>(const EuclideanVector<double,3>& vec1, const EuclideanVector<double,3>& vec2);
