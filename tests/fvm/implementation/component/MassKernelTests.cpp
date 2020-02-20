/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Tests for the MassKernels Operations
 */

#define BOOST_TEST_MODULE MassKernels
#include <boost/test/unit_test.hpp>
#include <boost/test/output_test_stream.hpp>

#include <stdexcept>
#include <string>

#include "MassKernels.h"
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


// === FluxMassDolfynFaceLoop ===
// Test 1: Test it runs without error on a small mesh
BOOST_AUTO_TEST_CASE(FluxMassDolfynFaceLoop_test1)
{
	cupcfd::error::eCodes status;
    cupcfd::comm::Communicator comm(MPI_COMM_WORLD);

	// Create a small test mesh
    cupcfd::partitioner::PartitionerNaiveConfig<int,int> partConfig;

	// Setup the source config
    meshgeo::MeshSourceStructGenConfig<int, double> meshSourceConfig(5, 5, 5, -1.0, 1.0, -1.0, 1.0, -1.0, 1.0);

	// Setup the config to use for building
    meshgeo::MeshConfig<int, double,int> meshConfig(partConfig, meshSourceConfig);

    meshgeo::CupCfdAoSMesh<int,double,int> * mesh;
	status = meshConfig.buildUnstructuredMesh(&mesh, comm);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	// Setup
	int nCells = mesh->properties.lTCells;
	int nOwnedCells = mesh->properties.lOCells;
	int nGhostCells = mesh->properties.lGhCells;
	int nBnds = mesh->properties.lBoundaries;
	int nFaces = mesh->properties.lFaces;
	int nRegions = mesh->properties.lRegions;

	cupcfd::geometry::euclidean::EuclideanVector<double,3> * dudx =
			(cupcfd::geometry::euclidean::EuclideanVector<double,3> *) malloc(sizeof(cupcfd::geometry::euclidean::EuclideanVector<double,3>) * nCells);

	cupcfd::geometry::euclidean::EuclideanVector<double,3> * dvdx =
			(cupcfd::geometry::euclidean::EuclideanVector<double,3> *) malloc(sizeof(cupcfd::geometry::euclidean::EuclideanVector<double,3>) * nCells);

	cupcfd::geometry::euclidean::EuclideanVector<double,3> * dwdx =
			(cupcfd::geometry::euclidean::EuclideanVector<double,3> *) malloc(sizeof(cupcfd::geometry::euclidean::EuclideanVector<double,3>) * nCells);

	cupcfd::geometry::euclidean::EuclideanVector<double,3> * dpdx =
			(cupcfd::geometry::euclidean::EuclideanVector<double,3> *) malloc(sizeof(cupcfd::geometry::euclidean::EuclideanVector<double,3>) * nCells);

	double * denCell = (double *) malloc(sizeof(double) * nCells);
	double * denBoundary = (double *) malloc(sizeof(double) * nBnds);
	double * uCell = (double *) malloc(sizeof(double) * nCells);
	double * vCell = (double *) malloc(sizeof(double) * nCells);
	double * wCell = (double *) malloc(sizeof(double) * nCells);
	double * massFlux = (double *) malloc(sizeof(double) * nFaces);
	double * p = (double *) malloc(sizeof(double) * nCells);
	double * ar = (double *) malloc(sizeof(double) * nCells);
	double * su = (double *) malloc(sizeof(double) * nCells);
	double * rface = (double *) malloc(sizeof(double) * nFaces * 2);

	double small = 1E-18;
	int icinl;
	int icout;
	int icsym;
	int icwal;
	bool solveTurbEnergy = false;
	bool solveTurbDiss = false;
	bool solveVisc = true;
	bool solveEnthalpy = false;

	double * teCell = (double *) malloc(sizeof(double) * nCells);
	double * teBoundary = (double *) malloc(sizeof(double) * nBnds);
	double * edCell = (double *) malloc(sizeof(double) * nCells);
	double * edBoundary = (double *) malloc(sizeof(double) * nBnds);
	double * viseffCell = (double *) malloc(sizeof(double) * nCells);
	double * viseffBoundary = (double *) malloc(sizeof(double) * nBnds);
	double * tCell = (double *) malloc(sizeof(double) * nCells);
	double * tBoundary = (double *) malloc(sizeof(double) * nBnds);

	FluxMassDolfynFaceLoop(*mesh,
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
	delete(mesh);
}

BOOST_AUTO_TEST_CASE(cleanup)
{
    MPI_Finalize();
}
