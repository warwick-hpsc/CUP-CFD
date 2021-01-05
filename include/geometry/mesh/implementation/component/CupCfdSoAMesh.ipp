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
 * Contains header level definitions for the CupCfdSoAMesh Class
 */

#ifndef CUPCFD_GEOMETRY_MESH_CUPCFD_SOA_MESH_IPP_H
#define CUPCFD_GEOMETRY_MESH_CUPCFD_SOA_MESH_IPP_H

namespace euc = cupcfd::geometry::euclidean;

namespace cupcfd
{
	namespace geometry
	{
		namespace mesh
		{
			template <class I, class T, class L>
			inline void CupCfdSoAMesh<I,T,L>::getCellCenter(I cellID, euc::EuclideanPoint<T,3>& center) {
				center = DBG_SAFE_VECTOR_LOOKUP(this->cellCenter, cellID);
			}
			
			template <class I, class T, class L>
			inline euc::EuclideanPoint<T,3> CupCfdSoAMesh<I,T,L>::getCellCenter(I cellID) {
				return DBG_SAFE_VECTOR_LOOKUP(this->cellCenter, cellID);
			}
			
			
			
			template <class I, class T, class L>
			inline void CupCfdSoAMesh<I,T,L>::getCellVolume(I cellID, T * vol) {
				*vol = DBG_SAFE_VECTOR_LOOKUP(this->cellVolume, cellID);
			}
			
			template <class I, class T, class L>
			inline T CupCfdSoAMesh<I,T,L>::getCellVolume(I cellID) {
				return DBG_SAFE_VECTOR_LOOKUP(this->cellVolume, cellID);
			}
			
			
			
			template <class I, class T, class L>
			inline void CupCfdSoAMesh<I,T,L>::getCellNFaces(I cellID, I * nFaces) {
				*nFaces = DBG_SAFE_VECTOR_LOOKUP(this->cellNGFaces, cellID);
			}

			template <class I, class T, class L>
			inline I CupCfdSoAMesh<I,T,L>::getCellNFaces(I cellID) {
				return DBG_SAFE_VECTOR_LOOKUP(this->cellNGFaces, cellID);
			}



			template <class I, class T, class L>
			inline void CupCfdSoAMesh<I,T,L>::getCellStoredNFaces(I cellID, I * nFaces) {
				*nFaces = DBG_SAFE_VECTOR_LOOKUP(this->cellNFaces, cellID);
			}

			template <class I, class T, class L>
			inline I CupCfdSoAMesh<I,T,L>::getCellStoredNFaces(I cellID) {
				return DBG_SAFE_VECTOR_LOOKUP(this->cellNFaces, cellID);
			}

			template <class I, class T, class L>
			inline void CupCfdSoAMesh<I,T,L>::getCellNVertices(I cellID, int * nVertices) {
				*nVertices = DBG_SAFE_VECTOR_LOOKUP(this->cellNGVertices, cellID);
			}

			template <class I, class T, class L>
			int CupCfdSoAMesh<I,T,L>::getCellNVertices(I cellID) {
				return DBG_SAFE_VECTOR_LOOKUP(this->cellNGVertices, cellID);
			}

			template <class I, class T, class L>
			inline void CupCfdSoAMesh<I,T,L>::getCellStoredNVertices(I cellID, I * nVertices) {
				*nVertices = DBG_SAFE_VECTOR_LOOKUP(this->cellNVertices, cellID);
			}

			template <class I, class T, class L>
			I CupCfdSoAMesh<I,T,L>::getCellStoredNVertices(I cellID) {
				return DBG_SAFE_VECTOR_LOOKUP(this->cellNVertices, cellID);
			}

			template <class I, class T, class L>
			inline void CupCfdSoAMesh<I,T,L>::getCellFaceID(I cellID, I cellFaceID, I * faceID) {
				I index = DBG_SAFE_VECTOR_LOOKUP(this->cellFaceMapCSRXAdj, cellID) + cellFaceID;
				*faceID = DBG_SAFE_VECTOR_LOOKUP(this->cellFaceMapCSRAdj, index);
			}

			template <class I, class T, class L>
			inline I CupCfdSoAMesh<I,T,L>::getCellFaceID(I cellID, I cellFaceID) {
				I index = DBG_SAFE_VECTOR_LOOKUP(this->cellFaceMapCSRXAdj, cellID) + cellFaceID;
				return DBG_SAFE_VECTOR_LOOKUP(this->cellFaceMapCSRAdj, index);
			}



			template <class I, class T, class L>
			inline void CupCfdSoAMesh<I,T,L>::setCellCenter(I cellID, euc::EuclideanPoint<T,3>& center) {
				DBG_SAFE_VECTOR_LOOKUP(this->cellCenter, cellID) = center;
			}

			template <class I, class T, class L>
			inline void CupCfdSoAMesh<I,T,L>::setCellVolume(I cellID, T vol) {
				DBG_SAFE_VECTOR_LOOKUP(this->cellVolume, cellID) = vol;
			}
			
			
			template <class I, class T, class L>
			void CupCfdSoAMesh<I,T,L>::getCellID(L cellLabel, I * cellID) {
				*cellID = DBG_SAFE_VECTOR_LOOKUP(this->cellBuildIDToLocalID, cellLabel);
			}
			
			template <class I, class T, class L>
			I CupCfdSoAMesh<I,T,L>::getCellID(L cellLabel) {
				return DBG_SAFE_VECTOR_LOOKUP(this->cellBuildIDToLocalID, cellLabel);
			}
			
			template <class I, class T, class L>
			inline void CupCfdSoAMesh<I,T,L>::getFaceNVertices(I faceID, I * nVertices) {
				*nVertices = this->getFaceNVertices(faceID);
			}
			
			template <class I, class T, class L>
			inline I CupCfdSoAMesh<I,T,L>::getFaceNVertices(I faceID) {
				// Currently we can only have three or four vertices max per face
				if(DBG_SAFE_VECTOR_LOOKUP(this->faceVertexID, faceID)[3] == -1) {
					return 3;
				} else {
					return 4;
				}
			}
			
			template <class I, class T, class L>
			inline void CupCfdSoAMesh<I,T,L>::getFaceCell1ID(I faceID, I * cellID) {
				*cellID = DBG_SAFE_VECTOR_LOOKUP(this->faceCell1ID, faceID);
			}

			template <class I, class T, class L>
			inline I CupCfdSoAMesh<I,T,L>::getFaceCell1ID(I faceID) {
				return DBG_SAFE_VECTOR_LOOKUP(this->faceCell1ID, faceID);
			}

			template <class I, class T, class L>
			inline void CupCfdSoAMesh<I,T,L>::getFaceCell2ID(I faceID, I * cellID) {
				*cellID = DBG_SAFE_VECTOR_LOOKUP(this->faceCell2ID, faceID);
			}

			template <class I, class T, class L>
			inline I CupCfdSoAMesh<I,T,L>::getFaceCell2ID(I faceID) {
				return DBG_SAFE_VECTOR_LOOKUP(this->faceCell2ID, faceID);
			}

			template <class I, class T, class L>
			inline void CupCfdSoAMesh<I,T,L>::getFaceLambda(I faceID, T * lambda) {
				*lambda = DBG_SAFE_VECTOR_LOOKUP(this->faceLambda, faceID);
			}

			template <class I, class T, class L>
			inline T CupCfdSoAMesh<I,T,L>::getFaceLambda(I faceID) {
				return DBG_SAFE_VECTOR_LOOKUP(this->faceLambda, faceID);
			}

			template <class I, class T, class L>
			inline void CupCfdSoAMesh<I,T,L>::getFaceNorm(I faceID, euc::EuclideanVector<T,3>& norm) {
				norm = DBG_SAFE_VECTOR_LOOKUP(this->faceNorm, faceID);
			}

			template <class I, class T, class L>
			inline euc::EuclideanVector<T,3> CupCfdSoAMesh<I,T,L>::getFaceNorm(I faceID) {
				return DBG_SAFE_VECTOR_LOOKUP(this->faceNorm, faceID);
			}

			template <class I, class T, class L>
			inline void CupCfdSoAMesh<I,T,L>::getFaceVertex(I faceID, I faceVertexID, I * vertexID) {
				*vertexID = DBG_SAFE_VECTOR_LOOKUP(this->faceVertexID, faceID)[faceVertexID];
			}

			template <class I, class T, class L>
			inline I CupCfdSoAMesh<I,T,L>::getFaceVertex(I faceID, I faceVertexID) {
				return DBG_SAFE_VECTOR_LOOKUP(this->faceVertexID, faceID)[faceVertexID];
			}

			template <class I, class T, class L>
			inline void CupCfdSoAMesh<I,T,L>::getFaceCenter(I faceID, euc::EuclideanPoint<T,3>& center) {
				center = DBG_SAFE_VECTOR_LOOKUP(this->faceCenter, faceID);
			}

			template <class I, class T, class L>
			inline euc::EuclideanPoint<T,3> CupCfdSoAMesh<I,T,L>::getFaceCenter(I faceID) {
				return DBG_SAFE_VECTOR_LOOKUP(this->faceCenter, faceID);
			}

			template <class I, class T, class L>
			inline void CupCfdSoAMesh<I,T,L>::getFaceRLencos(I faceID, T * rlencos) {
				*rlencos = DBG_SAFE_VECTOR_LOOKUP(this->faceRLencos, faceID);
			}

			template <class I, class T, class L>
			inline T CupCfdSoAMesh<I,T,L>::getFaceRLencos(I faceID) {
				return DBG_SAFE_VECTOR_LOOKUP(this->faceRLencos, faceID);
			}

			template <class I, class T, class L>
			inline void CupCfdSoAMesh<I,T,L>::getFaceArea(I faceID, T * area) {
				*area = DBG_SAFE_VECTOR_LOOKUP(this->faceArea, faceID);
			}

			template <class I, class T, class L>
			inline T CupCfdSoAMesh<I,T,L>::getFaceArea(I faceID) {
				return DBG_SAFE_VECTOR_LOOKUP(this->faceArea, faceID);
			}

			template <class I, class T, class L>
			inline void CupCfdSoAMesh<I,T,L>::getFaceXpac(I faceID, euc::EuclideanPoint<T,3>& xpac) {
				xpac = DBG_SAFE_VECTOR_LOOKUP(this->faceXpac, faceID);
			}

			template <class I, class T, class L>
			inline euc::EuclideanPoint<T,3> CupCfdSoAMesh<I,T,L>::getFaceXpac(I faceID) {
				return DBG_SAFE_VECTOR_LOOKUP(this->faceXpac, faceID);
			}

			template <class I, class T, class L>
			inline void CupCfdSoAMesh<I,T,L>::getFaceXnac(I faceID, euc::EuclideanPoint<T,3>& xnac) {
				xnac = DBG_SAFE_VECTOR_LOOKUP(this->faceXnac, faceID);
			}

			template <class I, class T, class L>
			inline euc::EuclideanPoint<T,3> CupCfdSoAMesh<I,T,L>::getFaceXnac(I faceID) {
				return DBG_SAFE_VECTOR_LOOKUP(this->faceXnac, faceID);
			}

			template <class I, class T, class L>
			inline void CupCfdSoAMesh<I,T,L>::getFaceIsBoundary(I faceID, bool * result) {
				if(DBG_SAFE_VECTOR_LOOKUP(this->faceBoundaryID, faceID) > -1) {
					*result = true;
				} else {
					*result = false;
				}
			}

			template <class I, class T, class L>
			inline bool CupCfdSoAMesh<I,T,L>::getFaceIsBoundary(I faceID) {
				if(DBG_SAFE_VECTOR_LOOKUP(this->faceBoundaryID, faceID) > -1) {
					return true;
				}
				return false;
			}

			template <class I, class T, class L>
			inline void CupCfdSoAMesh<I,T,L>::getFaceBoundaryID(I faceID, I * boundaryID) {
				*boundaryID = DBG_SAFE_VECTOR_LOOKUP(this->faceBoundaryID, faceID);
			}

			template <class I, class T, class L>
			inline I CupCfdSoAMesh<I,T,L>::getFaceBoundaryID(I faceID) {
				return DBG_SAFE_VECTOR_LOOKUP(this->faceBoundaryID, faceID);
			}

			template <class I, class T, class L>
			inline void CupCfdSoAMesh<I,T,L>::setFaceCell1ID(I faceID, I cellID) {
				DBG_SAFE_VECTOR_LOOKUP(this->faceCell1ID, faceID) = cellID;
			}

			template <class I, class T, class L>
			inline void CupCfdSoAMesh<I,T,L>::setFaceCell2ID(I faceID, I cellID) {
				DBG_SAFE_VECTOR_LOOKUP(this->faceCell2ID, faceID) = cellID;
			}

			template <class I, class T, class L>
			inline void CupCfdSoAMesh<I,T,L>::setFaceLambda(I faceID, T lambda) {
				DBG_SAFE_VECTOR_LOOKUP(this->faceLambda, faceID) = lambda;
			}

			template <class I, class T, class L>
			inline void CupCfdSoAMesh<I,T,L>::setFaceNorm(I faceID, euc::EuclideanVector<T,3>& norm) {
				DBG_SAFE_VECTOR_LOOKUP(this->faceNorm, faceID) = norm;
			}

			template <class I, class T, class L>
			inline void CupCfdSoAMesh<I,T,L>::setFaceVertex(I faceID, I faceVertexID, I vertexID) {
				DBG_SAFE_VECTOR_LOOKUP(this->faceVertexID, faceID)[faceVertexID] = vertexID;
			}

			template <class I, class T, class L>
			inline void CupCfdSoAMesh<I,T,L>::setFaceCenter(I faceID, euc::EuclideanPoint<T,3>& center) {
				DBG_SAFE_VECTOR_LOOKUP(this->faceCenter, faceID) = center;
			}

			template <class I, class T, class L>
			inline void CupCfdSoAMesh<I,T,L>::setFaceRLencos(I faceID, T rlencos) {
				DBG_SAFE_VECTOR_LOOKUP(this->faceRLencos, faceID) = rlencos;
			}

			template <class I, class T, class L>
			inline void CupCfdSoAMesh<I,T,L>::setFaceArea(I faceID, T area) {
				DBG_SAFE_VECTOR_LOOKUP(this->faceArea, faceID) = area;
			}

			template <class I, class T, class L>
			inline void CupCfdSoAMesh<I,T,L>::setFaceXpac(I faceID, euc::EuclideanPoint<T,3>& xpac) {
				DBG_SAFE_VECTOR_LOOKUP(this->faceXpac, faceID) = xpac;
			}

			template <class I, class T, class L>
			inline void CupCfdSoAMesh<I,T,L>::setFaceXnac(I faceID, euc::EuclideanPoint<T,3>& xnac) {
				DBG_SAFE_VECTOR_LOOKUP(this->faceXnac, faceID) = xnac;
			}

			template <class I, class T, class L>
			inline void CupCfdSoAMesh<I,T,L>::setFaceBoundaryID(I faceID, I boundaryID) {
				DBG_SAFE_VECTOR_LOOKUP(this->faceBoundaryID, faceID) = boundaryID;
			}
			
			template <class I, class T, class L>
			void CupCfdSoAMesh<I,T,L>::getFaceID(L faceLabel, I * faceID) {
				*faceID = DBG_SAFE_VECTOR_LOOKUP(this->faceBuildIDToLocalID, faceLabel);
			}

			template <class I, class T, class L>
			I CupCfdSoAMesh<I,T,L>::getFaceID(L faceLabel) {
				return DBG_SAFE_VECTOR_LOOKUP(this->faceBuildIDToLocalID, faceLabel);
			}
			
			template <class I, class T, class L>
			inline void CupCfdSoAMesh<I,T,L>::getBoundaryFaceID(I boundaryID, I * faceID) {
				*faceID = DBG_SAFE_VECTOR_LOOKUP(this->boundaryFaceID, boundaryID);
			}

			template <class I, class T, class L>
			inline I CupCfdSoAMesh<I,T,L>::getBoundaryFaceID(I boundaryID) {
				return DBG_SAFE_VECTOR_LOOKUP(this->boundaryFaceID, boundaryID);
			}

			template <class I, class T, class L>
			inline void CupCfdSoAMesh<I,T,L>::getBoundaryVertex(I boundaryID, I boundaryVertexID, I * vertexID) {
				*vertexID = DBG_SAFE_VECTOR_LOOKUP(this->boundaryVertexID, boundaryID)[boundaryVertexID];
			}

			template <class I, class T, class L>
			inline I CupCfdSoAMesh<I,T,L>::getBoundaryVertex(I boundaryID, I boundaryVertexID) {
				return DBG_SAFE_VECTOR_LOOKUP(this->boundaryVertexID, boundaryID)[boundaryVertexID];
			}

			template <class I, class T, class L>
			inline void CupCfdSoAMesh<I,T,L>::getBoundaryDistance(I boundaryID, T * distance) {
				*distance = DBG_SAFE_VECTOR_LOOKUP(this->boundaryDistance, boundaryID);
			}

			template <class I, class T, class L>
			inline T CupCfdSoAMesh<I,T,L>::getBoundaryDistance(I boundaryID) {
				return DBG_SAFE_VECTOR_LOOKUP(this->boundaryDistance, boundaryID);
			}

			template <class I, class T, class L>
			inline void CupCfdSoAMesh<I,T,L>::getBoundaryYPlus(I boundaryID, T * yPlus) {
				*yPlus = DBG_SAFE_VECTOR_LOOKUP(this->boundaryYPlus, boundaryID);
			}

			template <class I, class T, class L>
			inline T CupCfdSoAMesh<I,T,L>::getBoundaryYPlus(I boundaryID) {
				return DBG_SAFE_VECTOR_LOOKUP(this->boundaryYPlus, boundaryID);
			}

			template <class I, class T, class L>
			inline void CupCfdSoAMesh<I,T,L>::getBoundaryUPlus(I boundaryID, T * uPlus) {
				*uPlus = DBG_SAFE_VECTOR_LOOKUP(this->boundaryUPlus, boundaryID);
			}

			template <class I, class T, class L>
			inline T CupCfdSoAMesh<I,T,L>::getBoundaryUPlus(I boundaryID) {
				return DBG_SAFE_VECTOR_LOOKUP(this->boundaryUPlus, boundaryID);
			}

			template <class I, class T, class L>
			inline void CupCfdSoAMesh<I,T,L>::getBoundaryRegionID(I boundaryID, I * regionID) {
				*regionID = DBG_SAFE_VECTOR_LOOKUP(this->boundaryRegionID, boundaryID);
			}

			template <class I, class T, class L>
			inline I CupCfdSoAMesh<I,T,L>::getBoundaryRegionID(I boundaryID) {
				return DBG_SAFE_VECTOR_LOOKUP(this->boundaryRegionID, boundaryID);
			}

			template <class I, class T, class L>
			inline void CupCfdSoAMesh<I,T,L>::getBoundaryShear(I boundaryID, euc::EuclideanVector<T,3>& shear) {
				shear = DBG_SAFE_VECTOR_LOOKUP(this->boundaryShear, boundaryID);
			}

			template <class I, class T, class L>
			inline euc::EuclideanVector<T,3> CupCfdSoAMesh<I,T,L>::getBoundaryShear(I boundaryID) {
				return DBG_SAFE_VECTOR_LOOKUP(this->boundaryShear, boundaryID);
			}

			template <class I, class T, class L>
			inline void CupCfdSoAMesh<I,T,L>::getBoundaryQ(I boundaryID, T * q) {
				*q = DBG_SAFE_VECTOR_LOOKUP(this->boundaryQ, boundaryID);
			}

			template <class I, class T, class L>
			inline T CupCfdSoAMesh<I,T,L>::getBoundaryQ(I boundaryID) {
				return DBG_SAFE_VECTOR_LOOKUP(this->boundaryQ, boundaryID);
			}

			template <class I, class T, class L>
			inline void CupCfdSoAMesh<I,T,L>::getBoundaryH(I boundaryID, T * h) {
				*h = DBG_SAFE_VECTOR_LOOKUP(this->boundaryH, boundaryID);
			}

			template <class I, class T, class L>
			inline T CupCfdSoAMesh<I,T,L>::getBoundaryH(I boundaryID) {
				return DBG_SAFE_VECTOR_LOOKUP(this->boundaryH, boundaryID);
			}

			template <class I, class T, class L>
			inline void CupCfdSoAMesh<I,T,L>::getBoundaryT(I boundaryID, T * t) {
				*t = DBG_SAFE_VECTOR_LOOKUP(this->boundaryT, boundaryID);
			}

			template <class I, class T, class L>
			inline T CupCfdSoAMesh<I,T,L>::getBoundaryT(I boundaryID) {
				return DBG_SAFE_VECTOR_LOOKUP(this->boundaryT, boundaryID);
			}

			template <class I, class T, class L>
			inline void CupCfdSoAMesh<I,T,L>::setBoundaryFaceID(I boundaryID, I faceID) {
				DBG_SAFE_VECTOR_LOOKUP(this->boundaryFaceID, boundaryID) = faceID;
			}

			template <class I, class T, class L>
			inline void CupCfdSoAMesh<I,T,L>::setBoundaryVertex(I boundaryID, I boundaryVertexID, I vertexID) {
				DBG_SAFE_VECTOR_LOOKUP(this->boundaryVertexID, boundaryID)[boundaryVertexID] = vertexID;
			}

			template <class I, class T, class L>
			inline void CupCfdSoAMesh<I,T,L>::setBoundaryDistance(I boundaryID, T distance) {
				DBG_SAFE_VECTOR_LOOKUP(this->boundaryDistance, boundaryID) = distance;
			}

			template <class I, class T, class L>
			inline void CupCfdSoAMesh<I,T,L>::setBoundaryYPlus(I boundaryID, T yPlus) {
				DBG_SAFE_VECTOR_LOOKUP(this->boundaryYPlus, boundaryID) = yPlus;
			}

			template <class I, class T, class L>
			inline void CupCfdSoAMesh<I,T,L>::setBoundaryUPlus(I boundaryID, T uPlus) {
				DBG_SAFE_VECTOR_LOOKUP(this->boundaryUPlus, boundaryID) = uPlus;
			}

			template <class I, class T, class L>
			inline void CupCfdSoAMesh<I,T,L>::setBoundaryRegionID(I boundaryID, I regionID) {
				DBG_SAFE_VECTOR_LOOKUP(this->boundaryRegionID, boundaryID) = regionID;
			}

			template <class I, class T, class L>
			inline void CupCfdSoAMesh<I,T,L>::setBoundaryShear(I boundaryID, euc::EuclideanVector<T,3>& shear) {
				DBG_SAFE_VECTOR_LOOKUP(this->boundaryShear, boundaryID) = shear;
			}

			template <class I, class T, class L>
			inline void CupCfdSoAMesh<I,T,L>::setBoundaryQ(I boundaryID, T q) {
				DBG_SAFE_VECTOR_LOOKUP(this->boundaryQ, boundaryID) = q;
			}

			template <class I, class T, class L>
			inline void CupCfdSoAMesh<I,T,L>::setBoundaryH(I boundaryID, T h) {
				DBG_SAFE_VECTOR_LOOKUP(this->boundaryH, boundaryID) = h;
			}

			template <class I, class T, class L>
			inline void CupCfdSoAMesh<I,T,L>::setBoundaryT(I boundaryID, T t) {
				DBG_SAFE_VECTOR_LOOKUP(this->boundaryT, boundaryID) = t;
			}
			
			template <class I, class T, class L>
			void CupCfdSoAMesh<I,T,L>::getBoundaryID(L boundaryLabel, I * boundaryID) {
				*boundaryID = DBG_SAFE_VECTOR_LOOKUP(this->boundaryBuildIDToLocalID, boundaryLabel);
			}

			template <class I, class T, class L>
			I CupCfdSoAMesh<I,T,L>::getBoundaryID(L boundaryLabel) {
				return DBG_SAFE_VECTOR_LOOKUP(this->boundaryBuildIDToLocalID, boundaryLabel);
			}
			
			template <class I, class T, class L>			
			inline void CupCfdSoAMesh<I,T,L>::getRegionType(I regionID, RType * type) {
				*type = DBG_SAFE_VECTOR_LOOKUP(this->regionType, regionID);
			}

			template <class I, class T, class L>
			inline RType CupCfdSoAMesh<I,T,L>::getRegionType(I regionID) {
				return DBG_SAFE_VECTOR_LOOKUP(this->regionType, regionID);
			}

			template <class I, class T, class L>
			inline void CupCfdSoAMesh<I,T,L>::getRegionStd(I regionID, bool * std) {
				*std = DBG_SAFE_VECTOR_LOOKUP(this->regionStd, regionID);
			}

			template <class I, class T, class L>
			inline bool CupCfdSoAMesh<I,T,L>::getRegionStd(I regionID) {
				return DBG_SAFE_VECTOR_LOOKUP(this->regionStd, regionID);
			}

			template <class I, class T, class L>
			inline void CupCfdSoAMesh<I,T,L>::getRegionYLog(I regionID, T * yLog) {
				*yLog = DBG_SAFE_VECTOR_LOOKUP(this->regionYLog, regionID);
			}

			template <class I, class T, class L>
			inline T CupCfdSoAMesh<I,T,L>::getRegionYLog(I regionID) {
				return DBG_SAFE_VECTOR_LOOKUP(this->regionYLog, regionID);
			}

			template <class I, class T, class L>
			inline void CupCfdSoAMesh<I,T,L>::getRegionELog(I regionID, T * eLog) {
				*eLog = DBG_SAFE_VECTOR_LOOKUP(this->regionELog, regionID);
			}

			template <class I, class T, class L>
			inline T CupCfdSoAMesh<I,T,L>::getRegionELog(I regionID) {
				return DBG_SAFE_VECTOR_LOOKUP(this->regionELog, regionID);
			}

			template <class I, class T, class L>
			inline void CupCfdSoAMesh<I,T,L>::getRegionDensity(I regionID, T * density) {
				*density = DBG_SAFE_VECTOR_LOOKUP(this->regionDensity, regionID);
			}

			template <class I, class T, class L>
			inline T CupCfdSoAMesh<I,T,L>::getRegionDensity(I regionID) {
				return DBG_SAFE_VECTOR_LOOKUP(this->regionDensity, regionID);
			}

			template <class I, class T, class L>
			inline void CupCfdSoAMesh<I,T,L>::getRegionTurbKE(I regionID, T * turbKE) {
				*turbKE = DBG_SAFE_VECTOR_LOOKUP(this->regionTurbKE, regionID);
			}

			template <class I, class T, class L>
			inline T CupCfdSoAMesh<I,T,L>::getRegionTurbKE(I regionID) {
				return DBG_SAFE_VECTOR_LOOKUP(this->regionTurbKE, regionID);
			}

			template <class I, class T, class L>
			inline void CupCfdSoAMesh<I,T,L>::getRegionTurbDiss(I regionID, T * turbDiss) {
				*turbDiss = DBG_SAFE_VECTOR_LOOKUP(this->regionTurbDiss, regionID);
			}

			template <class I, class T, class L>
			inline T CupCfdSoAMesh<I,T,L>::getRegionTurbDiss(I regionID) {
				return DBG_SAFE_VECTOR_LOOKUP(this->regionTurbDiss, regionID);
			}

			template <class I, class T, class L>
			inline void CupCfdSoAMesh<I,T,L>::getRegionSplvl(I regionID, T * splvl) {
				*splvl = DBG_SAFE_VECTOR_LOOKUP(this->regionSplvl, regionID);
			}

			template <class I, class T, class L>
			inline T CupCfdSoAMesh<I,T,L>::getRegionSplvl(I regionID) {
				return DBG_SAFE_VECTOR_LOOKUP(this->regionSplvl, regionID);
			}

			template <class I, class T, class L>
			inline void CupCfdSoAMesh<I,T,L>::getRegionDen(I regionID, T * den) {
				*den = DBG_SAFE_VECTOR_LOOKUP(this->regionDen, regionID);
			}

			template <class I, class T, class L>
			inline T CupCfdSoAMesh<I,T,L>::getRegionDen(I regionID) {
				return DBG_SAFE_VECTOR_LOOKUP(this->regionDen, regionID);
			}

			template <class I, class T, class L>
			inline void CupCfdSoAMesh<I,T,L>::getRegionForceTangent(I regionID, euc::EuclideanVector<T,3>& forceTangent) {
				forceTangent = DBG_SAFE_VECTOR_LOOKUP(this->regionForceTangent, regionID);
			}

			template <class I, class T, class L>
			inline euc::EuclideanVector<T,3> CupCfdSoAMesh<I,T,L>::getRegionForceTangent(I regionID) {
				return DBG_SAFE_VECTOR_LOOKUP(this->regionForceTangent, regionID);
			}

			template <class I, class T, class L>
			inline void CupCfdSoAMesh<I,T,L>::getRegionUVW(I regionID, euc::EuclideanVector<T,3>& uvw) {
				uvw = DBG_SAFE_VECTOR_LOOKUP(this->regionUVW, regionID);
			}

			template <class I, class T, class L>
			inline euc::EuclideanVector<T,3> CupCfdSoAMesh<I,T,L>::getRegionUVW(I regionID) {
				return DBG_SAFE_VECTOR_LOOKUP(this->regionUVW, regionID);
			}

			template <class I, class T, class L>
			inline void CupCfdSoAMesh<I,T,L>::getRegionName(I regionID, std::string& regionName) {
				regionName = DBG_SAFE_VECTOR_LOOKUP(this->regionName, regionID);
			}

			template <class I, class T, class L>
			inline std::string CupCfdSoAMesh<I,T,L>::getRegionName(I regionID) {
				return DBG_SAFE_VECTOR_LOOKUP(this->regionName, regionID);
			}

			template <class I, class T, class L>
			inline void CupCfdSoAMesh<I,T,L>::getRegionFlux(I regionID, bool * flux) {
				*flux = DBG_SAFE_VECTOR_LOOKUP(this->regionFlux, regionID);
			}

			template <class I, class T, class L>
			inline bool CupCfdSoAMesh<I,T,L>::getRegionFlux(I regionID) {
				return DBG_SAFE_VECTOR_LOOKUP(this->regionFlux, regionID);
			}

			template <class I, class T, class L>
			inline void CupCfdSoAMesh<I,T,L>::getRegionAdiab(I regionID, bool * adiab) {
				*adiab = DBG_SAFE_VECTOR_LOOKUP(this->regionAdiab, regionID);
			}

			template <class I, class T, class L>
			inline bool CupCfdSoAMesh<I,T,L>::getRegionAdiab(I regionID) {
				return DBG_SAFE_VECTOR_LOOKUP(this->regionAdiab, regionID);
			}

			template <class I, class T, class L>
			inline void CupCfdSoAMesh<I,T,L>::getRegionR(I regionID, T * r) {
				*r = DBG_SAFE_VECTOR_LOOKUP(this->regionR, regionID);
			}

			template <class I, class T, class L>
			inline T CupCfdSoAMesh<I,T,L>::getRegionR(I regionID) {
				return DBG_SAFE_VECTOR_LOOKUP(this->regionR, regionID);
			}

			template <class I, class T, class L>
			inline void CupCfdSoAMesh<I,T,L>::getRegionT(I regionID, T * t) {
				*t = DBG_SAFE_VECTOR_LOOKUP(this->regionT, regionID);
			}

			template <class I, class T, class L>
			inline T CupCfdSoAMesh<I,T,L>::getRegionT(I regionID) {
				return DBG_SAFE_VECTOR_LOOKUP(this->regionT, regionID);
			}

			template <class I, class T, class L>
			inline void CupCfdSoAMesh<I,T,L>::setRegionType(I regionID, RType type) {
				DBG_SAFE_VECTOR_LOOKUP(this->regionType, regionID) = type;
			}

			template <class I, class T, class L>
			inline void CupCfdSoAMesh<I,T,L>::setRegionStd(I regionID, bool std) {
				DBG_SAFE_VECTOR_LOOKUP(this->regionStd, regionID) = std;
			}

			template <class I, class T, class L>
			inline void CupCfdSoAMesh<I,T,L>::setRegionYLog(I regionID, T yLog) {
				DBG_SAFE_VECTOR_LOOKUP(this->regionYLog, regionID) = yLog;
			}

			template <class I, class T, class L>
			inline void CupCfdSoAMesh<I,T,L>::setRegionELog(I regionID, T eLog) {
				DBG_SAFE_VECTOR_LOOKUP(this->regionELog, regionID) = eLog;
			}

			template <class I, class T, class L>
			inline void CupCfdSoAMesh<I,T,L>::setRegionDensity(I regionID, T density) {
				DBG_SAFE_VECTOR_LOOKUP(this->regionDensity, regionID) = density;
			}

			template <class I, class T, class L>
			inline void CupCfdSoAMesh<I,T,L>::setRegionTurbKE(I regionID, T turbKE) {
				DBG_SAFE_VECTOR_LOOKUP(this->regionTurbKE, regionID) = turbKE;
			}

			template <class I, class T, class L>
			inline void CupCfdSoAMesh<I,T,L>::setRegionTurbDiss(I regionID, T turbDiss) {
				DBG_SAFE_VECTOR_LOOKUP(this->regionTurbDiss, regionID) = turbDiss;
			}

			template <class I, class T, class L>
			inline void CupCfdSoAMesh<I,T,L>::setRegionSplvl(I regionID, T splvl) {
				DBG_SAFE_VECTOR_LOOKUP(this->regionSplvl, regionID) = splvl;
			}

			template <class I, class T, class L>
			inline void CupCfdSoAMesh<I,T,L>::setRegionDen(I regionID, T den) {
				DBG_SAFE_VECTOR_LOOKUP(this->regionDen, regionID) = den;
			}

			template <class I, class T, class L>
			inline void CupCfdSoAMesh<I,T,L>::setRegionForceTangent(I regionID, euc::EuclideanVector<T,3>& forceTangent) {
				DBG_SAFE_VECTOR_LOOKUP(this->regionForceTangent, regionID) = forceTangent;
			}

			template <class I, class T, class L>
			inline void CupCfdSoAMesh<I,T,L>::setRegionUVW(I regionID, euc::EuclideanVector<T,3>& uvw) {
				DBG_SAFE_VECTOR_LOOKUP(this->regionUVW, regionID) = uvw;
			}

			template <class I, class T, class L>
			inline void CupCfdSoAMesh<I,T,L>::setRegionName(I regionID, std::string& regionName) {
				DBG_SAFE_VECTOR_LOOKUP(this->regionName, regionID) = regionName;
			}

			template <class I, class T, class L>
			inline void CupCfdSoAMesh<I,T,L>::setRegionFlux(I regionID, bool flux) {
				DBG_SAFE_VECTOR_LOOKUP(this->regionFlux, regionID) = flux;
			}

			template <class I, class T, class L>
			inline void CupCfdSoAMesh<I,T,L>::setRegionAdiab(I regionID, bool adiab) {
				DBG_SAFE_VECTOR_LOOKUP(this->regionAdiab, regionID) = adiab;
			}

			template <class I, class T, class L>
			inline void CupCfdSoAMesh<I,T,L>::setRegionR(I regionID, T r) {
				DBG_SAFE_VECTOR_LOOKUP(this->regionR, regionID) = r;
			}

			template <class I, class T, class L>
			inline void CupCfdSoAMesh<I,T,L>::setRegionT(I regionID, T t) {
				DBG_SAFE_VECTOR_LOOKUP(this->regionT, regionID) = t;
			}
			
			template <class I, class T, class L>
			void CupCfdSoAMesh<I,T,L>::getRegionID(L regionLabel, I * regionID) {
				*regionID = DBG_SAFE_VECTOR_LOOKUP(this->regionBuildIDToLocalID, regionLabel);;
			}

			template <class I, class T, class L>
			I CupCfdSoAMesh<I,T,L>::getRegionID(L regionLabel) {
				return DBG_SAFE_VECTOR_LOOKUP(this->regionBuildIDToLocalID, regionLabel);
			}
			
			template <class I, class T, class L>
			inline void CupCfdSoAMesh<I,T,L>::getVertexPos(I vertexID, euc::EuclideanPoint<T,3>& pos) {
				pos = DBG_SAFE_VECTOR_LOOKUP(this->verticesPos, vertexID);
			}

			template <class I, class T, class L>
			inline euc::EuclideanPoint<T,3> CupCfdSoAMesh<I,T,L>::getVertexPos(I vertexID) {
				return DBG_SAFE_VECTOR_LOOKUP(this->verticesPos, vertexID);
			}

			template <class I, class T, class L>
			inline void CupCfdSoAMesh<I,T,L>::setVertexPos(I vertexID, euc::EuclideanPoint<T,3>& pos) {
				DBG_SAFE_VECTOR_LOOKUP(this->verticesPos, vertexID) = pos;
			}

			template <class I, class T, class L>
			inline void CupCfdSoAMesh<I,T,L>::getVertexID(L vertexLabel, I * vertexID) {
				*vertexID = DBG_SAFE_VECTOR_LOOKUP(this->vertexBuildIDToLocalID, vertexLabel);
			}

			template <class I, class T, class L>
			inline I CupCfdSoAMesh<I,T,L>::getVertexID(L vertexLabel) {
				return DBG_SAFE_VECTOR_LOOKUP(this->vertexBuildIDToLocalID, vertexLabel);
			}					
		}
	}
}

#endif
