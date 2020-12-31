/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Header-Level Definitions of the MeshSourceStructGenConfig class
 */

#ifndef CUPCFD_GEOMETRY_MESH_MESH_SOURCE_STRUCT_GEN_CONFIG_IPP_H
#define CUPCFD_GEOMETRY_MESH_MESH_SOURCE_STRUCT_GEN_CONFIG_IPP_H

#include "MeshStructGenSource.h"

namespace cupcfd
{
	namespace geometry
	{
		namespace mesh
		{
			template <class I, class T>
			inline I MeshSourceStructGenConfig<I,T>::getCellX()
			{
				return this->cellX;
			}

			template <class I, class T>
			inline void MeshSourceStructGenConfig<I,T>::setCellX(I cellX)
			{
				this->cellX = cellX;
			}

			template <class I, class T>
			inline I MeshSourceStructGenConfig<I,T>::getCellY()
			{
				return this->cellY;
			}

			template <class I, class T>
			inline void MeshSourceStructGenConfig<I,T>::setCellY(I cellY)
			{
				this->cellY = cellY;
			}

			template <class I, class T>
			inline I MeshSourceStructGenConfig<I,T>::getCellZ()
			{
				return this->cellZ;
			}

			template <class I, class T>
			inline void MeshSourceStructGenConfig<I,T>::setCellZ(I cellZ)
			{
				this->cellZ = cellZ;
			}

			template <class I, class T>
			inline T MeshSourceStructGenConfig<I,T>::getSpatialMinX()
			{
				return this->sMinX;
			}

			template <class I, class T>
			inline void MeshSourceStructGenConfig<I,T>::setSpatialMinX(T sMinX)
			{
				this->sMinX = sMinX;
			}

			template <class I, class T>
			inline T MeshSourceStructGenConfig<I,T>::getSpatialMinY()
			{
				return this->sMinY;
			}

			template <class I, class T>
			inline void MeshSourceStructGenConfig<I,T>::setSpatialMinY(T sMinY)
			{
				this->sMinY = sMinY;
			}

			template <class I, class T>
			inline T MeshSourceStructGenConfig<I,T>::getSpatialMinZ()
			{
				return this->sMinZ;
			}

			template <class I, class T>
			inline void MeshSourceStructGenConfig<I,T>::setSpatialMinZ(T sMinZ)
			{
				this->sMinZ = sMinZ;
			}

			template <class I, class T>
			inline T MeshSourceStructGenConfig<I,T>::getSpatialMaxX()
			{
				return this->sMaxX;
			}

			template <class I, class T>
			inline void MeshSourceStructGenConfig<I,T>::setSpatialMaxX(T sMaxX)
			{
				this->sMaxX = sMaxX;
			}

			template <class I, class T>
			inline T MeshSourceStructGenConfig<I,T>::getSpatialMaxY()
			{
				return this->sMaxY;
			}

			template <class I, class T>
			inline void MeshSourceStructGenConfig<I,T>::setSpatialMaxY(T sMaxY)
			{
				this->sMaxY = sMaxY;
			}

			template <class I, class T>
			inline T MeshSourceStructGenConfig<I,T>::getSpatialMaxZ()
			{
				return this->sMaxZ;
			}

			template <class I, class T>
			inline void MeshSourceStructGenConfig<I,T>::setSpatialMaxZ(T sMaxZ)
			{
				this->sMaxZ = sMaxZ;
			}

			// === Overridden Inherited Methods ===
			
			template <class I, class T>
			inline void MeshSourceStructGenConfig<I,T>::operator=(const MeshSourceStructGenConfig<I,T>& source)
			{
				// Parent copy
				MeshSourceConfig<I,T,I>::operator=(source);

				// Derived Members Copy
				this->cellX = source.cellX;
				this->cellY = source.cellY;
				this->cellZ = source.cellZ;
				this->sMinX = source.sMinX;
				this->sMinY = source.sMinY;
				this->sMinZ = source.sMinZ;
				this->sMaxX = source.sMaxX;
				this->sMaxY = source.sMaxY;
				this->sMaxZ = source.sMaxZ;
			}
			
			template <class I, class T>
			cupcfd::error::eCodes MeshSourceStructGenConfig<I,T>::buildMeshSource(MeshSource<I,T,I> ** source)
			{
				I nX, nY, nZ;
				T sMinX, sMaxX, sMinY, sMaxY, sMinZ, sMaxZ;

				// === Read Options From Config ===
				nX = this->getCellX();
				nY = this->getCellY();
				nZ = this->getCellZ();

				sMinX = this->getSpatialMinX();
				sMaxX = this->getSpatialMaxX();
				sMinY = this->getSpatialMinY();
				sMaxY = this->getSpatialMaxY();
				sMinZ = this->getSpatialMinZ();
				sMaxZ = this->getSpatialMaxZ();

				*source = new cupcfd::geometry::mesh::MeshStructGenSource<I,T>(nX, nY, nZ, sMinX, sMaxX, sMinY, sMaxY, sMinZ, sMaxZ);

				return cupcfd::error::E_SUCCESS;
			}
			
		} // namespace mesh
	} // namespace geometry
} // namespace cupcfd

#endif
