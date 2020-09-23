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
 * Contains header level definitions for the Finite Volume Gradient Operations
 */

#ifndef CUPCFD_FVM_GRADIENT_IPP_H
#define CUPCFD_FVM_GRADIENT_IPP_H

#include <iostream>

namespace cupcfd
{
	namespace fvm
	{
		template <class M, class I, class T, class L>
		cupcfd::error::eCodes GradientPhiGaussDolfyn(cupcfd::geometry::mesh::UnstructuredMeshInterface<M,I,T,L>& mesh, I nGradient,
													T * phiCell, I nPhiCell,
													T * phiBoundary, I nPhiBoundary,
													cupcfd::geometry::euclidean::EuclideanVector<T,3> * dPhidxCell, I nDPhidxCell,
													cupcfd::geometry::euclidean::EuclideanVector<T,3> * dPhidxoCell, I nDPhidxoCell)
		{

			T facn, facp, fact;
			// T xp, xn, xf, xnorm, xpac, xnac, delp, deln;
			T phiFace, delta, vol;
			I ip, ib, in;

			cupcfd::geometry::euclidean::EuclideanPoint<T,3> xac;
			cupcfd::geometry::euclidean::EuclideanVector<T,3> dPhidxac;
			cupcfd::geometry::euclidean::EuclideanVector<T,3> corrTmp;

			I nFac = mesh.properties.lFaces;
			// I nCel = mesh.properties.lOCells;

			// Zero Cell Values
			for (I i = 0; i < nDPhidxoCell; i++)
			{
				dPhidxoCell[i].cmp[0] = (T) 0;
				dPhidxoCell[i].cmp[1] = (T) 0;
				dPhidxoCell[i].cmp[2] = (T) 0;
			}

			// Gradient Loop
			for(I iGrad = 0; iGrad < nGradient; iGrad++)
			{
				// Reset
				for (I i = 0; i < nDPhidxCell; i++)
				{
					dPhidxCell[i].cmp[0] = (T) 0;
					dPhidxCell[i].cmp[1] = (T) 0;
					dPhidxCell[i].cmp[2] = (T) 0;
				}

				// Face Loop
				for(I i = 0; i < nFac; i++)
				{			
					// Get Cell 1 Index
					ip = mesh.getFaceCell1ID(i);

					// Get Cell 2 Index
					in = mesh.getFaceCell2ID(i);

					bool isBoundary;
					mesh.getFaceIsBoundary(i, &isBoundary);

					if(!isBoundary)
					{
						facn = mesh.getFaceLambda(i);
						facp = 1.0 - facn;

						xac = (mesh.getCellCenter(in) * facn) + (mesh.getCellCenter(ip) * facp);

						dPhidxac = (dPhidxoCell[in] * facn) + (dPhidxoCell[ip] * facp);

						#ifndef NDEBUG
							if (ip >= nPhiCell || in >= nPhiCell) {
								return cupcfd::error::E_INVALID_INDEX;
							}
						#endif
						phiFace = (phiCell[in] * facn) + (phiCell[ip] * facp);

						corrTmp = mesh.getFaceCenter(i) - xac;

						dPhidxac.dotProduct(corrTmp, &delta);
						phiFace = phiFace + delta;

						dPhidxCell[ip] = dPhidxCell[ip] + (phiFace * mesh.getFaceNorm(i));
						dPhidxCell[in] = dPhidxCell[in] + (phiFace * mesh.getFaceNorm(i));
					}
					else
					{
						ib = mesh.getFaceBoundaryID(i);
						#ifndef NDEBUG
							if (ib >= nPhiBoundary) {
								return cupcfd::error::E_INVALID_INDEX;
							}
						#endif
						phiFace = phiBoundary[ib];

						dPhidxCell[ip] = dPhidxCell[ip] + (phiFace * mesh.getFaceNorm(i));
					}
				}

				// Cell Loop
				// Loop over local cells only? Or ghost cells too?
				// Since faces can access ghost cells, presume these must be updated
				// for ghost cells also.
				for(I i = 0; i < mesh.properties.lTCells; i++)
				{
					mesh.getCellVolume(i, &vol);
					fact = 1.0/vol;
					dPhidxCell[i] = fact * dPhidxCell[i];
				}

				// Copy
				for(I i = 0; i < nDPhidxoCell; i++)
				{
					dPhidxoCell[i] = dPhidxCell[i];
				}
			}
			
			return cupcfd::error::E_SUCCESS;
		}
	}
}

#endif
