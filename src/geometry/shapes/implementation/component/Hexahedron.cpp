/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Definitions for the Hexahedron class
 *
 */

#include "Hexahedron.h"
#include "EuclideanVector3D.h"

namespace euc = cupcfd::geometry::euclidean;

namespace cupcfd
{
	namespace geometry
	{
		namespace shapes
		{
			template <class T>
			void Hexahedron<T>::detectOpposingFaces() {
				// Find two opposing faces, for Tetrahedron decomposition:
				uint top_idx = 0;
				this->top_face_idx = top_idx;
				bool adjacent_face_found = false;
				uint adjacent_face_idx;
				for (uint i=1; i<6; i++) {
					uint num_shared_vertices = 0;
					bool vertex0_shared = false;
					for (uint v1=0; v1<4; v1++) {
						for (uint v2=0; v2<4; v2++) {
							if (faces[top_idx].vertices[v1] == faces[i].vertices[v2]) {
								// Found match for vertex 'v1' in non-top face
								num_shared_vertices++;
								if (v1 == 0) {
									vertex0_shared = true;
								}
								break;
							}
						}
					}
					if (num_shared_vertices>2) {
						fprintf(stderr, "Hexahedron being constructured with two faces that share %d vertices (should be < 3)\n", num_shared_vertices);
						HARD_CHECK_ECODE(cupcfd::error::E_GEOMETRY_POLYHEDRON_BAD_CONSTRUCTION)
					}
					if (num_shared_vertices==2 && vertex0_shared) {
						adjacent_face_found = true;
						adjacent_face_idx = i;
						break;
					}
				}
				if (!adjacent_face_found) {
					HARD_CHECK_ECODE(cupcfd::error::E_GEOMETRY_POLYHEDRON_BAD_CONSTRUCTION)
				}
				bool bottom_found = false;
				uint bottom_idx;
				for (uint i=1; i<6; i++) {
					uint num_top_shared_vertices = 0;
					uint num_adjacent_face_shared_vertices = 0;
					for (uint v1=0; v1<4; v1++) {
						for (uint v2=0; v2<4; v2++) {
							if (faces[top_idx].vertices[v1] == faces[i].vertices[v2]) {
								num_top_shared_vertices++;
							}
							if (faces[adjacent_face_idx].vertices[v1] == faces[i].vertices[v2]) {
								num_adjacent_face_shared_vertices++;
							}
						}
					}
					if (num_top_shared_vertices>2) {
						fprintf(stderr, "Hexahedron being constructured with two faces that share %d vertices (should be < 3)\n", num_top_shared_vertices);
						HARD_CHECK_ECODE(cupcfd::error::E_GEOMETRY_POLYHEDRON_BAD_CONSTRUCTION)
					}
					if (num_adjacent_face_shared_vertices>2) {
						fprintf(stderr, "Hexahedron being constructured with two faces that share %d vertices (should be < 3)\n", num_adjacent_face_shared_vertices);
						HARD_CHECK_ECODE(cupcfd::error::E_GEOMETRY_POLYHEDRON_BAD_CONSTRUCTION)
					}
					if (num_top_shared_vertices==0 && num_adjacent_face_shared_vertices==2) {
						bottom_found = true;
						bottom_idx = i;
						break;
					}
				}
				if (!bottom_found) {
					HARD_CHECK_ECODE(cupcfd::error::E_GEOMETRY_POLYHEDRON_BAD_CONSTRUCTION)
				}
				this->bottom_face_idx = bottom_idx;
				
				// Now know top/bottom orientation, and all face normals point out, 
				// shift bottom face vertices so first vertex is connected to 
				// first vertex of top face:
				// a. Find side1 vertex that matches top vertex 0
				// b. Find connected vertex that matches with one in bottom face
				// c. Shift bottom face vertices so that connected vertex is at index 0
				bool aligned_bottom_face = false;
				bool found_adjacent_vertex = false;
				for (int s=0; s<4; s++) {
					if (this->faces[adjacent_face_idx].vertices[s] == this->faces[top_idx].vertices[0]) {
						found_adjacent_vertex = true;

						// Not certain whether to look-ahead, or look-behind, by one, 
						// to get from top to bottom so look both ways:
						const int s2a=(s+1)%4;
						const int s2b=(s-1+4)%4;
						euc::EuclideanPoint<T,3> v2a = this->faces[adjacent_face_idx].vertices[s2a];
						euc::EuclideanPoint<T,3> v2b = this->faces[adjacent_face_idx].vertices[s2b];
						bool found_bottom_vertex = false;
						uint bottom_shift = 0;
						for (int b=0; b<4; b++) {
							if ( (this->faces[bottom_idx].vertices[b] == v2a) || 
								(this->faces[bottom_idx].vertices[b] == v2b) ) {
								found_bottom_vertex = true;
								bottom_shift = -b;
								break;
							}
						}
						if (!found_bottom_vertex) {
							HARD_CHECK_ECODE(cupcfd::error::E_GEOMETRY_POLYHEDRON_BAD_CONSTRUCTION)
						}
						if (bottom_shift != 0) {
							this->faces[bottom_idx].shiftVertices(bottom_shift);
						}
						aligned_bottom_face = true;
						break;
					}
				}
				if (!found_adjacent_vertex) {
					printf("Failed to find adjacent vertex that matches top vertex 0\n");
					HARD_CHECK_ECODE(cupcfd::error::E_GEOMETRY_POLYHEDRON_BAD_CONSTRUCTION)
				}
				if (!aligned_bottom_face) {
					printf("Failed to align top and bottom faces of Hexahedron.\n");
					printf("Top face:\n");
					this->faces[top_idx].print();
					printf("Adjacent face:\n");
					this->faces[adjacent_face_idx].print();
					printf("Bottom face:\n");
					this->faces[bottom_idx].print();
					HARD_CHECK_ECODE(cupcfd::error::E_GEOMETRY_POLYHEDRON_BAD_CONSTRUCTION)
				}
			}

			template <class T>
			void Hexahedron<T>::orientFaceNormals() {
				// Ensure all face normals point away from centroid:
				euc::EuclideanPoint<T,3> orientingCentroid(this->faces[0].getCentroid());
				for (int f=1; f<6; f++) {
					orientingCentroid = orientingCentroid + this->faces[f].getCentroid();
				}
				orientingCentroid = orientingCentroid / T(6);
				T dp;
				for (int f=0; f<6; f++) {
					euc::EuclideanVector3D<T> v = this->faces[f].getCentroid() - orientingCentroid;
					dp = this->faces[f].getNormal().dotProduct(v);
					if (dp < T(0.0)) {
						// printf("Reversing face %d/6 (dotP = %.2e)\n", f+1, dp);
						this->faces[f].reverseVertexOrdering();
					}
				}
			}

			template<class T>
			bool Hexahedron<T>::isPointInside(const euc::EuclideanPoint<T,3>& point) {
				// ToDo: This algorithm could be moved up to a more general level in Polyhedron.
				// However, we would need to either (a) find a way to export the vertices (overhead of extra copies)
				// or (b) store a general vertex member list (such as an array) at the Polyhedron level - however
				//since the size would be variable, this would be an array pointer, not a fixed size, and so every
				//Polyhedron object would just point to a different fragmented arbitrary location in memory - would be bad
				//for creating contiguous arrays of Polyhedrons (not that I would recommend AoS as an approach).
				
				// Algorithm: Construct a vector from the point to each face
				// Assuming the normal vector direction is pointing out, if the dot-product of the face normal 
				// vector and the point->face vector is positive, they are both in the same direction, and
				// thus the point is 'inside' for that face.
				// Repeat for all faces, and if 'inside' all of them, the point is inside the polygon
				
				double dotProd;
				euc::EuclideanVector<T,3> pointVector;
						
				for(int i = 0; i < 6; i++) {
					// ToDo: This is presuming that all type 'P' base objects have a vertices data structure
					// Could make it so Polygon3D has a pointer to a vertices array, and then leave the 
					// storage implementation to the subclasses and updating the pointer to it.
					
					// Create a vector from point to face:
					pointVector = this->faces[i].getCentroid() - point;
					
					// Check if on face, count as inside. Not that this is expected to be remotely likely,
					// but need to catch this edge case before dot product.
					if(this->faces[i].isPointInside(point)) {
						return true;
					}
					
					// Calculate dot product - negative means point is outside of this face
					dotProd = pointVector.dotProduct(this->faces[i].getNormal());
					if(dotProd < T(0)) {
						return false;
					}
				}
				
				return true;
			}
			
			template <class T>
			bool Hexahedron<T>::isPointOnEdge(const euc::EuclideanPoint<T,3>& point) {
				for (int i1=0; i1<4; i1++) {
					int i2=(i1+1)%4;
					
					// Top face edge:
					if (isPointOnLine(this->faces[this->top_face_idx].vertices[i1], 
										this->faces[this->top_face_idx].vertices[i2],
										point)) {
						return true;
					}

					// Bottom face edge:
					if (isPointOnLine(this->faces[this->bottom_face_idx].vertices[i1], 
										this->faces[this->bottom_face_idx].vertices[i2],
										point)) {
						return true;
					}

					// Edge between top and bottom faces:
					int ib = (4-i1)%4;
					if (isPointOnLine(this->faces[this->top_face_idx].vertices[i1], 
										this->faces[this->bottom_face_idx].vertices[ib],
										point)) {
						return true;
					}
				}
				
				return false;
			}

			template class cupcfd::geometry::shapes::Hexahedron<double>;
			template class cupcfd::geometry::shapes::Hexahedron<float>;
		}
	}
}

