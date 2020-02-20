/*
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Unit Tests for the concrete methods of the CupCfdAoSMeshCell class
 */

#define BOOST_TEST_MODULE CupCfdAoSMeshCell
#include <boost/test/unit_test.hpp>
#include <boost/test/output_test_stream.hpp>
#include <stdexcept>

#include "CupCfdAoSMeshCell.h"
#include "Error.h"

using namespace cupcfd::geometry::mesh;

// === Default Constructor ===
// Test 1: Test that the default values are as expected
BOOST_AUTO_TEST_CASE(constructor_test1)
{
	CupCfdAoSMeshCell<int, double> cell;

	BOOST_CHECK_EQUAL(cell.vol, 0.0);

	BOOST_CHECK_EQUAL(cell.center.cmp[0], 0.0);
	BOOST_CHECK_EQUAL(cell.center.cmp[1], 0.0);
	BOOST_CHECK_EQUAL(cell.center.cmp[2], 0.0);
}
