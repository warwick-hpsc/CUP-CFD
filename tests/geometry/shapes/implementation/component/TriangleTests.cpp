/*
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Unit Tests for the methods of the Triangle class
 */

#define BOOST_TEST_MODULE Triangle
#include <boost/test/unit_test.hpp>
#include <boost/test/output_test_stream.hpp>
#include <stdexcept>

#include "Triangle.h"
#include "EuclideanPoint.h"

using namespace cupcfd::geometry::shapes;
namespace euc = cupcfd::geometry::euclidean;
namespace utf = boost::unit_test;

// === heronsFormula ===
// Test 1: Test the area is computed correctly - 2D
BOOST_AUTO_TEST_CASE(heronsFormula_test1, * utf::tolerance(0.00001))
{
	// Setup
	euc::EuclideanPoint<double,2> p1(3.0, 4.0);
	euc::EuclideanPoint<double,2> p2(3.15, 12.0);
	euc::EuclideanPoint<double,2> p3(3.3, 4.0);

	// Test and Check
	double area = Triangle<double,2>::heronsFormula(p1, p2, p3);
	BOOST_TEST(area == 1.2);
}

// Test 2: Test the area is computed correctly - 2D
BOOST_AUTO_TEST_CASE(heronsFormula_test2, * utf::tolerance(0.00001))
{
	// Setup
	euc::EuclideanPoint<double,2> p1(3.0, 4.0);
	euc::EuclideanPoint<double,2> p2(6.0, 4.0);
	euc::EuclideanPoint<double,2> p3(3.0, 12.0);

	// Test and Check
	double area = Triangle<double,2>::heronsFormula(p1, p2, p3);
	BOOST_TEST(area == 12.0);
}

// Test 3: Test the area is computed correctly - 3D
BOOST_AUTO_TEST_CASE(areaHeronsFormula_test3,  * utf::tolerance(0.00001))
{
	// Setup
	euc::EuclideanPoint<double,3> p1(3.0, 4.0, 8.0);
	euc::EuclideanPoint<double,3> p2(3.15, 12.0, 14.0);
	euc::EuclideanPoint<double,3> p3(3.3, 4.0, 9.0);

	// Test and Check
	double area = Triangle<double,3>::heronsFormula(p1, p2, p3);
	BOOST_TEST(area == 4.25683);
}

// Test 4: Test the area is computed correctly - 3D
BOOST_AUTO_TEST_CASE(areaHeronsFormula_test4,  * utf::tolerance(0.00001))
{
	// Setup
	euc::EuclideanPoint<double,3> p1(3.0, 4.0, 8.9);
	euc::EuclideanPoint<double,3> p2(6.0, 4.0, 7.6);
	euc::EuclideanPoint<double,3> p3(3.0, 12.0, 15.4);

	// Test and Check
	double area = Triangle<double,3>::heronsFormula(p1, p2, p3);
	BOOST_TEST(area == 16.3126);
}
