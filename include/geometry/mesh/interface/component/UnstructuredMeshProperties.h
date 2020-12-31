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
 * Declarations for the UnstructuredMeshProperties Class
 */

#ifndef CUPCFD_GEOMETRY_UNSTRUCTURED_MESH_PROPERTIES_INCLUDE_H
#define CUPCFD_GEOMETRY_UNSTRUCTURED_MESH_PROPERTIES_INCLUDE_H

#include "Error.h"

namespace cupcfd
{
	namespace geometry
	{
		namespace mesh
		{
			/**
			 * This class stores the general structural properties of
			 * an unstructured mesh.
			 *
			 * @tparam I Type of mesh index scheme
			 * @tparam T Type of mesh euclidean space
			 */
			template <class I, class T>
			class UnstructuredMeshProperties
			{
				public:
					// === Members ===

					/** Global Number of Cells in Mesh **/
					I nCells;

					/** Global Number of Faces in Mesh **/
					I nFaces;

					/** Global Number of Vertices in Mesh **/
					I nVertices;

					/** Global Number of Boundaries in Mesh **/
					I nBoundaries;

					/** Global Number of Regions in Mesh **/
					I nRegions;

					/** Maximum number of faces per cell **/
					I nMaxFaces;

					/** Scale factor of mesh **/
					T scaleFactor;

					/** Number of Locally Owned Cells on this process **/
					I lOCells;

					/** Number of Local Ghost Cells on this process **/
					I lGhCells;

					/** Number of Local Total Cells on this process **/
					I lTCells;

					/** Number of Local Faces on this process **/
					I lFaces;

					/** Number of Local Vertices on this process **/
					I lVertices;

					/** Number of Local Boundaries on this process **/
					I lBoundaries;

					/** Number of Local Regions on this process **/
					I lRegions;

					// === Constructor/Deconstructors ===

					/**
					 * Default constructor. Sets up all properties to be zero.
					 *
					 * @tparam I Type of mesh index scheme
					 * @tparam T Type of mesh euclidean space
					 */
					UnstructuredMeshProperties();


					/**
					 * Constructor:
					 * Create a new object that has the values set to that of the source provided.
					 *
					 * @param source The object to copy the values from
					 *
					 * @tparam I Type of mesh index scheme
					 * @tparam T Type of mesh euclidean space
					 */
					UnstructuredMeshProperties(UnstructuredMeshProperties<I,T>& source);

					/**
					 * Creates and initialises the values of the UnstructuredMeshProperties
					 * object to those provided.
					 *
					 * @param nCells Number of global cells
					 * @param nFaces Number of global faces
					 * @param nVertices Number of global Vertices
					 * @param nBoundaries Number of global boundaries
					 * @param nRegions Number of regions
					 * @param nMaxFaces Maximum number of faces per cell
					 * @param scaleFactor Mesh scalefactor
					 * @param lOCells Number of local, rank owned, cells
					 * @param lGhCells Number of ghost cells
					 * @param lTCells Number of local total cells
					 * @param lFaces Number of local faces
					 * @param lVertices Number of local vertices
					 * @param lBoundaries Number of local boundaries
					 * @param lRegions Number of local regions
					 *
					 * @tparam I Type of mesh index scheme
					 * @tparam T Type of mesh euclidean space
					 */
					UnstructuredMeshProperties(I nCells, I nFaces, I nVertices, I nBoundaries,
								   I nRegions, I nMaxFaces, T scaleFactor,
								   I lOCells, I lGhCells, I lTCells, I lFaces,
								   I lVertices, I lBoundaries, I lRegions);

					/**
					 * Default Deconstructor. Currently does nothing.
					 *
					 * @tparam I Type of mesh index scheme
					 * @tparam T Type of mesh euclidean space
					 */
					~UnstructuredMeshProperties();

					// === Concrete Methods ===

					/**
					 * Reset the Mesh Properties Object to default values.
					 * The default values for all variables is 0.
					 *
					 * @tparam I Type of mesh index scheme
					 * @tparam T Type of mesh euclidean space
					 */
					void reset();

					/**
					 * Deep copy of properties values from source to this object
					 *
					 * @param source The properties object to copy the values from
					 *
					 * @tparam I Type of mesh index scheme
					 * @tparam T Type of mesh euclidean space
					 */
					virtual void operator=(UnstructuredMeshProperties<I,T>& source);

					/**
					 * Clone this object, creating a new object with the same values
					 *
					 * @tparam I Type of mesh index scheme
					 * @tparam T Type of mesh euclidean space
					 *
					 * @return A pointer to the cloned object
					 */
					__attribute__((warn_unused_result))
					virtual UnstructuredMeshProperties<I,T> * clone();

					// ToDo: Getters and setters
			};
		}
	} // namespace geometry
} // namespace cupcfd

// Include Header Level Definitions
#include "UnstructuredMeshProperties.ipp"

#endif
