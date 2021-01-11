/*
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Unit Tests for the methods of the Quadrilateral3D class
 */


#define BOOST_TEST_MODULE Quadrilateral3D
#include <boost/test/unit_test.hpp>
#include <boost/test/output_test_stream.hpp>
#include <stdexcept>

#include "Quadrilateral3D.h"
#include "EuclideanPoint.h"

using namespace cupcfd::geometry::shapes;
namespace euc = cupcfd::geometry::euclidean;
namespace utf = boost::unit_test;

// === Constructor ===
// Test 1: Create a Quadrilateral in a 2D Space
BOOST_AUTO_TEST_CASE(constructor_test1, * utf::tolerance(0.00001))
{
	euc::EuclideanPoint<double, 3> p1(0.12, 0.34, 0.7);
	euc::EuclideanPoint<double, 3> p2(0.15, 0.36, 0.7);
	euc::EuclideanPoint<double, 3> p3(0.14, 0.32, 0.7);
	euc::EuclideanPoint<double, 3> p4(0.11, 0.31, 0.7);

	Quadrilateral3D<double> shape(p1, p2, p3, p4);

	BOOST_CHECK_EQUAL(shape.numVertices, 4);
	BOOST_CHECK_EQUAL(shape.vertices[0].cmp[0], 0.12);
	BOOST_CHECK_EQUAL(shape.vertices[0].cmp[1], 0.34);
	BOOST_CHECK_EQUAL(shape.vertices[0].cmp[2], 0.7);
	BOOST_CHECK_EQUAL(shape.vertices[1].cmp[0], 0.15);
	BOOST_CHECK_EQUAL(shape.vertices[1].cmp[1], 0.36);
	BOOST_CHECK_EQUAL(shape.vertices[1].cmp[2], 0.7);
	BOOST_CHECK_EQUAL(shape.vertices[2].cmp[0], 0.14);
	BOOST_CHECK_EQUAL(shape.vertices[2].cmp[1], 0.32);
	BOOST_CHECK_EQUAL(shape.vertices[2].cmp[2], 0.7);
	BOOST_CHECK_EQUAL(shape.vertices[3].cmp[0], 0.11);
	BOOST_CHECK_EQUAL(shape.vertices[3].cmp[1], 0.31);
	BOOST_CHECK_EQUAL(shape.vertices[3].cmp[2], 0.7);
}

// Test 2: Copy Constructor
BOOST_AUTO_TEST_CASE(constructor_test2, * utf::tolerance(0.00001))
{
	euc::EuclideanPoint<double, 3> p1(0.12, 0.34, 0.7);
	euc::EuclideanPoint<double, 3> p2(0.15, 0.36, 0.7);
	euc::EuclideanPoint<double, 3> p3(0.14, 0.32, 0.7);
	euc::EuclideanPoint<double, 3> p4(0.11, 0.31, 0.7);

	Quadrilateral3D<double> shape(p1, p2, p3, p4);
	Quadrilateral3D<double> shape2(shape);

	BOOST_CHECK_EQUAL(shape2.numVertices, 4);
	BOOST_CHECK_EQUAL(shape2.vertices[0].cmp[0], 0.12);
	BOOST_CHECK_EQUAL(shape2.vertices[0].cmp[1], 0.34);
	BOOST_CHECK_EQUAL(shape2.vertices[0].cmp[2], 0.7);
	BOOST_CHECK_EQUAL(shape2.vertices[1].cmp[0], 0.15);
	BOOST_CHECK_EQUAL(shape2.vertices[1].cmp[1], 0.36);
	BOOST_CHECK_EQUAL(shape2.vertices[1].cmp[2], 0.7);
	BOOST_CHECK_EQUAL(shape2.vertices[2].cmp[0], 0.14);
	BOOST_CHECK_EQUAL(shape2.vertices[2].cmp[1], 0.32);
	BOOST_CHECK_EQUAL(shape2.vertices[2].cmp[2], 0.7);
	BOOST_CHECK_EQUAL(shape2.vertices[3].cmp[0], 0.11);
	BOOST_CHECK_EQUAL(shape2.vertices[3].cmp[1], 0.31);
	BOOST_CHECK_EQUAL(shape2.vertices[3].cmp[2], 0.7);
}

// === Operator= ===
// Test 1: Test correct copy of values
BOOST_AUTO_TEST_CASE(operator_copy_test1, * utf::tolerance(0.00001))
{

}

// === clone ===
// Test 1: Test correct clone of object
BOOST_AUTO_TEST_CASE(clone_test1, * utf::tolerance(0.00001))
{
	euc::EuclideanPoint<double, 3> p1(0.12, 0.34, 0.7);
	euc::EuclideanPoint<double, 3> p2(0.15, 0.36, 0.7);
	euc::EuclideanPoint<double, 3> p3(0.14, 0.32, 0.7);
	euc::EuclideanPoint<double, 3> p4(0.11, 0.31, 0.7);

	Quadrilateral3D<double> shape(p1, p2, p3, p4);
	Quadrilateral3D<double> * shape2;

	shape2 = shape.clone();

	BOOST_CHECK_EQUAL(shape2->vertices[0].cmp[0], 0.12);
	BOOST_CHECK_EQUAL(shape2->vertices[0].cmp[1], 0.34);
	BOOST_CHECK_EQUAL(shape2->vertices[0].cmp[2], 0.7);
	BOOST_CHECK_EQUAL(shape2->vertices[1].cmp[0], 0.15);
	BOOST_CHECK_EQUAL(shape2->vertices[1].cmp[1], 0.36);
	BOOST_CHECK_EQUAL(shape2->vertices[1].cmp[2], 0.7);
	BOOST_CHECK_EQUAL(shape2->vertices[2].cmp[0], 0.14);
	BOOST_CHECK_EQUAL(shape2->vertices[2].cmp[1], 0.32);
	BOOST_CHECK_EQUAL(shape2->vertices[2].cmp[2], 0.7);
	BOOST_CHECK_EQUAL(shape2->vertices[3].cmp[0], 0.11);
	BOOST_CHECK_EQUAL(shape2->vertices[3].cmp[1], 0.31);
	BOOST_CHECK_EQUAL(shape2->vertices[3].cmp[2], 0.7);

	delete(shape2);
}

// === validQuadrilateral ===

// Test 1: Test a set of points that are a valid quadrilateral - 3D
BOOST_AUTO_TEST_CASE(validQuadrilateral_test1, * utf::tolerance(0.00001))
{

}

// Test 2: Test a set of points where the ordering is incorrect (i.e. edges overlap)- 3D
BOOST_AUTO_TEST_CASE(validQuadrilateral_test2, * utf::tolerance(0.00001))
{

}

// Test 3: Test a set of points that are not coplanar - 3D
BOOST_AUTO_TEST_CASE(validQuadrilateral_test3, * utf::tolerance(0.00001))
{

}

// === getCentroid ===
// Test 1: Compute the correct centroid for a arbitrary Quadrilateral in 3D
BOOST_AUTO_TEST_CASE(getCentroid_test1, * utf::tolerance(0.00001))
{
	euc::EuclideanPoint<double, 3> p1(0.1, 0.38, 0.7);
	euc::EuclideanPoint<double, 3> p2(0.2, 0.38, 0.7);
	euc::EuclideanPoint<double, 3> p3(0.0833, 0.16, 0.7);
	euc::EuclideanPoint<double, 3> p4(0.01672, 0.16, 0.7);

	Quadrilateral3D<double> shape(p1, p2, p3, p4);
	euc::EuclideanPoint<double,3> centroid = shape.getCentroid();
	BOOST_TEST(centroid.cmp[0] == 0.103348);
	BOOST_TEST(centroid.cmp[1] == 0.277356);
	BOOST_TEST(centroid.cmp[2] == 0.7);
}

// === isPointInside ===
// Test 1: Test a point on point a is inside
BOOST_AUTO_TEST_CASE(isPointInside_test1, * utf::tolerance(0.00001))
{
	euc::EuclideanPoint<double, 3> p1(0.1, 0.38, 0.7);
	euc::EuclideanPoint<double, 3> p2(0.2, 0.38, 0.7);
	euc::EuclideanPoint<double, 3> p3(0.0833, 0.16, 0.78);
	euc::EuclideanPoint<double, 3> p4(0.01672, 0.16, 0.78);

	Quadrilateral3D<double> shape(p1, p2, p3, p4);
	bool isPointInside = shape.isPointInside(p1);
	BOOST_CHECK_EQUAL(isPointInside, true);
}

// Test 2: Test a point on point b is inside
BOOST_AUTO_TEST_CASE(isPointInside_test2, * utf::tolerance(0.00001))
{
	euc::EuclideanPoint<double, 3> p1(0.1, 0.38, 0.7);
	euc::EuclideanPoint<double, 3> p2(0.2, 0.38, 0.7);
	euc::EuclideanPoint<double, 3> p3(0.0833, 0.16, 0.78);
	euc::EuclideanPoint<double, 3> p4(0.01672, 0.16, 0.78);

	Quadrilateral3D<double> shape(p1, p2, p3, p4);
	bool isPointInside = shape.isPointInside(p2);
	BOOST_CHECK_EQUAL(isPointInside, true);
}

// Test 3: Test a point on point c is inside
BOOST_AUTO_TEST_CASE(isPointInside_test3, * utf::tolerance(0.00001))
{
	euc::EuclideanPoint<double, 3> p1(0.1, 0.38, 0.7);
	euc::EuclideanPoint<double, 3> p2(0.2, 0.38, 0.7);
	euc::EuclideanPoint<double, 3> p3(0.0833, 0.16, 0.78);
	euc::EuclideanPoint<double, 3> p4(0.01672, 0.16, 0.78);

	Quadrilateral3D<double> shape(p1, p2, p3, p4);
	bool isPointInside = shape.isPointInside(p3);
	BOOST_CHECK_EQUAL(isPointInside, true);
}

// Test 4: Test a point on point d is inside
BOOST_AUTO_TEST_CASE(isPointInside_test4, * utf::tolerance(0.00001))
{
	euc::EuclideanPoint<double, 3> p1(0.1, 0.38, 0.7);
	euc::EuclideanPoint<double, 3> p2(0.2, 0.38, 0.7);
	euc::EuclideanPoint<double, 3> p3(0.0833, 0.16, 0.78);
	euc::EuclideanPoint<double, 3> p4(0.01672, 0.16, 0.78);

	Quadrilateral3D<double> shape(p1, p2, p3, p4);
	bool isPointInside = shape.isPointInside(p4);
	BOOST_CHECK_EQUAL(isPointInside, true);
}

// Test 5: Test a point on edge ab is inside
BOOST_AUTO_TEST_CASE(isPointInside_test5, * utf::tolerance(0.00001))
{
	euc::EuclideanPoint<double, 3> p1(0.1, 0.38, 0.7);
	euc::EuclideanPoint<double, 3> p2(0.2, 0.38, 0.7);
	euc::EuclideanPoint<double, 3> p3(0.0833, 0.16, 0.78);
	euc::EuclideanPoint<double, 3> p4(0.01672, 0.16, 0.78);

	euc::EuclideanPoint<double, 3> point(0.15, 0.38, 0.7);
	Quadrilateral3D<double> shape(p1, p2, p3, p4);
	bool isPointInside = shape.isPointInside(point);
	BOOST_CHECK_EQUAL(isPointInside, true);
}

// Test 6: Test a point on edge bc is inside
BOOST_AUTO_TEST_CASE(isPointInside_test6, * utf::tolerance(0.00001))
{
	euc::EuclideanPoint<double, 3> p1(0.1, 0.38, 0.7);
	euc::EuclideanPoint<double, 3> p2(0.2, 0.38, 0.7);
	euc::EuclideanPoint<double, 3> p3(0.0833, 0.16, 0.78);
	euc::EuclideanPoint<double, 3> p4(0.01672, 0.16, 0.78);

	euc::EuclideanPoint<double, 3> point(0.14165, 0.27, 0.74);
	Quadrilateral3D<double> shape(p1, p2, p3, p4);
	bool isPointInside = shape.isPointInside(point);
	BOOST_CHECK_EQUAL(isPointInside, true);
}

// Test 7: Test a point on edge cd is inside
BOOST_AUTO_TEST_CASE(isPointInside_test7, * utf::tolerance(0.00001))
{
	euc::EuclideanPoint<double, 3> p1(0.1, 0.38, 0.7);
	euc::EuclideanPoint<double, 3> p2(0.2, 0.38, 0.7);
	euc::EuclideanPoint<double, 3> p3(0.0833, 0.16, 0.78);
	euc::EuclideanPoint<double, 3> p4(0.01672, 0.16, 0.78);

	euc::EuclideanPoint<double, 3> point(0.05001, 0.16, 0.78);
	Quadrilateral3D<double> shape(p1, p2, p3, p4);
	bool isPointInside = shape.isPointInside(point);
	BOOST_CHECK_EQUAL(isPointInside, true);
}

// Test 8: Test a point on edge ad is inside
BOOST_AUTO_TEST_CASE(isPointInside_test8, * utf::tolerance(0.00001))
{
	euc::EuclideanPoint<double, 3> p1(0.1, 0.38, 0.7);
	euc::EuclideanPoint<double, 3> p2(0.2, 0.38, 0.7);
	euc::EuclideanPoint<double, 3> p3(0.0833, 0.16, 0.78);
	euc::EuclideanPoint<double, 3> p4(0.01672, 0.16, 0.78);

	euc::EuclideanPoint<double, 3> point(0.05836, 0.27, 0.74);
	Quadrilateral3D<double> shape(p1, p2, p3, p4);
	bool isPointInside = shape.isPointInside(point);
	BOOST_CHECK_EQUAL(isPointInside, true);
}

// Test 9: Test the centroid is detected as inside (zero vector case)
BOOST_AUTO_TEST_CASE(isPointInside_test9, * utf::tolerance(0.00001))
{
	euc::EuclideanPoint<double, 3> p1(0.1, 0.38, 0.7);
	euc::EuclideanPoint<double, 3> p2(0.2, 0.38, 0.7);
	euc::EuclideanPoint<double, 3> p3(0.0833, 0.16, 0.78);
	euc::EuclideanPoint<double, 3> p4(0.01672, 0.16, 0.78);

	Quadrilateral3D<double> shape(p1,p2,p3,p4);
	euc::EuclideanPoint<double, 3> centroid = shape.getCentroid();
	bool isPointInside = shape.isPointInside(centroid);
	BOOST_CHECK_EQUAL(isPointInside, true);
}

// Test 10: Test arbitary point is inside
BOOST_AUTO_TEST_CASE(isPointInside_test10, * utf::tolerance(0.00001))
{
	euc::EuclideanPoint<double, 3> p1(0.1, 0.38, 0.7);
	euc::EuclideanPoint<double, 3> p2(0.2, 0.38, 0.7);
	euc::EuclideanPoint<double, 3> p3(0.0833, 0.16, 0.78);
	euc::EuclideanPoint<double, 3> p4(0.01672, 0.16, 0.78);

	// 20% along bimedian line from cd to ab
	euc::EuclideanPoint<double, 3> point(0.070008, 0.204, 0.764);
	Quadrilateral3D<double> shape(p1, p2, p3, p4);
	bool isPointInside = shape.isPointInside(point);
	BOOST_CHECK_EQUAL(isPointInside, true);
}

// Test 11: Test arbitary point should be found outside
BOOST_AUTO_TEST_CASE(isPointInside_test11, * utf::tolerance(0.00001))
{
	euc::EuclideanPoint<double, 3> p1(0.1, 0.38, 0.7);
	euc::EuclideanPoint<double, 3> p2(0.2, 0.38, 0.7);
	euc::EuclideanPoint<double, 3> p3(0.0833, 0.16, 0.78);
	euc::EuclideanPoint<double, 3> p4(0.01672, 0.16, 0.78);

	// 20% along bimedian line from cd to ab
	euc::EuclideanPoint<double, 3> point(-100.0, -100.0, -100.0);
	Quadrilateral3D<double> shape(p1, p2, p3, p4);
	bool isPointInside = shape.isPointInside(point);
	BOOST_CHECK_EQUAL(isPointInside, false);
}

// Test 12: Test point directly above on z plane is found outside
BOOST_AUTO_TEST_CASE(isPointInside_test12, * utf::tolerance(0.00001))
{
	euc::EuclideanPoint<double, 3> p1(0.1, 0.38, 0.7);
	euc::EuclideanPoint<double, 3> p2(0.2, 0.38, 0.7);
	euc::EuclideanPoint<double, 3> p3(0.0833, 0.16, 0.78);
	euc::EuclideanPoint<double, 3> p4(0.01672, 0.16, 0.78);

	euc::EuclideanPoint<double, 3> point(0.0833, 0.16, 0.82);
	Quadrilateral3D<double> shape(p1, p2, p3, p4);
	bool isPointInside = shape.isPointInside(point);
	BOOST_CHECK_EQUAL(isPointInside, false);
}

// Test 13: Test point directly below on z plane is found outside
BOOST_AUTO_TEST_CASE(isPointInside_test13, * utf::tolerance(0.00001))
{
	euc::EuclideanPoint<double, 3> p1(0.1, 0.38, 0.7);
	euc::EuclideanPoint<double, 3> p2(0.2, 0.38, 0.7);
	euc::EuclideanPoint<double, 3> p3(0.0833, 0.16, 0.78);
	euc::EuclideanPoint<double, 3> p4(0.01672, 0.16, 0.78);

	euc::EuclideanPoint<double, 3> point(0.0833, 0.16, 0.7);
	Quadrilateral3D<double> shape(p1, p2, p3, p4);
	bool isPointInside = shape.isPointInside(point);
	BOOST_CHECK_EQUAL(isPointInside, false);
}

// === getArea ===
// Test 1: Compute the area correctly in a 3D space
BOOST_AUTO_TEST_CASE(getArea_test1, * utf::tolerance(0.00001))
{
	euc::EuclideanPoint<double, 3> p1(0.1, 0.34, 0.7);
	euc::EuclideanPoint<double, 3> p2(0.2, 0.34, 0.7);
	euc::EuclideanPoint<double, 3> p3(0.2, 0.16, 0.78);
	euc::EuclideanPoint<double, 3> p4(0.1, 0.16, 0.78);

	Quadrilateral3D<double> shape(p1, p2, p3, p4);

	double area = shape.getArea();
	BOOST_TEST(area == 0.0196977);
}

// === getNormal ===
// Test 1: Compute the correct normal in one direction
BOOST_AUTO_TEST_CASE(getNormal_test1, * utf::tolerance(0.00001))
{
	euc::EuclideanPoint<double, 3> p1(0.1, 0.34, 0.7);
	euc::EuclideanPoint<double, 3> p2(0.2, 0.34, 0.7);
	euc::EuclideanPoint<double, 3> p3(0.2, 0.16, 0.78);
	euc::EuclideanPoint<double, 3> p4(0.1, 0.16, 0.78);

	Quadrilateral3D<double> shape(p1, p2, p3, p4);

	euc::EuclideanVector3D<double> normal = shape.getNormal();
	BOOST_TEST(normal.cmp[0] == 0.0);
	BOOST_TEST(normal.cmp[1] == -0.008);
	BOOST_TEST(normal.cmp[2] == -0.018);
}

// Test 2: Compute the correct normal when order of vertices is reversed
BOOST_AUTO_TEST_CASE(getNormal_test2, * utf::tolerance(0.00001))
{
	euc::EuclideanPoint<double, 3> p1(0.1, 0.34, 0.7);
	euc::EuclideanPoint<double, 3> p2(0.2, 0.34, 0.7);
	euc::EuclideanPoint<double, 3> p3(0.2, 0.16, 0.78);
	euc::EuclideanPoint<double, 3> p4(0.1, 0.16, 0.78);

	Quadrilateral3D<double> shape(p4, p3, p2, p1);

	euc::EuclideanVector3D<double> normal = shape.getNormal();
	BOOST_TEST(normal.cmp[0] == 0.0);
	BOOST_TEST(normal.cmp[1] == 0.008);
	BOOST_TEST(normal.cmp[2] == 0.018);
}
