/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Contains the declarations for the MeshStructGenSource class
 */

#ifndef CUPCFD_GEOMETRY_MESH_MESH_STRUCT_GEN_SOURCE_INCLUDE_H
#define CUPCFD_GEOMETRY_MESH_MESH_STRUCT_GEN_SOURCE_INCLUDE_H

// Parent Class
#include "MeshSource.h"

namespace cupcfd
{
	namespace geometry
	{
		namespace mesh
		{
			/**
			 * This class uses the provided parameters to generate
			 * the details of a regular structured grid.
			 *
			 * It uses a fixed labelling format to ensure that the values
			 * are consistent across ranks since neighbouring cells may cross
			 * boundaries and thus their labels must be correct.
			 *
			 * The grid consists of a set of cell-centered positional cartesian coordinates for the position of
			 * the cell (0,0,0) -> (n-1, n-1, n-1), with a seperate
			 * set of cartesian spatial coordinates xMin, xMax, yMin, yMax
			 * and zMin, zMax in the spatial domain.
			 *
			 * The values of xMin, xMax, yMin, yMax, zMin and zMax can be derived from the
			 * position coordinate (x,y,z) of the cell, and likewise the neighbours can be
			 * determined from the same position coordinate [(x-1, y, z), (x+1,y,z) etc].
			 *
			 * Vertices and Faces are labelled in the order in which they are encountered,
			 * traversing through the x dimension, then y, then x.
			 * The positional coordinates used for these are node center and face center respectively.
			 *
			 * Uses the same type as the indexing scheme for the labelling scheme.
			 */
			template <class I, class T>
			class MeshStructGenSource : public MeshSource<I,T,I>
			{
				public:
					// === Members ===

					/** The number of cells in the X dimension **/
					I nX;

					/** The number of cells in the Y dimension **/
					I nY;

					/** The number of cells in the Z dimension **/
					I nZ;

					/** The minimum spatial size in the X dimension **/
					T sMinX;

					/** The minimum spatial size in the Y dimension **/
					T sMinY;

					/** The minimum spatial size in the Z dimension **/
					T sMinZ;

					/** The maximum spatial size in the X dimension **/
					T sMaxX;

					/** The maximum spatial size in the Y dimension **/
					T sMaxY;

					/** The maximum spatial size in the Z dimension **/
					T sMaxZ;

					/** X Spatial Divisions **/
					T dSx;

					/** Y Spatial Divisions **/
					T dSy;

					/** Z Spatial Divisions **/
					T dSz;

					// === Constructors/Deconstructors ===

					/**
					 * Open the specified file and treat it as a HDF5 file
					 * with a the specific HDF5 schema.
					 *
					 * @param nX The number of cells in the X dimension
					 * @param nY The number of cells in the Y dimension
					 * @param nZ The number of cells in the Z dimension
					 * @param sMinX The minimum spatial size in the X dimension
					 * @param sMaxX The maximum spatial size in the X dimension
					 * @param sMinY The minimum spatial size in the Y dimension
					 * @param sMaxY The maximum spatial size in the Y dimension
					 * @param sMinZ The minimum spatial size in the Z dimension
					 * @param sMaxZ The maximum spatial size in the Z dimension
					 */
					MeshStructGenSource(I nX, I nY, I nZ, T sMinX, T sMaxX, T sMinY, T sMaxY, T sMinZ, T sMaxZ);

					/**
					 * Deconstructor.
					 */
					virtual ~MeshStructGenSource();

					// === Concrete Methods ===

					/**
					 * Utility function for generation - Computes a unique label given a positional coordinate
					 * and the coordinate range across a coordinate system.
					 *
					 * @param xCoord The X Positional Coordinate
					 * @param yCoord The Y Positional Coordinate
					 * @param zCoord The Z Positional Coordinate
					 * @param xMin The minimum X Positional Coordinate in the scheme
					 * @param xMax The maximum X Positional Coordinate in the scheme
					 * @param yMin The minimum Y Positional Coordinate in the scheme
					 * @param yMax The minimum Y Positional Coordinate in the scheme
					 * @param zMin The minimum Z Positional Coordinate in the scheme
					 * @param zMax The minimum Z Positional Coordinate in the scheme
					 *
					 * @tparam I The type of the coordinate scheme (integer based)
					 *
					 * @return The unique coordinate label
					 */
					I calculateLabel(I xCoord, I yCoord, I zCoord, I xMin, I xMax, I yMin, I yMax, I zMin);

					/**
					 * Utility functions - compute the X coordinate in a scheme for a given label.
					 * Labels are assigned in order of traversing X, then Y, then Z coordinates.
					 *
					 * @param label The label
					 * @param xMin The minimum X Positional Coordinate in the scheme
					 * @param xMax The maximum X Positional Coordinate in the scheme
					 */
					I calculateXCoord(I label, I xMin, I xMax);

					/**
					 * Utility functions - compute the Y coordinate in a scheme for a given label.
					 * Labels are assigned in order of traversing X, then Y, then Z coordinates.
					 *
					 * @param label The label
					 * @param xMin The minimum X Positional Coordinate in the scheme
					 * @param xMax The maximum X Positional Coordinate in the scheme
					 * @param yMin The minimum Y Positional Coordinate in the scheme
					 * @param yMax The minimum Y Positional Coordinate in the scheme
					 */
					I calculateYCoord(I label, I xMin, I xMax, I yMin, I yMax);


					/**
					 * Utility functions - compute the Z coordinate in a scheme for a given label.
					 * Labels are assigned in order of traversing X, then Y, then Z coordinates.
					 *
					 * @param label The label
					 * @param xMin The minimum X Positional Coordinate in the scheme
					 * @param xMax The maximum X Positional Coordinate in the scheme
					 * @param yMin The minimum Y Positional Coordinate in the scheme
					 * @param yMax The minimum Y Positional Coordinate in the scheme
					 * @param zMin The minimum Z Positional Coordinate in the scheme
					 * @param zMax The minimum Z Positional Coordinate in the scheme
					 */
					I calculateZCoord(I label, I xMin, I xMax, I yMin, I yMax, I zMin, I zMax);

					// === Overloaded Inherited Virtual Methods ===

					cupcfd::error::eCodes getCellLabels(I * labels, I nLabels, I * indexes, I nIndexes);
					cupcfd::error::eCodes getFaceLabels(I * labels, I nLabels, I * indexes, I nIndexes);
					cupcfd::error::eCodes getVertexLabels(I * labels, I nLabels, I * indexes, I nIndexes);
					cupcfd::error::eCodes getBoundaryLabels(I * labels, I nLabels, I * indexes, I nIndexes);
					cupcfd::error::eCodes getRegionLabels(I * labels, I nLabels, I * indexes, I nIndexes);

					cupcfd::error::eCodes getCellCount(I * cellCount);
					cupcfd::error::eCodes getFaceCount(I * faceCount);
					cupcfd::error::eCodes getBoundaryCount(I * boundaryCount);
					cupcfd::error::eCodes getRegionCount(I * regionCount);
					cupcfd::error::eCodes getVertexCount(I * vertexCount);
					cupcfd::error::eCodes getMaxFaceCount(I * maxFaceCount);
					cupcfd::error::eCodes getMaxVertexCount(I * maxVertexCount);
					cupcfd::error::eCodes getCellNFaces(I * nFaces, I nNFaces, I * cellLabels, I nCellLabels);
					cupcfd::error::eCodes getCellVolume(T * cellVol, I nCellVol, I * cellLabels, I nCellLabels);
					cupcfd::error::eCodes getCellCenter(cupcfd::geometry::euclidean::EuclideanPoint<T,3> * cellCenter, I nCellCenter, I * cellLabels, I nCellLabels);
					cupcfd::error::eCodes getCellFaceLabels(I * csrIndices, I nCsrIndices, I * csrData, I nCsrData, I * cellLabels, I nCellLabels);
					cupcfd::error::eCodes getFaceIsBoundary(bool * isBoundary, I nIsBoundary, I * faceLabels, I nFaceLabels);
					cupcfd::error::eCodes getFaceNVertices(I * nVertices, I nNVertices, I * faceLabels, I nFaceLabels);
					cupcfd::error::eCodes getFaceBoundaryLabels(I * faceBoundaryLabels, I nFaceBoundaryLabels, I * faceLabels, I nFaceLabels);
					cupcfd::error::eCodes getFaceCell1Labels(I * faceCell1Labels, I nFaceCell1Labels, I * faceLabels, I nFaceLabels);
					cupcfd::error::eCodes getFaceCell2Labels(I * faceCell2Labels, I nFaceCell2Labels, I * faceLabels, I nFaceLabels);
					cupcfd::error::eCodes getFaceLambda(T * faceLambda, I nFaceLambda, I * faceLabels, I nFaceLabels);
					cupcfd::error::eCodes getFaceArea(T * faceArea, I nFaceArea, I * faceLabels, I nFaceLabels);
					cupcfd::error::eCodes getFaceNormal(cupcfd::geometry::euclidean::EuclideanVector<T,3> * faceNormal, I nFaceNormal, I * faceLabels, I nFaceLabels);
					cupcfd::error::eCodes getFaceCenter(cupcfd::geometry::euclidean::EuclideanPoint<T,3> * faceCenter, I nFaceCenter, I * faceLabels, I nFaceLabels);
					cupcfd::error::eCodes getFaceVerticesLabelsCSR(I * csrIndices, I nCsrIndices, I * csrData, I nCsrData,  I * faceLabels, I nFaceLabels);
					cupcfd::error::eCodes getVertexCoords(cupcfd::geometry::euclidean::EuclideanPoint<T,3> * vertCoords, I nVertCoords, I * vertexLabels, I nVertexLabels);
					cupcfd::error::eCodes getBoundaryFaceLabels(I * boundaryFaceLabels, I nBoundaryFaceLabels, I * boundaryLabels, I nBoundaryLabels);
					cupcfd::error::eCodes getBoundaryNVertices(I * nVertices, I nNVertices, I * boundaryLabels, I nBoundaryLabels);
					cupcfd::error::eCodes getBoundaryRegionLabels(I * boundaryRegionLabels, I nBoundaryRegionLabels, I * boundaryLabels, I nBoundaryLabels);
					cupcfd::error::eCodes getBoundaryVerticesLabelsCSR(I * csrIndices, I nCsrIndices, I * csrData, I nCsrData,  I * faceLabels, I nFaceLabels);
					cupcfd::error::eCodes getBoundaryDistance(T * boundaryDistance, I nBoundaryDistance, I * boundaryLabels, I nBoundaryLabels);
					cupcfd::error::eCodes getRegionName(std::string * names, I nNames, I * regionLabels, I nRegionLabels);
			};
		} // namespace mesh
	} // namespace geometry
} // namespace cupcfd

#endif
