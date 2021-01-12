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


#define BOOST_TEST_MODULE Triangle3D
#include <boost/test/unit_test.hpp>
#include <boost/test/output_test_stream.hpp>
#include <stdexcept>

#include "Triangle3D.h"
#include "EuclideanPoint.h"

using namespace cupcfd::geometry::shapes;
namespace euc = cupcfd::geometry::euclidean;
namespace utf = boost::unit_test;

// === Constructor ===
// Test 1: Constructor: 3 Defined Points - 3D
BOOST_AUTO_TEST_CASE(constructor_test1, * utf::tolerance(0.00001))
{
	euc::EuclideanPoint<double,3> p1(3.0, 4.0, 5.0);
	euc::EuclideanPoint<double,3> p2(4.0, 12.0, 20.0);
	euc::EuclideanPoint<double,3> p3(3.3, 15.0, 21.0);

	Triangle3D<double> shape(p1, p2, p3);

	BOOST_CHECK_EQUAL(shape.numVertices, 3);
	BOOST_CHECK_EQUAL(shape.vertices[0].cmp[0], p1.cmp[0]);
	BOOST_CHECK_EQUAL(shape.vertices[0].cmp[1], p1.cmp[1]);
	BOOST_CHECK_EQUAL(shape.vertices[0].cmp[2], p1.cmp[2]);
	BOOST_CHECK_EQUAL(shape.vertices[1].cmp[0], p2.cmp[0]);
	BOOST_CHECK_EQUAL(shape.vertices[1].cmp[1], p2.cmp[1]);
	BOOST_CHECK_EQUAL(shape.vertices[1].cmp[2], p2.cmp[2]);
	BOOST_CHECK_EQUAL(shape.vertices[2].cmp[0], p3.cmp[0]);
	BOOST_CHECK_EQUAL(shape.vertices[2].cmp[1], p3.cmp[1]);
	BOOST_CHECK_EQUAL(shape.vertices[2].cmp[2], p3.cmp[2]);
}

// Test 2: Copy Constructor
BOOST_AUTO_TEST_CASE(constructor_test2, * utf::tolerance(0.00001))
{
	euc::EuclideanPoint<double,3> p1(3.0, 4.0, 5.0);
	euc::EuclideanPoint<double,3> p2(4.0, 12.0, 20.0);
	euc::EuclideanPoint<double,3> p3(3.3, 15.0, 21.0);

	Triangle3D<double> shape2(p1, p2, p3);
	Triangle3D<double> shape(shape2);

	BOOST_CHECK_EQUAL(shape.numVertices, 3);
	BOOST_CHECK_EQUAL(shape.vertices[0].cmp[0], p1.cmp[0]);
	BOOST_CHECK_EQUAL(shape.vertices[0].cmp[1], p1.cmp[1]);
	BOOST_CHECK_EQUAL(shape.vertices[0].cmp[2], p1.cmp[2]);
	BOOST_CHECK_EQUAL(shape.vertices[1].cmp[0], p2.cmp[0]);
	BOOST_CHECK_EQUAL(shape.vertices[1].cmp[1], p2.cmp[1]);
	BOOST_CHECK_EQUAL(shape.vertices[1].cmp[2], p2.cmp[2]);
	BOOST_CHECK_EQUAL(shape.vertices[2].cmp[0], p3.cmp[0]);
	BOOST_CHECK_EQUAL(shape.vertices[2].cmp[1], p3.cmp[1]);
	BOOST_CHECK_EQUAL(shape.vertices[2].cmp[2], p3.cmp[2]);
}

// === getCentroid (static) ===
// Test 1: Correctly compute the center from three points
BOOST_AUTO_TEST_CASE(computeCenter_test1, * utf::tolerance(0.00001))
{
	euc::EuclideanPoint<double,3> p1(3.0, 4.0, 5.0);
	euc::EuclideanPoint<double,3> p2(4.0, 12.0, 20.0);
	euc::EuclideanPoint<double,3> p3(3.3, 15.0, 21.0);

	Triangle3D<double> tri(p1, p2, p3);
	euc::EuclideanPoint<double,3> center = tri.getCentroid();

	BOOST_TEST(center.cmp[0] == 3.433333);
	BOOST_TEST(center.cmp[1] == 10.333333);
	BOOST_TEST(center.cmp[2] == 15.333333);
}

// === getCentroid (nonstatic) ===
// Test 1: Correctly compute the center for the triangles points
BOOST_AUTO_TEST_CASE(computeCenter_test2, * utf::tolerance(0.00001))
{
	euc::EuclideanPoint<double,3> p1(3.0, 4.0, 5.0);
	euc::EuclideanPoint<double,3> p2(4.0, 12.0, 20.0);
	euc::EuclideanPoint<double,3> p3(3.3, 15.0, 21.0);

	Triangle3D<double> shape(p1, p2, p3);
	euc::EuclideanPoint<double,3> center = shape.getCentroid();

	BOOST_TEST(center.cmp[0] == 3.433333);
	BOOST_TEST(center.cmp[1] == 10.333333);
	BOOST_TEST(center.cmp[2] == 15.333333);
}

// === isPointInside ===
// Test 1: Test point on point a is inside
BOOST_AUTO_TEST_CASE(isPointInside_test1,  * utf::tolerance(0.00001))
{
	euc::EuclideanPoint<double,3> p1(3.0, 4.0, 5.0);
	euc::EuclideanPoint<double,3> p2(4.0, 12.0, 20.0);
	euc::EuclideanPoint<double,3> p3(3.3, 15.0, 21.0);

	Triangle3D<double> shape(p1, p2, p3);
	bool isInside = shape.isPointInside(p1);
	BOOST_CHECK_EQUAL(isInside, true);
}

// Test 2: Test point on point b is inside
BOOST_AUTO_TEST_CASE(isPointInside_test2,  * utf::tolerance(0.00001))
{
	euc::EuclideanPoint<double,3> p1(3.0, 4.0, 5.0);
	euc::EuclideanPoint<double,3> p2(4.0, 12.0, 20.0);
	euc::EuclideanPoint<double,3> p3(3.3, 15.0, 21.0);

	Triangle3D<double> shape(p1, p2, p3);
	bool isInside = shape.isPointInside(p2);
	BOOST_CHECK_EQUAL(isInside, true);
}

// Test 3: Test point on point c is inside
BOOST_AUTO_TEST_CASE(isPointInside_test3,  * utf::tolerance(0.00001))
{
	euc::EuclideanPoint<double,3> p1(3.0, 4.0, 5.0);
	euc::EuclideanPoint<double,3> p2(4.0, 12.0, 20.0);
	euc::EuclideanPoint<double,3> p3(3.3, 15.0, 21.0);

	Triangle3D<double> shape(p1, p2, p3);
	bool isInside = shape.isPointInside(p3);
	BOOST_CHECK_EQUAL(isInside, true);
}

// Test 4: Test point on edge ab is inside
BOOST_AUTO_TEST_CASE(isPointInside_test4,  * utf::tolerance(0.00001))
{
	euc::EuclideanPoint<double,3> p1(3.0, 4.0, 5.0);
	euc::EuclideanPoint<double,3> p2(4.0, 12.0, 20.0);
	euc::EuclideanPoint<double,3> p3(3.3, 15.0, 21.0);

	euc::EuclideanPoint<double,3> p(3.5, 8.0, 12.5);

	Triangle3D<double> shape(p1, p2, p3);
	bool isInside = shape.isPointInside(p);
	BOOST_CHECK_EQUAL(isInside, true);
}

// Test 5: Test point on edge bc is inside
BOOST_AUTO_TEST_CASE(isPointInside_test5,  * utf::tolerance(0.00001))
{
	euc::EuclideanPoint<double,3> p1(3.0, 4.0, 5.0);
	euc::EuclideanPoint<double,3> p2(4.0, 12.0, 20.0);
	euc::EuclideanPoint<double,3> p3(3.3, 15.0, 21.0);

	euc::EuclideanPoint<double,3> p(3.65, 13.5, 20.5);

	Triangle3D<double> shape(p1, p2, p3);
	bool isInside = shape.isPointInside(p);
	BOOST_CHECK_EQUAL(isInside, true);
}

// Test 6: Test point on edge ac is inside
BOOST_AUTO_TEST_CASE(isPointInside_test6,  * utf::tolerance(0.00001))
{
	euc::EuclideanPoint<double,3> p1(3.0, 4.0, 5.0);
	euc::EuclideanPoint<double,3> p2(4.0, 12.0, 20.0);
	euc::EuclideanPoint<double,3> p3(3.3, 15.0, 21.0);

	euc::EuclideanPoint<double,3> p(3.15, 9.5, 13.0);

	Triangle3D<double> shape(p1, p2, p3);
	bool isInside = shape.isPointInside(p);
	BOOST_CHECK_EQUAL(isInside, true);
}

// Test 7: Test point on same plane inside polygon is inside
BOOST_AUTO_TEST_CASE(isPointInside_test7,  * utf::tolerance(0.00001))
{
	euc::EuclideanPoint<double,3> p1(3.0, 4.0, 5.0);
	euc::EuclideanPoint<double,3> p2(4.0, 12.0, 20.0);
	euc::EuclideanPoint<double,3> p3(3.3, 15.0, 21.0);

	euc::EuclideanPoint<double,3> p(3.32, 10.0, 14.4);

	Triangle3D<double> shape(p1, p2, p3);
	bool isInside = shape.isPointInside(p);
	BOOST_CHECK_EQUAL(isInside, true);
}

// Test 8: Test point on same plane outside polygon is detected as outside
BOOST_AUTO_TEST_CASE(isPointInside_test8,  * utf::tolerance(0.00001))
{
	euc::EuclideanPoint<double,3> p1(3.0, 4.0, 5.0);
	euc::EuclideanPoint<double,3> p2(4.0, 12.0, 20.0);
	euc::EuclideanPoint<double,3> p3(3.3, 15.0, 21.0);

	euc::EuclideanPoint<double,3> p(2.3, 7.0, 6.0);

	Triangle3D<double> shape(p1, p2, p3);
	bool isInside = shape.isPointInside(p);
	BOOST_CHECK_EQUAL(isInside, false);
}

// Test 9: Test point directly above but otherwise inside polygon is detected as outside
BOOST_AUTO_TEST_CASE(isPointInside_test9,  * utf::tolerance(0.00001))
{
	euc::EuclideanPoint<double,3> p1(3.0, 4.0, 5.0);
	euc::EuclideanPoint<double,3> p2(4.0, 12.0, 20.0);
	euc::EuclideanPoint<double,3> p3(3.3, 15.0, 21.0);

	euc::EuclideanPoint<double,3> p(3.0, 4.0, 8.0);

	Triangle3D<double> shape(p1, p2, p3);
	bool isInside = shape.isPointInside(p);
	BOOST_CHECK_EQUAL(isInside, false);
}

// Test 10: Test point directly below but otherwise inside polygon is detected as outside
BOOST_AUTO_TEST_CASE(isPointInside_test10,  * utf::tolerance(0.00001))
{
	euc::EuclideanPoint<double,3> p1(3.0, 4.0, 5.0);
	euc::EuclideanPoint<double,3> p2(4.0, 12.0, 20.0);
	euc::EuclideanPoint<double,3> p3(3.3, 15.0, 21.0);

	euc::EuclideanPoint<double,3> p(3.0, 4.0, 2.0);

	Triangle3D<double> shape(p1, p2, p3);
	bool isInside = shape.isPointInside(p);
	BOOST_CHECK_EQUAL(isInside, false);
}

// Test 11: Test centroid point is inside
BOOST_AUTO_TEST_CASE(isPointInside_test11,  * utf::tolerance(0.00001))
{
	euc::EuclideanPoint<double,3> p1(3.0, 4.0, 5.0);
	euc::EuclideanPoint<double,3> p2(4.0, 12.0, 20.0);
	euc::EuclideanPoint<double,3> p3(3.3, 15.0, 21.0);

	Triangle3D<double> shape(p1, p2, p3);
	euc::EuclideanPoint<double,3> centroid = shape.getCentroid();
	bool isInside = shape.isPointInside(centroid);
	BOOST_CHECK_EQUAL(isInside, true);
}

// === getArea ===
// Test 1: Test the area is computed correctly
BOOST_AUTO_TEST_CASE(getArea_test1, * utf::tolerance(0.00001))
{
	// Setup
	euc::EuclideanPoint<double,3> p1(3.0, 4.0, 8.0);
	euc::EuclideanPoint<double,3> p2(3.15, 12.0, 14.0);
	euc::EuclideanPoint<double,3> p3(3.3, 4.0, 9.0);

	// Test and Check
	Triangle3D<double> shape(p1, p2, p3);
	double area = shape.getArea();
	BOOST_TEST(area == 4.25683);
}

// Test 2: Test the area is computed correctly
BOOST_AUTO_TEST_CASE(getArea_test2,  * utf::tolerance(0.00001))
{
	// Setup
	euc::EuclideanPoint<double,3> p1(3.0, 4.0, 8.9);
	euc::EuclideanPoint<double,3> p2(6.0, 4.0, 7.6);
	euc::EuclideanPoint<double,3> p3(3.0, 12.0, 15.4);

	// Test and Check
	Triangle3D<double> shape(p1, p2, p3);
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

	Triangle3D<double> shape(p1, p2, p3);


	// Technically this is the unit vector, but in non-reduced form
	euc::EuclideanVector3D<double> normalCmp(0.0,0.0,25.0);
	euc::EuclideanVector3D<double> normal = shape.getNormal();

	BOOST_TEST(normal.cmp[0] == normalCmp.cmp[0]);
	BOOST_TEST(normal.cmp[1] == normalCmp.cmp[1]);
	BOOST_TEST(normal.cmp[2] == normalCmp.cmp[2]);
}

// Test 2: Compute the correct normal, and check that the direction is as expected for clockwise vertices
// from origin
BOOST_AUTO_TEST_CASE(getNormal_test2,  * utf::tolerance(0.00001))
{
	euc::EuclideanPoint<double,3> p1(0.0, 5.0, 2.0);
	euc::EuclideanPoint<double,3> p2(5.0, 0.0, 2.0);
	euc::EuclideanPoint<double,3> p3(0.0, 0.0, 2.0);

	Triangle3D<double> shape(p1, p2, p3);

	// Technically this is the unit vector, but in non-reduced form
	euc::EuclideanVector3D<double> normalCmp(0.0,0.0,-25.0);
	euc::EuclideanVector3D<double> normal = shape.getNormal();

	BOOST_TEST(normal.cmp[0] == normalCmp.cmp[0]);
	BOOST_TEST(normal.cmp[1] == normalCmp.cmp[1]);
	BOOST_TEST(normal.cmp[2] == normalCmp.cmp[2]);
}
