/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Tests for the concrete methods of the MeshStructGenSourceConfig class.
 */

#define BOOST_TEST_MODULE MeshStructGenSourceConfig

// Boost setup
#include <boost/test/unit_test.hpp>
#include <boost/test/output_test_stream.hpp>
#include <stdexcept>

#include "Error.h"
#include "MeshSourceStructGenConfig.h"


// Namespaces
namespace utf = boost::unit_test;

using namespace cupcfd::geometry::mesh;

// === Constructor ===

// Test 1: Test Constructor that passes parameters
BOOST_AUTO_TEST_CASE(constructor_test1, * utf::tolerance(0.00001))
{

}

// Test 2: Test Constructor with Referenced Config copies values correctly
BOOST_AUTO_TEST_CASE(constructor_test2, * utf::tolerance(0.00001))
{

}

// === set/getCellX ===
// Test 1: Test value can correctly be set and retrieved
BOOST_AUTO_TEST_CASE(getCellX_test1, * utf::tolerance(0.00001))
{

}

// === set/getCellY ===
// Test 1: Test value can correctly be set and retrieved
BOOST_AUTO_TEST_CASE(getCellY_test1, * utf::tolerance(0.00001))
{

}

// === set/getCellZ ===
// Test 1: Test value can correctly be set and retrieved
BOOST_AUTO_TEST_CASE(getCellZ_test1, * utf::tolerance(0.00001))
{

}

// === set/getSpatialMinX ===
// Test 1: Test value can correctly be set and retrieved
BOOST_AUTO_TEST_CASE(getSpatialMinX_test1, * utf::tolerance(0.00001))
{

}

// === set/getSpatialMinY ===
// Test 1: Test value can correctly be set and retrieved
BOOST_AUTO_TEST_CASE(getSpatialMinY_test1, * utf::tolerance(0.00001))
{

}

// === set/getSpatialMinZ ===
// Test 1: Test value can correctly be set and retrieved
BOOST_AUTO_TEST_CASE(getSpatialMinZ_test1, * utf::tolerance(0.00001))
{

}

// === set/getSpatialMaxX ===
// Test 1: Test value can correctly be set and retrieved
BOOST_AUTO_TEST_CASE(getSpatialMaxX_test1, * utf::tolerance(0.00001))
{

}

// === set/getSpatialMaxY ===
// Test 1: Test value can correctly be set and retrieved
BOOST_AUTO_TEST_CASE(getSpatialMaxY_test1, * utf::tolerance(0.00001))
{

}

// === set/getSpatialMaxZ ===
// Test 1: Test value can correctly be set and retrieved
BOOST_AUTO_TEST_CASE(getSpatialMaxZ_test1, * utf::tolerance(0.00001))
{

}

// === operator= ===
// Test 1: Test values are correctly copied from one config to another
BOOST_AUTO_TEST_CASE(operator_equals_test1, * utf::tolerance(0.00001))
{

}

// === clone ===
// Test 1: Test values are correctly copied from one config to another
// and the objects are distinct pointers
BOOST_AUTO_TEST_CASE(clone_test1, * utf::tolerance(0.00001))
{

}

// === buildMeshSource ===
// Test 1: Build a suitable mesh source object
BOOST_AUTO_TEST_CASE(buildMeshSource_test1, * utf::tolerance(0.00001))
{

}
