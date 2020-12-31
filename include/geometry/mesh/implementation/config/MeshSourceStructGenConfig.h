/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Declaration of the MeshSourceStructGenConfig class
 */

#ifndef CUPCFD_GEOMETRY_MESH_MESH_SOURCE_STRUCT_GEN_CONFIG_INCLUDE_H
#define CUPCFD_GEOMETRY_MESH_MESH_SOURCE_STRUCT_GEN_CONFIG_INCLUDE_H

#include "MeshSourceConfig.h"

namespace cupcfd
{
	namespace geometry
	{
		namespace mesh
		{
			/**
			 * Stores Mesh Configuration for generating a mesh that has a 'structured'
			 * layout, i.e. a fixed size grid of cells.
			 *
			 * @tparam I The type of the indexing scheme/ cell labels
			 * @tparam T The type of the stored array data
			 */
			template <class I, class T>
			class MeshSourceStructGenConfig : public MeshSourceConfig<I,T,I>
			{
				public:
					// === Members ===

					/** Number of cells in the X dimension**/
					I cellX;

					/** Numbers of cells in the Y dimension **/
					I cellY;

					/** Number of cells in the Z dimension **/
					I cellZ;

					/** Minimum spatial position in the X dimension **/
					T sMinX;

					/** Maximum spatial position in the X dimension **/
					T sMaxX;

					/** Minimum spatial position in the Y dimension **/
					T sMinY;

					/** Maximum spatial position in the Y dimension **/
					T sMaxY;

					/** Minimum spatial position in the Z dimension **/
					T sMinZ;

					/** Maximum spatial position in the Z dimension **/
					T sMaxZ;


					// === Constructors/Deconstructors ===

					/**
					 * Constructor:
					 * Initialise parameters to those provided
					 *
					 * @param cellX The number of cells in the X dimension.
					 * @param cellY The number of cells in the Y dimension.
					 * @param celLZ The number of cells in the Z dimension.
					 * @param sMinX The minimum value in the spatial domain of the X dimension.
					 * @param sMaxX The maximum value in the spatial domain of the X dimension.
					 * @param sMinY The minimum value in the spatial domain of the Y dimension.
					 * @param sMaxY The maximum value in the spatial domain of the Y dimension.
					 * @param sMinZ The minimum value in the spatial domain of the Z dimension.
					 * @param sMaxZ The maximum value in the spatial domain of the Z dimension.
					 *
					 * @tparam I The type of the indexing scheme/ cell labels
					 * @tparam T The type of the stored array data
					 */
					MeshSourceStructGenConfig(I cellX, I cellY, I cellZ,
									    T sMinX, T sMaxX, T sMinY, T sMaxY, T sMinZ, T sMaxZ);

					/**
					 * Constructor:
					 * Copy the configuration stored in a source object to this object.
					 *
					 * @param source The object to copy the configuration from.
					 *
					 * @tparam I The type of the indexing scheme/ cell labels
					 * @tparam T The type of the stored array data
					 */
					MeshSourceStructGenConfig(MeshSourceStructGenConfig<I,T>& source);

					/**
					 * Deconstructor.
					 *
					 * @tparam I The type of the indexing scheme/ cell labels
					 * @tparam T The type of the stored array data
					 */
					~MeshSourceStructGenConfig();

					// === Concrete Methods ===

					/**
					 * Get the number of cells in the X dimension.
					 *
					 * @tparam I The type of the indexing scheme/ cell labels
					 * @tparam T The type of the stored array data
					 *
					 * @return Number of cells in the X dimension.
					 */
					__attribute__((warn_unused_result))
					inline I getCellX();

					/**
					 * Set the number of cells in the X dimension.
					 *
					 * @param cellX Number of cells in the X dimension.
					 *
					 * @tparam I The type of the indexing scheme/ cell labels
					 * @tparam T The type of the stored array data
					 *
					 * @return Nothing
					 */
					inline void setCellX(I cellX);

					/**
					 * Get the number of cells in the Y dimension.
					 *
					 * @tparam I The type of the indexing scheme/ cell labels
					 * @tparam T The type of the stored array data
					 *
					 * @return Number of cells in the Y dimension.
					 */
					__attribute__((warn_unused_result))
					inline I getCellY();

					/**
					 * Set the number of cells in the Y dimension.
					 *
					 * @param cellY Number of cells in the Y dimension.
					 *
					 * @tparam I The type of the indexing scheme/ cell labels
					 * @tparam T The type of the stored array data
					 *
					 * @return Nothing
					 */
					inline void setCellY(I cellY);

					/**
					 * Get the number of cells in the Z dimension.
					 *
					 * @tparam I The type of the indexing scheme/ cell labels
					 * @tparam T The type of the stored array data
					 *
					 * @return Number of cells in the Z dimension.
					 */
					__attribute__((warn_unused_result))
					inline I getCellZ();

					/**
					 * Set the number of cells in the Z dimension.
					 *
					 * @param cellZ Number of cells in the Z dimension.
					 *
					 * @tparam I The type of the indexing scheme/ cell labels
					 * @tparam T The type of the stored array data
					 *
					 * @return Nothing
					 */
					inline void setCellZ(I cellZ);

					/**
					 * Get the minimum spatial position in the X dimension
					 *
					 * @tparam I The type of the indexing scheme/ cell labels
					 * @tparam T The type of the stored array data
					 *
					 * @return Minimum spatial position in the X dimension.
					 */
					__attribute__((warn_unused_result))
					inline T getSpatialMinX();

					/**
					 * Set the minimum position in the spatial domain of the X dimension.
					 *
					 * @param sMinX The minimum position in the X dimension.
					 *
					 * @tparam I The type of the indexing scheme/ cell labels
					 * @tparam T The type of the stored array data
					 *
					 * @return Nothing
					 */
					inline void setSpatialMinX(T sMinX);

					/**
					 * Get the minimum spatial position in the Y dimension
					 *
					 * @tparam I The type of the indexing scheme/ cell labels
					 * @tparam T The type of the stored array data
					 *
					 * @return Minimum spatial position in the Y dimension.
					 */
					__attribute__((warn_unused_result))
					inline T getSpatialMinY();

					/**
					 * Set the minimum position in the spatial domain of the Y dimension.
					 *
					 * @param sMinY The minimum position in the Y dimension.
					 *
					 * @tparam I The type of the indexing scheme/ cell labels
					 * @tparam T The type of the stored array data
					 *
					 * @return Nothing
					 */
					inline void setSpatialMinY(T sMinY);

					/**
					 * Get the minimum spatial position in the Z dimension
					 *
					 * @tparam I The type of the indexing scheme/ cell labels
					 * @tparam T The type of the stored array data
					 *
					 * @return Minimum spatial position in the Z dimension.
					 */
					__attribute__((warn_unused_result))
					inline T getSpatialMinZ();

					/**
					 * Set the minimum position in the spatial domain of the Z dimension.
					 *
					 * @param sMinZ The minimum position in the Z dimension.
					 *
					 * @tparam I The type of the indexing scheme/ cell labels
					 * @tparam T The type of the stored array data
					 *
					 * @return Nothing
					 */
					inline void setSpatialMinZ(T sMinZ);

					/**
					 * Get the maximum spatial position in the X dimension
					 *
					 * @tparam I The type of the indexing scheme/ cell labels
					 * @tparam T The type of the stored array data
					 *
					 * @return Maximum spatial position in the X dimension.
					 */
					__attribute__((warn_unused_result))
					inline T getSpatialMaxX();

					/**
					 * Set the maximum position in the spatial domain of the X dimension.
					 *
					 * @param sMaxX The maximum position in the X dimension.
					 *
					 * @tparam I The type of the indexing scheme/ cell labels
					 * @tparam T The type of the stored array data
					 *
					 * @return Nothing
					 */
					inline void setSpatialMaxX(T sMaxX);

					/**
					 * Get the maximum spatial position in the Y dimension
					 *
					 * @tparam I The type of the indexing scheme/ cell labels
					 * @tparam T The type of the stored array data
					 *
					 * @return Maximum spatial position in the Y dimension.
					 */
					__attribute__((warn_unused_result))
					inline T getSpatialMaxY();

					/**
					 * Set the maximum position in the spatial domain of the Y dimension.
					 *
					 * @param sMaxY The maximum position in the Y dimension.
					 *
					 * @tparam I The type of the indexing scheme/ cell labels
					 * @tparam T The type of the stored array data
					 *
					 * @return Nothing
					 */
					inline void setSpatialMaxY(T sMaxY);

					/**
					 * Get the maximum spatial position in the Z dimension
					 *
					 * @tparam I The type of the indexing scheme/ cell labels
					 * @tparam T The type of the stored array data
					 *
					 * @return Maximum spatial position in the Z dimension.
					 */
					__attribute__((warn_unused_result))
					inline T getSpatialMaxZ();

					/**
					 * Set the maximum position in the spatial domain of the Z dimension.
					 *
					 * @param sMaxZ The maximum position in the Z dimension.
					 *
					 * @tparam I The type of the indexing scheme/ cell labels
					 * @tparam T The type of the stored array data
					 *
					 * @return Nothing
					 */
					inline void setSpatialMaxZ(T sMaxZ);

					// === Overloaded Methods ===

					inline void operator=(const MeshSourceStructGenConfig<I,T>& source);
					MeshSourceStructGenConfig<I,T> * clone();
					__attribute__((warn_unused_result))
					cupcfd::error::eCodes buildMeshSource(MeshSource<I,T,I> ** source);
			};
		} // namespace mesh
	} // namespace geometry
} // namespace cupcfd

// Include Header Level Definitions
#include "MeshSourceStructGenConfig.ipp"

#endif
