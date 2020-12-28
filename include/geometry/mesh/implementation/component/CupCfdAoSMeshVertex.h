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
 * Contains declarations for the CupCfdAoSMeshVertex Class
 */

#include "EuclideanPoint.h"

#ifndef CUPCFD_GEOMETRY_MESH_CUPCFD_AOS_MESH_VERTEX_INCLUDE_H
#define CUPCFD_GEOMETRY_MESH_CUPCFD_AOS_MESH_VERTEX_INCLUDE_H

// Use to pad the class to a certain size (though this may depend on compiler behaviours and padding etc)
#ifndef CUPCFD_AOS_MESH_VERTEX_PADDING
#define CUPCFD_AOS_MESH_VERTEX_PADDING 0
#endif

#include "EuclideanVector.h"

namespace cupcfd
{
	namespace geometry
	{
		namespace mesh
		{
			template <class T>
			class CupCfdAoSMeshVertex
			{
				public:
					// === Members ===

					/** Vertex position in euclidean space from origin of (0,0,0) **/
					cupcfd::geometry::euclidean::EuclideanPoint<T,3> pos;

					/**
					 * Padding to increase class byte size - sizeof(char) * CUPCFD_AOS_MESH_VERTEX_PADDING
					 * where CUPCFD_AOS_MESH_VERTEX_PADDING is defined at compile type.
					 * Default size of CUPCFD_AOS_MESH_VERTEX_PADDING is 0.
					 */
					char padding[CUPCFD_AOS_MESH_VERTEX_PADDING];

					// === Constructors/Deconstructors

					/**
					 * Default constructor.
					 * Sets position to default origin of (0,0,0)
					 *
					 * @tparam I The type of the indexing scheme (integer based)
					 * @tparam T The type of the stored array data
					 */
					CupCfdAoSMeshVertex();

					/**
					 * Deconstructor.
					 */
					~CupCfdAoSMeshVertex();

					// === Concrete Methods ===

					// === Pure Virtual Methods ===
			};
		}
	}
}

// Include Header Level Definitions
#include "CupCfdAoSMeshVertex.ipp"

#endif
