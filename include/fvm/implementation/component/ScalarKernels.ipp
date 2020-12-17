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


#ifndef CUPCFD_FVM_SCALAR_IPP_H
#define CUPCFD_FVM_SCALAR_IPP_H

namespace cupcfd
{
	namespace fvm
	{
		template <class M, class I, class T, class L>
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
														// T sigma, T sigma2, T vislam,
														int ivar, int VarT, T Sigma_T, T Prandtl,
														int VarTE, T Sigma_k, int VarED, T Sigma_e,
														T Sigma_s, T Schmidt, T GammaBlend, T Small, T Large, T TMCmu)
		{
			cupcfd::geometry::mesh::RType it;
			I i, ip, in, ib, ir;
			T facn, facp;

			// T Phiac;
			T Visac;
			T VisLam;
			T PhiFlux;
			T VisFace;
			// T fce, fci, fdi, fde1, fde2, d1, fde, f;
			T fce, fci, fdi, fde1, d1, fde, f;
			T s2, dn;
			T Tdif;

			T blend, peclet, Resist, Hcoef, SLres, Cmu25, Tplus, utau;

			cupcfd::geometry::euclidean::EuclideanPoint<T,3> Xac;
			cupcfd::geometry::euclidean::EuclideanPoint<T,3> tmpPoint;

			cupcfd::geometry::euclidean::EuclideanVector<T,3> Xpn;
			cupcfd::geometry::euclidean::EuclideanVector<T,3> norm;
			cupcfd::geometry::euclidean::EuclideanVector<T,3> tmpVec;
			cupcfd::geometry::euclidean::EuclideanVector<T,3> dPhidxac;
			cupcfd::geometry::euclidean::EuclideanVector<T,3> d2, d3, ds;

			T pe0, pe1;
			T QTransferIn, QTransferOut;
			T Atot, qmin, qmax, qtot, hmin, hmax, htot;

			// This is set by the DiffScheme in the original
			T PhiFace;


			pe0 = Large;
			pe1 = Large;
			QTransferIn = 0.0;
			QTransferOut = 0.0;
			Atot = 0.0;
			qmin = Large;
			qmax = -Large;
			qtot = 0.0;
			hmin = Large;
			hmax = -Large;
			htot = 0.0;

			for(i = 0; i < mesh.properties.lFaces; i++)
			{
				#ifndef NDEBUG
					if (i >= nMassFlux) {
						DEBUGGABLE_ERROR; return cupcfd::error::E_INVALID_INDEX;
					}
				#endif

				ip = mesh.getFaceCell1ID(i);
				in = mesh.getFaceCell2ID(i);

				#ifndef NDEBUG
					if (ip >= nVisEff || in >= nVisEff) {
						DEBUGGABLE_ERROR; return cupcfd::error::E_INVALID_INDEX;
					}
					if (ip >= nAu) {
						DEBUGGABLE_ERROR; return cupcfd::error::E_INVALID_INDEX;
					}
					if (ip >= nSu || in >= nSu) {
						DEBUGGABLE_ERROR; return cupcfd::error::E_INVALID_INDEX;
					}
					if (ip >= ndPhidx || in >= ndPhidx) {
						DEBUGGABLE_ERROR; return cupcfd::error::E_INVALID_INDEX;
					}
				#endif

				bool isBoundary = mesh.getFaceIsBoundary(i);

				if(!isBoundary)
				{
					#ifndef NDEBUG
						if (ip >= nPhiCell || in >= nPhiCell) {
							DEBUGGABLE_ERROR; return cupcfd::error::E_INVALID_INDEX;
						}
					#endif

					facn = mesh.getFaceLambda(i);
					facp = 1.0 - facn;

					Xac = mesh.getCellCenter(in) * facn + mesh.getCellCenter(ip) * facp;

					// Phiac = PhiCell[in] * facn + PhiCell[ip] * facp;
					Visac = VisEff[in] * facn + VisEff[ip] * facp;


					if(SolveTurb)
					{
							Visac = Visac - VisLam;

							if(ivar == VarT)
							{
								Visac = ( VisLam + Visac / Sigma_T )/Prandtl;
							}
							else if( ivar == VarTE )
							{
								Visac = VisLam + Visac / Sigma_k;
							}
							else if( ivar == VarED )
							{
								Visac = VisLam + Visac / Sigma_e;
							}
							else
							{
								Visac = ( VisLam + Visac / Sigma_s )/Schmidt;
							}
					}
					else
					{
							if( ivar == VarT )
							{
								Visac  = Visac / Prandtl;
							}
							else
							{
								Visac  = Visac / Schmidt;
							}
					}


					dPhidxac = dPhidx[in] * facn + dPhidx[ip] * facp;

					cupcfd::geometry::euclidean::EuclideanVector<T,3> tmp;
					tmpPoint = mesh.getFaceCenter(i);
					tmpVec = tmpPoint - Xac;

					// T delta = dPhidxac.dotProduct(tmpVec);

					Xpn = mesh.getCellCenter(in) - mesh.getCellCenter(ip);
					VisFace  = Visac * mesh.getFaceRLencos(i);

					//call SelectDiffSchemeScalar(i,iScheme,ip,in, &
					//                            Phi,dPhidx,PhiFace)

					fce = MassFlux[i] * PhiFace;

					cupcfd::geometry::euclidean::EuclideanVector<T,3> norm;
					norm = mesh.getFaceNorm(i);
					fde1 = Visac * dPhidxac.dotProduct(norm);

					d1  = Xpn.dotProduct(norm);
					s2  = mesh.getFaceArea(i) * mesh.getFaceArea(i);

					d2  = Xpn * s2/d1;
					d3  = norm - d2;

					// fde2 = Visac * d3.dotProduct(dPhidxac);

					fci = fmin(MassFlux[i], 0.0) * PhiCell[in] + fmax(MassFlux[i], 0.0) * PhiCell[ip];

					fdi = VisFace * dPhidxac.dotProduct(Xpn);

					#ifndef NDEBUG
						if ((i*2)+1 >= nRFace) {
							DEBUGGABLE_ERROR; return cupcfd::error::E_INVALID_INDEX;
						} 
					#endif
					RFace[(i*2)] = -VisFace - fmax(MassFlux[i], 0.0);
					RFace[(i*2)+1] = -VisFace + fmin(MassFlux[i], 0.0);

					blend = GammaBlend * (fce - fci);
					Su[ip] = Su[ip] - blend + fde1 - fdi;
					Su[in] = Su[in] + blend - fde1 + fdi;

					T length;
					Xpn.length(&length);
					peclet = MassFlux[i]/ mesh.getFaceArea(i) * length/(Visac + Small);
					pe0 = fmin(pe0, peclet);
					pe1 = fmax(pe1, peclet);
				}
				else
				{
					#ifndef NDEBUG
						if (ip >= nDen) {
							DEBUGGABLE_ERROR; return cupcfd::error::E_INVALID_INDEX;
						}
					#endif

					ib = mesh.getFaceBoundaryID(i);
					ir = mesh.getBoundaryRegionID(ib);
					it = mesh.getRegionType(ir);
					ip = mesh.getFaceCell1ID(i);

					#ifndef NDEBUG
						if (ib >= nPhiBoundary) {
							DEBUGGABLE_ERROR; return cupcfd::error::E_INVALID_INDEX;
						}
						if (ib >= nVisEffBoundary) {
							DEBUGGABLE_ERROR; return cupcfd::error::E_INVALID_INDEX;
						}
					#endif

					if( it == cupcfd::geometry::mesh::RTYPE_INLET)
					{

					dPhidxac = dPhidx[ip];
					Xac = mesh.getFaceCenter(i);

					// Will Skip User items for Now
					if( ivar == VarT )
					{
						//PhiFace  = Reg(ir)%T;
					}
					else if( ivar == VarTE )
					{
						//PhiFace  = Reg(ir)%k;
					}
					else if( ivar == VarED )
					{
						//PhiFace  = Reg(ir)%e
					}
					// Skip Other handling for now
					//else if( ivar > NVar )
					//{
						//PhiFace  = ScReg(ir,(iVar-Nvar))%value
					//}
					else
					{
						// ToDo: Error Case - Need to change handling, doesn't originally set to 0.0
						PhiFace = T (0);
					}

					Visac = visEffBoundary[ib];

					if( SolveTurb )
					{
						Visac = Visac - VisLam;

						if( ivar == VarT )
						{
							Visac = ( VisLam + Visac / Sigma_T )/Prandtl;
						}
						else if( ivar == VarTE )
						{
							Visac = VisLam + Visac / Sigma_k;
						}
						else if( ivar == VarED )
						{
							Visac = VisLam + Visac / Sigma_e;
						}
						else
						{
							Visac = ( VisLam + Visac / Sigma_s )/Schmidt;
						}
					}
					else
					{
						if( ivar == VarT )
						{
							Visac  = Visac / Prandtl;
						}
						else
						{
							Visac  = Visac / Schmidt;
						}
					}

					Xpn = Xac - mesh.getCellCenter(ip);
					VisFace = Visac * mesh.getFaceRLencos(i);

					norm = mesh.getFaceNorm(i);
					fde = Visac * dPhidxac.dotProduct(norm);

					fce = MassFlux[i] * PhiFace;
					fde = fde;

					fci = fmin( MassFlux[i] , 0.0 ) * PhiFace + fmax(MassFlux[i], 0.0) * PhiCell[ip];
					fdi = VisFace * dPhidxac.dotProduct(Xpn);
					f   = -VisFace + fmin(MassFlux[i], 0.0);

					Au[ip] = Au[ip] - f;
					Su[ip] = Su[ip] + fde - fdi - f*PhiFace;
					PhiBoundary[ib] = PhiFace;

					}
					else if( it == cupcfd::geometry::mesh::RTYPE_OUTLET)
					{
						dPhidxac = dPhidx[ip];
						Xac = mesh.getFaceCenter(i);
						Visac = VisEff[ip];

						if(SolveTurb)
						{
							Visac = Visac - VisLam;

							if(ivar == VarT)
							{
								Visac = ( VisLam + Visac / Sigma_T )/Prandtl;
							}
							else if( ivar == VarTE )
							{
								Visac = VisLam + Visac / Sigma_k;
							}
							else if( ivar == VarED )
							{
								Visac = VisLam + Visac / Sigma_e;
							}
							else
							{
								Visac = ( VisLam + Visac / Sigma_s )/Schmidt;
							}
						}
						else
						{
						if( ivar == VarT )
						{
							Visac  = Visac / Prandtl;
						}
						else
						{
							Visac  = Visac / Schmidt;
						}
						}

						Xpn      = Xac - mesh.getCellCenter(ip);
						PhiFace  = PhiCell[ip] + dPhidx[ip].dotProduct(Xpn);
						VisFace  = Visac * mesh.getFaceRLencos(i);

						fce = MassFlux[i] * PhiFace;
						norm = mesh.getFaceNorm(i);
						fde = Visac * dPhidxac.dotProduct(norm);

						fci = MassFlux[i] * PhiCell[ip];
						fdi = VisFace * dPhidxac.dotProduct(Xpn);

						Su[ip] = Su[ip]  + fde - fdi;
						PhiBoundary[ib] = PhiFace;
					}
					else if(it == cupcfd::geometry::mesh::RTYPE_SYMP)
					{
					ds = mesh.getFaceCenter(i) - mesh.getCellCenter(ip);
					PhiBoundary[ib] = PhiCell[ip] + dPhidx[ip].dotProduct(ds);
					}
					else if(it == cupcfd::geometry::mesh::RTYPE_WALL)
					{
						if(SolveEnthalpy && ivar == VarT)
						{
							#ifndef NDEBUG
								if (ib >= nCpBoundary) {
									DEBUGGABLE_ERROR; return cupcfd::error::E_INVALID_INDEX;
								}
							#endif

							if(mesh.getRegionAdiab(ir))
							{
								PhiBoundary[ib] = PhiCell[ip];
								mesh.setBoundaryQ(ib, 0.0);
							}
							else
							{
								if(mesh.getRegionFlux(ir))
								{
									PhiFlux = mesh.getRegionT(ir);
								}
								else
								{
									PhiFace = mesh.getRegionT(ir);
									PhiBoundary[ib] = PhiFace;
								}

								Visac  = visEffBoundary[ib];
								dn     = mesh.getBoundaryDistance(ib);
								Resist = mesh.getRegionR(ir);

								if(!SolveTurb )
								{
									VisFace = VisLam / Prandtl / dn;
									Hcoef = 1.0/(1.0/VisFace + Resist*CpBoundary[ib]) * mesh.getFaceArea(i);
								}
								else
								{
									#ifndef NDEBUG
										if (ip >= nTE) {
											DEBUGGABLE_ERROR; return cupcfd::error::E_INVALID_INDEX;
										}
									#endif

									SLres = 9.24 * (pow((Prandtl/Sigma_T), 0.75) - 1.0 ) * (1.0 + 0.28 * exp(-0.007 * Prandtl/Sigma_T));
									Cmu25 = pow(TMCmu,0.25);
									Tplus = Sigma_T * (mesh.getBoundaryUPlus(ib) + SLres);
									utau  = Cmu25 * sqrt(TE[ip]);

									if( mesh.getBoundaryYPlus(ib) < mesh.getRegionYLog(ir))
									{
										VisFace = VisLam / Prandtl / dn;
										Hcoef   = 1.0/( 1.0/VisFace + Resist*CpBoundary[ib]) * mesh.getFaceArea(i);
									}
									else
									{
										VisFace = Den[ip] * utau/(Tplus + Small);
										Hcoef   = 1.0/(1.0/VisFace + Resist*CpBoundary[ib]) * mesh.getFaceArea(i);
									}
								}

								if(mesh.getRegionFlux(ir))
								{
									PhiFace = PhiCell[ip] + PhiFlux / (Hcoef * CpBoundary[ib]/ mesh.getFaceArea(i));
									PhiBoundary[ib] = PhiFace;
								}

								Au[ip] = Au[ip] + Hcoef;
								Su[ip] = Su[ip] + Hcoef * PhiFace;

								Tdif = (PhiFace - PhiCell[ip]);
								T tmpVal;

								tmpVal =  Hcoef * CpBoundary[ib] / mesh.getFaceArea(i);
								mesh.setBoundaryH(ib, tmpVal);

								tmpVal = mesh.getBoundaryH(ib) * Tdif;
								mesh.setBoundaryQ(ib, tmpVal);

								tmpVal = VisFace * CpBoundary[ib];
								mesh.setBoundaryH(ib, tmpVal);

								mesh.setBoundaryT(ib, PhiFace);

								if(mesh.getBoundaryQ(ib) > 0.0)
								{
									QTransferIn = QTransferIn + mesh.getBoundaryQ(ib) * mesh.getFaceArea(i);
								}
								else
								{
									QTransferOut = QTransferOut + mesh.getBoundaryQ(ib) * mesh.getFaceArea(i);
								}

								qmin = fmin(qmin, mesh.getBoundaryQ(ib));
								qmax = fmax(qmax, mesh.getBoundaryQ(ib));
								hmin = fmin(hmin, mesh.getBoundaryH(ib));
								hmax = fmax(hmax, mesh.getBoundaryH(ib));

								Atot = Atot + mesh.getFaceArea(i);
								qtot = qtot + mesh.getBoundaryQ(ib) * mesh.getFaceArea(i);
								htot = htot + mesh.getBoundaryH(ib) * mesh.getFaceArea(i);
							}
						}
						// Skip these for now
						//else if( SolveTurb )
						//{
						//}
						//else if( SolveScalars && iVar > Nvar )
						//{
						//   PhiBoundary[ib] = PhiCell[ip];
						//}
						else
						{
						}
					}
				}
			}

			return cupcfd::error::E_SUCCESS;
		}
	}
}

#endif
