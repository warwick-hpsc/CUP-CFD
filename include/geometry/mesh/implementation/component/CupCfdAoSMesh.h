/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 * This implementation was coded from scratch, but the original basis for the
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
 * Contains declarations for the CupCfdAoSMesh Class
 */

#ifndef CUPCFD_GEOMETRY_MESH_CUPCFD_AOS_MESH_INCLUDE_H
#define CUPCFD_GEOMETRY_MESH_CUPCFD_AOS_MESH_INCLUDE_H

// Interface/Base Class
#include "UnstructuredMeshInterface.h"

// Mesh Size Data
#include "UnstructuredMeshProperties.h"

// AoS Structures
#include "CupCfdAoSMeshBoundary.h"
#include "CupCfdAoSMeshCell.h"
#include "CupCfdAoSMeshFace.h"
#include "CupCfdAoSMeshRegion.h"
#include "CupCfdAoSMeshVertex.h"

// Error Codes
#include "Error.h"

// C++ Library
#include <vector>
#include <map>

namespace cupcfd
{
	namespace geometry
	{
		namespace mesh
		{
			/**
			 * Unstructure Mesh Data Storage Format.
			 *
			 * This format is an AoS format for the geometry data, intended to act as
			 * a proxy for comparison.
			 */
			template <class I, class T, class L>
			class CupCfdAoSMesh : public UnstructuredMeshInterface<CupCfdAoSMesh<I,T,L>,I,T,L>
			{
				public:
					// === Members ===

					// Mesh Data

					/** Boundary Data **/
					std::vector<CupCfdAoSMeshBoundary<I,T>> boundaries;

					/** Cell Data **/
					std::vector<CupCfdAoSMeshCell<I,T>> cells;

					/** Face Data **/
					std::vector<CupCfdAoSMeshFace<I,T>> faces;

					/** Region Data **/
					std::vector<CupCfdAoSMeshRegion<I,T>> regions;

					/** Vertices Data **/
					std::vector<CupCfdAoSMeshVertex<T>> vertices;

					// === Cell->Face Mappings ===
					// These two menbers are used for mapping Cells to Faces. Done here as CSR rather than
					// in the Cell structure to avoid inflating the size of Cell structs/classes for
					// proxy reasons

					/** Cell -> Face CSR Lookup Data - XAdj - Cell->A Start Index Lookup **/
					std::vector<I> cellFaceMapCSRXAdj;

					/** Cell -> Face CSR Lookup Data - Adj - Face Indexes **/
					std::vector<I> cellFaceMapCSRAdj;

					/** Storage for number of faces per cell globally across mesh, indexed by local ID **/
					std::vector<I> cellNGFaces;

					/**
					 * Storage for number of faces per cell stored on this mesh, indexed by local ID
					 * (primarily different for ghost cells, where not all faces are stored).
					 *
					 **/
					std::vector<I> cellNFaces;

					/**
					 * Storage for number of vertices per cell globally across mesh, indexed by local ID
					 * - i.e. for ghost cells this stores the actual value and not just what is stored
					 * locally.
					 **/
					std::vector<I> cellNGVertices;

					/**
					 * Storage for number of vertices per cell stored on this mesh, indexed by local ID
					 * - i.e. for ghost cells this stores the actual value and not just what is stored
					 * locally.
					 **/
					std::vector<I> cellNVertices;

					/**
					 * Scratch Space - Map Scheme Build Vertex Label to Local ID
					 * Used as a temporary structure prior to finalising to lookup the locally assigned ID
					 * from the global ID of the index scheme used to build the mesh.
					 */
					std::map<L, I> vertexBuildIDToLocalID;

					/**
					 * Scratch Space - Map Scheme Build Boundary Label to Local ID
					 * Used as a temporary structure prior to finalising to lookup the locally assigned ID
					 * from the global ID of the index scheme used to build the mesh.
					 */
					std::map<L, I> boundaryBuildIDToLocalID;

					/**
					 * Scratch Space - Map Scheme Build Region Label to Local ID
					 * Used as a temporary structure prior to finalising to lookup the locally assigned ID
					 * from the global ID of the index scheme used to build the mesh.
					 */
					std::map<L, I> regionBuildIDToLocalID;

					/**
					 * Scratch Space - Map Scheme Build Cell Label to Local ID
					 * Used as a temporary structure prior to finalising to lookup the locally assigned ID
					 * from the global ID of the index scheme used to build the mesh.
					 */
					std::map<L, I> cellBuildIDToLocalID;

					/**
					 * Scratch Space - Map Scheme Build Face Label to Local ID
					 * Used as a temporary structure prior to finalising to lookup the locally assigned ID
					 * from the global ID of the index scheme used to build the mesh.
					 */
					std::map<L, I> faceBuildIDToLocalID;

					// === Constructors/Deconstructors ===

					/**
					 * Default constructor:
					 * Creates an empty CupCfdAoSMesh with no cells or other components,
					 * and counts set to zero.
					 */
					CupCfdAoSMesh(cupcfd::comm::Communicator& comm);

					/**
					 * Deconstructor.
					 */
					virtual ~CupCfdAoSMesh();

					// === Concrete Methods ===
					void reset();

					// === Inherited Method Overloads ===

					// =========================== Cell Operators ===========================

					inline void getCellCenter(I cellID, cupcfd::geometry::euclidean::EuclideanPoint<T,3>& center);

					inline cupcfd::geometry::euclidean::EuclideanPoint<T,3> getCellCenter(I cellID);

					inline void getCellVolume(I cellID, T * vol);

					inline T getCellVolume(I cellID);

					inline void getCellNFaces(I cellID, I * nFaces);

					inline I getCellNFaces(I cellID);

					inline void getCellStoredNFaces(I cellID, I * nFaces);

					inline I getCellStoredNFaces(I cellID);

					inline void getCellNVertices(I cellID, int * nVertices);

					int getCellNVertices(I cellID);

					inline void getCellStoredNVertices(I cellID, I * nVertices);

					I getCellStoredNVertices(I cellID);

					inline void getCellFaceID(I cellID, I cellFaceID, I * faceID);

					inline I getCellFaceID(I cellID, I cellFaceID);

					inline void setCellCenter(I cellID, cupcfd::geometry::euclidean::EuclideanPoint<T,3>& center);

					inline void setCellVolume(I cellID, T vol);

					__attribute__((warn_unused_result))
					cupcfd::error::eCodes addCell(L cellLabel,
													cupcfd::geometry::euclidean::EuclideanPoint<T,3>& center,
													T vol,
													bool isLocal);

					__attribute__((warn_unused_result))
					cupcfd::error::eCodes addCell(L cellLabel, bool isLocal);

					void getCellID(L cellLabel, I * cellID);

					I getCellID(L cellLabel);

					// =========================== Face Operators ===========================

					inline void getFaceNVertices(I faceID, I * nVertices);
					inline I getFaceNVertices(I faceID);
					inline void getFaceCell1ID(I faceID, I * cellID);
					inline I getFaceCell1ID(I faceID);
					inline void getFaceCell2ID(I faceID, I * cellID);
					inline I getFaceCell2ID(I faceID);
					inline void getFaceLambda(I faceID, T * lambda);
					inline T getFaceLambda(I faceID);
					inline void getFaceNorm(I faceID, cupcfd::geometry::euclidean::EuclideanVector<T,3>& norm);
					inline cupcfd::geometry::euclidean::EuclideanVector<T,3> getFaceNorm(I faceID);
					inline void getFaceVertex(I faceID, I faceVertexID, I * vertexID);
					inline I getFaceVertex(I faceID, I faceVertexID);
					inline void getFaceCenter(I faceID, cupcfd::geometry::euclidean::EuclideanPoint<T,3>& center);
					inline cupcfd::geometry::euclidean::EuclideanPoint<T,3> getFaceCenter(I faceID);
					inline void getFaceRLencos(I faceID, T * rlencos);
					inline T getFaceRLencos(I faceID);
					inline void getFaceArea(I faceID, T * area);
					inline T getFaceArea(I faceID);
					inline void getFaceXpac(I faceID, cupcfd::geometry::euclidean::EuclideanPoint<T,3>& xpac);
					inline cupcfd::geometry::euclidean::EuclideanPoint<T,3> getFaceXpac(I faceID);
					inline void getFaceXnac(I faceID, cupcfd::geometry::euclidean::EuclideanPoint<T,3>& xnac);
					inline cupcfd::geometry::euclidean::EuclideanPoint<T,3> getFaceXnac(I faceID);
					inline void getFaceIsBoundary(I faceID, bool * result);
					inline bool getFaceIsBoundary(I faceID);
					inline void getFaceBoundaryID(I faceID, I * boundaryID);
					inline I getFaceBoundaryID(I faceID);
					inline void setFaceCell1ID(I faceID, I cellID);
					inline void setFaceCell2ID(I faceID, I cellID);
					inline void setFaceLambda(I faceID, T lambda);
					inline void setFaceNorm(I faceID, cupcfd::geometry::euclidean::EuclideanVector<T,3>& norm);
					inline void setFaceVertex(I faceID, I faceVertexID, I vertexID);
					inline void setFaceCenter(I faceID, cupcfd::geometry::euclidean::EuclideanPoint<T,3>& center);
					inline void setFaceRLencos(I faceID, T rlencos);
					inline void setFaceArea(I faceID, T area);
					inline void setFaceXpac(I faceID, cupcfd::geometry::euclidean::EuclideanPoint<T,3>& xpac);
					inline void setFaceXnac(I faceID, cupcfd::geometry::euclidean::EuclideanPoint<T,3>& xnac);
					inline void setFaceBoundaryID(I faceID, I boundaryID);

					__attribute__((warn_unused_result))
					cupcfd::error::eCodes addFace(L faceLabel,
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
													T area);

					__attribute__((warn_unused_result))
					cupcfd::error::eCodes addFace(L faceLabel,
													L cell1Label,
													L cell2OrBoundaryLabel,
													bool isBoundary,
													L * vertexLabels, I nVertexLabels);


					void getFaceID(L faceLabel, I * faceID);
					I getFaceID(L faceLabel);

					// =========================== Boundary Operators ===========================

					inline void getBoundaryFaceID(I boundaryID, I * faceID);
					inline I getBoundaryFaceID(I boundaryID);
					inline void getBoundaryVertex(I boundaryID, I boundaryVertexID, I * vertexID);
					inline I getBoundaryVertex(I boundaryID, I boundaryVertexID);
					inline void getBoundaryDistance(I boundaryID, T * distance);
					inline T getBoundaryDistance(I boundaryID);
					inline void getBoundaryYPlus(I boundaryID, T * yPlus);
					inline T getBoundaryYPlus(I boundaryID);
					inline void getBoundaryUPlus(I boundaryID, T * uPlus);
					inline T getBoundaryUPlus(I boundaryID);
					inline void getBoundaryRegionID(I boundaryID, I * regionID);
					inline I getBoundaryRegionID(I boundaryID);
					inline void getBoundaryShear(I boundaryID, cupcfd::geometry::euclidean::EuclideanVector<T,3>& shear);
					inline cupcfd::geometry::euclidean::EuclideanVector<T,3> getBoundaryShear(I boundaryID);
					inline void getBoundaryQ(I boundaryID, T * q);
					inline T getBoundaryQ(I boundaryID);
					inline void getBoundaryH(I boundaryID, T * h);
					inline T getBoundaryH(I boundaryID);
					inline void getBoundaryT(I boundaryID, T * t);
					inline T getBoundaryT(I boundaryID);
					inline void setBoundaryFaceID(I boundaryID, I faceID);
					inline void setBoundaryVertex(I boundaryID, I boundaryVertexID, I vertexID);
					inline void setBoundaryDistance(I boundaryID, T distance);
					inline void setBoundaryYPlus(I boundaryID, T yPlus);
					inline void setBoundaryUPlus(I boundaryID, T uPlus);
					inline void setBoundaryRegionID(I boundaryID, I regionID);
					inline void setBoundaryShear(I boundaryID, cupcfd::geometry::euclidean::EuclideanVector<T,3>& shear);
					inline void setBoundaryQ(I boundaryID, T q);
					inline void setBoundaryH(I boundaryID, T h);
					inline void setBoundaryT(I boundaryID, T t);

					__attribute__((warn_unused_result))
					cupcfd::error::eCodes addBoundary(L boundaryLabel,
													L regionLabel,
													L * vertexLabels, I nVertexLabels,
													T distance);

					__attribute__((warn_unused_result))
					cupcfd::error::eCodes addBoundary(L boundaryLabel,
													L regionLabel,
													L * vertexLabels, I nVertexLabels);

					void getBoundaryID(L boundaryLabel, I * boundaryID);
					I getBoundaryID(L boundaryLabel);

					// =========================== Region Operators ===========================

					inline void getRegionType(I regionID, RType * type);
					inline RType getRegionType(I regionID);
					inline void getRegionStd(I regionID, bool * std);
					inline bool getRegionStd(I regionID);
					inline void getRegionYLog(I regionID, T * yLog);
					inline T getRegionYLog(I regionID);
					inline void getRegionELog(I regionID, T * eLog);
					inline T getRegionELog(I regionID);
					inline void getRegionDensity(I regionID, T * density);
					inline T getRegionDensity(I regionID);
					inline void getRegionTurbKE(I regionID, T * turbKE);
					inline T getRegionTurbKE(I regionID);
					inline void getRegionTurbDiss(I regionID, T * turbDiss);
					inline T getRegionTurbDiss(I regionID);
					inline void getRegionSplvl(I regionID, T * splvl);
					inline T getRegionSplvl(I regionID);
					inline void getRegionDen(I regionID, T * den);
					inline T getRegionDen(I regionID);
					inline void getRegionForceTangent(I regionID, cupcfd::geometry::euclidean::EuclideanVector<T,3>& forceTangent);
					inline cupcfd::geometry::euclidean::EuclideanVector<T,3> getRegionForceTangent(I regionID);
					inline void getRegionUVW(I regionID, cupcfd::geometry::euclidean::EuclideanVector<T,3>& uvw);
					inline cupcfd::geometry::euclidean::EuclideanVector<T,3> getRegionUVW(I regionID);
					inline void getRegionName(I regionID, std::string& regionName);
					inline std::string getRegionName(I regionID);
					inline void getRegionFlux(I regionID, bool * flux);
					inline bool getRegionFlux(I regionID);
					inline void getRegionAdiab(I regionID, bool * adiab);
					inline bool getRegionAdiab(I regionID);
					inline void getRegionR(I regionID, T * r);
					inline T getRegionR(I regionID);
					inline void getRegionT(I regionID, T * t);
					inline T getRegionT(I regionID);
					inline void setRegionType(I regionID, RType type);
					inline void setRegionStd(I regionID, bool std);
					inline void setRegionYLog(I regionID, T yLog);
					inline void setRegionELog(I regionID, T eLog);
					inline void setRegionDensity(I regionID, T density);
					inline void setRegionTurbKE(I regionID, T turbKE);
					inline void setRegionTurbDiss(I regionID, T turbDiss);
					inline void setRegionSplvl(I regionID, T splvl);
					inline void setRegionDen(I regionID, T den);
					inline void setRegionForceTangent(I regionID, cupcfd::geometry::euclidean::EuclideanVector<T,3>& forceTangent);
					inline void setRegionUVW(I regionID, cupcfd::geometry::euclidean::EuclideanVector<T,3>& uvw);
					inline void setRegionName(I regionID, std::string& regionName);
					inline void setRegionFlux(I regionID, bool flux);
					inline void setRegionAdiab(I regionID, bool adiab);
					inline void setRegionR(I regionID, T r);
					inline void setRegionT(I regionID, T t);

					__attribute__((warn_unused_result))
					cupcfd::error::eCodes addRegion(L regionLabel,
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
													std::string& regionName);

					__attribute__((warn_unused_result))
					cupcfd::error::eCodes addRegion(L regionLabel,
													std::string& regionName);

					void getRegionID(L regionLabel, I * regionID);
					I getRegionID(L regionLabel);

					// =========================== Vertex Operators ===========================

					inline void getVertexPos(I vertexID, cupcfd::geometry::euclidean::EuclideanPoint<T,3>& pos);
					inline cupcfd::geometry::euclidean::EuclideanPoint<T,3> getVertexPos(I vertexID);
					inline void setVertexPos(I vertexID, cupcfd::geometry::euclidean::EuclideanPoint<T,3>& pos);
					__attribute__((warn_unused_result))
					cupcfd::error::eCodes addVertex(L vertexLabel, cupcfd::geometry::euclidean::EuclideanPoint<T,3>& pos);
					inline void getVertexID(L vertexLabel, I * vertexID);
					inline I getVertexID(L vertexLabel);

					// === Other ===

					/**
					 * The finalize step of CupCfdAoSMesh does the following:
					 * (1) Refreshes the Cell -> Face Mapping based on the set Face(Cell1) and Face(Cell2) values
					 * (2) Updates the mesh properties values based on the currently stored data
					 */
					__attribute__((warn_unused_result))
					cupcfd::error::eCodes finalize();

					// === CupCfdAoSMesh Specific Concrete Methods ===

					/**
					 *
					 */
					__attribute__((warn_unused_result))
					cupcfd::error::eCodes updateCellLocalIndexes();

					/**
					 * This updates the internal CSR members responsible for storing the cell->face mappings,
					 * as well as the number of stored faces and vertices per cell.
					 * This mapping is not updated upon adding a new face, since it would result in O(m * n) operations
					 * (Lookup indexes for O(n) cells would have to be updated each time a face was added)
					 *
					 * Instead, this function does a single pass to build the CSR data structure using the currently stored face
					 * and cell data.
					 *
					 * @tparam I The type of the indexing scheme (integer based)
					 * @tparam T The type of the stored array data
					 *
					 * @return An error status indicating the success or failure of the operation
					 * @retval cupcfd::error::E_ARRAY_SUCCESS Success
					 */
					__attribute__((warn_unused_result))
					cupcfd::error::eCodes updateCellFaceMap();

					/**
					 *
					 */
					__attribute__((warn_unused_result))
					cupcfd::error::eCodes exchangeCellGlobalNVertices();

					/**
					 *
					 */
					__attribute__((warn_unused_result))
					cupcfd::error::eCodes exchangeCellGlobalNFaces();

					// === Pure Methods ===
			};
		}
	}
}

// Include Header Level Definitions
#include "CupCfdAoSMesh.ipp"

#endif
