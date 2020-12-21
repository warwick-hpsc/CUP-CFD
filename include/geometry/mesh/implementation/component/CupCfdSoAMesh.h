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
 * Contains declarations for the CupCfdSoAMesh Class
 */

#ifndef CUPCFD_GEOMETRY_MESH_CUPCFD_SOA_MESH_INCLUDE_H
#define CUPCFD_GEOMETRY_MESH_CUPCFD_SOA_MESH_INCLUDE_H

// Interface/Base Class
#include "UnstructuredMeshInterface.h"

// Mesh Size Data
#include "UnstructuredMeshProperties.h"

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
			class CupCfdSoAMesh : public UnstructuredMeshInterface<CupCfdSoAMesh<I,T,L>,I,T,L>
			{
				public:
					// === Members ===

					// Mesh Data

					// ToDo: Think vectors are usually contiguous, but are they guaranteed to be?
					// Worst case is copy across to an array once the finalize step is triggered.

					/** Boundary Data **/
					std::vector<I> boundaryFaceID;
					std::vector<std::array<I,4>> boundaryVertexID;
					std::vector<T> boundaryDistance;
					std::vector<T> boundaryYPlus;
					std::vector<T> boundaryUPlus;
					std::vector<I> boundaryRegionID;
					std::vector<cupcfd::geometry::euclidean::EuclideanVector<T,3>> boundaryShear;
					std::vector<T> boundaryQ;
					std::vector<T> boundaryH;
					std::vector<T> boundaryT;

					/** Cell Data **/
					std::vector<cupcfd::geometry::euclidean::EuclideanPoint<T,3>> cellCenter;
					std::vector<T> cellVolume;

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

					// These two menbers are used for mapping Cells to Faces. Done here as CSR rather than
					// in the Cell structure to avoid inflating the size of Cell structs/classes for
					// proxy reasons

					/** Cell -> Face CSR Lookup Data - XAdj - Cell->A Start Index Lookup **/
					std::vector<I> cellFaceMapCSRXAdj;

					/** Cell -> Face CSR Lookup Data - Adj - Face Indexes **/
					std::vector<I> cellFaceMapCSRAdj;


					/** Face Data **/
					std::vector<I> faceCell1ID;
					std::vector<I> faceCell2ID;
					std::vector<T> faceLambda;
					std::vector<cupcfd::geometry::euclidean::EuclideanVector<T,3>> faceNorm;
					std::vector<cupcfd::geometry::euclidean::EuclideanPoint<T,3>> faceCenter;
					std::vector<T> faceRLencos;
					std::vector<T> faceArea;
					std::vector<cupcfd::geometry::euclidean::EuclideanPoint<T,3>> faceXpac;
					std::vector<cupcfd::geometry::euclidean::EuclideanPoint<T,3>> faceXnac;
					std::vector<I> faceBoundaryID;

					// Currently hard-coded to 4 vertices max.
					std::vector<std::array<I,4>> faceVertexID;


					/** Region Data **/
					std::vector<RType> regionType;
					std::vector<bool> regionStd;
					std::vector<T> regionYLog;
					std::vector<T> regionELog;
					std::vector<T> regionDensity;
					std::vector<T> regionTurbKE;
					std::vector<T> regionTurbDiss;
					std::vector<T> regionSplvl;
					std::vector<T> regionDen;
					std::vector<cupcfd::geometry::euclidean::EuclideanVector<T,3>> regionForceTangent;
					std::vector<cupcfd::geometry::euclidean::EuclideanVector<T,3>> regionUVW;
					std::vector<std::string> regionName;
					std::vector<bool> regionFlux;
					std::vector<bool> regionAdiab;
					std::vector<T> regionR;
					std::vector<T> regionT;

					/** Vertices Data **/
					std::vector<cupcfd::geometry::euclidean::EuclideanPoint<T,3>> verticesPos;


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
					CupCfdSoAMesh(cupcfd::comm::Communicator& comm);

					/**
					 * Deconstructor.
					 */
					virtual ~CupCfdSoAMesh();

					// === Concrete Methods ===
					__attribute__((warn_unused_result))
					cupcfd::error::eCodes reset();

					// === Inherited Method Overloads ===

					// =========================== Cell Operators ===========================

					__attribute__((warn_unused_result))
					inline cupcfd::error::eCodes getCellCenter(I cellID, cupcfd::geometry::euclidean::EuclideanPoint<T,3>& center);

					inline cupcfd::geometry::euclidean::EuclideanPoint<T,3> getCellCenter(I cellID);

					__attribute__((warn_unused_result))
					inline cupcfd::error::eCodes getCellVolume(I cellID, T * vol);

					inline T getCellVolume(I cellID);

					__attribute__((warn_unused_result))
					inline cupcfd::error::eCodes getCellNFaces(I cellID, I * nFaces);

					inline I getCellNFaces(I cellID);

					__attribute__((warn_unused_result))
					inline cupcfd::error::eCodes getCellStoredNFaces(I cellID, I * nFaces);

					inline I getCellStoredNFaces(I cellID);

					__attribute__((warn_unused_result))
					inline cupcfd::error::eCodes getCellNVertices(I cellID, int * nVertices);

					int getCellNVertices(I cellID);

					__attribute__((warn_unused_result))
					inline cupcfd::error::eCodes getCellStoredNVertices(I cellID, I * nVertices);

					I getCellStoredNVertices(I cellID);

					__attribute__((warn_unused_result))
					inline cupcfd::error::eCodes getCellFaceID(I cellID, I cellFaceID, I * faceID);

					inline I getCellFaceID(I cellID, I cellFaceID);

					__attribute__((warn_unused_result))
					inline cupcfd::error::eCodes setCellCenter(I cellID, cupcfd::geometry::euclidean::EuclideanPoint<T,3>& center);

					__attribute__((warn_unused_result))
					inline cupcfd::error::eCodes setCellVolume(I cellID, T vol);

					__attribute__((warn_unused_result))
					cupcfd::error::eCodes addCell(L cellLabel,
													cupcfd::geometry::euclidean::EuclideanPoint<T,3>& center,
													T vol,
													bool isLocal);

					__attribute__((warn_unused_result))
					cupcfd::error::eCodes addCell(L cellLabel, bool isLocal);

					__attribute__((warn_unused_result))
					cupcfd::error::eCodes getCellID(L cellLabel, I * cellID);

					I getCellID(L cellLabel);

					// =========================== Face Operators ===========================

					__attribute__((warn_unused_result))
					inline cupcfd::error::eCodes getFaceNVertices(I faceID, I * nVertices);
					inline I getFaceNVertices(I faceID);
					__attribute__((warn_unused_result))
					inline cupcfd::error::eCodes getFaceCell1ID(I faceID, I * cellID);
					inline I getFaceCell1ID(I faceID);
					__attribute__((warn_unused_result))
					inline cupcfd::error::eCodes getFaceCell2ID(I faceID, I * cellID);
					inline I getFaceCell2ID(I faceID);
					__attribute__((warn_unused_result))
					inline cupcfd::error::eCodes getFaceLambda(I faceID, T * lambda);
					inline T getFaceLambda(I faceID);
					__attribute__((warn_unused_result))
					inline cupcfd::error::eCodes getFaceNorm(I faceID, cupcfd::geometry::euclidean::EuclideanVector<T,3>& norm);
					inline cupcfd::geometry::euclidean::EuclideanVector<T,3> getFaceNorm(I faceID);
					__attribute__((warn_unused_result))
					inline cupcfd::error::eCodes getFaceVertex(I faceID, I faceVertexID, I * vertexID);
					inline I getFaceVertex(I faceID, I faceVertexID);
					__attribute__((warn_unused_result))
					inline cupcfd::error::eCodes getFaceCenter(I faceID, cupcfd::geometry::euclidean::EuclideanPoint<T,3>& center);
					inline cupcfd::geometry::euclidean::EuclideanPoint<T,3> getFaceCenter(I faceID);
					__attribute__((warn_unused_result))
					inline cupcfd::error::eCodes getFaceRLencos(I faceID, T * rlencos);
					inline T getFaceRLencos(I faceID);
					__attribute__((warn_unused_result))
					inline cupcfd::error::eCodes getFaceArea(I faceID, T * area);
					inline T getFaceArea(I faceID);
					__attribute__((warn_unused_result))
					inline cupcfd::error::eCodes getFaceXpac(I faceID, cupcfd::geometry::euclidean::EuclideanPoint<T,3>& xpac);
					inline cupcfd::geometry::euclidean::EuclideanPoint<T,3> getFaceXpac(I faceID);
					__attribute__((warn_unused_result))
					inline cupcfd::error::eCodes getFaceXnac(I faceID, cupcfd::geometry::euclidean::EuclideanPoint<T,3>& xnac);
					inline cupcfd::geometry::euclidean::EuclideanPoint<T,3> getFaceXnac(I faceID);
					__attribute__((warn_unused_result))
					inline cupcfd::error::eCodes getFaceIsBoundary(I faceID, bool * result);
					inline bool getFaceIsBoundary(I faceID);
					__attribute__((warn_unused_result))
					inline cupcfd::error::eCodes getFaceBoundaryID(I faceID, I * boundaryID);
					inline I getFaceBoundaryID(I faceID);
					__attribute__((warn_unused_result))
					inline cupcfd::error::eCodes setFaceCell1ID(I faceID, I cellID);
					__attribute__((warn_unused_result))
					inline cupcfd::error::eCodes setFaceCell2ID(I faceID, I cellID);
					__attribute__((warn_unused_result))
					inline cupcfd::error::eCodes setFaceLambda(I faceID, T lambda);
					__attribute__((warn_unused_result))
					inline cupcfd::error::eCodes setFaceNorm(I faceID, cupcfd::geometry::euclidean::EuclideanVector<T,3>& norm);
					__attribute__((warn_unused_result))
					inline cupcfd::error::eCodes setFaceVertex(I faceID, I faceVertexID, I vertexID);
					__attribute__((warn_unused_result))
					inline cupcfd::error::eCodes setFaceCenter(I faceID, cupcfd::geometry::euclidean::EuclideanPoint<T,3>& center);
					__attribute__((warn_unused_result))
					inline cupcfd::error::eCodes setFaceRLencos(I faceID, T rlencos);
					__attribute__((warn_unused_result))
					inline cupcfd::error::eCodes setFaceArea(I faceID, T area);
					__attribute__((warn_unused_result))
					inline cupcfd::error::eCodes setFaceXpac(I faceID, cupcfd::geometry::euclidean::EuclideanPoint<T,3>& xpac);
					__attribute__((warn_unused_result))
					inline cupcfd::error::eCodes setFaceXnac(I faceID, cupcfd::geometry::euclidean::EuclideanPoint<T,3>& xnac);
					__attribute__((warn_unused_result))
					inline cupcfd::error::eCodes setFaceBoundaryID(I faceID, I boundaryID);

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


					__attribute__((warn_unused_result))
					cupcfd::error::eCodes getFaceID(L faceLabel, I * faceID);
					I getFaceID(L faceLabel);

					// =========================== Boundary Operators ===========================

__attribute__((warn_unused_result))
					inline cupcfd::error::eCodes getBoundaryFaceID(I boundaryID, I * faceID);
					inline I getBoundaryFaceID(I boundaryID);
					__attribute__((warn_unused_result))
					inline cupcfd::error::eCodes getBoundaryVertex(I boundaryID, I boundaryVertexID, I * vertexID);
					inline I getBoundaryVertex(I boundaryID, I boundaryVertexID);
					__attribute__((warn_unused_result))
					inline cupcfd::error::eCodes getBoundaryDistance(I boundaryID, T * distance);
					inline T getBoundaryDistance(I boundaryID);
					__attribute__((warn_unused_result))
					inline cupcfd::error::eCodes getBoundaryYPlus(I boundaryID, T * yPlus);
					inline T getBoundaryYPlus(I boundaryID);
					__attribute__((warn_unused_result))
					inline cupcfd::error::eCodes getBoundaryUPlus(I boundaryID, T * uPlus);
					inline T getBoundaryUPlus(I boundaryID);
					__attribute__((warn_unused_result))
					inline cupcfd::error::eCodes getBoundaryRegionID(I boundaryID, I * regionID);
					inline I getBoundaryRegionID(I boundaryID);
					__attribute__((warn_unused_result))
					inline cupcfd::error::eCodes getBoundaryShear(I boundaryID, cupcfd::geometry::euclidean::EuclideanVector<T,3>& shear);
					inline cupcfd::geometry::euclidean::EuclideanVector<T,3> getBoundaryShear(I boundaryID);
					__attribute__((warn_unused_result))
					inline cupcfd::error::eCodes getBoundaryQ(I boundaryID, T * q);
					inline T getBoundaryQ(I boundaryID);
					__attribute__((warn_unused_result))
					inline cupcfd::error::eCodes getBoundaryH(I boundaryID, T * h);
					inline T getBoundaryH(I boundaryID);
					__attribute__((warn_unused_result))
					inline cupcfd::error::eCodes getBoundaryT(I boundaryID, T * t);
					inline T getBoundaryT(I boundaryID);
					__attribute__((warn_unused_result))
					inline cupcfd::error::eCodes setBoundaryFaceID(I boundaryID, I faceID);
					__attribute__((warn_unused_result))
					inline cupcfd::error::eCodes setBoundaryVertex(I boundaryID, I boundaryVertexID, I vertexID);
					__attribute__((warn_unused_result))
					inline cupcfd::error::eCodes setBoundaryDistance(I boundaryID, T distance);
					__attribute__((warn_unused_result))
					inline cupcfd::error::eCodes setBoundaryYPlus(I boundaryID, T yPlus);
					__attribute__((warn_unused_result))
					inline cupcfd::error::eCodes setBoundaryUPlus(I boundaryID, T uPlus);
					__attribute__((warn_unused_result))
					inline cupcfd::error::eCodes setBoundaryRegionID(I boundaryID, I regionID);
					__attribute__((warn_unused_result))
					inline cupcfd::error::eCodes setBoundaryShear(I boundaryID, cupcfd::geometry::euclidean::EuclideanVector<T,3>& shear);
					__attribute__((warn_unused_result))
					inline cupcfd::error::eCodes setBoundaryQ(I boundaryID, T q);
					__attribute__((warn_unused_result))
					inline cupcfd::error::eCodes setBoundaryH(I boundaryID, T h);
					__attribute__((warn_unused_result))
					inline cupcfd::error::eCodes setBoundaryT(I boundaryID, T t);

					__attribute__((warn_unused_result))
					cupcfd::error::eCodes addBoundary(L boundaryLabel,
													L regionLabel,
													L * vertexLabels, I nVertexLabels,
													T distance);

					__attribute__((warn_unused_result))
					cupcfd::error::eCodes addBoundary(L boundaryLabel,
													L regionLabel,
													L * vertexLabels, I nVertexLabels);

					__attribute__((warn_unused_result))
					cupcfd::error::eCodes getBoundaryID(L boundaryLabel, I * boundaryID);
					I getBoundaryID(L boundaryLabel);

					// =========================== Region Operators ===========================

					__attribute__((warn_unused_result))
					inline cupcfd::error::eCodes getRegionType(I regionID, RType * type);
					inline RType getRegionType(I regionID);
					__attribute__((warn_unused_result))
					inline cupcfd::error::eCodes getRegionStd(I regionID, bool * std);
					inline bool getRegionStd(I regionID);
					__attribute__((warn_unused_result))
					inline cupcfd::error::eCodes getRegionYLog(I regionID, T * yLog);
					inline T getRegionYLog(I regionID);
					__attribute__((warn_unused_result))
					inline cupcfd::error::eCodes getRegionELog(I regionID, T * eLog);
					inline T getRegionELog(I regionID);
					__attribute__((warn_unused_result))
					inline cupcfd::error::eCodes getRegionDensity(I regionID, T * density);
					inline T getRegionDensity(I regionID);
					__attribute__((warn_unused_result))
					inline cupcfd::error::eCodes getRegionTurbKE(I regionID, T * turbKE);
					inline T getRegionTurbKE(I regionID);
					__attribute__((warn_unused_result))
					inline cupcfd::error::eCodes getRegionTurbDiss(I regionID, T * turbDiss);
					inline T getRegionTurbDiss(I regionID);
					__attribute__((warn_unused_result))
					inline cupcfd::error::eCodes getRegionSplvl(I regionID, T * splvl);
					inline T getRegionSplvl(I regionID);
					__attribute__((warn_unused_result))
					inline cupcfd::error::eCodes getRegionDen(I regionID, T * den);
					inline T getRegionDen(I regionID);
					__attribute__((warn_unused_result))
					inline cupcfd::error::eCodes getRegionForceTangent(I regionID, cupcfd::geometry::euclidean::EuclideanVector<T,3>& forceTangent);
					inline cupcfd::geometry::euclidean::EuclideanVector<T,3> getRegionForceTangent(I regionID);
					__attribute__((warn_unused_result))
					inline cupcfd::error::eCodes getRegionUVW(I regionID, cupcfd::geometry::euclidean::EuclideanVector<T,3>& uvw);
					inline cupcfd::geometry::euclidean::EuclideanVector<T,3> getRegionUVW(I regionID);
					__attribute__((warn_unused_result))
					inline cupcfd::error::eCodes getRegionName(I regionID, std::string& regionName);
					inline std::string getRegionName(I regionID);
					__attribute__((warn_unused_result))
					inline cupcfd::error::eCodes getRegionFlux(I regionID, bool * flux);
					inline bool getRegionFlux(I regionID);
					__attribute__((warn_unused_result))
					inline cupcfd::error::eCodes getRegionAdiab(I regionID, bool * adiab);
					inline bool getRegionAdiab(I regionID);
					__attribute__((warn_unused_result))
					inline cupcfd::error::eCodes getRegionR(I regionID, T * r);
					inline T getRegionR(I regionID);
					__attribute__((warn_unused_result))
					inline cupcfd::error::eCodes getRegionT(I regionID, T * t);
					inline T getRegionT(I regionID);
					__attribute__((warn_unused_result))
					inline cupcfd::error::eCodes setRegionType(I regionID, RType type);
					__attribute__((warn_unused_result))
					inline cupcfd::error::eCodes setRegionStd(I regionID, bool std);
					__attribute__((warn_unused_result))
					inline cupcfd::error::eCodes setRegionYLog(I regionID, T yLog);
					__attribute__((warn_unused_result))
					inline cupcfd::error::eCodes setRegionELog(I regionID, T eLog);
					__attribute__((warn_unused_result))
					inline cupcfd::error::eCodes setRegionDensity(I regionID, T density);
					__attribute__((warn_unused_result))
					inline cupcfd::error::eCodes setRegionTurbKE(I regionID, T turbKE);
					__attribute__((warn_unused_result))
					inline cupcfd::error::eCodes setRegionTurbDiss(I regionID, T turbDiss);
					__attribute__((warn_unused_result))
					inline cupcfd::error::eCodes setRegionSplvl(I regionID, T splvl);
					__attribute__((warn_unused_result))
					inline cupcfd::error::eCodes setRegionDen(I regionID, T den);
					__attribute__((warn_unused_result))
					inline cupcfd::error::eCodes setRegionForceTangent(I regionID, cupcfd::geometry::euclidean::EuclideanVector<T,3>& forceTangent);
					__attribute__((warn_unused_result))
					inline cupcfd::error::eCodes setRegionUVW(I regionID, cupcfd::geometry::euclidean::EuclideanVector<T,3>& uvw);
					__attribute__((warn_unused_result))
					inline cupcfd::error::eCodes setRegionName(I regionID, std::string& regionName);
					__attribute__((warn_unused_result))
					inline cupcfd::error::eCodes setRegionFlux(I regionID, bool flux);
					__attribute__((warn_unused_result))
					inline cupcfd::error::eCodes setRegionAdiab(I regionID, bool adiab);
					__attribute__((warn_unused_result))
					inline cupcfd::error::eCodes setRegionR(I regionID, T r);
					__attribute__((warn_unused_result))
					inline cupcfd::error::eCodes setRegionT(I regionID, T t);

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

					__attribute__((warn_unused_result))
					cupcfd::error::eCodes getRegionID(L regionLabel, I * regionID);
					I getRegionID(L regionLabel);

					// =========================== Vertex Operators ===========================

					__attribute__((warn_unused_result))
					inline cupcfd::error::eCodes getVertexPos(I vertexID, cupcfd::geometry::euclidean::EuclideanPoint<T,3>& pos);
					inline cupcfd::geometry::euclidean::EuclideanPoint<T,3> getVertexPos(I vertexID);
					__attribute__((warn_unused_result))
					inline cupcfd::error::eCodes setVertexPos(I vertexID, cupcfd::geometry::euclidean::EuclideanPoint<T,3>& pos);
					cupcfd::error::eCodes addVertex(L vertexLabel, cupcfd::geometry::euclidean::EuclideanPoint<T,3>& pos);
					__attribute__((warn_unused_result))
					inline cupcfd::error::eCodes getVertexID(L vertexLabel, I * vertexID);
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
#include "CupCfdSoAMesh.ipp"

#endif
