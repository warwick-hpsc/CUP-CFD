/*
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
 *
 * @section DESCRIPTION
 *
 * Declarations for Finite Volume Viscosity operations
 */

#ifndef CUPCFD_FVM_VISCOSITY_KERNELS_INCLUDE_H
#define CUPCFD_FVM_VISCOSITY_KERNELS_INCLUDE_H

#include "UnstructuredMeshInterface.h"

namespace cupcfd
{
	namespace fvm
	{
		template <class M, class I, class T, class L>
		cupcfd::error::eCodes calculateViscosityDolfynCellLoop1(cupcfd::geometry::mesh::UnstructuredMeshInterface<M,I,T,L>& mesh,
																T small,
																T cmu,
																T visURF,
																T visLam,
																T * TE, I nTE,
																T * ED, I nED,
																T * den, I nDen,
																T * visEff, I nVisEff);

		template <class M, class I, class T, class L>
		void calculateViscosityDolfynRegionLoop(T kappa, cupcfd::geometry::mesh::UnstructuredMeshInterface<M,I,T,L>& mesh);

		template <class M, class I, class T, class L>
		cupcfd::error::eCodes calculateViscosityDolfynBoundaryLoop(cupcfd::geometry::mesh::UnstructuredMeshInterface<M,I,T,L>& mesh,
																	T tmcmu, T large, T small, T kappa, T visLam,
																	T * TE, I nTE,
																	T * den, I nDen,
																	T * visEffCell, I nVisEffCell,
																	T * visEffBoundary, I nVisEffBoundary);

		template <class M, class I, class T, class L>
		cupcfd::error::eCodes calculateViscosityDolfynCellLoop2(cupcfd::geometry::mesh::UnstructuredMeshInterface<M,I,T,L>& mesh,
																T visLam,
																T * visEffCell, I nVisEffCell,
																T * visEffBoundary, I nVisEffBoundary);
	}
}

// Include Header Definitions
#include "ViscosityKernels.ipp"

#endif
