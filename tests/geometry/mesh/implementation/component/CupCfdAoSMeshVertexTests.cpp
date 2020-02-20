/*
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Unit Tests for the concrete methods of the CupCfdAoSMeshVertex class
 */

#define BOOST_TEST_MODULE CupCfdAoSMeshVertex
#include <boost/test/unit_test.hpp>
#include <boost/test/output_test_stream.hpp>
#include <stdexcept>

#include "CupCfdAoSMeshVertex.h"
#include "Error.h"

using namespace cupcfd::geometry::mesh;

// === Default Constructor ===
// Test 1: Test that the default values are as expected
BOOST_AUTO_TEST_CASE(constructor_test1)
{
	CupCfdAoSMeshVertex<double> vertex;

	BOOST_CHECK_EQUAL(vertex.pos.cmp[0], 0.0);
	BOOST_CHECK_EQUAL(vertex.pos.cmp[1], 0.0);
	BOOST_CHECK_EQUAL(vertex.pos.cmp[2], 0.0);
}
