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
#include "EuclideanPoint.h"
#include "MeshSource.h"
#include "Polyhedron.h"
#include "Tetrahedron.h"
#include "TriPrism.h"
#include "QuadPyramid.h"
#include "Hexahedron.h"

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
					virtual cupcfd::error::eCodes getCellCenter(I cellID, cupcfd::geometry::euclidean::EuclideanPoint<T,3>& center) = 0;

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
					virtual cupcfd::geometry::euclidean::EuclideanPoint<T,3> getCellCenter(I cellID) = 0;

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
					virtual cupcfd::error::eCodes getCellVolume(I cellID, T * vol) = 0;

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
					virtual T getCellVolume(I cellID) = 0;


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
					virtual cupcfd::error::eCodes getCellNFaces(I cellID, I * nFaces) = 0;

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
					virtual I getCellNFaces(I cellID) = 0;

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
					virtual cupcfd::error::eCodes getCellStoredNFaces(I cellID, I * nFaces) = 0;

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
					virtual I getCellStoredNFaces(I cellID) = 0;

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
					virtual cupcfd::error::eCodes getCellNVertices(I cellID, I * nVertices) = 0;

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
					virtual I getCellNVertices(I cellID) = 0;

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
					virtual cupcfd::error::eCodes getCellStoredNVertices(I cellID, I * nVertices) = 0;

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
					virtual I getCellStoredNVertices(I cellID) = 0;

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
					virtual cupcfd::error::eCodes getCellFaceID(I cellID, I cellFaceID, I * faceID) = 0;

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
					virtual I getCellFaceID(I cellID, I cellFaceID) = 0;

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
					virtual cupcfd::error::eCodes setCellCenter(I cellID, cupcfd::geometry::euclidean::EuclideanPoint<T,3>& center) = 0;

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
					virtual cupcfd::error::eCodes setCellVolume(I cellID, T vol) = 0;

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
					virtual cupcfd::error::eCodes addCell(L cellLabel,
													   cupcfd::geometry::euclidean::EuclideanPoint<T,3>& center,
													   T vol,
													   bool isLocal) = 0;

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
					virtual cupcfd::error::eCodes addCell(L cellLabel, bool isLocal) = 0;

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
					virtual cupcfd::error::eCodes getCellID(L cellLabel, I * cellID) = 0;

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
					virtual I getCellID(L cellLabel) = 0;

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
					virtual cupcfd::error::eCodes getFaceNVertices(I faceID, I * nVertices) = 0;

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
					virtual I getFaceNVertices(I faceID) = 0;

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
					virtual cupcfd::error::eCodes getFaceCell1ID(I faceID, I * cellID) = 0;

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
					virtual I getFaceCell1ID(I faceID) = 0;

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
					virtual cupcfd::error::eCodes getFaceCell2ID(I faceID, I * cellID) = 0;

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
					virtual I getFaceCell2ID(I faceID) = 0;

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
					virtual cupcfd::error::eCodes getFaceLambda(I faceID, T * lambda) = 0;

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
					virtual T getFaceLambda(I faceID) = 0;

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
					virtual cupcfd::error::eCodes getFaceNorm(I faceID, cupcfd::geometry::euclidean::EuclideanVector<T,3>& norm) = 0;

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
					virtual cupcfd::geometry::euclidean::EuclideanVector<T,3> getFaceNorm(I faceID) = 0;

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
					virtual cupcfd::error::eCodes getFaceVertex(I faceID, I faceVertexID, I * vertexID) = 0;

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
					virtual I getFaceVertex(I faceID, I faceVertexID) = 0;

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
					virtual cupcfd::error::eCodes getFaceCenter(I faceID, cupcfd::geometry::euclidean::EuclideanPoint<T,3>& center) = 0;

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
					virtual cupcfd::geometry::euclidean::EuclideanPoint<T,3> getFaceCenter(I faceID) = 0;

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
					virtual cupcfd::error::eCodes getFaceRLencos(I faceID, T * rlencos) = 0;

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
					virtual T getFaceRLencos(I faceID) = 0;

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
					virtual cupcfd::error::eCodes getFaceArea(I faceID, T * area) = 0;

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
					virtual T getFaceArea(I faceID) = 0;

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
					virtual cupcfd::error::eCodes getFaceXpac(I faceID, cupcfd::geometry::euclidean::EuclideanPoint<T,3>& xpac) = 0;

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
					virtual cupcfd::geometry::euclidean::EuclideanPoint<T,3> getFaceXpac(I faceID) = 0;

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
					virtual cupcfd::error::eCodes getFaceXnac(I faceID, cupcfd::geometry::euclidean::EuclideanPoint<T,3>& xnac) = 0;

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
					virtual cupcfd::geometry::euclidean::EuclideanPoint<T,3> getFaceXnac(I faceID) = 0;

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
					virtual cupcfd::error::eCodes getFaceIsBoundary(I faceID, bool * result) = 0;

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
					virtual bool getFaceIsBoundary(I faceID) = 0;

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
					virtual cupcfd::error::eCodes getFaceBoundaryID(I faceID, I * boundaryID) = 0;

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
					virtual I getFaceBoundaryID(I faceID) = 0;

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
					virtual cupcfd::error::eCodes setFaceCell1ID(I faceID, I cellID) = 0;

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
					virtual cupcfd::error::eCodes setFaceCell2ID(I faceID, I cellID) = 0;

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
					virtual cupcfd::error::eCodes setFaceLambda(I faceID, T lambda) = 0;

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
					virtual cupcfd::error::eCodes setFaceNorm(I faceID, cupcfd::geometry::euclidean::EuclideanVector<T,3>& norm) = 0;

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
					virtual cupcfd::error::eCodes setFaceVertex(I faceID, I faceVertexID, I vertexID) = 0;

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
					virtual cupcfd::error::eCodes setFaceCenter(I faceID, cupcfd::geometry::euclidean::EuclideanPoint<T,3>& center) = 0;

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
					virtual cupcfd::error::eCodes setFaceRLencos(I faceID, T rlencos) = 0;

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
					virtual cupcfd::error::eCodes setFaceArea(I faceID, T area) = 0;

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
					virtual cupcfd::error::eCodes setFaceXpac(I faceID, cupcfd::geometry::euclidean::EuclideanPoint<T,3>& xpac) = 0;

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
					virtual cupcfd::error::eCodes setFaceXnac(I faceID, cupcfd::geometry::euclidean::EuclideanPoint<T,3>& xnac) = 0;

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
					virtual cupcfd::error::eCodes setFaceBoundaryID(I faceID, I boundaryID) = 0;


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
					virtual cupcfd::error::eCodes addFace(
								L faceLabel,
								L cell1Label,
								L cell2OrBoundaryLabel,
								bool isBoundary,
								T lambda,
								cupcfd::geometry::euclidean::EuclideanVector<T,3>& norm,
								L * vertexLabels, I nVertexLabels,
								cupcfd::geometry::euclidean::EuclideanPoint<T,3>& center,
								cupcfd::geometry::euclidean::EuclideanPoint<T,3>& xpac,
								cupcfd::geometry::euclidean::EuclideanPoint<T,3>& xnac,
								T rlencos,
								T area) = 0;

					virtual cupcfd::error::eCodes addFace(
								L faceLabel,
								L cell1Label,
								L cell2OrBoundaryLabel,
								bool isBoundary,
								L * vertexLabels, I nVertexLabels) = 0;


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
					virtual cupcfd::error::eCodes getFaceID(L faceLabel, I * faceID) = 0;

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
					virtual I getFaceID(L faceLabel) = 0;

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
					virtual cupcfd::error::eCodes getBoundaryFaceID(I boundaryID, I * faceID) = 0;

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
					virtual I getBoundaryFaceID(I boundaryID) = 0;

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
					virtual cupcfd::error::eCodes getBoundaryVertex(I boundaryID, I boundaryVertexID, I * vertexID) = 0;

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
					virtual I getBoundaryVertex(I boundaryID, I boundaryVertexID) = 0;

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
					virtual cupcfd::error::eCodes getBoundaryDistance(I boundaryID, T * distance) = 0;

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
					virtual T getBoundaryDistance(I boundaryID) = 0;

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
					virtual cupcfd::error::eCodes getBoundaryYPlus(I boundaryID, T * yPlus) = 0;

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
					virtual T getBoundaryYPlus(I boundaryID) = 0;

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
					virtual cupcfd::error::eCodes getBoundaryUPlus(I boundaryID, T * uPlus) = 0;

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
					virtual T getBoundaryUPlus(I boundaryID) = 0;

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
					virtual cupcfd::error::eCodes getBoundaryRegionID(I boundaryID, I * regionID) = 0;

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
					virtual I getBoundaryRegionID(I boundaryID) = 0;

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
					virtual cupcfd::error::eCodes getBoundaryShear(I boundaryID, cupcfd::geometry::euclidean::EuclideanVector<T,3>& shear) = 0;

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
					virtual cupcfd::geometry::euclidean::EuclideanVector<T,3> getBoundaryShear(I boundaryID) = 0;

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
					virtual cupcfd::error::eCodes getBoundaryQ(I boundaryID, T * q) = 0;

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
					virtual T getBoundaryQ(I boundaryID) = 0;

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
					virtual cupcfd::error::eCodes getBoundaryH(I boundaryID, T * h) = 0;

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
					virtual T getBoundaryH(I boundaryID) = 0;

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
					virtual cupcfd::error::eCodes getBoundaryT(I boundaryID, T * t) = 0;

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
					virtual T getBoundaryT(I boundaryID) = 0;


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
					virtual cupcfd::error::eCodes setBoundaryFaceID(I boundaryID, I faceID) = 0;

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
					virtual cupcfd::error::eCodes setBoundaryVertex(I boundaryID, I boundaryVertexID, I vertexID) = 0;

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
					virtual cupcfd::error::eCodes setBoundaryDistance(I boundaryID, T distance) = 0;

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
					virtual cupcfd::error::eCodes setBoundaryYPlus(I boundaryID, T yPlus) = 0;

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
					virtual cupcfd::error::eCodes setBoundaryUPlus(I boundaryID, T uPlus) = 0;

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
					virtual cupcfd::error::eCodes setBoundaryRegionID(I boundaryID, I regionID) = 0;

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
					virtual cupcfd::error::eCodes setBoundaryShear(I boundaryID, cupcfd::geometry::euclidean::EuclideanVector<T,3>& shear) = 0;

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
					virtual cupcfd::error::eCodes setBoundaryQ(I boundaryID, T q) = 0;

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
					virtual cupcfd::error::eCodes setBoundaryH(I boundaryID, T h) = 0;

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
					virtual cupcfd::error::eCodes setBoundaryT(I boundaryID, T t) = 0;

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
					virtual cupcfd::error::eCodes addBoundary(
							L boundaryLabel,
							L regionLabel,
							L * vertexLabels, I nVertexLabels,
							T distance) = 0;

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
					virtual cupcfd::error::eCodes addBoundary(
							L boundaryLabel,
							L regionLabel,
							L * vertexLabels, I nVertexLabels) = 0;


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
					virtual cupcfd::error::eCodes getBoundaryID(L boundaryLabel, I * boundaryID) = 0;

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
					virtual I getBoundaryID(L boundaryLabel) = 0;


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
					virtual cupcfd::error::eCodes getRegionType(I regionID, RType * type) = 0;

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
					virtual RType getRegionType(I regionID) = 0;

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
					virtual cupcfd::error::eCodes getRegionStd(I regionID, bool * std) = 0;

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
					virtual bool getRegionStd(I regionID) = 0;

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
					virtual cupcfd::error::eCodes getRegionYLog(I regionID, T * yLog) = 0;

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
					virtual T getRegionYLog(I regionID) = 0;

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
					virtual cupcfd::error::eCodes getRegionELog(I regionID, T * eLog) = 0;

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
					virtual T getRegionELog(I regionID) = 0;

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
					virtual cupcfd::error::eCodes getRegionDensity(I regionID, T * density) = 0;

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
					virtual T getRegionDensity(I regionID) = 0;

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
					virtual cupcfd::error::eCodes getRegionTurbKE(I regionID, T * turbKE) = 0;

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
					virtual T getRegionTurbKE(I regionID) = 0;

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
					virtual cupcfd::error::eCodes getRegionTurbDiss(I regionID, T * turbDiss) = 0;

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
					virtual T getRegionTurbDiss(I regionID) = 0;

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
					virtual cupcfd::error::eCodes getRegionSplvl(I regionID, T * splvl) = 0;

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
					virtual T getRegionSplvl(I regionID) = 0;

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
					virtual cupcfd::error::eCodes getRegionDen(I regionID, T * den) = 0;

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
					virtual T getRegionDen(I regionID) = 0;

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
					virtual cupcfd::error::eCodes getRegionForceTangent(I regionID, cupcfd::geometry::euclidean::EuclideanVector<T,3>& forceTangent) = 0;

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
					virtual cupcfd::geometry::euclidean::EuclideanVector<T,3> getRegionForceTangent(I regionID) = 0;

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
					virtual cupcfd::error::eCodes getRegionUVW(I regionID, cupcfd::geometry::euclidean::EuclideanVector<T,3>& uvw) = 0;

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
					virtual cupcfd::geometry::euclidean::EuclideanVector<T,3> getRegionUVW(I regionID) = 0;

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
					virtual cupcfd::error::eCodes getRegionName(I regionID, std::string& regionName) = 0;

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
					virtual std::string getRegionName(I regionID) = 0;

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
					virtual cupcfd::error::eCodes getRegionFlux(I regionID, bool * flux) = 0;

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
					virtual bool getRegionFlux(I regionID) = 0;

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
					virtual cupcfd::error::eCodes getRegionAdiab(I regionID, bool * adiab) = 0;

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
					virtual bool getRegionAdiab(I regionID) = 0;

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
					virtual cupcfd::error::eCodes getRegionR(I regionID, T * r) = 0;

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
					virtual T getRegionR(I regionID) = 0;

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
					virtual cupcfd::error::eCodes getRegionT(I regionID, T * t) = 0;

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
					virtual T getRegionT(I regionID) = 0;


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
					virtual cupcfd::error::eCodes setRegionType(I regionID, RType type) = 0;

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
					virtual cupcfd::error::eCodes setRegionStd(I regionID, bool std) = 0;

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
					virtual cupcfd::error::eCodes setRegionYLog(I regionID, T yLog) = 0;

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
					virtual cupcfd::error::eCodes setRegionELog(I regionID, T eLog) = 0;

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
					virtual cupcfd::error::eCodes setRegionDensity(I regionID, T density) = 0;

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
					virtual cupcfd::error::eCodes setRegionTurbKE(I regionID, T turbKE) = 0;

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
					virtual cupcfd::error::eCodes setRegionTurbDiss(I regionID, T turbDiss) = 0;

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
					virtual cupcfd::error::eCodes setRegionSplvl(I regionID, T splvl) = 0;

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
					virtual cupcfd::error::eCodes setRegionDen(I regionID, T den) = 0;

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
					virtual cupcfd::error::eCodes setRegionForceTangent(I regionID, cupcfd::geometry::euclidean::EuclideanVector<T,3>& forceTangent) = 0;

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
					virtual cupcfd::error::eCodes setRegionUVW(I regionID, cupcfd::geometry::euclidean::EuclideanVector<T,3>& uvw) = 0;

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
					virtual cupcfd::error::eCodes setRegionName(I regionID, std::string& regionName) = 0;

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
					virtual cupcfd::error::eCodes setRegionFlux(I regionID, bool flux) = 0;

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
					virtual cupcfd::error::eCodes setRegionAdiab(I regionID, bool adiab) = 0;

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
					virtual cupcfd::error::eCodes setRegionR(I regionID, T r) = 0;

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
					virtual cupcfd::error::eCodes setRegionT(I regionID, T t) = 0;

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
					virtual cupcfd::error::eCodes addRegion(
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
										cupcfd::geometry::euclidean::EuclideanVector<T,3>& forceTangent,
										cupcfd::geometry::euclidean::EuclideanVector<T,3>& uvw,
										std::string& regionName) = 0;

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
					virtual cupcfd::error::eCodes addRegion(L regionLabel, std::string& regionName) = 0;

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
					virtual cupcfd::error::eCodes getRegionID(L regionLabel, I * regionID) = 0;


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
					virtual I getRegionID(L regionLabel) = 0;

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
					virtual cupcfd::error::eCodes getVertexPos(I vertexID, cupcfd::geometry::euclidean::EuclideanPoint<T,3>& pos) = 0;

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
					virtual cupcfd::geometry::euclidean::EuclideanPoint<T,3> getVertexPos(I vertexID) = 0;

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
					virtual cupcfd::error::eCodes setVertexPos(I vertexLabel, cupcfd::geometry::euclidean::EuclideanPoint<T,3>& pos) = 0;


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
					virtual cupcfd::error::eCodes addVertex(L vertexLabel, cupcfd::geometry::euclidean::EuclideanPoint<T,3>& pos) = 0;


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
					virtual cupcfd::error::eCodes getVertexID(L vertexLabel, I * vertexID) = 0;

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
					virtual I getVertexID(L vertexLabel) = 0;

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
					cupcfd::geometry::shapes::PolyhedronType getCellPolyhedronType(I cellID);

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
					cupcfd::error::eCodes buildPolyhedron(I cellID, cupcfd::geometry::shapes::TriPrism<T> ** shape);

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
					 cupcfd::error::eCodes buildPolyhedron(I cellID, cupcfd::geometry::shapes::QuadPyramid<T> ** shape);

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
					 cupcfd::error::eCodes buildPolyhedron(I cellID, cupcfd::geometry::shapes::Tetrahedron<T> ** shape);

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
					cupcfd::error::eCodes buildPolyhedron(I cellID, cupcfd::geometry::shapes::Hexahedron<T> ** shape);

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
					cupcfd::error::eCodes findCellID(cupcfd::geometry::euclidean::EuclideanPoint<T,3>& point,  I * localCellID, I * globalCellID);
			};
		}
	}
}

// Include Header Level Definitions
#include "UnstructuredMeshInterface.ipp"

#endif
