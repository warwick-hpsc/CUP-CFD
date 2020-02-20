/*
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Tests for the MeshSourceStructGenConfigJSON class
 */

#define BOOST_TEST_MODULE MeshSourceStructGenConfigJSON
#include <boost/test/unit_test.hpp>
#include <boost/test/output_test_stream.hpp>
#include <stdexcept>


#include "MeshSourceStructGenConfigJSON.h"

using namespace cupcfd::geometry::mesh;

// Namespaces
namespace utf = boost::unit_test;

// === Constructor ===
// Test 1: Successfully setup the object
BOOST_AUTO_TEST_CASE(constructor_test1)
{
	std::string topLevel[0] = {};
	MeshSourceStructGenConfigJSON<int, double> configFile("../tests/geometry/mesh/data/MeshSourceStructGenConfig.json", topLevel, 0);
}

// === getMeshCellSizeX ===
// Test 1:
BOOST_AUTO_TEST_CASE(getMeshCellSizeX_test1)
{
	cupcfd::error::eCodes status;
	int cellX;

	std::string topLevel[0] = {};
	MeshSourceStructGenConfigJSON<int, double> configFile("../tests/geometry/mesh/data/MeshSourceStructGenConfig.json", topLevel, 0);
	status = configFile.getMeshCellSizeX(&cellX);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	BOOST_CHECK_EQUAL(cellX, 127);
}

// Test 2: Error Case - E_CONFIG_OPT_NOT_FOUND
BOOST_AUTO_TEST_CASE(getMeshCellSizeX_test2)
{

}

// Test 3: Error Case - E_CONFIG_INVALID_VALUE
BOOST_AUTO_TEST_CASE(getMeshCellSizeX_test3)
{

}

// Test 4: Error Case - E_CONFIG_INVALID_OPTION
BOOST_AUTO_TEST_CASE(getMeshCellSizeX_test4)
{

}

// === getMeshCellSizeY ===
// Test 1:
BOOST_AUTO_TEST_CASE(getMeshCellSizeY_test1)
{
	cupcfd::error::eCodes status;
	int cellY;

	std::string topLevel[0] = {};
	MeshSourceStructGenConfigJSON<int, double> configFile("../tests/geometry/mesh/data/MeshSourceStructGenConfig.json", topLevel, 0);
	status = configFile.getMeshCellSizeY(&cellY);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	BOOST_CHECK_EQUAL(cellY, 301);
}

// Test 2: Error Case - E_CONFIG_OPT_NOT_FOUND
BOOST_AUTO_TEST_CASE(getMeshCellSizeY_test2)
{

}

// Test 3: Error Case - E_CONFIG_INVALID_VALUE
BOOST_AUTO_TEST_CASE(getMeshCellSizeY_test3)
{

}

// Test 4: Error Case - E_CONFIG_INVALID_OPTION
BOOST_AUTO_TEST_CASE(getMeshCellSizeY_test4)
{

}

// === getMeshCellSizeZ ===
// Test 1:
BOOST_AUTO_TEST_CASE(getMeshCellSizeZ_test1)
{
	cupcfd::error::eCodes status;
	int cellZ;

	std::string topLevel[0] = {};
	MeshSourceStructGenConfigJSON<int, double> configFile("../tests/geometry/mesh/data/MeshSourceStructGenConfig.json", topLevel, 0);
	status = configFile.getMeshCellSizeZ(&cellZ);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	BOOST_CHECK_EQUAL(cellZ, 54);
}

// Test 2: Error Case - E_CONFIG_OPT_NOT_FOUND
BOOST_AUTO_TEST_CASE(getMeshCellSizeZ_test2)
{

}

// Test 3: Error Case - E_CONFIG_INVALID_VALUE
BOOST_AUTO_TEST_CASE(getMeshCellSizeZ_test3)
{

}

// Test 4: Error Case - E_CONFIG_INVALID_OPTION
BOOST_AUTO_TEST_CASE(getMeshCellSizeZ_test4)
{

}

// === getMeshSpatialXMin ===
// Test 1:
BOOST_AUTO_TEST_CASE(getMeshSpatialXMin_test1)
{
	cupcfd::error::eCodes status;
	double sXMin;

	std::string topLevel[0] = {};
	MeshSourceStructGenConfigJSON<int, double> configFile("../tests/geometry/mesh/data/MeshSourceStructGenConfig.json", topLevel, 0);
	status = configFile.getMeshSpatialXMin(&sXMin);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	BOOST_CHECK_EQUAL(sXMin, -1.5);
}

// Test 2: Error Case - E_CONFIG_OPT_NOT_FOUND
BOOST_AUTO_TEST_CASE(getMeshSpatialXMin_test2)
{

}

// Test 3: Error Case - E_CONFIG_INVALID_VALUE
BOOST_AUTO_TEST_CASE(getMeshSpatialXMin_test3)
{

}

// Test 4: Error Case - E_CONFIG_INVALID_OPTION
BOOST_AUTO_TEST_CASE(getMeshSpatialXMin_test4)
{

}

// === getMeshSpatialYMin ===
// Test 1:
BOOST_AUTO_TEST_CASE(getMeshSpatialYMin_test1)
{
	cupcfd::error::eCodes status;
	double sYMin;

	std::string topLevel[0] = {};
	MeshSourceStructGenConfigJSON<int, double> configFile("../tests/geometry/mesh/data/MeshSourceStructGenConfig.json", topLevel, 0);
	status = configFile.getMeshSpatialYMin(&sYMin);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	BOOST_CHECK_EQUAL(sYMin, -1.2);
}

// Test 2: Error Case - E_CONFIG_OPT_NOT_FOUND
BOOST_AUTO_TEST_CASE(getMeshSpatialYMin_test2)
{

}

// Test 3: Error Case - E_CONFIG_INVALID_VALUE
BOOST_AUTO_TEST_CASE(getMeshSpatialYMin_test3)
{

}

// Test 4: Error Case - E_CONFIG_INVALID_OPTION
BOOST_AUTO_TEST_CASE(getMeshSpatialYMin_test4)
{

}

// === getMeshSpatialZMin ===
// Test 1:
BOOST_AUTO_TEST_CASE(getMeshSpatialZMin_test1)
{
	cupcfd::error::eCodes status;
	double sZMin;

	std::string topLevel[0] = {};
	MeshSourceStructGenConfigJSON<int, double> configFile("../tests/geometry/mesh/data/MeshSourceStructGenConfig.json", topLevel, 0);
	status = configFile.getMeshSpatialZMin(&sZMin);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	BOOST_CHECK_EQUAL(sZMin, -2.7);
}

// Test 2: Error Case - E_CONFIG_OPT_NOT_FOUND
BOOST_AUTO_TEST_CASE(getMeshSpatialZMin_test2)
{

}

// Test 3: Error Case - E_CONFIG_INVALID_VALUE
BOOST_AUTO_TEST_CASE(getMeshSpatialZMin_test3)
{

}

// Test 4: Error Case - E_CONFIG_INVALID_OPTION
BOOST_AUTO_TEST_CASE(getMeshSpatialZMin_test4)
{

}

// === getMeshSpatialXMax ===
// Test 1:
BOOST_AUTO_TEST_CASE(getMeshSpatialXMax_test1)
{
	cupcfd::error::eCodes status;
	double sXMax;

	std::string topLevel[0] = {};
	MeshSourceStructGenConfigJSON<int, double> configFile("../tests/geometry/mesh/data/MeshSourceStructGenConfig.json", topLevel, 0);
	status = configFile.getMeshSpatialXMax(&sXMax);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	BOOST_CHECK_EQUAL(sXMax, 3.4);
}

// Test 2: Error Case - E_CONFIG_OPT_NOT_FOUND
BOOST_AUTO_TEST_CASE(getMeshSpatialXMax_test2)
{

}

// Test 3: Error Case - E_CONFIG_INVALID_VALUE
BOOST_AUTO_TEST_CASE(getMeshSpatialXMax_test3)
{

}

// Test 4: Error Case - E_CONFIG_INVALID_OPTION
BOOST_AUTO_TEST_CASE(getMeshSpatialXMax_test4)
{

}

// === getMeshSpatialYMax ===
// Test 1:
BOOST_AUTO_TEST_CASE(getMeshSpatialYMax_test1)
{
	cupcfd::error::eCodes status;
	double sYMax;

	std::string topLevel[0] = {};
	MeshSourceStructGenConfigJSON<int, double> configFile("../tests/geometry/mesh/data/MeshSourceStructGenConfig.json", topLevel, 0);
	status = configFile.getMeshSpatialYMax(&sYMax);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	BOOST_CHECK_EQUAL(sYMax, 5.6);
}

// Test 2: Error Case - E_CONFIG_OPT_NOT_FOUND
BOOST_AUTO_TEST_CASE(getMeshSpatialYMax_test2)
{

}

// Test 3: Error Case - E_CONFIG_INVALID_VALUE
BOOST_AUTO_TEST_CASE(getMeshSpatialYMax_test3)
{

}

// Test 4: Error Case - E_CONFIG_INVALID_OPTION
BOOST_AUTO_TEST_CASE(getMeshSpatialYMax_test4)
{

}

// === getMeshSpatialZMax ===
// Test 1:
BOOST_AUTO_TEST_CASE(getMeshSpatialZMax_test1)
{
	cupcfd::error::eCodes status;
	double sZMax;

	std::string topLevel[0] = {};
	MeshSourceStructGenConfigJSON<int, double> configFile("../tests/geometry/mesh/data/MeshSourceStructGenConfig.json", topLevel, 0);
	status = configFile.getMeshSpatialZMax(&sZMax);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	BOOST_CHECK_EQUAL(sZMax, 7.9);
}

// Test 2: Error Case - E_CONFIG_OPT_NOT_FOUND
BOOST_AUTO_TEST_CASE(getMeshSpatialZMax_test2)
{

}

// Test 3: Error Case - E_CONFIG_INVALID_VALUE
BOOST_AUTO_TEST_CASE(getMeshSpatialZMax_test3)
{

}

// Test 4: Error Case - E_CONFIG_INVALID_OPTION
BOOST_AUTO_TEST_CASE(getMeshSpatialZMax_test4)
{

}

// === buildMeshSourceConfig ===
BOOST_AUTO_TEST_CASE(buildMeshSourceConfig)
{
	cupcfd::error::eCodes status;
	std::string filePath;

	std::string topLevel[0] = {};
	MeshSourceStructGenConfigJSON<int, double> configFile1("../tests/geometry/mesh/data/MeshSourceStructGenConfig.json", topLevel, 0);

	MeshSourceConfig<int,double,int> * config;
	status = configFile1.buildMeshSourceConfig(&config);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
}
