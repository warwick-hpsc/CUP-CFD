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
 */

#include <cstdlib>

#include "CupCfdAoSMesh.h"
#include "CupCfdAoSMeshCell.h"
#include "CupCfdAoSMeshBoundary.h"
#include "CupCfdAoSMeshFace.h"
#include "CupCfdAoSMeshRegion.h"
#include "CupCfdAoSMeshVertex.h"

#include "ArrayDrivers.h"
#include "SortDrivers.h"

#include <iostream>

#include "ExchangePattern.h"
#include "ExchangePatternConfig.h"

namespace cupcfd
{
	namespace geometry
	{
		namespace mesh
		{
			template <class I, class T, class L>
			CupCfdAoSMesh<I,T,L>::CupCfdAoSMesh(cupcfd::comm::Communicator& comm)
			:UnstructuredMeshInterface<CupCfdAoSMesh<I,T,L>,I,T,L>(comm)
			{
				// Parent Constructor Sets Properties to Defaults

				// Data stores initially already empty as needed
			}

			template <class I, class T, class L>
			CupCfdAoSMesh<I,T,L>::~CupCfdAoSMesh()
			{
				// Nothing to do, vectors should clean up mesh objects automatically
			}

			// ============ Inherited Overloaded Methods  ============

			// === Cell Operators ===
			template <class I, class T, class L>
			cupcfd::error::eCodes CupCfdAoSMesh<I,T,L>::addCell(
						L cellLabel,
						cupcfd::geometry::euclidean::EuclideanPoint<T,3>& center,
						T vol,
						bool isLocal) {
				// Can only add if not finalized
				if(this->finalized) {
					return cupcfd::error::E_FINALIZED;
				}

				// Check Cell bID does not already exist
				if(this->cellBuildIDToLocalID.find(cellLabel) != this->cellBuildIDToLocalID.end()) {
					return cupcfd::error::E_MESH_EXISTING_CELL;
				}

				cupcfd::error::eCodes status;

				// Create a cell object
				CupCfdAoSMeshCell<I, T> cell;

				// Copy the cell center
				cell.center = center;

				// Copy the cell volume
				cell.vol = vol;

				// Add the cell data to a local store
				// Note: This will need to be reordered according to the order of the
				// connectivity graph later.
				this->cells.push_back(cell);

				// Add Cell Local Face Count, initialise to 0
				this->cellNFaces.push_back(0);

				// Add Cell Global Face Count, initalise to 0
				this->cellNGFaces.push_back(0);

				// Add Cell Local Vertex Count, initialise to 0
				this->cellNVertices.push_back(0);

				// Add Cell Global Vertex Count, initialise to 0
				this->cellNGVertices.push_back(0);

				// Update the mesh properties
				this->properties.lTCells = this->properties.lTCells + 1;

				if(isLocal) {
					this->properties.lOCells = this->properties.lOCells + 1;

					// Add the cell to the connectivity graph
					status = this->cellConnGraph->addLocalNode(cellLabel);
					CHECK_ERROR_CODE(status)
				}
				else {
					this->properties.lGhCells = this->properties.lGhCells + 1;

					// Add the cell to the connectivity graph
					status = this->cellConnGraph->addGhostNode(cellLabel);
					CHECK_ERROR_CODE(status)
				}

				// Store the current 'local ID' - this will ultimately be updated to
				// the local ID inside the connectivity graph.
				// However, the current ID in the graph may change after adding
				// subsequent cells, so we cannot set it properly now.
				this->cellBuildIDToLocalID[cellLabel] = this->cells.size() - 1;

				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			cupcfd::error::eCodes CupCfdAoSMesh<I,T,L>::addCell(L cellLabel, bool isLocal) {
				// Create empty center, vol for now
				cupcfd::geometry::euclidean::EuclideanPoint<T,3> center(T(0), T(0), T(0));
				T vol = T(0);

				// Pass to more descriptive function
				this->addCell(cellLabel, center, vol, isLocal);
				return cupcfd::error::E_SUCCESS;
			}

			// === Vertex Operators ===

			template <class I, class T, class L>
			cupcfd::error::eCodes CupCfdAoSMesh<I,T,L>::addVertex(L vertexLabel, cupcfd::geometry::euclidean::EuclideanPoint<T,3>& pos) {
				// Can only add if not finalized
				if(this->finalized == true) {
					return cupcfd::error::E_FINALIZED;
				}

				// Check the vertex doesn't already exist
				if(this->vertexBuildIDToLocalID.find(vertexLabel) != this->vertexBuildIDToLocalID.end()) {
					return cupcfd::error::E_MESH_EXISTING_VERTEX;
				}

				// Vertex Object
				CupCfdAoSMeshVertex<T> vertex;

				// Copy Position
				vertex.pos = pos;

				// Store Vertex in Mesh Local Store
				this->vertices.push_back(vertex);

				// Update Mesh Properties
				this->properties.lVertices = this->properties.lVertices + 1;

				// Store a mapping of the Build ID to the Local ID
				this->vertexBuildIDToLocalID[vertexLabel] = this->vertices.size() - 1;

				return cupcfd::error::E_SUCCESS;
			}

			// === Region Operators ===

			template <class I, class T, class L>
			cupcfd::error::eCodes CupCfdAoSMesh<I,T,L>::addRegion(L regionLabel,
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
				// Can only add if not finalized
				if(this->finalized == true) {
					return cupcfd::error::E_FINALIZED;
				}

				// Check Region bID does not already exist
				if(this->regionBuildIDToLocalID.find(regionLabel) != this->regionBuildIDToLocalID.end()) {
					return cupcfd::error::E_MESH_EXISTING_REGION;
				}

				// Region Object
				CupCfdAoSMeshRegion<I,T> region;
				region.rType = type;
				region.flux = flux;
				region.adiab = adiab;
				region.std = std;
				region.ylog = ylog;
				region.elog = elog;
				region.density = density;
				region.turbKE = turbKE;
				region.turbDiss = turbDiss;
				region.splvl = splvl;
				region.den = den;
				region.forceTangent = forceTangent;
				region.uvw = uvw;
				region.regionName = regionName;
				region.r = r;
				region.t = t;

				// Add to local store
				this->regions.push_back(region);

				// Update Mesh Properties
				this->properties.lRegions = this->properties.lRegions + 1;

				// Store Mapping from build ID to local ID
				regionBuildIDToLocalID[regionLabel] = this->regions.size() - 1;

				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			cupcfd::error::eCodes CupCfdAoSMesh<I,T,L>::addRegion(
								L regionLabel,
								std::string& regionName) {
				RType type = RTYPE_DEFAULT;

				cupcfd::geometry::euclidean::EuclideanVector<T,3> forceTangent(T(0), T(0), T(0));
				cupcfd::geometry::euclidean::EuclideanVector<T,3> uvw(T(0), T(0), T(0));

				// Pass defaults through to more detailed method
				return this->addRegion(regionLabel, type, false, false, false, T(0), T(0), T(0), T(0), T(0), T(0), T(0), T(0), T(0),
									   forceTangent, uvw, regionName);
			}

			// === Boundary Operators ===

			template <class I, class T, class L>
			cupcfd::error::eCodes CupCfdAoSMesh<I,T,L>::addBoundary(
					L boundaryLabel,
					L regionLabel,
					L * vertexLabels, I nVertexLabels,
					T distance) {
				// Can only add if not finalized
				if(this->finalized == true) {
					return cupcfd::error::E_FINALIZED;
				}

				// Check Boundary bID does not already exist
				if(this->boundaryBuildIDToLocalID.find(boundaryLabel) != this->boundaryBuildIDToLocalID.end()) {
					return cupcfd::error::E_MESH_EXISTING_BOUNDARY;
				}

				// Create the object
				CupCfdAoSMeshBoundary<I,T> boundary;

				// Initially the faceID is unknown
				// It will be updated as part of the finalize stage based on the face->boundary mappings
				boundary.faceID = -1;

				// Check the vertex count is valid
				// Currently, we only handle 3-4 vertex faces/boundaries
				if(nVertexLabels < 3 || nVertexLabels > 4) {
					return cupcfd::error::E_MESH_INVALID_VERTEX_COUNT;
				}

				// Copy the vertexIDs, if they exist
				for(I i = 0; i < nVertexLabels; i++) {
					// Check the vertex exists
					if(this->vertexBuildIDToLocalID.find(vertexLabels[i]) == this->vertexBuildIDToLocalID.end()) {
						return cupcfd::error::E_MESH_INVALID_VERTEX_LABEL;
					}

					// Point Boundary at Vertex Local ID
					boundary.verticesID[i] = vertexBuildIDToLocalID[vertexLabels[i]];
				}

				// If only 3 vertices, set the 4th to -1
				if(nVertexLabels == 3) {
					boundary.verticesID[3] = -1;
				}

				// Copy distance
				boundary.distance = distance;

				// Default to 0
				boundary.yplus = T(0);

				// Default to 0
				boundary.uplus = T(0);

				// Check the Region ID Exists
				if(this->regionBuildIDToLocalID.find(regionLabel) == this->regionBuildIDToLocalID.end()) {
					return cupcfd::error::E_MESH_INVALID_REGION_LABEL;
				}

				// Set the internal boundary region ID to point at the local region ID
				boundary.regionID = this->regionBuildIDToLocalID[regionLabel];

				// Default to 0
				boundary.shear = cupcfd::geometry::euclidean::EuclideanVector<T,3>(T(0), T(0), T(0));
				boundary.q = T(0);
				boundary.h = T(0);
				boundary.t = T(0);

				this->boundaries.push_back(boundary);
				this->properties.lBoundaries = this->properties.lBoundaries + 1;
				this->boundaryBuildIDToLocalID[boundaryLabel] = boundaries.size() - 1;

				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			cupcfd::error::eCodes CupCfdAoSMesh<I,T,L>::addBoundary(
					L boundaryLabel,
					L regionLabel,
					L * vertexLabels, I nVertexLabels) {
				T distance = 0.0;

				// Passthrough to more detailed function
				return this->addBoundary(boundaryLabel, regionLabel, vertexLabels, nVertexLabels, distance);
			}

			// === Face Operators ===

			template <class I, class T, class L>
			cupcfd::error::eCodes CupCfdAoSMesh<I,T,L>::addFace(
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
				cupcfd::error::eCodes status;

				// Can only add if not finalized
				if(this->finalized == true) {
					return cupcfd::error::E_FINALIZED;
				}

				// Check Face bID does not already exist
				if(this->faceBuildIDToLocalID.find(faceLabel) != this->faceBuildIDToLocalID.end()) {
					return cupcfd::error::E_MESH_EXISTING_FACE;
				}

				// Create Object
				CupCfdAoSMeshFace<I, T> face;

				// Check Cell 1 exists
				if(this->cellBuildIDToLocalID.find(cell1Label) == this->cellBuildIDToLocalID.end()) {
					return cupcfd::error::E_MESH_INVALID_CELL_LABEL;
				}

				// Point the face at the (current) local Cell ID for the given Build Cell ID
				face.cell1ID = this->cellBuildIDToLocalID[cell1Label];


				if(isBoundary) {
					// Check the Boundary ID exists
					if(this->boundaryBuildIDToLocalID.find(cell2OrBoundaryLabel) == this->boundaryBuildIDToLocalID.end()) {
						return cupcfd::error::E_MESH_INVALID_BOUNDARY_LABEL;
					}

					// Set the boundary ID to point at the local ID of the provided build boundary ID
					face.bndID = this->boundaryBuildIDToLocalID[cell2OrBoundaryLabel];

					// Boundary Face, fix cell 2 to -1 (i.e. no cell)
					face.cell2ID = -1;
				}
				else {
					// Check the Cell ID exists
					if(this->cellBuildIDToLocalID.find(cell2OrBoundaryLabel) == this->cellBuildIDToLocalID.end()) {
						return cupcfd::error::E_MESH_INVALID_CELL_LABEL;
					}

					// Check this is not a face between two ghost cells
					// We only accept faces between local->local, or local->ghost cells
					// Since this graph shouldn't be finalised yet, the data will be stored in the build graph
					if(!isBoundary) {
						bool foundGhost1, foundGhost2;
						status = this->cellConnGraph->existsGhostNode(cell1Label, &foundGhost1);
						CHECK_ERROR_CODE(status)
						status = this->cellConnGraph->existsGhostNode(cell2OrBoundaryLabel, &foundGhost2);
						CHECK_ERROR_CODE(status)

						if(foundGhost1 && foundGhost2) {
							return cupcfd::error::E_MESH_INVALID_FACE;
						}
					}

					// Check Edge does not already exist in connectivity graph
					bool found;
					status = this->cellConnGraph->existsEdge(cell1Label, cell2OrBoundaryLabel, &found);
					CHECK_ERROR_CODE(status)
					if(found) {
						return cupcfd::error::E_MESH_FACE_EDGE_EXISTS;
					}

					status = this->cellConnGraph->existsEdge(cell2OrBoundaryLabel, cell1Label, &found);
					CHECK_ERROR_CODE(status)
					if(found) {
						return cupcfd::error::E_MESH_FACE_EDGE_EXISTS;
					}

					// Set the cell 2 ID to point at the local ID of the provided build Cell 2 ID
					face.cell2ID = this->cellBuildIDToLocalID[cell2OrBoundaryLabel];

					// Not a boundary face, fix it's ID to -1 (i.e. no boundary)
					face.bndID = -1;

					// Add the edge to the cell connectivity graph since not a boundary face
					// Use the Build Global IDs for the connectivity graph nodes.
					status = this->cellConnGraph->addUndirectedEdge(cell1Label, cell2OrBoundaryLabel);
					CHECK_ERROR_CODE(status)
					if (status != cupcfd::error::E_SUCCESS) {
						return status;
					}
				}

				// Copy the face lambda
				face.lambda = lambda;

				// Copy the face norm
				face.norm = norm;

				// Check the vertex count is valid
				// Currently, we only handle 3-4 vertex faces/boundaries
				if(nVertexLabels < 3 || nVertexLabels > 4) {
					return cupcfd::error::E_MESH_INVALID_VERTEX_COUNT;
				}

				// Copy the vertexIDs, if they exist
				for(I i = 0; i < nVertexLabels; i++) {
					// Check the vertex exists
					if(this->vertexBuildIDToLocalID.find(vertexLabels[i]) == this->vertexBuildIDToLocalID.end()) {
						return cupcfd::error::E_MESH_INVALID_VERTEX_LABEL;
					}

					// Point Boundary at Vertex Local ID
					face.verticesID[i] = vertexBuildIDToLocalID[vertexLabels[i]];
				}

				// if only 3 vertices, set the 4th to -1
				if(nVertexLabels == 3) {
					face.verticesID[3] = -1;
				}

				// Copy the center
				face.center = center;

				// Copy the rlencos
				face.rlencos = rlencos;

				// Copy the area
				face.area = area;

				// Default xpac to 0
				face.xpac = xpac;

				// Default xnac to 0
				face.xnac = xnac;

				// Add face to local store, saving the addition
				this->faces.push_back(face);

				// Update mesh properties
				this->properties.lFaces = this->properties.lFaces + 1;

				// Store local ID for given build ID
				this->faceBuildIDToLocalID[faceLabel] = this->faces.size() - 1;

				// Update the Local Face Count for Cell 1
				this->cellNFaces[this->cellBuildIDToLocalID[cell1Label]] = this->cellNFaces[this->cellBuildIDToLocalID[cell1Label]] + 1;

				if(!isBoundary) {
					// Update the Local Face Count for Cell 2
					this->cellNFaces[this->cellBuildIDToLocalID[cell2OrBoundaryLabel]]++;
				}

				if(isBoundary) {
					// Update the referenced boundary to point back at this face's local ID
					int localBoundaryID = this->boundaryBuildIDToLocalID[cell2OrBoundaryLabel];
					this->boundaries[localBoundaryID].faceID = this->faceBuildIDToLocalID[faceLabel];
				}

				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			cupcfd::error::eCodes CupCfdAoSMesh<I,T,L>::addFace(
						L faceLabel,
						L cell1Label,
						L cell2OrBoundaryLabel,
						bool isBoundary,
						L * vertexLabels, I nVertexLabels) {
				// Set Defaults
				T lambda = T(0);
				T rlencos = T(0);
				T area = T(0);
				cupcfd::geometry::euclidean::EuclideanVector<T,3> norm(T(0), T(0), T(0));
				cupcfd::geometry::euclidean::EuclideanPoint<T,3> center(T(0), T(0), T(0));
				cupcfd::geometry::euclidean::EuclideanPoint<T,3> xpac(T(0), T(0), T(0));
				cupcfd::geometry::euclidean::EuclideanPoint<T,3> xnac(T(0), T(0), T(0));

				// Passthrough to more detailed function
				return this->addFace(faceLabel, cell1Label, cell2OrBoundaryLabel, isBoundary,
							  lambda, norm, vertexLabels, nVertexLabels, center, xpac, xnac, rlencos, area);
			}

			// === Concrete Methods ===

			template <class I, class T, class L>
			cupcfd::error::eCodes CupCfdAoSMesh<I,T,L>::reset() {
				// Reset Data Stores
				this->boundaries.clear();
				this->cells.clear();
				this->faces.clear();
				this->regions.clear();
				this->vertices.clear();

				this->cellNGFaces.clear();
				this->cellNFaces.clear();
				this->cellNGVertices.clear();
				this->cellNVertices.clear();
				this->cellFaceMapCSRXAdj.resize(0);
				this->cellFaceMapCSRAdj.resize(0);

				// Reset Mesh Properties
				this->properties.reset();

				// Reset to unfinalised
				this->finalized = false;

				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			cupcfd::error::eCodes CupCfdAoSMesh<I,T,L>::updateCellFaceMap() {
				// Reset in case it was in use
				this->cellFaceMapCSRXAdj.clear();
				this->cellFaceMapCSRAdj.clear();

				// Number of locally stored faces per cell already set in this->NFaces, updated in addCell.
				// Use this value to rebuild the cell->face mappings
				I ptr = 0;
				I faceCountSum = 0;

				// Part (1) Store the index lookups in cellFaceMapCSRAdj
				I iLimit;
				iLimit = cupcfd::utility::drivers::safeConvertSizeT<I>(this->cells.size());
				for(I i = 0; i < iLimit; i++) {
					this->cellFaceMapCSRXAdj.push_back(ptr);
					ptr = ptr + this->getCellStoredNFaces(i);

					// We need to know the size of cellFaceMapCSRAdj which is
					// 1 entry per face per cell, which will be
					// greater than the number of faces (lFaces) due to shared faces.
					// Can find this from a sum of stored faces.
					faceCountSum = faceCountSum + this->getCellStoredNFaces(i);
				}
				this->cellFaceMapCSRXAdj.push_back(ptr);

				for(I i = 0; i < faceCountSum; i++) {
					this->cellFaceMapCSRAdj.push_back(I(-1));
				}

				// Store Face IDs in the Cell Mapping Vector
				// Not a guaranteed order within a cell list
				iLimit = cupcfd::utility::drivers::safeConvertSizeT<I>(this->faces.size());
				for(I i = 0; i < iLimit; i++) {
					I faceID = i;

					// Cell 1 ID
					I cell1ID = faces[i].cell1ID;
					I rangeStart = this->cellFaceMapCSRXAdj[cell1ID];
					I rangeEnd = this->cellFaceMapCSRXAdj[cell1ID+1];

					for(I j = rangeStart; j < rangeEnd; j++) {
						if(this->cellFaceMapCSRAdj[j] == -1) {
							this->cellFaceMapCSRAdj[j] = faceID;
							break;
						}
					}

					// Cell 2 if not boundary
					if(faces[i].cell2ID > -1) {
						I cell2ID = faces[i].cell2ID;
						rangeStart = this->cellFaceMapCSRXAdj[cell2ID];
						rangeEnd = this->cellFaceMapCSRXAdj[cell2ID+1];

						for(I j = rangeStart; j < rangeEnd; j++) {
							if(this->cellFaceMapCSRAdj[j] == -1) {
								this->cellFaceMapCSRAdj[j] = faceID;
								break;
							}
						}
					}
				}

				// Tidyup - Not strictly necessary, but we will sort the face IDs within each cell range
				iLimit = cupcfd::utility::drivers::safeConvertSizeT<I>(this->cells.size());
				for(I i = 0; i < iLimit; i++) {
					I rangeStart = this->cellFaceMapCSRXAdj[i];
					I rangeSize = this->cellFaceMapCSRXAdj[i+1] - this->cellFaceMapCSRXAdj[i];

					cupcfd::utility::drivers::merge_sort(&cellFaceMapCSRAdj[rangeStart], rangeSize);
				}

				// Now that the cell -> face map is complete, let us compute the number of locally stored
				// faces and vertices associated with a cell

				// Faces - can derive quickly from CSR mappings
				iLimit = cupcfd::utility::drivers::safeConvertSizeT<I>(this->cells.size());
				for(I i = 0; i < iLimit; i++) {
					this->cellNFaces[i] = this->cellFaceMapCSRXAdj[i+1] - this->cellFaceMapCSRXAdj[i];

					// Temporarily set global nfaces to the same as local stored face counts
					// This should be overridden for ghost cells by the exchange function
					this->cellNGFaces[i] = this->cellNFaces[i];
				}

				// Vertices - Build a list of stored vertices for stored associated faces and remove duplicates
				// for(I i = 0; i < this->cells.size(); i++)
				iLimit = cupcfd::utility::drivers::safeConvertSizeT<I>(this->cells.size());
				for(I i = 0; i < iLimit; i++) {
					std::vector<I> vertexIDs;

					// Number of stored faces
					I cellFaceCount = this->cellNFaces[i];

					for(I j = 0; j < cellFaceCount; j++) {
						I faceID = this->getCellFaceID(i, j);

						for(I k = 0; k < 4; k++) {
							I vertexID = this->getFaceVertex(faceID, k);

							if(vertexID != -1) {
								vertexIDs.push_back(vertexID);
							}
						}
					}

					I nVertices = vertexIDs.size();

					// Store number of distinct vertices local IDs stored for this cell on this rank
					cupcfd::utility::drivers::distinctCount(&vertexIDs[0], nVertices, &(this->cellNVertices[i]));

					// Set the global amount to the same as the local amount - for local cells this is the same,
					// for ghost cells this will be incorrect and should be overridden by an exchange function
					this->cellNGVertices[i] = this->cellNVertices[i];
				}

				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			cupcfd::error::eCodes CupCfdAoSMesh<I,T,L>::updateCellLocalIndexes() {
				// We have two mappings
				// (a) cellBuildIDToLocalID - This maps the build cell ID (e.g. from a file) to the cell data
				// (b) Connectivity Graph.local - This maps the build Cell ID (the node) to a local ID in the graph

				// We wish to reorder the cell data (this->cells) to the same ordering as the local IDs in the
				// connectivity graph, and update cellBuildIDToLocalID and the face->cell mappings accordingly.

				cupcfd::error::eCodes status;

				// First, let us create useful data stores
				// (a) A vector of valid Keys (Build Cell IDs)
				std::vector<I> keys;
				for(typename std::map<I,I>::iterator iter = this->cellBuildIDToLocalID.begin(); iter != this->cellBuildIDToLocalID.end(); iter++) {
					keys.push_back(iter->first);
				}


				// (b) A mapping of the current local cell IDs in the map to their key
				std::map<I,I> localToBuildID;
				I iLimit;
				iLimit = cupcfd::utility::drivers::safeConvertSizeT<I>(keys.size());
				for(I i = 0; i < iLimit; i++) {
					I lID = this->cellBuildIDToLocalID[keys[i]];
					localToBuildID[lID] = keys[i];
				}

				// Now we begin correcting all cell related structures and mappings

				// (1) Remap the face->cell mappings to the graph local IDs
				iLimit = cupcfd::utility::drivers::safeConvertSizeT<I>(faces.size());
				for(I i = 0; i < iLimit; i++) {
					// Update Cell 1 ID by mapping the current local back to the build ID, then to the local ID in the graph
					I bID = localToBuildID[faces[i].cell1ID];
					I graphLocalID;
					status = this->cellConnGraph->connGraph.getNodeLocalIndex(bID, &graphLocalID);
					CHECK_ERROR_CODE(status)
					faces[i].cell1ID = graphLocalID;

					// Get and Update Cell 2 IF this is not a boundary face
					if(faces[i].cell2ID > -1) {
						bID = localToBuildID[faces[i].cell2ID];
						status = this->cellConnGraph->connGraph.getNodeLocalIndex(bID, &graphLocalID);
						CHECK_ERROR_CODE(status)
						faces[i].cell2ID = graphLocalID;
					}
				}

				// ToDo: Shuffle should be converted easily enough into a reusable utility function
				// (2) Shuffle the cell data structures
				// Make a copy
				std::vector<CupCfdAoSMeshCell<I,T>> tmpCells;

				iLimit = cupcfd::utility::drivers::safeConvertSizeT<I>(cells.size());
				for(I i = 0; i < iLimit; i++) {
					tmpCells.push_back(this->cells[i]);
				}

				// Now we reorder using tmpCells as the source
				iLimit = cupcfd::utility::drivers::safeConvertSizeT<I>(keys.size());
				for(I i = 0; i < iLimit; i++) {
					I currentLocalID = this->cellBuildIDToLocalID[keys[i]];
					I newLocalID;

					// Get Graph Local ID
					status = this->cellConnGraph->connGraph.getNodeLocalIndex(keys[i], &newLocalID);
					CHECK_ERROR_CODE(status)

					// Update Map To Point at new Local ID
					this->cellBuildIDToLocalID[keys[i]] = newLocalID;

					// Copy Data to new position at newLocalID
					this->cells[newLocalID] = tmpCells[currentLocalID];
				}

				std::vector<I> tmpVec;

				// (3) Shuffle the cellNFaces data structure
				// Make a copy
				tmpVec.clear();
				iLimit = cupcfd::utility::drivers::safeConvertSizeT<I>(cellNFaces.size());
				for(I i = 0; i < iLimit; i++) {
					tmpVec.push_back(this->cellNFaces[i]);
				}

				// Update the source to shuffled order
				iLimit = cupcfd::utility::drivers::safeConvertSizeT<I>(this->cellNFaces.size());
				for(I i = 0; i < iLimit; i++) {
					I label = localToBuildID[i];
					I graphLocalID;
					status = this->cellConnGraph->connGraph.getNodeLocalIndex(label, &graphLocalID);
					CHECK_ERROR_CODE(status)
					this->cellNFaces[graphLocalID] = tmpVec[i];
				}

				// (4) Shuffle the cellNGFaces data structure
				// Make a copy
				tmpVec.clear();
				iLimit = cupcfd::utility::drivers::safeConvertSizeT<I>(this->cellNGFaces.size());
				for(I i = 0; i < iLimit; i++) {
					tmpVec.push_back(this->cellNGFaces[i]);
				}

				// Update the source to shuffled order
				iLimit = cupcfd::utility::drivers::safeConvertSizeT<I>(this->cellNGFaces.size());
				for(I i = 0; i < iLimit; i++) {
					I label = localToBuildID[i];
					I graphLocalID;
					status = this->cellConnGraph->connGraph.getNodeLocalIndex(label, &graphLocalID);
					CHECK_ERROR_CODE(status)
					this->cellNGFaces[graphLocalID] = tmpVec[i];
				}

				// (5) Shuffle the cellNGVertices data structure
				// Make a copy
				tmpVec.clear();
				iLimit = cupcfd::utility::drivers::safeConvertSizeT<I>(this->cellNGVertices.size());
				for(I i = 0; i < iLimit; i++) {
					tmpVec.push_back(this->cellNGVertices[i]);
				}

				// Update the source to shuffled order
				iLimit = cupcfd::utility::drivers::safeConvertSizeT<I>(this->cellNGVertices.size());
				for(I i = 0; i < iLimit; i++) {
					I label = localToBuildID[i];
					I graphLocalID;
					status = this->cellConnGraph->connGraph.getNodeLocalIndex(label, &graphLocalID);
					CHECK_ERROR_CODE(status)
					this->cellNGVertices[graphLocalID] = tmpVec[i];
				}

				// (6) Shuffle the cellNVertices data structure
				// Make a copy
				tmpVec.clear();
				iLimit = cupcfd::utility::drivers::safeConvertSizeT<I>(this->cellNVertices.size());
				for(I i = 0; i < iLimit; i++) {
					tmpVec.push_back(this->cellNVertices[i]);
				}

				iLimit = cupcfd::utility::drivers::safeConvertSizeT<I>(this->cellNVertices.size());
				for(I i = 0; i < iLimit; i++) {
					I label = localToBuildID[i];
					I graphLocalID;
					status = this->cellConnGraph->connGraph.getNodeLocalIndex(label, &graphLocalID);
					CHECK_ERROR_CODE(status)
					this->cellNVertices[graphLocalID] = tmpVec[i];
				}

				// (7) Shuffle the Cell->Face CSR Data Structure
				// For now, we will not re-shuffle this, and presume that it is rebuilt/built after calling this method
				// by calling updateCellFaceMap

				// (8) Finally, correct the cellBuildIDToLocalID data structure to point at the new
				// mesh local IDs (i.e. the graph local IDs)
				iLimit = cupcfd::utility::drivers::safeConvertSizeT<I>(cellBuildIDToLocalID.size());
				for(I i = 0; i < iLimit; i++) {
					I label = localToBuildID[i];
					I graphCellID;
					status = this->cellConnGraph->connGraph.getNodeLocalIndex(label, &graphCellID);
					CHECK_ERROR_CODE(status)
					this->cellBuildIDToLocalID[label] = graphCellID;
				}

				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			cupcfd::error::eCodes CupCfdAoSMesh<I,T,L>::finalize() {
				cupcfd::error::eCodes status;
				// Now all data has been added, finalize the connectivity graph to ensure all
				// structures are fixed

				// This will be blocking on all members of the graph communicator, and so makes
				// this method also a blocking method

				status = this->cellConnGraph->finalize();
				CHECK_ERROR_CODE(status)
				if(status != cupcfd::error::E_SUCCESS) {
					return cupcfd::error::E_ERROR;
				}

				// Check that every boundary is mapped to a face
				I iLimit;
				iLimit = cupcfd::utility::drivers::safeConvertSizeT<I>(this->boundaries.size());
				for(I i = 0; i < iLimit; i++) {
					if(this->boundaries[i].faceID == -1) {
						return cupcfd::error::E_MESH_UNMAPPED_BOUNDARY_FACE;
					}
				}


				// Now all cells have been added, we need to order the local cell objects
				// to have the same local indexes as in the connectivity graph
				// This also requires an update of the face->cell1 and face->cell2 mappings
				// where appropriate

				// Important - This should be done before Cell->Face Mapping and Cell->NVertices Mapping
				// to ensure that the correct indexes are used for those functions
				this->updateCellLocalIndexes();

				// Most data is stored in AoS structures already. However, we need to update
				// the Cell -> Face Mapping as it is stored in a separate CSR and is not updated
				// by any of the add/set functions due to the performance overheads of doing it
				// element by element (constant inserts, reindexing)
				this->updateCellFaceMap();

				// Exchange the Global NVertices so Ghost Cells have the correct values
				this->exchangeCellGlobalNVertices();

				// Exchange the Global NFace counts so Ghost Cells have the correct values
				this->exchangeCellGlobalNFaces();

				// Update status
				this->finalized = true;

				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			cupcfd::error::eCodes CupCfdAoSMesh<I,T,L>::exchangeCellGlobalNVertices() {
				// Should only be called as part of the finalise step, since it requires the connectivity graph to
				// be set up

				// ToDo: This should be moved out to a configuration item rather than hardcoded.
				cupcfd::comm::ExchangePatternConfig exchangePatternConfig(cupcfd::comm::EXCHANGE_NONBLOCKING_ONE_SIDED);
				cupcfd::comm::ExchangePattern<I> * exchangePattern;

				cupcfd::error::eCodes status;

				// Build the exchange pattern for cells
				status = exchangePatternConfig.buildExchangePattern(&exchangePattern, *(this->cellConnGraph));
				CHECK_ERROR_CODE(status)
				if (status != cupcfd::error::E_SUCCESS) {
					return status;
				}

				// Build an array of the number of vertices for local cells, leave space for ghost cells
				// but their values don't matter at this stage
				I nCells = this->cellConnGraph->nLONodes + this->cellConnGraph->nLGhNodes;

				I * nVertices = (I *) malloc(sizeof(I) * nCells);

				for(I i = 0; i < nCells; i++) {
					// For local cells, stored vertices = global vertices
					// Ghost cell data will be overwritten after exchange
					nVertices[i] = this->getCellStoredNVertices(i);
				}

				// MPI Exchange
				exchangePattern->exchangeStart(nVertices, nCells);
				exchangePattern->exchangeStop(nVertices, nCells);

				// Overwrite the Global Counts for Ghost Cells on this rank with the received data
				for(I i = this->cellConnGraph->nLONodes; i < nCells; i++) {
					this->cellNGVertices[i] = nVertices[i];
				}

				free(nVertices);

				delete exchangePattern;

				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T, class L>
			cupcfd::error::eCodes CupCfdAoSMesh<I,T,L>::exchangeCellGlobalNFaces() {
				// Should only be called as part of the finalise step, since it requires the connectivity graph to
				// be set up

				// ToDo: This should be moved out to a configuration item rather than hardcoded.
				cupcfd::comm::ExchangePatternConfig exchangePatternConfig(cupcfd::comm::EXCHANGE_NONBLOCKING_ONE_SIDED);
				cupcfd::comm::ExchangePattern<I> * exchangePattern;

				cupcfd::error::eCodes status;

				// Build the exchange pattern for cells
				status = exchangePatternConfig.buildExchangePattern(&exchangePattern, *(this->cellConnGraph));
				CHECK_ERROR_CODE(status)
				if (status != cupcfd::error::E_SUCCESS) {
					return status;
				}

				// Build an array of the number of vertices for local cells, leave space for ghost cells
				// but their values don't matter at this stage
				I nCells = this->cellConnGraph->nLONodes + this->cellConnGraph->nLGhNodes;

				I * nFaces = (I *) malloc(sizeof(I) * nCells);

				for(I i = 0; i < nCells; i++) {
					// For local cells, stored vertices = global vertices
					// Ghost cell data will be overwritten after exchange
					nFaces[i] = this->getCellStoredNFaces(i);
				}

				// MPI Exchange
				exchangePattern->exchangeStart(nFaces, nCells);
				exchangePattern->exchangeStop(nFaces, nCells);

				// Overwrite the Global Counts for Ghost Cells on this rank with the received data
				for(I i = this->cellConnGraph->nLONodes; i < nCells; i++) {
					this->cellNGFaces[i] = nFaces[i];
				}

				free(nFaces);
				delete exchangePattern;

				return cupcfd::error::E_SUCCESS;
			}

			// Explicit Instantiation
			template class CupCfdAoSMesh<int, float, int>;
			template class CupCfdAoSMesh<int, double, int>;
		}
	}
}
