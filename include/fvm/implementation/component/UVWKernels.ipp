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
 */
 
 #ifndef CUPCFD_FVM_UVW_IPP_H
 #define CUPCFD_FVM_UVW_IPP_H
 
#include <cmath>
#include <algorithm>

 namespace cupcfd
{
	namespace fvm
	{
		template <class M, class I, class T, class L>
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
													T * aw, I nAw)
		{
			// T pe0 = 9999.0;
			// T pe1 = -9999.0;
			// T totalForce = 0.0;

			I ip, in, ib, ir;
			cupcfd::geometry::mesh::RType it;
			cupcfd::geometry::euclidean::EuclideanPoint<T,3> xac;

			T facn;
			T facp;
			// T uac, vac, wac;
			T visac;
			T visFace;
			T uFace, vFace, wFace;
			T fuce, fvce, fwce;
			T sx, sy, sz;
			T fude1, fvde1, fwde1;
			T fude, fvde, fwde;
			T fmin, fmax;
			T fuci, fvci, fwci;
			T fudi, fvdi, fwdi;
			T blendU, blendV, blendW;
			T f;
			// T rlencos;

			cupcfd::geometry::euclidean::EuclideanVector<T,3> dudxac;
			cupcfd::geometry::euclidean::EuclideanVector<T,3> dvdxac;
			cupcfd::geometry::euclidean::EuclideanVector<T,3> dwdxac;
			cupcfd::geometry::euclidean::EuclideanVector<T,3> xpn;
			cupcfd::geometry::euclidean::EuclideanVector<T,3> norm;

			cupcfd::geometry::euclidean::EuclideanPoint<T,3> center1;
			cupcfd::geometry::euclidean::EuclideanPoint<T,3> center2;
			cupcfd::geometry::euclidean::EuclideanPoint<T,3> center3;


			for(I i = 0; i < mesh.properties.lFaces; i++)
			{
				#ifndef DEBUG
					if (i >= nMassFlux) {
						return cupcfd::error::E_INVALID_INDEX;
					}
				#endif

				// Get Cell 1 Index
				ip = mesh.getFaceCell1ID(i);

				// Get Cell 2 Index
				in = mesh.getFaceCell2ID(i);

				#ifndef DEBUG
					if (ip >= nUCell || in >= nUCell) {
						return cupcfd::error::E_INVALID_INDEX;
					}
					if (ip >= nVCell || in >= nVCell) {
						return cupcfd::error::E_INVALID_INDEX;
					}
					if (ip >= nWCell || in >= nWCell) {
						return cupcfd::error::E_INVALID_INDEX;
					}
					if (ip >= nVisEffCell || in >= nVisEffCell) {
						return cupcfd::error::E_INVALID_INDEX;
					}
					if (ip >= nDudx || in >= nDudx) {
						return cupcfd::error::E_INVALID_INDEX;
					}
					if (ip >= nDvdx || in >= nDvdx) {
						return cupcfd::error::E_INVALID_INDEX;
					}
					if (ip >= nDwdx || in >= nDwdx) {
						return cupcfd::error::E_INVALID_INDEX;
					}
					if (ip >= nSu || in >= nSu) {
						return cupcfd::error::E_INVALID_INDEX;
					}
					if (ip >= nSv || in >= nSv) {
						return cupcfd::error::E_INVALID_INDEX;
					}
					if (ip >= nSw || in >= nSw) {
						return cupcfd::error::E_INVALID_INDEX;
					}
				#endif

				bool isBoundary = mesh.getFaceIsBoundary(i);

				if(!isBoundary)
				{

					// Non-Boundary Face
					facn = mesh.getFaceLambda(i);
					facp = 1.0 - facn;

					xac = (mesh.getCellCenter(in) * facn) + (mesh.getCellCenter(ip) * facp);
					// uac = uCell[in] * facn + uCell[ip] * facp;
					// vac = vCell[in] * facn + vCell[ip] * facp;
					// wac = wCell[in] * facn + wCell[ip] * facp;

					dudxac = dudx[in] * facn + dudx[ip] * facp;
					dvdxac = dvdx[in] * facn + dvdx[ip] * facp;
					dwdxac = dwdx[in] * facn + dwdx[ip] * facp;

					visac = visEffCell[in] * facn + visEffCell[ip] * facp;
					xpn = mesh.getCellCenter(in) - mesh.getCellCenter(ip);
					visFace = visac * mesh.getFaceRLencos(i);

					   //    call SelectDiffSchemeVector(i,iScheme,iP,iN,     &
					   //                                U,V,W,               &
						//			   dUdX,dVdX,dWdX,      &
						//			   UFace,VFace,Wface)


					fuce = massFlux[i] * uFace;
					fvce = massFlux[i] * vFace;
					fwce = massFlux[i] * wFace;

					mesh.getFaceNorm(i, norm);
					sx = norm.cmp[0];
					sy = norm.cmp[1];
					sz = norm.cmp[2];

					fude1 = (dudxac.cmp[0] + dudxac.cmp[0]) * sx + (dudxac.cmp[1] + dvdxac.cmp[0]) * sy + (dudxac.cmp[2] + dwdxac.cmp[0]) * sz;
					fvde1 = (dudxac.cmp[1] + dvdxac.cmp[0]) * sx + (dvdxac.cmp[1] + dvdxac.cmp[1]) * sy + (dvdxac.cmp[2] + dwdxac.cmp[1]) * sz;
					fwde1 = (dudxac.cmp[2] + dwdxac.cmp[0]) * sx + (dwdxac.cmp[1] + dvdxac.cmp[2]) * sy + (dwdxac.cmp[2] + dwdxac.cmp[2]) * sz;

					fude = visac * fude1;
					fvde = visac * fvde1;
					fwde = visac * fwde1;

					fmin = std::min(massFlux[i], T(0.0));
					fmax = std::max(massFlux[i], T(0.0));

					fuci = fmin * uCell[in] + fmax * uCell[ip];
					fvci = fmin * vCell[in] + fmax * vCell[ip];
					fwci = fmin * wCell[in] + fmax * wCell[ip];

					T rDotProduct;

					dudxac.dotProduct(xpn, &rDotProduct);
					fudi = visFace * rDotProduct;

					dvdxac.dotProduct(xpn, &rDotProduct);
					fvdi = visFace * rDotProduct;

					dwdxac.dotProduct(xpn, &rDotProduct);
					fwdi = visFace * rDotProduct;

					#ifndef NDEBUG
						if ((i*2)+1 >= nRFace) {
							return cupcfd::error::E_INVALID_INDEX;
						}
					#endif
					rFace[i*2] = -visFace - std::max(massFlux[i], T(0.0));
					rFace[(i*2)+1] = -visFace + std::min(massFlux[i], T(0.0));

					blendU = gammaBlend * (fuce - fuci);
					blendV = gammaBlend * (fvce - fvci);
					blendW = gammaBlend * (fwce - fwci);

					su[ip] = su[ip] - blendU + fude - fudi;
					su[in] = su[in] + blendU - fude + fudi;

					sv[ip] = sv[ip] - blendV + fvde - fvdi;
					sv[in] = sv[in] + blendV - fvde + fvdi;

					sw[ip] = sw[ip] - blendW + fwde - fwdi;
					sw[in] = sw[in] + blendW - fwde + fwdi;

					double xpn_length;
					xpn.length(&xpn_length);
					// Leave these off for now, may reenable at later point
					// T peclet;
					// peclet = massFlux[i]/mesh.getFaceArea(i) * xpn_length/(visac+small);
					//pe0 = min(pe0, peclet);
					//pe1 = max(pe1, peclet);
				}
				else
				{
					ib = mesh.getFaceBoundaryID(i);
					ir = mesh.getBoundaryRegionID(ib);
					it = mesh.getRegionType(ir);
					ip = mesh.getFaceCell1ID(i);

					#ifndef DEBUG
						if (ib >= nUBoundary) {
							return cupcfd::error::E_INVALID_INDEX;
						}
						if (ib >= nVBoundary) {
							return cupcfd::error::E_INVALID_INDEX;
						}
						if (ib >= nWBoundary) {
							return cupcfd::error::E_INVALID_INDEX;
						}
						if (ib >= nVisEffBoundary) {
							return cupcfd::error::E_INVALID_INDEX;
						}
						if (ip >= nAu) {
							return cupcfd::error::E_INVALID_INDEX;
						}
						if (ip >= nAv) {
							return cupcfd::error::E_INVALID_INDEX;
						}
						if (ip >= nAw) {
							return cupcfd::error::E_INVALID_INDEX;
						}
					#endif

					// Boundary Face

					if(it == cupcfd::geometry::mesh::RTYPE_INLET)
					{
						// Skip/Ignore User/UserInlet for now

						dudxac = dudx[ip];
						dvdxac = dvdx[ip];
						dwdxac = dwdx[ip];

						xac = mesh.getFaceCenter(i);

						cupcfd::geometry::euclidean::EuclideanVector<T,3> uvw = mesh.getRegionUVW(ir);

						uFace = uvw.cmp[0];
						vFace = uvw.cmp[1];
						wFace = uvw.cmp[2];

						visac = visEffBoundary[ib];
						xpn = xac - mesh.getCellCenter(ip);
						visFace = visac * mesh.getFaceRLencos(i);

						fuce = massFlux[i] * uFace;
						fvce = massFlux[i] * vFace;
						fwce = massFlux[i] * wFace;

						norm = mesh.getFaceNorm(i);
						sx = norm.cmp[0];
						sy = norm.cmp[1];
						sz = norm.cmp[2];

						fude = (dudxac.cmp[0] + dudxac.cmp[0]) * sx + (dudxac.cmp[1] + dvdxac.cmp[0]) * sy + (dudxac.cmp[2] + dwdxac.cmp[0]) * sz;
						fvde = (dudxac.cmp[1] + dvdxac.cmp[0]) * sx + (dvdxac.cmp[1] + dvdxac.cmp[1]) * sy + (dvdxac.cmp[2] + dwdxac.cmp[1]) * sz;
						fwde = (dudxac.cmp[2] + dwdxac.cmp[0]) * sx + (dwdxac.cmp[1] + dvdxac.cmp[2]) * sy + (dwdxac.cmp[2] + dwdxac.cmp[2]) * sz;

						fude = visac * fude;
						fvde = visac * fvde;
						fwde = visac * fwde;

						fmin = std::min(massFlux[i], T(0.0));
						fmax = std::max(massFlux[i], T(0.0));

						fuci = fmin * uFace + fmax * uCell[ip];
						fvci = fmin * vFace + fmax * vCell[ip];
						fwci = fmin * wFace + fmax * wCell[ip];

						T rDotProduct;

						dudxac.dotProduct(xpn, &rDotProduct);
						fudi = visFace * rDotProduct;

						dvdxac.dotProduct(xpn, &rDotProduct);
						fvdi = visFace * rDotProduct;

						dwdxac.dotProduct(xpn, &rDotProduct);
						fwdi = visFace * rDotProduct;

						f = -visFace + std::min(massFlux[i], T(0.0));

						au[ip] = au[ip] - f;
						su[ip] = su[ip] - f * uFace + fude - fudi;
						uBoundary[ib] = uFace;

						av[ip] = av[ip] - f;
						sv[ip] = sv[ip] - f * vFace + fvde - fvdi;
						vBoundary[ib] = vFace;

						aw[ip] = aw[ip] - f;
						sw[ip] = sw[ip] - f * wFace + fwde - fwdi;
						wBoundary[ib] = wFace;
					}
					else if(it == cupcfd::geometry::mesh::RTYPE_OUTLET)
					{

						dudxac = dudx[ip];
						dvdxac = dvdx[ip];
						dwdxac = dwdx[ip];

						xac = mesh.getFaceCenter(i);
						visac = visEffCell[ip];

						xpn = xac - mesh.getCellCenter(ip);

						uFace = uCell[ip];
						vFace = vCell[ip];
						wFace = wCell[ip];

						visFace = visac * mesh.getFaceRLencos(i);

						fuce = massFlux[i] * uFace;
						fvce = massFlux[i] * vFace;
						fwce = massFlux[i] * wFace;

						norm = mesh.getFaceNorm(i);
						sx = norm.cmp[0];
						sy = norm.cmp[1];
						sz = norm.cmp[2];

						fude = (dudxac.cmp[0] + dudxac.cmp[0]) * sx + (dudxac.cmp[1] + dvdxac.cmp[0]) * sy + (dudxac.cmp[2] + dwdxac.cmp[0]) * sz;
						fvde = (dvdxac.cmp[0] + dudxac.cmp[1]) * sx + (dvdxac.cmp[1] + dvdxac.cmp[1]) * sy + (dvdxac.cmp[2] + dwdxac.cmp[1]) * sz;
						fwde = (dwdxac.cmp[0] + dudxac.cmp[2]) * sx + (dwdxac.cmp[1] + dvdxac.cmp[2]) * sy + (dwdxac.cmp[2] + dwdxac.cmp[2]) * sz;

						fude = visac * fude;
						fvde = visac * fvde;
						fwde = visac * fwde;

						fmin = std::min(massFlux[i], T(0.0));
						fmax = std::max(massFlux[i], T(0.0));

						fuci = fmin * uFace + fmax * uCell[ip];
						fvci = fmin * vFace + fmax * vCell[ip];
						fwci = fmin * wFace + fmax * wCell[ip];

						T rDotProduct;

						dudxac.dotProduct(xpn, &rDotProduct);
						fudi = visFace * rDotProduct;

						dvdxac.dotProduct(xpn, &rDotProduct);
						fvdi = visFace * rDotProduct;

						dwdxac.dotProduct(xpn, &rDotProduct);
						fwdi = visFace * rDotProduct;

						if(massFlux[i] < 0.0)
						{
							massFlux[i] = small;
						}

						f = -visFace + std::min(massFlux[i], T(0.0));

						au[ip] = au[ip] - f;
						su[ip] = su[ip] -f * uFace + fude - fudi;
						uBoundary[ib] = uFace;

						av[ip] = av[ip] - f;
						sv[ip] = sv[ip] - f * vFace + fvde - fvdi;
						vBoundary[ib] = vFace;

						aw[ip] = aw[ip] - f;
						sw[ip] = sw[ip] - f * wFace + fwde - fwdi;
						wBoundary[ib] = wFace;
					}
					else if(it == cupcfd::geometry::mesh::RTYPE_SYMP)
					{
						cupcfd::geometry::euclidean::EuclideanVector<T,3> tmp;

						xac = mesh.getFaceCenter(i);
						xpn = xac - mesh.getCellCenter(ip);

						dudxac = dudx[ip];
						dvdxac = dvdx[ip];
						dwdxac = dwdx[ip];
						visac = visEffCell[ip];

						// T rDotProduct;
						T du, dv, dw, dp, dn;

						cupcfd::geometry::euclidean::EuclideanVector<T,3> xn;
						cupcfd::geometry::euclidean::EuclideanVector<T,3> un;
						cupcfd::geometry::euclidean::EuclideanVector<T,3> tauNN;
						cupcfd::geometry::euclidean::EuclideanVector<T,3> us;
						cupcfd::geometry::euclidean::EuclideanVector<T,3> force;

						dudxac.dotProduct(xpn, &du);
						dvdxac.dotProduct(xpn, &dv);
						dwdxac.dotProduct(xpn, &dw);

						us.cmp[0] = uCell[ip] + du;
						us.cmp[1] = vCell[ip] + dv;
						us.cmp[2] = wCell[ip] + dw;

						xn = 0.0 - mesh.getFaceNorm(i);
						xn.normalise();

						us.dotProduct(xn, &dp);
						un = dp * xn;

						dn = mesh.getBoundaryDistance(ib);
						tauNN = 2.0 * visac * un/dn;
						force = tauNN * mesh.getFaceArea(i);

						// Assume not initialisation
						//if(!init)
						//{
						au[ip] = au[ip] + visFace;
						av[ip] = av[ip] + visFace;
						aw[ip] = aw[ip] + visFace;

						su[ip] = su[ip] + visFace * uCell[ip] - force.cmp[0];
						sv[ip] = sv[ip] + visFace * vCell[ip] - force.cmp[1];
						sw[ip] = sw[ip] + visFace * wCell[ip] - force.cmp[2];
						//}

						uBoundary[ib] = us.cmp[0];
						vBoundary[ib] = us.cmp[1];
						wBoundary[ib] = us.cmp[2];
					}
					else if(it == cupcfd::geometry::mesh::RTYPE_WALL)
					{
						T coef;
						T dp, dn;
						T uvel;

						cupcfd::geometry::euclidean::EuclideanVector<T,3> uw;
						cupcfd::geometry::euclidean::EuclideanVector<T,3> un;
						cupcfd::geometry::euclidean::EuclideanVector<T,3> xn;
						cupcfd::geometry::euclidean::EuclideanVector<T,3> up;
						cupcfd::geometry::euclidean::EuclideanVector<T,3> ut;
						cupcfd::geometry::euclidean::EuclideanVector<T,3> tauNT;
						cupcfd::geometry::euclidean::EuclideanVector<T,3> force;
						cupcfd::geometry::euclidean::EuclideanVector<T,3> tmp;

						xac = mesh.getFaceCenter(i);
						uw = mesh.getRegionUVW(ir);

						dudxac = dudx[ip];
						dvdxac = dvdx[ip];
						dwdxac = dwdx[ip];

						visac = visEffBoundary[ib];

						xpn = mesh.getFaceCenter(i) - mesh.getCellCenter(ip);

						// ToDo: Do we want to force this to be a double here?
						// May also wish for it to just be a float - move out to template?
						double xpn_length;
						xpn.length(&xpn_length);
						coef = visac * mesh.getFaceArea(i) / xpn_length;

						xn = mesh.getFaceNorm(i);
						xn.normalise();

						up.cmp[0] = uCell[ip];
						up.cmp[1] = vCell[ip];
						up.cmp[2] = wCell[ip];

						up = up - uw;

						up.dotProduct(xn, &dp);

						un = dp * xn;
						ut = up - un;
						uvel = fabs(ut.cmp[0]) + fabs(ut.cmp[1]) + fabs(ut.cmp[2]);

						if(uvel > small)
						{
							dn = mesh.getBoundaryDistance(ib);
							tauNT = visac * ut/dn;
							force = tauNT * mesh.getFaceArea(i);
							mesh.setBoundaryShear(ib, force);
						}
						else
						{
							force.cmp[0] = 0.0;
							force.cmp[1] = 0.0;
							force.cmp[2] = 0.0;
							mesh.setBoundaryShear(ib, force);
						}

						// Assume not initialisation?
						//if(!init)
						//{
						au[ip] = au[ip] + coef;
						av[ip] = av[ip] + coef;
						aw[ip] = aw[ip] + coef;

						su[ip] = su[ip] + coef * uCell[ip] - force.cmp[0];
						sv[ip] = sv[ip] + coef * vCell[ip] - force.cmp[1];
						sw[ip] = sw[ip] + coef * wCell[ip] - force.cmp[2];
						//}

						uBoundary[ib] = uw.cmp[0];
						vBoundary[ib] = uw.cmp[1];
						wBoundary[ib] = uw.cmp[2];
					}
				}
			}

			return cupcfd::error::E_SUCCESS;
		}

		template <class M, class I, class T, class L>
		void FluxUVWDolfynRegionLoop1(cupcfd::geometry::mesh::UnstructuredMeshInterface<M,I,T,L>& mesh)
		{
			I ir;

			cupcfd::geometry::euclidean::EuclideanVector<T,3> zero((T) 0, (T) 0, (T) 0);

			for(ir = 0; ir < mesh.properties.lRegions; ir++)
			{
				mesh.setRegionForceTangent(ir, zero);
			}
		}


		template <class M, class I, class T, class L>
		void FluxUVWDolfynBndsLoop1(cupcfd::geometry::mesh::UnstructuredMeshInterface<M,I,T,L>& mesh)
		{
			I ib, ir;
			cupcfd::geometry::mesh::RType it;
			cupcfd::geometry::euclidean::EuclideanVector<T,3> shear;
			cupcfd::geometry::euclidean::EuclideanVector<T,3> forceTangent;
			cupcfd::geometry::euclidean::EuclideanVector<T,3> tmp;

			for(ib = 0; ib < mesh.properties.lBoundaries; ib++)
			{
				ir = mesh.getBoundaryRegionID(ib);
				it = mesh.getRegionType(ir);

				if(it == cupcfd::geometry::mesh::RTYPE_WALL)
				{
					tmp = mesh.getRegionForceTangent(ir) + mesh.getBoundaryShear(ib);
					mesh.setRegionForceTangent(ir, tmp);
				}
			}
		}
	}
}

#endif
