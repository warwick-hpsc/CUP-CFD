/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Contains the definitions for the MeshStructGenSource class
 */

// Parent Class
#include "MeshStructGenSource.h"

#include <iostream>

#include "Quadrilateral3D.h"
#include "Hexahedron.h"
#include "Polygon3D.h"

namespace euc = cupcfd::geometry::euclidean;

namespace cupcfd
{
	namespace geometry
	{
		namespace mesh
		{
			// === Constructors/Deconstructors ===

			template <class I, class T>
			MeshStructGenSource<I,T>::MeshStructGenSource(I nX, I nY, I nZ, T sMinX, T sMaxX, T sMinY, T sMaxY, T sMinZ, T sMaxZ)
			:nX(nX),
			 nY(nY),
			 nZ(nZ),
			 sMinX(sMinX),
			 sMaxX(sMaxX),
			 sMinY(sMinY),
			 sMaxY(sMaxY),
			 sMinZ(sMinZ),
			 sMaxZ(sMaxZ)
			{
				// Compute Spatial Division
				this->dSx = (this->sMaxX - this->sMinX) / T(this->nX);
				this->dSy = (this->sMaxY - this->sMinY) / T(this->nY);
				this->dSz = (this->sMaxZ - this->sMinZ) / T(this->nZ);
			}

			template <class I, class T>
			MeshStructGenSource<I,T>::~MeshStructGenSource()
			{

			}

			// === Concrete Methods ===

			template <class I, class T>
			I MeshStructGenSource<I,T>::calculateLabel(I xCoord, I yCoord, I zCoord, I xMin, I xMax, I yMin, I yMax, I zMin) {
				// Sizes
				I xSize = (xMax - xMin) + 1;
				I ySize = (yMax - yMin) + 1;

				// Offset to zero-indexed
				I xCoordZero = xCoord - xMin;
				I yCoordZero = yCoord - yMin;
				I zCoordZero = zCoord - zMin;

				return xCoordZero + (yCoordZero * xSize) + (zCoordZero * (xSize * ySize));
			}

			template <class I, class T>
			I MeshStructGenSource<I,T>::calculateXCoord(I label, I xMin, I xMax) {
				// Compute position in base zero, then offset to xMin base.
				I xSize = (xMax - xMin) + 1;

				return (label % xSize) + xMin;
			}

			template <class I, class T>
			I MeshStructGenSource<I,T>::calculateYCoord(I label, I xMin, I xMax, I yMin, I yMax) {
				// Compute position in base zero, then offset to yMin base.
				I xSize = (xMax - xMin) + 1;
				I ySize = (yMax - yMin) + 1;

				return ((label / xSize) % ySize) + yMin;
			}

			template <class I, class T>
			I MeshStructGenSource<I,T>::calculateZCoord(I label, I xMin, I xMax, I yMin, I yMax, I zMin, I zMax) {
				// Compute position in base zero, then offset to zMin base.
				I xSize = (xMax - xMin) + 1;
				I ySize = (yMax - yMin) + 1;
				I zSize = (zMax - zMin) + 1;

				return ((label / (xSize * ySize)) % zSize) + zMin;
			}


			// === Overloaded Inherited Virtual Methods ===

			template <class I, class T>
			cupcfd::error::eCodes MeshStructGenSource<I,T>::getCellLabels(I * labels, I nLabels, I * indexes, I nIndexes) {
				if (nLabels != nIndexes) {
					return cupcfd::error::E_ARRAY_SIZE_MISMATCH;
				}

				// Struct Gen uses zero-based index labels
				// Labels are zero-index based, in order of traversing through X,Y,Z coordinate from
				// (0,0,0) to (nx-1, ny-1, nz-1) in priority of X, Y, Z.

				// Since we are using zero-based indexes, we will just assign the same label as the index
				for(I i = 0; i < nIndexes; i++) {
					labels[i] = indexes[i];
				}

				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T>
			cupcfd::error::eCodes MeshStructGenSource<I,T>::getFaceLabels(I * labels, I nLabels, I * indexes, I nIndexes) {
				if (nLabels != nIndexes) {
					return cupcfd::error::E_ARRAY_SIZE_MISMATCH;
				}

				// Struct Gen uses zero-based index labels

				// For the face labels, we will use three different coordinate schemes for the YZ faces, XZ faces and XY faces.

				// For the YZ faces, the X coordinates use the nodal coordinates in the X dimension for the low and high faces of a cell.
				// The Y, Z coordinates are the same as those of the cell coodinates the face are attached to.

				// For the XZ faces, the Y coordinates use the nodal coordinates in the Y dimension for the low and high faces of a cell.
				// The X, Z coordinates are the same as those of the cell coodinates the faces are attached to.

				// For the XY faces, the Z coordinates use the nodal coordinates in the Z dimension for the low and high faces of a cell.
				// The X, Y coordinates are the same as those of the cell coodinates the faces are attached to.

				// The labels are then computed based on these coordinates, but the label values are then offset to assign in order of
				// YZ faces, XZ Faces, XY faces i.e.:
				// 0 in YZ faces is label 0
				// 0 in XZ faces = label 0 + number of YZ faces
				// 0 in XY faces = label 0 + number of YZ faces + numberof XZ faces.

				// Since we are using zero-based indexes, we will just assign the same label as the index
				for(I i = 0; i < nIndexes; i++) {
					labels[i] = indexes[i];
				}

				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T>
			cupcfd::error::eCodes MeshStructGenSource<I,T>::getVertexLabels(I * labels, I nLabels, I * indexes, I nIndexes) {
				if (nLabels != nIndexes) {
					return cupcfd::error::E_ARRAY_SIZE_MISMATCH;
				}

				// Struct Gen uses zero-based index labels
				// Boundary Labels are zero-index based, in order of traversing through X,Y,Z coordinate from
				// (0,0,0) to (nx-1, ny-1, nz-1) in priority of X, Y, Z, but using node-centered positional coordinates.

				// Since we are using zero-based indexes, we will just assign the same label as the index
				for(I i = 0; i < nIndexes; i++) {
					labels[i] = indexes[i];
				}

				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T>
			cupcfd::error::eCodes MeshStructGenSource<I,T>::getBoundaryLabels(I * labels, I nLabels, I * indexes, I nIndexes) {
				if (nLabels != nIndexes) {
					return cupcfd::error::E_ARRAY_SIZE_MISMATCH;
				}

				// Struct Gen uses zero-based index labels
				// Boundary Labels are zero-index based, in order of traversing through X,Y,Z coordinate from
				// (0,0,0) to (nx-1, ny-1, nz-1) in priority of X, Y, Z. They are like faces, but assigned only to
				// boundary faces.

				// Since we are using zero-based indexes, we will just assign the same label as the index
				for(I i = 0; i < nIndexes; i++) {
					labels[i] = indexes[i];
				}

				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T>
			cupcfd::error::eCodes MeshStructGenSource<I,T>::getRegionLabels(I * labels, I nLabels, I * indexes, I nIndexes) {
				if (nLabels != nIndexes) {
					return cupcfd::error::E_ARRAY_SIZE_MISMATCH;
				}

				// Since we are using zero-based indexes, we will just assign the same label as the index
				for(I i = 0; i < nIndexes; i++) {
					labels[i] = indexes[i];
				}

				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T>
			cupcfd::error::eCodes MeshStructGenSource<I,T>::getCellCount(I * cellCount) {
				*cellCount = this->nX * this->nY * this->nZ;

				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T>
			cupcfd::error::eCodes MeshStructGenSource<I,T>::getFaceCount(I * faceCount) {
				// Number of faces in X dimension = Number of YZ plane faces = Y Cells * Z Cells * (XCells + 1)
				I faceYZ = this->nY * this->nZ * (this->nX + 1);
				I faceXZ = this->nX * this->nZ * (this->nY + 1);
				I faceXY = this->nX * this->nY * (this->nZ + 1);

				*faceCount = faceYZ + faceXZ + faceXY;

				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T>
			cupcfd::error::eCodes MeshStructGenSource<I,T>::getBoundaryCount(I * boundaryCount) {
				// Same computation as for number of faces, but only two planes in a dimension
				// count as boundary (the wall)
				I faceYZ = this->nY * this->nZ * 2;
				I faceXZ = this->nX * this->nZ * 2;
				I faceXY = this->nX * this->nY * 2;

				*boundaryCount = faceYZ + faceXZ + faceXY;

				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T>
			cupcfd::error::eCodes MeshStructGenSource<I,T>::getRegionCount(I * regionCount) {
				// Just one region, the boundary wall
				*regionCount = 1;

				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T>
			cupcfd::error::eCodes MeshStructGenSource<I,T>::getVertexCount(I * vertexCount) {
				// Number of vertices in a dimension = number of cells + 1
				*vertexCount = (this->nX + 1) * (this->nY + 1) * (this->nZ + 1);

				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T>
			cupcfd::error::eCodes MeshStructGenSource<I,T>::getMaxFaceCount(I * maxFaceCount) {
				// Max Face Count capped at 6
				*maxFaceCount = 6;

				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T>
			cupcfd::error::eCodes MeshStructGenSource<I,T>::getMaxVertexCount(I * maxVertexCount) {
				// Max Vertex Count per Face capped at 4
				*maxVertexCount = 4;

				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T>
			cupcfd::error::eCodes MeshStructGenSource<I,T>::getCellNFaces(I * nFaces, I nNFaces, I * cellLabels __attribute__((unused)), I nCellLabels __attribute__((unused))) {
				for(I i = 0; i < nNFaces; i++) {
					nFaces[i] = 6;
				}

				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T>
			cupcfd::error::eCodes MeshStructGenSource<I,T>::getCellVolume(T * cellVol, I nCellVol, I * cellLabels, I nCellLabels) {
				cupcfd::error::eCodes status;

				// Could do this via retrieving cell faces, vertices etc. However, since this is structured we can
				// just do the calculation directly on a hexahedron volume

				if (nCellVol != nCellLabels) {
					return cupcfd::error::E_ARRAY_SIZE_MISMATCH;
				}

				euc::EuclideanPoint<T,3> * centers = (euc::EuclideanPoint<T,3> *)malloc(sizeof(euc::EuclideanPoint<T,3>) * nCellLabels);

				status = this->getCellCenter(centers, nCellLabels, cellLabels, nCellLabels);
				CHECK_ECODE(status)

				T dX = this->dSx/2;
				T dY = this->dSy/2;
				T dZ = this->dSz/2;

				for(I i = 0; i < nCellLabels; i++) {
					euc::EuclideanPoint<T,3> tlf = centers[i] + euc::EuclideanPoint<T,3>(-dX, dY, dZ);
					euc::EuclideanPoint<T,3> trf = centers[i] + euc::EuclideanPoint<T,3>(dX, dY, dZ);
					euc::EuclideanPoint<T,3> tlb = centers[i] + euc::EuclideanPoint<T,3>(-dX, -dY, dZ);
					euc::EuclideanPoint<T,3> trb = centers[i] + euc::EuclideanPoint<T,3>(dX, -dY, dZ);
					euc::EuclideanPoint<T,3> blf = centers[i] + euc::EuclideanPoint<T,3>(-dX, dY, -dZ);
					euc::EuclideanPoint<T,3> brf = centers[i] + euc::EuclideanPoint<T,3>(dX, dY, -dZ);
					euc::EuclideanPoint<T,3> blb = centers[i] + euc::EuclideanPoint<T,3>(-dX, -dY, -dZ);
					euc::EuclideanPoint<T,3> brb = centers[i] + euc::EuclideanPoint<T,3>(dX, -dY, -dZ);

					cupcfd::geometry::shapes::Hexahedron<T> cellShape(tlf, trf, blf, brf, tlb, trb, blb, brb);
					cellVol[i] = cellShape.computeVolume();
				}

				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T>
			cupcfd::error::eCodes MeshStructGenSource<I,T>::getCellCenter(euc::EuclideanPoint<T,3> * cellCenter, I nCellCenter, I * cellLabels, I nCellLabels) {
				if (nCellCenter != nCellLabels) {
					return cupcfd::error::E_ARRAY_SIZE_MISMATCH;
				}

				// Loop over each label
				for(I i = 0; i < nCellLabels; i++) {
					// Get the cell coordinates in the cell coordinate scheme
					I xCoord = calculateXCoord(cellLabels[i], 0, this->nX - 1);
					I yCoord = calculateYCoord(cellLabels[i], 0, this->nX-1, 0, this->nY - 1);
					I zCoord = calculateZCoord(cellLabels[i], 0, this->nX-1, 0, this->nY -1, 0, this->nZ - 1);

					// Compute the Spatial Low, High Values for the three dimensions
					T xSLow = sMinX + (xCoord * this->dSx);
					T xSHigh = sMinX + (xCoord+1) * this->dSx;

					T ySLow = sMinY + (yCoord * this->dSy);
					T ySHigh = sMinY + ((yCoord+1) * this->dSy);

					T zSLow = sMinZ + (zCoord * this->dSz);
					T zSHigh = sMinZ + ((zCoord+1) * this->dSz);

					// Set X, Y, Z values of the vector
					// Center is inbetween low, high positions
					cellCenter[i].cmp[0] = xSLow + ((xSHigh - xSLow) / 2.0);
					cellCenter[i].cmp[1] = ySLow + ((ySHigh - ySLow) / 2.0);
					cellCenter[i].cmp[2] = zSLow + ((zSHigh - zSLow) / 2.0);
				}

				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T>
			cupcfd::error::eCodes MeshStructGenSource<I,T>::getCellFaceLabels(I * csrIndices, I nCsrIndices, I * csrData, I nCsrData, I * cellLabels, I nCellLabels) {
				// Error Checks: Array sizes need to be correct for CSR
				// nCsrIndices = nCellLabels + 1
				if (nCsrIndices != (nCellLabels+1)) {
					return cupcfd::error::E_ARRAY_SIZE_MISMATCH;
				}
				// nCsrData = sum(nFaces) for each cell in cell labels
				I nFacesTotal = 0;
				for(I i = 0; i < nCellLabels; i++) {
					nFacesTotal += 6;
				}
				if (nCsrData != nFacesTotal) {
					return cupcfd::error::E_ARRAY_SIZE_MISMATCH;
				}

				// Loop over each label
				I csrDataPtr = 0;

				// Number of faces in these planes.
				I nFaceYZ = this->nY * this->nZ * (this->nX + 1);
				I nFaceXZ = this->nX * this->nZ * (this->nY + 1);

				for(I i = 0; i < nCellLabels; i++) {
					// Get the cell coordinates
					I xCoord = calculateXCoord(cellLabels[i], 0, this->nX - 1);
					I yCoord = calculateYCoord(cellLabels[i], 0, this->nX - 1, 0, this->nY - 1);
					I zCoord = calculateZCoord(cellLabels[i], 0, this->nX - 1, 0, this->nY - 1, 0, this->nZ - 1);

					// Set the start of the data range for this label
					csrIndices[i] = csrDataPtr;

					// Compute the face coordinates
					// For the face labels, we will use three different coordinate schemes for the YZ faces, XZ faces and XY faces.

					// For the YZ faces, the X coordinates use the nodal coordinates in the X dimension for the low and high faces of a cell.
					// The Y, Z coordinates are the same as those of the cell coodinates the face are attached to.

					// For the XZ faces, the Y coordinates use the nodal coordinates in the Y dimension for the low and high faces of a cell.
					// The X, Z coordinates are the same as those of the cell coodinates the faces are attached to.

					// For the XY faces, the Z coordinates use the nodal coordinates in the Z dimension for the low and high faces of a cell.
					// The X, Y coordinates are the same as those of the cell coodinates the faces are attached to.

					// The labels are then computed based on these coordinates, but the label values are then offset to assign in order of
					// YZ faces, XZ Faces, XY faces i.e.:
					// 0 in YZ faces is label 0
					// 0 in XZ faces = label 0 + number of YZ faces
					// 0 in XY faces = label 0 + number of YZ faces + numberof XZ faces.

					I nodeXLow = xCoord;
					I nodeXHigh = xCoord + 1;
					I nodeYLow = yCoord;
					I nodeYHigh = yCoord + 1;
					I nodeZLow = zCoord;
					I nodeZHigh = zCoord + 1;

					// Example: For a YZ Face, we use the nodal coodinates in the X dimension, and the cell coodinates
					// in the Y, Z dimension. The coordinate min, max of the range is 1 high in the X dimension accordingly.
					I YZLowLabel  = calculateLabel(nodeXLow,  yCoord,    zCoord,    0, this->nX,   0, this->nY-1, 0);//, this->nZ-1);
					I YZHighLabel = calculateLabel(nodeXHigh, yCoord,    zCoord,    0, this->nX,   0, this->nY-1, 0);//, this->nZ-1);
					I XZLowLabel  = calculateLabel(xCoord,    nodeYLow,  zCoord,    0, this->nX-1, 0, this->nY,   0);//, this->nZ-1);
					I XZHighLabel = calculateLabel(xCoord,    nodeYHigh, zCoord,    0, this->nX-1, 0, this->nY,   0);//, this->nZ-1);
					I XYLowLabel  = calculateLabel(xCoord,    yCoord,    nodeZLow,  0, this->nX-1, 0, this->nY-1, 0);//, this->nZ);
					I XYHighLabel = calculateLabel(xCoord,    yCoord,    nodeZHigh, 0, this->nX-1, 0, this->nY-1, 0);//, this->nZ);

					// Since we are using three different coordinate systems, the three sets of labels will all use the same labels.
					// To address this, offset the label schemes for each plane.
					XZLowLabel = XZLowLabel + nFaceYZ;
					XZHighLabel = XZHighLabel + nFaceYZ;

					XYLowLabel = XYLowLabel + nFaceYZ + nFaceXZ;
					XYHighLabel = XYHighLabel + nFaceYZ + nFaceXZ;

					// Store in Data Array
					csrData[csrDataPtr] = YZLowLabel;
					csrData[csrDataPtr + 1] = YZHighLabel;
					csrData[csrDataPtr + 2] = XZLowLabel;
					csrData[csrDataPtr + 3] = XZHighLabel;
					csrData[csrDataPtr + 4] = XYLowLabel;
					csrData[csrDataPtr + 5] = XYHighLabel;

					// Update the data pointer
					csrDataPtr = csrDataPtr + 6;
				}

				// Set the last position of the CSR indices to point at 1 past the last valid index of csrData
				// (nCSRIndices should equal nCellLabels + 1)
				csrIndices[nCellLabels] = nCsrData;

				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T>
			cupcfd::error::eCodes MeshStructGenSource<I,T>::getFaceIsBoundary(bool * isBoundary, I nIsBoundary, I * faceLabels, I nFaceLabels) {
				if (nIsBoundary != nFaceLabels) {
					return cupcfd::error::E_ARRAY_SIZE_MISMATCH;
				}

				// Number of faces in these planes.
				I nFaceYZ = this->nY * this->nZ * (this->nX + 1);
				I nFaceXZ = this->nX * this->nZ * (this->nY + 1);

				// Loop over labels
				for(I i = 0; i < nFaceLabels; i++) {
					// Determine which type of face it is - YZ, XZ or XY.
					if(faceLabels[i] < nFaceYZ) {
						// YZ Face
						// Determine the Face X Coordinate
						// If it equals 0 or max nodal coordinate, it is a boundary
						I xCoord = calculateXCoord(faceLabels[i], 0, this->nX);

						if(xCoord == 0 || xCoord == this->nX) {
							isBoundary[i] = true;
						} else {
							isBoundary[i] = false;
						}
					}
					else if(faceLabels[i] < (nFaceYZ + nFaceXZ)) {
						// XZ Face
						// Determine the Face Y Coordinate
						// If it equals 0 or max nodal coordinate, it is a boundary
						// We need to remove the offset to ensure we are using the correct labels
						// internal to a coordinate system (since the offsets were computed using
						// the sizes of a different coordinate system)

						I yCoord = calculateYCoord(faceLabels[i] - nFaceYZ, 0, this->nX - 1, 0, this->nY);

						if(yCoord == 0 || yCoord == this->nY) {
							isBoundary[i] = true;
						} else {
							isBoundary[i] = false;
						}
					} else {
						// XY Face
						// Determine the Face Z Coordinate
						// If it equals 0 or max nodal coordinate, it is a boundary
						I zCoord = calculateZCoord(faceLabels[i] - (nFaceYZ + nFaceXZ), 0, this->nX - 1, 0, this->nY - 1, 0, this->nZ);

						if(zCoord == 0 || zCoord == this->nZ) {
							isBoundary[i] = true;
						} else {
							isBoundary[i] = false;
						}
					}
				}

				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T>
			cupcfd::error::eCodes MeshStructGenSource<I,T>::getFaceNVertices(I * nVertices, I nNVertices __attribute__((unused)), I * faceLabels __attribute__((unused)), I nFaceLabels) {
				// All faces in this setup have 4 vertices.
				for(I i = 0; i < nFaceLabels; i++) {
					nVertices[i] = 4;
				}

				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T>
			cupcfd::error::eCodes MeshStructGenSource<I,T>::getFaceBoundaryLabels(I * faceBoundaryLabels, I nFaceBoundaryLabels, I * faceLabels, I nFaceLabels) {
				if (nFaceBoundaryLabels != nFaceLabels) {
					return cupcfd::error::E_ARRAY_SIZE_MISMATCH;
				}

				cupcfd::error::eCodes status;

				// Error Check: Check they are all boundary faces
				bool * isBoundary = (bool *) malloc(sizeof(bool) * nFaceLabels);

				status = getFaceIsBoundary(isBoundary, nFaceLabels, faceLabels, nFaceLabels);
				CHECK_ECODE(status)

				for(I i = 0; i < nFaceLabels; i++) {
					if(isBoundary[i] == false) {
						free(isBoundary);
						return cupcfd::error::E_ERROR;
					}
				}

				free(isBoundary);

				// Compute Boundary Labels
				// Boundary Label Scheme is the same as the face labelling scheme but we can only have boundaries on the edge.

				// Number of faces in these planes.
				I nFaceYZ = this->nY * this->nZ * (this->nX + 1);
				I nFaceXZ = this->nX * this->nZ * (this->nY + 1);

				// Number of Boundaries in these planes
				I nBoundYZ = this->nY * this->nZ * 2;
				I nBoundXZ = this->nX * this->nZ * 2;

				// Loop over labels to convert form face labels to boundaries labels
				// Effectively, convert back to coordinates from the face labels.
				// Since this only has boundaries at the edges we can treat the boundaries as having a
				// dimension size of two in one direction.
				// E.g. the labelling scheme for a YZ boundary is effectively 2 * nY * nZ boundaries
				// with coordinates ranging from 0->1 * 0->nY-1 * 0->nZ-1.

				for(I i = 0; i < nFaceLabels; i++) {
					// Determine which type of face it is - YZ, XZ or XY.
					if(faceLabels[i] < nFaceYZ) {
						// YZ Boundary
						// Get Face X,Y,Z Coordinates for a YZ Face
						// Dimensions 0->nX * 0->nY-1 * 0->nZ-1
						I xCoord = calculateXCoord(faceLabels[i], 0, this->nX);
						I yCoord = calculateYCoord(faceLabels[i], 0, this->nX, 0, this->nY - 1);
						I zCoord = calculateZCoord(faceLabels[i], 0, this->nX, 0, this->nY - 1, 0, this->nZ - 1);

						if(xCoord == 0) {
							// Low Grid Edge
							faceBoundaryLabels[i] = calculateLabel(0, yCoord, zCoord, 0, 1, 0, this->nY - 1, 0);//, this->nZ - 1);
						}
						else if(xCoord == this->nX)
						{
							// High Grid Edge
							faceBoundaryLabels[i] = calculateLabel(1, yCoord, zCoord, 0, 1, 0, this->nY - 1, 0);//, this->nZ - 1);
						} else {
							// Error - This isn't a boundary edge int he structured grid setup
							return cupcfd::error::E_ERROR;
						}
					}
					else if(faceLabels[i] < (nFaceYZ + nFaceXZ)) {
						// XZ Boundary
						// Get Face X,Y,Z Coordinates for a XZ Face
						// Dimensions 0->nX-1 * 0->nY * 0->nZ-1
						I xCoord = calculateXCoord(faceLabels[i] - nFaceYZ, 0, this->nX -1);
						I yCoord = calculateYCoord(faceLabels[i] - nFaceYZ, 0, this->nX -1, 0, this->nY);
						I zCoord = calculateZCoord(faceLabels[i] - nFaceYZ, 0, this->nX -1, 0, this->nY, 0, this->nZ - 1);

						if(yCoord == 0) {
							// Low Grid Edge
							// Add offset to start indexes in new range
							faceBoundaryLabels[i] = nBoundYZ + calculateLabel(xCoord, 0, zCoord, 0, this->nX - 1, 0, 1, 0);//, this->nZ - 1);
						}
						else if(yCoord == this->nY) {
							// High Grid Edge
							// Add offset to start indexes in new range
							faceBoundaryLabels[i] = nBoundYZ + calculateLabel(xCoord, 1, zCoord, 0, this->nX - 1, 0, 1, 0);//, this->nZ - 1);
						} else {
							// Error - This isn't a boundary edge int he structured grid setup
							return cupcfd::error::E_ERROR;
						}

					} else {
						// XY Boundary
						// Get Face X,Y,Z Coordinates for a XZ Face
						// Dimensions 0->nX-1 * 0->nY-1 * 0->nZ
						I xCoord = calculateXCoord(faceLabels[i] - (nFaceYZ + nFaceXZ), 0, this->nX -1);
						I yCoord = calculateYCoord(faceLabels[i] - (nFaceYZ + nFaceXZ), 0, this->nX -1, 0, this->nY-1);
						I zCoord = calculateZCoord(faceLabels[i] - (nFaceYZ + nFaceXZ), 0, this->nX -1, 0, this->nY-1, 0, this->nZ);

						if(zCoord == 0) {
							// Low Grid Edge
							// Add offset to start indexes in new range
							faceBoundaryLabels[i] = nBoundYZ + nBoundXZ + calculateLabel(xCoord, yCoord, 0, 0, this->nX - 1, 0, this->nY - 1, 0);//, 1);
						}
						else if(zCoord == this->nZ) {
							// High Grid Edge
							// Add offset to start indexes in new range
							faceBoundaryLabels[i] = nBoundYZ + nBoundXZ + calculateLabel(xCoord, yCoord, 1, 0, this->nX - 1, 0, this->nY - 1, 0);//, 1);
						} else {
							// Error - This isn't a boundary edge in the structured grid setup
							return cupcfd::error::E_ERROR;
						}
					}
				}

				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T>
			cupcfd::error::eCodes MeshStructGenSource<I,T>::getFaceCell1Labels(I * faceCell1Labels, I nFaceCell1Labels, I * faceLabels, I nFaceLabels) {
				if (nFaceCell1Labels != nFaceLabels) {
					return cupcfd::error::E_ARRAY_SIZE_MISMATCH;
				}

				// Number of faces in these planes.
				I nFaceYZ = this->nY * this->nZ * (this->nX + 1);
				I nFaceXZ = this->nX * this->nZ * (this->nY + 1);

				// Cell 1 of a face is either (a) The low face if there are two cells either side or
				// (b) the sole face if it is a boundary face.

				// Loop over the face indices
				for(I i = 0; i < nFaceLabels; i++) {
					// Determine which type of face it is - YZ, XZ or XY.
					if(faceLabels[i] < nFaceYZ) {
						// YZ Boundary
						// Get Face X,Y,Z Coordinates for a YZ Face
						// Dimensions 0->nX * 0->nY-1 * 0->nZ-1
						I xCoord = calculateXCoord(faceLabels[i], 0, this->nX);
						I yCoord = calculateYCoord(faceLabels[i], 0, this->nX, 0, this->nY - 1);
						I zCoord = calculateZCoord(faceLabels[i], 0, this->nX, 0, this->nY - 1, 0, this->nZ - 1);

						// YZ Faces share the Y,Z Coordinates with the cell in the cell system.
						if(xCoord == 0) {
							// This face only has one cell, and it is the low face of the cell.
							// Therefore the cell shares the same X coord as the face using the cell ranges for
							// its coordinates.
							faceCell1Labels[i] = calculateLabel(xCoord, yCoord, zCoord, 0, this->nX - 1, 0, this->nY - 1, 0);//, this->nZ - 1);
						}
						else if(xCoord == this->nX) {
							// This face only has one cell, and it is the high face of the cell.
							// Therefore the cell X coord is one less in it's system than the face X coord.
							faceCell1Labels[i] = calculateLabel(xCoord-1, yCoord, zCoord, 0, this->nX - 1, 0, this->nY - 1, 0);//, this->nZ - 1);
						} else {
							// This face has two cells, so we will treat cell 1 as a low cell, and
							// so the cell X coord is one less in it's system than the face X coord.
							faceCell1Labels[i] = calculateLabel(xCoord-1, yCoord, zCoord, 0, this->nX - 1, 0, this->nY - 1, 0);//, this->nZ - 1);
						}
					}
					else if(faceLabels[i] < (nFaceYZ + nFaceXZ)) {
						// XZ Boundary
						// Get Face X,Y,Z Coordinates for a XZ Face
						// Dimensions 0->nX-1 * 0->nY * 0->nZ-1
						// Remember to remove offsets
						I xCoord = calculateXCoord(faceLabels[i] - nFaceYZ, 0, this->nX -1);
						I yCoord = calculateYCoord(faceLabels[i] - nFaceYZ, 0, this->nX -1, 0, this->nY);
						I zCoord = calculateZCoord(faceLabels[i] - nFaceYZ, 0, this->nX -1, 0, this->nY, 0, this->nZ - 1);

						// XZ Faces share the X,Z Coordinates with the cell in the cell system.
						if(yCoord == 0) {
							// This face only has one cell, and it is the low face of the cell.
							// Therefore the cell shares the same Y coord as the face using the cell ranges for
							// its coordinates.
							faceCell1Labels[i] = calculateLabel(xCoord, yCoord, zCoord, 0, this->nX - 1, 0, this->nY - 1, 0);//, this->nZ - 1);
						}
						else if(yCoord == this->nY) {
							// This face only has one cell, and it is the high face of the cell.
							// Therefore the cell Y coord is one less in it's system than the face Y coord.
							faceCell1Labels[i] = calculateLabel(xCoord, yCoord-1, zCoord, 0, this->nX - 1, 0, this->nY - 1, 0);//, this->nZ - 1);
						}
						else {
							// This face has two cells, so we will treat cell 1 as a low cell, and
							// so the cell Y coord is one less in it's system than the face Y coord.
							faceCell1Labels[i] = calculateLabel(xCoord, yCoord-1, zCoord, 0, this->nX - 1, 0, this->nY - 1, 0);//, this->nZ - 1);
						}
					} else {
						// XY Boundary
						// Get Face X,Y,Z Coordinates for a XZ Face
						// Dimensions 0->nX-1 * 0->nY-1 * 0->nZ
						// Remember to remove offsets
						I xCoord = calculateXCoord(faceLabels[i] - (nFaceYZ + nFaceXZ), 0, this->nX -1);
						I yCoord = calculateYCoord(faceLabels[i] - (nFaceYZ + nFaceXZ), 0, this->nX -1, 0, this->nY-1);
						I zCoord = calculateZCoord(faceLabels[i] - (nFaceYZ + nFaceXZ), 0, this->nX -1, 0, this->nY-1, 0, this->nZ);

						// XY Faces share the X,Y Coordinates with the cell in the cell system.
						if(zCoord == 0) {
							// This face only has one cell, and it is the low face of the cell.
							// Therefore the cell shares the same Z coord as the face using the cell ranges for
							// its coordinates.
							faceCell1Labels[i] = calculateLabel(xCoord, yCoord, zCoord, 0, this->nX - 1, 0, this->nY - 1, 0);//, this->nZ - 1);
						}
						else if(zCoord == this->nZ) {
							// This face only has one cell, and it is the high face of the cell.
							// Therefore the cell Z coord is one less in it's system than the face Z coord.
							faceCell1Labels[i] = calculateLabel(xCoord, yCoord, zCoord-1, 0, this->nX - 1, 0, this->nY - 1, 0);//, this->nZ - 1);
						} else {
							// This face has two cells, so we will treat cell 1 as a low cell, and
							// so the cell Z coord is one less in it's system than the face Z coord.
							faceCell1Labels[i] = calculateLabel(xCoord, yCoord, zCoord-1, 0, this->nX - 1, 0, this->nY - 1, 0);//, this->nZ - 1);
						}
					}
				}

				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T>
			cupcfd::error::eCodes MeshStructGenSource<I,T>::getFaceCell2Labels(I * faceCell2Labels, I nFaceCell2Labels, I * faceLabels, I nFaceLabels) {
				if (nFaceCell2Labels != nFaceLabels) {
					return cupcfd::error::E_ARRAY_SIZE_MISMATCH;
				}

				// Number of faces in these planes.
				I nFaceYZ = this->nY * this->nZ * (this->nX + 1);
				I nFaceXZ = this->nX * this->nZ * (this->nY + 1);

				// Cell 1 of a face is either (a) The low face if there are two cells either side or
				// (b) the sole face if it is a boundary face.

				// Loop over the face indices
				for(I i = 0; i < nFaceLabels; i++) {
					// Determine which type of face it is - YZ, XZ or XY.
					if(faceLabels[i] < nFaceYZ) {
						// YZ Boundary
						// Get Face X,Y,Z Coordinates for a YZ Face
						// Dimensions 0->nX * 0->nY-1 * 0->nZ-1
						I xCoord = calculateXCoord(faceLabels[i], 0, this->nX);
						I yCoord = calculateYCoord(faceLabels[i], 0, this->nX, 0, this->nY - 1);
						I zCoord = calculateZCoord(faceLabels[i], 0, this->nX, 0, this->nY - 1, 0, this->nZ - 1);

						// YZ Faces share the Y,Z Coordinates with the cell in the cell system.
						if(xCoord == 0) {
							// This face only has one cell, and it is the low face of the cell.
							// Therefore the cell shares the same X coord as the face using the cell ranges for
							// its coordinates.
							faceCell2Labels[i] = -1;
						}
						else if(xCoord == this->nX) {
							// This face only has one cell, and it is the high face of the cell.
							// Therefore the cell X coord is one less in it's system than the face X coord.
							faceCell2Labels[i] = -1;
						} else {
							// This face has two cells, so we will treat cell 2 as a high cell, and
							// so the cell X coord is the same as the face coord in the cell scheme
							faceCell2Labels[i] = calculateLabel(xCoord, yCoord, zCoord, 0, this->nX - 1, 0, this->nY - 1, 0);//, this->nZ - 1);
						}
					}
					else if(faceLabels[i] < (nFaceYZ + nFaceXZ)) {
						// XZ Boundary
						// Get Face X,Y,Z Coordinates for a XZ Face
						// Dimensions 0->nX-1 * 0->nY * 0->nZ-1
						// Remember to remove offsets
						I xCoord = calculateXCoord(faceLabels[i] - nFaceYZ, 0, this->nX -1);
						I yCoord = calculateYCoord(faceLabels[i] - nFaceYZ, 0, this->nX -1, 0, this->nY);
						I zCoord = calculateZCoord(faceLabels[i] - nFaceYZ, 0, this->nX -1, 0, this->nY, 0, this->nZ - 1);

						// XZ Faces share the X,Z Coordinates with the cell in the cell system.
						if(yCoord == 0) {
							// This face only has one cell, and it is the low face of the cell.
							// Therefore the cell shares the same Y coord as the face using the cell ranges for
							// its coordinates.
							faceCell2Labels[i] = -1;
						}
						else if(yCoord == this->nY) {
							// This face only has one cell, and it is the high face of the cell.
							// Therefore the cell Y coord is one less in it's system than the face Y coord.
							faceCell2Labels[i] = -1;
						} else {
							// This face has two cells, so we will treat cell 1 as a low cell, and
							// so the cell X coord is the same as the face coord in the cell scheme
							faceCell2Labels[i] = calculateLabel(xCoord, yCoord, zCoord, 0, this->nX - 1, 0, this->nY - 1, 0);//, this->nZ - 1);
						}
					} else {
						// XY Boundary
						// Get Face X,Y,Z Coordinates for a XZ Face
						// Dimensions 0->nX-1 * 0->nY-1 * 0->nZ
						// Remember to remove offsets
						I xCoord = calculateXCoord(faceLabels[i] - (nFaceYZ + nFaceXZ), 0, this->nX -1);
						I yCoord = calculateYCoord(faceLabels[i] - (nFaceYZ + nFaceXZ), 0, this->nX -1, 0, this->nY-1);
						I zCoord = calculateZCoord(faceLabels[i] - (nFaceYZ + nFaceXZ), 0, this->nX -1, 0, this->nY-1, 0, this->nZ);

						// XY Faces share the X,Y Coordinates with the cell in the cell system.
						if(zCoord == 0) {
							// This face only has one cell, and it is the low face of the cell.
							// Therefore the cell shares the same Z coord as the face using the cell ranges for
							// its coordinates.
							faceCell2Labels[i] = -1;
						}
						else if(zCoord == this->nZ) {
							// This face only has one cell, and it is the high face of the cell.
							// Therefore the cell Z coord is one less in it's system than the face Z coord.
							faceCell2Labels[i] = -1;
						} else {
							// This face has two cells, so we will treat cell 1 as a low cell, and
							// so the cell X coord is the same as the face coord in the cell scheme
							faceCell2Labels[i] = calculateLabel(xCoord, yCoord, zCoord, 0, this->nX - 1, 0, this->nY - 1, 0);//, this->nZ - 1);
						}
					}
				}

				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T>
			cupcfd::error::eCodes MeshStructGenSource<I,T>::getFaceLambda(T * faceLambda, I nFaceLambda, I * faceLabels, I nFaceLabels) {
				cupcfd::error::eCodes status;

				if (nFaceLambda != nFaceLabels) {
					return cupcfd::error::E_ARRAY_SIZE_MISMATCH;
				}

				// Since the cells are equally sized, think this should always be 0.5 since they are equidistant from the face?

				bool * isBoundary = (bool *) malloc(sizeof(bool) * nFaceLabels);

				status = this->getFaceIsBoundary(isBoundary, nFaceLabels, faceLabels, nFaceLabels);
				CHECK_ECODE(status)

				for(I i = 0; i < nFaceLabels; i++) {
					if(isBoundary[i]) {
						faceLambda[i] = T(-1);
					} else {
						faceLambda[i] = T(0.5);
					}
				}

				free(isBoundary);

				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T>
			cupcfd::error::eCodes MeshStructGenSource<I,T>::getFaceArea(T * faceArea, I nFaceArea, I * faceLabels, I nFaceLabels) {
				cupcfd::error::eCodes status;

				if (nFaceArea != nFaceLabels) {
					return cupcfd::error::E_ARRAY_SIZE_MISMATCH;
				}
				
				// Retrieve the face vertex labels - since this is a structured grid, we know there should always be 4
				I * csrIndices = (I*) malloc(sizeof(I) * nFaceLabels + 1);
				I * csrData = (I*) malloc(sizeof(I) * nFaceLabels * 4);
				status = this->getFaceVerticesLabelsCSR(csrIndices, nFaceLabels + 1, csrData, nFaceLabels * 4,  faceLabels, nFaceLabels);
				CHECK_ECODE(status)

				// Get the vertex points for each label
				euc::EuclideanPoint<T,3> * vertexPos = (euc::EuclideanPoint<T,3> *)malloc(sizeof(euc::EuclideanPoint<T,3>) * nFaceLabels * 4);
				status = this->getVertexCoords(vertexPos, nFaceLabels * 4, csrData, nFaceLabels * 4);
				CHECK_ECODE(status)

				// Compute the area
				for(I i = 0; i < nFaceLabels; i++) {
					faceArea[i] = cupcfd::geometry::shapes::Quadrilateral3D<T>::triangularAreaSum(vertexPos[i],
																									vertexPos[i]+1,
																									vertexPos[i]+2,
																									vertexPos[i]+3);
				}

				free(csrIndices);
				free(csrData);
				free(vertexPos);

				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T>
			cupcfd::error::eCodes MeshStructGenSource<I,T>::getFaceNormal(euc::EuclideanVector<T,3> * faceNormal, I nFaceNormal, I * faceLabels, I nFaceLabels) {
				cupcfd::error::eCodes status;

				if (nFaceNormal != nFaceLabels) {
					return cupcfd::error::E_ARRAY_SIZE_MISMATCH;
				}
				
				// Temporary Storage
				I * csrIndices = (I*) malloc(sizeof(I) * nFaceLabels + 1);
				I * csrData = (I*) malloc(sizeof(I) * nFaceLabels * 4);
				I * cell1Labels = (I*) malloc(sizeof(I) * nFaceLabels);

				euc::EuclideanPoint<T,3> * vertexPos = (euc::EuclideanPoint<T,3> *)malloc(sizeof(euc::EuclideanPoint<T,3>) * nFaceLabels * 4);

				euc::EuclideanPoint<T,3> * cellCenters = (euc::EuclideanPoint<T,3> *)malloc(sizeof(euc::EuclideanPoint<T,3>) * nFaceLabels);

				// Compute Face Normal - Should Face Outwards From Cell 1
				// Get Cell 1 ID
				status = this->getFaceCell1Labels(cell1Labels, nFaceLabels, faceLabels, nFaceLabels);
				CHECK_ECODE(status)

				// Get Cell Center
				status = this->getCellCenter(cellCenters, nFaceLabels, cell1Labels, nFaceLabels);
				CHECK_ECODE(status)

				// Retrieve the face vertex labels - since this is a structured grid, we know there should always be 4
				status = this->getFaceVerticesLabelsCSR(csrIndices, nFaceLabels + 1, csrData, nFaceLabels * 4,  faceLabels, nFaceLabels);
				CHECK_ECODE(status)

				// Get the vertex points for each label
				status = this->getVertexCoords(vertexPos, nFaceLabels * 4, csrData, nFaceLabels * 4);
				CHECK_ECODE(status)

				for(I i = 0; i < nFaceLabels; i++) {
					// Determine the direction of the face vertices. We know each subsequent vertex
					// is connected by an edge, but not whether clockwise or anti-clockwise when observing from the cell center
					bool isClockwise = cupcfd::geometry::shapes::isVertexOrderClockwise(cellCenters[i], &vertexPos[i*4], 4);

					// Setup the normal computation such that they appear clockwise when observing from the cell center (so the normal
					// faces outwards)
					if(isClockwise) {
						// Clockwise when viewed from Cell Center
						faceNormal[i] = euc::EuclideanPlane3D<T>::normal(vertexPos[(i*4)], vertexPos[(i*4)+1], vertexPos[(i*4)+2]);
					} else {
						// Anti-Clockwise when viewed from Cell Center so invert for computation
						faceNormal[i] = euc::EuclideanPlane3D<T>::normal(vertexPos[(i*4)+2], vertexPos[(i*4)+1], vertexPos[(i*4)]);
					}

				}

				free(csrIndices);
				free(csrData);
				free(vertexPos);
				free(cell1Labels);
				free(cellCenters);

				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T>
			cupcfd::error::eCodes MeshStructGenSource<I,T>::getFaceCenter(euc::EuclideanPoint<T,3> * faceCenter, I nFaceCenter, I * faceLabels, I nFaceLabels) {
				cupcfd::error::eCodes status;

				if (nFaceCenter != nFaceLabels) {
					return cupcfd::error::E_ARRAY_SIZE_MISMATCH;
				}
				
				// Retrieve the face vertex labels - since this is a structured grid, we know there should always be 4
				I * csrIndices = (I*) malloc(sizeof(I) * nFaceLabels + 1);
				I * csrData = (I*) malloc(sizeof(I) * nFaceLabels * 4);
				status = this->getFaceVerticesLabelsCSR(csrIndices, nFaceLabels + 1, csrData, nFaceLabels * 4,  faceLabels, nFaceLabels);
				CHECK_ECODE(status)

				// Get the vertex points for each label
				euc::EuclideanPoint<T,3> * vertexPos = (euc::EuclideanPoint<T,3> *)malloc(sizeof(euc::EuclideanPoint<T,3>) * nFaceLabels * 4);
				status = this->getVertexCoords(vertexPos, nFaceLabels * 4, csrData, nFaceLabels * 4);
				CHECK_ECODE(status)

				// Compute the area
				for(I i = 0; i < nFaceLabels; i++) {
					cupcfd::geometry::shapes::Quadrilateral3D<T> shape(vertexPos[i], vertexPos[i]+1, vertexPos[i]+2, vertexPos[i]+3);
					faceCenter[i] = shape.computeCentroid();
				}

				free(csrIndices);
				free(csrData);
				free(vertexPos);

				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T>
			cupcfd::error::eCodes MeshStructGenSource<I,T>::getFaceVerticesLabelsCSR(I * csrIndices, I nCsrIndices, I * csrData, I nCsrData,  I * faceLabels, I nFaceLabels) {
				// Error Checks: Array Sizes
				// nCsrIndices = nFaceLabels + 1
				if(nCsrIndices != nFaceLabels + 1) {
					return cupcfd::error::E_ARRAY_SIZE_MISMATCH;
				}

				// nCsrData = Sum of vertices per each face = 4 * nFaceLabels for a structured cube grid.
				if(nCsrData != nFaceLabels*4) {
					return cupcfd::error::E_ARRAY_SIZE_MISMATCH;
				}


				// Vertices are labelled in X,Y,Z order based on the nodal coordinate system - i.e.
				// 0: 0,0,0
				// 1: 1,0,0
				// 3: 2,0,0
				// 4: 0,1,0
				// etc...

				// Convert Face Label into its X, Y, Z coordinates in the face coordinate system (depends on whether it is an YZ, XZ or XY face)
				// and then translate those coordinates back into the nodal coordinate system to compute the vertex labels.

				// Number of faces in these planes.
				I nFaceYZ = this->nY * this->nZ * (this->nX + 1);
				I nFaceXZ = this->nX * this->nZ * (this->nY + 1);

				I csrDataPtr = 0;

				// Loop over the face indices
				for(I i = 0; i < nFaceLabels; i++) {
					// Set the CSR Indices Ptr into the data array
					csrIndices[i] = csrDataPtr;

					// Determine which type of face it is - YZ, XZ or XY.
					if(faceLabels[i] < nFaceYZ) {
						// YZ Face
						// Get Face X,Y,Z Coordinates for a YZ Face
						// Dimensions 0->nX * 0->nY-1 * 0->nZ-1
						I xCoord = calculateXCoord(faceLabels[i], 0, this->nX);
						I yCoord = calculateYCoord(faceLabels[i], 0, this->nX, 0, this->nY - 1);
						I zCoord = calculateZCoord(faceLabels[i], 0, this->nX, 0, this->nY - 1, 0, this->nZ - 1);

						// In YZ plane, all vertices share the same X coordinate as the face.
						// The set of vertices are then a permutation set of the face's yCoord, yCoord + 1, zCoord and zCoord + 1

						// Compute the labels for the 4 vertices using nodal coordinate in all three dimensions
						// Add them in order such that there is an edge going from vertex 0
						csrData[csrDataPtr] = calculateLabel(xCoord, yCoord, zCoord, 0, this->nX, 0, this->nY, 0);//, this->nZ);
						csrData[csrDataPtr+1] = calculateLabel(xCoord, yCoord+1, zCoord, 0, this->nX, 0, this->nY, 0);//, this->nZ);
						csrData[csrDataPtr+2] = calculateLabel(xCoord, yCoord+1, zCoord+1, 0, this->nX, 0, this->nY, 0);//, this->nZ);
						csrData[csrDataPtr+3] = calculateLabel(xCoord, yCoord, zCoord+1, 0, this->nX, 0, this->nY, 0);//, this->nZ);

						// Advance the Data Pointer
						csrDataPtr = csrDataPtr + 4;
					}
					else if(faceLabels[i] < (nFaceYZ + nFaceXZ)) {
						// XZ Face
						// Get Face X,Y,Z Coordinates for a XZ Face
						// Dimensions 0->nX-1 * 0->nY * 0->nZ-1
						// Remember to remove offsets
						I xCoord = calculateXCoord(faceLabels[i] - nFaceYZ, 0, this->nX -1);
						I yCoord = calculateYCoord(faceLabels[i] - nFaceYZ, 0, this->nX -1, 0, this->nY);
						I zCoord = calculateZCoord(faceLabels[i] - nFaceYZ, 0, this->nX -1, 0, this->nY, 0, this->nZ - 1);

						// In XZ plane, all vertices share the same Y coordinate as the face.
						// The set of vertices are then a permutation set of the face's xCoord, xCoord + 1, zCoord and zCoord + 1

						// Compute the labels for the 4 vertices using nodal coordinate in all three dimensions
						csrData[csrDataPtr] = calculateLabel(xCoord, yCoord, zCoord, 0, this->nX, 0, this->nY, 0);//, this->nZ);
						csrData[csrDataPtr+1] = calculateLabel(xCoord+1, yCoord, zCoord, 0, this->nX, 0, this->nY, 0);//, this->nZ);
						csrData[csrDataPtr+2] = calculateLabel(xCoord+1, yCoord, zCoord+1, 0, this->nX, 0, this->nY, 0);//, this->nZ);
						csrData[csrDataPtr+3] = calculateLabel(xCoord, yCoord, zCoord+1, 0, this->nX, 0, this->nY, 0);//, this->nZ);

						// Advance the Data Pointer
						csrDataPtr = csrDataPtr + 4;
					} else {
						// XY Face
						// Get Face X,Y,Z Coordinates for a XZ Face
						// Dimensions 0->nX-1 * 0->nY-1 * 0->nZ
						// Remember to remove offsets
						I xCoord = calculateXCoord(faceLabels[i] - (nFaceYZ + nFaceXZ), 0, this->nX -1);
						I yCoord = calculateYCoord(faceLabels[i] - (nFaceYZ + nFaceXZ), 0, this->nX -1, 0, this->nY-1);
						I zCoord = calculateZCoord(faceLabels[i] - (nFaceYZ + nFaceXZ), 0, this->nX -1, 0, this->nY-1, 0, this->nZ);

						// In XY plane, all vertices share the same Z coordinate as the face.
						// The set of vertices are then a permutation set of the face's xCoord, xCoord + 1, yCoord and yCoord + 1

						// Compute the labels for the 4 vertices using nodal coordinate in all three dimensions
						csrData[csrDataPtr] = calculateLabel(xCoord, yCoord, zCoord, 0, this->nX, 0, this->nY, 0);//, this->nZ);
						csrData[csrDataPtr+1] = calculateLabel(xCoord+1, yCoord, zCoord, 0, this->nX, 0, this->nY, 0);//, this->nZ);
						csrData[csrDataPtr+2] = calculateLabel(xCoord+1, yCoord+1, zCoord, 0, this->nX, 0, this->nY, 0);//, this->nZ);
						csrData[csrDataPtr+3] = calculateLabel(xCoord, yCoord+1, zCoord, 0, this->nX, 0, this->nY, 0);//, this->nZ);

						// Advance the Data Pointer
						csrDataPtr = csrDataPtr + 4;
					}
				}

				// Update the last position of csrIndices
				csrIndices[nCsrIndices-1] = csrDataPtr;

				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T>
			cupcfd::error::eCodes MeshStructGenSource<I,T>::getVertexCoords(euc::EuclideanPoint<T,3> * vertCoords, I nVertCoords, I * vertexLabels, I nVertexLabels) {
				if(nVertCoords != nVertexLabels) {
					return cupcfd::error::E_ARRAY_SIZE_MISMATCH;
				}

				// Vertices are assigned labels in order of X, Y,Z Coordinates using the node coordinate scheme
				// from 0->nX * 0->nY * 0->nZ (1 higher in each dimension than the number of cells)

				for(I i = 0; i < nVertexLabels; i++) {
					// Determine the X, Y and Z Positional Coordinates
					I xCoord = calculateXCoord(vertexLabels[i], 0, this->nX);
					I yCoord = calculateYCoord(vertexLabels[i], 0, this->nX, 0, this->nY);
					I zCoord = calculateZCoord(vertexLabels[i], 0, this->nX, 0, this->nY, 0, this->nZ);

					// Set X Spatial Coordinate
					vertCoords[i].cmp[0] = this->sMinX + (this->dSx * xCoord);

					// Set Y Spatial Coordinate
					vertCoords[i].cmp[1] = this->sMinY + (this->dSy * yCoord);

					// Set Z Spatial Coordinate
					vertCoords[i].cmp[2] = this->sMinZ + (this->dSz * zCoord);
				}

				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T>
			cupcfd::error::eCodes MeshStructGenSource<I,T>::getBoundaryFaceLabels(I * boundaryFaceLabels, I nBoundaryFaceLabels, I * boundaryLabels, I nBoundaryLabels) {
				if(nBoundaryFaceLabels != nBoundaryLabels) {
					return cupcfd::error::E_ARRAY_SIZE_MISMATCH;
				}

				// Essentially an inversion of face->boundary

				// Number of faces in these planes.
				I nFaceYZ = this->nY * this->nZ * (this->nX + 1);
				I nFaceXZ = this->nX * this->nZ * (this->nY + 1);

				// Number of Boundaries in these planes
				I nBoundYZ = this->nY * this->nZ * 2;
				I nBoundXZ = this->nX * this->nZ * 2;

				// Loop over labels to convert form face labels to boundaries labels
				// Effectively, convert back to coordinates from the face labels.
				// Since this only has boundaries at the edges we can treat the boundaries as having a
				// dimension size of two in one direction.
				// E.g. the labelling scheme for a YZ boundary is effectively 2 * nY * nZ boundaries
				// with coordinates ranging from 0->1 * 0->nY-1 * 0->nZ-1.

				for(I i = 0; i < nBoundaryLabels; i++) {
					// Determine which type of Boundary it is - YZ, XZ or XY.
					if(boundaryLabels[i] < nBoundYZ) {
						// YZ Boundary
						// Get Boundary X,Y,Z Coordinates for a YZ Boundary
						// Dimensions 0->1 * 0->nY-1 * 0->nZ-1
						I xCoord = calculateXCoord(boundaryLabels[i], 0, 1);
						I yCoord = calculateYCoord(boundaryLabels[i], 0, 1, 0, this->nY - 1);
						I zCoord = calculateZCoord(boundaryLabels[i], 0, 1, 0, this->nY - 1, 0, this->nZ - 1);

						if(xCoord == 0) {
							// Low Grid Edge
							boundaryFaceLabels[i] = calculateLabel(0, yCoord, zCoord, 0, this->nX, 0, this->nY - 1, 0);//, this->nZ - 1);
						}
						else if(xCoord == 1) {
							// High Grid Edge
							boundaryFaceLabels[i] = calculateLabel(this->nX, yCoord, zCoord, 0, this->nX, 0, this->nY - 1, 0);//, this->nZ - 1);
						} else {
							// Error - This isn't a boundary edge int he structured grid setup
							return cupcfd::error::E_ERROR;
						}
					}
					else if(boundaryLabels[i] < (nBoundYZ + nBoundXZ)) {
						// XZ Boundary
						// Get Face X,Y,Z Coordinates for a XZ Boundary
						// Dimensions 0->nX-1 * 0->1 * 0->nZ-1
						I xCoord = calculateXCoord(boundaryLabels[i] - nBoundYZ, 0, this->nX -1);
						I yCoord = calculateYCoord(boundaryLabels[i] - nBoundYZ, 0, this->nX -1, 0, 1);
						I zCoord = calculateZCoord(boundaryLabels[i] - nBoundYZ, 0, this->nX -1, 0, 1, 0, this->nZ - 1);

						if(yCoord == 0) {
							// Low Grid Edge
							// Add offset to start indexes in new range
							boundaryFaceLabels[i] = nFaceYZ + calculateLabel(xCoord, 0, zCoord, 0, this->nX - 1, 0, this->nY, 0);//, this->nZ - 1);
						}
						else if(yCoord == 1) {
							// High Grid Edge
							// Add offset to start indexes in new range
							boundaryFaceLabels[i] = nFaceYZ + calculateLabel(xCoord, this->nY, zCoord, 0, this->nX - 1, 0, this->nY, 0);//, this->nZ - 1);
						} else {
							// Error - This isn't a boundary edge int he structured grid setup
							return cupcfd::error::E_ERROR;
						}

					} else {
						// XY Boundary
						// Get Face X,Y,Z Coordinates for a XZ Boundary
						// Dimensions 0->nX-1 * 0->nY-1 * 0->1
						I xCoord = calculateXCoord(boundaryLabels[i] - (nBoundYZ + nBoundXZ), 0, this->nX -1);
						I yCoord = calculateYCoord(boundaryLabels[i] - (nBoundYZ + nBoundXZ), 0, this->nX -1, 0, this->nY-1);
						I zCoord = calculateZCoord(boundaryLabels[i] - (nBoundYZ + nBoundXZ), 0, this->nX -1, 0, this->nY-1, 0, 1);

						if(zCoord == 0) {
							// Low Grid Edge
							// Add offset to start indexes in new range
							boundaryFaceLabels[i] = nFaceYZ + nFaceXZ + calculateLabel(xCoord, yCoord, 0, 0, this->nX - 1, 0, this->nY - 1, 0);//, this->nZ);
						}
						else if(zCoord == 1) {
							// High Grid Edge
							// Add offset to start indexes in new range
							boundaryFaceLabels[i] = nFaceYZ + nFaceXZ + calculateLabel(xCoord, yCoord, this->nZ, 0, this->nX - 1, 0, this->nY - 1, 0);//, this->nZ);
						} else {
							// Error - This isn't a boundary edge int he structured grid setup
							return cupcfd::error::E_ERROR;
						}
					}
				}

				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T>
			cupcfd::error::eCodes MeshStructGenSource<I,T>::getBoundaryNVertices(I * nVertices, I nNVertices __attribute__((unused)), I * boundaryLabels __attribute__((unused)), I nBoundaryLabels) {
				// All boundaries in this setup have 4 vertices.
				for(I i = 0; i < nBoundaryLabels; i++) {
					nVertices[i] = 4;
				}

				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T>
			cupcfd::error::eCodes MeshStructGenSource<I,T>::getBoundaryRegionLabels(I * boundaryRegionLabels, I nBoundaryRegionLabels __attribute__((unused)), I * boundaryLabels __attribute__((unused)), I nBoundaryLabels) {
				// Only one region for now.
				for(I i = 0; i < nBoundaryLabels; i++) {
					boundaryRegionLabels[i] = 0;
				}

				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T>
			cupcfd::error::eCodes MeshStructGenSource<I,T>::getBoundaryVerticesLabelsCSR(I * csrIndices, I nCsrIndices, I * csrData, I nCsrData,  I * boundaryLabels, I nBoundaryLabels) {
				// === Passthrough to the face vertex lookup. ===
				// We can reuse the face lookup method.
				// This does use a little more memory, since we have to temporarily
				// get a store of the face label mapping for the boundaries.
				cupcfd::error::eCodes status;

				// Get the face labels for the boundaries
				I * faceLabels = (I *) malloc(sizeof(I) * nBoundaryLabels);

				status = this->getBoundaryFaceLabels(faceLabels, nBoundaryLabels, boundaryLabels, nBoundaryLabels);
				CHECK_ECODE(status)

				// Use the Face->Vertex Lookup method
				status = this->getFaceVerticesLabelsCSR(csrIndices, nCsrIndices, csrData, nCsrData,  faceLabels, nBoundaryLabels);
				CHECK_ECODE(status)

				free(faceLabels);

				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T>
			cupcfd::error::eCodes MeshStructGenSource<I,T>::getBoundaryDistance(T * boundaryDistance __attribute__((unused)), I nBoundaryDistance __attribute__((unused)), I * boundaryLabels __attribute__((unused)), I nBoundaryLabels __attribute__((unused)))
			{
				// Don't have a value for this currently.
				return cupcfd::error::E_SOURCE_MISSING;
			}

			template <class I, class T>
			cupcfd::error::eCodes MeshStructGenSource<I,T>::getRegionName(std::string * names, I nNames __attribute__((unused)), I * regionLabels __attribute__((unused)), I nRegionLabels)
			{
				// Always return "Default" for now.
				for(I i = 0; i < nRegionLabels; i++) {
					names[i] = "Default";
				}

				return cupcfd::error::E_SUCCESS;
			}
		} // namespace mesh
	} // namespace geometry
} // namespace cupcfd

// Explicit Instantiation
template class cupcfd::geometry::mesh::MeshStructGenSource<int, float>;
template class cupcfd::geometry::mesh::MeshStructGenSource<int, double>;
