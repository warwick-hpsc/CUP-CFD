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
 * Contains declarations for the CupCfdAoSMeshFace Class
 */

#ifndef CUPCFD_GEOMETRY_MESH_CUPCFD_AOS_MESH_FACE_INCLUDE_H
#define CUPCFD_GEOMETRY_MESH_CUPCFD_AOS_MESH_FACE_INCLUDE_H

// Use to pad the class to a certain size (though this may depend on compiler behaviours and padding etc)
#ifndef CUPCFD_AOS_MESH_FACE_PADDING
#define CUPCFD_AOS_MESH_FACE_PADDING 0
#endif

#include "CupCfdAoSMeshBoundary.h"
#include "EuclideanPoint.h"
#include "EuclideanVector.h"

namespace cupcfd
{
	namespace geometry
	{
		namespace mesh
		{
			/**
			 * Store Face Data for an AoS Format
			 */
			template <class I, class T>
			class CupCfdAoSMeshFace
			{
				public:
					// === Members ===

					/** Local Reference ID for an associated boundary. A -1 indicates no association/non-boundary face  **/
					I bndID;

					/** Local Reference ID for the first associated cell to this face. A -1 indicates no association **/
					I cell1ID;

					/** Local Reference ID for the second associated cell to this face. A -1 indicates no association/a boundary face **/
					I cell2ID;

					/** Store up to 4 vertices to capture the boundary's position in euclidean space. A -1 indicates no association
					 *  (e.g.  verticesID[0] > -1, verticesID[1] > -1, verticesID[2] > -1, verticesID[3] == -1 indicates only three vertices,
					 *  a triangular boundary).
					 **/
					I verticesID[4];

					/** The boundary lambda value **/
					T lambda;

					/** The boundary rlencos value **/
					T rlencos;

					/** The boundary area **/
					T area;

					/** The boundary center **/
					cupcfd::geometry::euclidean::EuclideanPoint<T,3> center;

					/** The boundary normal vector **/
					cupcfd::geometry::euclidean::EuclideanVector<T,3> norm;

					/** The boundary xpac storage vector **/
					cupcfd::geometry::euclidean::EuclideanPoint<T,3> xpac;

					/** The boundary xnac storage vector **/
					cupcfd::geometry::euclidean::EuclideanPoint<T,3> xnac;

					/** Padding to boost the class byte size. Size determined by sizeof(char) * CUPCFD_AOS_MESH_FACE_PADDING, where
					 * CUPCFD_AOS_MESH_FACE_PADDING is defined at compile type.
					 * Default value of CUPCFD_AOS_MESH_FACE_PADDING is 0.
					 */
					char padding[CUPCFD_AOS_MESH_FACE_PADDING];

					// === Constructors/Deconstructors

					/**
					 * Default constructor. Sets all association references to -1, and values to 0 or null/empty equivalents.
					 *
					 * @tparam I The type of the indexing scheme (integer based)
					 * @tparam T The type of the stored array data
					 */
					CupCfdAoSMeshFace();

					/**
					 * Deconstructor
					 */
					virtual ~CupCfdAoSMeshFace();

					// === Concrete Methods ===

					// === Pure Virtual Methods ===
			};
		}
	}
}

// Include Header Level Definitions
#include "CupCfdAoSMeshFace.ipp"

#endif
