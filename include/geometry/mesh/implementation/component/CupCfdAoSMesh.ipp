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
			inline cupcfd::error::eCodes CupCfdAoSMesh<I,T,L>::getCellCenter(I cellID, cupcfd::geometry::euclidean::EuclideanPoint<T,3>& center)
			{
				center = this->cells[cellID].center;
				return cupcfd::error::E_SUCCESS;
			}
			
			template <class I, class T, class L>
			inline cupcfd::geometry::euclidean::EuclideanPoint<T,3> CupCfdAoSMesh<I,T,L>::getCellCenter(I cellID)
			{
				return this->cells[cellID].center;
			}
			
			template <class I, class T, class L>
			inline cupcfd::error::eCodes CupCfdAoSMesh<I,T,L>::getCellVolume(I cellID, T * vol)
			{
				*vol = this->cells[cellID].vol;
				return cupcfd::error::E_SUCCESS;
			}
			
			template <class I, class T, class L>
			inline T CupCfdAoSMesh<I,T,L>::getCellVolume(I cellID)
			{
				return this->cells[cellID].vol;
			}
			
			template <class I, class T, class L>
			inline cupcfd::error::eCodes CupCfdAoSMesh<I,T,L>::getCellNFaces(I cellID, I * nFaces)
			{
				*nFaces = this->cellNGFaces[cellID];
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			inline I CupCfdAoSMesh<I,T,L>::getCellNFaces(I cellID)
			{
				return this->cellNGFaces[cellID];
			}

			template <class I, class T, class L>
			inline cupcfd::error::eCodes CupCfdAoSMesh<I,T,L>::getCellStoredNFaces(I cellID, I * nFaces)
			{
				*nFaces = cellNFaces[cellID];
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			inline I CupCfdAoSMesh<I,T,L>::getCellStoredNFaces(I cellID)
			{
				return cellNFaces[cellID];
			}

			template <class I, class T, class L>
			inline cupcfd::error::eCodes CupCfdAoSMesh<I,T,L>::getCellNVertices(I cellID, int * nVertices)
			{
				if(this->finalized == false)
				{
					return cupcfd::error::E_UNFINALIZED;
				}

				*nVertices = this->cellNGVertices[cellID];

				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			int CupCfdAoSMesh<I,T,L>::getCellNVertices(I cellID)
			{
				return this->cellNGVertices[cellID];
			}

			template <class I, class T, class L>
			inline cupcfd::error::eCodes CupCfdAoSMesh<I,T,L>::getCellStoredNVertices(I cellID, I * nVertices)
			{
				*nVertices = cellNVertices[cellID];
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			I CupCfdAoSMesh<I,T,L>::getCellStoredNVertices(I cellID)
			{
				return cellNVertices[cellID];
			}

			template <class I, class T, class L>
			inline cupcfd::error::eCodes CupCfdAoSMesh<I,T,L>::getCellFaceID(I cellID, I cellFaceID, I * faceID)
			{
				I index = this->cellFaceMapCSRXAdj[cellID] + cellFaceID;
				*faceID = this->cellFaceMapCSRAdj[index];

				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			inline I CupCfdAoSMesh<I,T,L>::getCellFaceID(I cellID, I cellFaceID)
			{
				I index = this->cellFaceMapCSRXAdj[cellID] + cellFaceID;
				return this->cellFaceMapCSRAdj[index];
			}

			template <class I, class T, class L>
			inline cupcfd::error::eCodes CupCfdAoSMesh<I,T,L>::setCellCenter(I cellID, cupcfd::geometry::euclidean::EuclideanPoint<T,3>& center)
			{
				this->cells[cellID].center = center;
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			inline cupcfd::error::eCodes CupCfdAoSMesh<I,T,L>::setCellVolume(I cellID, T vol)
			{
				this->cells[cellID].vol = vol;
				return cupcfd::error::E_SUCCESS;
			}
			
			template <class I, class T, class L>
			cupcfd::error::eCodes CupCfdAoSMesh<I,T,L>::getCellID(L cellLabel, I * cellID)
			{
				*cellID = this->cellBuildIDToLocalID[cellLabel];
				return cupcfd::error::E_SUCCESS;
			}
			
			template <class I, class T, class L>
			I CupCfdAoSMesh<I,T,L>::getCellID(L cellLabel)
			{
				return this->cellBuildIDToLocalID[cellLabel];
			}
			
			template <class I, class T, class L>
			inline cupcfd::error::eCodes CupCfdAoSMesh<I,T,L>::getFaceNVertices(I faceID, I * nVertices)
			{
				*nVertices = this->getFaceNVertices(faceID);
				return cupcfd::error::E_SUCCESS;
			}
			
			template <class I, class T, class L>
			inline I CupCfdAoSMesh<I,T,L>::getFaceNVertices(I faceID)
			{
				// Currently we can only have three or four vertices max per face
				if(this->faces[faceID].verticesID[3] == -1)
				{
					return 3;
				}
				else
				{
					return 4;
				}
			}
			
			template <class I, class T, class L>
			inline cupcfd::error::eCodes CupCfdAoSMesh<I,T,L>::getFaceCell1ID(I faceID, I * cellID)
			{
				*cellID = this->faces[faceID].cell1ID;
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			inline I CupCfdAoSMesh<I,T,L>::getFaceCell1ID(I faceID)
			{
				return this->faces[faceID].cell1ID;
			}

			template <class I, class T, class L>
			inline cupcfd::error::eCodes CupCfdAoSMesh<I,T,L>::getFaceCell2ID(I faceID, I * cellID)
			{
				*cellID = this->faces[faceID].cell2ID;
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			inline I CupCfdAoSMesh<I,T,L>::getFaceCell2ID(I faceID)
			{
				return this->faces[faceID].cell2ID;
			}

			template <class I, class T, class L>
			inline cupcfd::error::eCodes CupCfdAoSMesh<I,T,L>::getFaceLambda(I faceID, T * lambda)
			{
				*lambda = this->faces[faceID].lambda;
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			inline T CupCfdAoSMesh<I,T,L>::getFaceLambda(I faceID)
			{
				return this->faces[faceID].lambda;
			}

			template <class I, class T, class L>
			inline cupcfd::error::eCodes CupCfdAoSMesh<I,T,L>::getFaceNorm(I faceID, cupcfd::geometry::euclidean::EuclideanVector<T,3>& norm)
			{
				norm = this->faces[faceID].norm;
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			inline cupcfd::geometry::euclidean::EuclideanVector<T,3> CupCfdAoSMesh<I,T,L>::getFaceNorm(I faceID)
			{
				return this->faces[faceID].norm;
			}

			template <class I, class T, class L>
			inline cupcfd::error::eCodes CupCfdAoSMesh<I,T,L>::getFaceVertex(I faceID, I faceVertexID, I * vertexID)
			{
				*vertexID = this->faces[faceID].verticesID[faceVertexID];
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			inline I CupCfdAoSMesh<I,T,L>::getFaceVertex(I faceID, I faceVertexID)
			{
				return this->faces[faceID].verticesID[faceVertexID];
			}

			template <class I, class T, class L>
			inline cupcfd::error::eCodes CupCfdAoSMesh<I,T,L>::getFaceCenter(I faceID, cupcfd::geometry::euclidean::EuclideanPoint<T,3>& center)
			{
				center = this->faces[faceID].center;
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			inline cupcfd::geometry::euclidean::EuclideanPoint<T,3> CupCfdAoSMesh<I,T,L>::getFaceCenter(I faceID)
			{
				return this->faces[faceID].center;
			}

			template <class I, class T, class L>
			inline cupcfd::error::eCodes CupCfdAoSMesh<I,T,L>::getFaceRLencos(I faceID, T * rlencos)
			{
				*rlencos = this->faces[faceID].rlencos;
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			inline T CupCfdAoSMesh<I,T,L>::getFaceRLencos(I faceID)
			{
				return this->faces[faceID].rlencos;
			}

			template <class I, class T, class L>
			inline cupcfd::error::eCodes CupCfdAoSMesh<I,T,L>::getFaceArea(I faceID, T * area)
			{
				*area = this->faces[faceID].area;
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			inline T CupCfdAoSMesh<I,T,L>::getFaceArea(I faceID)
			{
				return this->faces[faceID].area;
			}

			template <class I, class T, class L>
			inline cupcfd::error::eCodes CupCfdAoSMesh<I,T,L>::getFaceXpac(I faceID, cupcfd::geometry::euclidean::EuclideanPoint<T,3>& xpac)
			{
				xpac = this->faces[faceID].xpac;
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			inline cupcfd::geometry::euclidean::EuclideanPoint<T,3> CupCfdAoSMesh<I,T,L>::getFaceXpac(I faceID)
			{
				return this->faces[faceID].xpac;
			}

			template <class I, class T, class L>
			inline cupcfd::error::eCodes CupCfdAoSMesh<I,T,L>::getFaceXnac(I faceID, cupcfd::geometry::euclidean::EuclideanPoint<T,3>& xnac)
			{
				xnac = this->faces[faceID].xnac;
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			inline cupcfd::geometry::euclidean::EuclideanPoint<T,3> CupCfdAoSMesh<I,T,L>::getFaceXnac(I faceID)
			{
				return this->faces[faceID].xnac;
			}

			template <class I, class T, class L>
			inline cupcfd::error::eCodes CupCfdAoSMesh<I,T,L>::getFaceIsBoundary(I faceID, bool * result)
			{
				if(this->faces[faceID].bndID > -1)
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
			inline bool CupCfdAoSMesh<I,T,L>::getFaceIsBoundary(I faceID)
			{
				if(this->faces[faceID].bndID > -1)
				{
					return true;
				}

				return false;
			}

			template <class I, class T, class L>
			inline cupcfd::error::eCodes CupCfdAoSMesh<I,T,L>::getFaceBoundaryID(I faceID, I * boundaryID)
			{
				*boundaryID = this->faces[faceID].bndID;
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			inline I CupCfdAoSMesh<I,T,L>::getFaceBoundaryID(I faceID)
			{
				return this->faces[faceID].bndID;
			}

			template <class I, class T, class L>
			inline cupcfd::error::eCodes CupCfdAoSMesh<I,T,L>::setFaceCell1ID(I faceID, I cellID)
			{
				this->faces[faceID].cell1ID = cellID;
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			inline cupcfd::error::eCodes CupCfdAoSMesh<I,T,L>::setFaceCell2ID(I faceID, I cellID)
			{
				this->faces[faceID].cell2ID = cellID;
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			inline cupcfd::error::eCodes CupCfdAoSMesh<I,T,L>::setFaceLambda(I faceID, T lambda)
			{
				this->faces[faceID].lambda = lambda;
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			inline cupcfd::error::eCodes CupCfdAoSMesh<I,T,L>::setFaceNorm(I faceID, cupcfd::geometry::euclidean::EuclideanVector<T,3>& norm)
			{
				this->faces[faceID].norm = norm;
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			inline cupcfd::error::eCodes CupCfdAoSMesh<I,T,L>::setFaceVertex(I faceID, I faceVertexID, I vertexID)
			{
				this->faces[faceID].verticesID[faceVertexID] = vertexID;
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			inline cupcfd::error::eCodes CupCfdAoSMesh<I,T,L>::setFaceCenter(I faceID, cupcfd::geometry::euclidean::EuclideanPoint<T,3>& center)
			{
				this->faces[faceID].center = center;
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			inline cupcfd::error::eCodes CupCfdAoSMesh<I,T,L>::setFaceRLencos(I faceID, T rlencos)
			{
				this->faces[faceID].rlencos = rlencos;
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			inline cupcfd::error::eCodes CupCfdAoSMesh<I,T,L>::setFaceArea(I faceID, T area)
			{
				this->faces[faceID].area = area;
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			inline cupcfd::error::eCodes CupCfdAoSMesh<I,T,L>::setFaceXpac(I faceID, cupcfd::geometry::euclidean::EuclideanPoint<T,3>& xpac)
			{
				this->faces[faceID].xpac = xpac;
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			inline cupcfd::error::eCodes CupCfdAoSMesh<I,T,L>::setFaceXnac(I faceID, cupcfd::geometry::euclidean::EuclideanPoint<T,3>& xnac)
			{
				this->faces[faceID].xnac = xnac;
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			inline cupcfd::error::eCodes CupCfdAoSMesh<I,T,L>::setFaceBoundaryID(I faceID, I boundaryID)
			{
				this->faces[faceID].bndID = boundaryID;
				return cupcfd::error::E_SUCCESS;
			}
			
			template <class I, class T, class L>
			cupcfd::error::eCodes CupCfdAoSMesh<I,T,L>::getFaceID(L faceLabel, I * faceID)
			{
				*faceID = this->faceBuildIDToLocalID[faceLabel];
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			I CupCfdAoSMesh<I,T,L>::getFaceID(L faceLabel)
			{
				return this->faceBuildIDToLocalID[faceLabel];
			}
			
			template <class I, class T, class L>
			inline cupcfd::error::eCodes CupCfdAoSMesh<I,T,L>::getBoundaryFaceID(I boundaryID, I * faceID)
			{
				*faceID = this->boundaries[boundaryID].faceID;
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			inline I CupCfdAoSMesh<I,T,L>::getBoundaryFaceID(I boundaryID)
			{
				return this->boundaries[boundaryID].faceID;
			}

			template <class I, class T, class L>
			inline cupcfd::error::eCodes CupCfdAoSMesh<I,T,L>::getBoundaryVertex(I boundaryID, I boundaryVertexID, I * vertexID)
			{
				*vertexID = this->boundaries[boundaryID].verticesID[boundaryVertexID];
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			inline I CupCfdAoSMesh<I,T,L>::getBoundaryVertex(I boundaryID, I boundaryVertexID)
			{
				return this->boundaries[boundaryID].verticesID[boundaryVertexID];
			}

			template <class I, class T, class L>
			inline cupcfd::error::eCodes CupCfdAoSMesh<I,T,L>::getBoundaryDistance(I boundaryID, T * distance)
			{
				*distance = this->boundaries[boundaryID].distance;
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			inline T CupCfdAoSMesh<I,T,L>::getBoundaryDistance(I boundaryID)
			{
				return this->boundaries[boundaryID].distance;
			}

			template <class I, class T, class L>
			inline cupcfd::error::eCodes CupCfdAoSMesh<I,T,L>::getBoundaryYPlus(I boundaryID, T * yPlus)
			{
				*yPlus = this->boundaries[boundaryID].yplus;
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			inline T CupCfdAoSMesh<I,T,L>::getBoundaryYPlus(I boundaryID)
			{
				return this->boundaries[boundaryID].yplus;
			}

			template <class I, class T, class L>
			inline cupcfd::error::eCodes CupCfdAoSMesh<I,T,L>::getBoundaryUPlus(I boundaryID, T * uPlus)
			{
				*uPlus = this->boundaries[boundaryID].uplus;
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			inline T CupCfdAoSMesh<I,T,L>::getBoundaryUPlus(I boundaryID)
			{
				return this->boundaries[boundaryID].uplus;
			}

			template <class I, class T, class L>
			inline cupcfd::error::eCodes CupCfdAoSMesh<I,T,L>::getBoundaryRegionID(I boundaryID, I * regionID)
			{
				*regionID = this->boundaries[boundaryID].regionID;
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			inline I CupCfdAoSMesh<I,T,L>::getBoundaryRegionID(I boundaryID)
			{
				return this->boundaries[boundaryID].regionID;
			}

			template <class I, class T, class L>
			inline cupcfd::error::eCodes CupCfdAoSMesh<I,T,L>::getBoundaryShear(I boundaryID, cupcfd::geometry::euclidean::EuclideanVector<T,3>& shear)
			{
				shear = this->boundaries[boundaryID].shear;
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			inline cupcfd::geometry::euclidean::EuclideanVector<T,3> CupCfdAoSMesh<I,T,L>::getBoundaryShear(I boundaryID)
			{
				return this->boundaries[boundaryID].shear;
			}

			template <class I, class T, class L>
			inline cupcfd::error::eCodes CupCfdAoSMesh<I,T,L>::getBoundaryQ(I boundaryID, T * q)
			{
				*q = this->boundaries[boundaryID].q;
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			inline T CupCfdAoSMesh<I,T,L>::getBoundaryQ(I boundaryID)
			{
				return this->boundaries[boundaryID].q;
			}

			template <class I, class T, class L>
			inline cupcfd::error::eCodes CupCfdAoSMesh<I,T,L>::getBoundaryH(I boundaryID, T * h)
			{
				*h = this->boundaries[boundaryID].h;
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			inline T CupCfdAoSMesh<I,T,L>::getBoundaryH(I boundaryID)
			{
				return this->boundaries[boundaryID].h;
			}

			template <class I, class T, class L>
			inline cupcfd::error::eCodes CupCfdAoSMesh<I,T,L>::getBoundaryT(I boundaryID, T * t)
			{
				*t = this->boundaries[boundaryID].t;
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			inline T CupCfdAoSMesh<I,T,L>::getBoundaryT(I boundaryID)
			{
				return this->boundaries[boundaryID].t;
			}

			template <class I, class T, class L>
			inline cupcfd::error::eCodes CupCfdAoSMesh<I,T,L>::setBoundaryFaceID(I boundaryID, I faceID)
			{
				this->boundaries[boundaryID].faceID = faceID;
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			inline cupcfd::error::eCodes CupCfdAoSMesh<I,T,L>::setBoundaryVertex(I boundaryID, I boundaryVertexID, I vertexID)
			{
				this->boundaries[boundaryID].verticesID[boundaryVertexID] = vertexID;
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			inline cupcfd::error::eCodes CupCfdAoSMesh<I,T,L>::setBoundaryDistance(I boundaryID, T distance)
			{
				this->boundaries[boundaryID].distance = distance;
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			inline cupcfd::error::eCodes CupCfdAoSMesh<I,T,L>::setBoundaryYPlus(I boundaryID, T yPlus)
			{
				this->boundaries[boundaryID].yplus = yPlus;
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			inline cupcfd::error::eCodes CupCfdAoSMesh<I,T,L>::setBoundaryUPlus(I boundaryID, T uPlus)
			{
				this->boundaries[boundaryID].uplus = uPlus;
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			inline cupcfd::error::eCodes CupCfdAoSMesh<I,T,L>::setBoundaryRegionID(I boundaryID, I regionID)
			{
				this->boundaries[boundaryID].regionID = regionID;
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			inline cupcfd::error::eCodes CupCfdAoSMesh<I,T,L>::setBoundaryShear(I boundaryID, cupcfd::geometry::euclidean::EuclideanVector<T,3>& shear)
			{
				this->boundaries[boundaryID].shear = shear;
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			inline cupcfd::error::eCodes CupCfdAoSMesh<I,T,L>::setBoundaryQ(I boundaryID, T q)
			{
				this->boundaries[boundaryID].q = q;
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			inline cupcfd::error::eCodes CupCfdAoSMesh<I,T,L>::setBoundaryH(I boundaryID, T h)
			{
				this->boundaries[boundaryID].h = h;
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			inline cupcfd::error::eCodes CupCfdAoSMesh<I,T,L>::setBoundaryT(I boundaryID, T t)
			{
				this->boundaries[boundaryID].t = t;
				return cupcfd::error::E_SUCCESS;
			}
			
			template <class I, class T, class L>
			cupcfd::error::eCodes CupCfdAoSMesh<I,T,L>::getBoundaryID(L boundaryLabel, I * boundaryID)
			{
				*boundaryID = this->boundaryBuildIDToLocalID[boundaryLabel];
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			I CupCfdAoSMesh<I,T,L>::getBoundaryID(L boundaryLabel)
			{
				return this->boundaryBuildIDToLocalID[boundaryLabel];
			}
			
			template <class I, class T, class L>			
			inline cupcfd::error::eCodes CupCfdAoSMesh<I,T,L>::getRegionType(I regionID, RType * type)
			{
				*type = this->regions[regionID].rType;
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			inline RType CupCfdAoSMesh<I,T,L>::getRegionType(I regionID)
			{
				return this->regions[regionID].rType;
			}

			template <class I, class T, class L>
			inline cupcfd::error::eCodes CupCfdAoSMesh<I,T,L>::getRegionStd(I regionID, bool * std)
			{
				*std = this->regions[regionID].std;
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			inline bool CupCfdAoSMesh<I,T,L>::getRegionStd(I regionID)
			{
				return this->regions[regionID].std;
			}

			template <class I, class T, class L>
			inline cupcfd::error::eCodes CupCfdAoSMesh<I,T,L>::getRegionYLog(I regionID, T * yLog)
			{
				*yLog = this->regions[regionID].ylog;
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			inline T CupCfdAoSMesh<I,T,L>::getRegionYLog(I regionID)
			{
				return this->regions[regionID].ylog;
			}

			template <class I, class T, class L>
			inline cupcfd::error::eCodes CupCfdAoSMesh<I,T,L>::getRegionELog(I regionID, T * eLog)
			{
				*eLog = this->regions[regionID].elog;
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			inline T CupCfdAoSMesh<I,T,L>::getRegionELog(I regionID)
			{
				return this->regions[regionID].elog;
			}

			template <class I, class T, class L>
			inline cupcfd::error::eCodes CupCfdAoSMesh<I,T,L>::getRegionDensity(I regionID, T * density)
			{
				*density = this->regions[regionID].density;
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			inline T CupCfdAoSMesh<I,T,L>::getRegionDensity(I regionID)
			{
				return this->regions[regionID].density;
			}

			template <class I, class T, class L>
			inline cupcfd::error::eCodes CupCfdAoSMesh<I,T,L>::getRegionTurbKE(I regionID, T * turbKE)
			{
				*turbKE = this->regions[regionID].turbKE;
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			inline T CupCfdAoSMesh<I,T,L>::getRegionTurbKE(I regionID)
			{
				return this->regions[regionID].turbKE;
			}

			template <class I, class T, class L>
			inline cupcfd::error::eCodes CupCfdAoSMesh<I,T,L>::getRegionTurbDiss(I regionID, T * turbDiss)
			{
				*turbDiss = this->regions[regionID].turbDiss;
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			inline T CupCfdAoSMesh<I,T,L>::getRegionTurbDiss(I regionID)
			{
				return this->regions[regionID].turbDiss;
			}

			template <class I, class T, class L>
			inline cupcfd::error::eCodes CupCfdAoSMesh<I,T,L>::getRegionSplvl(I regionID, T * splvl)
			{
				*splvl = this->regions[regionID].splvl;
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			inline T CupCfdAoSMesh<I,T,L>::getRegionSplvl(I regionID)
			{
				return this->regions[regionID].splvl;
			}

			template <class I, class T, class L>
			inline cupcfd::error::eCodes CupCfdAoSMesh<I,T,L>::getRegionDen(I regionID, T * den)
			{
				*den = this->regions[regionID].den;
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			inline T CupCfdAoSMesh<I,T,L>::getRegionDen(I regionID)
			{
				return this->regions[regionID].den;
			}

			template <class I, class T, class L>
			inline cupcfd::error::eCodes CupCfdAoSMesh<I,T,L>::getRegionForceTangent(I regionID, cupcfd::geometry::euclidean::EuclideanVector<T,3>& forceTangent)
			{
				forceTangent = this->regions[regionID].forceTangent;
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			inline cupcfd::geometry::euclidean::EuclideanVector<T,3> CupCfdAoSMesh<I,T,L>::getRegionForceTangent(I regionID)
			{
				return this->regions[regionID].forceTangent;
			}

			template <class I, class T, class L>
			inline cupcfd::error::eCodes CupCfdAoSMesh<I,T,L>::getRegionUVW(I regionID, cupcfd::geometry::euclidean::EuclideanVector<T,3>& uvw)
			{
				uvw = this->regions[regionID].uvw;
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			inline cupcfd::geometry::euclidean::EuclideanVector<T,3> CupCfdAoSMesh<I,T,L>::getRegionUVW(I regionID)
			{
				return this->regions[regionID].uvw;
			}

			template <class I, class T, class L>
			inline cupcfd::error::eCodes CupCfdAoSMesh<I,T,L>::getRegionName(I regionID, std::string& regionName)
			{
				regionName = this->regions[regionID].regionName;
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			inline std::string CupCfdAoSMesh<I,T,L>::getRegionName(I regionID)
			{
				return this->regions[regionID].regionName;
			}

			template <class I, class T, class L>
			inline cupcfd::error::eCodes CupCfdAoSMesh<I,T,L>::getRegionFlux(I regionID, bool * flux)
			{
				*flux = this->regions[regionID].flux;
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			inline bool CupCfdAoSMesh<I,T,L>::getRegionFlux(I regionID)
			{
				return this->regions[regionID].flux;
			}

			template <class I, class T, class L>
			inline cupcfd::error::eCodes CupCfdAoSMesh<I,T,L>::getRegionAdiab(I regionID, bool * adiab)
			{
				*adiab = this->regions[regionID].adiab;
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			inline bool CupCfdAoSMesh<I,T,L>::getRegionAdiab(I regionID)
			{
				return this->regions[regionID].adiab;
			}

			template <class I, class T, class L>
			inline cupcfd::error::eCodes CupCfdAoSMesh<I,T,L>::getRegionR(I regionID, T * r)
			{
				*r = this->regions[regionID].r;
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			inline T CupCfdAoSMesh<I,T,L>::getRegionR(I regionID)
			{
				return this->regions[regionID].r;
			}

			template <class I, class T, class L>
			inline cupcfd::error::eCodes CupCfdAoSMesh<I,T,L>::getRegionT(I regionID, T * t)
			{
				*t = this->regions[regionID].t;
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			inline T CupCfdAoSMesh<I,T,L>::getRegionT(I regionID)
			{
				return this->regions[regionID].t;
			}

			template <class I, class T, class L>
			inline cupcfd::error::eCodes CupCfdAoSMesh<I,T,L>::setRegionType(I regionID, RType type)
			{
				this->regions[regionID].rType = type;
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			inline cupcfd::error::eCodes CupCfdAoSMesh<I,T,L>::setRegionStd(I regionID, bool std)
			{
				this->regions[regionID].std = std;
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			inline cupcfd::error::eCodes CupCfdAoSMesh<I,T,L>::setRegionYLog(I regionID, T yLog)
			{
				this->regions[regionID].ylog = yLog;
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			inline cupcfd::error::eCodes CupCfdAoSMesh<I,T,L>::setRegionELog(I regionID, T eLog)
			{
				this->regions[regionID].elog = eLog;
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			inline cupcfd::error::eCodes CupCfdAoSMesh<I,T,L>::setRegionDensity(I regionID, T density)
			{
				this->regions[regionID].density = density;
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			inline cupcfd::error::eCodes CupCfdAoSMesh<I,T,L>::setRegionTurbKE(I regionID, T turbKE)
			{
				this->regions[regionID].turbKE = turbKE;
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			inline cupcfd::error::eCodes CupCfdAoSMesh<I,T,L>::setRegionTurbDiss(I regionID, T turbDiss)
			{
				this->regions[regionID].turbDiss = turbDiss;
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			inline cupcfd::error::eCodes CupCfdAoSMesh<I,T,L>::setRegionSplvl(I regionID, T splvl)
			{
				this->regions[regionID].splvl = splvl;
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			inline cupcfd::error::eCodes CupCfdAoSMesh<I,T,L>::setRegionDen(I regionID, T den)
			{
				this->regions[regionID].den = den;
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			inline cupcfd::error::eCodes CupCfdAoSMesh<I,T,L>::setRegionForceTangent(I regionID, cupcfd::geometry::euclidean::EuclideanVector<T,3>& forceTangent)
			{
				this->regions[regionID].forceTangent = forceTangent;
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			inline cupcfd::error::eCodes CupCfdAoSMesh<I,T,L>::setRegionUVW(I regionID, cupcfd::geometry::euclidean::EuclideanVector<T,3>& uvw)
			{
				this->regions[regionID].uvw = uvw;
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			inline cupcfd::error::eCodes CupCfdAoSMesh<I,T,L>::setRegionName(I regionID, std::string& regionName)
			{
				this->regions[regionID].regionName = regionName;
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			inline cupcfd::error::eCodes CupCfdAoSMesh<I,T,L>::setRegionFlux(I regionID, bool flux)
			{
				this->regions[regionID].flux = flux;
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			inline cupcfd::error::eCodes CupCfdAoSMesh<I,T,L>::setRegionAdiab(I regionID, bool adiab)
			{
				this->regions[regionID].adiab = adiab;
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			inline cupcfd::error::eCodes CupCfdAoSMesh<I,T,L>::setRegionR(I regionID, T r)
			{
				this->regions[regionID].r = r;
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			inline cupcfd::error::eCodes CupCfdAoSMesh<I,T,L>::setRegionT(I regionID, T t)
			{
				this->regions[regionID].t = t;
				return cupcfd::error::E_SUCCESS;
			}
			
			template <class I, class T, class L>
			cupcfd::error::eCodes CupCfdAoSMesh<I,T,L>::getRegionID(L regionLabel, I * regionID)
			{
				*regionID = this->regionBuildIDToLocalID[regionLabel];;
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			I CupCfdAoSMesh<I,T,L>::getRegionID(L regionLabel)
			{
				return this->regionBuildIDToLocalID[regionLabel];
			}
			
			template <class I, class T, class L>
			inline cupcfd::error::eCodes CupCfdAoSMesh<I,T,L>::getVertexPos(I vertexID, cupcfd::geometry::euclidean::EuclideanPoint<T,3>& pos)
			{
				pos = this->vertices[vertexID].pos;
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			inline cupcfd::geometry::euclidean::EuclideanPoint<T,3> CupCfdAoSMesh<I,T,L>::getVertexPos(I vertexID)
			{
				return this->vertices[vertexID].pos;
			}

			template <class I, class T, class L>
			inline cupcfd::error::eCodes CupCfdAoSMesh<I,T,L>::setVertexPos(I vertexID, cupcfd::geometry::euclidean::EuclideanPoint<T,3>& pos)
			{
				this->vertices[vertexID].pos = pos;
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			inline cupcfd::error::eCodes CupCfdAoSMesh<I,T,L>::getVertexID(L vertexLabel, I * vertexID)
			{
				*vertexID = this->vertexBuildIDToLocalID[vertexLabel];
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			inline I CupCfdAoSMesh<I,T,L>::getVertexID(L vertexLabel)
			{
				return this->vertexBuildIDToLocalID[vertexLabel];
			}					
		}
	}
}

#endif
