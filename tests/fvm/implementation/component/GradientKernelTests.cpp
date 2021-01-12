/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Tests for the MeshStructGenSource Class
 */

#define BOOST_TEST_MODULE GradientKernels
#include <boost/test/unit_test.hpp>
#include <boost/test/output_test_stream.hpp>

#include <stdexcept>
#include <string>

#include "GradientKernels.h"
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


// === GradientPhiGaussDolfyn ===
// Test 1: Test it runs without error on a small mesh
BOOST_AUTO_TEST_CASE(GradientPhiGaussDolfyn_test1)
{
	cupcfd::error::eCodes status;
    cupcfd::comm::Communicator comm(MPI_COMM_WORLD);

	// Create a small test mesh
    cupcfd::partitioner::PartitionerNaiveConfig<int,int> partConfig;

	// Setup the source config
    meshgeo::MeshSourceStructGenConfig<int, double> meshSourceConfig(5, 5, 5, -1.0, 1.0, -1.0, 1.0, -1.0, 1.0);

	// Setup the config to use for building
    meshgeo::MeshConfig<int,double,int> meshConfig(partConfig, meshSourceConfig);

    meshgeo::CupCfdAoSMesh<int,double,int> * mesh;
	status = meshConfig.buildUnstructuredMesh(&mesh, comm);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	// Setup
	double * phiCell = (double *) malloc(sizeof(double) * mesh->properties.lTCells);
	double * phiBoundary = (double *) malloc(sizeof(double) * mesh->properties.lTCells);
	euc::EuclideanVector<double,3> * dPhidxCell = (euc::EuclideanVector<double,3> *) malloc(sizeof(euc::EuclideanVector<double,3>) * mesh->properties.lTCells);
	euc::EuclideanVector<double,3> * dPhidxoCell  = (euc::EuclideanVector<double,3> *) malloc(sizeof(euc::EuclideanVector<double,3>) * mesh->properties.lTCells);

	// Won't bother populating the data arrays for now

	status = GradientPhiGaussDolfyn(*mesh, 1, phiCell, mesh->properties.lTCells,
			phiBoundary, mesh->properties.lTCells,
			dPhidxCell, mesh->properties.lTCells,
			dPhidxoCell, mesh->properties.lTCells);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	free(phiCell);
	free(phiBoundary);
	free(dPhidxCell);
	free(dPhidxoCell);
	delete(mesh);
}

BOOST_AUTO_TEST_CASE(cleanup)
{
    MPI_Finalize();
}
