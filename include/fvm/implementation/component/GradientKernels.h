/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 * These kernels were ported to C++/derived from Dolfyn:
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
 * Contains the kernel declarations for finite volume gradient operations
 */

#ifndef CUPCFD_FVM_GRADIENT_INCLUDE_H
#define CUPCFD_FVM_GRADIENT_INCLUDE_H

#include "EuclideanVector.h"
#include "UnstructuredMeshInterface.h"
#include "Error.h"

namespace cupcfd
{
	namespace fvm
	{
		/**
		 * Compute the gradient of the cell by interpolating at the faces.
		 * Kernel taken from Dolfyn.
		 *
		 * @tparam M The implementing class of the UnstructuredMeshInterface
		 * @tparam I The datatype of the indexing scheme
		 * @tparam T The datatype of computation/mesh/stateful data
		 * @tparam L The label datatype of the unstructured mesh
		 */
		template <class M, class I, class T, class L>
		cupcfd::error::eCodes GradientPhiGaussDolfyn(cupcfd::geometry::mesh::UnstructuredMeshInterface<M,I,T,L>& mesh, I nGradient,
													T * phiCell, I nPhiCell,
													T * phiBoundary, I nPhiBoundary,
													cupcfd::geometry::euclidean::EuclideanVector<T,3> * dPhidxCell, I nDPhidxCell,
													cupcfd::geometry::euclidean::EuclideanVector<T,3> * dPhidxoCell, I nDPhidxoCell);
	}
}

// Include Header Level Definitions
#include "GradientKernels.ipp"

#endif
