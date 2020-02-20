/*
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Unit Tests for the concrete methods of the Triangle2D class
 */

#define BOOST_TEST_MODULE Triangle2D
#include <boost/test/unit_test.hpp>
#include <boost/test/output_test_stream.hpp>
#include <stdexcept>

#include "Triangle2D.h"
#include "EuclideanPoint.h"

using namespace cupcfd::geometry::shapes;
namespace euc = cupcfd::geometry::euclidean;
namespace utf = boost::unit_test;

// === Constructor ===
// Test 1: Constructor: 3 Defined Points - 2D
BOOST_AUTO_TEST_CASE(constructor_test1, * utf::tolerance(0.00001))
{
	euc::EuclideanPoint<double,2> p1(3.0, 4.0);
	euc::EuclideanPoint<double,2> p2(4.0, 12.0);
	euc::EuclideanPoint<double,2> p3(3.3, 15.0);

	Triangle2D<double> shape(p1, p2, p3);

	BOOST_CHECK_EQUAL(shape.nVertices, 3);
	BOOST_CHECK_EQUAL(shape.nEdges, 3);
	BOOST_CHECK_EQUAL(shape.vertices[0].cmp[0], p1.cmp[0]);
	BOOST_CHECK_EQUAL(shape.vertices[0].cmp[1], p1.cmp[1]);
	BOOST_CHECK_EQUAL(shape.vertices[1].cmp[0], p2.cmp[0]);
	BOOST_CHECK_EQUAL(shape.vertices[1].cmp[1], p2.cmp[1]);
	BOOST_CHECK_EQUAL(shape.vertices[2].cmp[0], p3.cmp[0]);
	BOOST_CHECK_EQUAL(shape.vertices[2].cmp[1], p3.cmp[1]);
}

// Test 2: Copy Constructor
BOOST_AUTO_TEST_CASE(constructor_test2, * utf::tolerance(0.00001))
{
	euc::EuclideanPoint<double,2> p1(3.0, 4.0);
	euc::EuclideanPoint<double,2> p2(4.0, 12.0);
	euc::EuclideanPoint<double,2> p3(3.3, 15.0);

	Triangle2D<double> shape(p1, p2, p3);
	Triangle2D<double> shape2(shape);

	BOOST_CHECK_EQUAL(shape2.nVertices, 3);
	BOOST_CHECK_EQUAL(shape2.nEdges, 3);
	BOOST_CHECK_EQUAL(shape2.vertices[0].cmp[0], p1.cmp[0]);
	BOOST_CHECK_EQUAL(shape2.vertices[0].cmp[1], p1.cmp[1]);
	BOOST_CHECK_EQUAL(shape2.vertices[1].cmp[0], p2.cmp[0]);
	BOOST_CHECK_EQUAL(shape2.vertices[1].cmp[1], p2.cmp[1]);
	BOOST_CHECK_EQUAL(shape2.vertices[2].cmp[0], p3.cmp[0]);
	BOOST_CHECK_EQUAL(shape2.vertices[2].cmp[1], p3.cmp[1]);
}

// === isPointInsideBarycentric (static, 2D) ===
BOOST_AUTO_TEST_CASE(isPointInsideBarycentric_static_test1, * utf::tolerance(0.00001))
{
	// Setup
	euc::EuclideanPoint<double,2> p1(3.0, 4.0);
	euc::EuclideanPoint<double,2> p2(4.0, 12.0);
	euc::EuclideanPoint<double,2> p3(3.3, 15.0);

	// Test and Check
	euc::EuclideanPoint<double,2> p4(3.1, 6.0);

	bool inside = Triangle2D<double>::isPointInsideBarycentric(p1,p2,p3,p4);
	BOOST_CHECK_EQUAL(inside, true);
}

// Test 2: Test a point on top of vertex a - 2D
BOOST_AUTO_TEST_CASE(isPointInsideBarycentric_static_test2, * utf::tolerance(0.00001))
{
	// Setup
	euc::EuclideanPoint<double,2> p1(3.0, 4.0);
	euc::EuclideanPoint<double,2> p2(4.0, 12.0);
	euc::EuclideanPoint<double,2> p3(3.3, 15.0);

	euc::EuclideanPoint<double,2> p4 = p1;

	// Test and Check
	bool inside = Triangle2D<double>::isPointInsideBarycentric(p1,p2,p3,p4);
	BOOST_CHECK_EQUAL(inside, true);
}

// Test 3: Test a point on top of vertex b - 2D
BOOST_AUTO_TEST_CASE(isPointInsideBarycentric_static_test3, * utf::tolerance(0.00001))
{
	// Setup
	euc::EuclideanPoint<double,2> p1(3.0, 4.0);
	euc::EuclideanPoint<double,2> p2(4.0, 12.0);
	euc::EuclideanPoint<double,2> p3(3.3, 15.0);

	euc::EuclideanPoint<double,2> p4 = p2;

	// Test and Check
	bool inside = Triangle2D<double>::isPointInsideBarycentric(p1,p2,p3,p4);
	BOOST_CHECK_EQUAL(inside, true);
}

// Test 4: Test a point on top of vertex c - 2D
BOOST_AUTO_TEST_CASE(isPointInsideBarycentric_static_test4, * utf::tolerance(0.00001))
{
	// Setup
	euc::EuclideanPoint<double,2> p1(3.0, 4.0);
	euc::EuclideanPoint<double,2> p2(4.0, 12.0);
	euc::EuclideanPoint<double,2> p3(3.3, 15.0);

	euc::EuclideanPoint<double,2> p4 = p3;

	// Test and Check
	bool inside = Triangle2D<double>::isPointInsideBarycentric(p1,p2,p3,p4);
	BOOST_CHECK_EQUAL(inside, true);
}

// Test 5: Test a point on edge ab - 2D
BOOST_AUTO_TEST_CASE(isPointInsideBarycentric_static_test5, * utf::tolerance(0.00001))
{
	// Setup
	euc::EuclideanPoint<double,2> p1(3.0, 4.0);
	euc::EuclideanPoint<double,2> p2(4.0, 12.0);
	euc::EuclideanPoint<double,2> p3(3.3, 15.0);

	// Test and Check
	euc::EuclideanPoint<double,2> p4(3.5, 8.0);

	bool inside = Triangle2D<double>::isPointInsideBarycentric(p1,p2,p3,p4);
	BOOST_CHECK_EQUAL(inside, true);
}

// Test 6: Test a point on edge ac - 2D
BOOST_AUTO_TEST_CASE(isPointInsideBarycentric_static_test6, * utf::tolerance(0.00001))
{
	// Setup
	euc::EuclideanPoint<double,2> p1(3.0, 4.0);
	euc::EuclideanPoint<double,2> p2(4.0, 12.0);
	euc::EuclideanPoint<double,2> p3(3.3, 15.0);

	// Test and Check
	euc::EuclideanPoint<double,2> p4(3.15, 9.5);

	bool inside = Triangle2D<double>::isPointInsideBarycentric(p1,p2,p3,p4);
	BOOST_CHECK_EQUAL(inside, true);
}

// Test 7: Test a point on edge bc - 2D
BOOST_AUTO_TEST_CASE(isPointInsideBarycentric_static_test7, * utf::tolerance(0.00001))
{
	// Setup
	euc::EuclideanPoint<double,2> p1(3.0, 4.0);
	euc::EuclideanPoint<double,2> p2(4.0, 12.0);
	euc::EuclideanPoint<double,2> p3(3.3, 15.0);

	// Test and Check
	euc::EuclideanPoint<double,2> p4(3.65, 13.5);

	bool inside = Triangle2D<double>::isPointInsideBarycentric(p1,p2,p3,p4);
	BOOST_CHECK_EQUAL(inside, true);
}

// Test 8: Test a point outside the Triangle - 2D
BOOST_AUTO_TEST_CASE(isPointInsideBarycentric_static_test8, * utf::tolerance(0.00001))
{
	// Setup
	euc::EuclideanPoint<double,2> p1(3.0, 4.0);
	euc::EuclideanPoint<double,2> p2(4.0, 12.0);
	euc::EuclideanPoint<double,2> p3(3.3, 15.0);

	// Test and Check
	euc::EuclideanPoint<double,2> p4(1.0, 1.34);

	bool inside = Triangle2D<double>::isPointInsideBarycentric(p1,p2,p3,p4);
	BOOST_CHECK_EQUAL(inside, false);
}

// Test 9: Test a point inside when a.cmp[1] == c.cmp[1] - 2D
BOOST_AUTO_TEST_CASE(isPointInsideBarycentric_static_test9, * utf::tolerance(0.00001))
{
	// Setup
	euc::EuclideanPoint<double,2> p1(3.0, 4.0);
	euc::EuclideanPoint<double,2> p2(3.15, 12.0);
	euc::EuclideanPoint<double,2> p3(3.3, 4.0);

	// Test and Check
	euc::EuclideanPoint<double,2> p4(3.15, 4.2);

	bool inside = Triangle2D<double>::isPointInsideBarycentric(p1,p2,p3,p4);
	BOOST_CHECK_EQUAL(inside, true);
}

// === computeCentroid (static) ===
// Test 1: Correctly compute the center from three points
BOOST_AUTO_TEST_CASE(computeCenter_test1, * utf::tolerance(0.00001))
{
	euc::EuclideanPoint<double,2> p1(3.0, 4.0);
	euc::EuclideanPoint<double,2> p2(4.0, 12.0);
	euc::EuclideanPoint<double,2> p3(3.3, 15.0);

	cupcfd::geometry::euclidean::EuclideanPoint<double,2> center = Triangle2D<double>::computeCentroid(p1, p2, p3);

	BOOST_TEST(center.cmp[0] == 3.433333);
	BOOST_TEST(center.cmp[1] == 10.333333);
}

// === computeCentroid (nonstatic) ===
// Test 1: Correctly compute the center for the triangles points
BOOST_AUTO_TEST_CASE(computeCenter_test2, * utf::tolerance(0.00001))
{
	euc::EuclideanPoint<double,2> p1(3.0, 4.0);
	euc::EuclideanPoint<double,2> p2(4.0, 12.0);
	euc::EuclideanPoint<double,2> p3(3.3, 15.0);

	Triangle2D<double> shape(p1,p2,p3);
	cupcfd::geometry::euclidean::EuclideanPoint<double,2> center = shape.computeCentroid();
	BOOST_TEST(center.cmp[0] == 3.433333);
	BOOST_TEST(center.cmp[1] == 10.333333);
}

// === isPointInside ===
BOOST_AUTO_TEST_CASE(isPointInside_test1, * utf::tolerance(0.00001))
{
	// Setup
	euc::EuclideanPoint<double,2> p1(3.0, 4.0);
	euc::EuclideanPoint<double,2> p2(4.0, 12.0);
	euc::EuclideanPoint<double,2> p3(3.3, 15.0);

	// Test and Check
	euc::EuclideanPoint<double,2> p4(3.1, 6.0);

	Triangle2D<double> shape(p1,p2,p3);
	bool inside = shape.isPointInside(p4);

	BOOST_CHECK_EQUAL(inside, true);
}

// Test 2: Test a point on top of vertex a - 2D
BOOST_AUTO_TEST_CASE(isPointInside_test2, * utf::tolerance(0.00001))
{
	// Setup
	euc::EuclideanPoint<double,2> p1(3.0, 4.0);
	euc::EuclideanPoint<double,2> p2(4.0, 12.0);
	euc::EuclideanPoint<double,2> p3(3.3, 15.0);

	euc::EuclideanPoint<double,2> p4 = p1;

	// Test and Check
	Triangle2D<double> shape(p1,p2,p3);
	bool inside = shape.isPointInside(p4);

	BOOST_CHECK_EQUAL(inside, true);
}

// Test 3: Test a point on top of vertex b - 2D
BOOST_AUTO_TEST_CASE(isPointInside_test3, * utf::tolerance(0.00001))
{
	// Setup
	euc::EuclideanPoint<double,2> p1(3.0, 4.0);
	euc::EuclideanPoint<double,2> p2(4.0, 12.0);
	euc::EuclideanPoint<double,2> p3(3.3, 15.0);

	euc::EuclideanPoint<double,2> p4 = p2;

	// Test and Check
	Triangle2D<double> shape(p1,p2,p3);
	bool inside = shape.isPointInside(p4);

	BOOST_CHECK_EQUAL(inside, true);
}

// Test 4: Test a point on top of vertex c - 2D
BOOST_AUTO_TEST_CASE(isPointInside_test4, * utf::tolerance(0.00001))
{
	// Setup
	euc::EuclideanPoint<double,2> p1(3.0, 4.0);
	euc::EuclideanPoint<double,2> p2(4.0, 12.0);
	euc::EuclideanPoint<double,2> p3(3.3, 15.0);

	euc::EuclideanPoint<double,2> p4 = p3;

	// Test and Check
	Triangle2D<double> shape(p1,p2,p3);
	bool inside = shape.isPointInside(p4);

	BOOST_CHECK_EQUAL(inside, true);
}

// Test 5: Test a point on edge ab - 2D
BOOST_AUTO_TEST_CASE(isPointInside_test5, * utf::tolerance(0.00001))
{
	// Setup
	euc::EuclideanPoint<double,2> p1(3.0, 4.0);
	euc::EuclideanPoint<double,2> p2(4.0, 12.0);
	euc::EuclideanPoint<double,2> p3(3.3, 15.0);

	// Test and Check
	euc::EuclideanPoint<double,2> p4(3.5, 8.0);

	Triangle2D<double> shape(p1,p2,p3);
	bool inside = shape.isPointInside(p4);

	BOOST_CHECK_EQUAL(inside, true);
}

// Test 6: Test a point on edge ac - 2D
BOOST_AUTO_TEST_CASE(isPointInside_test6, * utf::tolerance(0.00001))
{
	// Setup
	euc::EuclideanPoint<double,2> p1(3.0, 4.0);
	euc::EuclideanPoint<double,2> p2(4.0, 12.0);
	euc::EuclideanPoint<double,2> p3(3.3, 15.0);

	// Test and Check
	euc::EuclideanPoint<double,2> p4(3.15, 9.5);

	Triangle2D<double> shape(p1,p2,p3);
	bool inside = shape.isPointInside(p4);

	BOOST_CHECK_EQUAL(inside, true);
}

// Test 7: Test a point on edge bc - 2D
BOOST_AUTO_TEST_CASE(isPointInside_test7, * utf::tolerance(0.00001))
{
	// Setup
	euc::EuclideanPoint<double,2> p1(3.0, 4.0);
	euc::EuclideanPoint<double,2> p2(4.0, 12.0);
	euc::EuclideanPoint<double,2> p3(3.3, 15.0);

	// Test and Check
	euc::EuclideanPoint<double,2> p4(3.65, 13.5);

	Triangle2D<double> shape(p1,p2,p3);
	bool inside = shape.isPointInside(p4);

	BOOST_CHECK_EQUAL(inside, true);
}

// Test 8: Test a point outside the Triangle - 2D
BOOST_AUTO_TEST_CASE(isPointInside_test8, * utf::tolerance(0.00001))
{
	// Setup
	euc::EuclideanPoint<double,2> p1(3.0, 4.0);
	euc::EuclideanPoint<double,2> p2(4.0, 12.0);
	euc::EuclideanPoint<double,2> p3(3.3, 15.0);

	// Test and Check
	euc::EuclideanPoint<double,2> p4(1.0, 1.34);

	Triangle2D<double> shape(p1,p2,p3);
	bool inside = shape.isPointInside(p4);

	BOOST_CHECK_EQUAL(inside, false);
}

// Test 9: Test a point inside when a.cmp[1] == c.cmp[1] - 2D
BOOST_AUTO_TEST_CASE(isPointInside_test9, * utf::tolerance(0.00001))
{
	// Setup
	euc::EuclideanPoint<double,2> p1(3.0, 4.0);
	euc::EuclideanPoint<double,2> p2(3.15, 12.0);
	euc::EuclideanPoint<double,2> p3(3.3, 4.0);

	// Test and Check
	euc::EuclideanPoint<double,2> p4(3.15, 4.2);

	Triangle2D<double> shape(p1,p2,p3);
	bool inside = shape.isPointInside(p4);

	BOOST_CHECK_EQUAL(inside, true);
}

// === computeArea ===
// Test 1: Test the area is computed correctly - 2D
BOOST_AUTO_TEST_CASE(computeArea_test1, * utf::tolerance(0.00001))
{
	// Setup
	euc::EuclideanPoint<double,2> p1(3.0, 4.0);
	euc::EuclideanPoint<double,2> p2(3.15, 12.0);
	euc::EuclideanPoint<double,2> p3(3.3, 4.0);

	// Test and Check
	Triangle2D<double> shape(p1, p2, p3);
	double area = shape.computeArea();
	BOOST_TEST(area == 1.2);
}

// Test 2: Test the area is computed correctly - 2D
BOOST_AUTO_TEST_CASE(computeArea_test2, * utf::tolerance(0.00001))
{
	// Setup
	euc::EuclideanPoint<double,2> p1(3.0, 4.0);
	euc::EuclideanPoint<double,2> p2(6.0, 4.0);
	euc::EuclideanPoint<double,2> p3(3.0, 12.0);

	// Test and Check
	Triangle2D<double> shape(p1, p2, p3);
	double area = shape.computeArea();
	BOOST_TEST(area == 12.0);
}
