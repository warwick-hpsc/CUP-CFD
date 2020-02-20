/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Implementation of the MeshSourceStructGenConfig class
 */

#include "MeshSourceStructGenConfig.h"

namespace cupcfd
{
	namespace geometry
	{
		namespace mesh
		{
			template <class I, class T>
			MeshSourceStructGenConfig<I,T>::MeshSourceStructGenConfig(I cellX, I cellY, I cellZ,
														  	  	  	  T sMinX, T sMaxX, T sMinY, T sMaxY, T sMinZ, T sMaxZ)
			: MeshSourceConfig<I,T,I>(),
			  cellX(cellX), cellY(cellY), cellZ(cellZ),
			  sMinX(sMinX), sMaxX(sMaxX),
			  sMinY(sMinY), sMaxY(sMaxY),
			  sMinZ(sMinZ), sMaxZ(sMaxZ)
			{
				// Nothing else to do beyond member instantiation list
			}

			template <class I, class T>
			MeshSourceStructGenConfig<I,T>::MeshSourceStructGenConfig(MeshSourceStructGenConfig<I,T>& source)
			: MeshSourceConfig<I,T,I>()
			{
				// Will end up setting partConfig twice but shouldn't be too significant.
				*this = source;
			}

			template <class I, class T>
			MeshSourceStructGenConfig<I,T>::~MeshSourceStructGenConfig()
			{
				// Nothing to do in derived
			}


			template <class I, class T>
			MeshSourceStructGenConfig<I,T> * MeshSourceStructGenConfig<I,T>::clone()
			{
				return new MeshSourceStructGenConfig<I,T>(*this);
			}
		}
	}
}

// Explicit Instantiation
template class cupcfd::geometry::mesh::MeshSourceStructGenConfig<int, float>;
template class cupcfd::geometry::mesh::MeshSourceStructGenConfig<int, double>;
