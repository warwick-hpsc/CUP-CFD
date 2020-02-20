/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Tests for the concrete methods of the MeshSourceFileConfig class.
 */

#define BOOST_TEST_MODULE MeshSourceFileConfig

// Boost setup
#include <boost/test/unit_test.hpp>
#include <boost/test/output_test_stream.hpp>
#include <stdexcept>

#include "Error.h"
#include "MeshSourceFileConfig.h"


// Namespaces
namespace utf = boost::unit_test;

using namespace cupcfd::geometry::mesh;

// === Constructor ===

// Test 1: Test Constructor with Parameter Passing working correctly
BOOST_AUTO_TEST_CASE(constructor_test1, * utf::tolerance(0.00001))
{

}

// Test 2: Test Constructor with Referenced Config copies values correctly
BOOST_AUTO_TEST_CASE(constructor_test2, * utf::tolerance(0.00001))
{

}

// === set/getFileFormat ===
// Test 1: Test value can correctly be set and retrieved
BOOST_AUTO_TEST_CASE(set_getFileFormat_test1, * utf::tolerance(0.00001))
{

}

// === set/getSourceFilePath
BOOST_AUTO_TEST_CASE(set_getSourceFilePath_test1, * utf::tolerance(0.00001))
{

}

// === operator= ===
// Test 1: Test values are correctly copied from one config to another
BOOST_AUTO_TEST_CASE(operator_equals_test1, * utf::tolerance(0.00001))
{

}

// === clone ===
// Test 1: Test object is correctly cloned and distinct
BOOST_AUTO_TEST_CASE(clone_test1, * utf::tolerance(0.00001))
{

}

// === buildMeshSource ===
// Test 1: Test we can build a correct HDF5 format mesh source
BOOST_AUTO_TEST_CASE(buildMeshSource2_test1)
{
	cupcfd::error::eCodes status;
	MeshSource<int, double, int> * source;
	MeshSourceFileConfig<int, double> config(MESH_FILE_FORMAT_HDF5, "../tests/geometry/mesh/data/MeshHDF5.hdf5");

	status = config.buildMeshSource(&source);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	// Check a couple of value retrieved are correct
	int cells, faces, boundaries, vertices;

	status = source->getCellCount(&cells);
	BOOST_CHECK_EQUAL(cells, 4);

	status = source->getFaceCount(&faces);
	BOOST_CHECK_EQUAL(faces, 20);

	status = source->getBoundaryCount(&boundaries);
	BOOST_CHECK_EQUAL(boundaries, 17);

	status = source->getVertexCount(&vertices);
	BOOST_CHECK_EQUAL(vertices, 18);
}
