/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Contains the declarations for the MeshSource Class.
 */

#ifndef CUPCFD_GEOMETRY_MESH_SOURCE_INCLUDE_H
#define CUPCFD_GEOMETRY_MESH_SOURCE_INCLUDE_H

#include <string>
#include "AdjacencyListCSR.h"
#include "DistributedAdjacencyList.h"
#include "Error.h"
#include "EuclideanPoint.h"
#include "EuclideanVector.h"

#include "SortDrivers.h"
#include "StatisticsDrivers.h"

#include <iostream>

namespace euc = cupcfd::geometry::euclidean;

namespace cupcfd
{
	namespace geometry
	{
		namespace mesh
		{
			/**
			 * Describes a basic interface for retrieving mesh related data from a mesh file.
			 * This is specific to geometric data only, not state/quantity data such as temperatures etc.
			 *
			 * Data is accessed via labels. These labels can be retrieved from the source by calling
			 * a suitable function and providing zero-based indices - i.e. effectively asking what are
			 * the labels of the, 1st, 2nd cell etc.
			 *
			 * The remainder of the functions are designed around accessing, where available, data about
			 * the mesh and its connections.
			 *
			 * @tparam I The type of the indexing scheme
			 * @tparam T The type of the mesh geometry data
			 * @tparam L The type of the labels in the mesh source
			 *
			 */
			template <class I, class T, class L>
			class MeshSource
			{
				public:
					// === Members ===
					I gidBase = 0;			// Base of the indexing scheme used

					// === Constructors/Deconstructors ===

					/**
					 *
					 */
					MeshSource();

					/**
					 * Deconstructor
					 */
					virtual ~MeshSource();

					// === Pure Virtual Methods ===

					/**
					 * Get the labels assigned to the cells in the mesh source for the requested indexes.
					 * The indexes can pull from anywhere in the range of 0->nCells - 1 and are zero-indexed.
					 * These indexes are effectively just the ith cell to appear in the source.
					 *
					 * This allows for retrieving a mapping based on the cell count to their labels in the source,
					 * which may use arbitrary labelling schemes (or even just non-zero schemes)
					 *
					 * @param labels The cell labels in the source. Updated by this function.
					 * @param nLabels The size of labels in the number of elements of type I.
					 * @param indexes An array of base-zero indexes.
					 * @param nIndexes The size of indexes in the number of elements of type I.
					 */
					__attribute__((warn_unused_result))
					virtual cupcfd::error::eCodes getCellLabels(L * labels, I nLabels, I * indexes, I nIndexes) = 0;

					/**
					 * Get the labels assigned to the face in the mesh source for the requested indexes.
					 * The indexes can pull from anywhere in the range of 0->nFaces - 1 and are zero-indexed.
					 *
					 * This allows for retrieving a mapping based on the cell count to their labels in the source,
					 * which may use arbitrary labelling schemes (or even just non-zero schemes)
					 *
					 * @param labels The labels in the source. Updated by this function.
					 * @param nLabels The size of labels in the number of elements of type I.
					 * @param indexes An array of base-zero indexes.
					 * @param nIndexes The size of indexes in the number of elements of type I.
					 */
					__attribute__((warn_unused_result))
					virtual cupcfd::error::eCodes getFaceLabels(L * labels, I nLabels, I * indexes, I nIndexes) = 0;

					/**
					 * Get the labels assigned to the vertices in the mesh source for the requested indexes.
					 * The indexes can pull from anywhere in the range of 0->nVertices - 1 and are zero-indexed.
					 *
					 * This allows for retrieving a mapping based on the cell count to their labels in the source,
					 * which may use arbitrary labelling schemes (or even just non-zero schemes)
					 *
					 * @param labels The labels in the source. Updated by this function.
					 * @param nLabels The size of labels in the number of elements of type I.
					 * @param indexes An array of base-zero indexes.
					 * @param nIndexes The size of indexes in the number of elements of type I.
					 */
					__attribute__((warn_unused_result))
					virtual cupcfd::error::eCodes getVertexLabels(L * labels, I nLabels, I * indexes, I nIndexes) = 0;

					/**
					 * Get the labels assigned to the boundaries in the mesh source for the requested indexes.
					 * The indexes can pull from anywhere in the range of 0->nBoundaries - 1 and are zero-indexed.
					 *
					 * This allows for retrieving a mapping based on the cell count to their labels in the source,
					 * which may use arbitrary labelling schemes (or even just non-zero schemes)
					 *
					 * @param labels The labels in the source. Updated by this function.
					 * @param nLabels The size of labels in the number of elements of type I.
					 * @param indexes An array of base-zero indexes.
					 * @param nIndexes The size of indexes in the number of elements of type I.
					 */
					__attribute__((warn_unused_result))
					virtual cupcfd::error::eCodes getBoundaryLabels(L * labels, I nLabels, I * indexes, I nIndexes) = 0;

					/**
					 * Get the labels assigned to the regions in the mesh source for the requested indexes.
					 * The indexes can pull from anywhere in the range of 0->nRegions - 1 and are zero-indexed.
					 *
					 * This allows for retrieving a mapping based on the cell count to their labels in the source,
					 * which may use arbitrary labelling schemes (or even just non-zero schemes)
					 *
					 * @param labels The labels in the source. Updated by this function.
					 * @param nLabels The size of labels in the number of elements of type I.
					 * @param indexes An array of base-zero indexes.
					 * @param nIndexes The size of indexes in the number of elements of type I.
					 */
					__attribute__((warn_unused_result))
					virtual cupcfd::error::eCodes getRegionLabels(L * labels, I nLabels, I * indexes, I nIndexes) = 0;

					/**
					 * Get the total number of cells in the mesh source
					 *
					 * @param cellCount A pointer to the location where the cell count will be stored
					 *
					 * @tparam I The type of the indexing scheme
					 * @tparam T The type of the stored node data
					 *
					 * @return An error status indicating the success or failure of the operation
					 * @retval cupcfd::error::E_SUCCESS The method completed successfully
					 * @retval cupcfd::error::E_SOURCE_MISSING This data is missing from the source
					 */
					__attribute__((warn_unused_result))
					virtual cupcfd::error::eCodes getCellCount(I * cellCount) = 0;

					/**
					 * Get the total number of faces in the mesh source
					 *
					 * @param faceCount A pointer to the location where the face count will be stored
					 *
					 * @tparam I The type of the indexing scheme
					 * @tparam T The type of the stored node data
					 *
					 * @return An error status indicating the success or failure of the operation
					 * @retval cupcfd::error::E_SUCCESS The method completed successfully
					 * @retval cupcfd::error::E_SOURCE_MISSING This data is missing from the source
					 */
					__attribute__((warn_unused_result))
					virtual cupcfd::error::eCodes getFaceCount(I * faceCount) = 0;

					/**
					 * Get the total number of boundaries in the mesh source
					 *
					 * @param boundaryCount A pointer to the location where the face count will be stored
					 *
					 * @tparam I The type of the indexing scheme
					 * @tparam T The type of the stored node data
					 *
					 * @return An error status indicating the success or failure of the operation
					 * @retval cupcfd::error::E_SUCCESS The method completed successfully
					 * @retval cupcfd::error::E_SOURCE_MISSING This data is missing from the source
					 */
					__attribute__((warn_unused_result))
					virtual cupcfd::error::eCodes getBoundaryCount(I * boundaryCount) = 0;

					/**
					 * Get the total number of regions in the mesh source
					 *
					 * @param regionCount A pointer to the location where the region count will be stored
					 *
					 * @tparam I The type of the indexing scheme
					 * @tparam T The type of the stored node data
					 *
					 * @return An error status indicating the success or failure of the operation
					 * @retval cupcfd::error::E_SUCCESS The method completed successfully
					 * @retval cupcfd::error::E_SOURCE_MISSING This data is missing from the source
					 */
					__attribute__((warn_unused_result))
					virtual cupcfd::error::eCodes getRegionCount(I * regionCount) = 0;

					/**
					 * Get the total number of vertices in the mesh source
					 *
					 * @param vertexCount A pointer to the location where the vertex count will be stored
					 *
					 * @tparam I The type of the indexing scheme
					 * @tparam T The type of the stored node data
					 *
					 * @return An error status indicating the success or failure of the operation
					 * @retval cupcfd::error::E_SUCCESS The method completed successfully
					 * @retval cupcfd::error::E_SOURCE_MISSING This data is missing from the source
					 */
					__attribute__((warn_unused_result))
					virtual cupcfd::error::eCodes getVertexCount(I * vertexCount) = 0;

					/**
					 * Get the maximum number of faces per cell stored in this mesh file
					 *
					 * @param maxFaceCount A pointer to the location where the max face count will be stored
					 *
					 * @tparam I The type of the indexing scheme
					 * @tparam T The type of the stored node data
					 *
					 * @return An error status indicating the success or failure of the operation
					 * @retval cupcfd::error::E_SUCCESS The method completed successfully
					 * @retval cupcfd::error::E_SOURCE_MISSING This data is missing from the source
					 */
					__attribute__((warn_unused_result))
					virtual cupcfd::error::eCodes getMaxFaceCount(I * maxFaceCount) = 0;


					/**
					 * Get the maximum number of vertexes per face or boundary in this mesh file
					 *
					 * @param maxFaceCount A pointer to the location where the max vertex count
					 *
					 * @tparam I The type of the indexing scheme
					 * @tparam T The type of the stored node data
					 *
					 * @return An error status indicating the success or failure of the operation
					 * @retval cupcfd::error::E_SUCCESS The method completed successfully
					 * @retval cupcfd::error::E_SOURCE_MISSING This data is missing from the source
					 */
					__attribute__((warn_unused_result))
					virtual cupcfd::error::eCodes getMaxVertexCount(I * maxVertexCount) = 0;

					/**
					 * Get the number of faces associated with a cell for specific cells in the mesh
					 *
					 * @param nFaces An array where the number of faces for each cell will be stored,
					 * indexed by their cell labels 0->n-1.
					 * @param nNFaces The size of the nFaces array
					 * @param cellLabels An array of labels for the cells we wish to retrieve data for,
					 * zero-based as per the general label scheme.
					 * @param nCellLabels The size of the cellLabels array (i.e. the number of cells to
					 * retrieve data for)
					 *
					 * @tparam I The type of the indexing scheme
					 * @tparam T The type of the stored node data
					 *
					 * @return An error status indicating the success or failure of the operation
					 * @retval cupcfd::error::E_SUCCESS The method completed successfully
					 * @retval cupcfd::error::E_SOURCE_MISSING This data is missing from the source
					 */
					__attribute__((warn_unused_result))
					virtual cupcfd::error::eCodes getCellNFaces(I * nFaces, I nNFaces, L * cellLabels, I nCellLabels) = 0;

					/**
					 * Get the volume of the cells stored in the mesh source for the cell labels provided
					 *
					 * @param cellVol An array where the volumes for each retrieved cell will be stored
					 * @param nCellVol The size of the cellVol array
					 * @param cellLabels An array of labels for the cells we wish to retrieve data for,
					 * zero-based as per the general label scheme.
					 * @param nCellLabels The size of the cellLabels array (i.e. the number of cells to
					 * retrieve data for)
					 *
					 * @tparam I The type of the indexing scheme
					 * @tparam T The type of the stored node data
					 *
					 * @return An error status indicating the success or failure of the operation
					 * @retval cupcfd::error::E_SUCCESS The method completed successfully
					 * @retval cupcfd::error::E_SOURCE_MISSING This data is missing from the source
					 */
					__attribute__((warn_unused_result))
					virtual cupcfd::error::eCodes getCellVolume(T * cellVol, I nCellVol, L * cellLabels, I nCellLabels) = 0;

					/**
					 * Get the cell center of the cells stored in the mesh source for cells with the requested labels.
					 *
					 * @param cellCenter An array to store the cell centers computed by this function.
					 * The i'th cell center in this array is paired with the i'th cell label in cellLabels.
					 * @parma nCellCenter The size of the cellCenter array in the number of elements of type T EuclideanPoints.
					 * @param cellLabels An array of labels for the cells we wish to retrieve data for,
					 * zero-based as per the general label scheme.
					 * @param nCellLabels The size of the cellLabels array (i.e. the number of cells to
					 * retrieve data for)
					 *
					 * @tparam I The type of the indexing scheme
					 * @tparam T The type of the stored node data
					 *
					 * @return An error status indicating the success or failure of the operation
					 * @retval cupcfd::error::E_SUCCESS The method completed successfully
					 * @retval cupcfd::error::E_SOURCE_MISSING This data is missing from the source
					 */
					__attribute__((warn_unused_result))
					virtual cupcfd::error::eCodes getCellCenter(euc::EuclideanPoint<T,3> * cellCenter, I nCellCenter, L * cellLabels, I nCellLabels) = 0;

					/**
					 * Get the labels of the faces attached to specified cells, stored in a CSR format.
					 *
					 * The CSR formats means that for a cellLabel at position i, the face labels associated with that cell label
					 * are stored in a range of csrData, starting at csrData[csrIndices[i]] through to csrData[csrIndices[i+1] - 1].
					 *
					 * This data arrays for this must be setup to be the correct size to hold all of the relevant data - specifically:
					 * nCsrIndices = nCellLabels + 1
					 * nCsrData = Sum of number of faces per label for all labels.
					 *
					 * The nCsrData can be obtained using a Sum of values provided by getCellNFaces(labels)
					 *
					 * @param csrIndices An array of the start of each data range for the cell labels in csrData, updated by this function.
					 * @param nCsrIndices The size of csrIndices in the number of elements of type I.
					 * @param csrData An array of the face labels associated with cells, grouped by cell label into blocks, with each group
					 * appearing in the same order as the cell labels in cellLabels that they are associated with. Updated by this function.
					 * @param nCsrData The size of csrData in the number of elements of type I
					 * @param cellLabels An array of labels for cells to lookup the assocaited face labels of.
					 * @param nCellLabels The size of cellLabels in the number of elements of type I
					 *
					 * @tparam I The type of the indexing scheme
					 * @tparam T The type of the stored node data
					 *
					 * @return An error status indicating the success or failure of the operation
					 * @retval cupcfd::error::E_SUCCESS The method completed successfully
					 * @retval cupcfd::error::E_SOURCE_MISSING This data is missing from the source
					 */
					__attribute__((warn_unused_result))
					virtual cupcfd::error::eCodes getCellFaceLabels(I * csrIndices, I nCsrIndices, I * csrData, I nCsrData, L * cellLabels, I nCellLabels) = 0;

					/**
					 * Determine whether the faces with the provided face labels are boundary faces.
					 *
					 * @param isBoundary An array to store the booleans computed by this function.
					 * The i'th boolean in this array is paired with the i'th cell label in cellLabels.
					 * @param nIsBoundary The size of the isBoundary array in the number of elements of type boolean
					 * @param faceLabels An array of labels for the faces we wish to retrieve data for,
					 * zero-based as per the general label scheme.
					 * @param nFaceLabels The size of the faceLabels array (i.e. the number of faces to
					 * retrieve data for)
					 *
					 * @tparam I The type of the indexing scheme
					 * @tparam T The type of the stored node data
					 *
					 * @return An error status indicating the success or failure of the operation
					 * @retval cupcfd::error::E_SUCCESS The method completed successfully
					 * @retval cupcfd::error::E_SOURCE_MISSING This data is missing from the source
					 */
					__attribute__((warn_unused_result))
					virtual cupcfd::error::eCodes getFaceIsBoundary(bool * isBoundary, I nIsBoundary, L * faceLabels, I nFaceLabels) = 0;

					/**
					 *
					 */
					__attribute__((warn_unused_result))
					virtual cupcfd::error::eCodes getFaceNVertices(I * nVertices, I nNVertices, L * faceLabels, I nFaceLabels) = 0;

					/**
					 * Get the labels of the boundaries associated with these faces.
					 *
					 * All faces must have boundaries associated with them, else an error will be returned.
					 *
					 * @param faceLabels An array of labels for the faces we wish to retrieve data for,
					 * zero-based as per the general label scheme.
					 * @param nFaceLabels The size of the faceLabels array (i.e. the number of faces to
					 * retrieve data for)
					 *
					 * @tparam I The type of the indexing scheme
					 * @tparam T The type of the stored node data
					 *
					 * @return An error status indicating the success or failure of the operation
					 * @retval cupcfd::error::E_SUCCESS The method completed successfully
					 * @retval cupcfd::error::E_SOURCE_MISSING This data is missing from the source
					 * @retval cupcfd::error::E_ERROR One or more faces are not boundaries
					 */
					__attribute__((warn_unused_result))
					virtual cupcfd::error::eCodes getFaceBoundaryLabels(I * faceBoundaryLabels, I nFaceBoundaryLabels, L * faceLabels, I nFaceLabels) = 0;

					/**
					 * Get the cell label for the first cell attached to the faces with the provided face labels.
					 *
					 * @param faceLabels An array of labels for the faces we wish to retrieve data for,
					 * zero-based as per the general label scheme.
					 * @param nFaceLabels The size of the faceLabels array (i.e. the number of faces to
					 * retrieve data for)
					 *
					 * @tparam I The type of the indexing scheme
					 * @tparam T The type of the stored node data
					 *
					 * @return An error status indicating the success or failure of the operation
					 * @retval cupcfd::error::E_SUCCESS The method completed successfully
					 * @retval cupcfd::error::E_SOURCE_MISSING This data is missing from the source
					 */
					__attribute__((warn_unused_result))
					virtual cupcfd::error::eCodes getFaceCell1Labels(I * faceCell1Labels, I nFaceCell1Labels, L * faceLabels, I nFaceLabels) = 0;

					/**
					 * Get the cell labels for the 2nd cell attached to each face with a provided face label.
					 * If such a cell does not exist (e.g. boundary faces), set the value to -1 instead.
					 *
					 * ToDo: -1 could potentially be a valid label in some odd setups. Make it so that an error
					 * is returned if you look for a face without a cell 2 instead.
					 *
					 * @param faceCell2Labels An array to store the retrieved cell 2 labels in.
					 * @param nFaceLabels The size of the faceCell2Labels array.
					 * @param faceLabels An array of labels for the faces we wish to retrieve data for,
					 * zero-based as per the general label scheme.
					 * @param nFaceLabels The size of the faceLabels array (i.e. the number of faces to
					 * retrieve data for).
					 *
					 * @tparam I The type of the indexing scheme
					 * @tparam T The type of the stored node data
					 *
					 * @return An error status indicating the success or failure of the operation
					 * @retval cupcfd::error::E_SUCCESS The method completed successfully
					 * @retval cupcfd::error::E_SOURCE_MISSING This data is missing from the source
					 */
					__attribute__((warn_unused_result))
					virtual cupcfd::error::eCodes getFaceCell2Labels(I * faceCell2Labels, I nFaceCell2Labels, L * faceLabels, I nFaceLabels) = 0;

					/**
					 * Retrieve the lambda for the faces with the provided labels.
					 *
					 * If a face does not have a lambda (e.g. boundary) then it returns -1 instead.
					 *
					 * @param faceLabels An array of labels for the faces we wish to retrieve data for,
					 * zero-based as per the general label scheme.
					 * @param nFaceLabels The size of the faceLabels array (i.e. the number of faces to
					 * retrieve data for)
					 *
					 * @tparam I The type of the indexing scheme
					 * @tparam T The type of the stored node data
					 *
					 * @return An error status indicating the success or failure of the operation
					 * @retval cupcfd::error::E_SUCCESS The method completed successfully
					 * @retval cupcfd::error::E_SOURCE_MISSING This data is missing from the source
					 */
					__attribute__((warn_unused_result))
					virtual cupcfd::error::eCodes getFaceLambda(T * faceLambda, I nFaceLambda, L * faceLabels, I nFaceLabels) = 0;

					/**
					 * Returns the face area for the faces with the provided labels
					 *
					 * @tparam I The type of the indexing scheme
					 * @tparam T The type of the stored node data
					 *
					 * @return An error status indicating the success or failure of the operation
					 * @retval cupcfd::error::E_SUCCESS The method completed successfully
					 * @retval cupcfd::error::E_SOURCE_MISSING This data is missing from the source
					 */
					__attribute__((warn_unused_result))
					virtual cupcfd::error::eCodes getFaceArea(T * faceArea, I nFaceArea, L * faceLabels, I nFaceLabels) = 0;

					/**
					 * Get the normal of the face. This direction of this vector is outwards from cell 1.
					 *
					 * @param faceLabels An array of labels for the faces we wish to retrieve data for,
					 * zero-based as per the general label scheme.
					 * @param nFaceLabels The size of the faceLabels array (i.e. the number of faces to
					 * retrieve data for)
					 *
					 * @tparam I The type of the indexing scheme
					 * @tparam T The type of the stored node data
					 *
					 * @return An error status indicating the success or failure of the operation
					 * @retval cupcfd::error::E_SUCCESS The method completed successfully
					 * @retval cupcfd::error::E_SOURCE_MISSING This data is missing from the source
					 */
					__attribute__((warn_unused_result))
					virtual cupcfd::error::eCodes getFaceNormal(euc::EuclideanVector<T,3> * faceNormal, I nFaceNormal,
																	 L * faceLabels, I nFaceLabels) = 0;

					/**
					 *
					 * @param faceLabels An array of labels for the faces we wish to retrieve data for,
					 * zero-based as per the general label scheme.
					 * @param nFaceLabels The size of the faceLabels array (i.e. the number of faces to
					 * retrieve data for)
					 *
					 * @tparam I The type of the indexing scheme
					 * @tparam T The type of the stored node data
					 *
					 * @return An error status indicating the success or failure of the operation
					 * @retval cupcfd::error::E_SUCCESS The method completed successfully
					 * @retval cupcfd::error::E_SOURCE_MISSING This data is missing from the source
					 */
					__attribute__((warn_unused_result))
					virtual cupcfd::error::eCodes getFaceCenter(euc::EuclideanPoint<T,3> * faceCenter, I nFaceCenter, L * faceLabels, I nFaceLabels) = 0;

					/**
					 * Get the vertex labels associated with a face in a CSR format.
					 *
					 * The vertices should be ordered such that is a an edge going from vertex[0] - > vertex[1], vertex[1]->vertex[2]... vertex[n-1]->vertex[0].
					 */
					__attribute__((warn_unused_result))
					virtual cupcfd::error::eCodes getFaceVerticesLabelsCSR(I * csrIndices, I nCsrIndices, I * csrData, I nCsrData,  L * faceLabels, I nFaceLabels) = 0;

					/**
					 *
					 * @param vertexLabels An array of labels for the vertices we wish to retrieve data for,
					 * zero-based as per the general label scheme.
					 * @param nVertexLabels The size of the vertexLabels array (i.e. the number of vertices to
					 * retrieve data for)
					 *
					 * @tparam I The type of the indexing scheme
					 * @tparam T The type of the stored node data
					 *
					 * @return An error status indicating the success or failure of the operation
					 * @retval cupcfd::error::E_SUCCESS The method completed successfully
					 * @retval cupcfd::error::E_SOURCE_MISSING This data is missing from the source
					 */
					__attribute__((warn_unused_result))
					virtual cupcfd::error::eCodes getVertexCoords(euc::EuclideanPoint<T,3> * vertCoords, I nVertCoords, L * vertexLabels, I nVertexLabels) = 0;

					/**
					 *
					 * @param boundaryLabels An array of labels for the boundaries we wish to retrieve data for,
					 * zero-based as per the general label scheme.
					 * @param nBoundaryLabels The size of the boundaryLabels array (i.e. the number of boundaries to
					 * retrieve data for)
					 *
					 * @tparam I The type of the indexing scheme
					 * @tparam T The type of the stored node data
					 *
					 * @return An error status indicating the success or failure of the operation
					 * @retval cupcfd::error::E_SUCCESS The method completed successfully
					 * @retval cupcfd::error::E_SOURCE_MISSING This data is missing from the source
					 */
					__attribute__((warn_unused_result))
					virtual cupcfd::error::eCodes getBoundaryFaceLabels(I * boundaryFaceLabels, I nBoundaryFaceLabels, L * boundaryLabels, I nBoundaryLabels) = 0;

					/**
					 *
					 */
					__attribute__((warn_unused_result))
					virtual cupcfd::error::eCodes getBoundaryNVertices(I * nVertices, I nNVertices, L * boundaryLabels, I nBoundaryLabels) = 0;

					/**
					 *
					 * @param boundaryLabels An array of labels for the boundaries we wish to retrieve data for,
					 * zero-based as per the general label scheme.
					 * @param nBoundaryLabels The size of the boundaryLabels array (i.e. the number of boundaries to
					 * retrieve data for)
					 *
					 * @tparam I The type of the indexing scheme
					 * @tparam T The type of the stored node data
					 *
					 * @return An error status indicating the success or failure of the operation
					 * @retval cupcfd::error::E_SUCCESS The method completed successfully
					 * @retval cupcfd::error::E_SOURCE_MISSING This data is missing from the source
					 */
					__attribute__((warn_unused_result))
					virtual cupcfd::error::eCodes getBoundaryRegionLabels(I * boundaryRegionLabels, I nBoundaryRegionLabels, L * boundaryLabels, I nBoundaryLabels) = 0;

					/**
					 * Get the vertex labels associated with a face in a CSR format.
					 * The vertices should be ordered such that is a an edge going from vertex[0] - > vertex[1], vertex[1]->vertex[2]... vertex[n-1]->vertex[0].
					 */
					__attribute__((warn_unused_result))
					virtual cupcfd::error::eCodes getBoundaryVerticesLabelsCSR(I * csrIndices, I nCsrIndices, I * csrData, I nCsrData,  L * faceLabels, I nFaceLabels) = 0;

					/**
					 *
					 * @param boundaryLabels An array of labels for the boundaries we wish to retrieve data for,
					 * zero-based as per the general label scheme.
					 * @param nBoundaryLabels The size of the boundaryLabels array (i.e. the number of boundaries to
					 * retrieve data for)
					 *
					 * @tparam I The type of the indexing scheme
					 * @tparam T The type of the stored node data
					 *
					 * @return An error status indicating the success or failure of the operation
					 * @retval cupcfd::error::E_SUCCESS The method completed successfully
					 * @retval cupcfd::error::E_SOURCE_MISSING This data is missing from the source
					 */
					__attribute__((warn_unused_result))
					virtual cupcfd::error::eCodes getBoundaryDistance(T * boundaryDistance, I nBoundaryDistance, L * boundaryLabels, I nBoundaryLabels) = 0;

					/**
					 *
					 * @param boundaryLabels An array of labels for the boundaries we wish to retrieve data for,
					 * zero-based as per the general label scheme.
					 * @param nBoundaryLabels The size of the boundaryLabels array (i.e. the number of boundaries to
					 * retrieve data for)
					 *
					 * @tparam I The type of the indexing scheme
					 * @tparam T The type of the stored node data
					 *
					 * @return An error status indicating the success or failure of the operation
					 * @retval cupcfd::error::E_SUCCESS The method completed successfully
					 * @retval cupcfd::error::E_SOURCE_MISSING This data is missing from the source
					 */
					__attribute__((warn_unused_result))
					virtual cupcfd::error::eCodes getRegionName(std::string * names, I nNames, L * regionLabels, I nRegionLabels) = 0;

					// === Concrete Methods ===

					/**
					 * Build a Distributed Adjacency List Connectivity Graph from a Mesh Source
					 * that uses mesh cell labels (as provided by the source)
					 * as nodes and mesh faces as undirected edges.
					 *
					 * This function will create the distributed adjacency list object as well
					 * as populate it with data and finalise it ready for use.
					 *
					 * Each mesh cell is assigned a single process as an owner. If an edge (face)
					 * exists between a process owned node/cell and a node/cell owned by another
					 * process, that off-process node is added as a ghost cell/node on the
					 * current process.
					 *
					 * The nodes in the graph will consist of the cell LABELS -
					 * i.e. they are not guaranteed to be zero based or incremental in nature.
					 * If such an index scheme is required, the distributed graph object has
					 * both a local index scheme (for local index assignments of nodes) and a
					 * global index scheme that is both zero-based and incremental across all ranks
					 * (useful for some systems such as partitioners). These are not currently exposed
					 * by getter function (TODO), but are accessible from the public internal data
					 * structures of connGraph and nodeToGlobal/globalToNode after finalising.
					 *
					 * The nodes assigned are determined by the provided labels. The node labels should
					 * be unique to a rank - they should not be assigned on more than one rank.
					 *
					 * @param graph A pointer to where the newly created graph object pointer will be stored.
					 * @param comm The communicator of all processes that will be members of the distributed graph
					 * @param source The source of the mesh data that contains the connectivity information.
					 * @param cellLabels The labels of the cells to be assigned to this process rank from the
					 * mesh source
					 * @param nCellLabels The size of cellLabels in the number of elements of type I
					 *
					 * @tparam I The type of the indexing scheme/node labels
					 * @tparam T The type of the stored mesh data
					 *
					 * @return An error status indicating the success or failure of the operation
					 * @retval cupcfd::error::E_SUCCESS The method completed successfully
					 */
					__attribute__((warn_unused_result))
					cupcfd::error::eCodes buildDistributedAdjacencyList(cupcfd::data_structures::DistributedAdjacencyList<I,I>& graph,
																		L *cellLabels, I nCellLabels);

					/**
					 * Build a Distributed Adjacency List Connectivity Graph from a Mesh Source
					 * that uses mesh cells as nodes and mesh faces as undirected edges.
					 *
					 * This function will create the distributed adjacency list object as well
					 * as populate it with data and finalise it ready for use.
					 *
					 * Each mesh cell is assigned a single process as an owner. If an edge (face)
					 * exists between a process owned node/cell and a node/cell owned by another
					 * process, that off-process node is added as a ghost cell/node on the
					 * current process.
					 *
					 * The nodes in the graph will consist of the cell LABELS -
					 * i.e. they are not guaranteed to be zero based or incremental in nature.
					 * If such an index scheme is required, the distributed graph object has
					 * both a local index scheme (for local index assignments of nodes) and a
					 * global index scheme that is both zero-based and incremental across all ranks
					 * (useful for some systems such as partitioners). These are not currently exposed
					 * by getter function (TODO), but are accessible from the public internal data
					 * structures of connGraph and nodeToGlobal/globalToNode after finalising.
					 *
					 * This function makes no guarantees that the graph is optimally distributed,
					 * only that it contains the correct connectivity information. It will attempt
					 * to even distribute the cells however since this may aid in partitioning efforts.
					 *
					 * @param graph A pointer to where the newly created graph object pointer will be stored.
					 * @param comm The communicator of all processes that will be members of the distributed graph
					 * @param source The source of the mesh data that contains the connectivity information.
					 *
					 * @tparam I The type of the indexing scheme/node labels
					 * @tparam T The type of the stored mesh data
					 *
					 * @return An error status indicating the success or failure of the operation
					 * @retval cupcfd::error::E_SUCCESS The method completed successfully
					 */
					__attribute__((warn_unused_result))
					cupcfd::error::eCodes buildDistributedAdjacencyList(cupcfd::data_structures::DistributedAdjacencyList<I,I>& graph,
																		cupcfd::comm::Communicator& comm);
			};
		}
	}
}

// Include Header Level Definitions
#include "MeshSource.ipp"

#endif
