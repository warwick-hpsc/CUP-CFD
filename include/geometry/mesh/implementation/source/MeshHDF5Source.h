/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Contains the declarations for the MeshHDF5Source class
 */

#ifndef CUPCFD_GEOMETRY_MESH_MESH_HDF5_SOURCE_INCLUDE_H
#define CUPCFD_GEOMETRY_MESH_MESH_HDF5_SOURCE_INCLUDE_H

// C++ Library
#include <string>

// Parent Class
#include "MeshSource.h"

// Data Structures
#include "AdjacencyListVector.h"
#include "AdjacencyListCSR.h"
#include "DistributedAdjacencyList.h"

namespace cupcfd
{
	namespace geometry
	{
		namespace mesh
		{
			/**
			 * Uses the same type as the indexing scheme for the labelling scheme.
			 */
			template <class I, class T>
			class MeshHDF5Source : public MeshSource<I,T,I>
			{
				public:
					// === Members ===
					std::string fileName;

					// === Constructors/Deconstructors ===

					/**
					 * Open the specified file and treat it as a HDF5 file
					 * with a the specific HDF5 schema.
					 *
					 * @param filePath The path to the file to open.
					 */
					MeshHDF5Source(std::string filePath);

					/**
					 * Deconstructor.
					 */
					virtual ~MeshHDF5Source();

					// === Concrete Methods ===

					/**
					 * Retrive an integer attribute from the HDF5 file.
					 *
					 * @param attrName The name of the attribute
					 * @param out A pointer to where the result will be stored.
					 *
					 * @return An error status indicating the success or failure of the operation
					 * @retval cupcfd::error::E_SUCCESS Success
					 */
					__attribute__((warn_unused_result))
					cupcfd::error::eCodes getAttribute(std::string attrName, int * out);

					/**
					 * Retrive a float attribute from the HDF5 file.
					 *
					 * @param attrName The name of the attribute
					 * @param out A pointer to where the result will be stored.
					 *
					 * @return An error status indicating the success or failure of the operation
					 * @retval cupcfd::error::E_SUCCESS Success
					 */
					__attribute__((warn_unused_result))
					cupcfd::error::eCodes getAttribute(std::string attrName, float * out);

					/**
					 * Retrive a double attribute from the HDF5 file.
					 *
					 * @param attrName The name of the attribute
					 * @param out A pointer to where the result will be stored.
					 *
					 * @return An error status indicating the success or failure of the operation
					 * @retval cupcfd::error::E_SUCCESS Success
					 */
					__attribute__((warn_unused_result))
					cupcfd::error::eCodes getAttribute(std::string attrName, double * out);

					// === Overloaded Inherited Virtual Methods ===

					__attribute__((warn_unused_result))
					cupcfd::error::eCodes getCellLabels(I * labels, I nLabels, I * indexes, I nIndexes);
					__attribute__((warn_unused_result))
					cupcfd::error::eCodes getFaceLabels(I * labels, I nLabels, I * indexes, I nIndexes);
					__attribute__((warn_unused_result))
					cupcfd::error::eCodes getVertexLabels(I * labels, I nLabels, I * indexes, I nIndexes);
					__attribute__((warn_unused_result))
					cupcfd::error::eCodes getBoundaryLabels(I * labels, I nLabels, I * indexes, I nIndexes);
					__attribute__((warn_unused_result))
					cupcfd::error::eCodes getRegionLabels(I * labels, I nLabels, I * indexes, I nIndexes);


					__attribute__((warn_unused_result))
					cupcfd::error::eCodes getCellCount(I * cellCount);
					__attribute__((warn_unused_result))
					cupcfd::error::eCodes getFaceCount(I * faceCount);
					__attribute__((warn_unused_result))
					cupcfd::error::eCodes getBoundaryCount(I * boundaryCount);
					__attribute__((warn_unused_result))
					cupcfd::error::eCodes getRegionCount(I * regionCount);
					__attribute__((warn_unused_result))
					cupcfd::error::eCodes getVertexCount(I * vertexCount);
					__attribute__((warn_unused_result))
					cupcfd::error::eCodes getMaxFaceCount(I * maxFaceCount);
					__attribute__((warn_unused_result))
					cupcfd::error::eCodes getMaxVertexCount(I * maxVertexCount);

					__attribute__((warn_unused_result))
					cupcfd::error::eCodes getCellNFaces(I * nFaces, I nNFaces, I * cellLabels, I nCellLabels);
					__attribute__((warn_unused_result))
					cupcfd::error::eCodes getCellVolume(T * cellVol, I nCellVol, I * cellLabels, I nCellLabels);
					__attribute__((warn_unused_result))
					cupcfd::error::eCodes getCellCenter(cupcfd::geometry::euclidean::EuclideanPoint<T,3> * cellCenter, I nCellCenter, I * cellLabels, I nCellLabels);
					__attribute__((warn_unused_result))
					cupcfd::error::eCodes getCellFaceLabels(I * csrIndices, I nCsrIndices, I * csrData, I nCsrData, I * cellLabels, I nCellLabels);
					__attribute__((warn_unused_result))
					cupcfd::error::eCodes getFaceIsBoundary(bool * isBoundary, I nIsBoundary, I * faceLabels, I nFaceLabels);
					__attribute__((warn_unused_result))
					cupcfd::error::eCodes getFaceNVertices(I * nVertices, I nNVertices, I * faceLabels, I nFaceLabels);
					__attribute__((warn_unused_result))
					cupcfd::error::eCodes getFaceBoundaryLabels(I * faceBoundaryLabels, I nFaceBoundaryLabels, I * faceLabels, I nFaceLabels);
					__attribute__((warn_unused_result))
					cupcfd::error::eCodes getFaceCell1Labels(I * faceCell1Labels, I nFaceCell1Labels, I * faceLabels, I nFaceLabels);
					__attribute__((warn_unused_result))
					cupcfd::error::eCodes getFaceCell2Labels(I * faceCell2Labels, I nFaceCell2Labels, I * faceLabels, I nFaceLabels);
					__attribute__((warn_unused_result))
					cupcfd::error::eCodes getFaceLambda(T * faceLambda, I nFaceLambda, I * faceLabels, I nFaceLabels);
					__attribute__((warn_unused_result))
					cupcfd::error::eCodes getFaceArea(T * faceLambda, I nFaceLambda, I * faceLabels, I nFaceLabels);
					__attribute__((warn_unused_result))
					cupcfd::error::eCodes getFaceNormal(cupcfd::geometry::euclidean::EuclideanVector<T,3> * faceNormal, I nFaceNormal, I * faceLabels, I nFaceLabels);
					__attribute__((warn_unused_result))
					cupcfd::error::eCodes getFaceCenter(cupcfd::geometry::euclidean::EuclideanPoint<T,3> * faceCenter, I nFaceCenter, I * faceLabels, I nFaceLabels);
					__attribute__((warn_unused_result))
					cupcfd::error::eCodes getFaceVerticesLabelsCSR(I * csrIndices, I nCsrIndices, I * csrData, I nCsrData,  I * faceLabels, I nFaceLabels);
					__attribute__((warn_unused_result))
					cupcfd::error::eCodes getVertexCoords(cupcfd::geometry::euclidean::EuclideanPoint<T,3> * vertCoords, I nVertCoords, I * vertexLabels, I nVertexLabels);
					__attribute__((warn_unused_result))
					cupcfd::error::eCodes getBoundaryFaceLabels(I * boundaryFaceLabels, I nBoundaryFaceLabels, I * boundaryLabels, I nBoundaryLabels);
					__attribute__((warn_unused_result))
					cupcfd::error::eCodes getBoundaryNVertices(I * nVertices, I nNVertices, I * boundaryLabels, I nBoundaryLabels);
					__attribute__((warn_unused_result))
					cupcfd::error::eCodes getBoundaryRegionLabels(I * boundaryRegionLabels, I nBoundaryRegionLabels, I * boundaryLabels, I nBoundaryLabels);
					__attribute__((warn_unused_result))
					cupcfd::error::eCodes getBoundaryVerticesLabelsCSR(I * csrIndices, I nCsrIndices, I * csrData, I nCsrData,  I * faceLabels, I nFaceLabels);
					__attribute__((warn_unused_result))
					cupcfd::error::eCodes getBoundaryDistance(T * boundaryDistance, I nBoundaryDistance, I * boundaryLabels, I nBoundaryLabels);
					__attribute__((warn_unused_result))
					cupcfd::error::eCodes getRegionName(std::string * names, I nNames, I * regionLabels, I nRegionLabels);
			};
		} // namespace mesh
	} // namespace geometry
} // namespace cupcfd

#endif
