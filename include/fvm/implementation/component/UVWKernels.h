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
 * Declarations for Finite Volume FluxUVW operations
 */

#ifndef CUPCFD_FVM_UVW_INCLUDE_H
#define CUPCFD_FVM_UVW_INCLUDE_H

#include "EuclideanVector.h"
#include "EuclideanVector3D.h"
#include "UnstructuredMeshInterface.h"

namespace cupcfd
{
	namespace fvm
	{
		/**
		 *
		 */
		template <class M, class I, class T, class L>
		__attribute__((warn_unused_result))
		cupcfd::error::eCodes FluxUVWDolfynFaceLoop1(cupcfd::geometry::mesh::UnstructuredMeshInterface<M,I,T,L>& mesh,
													T gammaBlend,
													T small,
													// T large,
													T * uCell, I nUCell,
													T * vCell, I nVCell,
													T * wCell, I nWCell,
													T * uBoundary, I nUBoundary,
													T * vBoundary, I nVBoundary,
													T * wBoundary, I nWBoundary,
													T * visEffCell, I nVisEffCell,
													T * visEffBoundary, I nVisEffBoundary,
													T * massFlux, I nMassFlux,
													cupcfd::geometry::euclidean::EuclideanVector<T,3> * dudx, I nDudx,
													cupcfd::geometry::euclidean::EuclideanVector<T,3> * dvdx, I nDvdx,
													cupcfd::geometry::euclidean::EuclideanVector<T,3> * dwdx, I nDwdx,
													T * rFace, I nRFace,
													T * su, I nSu,
													T * sv, I nSv,
													T * sw, I nSw,
													T * au, I nAu,
													T * av, I nAv,
													T * aw, I nAw);

		/**
		 *
		 */
		template <class M, class I, class T, class L>
		void FluxUVWDolfynRegionLoop1(cupcfd::geometry::mesh::UnstructuredMeshInterface<M,I,T,L>& mesh);

		/**
		 *
		 */
		template <class M, class I, class T, class L>
		void FluxUVWDolfynBndsLoop1(cupcfd::geometry::mesh::UnstructuredMeshInterface<M,I,T,L>& mesh);
	}
}

// Include Header Level Definitions
#include "UVWKernels.ipp"

#endif
