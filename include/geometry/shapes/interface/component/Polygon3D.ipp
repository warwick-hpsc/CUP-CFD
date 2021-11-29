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
 */

#ifndef CUPCFD_GEOMETRY_SHAPES_POLYGON3D_3D_IPP_H
#define CUPCFD_GEOMETRY_SHAPES_POLYGON3D_3D_IPP_H

#include "EuclideanPlane3D.h"
#include "EuclideanVector2D.h"
#include "EuclideanVector3D.h"
#include "Matrix.h"
#include "Triangle3D.h"
#include "Polygon2D.h"

namespace euc = cupcfd::geometry::euclidean;

namespace cupcfd
{
	namespace geometry
	{
		namespace shapes
		{
			template <class T, uint V>
			template<class...Args>
			Polygon3D<T,V>::Polygon3D(Args...v)
			: Polygon<Polygon3D<T,V>,T,3,V>::Polygon( (v)... )
			{
				static_assert(sizeof...(Args) == V, "Polygon constructor dimension does not match number of parameters");

				if (this->numVertices > 3) {
					if (!this->coplanar()) {
						printf("Vertices not coplanar:\n");
						this->print();
						HARD_CHECK_ECODE(cupcfd::error::E_GEOMETRY_LOGIC_ERROR);
					}

					if (!this->verifyNoEdgesIntersect()) {
						HARD_CHECK_ECODE(cupcfd::error::E_GEOMETRY_LOGIC_ERROR);
					}
				}
			}
			
			template <class T, uint V>
			T Polygon3D<T,V>::getArea() {
				if (!this->areaComputed) {
					this->area = this->computeArea();
					if (this->area == T(0)) {
						fprintf(stderr, "Polygon3D area is zero\n");
						HARD_CHECK_ECODE(cupcfd::error::E_GEOMETRY_ZERO_AREA);
					}
					this->areaComputed = true;
				}
				return this->area;
			}

			template <class T, uint V>
			euc::EuclideanPoint<T,3> Polygon3D<T,V>::getCentroid() {
				if (!this->centroidComputed) {
					this->centroid = this->computeCentroid();
					this->centroidComputed = true;
				}
				return this->centroid;
			}

			template <class T, uint V>
			euc::EuclideanVector3D<T> Polygon3D<T,V>::getNormal() {
				if (!this->normalComputed) {
					this->normal = this->computeNormal();
					this->normalComputed = true;
				}
				return this->normal;
			}

			template <class T, uint V>
			bool Polygon3D<T,V>::coplanar() {
				// Calculate plane normal from first three vertices:
				euc::EuclideanVector3D<T> normal = this->computeNormal();
				// Check if any other vertex is NOT coplanar:
				T tolerance = T(1e-10)*(this->vertices[1]-this->vertices[0]).length();
				for (uint i=3; i<V; i++){
					euc::EuclideanVector3D<T> v = this->vertices[i] - this->vertices[0];
					T dp = v.dotProduct(normal);
					if (dp > tolerance) {
						return false;
					}
				}
				return true;
			}

			template <class T, uint V>
			void Polygon3D<T,V>::alignNormalWithVector(euc::EuclideanVector3D<T>& v) {
				// Record original vertices:
				euc::EuclideanPoint<T,3> vertices[V];
				for (uint i=0; i<V; i++) {
					vertices[i] = this->vertices[i];
				}

				// Calculate rotation matrix:
				euc::EuclideanVector3D<T> normal = this->getNormal();
				normal.normalise();
				euc::Matrix<T,3,3> rotMatrix2;
				rotMatrix2 = euc::calculateRotationMatrix(normal, v);

				// Apply rotation:
				for (uint i=0; i<V; i++) {
					this->vertices[i] = rotMatrix2 * this->vertices[i];
				}
				this->normalComputed = false;

				euc::EuclideanVector3D<T> normalRotated = rotMatrix2 * normal;
				normalRotated.normalise();

				// Verify that rotation aligned polygon normal with Z-axis:
				euc::EuclideanVector3D<T> normalActual = this->getNormal();
				normalActual.normalise();
				T cosine = v.dotProduct(normalActual);
				if (cosine < T(0.9999)) {
					// 'cosine' should be 1.0 exactly, but allow a tiny tolerance for floating-point

					printf("Polygon rotation failed to align its normal with specified vector\n");

					printf("Polygon vertices before:\n");
					for (uint i=0; i<V; i++) {
						printf("V %d: [ %.3f %.3f %.3f ]\n", i, vertices[i].cmp[0], vertices[i].cmp[1], vertices[i].cmp[2]);
					}
					printf("Normal before: ");
					normal.print(); std::cout << std::endl;
					
					printf("Polygon vertices after:\n");
					for (uint i=0; i<V; i++) {
						printf("V %d: [ %.3f %.3f %.3f ]\n", i, this->vertices[i].cmp[0], this->vertices[i].cmp[1], this->vertices[i].cmp[2]);
					}
					printf("Normal after: ");
					normalActual.print(); std::cout << std::endl;
					// printf("Normal rotated (should match above): ");
					// normalRotated.print(); std::cout << std::endl;
					printf("Cosine between normal and Z-axis = %.4f (%.1f degrees)\n", cosine, acos(cosine)/3.14157*180.0);

					printf("Target vector: ");
					v.print(); std::cout << std::endl;

					printf("Rotation matrix: \n");
					rotMatrix2.print(); std::cout << std::endl;

					printf("\n");

					HARD_CHECK_ECODE(cupcfd::error::E_ERROR);
				}
			}
			
			// Probably an actual geometric term for 'polygon edges do not intersect':
			// template <class S, class T, uint N, uint V>
			template <class T, uint V>
			bool Polygon3D<T,V>::verifyNoEdgesIntersect() {
				// floating-point rounding errors could prevent detection of any true
				// 3D intersection point. Best bet is to rotate the polygon so
				// normal becomes Z-axis (can do this because coplanar). 
				// Then only working in 2D so will definitely find 
				// intersection point.
				euc::EuclideanVector3D<T> zAxis(T(0), T(0), T(1));
				Polygon3D<T,V> copy(*this);
				copy.alignNormalWithVector(zAxis);
				euc::EuclideanPoint<T,2> vertices2D[V];
				for (uint i=0; i<V; i++) {
					vertices2D[i].cmp[0] = copy.vertices[i].cmp[0];
					vertices2D[i].cmp[1] = copy.vertices[i].cmp[1];
				}

				Polygon2D<T,V> thisAs2D(vertices2D);
				return thisAs2D.verifyNoEdgesIntersect();
			}
		}
	}
}

#endif
