/*
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Unit Tests for the concrete methods of the CupCfdAoSMeshRegion class
 */

#define BOOST_TEST_MODULE CupCfdAoSMeshRegion
#include <boost/test/unit_test.hpp>
#include <boost/test/output_test_stream.hpp>
#include <stdexcept>

#include "CupCfdAoSMeshRegion.h"
#include "Error.h"

using namespace cupcfd::geometry::mesh;

// === Default Constructor ===
// Test 1: Test that the default values are as expected
BOOST_AUTO_TEST_CASE(constructor_test1)
{
	CupCfdAoSMeshRegion<int, double> region;

	BOOST_CHECK_EQUAL(region.rType, RTYPE_DEFAULT);
	BOOST_CHECK_EQUAL(region.std, false);
	BOOST_CHECK_EQUAL(region.flux, false);
	BOOST_CHECK_EQUAL(region.adiab, false);
	BOOST_CHECK_EQUAL(region.ylog, 0.0);
	BOOST_CHECK_EQUAL(region.elog, 0.0);
	BOOST_CHECK_EQUAL(region.density, 0.0);
	BOOST_CHECK_EQUAL(region.turbKE, 0.0);
	BOOST_CHECK_EQUAL(region.turbDiss, 0.0);
	BOOST_CHECK_EQUAL(region.splvl, 0.0);
	BOOST_CHECK_EQUAL(region.den, 0.0);
	BOOST_CHECK_EQUAL(region.r, 0.0);
	BOOST_CHECK_EQUAL(region.t, 0.0);
	BOOST_CHECK_EQUAL(region.forceTangent.cmp[0], 0.0);
	BOOST_CHECK_EQUAL(region.forceTangent.cmp[1], 0.0);
	BOOST_CHECK_EQUAL(region.forceTangent.cmp[2], 0.0);
	BOOST_CHECK_EQUAL(region.uvw.cmp[0], 0.0);
	BOOST_CHECK_EQUAL(region.uvw.cmp[1], 0.0);
	BOOST_CHECK_EQUAL(region.uvw.cmp[2], 0.0);
	BOOST_CHECK_EQUAL(region.regionName,"Default Region");
}
