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
 * Header Definitions for Finite Volume Viscosity operations
 */

#ifndef CUPCFD_FVM_VISCOSITY_KERNELS_IPP_H
#define CUPCFD_FVM_VISCOSITY_KERNELS_IPP_H

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
																T * visEff, I nVisEff)
		{
			I ip;

			T visOld;
			T visNew;

			for(ip = 0; ip < mesh.properties.lTCells; ip++)
			{
				#ifdef DEBUG
					if (ip >= nTE) {
						return cupcfd::error::E_INVALID_INDEX;
					}
					if (ip >= nED) {
						return cupcfd::error::E_INVALID_INDEX;
					}
					if (ip >= nDen) {
						return cupcfd::error::E_INVALID_INDEX;
					}
					if (ip >= nVisEff) {
						return cupcfd::error::E_INVALID_INDEX;
					}
				#endif

				visOld = visEff[ip];

				if(ED[ip] > small)
				{
					visNew = visLam + cmu * den[ip] * (TE[ip]*TE[ip])/ED[ip];
				}
				else
				{
					visNew = visLam;
				}

				visEff[ip] = visOld + visURF * (visNew - visOld);
			}

			return cupcfd::error::E_SUCCESS;
		}

		template <class M, class I, class T, class L>
		void calculateViscosityDolfynRegionLoop(T kappa, cupcfd::geometry::mesh::UnstructuredMeshInterface<M,I,T,L>& mesh)
		{
			I ir;
			T yplus;
			T yplustmp;
			T elog;

			for(ir = 0; ir < mesh.properties.lRegions; ir++)
			{
				if(mesh.getRegionType(ir) == cupcfd::geometry::mesh::RTYPE_WALL && mesh.getRegionStd(ir))
				{
					yplus = 11.0;
					elog = mesh.getRegionELog(ir);

					I i = 0;
					while((fabs(yplus - yplustmp) > 0.001) && (i < 50))
					{
						yplustmp = yplus;
						yplus = log(elog * yplustmp)/kappa;
						i = i + 1;
					}

					mesh.setRegionYLog(ir, yplus);
				}
				else if(mesh.getRegionType(ir) == cupcfd::geometry::mesh::RTYPE_WALL && !(mesh.getRegionStd(ir)))
				{
					mesh.setRegionYLog(ir, 0.0);
				}
			}
		}

		template <class M, class I, class T, class L>
		cupcfd::error::eCodes calculateViscosityDolfynBoundaryLoop(cupcfd::geometry::mesh::UnstructuredMeshInterface<M,I,T,L>& mesh,
																	T tmcmu, T large, T small, T kappa, T visLam,
																	T * TE, I nTE,
																	T * den, I nDen,
																	T * visEffCell, I nVisEffCell,
																	T * visEffBoundary, I nVisEffBoundary)
		{
			T cmu = tmcmu;
			T cmu25 = pow(cmu, 0.25);
			// T visURF = 1.0;

			T uplus;
			T yplus;
			T yplusMin = large;
			T yplusMax = small;

			T dist;
			T turb;
			T zero = 0.0;
			T utau;
			T d0;
			T z0;
			T tmp;

			for(I ib = 0; ib < mesh.properties.lBoundaries; ib++)
			{
				I i = mesh.getBoundaryFaceID(ib);
				I ir = mesh.getBoundaryRegionID(ib);
				cupcfd::geometry::mesh::RType it = mesh.getRegionType(ir);
				I ip = mesh.getFaceCell1ID(i);
				// I in = mesh.getFaceCell2ID(i);

				#ifdef DEBUG
					if (ib >= nVisEffBoundary) {
						return cupcfd::error::E_INVALID_INDEX;
					}
				#endif

				if(it == cupcfd::geometry::mesh::RTYPE_INLET)
				{
					// Skip User Option from Dolfyn for now
					visEffBoundary[ib] = visLam + mesh.getRegionDensity(ir) * cmu *
										 (mesh.getRegionTurbKE(ir) * mesh.getRegionTurbKE(ir)) / (mesh.getRegionTurbDiss(ir) + small);
				}
				else if(it == cupcfd::geometry::mesh::RTYPE_OUTLET)
				{
					#ifdef DEBUG
						if (ib >= nVisEffCell) {
							return cupcfd::error::E_INVALID_INDEX;
						}
					#endif

					visEffBoundary[ib] = visEffCell[ip];
				}
				else if(it == cupcfd::geometry::mesh::RTYPE_SYMP)
				{
					#ifdef DEBUG
						if (ib >= nVisEffCell) {
							return cupcfd::error::E_INVALID_INDEX;
						}
					#endif

					visEffBoundary[ib] = visEffCell[ip];
				}
				else if(it == cupcfd::geometry::mesh::RTYPE_WALL)
				{
					#ifdef DEBUG
						if (ib >= nTE) {
							return cupcfd::error::E_INVALID_INDEX;
						}
						if (ip >= nDen) {
							return cupcfd::error::E_INVALID_INDEX;
						}
					#endif

					dist = mesh.getBoundaryDistance(ib);
					turb = TE[ip];

					if(turb < zero)
					{
						turb = zero;
					}

					utau = cmu25 * sqrt(turb);
					yplus = dist * utau * den[ip] / (visLam + small);
					mesh.setBoundaryYPlus(ib, yplus);

					if(mesh.getRegionStd(ir))
					{
						if(yplus < mesh.getRegionYLog(ir))
						{
							uplus = yplus;

							if(yplus < small)
							{
								yplus = 0.0;
								uplus = small;
							}
						}
						else
						{
							tmp = mesh.getRegionELog(ir) * mesh.getBoundaryYPlus(ib);
							if(tmp < 1.1)
							{
								tmp = 1.1;
							}

							uplus = log(tmp)/kappa;
						}

						mesh.setBoundaryUPlus(ib, uplus);
						yplusMin = std::min(yplusMin, yplus);
						yplusMax = std::max(yplusMax, yplus);
						visEffBoundary[ib] = std::max(T(1.0), yplus/uplus) * visLam;
					}
					else
					{
						d0 = 0.0;
						z0 = 0.1;

						uplus = log((dist-d0)/z0)/kappa;
						mesh.setBoundaryUPlus(ib, uplus);
						visEffBoundary[ib] = std::max(T(1.0), yplus/uplus) * visLam;

						// These were originally used a print statement that is now removed
						yplusMin = std::min(yplusMin, yplus);
						yplusMax = std::max(yplusMax, yplus);
					}
				}
			}

			return cupcfd::error::E_SUCCESS;
		}

		template <class M, class I, class T, class L>
		cupcfd::error::eCodes calculateViscosityDolfynCellLoop2(cupcfd::geometry::mesh::UnstructuredMeshInterface<M,I,T,L>& mesh,
																T visLam,
																T * visEffCell, I nVisEffCell,
																T * visEffBoundary, I nVisEffBoundary)
		{
			for(I ip = 0; ip < mesh.properties.lTCells; ip++)
			{
				#ifdef DEBUG
					if (ip >= nVisEffCell) {
						return cupcfd::error::E_INVALID_INDEX;
					}
				#endif

				visEffCell[ip] = std::min(visEffCell[ip], T(1000000.0) * visLam);
			}

			for(I ip = 0; ip < mesh.properties.lBoundaries; ip++)
			{
				#ifdef DEBUG
					if (ip >= nVisEffBoundary) {
						return cupcfd::error::E_INVALID_INDEX;
					}
				#endif

				visEffBoundary[ip] = std::min(visEffBoundary[ip], T(1000000.0) * visLam);
			}

			return cupcfd::error::E_SUCCESS;
		}
	}
}

#endif
