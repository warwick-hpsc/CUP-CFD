/*
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
 * Contains the definitions for the MeshHDF5Source Class.
 */

#include <string>

// Function/Class Declarations
#include "MeshHDF5Source.h"

// Utility Functions
#include "ArrayDrivers.h"
#include "SortDrivers.h"
#include "StatisticsDrivers.h"

// For access to HDF5 Interface
#include "HDF5Access.h"
#include "HDF5Properties.h"
#include "HDF5Record.h"

// ToDo: Most of these accessors could be compressed further into generic HDF5 reads for full datasets etc.

// For the labelling scheme of this file, we just map it 1->1 where the input zero-based label/index = the HDF5 index/label
// Since the file stores 1-based indexes, we convert them to zero-based indexes before returning them from functions.

namespace cupcfd
{
	namespace geometry
	{
		namespace mesh
		{
			template <class I, class T>
			MeshHDF5Source<I,T>::MeshHDF5Source(std::string filePath)
			: MeshSource<I,T,I>()
			{
				this->fileName = filePath;
				this->gidBase = 1;
			}

			template <class I, class T>
			MeshHDF5Source<I,T>::~MeshHDF5Source()
			{
			}

			template <class I, class T>
			cupcfd::error::eCodes MeshHDF5Source<I,T>::getAttribute(std::string attrName, int * out) {
				// This format stores its attributes at the root level
				cupcfd::io::hdf5::HDF5Record record("/",attrName,true);
				cupcfd::io::hdf5::HDF5Access access(this->fileName, record);
				access.readData(out);
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T>
			cupcfd::error::eCodes MeshHDF5Source<I,T>::getAttribute(std::string attrName, float * out) {
				// This format stores its attributes at the root level
				cupcfd::io::hdf5::HDF5Record record("/",attrName,true);
				cupcfd::io::hdf5::HDF5Access access(this->fileName, record);
				access.readData(out);
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T>
			cupcfd::error::eCodes MeshHDF5Source<I,T>::getAttribute(std::string attrName, double * out) {
				// This format stores its attributes at the root level
				cupcfd::io::hdf5::HDF5Record record("/",attrName,true);
				cupcfd::io::hdf5::HDF5Access access(this->fileName, record);
				access.readData(out);
				return cupcfd::error::E_SUCCESS;
			}

			// === Inherited Overloaded Methods ===

			template <class I, class T>
			cupcfd::error::eCodes MeshHDF5Source<I,T>::getCellLabels(I * labels, I nLabels, I * indexes, I nIndexes) {
				if (nLabels != nIndexes) {
					return cupcfd::error::E_ARRAY_MISMATCH_SIZE;
				}

				// Labels in this format are just numerically ascending, but indexed from 1 so adjust from zero-based indexes.
				for(I i = 0; i < nIndexes; i++) {
					labels[i] = indexes[i] + 1;
				}

				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T>
			cupcfd::error::eCodes MeshHDF5Source<I,T>::getFaceLabels(I * labels, I nLabels, I * indexes, I nIndexes) {
				if (nLabels != nIndexes) {
					return cupcfd::error::E_ARRAY_MISMATCH_SIZE;
				}

				// Labels in this format are just numerically ascending, but indexed from 1 so adjust from zero-based indexes.
				for(I i = 0; i < nIndexes; i++) {
					labels[i] = indexes[i] + 1;
				}

				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T>
			cupcfd::error::eCodes MeshHDF5Source<I,T>::getVertexLabels(I * labels, I nLabels, I * indexes, I nIndexes) {
				if (nLabels != nIndexes) {
					return cupcfd::error::E_ARRAY_MISMATCH_SIZE;
				}

				// Labels in this format are just numerically ascending, but indexed from 1 so adjust from zero-based indexes.
				for(I i = 0; i < nIndexes; i++) {
					labels[i] = indexes[i] + 1;
				}

				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T>
			cupcfd::error::eCodes MeshHDF5Source<I,T>::getBoundaryLabels(I * labels, I nLabels, I * indexes, I nIndexes) {
				if (nLabels != nIndexes) {
					return cupcfd::error::E_ARRAY_MISMATCH_SIZE;
				}

				// Labels in this format are just numerically ascending, but indexed from 1 so adjust from zero-based indexes.
				for(I i = 0; i < nIndexes; i++) {
					labels[i] = indexes[i] + 1;
				}

				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T>
			cupcfd::error::eCodes MeshHDF5Source<I,T>::getRegionLabels(I * labels, I nLabels, I * indexes, I nIndexes) {
				if (nLabels != nIndexes) {
					return cupcfd::error::E_ARRAY_MISMATCH_SIZE;
				}

				// Labels in this format are just numerically ascending, but indexed from 1 so adjust from zero-based indexes.
				for(I i = 0; i < nIndexes; i++) {
					labels[i] = indexes[i] + 1;
				}

				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T>
			cupcfd::error::eCodes MeshHDF5Source<I,T>::getCellCount(I * cellCount) {
				this->getAttribute("ncel", cellCount);
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T>
			cupcfd::error::eCodes MeshHDF5Source<I,T>::getFaceCount(I * faceCount) {
				this->getAttribute("nfac", faceCount);
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T>
			cupcfd::error::eCodes MeshHDF5Source<I,T>::getBoundaryCount(I * boundaryCount) {
				this->getAttribute("nbnd", boundaryCount);
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T>
			cupcfd::error::eCodes MeshHDF5Source<I,T>::getRegionCount(I * regionCount) {
				this->getAttribute("nreg", regionCount);
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T>
			cupcfd::error::eCodes MeshHDF5Source<I,T>::getVertexCount(I * vertexCount) {
				this->getAttribute("nvrt", vertexCount);
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T>
			cupcfd::error::eCodes MeshHDF5Source<I,T>::getMaxFaceCount(I * maxFaceCount) {
				this->getAttribute("maxfaces", maxFaceCount);
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T>
			cupcfd::error::eCodes MeshHDF5Source<I,T>::getMaxVertexCount(I * maxVertexCount) {
				// Max vertex count per cell/boundary currently always hardcoded at 4
				*maxVertexCount = 4;
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T>
			cupcfd::error::eCodes MeshHDF5Source<I,T>::getCellNFaces(I * nFaces, I nNFaces, I * cellLabels, I nCellLabels) {
				// Size Check
				if (nNFaces != nCellLabels) {
					return cupcfd::error::E_ARRAY_MISMATCH_SIZE;
				}

				// Setup Access
				cupcfd::io::hdf5::HDF5Record record("/cell", "nfaces" , false);
				cupcfd::io::hdf5::HDF5Access access(this->fileName, record);
				cupcfd::io::hdf5::HDF5Properties properties(access);

				// Size vs Properties Size Check


				// Select Indexes
				//Labels in this file are index 1, HDF5 requires index 0, so correct for the offset.
				for(I i = 0; i < nCellLabels; i++) {
					properties.addIndex(cellLabels[i] - 1);
				}

				access.readData(nFaces, properties);

				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T>
			cupcfd::error::eCodes MeshHDF5Source<I,T>::getCellVolume(T * cellVol, I nCellVol, I * cellLabels, I nCellLabels) {
				// Size Check
				if (nCellVol != nCellLabels) {
					return cupcfd::error::E_ARRAY_MISMATCH_SIZE;
				}

				// Setup Access
				cupcfd::io::hdf5::HDF5Record record("/cell", "vol" , false);
				cupcfd::io::hdf5::HDF5Access access(this->fileName, record);
				cupcfd::io::hdf5::HDF5Properties properties(access);

				// Select Indexes
				//Labels in this file are index 1, HDF5 requires index 0, so correct for the offset.
				for(I i = 0; i < nCellLabels; i++) {
					properties.addIndex(cellLabels[i] - 1);
				}

				access.readData(cellVol, properties);

				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T>
			cupcfd::error::eCodes MeshHDF5Source<I,T>::getCellCenter(cupcfd::geometry::euclidean::EuclideanPoint<T,3> * cellCenter, I nCellCenter, I * cellLabels, I nCellLabels) {
				// Size Check
				if (nCellCenter != nCellLabels) {
					return cupcfd::error::E_ARRAY_MISMATCH_SIZE;
				}

				// Setup Access
				cupcfd::io::hdf5::HDF5Record record("/cell", "x" , false);
				cupcfd::io::hdf5::HDF5Access access(this->fileName, record);
				cupcfd::io::hdf5::HDF5Properties propertiesX(access);
				cupcfd::io::hdf5::HDF5Properties propertiesY(access);
				cupcfd::io::hdf5::HDF5Properties propertiesZ(access);

				// Labels in this file are index 1, HDF5 requires index 0, so correct for the offset.
				for(I i = 0; i < nCellLabels; i++) {
					propertiesX.addIndex(cellLabels[i] - 1, 0);
					propertiesY.addIndex(cellLabels[i] - 1, 1);
					propertiesZ.addIndex(cellLabels[i] - 1, 2);
				}

				// Think this could be condensed into a single read by adding all indexes to one property.
				// Temp store
				T * destX = (T *) malloc(sizeof(T) * nCellLabels);
				T * destY = (T *) malloc(sizeof(T) * nCellLabels);
				T * destZ = (T *) malloc(sizeof(T) * nCellLabels);

				access.readData(destX, propertiesX);
				access.readData(destY, propertiesY);
				access.readData(destZ, propertiesZ);

				for(I i = 0; i < nCellLabels; i++) {
					cellCenter[i].cmp[0] = destX[i];
					cellCenter[i].cmp[1] = destY[i];
					cellCenter[i].cmp[2] = destZ[i];
				}

				free(destX);
				free(destY);
				free(destZ);

				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T>
			cupcfd::error::eCodes MeshHDF5Source<I,T>::getCellFaceLabels(I * csrIndices, I nCsrIndices, I * csrData, I nCsrData, I * cellLabels, I nCellLabels) {
				// Check Size of Arrays are suitable
				if (nCsrIndices != (nCellLabels+1)) {
					return cupcfd::error::E_ARRAY_MISMATCH_SIZE;
				}

				// ================================================================
				// We need the nFace data to know how many faces there are.
				// ================================================================
				// Max Faces
				I maxFaceCount;
				this->getMaxFaceCount(&maxFaceCount);

				I * nFaces = (I *) malloc(sizeof(I) * nCellLabels);
				this->getCellNFaces(nFaces, nCellLabels, cellLabels, nCellLabels);

				I nFacesTotal = 0;
				for(I i=0; i<nCellLabels; i++) {
					nFacesTotal += nFaces[i];
				}
				if (nCsrData != nFacesTotal) {
					return cupcfd::error::E_ARRAY_MISMATCH_SIZE;
				}

				// Setup Access
				cupcfd::io::hdf5::HDF5Record record("/cell", "cface" , false);
				cupcfd::io::hdf5::HDF5Access access(this->fileName, record);
				cupcfd::io::hdf5::HDF5Properties properties(access);

				// Read Data
				I * cellFaceIndexes = (I *) malloc(sizeof(I) * maxFaceCount * nCellLabels);

				// Labels in this file are index 1, HDF5 requires index 0, so correct for the offset.
				for(I i = 0; i < nCellLabels; i++) {
					for(I j = 0; j < maxFaceCount; j++) {
						properties.addIndex(cellLabels[i] - 1, j);
					}
				}

				access.readData(cellFaceIndexes, properties);

				// Set this to -value for index 0, so we don't have to extract the first loop.
				I ptr = 0;

				for(I i=0; i<nCellLabels; i++) {
					// Setup the csrIndices part of the csr array
					csrIndices[i] = ptr;

					// Loop over nFaces
					// This will skip 'garbage data' where space is allocated up to maxFaceCount
					// but a cells has fewer than maxFaceCount faces (e.g. max of 6, but only 4 faces)
					// since HDF5 files have to make space regardless.
					for(I j=0; j<nFaces[i];j++) {
						csrData[ptr + j] = cellFaceIndexes[(i * maxFaceCount) + j];
					}

					ptr += nFaces[i];
				}

				// csrIndices array size is 1 greater than total number of cells (for CSR formatting)
				// Last entry holds total number of adjacent faces
				csrIndices[nCellLabels] = ptr;

				free(cellFaceIndexes);
				free(nFaces);

				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T>
			cupcfd::error::eCodes MeshHDF5Source<I,T>::getFaceIsBoundary(bool * isBoundary, I nIsBoundary, I * faceLabels, I nFaceLabels) {
				// Size Check
				if (nIsBoundary != nFaceLabels) {
					return cupcfd::error::E_ARRAY_MISMATCH_SIZE;
				}

				// If Cell 2 is 0, then it is a boundary face.

				// Temporary store for cell 2 data
				I * cell2Data = (I *) malloc(sizeof(I) * nFaceLabels);

				cupcfd::io::hdf5::HDF5Record record("/face", "cell2" , false);
				cupcfd::io::hdf5::HDF5Access access(this->fileName, record);
				cupcfd::io::hdf5::HDF5Properties properties(access);

				// Select Indexes
				// Labels in this file are stored as index 1, HDF5 requires index 0, so correct for the offset.
				for(I i = 0; i < nFaceLabels; i++) {
					properties.addIndex(faceLabels[i] - 1);
				}

				access.readData(cell2Data, properties);

				for(I i = 0; i < nFaceLabels; i++) {
					isBoundary[i] = (cell2Data[i] == 0);
				}

				free(cell2Data);

				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T>
			cupcfd::error::eCodes MeshHDF5Source<I,T>::getFaceNVertices(I * nVertices, I nNVertices, I * faceLabels, I nFaceLabels) {
				// Error Check: Size Check
				if (nNVertices != nFaceLabels) {
					return cupcfd::error::E_ARRAY_MISMATCH_SIZE;
				}

				// Setup Access
				cupcfd::io::hdf5::HDF5Record record("/face", "vertices" , false);
				cupcfd::io::hdf5::HDF5Access access(this->fileName, record);
				cupcfd::io::hdf5::HDF5Properties propertiesVert(access);

				// Select Indexes
				// Labels in this file are index 1, HDF5 requires index 0, so correct for the offset.
				// Since this format only ever has a maximum of 4, and we can't have less than 3 vertices per face,
				// we only need to check the 4th position.
				for(I i = 0; i < nFaceLabels; i++) {
					propertiesVert.addIndex(faceLabels[i] - 1, 3);
				}

				I * vert4 = (I *) malloc(sizeof(I) * nFaceLabels);
				access.readData(vert4, propertiesVert);

				for(I i = 0; i < nFaceLabels; i++) {
					// this format treats a stored value of 0 as 'No Label/No Vertex'
					if(vert4[i] == 0) {
						nVertices[i] = 3;
					} else {
						nVertices[i] = 4;
					}
				}

				free(vert4);

				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T>
			cupcfd::error::eCodes MeshHDF5Source<I,T>::getFaceBoundaryLabels(I * faceBoundaryLabels, I nFaceBoundaryLabels, I * faceLabels, I nFaceLabels) {
				cupcfd::error::eCodes status;

				// Error Check: Size Check
				if (nFaceBoundaryLabels != nFaceLabels) {
					return cupcfd::error::E_ARRAY_MISMATCH_SIZE;
				}

				// Error Check: Check they are all boundary faces
				bool * isBoundary = (bool *) malloc(sizeof(bool) * nFaceLabels);

				status = getFaceIsBoundary(isBoundary, nFaceLabels, faceLabels, nFaceLabels);
				CHECK_ERROR_CODE(status)
				if(status != cupcfd::error::E_SUCCESS) return status;

				for(I i = 0; i < nFaceLabels; i++) {
					if(isBoundary[i] == false) {
						free(isBoundary);
						return cupcfd::error::E_ERROR;
					}
				}

				free(isBoundary);

				// Setup Access
				cupcfd::io::hdf5::HDF5Record record("/face", "bnd" , false);
				cupcfd::io::hdf5::HDF5Access access(this->fileName, record);
				cupcfd::io::hdf5::HDF5Properties properties(access);

				// Labels in this file are stored as index 1, HDF5 requires index 0, so correct for the offset.
				for(I i = 0; i < nFaceLabels; i++) {
					properties.addIndex(faceLabels[i] - 1);
				}

				access.readData(faceBoundaryLabels, properties);

				// HDF5	Interface Error Check

				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T>
			cupcfd::error::eCodes MeshHDF5Source<I,T>::getFaceCell1Labels(I * faceCell1Labels, I nFaceCell1Labels, I * faceLabels, I nFaceLabels) {
				// Size Check
				if (nFaceCell1Labels != nFaceLabels) {
					return cupcfd::error::E_ARRAY_MISMATCH_SIZE;
				}

				// Setup Access
				cupcfd::io::hdf5::HDF5Record record("/face", "cell1" , false);
				cupcfd::io::hdf5::HDF5Access access(this->fileName, record);
				cupcfd::io::hdf5::HDF5Properties properties(access);

				// Select Indexes
				// Labels in this file are stored as index 1, HDF5 requires index 0, so correct for the offset.
				for(I i = 0; i < nFaceLabels; i++) {
					properties.addIndex(faceLabels[i] - 1);
				}

				access.readData(faceCell1Labels, properties);

				// HDF5	Interface Error Check

				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T>
			cupcfd::error::eCodes MeshHDF5Source<I,T>::getFaceCell2Labels(I * faceCell2Labels, I nFaceCell2Labels, I * faceLabels, I nFaceLabels) {
				// Size Check
				if (nFaceCell2Labels != nFaceLabels) {
					return cupcfd::error::E_ARRAY_MISMATCH_SIZE;
				}

				// Setup Access
				cupcfd::io::hdf5::HDF5Record record("/face", "cell2" , false);
				cupcfd::io::hdf5::HDF5Access access(this->fileName, record);
				cupcfd::io::hdf5::HDF5Properties properties(access);

				// Select Indexes
				// Labels in this file are stored as index 1, HDF5 requires index 0, so correct for the offset.
				for(I i = 0; i < nFaceLabels; i++)
				{
					properties.addIndex(faceLabels[i] - 1);
				}

				access.readData(faceCell2Labels, properties);

				for(I i = 0; i < nFaceLabels; i++) {
					// ToDo: Would like to not return -1 at all, but raise an error instead...
					// use -1 as a flag currently (since 0 could be a valid label) but technically
					// -1 could also be a valid label in some formats (OK here though since this
					// format shouldn't use -1 as a valid label).
					if(faceCell2Labels[i] == 0) {
						faceCell2Labels[i] = -1;
					}
				}

				// ToDo: HDF5 Interface Error Check

				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T>
			cupcfd::error::eCodes MeshHDF5Source<I,T>::getFaceArea(T * faceLambda, I nFaceLambda, I * faceLabels, I nFaceLabels) {
				// Size Check
				if (nFaceLambda != nFaceLabels) {
					return cupcfd::error::E_ARRAY_MISMATCH_SIZE;
				}

				// Setup Access
				cupcfd::io::hdf5::HDF5Record record("/face", "area" , false);
				cupcfd::io::hdf5::HDF5Access access(this->fileName, record);
				cupcfd::io::hdf5::HDF5Properties properties(access);

				// Select Indexes
				// Labels in this file are index 1, HDF5 requires index 0, so correct for the offset.
				for(I i = 0; i < nFaceLabels; i++) {
					properties.addIndex(faceLabels[i] - 1);
				}

				access.readData(faceLambda, properties);

				// HDF5	Interface Error Check

				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T>
			cupcfd::error::eCodes MeshHDF5Source<I,T>::getFaceLambda(T * faceLambda, I nFaceLambda, I * faceLabels, I nFaceLabels) {
				// Size Check
				if (nFaceLambda != nFaceLabels) {
					return cupcfd::error::E_ARRAY_MISMATCH_SIZE;
				}

				// Setup Access
				cupcfd::io::hdf5::HDF5Record record("/face", "lambda" , false);
				cupcfd::io::hdf5::HDF5Access access(this->fileName, record);
				cupcfd::io::hdf5::HDF5Properties properties(access);

				// Select Indexes
				// Labels in this file are index 1, HDF5 requires index 0, so correct for the offset.
				for(I i = 0; i < nFaceLabels; i++) {
					properties.addIndex(faceLabels[i] - 1);
				}

				access.readData(faceLambda, properties);

				// HDF5	Interface Error Check

				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T>
			cupcfd::error::eCodes MeshHDF5Source<I,T>::getFaceNormal(cupcfd::geometry::euclidean::EuclideanVector<T,3> * faceNormal, I nFaceNormal, I * faceLabels, I nFaceLabels) {
				// Size Check
				if (nFaceNormal != nFaceLabels) {
					return cupcfd::error::E_ARRAY_MISMATCH_SIZE;
				}

				// Setup Access
				cupcfd::io::hdf5::HDF5Record record("/face", "n" , false);
				cupcfd::io::hdf5::HDF5Access access(this->fileName, record);
				cupcfd::io::hdf5::HDF5Properties propertiesX(access);
				cupcfd::io::hdf5::HDF5Properties propertiesY(access);
				cupcfd::io::hdf5::HDF5Properties propertiesZ(access);

				// Select Indexes
				// Labels in this file are index 1, HDF5 requires index 0, so correct for the offset.
				for(I i = 0; i < nFaceLabels; i++) {
					propertiesX.addIndex(faceLabels[i] - 1, 0);
					propertiesY.addIndex(faceLabels[i] - 1, 1);
					propertiesZ.addIndex(faceLabels[i] - 1, 2);
				}

				// Think this could be condensed into a single read by adding all indexes to one property.
				// Temp store
				T * destX = (T *) malloc(sizeof(T) * nFaceLabels);
				T * destY = (T *) malloc(sizeof(T) * nFaceLabels);
				T * destZ = (T *) malloc(sizeof(T) * nFaceLabels);

				access.readData(destX, propertiesX);
				access.readData(destY, propertiesY);
				access.readData(destZ, propertiesZ);

				for(I i = 0; i < nFaceLabels; i++) {
					faceNormal[i].cmp[0] = destX[i];
					faceNormal[i].cmp[1] = destY[i];
					faceNormal[i].cmp[2] = destZ[i];
				}

				free(destX);
				free(destY);
				free(destZ);

				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T>
			cupcfd::error::eCodes MeshHDF5Source<I,T>::getFaceCenter(cupcfd::geometry::euclidean::EuclideanPoint<T,3> * faceCenter, I nFaceCenter, I * faceLabels, I nFaceLabels) {
				// Size Check
				if (nFaceCenter != nFaceLabels) {
					return cupcfd::error::E_ARRAY_MISMATCH_SIZE;
				}

				// Setup Access
				cupcfd::io::hdf5::HDF5Record record("/face", "x" , false);
				cupcfd::io::hdf5::HDF5Access access(this->fileName, record);
				cupcfd::io::hdf5::HDF5Properties propertiesX(access);
				cupcfd::io::hdf5::HDF5Properties propertiesY(access);
				cupcfd::io::hdf5::HDF5Properties propertiesZ(access);

				// Select Indexes
				// Labels in this file are index 1, HDF5 requires index 0, so correct for the offset.
				for(I i = 0; i < nFaceLabels; i++) {
					propertiesX.addIndex(faceLabels[i] - 1, 0);
					propertiesY.addIndex(faceLabels[i] - 1, 1);
					propertiesZ.addIndex(faceLabels[i] - 1, 2);
				}

				// Think this could be condensed into a single read by adding all indexes to one property.
				// Temp store
				T * destX = (T *) malloc(sizeof(T) * nFaceLabels);
				T * destY = (T *) malloc(sizeof(T) * nFaceLabels);
				T * destZ = (T *) malloc(sizeof(T) * nFaceLabels);

				access.readData(destX, propertiesX);
				access.readData(destY, propertiesY);
				access.readData(destZ, propertiesZ);

				for(I i = 0; i < nFaceLabels; i++) {
					faceCenter[i].cmp[0] = destX[i];
					faceCenter[i].cmp[1] = destY[i];
					faceCenter[i].cmp[2] = destZ[i];
				}

				free(destX);
				free(destY);
				free(destZ);

				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T>
			cupcfd::error::eCodes MeshHDF5Source<I,T>::getFaceVerticesLabelsCSR(I * csrIndices, I nCsrIndices, I * csrData, I nCsrData,  I * faceLabels, I nFaceLabels) {
				if (nCsrIndices != (nFaceLabels+1)) {
					return cupcfd::error::E_ARRAY_MISMATCH_SIZE;
				}

				// ToDo: This can be faster: Using these function as they are currently written will reload the same data more than once.
				I nFaces = nFaceLabels;
				I * nFaceVertexCount = (I *) malloc(sizeof(I) * nFaces);
				this->getFaceNVertices(nFaceVertexCount, nFaces, faceLabels, nFaceLabels);

				I nVerticesTotal = 0;
				for(I i = 0; i < nFaces; i++) {
					if(nFaceVertexCount[i] == 3) {
						nVerticesTotal += 3;
					} else {
						// Assuming must be 4 vertices, 1 or 2 are not valid
						// Presumes file data is correct of course, but if it isn't then
						// there are likely many, many possible error outcomes.
						nVerticesTotal += 4;
					}
				}
				if (nVerticesTotal != nCsrData) {
					return cupcfd::error::E_ARRAY_MISMATCH_SIZE;
				}

				// Update indices array
				I ptr = 0;
				for(I i = 0; i < nFaces; i++) {
					csrIndices[i] = ptr;
					ptr += nFaceVertexCount[i];
				}

				// Last position has 1 past the last valid index (csrIndices size should be nFaces + 1)
				csrIndices[nFaces] = ptr;

				// Update CSR Data array
				I * vert1 = (I *) malloc(sizeof(I) * nFaces);
				I * vert2 = (I *) malloc(sizeof(I) * nFaces);
				I * vert3 = (I *) malloc(sizeof(I) * nFaces);
				I * vert4 = (I *) malloc(sizeof(I) * nFaces);

				cupcfd::io::hdf5::HDF5Record record("/face", "vertices" , false);
				cupcfd::io::hdf5::HDF5Access access(this->fileName, record);
				cupcfd::io::hdf5::HDF5Properties propertiesVert1(access);
				cupcfd::io::hdf5::HDF5Properties propertiesVert2(access);
				cupcfd::io::hdf5::HDF5Properties propertiesVert3(access);
				cupcfd::io::hdf5::HDF5Properties propertiesVert4(access);

				// Select Indexes
				// Labels in this file are index 1, HDF5 requires index 0, so correct for the offset.
				for(I i = 0; i < nFaceLabels; i++) {
					propertiesVert1.addIndex(faceLabels[i] - 1, 0);
					propertiesVert2.addIndex(faceLabels[i] - 1, 1);
					propertiesVert3.addIndex(faceLabels[i] - 1, 2);
					propertiesVert4.addIndex(faceLabels[i] - 1, 3);
				}

				access.readData(vert1, propertiesVert1);
				access.readData(vert2, propertiesVert2);
				access.readData(vert3, propertiesVert3);
				access.readData(vert4, propertiesVert4);

				for(I i = 0; i < nFaces; i++) {
					I rangeStart = csrIndices[i];

					if(nFaceVertexCount[i] == 3) {
						csrData[rangeStart]     = vert1[i];
						csrData[rangeStart + 1] = vert2[i];
						csrData[rangeStart + 2] = vert3[i];
					} else {
						// Assuming must be 4 vertices, 1 or 2 are not valid
						// Presumes file data is correct of course, but if it isn't then
						// there are likely many, many possible error outcomes.
						csrData[rangeStart]     = vert1[i];
						csrData[rangeStart + 1] = vert2[i];
						csrData[rangeStart + 2] = vert3[i];
						csrData[rangeStart + 3] = vert4[i];
					}
				}

				free(vert1);
				free(vert2);
				free(vert3);
				free(vert4);
				free(nFaceVertexCount);

				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T>
			cupcfd::error::eCodes MeshHDF5Source<I,T>::getVertexCoords(cupcfd::geometry::euclidean::EuclideanPoint<T,3> * vertCoords, I nVertCoords, I * vertexLabels, I nVertexLabels) {
				// Size Check
				if (nVertCoords != nVertexLabels) {
					return cupcfd::error::E_ARRAY_MISMATCH_SIZE;
				}

				// Setup Access
				cupcfd::io::hdf5::HDF5Record record("vert" , false);
				cupcfd::io::hdf5::HDF5Access access(this->fileName, record);
				cupcfd::io::hdf5::HDF5Properties propertiesX(access);
				cupcfd::io::hdf5::HDF5Properties propertiesY(access);
				cupcfd::io::hdf5::HDF5Properties propertiesZ(access);

				// Select Indexes
				// Labels in this file are index 1, HDF5 requires index 0, so correct for the offset.
				for(I i = 0; i < nVertexLabels; i++) {
					propertiesX.addIndex(vertexLabels[i] - 1, 0);
					propertiesY.addIndex(vertexLabels[i] - 1, 1);
					propertiesZ.addIndex(vertexLabels[i] - 1, 2);
				}

				// Think this could be condensed into a single read by adding all indexes to one property.
				// Temp store
				T * destX = (T *) malloc(sizeof(T) * nVertexLabels);
				T * destY = (T *) malloc(sizeof(T) * nVertexLabels);
				T * destZ = (T *) malloc(sizeof(T) * nVertexLabels);

				access.readData(destX, propertiesX);
				access.readData(destY, propertiesY);
				access.readData(destZ, propertiesZ);

				for(I i = 0; i < nVertCoords; i++) {
					vertCoords[i].cmp[0] = destX[i];
					vertCoords[i].cmp[1] = destY[i];
					vertCoords[i].cmp[2] = destZ[i];
				}

				free(destX);
				free(destY);
				free(destZ);

				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T>
			cupcfd::error::eCodes MeshHDF5Source<I,T>::getBoundaryFaceLabels(I * boundaryFaceLabels, I nBoundaryFaceLabels, I * boundaryLabels, I nBoundaryLabels) {
				if(nBoundaryLabels > 0) {
					// Size Check
					if (nBoundaryLabels != nBoundaryFaceLabels) {
						return cupcfd::error::E_ARRAY_MISMATCH_SIZE;
					}

					// Setup Access
					cupcfd::io::hdf5::HDF5Record record("/bnd", "face" , false);
					cupcfd::io::hdf5::HDF5Access access(this->fileName, record);
					cupcfd::io::hdf5::HDF5Properties properties(access);

					// Select Indexes
					// Labels in this file are index 1, HDF5 requires index 0, so correct for the offset.
					for(I i = 0; i < nBoundaryLabels; i++) {
						properties.addIndex(boundaryLabels[i] - 1);
					}

					access.readData(boundaryFaceLabels, properties);

					// HDF5	Interface Error Check
				}

				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T>
			cupcfd::error::eCodes MeshHDF5Source<I,T>::getBoundaryNVertices(I * nVertices, I nNVertices, I * boundaryLabels, I nBoundaryLabels) {
				// ToDo: We could just read vert4 and ignore the rest....

				// Data not stored directly in file, need to read in 4 vertices and count
				I * vert4 = (I *) malloc(sizeof(I) * nBoundaryLabels);

				// Size Check
				if (nNVertices != nBoundaryLabels) {
					return cupcfd::error::E_ARRAY_MISMATCH_SIZE;
				}

				// Setup Access
				cupcfd::io::hdf5::HDF5Record record("/bnd", "vertices" , false);
				cupcfd::io::hdf5::HDF5Access access(this->fileName, record);
				cupcfd::io::hdf5::HDF5Properties propertiesVert(access);

				// Select Indexes
				// Labels in this file are index 1, HDF5 requires index 0, so correct for the offset.
				for(I i = 0; i < nBoundaryLabels; i++) {
					propertiesVert.addIndex(boundaryLabels[i] - 1, 3);
				}

				access.readData(vert4, propertiesVert);

				// Only the presence or lack of vertex 4 matters
				for(I i = 0; i < nBoundaryLabels; i++) {
					if(vert4[i] == 0) {
						nVertices[i] = 3;
					} else {
						nVertices[i] = 4;
					}
				}

				free(vert4);

				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T>
			cupcfd::error::eCodes MeshHDF5Source<I,T>::getBoundaryRegionLabels(I * boundaryRegionLabels, I nBoundaryRegionLabels, I * boundaryLabels, I nBoundaryLabels) {
				if(nBoundaryLabels > 0) {
					// Size Check
					if (nBoundaryRegionLabels != nBoundaryLabels) {
						return cupcfd::error::E_ARRAY_MISMATCH_SIZE;
					}

					// Setup Access
					cupcfd::io::hdf5::HDF5Record record("/bnd", "rid" , false);
					cupcfd::io::hdf5::HDF5Access access(this->fileName, record);
					cupcfd::io::hdf5::HDF5Properties properties(access);

					// Select Indexes
					// Labels in this file are index 1, HDF5 requires index 0, so correct for the offset.
					for(I i = 0; i < nBoundaryLabels; i++) {
						properties.addIndex(boundaryLabels[i] - 1);
					}

					access.readData(boundaryRegionLabels, properties);

					// HDF5	Interface Error Check
				}

				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T>
			cupcfd::error::eCodes MeshHDF5Source<I,T>::getBoundaryVerticesLabelsCSR(I * csrIndices, I nCsrIndices, I * csrData, I nCsrData,  I * boundaryLabels, I nBoundaryLabels) {
				// Size Check
				if (nCsrIndices != (nBoundaryLabels+1)) {
					return cupcfd::error::E_ARRAY_MISMATCH_SIZE;
				}

				// ToDo: This can be faster: Using these function as they are currently written will reload the same data more than once.
				I nBoundaries = nBoundaryLabels;

				I * nBoundaryVertexCount = (I *) malloc(sizeof(I) * nBoundaries);
				this->getBoundaryNVertices(nBoundaryVertexCount, nBoundaries, boundaryLabels, nBoundaryLabels);

				I nVerticesTotal = 0;
				for(I i = 0; i < nBoundaries; i++) {
					if(nBoundaryVertexCount[i] == 3) {
						nVerticesTotal += 3;
					} else {
						// Assuming must be 4 vertices if not 3 since this file format seems hard capped at 4, 1 or 2 are not valid.
						// Presumes file data is correct of course, but if it isn't then
						// there are likely many, many possible error outcomes.
						nVerticesTotal += 4;
					}
				}
				if (nVerticesTotal != nCsrData) {
					return cupcfd::error::E_ARRAY_MISMATCH_SIZE;
				}

				// Update indices array
				I ptr = 0;
				for(I i = 0; i < nBoundaries; i++)
				{
					csrIndices[i] = ptr;
					ptr = ptr + nBoundaryVertexCount[i];
				}

				// Last position has 1 past the last valid index (csrIndices size should be nBoundaries + 1)
				csrIndices[nBoundaries] = ptr;

				// Update CSR Data array
				I * vert1 = (I *) malloc(sizeof(I) * nBoundaries);
				I * vert2 = (I *) malloc(sizeof(I) * nBoundaries);
				I * vert3 = (I *) malloc(sizeof(I) * nBoundaries);
				I * vert4 = (I *) malloc(sizeof(I) * nBoundaries);

				cupcfd::io::hdf5::HDF5Record record("/bnd", "vertices" , false);
				cupcfd::io::hdf5::HDF5Access access(this->fileName, record);
				cupcfd::io::hdf5::HDF5Properties propertiesVert1(access);
				cupcfd::io::hdf5::HDF5Properties propertiesVert2(access);
				cupcfd::io::hdf5::HDF5Properties propertiesVert3(access);
				cupcfd::io::hdf5::HDF5Properties propertiesVert4(access);

				// Select Indexes
				// Labels in this file are index 1, HDF5 requires index 0, so correct for the offset.
				for(I i = 0; i < nBoundaryLabels; i++) {
					propertiesVert1.addIndex(boundaryLabels[i] - 1, 0);
					propertiesVert2.addIndex(boundaryLabels[i] - 1, 1);
					propertiesVert3.addIndex(boundaryLabels[i] - 1, 2);
					propertiesVert4.addIndex(boundaryLabels[i] - 1, 3);
				}

				access.readData(vert1, propertiesVert1);
				access.readData(vert2, propertiesVert2);
				access.readData(vert3, propertiesVert3);
				access.readData(vert4, propertiesVert4);

				for(I i = 0; i < nBoundaries; i++) {
					I rangeStart = csrIndices[i];

					if(nBoundaryVertexCount[i] == 3) {
						csrData[rangeStart]     = vert1[i];
						csrData[rangeStart + 1] = vert2[i];
						csrData[rangeStart + 2] = vert3[i];
					} else {
						// Assuming must be 4 vertices if not 3 since this file format seems hard capped at 4, 1 or 2 are not valid.
						// Presumes file data is correct of course, but if it isn't then
						// there are likely many, many possible error outcomes.
						csrData[rangeStart]     = vert1[i];
						csrData[rangeStart + 1] = vert2[i];
						csrData[rangeStart + 2] = vert3[i];
						csrData[rangeStart + 3] = vert4[i];
					}
				}

				free(vert1);
				free(vert2);
				free(vert3);
				free(vert4);
				free(nBoundaryVertexCount);

				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T>
			cupcfd::error::eCodes MeshHDF5Source<I,T>::getBoundaryDistance(T * boundaryDistance, I nBoundaryDistance, I * boundaryLabels, I nBoundaryLabels) {
				if(nBoundaryLabels > 0) {
					// Size Check
					if (nBoundaryDistance != nBoundaryLabels) {
						return cupcfd::error::E_ARRAY_MISMATCH_SIZE;
					}

					// Setup Access
					cupcfd::io::hdf5::HDF5Record record("/bnd", "distance" , false);
					cupcfd::io::hdf5::HDF5Access access(this->fileName, record);
					cupcfd::io::hdf5::HDF5Properties properties(access);

					// Select Indexes
					// Labels in this file are index 1, HDF5 requires index 0, so correct for the offset.
					for(I i = 0; i < nBoundaryLabels; i++) {
						properties.addIndex(boundaryLabels[i] - 1);
					}

					access.readData(boundaryDistance, properties);

					// HDF5	Interface Error Check
				}

				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T>
			cupcfd::error::eCodes MeshHDF5Source<I,T>::getRegionName(std::string * names __attribute__((unused)), I nNames __attribute__((unused)), I * regionLabels __attribute__((unused)), I nRegionLabels __attribute__((unused))) {
				// ToDo
				return cupcfd::error::E_NOT_IMPLEMENTED;
			}
		}
	}
}

// Explicit Instantiation
template class cupcfd::geometry::mesh::MeshHDF5Source<int,float>;
template class cupcfd::geometry::mesh::MeshHDF5Source<int,double>;
