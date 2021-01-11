/*
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Tests for the MeshConfigSourceJSON class
 */

#define BOOST_TEST_MODULE MeshConfigSourceJSON
#include <boost/test/unit_test.hpp>
#include <boost/test/output_test_stream.hpp>
#include <stdexcept>
#include "MeshConfigSourceJSON.h"
#include "CupCfdAoSMesh.h"

using namespace cupcfd::geometry::mesh;

// Namespaces
namespace utf = boost::unit_test;

// Setup
BOOST_AUTO_TEST_CASE(setup)
{
    int argc = boost::unit_test::framework::master_test_suite().argc;
    char ** argv = boost::unit_test::framework::master_test_suite().argv;

    MPI_Init(&argc, &argv);
}


// === Constructor ===
// Test 1: Successfully setup the object
BOOST_AUTO_TEST_CASE(constructor_test1)
{
	std::string topLevel[0] = {};
	MeshConfigSourceJSON<int, double, int> configFile("../tests/geometry/mesh/data/MeshConfig.json", topLevel, 0);
}

// === getMeshSourceConfig ===
// Test 1:
BOOST_AUTO_TEST_CASE(getMeshSourceConfig_test1)
{
	std::string topLevel[0] = {};
	MeshConfigSourceJSON<int, double, int> configFile("../tests/geometry/mesh/data/MeshConfig.json", topLevel, 0);

	cupcfd::error::eCodes status;

	MeshSourceConfig<int,double,int> * meshSourceConfig;
	configFile.getMeshSourceConfig(&meshSourceConfig);
	if(meshSourceConfig == nullptr) {
		// Don't want nullptr's
		BOOST_CHECK_EQUAL(1,0);
	}
}

// Test 2: Error Case - E_CONFIG_OPT_NOT_FOUND
BOOST_AUTO_TEST_CASE(getMeshSourceConfig_test2)
{

}

// Test 3: Error Case - E_CONFIG_INVALID_VALUE
BOOST_AUTO_TEST_CASE(getMeshSourceConfig_test3)
{

}

// Test 4: Error Case - E_CONFIG_INVALID_OPTION
BOOST_AUTO_TEST_CASE(getMeshSourceConfig_test4)
{

}

// === getPartitionerConfig ===
// Test 1: Successful retrieval of a partitioner config
BOOST_AUTO_TEST_CASE(getPartitionerConfig_test1)
{
	std::string topLevel[0] = {};
	MeshConfigSourceJSON<int, double, int> configFile("../tests/geometry/mesh/data/MeshConfig.json", topLevel, 0);

	cupcfd::error::eCodes status;
	cupcfd::partitioner::PartitionerConfig<int,int> * partConfig;

	status = configFile.getPartitionerConfig(&partConfig);

	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
}

// Test 2: Error Case - E_CONFIG_OPT_NOT_FOUND
BOOST_AUTO_TEST_CASE(getPartitionerConfig_test2)
{

}

// Test 3: Error Case - E_CONFIG_INVALID_VALUE
BOOST_AUTO_TEST_CASE(getPartitionerConfig_test3)
{

}

// Test 4: Error Case - E_CONFIG_INVALID_OPTION
BOOST_AUTO_TEST_CASE(getPartitionerConfig_test4)
{

}

// === buildMeshConfig ===
// Test 1:
BOOST_AUTO_TEST_CASE(buildMeshConfig_test1)
{
	cupcfd::comm::Communicator comm(MPI_COMM_WORLD);

	std::string topLevel[0] = {};
	MeshConfigSourceJSON<int, double, int> configFile("../tests/geometry/mesh/data/MeshConfig.json", topLevel, 0);

	cupcfd::error::eCodes status;
	MeshConfig<int,double,int> * meshConfig;

	status = configFile.buildMeshConfig(&meshConfig);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	// See if we can go so far as to build a mesh
	CupCfdAoSMesh<int, double, int> * mesh;

	status = meshConfig->buildUnstructuredMesh(&mesh, comm);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
}

BOOST_AUTO_TEST_CASE(cleanup)
{
    MPI_Finalize();
}
