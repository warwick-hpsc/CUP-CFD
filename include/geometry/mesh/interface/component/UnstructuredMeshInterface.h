/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 * This interface was coded from scratch, but the original basis for the
 * data structure was derived from Dolfyn:
 * Copyright 2003-2009 Henk Krus, Cyclone Fluid Dynamics BV
 * All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.dolfyn.net/license.html
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 * @section DESCRIPTION
 *
 * Contains declarations of the UnstructuredMeshInterface Class
 */

#ifndef CUPCFD_GEOMETRY_UNSTRUCTURED_MESH_INTERFACE_INCLUDE_H
#define CUPCFD_GEOMETRY_UNSTRUCTURED_MESH_INTERFACE_INCLUDE_H

#include <string>

#include "Error.h"
#include "UnstructuredMeshProperties.h"
#include "Communicator.h"
#include "DistributedAdjacencyList.h"
#include "EuclideanVector.h"
#include "EuclideanVector3D.h"
#include "EuclideanPoint.h"
#include "MeshSource.h"
#include "Polyhedron.h"
#include "Tetrahedron.h"
#include "TriPrism.h"
#include "QuadPyramid.h"
#include "Hexahedron.h"

namespace euc = cupcfd::geometry::euclidean;
namespace shapes = cupcfd::geometry::shapes;

namespace cupcfd
{
	namespace geometry
	{
		namespace mesh
		{
			/** Boundary Types **/
			enum RType
			{
				RTYPE_DEFAULT,
				RTYPE_INLET,
				RTYPE_OUTLET,
				RTYPE_SYMP,
				RTYPE_WALL
			};

			/**
			 * Types of Cell:
			 * https://en.wikipedia.org/wiki/Types_of_mesh
			 * Does not currently support arbitrary polyhedrons
			 */
			enum CellType
			{
				TETRAHEDRON,
				QUADPYRAMID,
				TRIPRISM,
				HEXHEDRAL,
			};

			/**
			 * The unstructured mesh class stores the geometry data and relationships between the various unstructured
			 * components - e.g. cell, face etc through the use of suitable indexes.
			 *
			 * Since the distributed nature may require careful indexing, the data is typically supplied in the context
			 * of an unknown data source that has assigned a global index to each of the components - i.e. each cell, vertex
			 * etc has a unique global ID of some form in the data source. This class will track that global ID provided when
			 * adding a component, and use it to remap to an internal local ID and an internal global ID for later use, as well
			 * as using it to track the relationships when adding components.
			 *
			 * This results in three different indexing schemes:
			 * (1) Build Label - Used for addCell, addFace etc methods - This is a global scheme that uniquely identifies each
			 * cell, vertex etc when constructing the mesh. May be obtained from another source such as a file.
			 * This can in effect be an arbitrary assignment, as long as it is globally unique across the mesh.
			 * By and large, this Label scheme is only used when building the mesh for 'add' methods and subsequently ignored
			 * in favour of Index Scheme (3) below.
			 *
			 * (2) Local ID - This is a remapped index scheme that assigns a local ID to each cell, vertex etc added, specific to
			 * this MPI rank. This ID is used for getters, setters etc. For cells, this is expected to be consistent with the local
			 * ID from the connectivity graph.
			 *
			 * (3) Global ID - This is a second Global Index scheme used for cells derived from the connectivity graph,
			 * but one constructed internally for use by other systems -e.g. remapping to facilitate partitioning etc.
			 *
			 * The cells of the mesh should be grouped into locally owned cells and ghost cells, such that
			 * all locally owned cells have a lower local cell ID than the ghost cells - this is to facilitate
			 * looping over locally owned or ghost cells only without having to do index lookups for local/ghost cells
			 * from the connectivity graph.
			 *
			 * The design pattern for this uses CRTP over inheritance, since it is desirable to avoid virtual overheads for
			 * getters and setters to permit a single interface access for kernels. This should allow for the use of different
			 * Unstructured Mesh specialisations behind the interface (e.g. different storage formats such AoS, SoA) without
			 * incurring the virtual method penalty or the need to reimplement multiple variants of the same kernel just to
			 * change the accessors.
			 *
			 * @tparam M The mesh data type where the underlying data is stored.
			 * @tparam I The type of the indexing scheme (integer based)
			 * @tparam T The type of the mesh data stores (i.e. float, double etc)
			 * @tparam L The type of the label used to identify components when adding (different from index)
			 */
			template <class M, class I, class T, class L>
			class UnstructuredMeshInterface
			{
				public:

					// === Members ===

					/** Stores properties of the mesh, such as sizes etc **/
					UnstructuredMeshProperties<I,T> properties;

					/**
					 * Stores the cell->cell connectivity graph.
					 * Edges are equivalent to faces.
					 * Does not track boundary conditions.
					 **/
					cupcfd::data_structures::DistributedAdjacencyList<I, I> * cellConnGraph;

					/**
					 * Track whether the mesh has been finalized (i.e. any setup comms are done)
					 * and the mesh is ready for use in computation
					 **/
					bool finalized;

					// === Constructors/Deconstructors

					/**
					 * Default constructors. Sets up mesh with no cells, faces, boundaries etc.
					 * Sizes are set to zero
					 *
					 * @param comm The communicator the mesh is distributed across
					 *
					 * @tparam I The type of the indexing scheme (integer based)
					 * @tparam T The type of the stored array data
					 */
					UnstructuredMeshInterface(cupcfd::comm::Communicator& comm);

					/**
					 * Sets up a mesh and populates it with data from a source of mesh data.
					 * This mesh is then automatically finalized to distribute the data.
					 *
					 * Due to the finalization stage, all members of comm must call this constructor.
					 *
					 * @param comm The communicator the mesh is distributed across
					 * @param data The souce of mesh data to load into the unstructured mesh
					 * @param assignedCellLabels The labels to load from the data source on this rank
					 * @param nAssignedCellLabels The number of elements of type L in cellLabels
					 *
					 * @tparam I The type of the indexing scheme (integer based)
					 * @tparam T The type of the stored array data
					 */
					UnstructuredMeshInterface(cupcfd::comm::Communicator& comm, MeshSource<I,T,L>& data,
											  L * assignedCellLabels, I nAssignedCellLabels);

					/**
					 * Deconstructor. Destroys internal data structures.
					 *
					 * @tparam I The type of the indexing scheme (integer based)
					 * @tparam T The type of the stored array data
					 */
					virtual ~UnstructuredMeshInterface();

					// === Interface Methods ===

					// =========================== Cell Operators ===========================

					/**
					 * Get the point at the center of the cell
					 *
					 * @param cellID The local ID of the cell on this process
					 * @param center The point object to be updated with the result
					 *
					 * @tparam I The type of the indexing scheme (integer based)
					 * @tparam T The type of the stored array data
					 *
					 * @return An error status indicating the success or failure of the operation
					 * @retval cupcfd::error::E_SUCCESS Success
					 */
					void getCellCenter(I cellID, euc::EuclideanPoint<T,3>& center);

					/**
					 * Get the stored cell center point of a cell
					 *
					 * @param cellID The local ID of the cell on this process
					 *
					 * @tparam I The type of the indexing scheme (integer based)
					 * @tparam T The type of the stored array data
					 *
					 * @return The center point of the cell
					 */
					__attribute__((warn_unused_result))
					euc::EuclideanPoint<T,3> getCellCenter(I cellID);

					/**
					 * Get the stored cell volume of a cell
					 *
					 * @param cellID The local ID (not label) of the cell on this process
					 * @param vol A pointer to the location where the retrieved cell volume will be stored
					 *
					 * @tparam I The type of the indexing scheme (integer based)
					 * @tparam T The type of the stored array data
					 *
					 * @return An error status indicating the success or failure of the operation
					 * @retval cupcfd::error::E_SUCCESS Success
					 */
					void getCellVolume(I cellID, T * vol);

					/**
					 * Get the stored cell volume of a cell.
					 *
					 * @param cellID The local ID (not label) of the cell on this process
					 *
					 * @tparam I The type of the indexing scheme (integer based)
					 * @tparam T The type of the stored array data
					 *
					 * @return The volume of the cell
					 */
					__attribute__((warn_unused_result))
					T getCellVolume(I cellID);


					/**
					 * Get the number of faces associated with a cell.
					 *
					 * For ghost cells, this reports the number of faces associated with cell in the overall
					 * global mesh (i.e. the geometric number), not the number of faces stored on this rank
					 * associated with the cell.
					 *
					 * Note: Ghost Cells will only have faces that adjoin a local cell associated with them
					 *
					 * Note: This method is only valid after calling finalize.
					 *
					 * @param cellID The local ID of the cell on this process
					 * @param nFaces A pointer to the location where the retrieved number of cell faces will be stored
					 *
					 * @tparam I The type of the indexing scheme (integer based)
					 * @tparam T The type of the stored array data
					 *
					 * @return An error status indicating the success or failure of the operation
					 * @retval cupcfd::error::E_SUCCESS Success
					 */
					void getCellNFaces(I cellID, I * nFaces);

					/**
					 * Get the number of faces associated with a cell
					 *
					 * For ghost cells, this reports the number of faces associated with cell in the overall
					 * global mesh (i.e. the geometric number), not the number of faces stored on this rank
					 * associated with the cell.
					 *
					 * Note: Ghost Cells will only have faces that adjoin a local cell associated with them
					 *
					 * Note: This method is only valid after calling finalize.
					 *
					 * @param cellID The local ID of the cell on this process
					 *
					 * @tparam I The type of the indexing scheme (integer based)
					 * @tparam T The type of the stored array data
					 *
					 */
					__attribute__((warn_unused_result))
					I getCellNFaces(I cellID);

					/**
					 * Get the number of faces associated with a cell that are stored on this rank.
					 *
					 * For local cells, this is the same as for getCellNFaces.
					 * For ghost cells, this is the number of associated faces that have a copy
					 * stored on this rank (since only faces that share a local->ghost boundary are stored).
					 *
					 * Note: This method is only valid after calling finalize.
					 *
					 * @param cellID The local ID of the cell on this process
					 * @param nFaces A pointer to the location where the number of cell faces will be stored
					 *
					 * @tparam I The type of the indexing scheme (integer based)
					 * @tparam T The type of the stored array data
					 *
					 * @return An error status indicating the success or failure of the operation
					 * @retval cupcfd::error::E_SUCCESS Success
					 */
					void getCellStoredNFaces(I cellID, I * nFaces);

					/**
					 * Get the number of faces associated with a cell that are stored on this rank.
					 *
					 * For local cells, this is the same as for getCellNFaces.
					 * For ghost cells, this is the number of associated faces that have a copy
					 * stored on this rank (since only faces that share a local->ghost boundary are stored).
					 *
					 * Note: This method is only valid after calling finalize.
					 *
					 * @param cellID The local ID of the cell on this process
					 *
					 * @tparam I The type of the indexing scheme (integer based)
					 * @tparam T The type of the stored array data
					 *
					 * @return The number of cell faces
					 */
					__attribute__((warn_unused_result))
					I getCellStoredNFaces(I cellID);

					/**
					 * Return the number of vertices a cell has
					 *
					 * For ghost cells, this reports the number of faces associated with cell in the overall
					 * global mesh (i.e. the geometric number), not the number of vertices stored on this rank
					 * that could be linked to the cell.
					 *
					 * Note: This method is only valid after calling finalize.
					 *
					 * @param cellID The local ID of the cell
					 * @param nVertices A pointer to the location where the number of vertices will be stored
					 *
					 * @tparam I The type of the indexing scheme (integer based)
					 * @tparam T The type of the stored array data
					 *
					 * @return An error status indicating the success or failure of the operation
					 * @retval cupcfd::error::E_SUCCESS Success
					 */
					void getCellNVertices(I cellID, I * nVertices);

					/**
					 * Return the number of vertices a cell has.
					 *
					 * For ghost cells, this reports the number of faces associated with cell in the overall
					 * global mesh (i.e. the geometric number), not the number of vertices stored on this rank
					 * that could be linked to the cell.
					 *
					 * Note: This method is only valid after calling finalize.
					 *
					 * @param cellID The local ID of the cell
					 *
					 * @tparam I The type of the indexing scheme (integer based)
					 * @tparam T The type of the stored array data
					 *
					 * @return The number of vertices
					 */
					__attribute__((warn_unused_result))
					I getCellNVertices(I cellID);

					/**
					 * Return the number of vertices associated with a cell that are stored on this rank.
					 *
					 * For local cells, this is the same as for getCellNVertices.
					 * For ghost cells, this is the number of associated vertices that have a copy
					 * stored on this rank (since only faces that share a local->ghost boundary are stored).
					 *
					 * Note: This method is only valid after calling finalize.
					 *
					 * @param cellID The local ID of the cell
					 * @param nVertices A pointer to the location where the number of vertices will be stored
					 *
					 * @tparam I The type of the indexing scheme (integer based)
					 * @tparam T The type of the stored array data
					 *
					 * @return An error status indicating the success or failure of the operation
					 * @retval cupcfd::error::E_SUCCESS Success
					 */
					void getCellStoredNVertices(I cellID, I * nVertices);

					/**
					 * Return the number of vertices associated with a cell that are stored on this rank.
					 *
					 * For local cells, this is the same as for getCellNVertices.
					 * For ghost cells, this is the number of associated vertices that have a copy
					 * stored on this rank (since only faces that share a local->ghost boundary are stored).
					 *
					 * Note: This method is only valid after calling finalize.
					 *
					 * @param cellID The local ID of the cell
					 *
					 * @tparam I The type of the indexing scheme (integer based)
					 * @tparam T The type of the stored array data
					 *
					 * @return The number of vertices
					 */
					__attribute__((warn_unused_result))
					I getCellStoredNVertices(I cellID);

					/**
					 * Get the local face ID of one of the faces associated with a cell that is stored on this rank.
					 *
					 * Note: Ghost Cells will only have faces that adjoin a local cell associated with them stored on this rank.
					 *
					 * @param cellID The local ID of the cell on this process
					 * @param cellFaceID Select one of the faces of the cell - 0 through the number of faces the cell has.
					 * The assigned value to a face is arbitrary, but looping through each id will loop over all faces of a cell.
					 * @param A pointer to the location where the local ID of the face in the mesh will be stored
					 *
					 * @tparam I The type of the indexing scheme (integer based)
					 * @tparam T The type of the stored array data
					 *
					 * @return An error status indicating the success or failure of the operation
					 * @retval cupcfd::error::E_SUCCESS Success
					 */
					void getCellFaceID(I cellID, I cellFaceID, I * faceID);

					/**
					 * Get the local face ID of one of the faces associated with a cell that is stored on this rank.
					 *
					 * Note: Ghost Cells will only have faces that adjoin a local cell associated with them stored on this rank.
					 *
					 * @param cellID The local ID of the cell on this process
					 * @param cellFaceID Select one of the faces of the cell - 0 through the number of faces the cell has.
					 *
					 * @tparam I The type of the indexing scheme (integer based)
					 * @tparam T The type of the stored array data
					 *
					 * @return The local ID of the face in the mesh on this process
					 */
					__attribute__((warn_unused_result))
					I getCellFaceID(I cellID, I cellFaceID);

					/**
					 * Set the stored center point of a cell
					 *
					 * @param cellID The local ID of the cell on this process
					 * @param center The new cell center point
					 *
					 * @tparam I The type of the indexing scheme (integer based)
					 * @tparam T The type of the stored array data
					 *
					 * @return An error status indicating the success or failure of the operation
					 * @retval cupcfd::error::E_SUCCESS Success
					 */
					void setCellCenter(I cellID, euc::EuclideanPoint<T,3>& center);

					/**
					 * Set the stored cell volume
					 *
					 * @param cellID The local ID of the cell in the mesh on this process
					 * @param The new cell volume
					 *
					 * @tparam I The type of the indexing scheme (integer based)
					 * @tparam T The type of the stored array data
					 *
					 * @return An error status indicating the success or failure of the operation
					 * @retval cupcfd::error::E_SUCCESS Success
					 */
					void setCellVolume(I cellID, T vol);

					/**
					 * Add a cell to the mesh on this rank.
					 *
					 * @param cellLabel A numeric cell label. This is a unique label associated with the cell.
					 * It is not the same as the local cell ID, which is created and managed by the unstructured mesh.
					 * @param center The center vector for the cell
					 * @param vol The volume for the cell
					 * @param isLocal Indicate whether the cell is a locally owned or ghost cell
					 *
					 * @tparam I The type of the indexing scheme (integer based)
					 * @tparam T The type of the stored array data
					 *
					 * @return The id of the added cell
					 */
					__attribute__((warn_unused_result))
					cupcfd::error::eCodes addCell(L cellLabel,
													   euc::EuclideanPoint<T,3>& center,
													   T vol,
													   bool isLocal);

					/**
					 * Add a cell to the mesh on this rank.
					 *
					 * @param cellLabel A numeric cell label. This is a unique label associated with the cell.
					 * It is not the same as the local cell ID, which is created and managed by the unstructured mesh.
					 * @param isLocal Indicate whether the cell is a locally owned or ghost cell
					 *
					 * @tparam I The type of the indexing scheme (integer based)
					 * @tparam T The type of the stored array data
					 *
					 * @return An error status indicating the success or failure of the operation
					 * @retval cupcfd::error::E_SUCCESS Success
					 */
					__attribute__((warn_unused_result))
					cupcfd::error::eCodes addCell(L cellLabel, bool isLocal);

					/**
					 * Retrieve the local cell ID on this MPI rank for the given cell label.
					 * Note: The function can be used before finalize, but the value returned
					 * may differ after calling finalize, meaning finalize will invalidate any
					 * prior returned values and they must be refreshed.
					 *
					 * @param cellLabel The cell label
					 * @param A pointer to the location where the numeric cell ID will be stored
					 *
					 * @tparam I The type of the indexing scheme (integer based)
					 * @tparam T The type of the stored array data
					 *
					 * @return An error status indicating the success or failure of the operation
					 * @retval cupcfd::error::E_SUCCESS Success
					 */
					void getCellID(L cellLabel, I * cellID);

					/**
					 * Retrieve the local cell ID on this MPI rank for the given cell label.
					 * Note: The function can be used before finalize, but the value returned
					 * may differ after calling finalize, meaning finalize will invalidate any
					 * prior returned values and they must be refreshed.
					 *
					 * @param cellLabel The cell label
					 *
					 * @tparam I The type of the indexing scheme (integer based)
					 * @tparam T The type of the stored array data
					 *
					 * @return The local cell ID for the given cell label
					 */
					__attribute__((warn_unused_result))
					I getCellID(L cellLabel);

					// =========================== Face Operators ===========================

					/**
					 * Get the number of vertices associated with a face
					 *
					 * @param faceID The local ID of the face in the mesh on this process
					 * @param A pointer to the location where the number of vertices will be stored
					 *
					 * @tparam I The type of the indexing scheme (integer based)
					 * @tparam T The type of the stored array data
					 *
					 * @return An error status indicating the success or failure of the operation
					 * @retval cupcfd::error::E_SUCCESS Success
					 */
					inline void getFaceNVertices(I faceID, I * nVertices);

					/**
					 * Get the number of vertices associated with a face
					 *
					 * @param faceID The local ID of the face in the mesh on this process
					 *
					 * @tparam I The type of the indexing scheme (integer based)
					 * @tparam T The type of the stored array data
					 *
					 * @return The number of vertices
					 */
					__attribute__((warn_unused_result))
					inline I getFaceNVertices(I faceID);

					/**
					 * Get the local cell ID in the mesh of the first cell associated with a face
					 *
					 * The first cell has the face normal going away from it
					 *
					 * @param faceID The local ID of the face in the mesh on this process
					 * @param A pointer to the location where the local cell ID retrieved by this method is stored
					 *
					 * @tparam I The type of the indexing scheme (integer based)
					 * @tparam T The type of the stored array data
					 *
					 * @return An error status indicating the success or failure of the operation
					 * @retval cupcfd::error::E_SUCCESS Success
					 */
					void getFaceCell1ID(I faceID, I * cellID);

					/**
					 * Get the local cell ID in the mesh of the first cell associated with a face
					 *
					 * The first cell has the face normal going away from it
					 *
					 * @param faceID The local ID of the face in the mesh on this process
					 *
					 * @tparam I The type of the indexing scheme (integer based)
					 * @tparam T The type of the stored array data
					 *
					 * @return The local cell ID in the mesh
					 */
					__attribute__((warn_unused_result))
					I getFaceCell1ID(I faceID);

					/**
					 * Get the local cell ID in the mesh of the second cell associated with a face
					 *
					 * The second associated cell has the face normal going towards it
					 *
					 * @param faceID The local ID of the face in the mesh on this process
					 *
					 * @tparam I The type of the indexing scheme (integer based)
					 * @tparam T The type of the stored array data
					 *
					 * @return An error status indicating the success or failure of the operation
					 * @retval cupcfd::error::E_SUCCESS Success
					 */
					void getFaceCell2ID(I faceID, I * cellID);

					/**
					 * Get the local cell ID in the mesh of the second cell associated with a face
					 *
					 * The second associated cell has the face normal going towards it
					 *
					 * @param faceID The local ID of the face in the mesh on this process
					 *
					 * @tparam I The type of the indexing scheme (integer based)
					 * @tparam T The type of the stored array data
					 *
					 * @return The local cell ID in the mesh
					 */
					__attribute__((warn_unused_result))
					I getFaceCell2ID(I faceID);

					/**
					 * Get the stored lambda value for this face
					 *
					 * @param faceID The local ID of the face in the mesh on this process
					 * @param lambda A pointer to the location where the retrieved lambda value is stored
					 *
					 * @tparam I The type of the indexing scheme (integer based)
					 * @tparam T The type of the stored array data
					 *
					 * @return An error status indicating the success or failure of the operation
					 * @retval cupcfd::error::E_SUCCESS Success
					 */
					void getFaceLambda(I faceID, T * lambda);

					/**
					 * Get the stored lambda value for this face
					 *
					 * @param faceID The local ID of the face in the mesh on this process
					 *
					 * @tparam I The type of the indexing scheme (integer based)
					 * @tparam T The type of the stored array data
					 *
					 * @return The stored face lambda value
					 */
					__attribute__((warn_unused_result))
					T getFaceLambda(I faceID);

					/**
					 * Get the stored face normal vector. This will point from cell 1 towards cell 2.
					 *
					 * @param faceID The local ID of the face in the mesh on this process
					 * @param norm The vector where the retrieved normal vector will be stored
					 *
					 * @tparam I The type of the indexing scheme (integer based)
					 * @tparam T The type of the stored array data
					 *
					 * @return An error status indicating the success or failure of the operation
					 * @retval cupcfd::error::E_SUCCESS Success
					 */
					void getFaceNorm(I faceID, euc::EuclideanVector3D<T>& norm);

					/**
					 * Get the stored face normal vector. This will point from cell 1 towards cell 2.
					 *
					 * @param faceID The local ID of the face in the mesh on this process
					 *
					 * @tparam I The type of the indexing scheme (integer based)
					 * @tparam T The type of the stored array data
					 *
					 * @return The stored face normal vector
					 */
					__attribute__((warn_unused_result))
					euc::EuclideanVector3D<T> getFaceNorm(I faceID);

					/**
					 * Get the local mesh ID of one of the face vertexes.
					 *
					 * It is currently expected that each vertex is connected via an edge to the previous vertex and next
					 * vertex in the list. The one exception to this is the last vertex, which is connected to the
					 * previous vertex and the very first vertex (and vice versa).
					 *
					 * No clockwise/anti-clockwise ordering is currently enforced, but this may change in the future.
					 *
					 * @param faceID The local ID of the face in the mesh on this process. This is set to -1 if the vertex does not exist
					 * (e.g. vertex 3 for a triangular face in 0->3 index scheme - only 0, 1 and 2 are valid vertexIDs)
					 * @param faceVertexID A value selecting one of the face vertexes, 0 though to number of vertices
					 * @param vertexID A pointer to the location where the retrieved local ID of the vertex in the mesh is stored
					 *
					 * @tparam I The type of the indexing scheme (integer based)
					 * @tparam T The type of the stored array data
					 *
					 * @return An error status indicating the success or failure of the operation
					 * @retval cupcfd::error::E_SUCCESS Success
					 */
					void getFaceVertex(I faceID, I faceVertexID, I * vertexID);

					/**
					 * Get the local mesh ID of one of the face vertexes
					 *
					 * It is currently expected that each vertex is connected via an edge to the previous vertex and next
					 * vertex in the list. The one exception to this is the last vertex, which is connected to the
					 * previous vertex and the very first vertex (and vice versa).
					 *
					 * No clockwise/anti-clockwise ordering is currently enforced, but this may change in the future.
					 *
					 * @param faceID The local ID of the face in the mesh on this process
					 * @param faceVertexID A value selecting one of the face vertexes, 0 though to number of vertices
					 *
					 * @tparam I The type of the indexing scheme (integer based)
					 * @tparam T The type of the stored array data
					 *
					 * @return The stored local ID of the vertex in the mesh. This is set to -1 if the vertex does not exist,
					 * for the given faceVertexID (e.g. ID 3 in a triangular face since there are only three vertices in a zero
					 * indexed scheme)
					 */
					__attribute__((warn_unused_result))
					I getFaceVertex(I faceID, I faceVertexID);

					/**
					 * Get the stored face center point
					 *
					 * @param faceID The local ID of the face in the mesh on this process
					 * @param center A point object whose values will be updated to those of the stored face center point
					 *
					 * @tparam I The type of the indexing scheme (integer based)
					 * @tparam T The type of the stored array data
					 *
					 * @return An error status indicating the success or failure of the operation
					 * @retval cupcfd::error::E_SUCCESS Success
					 */
					void getFaceCenter(I faceID, euc::EuclideanPoint<T,3>& center);

					/**
					 * Get the stored face center point
					 *
					 * @param faceID The local ID of the face in the mesh on this process
					 *
					 * @tparam I The type of the indexing scheme (integer based)
					 * @tparam T The type of the stored array data
					 *
					 * @return The stored face center point
					 */
					__attribute__((warn_unused_result))
					euc::EuclideanPoint<T,3> getFaceCenter(I faceID);

					/**
					 * Get the stored face rlencos
					 *
					 * @param faceID The local ID of the face in the mesh on this process
					 * @param rlencos A pointer to the location where the retrieved rlencos will be stored
					 *
					 * @tparam I The type of the indexing scheme (integer based)
					 * @tparam T The type of the stored array data
					 *
					 * @return An error status indicating the success or failure of the operation
					 * @retval cupcfd::error::E_SUCCESS Success
					 */
					void getFaceRLencos(I faceID, T * rlencos);

					/**
					 * Get the stored face rlencos
					 *
					 * @param faceID The local ID of the face in the mesh on this process
					 *
					 * @tparam I The type of the indexing scheme (integer based)
					 * @tparam T The type of the stored array data
					 *
					 * @return The stored face rlencos
					 */
					__attribute__((warn_unused_result))
					T getFaceRLencos(I faceID);

					/**
					 * Get the stored face area
					 *
					 * @param faceID The local ID of the face in the mesh on this process
					 * @param A pointer to where the retrieved face area will be stored
					 *
					 * @tparam I The type of the indexing scheme (integer based)
					 * @tparam T The type of the stored array data
					 *
					 * @return An error status indicating the success or failure of the operation
					 * @retval cupcfd::error::E_SUCCESS Success
					 */
					void getFaceArea(I faceID, T * area);

					/**
					 * Get the stored face area
					 *
					 * @param faceID The local ID of the face in the mesh on this process
					 *
					 * @tparam I The type of the indexing scheme (integer based)
					 * @tparam T The type of the stored array data
					 *
					 * @return The stored face area
					 */
					__attribute__((warn_unused_result))
					T getFaceArea(I faceID);

					/**
					 * Get the stored face xpac
					 *
					 * @param faceID The local ID of the face in the mesh on this process
					 * @param xpac The point object that will have its values set to that of the stored face xpac
					 *
					 * @tparam I The type of the indexing scheme (integer based)
					 * @tparam T The type of the stored array data
					 *
					 * @return An error status indicating the success or failure of the operation
					 * @retval cupcfd::error::E_SUCCESS Success
					 */
					void getFaceXpac(I faceID, euc::EuclideanPoint<T,3>& xpac);

					/**
					 * Get the stored face xpac
					 *
					 * @param faceID The local ID of the face in the mesh on this process
					 *
					 * @tparam I The type of the indexing scheme (integer based)
					 * @tparam T The type of the stored array data
					 *
					 * @return The stored face xpac
					 */
					__attribute__((warn_unused_result))
					euc::EuclideanPoint<T,3> getFaceXpac(I faceID);

					/**
					 * Get the stored face xnac
					 *
					 * @param faceID The local ID of the face in the mesh on this process
					 * @param xnac The object where the result will be stored
					 *
					 * @tparam I The type of the indexing scheme (integer based)
					 * @tparam T The type of the stored array data
					 *
					 * @return An error status indicating the success or failure of the operation
					 * @retval cupcfd::error::E_SUCCESS Success
					 */
					void getFaceXnac(I faceID, euc::EuclideanPoint<T,3>& xnac);

					/**
					 * Get the stored face xnac
					 *
					 * @param faceID The local ID of the face in the mesh on this process
					 *
					 * @tparam I The type of the indexing scheme (integer based)
					 * @tparam T The type of the stored array data
					 *
					 * @return The stored face xnac
					 */
					__attribute__((warn_unused_result))
					euc::EuclideanPoint<T,3> getFaceXnac(I faceID);

					/**
					 * Identify whether the face is a boundary face
					 *
					 * @param faceID The local ID of the face in the mesh on this process
					 * @param bool Where the result will be stored - true is a boundary, false if not
					 *
					 * @tparam I The type of the indexing scheme (integer based)
					 * @tparam T The type of the stored array data
					 *
					 * @return An error status indicating the success or failure of the operation
					 * @retval cupcfd::error::E_SUCCESS Success
					 */
					void getFaceIsBoundary(I faceID, bool * result);

					/**
					 * Identify whether the face is a boundary face
					 *
					 * @param faceID The local ID of the face in the mesh on this process
					 *
					 * @tparam I The type of the indexing scheme (integer based)
					 * @tparam T The type of the stored array data
					 *
					 * @return Whether the face is a boundary
					 * @retval true The face is a boundary
					 * @retval false The face is not a boundary
					 */
					__attribute__((warn_unused_result))
					bool getFaceIsBoundary(I faceID);

					/**
					 * Get the stored local boundary id in the mesh of the boundary associated with a face
					 *
					 * @param faceID The local ID of the face in the mesh on this process
					 * @param A pointer to where the boundary ID will be stored. This will be -1 if this is not a boundary face.
					 *
					 * @tparam I The type of the indexing scheme (integer based)
					 * @tparam T The type of the stored array data
					 *
					 * @return An error status indicating the success or failure of the operation
					 * @retval cupcfd::error::E_SUCCESS Success
					 */
					void getFaceBoundaryID(I faceID, I * boundaryID);

					/**
					 * Get the stored local mesh id of the boundary associated with a face
					 *
					 * @param faceID The local ID of the face in the mesh on this process
					 *
					 * @tparam I The type of the indexing scheme (integer based)
					 * @tparam T The type of the stored array data
					 *
					 * @return The stored local boundary id in the mesh
					 * @retval -1 The face is not a boundary face
					 * @retval >-1 The boundary id
					 */
					__attribute__((warn_unused_result))
					I getFaceBoundaryID(I faceID);

					/**
					 * Set the stored cell ID of the first associated cell of the face
					 *
					 * @param faceID The local ID of the face in the mesh on this process
					 * @param cellID The cellID to set the face cell ID to
					 *
					 * @tparam I The type of the indexing scheme (integer based)
					 * @tparam T The type of the stored array data
					 *
					 * @return An error status indicating the success or failure of the operation
					 * @retval cupcfd::error::E_SUCCESS Success
					 */
					void setFaceCell1ID(I faceID, I cellID);

					/**
					 * Set the stored cell ID of the second associated cell of the face
					 *
					 * @param faceID The local ID of the face in the mesh on this process
					 * @param cellID The cellID to set the face cell ID to
					 *
					 * @tparam I The type of the indexing scheme (integer based)
					 * @tparam T The type of the stored array data
					 *
					 * @return An error status indicating the success or failure of the operation
					 * @retval cupcfd::error::E_SUCCESS Success
					 */
					void setFaceCell2ID(I faceID, I cellID);

					/**
					 * Set the stored face lambda of a face in the mesh
					 *
					 * @param faceID The local ID of the face in the mesh on this process
					 * @param lambda The value to set the stored face lambda value to
					 *
					 * @tparam I The type of the indexing scheme (integer based)
					 * @tparam T The type of the stored array data
					 *
					 * @return An error status indicating the success or failure of the operation
					 * @retval cupcfd::error::E_SUCCESS Success
					 */
					void setFaceLambda(I faceID, T lambda);

					/**
					 * Set the stored face normal vector
					 *
					 * @param faceID The local ID of the face in the mesh on this process
					 * @param lambda The value to set the stored face norm value to
					 *
					 * @tparam I The type of the indexing scheme (integer based)
					 * @tparam T The type of the stored array data
					 *
					 * @return An error status indicating the success or failure of the operation
					 * @retval cupcfd::error::E_SUCCESS Success
					 */
					void setFaceNorm(I faceID, euc::EuclideanVector3D<T>& norm);

					/**
					 * Set the vertex of one of the associated face vertices
					 *
					 * @param faceID The local ID of the face in the mesh on this process
					 * @param lambda The value to set the stored face vertex ID value to
					 *
					 * @tparam I The type of the indexing scheme (integer based)
					 * @tparam T The type of the stored array data
					 *
					 * @return An error status indicating the success or failure of the operation
					 * @retval cupcfd::error::E_SUCCESS Success
					 */
					void setFaceVertex(I faceID, I faceVertexID, I vertexID);

					/**
					 * Set the stored face center
					 *
					 * @param faceID The local ID of the face in the mesh on this process
					 * @param lambda The value to set the stored face center value to
					 *
					 * @tparam I The type of the indexing scheme (integer based)
					 * @tparam T The type of the stored array data
					 *
					 * @return An error status indicating the success or failure of the operation
					 * @retval cupcfd::error::E_SUCCESS Success
					 */
					void setFaceCenter(I faceID, euc::EuclideanPoint<T,3>& center);

					/**
					 * Set the stored face rlencos
					 *
					 * @param faceID The local ID of the face in the mesh on this process
					 * @param lambda The value to set the stored face rlencos value to
					 *
					 * @tparam I The type of the indexing scheme (integer based)
					 * @tparam T The type of the stored array data
					 *
					 * @return An error status indicating the success or failure of the operation
					 * @retval cupcfd::error::E_SUCCESS Success
					 */
					void setFaceRLencos(I faceID, T rlencos);

					/**
					 * Set the stored face area
					 *
					 * @param faceID The local ID of the face in the mesh on this process
					 * @param lambda The value to set the stored face area value to
					 *
					 * @tparam I The type of the indexing scheme (integer based)
					 * @tparam T The type of the stored array data
					 *
					 * @return An error status indicating the success or failure of the operation
					 * @retval cupcfd::error::E_SUCCESS Success
					 */
					void setFaceArea(I faceID, T area);

					/**
					 * Set the stored face xpac
					 *
					 * @param faceID The local ID of the face in the mesh on this process
					 * @param lambda The value to set the stored face xpac to
					 *
					 * @tparam I The type of the indexing scheme (integer based)
					 * @tparam T The type of the stored array data
					 *
					 * @return An error status indicating the success or failure of the operation
					 * @retval cupcfd::error::E_SUCCESS Success
					 */
					void setFaceXpac(I faceID, euc::EuclideanPoint<T,3>& xpac);

					/**
					 * Set the stored face xnac
					 *
					 * @param faceID The local ID of the face in the mesh on this process
					 * @param lambda The value to set the stored face xnac to
					 *
					 * @tparam I The type of the indexing scheme (integer based)
					 * @tparam T The type of the stored array data
					 *
					 * @return An error status indicating the success or failure of the operation
					 * @retval cupcfd::error::E_SUCCESS Success
					 */
					void setFaceXnac(I faceID, euc::EuclideanPoint<T,3>& xnac);

					/**
					 * Set the stored face local boundary ID
					 *
					 * @param faceID The local ID of the face in the mesh on this process
					 * @param lambda The value to set the stored face boundaryID value to
					 *
					 * @tparam I The type of the indexing scheme (integer based)
					 * @tparam T The type of the stored array data
					 *
					 * @return An error status indicating the success or failure of the operation
					 * @retval cupcfd::error::E_SUCCESS Success
					 */
					void setFaceBoundaryID(I faceID, I boundaryID);


					/**
					 * Store a face in the unstructured mesh with the provided faceLabel.
					 * FaceLabel is an arbitrary label, but must be unique.
					 * Internally, a zero-indexed localID will be generated (faceID).
					 *
					 * @param faceLabel A numeric face label for the new face.
					 * This is a unique label associated with a face, globally across all ranks.
					 * It is not the same as the local face ID, which is created and managed by the unstructured mesh.
					 *
					 * @param cell1Label An existing cell label, associated with the first cell on one side of the face
					 * @param cell2OrBoundaryLabel An existing cell or boundary label. If isBoundary is true, this is treated
					 * as the label for the cell on the second side of the face. if isBoundary is false, this is treated as
					 * the label for an associated mesh boundary.
					 * @param isBoundary Set whether cell2IDOrBoundaryID is a cell (false) or a boundary (true)
					 * @param lambda The lambda of the face
					 * @param norm The normal of the face.
					 * @param vertexLabels An array of labels associated with the vertexs that make up this face.
					 * Important - these vertices must be listed in order of connecting edges - i.e. vertexLabels[0]
					 * connects to vertexLabels[1], 1 to 2, 2 to 3.... till n-1 connects to 0
					 * @param nVertexLabels The size of vertexLabels in the number of elements of type I.
					 * @param center The center of the face
					 * @param rlencos The rlencos value of the face
					 * @param area The area of the face
					 * @param xpac The xpac value of the face
					 * @param xnac The xnac value of the face
					 *
					 * @tparam I The type of the indexing scheme (integer based)
					 * @tparam T The type of the stored array data
					 *
					 * @return An error status indicating the success or failure of the operation
					 * @retval cupcfd::error::E_SUCCESS Success
					 */
					__attribute__((warn_unused_result))
					cupcfd::error::eCodes addFace(
								L faceLabel,
								L cell1Label,
								L cell2OrBoundaryLabel,
								bool isBoundary,
								T lambda,
								euc::EuclideanVector3D<T>& norm,
								L * vertexLabels, I nVertexLabels,
								euc::EuclideanPoint<T,3>& center,
								euc::EuclideanPoint<T,3>& xpac,
								euc::EuclideanPoint<T,3>& xnac,
								T rlencos,
								T area);

					__attribute__((warn_unused_result))
					cupcfd::error::eCodes addFace(
								L faceLabel,
								L cell1Label,
								L cell2OrBoundaryLabel,
								bool isBoundary,
								L * vertexLabels, I nVertexLabels);


					/**
					 * Retrieve the local face ID on this MPI rank for the given face label.
					 * Note: The function can be used before finalize, but the value returned
					 * may differ after calling finalize, meaning finalize will invalidate any
					 * prior returned values and they must be refreshed.
					 *
					 * @param faceLabel The face label
					 * @param faceID A pointer to the location where the numeric face ID will be stored
					 *
					 * @tparam I The type of the indexing scheme (integer based)
					 * @tparam T The type of the stored array data
					 *
					 * @return An error status indicating the success or failure of the operation
					 * @retval cupcfd::error::E_SUCCESS Success
					 */
					void getFaceID(L faceLabel, I * faceID);

					/**
					 * Retrieve the local face ID on this MPI rank for the given face label.
					 * Note: The function can be used before finalize, but the value returned
					 * may differ after calling finalize, meaning finalize will invalidate any
					 * prior returned values and they must be refreshed.
					 *
					 * @param faceLabel The face label
					 *
					 * @tparam I The type of the indexing scheme (integer based)
					 * @tparam T The type of the stored array data
					 *
					 * @return The face ID
					 */
					__attribute__((warn_unused_result))
					I getFaceID(L faceLabel);

					// === Boundary Operators ===

					/**
					 * Get the local face ID in the mesh associated with this boundary
					 *
					 * @param boundaryID The local boundary ID in the mesh to get the value from
					 * @param faceID A pointer to where the result will be stored
					 *
					 * @tparam I The type of the indexing scheme (integer based)
					 * @tparam T The type of the stored array data
					 *
					 * @return An error status indicating the success or failure of the operation
					 * @retval cupcfd::error::E_SUCCESS Success
					 */
					void getBoundaryFaceID(I boundaryID, I * faceID);

					/**
					 * Get the local face ID in the mesh associated with this boundary
					 *
					 * @param boundaryID The local boundary ID in the mesh to get the value from
					 *
					 * @tparam I The type of the indexing scheme (integer based)
					 * @tparam T The type of the stored array data
					 *
					 * @return The local face ID associated with this local boundary ID.
					 */
					__attribute__((warn_unused_result))
					I getBoundaryFaceID(I boundaryID);

					/**
					 * Get the local vertex ID in the mesh of a vertex associated with the boundary
					 *
					 * @param boundaryID The local boundary ID in the mesh to get the value from
					 * @param boundaryVertexID An identifier of which vertex of the boundary (0->nBoundaryVertices) to get the local vertex ID of
					 * @param vertexID A pointer to the where the result will be stored
					 *
					 * @tparam I The type of the indexing scheme (integer based)
					 * @tparam T The type of the stored array data
					 *
					 * @return An error status indicating the success or failure of the operation
					 * @retval cupcfd::error::E_SUCCESS Success
					 */
					void getBoundaryVertex(I boundaryID, I boundaryVertexID, I * vertexID);

					/**
					 * Get the local vertex ID in the mesh of a vertex associated with the boundary
					 *
					 * @param boundaryID The local boundary ID in the mesh to get the value from
					 * @param boundaryVertexID An identifier of which vertex of the boundary (0->nBoundaryVertices) to get the local vertex ID of
					 *
					 * @tparam I The type of the indexing scheme (integer based)
					 * @tparam T The type of the stored array data
					 *
					 * @return The local vertex ID in the mesh
					 */
					__attribute__((warn_unused_result))
					I getBoundaryVertex(I boundaryID, I boundaryVertexID);

					/**
					 * Get the stored boundary distance
					 *
					 * @param boundaryID The local boundary ID in the mesh to get the value from
					 * @param distance A pointer to the location where the result will be stored
					 *
					 * @tparam I The type of the indexing scheme (integer based)
					 * @tparam T The type of the stored array data
					 *
					 * @return An error status indicating the success or failure of the operation
					 * @retval cupcfd::error::E_SUCCESS Success
					 */
					void getBoundaryDistance(I boundaryID, T * distance);

					/**
					 * Get the stored boundary distance
					 *
					 * @param boundaryID The local boundary ID in the mesh to get the value from
					 *
					 * @tparam I The type of the indexing scheme (integer based)
					 * @tparam T The type of the stored array data
					 *
					 * @return The stored boundary distance
					 */
					__attribute__((warn_unused_result))
					T getBoundaryDistance(I boundaryID);

					/**
					 * Get the stored boundary yplus
					 *
					 * @param boundaryID The local boundary ID in the mesh to get the value from
					 * @param yPlus A pointer to the location where the result will be stored
					 *
					 * @tparam I The type of the indexing scheme (integer based)
					 * @tparam T The type of the stored array data
					 *
					 * @return An error status indicating the success or failure of the operation
					 * @retval cupcfd::error::E_SUCCESS Success
					 */
					void getBoundaryYPlus(I boundaryID, T * yPlus);

					/**
					 * Get the stored boundary yplus
					 *
					 * @param boundaryID The local boundary ID in the mesh to get the value from
					 *
					 * @tparam I The type of the indexing scheme (integer based)
					 * @tparam T The type of the stored array data
					 *
					 * @return The stored boundary yplus
					 */
					__attribute__((warn_unused_result))
					T getBoundaryYPlus(I boundaryID);

					/**
					 * Get the stored boundary uplus
					 *
					 * @param boundaryID The local boundary ID in the mesh to get the value from
					 * @param uPlus A pointer to the location where the result will be stored
					 *
					 * @tparam I The type of the indexing scheme (integer based)
					 * @tparam T The type of the stored array data
					 *
					 * @return An error status indicating the success or failure of the operation
					 * @retval cupcfd::error::E_SUCCESS Success
					 */
					void getBoundaryUPlus(I boundaryID, T * uPlus);

					/**
					 * Get the stored boundary uplus
					 *
					 * @param boundaryID The local boundary ID in the mesh to get the value from
					 *
					 * @tparam I The type of the indexing scheme (integer based)
					 * @tparam T The type of the stored array data
					 *
					 * @return The stored boundary uplus
					 */
					__attribute__((warn_unused_result))
					T getBoundaryUPlus(I boundaryID);

					/**
					 * Get the local region id in the mesh of the associated region of this boundary
					 *
					 * @param boundaryID The local boundary ID in the mesh to get the value from
					 * @param regionID A pointer to the location where the result will be stored
					 *
					 * @tparam I The type of the indexing scheme (integer based)
					 * @tparam T The type of the stored array data
					 *
					 * @return An error status indicating the success or failure of the operation
					 * @retval cupcfd::error::E_SUCCESS Success
					 */
					void getBoundaryRegionID(I boundaryID, I * regionID);

					/**
					 * Get the local region id in the mesh of the associated region of this boundary
					 *
					 * @param boundaryID The local boundary ID in the mesh to get the value from
					 *
					 * @tparam I The type of the indexing scheme (integer based)
					 * @tparam T The type of the stored array data
					 *
					 * @return The local region ID in the mesh associated with this boundary
					 */
					__attribute__((warn_unused_result))
					I getBoundaryRegionID(I boundaryID);

					/**
					 * Get the stored boundary shear
					 *
					 * @param boundaryID The local boundary ID in the mesh to get the value from
					 * @param shear A pointer to the location where the result will be stored
					 *
					 * @tparam I The type of the indexing scheme (integer based)
					 * @tparam T The type of the stored array data
					 *
					 * @return An error status indicating the success or failure of the operation
					 * @retval cupcfd::error::E_SUCCESS Success
					 */
					void getBoundaryShear(I boundaryID, euc::EuclideanVector<T,3>& shear);

					/**
					 * Get the stored boundary shear
					 *
					 * @param boundaryID The local boundary ID in the mesh to get the value from
					 *
					 * @tparam I The type of the indexing scheme (integer based)
					 * @tparam T The type of the stored array data
					 *
					 * @return The stored boundary shear
					 */
					__attribute__((warn_unused_result))
					euc::EuclideanVector<T,3> getBoundaryShear(I boundaryID);

					/**
					 * Get the stored boundary Q
					 *
					 * @param boundaryID The local boundary ID in the mesh to get the value from
					 * @param q A pointer to the location where the result will be stored
					 *
					 * @tparam I The type of the indexing scheme (integer based)
					 * @tparam T The type of the stored array data
					 *
					 * @return An error status indicating the success or failure of the operation
					 * @retval cupcfd::error::E_SUCCESS Success
					 */
					void getBoundaryQ(I boundaryID, T * q);

					/**
					 * Get the stored boundary Q
					 *
					 * @param boundaryID The local boundary ID in the mesh to get the value from
					 *
					 * @tparam I The type of the indexing scheme (integer based)
					 * @tparam T The type of the stored array data
					 *
					 * @return The stored boundary Q
					 */
					__attribute__((warn_unused_result))
					T getBoundaryQ(I boundaryID);

					/**
					 * Get the stored boundary H
					 *
					 * @param boundaryID The local boundary ID in the mesh to get the value from
					 * @param h A pointer to the location where the result will be stored
					 *
					 * @tparam I The type of the indexing scheme (integer based)
					 * @tparam T The type of the stored array data
					 *
					 * @return An error status indicating the success or failure of the operation
					 * @retval cupcfd::error::E_SUCCESS Success
					 */
					void getBoundaryH(I boundaryID, T * h);

					/**
					 * Get the stored boundary H
					 *
					 * @param boundaryID The local boundary ID in the mesh to get the value from
					 *
					 * @tparam I The type of the indexing scheme (integer based)
					 * @tparam T The type of the stored array data
					 *
					 * @return The stored boundary H
					 */
					__attribute__((warn_unused_result))
					T getBoundaryH(I boundaryID);

					/**
					 * Get the stored boundary T
					 *
					 * @param boundaryID The local boundary ID in the mesh to get the value from
					 * @param t A pointer to the location where the result will be stored
					 *
					 * @tparam I The type of the indexing scheme (integer based)
					 * @tparam T The type of the stored array data
					 *
					 * @return An error status indicating the success or failure of the operation
					 * @retval cupcfd::error::E_SUCCESS Success
					 */
					void getBoundaryT(I boundaryID, T * t);

					/**
					 * Get the stored boundary T
					 *
					 * @param boundaryID The local boundary ID in the mesh to get the value from
					 *
					 * @tparam I The type of the indexing scheme (integer based)
					 * @tparam T The type of the stored array data
					 *
					 * @return The stored boundary T
					 */
					__attribute__((warn_unused_result))
					T getBoundaryT(I boundaryID);


					/**
					 * Set the local face ID in the mesh associated with this boundary
					 *
					 * @param boundaryID The local boundary ID in the mesh to get the value from
					 * @param faceID The value to set the boundary face ID to
					 *
					 * @tparam I The type of the indexing scheme (integer based)
					 * @tparam T The type of the stored array data
					 *
					 * @return An error status indicating the success or failure of the operation
					 * @retval cupcfd::error::E_SUCCESS Success
					 */
					void setBoundaryFaceID(I boundaryID, I faceID);

					/**
					 * Set the local vertex ID in the mesh of a vertex associated with this boundary
					 *
					 * @param boundaryID The local boundary ID in the mesh to get the value from
					 * @param boundaryVertexID An identifier of which vertex with the boundary to update (0-> number of boundary vertices)
					 * @param vertexID The local vertex ID in the mesh to set the boudnary vertex ID to
					 *
					 * @tparam I The type of the indexing scheme (integer based)
					 * @tparam T The type of the stored array data
					 *
					 * @return An error status indicating the success or failure of the operation
					 * @retval cupcfd::error::E_SUCCESS Success
					 */
					void setBoundaryVertex(I boundaryID, I boundaryVertexID, I vertexID);

					/**
					 * Set the stored boundary distance
					 *
					 * @param boundaryID The local boundary ID in the mesh to get the value from
					 * @param distance The value to set the stored boundary distance to
					 *
					 * @tparam I The type of the indexing scheme (integer based)
					 * @tparam T The type of the stored array data
					 *
					 * @return An error status indicating the success or failure of the operation
					 * @retval cupcfd::error::E_SUCCESS Success
					 */
					void setBoundaryDistance(I boundaryID, T distance);

					/**
					 * Set the stored boundary yplus
					 *
					 * @param boundaryID The local boundary ID in the mesh to get the value from
					 * @param The value to set the stored boundary yplus to
					 *
					 * @tparam I The type of the indexing scheme (integer based)
					 * @tparam T The type of the stored array data
					 *
					 * @return An error status indicating the success or failure of the operation
					 * @retval cupcfd::error::E_SUCCESS Success
					 */
					void setBoundaryYPlus(I boundaryID, T yPlus);

					/**
					 * Set the stored boundary uplus
					 *
					 * @param boundaryID The local boundary ID in the mesh to get the value from
					 * @param The value to set the stored boundary upus to
					 *
					 * @tparam I The type of the indexing scheme (integer based)
					 * @tparam T The type of the stored array data
					 *
					 * @return An error status indicating the success or failure of the operation
					 * @retval cupcfd::error::E_SUCCESS Success
					 */
					void setBoundaryUPlus(I boundaryID, T uPlus);

					/**
					 * Set the local region ID in the mesh associated with this boundary
					 *
					 * @param boundaryID The local boundary ID in the mesh to get the value from
					 * @param regionID The value to set the stored region ID to
					 *
					 * @tparam I The type of the indexing scheme (integer based)
					 * @tparam T The type of the stored array data
					 *
					 * @return An error status indicating the success or failure of the operation
					 * @retval cupcfd::error::E_SUCCESS Success
					 */
					void setBoundaryRegionID(I boundaryID, I regionID);

					/**
					 * Set the stored boundary shear
					 *
					 * @param boundaryID The local boundary ID in the mesh to get the value from
					 * @param shear The vector to set the stored shear vector to
					 *
					 * @tparam I The type of the indexing scheme (integer based)
					 * @tparam T The type of the stored array data
					 *
					 * @return An error status indicating the success or failure of the operation
					 * @retval cupcfd::error::E_SUCCESS Success
					 */
					void setBoundaryShear(I boundaryID, euc::EuclideanVector<T,3>& shear);

					/**
					 * Set the stored boundary Q
					 *
					 * @param boundaryID The local boundary ID in the mesh to get the value from
					 * @param q The value to set the stored q to
					 *
					 * @tparam I The type of the indexing scheme (integer based)
					 * @tparam T The type of the stored array data
					 *
					 * @return An error status indicating the success or failure of the operation
					 * @retval cupcfd::error::E_SUCCESS Success
					 */
					void setBoundaryQ(I boundaryID, T q);

					/**
					 * Set the stored boundary H
					 *
					 * @param boundaryID The local boundary ID in the mesh to get the value from
					 * @param h The value to set the stored h value to
					 *
					 * @tparam I The type of the indexing scheme (integer based)
					 * @tparam T The type of the stored array data
					 *
					 * @return An error status indicating the success or failure of the operation
					 * @retval cupcfd::error::E_SUCCESS Success
					 */
					void setBoundaryH(I boundaryID, T h);

					/**
					 * Set the stored boundary T
					 *
					 * @param boundaryID The local boundary ID in the mesh to get the value from
					 * @param t The value to set the stored t value to
					 *
					 * @tparam I The type of the indexing scheme (integer based)
					 * @tparam T The type of the stored array data
					 *
					 * @return An error status indicating the success or failure of the operation
					 * @retval cupcfd::error::E_SUCCESS Success
					 */
					void setBoundaryT(I boundaryID, T t);

					/**
					 * Add a boundary to the mesh on this rank.
					 *
					 * @param boundaryLabel A numeric face label for this boundary.
					 * This is a new, unique label not yet used with any boundary label.
					 * @param regionLabel An existing label of the region to be associated with this boundary.
					 * @param vertexLabels An array of the existing labels of vertices associated with this boundary. Must
					 * be a minimum of 3, maximum of 4.
					 * Important - these vertices must be listed in order of connecting edges - i.e. vertexLabels[0]
					 * connects to vertexLabels[1], 1 to 2, 2 to 3.... till n-1 connects to 0
					 * @param nVertexLabels The number of labels in vertexLabels (minimum of 3, maximum of 4)
					 * @param distance The boundary distance
					 *
					 * @tparam I The type of the indexing scheme (integer based)
					 * @tparam T The type of the stored array data
					 *
					 * @return The local id of the added boundary in the unstructured mesh
					 */
					__attribute__((warn_unused_result))
					cupcfd::error::eCodes addBoundary(
							L boundaryLabel,
							L regionLabel,
							L * vertexLabels, I nVertexLabels,
							T distance);

					/**
					 * Add a boundary to the mesh on this rank. Boundary properties are set to defaults.
					 *
					 * @param boundaryLabel A numeric face label for this boundary.
					 * This is a new, unique label not yet used with any boundary label.
					 * @param regionLabel An existing label of the region to be associated with this boundary.
					 * @param vertexLabels An array of the existing labels of vertices associated with this boundary. Must
					 * be a minimum of 3, maximum of 4.
					 * @param nVertexLabels The number of labels in vertexLabels (minimum of 3, maximum of 4)
					 *
					 * @tparam I The type of the indexing scheme (integer based)
					 * @tparam T The type of the stored array data
					 *
					 * @return The local id of the added boundary in the unstructured mesh
					 */
					__attribute__((warn_unused_result))
					cupcfd::error::eCodes addBoundary(
							L boundaryLabel,
							L regionLabel,
							L * vertexLabels, I nVertexLabels);


					/**
					 * Retrieve the local boundary ID on this MPI rank for the given boundary label.
					 * Note: The function can be used before finalize, but the value returned
					 * may differ after calling finalize, meaning finalize will invalidate any
					 * prior returned values and they must be refreshed.
					 *
					 * @param boundaryLabel The boundary label
					 * @param boundaryID A pointer to the location where the numeric cell ID will be stored
					 *
					 * @tparam I The type of the indexing scheme (integer based)
					 * @tparam T The type of the stored array data
					 *
					 * @return An error status indicating the success or failure of the operation
					 * @retval cupcfd::error::E_SUCCESS Success
					 */
					void getBoundaryID(L boundaryLabel, I * boundaryID);

					/**
					 * Retrieve the local boundary ID on this MPI rank for the given boundary label.
					 * Note: The function can be used before finalize, but the value returned
					 * may differ after calling finalize, meaning finalize will invalidate any
					 * prior returned values and they must be refreshed.
					 *
					 * @param boundaryLabel The boundary label
					 *
					 * @tparam I The type of the indexing scheme (integer based)
					 * @tparam T The type of the stored array data
					 *
					 * @return The local cell ID for the given cell label
					 */
					__attribute__((warn_unused_result))
					I getBoundaryID(L boundaryLabel);


					// === Region Operators ===

					/**
					 * Get the stored region type
					 *
					 * @param regionID The local region ID in the mesh to get the value from
					 * @param type A pointer to the location where the result is stored
					 *
					 * @tparam I The type of the indexing scheme (integer based)
					 * @tparam T The type of the stored array data
					 *
					 * @return An error status indicating the success or failure of the operation
					 * @retval cupcfd::error::E_SUCCESS Success
					 */
					void getRegionType(I regionID, RType * type);

					/**
					 * Get the stored region type
					 *
					 * @param regionID The local region ID in the mesh to get the value from
					 *
					 * @tparam I The type of the indexing scheme (integer based)
					 * @tparam T The type of the stored array data
					 *
					 * @return The stored region type
					 */
					__attribute__((warn_unused_result))
					RType getRegionType(I regionID);

					/**
					 * Get the stored region std
					 *
					 * @param regionID The local region ID in the mesh to get the value from
					 * @param std A pointer to the location where the result is stored
					 *
					 * @tparam I The type of the indexing scheme (integer based)
					 * @tparam T The type of the stored array data
					 *
					 * @return An error status indicating the success or failure of the operation
					 * @retval cupcfd::error::E_SUCCESS Success
					 */
					void getRegionStd(I regionID, bool * std);

					/**
					 * Get the stored region std
					 *
					 * @param regionID The local region ID in the mesh to get the value from
					 *
					 * @tparam I The type of the indexing scheme (integer based)
					 * @tparam T The type of the stored array data
					 *
					 * @return The stored region std
					 */
					__attribute__((warn_unused_result))
					bool getRegionStd(I regionID);

					/**
					 * Get the stored region ylog
					 *
					 * @param regionID The local region ID in the mesh to get the value from
					 * @param yLog A pointer to the location where the result is stored
					 *
					 * @tparam I The type of the indexing scheme (integer based)
					 * @tparam T The type of the stored array data
					 *
					 * @return An error status indicating the success or failure of the operation
					 * @retval cupcfd::error::E_SUCCESS Success
					 */
					void getRegionYLog(I regionID, T * yLog);

					/**
					 * Get the stored region ylog
					 *
					 * @param regionID The local region ID in the mesh to get the value from
					 *
					 * @tparam I The type of the indexing scheme (integer based)
					 * @tparam T The type of the stored array data
					 *
					 * @return The stored region ylog
					 */
					__attribute__((warn_unused_result))
					T getRegionYLog(I regionID);

					/**
					 * Get the stored region elog
					 *
					 * @param regionID The local region ID in the mesh to get the value from
					 * @param eLog A pointer to the location where the result is stored
					 *
					 * @tparam I The type of the indexing scheme (integer based)
					 * @tparam T The type of the stored array data
					 *
					 * @return An error status indicating the success or failure of the operation
					 * @retval cupcfd::error::E_SUCCESS Success
					 */
					void getRegionELog(I regionID, T * eLog);

					/**
					 * Get the stored region elog
					 *
					 * @param regionID The local region ID in the mesh to get the value from
					 *
					 * @tparam I The type of the indexing scheme (integer based)
					 * @tparam T The type of the stored array data
					 *
					 * @return The stored region elog
					 */
					__attribute__((warn_unused_result))
					T getRegionELog(I regionID);

					/**
					 * Get the stored region density
					 *
					 * @param regionID The local region ID in the mesh to get the value from
					 * @param density A pointer to the location where the result is stored
					 *
					 * @tparam I The type of the indexing scheme (integer based)
					 * @tparam T The type of the stored array data
					 *
					 * @return An error status indicating the success or failure of the operation
					 * @retval cupcfd::error::E_SUCCESS Success
					 */
					void getRegionDensity(I regionID, T * density);

					/**
					 * Get the stored region density
					 *
					 * @param regionID The local region ID in the mesh to get the value from
					 *
					 * @tparam I The type of the indexing scheme (integer based)
					 * @tparam T The type of the stored array data
					 *
					 * @return The stored region density
					 */
					__attribute__((warn_unused_result))
					T getRegionDensity(I regionID);

					/**
					 * Get the stored region turbke
					 *
					 * @param regionID The local region ID in the mesh to get the value from
					 * @param turbKE A pointer to the location where the result is stored
					 *
					 * @tparam I The type of the indexing scheme (integer based)
					 * @tparam T The type of the stored array data
					 *
					 * @return An error status indicating the success or failure of the operation
					 * @retval cupcfd::error::E_SUCCESS Success
					 */
					void getRegionTurbKE(I regionID, T * turbKE);

					/**
					 * Get the stored region turbke
					 *
					 * @param regionID The local region ID in the mesh to get the value from
					 *
					 * @tparam I The type of the indexing scheme (integer based)
					 * @tparam T The type of the stored array data
					 *
					 * @return The stored region turbke
					 */
					__attribute__((warn_unused_result))
					T getRegionTurbKE(I regionID);

					/**
					 * Get the stored region turbdiss
					 *
					 * @param regionID The local region ID in the mesh to get the value from
					 * @param turbDiss A pointer to the location where the result is stored
					 *
					 * @tparam I The type of the indexing scheme (integer based)
					 * @tparam T The type of the stored array data
					 *
					 * @return An error status indicating the success or failure of the operation
					 * @retval cupcfd::error::E_SUCCESS Success
					 */
					void getRegionTurbDiss(I regionID, T * turbDiss);

					/**
					 * Get the stored region turbdiss
					 *
					 * @param regionID The local region ID in the mesh to get the value from
					 *
					 * @tparam I The type of the indexing scheme (integer based)
					 * @tparam T The type of the stored array data
					 *
					 * @return The stored region turbDiss
					 */
					__attribute__((warn_unused_result))
					T getRegionTurbDiss(I regionID);

					/**
					 * Get the stored region splvl
					 *
					 * @param regionID The local region ID in the mesh to get the value from
					 * @param splvl A pointer to the location where the result is stored
					 *
					 * @tparam I The type of the indexing scheme (integer based)
					 * @tparam T The type of the stored array data
					 *
					 * @return An error status indicating the success or failure of the operation
					 * @retval cupcfd::error::E_SUCCESS Success
					 */
					void getRegionSplvl(I regionID, T * splvl);

					/**
					 * Get the stored region splvl
					 *
					 * @param regionID The local region ID in the mesh to get the value from
					 *
					 * @tparam I The type of the indexing scheme (integer based)
					 * @tparam T The type of the stored array data
					 *
					 * @return The stored region splvl
					 */
					__attribute__((warn_unused_result))
					T getRegionSplvl(I regionID);

					/**
					 * Get the stored region den
					 *
					 * @param regionID The local region ID in the mesh to get the value from
					 * @param den A pointer to the location where the result is stored
					 *
					 * @tparam I The type of the indexing scheme (integer based)
					 * @tparam T The type of the stored array data
					 *
					 * @return An error status indicating the success or failure of the operation
					 * @retval cupcfd::error::E_SUCCESS Success
					 */
					void getRegionDen(I regionID, T * den);

					/**
					 * Get the stored region den
					 *
					 * @param regionID The local region ID in the mesh to get the value from
					 *
					 * @tparam I The type of the indexing scheme (integer based)
					 * @tparam T The type of the stored array data
					 *
					 * @return The stored region den
					 */
					__attribute__((warn_unused_result))
					T getRegionDen(I regionID);

					/**
					 * Get the stored region force tangent
					 *
					 * @param regionID The local region ID in the mesh to get the value from
					 * @param forceTangent A pointer to the location where the result is stored
					 *
					 * @tparam I The type of the indexing scheme (integer based)
					 * @tparam T The type of the stored array data
					 *
					 * @return An error status indicating the success or failure of the operation
					 * @retval cupcfd::error::E_SUCCESS Success
					 */
					void getRegionForceTangent(I regionID, euc::EuclideanVector<T,3>& forceTangent);

					/**
					 * Get the stored region force tangent
					 *
					 * @param regionID The local region ID in the mesh to get the value from
					 *
					 * @tparam I The type of the indexing scheme (integer based)
					 * @tparam T The type of the stored array data
					 *
					 * @return The stored region tangent
					 */
					__attribute__((warn_unused_result))
					euc::EuclideanVector<T,3> getRegionForceTangent(I regionID);

					/**
					 * Get the stored region uvw
					 *
					 * @param regionID The local region ID in the mesh to get the value from
					 * @param uvw A pointer to the location where the result is stored
					 *
					 * @tparam I The type of the indexing scheme (integer based)
					 * @tparam T The type of the stored array data
					 *
					 * @return An error status indicating the success or failure of the operation
					 * @retval cupcfd::error::E_SUCCESS Success
					 */
					void getRegionUVW(I regionID, euc::EuclideanVector<T,3>& uvw);

					/**
					 * Get the stored region uvw
					 *
					 * @param regionID The local region ID in the mesh to get the value from
					 *
					 * @tparam I The type of the indexing scheme (integer based)
					 * @tparam T The type of the stored array data
					 *
					 * @return The stored region uvw
					 */
					__attribute__((warn_unused_result))
					euc::EuclideanVector<T,3> getRegionUVW(I regionID);

					/**
					 * Get the stored region name
					 *
					 * @param regionID The local region ID in the mesh to get the value from
					 * @param regionName A pointer to the location where the result is stored
					 *
					 * @tparam I The type of the indexing scheme (integer based)
					 * @tparam T The type of the stored array data
					 *
					 * @return An error status indicating the success or failure of the operation
					 * @retval cupcfd::error::E_SUCCESS Success
					 */
					void getRegionName(I regionID, std::string& regionName);

					/**
					 * Get the stored region name
					 *
					 * @param regionID The local region ID in the mesh to get the value from
					 *
					 * @tparam I The type of the indexing scheme (integer based)
					 * @tparam T The type of the stored array data
					 *
					 * @return The stored region name
					 */
					__attribute__((warn_unused_result))
					std::string getRegionName(I regionID);

					/**
					 * Get the stored region flux
					 *
					 * @param regionID The local region ID in the mesh to get the value from
					 * @param flux A pointer to the location where the result is stored
					 *
					 * @tparam I The type of the indexing scheme (integer based)
					 * @tparam T The type of the stored array data
					 *
					 * @return An error status indicating the success or failure of the operation
					 * @retval cupcfd::error::E_SUCCESS Success
					 */
					void getRegionFlux(I regionID, bool * flux);

					/**
					 * Get the stored region flux
					 *
					 * @param regionID The local region ID in the mesh to get the value from
					 *
					 * @tparam I The type of the indexing scheme (integer based)
					 * @tparam T The type of the stored array data
					 *
					 * @return The stored region flux
					 */
					__attribute__((warn_unused_result))
					bool getRegionFlux(I regionID);

					/**
					 * Get the stored region adiab
					 *
					 * @param regionID The local region ID in the mesh to get the value from
					 * @param adiab A pointer to the location where the result is stored
					 *
					 * @tparam I The type of the indexing scheme (integer based)
					 * @tparam T The type of the stored array data
					 *
					 * @return An error status indicating the success or failure of the operation
					 * @retval cupcfd::error::E_SUCCESS Success
					 */
					void getRegionAdiab(I regionID, bool * adiab);

					/**
					 * Get the stored region adiab
					 *
					 * @param regionID The local region ID in the mesh to get the value from
					 *
					 * @tparam I The type of the indexing scheme (integer based)
					 * @tparam T The type of the stored array data
					 *
					 * @return The stored region adaib
					 */
					__attribute__((warn_unused_result))
					bool getRegionAdiab(I regionID);

					/**
					 * Get the stored region R
					 *
					 * @param regionID The local region ID in the mesh to get the value from
					 * @param t A pointer to the location where the result is stored
					 *
					 * @tparam I The type of the indexing scheme (integer based)
					 * @tparam T The type of the stored array data
					 *
					 * @return An error status indicating the success or failure of the operation
					 * @retval cupcfd::error::E_SUCCESS Success
					 */
					void getRegionR(I regionID, T * r);

					/**
					 * Get the stored region R
					 *
					 * @param regionID The local region ID in the mesh to get the value from
					 *
					 * @tparam I The type of the indexing scheme (integer based)
					 * @tparam T The type of the stored array data
					 *
					 * @return The stored region R
					 */
					__attribute__((warn_unused_result))
					T getRegionR(I regionID);

					/**
					 * Get the stored region T
					 *
					 * @param regionID The local region ID in the mesh to get the value from
					 * @param t A pointer to the location where the result is stored
					 *
					 * @tparam I The type of the indexing scheme (integer based)
					 * @tparam T The type of the stored array data
					 *
					 * @return An error status indicating the success or failure of the operation
					 * @retval cupcfd::error::E_SUCCESS Success
					 */
					void getRegionT(I regionID, T * t);

					/**
					 * Get the stored region T
					 *
					 * @param regionID The local region ID in the mesh to get the value from
					 *
					 * @tparam I The type of the indexing scheme (integer based)
					 * @tparam T The type of the stored array data
					 *
					 * @return The stored region T
					 */
					__attribute__((warn_unused_result))
					T getRegionT(I regionID);


					/**
					 * Set the stored region type
					 *
					 * @param regionID The local region ID in the mesh to set the value of
					 * @param type The value to set the stored value to
					 *
					 * @tparam I The type of the indexing scheme (integer based)
					 * @tparam T The type of the stored array data
					 *
					 * @return An error status indicating the success or failure of the operation
					 * @retval cupcfd::error::E_SUCCESS Success
					 */
					void setRegionType(I regionID, RType type);

					/**
					 * Set the stored region std
					 *
					 * @param regionID The local region ID in the mesh to set the value of
					 * @param std The value to set the stored value to
					 *
					 * @tparam I The type of the indexing scheme (integer based)
					 * @tparam T The type of the stored array data
					 *
					 * @return An error status indicating the success or failure of the operation
					 * @retval cupcfd::error::E_SUCCESS Success
					 */
					void setRegionStd(I regionID, bool std);

					/**
					 * Set the stored region ylog
					 *
					 * @param regionID The local region ID in the mesh to set the value of
					 * @param yLog The value to set the stored value to
					 *
					 * @tparam I The type of the indexing scheme (integer based)
					 * @tparam T The type of the stored array data
					 *
					 * @return An error status indicating the success or failure of the operation
					 * @retval cupcfd::error::E_SUCCESS Success
					 */
					void setRegionYLog(I regionID, T yLog);

					/**
					 * Set the stored region elog
					 *
					 * @param regionID The local region ID in the mesh to set the value of
					 * @param eLog The value to set the stored value to
					 *
					 * @tparam I The type of the indexing scheme (integer based)
					 * @tparam T The type of the stored array data
					 *
					 * @return An error status indicating the success or failure of the operation
					 * @retval cupcfd::error::E_SUCCESS Success
					 */
					void setRegionELog(I regionID, T eLog);

					/**
					 * Set the stored region density
					 *
					 * @param regionID The local region ID in the mesh to set the value of
					 * @param density The value to set the stored value to
					 *
					 * @tparam I The type of the indexing scheme (integer based)
					 * @tparam T The type of the stored array data
					 *
					 * @return An error status indicating the success or failure of the operation
					 * @retval cupcfd::error::E_SUCCESS Success
					 */
					void setRegionDensity(I regionID, T density);

					/**
					 * Set the stored region turbke
					 *
					 * @param regionID The local region ID in the mesh to set the value of
					 * @param turbKE The value to set the stored value to
					 *
					 * @tparam I The type of the indexing scheme (integer based)
					 * @tparam T The type of the stored array data
					 *
					 * @return An error status indicating the success or failure of the operation
					 * @retval cupcfd::error::E_SUCCESS Success
					 */
					void setRegionTurbKE(I regionID, T turbKE);

					/**
					 * Set the stored region turbdiss
					 *
					 * @param regionID The local region ID in the mesh to set the value of
					 * @param turbDiss The value to set the stored value to
					 *
					 * @tparam I The type of the indexing scheme (integer based)
					 * @tparam T The type of the stored array data
					 *
					 * @return An error status indicating the success or failure of the operation
					 * @retval cupcfd::error::E_SUCCESS Success
					 */
					void setRegionTurbDiss(I regionID, T turbDiss);

					/**
					 * Set the stored region splvl
					 *
					 * @param regionID The local region ID in the mesh to set the value of
					 * @param splvl The value to set the stored value to
					 *
					 * @tparam I The type of the indexing scheme (integer based)
					 * @tparam T The type of the stored array data
					 *
					 * @return An error status indicating the success or failure of the operation
					 * @retval cupcfd::error::E_SUCCESS Success
					 */
					void setRegionSplvl(I regionID, T splvl);

					/**
					 * Set the stored region den
					 *
					 * @param regionID The local region ID in the mesh to set the value of
					 * @param den The value to set the stored value to
					 *
					 * @tparam I The type of the indexing scheme (integer based)
					 * @tparam T The type of the stored array data
					 *
					 * @return An error status indicating the success or failure of the operation
					 * @retval cupcfd::error::E_SUCCESS Success
					 */
					void setRegionDen(I regionID, T den);

					/**
					 * Set the stored region force tangent
					 *
					 * @param regionID The local region ID in the mesh to set the value of
					 * @param forceTangent The value to set the stored value to
					 *
					 * @tparam I The type of the indexing scheme (integer based)
					 * @tparam T The type of the stored array data
					 *
					 * @return An error status indicating the success or failure of the operation
					 * @retval cupcfd::error::E_SUCCESS Success
					 */
					void setRegionForceTangent(I regionID, euc::EuclideanVector<T,3>& forceTangent);

					/**
					 * Set the stored region uvw
					 *
					 * @param regionID The local region ID in the mesh to set the value of
					 * @param uvw The value to set the stored value to
					 *
					 * @tparam I The type of the indexing scheme (integer based)
					 * @tparam T The type of the stored array data
					 *
					 * @return An error status indicating the success or failure of the operation
					 * @retval cupcfd::error::E_SUCCESS Success
					 */
					void setRegionUVW(I regionID, euc::EuclideanVector<T,3>& uvw);

					/**
					 * Set the stored region name
					 *
					 * @param regionID The local region ID in the mesh to set the value of
					 * @param regionName The value to set the stored value to
					 *
					 * @tparam I The type of the indexing scheme (integer based)
					 * @tparam T The type of the stored array data
					 *
					 * @return An error status indicating the success or failure of the operation
					 * @retval cupcfd::error::E_SUCCESS Success
					 */
					void setRegionName(I regionID, std::string& regionName);

					/**
					 * Set the stored region flux
					 *
					 * @param regionID The local region ID in the mesh to set the value of
					 * @param flux The value to set the stored value to
					 *
					 * @tparam I The type of the indexing scheme (integer based)
					 * @tparam T The type of the stored array data
					 *
					 * @return An error status indicating the success or failure of the operation
					 * @retval cupcfd::error::E_SUCCESS Success
					 */
					void setRegionFlux(I regionID, bool flux);

					/**
					 * Set the stored region adiab
					 *
					 * @param regionID The local region ID in the mesh to set the value of
					 * @param adiab The value to set the stored value to
					 *
					 * @tparam I The type of the indexing scheme (integer based)
					 * @tparam T The type of the stored array data
					 *
					 * @return An error status indicating the success or failure of the operation
					 * @retval cupcfd::error::E_SUCCESS Success
					 */
					void setRegionAdiab(I regionID, bool adiab);

					/**
					 * Set the stored region R of a region
					 *
					 * @param regionID The local region ID in the mesh to set the value of
					 * @param r The value to set the stored value to
					 *
					 * @tparam I The type of the indexing scheme (integer based)
					 * @tparam T The type of the stored array data
					 *
					 * @return An error status indicating the success or failure of the operation
					 * @retval cupcfd::error::E_SUCCESS Success
					 */
					void setRegionR(I regionID, T r);

					/**
					 * Set the stored region T of a region
					 *
					 * @param regionID The local region ID in the mesh to set the value of
					 * @param t The value to set the stored value to
					 *
					 * @tparam I The type of the indexing scheme (integer based)
					 * @tparam T The type of the stored array data
					 *
					 * @return An error status indicating the success or failure of the operation
					 * @retval cupcfd::error::E_SUCCESS Success
					 */
					void setRegionT(I regionID, T t);

					/**
					 * Add a region with a set type and name. All other characteristics of the region are set to defaults.
					 *
					 * @param regionLabel A numeric label for this region. This is a new, unique label not yet used with any region.
					 * @param type The region type
					 * @param regionName The region name
					 *
					 * @tparam I The type of the indexing scheme (integer based)
					 * @tparam T The type of the stored array data
					 *
					 * @return An error status indicating the success or failure of the operation
					 * @retval cupcfd::error::E_SUCCESS Success
					 */
					__attribute__((warn_unused_result))
					cupcfd::error::eCodes addRegion(
										L regionLabel,
										RType type,
										bool std,
										bool flux,
										bool adiab,
										T ylog,
										T elog,
										T density,
										T turbKE,
										T turbDiss,
										T splvl,
										T den,
										T r,
										T t,
										euc::EuclideanVector<T,3>& forceTangent,
										euc::EuclideanVector<T,3>& uvw,
										std::string& regionName);

					/**
					 * Add a region with a name a default type. All other characteristics of the region are set to defaults.
					 *
					 * @param regionLabel A numeric label for this region. This is a new, unique label not yet used with any region.
					 * @param regionName The region name
					 *
					 * @tparam I The type of the indexing scheme (integer based)
					 * @tparam T The type of the stored array data
					 *
					 * @return An error status indicating the success or failure of the operation
					 * @retval cupcfd::error::E_SUCCESS Success
					 */
					__attribute__((warn_unused_result))
					cupcfd::error::eCodes addRegion(
										L regionLabel,
										std::string& regionName);

					/**
					 * Retrieve the local region ID on this MPI rank for the given region label.
					 * Note: The function can be used before finalize, but the value returned
					 * may differ after calling finalize, meaning finalize will invalidate any
					 * prior returned values and they must be refreshed.
					 *
					 * @param regionLabel The region label
					 * @param regionID A pointer to the location where the local region ID will be stored
					 *
					 * @tparam I The type of the indexing scheme (integer based)
					 * @tparam T The type of the stored array data
					 *
					 * @return An error status indicating the success or failure of the operation
					 * @retval cupcfd::error::E_SUCCESS Success
					 */
					void getRegionID(L regionLabel, I * regionID);


					/**
					 * Retrieve the local region ID on this MPI rank for the given region label.
					 * Note: The function can be used before finalize, but the value returned
					 * may differ after calling finalize, meaning finalize will invalidate any
					 * prior returned values and they must be refreshed.
					 *
					 * @param regionLabel The region label
					 *
					 * @tparam I The type of the indexing scheme (integer based)
					 * @tparam T The type of the stored array data
					 *
					 * @return The local region ID for the given vertex label
					 */
					__attribute__((warn_unused_result))
					I getRegionID(L regionLabel);

					// === Vertex Operators ===

					/**
					 * Get the position of a vertex
					 *
					 * @param vertexID The local ID of the vertex in the mesh to get the position of
					 * @param pos A point whose values will be set to the position of the vertex
					 *
					 * @tparam I The type of the indexing scheme (integer based)
					 * @tparam T The type of the stored array data
					 *
					 * @return An error status indicating the success or failure of the operation
					 * @retval cupcfd::error::E_SUCCESS Success
					 */
					void getVertexPos(I vertexID, euc::EuclideanPoint<T,3>& pos);

					/**
					 * Get the position of a vertex
					 *
					 * @param vertexID The local ID of the vertex in the mesh to get the position of
					 *
					 * @tparam I The type of the indexing scheme (integer based)
					 * @tparam T The type of the stored array data
					 *
					 * @return The vertex position
					 */
					__attribute__((warn_unused_result))
					euc::EuclideanPoint<T,3> getVertexPos(I vertexID);

					/**
					 * Set the position of a vertex
					 *
					 * @param vertexID The local ID of the vertex in the mesh to set the position of
					 * @param pos The point to set the stored position of the vertex to
					 *
					 * @tparam I The type of the indexing scheme (integer based)
					 * @tparam T The type of the stored array data
					 *
					 * @return An error status indicating the success or failure of the operation
					 * @retval cupcfd::error::E_SUCCESS Success
					 */
					void setVertexPos(I vertexLabel, euc::EuclideanPoint<T,3>& pos);


					/**
					 * Adds a vertex to the mesh on this rank
					 *
					 * @param vertexLabel A numeric label for this vertex. This is a new, unique label not yet used with any vertex.
					 * @param pos The position of the vertex
					 *
					 * @tparam I The type of the indexing scheme (integer based)
					 * @tparam T The type of the stored array data
					 *
					 * @return An error status indicating the success or failure of the operation
					 * @retval cupcfd::error::E_SUCCESS Success
					 */
					__attribute__((warn_unused_result))
					cupcfd::error::eCodes addVertex(L vertexLabel, euc::EuclideanPoint<T,3>& pos);


					/**
					 * Retrieve the local vertex ID on this MPI rank for the given vertex label.
					 * Note: The function can be used before finalize, but the value returned
					 * may differ after calling finalize, meaning finalize will invalidate any
					 * prior returned values and they must be refreshed.
					 *
					 * @param vertexLabel The vertex label
					 * @param vertexID A pointer to the location where the local vertex ID will be stored
					 *
					 * @tparam I The type of the indexing scheme (integer based)
					 * @tparam T The type of the stored array data
					 *
					 * @return An error status indicating the success or failure of the operation
					 * @retval cupcfd::error::E_SUCCESS Success
					 */
					void getVertexID(L vertexLabel, I * vertexID);

					/**
					 * Retrieve the local vertex ID on this MPI rank for the given vertex label.
					 * Note: The function can be used before finalize, but the value returned
					 * may differ after calling finalize, meaning finalize will invalidate any
					 * prior returned values and they must be refreshed.
					 *
					 * @param vertexLabel The vertex label
					 *
					 * @tparam I The type of the indexing scheme (integer based)
					 * @tparam T The type of the stored array data
					 *
					 * @return The local vertex ID for the given vertex label
					 */
					__attribute__((warn_unused_result))
					I getVertexID(L vertexLabel);

					// === Other ===

					/**
					 * Utility: Get the cell Polyhedron type.
					 *
					 * Used to discover which polyhedron type should be used for buildPolyhedron().
					 *
					 * Can be used for ghost cells too (since the geometry number of vertices and faces
					 * are known) but since some of the geometry data may not be stored locally, it is
					 * not guaranteed that ghost cells can be built into Polyhedron objects.
					 *
					 * @param localID Cell Local ID
					 *
					 * @return An identifier for the Polyhedron type
					 */
					shapes::PolyhedronType getCellPolyhedronType(I cellID);

					/**
					 * Builds a TriPrism Polyhedron object from a local cell ID.
					 *
					 * Ideally we can operate on the mesh geometry data directly to avoid unnecessary loads/copies,
					 * but this function is provided so that polyhedron geometry can be recreated for computing
					 * geometric properties such as area, volume etc etc. This may be of use in setup phases
					 * of a simulation where mesh properties needed to be computed.
					 *
					 * Caveats:
					 *
					 * (a) Can only be done for locally owned cells (we are likely missing some geometry data
					 * for ghost cells if it doesn't connect to a local cell)
					 *
					 * (b) The Polyhedron class is designed around CRTP. This means that the type of polyhedron
					 * must be known at call time of this function (getCellPolyhedronType can
					 * be used for this). Behaviour is unknown if the incorrect variant is called for a cell
					 * with a mismatching type.
					 *
					 * (c) Does not guarantee a particular orientation of Polyhedron (i.e. what constistutes top,
					 * bottom, front back etc). The vertexes are correct relative to their positions however.
					 *
					 * @param cellID The local ID for the cell
					 * @param shape A pointer to where the pointer for the newly created object will be stored.
					 *
					 * @return An error status indicating the success or failure of the operation
					 * @retval cupcfd::error::E_SUCCESS Success
					 * @retval cupcfd::error::E_GEOMETRY_POLYHEDRON_MISMATCH The cell is of a different type to
					 * the requested polyhedron
					 * @retval cupcfd::error::E_GEOMETRY_NFACE_MISMATCH One or more parts has an unexpected mismatch in
					 * the face count (e.g. total, number of triangular etc)
					 * @retval cupcfd::error::E_GEOMETRY_NVERT_MISMATCH There was a mismatch between the expected
					 * and actual number of vertices
					 */
					__attribute__((warn_unused_result))
					cupcfd::error::eCodes buildPolyhedron(I cellID, shapes::TriPrism<T> ** shape);

					/**
					 * Builds a QuadPyramid Polyhedron object from a local cell ID.
					 *
					 * Ideally we can operate on the mesh geometry data directly to avoid unnecessary loads/copies,
					 * but this function is provided so that polyhedron geometry can be recreated for computing
					 * geometric properties such as area, volume etc etc. This may be of use in setup phases
					 * of a simulation where mesh properties needed to be computed.
					 *
					 * Caveats:
					 *
					 * (a) Can only be done for locally owned cells (we are likely missing some geometry data
					 * for ghost cells if it doesn't connect to a local cell)
					 *
					 * (b) The Polyhedron class is designed around CRTP. This means that the type of polyhedron
					 * must be known at call time of this function (getCellPolyhedronType can
					 * be used for this). Behaviour is unknown if the incorrect variant is called for a cell
					 * with a mismatching type.
					 *
					 * (c) Does not guarantee a particular orientation of Polyhedron (i.e. what constistutes top,
					 * bottom, front back etc). The vertexes are correct relative to their positions however.
					 *
					 * @param cellID The local ID for the cell
					 * @param shape A pointer to where the pointer for the newly created object will be stored.
					 *
					 * @return An error status indicating the success or failure of the operation
					 * @retval cupcfd::error::E_SUCCESS Success
					 * @retval cupcfd::error::E_GEOMETRY_POLYHEDRON_MISMATCH The cell is of a different type to
					 * the requested polyhedron
					 */
					__attribute__((warn_unused_result))
					cupcfd::error::eCodes buildPolyhedron(I cellID, shapes::QuadPyramid<T> ** shape);

					/**
					 * Builds a Tetrahedron Polyhedron object from a local cell ID.
					 *
					 * Ideally we can operate on the mesh geometry data directly to avoid unnecessary loads/copies,
					 * but this function is provided so that polyhedron geometry can be recreated for computing
					 * geometric properties such as area, volume etc etc. This may be of use in setup phases
					 * of a simulation where mesh properties needed to be computed.
					 *
					 * Caveats:
					 *
					 * (a) Can only be done for locally owned cells (we are likely missing some geometry data
					 * for ghost cells if it doesn't connect to a local cell)
					 *
					 * (b) The Polyhedron class is designed around CRTP. This means that the type of polyhedron
					 * must be known at call time of this function (getCellPolyhedronType can
					 * be used for this). Behaviour is unknown if the incorrect variant is called for a cell
					 * with a mismatching type.
					 *
					 * (c) Does not guarantee a particular orientation of Polyhedron (i.e. what constistutes top,
					 * bottom, front back etc). The vertexes are correct relative to their positions however.
					 *
					 * @param cellID The local ID for the cell
					 * @param shape A pointer to where the pointer for the newly created object will be stored.
					 *
					 * @return An error status indicating the success or failure of the operation
					 * @retval cupcfd::error::E_SUCCESS Success
					 * @retval cupcfd::error::E_GEOMETRY_POLYHEDRON_MISMATCH The cell is of a different type to
					 * the requested polyhedron
					 */
					__attribute__((warn_unused_result))
					cupcfd::error::eCodes buildPolyhedron(I cellID, shapes::Tetrahedron<T> ** shape);

					/**
					 * Builds a Hexahedron Polyhedron object from a local cell ID.
					 *
					 * Ideally we can operate on the mesh geometry data directly to avoid unnecessary loads/copies,
					 * but this function is provided so that polyhedron geometry can be recreated for computing
					 * geometric properties such as area, volume etc etc. This may be of use in setup phases
					 * of a simulation where mesh properties needed to be computed.
					 *
					 * Caveats:
					 *
					 * (a) Can only be done for locally owned cells (we are likely missing some geometry data
					 * for ghost cells if it doesn't connect to a local cell)
					 *
					 * (b) The Polyhedron class is designed around CRTP. This means that the type of polyhedron
					 * must be known at call time of this function (getCellPolyhedronType can
					 * be used for this). Behaviour is unknown if the incorrect variant is called for a cell
					 * with a mismatching type.
					 *
					 * (c) Does not guarantee a particular orientation of Polyhedron (i.e. what constistutes top,
					 * bottom, front back etc). The vertexes are correct relative to their positions however.
					 *
					 * @param cellID The local ID for the cell
					 * @param shape A pointer to where the pointer for the newly created object will be stored.
					 *
					 * @return An error status indicating the success or failure of the operation
					 * @retval cupcfd::error::E_SUCCESS Success
					 * @retval cupcfd::error::E_GEOMETRY_POLYHEDRON_MISMATCH The cell is of a different type to
					 * the requested polyhedron
					 */
					__attribute__((warn_unused_result))
					cupcfd::error::eCodes buildPolyhedron(I cellID, shapes::Hexahedron<T> ** shape);

					__attribute__((warn_unused_result))
					cupcfd::error::eCodes buildPolyhedronV2(I cellID, shapes::Hexahedron<T> ** shape);

					/**
					 * Sets up a mesh and populates it with data from a source of mesh data.
					 *
					 * Due to the finalization stage, all members of comm must call this constructor.
					 *
					 * @param data The souce of mesh data to load into the unstructured mesh
					 * @param assignedCellLabels The labels to load from the data source on this rank
					 * @param nAssignedCellLabels The number of elements of type L in cellLabels
					 *
					 * @tparam I The type of the indexing scheme (integer based)
					 * @tparam T The type of the stored array data
					 */
					__attribute__((warn_unused_result))
					cupcfd::error::eCodes addData(MeshSource<I,T,L>& data, L * assignedCellLabels, I nAssignedCellLabels);

					/**
					 * Finalize the construction of the mesh structure.
					 * This may trigger numerous internal computations and/or restructuring, and is required before the use
					 * of select operations from this class.
					 *
					 * After this is called, no further cells, regions, vertex, faces or boundaries may be added, although
					 * some associated data can be modified via the use of getters or setters. It can be expected that
					 * the mesh structure is in a consistent state ready for use.
					 *
					 * @tparam I The type of the indexing scheme (integer based)
					 * @tparam T The type of the stored array data
					 *
					 * @return An error status indicating the success or failure of the operation
					 * @retval cupcfd::error::E_SUCCESS Success
					 */
					__attribute__((warn_unused_result))
					cupcfd::error::eCodes finalize();

					/**
					 * Find the local and global cell IDs that contain the coordinates defined by point.
					 *
					 * ToDo: Warning - In the event that the point is on an edge or vertex
					 * between a local cell and a ghost cell there is a risk that, if this is the case, the emitter
					 * could be 'found' on two different processes which could cause the potential for unintended
					 * duplication.
					 *
					 * If the cell that contains it does not exist locally on this rank, then the values are unset and
					 * a suitable error code returned.
					 *
					 * If the point sits on a edge and/or vertex, it will report the first cell encountered as the cell.
					 *
					 * Note: In distributed setups, this means that more than one rank may find a cell containing the point
					 * if it is on the edge or vertex of a boundary. In such a case this process will still report
					 * a cell for each, but the error code will indicate the shared nature. It is left to the callee
					 * to handle the behaviour in such a case.
					 *
					 * @param point The point we wish to find the cell container ID for.
					 * @param localCellID A pointer to the location that will be updated with the local ID of the cell
					 * @param globalCellID A pointer to the location that will be updated with the global ID of the cell
					 *
					 * @return An error status indicating the success or failure of the operation
					 * @retval cupcfd::error::E_SUCCESS Success
					 * @retval cupcfd::error::E_GEOMETRY_NO_VALID_CELL No suitable cell found on this rank
					 */
					__attribute__((warn_unused_result))
					cupcfd::error::eCodes findCellID(euc::EuclideanPoint<T,3>& point,  I * localCellID, I * globalCellID);
			};
		}
	}
}

// Include Header Level Definitions
#include "UnstructuredMeshInterface.ipp"

#endif
