/*
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Unit Tests for the concrete methods of the CupCfdAoSMeshFace class
 */

#define BOOST_TEST_MODULE CupCfdAoSMeshFace
#include <boost/test/unit_test.hpp>
#include <boost/test/output_test_stream.hpp>
#include <stdexcept>

#include "CupCfdAoSMeshFace.h"
#include "Error.h"

using namespace cupcfd::geometry::mesh;

// === Default Constructor ===
// Test 1: Test that the default values are as expected
BOOST_AUTO_TEST_CASE(constructor_test1)
{
	CupCfdAoSMeshFace<int, double> face;

	BOOST_CHECK_EQUAL(face.bndID, -1);
	BOOST_CHECK_EQUAL(face.cell1ID, -1);
	BOOST_CHECK_EQUAL(face.cell2ID, -1);
	BOOST_CHECK_EQUAL(face.verticesID[0], -1);
	BOOST_CHECK_EQUAL(face.verticesID[1], -1);
	BOOST_CHECK_EQUAL(face.verticesID[2], -1);
	BOOST_CHECK_EQUAL(face.verticesID[3], -1);
	BOOST_CHECK_EQUAL(face.lambda, 0.0);
	BOOST_CHECK_EQUAL(face.rlencos, 0.0);
	BOOST_CHECK_EQUAL(face.area, 0.0);
	BOOST_CHECK_EQUAL(face.center.cmp[0], 0.0);
	BOOST_CHECK_EQUAL(face.center.cmp[1], 0.0);
	BOOST_CHECK_EQUAL(face.center.cmp[2], 0.0);
	BOOST_CHECK_EQUAL(face.norm.cmp[0], 0.0);
	BOOST_CHECK_EQUAL(face.norm.cmp[1], 0.0);
	BOOST_CHECK_EQUAL(face.norm.cmp[2], 0.0);
	BOOST_CHECK_EQUAL(face.xpac.cmp[0], 0.0);
	BOOST_CHECK_EQUAL(face.xpac.cmp[1], 0.0);
	BOOST_CHECK_EQUAL(face.xpac.cmp[2], 0.0);
	BOOST_CHECK_EQUAL(face.xnac.cmp[0], 0.0);
	BOOST_CHECK_EQUAL(face.xnac.cmp[1], 0.0);
	BOOST_CHECK_EQUAL(face.xnac.cmp[2], 0.0);
}
