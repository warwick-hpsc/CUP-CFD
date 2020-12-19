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

namespace cupcfd
{
	namespace geometry
	{
		namespace mesh
		{
			template <class I, class T, class L>
			inline cupcfd::error::eCodes CupCfdSoAMesh<I,T,L>::getCellCenter(I cellID, cupcfd::geometry::euclidean::EuclideanPoint<T,3>& center)
			{
				center = this->cellCenter[cellID];
				return cupcfd::error::E_SUCCESS;
			}
			
			template <class I, class T, class L>
			inline cupcfd::geometry::euclidean::EuclideanPoint<T,3> CupCfdSoAMesh<I,T,L>::getCellCenter(I cellID)
			{
				return this->cellCenter[cellID];
			}
			
			
			
			template <class I, class T, class L>
			inline cupcfd::error::eCodes CupCfdSoAMesh<I,T,L>::getCellVolume(I cellID, T * vol)
			{
				*vol = this->cellVolume[cellID];
				return cupcfd::error::E_SUCCESS;
			}
			
			template <class I, class T, class L>
			inline T CupCfdSoAMesh<I,T,L>::getCellVolume(I cellID)
			{
				return this->cellVolume[cellID];
			}
			
			
			
			template <class I, class T, class L>
			inline cupcfd::error::eCodes CupCfdSoAMesh<I,T,L>::getCellNFaces(I cellID, I * nFaces)
			{
				*nFaces = this->cellNGFaces[cellID];
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			inline I CupCfdSoAMesh<I,T,L>::getCellNFaces(I cellID)
			{
				return this->cellNGFaces[cellID];
			}



			template <class I, class T, class L>
			inline cupcfd::error::eCodes CupCfdSoAMesh<I,T,L>::getCellStoredNFaces(I cellID, I * nFaces)
			{
				*nFaces = this->cellNFaces[cellID];
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			inline I CupCfdSoAMesh<I,T,L>::getCellStoredNFaces(I cellID)
			{
				return this->cellNFaces[cellID];
			}

			template <class I, class T, class L>
			inline cupcfd::error::eCodes CupCfdSoAMesh<I,T,L>::getCellNVertices(I cellID, int * nVertices)
			{
				if(this->finalized == false)
				{
					return cupcfd::error::E_UNFINALIZED;
				}

				*nVertices = this->cellNGVertices[cellID];

				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			int CupCfdSoAMesh<I,T,L>::getCellNVertices(I cellID)
			{
				return this->cellNGVertices[cellID];
			}

			template <class I, class T, class L>
			inline cupcfd::error::eCodes CupCfdSoAMesh<I,T,L>::getCellStoredNVertices(I cellID, I * nVertices)
			{
				*nVertices = cellNVertices[cellID];
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			I CupCfdSoAMesh<I,T,L>::getCellStoredNVertices(I cellID)
			{
				return cellNVertices[cellID];
			}

			template <class I, class T, class L>
			inline cupcfd::error::eCodes CupCfdSoAMesh<I,T,L>::getCellFaceID(I cellID, I cellFaceID, I * faceID)
			{
				I index = this->cellFaceMapCSRXAdj[cellID] + cellFaceID;
				*faceID = this->cellFaceMapCSRAdj[index];

				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			inline I CupCfdSoAMesh<I,T,L>::getCellFaceID(I cellID, I cellFaceID)
			{
				I index = this->cellFaceMapCSRXAdj[cellID] + cellFaceID;
				return this->cellFaceMapCSRAdj[index];
			}



			template <class I, class T, class L>
			inline cupcfd::error::eCodes CupCfdSoAMesh<I,T,L>::setCellCenter(I cellID, cupcfd::geometry::euclidean::EuclideanPoint<T,3>& center)
			{
				this->cellCenter[cellID] = center;
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			inline cupcfd::error::eCodes CupCfdSoAMesh<I,T,L>::setCellVolume(I cellID, T vol)
			{
				this->cellVolume[cellID] = vol;
				return cupcfd::error::E_SUCCESS;
			}
			
			
			template <class I, class T, class L>
			cupcfd::error::eCodes CupCfdSoAMesh<I,T,L>::getCellID(L cellLabel, I * cellID)
			{
				*cellID = this->cellBuildIDToLocalID[cellLabel];
				return cupcfd::error::E_SUCCESS;
			}
			
			template <class I, class T, class L>
			I CupCfdSoAMesh<I,T,L>::getCellID(L cellLabel)
			{
				return this->cellBuildIDToLocalID[cellLabel];
			}
			
			template <class I, class T, class L>
			inline cupcfd::error::eCodes CupCfdSoAMesh<I,T,L>::getFaceNVertices(I faceID, I * nVertices)
			{
				*nVertices = this->getFaceNVertices(faceID);
				return cupcfd::error::E_SUCCESS;
			}
			
			template <class I, class T, class L>
			inline I CupCfdSoAMesh<I,T,L>::getFaceNVertices(I faceID)
			{
				// Currently we can only have three or four vertices max per face
				if(this->faceVertexID[faceID][3] == -1)
				{
					return 3;
				}
				else
				{
					return 4;
				}
			}
			
			template <class I, class T, class L>
			inline cupcfd::error::eCodes CupCfdSoAMesh<I,T,L>::getFaceCell1ID(I faceID, I * cellID)
			{
				*cellID = this->faceCell1ID[faceID];
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			inline I CupCfdSoAMesh<I,T,L>::getFaceCell1ID(I faceID)
			{
				return this->faceCell1ID[faceID];
			}

			template <class I, class T, class L>
			inline cupcfd::error::eCodes CupCfdSoAMesh<I,T,L>::getFaceCell2ID(I faceID, I * cellID)
			{
				*cellID = this->faceCell2ID[faceID];
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			inline I CupCfdSoAMesh<I,T,L>::getFaceCell2ID(I faceID)
			{
				return this->faceCell2ID[faceID];
			}

			template <class I, class T, class L>
			inline cupcfd::error::eCodes CupCfdSoAMesh<I,T,L>::getFaceLambda(I faceID, T * lambda)
			{
				*lambda = this->faceLambda[faceID];
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			inline T CupCfdSoAMesh<I,T,L>::getFaceLambda(I faceID)
			{
				return this->faceLambda[faceID];
			}

			template <class I, class T, class L>
			inline cupcfd::error::eCodes CupCfdSoAMesh<I,T,L>::getFaceNorm(I faceID, cupcfd::geometry::euclidean::EuclideanVector<T,3>& norm)
			{
				norm = this->faceNorm[faceID];
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			inline cupcfd::geometry::euclidean::EuclideanVector<T,3> CupCfdSoAMesh<I,T,L>::getFaceNorm(I faceID)
			{
				return this->faceNorm[faceID];
			}

			template <class I, class T, class L>
			inline cupcfd::error::eCodes CupCfdSoAMesh<I,T,L>::getFaceVertex(I faceID, I faceVertexID, I * vertexID)
			{
				*vertexID = this->faceVertexID[faceID][faceVertexID];
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			inline I CupCfdSoAMesh<I,T,L>::getFaceVertex(I faceID, I faceVertexID)
			{
				return this->faceVertexID[faceID][faceVertexID];
			}

			template <class I, class T, class L>
			inline cupcfd::error::eCodes CupCfdSoAMesh<I,T,L>::getFaceCenter(I faceID, cupcfd::geometry::euclidean::EuclideanPoint<T,3>& center)
			{
				center = this->faceCenter[faceID];
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			inline cupcfd::geometry::euclidean::EuclideanPoint<T,3> CupCfdSoAMesh<I,T,L>::getFaceCenter(I faceID)
			{
				return this->faceCenter[faceID];
			}

			template <class I, class T, class L>
			inline cupcfd::error::eCodes CupCfdSoAMesh<I,T,L>::getFaceRLencos(I faceID, T * rlencos)
			{
				*rlencos = this->faceRLencos[faceID];
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			inline T CupCfdSoAMesh<I,T,L>::getFaceRLencos(I faceID)
			{
				return this->faceRLencos[faceID];
			}

			template <class I, class T, class L>
			inline cupcfd::error::eCodes CupCfdSoAMesh<I,T,L>::getFaceArea(I faceID, T * area)
			{
				*area = this->faceArea[faceID];
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			inline T CupCfdSoAMesh<I,T,L>::getFaceArea(I faceID)
			{
				return this->faceArea[faceID];
			}

			template <class I, class T, class L>
			inline cupcfd::error::eCodes CupCfdSoAMesh<I,T,L>::getFaceXpac(I faceID, cupcfd::geometry::euclidean::EuclideanPoint<T,3>& xpac)
			{
				xpac = this->faceXpac[faceID];
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			inline cupcfd::geometry::euclidean::EuclideanPoint<T,3> CupCfdSoAMesh<I,T,L>::getFaceXpac(I faceID)
			{
				return this->faceXpac[faceID];
			}

			template <class I, class T, class L>
			inline cupcfd::error::eCodes CupCfdSoAMesh<I,T,L>::getFaceXnac(I faceID, cupcfd::geometry::euclidean::EuclideanPoint<T,3>& xnac)
			{
				xnac = this->faceXnac[faceID];
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			inline cupcfd::geometry::euclidean::EuclideanPoint<T,3> CupCfdSoAMesh<I,T,L>::getFaceXnac(I faceID)
			{
				return this->faceXnac[faceID];
			}

			template <class I, class T, class L>
			inline cupcfd::error::eCodes CupCfdSoAMesh<I,T,L>::getFaceIsBoundary(I faceID, bool * result)
			{
				if(this->faceBoundaryID[faceID] > -1)
				{
					*result = true;
				}
				else
				{
					*result = false;
				}

				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			inline bool CupCfdSoAMesh<I,T,L>::getFaceIsBoundary(I faceID)
			{
				if(this->faceBoundaryID[faceID] > -1)
				{
					return true;
				}

				return false;
			}

			template <class I, class T, class L>
			inline cupcfd::error::eCodes CupCfdSoAMesh<I,T,L>::getFaceBoundaryID(I faceID, I * boundaryID)
			{
				*boundaryID = this->faceBoundaryID[faceID];
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			inline I CupCfdSoAMesh<I,T,L>::getFaceBoundaryID(I faceID)
			{
				return this->faceBoundaryID[faceID];
			}

			template <class I, class T, class L>
			inline cupcfd::error::eCodes CupCfdSoAMesh<I,T,L>::setFaceCell1ID(I faceID, I cellID)
			{
				this->faceCell1ID[faceID] = cellID;
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			inline cupcfd::error::eCodes CupCfdSoAMesh<I,T,L>::setFaceCell2ID(I faceID, I cellID)
			{
				this->faceCell2ID[faceID] = cellID;
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			inline cupcfd::error::eCodes CupCfdSoAMesh<I,T,L>::setFaceLambda(I faceID, T lambda)
			{
				this->faceLambda[faceID] = lambda;
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			inline cupcfd::error::eCodes CupCfdSoAMesh<I,T,L>::setFaceNorm(I faceID, cupcfd::geometry::euclidean::EuclideanVector<T,3>& norm)
			{
				this->faceNorm[faceID] = norm;
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			inline cupcfd::error::eCodes CupCfdSoAMesh<I,T,L>::setFaceVertex(I faceID, I faceVertexID, I vertexID)
			{
				this->faceVertexID[faceID][faceVertexID] = vertexID;
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			inline cupcfd::error::eCodes CupCfdSoAMesh<I,T,L>::setFaceCenter(I faceID, cupcfd::geometry::euclidean::EuclideanPoint<T,3>& center)
			{
				this->faceCenter[faceID] = center;
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			inline cupcfd::error::eCodes CupCfdSoAMesh<I,T,L>::setFaceRLencos(I faceID, T rlencos)
			{
				this->faceRLencos[faceID] = rlencos;
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			inline cupcfd::error::eCodes CupCfdSoAMesh<I,T,L>::setFaceArea(I faceID, T area)
			{
				this->faceArea[faceID] = area;
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			inline cupcfd::error::eCodes CupCfdSoAMesh<I,T,L>::setFaceXpac(I faceID, cupcfd::geometry::euclidean::EuclideanPoint<T,3>& xpac)
			{
				this->faceXpac[faceID] = xpac;
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			inline cupcfd::error::eCodes CupCfdSoAMesh<I,T,L>::setFaceXnac(I faceID, cupcfd::geometry::euclidean::EuclideanPoint<T,3>& xnac)
			{
				this->faceXnac[faceID] = xnac;
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			inline cupcfd::error::eCodes CupCfdSoAMesh<I,T,L>::setFaceBoundaryID(I faceID, I boundaryID)
			{
				this->faceBoundaryID[faceID] = boundaryID;
				return cupcfd::error::E_SUCCESS;
			}
			
			template <class I, class T, class L>
			cupcfd::error::eCodes CupCfdSoAMesh<I,T,L>::getFaceID(L faceLabel, I * faceID)
			{
				*faceID = this->faceBuildIDToLocalID[faceLabel];
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			I CupCfdSoAMesh<I,T,L>::getFaceID(L faceLabel)
			{
				return this->faceBuildIDToLocalID[faceLabel];
			}
			
			template <class I, class T, class L>
			inline cupcfd::error::eCodes CupCfdSoAMesh<I,T,L>::getBoundaryFaceID(I boundaryID, I * faceID)
			{
				*faceID = this->boundaryFaceID[boundaryID];
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			inline I CupCfdSoAMesh<I,T,L>::getBoundaryFaceID(I boundaryID)
			{
				return this->boundaryFaceID[boundaryID];
			}

			template <class I, class T, class L>
			inline cupcfd::error::eCodes CupCfdSoAMesh<I,T,L>::getBoundaryVertex(I boundaryID, I boundaryVertexID, I * vertexID)
			{
				*vertexID = this->boundaryVertexID[boundaryID][boundaryVertexID];
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			inline I CupCfdSoAMesh<I,T,L>::getBoundaryVertex(I boundaryID, I boundaryVertexID)
			{
				return this->boundaryVertexID[boundaryID][boundaryVertexID];
			}

			template <class I, class T, class L>
			inline cupcfd::error::eCodes CupCfdSoAMesh<I,T,L>::getBoundaryDistance(I boundaryID, T * distance)
			{
				*distance = this->boundaryDistance[boundaryID];
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			inline T CupCfdSoAMesh<I,T,L>::getBoundaryDistance(I boundaryID)
			{
				return this->boundaryDistance[boundaryID];
			}

			template <class I, class T, class L>
			inline cupcfd::error::eCodes CupCfdSoAMesh<I,T,L>::getBoundaryYPlus(I boundaryID, T * yPlus)
			{
				*yPlus = this->boundaryYPlus[boundaryID];
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			inline T CupCfdSoAMesh<I,T,L>::getBoundaryYPlus(I boundaryID)
			{
				return this->boundaryYPlus[boundaryID];
			}

			template <class I, class T, class L>
			inline cupcfd::error::eCodes CupCfdSoAMesh<I,T,L>::getBoundaryUPlus(I boundaryID, T * uPlus)
			{
				*uPlus = this->boundaryUPlus[boundaryID];
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			inline T CupCfdSoAMesh<I,T,L>::getBoundaryUPlus(I boundaryID)
			{
				return this->boundaryUPlus[boundaryID];
			}

			template <class I, class T, class L>
			inline cupcfd::error::eCodes CupCfdSoAMesh<I,T,L>::getBoundaryRegionID(I boundaryID, I * regionID)
			{
				*regionID = this->boundaryRegionID[boundaryID];
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			inline I CupCfdSoAMesh<I,T,L>::getBoundaryRegionID(I boundaryID)
			{
				return this->boundaryRegionID[boundaryID];
			}

			template <class I, class T, class L>
			inline cupcfd::error::eCodes CupCfdSoAMesh<I,T,L>::getBoundaryShear(I boundaryID, cupcfd::geometry::euclidean::EuclideanVector<T,3>& shear)
			{
				shear = this->boundaryShear[boundaryID];
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			inline cupcfd::geometry::euclidean::EuclideanVector<T,3> CupCfdSoAMesh<I,T,L>::getBoundaryShear(I boundaryID)
			{
				return this->boundaryShear[boundaryID];
			}

			template <class I, class T, class L>
			inline cupcfd::error::eCodes CupCfdSoAMesh<I,T,L>::getBoundaryQ(I boundaryID, T * q)
			{
				*q = this->boundaryQ[boundaryID];
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			inline T CupCfdSoAMesh<I,T,L>::getBoundaryQ(I boundaryID)
			{
				return this->boundaryQ[boundaryID];
			}

			template <class I, class T, class L>
			inline cupcfd::error::eCodes CupCfdSoAMesh<I,T,L>::getBoundaryH(I boundaryID, T * h)
			{
				*h = this->boundaryH[boundaryID];
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			inline T CupCfdSoAMesh<I,T,L>::getBoundaryH(I boundaryID)
			{
				return this->boundaryH[boundaryID];
			}

			template <class I, class T, class L>
			inline cupcfd::error::eCodes CupCfdSoAMesh<I,T,L>::getBoundaryT(I boundaryID, T * t)
			{
				*t = this->boundaryT[boundaryID];
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			inline T CupCfdSoAMesh<I,T,L>::getBoundaryT(I boundaryID)
			{
				return this->boundaryT[boundaryID];
			}

			template <class I, class T, class L>
			inline cupcfd::error::eCodes CupCfdSoAMesh<I,T,L>::setBoundaryFaceID(I boundaryID, I faceID)
			{
				this->boundaryFaceID[boundaryID] = faceID;
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			inline cupcfd::error::eCodes CupCfdSoAMesh<I,T,L>::setBoundaryVertex(I boundaryID, I boundaryVertexID, I vertexID)
			{
				this->boundaryVertexID[boundaryID][boundaryVertexID] = vertexID;
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			inline cupcfd::error::eCodes CupCfdSoAMesh<I,T,L>::setBoundaryDistance(I boundaryID, T distance)
			{
				this->boundaryDistance[boundaryID] = distance;
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			inline cupcfd::error::eCodes CupCfdSoAMesh<I,T,L>::setBoundaryYPlus(I boundaryID, T yPlus)
			{
				this->boundaryYPlus[boundaryID] = yPlus;
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			inline cupcfd::error::eCodes CupCfdSoAMesh<I,T,L>::setBoundaryUPlus(I boundaryID, T uPlus)
			{
				this->boundaryUPlus[boundaryID] = uPlus;
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			inline cupcfd::error::eCodes CupCfdSoAMesh<I,T,L>::setBoundaryRegionID(I boundaryID, I regionID)
			{
				this->boundaryRegionID[boundaryID] = regionID;
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			inline cupcfd::error::eCodes CupCfdSoAMesh<I,T,L>::setBoundaryShear(I boundaryID, cupcfd::geometry::euclidean::EuclideanVector<T,3>& shear)
			{
				this->boundaryShear[boundaryID] = shear;
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			inline cupcfd::error::eCodes CupCfdSoAMesh<I,T,L>::setBoundaryQ(I boundaryID, T q)
			{
				this->boundaryQ[boundaryID] = q;
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			inline cupcfd::error::eCodes CupCfdSoAMesh<I,T,L>::setBoundaryH(I boundaryID, T h)
			{
				this->boundaryH[boundaryID] = h;
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			inline cupcfd::error::eCodes CupCfdSoAMesh<I,T,L>::setBoundaryT(I boundaryID, T t)
			{
				this->boundaryT[boundaryID] = t;
				return cupcfd::error::E_SUCCESS;
			}
			
			template <class I, class T, class L>
			cupcfd::error::eCodes CupCfdSoAMesh<I,T,L>::getBoundaryID(L boundaryLabel, I * boundaryID)
			{
				*boundaryID = this->boundaryBuildIDToLocalID[boundaryLabel];
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			I CupCfdSoAMesh<I,T,L>::getBoundaryID(L boundaryLabel)
			{
				return this->boundaryBuildIDToLocalID[boundaryLabel];
			}
			
			template <class I, class T, class L>			
			inline cupcfd::error::eCodes CupCfdSoAMesh<I,T,L>::getRegionType(I regionID, RType * type)
			{
				*type = this->regionType[regionID];
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			inline RType CupCfdSoAMesh<I,T,L>::getRegionType(I regionID)
			{
				return this->regionType[regionID];
			}

			template <class I, class T, class L>
			inline cupcfd::error::eCodes CupCfdSoAMesh<I,T,L>::getRegionStd(I regionID, bool * std)
			{
				*std = this->regionStd[regionID];
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			inline bool CupCfdSoAMesh<I,T,L>::getRegionStd(I regionID)
			{
				return this->regionStd[regionID];
			}

			template <class I, class T, class L>
			inline cupcfd::error::eCodes CupCfdSoAMesh<I,T,L>::getRegionYLog(I regionID, T * yLog)
			{
				*yLog = this->regionYLog[regionID];
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			inline T CupCfdSoAMesh<I,T,L>::getRegionYLog(I regionID)
			{
				return this->regionYLog[regionID];
			}

			template <class I, class T, class L>
			inline cupcfd::error::eCodes CupCfdSoAMesh<I,T,L>::getRegionELog(I regionID, T * eLog)
			{
				*eLog = this->regionELog[regionID];
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			inline T CupCfdSoAMesh<I,T,L>::getRegionELog(I regionID)
			{
				return this->regionELog[regionID];
			}

			template <class I, class T, class L>
			inline cupcfd::error::eCodes CupCfdSoAMesh<I,T,L>::getRegionDensity(I regionID, T * density)
			{
				*density = this->regionDensity[regionID];
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			inline T CupCfdSoAMesh<I,T,L>::getRegionDensity(I regionID)
			{
				return this->regionDensity[regionID];
			}

			template <class I, class T, class L>
			inline cupcfd::error::eCodes CupCfdSoAMesh<I,T,L>::getRegionTurbKE(I regionID, T * turbKE)
			{
				*turbKE = this->regionTurbKE[regionID];
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			inline T CupCfdSoAMesh<I,T,L>::getRegionTurbKE(I regionID)
			{
				return this->regionTurbKE[regionID];
			}

			template <class I, class T, class L>
			inline cupcfd::error::eCodes CupCfdSoAMesh<I,T,L>::getRegionTurbDiss(I regionID, T * turbDiss)
			{
				*turbDiss = this->regionTurbDiss[regionID];
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			inline T CupCfdSoAMesh<I,T,L>::getRegionTurbDiss(I regionID)
			{
				return this->regionTurbDiss[regionID];
			}

			template <class I, class T, class L>
			inline cupcfd::error::eCodes CupCfdSoAMesh<I,T,L>::getRegionSplvl(I regionID, T * splvl)
			{
				*splvl = this->regionSplvl[regionID];
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			inline T CupCfdSoAMesh<I,T,L>::getRegionSplvl(I regionID)
			{
				return this->regionSplvl[regionID];
			}

			template <class I, class T, class L>
			inline cupcfd::error::eCodes CupCfdSoAMesh<I,T,L>::getRegionDen(I regionID, T * den)
			{
				*den = this->regionDen[regionID];
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			inline T CupCfdSoAMesh<I,T,L>::getRegionDen(I regionID)
			{
				return this->regionDen[regionID];
			}

			template <class I, class T, class L>
			inline cupcfd::error::eCodes CupCfdSoAMesh<I,T,L>::getRegionForceTangent(I regionID, cupcfd::geometry::euclidean::EuclideanVector<T,3>& forceTangent)
			{
				forceTangent = this->regionForceTangent[regionID];
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			inline cupcfd::geometry::euclidean::EuclideanVector<T,3> CupCfdSoAMesh<I,T,L>::getRegionForceTangent(I regionID)
			{
				return this->regionForceTangent[regionID];
			}

			template <class I, class T, class L>
			inline cupcfd::error::eCodes CupCfdSoAMesh<I,T,L>::getRegionUVW(I regionID, cupcfd::geometry::euclidean::EuclideanVector<T,3>& uvw)
			{
				uvw = this->regionUVW[regionID];
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			inline cupcfd::geometry::euclidean::EuclideanVector<T,3> CupCfdSoAMesh<I,T,L>::getRegionUVW(I regionID)
			{
				return this->regionUVW[regionID];
			}

			template <class I, class T, class L>
			inline cupcfd::error::eCodes CupCfdSoAMesh<I,T,L>::getRegionName(I regionID, std::string& regionName)
			{
				regionName = this->regionName[regionID];
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			inline std::string CupCfdSoAMesh<I,T,L>::getRegionName(I regionID)
			{
				return this->regionName[regionID];
			}

			template <class I, class T, class L>
			inline cupcfd::error::eCodes CupCfdSoAMesh<I,T,L>::getRegionFlux(I regionID, bool * flux)
			{
				*flux = this->regionFlux[regionID];
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			inline bool CupCfdSoAMesh<I,T,L>::getRegionFlux(I regionID)
			{
				return this->regionFlux[regionID];
			}

			template <class I, class T, class L>
			inline cupcfd::error::eCodes CupCfdSoAMesh<I,T,L>::getRegionAdiab(I regionID, bool * adiab)
			{
				*adiab = this->regionAdiab[regionID];
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			inline bool CupCfdSoAMesh<I,T,L>::getRegionAdiab(I regionID)
			{
				return this->regionAdiab[regionID];
			}

			template <class I, class T, class L>
			inline cupcfd::error::eCodes CupCfdSoAMesh<I,T,L>::getRegionR(I regionID, T * r)
			{
				*r = this->regionR[regionID];
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			inline T CupCfdSoAMesh<I,T,L>::getRegionR(I regionID)
			{
				return this->regionR[regionID];
			}

			template <class I, class T, class L>
			inline cupcfd::error::eCodes CupCfdSoAMesh<I,T,L>::getRegionT(I regionID, T * t)
			{
				*t = this->regionT[regionID];
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			inline T CupCfdSoAMesh<I,T,L>::getRegionT(I regionID)
			{
				return this->regionT[regionID];
			}

			template <class I, class T, class L>
			inline cupcfd::error::eCodes CupCfdSoAMesh<I,T,L>::setRegionType(I regionID, RType type)
			{
				this->regionType[regionID] = type;
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			inline cupcfd::error::eCodes CupCfdSoAMesh<I,T,L>::setRegionStd(I regionID, bool std)
			{
				this->regionStd[regionID] = std;
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			inline cupcfd::error::eCodes CupCfdSoAMesh<I,T,L>::setRegionYLog(I regionID, T yLog)
			{
				this->regionYLog[regionID] = yLog;
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			inline cupcfd::error::eCodes CupCfdSoAMesh<I,T,L>::setRegionELog(I regionID, T eLog)
			{
				this->regionELog[regionID] = eLog;
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			inline cupcfd::error::eCodes CupCfdSoAMesh<I,T,L>::setRegionDensity(I regionID, T density)
			{
				this->regionDensity[regionID] = density;
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			inline cupcfd::error::eCodes CupCfdSoAMesh<I,T,L>::setRegionTurbKE(I regionID, T turbKE)
			{
				this->regionTurbKE[regionID] = turbKE;
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			inline cupcfd::error::eCodes CupCfdSoAMesh<I,T,L>::setRegionTurbDiss(I regionID, T turbDiss)
			{
				this->regionTurbDiss[regionID] = turbDiss;
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			inline cupcfd::error::eCodes CupCfdSoAMesh<I,T,L>::setRegionSplvl(I regionID, T splvl)
			{
				this->regionSplvl[regionID] = splvl;
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			inline cupcfd::error::eCodes CupCfdSoAMesh<I,T,L>::setRegionDen(I regionID, T den)
			{
				this->regionDen[regionID] = den;
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			inline cupcfd::error::eCodes CupCfdSoAMesh<I,T,L>::setRegionForceTangent(I regionID, cupcfd::geometry::euclidean::EuclideanVector<T,3>& forceTangent)
			{
				this->regionForceTangent[regionID] = forceTangent;
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			inline cupcfd::error::eCodes CupCfdSoAMesh<I,T,L>::setRegionUVW(I regionID, cupcfd::geometry::euclidean::EuclideanVector<T,3>& uvw)
			{
				this->regionUVW[regionID] = uvw;
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			inline cupcfd::error::eCodes CupCfdSoAMesh<I,T,L>::setRegionName(I regionID, std::string& regionName)
			{
				this->regionName[regionID] = regionName;
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			inline cupcfd::error::eCodes CupCfdSoAMesh<I,T,L>::setRegionFlux(I regionID, bool flux)
			{
				this->regionFlux[regionID] = flux;
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			inline cupcfd::error::eCodes CupCfdSoAMesh<I,T,L>::setRegionAdiab(I regionID, bool adiab)
			{
				this->regionAdiab[regionID] = adiab;
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			inline cupcfd::error::eCodes CupCfdSoAMesh<I,T,L>::setRegionR(I regionID, T r)
			{
				this->regionR[regionID] = r;
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			inline cupcfd::error::eCodes CupCfdSoAMesh<I,T,L>::setRegionT(I regionID, T t)
			{
				this->regionT[regionID] = t;
				return cupcfd::error::E_SUCCESS;
			}
			
			template <class I, class T, class L>
			cupcfd::error::eCodes CupCfdSoAMesh<I,T,L>::getRegionID(L regionLabel, I * regionID)
			{
				*regionID = this->regionBuildIDToLocalID[regionLabel];;
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			I CupCfdSoAMesh<I,T,L>::getRegionID(L regionLabel)
			{
				return this->regionBuildIDToLocalID[regionLabel];
			}
			
			template <class I, class T, class L>
			inline cupcfd::error::eCodes CupCfdSoAMesh<I,T,L>::getVertexPos(I vertexID, cupcfd::geometry::euclidean::EuclideanPoint<T,3>& pos)
			{
				pos = this->verticesPos[vertexID];
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			inline cupcfd::geometry::euclidean::EuclideanPoint<T,3> CupCfdSoAMesh<I,T,L>::getVertexPos(I vertexID)
			{
				return this->verticesPos[vertexID];
			}

			template <class I, class T, class L>
			inline cupcfd::error::eCodes CupCfdSoAMesh<I,T,L>::setVertexPos(I vertexID, cupcfd::geometry::euclidean::EuclideanPoint<T,3>& pos)
			{
				this->verticesPos[vertexID] = pos;
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			inline cupcfd::error::eCodes CupCfdSoAMesh<I,T,L>::getVertexID(L vertexLabel, I * vertexID)
			{
				*vertexID = this->vertexBuildIDToLocalID[vertexLabel];
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			inline I CupCfdSoAMesh<I,T,L>::getVertexID(L vertexLabel)
			{
				return this->vertexBuildIDToLocalID[vertexLabel];
			}					
		}
	}
}

#endif
