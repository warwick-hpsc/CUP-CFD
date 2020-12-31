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
 * Contains header level definitions for the CupCfdAoSMesh Class
 */

#ifndef CUPCFD_GEOMETRY_MESH_CUPCFD_AOS_MESH_IPP_H
#define CUPCFD_GEOMETRY_MESH_CUPCFD_AOS_MESH_IPP_H

namespace cupcfd
{
	namespace geometry
	{
		namespace mesh
		{
			template <class I, class T, class L>
			inline void CupCfdAoSMesh<I,T,L>::getCellCenter(I cellID, cupcfd::geometry::euclidean::EuclideanPoint<T,3>& center) {
				center = DBG_SAFE_VECTOR_LOOKUP(this->cells, cellID).center;
			}
			
			template <class I, class T, class L>
			inline cupcfd::geometry::euclidean::EuclideanPoint<T,3> CupCfdAoSMesh<I,T,L>::getCellCenter(I cellID) {
				return DBG_SAFE_VECTOR_LOOKUP(this->cells, cellID).center;
			}
			
			template <class I, class T, class L>
			inline void CupCfdAoSMesh<I,T,L>::getCellVolume(I cellID, T * vol) {
				*vol = DBG_SAFE_VECTOR_LOOKUP(this->cells, cellID).vol;
			}
			
			template <class I, class T, class L>
			inline T CupCfdAoSMesh<I,T,L>::getCellVolume(I cellID) {
				return DBG_SAFE_VECTOR_LOOKUP(this->cells, cellID).vol;
			}
			
			template <class I, class T, class L>
			inline void CupCfdAoSMesh<I,T,L>::getCellNFaces(I cellID, I * nFaces) {
				*nFaces = DBG_SAFE_VECTOR_LOOKUP(this->cellNGFaces, cellID);
			}

			template <class I, class T, class L>
			inline I CupCfdAoSMesh<I,T,L>::getCellNFaces(I cellID) {
				return DBG_SAFE_VECTOR_LOOKUP(this->cellNGFaces, cellID);
			}

			template <class I, class T, class L>
			inline void CupCfdAoSMesh<I,T,L>::getCellStoredNFaces(I cellID, I * nFaces) {
				*nFaces = DBG_SAFE_VECTOR_LOOKUP(this->cellNFaces, cellID);
			}

			template <class I, class T, class L>
			inline I CupCfdAoSMesh<I,T,L>::getCellStoredNFaces(I cellID) {
				return DBG_SAFE_VECTOR_LOOKUP(this->cellNFaces, cellID);
			}

			template <class I, class T, class L>
			inline void CupCfdAoSMesh<I,T,L>::getCellNVertices(I cellID, int * nVertices) {
				*nVertices = DBG_SAFE_VECTOR_LOOKUP(this->cellNGVertices, cellID);
			}

			template <class I, class T, class L>
			int CupCfdAoSMesh<I,T,L>::getCellNVertices(I cellID) {
				return DBG_SAFE_VECTOR_LOOKUP(this->cellNGVertices, cellID);
			}

			template <class I, class T, class L>
			inline void CupCfdAoSMesh<I,T,L>::getCellStoredNVertices(I cellID, I * nVertices) {
				*nVertices = DBG_SAFE_VECTOR_LOOKUP(cellNVertices, cellID);
			}

			template <class I, class T, class L>
			I CupCfdAoSMesh<I,T,L>::getCellStoredNVertices(I cellID) {
				return DBG_SAFE_VECTOR_LOOKUP(cellNVertices, cellID);
			}

			template <class I, class T, class L>
			inline void CupCfdAoSMesh<I,T,L>::getCellFaceID(I cellID, I cellFaceID, I * faceID) {
				// I index = this->cellFaceMapCSRXAdj[cellID] + cellFaceID;
				I index = DBG_SAFE_VECTOR_LOOKUP(this->cellFaceMapCSRXAdj, cellID) + cellFaceID;
				// *faceID = this->cellFaceMapCSRAdj[index];
				*faceID = DBG_SAFE_VECTOR_LOOKUP(this->cellFaceMapCSRAdj, index);
			}

			template <class I, class T, class L>
			inline I CupCfdAoSMesh<I,T,L>::getCellFaceID(I cellID, I cellFaceID) {
				I index = DBG_SAFE_VECTOR_LOOKUP(this->cellFaceMapCSRXAdj, cellID) + cellFaceID;
				return DBG_SAFE_VECTOR_LOOKUP(this->cellFaceMapCSRAdj, index);
			}

			template <class I, class T, class L>
			inline void CupCfdAoSMesh<I,T,L>::setCellCenter(I cellID, cupcfd::geometry::euclidean::EuclideanPoint<T,3>& center) {
				DBG_SAFE_VECTOR_LOOKUP(this->cells, cellID).center = center;
			}

			template <class I, class T, class L>
			inline void CupCfdAoSMesh<I,T,L>::setCellVolume(I cellID, T vol) {
				DBG_SAFE_VECTOR_LOOKUP(this->cells, cellID).vol = vol;
			}
			
			template <class I, class T, class L>
			void CupCfdAoSMesh<I,T,L>::getCellID(L cellLabel, I * cellID) {
				*cellID = DBG_SAFE_VECTOR_LOOKUP(this->cellBuildIDToLocalID, cellLabel);
			}
			
			template <class I, class T, class L>
			I CupCfdAoSMesh<I,T,L>::getCellID(L cellLabel) {
				return DBG_SAFE_VECTOR_LOOKUP(this->cellBuildIDToLocalID, cellLabel);
			}
			
			template <class I, class T, class L>
			inline void CupCfdAoSMesh<I,T,L>::getFaceNVertices(I faceID, I * nVertices) {
				*nVertices = this->getFaceNVertices(faceID);
			}
			
			template <class I, class T, class L>
			inline I CupCfdAoSMesh<I,T,L>::getFaceNVertices(I faceID) {
				// Currently we can only have three or four vertices max per face
				if(DBG_SAFE_VECTOR_LOOKUP(this->faces, faceID).verticesID[3] == -1) {
					return 3;
				} else {
					return 4;
				}
			}
			
			template <class I, class T, class L>
			inline void CupCfdAoSMesh<I,T,L>::getFaceCell1ID(I faceID, I * cellID) {
				*cellID = DBG_SAFE_VECTOR_LOOKUP(this->faces, faceID).cell1ID;
			}

			template <class I, class T, class L>
			inline I CupCfdAoSMesh<I,T,L>::getFaceCell1ID(I faceID) {
				return DBG_SAFE_VECTOR_LOOKUP(this->faces, faceID).cell1ID;
			}

			template <class I, class T, class L>
			inline void CupCfdAoSMesh<I,T,L>::getFaceCell2ID(I faceID, I * cellID) {
				*cellID = DBG_SAFE_VECTOR_LOOKUP(this->faces, faceID).cell2ID;
			}

			template <class I, class T, class L>
			inline I CupCfdAoSMesh<I,T,L>::getFaceCell2ID(I faceID) {
				return DBG_SAFE_VECTOR_LOOKUP(this->faces, faceID).cell2ID;
			}

			template <class I, class T, class L>
			inline void CupCfdAoSMesh<I,T,L>::getFaceLambda(I faceID, T * lambda) {
				*lambda = DBG_SAFE_VECTOR_LOOKUP(this->faces, faceID).lambda;
			}

			template <class I, class T, class L>
			inline T CupCfdAoSMesh<I,T,L>::getFaceLambda(I faceID) {
				return DBG_SAFE_VECTOR_LOOKUP(this->faces, faceID).lambda;
			}

			template <class I, class T, class L>
			inline void CupCfdAoSMesh<I,T,L>::getFaceNorm(I faceID, cupcfd::geometry::euclidean::EuclideanVector<T,3>& norm) {
				norm = DBG_SAFE_VECTOR_LOOKUP(this->faces, faceID).norm;
			}

			template <class I, class T, class L>
			inline cupcfd::geometry::euclidean::EuclideanVector<T,3> CupCfdAoSMesh<I,T,L>::getFaceNorm(I faceID) {
				return DBG_SAFE_VECTOR_LOOKUP(this->faces, faceID).norm;
			}

			template <class I, class T, class L>
			inline void CupCfdAoSMesh<I,T,L>::getFaceVertex(I faceID, I faceVertexID, I * vertexID) {
				*vertexID = DBG_SAFE_VECTOR_LOOKUP(this->faces, faceID).verticesID[faceVertexID];
			}

			template <class I, class T, class L>
			inline I CupCfdAoSMesh<I,T,L>::getFaceVertex(I faceID, I faceVertexID) {
				return DBG_SAFE_VECTOR_LOOKUP(this->faces, faceID).verticesID[faceVertexID];
			}

			template <class I, class T, class L>
			inline void CupCfdAoSMesh<I,T,L>::getFaceCenter(I faceID, cupcfd::geometry::euclidean::EuclideanPoint<T,3>& center) {
				center = DBG_SAFE_VECTOR_LOOKUP(this->faces, faceID).center;
			}

			template <class I, class T, class L>
			inline cupcfd::geometry::euclidean::EuclideanPoint<T,3> CupCfdAoSMesh<I,T,L>::getFaceCenter(I faceID) {
				return DBG_SAFE_VECTOR_LOOKUP(this->faces, faceID).center;
			}

			template <class I, class T, class L>
			inline void CupCfdAoSMesh<I,T,L>::getFaceRLencos(I faceID, T * rlencos) {
				*rlencos = DBG_SAFE_VECTOR_LOOKUP(this->faces, faceID).rlencos;
			}

			template <class I, class T, class L>
			inline T CupCfdAoSMesh<I,T,L>::getFaceRLencos(I faceID) {
				return DBG_SAFE_VECTOR_LOOKUP(this->faces, faceID).rlencos;
			}

			template <class I, class T, class L>
			inline void CupCfdAoSMesh<I,T,L>::getFaceArea(I faceID, T * area) {
				*area = DBG_SAFE_VECTOR_LOOKUP(this->faces, faceID).area;
			}

			template <class I, class T, class L>
			inline T CupCfdAoSMesh<I,T,L>::getFaceArea(I faceID) {
				return DBG_SAFE_VECTOR_LOOKUP(this->faces, faceID).area;
			}

			template <class I, class T, class L>
			inline void CupCfdAoSMesh<I,T,L>::getFaceXpac(I faceID, cupcfd::geometry::euclidean::EuclideanPoint<T,3>& xpac) {
				xpac = DBG_SAFE_VECTOR_LOOKUP(this->faces, faceID).xpac;
			}

			template <class I, class T, class L>
			inline cupcfd::geometry::euclidean::EuclideanPoint<T,3> CupCfdAoSMesh<I,T,L>::getFaceXpac(I faceID) {
				return DBG_SAFE_VECTOR_LOOKUP(this->faces, faceID).xpac;
			}

			template <class I, class T, class L>
			inline void CupCfdAoSMesh<I,T,L>::getFaceXnac(I faceID, cupcfd::geometry::euclidean::EuclideanPoint<T,3>& xnac) {
				xnac = DBG_SAFE_VECTOR_LOOKUP(this->faces, faceID).xnac;
			}

			template <class I, class T, class L>
			inline cupcfd::geometry::euclidean::EuclideanPoint<T,3> CupCfdAoSMesh<I,T,L>::getFaceXnac(I faceID) {
				return DBG_SAFE_VECTOR_LOOKUP(this->faces, faceID).xnac;
			}

			template <class I, class T, class L>
			inline void CupCfdAoSMesh<I,T,L>::getFaceIsBoundary(I faceID, bool * result) {
				if(DBG_SAFE_VECTOR_LOOKUP(this->faces, faceID).bndID > -1) {
					*result = true;
				} else {
					*result = false;
				}
			}

			template <class I, class T, class L>
			inline bool CupCfdAoSMesh<I,T,L>::getFaceIsBoundary(I faceID) {
				if(DBG_SAFE_VECTOR_LOOKUP(this->faces, faceID).bndID > -1) {
					return true;
				}
				return false;
			}

			template <class I, class T, class L>
			inline void CupCfdAoSMesh<I,T,L>::getFaceBoundaryID(I faceID, I * boundaryID) {
				*boundaryID = DBG_SAFE_VECTOR_LOOKUP(this->faces, faceID).bndID;
			}

			template <class I, class T, class L>
			inline I CupCfdAoSMesh<I,T,L>::getFaceBoundaryID(I faceID) {
				return DBG_SAFE_VECTOR_LOOKUP(this->faces, faceID).bndID;
			}

			template <class I, class T, class L>
			inline void CupCfdAoSMesh<I,T,L>::setFaceCell1ID(I faceID, I cellID) {
				DBG_SAFE_VECTOR_LOOKUP(this->faces, faceID).cell1ID = cellID;
			}

			template <class I, class T, class L>
			inline void CupCfdAoSMesh<I,T,L>::setFaceCell2ID(I faceID, I cellID) {
				DBG_SAFE_VECTOR_LOOKUP(this->faces, faceID).cell2ID = cellID;
			}

			template <class I, class T, class L>
			inline void CupCfdAoSMesh<I,T,L>::setFaceLambda(I faceID, T lambda) {
				DBG_SAFE_VECTOR_LOOKUP(this->faces, faceID).lambda = lambda;
			}

			template <class I, class T, class L>
			inline void CupCfdAoSMesh<I,T,L>::setFaceNorm(I faceID, cupcfd::geometry::euclidean::EuclideanVector<T,3>& norm) {
				DBG_SAFE_VECTOR_LOOKUP(this->faces, faceID).norm = norm;
			}

			template <class I, class T, class L>
			inline void CupCfdAoSMesh<I,T,L>::setFaceVertex(I faceID, I faceVertexID, I vertexID) {
				DBG_SAFE_VECTOR_LOOKUP(this->faces, faceID).verticesID[faceVertexID] = vertexID;
			}

			template <class I, class T, class L>
			inline void CupCfdAoSMesh<I,T,L>::setFaceCenter(I faceID, cupcfd::geometry::euclidean::EuclideanPoint<T,3>& center) {
				DBG_SAFE_VECTOR_LOOKUP(this->faces, faceID).center = center;
			}

			template <class I, class T, class L>
			inline void CupCfdAoSMesh<I,T,L>::setFaceRLencos(I faceID, T rlencos) {
				DBG_SAFE_VECTOR_LOOKUP(this->faces, faceID).rlencos = rlencos;
			}

			template <class I, class T, class L>
			inline void CupCfdAoSMesh<I,T,L>::setFaceArea(I faceID, T area) {
				DBG_SAFE_VECTOR_LOOKUP(this->faces, faceID).area = area;
			}

			template <class I, class T, class L>
			inline void CupCfdAoSMesh<I,T,L>::setFaceXpac(I faceID, cupcfd::geometry::euclidean::EuclideanPoint<T,3>& xpac) {
				DBG_SAFE_VECTOR_LOOKUP(this->faces, faceID).xpac = xpac;
			}

			template <class I, class T, class L>
			inline void CupCfdAoSMesh<I,T,L>::setFaceXnac(I faceID, cupcfd::geometry::euclidean::EuclideanPoint<T,3>& xnac) {
				DBG_SAFE_VECTOR_LOOKUP(this->faces, faceID).xnac = xnac;
			}

			template <class I, class T, class L>
			inline void CupCfdAoSMesh<I,T,L>::setFaceBoundaryID(I faceID, I boundaryID) {
				DBG_SAFE_VECTOR_LOOKUP(this->faces, faceID).bndID = boundaryID;
			}
			
			template <class I, class T, class L>
			void CupCfdAoSMesh<I,T,L>::getFaceID(L faceLabel, I * faceID) {
				*faceID = DBG_SAFE_VECTOR_LOOKUP(this->faceBuildIDToLocalID, faceLabel);
			}

			template <class I, class T, class L>
			I CupCfdAoSMesh<I,T,L>::getFaceID(L faceLabel) {
				return DBG_SAFE_VECTOR_LOOKUP(this->faceBuildIDToLocalID, faceLabel);
			}
			
			template <class I, class T, class L>
			inline void CupCfdAoSMesh<I,T,L>::getBoundaryFaceID(I boundaryID, I * faceID) {
				*faceID = DBG_SAFE_VECTOR_LOOKUP(this->boundaries, boundaryID).faceID;
			}

			template <class I, class T, class L>
			inline I CupCfdAoSMesh<I,T,L>::getBoundaryFaceID(I boundaryID) {
				return DBG_SAFE_VECTOR_LOOKUP(this->boundaries, boundaryID).faceID;
			}

			template <class I, class T, class L>
			inline void CupCfdAoSMesh<I,T,L>::getBoundaryVertex(I boundaryID, I boundaryVertexID, I * vertexID) {
				*vertexID = DBG_SAFE_VECTOR_LOOKUP(this->boundaries, boundaryID).verticesID[boundaryVertexID];
			}

			template <class I, class T, class L>
			inline I CupCfdAoSMesh<I,T,L>::getBoundaryVertex(I boundaryID, I boundaryVertexID) {
				return DBG_SAFE_VECTOR_LOOKUP(this->boundaries, boundaryID).verticesID[boundaryVertexID];
			}

			template <class I, class T, class L>
			inline void CupCfdAoSMesh<I,T,L>::getBoundaryDistance(I boundaryID, T * distance) {
				*distance = DBG_SAFE_VECTOR_LOOKUP(this->boundaries, boundaryID).distance;
			}

			template <class I, class T, class L>
			inline T CupCfdAoSMesh<I,T,L>::getBoundaryDistance(I boundaryID) {
				return DBG_SAFE_VECTOR_LOOKUP(this->boundaries, boundaryID).distance;
			}

			template <class I, class T, class L>
			inline void CupCfdAoSMesh<I,T,L>::getBoundaryYPlus(I boundaryID, T * yPlus) {
				*yPlus = DBG_SAFE_VECTOR_LOOKUP(this->boundaries, boundaryID).yplus;
			}

			template <class I, class T, class L>
			inline T CupCfdAoSMesh<I,T,L>::getBoundaryYPlus(I boundaryID) {
				return DBG_SAFE_VECTOR_LOOKUP(this->boundaries, boundaryID).yplus;
			}

			template <class I, class T, class L>
			inline void CupCfdAoSMesh<I,T,L>::getBoundaryUPlus(I boundaryID, T * uPlus) {
				*uPlus = DBG_SAFE_VECTOR_LOOKUP(this->boundaries, boundaryID).uplus;
			}

			template <class I, class T, class L>
			inline T CupCfdAoSMesh<I,T,L>::getBoundaryUPlus(I boundaryID) {
				return DBG_SAFE_VECTOR_LOOKUP(this->boundaries, boundaryID).uplus;
			}

			template <class I, class T, class L>
			inline void CupCfdAoSMesh<I,T,L>::getBoundaryRegionID(I boundaryID, I * regionID) {
				*regionID = DBG_SAFE_VECTOR_LOOKUP(this->boundaries, boundaryID).regionID;
			}

			template <class I, class T, class L>
			inline I CupCfdAoSMesh<I,T,L>::getBoundaryRegionID(I boundaryID) {
				return DBG_SAFE_VECTOR_LOOKUP(this->boundaries, boundaryID).regionID;
			}

			template <class I, class T, class L>
			inline void CupCfdAoSMesh<I,T,L>::getBoundaryShear(I boundaryID, cupcfd::geometry::euclidean::EuclideanVector<T,3>& shear) {
				shear = DBG_SAFE_VECTOR_LOOKUP(this->boundaries, boundaryID).shear;
			}

			template <class I, class T, class L>
			inline cupcfd::geometry::euclidean::EuclideanVector<T,3> CupCfdAoSMesh<I,T,L>::getBoundaryShear(I boundaryID) {
				return DBG_SAFE_VECTOR_LOOKUP(this->boundaries, boundaryID).shear;
			}

			template <class I, class T, class L>
			inline void CupCfdAoSMesh<I,T,L>::getBoundaryQ(I boundaryID, T * q) {
				*q = DBG_SAFE_VECTOR_LOOKUP(this->boundaries, boundaryID).q;
			}

			template <class I, class T, class L>
			inline T CupCfdAoSMesh<I,T,L>::getBoundaryQ(I boundaryID) {
				return DBG_SAFE_VECTOR_LOOKUP(this->boundaries, boundaryID).q;
			}

			template <class I, class T, class L>
			inline void CupCfdAoSMesh<I,T,L>::getBoundaryH(I boundaryID, T * h) {
				*h = DBG_SAFE_VECTOR_LOOKUP(this->boundaries, boundaryID).h;
			}

			template <class I, class T, class L>
			inline T CupCfdAoSMesh<I,T,L>::getBoundaryH(I boundaryID) {
				return DBG_SAFE_VECTOR_LOOKUP(this->boundaries, boundaryID).h;
			}

			template <class I, class T, class L>
			inline void CupCfdAoSMesh<I,T,L>::getBoundaryT(I boundaryID, T * t) {
				*t = DBG_SAFE_VECTOR_LOOKUP(this->boundaries, boundaryID).t;
			}

			template <class I, class T, class L>
			inline T CupCfdAoSMesh<I,T,L>::getBoundaryT(I boundaryID) {
				return DBG_SAFE_VECTOR_LOOKUP(this->boundaries, boundaryID).t;
			}

			template <class I, class T, class L>
			inline void CupCfdAoSMesh<I,T,L>::setBoundaryFaceID(I boundaryID, I faceID) {
				DBG_SAFE_VECTOR_LOOKUP(this->boundaries, boundaryID).faceID = faceID;
			}

			template <class I, class T, class L>
			inline void CupCfdAoSMesh<I,T,L>::setBoundaryVertex(I boundaryID, I boundaryVertexID, I vertexID) {
				DBG_SAFE_VECTOR_LOOKUP(this->boundaries, boundaryID).verticesID[boundaryVertexID] = vertexID;
			}

			template <class I, class T, class L>
			inline void CupCfdAoSMesh<I,T,L>::setBoundaryDistance(I boundaryID, T distance) {
				DBG_SAFE_VECTOR_LOOKUP(this->boundaries, boundaryID).distance = distance;
			}

			template <class I, class T, class L>
			inline void CupCfdAoSMesh<I,T,L>::setBoundaryYPlus(I boundaryID, T yPlus) {
				DBG_SAFE_VECTOR_LOOKUP(this->boundaries, boundaryID).yplus = yPlus;
			}

			template <class I, class T, class L>
			inline void CupCfdAoSMesh<I,T,L>::setBoundaryUPlus(I boundaryID, T uPlus) {
				DBG_SAFE_VECTOR_LOOKUP(this->boundaries, boundaryID).uplus = uPlus;
			}

			template <class I, class T, class L>
			inline void CupCfdAoSMesh<I,T,L>::setBoundaryRegionID(I boundaryID, I regionID) {
				DBG_SAFE_VECTOR_LOOKUP(this->boundaries, boundaryID).regionID = regionID;
			}

			template <class I, class T, class L>
			inline void CupCfdAoSMesh<I,T,L>::setBoundaryShear(I boundaryID, cupcfd::geometry::euclidean::EuclideanVector<T,3>& shear) {
				DBG_SAFE_VECTOR_LOOKUP(this->boundaries, boundaryID).shear = shear;
			}

			template <class I, class T, class L>
			inline void CupCfdAoSMesh<I,T,L>::setBoundaryQ(I boundaryID, T q) {
				DBG_SAFE_VECTOR_LOOKUP(this->boundaries, boundaryID).q = q;
			}

			template <class I, class T, class L>
			inline void CupCfdAoSMesh<I,T,L>::setBoundaryH(I boundaryID, T h) {
				DBG_SAFE_VECTOR_LOOKUP(this->boundaries, boundaryID).h = h;
			}

			template <class I, class T, class L>
			inline void CupCfdAoSMesh<I,T,L>::setBoundaryT(I boundaryID, T t) {
				DBG_SAFE_VECTOR_LOOKUP(this->boundaries, boundaryID).t = t;
			}
			
			template <class I, class T, class L>
			void CupCfdAoSMesh<I,T,L>::getBoundaryID(L boundaryLabel, I * boundaryID) {
				*boundaryID = DBG_SAFE_VECTOR_LOOKUP(this->boundaryBuildIDToLocalID, boundaryLabel);
			}

			template <class I, class T, class L>
			I CupCfdAoSMesh<I,T,L>::getBoundaryID(L boundaryLabel) {
				return DBG_SAFE_VECTOR_LOOKUP(this->boundaryBuildIDToLocalID, boundaryLabel);
			}
			
			template <class I, class T, class L>			
			inline void CupCfdAoSMesh<I,T,L>::getRegionType(I regionID, RType * type) {
				*type = DBG_SAFE_VECTOR_LOOKUP(this->regions, regionID).rType;
			}

			template <class I, class T, class L>
			inline RType CupCfdAoSMesh<I,T,L>::getRegionType(I regionID) {
				return DBG_SAFE_VECTOR_LOOKUP(this->regions, regionID).rType;
			}

			template <class I, class T, class L>
			inline void CupCfdAoSMesh<I,T,L>::getRegionStd(I regionID, bool * std) {
				*std = DBG_SAFE_VECTOR_LOOKUP(this->regions, regionID).std;
			}

			template <class I, class T, class L>
			inline bool CupCfdAoSMesh<I,T,L>::getRegionStd(I regionID) {
				return DBG_SAFE_VECTOR_LOOKUP(this->regions, regionID).std;
			}

			template <class I, class T, class L>
			inline void CupCfdAoSMesh<I,T,L>::getRegionYLog(I regionID, T * yLog) {
				*yLog = DBG_SAFE_VECTOR_LOOKUP(this->regions, regionID).ylog;
			}

			template <class I, class T, class L>
			inline T CupCfdAoSMesh<I,T,L>::getRegionYLog(I regionID) {
				return DBG_SAFE_VECTOR_LOOKUP(this->regions, regionID).ylog;
			}

			template <class I, class T, class L>
			inline void CupCfdAoSMesh<I,T,L>::getRegionELog(I regionID, T * eLog) {
				*eLog = DBG_SAFE_VECTOR_LOOKUP(this->regions, regionID).elog;
			}

			template <class I, class T, class L>
			inline T CupCfdAoSMesh<I,T,L>::getRegionELog(I regionID) {
				return DBG_SAFE_VECTOR_LOOKUP(this->regions, regionID).elog;
			}

			template <class I, class T, class L>
			inline void CupCfdAoSMesh<I,T,L>::getRegionDensity(I regionID, T * density) {
				*density = DBG_SAFE_VECTOR_LOOKUP(this->regions, regionID).density;
			}

			template <class I, class T, class L>
			inline T CupCfdAoSMesh<I,T,L>::getRegionDensity(I regionID) {
				return DBG_SAFE_VECTOR_LOOKUP(this->regions, regionID).density;
			}

			template <class I, class T, class L>
			inline void CupCfdAoSMesh<I,T,L>::getRegionTurbKE(I regionID, T * turbKE) {
				*turbKE = DBG_SAFE_VECTOR_LOOKUP(this->regions, regionID).turbKE;
			}

			template <class I, class T, class L>
			inline T CupCfdAoSMesh<I,T,L>::getRegionTurbKE(I regionID) {
				return DBG_SAFE_VECTOR_LOOKUP(this->regions, regionID).turbKE;
			}

			template <class I, class T, class L>
			inline void CupCfdAoSMesh<I,T,L>::getRegionTurbDiss(I regionID, T * turbDiss) {
				*turbDiss = DBG_SAFE_VECTOR_LOOKUP(this->regions, regionID).turbDiss;
			}

			template <class I, class T, class L>
			inline T CupCfdAoSMesh<I,T,L>::getRegionTurbDiss(I regionID) {
				return DBG_SAFE_VECTOR_LOOKUP(this->regions, regionID).turbDiss;
			}

			template <class I, class T, class L>
			inline void CupCfdAoSMesh<I,T,L>::getRegionSplvl(I regionID, T * splvl) {
				*splvl = DBG_SAFE_VECTOR_LOOKUP(this->regions, regionID).splvl;
			}

			template <class I, class T, class L>
			inline T CupCfdAoSMesh<I,T,L>::getRegionSplvl(I regionID) {
				return DBG_SAFE_VECTOR_LOOKUP(this->regions, regionID).splvl;
			}

			template <class I, class T, class L>
			inline void CupCfdAoSMesh<I,T,L>::getRegionDen(I regionID, T * den) {
				*den = DBG_SAFE_VECTOR_LOOKUP(this->regions, regionID).den;
			}

			template <class I, class T, class L>
			inline T CupCfdAoSMesh<I,T,L>::getRegionDen(I regionID) {
				return DBG_SAFE_VECTOR_LOOKUP(this->regions, regionID).den;
			}

			template <class I, class T, class L>
			inline void CupCfdAoSMesh<I,T,L>::getRegionForceTangent(I regionID, cupcfd::geometry::euclidean::EuclideanVector<T,3>& forceTangent) {
				forceTangent = DBG_SAFE_VECTOR_LOOKUP(this->regions, regionID).forceTangent;
			}

			template <class I, class T, class L>
			inline cupcfd::geometry::euclidean::EuclideanVector<T,3> CupCfdAoSMesh<I,T,L>::getRegionForceTangent(I regionID) {
				return DBG_SAFE_VECTOR_LOOKUP(this->regions, regionID).forceTangent;
			}

			template <class I, class T, class L>
			inline void CupCfdAoSMesh<I,T,L>::getRegionUVW(I regionID, cupcfd::geometry::euclidean::EuclideanVector<T,3>& uvw) {
				uvw = DBG_SAFE_VECTOR_LOOKUP(this->regions, regionID).uvw;
			}

			template <class I, class T, class L>
			inline cupcfd::geometry::euclidean::EuclideanVector<T,3> CupCfdAoSMesh<I,T,L>::getRegionUVW(I regionID) {
				return DBG_SAFE_VECTOR_LOOKUP(this->regions, regionID).uvw;
			}

			template <class I, class T, class L>
			inline void CupCfdAoSMesh<I,T,L>::getRegionName(I regionID, std::string& regionName) {
				regionName = DBG_SAFE_VECTOR_LOOKUP(this->regions, regionID).regionName;
			}

			template <class I, class T, class L>
			inline std::string CupCfdAoSMesh<I,T,L>::getRegionName(I regionID) {
				return DBG_SAFE_VECTOR_LOOKUP(this->regions, regionID).regionName;
			}

			template <class I, class T, class L>
			inline void CupCfdAoSMesh<I,T,L>::getRegionFlux(I regionID, bool * flux) {
				*flux = DBG_SAFE_VECTOR_LOOKUP(this->regions, regionID).flux;
			}

			template <class I, class T, class L>
			inline bool CupCfdAoSMesh<I,T,L>::getRegionFlux(I regionID) {
				return DBG_SAFE_VECTOR_LOOKUP(this->regions, regionID).flux;
			}

			template <class I, class T, class L>
			inline void CupCfdAoSMesh<I,T,L>::getRegionAdiab(I regionID, bool * adiab) {
				*adiab = DBG_SAFE_VECTOR_LOOKUP(this->regions, regionID).adiab;
			}

			template <class I, class T, class L>
			inline bool CupCfdAoSMesh<I,T,L>::getRegionAdiab(I regionID) {
				return DBG_SAFE_VECTOR_LOOKUP(this->regions, regionID).adiab;
			}

			template <class I, class T, class L>
			inline void CupCfdAoSMesh<I,T,L>::getRegionR(I regionID, T * r) {
				*r = DBG_SAFE_VECTOR_LOOKUP(this->regions, regionID).r;
			}

			template <class I, class T, class L>
			inline T CupCfdAoSMesh<I,T,L>::getRegionR(I regionID) {
				return DBG_SAFE_VECTOR_LOOKUP(this->regions, regionID).r;
			}

			template <class I, class T, class L>
			inline void CupCfdAoSMesh<I,T,L>::getRegionT(I regionID, T * t) {
				*t = DBG_SAFE_VECTOR_LOOKUP(this->regions, regionID).t;
			}

			template <class I, class T, class L>
			inline T CupCfdAoSMesh<I,T,L>::getRegionT(I regionID) {
				return DBG_SAFE_VECTOR_LOOKUP(this->regions, regionID).t;
			}

			template <class I, class T, class L>
			inline void CupCfdAoSMesh<I,T,L>::setRegionType(I regionID, RType type) {
				DBG_SAFE_VECTOR_LOOKUP(this->regions, regionID).rType = type;
			}

			template <class I, class T, class L>
			inline void CupCfdAoSMesh<I,T,L>::setRegionStd(I regionID, bool std) {
				DBG_SAFE_VECTOR_LOOKUP(this->regions, regionID).std = std;
			}

			template <class I, class T, class L>
			inline void CupCfdAoSMesh<I,T,L>::setRegionYLog(I regionID, T yLog) {
				DBG_SAFE_VECTOR_LOOKUP(this->regions, regionID).ylog = yLog;
			}

			template <class I, class T, class L>
			inline void CupCfdAoSMesh<I,T,L>::setRegionELog(I regionID, T eLog) {
				DBG_SAFE_VECTOR_LOOKUP(this->regions, regionID).elog = eLog;
			}

			template <class I, class T, class L>
			inline void CupCfdAoSMesh<I,T,L>::setRegionDensity(I regionID, T density) {
				DBG_SAFE_VECTOR_LOOKUP(this->regions, regionID).density = density;
			}

			template <class I, class T, class L>
			inline void CupCfdAoSMesh<I,T,L>::setRegionTurbKE(I regionID, T turbKE) {
				DBG_SAFE_VECTOR_LOOKUP(this->regions, regionID).turbKE = turbKE;
			}

			template <class I, class T, class L>
			inline void CupCfdAoSMesh<I,T,L>::setRegionTurbDiss(I regionID, T turbDiss) {
				DBG_SAFE_VECTOR_LOOKUP(this->regions, regionID).turbDiss = turbDiss;
			}

			template <class I, class T, class L>
			inline void CupCfdAoSMesh<I,T,L>::setRegionSplvl(I regionID, T splvl) {
				DBG_SAFE_VECTOR_LOOKUP(this->regions, regionID).splvl = splvl;
			}

			template <class I, class T, class L>
			inline void CupCfdAoSMesh<I,T,L>::setRegionDen(I regionID, T den) {
				DBG_SAFE_VECTOR_LOOKUP(this->regions, regionID).den = den;
			}

			template <class I, class T, class L>
			inline void CupCfdAoSMesh<I,T,L>::setRegionForceTangent(I regionID, cupcfd::geometry::euclidean::EuclideanVector<T,3>& forceTangent) {
				DBG_SAFE_VECTOR_LOOKUP(this->regions, regionID).forceTangent = forceTangent;
			}

			template <class I, class T, class L>
			inline void CupCfdAoSMesh<I,T,L>::setRegionUVW(I regionID, cupcfd::geometry::euclidean::EuclideanVector<T,3>& uvw) {
				DBG_SAFE_VECTOR_LOOKUP(this->regions, regionID).uvw = uvw;
			}

			template <class I, class T, class L>
			inline void CupCfdAoSMesh<I,T,L>::setRegionName(I regionID, std::string& regionName) {
				DBG_SAFE_VECTOR_LOOKUP(this->regions, regionID).regionName = regionName;
			}

			template <class I, class T, class L>
			inline void CupCfdAoSMesh<I,T,L>::setRegionFlux(I regionID, bool flux) {
				DBG_SAFE_VECTOR_LOOKUP(this->regions, regionID).flux = flux;
			}

			template <class I, class T, class L>
			inline void CupCfdAoSMesh<I,T,L>::setRegionAdiab(I regionID, bool adiab) {
				DBG_SAFE_VECTOR_LOOKUP(this->regions, regionID).adiab = adiab;
			}

			template <class I, class T, class L>
			inline void CupCfdAoSMesh<I,T,L>::setRegionR(I regionID, T r) {
				DBG_SAFE_VECTOR_LOOKUP(this->regions, regionID).r = r;
			}

			template <class I, class T, class L>
			inline void CupCfdAoSMesh<I,T,L>::setRegionT(I regionID, T t) {
				DBG_SAFE_VECTOR_LOOKUP(this->regions, regionID).t = t;
			}
			
			template <class I, class T, class L>
			void CupCfdAoSMesh<I,T,L>::getRegionID(L regionLabel, I * regionID) {
				*regionID = DBG_SAFE_VECTOR_LOOKUP(this->regionBuildIDToLocalID, regionLabel);;
			}

			template <class I, class T, class L>
			I CupCfdAoSMesh<I,T,L>::getRegionID(L regionLabel) {
				return DBG_SAFE_VECTOR_LOOKUP(this->regionBuildIDToLocalID, regionLabel);
			}
			
			template <class I, class T, class L>
			inline void CupCfdAoSMesh<I,T,L>::getVertexPos(I vertexID, cupcfd::geometry::euclidean::EuclideanPoint<T,3>& pos) {
				pos = DBG_SAFE_VECTOR_LOOKUP(this->vertices, vertexID).pos;
			}

			template <class I, class T, class L>
			inline cupcfd::geometry::euclidean::EuclideanPoint<T,3> CupCfdAoSMesh<I,T,L>::getVertexPos(I vertexID) {
				return DBG_SAFE_VECTOR_LOOKUP(this->vertices, vertexID).pos;
			}

			template <class I, class T, class L>
			inline void CupCfdAoSMesh<I,T,L>::setVertexPos(I vertexID, cupcfd::geometry::euclidean::EuclideanPoint<T,3>& pos) {
				DBG_SAFE_VECTOR_LOOKUP(this->vertices, vertexID).pos = pos;
			}

			template <class I, class T, class L>
			inline void CupCfdAoSMesh<I,T,L>::getVertexID(L vertexLabel, I * vertexID) {
				*vertexID = DBG_SAFE_VECTOR_LOOKUP(this->vertexBuildIDToLocalID, vertexLabel);
			}

			template <class I, class T, class L>
			inline I CupCfdAoSMesh<I,T,L>::getVertexID(L vertexLabel) {
				return DBG_SAFE_VECTOR_LOOKUP(this->vertexBuildIDToLocalID, vertexLabel);
			}					
		}
	}
}

#endif
