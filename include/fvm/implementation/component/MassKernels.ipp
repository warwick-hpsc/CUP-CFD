/*
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 * These kernels were ported/derived to C++ from Dolfyn:
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
 * Contains header level definitions for the Finite Volume Mass Operations
 */

#ifndef CUPCFD_FVM_MASS_IPP_H
#define CUPCFD_FVM_MASS_IPP_H

namespace cupcfd
{
	namespace fvm
	{
		template <class M, class I, class T, class L>
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
													T * tBoundary, I nTBoundary) {
			I ip, in, ib, ir;
			T facn, facp;
			T denf;
			cupcfd::geometry::euclidean::EuclideanVector<T,3> dudxac;
			cupcfd::geometry::euclidean::EuclideanVector<T,3> dvdxac;
			cupcfd::geometry::euclidean::EuclideanVector<T,3> dwdxac;
			cupcfd::geometry::euclidean::EuclideanPoint<T,3> xface;
			cupcfd::geometry::euclidean::EuclideanVector<T,3> delta;
			cupcfd::geometry::euclidean::EuclideanVector<T,3> xnorm;
			cupcfd::geometry::euclidean::EuclideanVector<T,3> xpn;
			cupcfd::geometry::euclidean::EuclideanVector<T,3> xpn2;
			cupcfd::geometry::mesh::RType it;
			cupcfd::geometry::euclidean::EuclideanPoint<T,3> xac;
			cupcfd::geometry::euclidean::EuclideanVector<T,3> uIn;
			cupcfd::geometry::euclidean::EuclideanPoint<T,3> xpac;
			cupcfd::geometry::euclidean::EuclideanPoint<T,3> xnac;
			cupcfd::geometry::euclidean::EuclideanVector<T,3> delp;
			cupcfd::geometry::euclidean::EuclideanVector<T,3> deln;
			cupcfd::geometry::euclidean::EuclideanVector<T,3> norm;

			T uFace, vFace, wFace;
			T pip, pin;
			T apv1, apv2, apv, fact, factv;
			T dpx, dpy, dpz;
			T dens;

			for(int i = 0; i < mesh.properties.lFaces; i++) {
				ip = mesh.getFaceCell1ID(i);
				in = mesh.getFaceCell2ID(i);

				bool isBoundary = mesh.getFaceIsBoundary(i);

				#ifdef DEBUG
					if (i >= nMassFlux) {
						return cupcfd::error::E_INVALID_INDEX;
					}
				#endif
				if(!isBoundary) {
					#ifdef DEBUG
						if (in >= nDudx || ip >= nDudx) {
							return cupcfd::error::E_INVALID_INDEX;
						}
						if (in >= nDvdx || ip >= nDvdx) {
							return cupcfd::error::E_INVALID_INDEX;
						}
						if (in >= nDwdx || ip >= nDwdx) {
							return cupcfd::error::E_INVALID_INDEX;
						}
						if (in >= nDpdx || ip >= nDpdx) {
							return cupcfd::error::E_INVALID_INDEX;
						}
						if (in >= nDenCell || ip >= nDenCell) {
							return cupcfd::error::E_INVALID_INDEX;
						}
						if (in >= nUCell || ip >= nUCell) {
							return cupcfd::error::E_INVALID_INDEX;
						}
						if (in >= nVCell || ip >= nVCell) {
							return cupcfd::error::E_INVALID_INDEX;
						}
						if (in >= nWCell || ip >= nWCell) {
							return cupcfd::error::E_INVALID_INDEX;
						}
						if (in >= nP || ip >= nP) {
							return cupcfd::error::E_INVALID_INDEX;
						}
						if (in >= nAr || ip >= nAr) {
							return cupcfd::error::E_INVALID_INDEX;
						}
					#endif

					facn = mesh.getFaceLambda(i);
					facp = 1.0 - facn;
					dudxac = dudx[in] * facn + dudx[ip] * facp;
					dvdxac = dvdx[in] * facn + dvdx[ip] * facp;
					dwdxac = dwdx[in] * facn + dwdx[ip] * facp;

					denf = denCell[in] * facn + denCell[ip] * facp;
					xac = mesh.getCellCenter(in) * facn + mesh.getCellCenter(ip) * facp;
					xface = mesh.getFaceCenter(i);
					delta = xface - xac;

					uFace = uCell[in]*facn + uCell[ip]*facp + dudxac.dotProduct(delta);
					vFace = vCell[in]*facn + vCell[ip]*facp + dvdxac.dotProduct(delta);
					wFace = wCell[in]*facn + wCell[ip]*facp + dwdxac.dotProduct(delta);

					massFlux[i] = denf * (uFace * mesh.getFaceNorm(i).cmp[0] +
										vFace * mesh.getFaceNorm(i).cmp[1] +
										wFace * mesh.getFaceNorm(i).cmp[2]);

					xnorm = mesh.getFaceNorm(i);
					xnorm.normalise();

					xpac = mesh.getFaceXpac(i);
					xnac = mesh.getFaceXnac(i);

					delp = xpac - mesh.getCellCenter(ip);
					pip = p[ip] + dpdx[ip].dotProduct(delp);

					deln = xpac - mesh.getCellCenter(in);
					pin = p[in] + dpdx[ip].dotProduct(deln);

					xpn = xnac - xpac;
					xpn2 = mesh.getCellCenter(in) - mesh.getCellCenter(ip);

					apv1 = denCell[ip] * ar[ip];
					apv2 = denCell[in] * ar[in];
					apv = apv2 * facn + apv1 * facp;

					factv = mesh.getCellVolume(in) * facn + mesh.getCellVolume(ip) * facp;
					apv *= mesh.getFaceArea(i) * factv/xpn2.dotProduct(xnorm);

					dpx = (dpdx[in].cmp[0] * facn + dpdx[ip].cmp[0] * facp) * xpn.cmp[0];
					dpy = (dpdx[in].cmp[1] * facn + dpdx[ip].cmp[1] * facp) * xpn.cmp[1];
					dpz = (dpdx[in].cmp[2] * facn + dpdx[ip].cmp[2] * facp) * xpn.cmp[2];

					fact = apv;

					#ifdef DEBUG
						if ( ((i*2)+1) >= nRFace) {
							return cupcfd::error::E_INVALID_INDEX;
						}
					#endif
					rface[(i*2)] = -fact;
					rface[(i*2)+1] = -fact;

					massFlux[i] = massFlux[i] - fact * ((pin-pip) - dpx - dpy - dpz);
				}
				else {
					ip = mesh.getFaceCell1ID(i);
					ib = mesh.getFaceBoundaryID(i);
					ir = mesh.getBoundaryRegionID(ib);
					it = mesh.getRegionType(ir);

					#ifdef DEBUG
						if (ip >= nSu) {
							return cupcfd::error::E_INVALID_INDEX;
						}
						if (ip >= nUCell) {
							return cupcfd::error::E_INVALID_INDEX;
						}
						if (ip >= nVCell) {
							return cupcfd::error::E_INVALID_INDEX;
						}
						if (ip >= nWCell) {
							return cupcfd::error::E_INVALID_INDEX;
						}
						if (ip >= nDenCell) {
							return cupcfd::error::E_INVALID_INDEX;
						}
						if (ib >= nDenBoundary) {
							return cupcfd::error::E_INVALID_INDEX;
						}
						if (ib >= nTeBoundary) {
							return cupcfd::error::E_INVALID_INDEX;
						}
						if (ib >= nTeCell) {
							return cupcfd::error::E_INVALID_INDEX;
						}
						if (ib >= nEdBoundary) {
							return cupcfd::error::E_INVALID_INDEX;
						}
						if (ib >= nViseffBoundary) {
							return cupcfd::error::E_INVALID_INDEX;
						}
						if (ib >= nTBoundary) {
							return cupcfd::error::E_INVALID_INDEX;
						}
						if (ip >= nEdCell) {
							return cupcfd::error::E_INVALID_INDEX;
						}
						if (ip >= nViseffCell) {
							return cupcfd::error::E_INVALID_INDEX;
						}
						if (ip >= nTCell) {
							return cupcfd::error::E_INVALID_INDEX;
						}
					#endif

					if(it == cupcfd::geometry::mesh::RTYPE_INLET) {
						*icinl = *icinl + 1;
						xac = mesh.getFaceCenter(i);

						// Ignoring User Option
						uIn = mesh.getRegionUVW(ir);
						dens = mesh.getRegionDen(ir);

						norm = mesh.getFaceNorm(i);
						massFlux[i] = dens * uIn.dotProduct(norm);
						su[ip] = su[ip] - massFlux[i];
					}
					else if(it == cupcfd::geometry::mesh::RTYPE_OUTLET) {
						*icout = *icout + 1;
						delta = mesh.getFaceCenter(i) - mesh.getCellCenter(ip);
						uFace = uCell[ip];
						vFace = vCell[ip];
						wFace = wCell[ip];

						denf = denCell[ip];
						denBoundary[ib] = denf;

						norm = mesh.getFaceNorm(i);
						massFlux[i] = denf * (uFace * norm.cmp[0] +
											 vFace * norm.cmp[1] +
											 wFace * norm.cmp[2]);

						if(massFlux[i] < 0.0) {
							massFlux[i] = small;

							if(solveTurbEnergy) {
								teBoundary[ib] = teCell[ip];
							}

							if(solveTurbDiss) {
								edBoundary[ib] = edCell[ip];
							}

							if(solveVisc) {
								viseffBoundary[ib] = viseffCell[ip];
							}

							if(solveEnthalpy) {
								tBoundary[ib] = tCell[ip];
							}

							// Skip SolveScalars
						}
					}
					else if(it == cupcfd::geometry::mesh::RTYPE_SYMP) {
						*icsym = *icsym + 1;
						massFlux[i] = 0.0;
					}
					else if(it == cupcfd::geometry::mesh::RTYPE_WALL) {
						*icwal = *icwal + 1;
						massFlux[i] = 0.0;
					}
				}
			}

			return cupcfd::error::E_SUCCESS;
		}

		template <class M, class I, class T, class L>
		cupcfd::error::eCodes FluxMassDolfynBoundaryLoop1(cupcfd::geometry::mesh::UnstructuredMeshInterface<M,I,T,L>& mesh,
														 T * massFlux, I nMassFlux,
														 T * flowin) {
			cupcfd::geometry::mesh::RType it;
			I ib, ir, i;

			*flowin = 0.0;

			for(ib = 0; ib < mesh.properties.lBoundaries; ib++) {
				ir = mesh.getBoundaryRegionID(ib);
				it = mesh.getRegionType(ir);

				if(it == cupcfd::geometry::mesh::RTYPE_INLET) {
					i = mesh.getBoundaryFaceID(ib);
					#ifdef DEBUG
						if (i >= nMassFlux) {
							return cupcfd::error::E_INVALID_INDEX;
						}
					#endif
					*flowin = *flowin + massFlux[i];
				}
			}

			return cupcfd::error::E_SUCCESS;
		}

		template <class M, class I, class T, class L>
		cupcfd::error::eCodes FluxMassDolfynBoundaryLoop2(cupcfd::geometry::mesh::UnstructuredMeshInterface<M,I,T,L>& mesh,
														 T * massFlux, I nMassFlux,
														 T * flowRegion, I nFlowRegion,
														 T * flowout) {
			cupcfd::geometry::mesh::RType it;
			I ib, ir, i;

			*flowout = 0.0;

			for(ib = 0; ib < mesh.properties.lBoundaries; ib++) {
				ir = mesh.getBoundaryRegionID(ib);
				it = mesh.getRegionType(ir);

				if(it == cupcfd::geometry::mesh::RTYPE_OUTLET) {
					i = mesh.getBoundaryFaceID(ib);
					#ifdef DEBUG
						if (i >= nMassFlux) {
							return cupcfd::error::E_INVALID_INDEX;
						}
						if (ir >= nFlowRegion) {
							return cupcfd::error::E_INVALID_INDEX;
						}
					#endif
					*flowout = *flowout + massFlux[i];
					flowRegion[ir] = flowRegion[ir] + massFlux[i];
				}
			}

			return cupcfd::error::E_SUCCESS;
		}


		template <class M, class I, class T, class L>
		cupcfd::error::eCodes FluxMassDolfynBoundaryLoop3(cupcfd::geometry::mesh::UnstructuredMeshInterface<M,I,T,L>& mesh,
										 T flowin, T * ratearea) {
			cupcfd::geometry::mesh::RType it;
			T areaout;
			I ib, ir, i;

			areaout = 0.0;

			for(ib = 0; ib < mesh.properties.lBoundaries; ib++) {
				ir = mesh.getBoundaryRegionID(ib);
				it = mesh.getRegionType(ir);

				if(it == cupcfd::geometry::mesh::RTYPE_OUTLET) {
					i = mesh.getBoundaryFaceID(ib);
					areaout = areaout + mesh.getFaceArea(i);
				}
			}

			*ratearea = -flowin/areaout;

			return cupcfd::error::E_SUCCESS;
		}

		template <class M, class I, class T, class L>
		cupcfd::error::eCodes FluxMassDolfynBoundaryLoop4(cupcfd::geometry::mesh::UnstructuredMeshInterface<M,I,T,L>& mesh,
														 T * massFlux, I nMassFlux,
														 T * uBoundary, I nUBoundary,
														 T * vBoundary, I nVboundary,
														 T * wBoundary, I nWBoundary,
														 T * denBoundary, I nDenBoundary,
														 T ratearea,
														 T * flowout) {
			cupcfd::geometry::mesh::RType it;
			cupcfd::geometry::euclidean::EuclideanVector<T,3> xnorm;

			I ib,ir, i;
			T split;
			T faceFlux;

			*flowout = 0.0;

			for(ib = 0; ib < mesh.properties.lBoundaries; ib++) {
				ir = mesh.getBoundaryRegionID(ib);
				it = mesh.getRegionType(ir);

				if(it == cupcfd::geometry::mesh::RTYPE_OUTLET) {
					i = mesh.getBoundaryFaceID(ib);

					#ifdef DEBUG
						if (i >= nMassFlux) {
							return cupcfd::error::E_INVALID_INDEX;
						}
						if (ib >= nUBoundary) {
							return cupcfd::error::E_INVALID_INDEX;
						}
						if (ib >= nVboundary) {
							return cupcfd::error::E_INVALID_INDEX;
						}
						if (ib >= nWBoundary) {
							return cupcfd::error::E_INVALID_INDEX;
						}
						if (ib >= nDenBoundary) {
							return cupcfd::error::E_INVALID_INDEX;
						}
					#endif

					split = mesh.getRegionSplvl(ir);
					massFlux[i] = ratearea * mesh.getFaceArea(i) * split;
					faceFlux = massFlux[i]/denBoundary[ib]/mesh.getFaceArea(i);

					xnorm = mesh.getFaceNorm(i);
					xnorm.normalise();

					uBoundary[ib] = faceFlux * xnorm.cmp[0];
					vBoundary[ib] = faceFlux * xnorm.cmp[1];
					wBoundary[ib] = faceFlux * xnorm.cmp[2];

					*flowout = *flowout + massFlux[i];
				}
			}

			return cupcfd::error::E_SUCCESS;
		}


		template <class M, class I, class T, class L>
		cupcfd::error::eCodes FluxMassDolfynBoundaryLoop5(cupcfd::geometry::mesh::UnstructuredMeshInterface<M,I,T,L>& mesh,
														T * massFlux, I nMassFlux,
														T * su, I nSu,
														T * uBoundary, I nUBoundary,
														T * vBoundary, I nVBoundary,
														T * wBoundary, I nWBoundary,
														T fact, bool solveU, bool solveV, bool solveW,
														T * flowFact, I nFlowFact,
														T * flowout2) {
			cupcfd::geometry::mesh::RType it;
			I ib, ir;

			for(ib = 0; ib < mesh.properties.lBoundaries; ib++) {
				ir = mesh.getBoundaryRegionID(ib);
				it = mesh.getRegionType(ir);

				if(it == cupcfd::geometry::mesh::RTYPE_OUTLET) {
					I i = mesh.getBoundaryFaceID(ib);

					#ifdef DEBUG
						if (i >= nMassFlux) {
							return cupcfd::error::E_INVALID_INDEX;
						}
						if (ir >= nFlowFact) {
							return cupcfd::error::E_INVALID_INDEX;
						}
						if (ib >= nUBoundary) {
							return cupcfd::error::E_INVALID_INDEX;
						}
						if (ib >= nVBoundary) {
							return cupcfd::error::E_INVALID_INDEX;
						}
						if (ib >= nWBoundary) {
							return cupcfd::error::E_INVALID_INDEX;
						}
					#endif

					massFlux[i] = massFlux[i] * flowFact[ir];
					*flowout2 = *flowout2 + massFlux[i];

					if(solveU) {
						uBoundary[ib] = uBoundary[ib] * fact;
					}

					if(solveV) {
						vBoundary[ib] = vBoundary[ib] * fact;
					}

					if(solveW) {
						wBoundary[ib] = wBoundary[ib] * fact;
					}

					int ip = mesh.getFaceCell1ID(i);
					#ifdef DEBUG
						if (ip >= nSu) {
							return cupcfd::error::E_INVALID_INDEX;
						}
					#endif
					su[ip] = su[ip] - massFlux[i];
				}
			}

			return cupcfd::error::E_SUCCESS;
		}

		template <class M, class I, class T, class L>
		cupcfd::error::eCodes FluxMassDolfynRegionLoop(cupcfd::geometry::mesh::UnstructuredMeshInterface<M,I,T,L>& mesh,
														T * flowFact, I nFlowFact,
														T * flowRegion, I nFlowRegion,
														T flowIn) {
			cupcfd::geometry::mesh::RType it;
			T split;
			I nRegions = mesh.properties.lRegions;

			for(I ir = 0; ir < nRegions; ir++) {
				it = mesh.getRegionType(ir);

				#ifdef DEBUG
					if (ir >= nFlowFact) {
						return cupcfd::error::E_INVALID_INDEX;
					}
					if (ir >= nFlowRegion) {
						return cupcfd::error::E_INVALID_INDEX;
					}
				#endif

				if(it == cupcfd::geometry::mesh::RTYPE_OUTLET) {
					split = mesh.getRegionSplvl(ir);
					flowFact[ir] = -(split * flowIn) / flowRegion[ir];
				}
				else {
					flowFact[ir] = 0.0;
				}
			}

			return cupcfd::error::E_SUCCESS;
		}
	}
}

#endif
