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
 *
 * @section DESCRIPTION
 *
 * Contains header level definitions for the Mass Kernels Operations
 */

#ifndef CUPCFD_FVM_MASS_INCLUDE_H
#define CUPCFD_FVM_MASS_INCLUDE_H

#include "UnstructuredMeshInterface.h"
#include "EuclideanVector.h"

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
		cupcfd::error::eCodes FluxMassDolfynFaceLoop(cupcfd::geometry::mesh::UnstructuredMeshInterface<M,I,T,L>& mesh,
													cupcfd::geometry::euclidean::EuclideanVector<T,3> * dudx, I nDudx,
													cupcfd::geometry::euclidean::EuclideanVector<T,3> * dvdx, I nDvdx,
													cupcfd::geometry::euclidean::EuclideanVector<T,3> * dwdx, I nDwdx,
													cupcfd::geometry::euclidean::EuclideanVector<T,3> * dpdx, I nDpdx,
													T * denCell, I nDenCell,
													T * denBoundary, I nDenBoundary,
													T * uCell, I nUCell,
													T * vCell, I nVCell,
													T * wCell, I nWCell,
													T * massFlux, I nMassFlux,
													T * p, I nP,
													T * ar, I nAr,
													T * su, I nSu,
													T * rface, I nRFace,
													T small,
													I * icinl,
													I * icout,
													I * icsym,
													I * icwal,
													bool solveTurbEnergy,
													bool solveTurbDiss,
													bool solveVisc,
													bool solveEnthalpy,
													T * teCell, I nTeCell,
													T * teBoundary, I nTeBoundary,
													T * edCell, I nEdCell,
													T * edBoundary, I nEdBoundary,
													T * viseffCell, I nViseffCell,
													T * viseffBoundary, I nViseffBoundary,
													T * tCell, I nTCell,
													T * tBoundary, I nTBoundary);

		/**
		 *
		 * @tparam M The implementing class of the UnstructuredMeshInterface
		 * @tparam I The datatype of the indexing scheme
		 * @tparam T The datatype of computation/mesh/stateful data
		 * @tparam L The label datatype of the unstructured mesh
		 */
		template <class M, class I, class T, class L>
		__attribute__((warn_unused_result))
		cupcfd::error::eCodes FluxMassDolfynBoundaryLoop1(cupcfd::geometry::mesh::UnstructuredMeshInterface<M,I,T,L>& mesh,
														T * massFlux, I nMassFlux,
														T * flowin);

		/**
		 *
		 * @tparam M The implementing class of the UnstructuredMeshInterface
		 * @tparam I The datatype of the indexing scheme
		 * @tparam T The datatype of computation/mesh/stateful data
		 * @tparam L The label datatype of the unstructured mesh
		 */
		template <class M, class I, class T, class L>
		__attribute__((warn_unused_result))
		cupcfd::error::eCodes FluxMassDolfynBoundaryLoop2(cupcfd::geometry::mesh::UnstructuredMeshInterface<M,I,T,L>& mesh,
														T * massFlux, I nMassFlux,
														T * flowRegion, I nFlowRegion,
														T * flowout);

		/**
		 *
		 * @tparam M The implementing class of the UnstructuredMeshInterface
		 * @tparam I The datatype of the indexing scheme
		 * @tparam T The datatype of computation/mesh/stateful data
		 * @tparam L The label datatype of the unstructured mesh
		 */
		template <class M, class I, class T, class L>
		void FluxMassDolfynBoundaryLoop3(cupcfd::geometry::mesh::UnstructuredMeshInterface<M,I,T,L>& mesh,
										T flowin, T * ratearea);

		/**
		 *
		 * @tparam M The implementing class of the UnstructuredMeshInterface
		 * @tparam I The datatype of the indexing scheme
		 * @tparam T The datatype of computation/mesh/stateful data
		 * @tparam L The label datatype of the unstructured mesh
		 */
		template <class M, class I, class T, class L>
		__attribute__((warn_unused_result))
		cupcfd::error::eCodes FluxMassDolfynBoundaryLoop4(cupcfd::geometry::mesh::UnstructuredMeshInterface<M,I,T,L>& mesh,
														T * massFlux, I nMassFlux,
														T * uBoundary, I nUBoundary,
														T * vBoundary, I nVboundary,
														T * wBoundary, I nWBoundary,
														T * denBoundary, I nDenBoundary,
														T ratearea,
														T * flowout);

		/**
		 *
		 * @tparam M The implementing class of the UnstructuredMeshInterface
		 * @tparam I The datatype of the indexing scheme
		 * @tparam T The datatype of computation/mesh/stateful data
		 * @tparam L The label datatype of the unstructured mesh
		 */
		template <class M, class I, class T, class L>
		__attribute__((warn_unused_result))
		cupcfd::error::eCodes FluxMassDolfynBoundaryLoop5(cupcfd::geometry::mesh::UnstructuredMeshInterface<M,I,T,L>& mesh,
														T * massFlux, I nMassFlux,
														T * su, I nSu,
														T * uBoundary, I nUBoundary,
														T * vBoundary, I nVBoundary,
														T * wBoundary, I nWBoundary,
														T fact, bool solveU, bool solveV, bool solveW,
														T * flowFact, I nFlowFact,
														T * flowout2);

		/**
		 *
		 * @tparam M The implementing class of the UnstructuredMeshInterface
		 * @tparam I The datatype of the indexing scheme
		 * @tparam T The datatype of computation/mesh/stateful data
		 * @tparam L The label datatype of the unstructured mesh
		 */
		template <class M, class I, class T, class L>
		__attribute__((warn_unused_result))
		cupcfd::error::eCodes FluxMassDolfynRegionLoop(cupcfd::geometry::mesh::UnstructuredMeshInterface<M,I,T,L>& mesh,
														T * flowFact, I nFlowFact,
														T * flowRegion, I nFlowRegion,
														T flowIn);
	}
}

// Include Header Level Definitions
#include "MassKernels.ipp"

#endif
