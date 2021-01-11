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

#ifndef CUPCFD_GEOMETRY_SHAPES_POLYGON_2D_IPP_H
#define CUPCFD_GEOMETRY_SHAPES_POLYGON_2D_IPP_H

namespace cupcfd
{
	namespace geometry
	{
		namespace shapes
		{
			template <class T, uint V>
			Polygon2D<T,V>::Polygon2D() {
			}

			template <class T, uint V>
			Polygon2D<T,V>::Polygon2D(euc::EuclideanPoint<T,2> vertices[V]) {
				for (uint i=0; i<V; i++) {
					this->vertices[i] = vertices[i];
				}
			}
			
			template <class T, uint V>
			Polygon2D<T,V>::~Polygon2D() {
			}
			
			// Probably an actual geometric term for 'polygon edges do not intersect':
			// template <class S, class T, uint N, uint V>
			template <class T, uint V>
			bool Polygon2D<T,V>::verifyNoEdgesIntersect() {
				// Scan for any crossing edges. 
				// Permute through all possible pairs of edges, except directly adjacent.
				// Maybe there is a better way to scan, but 
				// this check is only performed once so does it matter?
				for (uint i1a=0; i1a<V; i1a++) {
					const uint i1b = (i1a+1)%V;
					euc::EuclideanPoint<T,2> e1a(this->vertices[i1a].cmp[0], this->vertices[i1a].cmp[1]);
					euc::EuclideanPoint<T,2> e1b(this->vertices[i1b].cmp[0], this->vertices[i1b].cmp[1]);

					for (uint i2a=i1b+1; i2a<V; i2a++) {
						const uint i2b = (i2a+1)%V;
						if (i2b == i1a) {
							// Edge 2 has iterated too far, skip
							continue;
						}
						euc::EuclideanPoint<T,2> e2a(this->vertices[i2a].cmp[0], this->vertices[i2a].cmp[1]);
						euc::EuclideanPoint<T,2> e2b(this->vertices[i2b].cmp[0], this->vertices[i2b].cmp[1]);

						euc::EuclideanPoint<T,2> intersectPoint(T(0.0), T(0.0));
						cupcfd::error::eCodes status = euc::computeVectorRangeIntersection(e1a, e1b, e2a, e2b, intersectPoint);
						if (status == cupcfd::error::E_SUCCESS) {
							printf("Intersection detected between polygon edges!\n");

							printf("Polygon vertices:\n");
							for (uint i=0; i<V; i++) {
								printf("V %d: [ %.2f %.2f ]\n", i, this->vertices[i].cmp[0], this->vertices[i].cmp[1]);
							}

							printf("Intersecting edges:\n");
							e1a.print(); printf(" --> "); e1b.print(); printf("\n");
							e2a.print(); printf(" --> "); e2b.print(); printf("\n");

							printf("Intersection point:\n");
							intersectPoint.print(); std::cout << std::endl;

							return false;
						}
					}
				}

				return true;
			}

			template <class T, uint V>
			T Polygon2D<T,V>::getArea() {
				if (!this->areaComputed) {
					this->area = this->computeArea();
					if (this->area == T(0)) {
						fprintf(stderr, "Polygon2D area is zero\n");
						HARD_CHECK_ECODE(cupcfd::error::E_GEOMETRY_ZERO_AREA);
					}
					this->areaComputed = true;
				}
				return this->area;
			}

			template <class T, uint V>
			euc::EuclideanPoint<T,2> Polygon2D<T,V>::computeCentroid() {
				// https://en.wikipedia.org/wiki/Centroid#Of_a_polygon

				euc::EuclideanPoint<T,2> centroid(T(0), T(0));
				for (uint i=0; i<V; i++) {
					uint i2=(i+1)%V;
					euc::EuclideanPoint<T,2> v1 = this->vertices[i];
					euc::EuclideanPoint<T,2> v2 = this->vertices[i2];
					centroid.cmp[0] += (v1.cmp[0] + v2.cmp[0]) * ( v1.cmp[0]*v2.cmp[1] - v2.cmp[0] * v1.cmp[1] );
					centroid.cmp[1] += (v1.cmp[1] + v2.cmp[1]) * ( v1.cmp[0]*v2.cmp[1] - v2.cmp[0] * v1.cmp[1] );
				}
				centroid *= T(1) / (T(6) * this->getArea());

				return centroid;
			}
			template <class T, uint V>
			euc::EuclideanPoint<T,2> Polygon2D<T,V>::getCentroid() {
				if (!this->centroidComputed) {
					this->centroid = this->computeCentroid();
					this->centroidComputed = true;
				}
				return this->centroid;
			}
		}
	}
}

#endif
