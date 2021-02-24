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
 * Contains declarations for the CupCfdAoSMeshBoundary Class
 */

#ifndef CUPCFD_GEOMETRY_MESH_CUPCFD_AOS_MESH_BOUNDARY_INCLUDE_H
#define CUPCFD_GEOMETRY_MESH_CUPCFD_AOS_MESH_BOUNDARY_INCLUDE_H

#include "CupCfdAoSMeshFace.h"
#include "EuclideanVector.h"

// Use to pad the class to a certain size (though this may depend on compiler behaviours and padding etc)
#ifndef CUPCFD_AOS_MESH_BOUNDARY_PADDING
#define CUPCFD_AOS_MESH_BOUNDARY_PADDING 0
#endif

namespace euc = cupcfd::geometry::euclidean;

namespace cupcfd
{
	namespace geometry
	{
		namespace mesh
		{
			// Forward Declaration to resolve circular dependency....
			template <class I, class T>
			class CupCfdAoSMeshFace;

			/**
			 * Class for storing boundary data in a CupCfdAoSMesh
			 */
			template <class I, class T>
			class CupCfdAoSMeshBoundary
			{
				public:
					// === Members ===

					/** Face that this boundary is associated with. A -1 indicates no association **/
					I faceID;

					/** Vertices that this boundary is associated with. A -1 indicates no association **/
					I verticesID[4];

					/** The region that this boundary is assocaited with. A -1 indicates no association **/
					I regionID;

					/** The boundary distance **/
					T distance;

					/** Boundary yplus storage **/
					T yplus;

					/** Boundary uplus storage **/
					T uplus;

					/** Boundary shear vector **/
					euc::EuclideanVector<T,3> shear;

					/** Boundary q value **/
					T q;

					/** Boundary h value **/
					T h;

					/** Boundary t value **/
					T t;

					// Need about 3 + 1 *3 + 3*3 doubles in total for class size?
					/** Padding to increase boundary size. Set by CUPCFD_AOS_MESH_BOUNDARY_PADDING compile time define. Default of 0 **/
					char padding[CUPCFD_AOS_MESH_BOUNDARY_PADDING];

					// === Constructors/Deconstructors

					/**
					 * Default Constructor - Set values to defaults of 0, association mappings to -1.
					 *
					 * @tparam I The type of the indexing scheme (integer based)
					 * @tparam T The type of the stored array data
					 */
					CupCfdAoSMeshBoundary();

					/**
					 * Deconstructor.
					 *
					 * @tparam I The type of the indexing scheme (integer based)
					 * @tparam T The type of the stored array data
					 */
					~CupCfdAoSMeshBoundary();

					// === Concrete Methods ===

					// === Pure Virtual Methods ===
			};
		}
	}
}

// Include Header Level Definitions
#include "CupCfdAoSMeshBoundary.ipp"

#endif
