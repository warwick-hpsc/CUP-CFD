/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Tests for the ScalarKernels Operations
 */

#define BOOST_TEST_MODULE ScalarKernels
#include <boost/test/unit_test.hpp>
#include <boost/test/output_test_stream.hpp>

#include <stdexcept>
#include <string>

#include "ScalarKernels.h"
#include "MeshConfig.h"
#include "MeshSourceStructGenConfig.h"
#include "CupCfdAoSMesh.h"
#include "EuclideanVector.h"

#include "PartitionerNaiveConfig.h"
#include "PartitionerConfig.h"

#include <cstdlib>

#include <iostream>

using namespace cupcfd::fvm;

namespace utf = boost::unit_test;
namespace euc = cupcfd::geometry::euclidean;
namespace meshgeo = cupcfd::geometry::mesh;

// Setup
BOOST_AUTO_TEST_CASE(setup)
{
    int argc = boost::unit_test::framework::master_test_suite().argc;
    char ** argv = boost::unit_test::framework::master_test_suite().argv;

    MPI_Init(&argc, &argv);
}


// === FluxScalarDolfynFaceLoop ===
// Test 1: Test it runs without error on a small mesh
BOOST_AUTO_TEST_CASE(FluxScalarDolfynFaceLoop_test1)
{
	cupcfd::error::eCodes status;
    cupcfd::comm::Communicator comm(MPI_COMM_WORLD);

	// Create a small test mesh
    cupcfd::partitioner::PartitionerNaiveConfig<int,int> partConfig;

	// Setup the source config
    meshgeo::MeshSourceStructGenConfig<int, double> meshSourceConfig(5, 5, 5, -1.0, 1.0, -1.0, 1.0, -1.0, 1.0);

	// Setup the config to use for building
    meshgeo::MeshConfig<int, double, int> meshConfig(partConfig, meshSourceConfig);

    meshgeo::CupCfdAoSMesh<int,double,int> * mesh;
	status = meshConfig.buildUnstructuredMesh(&mesh, comm);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	int nCells = mesh->properties.lTCells;
	int nOwnedCells = mesh->properties.lOCells;
	int nGhostCells = mesh->properties.lGhCells;
	int nBnds = mesh->properties.lBoundaries;
	int nFaces = mesh->properties.lFaces;
	int nRegions = mesh->properties.lRegions;
	int repetitions = 1000;

	double * PhiCell = (double *) malloc(sizeof(double) * nCells);
	double * VisEff = (double *) malloc(sizeof(double) * nCells);
	double * Au = (double *) malloc(sizeof(double) * nCells);
	double * Su = (double *) malloc(sizeof(double) * nCells);
	double * Den = (double *) malloc(sizeof(double) * nCells);
	double * MassFlux = (double *) malloc(sizeof(double) * nFaces);
	double * TE = (double *) malloc(sizeof(double) * nCells);
	double * PhiBoundary = (double *) malloc(sizeof(double) * nBnds);
	double * CpBoundary = (double *) malloc(sizeof(double) * nBnds);
	double * visEffBoundary = (double *) malloc(sizeof(double) * nBnds);
	double * RFace = (double *) malloc(sizeof(double) * nFaces * 2);
	cupcfd::geometry::euclidean::EuclideanVector<double,3> * dPhidx =
		(cupcfd::geometry::euclidean::EuclideanVector<double,3> *)
		malloc(sizeof(cupcfd::geometry::euclidean::EuclideanVector<double,3>) * nCells);

	bool SolveTurb = true;
	bool SolveEnthalpy = false;
	double sigma = 1.0;
	double sigma2 = 1.2;
	double vislam = 1.3;
	int ivar = 1;
	int VarT = 2;
	double Sigma_T = 1.4;
	double Prandtl = 1.2E-3;
	int VarTE = 3;
	double Sigma_k = 1.5;
	int VarED = 4;
	double Sigma_e = 1.6;
	double Sigma_s = 1.7;
	double Schmidt = 1.4E-3;
	double GammaBlend = 1.1;
	double Small = 1E-18;
	double Large = 1E+18;
	double TMCmu = 1.8;

	// Won't bother populating the data arrays for now


	cupcfd::fvm::FluxScalarDolfynFaceLoop(*mesh,
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

	delete(mesh);
}

BOOST_AUTO_TEST_CASE(cleanup)
{
    MPI_Finalize();
}
