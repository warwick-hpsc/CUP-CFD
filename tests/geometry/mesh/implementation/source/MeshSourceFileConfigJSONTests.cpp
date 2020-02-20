/*
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Tests for the MeshSourceFileConfigJSON class
 */

#define BOOST_TEST_MODULE MeshSourceFileConfigJSON
#include <boost/test/unit_test.hpp>
#include <boost/test/output_test_stream.hpp>
#include <stdexcept>


#include "MeshSourceFileConfigJSON.h"

using namespace cupcfd::geometry::mesh;

// Namespaces
namespace utf = boost::unit_test;

// === Constructor ===
// Test 1: Successfully setup the object
BOOST_AUTO_TEST_CASE(constructor_test1)
{
	std::string topLevel[0] = {};
	MeshSourceFileConfigJSON<int, double> configFile("../tests/geometry/mesh/data/MeshSourceFileConfigHDF5.json", topLevel, 0);
}

// === getFileFormat ===
// Test 1:
BOOST_AUTO_TEST_CASE(getFileFormat_test1)
{
	cupcfd::error::eCodes status;
	MeshFileFormat fileFormat;

	std::string topLevel[0] = {};
	MeshSourceFileConfigJSON<int, double> configFile1("../tests/geometry/mesh/data/MeshSourceFileConfigHDF5.json", topLevel, 0);

	status = configFile1.getFileFormat(&fileFormat);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	BOOST_CHECK_EQUAL(fileFormat, MESH_FILE_FORMAT_HDF5);
}

// Test 2: Error Case - E_CONFIG_OPT_NOT_FOUND
BOOST_AUTO_TEST_CASE(getFileFormat_test2)
{

}

// Test 3: Error Case - E_CONFIG_INVALID_VALUE
BOOST_AUTO_TEST_CASE(getFileFormat_test3)
{

}

// Test 4: Error Case - E_CONFIG_INVALID_OPTION
BOOST_AUTO_TEST_CASE(getFileFormat_test4)
{

}

// === getFilePath ===
// Test 1:
BOOST_AUTO_TEST_CASE(getFilePath)
{
	cupcfd::error::eCodes status;
	std::string filePath;

	std::string topLevel[0] = {};
	MeshSourceFileConfigJSON<int, double> configFile1("../tests/geometry/mesh/data/MeshSourceFileConfigHDF5.json", topLevel, 0);

	status = configFile1.getFilePath(filePath);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	BOOST_CHECK_EQUAL(filePath, "../tests/geometry/mesh/data/MeshHDF5.hdf5");
}

// Test 2: Error Case - E_CONFIG_OPT_NOT_FOUND
BOOST_AUTO_TEST_CASE(readMeshSourceFileLocation_test2)
{

}

// Test 3: Error Case - E_CONFIG_INVALID_VALUE
BOOST_AUTO_TEST_CASE(readMeshSourceFileLocation_test3)
{

}

// Test 4: Error Case - E_CONFIG_INVALID_OPTION
BOOST_AUTO_TEST_CASE(readMeshSourceFileLocation_test4)
{

}

// === buildMeshSourceConfig ===
BOOST_AUTO_TEST_CASE(buildMeshSourceConfig)
{
	cupcfd::error::eCodes status;
	std::string filePath;

	std::string topLevel[0] = {};
	MeshSourceFileConfigJSON<int, double> configFile1("../tests/geometry/mesh/data/MeshSourceFileConfigHDF5.json", topLevel, 0);


	MeshSourceConfig<int,double,int> * config;
	status = configFile1.buildMeshSourceConfig(&config);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
}
