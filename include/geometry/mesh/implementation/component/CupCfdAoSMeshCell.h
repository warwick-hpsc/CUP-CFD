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
 * Contains the declarations for the CupCfdAoSMeshCell Class
 */

#ifndef CUPCFD_GEOMETRY_MESH_CUPCFD_AOS_MESH_CELL_INCLUDE_H
#define CUPCFD_GEOMETRY_MESH_CUPCFD_AOS_MESH_CELL_INCLUDE_H

// Use to pad the class to a certain size (though this may depend on compiler behaviours and padding etc)
#ifndef CUPCFD_AOS_MESH_CELL_PADDING
#define CUPCFD_AOS_MESH_CELL_PADDING 0
#endif

#include "EuclideanPoint.h"

namespace cupcfd
{
	namespace geometry
	{
		namespace mesh
		{
			/**
			 * Store cell data for an AoS format
			 */
			template <class I, class T>
			class CupCfdAoSMeshCell
			{
				public:
					// === Members ===
					/** Cell Center **/
					cupcfd::geometry::euclidean::EuclideanPoint<T,3> center;

					/** Cell Volume **/
					T vol;

					/** Cell Padding to inflate class byte size. Size determined by value of CUPCFD_AOS_MESH_CELL_PADDING
					 *  at compile time. Default of 0.
					 */
					char padding[CUPCFD_AOS_MESH_CELL_PADDING];

					// === Constructors/Deconstructors
					/**
					 * Default constructor. Sets all cell members to 0 or empty/null equivalents
					 *
					 * @tparam I The type of the indexing scheme (integer based)
					 * @tparam T The type of the stored array data
					 */
					CupCfdAoSMeshCell();

					/**
					 * Deconstructor
					 *
					 * @tparam I The type of the indexing scheme (integer based)
					 * @tparam T The type of the stored array data
					 */
					virtual ~CupCfdAoSMeshCell();


					/**
					 * Deep copies the cell components from the source to this object
					 *
					 * @param source The source cell to copy from
					 *
					 * @tparam I The type of the indexing system
					 * @tparam T The type of the data store
					 *
					 * @return Nothing
					 */
					inline void operator=(const CupCfdAoSMeshCell<I,T>& source)
					{
						this->center = source.center;
						this->vol = source.vol;

						// We don't care about the contents of padding, it is used for space, not valid data
					}

					// === Concrete Methods ===

					// === Pure Virtual Methods ===
			};
		}
	}
}

// Include Header Level Definitions
#include "CupCfdAoSMeshCell.ipp"

#endif
