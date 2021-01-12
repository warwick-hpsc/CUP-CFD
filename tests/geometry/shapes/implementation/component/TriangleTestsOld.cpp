/*
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Unit Tests for the concrete methods of the Triangle class
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

// === Constructor ===
// Test 1: Constructor: 3 Defined Points - 2D
BOOST_AUTO_TEST_CASE(constructor_test1)
{
	euc::EuclideanPoint<double,2> p1(3.0, 4.0);
	euc::EuclideanPoint<double,2> p2(4.0, 12.0);
	euc::EuclideanPoint<double,2> p3(3.3, 15.0);

	Triangle<double,2> shape(p1, p2, p3);

	BOOST_CHECK_EQUAL(shape.a.cmp[0], p1.cmp[0]);
	BOOST_CHECK_EQUAL(shape.a.cmp[1], p1.cmp[1]);
	BOOST_CHECK_EQUAL(shape.b.cmp[0], p2.cmp[0]);
	BOOST_CHECK_EQUAL(shape.b.cmp[1], p2.cmp[1]);
	BOOST_CHECK_EQUAL(shape.c.cmp[0], p3.cmp[0]);
	BOOST_CHECK_EQUAL(shape.c.cmp[1], p3.cmp[1]);
}

// Test 2: Constructor: 3 Defined Points - 3D
BOOST_AUTO_TEST_CASE(constructor_test2)
{
	euc::EuclideanPoint<double,3> p1(3.0, 4.0, 5.0);
	euc::EuclideanPoint<double,3> p2(4.0, 12.0, 20.0);
	euc::EuclideanPoint<double,3> p3(3.3, 15.0, 21.0);

	Triangle<double,3> shape(p1, p2, p3);
	BOOST_CHECK_EQUAL(shape.a.cmp[0], p1.cmp[0]);
	BOOST_CHECK_EQUAL(shape.a.cmp[1], p1.cmp[1]);
	BOOST_CHECK_EQUAL(shape.a.cmp[2], p1.cmp[2]);
	BOOST_CHECK_EQUAL(shape.b.cmp[0], p2.cmp[0]);
	BOOST_CHECK_EQUAL(shape.b.cmp[1], p2.cmp[1]);
	BOOST_CHECK_EQUAL(shape.b.cmp[2], p2.cmp[2]);
	BOOST_CHECK_EQUAL(shape.c.cmp[0], p3.cmp[0]);
	BOOST_CHECK_EQUAL(shape.c.cmp[1], p3.cmp[1]);
	BOOST_CHECK_EQUAL(shape.c.cmp[2], p3.cmp[2]);
}

// === isPointInsideBarycentric (static, 2D) ===
BOOST_AUTO_TEST_CASE(isPointInsideBarycentric_static_test1)
{
	// Setup
	euc::EuclideanPoint<double,2> p1(3.0, 4.0);
	euc::EuclideanPoint<double,2> p2(4.0, 12.0);
	euc::EuclideanPoint<double,2> p3(3.3, 15.0);

	// Test and Check
	euc::EuclideanPoint<double,2> p4(3.1, 6.0);

	bool inside = Triangle<double,2>::isPointInsideBarycentric(p1,p2,p3,p4);
	BOOST_CHECK_EQUAL(inside, true);
}

// Test 2: Test a point on top of vertex a - 2D
BOOST_AUTO_TEST_CASE(isPointInsideBarycentric_static_test2)
{
	// Setup
	euc::EuclideanPoint<double,2> p1(3.0, 4.0);
	euc::EuclideanPoint<double,2> p2(4.0, 12.0);
	euc::EuclideanPoint<double,2> p3(3.3, 15.0);

	euc::EuclideanPoint<double,2> p4 = p1;

	// Test and Check
	bool inside = Triangle<double,2>::isPointInsideBarycentric(p1,p2,p3,p4);
	BOOST_CHECK_EQUAL(inside, true);
}

// Test 3: Test a point on top of vertex b - 2D
BOOST_AUTO_TEST_CASE(isPointInsideBarycentric_static_test3)
{
	// Setup
	euc::EuclideanPoint<double,2> p1(3.0, 4.0);
	euc::EuclideanPoint<double,2> p2(4.0, 12.0);
	euc::EuclideanPoint<double,2> p3(3.3, 15.0);

	euc::EuclideanPoint<double,2> p4 = p2;

	// Test and Check
	bool inside = Triangle<double,2>::isPointInsideBarycentric(p1,p2,p3,p4);
	BOOST_CHECK_EQUAL(inside, true);
}

// Test 4: Test a point on top of vertex c - 2D
BOOST_AUTO_TEST_CASE(isPointInsideBarycentric_static_test4)
{
	// Setup
	euc::EuclideanPoint<double,2> p1(3.0, 4.0);
	euc::EuclideanPoint<double,2> p2(4.0, 12.0);
	euc::EuclideanPoint<double,2> p3(3.3, 15.0);

	euc::EuclideanPoint<double,2> p4 = p3;

	// Test and Check
	bool inside = Triangle<double,2>::isPointInsideBarycentric(p1,p2,p3,p4);
	BOOST_CHECK_EQUAL(inside, true);
}

// Test 5: Test a point on edge ab - 2D
BOOST_AUTO_TEST_CASE(isPointInsideBarycentric_static_test5)
{
	// Setup
	euc::EuclideanPoint<double,2> p1(3.0, 4.0);
	euc::EuclideanPoint<double,2> p2(4.0, 12.0);
	euc::EuclideanPoint<double,2> p3(3.3, 15.0);

	// Test and Check
	euc::EuclideanPoint<double,2> p4(3.5, 8.0);

	bool inside = Triangle<double,2>::isPointInsideBarycentric(p1,p2,p3,p4);
	BOOST_CHECK_EQUAL(inside, true);
}

// Test 6: Test a point on edge ac - 2D
BOOST_AUTO_TEST_CASE(isPointInsideBarycentric_static_test6)
{
	// Setup
	euc::EuclideanPoint<double,2> p1(3.0, 4.0);
	euc::EuclideanPoint<double,2> p2(4.0, 12.0);
	euc::EuclideanPoint<double,2> p3(3.3, 15.0);

	// Test and Check
	euc::EuclideanPoint<double,2> p4(3.15, 9.5);

	bool inside = Triangle<double,2>::isPointInsideBarycentric(p1,p2,p3,p4);
	BOOST_CHECK_EQUAL(inside, true);
}

// Test 7: Test a point on edge bc - 2D
BOOST_AUTO_TEST_CASE(isPointInsideBarycentric_static_test7)
{
	// Setup
	euc::EuclideanPoint<double,2> p1(3.0, 4.0);
	euc::EuclideanPoint<double,2> p2(4.0, 12.0);
	euc::EuclideanPoint<double,2> p3(3.3, 15.0);

	// Test and Check
	euc::EuclideanPoint<double,2> p4(3.65, 13.5);

	bool inside = Triangle<double,2>::isPointInsideBarycentric(p1,p2,p3,p4);
	BOOST_CHECK_EQUAL(inside, true);
}

// Test 8: Test a point outside the Triangle - 2D
BOOST_AUTO_TEST_CASE(isPointInsideBarycentric_static_test8)
{
	// Setup
	euc::EuclideanPoint<double,2> p1(3.0, 4.0);
	euc::EuclideanPoint<double,2> p2(4.0, 12.0);
	euc::EuclideanPoint<double,2> p3(3.3, 15.0);

	// Test and Check
	euc::EuclideanPoint<double,2> p4(1.0, 1.34);

	bool inside = Triangle<double,2>::isPointInsideBarycentric(p1,p2,p3,p4);
	BOOST_CHECK_EQUAL(inside, false);
}

// Test 9: Test a point inside when a.cmp[1] == c.cmp[1] - 2D
BOOST_AUTO_TEST_CASE(isPointInsideBarycentric_static_test9)
{
	// Setup
	euc::EuclideanPoint<double,2> p1(3.0, 4.0);
	euc::EuclideanPoint<double,2> p2(3.15, 12.0);
	euc::EuclideanPoint<double,2> p3(3.3, 4.0);

	// Test and Check
	euc::EuclideanPoint<double,2> p4(3.15, 4.2);

	bool inside = Triangle<double,2>::isPointInsideBarycentric(p1,p2,p3,p4);
	BOOST_CHECK_EQUAL(inside, true);
}

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

	Triangle<double,3> shape(p1, p2, p3);

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

	Triangle<double,3> shape(p1, p2, p3);

	// Test and Check
	double area = Triangle<double,3>::heronsFormula(p1, p2, p3);
	BOOST_TEST(area == 16.3126);
}

// === getArea ===
// Test 1: Test the area is computed correctly - 2D
BOOST_AUTO_TEST_CASE(getArea_test1, * utf::tolerance(0.00001))
{
	// Setup
	euc::EuclideanPoint<double,2> p1(3.0, 4.0);
	euc::EuclideanPoint<double,2> p2(3.15, 12.0);
	euc::EuclideanPoint<double,2> p3(3.3, 4.0);

	// Test and Check
	Triangle<double,2> shape(p1, p2, p3);
	double area = shape.getArea();
	BOOST_TEST(area == 1.2);
}

// Test 2: Test the area is computed correctly - 2D
BOOST_AUTO_TEST_CASE(getArea_test2, * utf::tolerance(0.00001))
{
	// Setup
	euc::EuclideanPoint<double,2> p1(3.0, 4.0);
	euc::EuclideanPoint<double,2> p2(6.0, 4.0);
	euc::EuclideanPoint<double,2> p3(3.0, 12.0);

	// Test and Check
	Triangle<double,2> shape(p1, p2, p3);
	double area = shape.getArea();
	BOOST_TEST(area == 12.0);
}

// Test 3: Test the area is computed correctly - 3D
BOOST_AUTO_TEST_CASE(getArea_test3,  * utf::tolerance(0.00001))
{
	// Setup
	euc::EuclideanPoint<double,3> p1(3.0, 4.0, 8.0);
	euc::EuclideanPoint<double,3> p2(3.15, 12.0, 14.0);
	euc::EuclideanPoint<double,3> p3(3.3, 4.0, 9.0);

	// Test and Check
	Triangle<double,3> shape(p1, p2, p3);
	double area = shape.getArea();
	BOOST_TEST(area == 4.25683);
}

// Test 4: Test the area is computed correctly - 3D
BOOST_AUTO_TEST_CASE(getArea_test4,  * utf::tolerance(0.00001))
{
	// Setup
	euc::EuclideanPoint<double,3> p1(3.0, 4.0, 8.9);
	euc::EuclideanPoint<double,3> p2(6.0, 4.0, 7.6);
	euc::EuclideanPoint<double,3> p3(3.0, 12.0, 15.4);

	// Test and Check
	Triangle<double,3> shape(p1, p2, p3);
	double area = shape.getArea();
	BOOST_TEST(area == 16.3126);
}

// === getNormal ===
// Test 1: Compute the correct normal, and check that the direction is as expected for anticlockwise
// vertices from origin
BOOST_AUTO_TEST_CASE(getNormal_test1,  * utf::tolerance(0.00001))
{
	euc::EuclideanPoint<double,3> p1(0.0, 0.0, 2.0);
	euc::EuclideanPoint<double,3> p2(5.0, 0.0, 2.0);
	euc::EuclideanPoint<double,3> p3(0.0, 5.0, 2.0);

	Triangle<double,3> shape(p1, p2, p3);

	euc::EuclideanVector<double,3> normalCmp(0.0,0.0,1.0);
	euc::EuclideanVector<double,3> normal = shape.getNormal();

	BOOST_TEST(normal.cmp[0] == normalCmp[0]);
	BOOST_TEST(normal.cmp[1] == normalCmp[1]);
	BOOST_TEST(normal.cmp[2] == normalCmp[2]);
}

// Test 2: Compute the correct normal, and check that the direction is as expected for clockwise vertices
// from origin
BOOST_AUTO_TEST_CASE(getNormal_test2,  * utf::tolerance(0.00001))
{
	euc::EuclideanPoint<double,3> p1(0.0, 5.0, 2.0);
	euc::EuclideanPoint<double,3> p2(5.0, 0.0, 2.0);
	euc::EuclideanPoint<double,3> p3(0.0, 0.0, 2.0);

	Triangle<double,3> shape(p1, p2, p3);

	euc::EuclideanVector<double,3> normalCmp(0.0,0.0,-1.0);
	euc::EuclideanVector<double,3> normal = shape.getNormal();

	BOOST_TEST(normal.cmp[0] == normalCmp[0]);
	BOOST_TEST(normal.cmp[1] == normalCmp[1]);
	BOOST_TEST(normal.cmp[2] == normalCmp[2]);
}

// Test 3: Test correct normal for a 2D triangle translated into a 3D space with a Z component of 0.
BOOST_AUTO_TEST_CASE(getNormal_test3,  * utf::tolerance(0.00001))
{

}
