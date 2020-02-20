/*
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Unit Tests for the concrete methods of the CupCfdAoSMeshBoundary class
 */

#define BOOST_TEST_MODULE CupCfdAoSMeshBoundary
#include <boost/test/unit_test.hpp>
#include <boost/test/output_test_stream.hpp>
#include <stdexcept>

#include "CupCfdAoSMeshBoundary.h"
#include "Error.h"

using namespace cupcfd::geometry::mesh;

// === Default Constructor ===
// Test 1: Test that the default values are as expected
BOOST_AUTO_TEST_CASE(constructor_test1)
{
	CupCfdAoSMeshBoundary<int, double> boundary;

	BOOST_CHECK_EQUAL(boundary.faceID, -1);
	BOOST_CHECK_EQUAL(boundary.verticesID[0], -1);
	BOOST_CHECK_EQUAL(boundary.verticesID[1], -1);
	BOOST_CHECK_EQUAL(boundary.verticesID[2], -1);
	BOOST_CHECK_EQUAL(boundary.verticesID[3], -1);
	BOOST_CHECK_EQUAL(boundary.regionID, -1);
	BOOST_CHECK_EQUAL(boundary.distance, 0.0);
	BOOST_CHECK_EQUAL(boundary.yplus, 0.0);
	BOOST_CHECK_EQUAL(boundary.uplus, 0.0);
	BOOST_CHECK_EQUAL(boundary.shear.cmp[0], 0.0);
	BOOST_CHECK_EQUAL(boundary.shear.cmp[1], 0.0);
	BOOST_CHECK_EQUAL(boundary.shear.cmp[2], 0.0);
	BOOST_CHECK_EQUAL(boundary.q, 0.0);
	BOOST_CHECK_EQUAL(boundary.h, 0.0);
	BOOST_CHECK_EQUAL(boundary.t, 0.0);
}
