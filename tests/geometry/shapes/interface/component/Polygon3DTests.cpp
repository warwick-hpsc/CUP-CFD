/*
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Unit Tests for the methods of the Polygon3D class
 */


#define BOOST_TEST_MODULE Polyhedron
#include <boost/test/unit_test.hpp>
#include <boost/test/output_test_stream.hpp>
#include <stdexcept>

#include "Polygon3D.h"

using namespace cupcfd::geometry::shapes;
namespace euc = cupcfd::geometry::euclidean;
namespace utf = boost::unit_test;

// Note we only test static methods in this tester since many methods just passthrough
// to an inheriting class (CRTP)

// === isVertexOrderClockwise ===
// Test 1: AntiClockwise from Origin
BOOST_AUTO_TEST_CASE(isVertexOrderClockwise_test1, * utf::tolerance(0.00001))
{
	// Setup
	euc::EuclideanPoint<double,3> observation(0.0, 0.0, 0.0);

	euc::EuclideanPoint<double,3> points[3];
	points[0] = euc::EuclideanPoint<double,3>(3.0, 4.0, 8.0);
	points[1] = euc::EuclideanPoint<double,3>(3.15, 6.0, 8.0);
	points[2] = euc::EuclideanPoint<double,3>(3.3, 4.0, 8.0);

	// Test and Check
	bool dir = isVertexOrderClockwise(observation, points, 3);
	BOOST_CHECK_EQUAL(dir, false);
}

// Test 2: Clockwise from origin
BOOST_AUTO_TEST_CASE(isVertexOrderClockwise_test2, * utf::tolerance(0.00001))
{
	// Setup
	euc::EuclideanPoint<double,3> observation(0.0, 0.0, 0.0);

	euc::EuclideanPoint<double,3> points[3];
	points[0] = euc::EuclideanPoint<double,3>(3.0, 4.0, 8.0);
	points[1] = euc::EuclideanPoint<double,3>(3.3, 4.0, 8.0);
	points[2] = euc::EuclideanPoint<double,3>(3.15, 6.0, 8.0);

	// Test and Check
	bool dir = isVertexOrderClockwise(observation, points, 3);
	BOOST_CHECK_EQUAL(dir, true);
}
