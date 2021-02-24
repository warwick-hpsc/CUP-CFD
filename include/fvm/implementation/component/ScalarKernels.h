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
 * @section DESCRIPTION
 *
 * Contains header level definitions for the Mass Kernels Operations
 */


#ifndef CUPCFD_FVM_SCALAR_INCLUDE_H
#define CUPCFD_FVM_SCALAR_INCLUDE_H

#include "EuclideanVector.h"
#include "UnstructuredMeshInterface.h"

namespace cupcfd
{
	namespace fvm
	{
		/**
		 *
		 *
		 * @tparam M The implementing class of the UnstructuredMeshInterface
		 * @tparam I The datatype of the indexing scheme
		 * @tparam T The datatype of computation/mesh/stateful data
		 * @tparam L The label datatype of the unstructured mesh
		 */
		template <class M, class I, class T, class L>
		__attribute__((warn_unused_result))
		cupcfd::error::eCodes FluxScalarDolfynFaceLoop(cupcfd::geometry::mesh::UnstructuredMeshInterface<M,I,T,L>& mesh,
														T * PhiCell, I nPhiCell,
														T * PhiBoundary, I nPhiBoundary,
														T * VisEff, I nVisEff,
														T * Au, I nAu,
														T * Su, I nSu,
														T * Den, I nDen,
														T * MassFlux, I nMassFlux,
														T * TE, I nTE,
														T * CpBoundary, I nCpBoundary,
														T * visEffBoundary, I nVisEffBoundary,
														T * RFace, I nRFace,
														cupcfd::geometry::euclidean::EuclideanVector<T,3> * dPhidx, int ndPhidx,
														bool SolveTurb, bool SolveEnthalpy,
														// T sigma, T sigma2, 
														T vislam,
														int ivar, int VarT, T Sigma_T, T Prandtl,
														int VarTE, T Sigma_k, int VarED, T Sigma_e,
														T Sigma_s, T Schmidt, T GammaBlend, T Small, T Large, T TMCmu);
	}
}

// Include Header Level Definitions
#include "ScalarKernels.ipp"

#endif
