/*
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 */

#include "UnstructuredMeshProperties.h"

namespace cupcfd
{
	namespace geometry
	{
		namespace mesh
		{
			template <class I, class T>
			UnstructuredMeshProperties<I, T>::UnstructuredMeshProperties()
			{
				// Pass through to the reset function
				this->reset();
			}

			template <class I, class T>
			UnstructuredMeshProperties<I, T>::UnstructuredMeshProperties(
						   I nCells, I nFaces, I nVertices, I nBoundaries,
						   I nRegions, I nMaxFaces, T scaleFactor,
						   I lOCells, I lGhCells, I lTCells, I lFaces,
						   I lVertices, I lBoundaries, I lRegions)
			{
				this->nCells = nCells;
				this->nFaces = nFaces;
				this->nVertices = nVertices;
				this->nBoundaries = nBoundaries;
				this->nRegions = nRegions;
				this->nMaxFaces = nMaxFaces;
				this->scaleFactor = scaleFactor;

				this->lOCells = lOCells;
				this->lGhCells = lGhCells;
				this->lTCells = lTCells;
				this->lFaces = lFaces;
				this->lVertices = lVertices;
				this->lBoundaries = lBoundaries;
				this->lRegions = lRegions;
			}

			template <class I, class T>
			UnstructuredMeshProperties<I, T>::UnstructuredMeshProperties(UnstructuredMeshProperties<I,T>& source)
			{
				// Pass-through to copy operator
				*this = source;
			}

			template <class I, class T>
			UnstructuredMeshProperties<I, T>::~UnstructuredMeshProperties()
			{
				// Currently does nothing.
			}

			template <class I, class T>
			cupcfd::error::eCodes UnstructuredMeshProperties<I, T>::reset()
			{
				this->nCells = (I) 0;
				this->nFaces = (I) 0;
				this->nVertices = (I) 0;
				this->nBoundaries = (I) 0;
				this->nRegions = (I) 0;
				this->nMaxFaces = (I) 0;
				this->scaleFactor = (T) 0;

				this->lOCells = (I) 0;
				this->lGhCells = (I) 0;
				this->lTCells = (I) 0;
				this->lFaces = (I) 0;
				this->lVertices = (I) 0;
				this->lBoundaries = (I) 0;
				this->lRegions = (I) 0;

				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T>
			void UnstructuredMeshProperties<I,T>::operator=(UnstructuredMeshProperties<I,T>& source)
			{
				// No need for reset, since everything is overwritten.

				this->nCells = source.nCells;
				this->nFaces = source.nFaces;
				this->nVertices = source.nVertices;
				this->nBoundaries = source.nBoundaries;
				this->nRegions = source.nRegions;
				this->nMaxFaces = source.nMaxFaces;
				this->scaleFactor = source.scaleFactor;

				this->lOCells = source.lOCells;
				this->lGhCells = source.lGhCells;
				this->lTCells = source.lTCells;
				this->lFaces = source.lFaces;
				this->lVertices = source.lVertices;
				this->lBoundaries = source.lBoundaries;
				this->lRegions = source.lRegions;
			}

			template <class I, class T>
			UnstructuredMeshProperties<I,T> * UnstructuredMeshProperties<I,T>::clone()
			{
				return new UnstructuredMeshProperties<I,T>(*this);
			}
		}
	}
}

// Explicit Instantiation
template class cupcfd::geometry::mesh::UnstructuredMeshProperties<int, float>;
template class cupcfd::geometry::mesh::UnstructuredMeshProperties<int, double>;

template class cupcfd::geometry::mesh::UnstructuredMeshProperties<long, float>;
template class cupcfd::geometry::mesh::UnstructuredMeshProperties<long, double>;
