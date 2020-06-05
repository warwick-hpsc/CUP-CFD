/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Contains the definitions for the BenchmarkKernels class.
 */

#include "BenchmarkKernels.h"

#include "tt_interface_c.h"
#include "CupCfdAoSMesh.h"
#include "CupCfdSoAMesh.h"

#include <cstdlib>

#include "ArrayKernels.h"

// Kernels
#include "GradientKernels.h"
#include "MassKernels.h"
#include "ScalarKernels.h"
#include "UVWKernels.h"
#include "ViscosityKernels.h"

namespace cupcfd
{
	namespace benchmark
	{
		template <class M, class I, class T, class L>
		BenchmarkKernels<M,I,T,L>::BenchmarkKernels(std::string benchmarkName,
																			std::shared_ptr<cupcfd::geometry::mesh::UnstructuredMeshInterface<M,I,T,L>> meshPtr,
																			I repetitions)
		: Benchmark<I,T>(benchmarkName, repetitions),
		  meshPtr(meshPtr)
		{

		}

		template <class M, class I, class T, class L>
		BenchmarkKernels<M,I,T,L>::~BenchmarkKernels()
		{

		}

		template <class M, class I, class T, class L>
		void BenchmarkKernels<M,I,T,L>::setupBenchmark()
		{
			// Nothing to currently do here
		}

		template <class M, class I, class T, class L>
		void BenchmarkKernels<M,I,T,L>::runBenchmark()
		{

			// Start tracking parameters/time for this block
			this->startBenchmarkBlock(this->benchmarkName);

			// Track Number of Repetitions
			TT_LogParameterInt("Repetitions", this->repetitions);

			for(int i = 0; i < this->repetitions; i++)
			{
				// Run each individual kernel benchmark
				this->GradientPhiGaussDolfynBenchmark();
				this->FluxMassDolfynFaceLoopBenchmark();
				this->FluxMassDolfynBoundaryLoop1Benchmark();
				this->FluxMassDolfynBoundaryLoop2Benchmark();
				this->FluxMassDolfynBoundaryLoop3Benchmark();
				this->FluxMassDolfynBoundaryLoop4Benchmark();
				this->FluxMassDolfynBoundaryLoop5Benchmark();
				this->FluxMassDolfynRegionLoopBenchmark();
				this->FluxScalarDolfynFaceLoopBenchmark();
				this->FluxUVWDolfynFaceLoop1Benchmark();
				this->FluxUVWDolfynRegionLoop1Benchmark();
				this->FluxUVWDolfynBndsLoop1Benchmark();
				this->calculateViscosityDolfynCellLoop1Benchmark();
				this->calculateViscosityDolfynRegionLoopBenchmark();
				this->calculateViscosityDolfynBoundaryLoopBenchmark();
				this->calculateViscosityDolfynCellLoop2Benchmark();
			}

			this->stopBenchmarkBlock(this->benchmarkName);
		}

		template <class M, class I, class T, class L>
		cupcfd::error::eCodes BenchmarkKernels<M,I,T,L>::GradientPhiGaussDolfynBenchmark()
		{
			// This Benchmark needs:
			// (a) An unstructured mesh
			// (b) Two type T arrays, one suitably sized for the cell count, one for the boundary count
			// (c) Two type vector<T> arrays, one suitably sized for the cell count, one for the boundary count
			// (d) Two more type vector<T> arrays, one suitably sized for the cell count, one for the boundary count
			// (e) nGradient parameter - we leave as 1 for now

			// Data Generation:
			// (a) The unstructured mesh is determined by the selected source when running cupcfd
			// (b) Two type T arrays generated by benchmark and populated with random values
			// (c) Two type vector<T> arrays generated by benchmark - populated with random values but kernel will zero
			// them anyway
			// (d) Two type vector<T> arrays generated by benchmark - populated with random values but kernel will zero
			// them anyway

			// === Setup Data ===
			// Sizes
			I nCells = meshPtr->properties.lTCells;
			I nOwnedCells = meshPtr->properties.lOCells;
			I nGhostCells = meshPtr->properties.lGhCells;
			I nBnds = meshPtr->properties.lBoundaries;
			I nFaces = meshPtr->properties.lFaces;

			// Data needed for the kernel
			I nGradient = 1;

			TT_EnterLoop("BenchmarkSetup");

			// Create Random Phi Data for Cells
			T * phiCell = (T *) malloc(sizeof(T) * nCells);
			cupcfd::utility::kernels::randomUniform(phiCell, nCells, (T) 1E-6 , (T) 1E-2);

			// Create RandomPhi Data for Boundaries
			T * phiBoundaries = (T *) malloc(sizeof(T) * nBnds);
			cupcfd::utility::kernels::randomUniform(phiBoundaries, nBnds, (T) 1E-6 , (T) 1E-2);

			// Kernel will zero these arrays regardless of contents
			cupcfd::geometry::euclidean::EuclideanVector<T,3> * dPhidxCell =
					(cupcfd::geometry::euclidean::EuclideanVector<T,3> *)
					malloc(sizeof(cupcfd::geometry::euclidean::EuclideanVector<T,3>) * nCells);

			// Kernel will zero these arrays regardless of contents
			cupcfd::geometry::euclidean::EuclideanVector<T,3> * dPhidxoCell =
					(cupcfd::geometry::euclidean::EuclideanVector<T,3> *)
					malloc(sizeof(cupcfd::geometry::euclidean::EuclideanVector<T,3>) * nCells);

			TT_Exit("BenchmarkSetup");

			// Start Timer
			TT_EnterLoop("GradPhiGaussDolfyn");

			// Track some parameters

			TT_LogParameterInt("LocalCells", nCells);
			TT_LogParameterInt("LocalOwnedCells", nOwnedCells);
			TT_LogParameterInt("LocalGhostCells", nGhostCells);
			TT_LogParameterInt("LocalBounds", nBnds);
			TT_LogParameterInt("LocalFaces", nFaces);

			// ToDo: Should add a configuration option to repeat the kernel X times per timing
			// to reduce impact of overheads at small cell/face counts
			cupcfd::fvm::GradientPhiGaussDolfyn(*meshPtr, nGradient,
													 phiCell, nCells,
													 phiBoundaries, nBnds,
													 dPhidxCell, nCells,
													 dPhidxoCell, nCells);

			// Stop Timer
			TT_Exit("GradPhiGaussDolfyn");

			free(phiCell);
			free(phiBoundaries);
			free(dPhidxCell);
			free(dPhidxoCell);

			return cupcfd::error::E_SUCCESS;
		}

		template <class M, class I, class T, class L>
		cupcfd::error::eCodes BenchmarkKernels<M,I,T,L>::FluxMassDolfynFaceLoopBenchmark()
		{
			I nCells = meshPtr->properties.lTCells;
			I nOwnedCells = meshPtr->properties.lOCells;
			I nGhostCells = meshPtr->properties.lGhCells;
			I nBnds = meshPtr->properties.lBoundaries;
			I nFaces = meshPtr->properties.lFaces;
			I nRegions = meshPtr->properties.lRegions;

			TT_EnterLoop("BenchmarkSetup");

			cupcfd::geometry::euclidean::EuclideanVector<T,3> * dudx =
					(cupcfd::geometry::euclidean::EuclideanVector<T,3> *) malloc(sizeof(cupcfd::geometry::euclidean::EuclideanVector<T,3>) * nCells);

			cupcfd::geometry::euclidean::EuclideanVector<T,3> * dvdx =
					(cupcfd::geometry::euclidean::EuclideanVector<T,3> *) malloc(sizeof(cupcfd::geometry::euclidean::EuclideanVector<T,3>) * nCells);

			cupcfd::geometry::euclidean::EuclideanVector<T,3> * dwdx =
					(cupcfd::geometry::euclidean::EuclideanVector<T,3> *) malloc(sizeof(cupcfd::geometry::euclidean::EuclideanVector<T,3>) * nCells);

			cupcfd::geometry::euclidean::EuclideanVector<T,3> * dpdx =
					(cupcfd::geometry::euclidean::EuclideanVector<T,3> *) malloc(sizeof(cupcfd::geometry::euclidean::EuclideanVector<T,3>) * nCells);

			T * denCell = (T *) malloc(sizeof(T) * nCells);
			cupcfd::utility::kernels::randomUniform(denCell, nCells, (T) 1E-6 , (T) 1E-2);

			T * denBoundary = (T *) malloc(sizeof(T) * nBnds);
			cupcfd::utility::kernels::randomUniform(denBoundary, nBnds, (T) 1E-6 , (T) 1E-2);

			T * uCell = (T *) malloc(sizeof(T) * nCells);
			cupcfd::utility::kernels::randomUniform(uCell, nCells, (T) 1E-6 , (T) 1E-2);

			T * vCell = (T *) malloc(sizeof(T) * nCells);
			cupcfd::utility::kernels::randomUniform(vCell, nCells, (T) 1E-6 , (T) 1E-2);

			T * wCell = (T *) malloc(sizeof(T) * nCells);
			cupcfd::utility::kernels::randomUniform(wCell, nCells, (T) 1E-6 , (T) 1E-2);

			T * massFlux = (T *) malloc(sizeof(T) * nFaces);
			cupcfd::utility::kernels::randomUniform(massFlux, nFaces, (T) 1E-6 , (T) 1E-2);

			T * p = (T *) malloc(sizeof(T) * nCells);
			cupcfd::utility::kernels::randomUniform(p, nCells, (T) 1E-6 , (T) 1E-2);

			T * ar = (T *) malloc(sizeof(T) * nCells);
			cupcfd::utility::kernels::randomUniform(ar, nCells, (T) 1E-6 , (T) 1E-2);

			T * su = (T *) malloc(sizeof(T) * nCells);
			cupcfd::utility::kernels::randomUniform(su, nCells, (T) 1E-6 , (T) 1E-2);

			T * rface = (T *) malloc(sizeof(T) * nFaces * 2);

			T small = 1E-18;
			I icinl;
			I icout;
			I icsym;
			I icwal;
			bool solveTurbEnergy = false;
			bool solveTurbDiss = false;
			bool solveVisc = true;
			bool solveEnthalpy = false;

			T * teCell = (T *) malloc(sizeof(T) * nCells);
			T * teBoundary = (T *) malloc(sizeof(T) * nBnds);
			T * edCell = (T *) malloc(sizeof(T) * nCells);
			T * edBoundary = (T *) malloc(sizeof(T) * nBnds);
			T * viseffCell = (T *) malloc(sizeof(T) * nCells);
			T * viseffBoundary = (T *) malloc(sizeof(T) * nBnds);
			T * tCell = (T *) malloc(sizeof(T) * nCells);
			T * tBoundary = (T *) malloc(sizeof(T) * nBnds);

			TT_Exit("BenchmarkSetup");

			// Start Timer
			TT_EnterLoop("FluxMassDolfyn");

			// Track some parameters
			TT_LogParameterInt("LocalCells", nCells);
			TT_LogParameterInt("LocalOwnedCells", nOwnedCells);
			TT_LogParameterInt("LocalGhostCells", nGhostCells);
			TT_LogParameterInt("LocalBounds", nBnds);
			TT_LogParameterInt("LocalFaces", nFaces);

			// ToDo: Should add a configuration option to repeat the kernel X times per timing
			// to reduce impact of overheads at small cell/face counts
			cupcfd::fvm::FluxMassDolfynFaceLoop(*meshPtr,
					dudx, nCells,
					dvdx, nCells,
					dwdx, nCells,
					dpdx, nCells,
					denCell, nCells,
					denBoundary, nBnds,
					uCell, nCells,
					vCell, nCells,
					wCell, nCells,
					massFlux, nFaces,
					p, nCells,
					ar, nCells,
					su, nCells,
					rface, nFaces * 2,
					small, &icinl, &icout, &icsym, &icwal,
					solveTurbEnergy, solveTurbDiss, solveVisc, solveEnthalpy,
					teCell, nCells,
					teBoundary, nBnds,
					edCell, nCells,
					edBoundary, nBnds,
					viseffCell, nCells,
					viseffBoundary, nBnds,
					tCell, nCells,
					tBoundary, nBnds);

			// Stop Timer
			TT_Exit("FluxMassDolfyn");

			free(dudx);
			free(dvdx);
			free(dwdx);
			free(dpdx);
			free(denCell);
			free(denBoundary);
			free(uCell);
			free(vCell);
			free(wCell);
			free(massFlux);
			free(p);
			free(ar);
			free(su);
			free(rface);

			return cupcfd::error::E_SUCCESS;
		}

		template <class M, class I, class T, class L>
		cupcfd::error::eCodes BenchmarkKernels<M,I,T,L>::FluxMassDolfynBoundaryLoop1Benchmark()
		{
			I nCells = meshPtr->properties.lTCells;
			I nOwnedCells = meshPtr->properties.lOCells;
			I nGhostCells = meshPtr->properties.lGhCells;
			I nBnds = meshPtr->properties.lBoundaries;
			I nFaces = meshPtr->properties.lFaces;
			I nRegions = meshPtr->properties.lRegions;

			 T * massFlux = (T *) malloc(sizeof(T) * nFaces);
			 cupcfd::utility::kernels::randomUniform(massFlux, nFaces, (T) 1E-6 , (T) 1E-2);

			 T flowin;

			// Start Timer
			TT_EnterLoop("FluxMassDolfynBoundary1");

			// Track some parameters
			TT_LogParameterInt("LocalCells", nCells);
			TT_LogParameterInt("LocalOwnedCells", nOwnedCells);
			TT_LogParameterInt("LocalGhostCells", nGhostCells);
			TT_LogParameterInt("LocalBounds", nBnds);
			TT_LogParameterInt("LocalFaces", nFaces);

			// ToDo: Should add a configuration option to repeat the kernel X times per timing
			// to reduce impact of overheads at small cell/face counts
			cupcfd::fvm::FluxMassDolfynBoundaryLoop1(*(this->meshPtr), massFlux, nFaces, &flowin);

			// Stop Timer
			TT_Exit("FluxMassDolfynBoundary1");

			free(massFlux);

			return cupcfd::error::E_SUCCESS;
		}

		template <class M, class I, class T, class L>
		cupcfd::error::eCodes BenchmarkKernels<M,I,T,L>::FluxMassDolfynBoundaryLoop2Benchmark()
		{
			I nCells = meshPtr->properties.lTCells;
			I nOwnedCells = meshPtr->properties.lOCells;
			I nGhostCells = meshPtr->properties.lGhCells;
			I nBnds = meshPtr->properties.lBoundaries;
			I nFaces = meshPtr->properties.lFaces;
			I nRegions = meshPtr->properties.lRegions;

			T * massFlux = (T *) malloc(sizeof(T) * nFaces);
			cupcfd::utility::kernels::randomUniform(massFlux, nFaces, (T) 1E-6 , (T) 1E-2);

			T * flowRegion = (T *) malloc(sizeof(T) * nRegions);
			cupcfd::utility::kernels::randomUniform(flowRegion, nRegions, (T) 1E-6 , (T) 1E-2);

			T flowout;

			// Start Timer
			TT_EnterLoop("FluxMassDolfynBoundary2");

			// Track some parameters
			TT_LogParameterInt("LocalCells", nCells);
			TT_LogParameterInt("LocalOwnedCells", nOwnedCells);
			TT_LogParameterInt("LocalGhostCells", nGhostCells);
			TT_LogParameterInt("LocalBounds", nBnds);
			TT_LogParameterInt("LocalFaces", nFaces);

			// ToDo: Should add a configuration option to repeat the kernel X times per timing
			// to reduce impact of overheads at small cell/face counts
			cupcfd::fvm::FluxMassDolfynBoundaryLoop2(*meshPtr, massFlux, nFaces, flowRegion, nRegions, &flowout);

			// Stop Timer
			TT_Exit("FluxMassDolfynBoundary2");

			free(massFlux);
			free(flowRegion);

			return cupcfd::error::E_SUCCESS;
		}

		template <class M, class I, class T, class L>
		cupcfd::error::eCodes BenchmarkKernels<M,I,T,L>::FluxMassDolfynBoundaryLoop3Benchmark()
		{
			I nCells = meshPtr->properties.lTCells;
			I nOwnedCells = meshPtr->properties.lOCells;
			I nGhostCells = meshPtr->properties.lGhCells;
			I nBnds = meshPtr->properties.lBoundaries;
			I nFaces = meshPtr->properties.lFaces;
			I nRegions = meshPtr->properties.lRegions;

			T flowin = 1.1;
			T ratearea;

			// Start Timer
			TT_EnterLoop("FluxMassDolfynBoundary3");

			// Track some parameters
			TT_LogParameterInt("LocalCells", nCells);
			TT_LogParameterInt("LocalOwnedCells", nOwnedCells);
			TT_LogParameterInt("LocalGhostCells", nGhostCells);
			TT_LogParameterInt("LocalBounds", nBnds);
			TT_LogParameterInt("LocalFaces", nFaces);

			// ToDo: Should add a configuration option to repeat the kernel X times per timing
			// to reduce impact of overheads at small cell/face counts
			cupcfd::fvm::FluxMassDolfynBoundaryLoop3(*meshPtr, flowin, &ratearea);

			// Stop Timer
			TT_Exit("FluxMassDolfynBoundary3");

			return cupcfd::error::E_SUCCESS;
		}

		template <class M, class I, class T, class L>
		cupcfd::error::eCodes BenchmarkKernels<M,I,T,L>::FluxMassDolfynBoundaryLoop4Benchmark()
		{
			I nCells = meshPtr->properties.lTCells;
			I nOwnedCells = meshPtr->properties.lOCells;
			I nGhostCells = meshPtr->properties.lGhCells;
			I nBnds = meshPtr->properties.lBoundaries;
			I nFaces = meshPtr->properties.lFaces;
			I nRegions = meshPtr->properties.lRegions;

			TT_EnterLoop("BenchmarkSetup");

			T * massFlux = (T *) malloc(sizeof(T) * nFaces);
			cupcfd::utility::kernels::randomUniform(massFlux, nFaces, (T) 1E-6 , (T) 1E-2);

			T * uBoundary = (T *) malloc(sizeof(T) * nBnds);
			cupcfd::utility::kernels::randomUniform(uBoundary, nBnds, (T) 1E-6 , (T) 1E-2);

			T * vBoundary = (T *) malloc(sizeof(T) * nBnds);
			cupcfd::utility::kernels::randomUniform(vBoundary, nBnds, (T) 1E-6 , (T) 1E-2);

			T * wBoundary = (T *) malloc(sizeof(T) * nBnds);
			cupcfd::utility::kernels::randomUniform(wBoundary, nBnds, (T) 1E-6 , (T) 1E-2);

			T * denBoundary = (T *) malloc(sizeof(T) * nBnds);
			cupcfd::utility::kernels::randomUniform(denBoundary, nBnds, (T) 1E-6 , (T) 1E-2);

			T ratearea = 1.0;
			T flowout;

			TT_Exit("BenchmarkSetup");

			// Start Timer
			TT_EnterLoop("FluxMassDolfynBoundary4");

			// Track some parameters
			TT_LogParameterInt("LocalCells", nCells);
			TT_LogParameterInt("LocalOwnedCells", nOwnedCells);
			TT_LogParameterInt("LocalGhostCells", nGhostCells);
			TT_LogParameterInt("LocalBounds", nBnds);
			TT_LogParameterInt("LocalFaces", nFaces);

			// ToDo: Should add a configuration option to repeat the kernel X times per timing
			// to reduce impact of overheads at small cell/face counts
			cupcfd::fvm::FluxMassDolfynBoundaryLoop4(*(this->meshPtr), massFlux, nFaces,
																			 uBoundary, nBnds,
																			 vBoundary, nBnds,
																			 wBoundary, nBnds,
																			 denBoundary, nBnds,
																			 ratearea,
																			 &flowout);

			// Stop Timer
			TT_Exit("FluxMassDolfynBoundary4");

			free(massFlux);
			free(uBoundary);
			free(vBoundary);
			free(wBoundary);
			free(denBoundary);

			return cupcfd::error::E_SUCCESS;
		}

		template <class M, class I, class T, class L>
		cupcfd::error::eCodes BenchmarkKernels<M,I,T,L>::FluxMassDolfynBoundaryLoop5Benchmark()
		{
			I nCells = meshPtr->properties.lTCells;
			I nOwnedCells = meshPtr->properties.lOCells;
			I nGhostCells = meshPtr->properties.lGhCells;
			I nBnds = meshPtr->properties.lBoundaries;
			I nFaces = meshPtr->properties.lFaces;
			I nRegions = meshPtr->properties.lRegions;

			TT_EnterLoop("BenchmarkSetup");

			T * massFlux = (T *) malloc(sizeof(T) * nFaces);
			cupcfd::utility::kernels::randomUniform(massFlux, nFaces, (T) 1E-6 , (T) 1E-2);

			T * su = (T *) malloc(sizeof(T) * nCells);
			cupcfd::utility::kernels::randomUniform(su, nCells, (T) 1E-6 , (T) 1E-2);

			T * uBoundary = (T *) malloc(sizeof(T) * nBnds);
			cupcfd::utility::kernels::randomUniform(uBoundary, nBnds, (T) 1E-6 , (T) 1E-2);

			T * vBoundary = (T *) malloc(sizeof(T) * nBnds);
			cupcfd::utility::kernels::randomUniform(vBoundary, nBnds, (T) 1E-6 , (T) 1E-2);

			T * wBoundary = (T *) malloc(sizeof(T) * nBnds);
			cupcfd::utility::kernels::randomUniform(wBoundary, nBnds, (T) 1E-6 , (T) 1E-2);

			T fact = 1.1;
			bool solveU = true;
			bool solveV = true;
			bool solveW = true;

			T * flowFact = (T *) malloc(sizeof(T) * nRegions);
			T flowout2;

			TT_Exit("BenchmarkSetup");

			// Start Timer
			TT_EnterLoop("FluxMassDolfynBoundary5");

			// Track some parameters
			TT_LogParameterInt("LocalCells", nCells);
			TT_LogParameterInt("LocalOwnedCells", nOwnedCells);
			TT_LogParameterInt("LocalGhostCells", nGhostCells);
			TT_LogParameterInt("LocalBounds", nBnds);
			TT_LogParameterInt("LocalFaces", nFaces);

			// ToDo: Should add a configuration option to repeat the kernel X times per timing
			// to reduce impact of overheads at small cell/face counts
			cupcfd::fvm::FluxMassDolfynBoundaryLoop5(*(meshPtr),
																	   massFlux, nFaces,
																	   su, nCells,
																			 uBoundary, nBnds,
																			 vBoundary, nBnds,
																			 wBoundary, nBnds,
																			 fact,
																			 solveU, solveV, solveW,
																			 flowFact, nRegions,
																			 &flowout2);

			// Stop Timer
			TT_Exit("FluxMassDolfynBoundary5");

			free(massFlux);
			free(su);
			free(uBoundary);
			free(vBoundary);
			free(wBoundary);

			return cupcfd::error::E_SUCCESS;
		}

		template <class M, class I, class T, class L>
		cupcfd::error::eCodes BenchmarkKernels<M,I,T,L>::FluxMassDolfynRegionLoopBenchmark()
		{
			I nCells = meshPtr->properties.lTCells;
			I nOwnedCells = meshPtr->properties.lOCells;
			I nGhostCells = meshPtr->properties.lGhCells;
			I nBnds = meshPtr->properties.lBoundaries;
			I nFaces = meshPtr->properties.lFaces;
			I nRegions = meshPtr->properties.lRegions;

			TT_EnterLoop("BenchmarkSetup");

			T * flowFact = (T *) malloc(sizeof(T) * nRegions);
			cupcfd::utility::kernels::randomUniform(flowFact, nRegions, (T) 1E-6 , (T) 1E-2);

			T * flowRegion = (T *) malloc(sizeof(T) * nRegions);
			cupcfd::utility::kernels::randomUniform(flowRegion, nRegions, (T) 1E-6 , (T) 1E-2);

			T flowIn = 0.0;

			TT_Exit("BenchmarkSetup");

			// Start Timer
			TT_EnterLoop("FluxMassDolfynRegion");

			// Track some parameters
			TT_LogParameterInt("LocalCells", nCells);
			TT_LogParameterInt("LocalOwnedCells", nOwnedCells);
			TT_LogParameterInt("LocalGhostCells", nGhostCells);
			TT_LogParameterInt("LocalBounds", nBnds);
			TT_LogParameterInt("LocalFaces", nFaces);

			// ToDo: Should add a configuration option to repeat the kernel X times per timing
			// to reduce impact of overheads at small cell/face counts
			cupcfd::fvm::FluxMassDolfynRegionLoop(*(this->meshPtr),
														  flowFact, nRegions,
														  flowRegion, nRegions,
														  flowIn);

			// Stop Timer
			TT_Exit("FluxMassDolfynRegion");

			free(flowFact);
			free(flowRegion);

			return cupcfd::error::E_SUCCESS;
		}

		template <class M, class I, class T, class L>
		cupcfd::error::eCodes BenchmarkKernels<M,I,T,L>::FluxScalarDolfynFaceLoopBenchmark()
		{
			I nCells = meshPtr->properties.lTCells;
			I nOwnedCells = meshPtr->properties.lOCells;
			I nGhostCells = meshPtr->properties.lGhCells;
			I nBnds = meshPtr->properties.lBoundaries;
			I nFaces = meshPtr->properties.lFaces;
			I nRegions = meshPtr->properties.lRegions;

			TT_EnterLoop("BenchmarkSetup");

			T * PhiCell = (T *) malloc(sizeof(T) * nCells);
			cupcfd::utility::kernels::randomUniform(PhiCell, nCells, (T) 1E-6 , (T) 1E-2);

			T * VisEff = (T *) malloc(sizeof(T) * nCells);
			cupcfd::utility::kernels::randomUniform(VisEff, nCells, (T) 1E-6 , (T) 1E-2);

			T * Au = (T *) malloc(sizeof(T) * nCells);
			cupcfd::utility::kernels::randomUniform(Au, nCells, (T) 1E-6 , (T) 1E-2);

			T * Su = (T *) malloc(sizeof(T) * nCells);
			cupcfd::utility::kernels::randomUniform(Su, nCells, (T) 1E-6 , (T) 1E-2);

			T * Den = (T *) malloc(sizeof(T) * nCells);
			cupcfd::utility::kernels::randomUniform(Den, nCells, (T) 1E-6 , (T) 1E-2);

			T * MassFlux = (T *) malloc(sizeof(T) * nFaces);
			cupcfd::utility::kernels::randomUniform(MassFlux, nFaces, (T) 1E-6 , (T) 1E-2);

			T * TE = (T *) malloc(sizeof(T) * nCells);
			cupcfd::utility::kernels::randomUniform(TE, nCells, (T) 1E-6 , (T) 1E-2);

			T * PhiBoundary = (T *) malloc(sizeof(T) * nBnds);
			cupcfd::utility::kernels::randomUniform(PhiBoundary, nBnds, (T) 1E-6 , (T) 1E-2);

			T * CpBoundary = (T *) malloc(sizeof(T) * nBnds);
			cupcfd::utility::kernels::randomUniform(CpBoundary, nBnds, (T) 1E-6 , (T) 1E-2);

			T * visEffBoundary = (T *) malloc(sizeof(T) * nBnds);
			cupcfd::utility::kernels::randomUniform(visEffBoundary, nBnds, (T) 1E-6 , (T) 1E-2);

			T * RFace = (T *) malloc(sizeof(T) * nFaces * 2);

			cupcfd::geometry::euclidean::EuclideanVector<T,3> * dPhidx =
				(cupcfd::geometry::euclidean::EuclideanVector<T,3> *)
				malloc(sizeof(cupcfd::geometry::euclidean::EuclideanVector<T,3>) * nCells);

			bool SolveTurb = true;
			bool SolveEnthalpy = false;
			T sigma = 1.0;
			T sigma2 = 1.2;
			T vislam = 1.3;
			int ivar = 1;
			int VarT = 2;
			T Sigma_T = 1.4;
			T Prandtl = 1.2E-3;
			int VarTE = 3;
			T Sigma_k = 1.5;
			int VarED = 4;
			T Sigma_e = 1.6;
			T Sigma_s = 1.7;
			T Schmidt = 1.4E-3;
			T GammaBlend = 1.1;
			T Small = 1E-18;
			T Large = 1E+18;
			T TMCmu = 1.8;

			TT_Exit("BenchmarkSetup");

			// Start Timer
			TT_EnterLoop("FluxScalarDolfyn");

			// Track some parameters
			TT_LogParameterInt("LocalCells", nCells);
			TT_LogParameterInt("LocalOwnedCells", nOwnedCells);
			TT_LogParameterInt("LocalGhostCells", nGhostCells);
			TT_LogParameterInt("LocalBounds", nBnds);
			TT_LogParameterInt("LocalFaces", nFaces);

			// ToDo: Should add a configuration option to repeat the kernel X times per timing
			// to reduce impact of overheads at small cell/face counts
			cupcfd::fvm::FluxScalarDolfynFaceLoop(*(this->meshPtr),
																	PhiCell, nCells,
																	PhiBoundary, nBnds,
																	VisEff, nCells,
																	Au, nCells,
																	Su, nCells,
																	Den, nCells,
																	MassFlux, nFaces,
																	TE, nCells,
																	CpBoundary, nBnds,
																	visEffBoundary, nBnds,
																	RFace, nFaces * 2,
																	dPhidx, nCells,
																	SolveTurb, SolveEnthalpy,
																	sigma, sigma2, vislam,
																	ivar, VarT, Sigma_T, Prandtl,
																	VarTE, Sigma_k, VarED, Sigma_e,
																	Sigma_s, Schmidt, GammaBlend, Small, Large, TMCmu);

			// Stop Timer
			TT_Exit("FluxScalarDolfyn");

			free(PhiCell);
			free(VisEff);
			free(Au);
			free(Su);
			free(Den);
			free(MassFlux);
			free(TE);
			free(CpBoundary);
			free(visEffBoundary);
			free(RFace);
			free(dPhidx);

			return cupcfd::error::E_SUCCESS;
		}

		template <class M, class I, class T, class L>
		cupcfd::error::eCodes BenchmarkKernels<M,I,T,L>::FluxUVWDolfynFaceLoop1Benchmark()
		{
			I nCells = meshPtr->properties.lTCells;
			I nOwnedCells = meshPtr->properties.lOCells;
			I nGhostCells = meshPtr->properties.lGhCells;
			I nBnds = meshPtr->properties.lBoundaries;
			I nFaces = meshPtr->properties.lFaces;
			I nRegions = meshPtr->properties.lRegions;

			T gammaBlend = 0.9;
			T small = 1E-18;
			T large = 1E+18;

			TT_EnterLoop("BenchmarkSetup");

			T * uCell = (T *) malloc(sizeof(T) * nCells);
			cupcfd::utility::kernels::randomUniform(uCell, nCells, (T) 1E-6 , (T) 1E-2);

			T * vCell = (T *) malloc(sizeof(T) * nCells);
			cupcfd::utility::kernels::randomUniform(vCell, nCells, (T) 1E-6 , (T) 1E-2);

			T * wCell = (T *) malloc(sizeof(T) * nCells);
			cupcfd::utility::kernels::randomUniform(wCell, nCells, (T) 1E-6 , (T) 1E-2);

			T * visEffCell = (T *) malloc(sizeof(T) * nCells);
			cupcfd::utility::kernels::randomUniform(visEffCell, nCells, (T) 1E-6 , (T) 1E-2);

			T * massFlux = (T *) malloc(sizeof(T) * nFaces);
			cupcfd::utility::kernels::randomUniform(massFlux, nFaces, (T) 1E-6 , (T) 1E-2);

			T * su = (T *) malloc(sizeof(T) * nCells);
			cupcfd::utility::kernels::randomUniform(su, nCells, (T) 1E-6 , (T) 1E-2);

			T * sv = (T *) malloc(sizeof(T) * nCells);
			cupcfd::utility::kernels::randomUniform(sv, nCells, (T) 1E-6 , (T) 1E-2);

			T * sw = (T *) malloc(sizeof(T) * nCells);
			cupcfd::utility::kernels::randomUniform(sw, nCells, (T) 1E-6 , (T) 1E-2);

			T * au = (T *) malloc(sizeof(T) * nCells);
			cupcfd::utility::kernels::randomUniform(au, nCells, (T) 1E-6 , (T) 1E-2);

			T * av = (T *) malloc(sizeof(T) * nCells);
			cupcfd::utility::kernels::randomUniform(av, nCells, (T) 1E-6 , (T) 1E-2);

			T * aw = (T *) malloc(sizeof(T) * nCells);
			cupcfd::utility::kernels::randomUniform(aw, nCells, (T) 1E-6 , (T) 1E-2);

			cupcfd::geometry::euclidean::EuclideanVector<T,3> * dudx =
					(cupcfd::geometry::euclidean::EuclideanVector<T,3> *) malloc(sizeof(cupcfd::geometry::euclidean::EuclideanVector<T,3>) * nCells);

			cupcfd::geometry::euclidean::EuclideanVector<T,3> * dvdx =
					(cupcfd::geometry::euclidean::EuclideanVector<T,3> *) malloc(sizeof(cupcfd::geometry::euclidean::EuclideanVector<T,3>) * nCells);

			cupcfd::geometry::euclidean::EuclideanVector<T,3> * dwdx =
					(cupcfd::geometry::euclidean::EuclideanVector<T,3> *) malloc(sizeof(cupcfd::geometry::euclidean::EuclideanVector<T,3>) * nCells);

			T * uBoundary = (T *) malloc(sizeof(T) * nBnds);
			cupcfd::utility::kernels::randomUniform(uBoundary, nBnds, (T) 1E-6 , (T) 1E-2);

			T * vBoundary = (T *) malloc(sizeof(T) * nBnds);
			cupcfd::utility::kernels::randomUniform(vBoundary, nBnds, (T) 1E-6 , (T) 1E-2);

			T * wBoundary = (T *) malloc(sizeof(T) * nBnds);
			cupcfd::utility::kernels::randomUniform(wBoundary, nBnds, (T) 1E-6 , (T) 1E-2);

			T * visEffBoundary = (T *) malloc(sizeof(T) * nBnds);
			cupcfd::utility::kernels::randomUniform(visEffBoundary, nBnds, (T) 1E-6 , (T) 1E-2);

			// [nfaces][2]
			T * rFace = (T *) malloc(sizeof(T) * nFaces * 2);

			TT_Exit("BenchmarkSetup");

			// Start Timer
			TT_EnterLoop("FluxUVWDolfynFace1");

			// Track some parameters
			TT_LogParameterInt("LocalCells", nCells);
			TT_LogParameterInt("LocalOwnedCells", nOwnedCells);
			TT_LogParameterInt("LocalGhostCells", nGhostCells);
			TT_LogParameterInt("LocalBounds", nBnds);
			TT_LogParameterInt("LocalFaces", nFaces);

			// ToDo: Should add a configuration option to repeat the kernel X times per timing
			// to reduce impact of overheads at small cell/face counts
			cupcfd::fvm::FluxUVWDolfynFaceLoop1(*(meshPtr), gammaBlend, small, large,
															uCell, nCells,
															vCell, nCells,
															wCell, nCells,
															uBoundary, nBnds,
															vBoundary, nBnds,
															wBoundary, nBnds,
															visEffCell, nCells,
															visEffBoundary, nBnds,
															massFlux, nFaces,
															dudx, nCells,
															dvdx, nCells,
															dwdx, nCells,
															rFace, nFaces * 2,
															su, nCells,
															sv, nCells,
															sw, nCells,
															au, nCells,
															av, nCells,
															aw, nCells);

			// Stop Timer
			TT_Exit("FluxUVWDolfynFace1");

			free(uCell);
			free(vCell);
			free(wCell);
			free(visEffCell);
			free(massFlux);
			free(su);
			free(sv);
			free(sw);
			free(au);
			free(av);
			free(aw);
			free(dudx);
			free(dvdx);
			free(dwdx);
			free(uBoundary);
			free(vBoundary);
			free(wBoundary);
			free(visEffBoundary);
			free(rFace);

			return cupcfd::error::E_SUCCESS;
		}

		template <class M, class I, class T, class L>
		cupcfd::error::eCodes BenchmarkKernels<M,I,T,L>::FluxUVWDolfynRegionLoop1Benchmark()
		{
			I nCells = meshPtr->properties.lTCells;
			I nOwnedCells = meshPtr->properties.lOCells;
			I nGhostCells = meshPtr->properties.lGhCells;
			I nBnds = meshPtr->properties.lBoundaries;
			I nFaces = meshPtr->properties.lFaces;
			I nRegions = meshPtr->properties.lRegions;

			// Start Timer
			TT_EnterLoop("FluxUVWDolfynRegion1");

			// Track some parameters
			TT_LogParameterInt("LocalCells", nCells);
			TT_LogParameterInt("LocalOwnedCells", nOwnedCells);
			TT_LogParameterInt("LocalGhostCells", nGhostCells);
			TT_LogParameterInt("LocalBounds", nBnds);
			TT_LogParameterInt("LocalFaces", nFaces);

			// ToDo: Should add a configuration option to repeat the kernel X times per timing
			// to reduce impact of overheads at small cell/face counts
			cupcfd::fvm::FluxUVWDolfynRegionLoop1(*(meshPtr));

			// Stop Timer
			TT_Exit("FluxUVWDolfynRegion1");

			return cupcfd::error::E_SUCCESS;
		}

		template <class M, class I, class T, class L>
		cupcfd::error::eCodes BenchmarkKernels<M,I,T,L>::FluxUVWDolfynBndsLoop1Benchmark()
		{
			I nCells = meshPtr->properties.lTCells;
			I nOwnedCells = meshPtr->properties.lOCells;
			I nGhostCells = meshPtr->properties.lGhCells;
			I nBnds = meshPtr->properties.lBoundaries;
			I nFaces = meshPtr->properties.lFaces;
			I nRegions = meshPtr->properties.lRegions;

			// Start Timer
			TT_EnterLoop("FluxUVWDolfynBnds1");

			// Track some parameters
			TT_LogParameterInt("LocalCells", nCells);
			TT_LogParameterInt("LocalOwnedCells", nOwnedCells);
			TT_LogParameterInt("LocalGhostCells", nGhostCells);
			TT_LogParameterInt("LocalBounds", nBnds);
			TT_LogParameterInt("LocalFaces", nFaces);

			// ToDo: Should add a configuration option to repeat the kernel X times per timing
			// to reduce impact of overheads at small cell/face counts
			cupcfd::fvm::FluxUVWDolfynBndsLoop1(*(meshPtr));

			// Stop Timer
			TT_Exit("FluxUVWDolfynBnds1");

			return cupcfd::error::E_SUCCESS;
		}

		template <class M, class I, class T, class L>
		cupcfd::error::eCodes BenchmarkKernels<M,I,T,L>::calculateViscosityDolfynCellLoop1Benchmark()
		{
			I nCells = meshPtr->properties.lTCells;
			I nOwnedCells = meshPtr->properties.lOCells;
			I nGhostCells = meshPtr->properties.lGhCells;
			I nBnds = meshPtr->properties.lBoundaries;
			I nFaces = meshPtr->properties.lFaces;
			I nRegions = meshPtr->properties.lRegions;

			// Arbitrary values
			T small = 1E-18;
			T cmu = 1E-4;
			T visURF = 0.232;
			T visLam = 0.00547;

			TT_EnterLoop("BenchmarkSetup");

			T * TE = (T *) malloc(sizeof(T) * nCells);
			cupcfd::utility::kernels::randomUniform(TE, nCells, (T) 1E-6 , (T) 1E-2);

			T * ED = (T *) malloc(sizeof(T) * nCells);
			cupcfd::utility::kernels::randomUniform(ED, nCells, (T) 1E-6 , (T) 1E-2);

			T * den = (T *) malloc(sizeof(T) * nCells);
			cupcfd::utility::kernels::randomUniform(den, nCells, (T) 1E-6 , (T) 1E-2);

			T * visEff = (T *) malloc(sizeof(T) * nCells);
			cupcfd::utility::kernels::randomUniform(visEff, nCells, (T) 1E-6 , (T) 1E-2);

			TT_Exit("BenchmarkSetup");

			// Start Timer
			TT_EnterLoop("calcViscDolfynCell1");

			// Track some parameters
			TT_LogParameterInt("LocalCells", nCells);
			TT_LogParameterInt("LocalOwnedCells", nOwnedCells);
			TT_LogParameterInt("LocalGhostCells", nGhostCells);
			TT_LogParameterInt("LocalBounds", nBnds);
			TT_LogParameterInt("LocalFaces", nFaces);

			// ToDo: Should add a configuration option to repeat the kernel X times per timing
			// to reduce impact of overheads at small cell/face counts
			cupcfd::fvm::calculateViscosityDolfynCellLoop1(*(meshPtr), small, cmu, visURF, visLam,
																						TE, nCells,
																						ED, nCells,
																						den, nCells,
																						visEff, nCells);

			// Stop Timer
			TT_Exit("calcViscDolfynCell1");

			free(TE);
			free(ED);
			free(den);
			free(visEff);

			return cupcfd::error::E_SUCCESS;
		}

		template <class M, class I, class T, class L>
		cupcfd::error::eCodes BenchmarkKernels<M,I,T,L>::calculateViscosityDolfynRegionLoopBenchmark()
		{
			I nCells = meshPtr->properties.lTCells;
			I nOwnedCells = meshPtr->properties.lOCells;
			I nGhostCells = meshPtr->properties.lGhCells;
			I nBnds = meshPtr->properties.lBoundaries;
			I nFaces = meshPtr->properties.lFaces;
			I nRegions = meshPtr->properties.lRegions;

			T kappa = 1.1;

			// Start Timer
			TT_EnterLoop("calViscDolfynRegion");

			// Track some parameters
			TT_LogParameterInt("LocalCells", nCells);
			TT_LogParameterInt("LocalOwnedCells", nOwnedCells);
			TT_LogParameterInt("LocalGhostCells", nGhostCells);
			TT_LogParameterInt("LocalBounds", nBnds);
			TT_LogParameterInt("LocalFaces", nFaces);

			// ToDo: Should add a configuration option to repeat the kernel X times per timing
			// to reduce impact of overheads at small cell/face counts
			cupcfd::fvm::calculateViscosityDolfynRegionLoop(kappa, *(this->meshPtr));

			// Stop Timer
			TT_Exit("calViscDolfynRegion");

			return cupcfd::error::E_SUCCESS;
		}

		template <class M, class I, class T, class L>
		cupcfd::error::eCodes BenchmarkKernels<M,I,T,L>::calculateViscosityDolfynBoundaryLoopBenchmark()
		{
			I nCells = meshPtr->properties.lTCells;
			I nOwnedCells = meshPtr->properties.lOCells;
			I nGhostCells = meshPtr->properties.lGhCells;
			I nBnds = meshPtr->properties.lBoundaries;
			I nFaces = meshPtr->properties.lFaces;
			I nRegions = meshPtr->properties.lRegions;

			T tmcmu = 1.0;
			T large = 1.0E+18;
			T small = 1.0E-18;
			T kappa = 1.1;
			T visLam = 1.2;

			T * TE = (T *) malloc(sizeof(T) * nCells);
			T * den = (T *) malloc(sizeof(T) * nCells);
			T * visEffCell = (T *) malloc(sizeof(T) * nCells);
			T * visEffBoundary = (T *) malloc(sizeof(T) * nBnds);

			// Start Timer
			TT_EnterLoop("calcViscDolfynBoundary");

			// Track some parameters
			TT_LogParameterInt("LocalCells", nCells);
			TT_LogParameterInt("LocalOwnedCells", nOwnedCells);
			TT_LogParameterInt("LocalGhostCells", nGhostCells);
			TT_LogParameterInt("LocalBounds", nBnds);
			TT_LogParameterInt("LocalFaces", nFaces);

			// ToDo: Should add a configuration option to repeat the kernel X times per timing
			// to reduce impact of overheads at small cell/face counts
			cupcfd::fvm::calculateViscosityDolfynBoundaryLoop(*(this->meshPtr),
						tmcmu, large, small, kappa, visLam,
						TE, nCells,
						den, nCells,
						visEffCell, nCells,
						visEffBoundary, nBnds);

			// Stop Timer
			TT_Exit("calcViscDolfynBoundary");

			free(TE);
			free(den);
			free(visEffCell);
			free(visEffBoundary);

			return cupcfd::error::E_SUCCESS;
		}

		template <class M, class I, class T, class L>
		cupcfd::error::eCodes BenchmarkKernels<M,I,T,L>::calculateViscosityDolfynCellLoop2Benchmark()
		{
			I nCells = meshPtr->properties.lTCells;
			I nOwnedCells = meshPtr->properties.lOCells;
			I nGhostCells = meshPtr->properties.lGhCells;
			I nBnds = meshPtr->properties.lBoundaries;
			I nFaces = meshPtr->properties.lFaces;
			I nRegions = meshPtr->properties.lRegions;

			TT_EnterLoop("BenchmarkSetup");

			T visLam = 1.1;
			T * visEffCell = (T *) malloc(sizeof(T) * nCells);
			cupcfd::utility::kernels::randomUniform(visEffCell, nCells, (T) 1E-6 , (T) 1E-2);

			T * visEffBoundary = (T *) malloc(sizeof(T) * nBnds);
			cupcfd::utility::kernels::randomUniform(visEffBoundary, nBnds, (T) 1E-6 , (T) 1E-2);

			TT_Exit("BenchmarkSetup");

			// Start Timer
			TT_EnterLoop("calViscDolfynCell2");

			// Track some parameters
			TT_LogParameterInt("LocalCells", nCells);
			TT_LogParameterInt("LocalOwnedCells", nOwnedCells);
			TT_LogParameterInt("LocalGhostCells", nGhostCells);
			TT_LogParameterInt("LocalBounds", nBnds);
			TT_LogParameterInt("LocalFaces", nFaces);

			// ToDo: Should add a configuration option to repeat the kernel X times per timing
			// to reduce impact of overheads at small cell/face counts
			cupcfd::fvm::calculateViscosityDolfynCellLoop2(*(this->meshPtr), visLam,
																					    visEffCell, nCells,
																					    visEffBoundary, nBnds);


			// Stop Timer
			TT_Exit("calViscDolfynCell2");

			free(visEffCell);
			free(visEffBoundary);

			return cupcfd::error::E_SUCCESS;
		}
	}
}

// Explicit Instantiation
template class cupcfd::benchmark::BenchmarkKernels<cupcfd::geometry::mesh::CupCfdAoSMesh<int, float, int>, int, float, int>;
template class cupcfd::benchmark::BenchmarkKernels<cupcfd::geometry::mesh::CupCfdAoSMesh<int, double, int>, int, double, int>;

template class cupcfd::benchmark::BenchmarkKernels<cupcfd::geometry::mesh::CupCfdSoAMesh<int, float, int>, int, float, int>;
template class cupcfd::benchmark::BenchmarkKernels<cupcfd::geometry::mesh::CupCfdSoAMesh<int, double, int>, int, double, int>;
