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
 * Contains declarations for the CupCfdAoSMeshRegion Class.
 */

#ifndef CUPCFD_GEOMETRY_MESH_CUPCFD_AOS_MESH_REGION_INCLUDE_H
#define CUPCFD_GEOMETRY_MESH_CUPCFD_AOS_MESH_REGION_INCLUDE_H

// Use to pad the class to a certain size (though this may depend on compiler behaviours and padding etc)
#ifndef CUPCFD_AOS_MESH_REGION_PADDING
#define CUPCFD_AOS_MESH_REGION_PADDING 0
#endif

#include "UnstructuredMeshInterface.h"
#include "EuclideanVector.h"
#include <string>

namespace cupcfd
{
	namespace geometry
	{
		namespace mesh
		{
			template <class I, class T>
			class CupCfdAoSMeshRegion
			{
				public:
					// === Members ===

					/** Define boundary type of the region - e.g. wall **/
					RType rType;

					/** Region std value **/
					bool std;

					/** Region flux value **/
					bool flux;

					/** Region adaiab value **/
					bool adiab;

					/** Region ylog value **/
					T ylog;

					/** Region elog value **/
					T elog;

					/** Region density value **/
					T density;

					/** Region turbKE value **/
					T turbKE;

					/** Region turbDiss value **/
					T turbDiss;

					/** Region splvl value **/
					T splvl;

					/** Region den value **/
					T den;

					/** Region r value **/
					T r;

					/** Region t value **/
					T t;

					/** Region force tangent vector **/
					cupcfd::geometry::euclidean::EuclideanVector<T,3> forceTangent;

					/** Region uvw vector **/
					cupcfd::geometry::euclidean::EuclideanVector<T,3> uvw;

					// ToDo: Fix size of this to cap/make char?
					// As a string, will be a pointer that will not increase the size of the class
					// which could be different behaviour.
					// However, regions are few so it's v. unlikely to have any impact, and this is more
					// convenient.
					/** Region Name **/
					std::string regionName;

					/**
					 * Padding to increase the class size - sizeof(bytes) * CUPCFD_AOS_MESH_REGION_PADDING, where
					 * CUPCFD_AOS_MESH_REGION_PADDING is defined at compile time.
					 * CUPCFD_AOS_MESH_REGION_PADDING has a default value of 0.
					 */
					char padding[CUPCFD_AOS_MESH_REGION_PADDING];

					// === Constructors/Deconstructors

					/**
					 * Default constructor.
					 * Sets association references to -1, and other values to 0/empty/null equivalents.
					 * Booleans are set to false.
					 *
					 * @tparam I The type of the indexing scheme (integer based)
					 * @tparam T The type of the stored array data
					 */
					CupCfdAoSMeshRegion();

					/**
					 * Deconstructor.
					 */
					~CupCfdAoSMeshRegion();

					// === Concrete Methods ===

					// === Pure Virtual Methods ===
			};
		}
	}
}

// Include Header Level Definitions
#include "CupCfdAoSMeshRegion.ipp"

#endif
