/*
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Tests for the UnstructuredMeshProperties Class
 */

#define BOOST_TEST_MODULE UnstructuredMeshProperties
#include <boost/test/unit_test.hpp>
#include <boost/test/output_test_stream.hpp>
#include <stdexcept>

#include "UnstructuredMeshProperties.h"
#include "Error.h"

using namespace cupcfd::geometry::mesh;

// === Constructors ===
// Test 1: Test Default Constructor
BOOST_AUTO_TEST_CASE(constructor_test1)
{
	UnstructuredMeshProperties<int, double> prop;

	// ToDo: Direct access should be replaced with getters and setters
	BOOST_CHECK_EQUAL(prop.nCells, 0);
	BOOST_CHECK_EQUAL(prop.nFaces, 0);
	BOOST_CHECK_EQUAL(prop.nVertices, 0);
	BOOST_CHECK_EQUAL(prop.nBoundaries, 0);
	BOOST_CHECK_EQUAL(prop.nRegions, 0);
	BOOST_CHECK_EQUAL(prop.nMaxFaces, 0);
	BOOST_CHECK_EQUAL(prop.scaleFactor, 0.0);
	BOOST_CHECK_EQUAL(prop.lOCells, 0);
	BOOST_CHECK_EQUAL(prop.lGhCells, 0);
	BOOST_CHECK_EQUAL(prop.lTCells, 0);
	BOOST_CHECK_EQUAL(prop.lFaces, 0);
	BOOST_CHECK_EQUAL(prop.lVertices, 0);
	BOOST_CHECK_EQUAL(prop.lBoundaries, 0);
	BOOST_CHECK_EQUAL(prop.lRegions, 0);
}

// Test 2: Test Constructor with different values
BOOST_AUTO_TEST_CASE(constructor_test2)
{

}

// Test 3: Test Copy Constructor
BOOST_AUTO_TEST_CASE(constructor_test3)
{

}

// === reset ===
// Test 1: Test values are correctly reset to zero
BOOST_AUTO_TEST_CASE(reset_test1)
{

}

// === operator= ===
BOOST_AUTO_TEST_CASE(operator_copy_test1)
{

}

// === clone ===
BOOST_AUTO_TEST_CASE(clone_test1)
{

}
