/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Header Level Definitions for the Hexahedron class
 *
 */

#ifndef CUPCFD_GEOMETRY_SHAPES_HEXAHEDRON_IPP_H
#define CUPCFD_GEOMETRY_SHAPES_HEXAHEDRON_IPP_H

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
			Hexahedron<T>::Hexahedron(const shapes::Quadrilateral3D<T>& f0,
									const shapes::Quadrilateral3D<T>& f1,
									const shapes::Quadrilateral3D<T>& f2,
									const shapes::Quadrilateral3D<T>& f3,
									const shapes::Quadrilateral3D<T>& f4,
									const shapes::Quadrilateral3D<T>& f5)
			{
				faces[0] = f0;
				faces[1] = f1;
				faces[2] = f2;
				faces[3] = f3;
				faces[4] = f4;
				faces[5] = f5;

				if (!this->verifyFacesDistinct()) {
					printf("Hexahedron constructed from two identifical faces:\n");
					HARD_CHECK_ECODE(cupcfd::error::E_GEOMETRY_POLYHEDRON_BAD_CONSTRUCTION)
				}

				this->orientFaceNormals();
				this->detectOpposingFaces();

				// Decompose Hexahedron into 5x Tetrahedrons:
				// https://ars.els-cdn.com/content/image/3-s2.0-B9780080983561000096-f09-11-9780080983561.jpg?_
				shapes::Quadrilateral3D<T> t = this->faces[this->top_face_idx];
				shapes::Quadrilateral3D<T> b = this->faces[this->bottom_face_idx];
				const euc::EuclideanPoint<T,3> h6 = t.vertices[0];
				const euc::EuclideanPoint<T,3> h7 = t.vertices[1];
				const euc::EuclideanPoint<T,3> h8 = t.vertices[2];
				const euc::EuclideanPoint<T,3> h5 = t.vertices[3];
				const euc::EuclideanPoint<T,3> h2 = b.vertices[0];
				const euc::EuclideanPoint<T,3> h1 = b.vertices[1];
				const euc::EuclideanPoint<T,3> h4 = b.vertices[2];
				const euc::EuclideanPoint<T,3> h3 = b.vertices[3];
				this->tetra[0] = shapes::Tetrahedron<T>(h1, Triangle3D<T>(h5, h6, h8));
				this->tetra[1] = shapes::Tetrahedron<T>(h1, Triangle3D<T>(h3, h8, h4));
				this->tetra[2] = shapes::Tetrahedron<T>(h3, Triangle3D<T>(h6, h8, h7));
				this->tetra[3] = shapes::Tetrahedron<T>(h1, Triangle3D<T>(h2, h3, h6));
				this->tetra[4] = shapes::Tetrahedron<T>(h1, Triangle3D<T>(h3, h8, h6));
			}

			template <class T>
			Hexahedron<T>::~Hexahedron() {
			}

			// === Concrete Methods ===

			template <class T>
			bool Hexahedron<T>::verifyFacesDistinct() {
				// Verify that no faces have same vertices:
				T dp;
				for (int i1=0; i1<6; i1++) {
					shapes::Quadrilateral3D<T> f1Copy(this->faces[i1]);
					for (int i2=i1+1; i2<6; i2++) {
						// Align normals to make vertex traversal easier:
						dp = f1Copy.getNormal().dotProduct(this->faces[i2].getNormal());
						if (dp < T(0)) {
							f1Copy.reverseVertexOrdering();
						}

						// If faces share any vertices, calculate array offset:
						int vShift=-1;
						for (int i=0; i<4; i++) {
							if (f1Copy.vertices[0] == this->faces[i2].vertices[i]) {
								vShift=1;
								break;
							}
						}
						if (vShift != -1) {
							// One vertex is shared, now check the remainder:
							bool all_vertices_match = true;
							for (int v1=0; v1<4; v1++) {
								int v2=(v1+vShift)%4;
								if (f1Copy.vertices[v1] != this->faces[i2].vertices[v2]) {
									all_vertices_match = false;
									break;
								}
							}
							if (all_vertices_match) {
								return false;
							}
						}
					}
				}
				return true;
			}

			template <class T>
			T Hexahedron<T>::getVolume() {
				if (!this->volumeComputed) {
					this->volume = this->computeVolume();
					this->volumeComputed = true;
				}
				return this->volume;
			}

			template <class T>
			euc::EuclideanPoint<T,3> Hexahedron<T>::getCentroid() {
				if (!this->centroidComputed) {
					this->centroid = this->computeCentroid();
					this->centroidComputed = true;
				}
				return this->centroid;
			}

			template<class T>
			T Hexahedron<T>::computeVolume() {
				// Divide the hexahedron into 5 tetrahedrons, and sum their volumes.
				T v = this->tetra[0].getVolume() + this->tetra[1].getVolume() + this->tetra[2].getVolume() + this->tetra[3].getVolume() + this->tetra[4].getVolume();
				return v;
			}
			
			template<class T>
			euc::EuclideanPoint<T,3> Hexahedron<T>::computeCentroid() {
				return (	  (this->tetra[0].getVolume() * this->tetra[0].getCentroid())
							+ (this->tetra[1].getVolume() * this->tetra[1].getCentroid()) 
							+ (this->tetra[2].getVolume() * this->tetra[2].getCentroid())
							+ (this->tetra[3].getVolume() * this->tetra[3].getCentroid())
							+ (this->tetra[4].getVolume() * this->tetra[4].getCentroid()) 	) 
						/ this->getVolume();
 			}
			
			template <class T>
			inline bool Hexahedron<T>::isPointOnVertex(const euc::EuclideanPoint<T,3>& point) {
				for (int i=0; i<4; i++) {
					if (point == this->faces[this->top_face_idx].vertices[i] || 
						point == this->faces[this->bottom_face_idx].vertices[i]) {
						return true;
					}
				}
				return false;
			}
		}
	}
}

#endif
