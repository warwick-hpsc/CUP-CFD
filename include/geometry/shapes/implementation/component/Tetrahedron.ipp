/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Header Level Definitions for the Tetrahedron class
 */

#ifndef CUPCFD_GEOMETRY_SHAPES_TETRAHEDRON_IPP_H
#define CUPCFD_GEOMETRY_SHAPES_TETRAHEDRON_IPP_H

namespace euc = cupcfd::geometry::euclidean;
namespace shapes = cupcfd::geometry::shapes;

namespace cupcfd
{
	namespace geometry
	{
		namespace shapes
		{
			// === Constructors/Deconstructors ===

			template <class T>
			Tetrahedron<T>::Tetrahedron()
			: Pyramid<Triangle3D<T>,T>(
					euc::EuclideanPoint<T,3>(T(0), T(0), T(1)),
					shapes::Triangle3D<T>(
						euc::EuclideanPoint<T,3>(T(0), T(0), T(0)),
						euc::EuclideanPoint<T,3>(T(1), T(0), T(0)),
						euc::EuclideanPoint<T,3>(T(0), T(1), T(0))))
			{
			}

			template <class T>
			Tetrahedron<T>::Tetrahedron(const euc::EuclideanPoint<T,3>& apex,
										const shapes::Triangle3D<T>& base)
			: Pyramid<Triangle3D<T>,T>(apex, base)
			{
			}
			
			template <class T>
			Tetrahedron<T>::~Tetrahedron()
			{
			}
			
			template <class T>
			void Tetrahedron<T>::print()
			{
				printf("Tetrahedron:\n");
				for (uint i=0; i<3; i++) {
					printf("- v%d/4: ", i+1); this->base.vertices[i].print(); printf("\n");
				}
				printf("- v4/4: "); this->apex.print(); printf("\n");
				printf("- centroid: "); this->getCentroid().print(); printf("\n");
				printf("- volume: %.2e\n", this->getVolume());
			}
		}
	}
}

#endif
