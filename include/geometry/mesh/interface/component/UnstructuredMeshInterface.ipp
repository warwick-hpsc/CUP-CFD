/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 * This interface was coded from scratch, but the original basis for the
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
 * Contains header level definitions for the UnstructuredMeshInterface Class
 */

#ifndef CUPCFD_GEOMETRY_UNSTRUCTURED_MESH_INTERFACE_IPP_H
#define CUPCFD_GEOMETRY_UNSTRUCTURED_MESH_INTERFACE_IPP_H

#include <vector>
#include "TriPrism.h"
#include "Tetrahedron.h"
#include "QuadPyramid.h"
#include "Hexahedron.h"

namespace euc = cupcfd::geometry::euclidean;

namespace cupcfd
{
	namespace geometry
	{
		namespace mesh
		{
			template <class M, class I, class T, class L>
			UnstructuredMeshInterface<M,I,T,L>::UnstructuredMeshInterface(cupcfd::comm::Communicator& comm)
			:properties()
			{
				// Setup an empty cell conectivity graph
				this->cellConnGraph = new cupcfd::data_structures::DistributedAdjacencyList<I, I>(comm);

				// Initially unfinalized
				this->finalized = false;
			}
					
			template <class M, class I, class T, class L>
			UnstructuredMeshInterface<M,I,T,L>::~UnstructuredMeshInterface() {
				// Cleanup the cell graph if set
				if(this->cellConnGraph != nullptr) {
					delete cellConnGraph;
				}
			}

			template <class M, class I, class T, class L>
			cupcfd::error::eCodes UnstructuredMeshInterface<M,I,T,L>::getCellCenter(I cellID, cupcfd::geometry::euclidean::EuclideanPoint<T,3>& center) {
				return static_cast<M*>(this)->getCellCenter(cellID, center);
			}

			template <class M, class I, class T, class L>
			cupcfd::geometry::euclidean::EuclideanPoint<T,3> UnstructuredMeshInterface<M,I,T,L>::getCellCenter(I cellID) {
				return static_cast<M*>(this)->getCellCenter(cellID);
			}

			template <class M, class I, class T, class L>
			cupcfd::error::eCodes UnstructuredMeshInterface<M,I,T,L>::getCellVolume(I cellID, T * vol) {
				return static_cast<M*>(this)->getCellVolume(cellID, vol);
			}

			template <class M, class I, class T, class L>
			T UnstructuredMeshInterface<M,I,T,L>::getCellVolume(I cellID) {
				return static_cast<M*>(this)->getCellVolume(cellID);
			}

			template <class M, class I, class T, class L>
			cupcfd::error::eCodes UnstructuredMeshInterface<M,I,T,L>::getCellNFaces(I cellID, I * nFaces) {
				return static_cast<M*>(this)->getCellNFaces(cellID, nFaces);
			}

			template <class M, class I, class T, class L>
			I UnstructuredMeshInterface<M,I,T,L>::getCellNFaces(I cellID) {
				return static_cast<M*>(this)->getCellNFaces(cellID);
			}

			template <class M, class I, class T, class L>
			cupcfd::error::eCodes UnstructuredMeshInterface<M,I,T,L>::getCellStoredNFaces(I cellID, I * nFaces) {
				return static_cast<M*>(this)->getCellStoredNFaces(cellID, nFaces);
			}

			template <class M, class I, class T, class L>
			I UnstructuredMeshInterface<M,I,T,L>::getCellStoredNFaces(I cellID) {
				return static_cast<M*>(this)->getCellStoredNFaces(cellID);
			}

			template <class M, class I, class T, class L>
			cupcfd::error::eCodes UnstructuredMeshInterface<M,I,T,L>::getCellNVertices(I cellID, I * nVertices) {
				return static_cast<M*>(this)->getCellNVertices(cellID, nVertices);
			}

			template <class M, class I, class T, class L>
			I UnstructuredMeshInterface<M,I,T,L>::getCellNVertices(I cellID) {
				return static_cast<M*>(this)->getCellNVertices(cellID);
			}

			template <class M, class I, class T, class L>
			cupcfd::error::eCodes UnstructuredMeshInterface<M,I,T,L>::getCellStoredNVertices(I cellID, I * nVertices) {
				return static_cast<M*>(this)->getCellStoredNVertices(cellID, nVertices);
			}

			template <class M, class I, class T, class L>
			I UnstructuredMeshInterface<M,I,T,L>::getCellStoredNVertices(I cellID) {
				return static_cast<M*>(this)->getCellStoredNVertices(cellID);
			} 

			template <class M, class I, class T, class L>
			cupcfd::error::eCodes UnstructuredMeshInterface<M,I,T,L>::getCellFaceID(I cellID, I cellFaceID, I * faceID) {
				return static_cast<M*>(this)->getCellFaceID(cellID, cellFaceID, faceID);
			}
			
			template <class M, class I, class T, class L>
			I UnstructuredMeshInterface<M,I,T,L>::getCellFaceID(I cellID, I cellFaceID) {
				return static_cast<M*>(this)->getCellFaceID(cellID, cellFaceID);
			}

			template <class M, class I, class T, class L>
			cupcfd::error::eCodes UnstructuredMeshInterface<M,I,T,L>::setCellCenter(I cellID, cupcfd::geometry::euclidean::EuclideanPoint<T,3>& center) {
				return static_cast<M*>(this)->setCellCenter(cellID, center);
			}

			template <class M, class I, class T, class L>
			cupcfd::error::eCodes UnstructuredMeshInterface<M,I,T,L>::setCellVolume(I cellID, T vol) {
				return static_cast<M*>(this)->setCellVolume(cellID, vol);
			}

			template <class M, class I, class T, class L>
			cupcfd::error::eCodes UnstructuredMeshInterface<M,I,T,L>::addCell(L cellLabel,
											   cupcfd::geometry::euclidean::EuclideanPoint<T,3>& center,
											   T vol,
											   bool isLocal) {
				return static_cast<M*>(this)->addCell(cellLabel, center, vol, isLocal);
			}

			template <class M, class I, class T, class L>
			cupcfd::error::eCodes UnstructuredMeshInterface<M,I,T,L>::addCell(L cellLabel, bool isLocal) {
				return static_cast<M*>(this)->addCell(cellLabel, isLocal);
			}

			template <class M, class I, class T, class L>
			cupcfd::error::eCodes UnstructuredMeshInterface<M,I,T,L>::getCellID(L cellLabel, I * cellID) {
				return static_cast<M*>(this)->getCellID(cellLabel, cellID);
			}

			template <class M, class I, class T, class L>
			I UnstructuredMeshInterface<M,I,T,L>::getCellID(L cellLabel) {
				return static_cast<M*>(this)->getCellID(cellLabel);
			}

			template <class M, class I, class T, class L>
			cupcfd::error::eCodes UnstructuredMeshInterface<M,I,T,L>::getFaceNVertices(I faceID, I * nVertices) {
				return static_cast<M*>(this)->getFaceNVertices(faceID, nVertices);
			}
			
			template <class M, class I, class T, class L>
			I UnstructuredMeshInterface<M,I,T,L>::getFaceNVertices(I faceID) {
				return static_cast<M*>(this)->getFaceNVertices(faceID);
			}

			template <class M, class I, class T, class L>
			cupcfd::error::eCodes UnstructuredMeshInterface<M,I,T,L>::getFaceCell1ID(I faceID, I * cellID) {
				return static_cast<M*>(this)->getFaceCell1ID(faceID, cellID);
			}

			template <class M, class I, class T, class L>
			I UnstructuredMeshInterface<M,I,T,L>::getFaceCell1ID(I faceID) {
				return static_cast<M*>(this)->getFaceCell1ID(faceID);
			}

			template <class M, class I, class T, class L>
			cupcfd::error::eCodes UnstructuredMeshInterface<M,I,T,L>::getFaceCell2ID(I faceID, I * cellID) {
				return static_cast<M*>(this)->getFaceCell2ID(faceID, cellID);
			}

			template <class M, class I, class T, class L>
			I UnstructuredMeshInterface<M,I,T,L>::getFaceCell2ID(I faceID) {
				return static_cast<M*>(this)->getFaceCell2ID(faceID);
			}

			template <class M, class I, class T, class L>
			cupcfd::error::eCodes UnstructuredMeshInterface<M,I,T,L>::getFaceLambda(I faceID, T * lambda) {
				return static_cast<M*>(this)->getFaceLambda(faceID, lambda);
			}

			template <class M, class I, class T, class L>
			T UnstructuredMeshInterface<M,I,T,L>::getFaceLambda(I faceID) {
				return static_cast<M*>(this)->getFaceLambda(faceID);
			}

			template <class M, class I, class T, class L>
			cupcfd::error::eCodes UnstructuredMeshInterface<M,I,T,L>::getFaceNorm(I faceID, cupcfd::geometry::euclidean::EuclideanVector<T,3>& norm) {
				return static_cast<M*>(this)->getFaceNorm(faceID, norm);
			}

			template <class M, class I, class T, class L>
			cupcfd::geometry::euclidean::EuclideanVector<T,3> UnstructuredMeshInterface<M,I,T,L>::getFaceNorm(I faceID) {
				return static_cast<M*>(this)->getFaceNorm(faceID);
			}
			
			template <class M, class I, class T, class L>
			cupcfd::error::eCodes UnstructuredMeshInterface<M,I,T,L>::getFaceVertex(I faceID, I faceVertexID, I * vertexID) {
				return static_cast<M*>(this)->getFaceVertex(faceID, faceVertexID, vertexID);
			}

			template <class M, class I, class T, class L>
			I UnstructuredMeshInterface<M,I,T,L>::getFaceVertex(I faceID, I faceVertexID) {
				return static_cast<M*>(this)->getFaceVertex(faceID, faceVertexID);
			}

			template <class M, class I, class T, class L>
			cupcfd::error::eCodes UnstructuredMeshInterface<M,I,T,L>::getFaceCenter(I faceID, cupcfd::geometry::euclidean::EuclideanPoint<T,3>& center) {
				return static_cast<M*>(this)->getFaceCenter(faceID, center);
			}

			template <class M, class I, class T, class L>
			cupcfd::geometry::euclidean::EuclideanPoint<T,3> UnstructuredMeshInterface<M,I,T,L>::getFaceCenter(I faceID) {
				return static_cast<M*>(this)->getFaceCenter(faceID);
			}

			template <class M, class I, class T, class L>
			cupcfd::error::eCodes UnstructuredMeshInterface<M,I,T,L>::getFaceRLencos(I faceID, T * rlencos) {
				return static_cast<M*>(this)->getFaceRLencos(faceID, rlencos);
			}

			template <class M, class I, class T, class L>
			T UnstructuredMeshInterface<M,I,T,L>::getFaceRLencos(I faceID) {
				return static_cast<M*>(this)->getFaceRLencos(faceID);
			}

			template <class M, class I, class T, class L>
			cupcfd::error::eCodes UnstructuredMeshInterface<M,I,T,L>::getFaceArea(I faceID, T * area) {
				return static_cast<M*>(this)->getFaceArea(faceID, area);
			}

			template <class M, class I, class T, class L>
			T UnstructuredMeshInterface<M,I,T,L>::getFaceArea(I faceID) {
				return static_cast<M*>(this)->getFaceArea(faceID);
			}

			template <class M, class I, class T, class L>
			cupcfd::error::eCodes UnstructuredMeshInterface<M,I,T,L>::getFaceXpac(I faceID, cupcfd::geometry::euclidean::EuclideanPoint<T,3>& xpac) {
				return static_cast<M*>(this)->getFaceXpac(faceID, xpac);
			}

			template <class M, class I, class T, class L>
			cupcfd::geometry::euclidean::EuclideanPoint<T,3> UnstructuredMeshInterface<M,I,T,L>::getFaceXpac(I faceID) {
				return static_cast<M*>(this)->getFaceXpac(faceID);
			}

			template <class M, class I, class T, class L>
			cupcfd::error::eCodes UnstructuredMeshInterface<M,I,T,L>::getFaceXnac(I faceID, cupcfd::geometry::euclidean::EuclideanPoint<T,3>& xnac) {
				return static_cast<M*>(this)->getFaceXnac(faceID, xnac);
			}

			template <class M, class I, class T, class L>
			cupcfd::geometry::euclidean::EuclideanPoint<T,3> UnstructuredMeshInterface<M,I,T,L>::getFaceXnac(I faceID) {
				return static_cast<M*>(this)->getFaceXnac(faceID);
			}

			template <class M, class I, class T, class L>
			cupcfd::error::eCodes UnstructuredMeshInterface<M,I,T,L>::getFaceIsBoundary(I faceID, bool * result) {
				return static_cast<M*>(this)->getFaceIsBoundary(faceID, result);
			}

			template <class M, class I, class T, class L>
			bool UnstructuredMeshInterface<M,I,T,L>::getFaceIsBoundary(I faceID) {
				return static_cast<M*>(this)->getFaceIsBoundary(faceID);
			}

			template <class M, class I, class T, class L>
			cupcfd::error::eCodes UnstructuredMeshInterface<M,I,T,L>::getFaceBoundaryID(I faceID, I * boundaryID) {
				return static_cast<M*>(this)->getFaceBoundaryID(faceID, boundaryID);
			}

			template <class M, class I, class T, class L>
			I UnstructuredMeshInterface<M,I,T,L>::getFaceBoundaryID(I faceID) {
				return static_cast<M*>(this)->getFaceBoundaryID(faceID);
			}

			template <class M, class I, class T, class L>
			cupcfd::error::eCodes UnstructuredMeshInterface<M,I,T,L>::setFaceCell1ID(I faceID, I cellID) {
				return static_cast<M*>(this)->setFaceCell1ID(faceID, cellID);
			}

			template <class M, class I, class T, class L>
			cupcfd::error::eCodes UnstructuredMeshInterface<M,I,T,L>::setFaceCell2ID(I faceID, I cellID) {
				return static_cast<M*>(this)->setFaceCell2ID(faceID, cellID);
			}

			template <class M, class I, class T, class L>
			cupcfd::error::eCodes UnstructuredMeshInterface<M,I,T,L>::setFaceLambda(I faceID, T lambda) {
				return static_cast<M*>(this)->setFaceLambda(faceID, lambda);
			}

			template <class M, class I, class T, class L>
			cupcfd::error::eCodes UnstructuredMeshInterface<M,I,T,L>::setFaceNorm(I faceID, cupcfd::geometry::euclidean::EuclideanVector<T,3>& norm) {
				return static_cast<M*>(this)->setFaceNorm(faceID, norm);
			}

			template <class M, class I, class T, class L>
			cupcfd::error::eCodes UnstructuredMeshInterface<M,I,T,L>::setFaceVertex(I faceID, I faceVertexID, I vertexID) {
				return static_cast<M*>(this)->setFaceVertex(faceID, faceVertexID, vertexID);
			}

			template <class M, class I, class T, class L>
			cupcfd::error::eCodes UnstructuredMeshInterface<M,I,T,L>::setFaceCenter(I faceID, cupcfd::geometry::euclidean::EuclideanPoint<T,3>& center) {
				return static_cast<M*>(this)->setFaceCenter(faceID, center);
			}

			template <class M, class I, class T, class L>
			cupcfd::error::eCodes UnstructuredMeshInterface<M,I,T,L>::setFaceRLencos(I faceID, T rlencos) {
				return static_cast<M*>(this)->setFaceRLencos(faceID, rlencos);
			}

			template <class M, class I, class T, class L>
			cupcfd::error::eCodes UnstructuredMeshInterface<M,I,T,L>::setFaceArea(I faceID, T area) {
				return static_cast<M*>(this)->setFaceArea(faceID, area);
			}

			template <class M, class I, class T, class L>
			cupcfd::error::eCodes UnstructuredMeshInterface<M,I,T,L>::setFaceXpac(I faceID, cupcfd::geometry::euclidean::EuclideanPoint<T,3>& xpac) {
				return static_cast<M*>(this)->setFaceXpac(faceID, xpac);
			}

			template <class M, class I, class T, class L>
			cupcfd::error::eCodes UnstructuredMeshInterface<M,I,T,L>::setFaceXnac(I faceID, cupcfd::geometry::euclidean::EuclideanPoint<T,3>& xnac) {
				return static_cast<M*>(this)->setFaceXnac(faceID, xnac);
			}

			template <class M, class I, class T, class L>
			cupcfd::error::eCodes UnstructuredMeshInterface<M,I,T,L>::setFaceBoundaryID(I faceID, I boundaryID) {
				return static_cast<M*>(this)->setFaceBoundaryID(faceID, boundaryID);
			}

			template <class M, class I, class T, class L>
			cupcfd::error::eCodes UnstructuredMeshInterface<M,I,T,L>::addFace(
						L faceLabel,
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
						T area) {
				return static_cast<M*>(this)->addFace(faceLabel, cell1Label, cell2OrBoundaryLabel, isBoundary, lambda,
													  norm, vertexLabels, nVertexLabels, center, xpac, xnac, rlencos, area);
			}

			template <class M, class I, class T, class L>
			cupcfd::error::eCodes UnstructuredMeshInterface<M,I,T,L>::addFace(
						L faceLabel,
						L cell1Label,
						L cell2OrBoundaryLabel,
						bool isBoundary,
						L * vertexLabels, I nVertexLabels) {
				return static_cast<M*>(this)->addFace(faceLabel, cell1Label, cell2OrBoundaryLabel, isBoundary, vertexLabels, nVertexLabels);
			}

			template <class M, class I, class T, class L>
			cupcfd::error::eCodes UnstructuredMeshInterface<M,I,T,L>::getFaceID(L faceLabel, I * faceID) {
				return static_cast<M*>(this)->getFaceID(faceLabel, faceID);
			}

			template <class M, class I, class T, class L>
			I UnstructuredMeshInterface<M,I,T,L>::getFaceID(L faceLabel) {
				return static_cast<M*>(this)->getFaceID(faceLabel);
			}

			template <class M, class I, class T, class L>
			cupcfd::error::eCodes UnstructuredMeshInterface<M,I,T,L>::getBoundaryFaceID(I boundaryID, I * faceID) {
				return static_cast<M*>(this)->getBoundaryFaceID(boundaryID, faceID);
			}

			template <class M, class I, class T, class L>
			I UnstructuredMeshInterface<M,I,T,L>::getBoundaryFaceID(I boundaryID) {
				return static_cast<M*>(this)->getBoundaryFaceID(boundaryID);
			}

			template <class M, class I, class T, class L>
			cupcfd::error::eCodes UnstructuredMeshInterface<M,I,T,L>::getBoundaryVertex(I boundaryID, I boundaryVertexID, I * vertexID) {
				return static_cast<M*>(this)->getBoundaryVertex(boundaryID, boundaryVertexID, vertexID);
			}

			template <class M, class I, class T, class L>
			I UnstructuredMeshInterface<M,I,T,L>::getBoundaryVertex(I boundaryID, I boundaryVertexID) {
				return static_cast<M*>(this)->getBoundaryVertex(boundaryID, boundaryVertexID);
			}

			template <class M, class I, class T, class L>
			cupcfd::error::eCodes UnstructuredMeshInterface<M,I,T,L>::getBoundaryDistance(I boundaryID, T * distance) {
				return static_cast<M*>(this)->getBoundaryDistance(boundaryID, distance);
			}

			template <class M, class I, class T, class L>
			T UnstructuredMeshInterface<M,I,T,L>::getBoundaryDistance(I boundaryID) {
				return static_cast<M*>(this)->getBoundaryDistance(boundaryID);
			}

			template <class M, class I, class T, class L>
			cupcfd::error::eCodes UnstructuredMeshInterface<M,I,T,L>::getBoundaryYPlus(I boundaryID, T * yPlus) {
				return static_cast<M*>(this)->getBoundaryYPlus(boundaryID, yPlus);
			}

			template <class M, class I, class T, class L>
			T UnstructuredMeshInterface<M,I,T,L>::getBoundaryYPlus(I boundaryID) {
				return static_cast<M*>(this)->getBoundaryYPlus(boundaryID);
			}

			template <class M, class I, class T, class L>
			cupcfd::error::eCodes UnstructuredMeshInterface<M,I,T,L>::getBoundaryUPlus(I boundaryID, T * uPlus) {
				return static_cast<M*>(this)->getBoundaryUPlus(boundaryID, uPlus);
			}

			template <class M, class I, class T, class L>
			T UnstructuredMeshInterface<M,I,T,L>::getBoundaryUPlus(I boundaryID) {
				return static_cast<M*>(this)->getBoundaryUPlus(boundaryID);
			}

			template <class M, class I, class T, class L>
			cupcfd::error::eCodes UnstructuredMeshInterface<M,I,T,L>::getBoundaryRegionID(I boundaryID, I * regionID) {
				return static_cast<M*>(this)->getBoundaryRegion(boundaryID, regionID);
			}

			template <class M, class I, class T, class L>
			I UnstructuredMeshInterface<M,I,T,L>::getBoundaryRegionID(I boundaryID) {
				return static_cast<M*>(this)->getBoundaryRegionID(boundaryID);
			}

			template <class M, class I, class T, class L>
			cupcfd::error::eCodes UnstructuredMeshInterface<M,I,T,L>::getBoundaryShear(I boundaryID, cupcfd::geometry::euclidean::EuclideanVector<T,3>& shear) {
				return static_cast<M*>(this)->getBoundaryShear(boundaryID, shear);
			}

			template <class M, class I, class T, class L>
			cupcfd::geometry::euclidean::EuclideanVector<T,3> UnstructuredMeshInterface<M,I,T,L>::getBoundaryShear(I boundaryID) {
				return static_cast<M*>(this)->getBoundaryShear(boundaryID);
			}

			template <class M, class I, class T, class L>
			cupcfd::error::eCodes UnstructuredMeshInterface<M,I,T,L>::getBoundaryQ(I boundaryID, T * q) {
				return static_cast<M*>(this)->getBoundaryQ(boundaryID, q);
			}

			template <class M, class I, class T, class L>
			T UnstructuredMeshInterface<M,I,T,L>::getBoundaryQ(I boundaryID) {
				return static_cast<M*>(this)->getBoundaryQ(boundaryID);
			}

			template <class M, class I, class T, class L>
			cupcfd::error::eCodes UnstructuredMeshInterface<M,I,T,L>::getBoundaryH(I boundaryID, T * h) {
				return static_cast<M*>(this)->getBoundaryH(boundaryID, h);
			}

			template <class M, class I, class T, class L>
			T UnstructuredMeshInterface<M,I,T,L>::getBoundaryH(I boundaryID) {
				return static_cast<M*>(this)->getBoundaryH(boundaryID);
			}

			template <class M, class I, class T, class L>
			cupcfd::error::eCodes UnstructuredMeshInterface<M,I,T,L>::getBoundaryT(I boundaryID, T * t) {
				return static_cast<M*>(this)->getBoundaryT(boundaryID, t);
			}

			template <class M, class I, class T, class L>
			T UnstructuredMeshInterface<M,I,T,L>::getBoundaryT(I boundaryID) {
				return static_cast<M*>(this)->getBoundaryT(boundaryID);
			}

			template <class M, class I, class T, class L>
			cupcfd::error::eCodes UnstructuredMeshInterface<M,I,T,L>::setBoundaryFaceID(I boundaryID, I faceID) {
				return static_cast<M*>(this)->setBoundaryFaceID(boundaryID, faceID);
			}

			template <class M, class I, class T, class L>
			cupcfd::error::eCodes UnstructuredMeshInterface<M,I,T,L>::setBoundaryVertex(I boundaryID, I boundaryVertexID, I vertexID) {
				return static_cast<M*>(this)->setBoundaryVertex(boundaryID, boundaryVertexID, vertexID);
			}

			template <class M, class I, class T, class L>
			cupcfd::error::eCodes UnstructuredMeshInterface<M,I,T,L>::setBoundaryDistance(I boundaryID, T distance) {
				return static_cast<M*>(this)->setBoundaryDistance(boundaryID, distance);
			}

			template <class M, class I, class T, class L>
			cupcfd::error::eCodes UnstructuredMeshInterface<M,I,T,L>::setBoundaryYPlus(I boundaryID, T yPlus) {
				return static_cast<M*>(this)->setBoundaryYPlus(boundaryID, yPlus);
			}

			template <class M, class I, class T, class L>
			cupcfd::error::eCodes UnstructuredMeshInterface<M,I,T,L>::setBoundaryUPlus(I boundaryID, T uPlus) {
				return static_cast<M*>(this)->setBoundaryUPlus(boundaryID, uPlus);
			}

			template <class M, class I, class T, class L>
			cupcfd::error::eCodes UnstructuredMeshInterface<M,I,T,L>::setBoundaryRegionID(I boundaryID, I regionID) {
				return static_cast<M*>(this)->setBoundaryRegionID(boundaryID, regionID);
			}

			template <class M, class I, class T, class L>
			cupcfd::error::eCodes UnstructuredMeshInterface<M,I,T,L>::setBoundaryShear(I boundaryID, cupcfd::geometry::euclidean::EuclideanVector<T,3>& shear) {
				return static_cast<M*>(this)->setBoundaryShear(boundaryID, shear);
			}

			template <class M, class I, class T, class L>
			cupcfd::error::eCodes UnstructuredMeshInterface<M,I,T,L>::setBoundaryQ(I boundaryID, T q) {
				return static_cast<M*>(this)->setBoundaryQ(boundaryID, q);
			}

			template <class M, class I, class T, class L>
			cupcfd::error::eCodes UnstructuredMeshInterface<M,I,T,L>::setBoundaryH(I boundaryID, T h) {
				return static_cast<M*>(this)->setBoundaryH(boundaryID, h);
			}

			template <class M, class I, class T, class L>
			cupcfd::error::eCodes UnstructuredMeshInterface<M,I,T,L>::setBoundaryT(I boundaryID, T t) {
				return static_cast<M*>(this)->setBoundaryT(boundaryID, t);
			}

			template <class M, class I, class T, class L>
			cupcfd::error::eCodes UnstructuredMeshInterface<M,I,T,L>::addBoundary(
					L boundaryLabel,
					L regionLabel,
					L * vertexLabels, I nVertexLabels,
					T distance) {
				return static_cast<M*>(this)->addBoundary(boundaryLabel, regionLabel, vertexLabels, nVertexLabels, distance);
			}

			template <class M, class I, class T, class L>
			cupcfd::error::eCodes UnstructuredMeshInterface<M,I,T,L>::addBoundary(
					L boundaryLabel,
					L regionLabel,
					L * vertexLabels, I nVertexLabels) {
				return static_cast<M*>(this)->addBoundary(boundaryLabel, regionLabel, vertexLabels, nVertexLabels);
			}

			template <class M, class I, class T, class L>
			cupcfd::error::eCodes UnstructuredMeshInterface<M,I,T,L>::getBoundaryID(L boundaryLabel, I * boundaryID) {
				return static_cast<M*>(this)->getBoundaryID(boundaryLabel, boundaryID);
			}

			template <class M, class I, class T, class L>
			I UnstructuredMeshInterface<M,I,T,L>::getBoundaryID(L boundaryLabel) {
				return static_cast<M*>(this)->getBoundaryID(boundaryLabel);
			}

			template <class M, class I, class T, class L>
			cupcfd::error::eCodes UnstructuredMeshInterface<M,I,T,L>::getRegionType(I regionID, RType * type) {
				return static_cast<M*>(this)->getRegionType(regionID, type);
			}

			template <class M, class I, class T, class L>
			RType UnstructuredMeshInterface<M,I,T,L>::getRegionType(I regionID) {
				return static_cast<M*>(this)->getRegionType(regionID);
			}

			template <class M, class I, class T, class L>
			cupcfd::error::eCodes UnstructuredMeshInterface<M,I,T,L>::getRegionStd(I regionID, bool * std) {
				return static_cast<M*>(this)->getRegionStd(regionID, std);
			}

			template <class M, class I, class T, class L>
			bool UnstructuredMeshInterface<M,I,T,L>::getRegionStd(I regionID) {
				return static_cast<M*>(this)->getRegionStd(regionID);
			}

			template <class M, class I, class T, class L>
			cupcfd::error::eCodes UnstructuredMeshInterface<M,I,T,L>::getRegionYLog(I regionID, T * yLog) {
				return static_cast<M*>(this)->getRegionYLog(regionID, yLog);
			}
			
			template <class M, class I, class T, class L>
			T UnstructuredMeshInterface<M,I,T,L>::getRegionYLog(I regionID) {
				return static_cast<M*>(this)->getRegionYLog(regionID);
			}

			template <class M, class I, class T, class L>
			cupcfd::error::eCodes UnstructuredMeshInterface<M,I,T,L>::getRegionELog(I regionID, T * eLog) {
				return static_cast<M*>(this)->getRegionELog(regionID, eLog);
			}

			template <class M, class I, class T, class L>
			T UnstructuredMeshInterface<M,I,T,L>::getRegionELog(I regionID) {
				return static_cast<M*>(this)->getRegionELog(regionID);
			}

			template <class M, class I, class T, class L>
			cupcfd::error::eCodes UnstructuredMeshInterface<M,I,T,L>::getRegionDensity(I regionID, T * density) {
				return static_cast<M*>(this)->getRegionDensity(regionID, density);
			}

			template <class M, class I, class T, class L>
			T UnstructuredMeshInterface<M,I,T,L>::getRegionDensity(I regionID) {
				return static_cast<M*>(this)->getRegionDensity(regionID);
			}

			template <class M, class I, class T, class L>
			cupcfd::error::eCodes UnstructuredMeshInterface<M,I,T,L>::getRegionTurbKE(I regionID, T * turbKE) {
				return static_cast<M*>(this)->getRegionTurbKE(regionID, turbKE);
			}

			template <class M, class I, class T, class L>
			T UnstructuredMeshInterface<M,I,T,L>::getRegionTurbKE(I regionID) {
				return static_cast<M*>(this)->getRegionTurbKE(regionID);
			}

			template <class M, class I, class T, class L>
			cupcfd::error::eCodes UnstructuredMeshInterface<M,I,T,L>::getRegionTurbDiss(I regionID, T * turbDiss) {
				return static_cast<M*>(this)->getRegionTurbDiss(regionID, turbDiss);
			}
			
			template <class M, class I, class T, class L>
			T UnstructuredMeshInterface<M,I,T,L>::getRegionTurbDiss(I regionID) {
				return static_cast<M*>(this)->getRegionTurbDiss(regionID);
			}

			template <class M, class I, class T, class L>
			cupcfd::error::eCodes UnstructuredMeshInterface<M,I,T,L>::getRegionSplvl(I regionID, T * splvl) {
				return static_cast<M*>(this)->getRegionSplvl(regionID, splvl);
			}

			template <class M, class I, class T, class L>
			T UnstructuredMeshInterface<M,I,T,L>::getRegionSplvl(I regionID) {
				return static_cast<M*>(this)->getRegionSplvl(regionID);
			}

			template <class M, class I, class T, class L>
			cupcfd::error::eCodes UnstructuredMeshInterface<M,I,T,L>::getRegionDen(I regionID, T * den) {
				return static_cast<M*>(this)->getRegionDen(regionID, den);
			}

			template <class M, class I, class T, class L>
			T UnstructuredMeshInterface<M,I,T,L>::getRegionDen(I regionID) {
				return static_cast<M*>(this)->getRegionDen(regionID);
			}

			template <class M, class I, class T, class L>
			cupcfd::error::eCodes UnstructuredMeshInterface<M,I,T,L>::getRegionForceTangent(I regionID, cupcfd::geometry::euclidean::EuclideanVector<T,3>& forceTangent) {
				return static_cast<M*>(this)->getRegionForceTangent(regionID, forceTangent);
			}

			template <class M, class I, class T, class L>
			cupcfd::geometry::euclidean::EuclideanVector<T,3> UnstructuredMeshInterface<M,I,T,L>::getRegionForceTangent(I regionID) {
				return static_cast<M*>(this)->getRegionForceTangent(regionID);
			}

			template <class M, class I, class T, class L>
			cupcfd::error::eCodes UnstructuredMeshInterface<M,I,T,L>::getRegionUVW(I regionID, cupcfd::geometry::euclidean::EuclideanVector<T,3>& uvw) {
				return static_cast<M*>(this)->getRegionUVW(regionID, uvw);
			}

			template <class M, class I, class T, class L>
			cupcfd::geometry::euclidean::EuclideanVector<T,3> UnstructuredMeshInterface<M,I,T,L>::getRegionUVW(I regionID) {
				return static_cast<M*>(this)->getRegionUVW(regionID);
			}

			template <class M, class I, class T, class L>
			cupcfd::error::eCodes UnstructuredMeshInterface<M,I,T,L>::getRegionName(I regionID, std::string& regionName) {
				return static_cast<M*>(this)->getRegionName(regionID, regionName);
			}

			template <class M, class I, class T, class L>
			std::string UnstructuredMeshInterface<M,I,T,L>::getRegionName(I regionID) {
				return static_cast<M*>(this)->getRegionName(regionID);
			}

			template <class M, class I, class T, class L>
			cupcfd::error::eCodes UnstructuredMeshInterface<M,I,T,L>::getRegionFlux(I regionID, bool * flux) {
				return static_cast<M*>(this)->getRegionFlux(regionID, flux);
			}

			template <class M, class I, class T, class L>
			bool UnstructuredMeshInterface<M,I,T,L>::getRegionFlux(I regionID) {
				return static_cast<M*>(this)->getRegionFlux(regionID);
			}

			template <class M, class I, class T, class L>
			cupcfd::error::eCodes UnstructuredMeshInterface<M,I,T,L>::getRegionAdiab(I regionID, bool * adiab) {
				return static_cast<M*>(this)->getRegionAdiab(regionID, adiab);
			}

			template <class M, class I, class T, class L>
			bool UnstructuredMeshInterface<M,I,T,L>::getRegionAdiab(I regionID) {
				return static_cast<M*>(this)->getRegionAdiab(regionID);
			}

			template <class M, class I, class T, class L>
			cupcfd::error::eCodes UnstructuredMeshInterface<M,I,T,L>::getRegionR(I regionID, T * r) {
				return static_cast<M*>(this)->getRegionR(regionID, r);
			}

			template <class M, class I, class T, class L>
			T UnstructuredMeshInterface<M,I,T,L>::getRegionR(I regionID) {
				return static_cast<M*>(this)->getRegionR(regionID);
			}

			template <class M, class I, class T, class L>
			cupcfd::error::eCodes UnstructuredMeshInterface<M,I,T,L>::getRegionT(I regionID, T * t) {
				return static_cast<M*>(this)->getRegionT(regionID, t);
			}

			template <class M, class I, class T, class L>
			T UnstructuredMeshInterface<M,I,T,L>::getRegionT(I regionID) {
				return static_cast<M*>(this)->getRegionT(regionID);
			}

			template <class M, class I, class T, class L>
			cupcfd::error::eCodes UnstructuredMeshInterface<M,I,T,L>::setRegionType(I regionID, RType type) {
				return static_cast<M*>(this)->setRegionType(regionID, type);
			}

			template <class M, class I, class T, class L>
			cupcfd::error::eCodes UnstructuredMeshInterface<M,I,T,L>::setRegionStd(I regionID, bool std) {
				return static_cast<M*>(this)->setRegionStd(regionID, std);
			}

			template <class M, class I, class T, class L>
			cupcfd::error::eCodes UnstructuredMeshInterface<M,I,T,L>::setRegionYLog(I regionID, T yLog) {
				return static_cast<M*>(this)->setRegionYLog(regionID, yLog);
			}

			template <class M, class I, class T, class L>
			cupcfd::error::eCodes UnstructuredMeshInterface<M,I,T,L>::setRegionELog(I regionID, T eLog) {
				return static_cast<M*>(this)->setRegionELog(regionID, eLog);
			}

			template <class M, class I, class T, class L>
			cupcfd::error::eCodes UnstructuredMeshInterface<M,I,T,L>::setRegionDensity(I regionID, T density) {
				return static_cast<M*>(this)->setRegionDensity(regionID, density);
			}

			template <class M, class I, class T, class L>
			cupcfd::error::eCodes UnstructuredMeshInterface<M,I,T,L>::setRegionTurbKE(I regionID, T turbKE) {
				return static_cast<M*>(this)->setRegionTurbKE(regionID, turbKE);
			}

			template <class M, class I, class T, class L>
			cupcfd::error::eCodes UnstructuredMeshInterface<M,I,T,L>::setRegionTurbDiss(I regionID, T turbDiss) {
				return static_cast<M*>(this)->setRegionTurbDiss(regionID, turbDiss);
			}

			template <class M, class I, class T, class L>
			cupcfd::error::eCodes UnstructuredMeshInterface<M,I,T,L>::setRegionSplvl(I regionID, T splvl) {
				return static_cast<M*>(this)->setRegionSplvl(regionID, splvl);
			}

			template <class M, class I, class T, class L>
			cupcfd::error::eCodes UnstructuredMeshInterface<M,I,T,L>::setRegionDen(I regionID, T den) {
				return static_cast<M*>(this)->setRegionDen(regionID, den);
			}

			template <class M, class I, class T, class L>
			cupcfd::error::eCodes UnstructuredMeshInterface<M,I,T,L>::setRegionForceTangent(I regionID, cupcfd::geometry::euclidean::EuclideanVector<T,3>& forceTangent) {
				return static_cast<M*>(this)->setRegionForceTangent(regionID, forceTangent);
			}

			template <class M, class I, class T, class L>
			cupcfd::error::eCodes UnstructuredMeshInterface<M,I,T,L>::setRegionUVW(I regionID, cupcfd::geometry::euclidean::EuclideanVector<T,3>& uvw) {
				return static_cast<M*>(this)->setRegionUVW(regionID, uvw);
			}

			template <class M, class I, class T, class L>
			cupcfd::error::eCodes UnstructuredMeshInterface<M,I,T,L>::setRegionName(I regionID, std::string& regionName) {
				return static_cast<M*>(this)->setRegionName(regionID, regionName);
			}

			template <class M, class I, class T, class L>
			cupcfd::error::eCodes UnstructuredMeshInterface<M,I,T,L>::setRegionFlux(I regionID, bool flux) {
				return static_cast<M*>(this)->setRegionFlux(regionID, flux);
			}

			template <class M, class I, class T, class L>
			cupcfd::error::eCodes UnstructuredMeshInterface<M,I,T,L>::setRegionAdiab(I regionID, bool adiab) {
				return static_cast<M*>(this)->setRegionAdiab(regionID, adiab);
			}

			template <class M, class I, class T, class L>
			cupcfd::error::eCodes UnstructuredMeshInterface<M,I,T,L>::setRegionR(I regionID, T r) {
				return static_cast<M*>(this)->setRegionR(regionID, r);
			}

			template <class M, class I, class T, class L>
			cupcfd::error::eCodes UnstructuredMeshInterface<M,I,T,L>::setRegionT(I regionID, T t) {
				return static_cast<M*>(this)->setRegionT(regionID, t);
			}

			template <class M, class I, class T, class L>
			cupcfd::error::eCodes UnstructuredMeshInterface<M,I,T,L>::addRegion(
								L regionLabel,
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
								std::string& regionName) {
				return static_cast<M*>(this)->addRegion(regionLabel, type, std, flux, adiab, ylog, elog,
														density, turbKE, turbDiss, splvl, den, r, t, forceTangent, uvw, regionName);
			}

			template <class M, class I, class T, class L>
			cupcfd::error::eCodes UnstructuredMeshInterface<M,I,T,L>::addRegion(
								L regionLabel,
								std::string& regionName) {
				return static_cast<M*>(this)->addRegion(regionLabel, regionName);
			}

			template <class M, class I, class T, class L>
			cupcfd::error::eCodes UnstructuredMeshInterface<M,I,T,L>::getRegionID(L regionLabel, I * regionID) {
				return static_cast<M*>(this)->getRegionID(regionLabel, regionID);
			}

			template <class M, class I, class T, class L>
			I UnstructuredMeshInterface<M,I,T,L>::getRegionID(L regionLabel) {
				return static_cast<M*>(this)->getRegionID(regionLabel);
			}

			template <class M, class I, class T, class L>
			cupcfd::error::eCodes UnstructuredMeshInterface<M,I,T,L>::getVertexPos(I vertexID, cupcfd::geometry::euclidean::EuclideanPoint<T,3>& pos) {
				return static_cast<M*>(this)->getVertexPos(vertexID, pos);
			}

			template <class M, class I, class T, class L>
			cupcfd::geometry::euclidean::EuclideanPoint<T,3> UnstructuredMeshInterface<M,I,T,L>::getVertexPos(I vertexID) {
				return static_cast<M*>(this)->getVertexPos(vertexID);
			}

			template <class M, class I, class T, class L>
			cupcfd::error::eCodes UnstructuredMeshInterface<M,I,T,L>::setVertexPos(I vertexLabel, cupcfd::geometry::euclidean::EuclideanPoint<T,3>& pos) {
				return static_cast<M*>(this)->setVertexPos(vertexLabel, pos);
			}

			template <class M, class I, class T, class L>
			cupcfd::error::eCodes UnstructuredMeshInterface<M,I,T,L>::addVertex(L vertexLabel, cupcfd::geometry::euclidean::EuclideanPoint<T,3>& pos) {
				return static_cast<M*>(this)->addVertex(vertexLabel, pos);
			}

			template <class M, class I, class T, class L>
			cupcfd::error::eCodes UnstructuredMeshInterface<M,I,T,L>::getVertexID(L vertexLabel, I * vertexID) {
				return static_cast<M*>(this)->getVertexID(vertexLabel, vertexID);
			}

			template <class M, class I, class T, class L>
			I UnstructuredMeshInterface<M,I,T,L>::getVertexID(L vertexLabel) {
				return static_cast<M*>(this)->getVertexID(vertexLabel);
			}

			template <class M, class I, class T, class L>
			cupcfd::geometry::shapes::PolyhedronType UnstructuredMeshInterface<M,I,T,L>::getCellPolyhedronType(I cellID) {
				I nVertices = this->getCellNVertices(cellID);
				I nFaces = this->getCellNFaces(cellID);

				// Get the Cell Type
				return cupcfd::geometry::shapes::findPolyhedronType(nVertices, nFaces);
			}
			

			template <class M, class I, class T, class L>
			cupcfd::error::eCodes UnstructuredMeshInterface<M,I,T,L>::buildPolyhedron(I cellID, cupcfd::geometry::shapes::TriPrism<T> ** shape) {
				// ToDo: We don't currently enforce any ordering (clockwise/anti-clockwise) at the interface level,
				// although we are going to expect that the vertices in order are connected by edges
				
				// For now, we will compute the relative positions of the points, but it
				// would be far more efficient if we control the ordering so that it is just known
				// or at the very least (compute their relative positions only once in the UnstructuredMesh)
				// This is complex, prone to error and inefficient otherwise
				
				// Error Check: Check that the provided cell ID is actually a TriPrism
				cupcfd::geometry::shapes::PolyhedronType type = this->getCellPolyhedronType(cellID);
								
				if(type != cupcfd::geometry::shapes::POLYHEDRON_TRIPRISM) {
					DEBUGGABLE_ERROR; return cupcfd::error::E_GEOMETRY_POLYHEDRON_MISMATCH;
				}
				
				// ToDo: Error Check - Check Cell is a locally owned cell
							
				// Since we validated it is a triprism, we can expect there are 5 faces
				// Retrieve the Face Data from the mesh for the cell
				I cellLocalFaceID[5];
				for(I i = 0; i  < 5; i++) {
					this->getCellFaceID(cellID, i, cellLocalFaceID + i);
				}
				
				// Get the number of vertices for each face, and determine which are the triangular faces
				I faceNVertices[5];
				I triFaceLabels[2];
				uint ptr = 0;
				for(I i = 0; i  < 5; i++) {
					faceNVertices[i] = this->getFaceNVertices(cellLocalFaceID[i]);
					if (faceNVertices[i] == 3) {
						triFaceLabels[ptr] = cellLocalFaceID[i];
						ptr++;
					}
				}
				if(ptr != 2) {
					DEBUGGABLE_ERROR; return cupcfd::error::E_GEOMETRY_NFACE_MISMATCH;
				}
				
				// Number of vertices including duplicates
				uint nVertDup = 18; // (3+3+4+4+4)
				
				// Get the vertex IDs for each face
				I faceVertexIDs[nVertDup];
				ptr = 0;
				for(I i = 0; i  < 5; i++) {
					for(I j = 0; j < faceNVertices[i]; j++) {
						faceVertexIDs[ptr] = this->getFaceVertex(cellLocalFaceID[i], j);
						ptr++;
					}
				}
				if(ptr != nVertDup) {
					DEBUGGABLE_ERROR; return cupcfd::error::E_GEOMETRY_NVERT_MISMATCH;
				}
				
				// Reconstruct the edges
				std::vector<I> edge1;
				std::vector<I> edge2;
				I tmpEdge1;
				I tmpEdge2;
				bool found;
				// Loop over faces
				ptr = 0;
				for(I i = 0; i < 5; i++) {
					// Loop over vertices within face (bar last)
					for(I j = 0; j < faceNVertices[i] - 1; j++) {
						// Get two edge vertices
						tmpEdge1 = faceVertexIDs[ptr];
						tmpEdge2 = faceVertexIDs[ptr+1];
						ptr++;

						// Search 
						found = false;
						for(I k = 0; k < (I)edge1.size(); k++) {
							if((edge1[k] == tmpEdge1 && edge2[k] == tmpEdge2) || (edge2[k] == tmpEdge1 && edge1[k] == tmpEdge2)) {
								found = true;
							}
						}
						if(!found) {
							edge1.push_back(tmpEdge1);
							edge2.push_back(tmpEdge2);
						}
					}
					
					// Last edge that loops around
					tmpEdge1 = faceVertexIDs[ptr];
					tmpEdge2 = faceVertexIDs[(ptr - faceNVertices[i]) + 1];
					ptr++;
					
					found = false;
					for(I k = 0; k < (I)edge1.size(); k++) {
						if((edge1[k] == tmpEdge1 && edge2[k] == tmpEdge2) || (edge2[k] == tmpEdge1 && edge1[k] == tmpEdge2)) {
							found = true;
						}
					}
					if(!found) {
						edge1.push_back(tmpEdge1);
						edge2.push_back(tmpEdge2);
					}
				}
				
				if(edge1.size() != (nVertDup/2)) {
					DEBUGGABLE_ERROR; return cupcfd::error::E_GEOMETRY_NEDGE_MISMATCH;
				}
				
				I tf;
			   	I tlb;
			   	I trb;
			    I bf;
			    I blb;
			   	I brb;
			   	
				// Determine the relative positions of the vertices amongst one another, orient them
				// For Triprism, figuring out the orientations of the two triangle faces should be sufficient
				// Compute the vector from the cell center to a point on each face
				// If the vectors are in the same direction as the normals then we know the ordering is clockwise from
				// internal, else it is anticlockwise
				
				// Face 1
				cupcfd::geometry::euclidean::EuclideanPoint<T,3> center = this->getCellCenter(cellID);
								
				I triFaceA = this->getFaceVertex(triFaceLabels[0], 0);
				I triFaceB = this->getFaceVertex(triFaceLabels[0], 1);
				I triFaceC = this->getFaceVertex(triFaceLabels[0], 2);
				
				cupcfd::geometry::euclidean::EuclideanPoint<T,3> triFaceAPos = this->getVertexPos(triFaceA);
				cupcfd::geometry::euclidean::EuclideanPoint<T,3> triFaceBPos = this->getVertexPos(triFaceB);
				cupcfd::geometry::euclidean::EuclideanPoint<T,3> triFaceCPos = this->getVertexPos(triFaceC);
				
				cupcfd::geometry::euclidean::EuclideanVector<T,3> centerFace1 = center - triFaceAPos;
				
				cupcfd::geometry::shapes::Triangle3D<T> face1(triFaceAPos, triFaceBPos, triFaceCPos);
				cupcfd::geometry::euclidean::EuclideanVector<T,3> normalFace1 = face1.computeNormal();
				
				T face1Dot = normalFace1.dotProduct(centerFace1);
								
				if(face1Dot < T(0)) {
					// Opposite directions, so anticlockwise internal, clockwise external
					tf = triFaceA;
					tlb = triFaceB;
					trb = triFaceC;
				}
				else {
					// Same Direction, so clockwise internal, anticlockwise external
					// Keep clockwise order, which should be tf->trb->tlb
					tf = triFaceA;
					tlb = triFaceC;
					trb = triFaceB;
				}

				// Now we have defined the labels of our 'top' face, find the adjoining vertices on the bottom face
				// and we have our polyhedron labels
				// There is probably a cheaper way of doing this but should be small enough to just do a search
				for(I i = 0; i < (I)(nVertDup/2); i++) {
					if(edge1[i] == tf) {
						if(edge2[i] != tlb && edge2[i] != trb) {
							bf = edge2[i];
						}
					}
					
					if(edge2[i] == tf) {
						if(edge1[i] != tlb && edge1[i] != trb) {
							bf = edge1[i];
						}
					}
					
					if(edge1[i] == tlb) {
						if(edge2[i] != tf && edge2[i] != trb) {
							blb = edge2[i];
						}
					}
					
					if(edge2[i] == tlb) {
						if(edge1[i] != tf && edge1[i] != trb) {
							blb = edge1[i];
						}
					}	
					
					if(edge1[i] == trb) {
						if(edge2[i] != tf && edge2[i] != tlb) {
							brb = edge2[i];
						}
					}	
					
					if(edge2[i] == trb) {
						if(edge1[i] != tf && edge1[i] != tlb) {
							brb = edge1[i];
						}
					}							
				}

				// ToDo: Error Check - should check all 6 vertices were set

				*shape = new cupcfd::geometry::shapes::TriPrism<T>(this->getVertexPos(tf), 
																	this->getVertexPos(tlb), 
																	this->getVertexPos(trb), 
																	this->getVertexPos(bf), 
																	this->getVertexPos(blb), 
																	this->getVertexPos(brb));

				return cupcfd::error::E_SUCCESS;
			}

			template <class M, class I, class T, class L>
			cupcfd::error::eCodes UnstructuredMeshInterface<M,I,T,L>::buildPolyhedron(I cellID, cupcfd::geometry::shapes::QuadPyramid<T> ** shape) {
				// ToDo: We don't currently enforce any ordering (clockwise/anti-clockwise) at the interface level,
				// although we are going to expect that the vertices in order are connected by edges
				
				// For now, we will compute the relative positions of the points, but it
				// would be far more efficient if we control the ordering so that it is just known
				// or at the very least (compute their relative positions only once in the UnstructuredMesh)
				// This is complex, prone to error and inefficient otherwise
				
				// Error Check: Check that the provided cell ID is actually a TriPrism
				cupcfd::geometry::shapes::PolyhedronType type = this->getCellPolyhedronType(cellID);
								
				if(type != cupcfd::geometry::shapes::POLYHEDRON_QUADPYRAMID) {
					DEBUGGABLE_ERROR; return cupcfd::error::E_GEOMETRY_POLYHEDRON_MISMATCH;
				}
				
				// ToDo: Error Check - Check Cell is a locally owned cell
							
				// Since we validated it is a triprism, we can expect there are 5 faces
				// Retrieve the Face Data from the mesh for the cell
				
				I cellLocalFaceID[5];
				for(I i = 0; i  < 5; i++) {
					this->getCellFaceID(cellID, i, cellLocalFaceID + i);
				}
				
				// Get the number of vertices for each face, and determine which
				// is the base
				I faceNVertices[5];
				I baseID;
				I ptr = 0;
				for(I i = 0; i  < 5; i++) {
					faceNVertices[i] = this->getFaceNVertices(cellLocalFaceID[i]);
					if(faceNVertices[i] == 4) {
						baseID = cellLocalFaceID[i];
						ptr++;
					}
				}
				if(ptr != 1) {
					DEBUGGABLE_ERROR; return cupcfd::error::E_GEOMETRY_NFACE_MISMATCH;
				}
				
				// Number of vertices including duplicates
				uint nVertDup = 16; // (3 + 3 + 3 + 3 + 4)
				
				// Get the vertex IDs for each face
				I faceVertexIDs[nVertDup];
				ptr = 0;
				for(I i = 0; i  < 5; i++) {
					for(I j = 0; j < faceNVertices[i]; j++) {
						faceVertexIDs[ptr] = this->getFaceVertex(cellLocalFaceID[i], j);
						ptr++;
					}
				}
				if(ptr != (I)nVertDup) {
					DEBUGGABLE_ERROR; return cupcfd::error::E_GEOMETRY_NVERT_MISMATCH;
				}
				
				// Reconstruct the edges
				std::vector<I> edge1;
				std::vector<I> edge2;
				I tmpEdge1;
				I tmpEdge2;
				bool found;
				// Loop over faces
				ptr = 0;
				for(I i = 0; i < 5; i++) {
					// Loop over vertices within face (bar last)
					for(I j = 0; j < faceNVertices[i] - 1; j++) {
						// Get two edge vertices
						tmpEdge1 = faceVertexIDs[ptr];
						tmpEdge2 = faceVertexIDs[ptr+1];
						ptr++;

						// Search 
						found = false;
						for(I k = 0; k < (I)edge1.size(); k++) {
							if((edge1[k] == tmpEdge1 && edge2[k] == tmpEdge2) || (edge2[k] == tmpEdge1 && edge1[k] == tmpEdge2)) {
								found = true;
							}
						}
						if(!found) {
							edge1.push_back(tmpEdge1);
							edge2.push_back(tmpEdge2);
						}
					}
					
					// Last edge that loops around
					tmpEdge1 = faceVertexIDs[ptr];
					tmpEdge2 = faceVertexIDs[(ptr - faceNVertices[i]) + 1];
					ptr++;
					found = false;
					for(I k = 0; k < (I)edge1.size(); k++) {
						if((edge1[k] == tmpEdge1 && edge2[k] == tmpEdge2) || (edge2[k] == tmpEdge1 && edge1[k] == tmpEdge2)) {
							found = true;
						}
					}
					if(!found) {
						edge1.push_back(tmpEdge1);
						edge2.push_back(tmpEdge2);
					}
				}
				
				if(edge1.size() != (nVertDup/2)) {
					DEBUGGABLE_ERROR; return cupcfd::error::E_GEOMETRY_NEDGE_MISMATCH;
				}
				
				I apex;
			   	cupcfd::geometry::euclidean::EuclideanPoint<T,3> basePos[4];
			   	
				// Determine the relative positions of the vertices amongst one another, orient them
				// Face 1
				I baseFaceA = this->getFaceVertex(baseID, 0);
				I baseFaceB = this->getFaceVertex(baseID, 1);
				I baseFaceC = this->getFaceVertex(baseID, 2);
				I baseFaceD = this->getFaceVertex(baseID, 3);

				basePos[0] = this->getVertexPos(baseFaceA);
				basePos[1] = this->getVertexPos(baseFaceB);
				basePos[2] = this->getVertexPos(baseFaceC);
				basePos[3] = this->getVertexPos(baseFaceD);
				
				// Set the apex point - must be the last remaining point
				for(I i = 0; i < (I)nVertDup; i++) {
					if(faceVertexIDs[i] != baseFaceA && faceVertexIDs[i] != baseFaceB && faceVertexIDs[i] != baseFaceC && faceVertexIDs[i] != baseFaceD) {
						apex = faceVertexIDs[i];
						break;
					}
				}
				
				// Vector from Apex to base must go through internal of shape
				// Therefore, determine the ordering of the base vertices when looking from the apex point (i.e. ordering from internal)
				
				cupcfd::geometry::euclidean::EuclideanPoint<T,3> apexPos = this->getVertexPos(apex);
				bool isClockwise = cupcfd::geometry::shapes::isVertexOrderClockwise(apexPos, basePos, 4);
				
				// Now we know the ordering, create the object
				if(isClockwise) {
					// Clockwise (base is in order of fl->fr->br->bl
					// Note: QuadPyramid Constructor Expects in order of fl, fr, bl, br
					*shape = new cupcfd::geometry::shapes::QuadPyramid<T>(apexPos, basePos[0], basePos[1], basePos[3], basePos[2]);
				}
				else {
					// Anti-Clockwise (base is in order of fl->bl->br->fr
					// Note: QuadPyramid Constructor Expects in order of fl, fr, bl, br
					*shape = new cupcfd::geometry::shapes::QuadPyramid<T>(apexPos, basePos[0], basePos[3], basePos[1], basePos[2]);
				}
				
				return cupcfd::error::E_SUCCESS;
			}
			
			template <class M, class I, class T, class L>
			cupcfd::error::eCodes UnstructuredMeshInterface<M,I,T,L>::buildPolyhedron(I cellID, cupcfd::geometry::shapes::Tetrahedron<T> ** shape) {
				// ToDo: We don't currently enforce any ordering (clockwise/anti-clockwise) at the interface level,
				// although we are going to expect that the vertices in order are connected by edges
				
				// For now, we will compute the relative positions of the points, but it
				// would be far more efficient if we control the ordering so that it is just known
				// or at the very least (compute their relative positions only once in the UnstructuredMesh)
				// This is complex, prone to error and inefficient otherwise
				
				// Error Check: Check that the provided cell ID is actually a TriPrism
				cupcfd::geometry::shapes::PolyhedronType type = this->getCellPolyhedronType(cellID);
								
				if(type != cupcfd::geometry::shapes::POLYHEDRON_TETRAHEDRON) {
					DEBUGGABLE_ERROR; return cupcfd::error::E_GEOMETRY_POLYHEDRON_MISMATCH;
				}
				
				// ToDo: Error Check - Check Cell is a locally owned cell
							
				// We can expect there are 4 faces
				// Retrieve the Face Data from the mesh for the cell
				
				I cellLocalFaceID[4];
				for(I i = 0; i  < 4; i++) {
					this->getCellFaceID(cellID, i, cellLocalFaceID + i);
				}
				
				// Get the number of vertices for each face, and determine which is the base
				
				I faceNVertices[4];
				I baseID;
				uint ptr = 0;
				for(I i = 0; i  < 4; i++) {
					faceNVertices[i] = this->getFaceNVertices(cellLocalFaceID[i]);
				}
				
				// All faces are triangular, just pick one as arbitrary base
				baseID = cellLocalFaceID[0];
				
				// Number of vertices including duplicates
				uint nVertDup = 13; // (3 + 3 + 3 + 4)
				
				// Get the vertex IDs for each face
				I faceVertexIDs[nVertDup];
				ptr = 0;
				for(I i = 0; i  < 4; i++) {
					for(I j = 0; j < faceNVertices[i]; j++) {
						faceVertexIDs[ptr] = this->getFaceVertex(cellLocalFaceID[i], j);
						ptr++;
					}
				}
				if(ptr != nVertDup) {
					DEBUGGABLE_ERROR; return cupcfd::error::E_GEOMETRY_NVERT_MISMATCH;
				}
				
				// Reconstruct the edges
				std::vector<I> edge1;
				std::vector<I> edge2;
				
				I tmpEdge1;
				I tmpEdge2;
				bool found;
				
				// Loop over faces
				ptr = 0;
				for(I i = 0; i < 4; i++) {
					// Loop over vertices within face (bar last)
					for(I j = 0; j < faceNVertices[i] - 1; j++) {
						// Get two edge vertices
						tmpEdge1 = faceVertexIDs[ptr];
						tmpEdge2 = faceVertexIDs[ptr+1];
						ptr++;

						// Search 
						found = false;
						for(I k = 0; k < (I)edge1.size(); k++) {
							if((edge1[k] == tmpEdge1 && edge2[k] == tmpEdge2) || (edge2[k] == tmpEdge1 && edge1[k] == tmpEdge2)) {
								found = true;
							}
						}
						if(!found) {
							edge1.push_back(tmpEdge1);
							edge2.push_back(tmpEdge2);
						}
					}
					
					// Last edge that loops around
					tmpEdge1 = faceVertexIDs[ptr];
					tmpEdge2 = faceVertexIDs[(ptr - faceNVertices[i]) + 1];
					ptr++;
					
					found = false;
					for(I k = 0; k < (I)edge1.size(); k++) {
						if((edge1[k] == tmpEdge1 && edge2[k] == tmpEdge2) || (edge2[k] == tmpEdge1 && edge1[k] == tmpEdge2)) {
							found = true;
						}
					}
					
					if(!found) {
						edge1.push_back(tmpEdge1);
						edge2.push_back(tmpEdge2);
					}
				}
				
				if(edge1.size() != (nVertDup/2)) {
					DEBUGGABLE_ERROR; return cupcfd::error::E_GEOMETRY_NEDGE_MISMATCH;
				}
				
				I apex;
			   	cupcfd::geometry::euclidean::EuclideanPoint<T,3> basePos[3];
			   	
				// Determine the relative positions of the vertices amongst one another, orient them
				// Face 1
				I baseFaceA = this->getFaceVertex(baseID, 0);
				I baseFaceB = this->getFaceVertex(baseID, 1);
				I baseFaceC = this->getFaceVertex(baseID, 2);

				basePos[0] = this->getVertexPos(baseFaceA);
				basePos[1] = this->getVertexPos(baseFaceB);
				basePos[2] = this->getVertexPos(baseFaceC);
				
				// Set the apex point - must be the last remaining point
				for(I i = 0; i < (I)nVertDup; i++) {
					if(faceVertexIDs[i] != baseFaceA && faceVertexIDs[i] != baseFaceB && faceVertexIDs[i] != baseFaceC) {
						apex = faceVertexIDs[i];
						break;
					}
				}
				
				// Vector from Apex to base must go through internal of shape
				// Therefore, determine the ordering of the base vertices when looking from the apex point (i.e. ordering from internal)
				
				cupcfd::geometry::euclidean::EuclideanPoint<T,3> apexPos = this->getVertexPos(apex);
				bool isClockwise = cupcfd::geometry::shapes::isVertexOrderClockwise(apexPos, basePos, 3);
				
				// Now we know the ordering, create the object
				if(isClockwise) {
					// Clockwise (base is in order of f->br->bl
					// Note: QuadPyramid Constructor Expects in order of f, bl, br
					*shape = new cupcfd::geometry::shapes::Tetrahedron<T>(apexPos, basePos[0], basePos[2], basePos[1]);
				}
				else {
					// Anti-Clockwise (base is in order of f->bl->br
					// Note: QuadPyramid Constructor Expects in order of f, bl, br
					*shape = new cupcfd::geometry::shapes::Tetrahedron<T>(apexPos, basePos[0], basePos[1], basePos[2]);
				}
				
				return cupcfd::error::E_SUCCESS;
			}
			
			template <class M, class I, class T, class L>
			cupcfd::error::eCodes UnstructuredMeshInterface<M,I,T,L>::buildPolyhedron(I cellID, cupcfd::geometry::shapes::Hexahedron<T> ** shape) {
				// ToDo: We don't currently enforce any ordering (clockwise/anti-clockwise) at the interface level,
				// although we are going to expect that the vertices in order are connected by edges
				
				// For now, we will compute the relative positions of the points, but it
				// would be far more efficient if we control the ordering so that it is just known
				// or at the very least (compute their relative positions only once in the UnstructuredMesh)
				// This is complex, prone to error and inefficient otherwise
				
				// Error Check: Check that the provided cell ID is actually a Hexahedron
				cupcfd::geometry::shapes::PolyhedronType type = this->getCellPolyhedronType(cellID);
								
				if(type != cupcfd::geometry::shapes::POLYHEDRON_HEXAHEDRON) {
					DEBUGGABLE_ERROR; return cupcfd::error::E_GEOMETRY_POLYHEDRON_MISMATCH;
				}
				
				// ToDo: Error Check - Check Cell is a locally owned cell
							
				// Since we validated it is a hexahedron, we can expect there are 6 faces
				// Retrieve the Face Data from the mesh for the cell
				
				I cellLocalFaceID[6];
				for(I i = 0; i  < 6; i++) {
					this->getCellFaceID(cellID, i, cellLocalFaceID + i);
				}
				
				// Get the number of vertices for each face, and determine which
				// is the base
				I faceNVertices[6];
				I baseID;
				uint ptr = 0;
				for(I i = 0; i  < 6; i++) {
					faceNVertices[i] = this->getFaceNVertices(cellLocalFaceID[i]);
					ptr++;
				}
				
				// Pick an arbitrary face as the base
				baseID = cellLocalFaceID[0];
				if(ptr != 6) {
					DEBUGGABLE_ERROR; return cupcfd::error::E_GEOMETRY_NFACE_MISMATCH;
				}
				
				// Number of vertices including duplicates
				uint nVertDup = 24; // (6*4)
				
				// Get the vertex IDs for each face
				I faceVertexIDs[nVertDup];
				ptr = 0;
				for(I i = 0; i  < 6; i++) {
					for(I j = 0; j < faceNVertices[i]; j++) {
						faceVertexIDs[ptr] = this->getFaceVertex(cellLocalFaceID[i], j);
						ptr++;
					}
				}
				if(ptr != nVertDup) {
					DEBUGGABLE_ERROR; return cupcfd::error::E_GEOMETRY_NVERT_MISMATCH;
				}
				
				// Reconstruct the edges
				std::vector<I> edge1;
				std::vector<I> edge2;
				I tmpEdge1;
				I tmpEdge2;
				bool found;
				// Loop over faces
				ptr = 0;
				for(I i = 0; i < 6; i++) {
					// Loop over vertices within face (bar last)
					for(I j = 0; j < faceNVertices[i] - 1; j++) {
						// Get two edge vertices
						tmpEdge1 = faceVertexIDs[ptr];
						tmpEdge2 = faceVertexIDs[ptr+1];
						ptr++;

						// Search 
						found = false;
						for(I k = 0; k < (I)edge1.size(); k++) {
							if((edge1[k] == tmpEdge1 && edge2[k] == tmpEdge2) || (edge2[k] == tmpEdge1 && edge1[k] == tmpEdge2)) {
								found = true;
							}
						}
						if(!found) {
							edge1.push_back(tmpEdge1);
							edge2.push_back(tmpEdge2);
						}
					}
					
					// Last edge that loops around
					tmpEdge1 = faceVertexIDs[ptr];
					tmpEdge2 = faceVertexIDs[(ptr - faceNVertices[i]) + 1];
					ptr++;
					found = false;
					for(I k = 0; k < (I)edge1.size(); k++) {
						if((edge1[k] == tmpEdge1 && edge2[k] == tmpEdge2) || (edge2[k] == tmpEdge1 && edge1[k] == tmpEdge2)) {
							found = true;
						}
					}
					if(!found) {
						edge1.push_back(tmpEdge1);
						edge2.push_back(tmpEdge2);
					}
				}
				
				if(edge1.size() != (nVertDup/2)) {
					DEBUGGABLE_ERROR; return cupcfd::error::E_GEOMETRY_NEDGE_MISMATCH;
				}
				
			   	cupcfd::geometry::euclidean::EuclideanPoint<T,3> basePos[4];
			   	
				// Determine the relative positions of the vertices amongst one another, orient them
				// Face 1
				I baseFaceA = this->getFaceVertex(baseID, 0);
				I baseFaceB = this->getFaceVertex(baseID, 1);
				I baseFaceC = this->getFaceVertex(baseID, 2);
				I baseFaceD = this->getFaceVertex(baseID, 3);

				basePos[0] = this->getVertexPos(baseFaceA);
				basePos[1] = this->getVertexPos(baseFaceB);
				basePos[2] = this->getVertexPos(baseFaceC);
				basePos[3] = this->getVertexPos(baseFaceD);
				
				// Find a vertex on the opposite face
				I oppVertexID;
				for(I i = 0; i < (I)nVertDup; i++) {
					if(faceVertexIDs[i] != baseFaceA && faceVertexIDs[i] != baseFaceB && faceVertexIDs[i] != baseFaceC && faceVertexIDs[i] != baseFaceD) {
						oppVertexID = faceVertexIDs[i];
						break;
					}
				} 
				
				// A vector from the opposite face to a point on the base face either traverses along an edge or goes through the hexahedron
				// Test the ordering of the base face
				bool isClockwise = cupcfd::geometry::shapes::isVertexOrderClockwise(this->getVertexPos(oppVertexID), basePos, 4);
				
				// Name scheme - t[op]/b[ottom], l[eft]/r[ight], f[ront]/b[ack]
				I tlf, trf, blf, brf, tlb, trb, blb, brb;
					
				// Now we know the ordering, create the object
				if(isClockwise) {
					// Clockwise (base is in order of blf, brf, brb, blb)
					blf = baseFaceA;
					brf = baseFaceB;
					brb = baseFaceC;
					blb = baseFaceD;
				}
				else {
					// Anti-Clockwise (base is in order of blf, blb, brb, brf)
					blf = baseFaceA;
					blb = baseFaceB;
					brb = baseFaceC;
					brf = baseFaceD;
				}
				
				// Since this is a hexahedron, if we just find the non-base face vertex joined to each of the base vertices,
				// we will know the labels for the opposite face
				
				for(I i = 0; i < (I)edge1.size(); i++) {
					// === blf ===
					if(edge1[i] == blf) {
						// ToDo: We could probably do this is a nicer way/more compressed code
						if(edge2[i] != brf && edge2[i] != brb && edge2[i] != blb) {
							// Must be the top face linked vertex - these functions naturally assume that the stored
							// mesh is correct....
							tlf = edge2[i];
						}
					}	
					
					if(edge2[i] == blf) {
						if(edge1[i] != brf && edge1[i] != brb && edge1[i] != blb) {
							// Must be the top face linked vertex - these functions naturally assume that the stored
							// mesh is correct....
							tlf = edge1[i];
						}
					}		
					
					// === brf ===
					
					if(edge1[i] == brf) {
						// ToDo: We could probably do this is a nicer way/more compressed code
						if(edge2[i] != blf && edge2[i] != brb && edge2[i] != blb) {
							// Must be the top face linked vertex - these functions naturally assume that the stored
							// mesh is correct....
							trf = edge2[i];
						}
					}	
	
					if(edge2[i] == brf) {
						// ToDo: We could probably do this is a nicer way/more compressed code
						if(edge1[i] != blf && edge1[i] != brb && edge1[i] != blb) {
							// Must be the top face linked vertex - these functions naturally assume that the stored
							// mesh is correct....
							trf = edge1[i];
						}
					}	
					
					// === brb ===
					
					if(edge1[i] == brb) {
						// ToDo: We could probably do this is a nicer way/more compressed code
						if(edge2[i] != blf && edge2[i] != brf && edge2[i] != blb) {
							// Must be the top face linked vertex - these functions naturally assume that the stored
							// mesh is correct....
							trb = edge2[i];
						}
					}	
			
					if(edge2[i] == brb) {
						// ToDo: We could probably do this is a nicer way/more compressed code
						if(edge1[i] != blf && edge1[i] != brf && edge1[i] != blb) {
							// Must be the top face linked vertex - these functions naturally assume that the stored
							// mesh is correct....
							trb = edge1[i];
						}
					}	
					
					// === blb ===
					
					if(edge1[i] == blb) {
						// ToDo: We could probably do this is a nicer way/more compressed code
						if(edge2[i] != blf && edge2[i] != brf && edge2[i] != brb) {
							// Must be the top face linked vertex - these functions naturally assume that the stored
							// mesh is correct....
							tlb = edge2[i];
						}
					}

					if(edge2[i] == blb) {
						// ToDo: We could probably do this is a nicer way/more compressed code
						if(edge1[i] != blf && edge1[i] != brf && edge1[i] != brb) {
							// Must be the top face linked vertex - these functions naturally assume that the stored
							// mesh is correct....
							tlb = edge1[i];
						}
					}																			
				} 
					   
				*shape = new cupcfd::geometry::shapes::Hexahedron<T>(this->getVertexPos(tlf),
																	this->getVertexPos(trf),
																	this->getVertexPos(blf),
																	this->getVertexPos(brf),
																	this->getVertexPos(tlb),
																	this->getVertexPos(trb),
																	this->getVertexPos(blb),
																	this->getVertexPos(brb));
				
				return cupcfd::error::E_SUCCESS;
			}
			
			template <class M, class I, class T, class L>
			cupcfd::error::eCodes UnstructuredMeshInterface<M,I,T,L>::addData(MeshSource<I,T,L>& data, L * assignedCellLabels, I nAssignedCellLabels) {
				cupcfd::error::eCodes status;

				// =================================
				// (1)  Data Label Stage
				// =================================
				
				// (a) Build a new connectivity graph using the allocated cells so we are
				// aware of the edges and ghost cells that are also needed
				cupcfd::data_structures::DistributedAdjacencyList<I,I> * partGraph;
				
				// Reuse the same communicator as the distributed graph, since all members of the comm associated with this
				// mesh must participate
				status = data.buildDistributedAdjacencyList(&partGraph, *(this->cellConnGraph->comm), assignedCellLabels, nAssignedCellLabels);
				if (status != cupcfd::error::E_SUCCESS) {
					return status;
				}

				//(b) Get the labels of local + ghost cells for this rank
				I lCells = partGraph->nLONodes;
				I ghCells = partGraph->nLGhNodes;
				I nCells = lCells + ghCells;
					
				if(lCells > 0) {
					// (c) Get the labels from the partition graph
					I * cellLabels = (I *) malloc(sizeof(I) * nCells);
					partGraph->getLocalNodes(cellLabels, lCells);
					partGraph->getGhostNodes(cellLabels + lCells, ghCells);

					// (d) Get the labels of all faces associated with only *local* cells - faces that are between local-> ghost will be caught in this.

					// (di) Get number of faces per cell
					I nCellFacesSum;
					I * nCellFaces = (I *) malloc(sizeof(I) * lCells);
					status = data.getCellNFaces(nCellFaces, lCells, cellLabels, lCells);
					if (status != cupcfd::error::E_SUCCESS) {
						return status;
					}

					// (dii) Summed number of faces per cell - needed for CSR sizes
					cupcfd::utility::drivers::sum(nCellFaces, lCells, &nCellFacesSum);

					// d(iii) Get Face Labels for each Cell in CSR Format - this will omit the non-existant faces when the faces per cell is below max faces
					I * cellFaceLabelInd = (I *) malloc(sizeof(I) * (lCells+1));
					I * cellFaceLabelData = (I *) malloc(sizeof(I) * nCellFacesSum);
					status = data.getCellFaceLabels(cellFaceLabelInd, lCells+1, cellFaceLabelData, nCellFacesSum, cellLabels, lCells);
					if (status != cupcfd::error::E_SUCCESS) {
						return status;
					}

					// (div) Remove duplicate face labels from where cells share faces
					// This leaves a single label per each face that is attached to a local cell
					I * faceLabelsDistinct;
					I nFaceLabelsDistinct;
					cupcfd::utility::drivers::distinctArray(cellFaceLabelData, nCellFacesSum, &faceLabelsDistinct, &nFaceLabelsDistinct);

					// (e) Get the labels of any boundaries associated with these faces
					// (ei) Reduce down to only faces that are boundaries
					bool * faceLabelIsBoundary = (bool *) malloc(sizeof(bool) * nFaceLabelsDistinct);
					status = data.getFaceIsBoundary(faceLabelIsBoundary, nFaceLabelsDistinct, faceLabelsDistinct, nFaceLabelsDistinct);
					if (status != cupcfd::error::E_SUCCESS) {
						return status;
					}

					int nFaceBoundaries = 0;
					for(int i = 0; i < nFaceLabelsDistinct; i++) {
						if(faceLabelIsBoundary[i] == true) {
							nFaceBoundaries = nFaceBoundaries + 1;
						}
					}
					
					I * faceWithBoundary = (I *) malloc(sizeof(I) * nFaceBoundaries);

					I nFaceWithoutBoundary = nFaceLabelsDistinct - nFaceBoundaries;
					I * faceWithoutBoundary = (I *) malloc(sizeof(I) * nFaceWithoutBoundary);

					I ptr = 0;
					I ptr2 = 0;
					for(int i = 0; i < nFaceLabelsDistinct; i++) {
						if(faceLabelIsBoundary[i] == true) {
							faceWithBoundary[ptr] = faceLabelsDistinct[i];
							ptr++;
						}
						else {
							faceWithoutBoundary[ptr2] = faceLabelsDistinct[i];
							ptr2 = ptr2 + 1;
						}
					}
					
					// (eii) Get the boundary labels for these faces
					I nBoundaryLabels = nFaceBoundaries;
					I * boundaryLabels = (I *) malloc(sizeof(I) * nBoundaryLabels);
					status = data.getFaceBoundaryLabels(boundaryLabels, nBoundaryLabels, faceWithBoundary, nFaceBoundaries);
					if (status != cupcfd::error::E_SUCCESS) {
						return status;
					}

					// (eiii) Boundary faces should be unique, but as a precaution remove repeats
					I * boundaryLabelsDistinct;
					I nBoundaryLabelsDistinct;
					cupcfd::utility::drivers::distinctArray(boundaryLabels, nBoundaryLabels, &boundaryLabelsDistinct, &nBoundaryLabelsDistinct);

					// (f) Get the distinct labels of any vertexes associated with the retrieved faces and boundaries

					// (fi) Get the number of vertices per face and sum to get the CSR size
					I * faceVerticesCount = (I *) malloc(sizeof(I) * nFaceLabelsDistinct);

					status = data.getFaceNVertices(faceVerticesCount, nFaceLabelsDistinct, faceLabelsDistinct, nFaceLabelsDistinct);
					if (status != cupcfd::error::E_SUCCESS) {
						return status;
					}

					I faceVerticesCountTotal;
					cupcfd::utility::drivers::sum(faceVerticesCount, nFaceLabelsDistinct, &faceVerticesCountTotal);

					// (fii) Get the number of vertices per boundary and sum to get the CSR size
					I * boundaryVerticesCount = (I *) malloc(sizeof(I) * nBoundaryLabelsDistinct);

					status = data.getFaceNVertices(boundaryVerticesCount, nBoundaryLabelsDistinct, boundaryLabelsDistinct, nBoundaryLabelsDistinct);
					if (status != cupcfd::error::E_SUCCESS) {
						return status;
					}

					I boundaryVerticesCountTotal;
					cupcfd::utility::drivers::sum(boundaryVerticesCount, nBoundaryLabelsDistinct, &boundaryVerticesCountTotal);

					// (fiii) Get the vertex labels for the face and boundary lists - CSR format.
					// Store the vertices labels in a combined array for both.

					I * vertLabelData = (I *) malloc(sizeof(I) * (faceVerticesCountTotal + boundaryVerticesCountTotal));

					I * faceVertLabelCSRInd = (I *) malloc(sizeof(I) * (nFaceLabelsDistinct+1));

					status = data.getFaceVerticesLabelsCSR(faceVertLabelCSRInd, nFaceLabelsDistinct+1,
															  vertLabelData, faceVerticesCountTotal,
															  faceLabelsDistinct, nFaceLabelsDistinct);
					if (status != cupcfd::error::E_SUCCESS) {
						return status;
					}

					I * bndVertLabelCSRInd = (I *) malloc(sizeof(I) * (nBoundaryLabelsDistinct+1));

					status = data.getFaceVerticesLabelsCSR(bndVertLabelCSRInd, nBoundaryLabelsDistinct+1,
															  vertLabelData + faceVerticesCountTotal, boundaryVerticesCountTotal,
															  boundaryLabelsDistinct, nBoundaryLabelsDistinct);
					if (status != cupcfd::error::E_SUCCESS) {
						return status;
					}

					// (fiv) Get distinct vertex labels
					I * vertexLabelsDistinct;
					I nVertexLabelsDistinct;
					cupcfd::utility::drivers::distinctArray(vertLabelData, faceVerticesCountTotal + boundaryVerticesCountTotal,
																 &vertexLabelsDistinct, &nVertexLabelsDistinct);

					// (f) Get the labels of any regions associated with the boundaries
					//  Since we're loading all regions, just make an label list of 0->regionCount-1 since we'll just read all since
					// their numbers are typically far far fewer
					I nRegions;
					status = data.getRegionCount(&nRegions);
					if (status != cupcfd::error::E_SUCCESS) {
						return status;
					}
					I * regionIndices = (I *) malloc(sizeof(I) * nRegions);
					I * regionLabels = (I *) malloc(sizeof(I) * nRegions);

					for(I i = 0; i < nRegions; i++) {
						regionIndices[i] = i;
					}

					status = data.getRegionLabels(regionLabels, nRegions, regionIndices,nRegions);
					if (status != cupcfd::error::E_SUCCESS) {
						return status;
					}
					free(regionIndices);
					
					// ====================================
					// (2)  Data Load and Mesh Build Stage
					// ====================================
					// Now all the appropriate labels are known, start constructing the Mesh by reading in data
					// Order should be in that of least dependancy - i.e.:
					// (i) Vertices
					// (ii) Regions
					// (iii) Cells
					// (iv) Boundaries (Need vertices)
					// (v) Faces (Needs vertices, cells, boundaries)

					// === Read Vertices Data ===
					//Final Vertices Labels are stored in vertexLabelsDistinct
					euc::EuclideanPoint<T,3> * pointTmpStore = (euc::EuclideanPoint<T,3> *) malloc(sizeof(euc::EuclideanPoint<T,3>) * nVertexLabelsDistinct);
					status = data.getVertexCoords(pointTmpStore, nVertexLabelsDistinct, vertexLabelsDistinct, nVertexLabelsDistinct);
					if (status != cupcfd::error::E_SUCCESS) {
						return status;
					}

					// Add Vertices to Mesh
					for(I i = 0; i < nVertexLabelsDistinct; i++) {
						this->addVertex(vertexLabelsDistinct[i], pointTmpStore[i]);
					}

					free(pointTmpStore);

					// === Read Region Data ===
					// ToDo: Read Region Names - Needs fixing of reading strings from HDF5

					// Add Regions to Mesh
					for(I i = 0; i < nRegions; i++) {
						// ToDo: Fix the region name
						std::string name = "Default";
						this->addRegion(regionLabels[i], name);
					}
					
					// === Read Boundary Data ===
					// Final labels in boundaryLabelsDistinct

					// Region Labels
					I nBoundaryRegionLabels = nBoundaryLabelsDistinct;
					I * boundaryRegionLabels = (I *) malloc(sizeof(I) * nBoundaryRegionLabels);
					status = data.getBoundaryRegionLabels(boundaryRegionLabels, nBoundaryRegionLabels, boundaryLabelsDistinct, nBoundaryLabelsDistinct);
					if (status != cupcfd::error::E_SUCCESS) {
						return status;
					}

					// Vertices Labels
					// Can reuse previous read starting at vertLabelData[faceVerticesCountTotal]

					// Boundary Distance
					T * bDistance = (T *) malloc(sizeof(T) * nBoundaryLabelsDistinct);
					status = data.getBoundaryDistance(bDistance, nBoundaryLabelsDistinct, boundaryLabelsDistinct, nBoundaryLabelsDistinct);
					if (status != cupcfd::error::E_SUCCESS) {
						return status;
					}

					// Add Boundaries to Mesh
					for(I i = 0; i < nBoundaryLabelsDistinct; i++) {
						I bLabel = boundaryLabelsDistinct[i];
						I rLabel = boundaryRegionLabels[i];

						I vertDataPtr = faceVerticesCountTotal + bndVertLabelCSRInd[i];
						I rangeSize = bndVertLabelCSRInd[i+1] - bndVertLabelCSRInd[i];

						this->addBoundary(bLabel, rLabel, vertLabelData + vertDataPtr, rangeSize, bDistance[i]);
					}

					free(bDistance);
					
					// === Read Cell Data ===
					// Read Cell Data for local *and* ghost cells
					// Final labels in cellLabels: Segmented into [local|Ghost cells] for 0->lCells-1 and lCells->lCells+ghCells-1
					pointTmpStore = (euc::EuclideanPoint<T,3> *) malloc(sizeof(euc::EuclideanPoint<T,3>) * nCells);
					T * cellVol = (T *) malloc(sizeof(T) * nCells);

					// Read Cell Center
					status = data.getCellCenter(pointTmpStore, nCells, cellLabels, nCells);
					if (status != cupcfd::error::E_SUCCESS) {
						return status;
					}

					// Read Cell Volume
					status = data.getCellVolume(cellVol, nCells, cellLabels, nCells);
					if (status != cupcfd::error::E_SUCCESS) {
						return status;
					}

					// Add Cells to Mesh
					// Local Cells
					for(I i = 0; i < lCells; i++) {
						this->addCell(cellLabels[i], pointTmpStore[i], cellVol[i], true);
					}

					// Ghost Cells
					for(I i = lCells; i < (lCells + ghCells); i++) {
						this->addCell(cellLabels[i], pointTmpStore[i], cellVol[i], false);
					}

					free(pointTmpStore);
					free(cellVol);

					// === Read Face Data ===
					// Final Face Labels in faceLabelsDistinct

					// Cell 1 Labels
					// For boundary and non-boundary faces
					I * fCell1Labels = (I *) malloc(sizeof(I) * nFaceLabelsDistinct);
					status = data.getFaceCell1Labels(fCell1Labels, nFaceLabelsDistinct, faceLabelsDistinct, nFaceLabelsDistinct);
					if (status != cupcfd::error::E_SUCCESS) {
						return status;
					}

					// Cell 2 Labels
					// For non-boundary faces only
					I * fCell2Labels = (I *) malloc(sizeof(I) * nFaceWithoutBoundary);
					status = data.getFaceCell2Labels(fCell2Labels, nFaceWithoutBoundary, faceWithoutBoundary, nFaceWithoutBoundary);
					if (status != cupcfd::error::E_SUCCESS) {
						return status;
					}

					// Vertex Labels
					// Reuse previously read values

					// Boundary Labels
					// Boundary faces only
					I * fBndLabels = (I *) malloc(sizeof(I) * nFaceBoundaries);
					status = data.getFaceBoundaryLabels(fBndLabels, nFaceBoundaries, faceWithBoundary, nFaceBoundaries);
					if (status != cupcfd::error::E_SUCCESS) {
						return status;
					}

					// Is Boundary
					bool * fIsBoundary = (bool *) malloc(sizeof(bool) * nFaceLabelsDistinct);
					status = data.getFaceIsBoundary(fIsBoundary, nFaceLabelsDistinct, faceLabelsDistinct, nFaceLabelsDistinct);
					if (status != cupcfd::error::E_SUCCESS) {
						return status;
					}

					// Face Lambda
					T * fLambda = (T *) malloc(sizeof(T) * nFaceLabelsDistinct);
					status = data.getFaceLambda(fLambda, nFaceLabelsDistinct, faceLabelsDistinct, nFaceLabelsDistinct);
					if (status != cupcfd::error::E_SUCCESS) {
						return status;
					}

					// Face Area
					T * fArea = (T *) malloc(sizeof(T) * nFaceLabelsDistinct);
					status = data.getFaceArea(fArea, nFaceLabelsDistinct, faceLabelsDistinct, nFaceLabelsDistinct);
					if (status != cupcfd::error::E_SUCCESS) {
						return status;
					}

					// Face Center
					euc::EuclideanPoint<T,3> * fCenter = (euc::EuclideanPoint<T,3> *) malloc(sizeof(euc::EuclideanPoint<T,3>) * nFaceLabelsDistinct);
					status = data.getFaceCenter(fCenter, nFaceLabelsDistinct, faceLabelsDistinct, nFaceLabelsDistinct);
					if (status != cupcfd::error::E_SUCCESS) {
						return status;
					}

					// Face Normal
					euc::EuclideanVector<T,3> * fNorm = (euc::EuclideanVector<T,3> *) malloc(sizeof(euc::EuclideanVector<T,3>) * nFaceLabelsDistinct);
					status = data.getFaceNormal(fNorm, nFaceLabelsDistinct, faceLabelsDistinct, nFaceLabelsDistinct);
					if (status != cupcfd::error::E_SUCCESS) {
						return status;
					}

					// Add Face Data
					ptr = 0;
					ptr2 = 0;
					for(I i = 0; i < nFaceLabelsDistinct; i++) {
						euc::EuclideanPoint<T,3> xpac(T(0), T(0), T(0));
						euc::EuclideanPoint<T,3> xnac(T(0), T(0), T(0));
						T rlencos = 0.0;

						// Vertex Index Pointer into VertLabelData
						I vertDataPtr = faceVertLabelCSRInd[i];

						// Number of vertices
						I rangeSize = faceVertLabelCSRInd[i+1] - faceVertLabelCSRInd[i];

						I fCell2OrBoundLabel;

						if(fIsBoundary[i]) {
							// Sanity check to make sure we're on the right label
							if(faceWithBoundary[ptr] != faceLabelsDistinct[i]) {
								DEBUGGABLE_ERROR; return cupcfd::error::E_ERROR;
							}

							fCell2OrBoundLabel = fBndLabels[ptr];
							ptr++;
						}
						else {
							// Sanity check to make sure we're on the right label
							if(faceWithoutBoundary[ptr2] != faceLabelsDistinct[i]) {
								DEBUGGABLE_ERROR; return cupcfd::error::E_ERROR;
							}

							fCell2OrBoundLabel = fCell2Labels[ptr2];
							ptr2 = ptr2 + 1;
						}

						status = this->addFace(faceLabelsDistinct[i], fCell1Labels[i], fCell2OrBoundLabel, fIsBoundary[i], fLambda[i], fNorm[i],
												  vertLabelData + vertDataPtr, rangeSize, fCenter[i], xpac, xnac, rlencos, fArea[i]);
						if (status != cupcfd::error::E_SUCCESS) {
							return status;
						}
					}

					// =================================
					// Cleanup
					// =================================

					free(cellLabels);
					free(boundaryLabels);
					free(regionLabels);

					free(cellFaceLabelInd);
					free(cellFaceLabelData);
					free(faceVertLabelCSRInd);
					free(bndVertLabelCSRInd);
					free(vertLabelData);

					free(boundaryLabelsDistinct);
					free(vertexLabelsDistinct);
					free(faceLabelsDistinct);
					free(faceWithBoundary);
					free(faceWithoutBoundary);

					free(fCell1Labels);
					free(fCell2Labels);
					free(fBndLabels);
					free(fIsBoundary);
					free(fLambda);
					free(fArea);
					free(fCenter);
					free(fNorm);
					
					return cupcfd::error::E_SUCCESS;
				}

				// =================================
				// (3)  Finalisation
				// =================================
				// Some Mesh structures will need the finalisation stage to ensure that some properties are correct.
				// E.g. Data is stored for mappings from face->other, but we may also need cell->face etc, so these will
				// be computed as part of the finalisation stage.
				// It is also the case that some data may be insufficient - e.g. ghost cells only store faces that adjoin
				// locally owned cells so we may need to retrieve data from the process that owns it locally to find out how many
				// faces, vertices etc it has. Additionally, if a mesh file format does not contain certain data, such as volumes,
				// areas etc, we will need to compute it here to ensure the mesh properties are correct, and then also ensure that
				// data is also propagated to any ghost cells that are unable to compute it locally (e.g. due to insufficient face data)

				status = this->finalize();
				if (status != cupcfd::error::E_SUCCESS) {
					return status;
				}

				// Cleanup any temporary structures/space outside of the mesh
				delete partGraph;

				return cupcfd::error::E_SUCCESS;
			}
			
			template <class M, class I, class T, class L>
			cupcfd::error::eCodes UnstructuredMeshInterface<M,I,T,L>::finalize() {
				return static_cast<M*>(this)->finalize();
			}
			
			template <class M, class I, class T, class L>
			cupcfd::error::eCodes UnstructuredMeshInterface<M,I,T,L>::findCellID(cupcfd::geometry::euclidean::EuclideanPoint<T,3>& point, 
																					  I * localCellID,
																					  I * globalCellID) {
				// Loop over and test each locally owned cell until we find the cell the point is located in (if any on this rank)
				// For each cell:
				// (a) Get the number of vertices and faces each cell has
				// (b) Determine the type of cell
				// (c) Test whether the position of the source is inside the cell
				// (ci) If so, updated the local and global cell IDs
				// (cii) If not, continue until no cells left to test
				cupcfd::error::eCodes status;
	
				// Part (a)
				I nCells = this->properties.lOCells;
	
				// Mesh should be sorted into local, then global cells so loop over the initial range for local cells
				for(I i = 0; i < nCells; i++) {
					// Part (b)
					// Get the number of vertices
					I nVertices = this->getCellNVertices(i);
	
					// Get the number of faces
					I nFaces = this->getCellNFaces(i);
	
					// Get the Cell Type
					cupcfd::geometry::shapes::PolyhedronType pType = cupcfd::geometry::shapes::findPolyhedronType(nVertices, nFaces);
	
					bool inside;
					
					if(pType == cupcfd::geometry::shapes::POLYHEDRON_TETRAHEDRON) {
						cupcfd::geometry::shapes::Tetrahedron<T> * shape1;
						this->buildPolyhedron(i, &shape1);
						inside = shape1->isPointInside(point);
						delete shape1;
					}
					else if(pType == cupcfd::geometry::shapes::POLYHEDRON_QUADPYRAMID) {
						cupcfd::geometry::shapes::QuadPyramid<T> * shape2;
						this->buildPolyhedron(i, &shape2);
						inside = shape2->isPointInside(point);
						delete shape2;
					}
					else if(pType == cupcfd::geometry::shapes::POLYHEDRON_TRIPRISM) {
						cupcfd::geometry::shapes::TriPrism<T> * shape3;
						this->buildPolyhedron(i, &shape3);
						inside = shape3->isPointInside(point);
						delete shape3;
					}
					else if(pType == cupcfd::geometry::shapes::POLYHEDRON_HEXAHEDRON) {
						cupcfd::geometry::shapes::Hexahedron<T> * shape4;
						this->buildPolyhedron(i, &shape4);																
						inside = shape4->isPointInside(point);						
						delete shape4;
					}
					else if(pType == cupcfd::geometry::shapes::POLYHEDRON_UNKNOWN) {
						DEBUGGABLE_ERROR; return cupcfd::error::E_ERROR;
					}
					else {
						DEBUGGABLE_ERROR; return cupcfd::error::E_ERROR;
					}
		
					// (c) Test whether the position point is inside the cell - stored in 'inside'
					// (ci) If so, update cell ID and stop
					if(inside) {
						// Local ID is just position in loop
						*localCellID = i;
	
						// Need to retrieve global ID from the connectivity graph
						// Get the Node for the localID
						L node;
						status = this->cellConnGraph->connGraph.getLocalIndexNode(i, &node);
						CHECK_ERROR_CODE(status)
						*globalCellID = this->cellConnGraph->nodeToGlobal[node];
	
						// Exit Loop by exiting function
						return cupcfd::error::E_SUCCESS;
					}
	
					// (cii) If not, let loops continue
				}
	
				// ToDo: Corner cases (point on edge, point on vertex)
	
				// No suitable cell was found in the loop
				DEBUGGABLE_ERROR; return cupcfd::error::E_GEOMETRY_NO_VALID_CELL;
				
			}
		
		}
	}
}
#endif
