/*
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Unit Tests for the BenchmarkKernels class
 */

#define BOOST_TEST_MODULE BenchmarkKernels
#include <boost/test/unit_test.hpp>
#include <boost/test/output_test_stream.hpp>
#include <stdexcept>

#include "BenchmarkKernels.h"
#include "Communicator.h"
#include "Error.h"
#include "tt_interface_c.h"
#include "PartitionerNaiveConfig.h"
#include "PartitionerConfig.h"
#include "MeshSourceStructGenConfig.h"
#include "MeshConfig.h"
#include "CupCfdAoSMesh.h"

#include <iostream>

using namespace cupcfd::benchmark;

// These tests require MPI
BOOST_AUTO_TEST_CASE(setup)
{
    int argc = boost::unit_test::framework::master_test_suite().argc;
    char ** argv = boost::unit_test::framework::master_test_suite().argv;
    MPI_Init(&argc, &argv);
	TreeTimerInit();
}

// === Constructor ===
// Test 1:
BOOST_AUTO_TEST_CASE(constructor_test1)
{
	cupcfd::comm::Communicator comm(MPI_COMM_WORLD);

	// Setup a Mesh
    cupcfd::partitioner::PartitionerNaiveConfig<int,int> partConfig;
	cupcfd::geometry::mesh::MeshSourceStructGenConfig<int,double> meshSourceConfig(10, 20, 20, -1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
	cupcfd::geometry::mesh::MeshConfig<int,double,int> meshConfig(partConfig, meshSourceConfig);
	cupcfd::error::eCodes status;

	cupcfd::geometry::mesh::CupCfdAoSMesh<int,double,int> * meshPtr;
	status = meshConfig.buildUnstructuredMesh(&meshPtr, comm);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS); 
	std::shared_ptr<cupcfd::geometry::mesh::CupCfdAoSMesh<int,double,int>> sharedPtr(meshPtr);

	BenchmarkKernels<cupcfd::geometry::mesh::CupCfdAoSMesh<int,double,int>, int, double, int> benchmark("KernelBench",sharedPtr, 1000);
}

// === runBenchmark ===
// Test 1: Test running all of the kernel benchmarks via runBenchmark function
BOOST_AUTO_TEST_CASE(runBenchmark_test1)
{
	cupcfd::comm::Communicator comm(MPI_COMM_WORLD);

	// Setup a Mesh
    cupcfd::partitioner::PartitionerNaiveConfig<int,int> partConfig;
	cupcfd::geometry::mesh::MeshSourceStructGenConfig<int,double> meshSourceConfig(10, 20, 21, -1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
	cupcfd::geometry::mesh::MeshConfig<int,double,int> meshConfig(partConfig, meshSourceConfig);
	cupcfd::error::eCodes status;

	cupcfd::geometry::mesh::CupCfdAoSMesh<int,double,int> * meshPtr;
	status = meshConfig.buildUnstructuredMesh(&meshPtr, comm);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	std::shared_ptr<cupcfd::geometry::mesh::CupCfdAoSMesh<int,double,int>> sharedPtr(meshPtr);

	BenchmarkKernels<cupcfd::geometry::mesh::CupCfdAoSMesh<int,double,int>, int, double, int> benchmark("KernelBench",sharedPtr, 1000);
	benchmark.runBenchmark();
}


// Finalize MPI
BOOST_AUTO_TEST_CASE(cleanup)
{
    // Cleanup MPI Environment
	TreeTimerFinalize();
    MPI_Finalize();
}
