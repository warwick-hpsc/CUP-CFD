/*
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Unit Tests for the methods of the QuadPyramid class
 */


#define BOOST_TEST_MODULE QuadPyramid
#include <boost/test/unit_test.hpp>
#include <boost/test/output_test_stream.hpp>
#include <stdexcept>

#include "QuadPyramid.h"
#include "EuclideanPoint.h"

using namespace cupcfd::geometry::shapes;
namespace euc = cupcfd::geometry::euclidean;
namespace utf = boost::unit_test;

// === Constructor ===
// Test 1: Create a QuadPyramid
BOOST_AUTO_TEST_CASE(constructor_test1, * utf::tolerance(0.00001))
{
	euc::EuclideanPoint<double, 3> p1(0.0, 0.0, 0.0);
	euc::EuclideanPoint<double, 3> p2(0.0, 10.0, 0.0);
	euc::EuclideanPoint<double, 3> p3(10.0, 10.0, 0.0);
	euc::EuclideanPoint<double, 3> p4(10.0, 0.0, 0.0);
	euc::EuclideanPoint<double, 3> apex(5.0, 5.0, 5.0);
	QuadPyramid<double> shape(apex, p2, p3, p1, p4);

	BOOST_CHECK_EQUAL(shape.apex.cmp[0], 5.0);
	BOOST_CHECK_EQUAL(shape.apex.cmp[1], 5.0);
	BOOST_CHECK_EQUAL(shape.apex.cmp[2], 5.0);

	// The base will be ordered in anticlockwise order from outside/clockwise from inside
	BOOST_CHECK_EQUAL(shape.base.vertices[0].cmp[0], 0.0);
	BOOST_CHECK_EQUAL(shape.base.vertices[0].cmp[1], 10.0);
	BOOST_CHECK_EQUAL(shape.base.vertices[0].cmp[2], 0.0);

	BOOST_CHECK_EQUAL(shape.base.vertices[1].cmp[0], 10.0);
	BOOST_CHECK_EQUAL(shape.base.vertices[1].cmp[1], 10.0);
	BOOST_CHECK_EQUAL(shape.base.vertices[1].cmp[2], 0.0);

	BOOST_CHECK_EQUAL(shape.base.vertices[2].cmp[0], 10.0);
	BOOST_CHECK_EQUAL(shape.base.vertices[2].cmp[1], 0.0);
	BOOST_CHECK_EQUAL(shape.base.vertices[2].cmp[2], 0.0);

	BOOST_CHECK_EQUAL(shape.base.vertices[3].cmp[0], 0.0);
	BOOST_CHECK_EQUAL(shape.base.vertices[3].cmp[1], 0.0);
	BOOST_CHECK_EQUAL(shape.base.vertices[3].cmp[2], 0.0);
}

// === isPointInside ===
// Test 1: Point inside Polyhedron

BOOST_AUTO_TEST_CASE(isPointInside_test1, * utf::tolerance(0.00001))
{
	euc::EuclideanPoint<double, 3> p1(0.0, 0.0, 0.0);
	euc::EuclideanPoint<double, 3> p2(0.0, 10.0, 0.0);
	euc::EuclideanPoint<double, 3> p3(10.0, 10.0, 0.0);
	euc::EuclideanPoint<double, 3> p4(10.0, 0.0, 0.0);
	euc::EuclideanPoint<double, 3> apex(5.0, 5.0, 5.0);
	QuadPyramid<double> shape(apex, p2, p3, p1, p4);

	euc::EuclideanPoint<double,3> point(5.0, 5.0, 1.2);
	bool isInside = shape.isPointInside(point);
	BOOST_CHECK_EQUAL(isInside, true);
}

// Test 2: Point is outside Polyhedron
BOOST_AUTO_TEST_CASE(isPointInside_test2, * utf::tolerance(0.00001))
{
	euc::EuclideanPoint<double, 3> p1(0.0, 0.0, 0.0);
	euc::EuclideanPoint<double, 3> p2(0.0, 10.0, 0.0);
	euc::EuclideanPoint<double, 3> p3(10.0, 10.0, 0.0);
	euc::EuclideanPoint<double, 3> p4(10.0, 0.0, 0.0);
	euc::EuclideanPoint<double, 3> apex(5.0, 5.0, 5.0);
	QuadPyramid<double> shape(apex, p2, p3, p1, p4);

	euc::EuclideanPoint<double,3> point(3.78, 11.7, 7.45);
	bool isInside = shape.isPointInside(point);
	BOOST_CHECK_EQUAL(isInside, false);
}


// Test 3: Point is on Base Point 0
BOOST_AUTO_TEST_CASE(isPointInside_test3, * utf::tolerance(0.00001))
{
	euc::EuclideanPoint<double, 3> p1(0.0, 0.0, 0.0);
	euc::EuclideanPoint<double, 3> p2(0.0, 10.0, 0.0);
	euc::EuclideanPoint<double, 3> p3(10.0, 10.0, 0.0);
	euc::EuclideanPoint<double, 3> p4(10.0, 0.0, 0.0);
	euc::EuclideanPoint<double, 3> apex(5.0, 5.0, 5.0);
	QuadPyramid<double> shape(apex, p2, p3, p1, p4);

	bool isInside = shape.isPointInside(p1);
	BOOST_CHECK_EQUAL(isInside, true);
}

// Test 4: Point is on Base Point 1
BOOST_AUTO_TEST_CASE(isPointInside_test4, * utf::tolerance(0.00001))
{
	euc::EuclideanPoint<double, 3> p1(0.0, 0.0, 0.0);
	euc::EuclideanPoint<double, 3> p2(0.0, 10.0, 0.0);
	euc::EuclideanPoint<double, 3> p3(10.0, 10.0, 0.0);
	euc::EuclideanPoint<double, 3> p4(10.0, 0.0, 0.0);
	euc::EuclideanPoint<double, 3> apex(5.0, 5.0, 5.0);
	QuadPyramid<double> shape(apex, p2, p3, p1, p4);

	bool isInside = shape.isPointInside(p2);
	BOOST_CHECK_EQUAL(isInside, true);
}

// Test 5: Point is on Base Point 2
BOOST_AUTO_TEST_CASE(isPointInside_test5, * utf::tolerance(0.00001))
{
	euc::EuclideanPoint<double, 3> p1(0.0, 0.0, 0.0);
	euc::EuclideanPoint<double, 3> p2(0.0, 10.0, 0.0);
	euc::EuclideanPoint<double, 3> p3(10.0, 10.0, 0.0);
	euc::EuclideanPoint<double, 3> p4(10.0, 0.0, 0.0);
	euc::EuclideanPoint<double, 3> apex(5.0, 5.0, 5.0);
	QuadPyramid<double> shape(apex, p2, p3, p1, p4);

	bool isInside = shape.isPointInside(p3);
	BOOST_CHECK_EQUAL(isInside, true);
}

// Test 6: Point is on Base Point 3
BOOST_AUTO_TEST_CASE(isPointInside_test6, * utf::tolerance(0.00001))
{
	euc::EuclideanPoint<double, 3> p1(0.0, 0.0, 0.0);
	euc::EuclideanPoint<double, 3> p2(0.0, 10.0, 0.0);
	euc::EuclideanPoint<double, 3> p3(10.0, 10.0, 0.0);
	euc::EuclideanPoint<double, 3> p4(10.0, 0.0, 0.0);
	euc::EuclideanPoint<double, 3> apex(5.0, 5.0, 5.0);
	QuadPyramid<double> shape(apex, p2, p3, p1, p4);

	bool isInside = shape.isPointInside(p4);
	BOOST_CHECK_EQUAL(isInside, true);
}


// Test 7: Point is on Apex
BOOST_AUTO_TEST_CASE(isPointInside_test7, * utf::tolerance(0.00001))
{
	euc::EuclideanPoint<double, 3> p1(0.0, 0.0, 0.0);
	euc::EuclideanPoint<double, 3> p2(0.0, 10.0, 0.0);
	euc::EuclideanPoint<double, 3> p3(10.0, 10.0, 0.0);
	euc::EuclideanPoint<double, 3> p4(10.0, 0.0, 0.0);
	euc::EuclideanPoint<double, 3> apex(5.0, 5.0, 5.0);
	QuadPyramid<double> shape(apex, p2, p3, p1, p4);

	bool isInside = shape.isPointInside(apex);
	BOOST_CHECK_EQUAL(isInside, true);
}

// Test 8: Point is on Base Plane
BOOST_AUTO_TEST_CASE(isPointInside_test8, * utf::tolerance(0.00001))
{
	euc::EuclideanPoint<double, 3> p1(0.0, 0.0, 0.0);
	euc::EuclideanPoint<double, 3> p2(0.0, 10.0, 0.0);
	euc::EuclideanPoint<double, 3> p3(10.0, 10.0, 0.0);
	euc::EuclideanPoint<double, 3> p4(10.0, 0.0, 0.0);
	euc::EuclideanPoint<double, 3> apex(5.0, 5.0, 5.0);
	QuadPyramid<double> shape(apex, p2, p3, p1, p4);

	euc::EuclideanPoint<double,3> point(7.6, 4.5, 0.0);
	bool isInside = shape.isPointInside(point);
	BOOST_CHECK_EQUAL(isInside, true);
}

// Test 9: Point is on Triangular Face 0
BOOST_AUTO_TEST_CASE(isPointInside_test9, * utf::tolerance(0.00001))
{
	euc::EuclideanPoint<double, 3> p1(0.0, 0.0, 0.0);
	euc::EuclideanPoint<double, 3> p2(0.0, 10.0, 0.0);
	euc::EuclideanPoint<double, 3> p3(10.0, 10.0, 0.0);
	euc::EuclideanPoint<double, 3> p4(10.0, 0.0, 0.0);
	euc::EuclideanPoint<double, 3> apex(5.0, 5.0, 5.0);
	QuadPyramid<double> shape(apex, p2, p3, p1, p4);


	euc::EuclideanPoint<double,3> point(5.0, 2.5, 2.5);
	bool isInside = shape.isPointInside(point);
	BOOST_CHECK_EQUAL(isInside, true);
}

// Test 10: Point is on Triangular Face 1
BOOST_AUTO_TEST_CASE(isPointInside_test10, * utf::tolerance(0.00001))
{
	euc::EuclideanPoint<double, 3> p1(0.0, 0.0, 0.0);
	euc::EuclideanPoint<double, 3> p2(0.0, 10.0, 0.0);
	euc::EuclideanPoint<double, 3> p3(10.0, 10.0, 0.0);
	euc::EuclideanPoint<double, 3> p4(10.0, 0.0, 0.0);
	euc::EuclideanPoint<double, 3> apex(5.0, 5.0, 5.0);
	QuadPyramid<double> shape(apex, p2, p3, p1, p4);


	euc::EuclideanPoint<double,3> point(5.0, 7.5, 2.5);
	bool isInside = shape.isPointInside(point);
	BOOST_CHECK_EQUAL(isInside, true);
}

// Test 11: Point is on Triangular Face 2
BOOST_AUTO_TEST_CASE(isPointInside_test11, * utf::tolerance(0.00001))
{
	euc::EuclideanPoint<double, 3> p1(0.0, 0.0, 0.0);
	euc::EuclideanPoint<double, 3> p2(0.0, 10.0, 0.0);
	euc::EuclideanPoint<double, 3> p3(10.0, 10.0, 0.0);
	euc::EuclideanPoint<double, 3> p4(10.0, 0.0, 0.0);
	euc::EuclideanPoint<double, 3> apex(5.0, 5.0, 5.0);
	QuadPyramid<double> shape(apex, p2, p3, p1, p4);


	euc::EuclideanPoint<double,3> point(2.5, 5.0, 2.5);
	bool isInside = shape.isPointInside(point);
	BOOST_CHECK_EQUAL(isInside, true);
}

// Test 12: Point is on Triangular Face 3
BOOST_AUTO_TEST_CASE(isPointInside_test12, * utf::tolerance(0.00001))
{
	euc::EuclideanPoint<double, 3> p1(0.0, 0.0, 0.0);
	euc::EuclideanPoint<double, 3> p2(0.0, 10.0, 0.0);
	euc::EuclideanPoint<double, 3> p3(10.0, 10.0, 0.0);
	euc::EuclideanPoint<double, 3> p4(10.0, 0.0, 0.0);
	euc::EuclideanPoint<double, 3> apex(5.0, 5.0, 5.0);
	QuadPyramid<double> shape(apex, p2, p3, p1, p4);

	euc::EuclideanPoint<double,3> point(7.5, 5.0, 2.5);
	bool isInside = shape.isPointInside(point);
	BOOST_CHECK_EQUAL(isInside, true);
}

// === computeVolume ===
// Test 1: Compute Volume of Pyramid
BOOST_AUTO_TEST_CASE(computeVolume_test1, * utf::tolerance(0.00001))
{
	euc::EuclideanPoint<double, 3> p1(0.0, 0.0, 0.0);
	euc::EuclideanPoint<double, 3> p2(0.0, 10.0, 0.0);
	euc::EuclideanPoint<double, 3> p3(10.0, 10.0, 0.0);
	euc::EuclideanPoint<double, 3> p4(10.0, 0.0, 0.0);
	euc::EuclideanPoint<double, 3> apex(5.0, 5.0, 5.0);

	QuadPyramid<double> shape(apex, p2, p1, p4, p3);
	double volume = shape.computeVolume();
	BOOST_TEST(volume == 166.6666666667);
}

// Test 2: Compute Volume of Oblique Pyramid
BOOST_AUTO_TEST_CASE(computeVolume_test2, * utf::tolerance(0.00001))
{
	euc::EuclideanPoint<double, 3> p1(0.0, 0.0, 0.0);
	euc::EuclideanPoint<double, 3> p2(0.0, 10.0, 0.0);
	euc::EuclideanPoint<double, 3> p3(10.0, 10.0, 0.0);
	euc::EuclideanPoint<double, 3> p4(10.0, 0.0, 0.0);
	euc::EuclideanPoint<double, 3> apex(2.7, 3.7, 8.9);

	QuadPyramid<double> shape(apex, p2, p1, p4, p3);
	double volume = shape.computeVolume();
	BOOST_TEST(volume == 296.6666666667);
}

// === computeCentroid ===
// Test 1: Compute Centroid of Pyramid
BOOST_AUTO_TEST_CASE(computeCentroid_test1, * utf::tolerance(0.00001))
{
	euc::EuclideanPoint<double, 3> p1(0.0, 0.0, 0.0);
	euc::EuclideanPoint<double, 3> p2(0.0, 10.0, 0.0);
	euc::EuclideanPoint<double, 3> p3(10.0, 10.0, 0.0);
	euc::EuclideanPoint<double, 3> p4(10.0, 0.0, 0.0);
	euc::EuclideanPoint<double, 3> apex(5.0, 5.0, 5.0);

	QuadPyramid<double> shape(apex, p2, p1, p4, p3);
	euc::EuclideanPoint<double, 3> centroid = shape.computeCentroid();
	BOOST_TEST(centroid.cmp[0] == 5.0);
	BOOST_TEST(centroid.cmp[1] == 5.0);
	BOOST_TEST(centroid.cmp[2] == 1.25);
}

// Test 2: Compute Centroid of Oblique Pyramid
BOOST_AUTO_TEST_CASE(computeCentroid_test2, * utf::tolerance(0.00001))
{
	euc::EuclideanPoint<double, 3> p1(0.0, 0.0, 0.0);
	euc::EuclideanPoint<double, 3> p2(0.0, 10.0, 0.0);
	euc::EuclideanPoint<double, 3> p3(10.0, 10.0, 0.0);
	euc::EuclideanPoint<double, 3> p4(10.0, 0.0, 0.0);
	euc::EuclideanPoint<double, 3> apex(2.7, 3.7, 8.9);

	QuadPyramid<double> shape(apex, p2, p1, p4, p3);
	euc::EuclideanPoint<double, 3> centroid = shape.computeCentroid();
	BOOST_TEST(centroid.cmp[0] == 4.425);
	BOOST_TEST(centroid.cmp[1] == 4.675);
	BOOST_TEST(centroid.cmp[2] == 2.225);
}

BOOST_AUTO_TEST_CASE(computeCentroid_test3, * utf::tolerance(0.00001))
{
	euc::EuclideanPoint<double, 3> p1(0.0, 0.0, 0.0);
	euc::EuclideanPoint<double, 3> p2(0.0, 5.0, 0.0);
	euc::EuclideanPoint<double, 3> p3(5.0, 5.0, 0.0);
	euc::EuclideanPoint<double, 3> p4(5.0, 0.0, 0.0);
	euc::EuclideanPoint<double, 3> apex(2.5, 2.5, 5.0);

	QuadPyramid<double> shape(apex, p2, p1, p4, p3);
	euc::EuclideanPoint<double, 3> centroid = shape.computeCentroid();
	BOOST_TEST(centroid.cmp[0] == 2.5);
	BOOST_TEST(centroid.cmp[1] == 2.5);
	BOOST_TEST(centroid.cmp[2] == 1.25);
}

// === isPointOnEdge ===
// Test 1: Test point that is not on an edge
BOOST_AUTO_TEST_CASE(isPointOnEdge_test1, * utf::tolerance(0.00001))
{
	euc::EuclideanPoint<double, 3> p1(0.0, 0.0, 0.0);
	euc::EuclideanPoint<double, 3> p2(0.0, 5.0, 0.0);
	euc::EuclideanPoint<double, 3> p3(5.0, 5.0, 0.0);
	euc::EuclideanPoint<double, 3> p4(5.0, 0.0, 0.0);
	euc::EuclideanPoint<double, 3> apex(2.5, 2.5, 5.0);

	QuadPyramid<double> shape(apex, p2, p1, p4, p3);
	euc::EuclideanPoint<double, 3> point(5.0, 4.52, 2.0);

	bool onEdge = shape.isPointOnEdge(point);

	BOOST_CHECK_EQUAL(onEdge, false);
}

// Test 2: Test point that is on an edge
BOOST_AUTO_TEST_CASE(isPointOnEdge_test2, * utf::tolerance(0.00001))
{
	euc::EuclideanPoint<double, 3> p1(0.0, 0.0, 0.0);
	euc::EuclideanPoint<double, 3> p2(0.0, 5.0, 0.0);
	euc::EuclideanPoint<double, 3> p3(5.0, 5.0, 0.0);
	euc::EuclideanPoint<double, 3> p4(5.0, 0.0, 0.0);
	euc::EuclideanPoint<double, 3> apex(2.5, 2.5, 5.0);

	QuadPyramid<double> shape(apex, p2, p1, p4, p3);
	euc::EuclideanPoint<double, 3> point(5.0, 4.52, 0.0);

	bool onEdge = shape.isPointOnEdge(point);

	BOOST_CHECK_EQUAL(onEdge, true);
}


// === isPointOnVertex ===
// Test 1: Test point that is not on a vertex
BOOST_AUTO_TEST_CASE(isPointOnVertex_test1, * utf::tolerance(0.00001))
{
	euc::EuclideanPoint<double, 3> p1(0.0, 0.0, 0.0);
	euc::EuclideanPoint<double, 3> p2(0.0, 5.0, 0.0);
	euc::EuclideanPoint<double, 3> p3(5.0, 5.0, 0.0);
	euc::EuclideanPoint<double, 3> p4(5.0, 0.0, 0.0);
	euc::EuclideanPoint<double, 3> apex(2.5, 2.5, 5.0);

	QuadPyramid<double> shape(apex, p2, p1, p4, p3);
	euc::EuclideanPoint<double, 3> point(5.0, 4.52, 0.0);

	bool onEdge = shape.isPointOnVertex(point);

	BOOST_CHECK_EQUAL(onEdge, false);
}


// Test 2: Test point that is on a vertex
BOOST_AUTO_TEST_CASE(isPointOnVertex_test2, * utf::tolerance(0.00001))
{
	euc::EuclideanPoint<double, 3> p1(0.0, 0.0, 0.0);
	euc::EuclideanPoint<double, 3> p2(0.0, 5.0, 0.0);
	euc::EuclideanPoint<double, 3> p3(5.0, 5.0, 0.0);
	euc::EuclideanPoint<double, 3> p4(5.0, 0.0, 0.0);
	euc::EuclideanPoint<double, 3> apex(2.5, 2.5, 5.0);

	QuadPyramid<double> shape(apex, p2, p1, p4, p3);
	euc::EuclideanPoint<double, 3> point(5.0, 5.0, 0.0);

	bool onEdge = shape.isPointOnVertex(point);

	BOOST_CHECK_EQUAL(onEdge, true);
}

