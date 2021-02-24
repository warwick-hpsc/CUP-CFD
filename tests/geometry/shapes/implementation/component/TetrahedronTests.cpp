/*
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Unit Tests for the methods of the Tetrahedron class
 */


#define BOOST_TEST_MODULE Tetrahedron
#include <boost/test/unit_test.hpp>
#include <boost/test/output_test_stream.hpp>
#include <stdexcept>

#include "Tetrahedron.h"
#include "EuclideanPoint.h"
#include "Triangle3D.h"
#include "Pyramid.h"

using namespace cupcfd::geometry::shapes;
namespace euc = cupcfd::geometry::euclidean;
namespace utf = boost::unit_test;

// === Constructor ===
// Test 1: Create a Tetrahedron
BOOST_AUTO_TEST_CASE(constructor_test1, * utf::tolerance(0.00001))
{
	euc::EuclideanPoint<double, 3> f(2.5, 5.0, 0.0);
	euc::EuclideanPoint<double, 3> bl(0.0, 0.0, 0.0);
	euc::EuclideanPoint<double, 3> br(5.0, 0.0, 0.0);
	euc::EuclideanPoint<double, 3> apex(2.5, 2.5, 7.0);

	Triangle3D<double> base(f, br, bl);
	Tetrahedron<double> shape(apex, base);

	BOOST_CHECK_EQUAL(shape.apex.cmp[0], 2.5);
	BOOST_CHECK_EQUAL(shape.apex.cmp[1], 2.5);
	BOOST_CHECK_EQUAL(shape.apex.cmp[2], 7.0);

	// Stored in anti-clockwise order from outside/clockwise from inside
	BOOST_CHECK_EQUAL(shape.base.vertices[0].cmp[0], 2.5);
	BOOST_CHECK_EQUAL(shape.base.vertices[0].cmp[1], 5.0);
	BOOST_CHECK_EQUAL(shape.base.vertices[0].cmp[2], 0.0);

	BOOST_CHECK_EQUAL(shape.base.vertices[1].cmp[0], 5.0);
	BOOST_CHECK_EQUAL(shape.base.vertices[1].cmp[1], 0.0);
	BOOST_CHECK_EQUAL(shape.base.vertices[1].cmp[2], 0.0);

	BOOST_CHECK_EQUAL(shape.base.vertices[2].cmp[0], 0.0);
	BOOST_CHECK_EQUAL(shape.base.vertices[2].cmp[1], 0.0);
	BOOST_CHECK_EQUAL(shape.base.vertices[2].cmp[2], 0.0);
}

// === isPointInside ===
// Test 1: Point is inside Polygon
BOOST_AUTO_TEST_CASE(isPointInside_test1, * utf::tolerance(0.00001))
{
	euc::EuclideanPoint<double, 3> f(2.5, 5.0, 0.0);
	euc::EuclideanPoint<double, 3> bl(0.0, 0.0, 0.0);
	euc::EuclideanPoint<double, 3> br(5.0, 0.0, 0.0);
	euc::EuclideanPoint<double, 3> apex(2.5, 2.5, 7.0);

	Triangle3D<double> base(f, br, bl);
	Tetrahedron<double> shape(apex, base);
	

	euc::EuclideanPoint<double,3> point(2.5, 2.5, 1.78);

	bool isInside = shape.isPointInside(point);

	BOOST_CHECK_EQUAL(isInside, true);
}

// Test 2: Point is outside Polygon
BOOST_AUTO_TEST_CASE(isPointInside_test2, * utf::tolerance(0.00001))
{
	euc::EuclideanPoint<double, 3> f(2.5, 5.0, 0.0);
	euc::EuclideanPoint<double, 3> bl(0.0, 0.0, 0.0);
	euc::EuclideanPoint<double, 3> br(5.0, 0.0, 0.0);
	euc::EuclideanPoint<double, 3> apex(2.5, 2.5, 7.0);

	Triangle3D<double> base(f, br, bl);
	Tetrahedron<double> shape(apex, base);
	

	euc::EuclideanPoint<double,3> point(2.5, 2.5, 18.6);

	bool isInside = shape.isPointInside(point);

	BOOST_CHECK_EQUAL(isInside, false);
}

// Test 3: Point is on Base Point 0
BOOST_AUTO_TEST_CASE(isPointInside_test3, * utf::tolerance(0.00001))
{
	euc::EuclideanPoint<double, 3> f(2.5, 5.0, 0.0);
	euc::EuclideanPoint<double, 3> bl(0.0, 0.0, 0.0);
	euc::EuclideanPoint<double, 3> br(5.0, 0.0, 0.0);
	euc::EuclideanPoint<double, 3> apex(2.5, 2.5, 7.0);

	Triangle3D<double> base(f, br, bl);
	Tetrahedron<double> shape(apex, base);
	

	bool isInside = shape.isPointInside(f);

	BOOST_CHECK_EQUAL(isInside, true);
}

// Test 4: Point is on Base Point 1
BOOST_AUTO_TEST_CASE(isPointInside_test4, * utf::tolerance(0.00001))
{
	euc::EuclideanPoint<double, 3> f(2.5, 5.0, 0.0);
	euc::EuclideanPoint<double, 3> bl(0.0, 0.0, 0.0);
	euc::EuclideanPoint<double, 3> br(5.0, 0.0, 0.0);
	euc::EuclideanPoint<double, 3> apex(2.5, 2.5, 7.0);

	Triangle3D<double> base(f, br, bl);
	Tetrahedron<double> shape(apex, base);
	

	bool isInside = shape.isPointInside(bl);

	BOOST_CHECK_EQUAL(isInside, true);
}

// Test 5: Point is on Base Point 2
BOOST_AUTO_TEST_CASE(isPointInside_test5, * utf::tolerance(0.00001))
{
	euc::EuclideanPoint<double, 3> f(2.5, 5.0, 0.0);
	euc::EuclideanPoint<double, 3> bl(0.0, 0.0, 0.0);
	euc::EuclideanPoint<double, 3> br(5.0, 0.0, 0.0);
	euc::EuclideanPoint<double, 3> apex(2.5, 2.5, 7.0);

	Triangle3D<double> base(f, br, bl);
	Tetrahedron<double> shape(apex, base);
	

	bool isInside = shape.isPointInside(br);

	BOOST_CHECK_EQUAL(isInside, true);
}

// Test 6: Point is on Apex
BOOST_AUTO_TEST_CASE(isPointInside_test6, * utf::tolerance(0.00001))
{
	euc::EuclideanPoint<double, 3> f(2.5, 5.0, 0.0);
	euc::EuclideanPoint<double, 3> bl(0.0, 0.0, 0.0);
	euc::EuclideanPoint<double, 3> br(5.0, 0.0, 0.0);
	euc::EuclideanPoint<double, 3> apex(2.5, 2.5, 7.0);

	Triangle3D<double> base(f, br, bl);
	Tetrahedron<double> shape(apex, base);
	

	bool isInside = shape.isPointInside(apex);

	BOOST_CHECK_EQUAL(isInside, true);
}

// Test 7: Point is on Base Plane
BOOST_AUTO_TEST_CASE(isPointInside_test7, * utf::tolerance(0.00001))
{
	euc::EuclideanPoint<double, 3> f(2.5, 5.0, 0.0);
	euc::EuclideanPoint<double, 3> bl(0.0, 0.0, 0.0);
	euc::EuclideanPoint<double, 3> br(5.0, 0.0, 0.0);
	euc::EuclideanPoint<double, 3> apex(2.5, 2.5, 7.0);

	Triangle3D<double> base(f, br, bl);
	Tetrahedron<double> shape(apex, base);
	

	euc::EuclideanPoint<double,3> point(2.5,2.5,0.0);

	bool isInside = shape.isPointInside(point);

	BOOST_CHECK_EQUAL(isInside, true);
}

// Test 8: Point is on Triangular Face 0
BOOST_AUTO_TEST_CASE(isPointInside_test8, * utf::tolerance(0.00001))
{
	euc::EuclideanPoint<double, 3> f(2.5, 5.0, 0.0);
	euc::EuclideanPoint<double, 3> bl(0.0, 0.0, 0.0);
	euc::EuclideanPoint<double, 3> br(5.0, 0.0, 0.0);
	euc::EuclideanPoint<double, 3> apex(2.5, 2.5, 7.0);

	Triangle3D<double> base(f, br, bl);
	Tetrahedron<double> shape(apex, base);
	

	// Test Face apex, f, br
	euc::EuclideanPoint<double,3> point(3.125, 2.5, 3.5);

	bool isInside = shape.isPointInside(point);

	BOOST_CHECK_EQUAL(isInside, true);
}

// Test 9: Point is on Triangular Face 1
BOOST_AUTO_TEST_CASE(isPointInside_test9, * utf::tolerance(0.00001))
{
	euc::EuclideanPoint<double, 3> f(2.5, 5.0, 0.0);
	euc::EuclideanPoint<double, 3> bl(0.0, 0.0, 0.0);
	euc::EuclideanPoint<double, 3> br(5.0, 0.0, 0.0);
	euc::EuclideanPoint<double, 3> apex(2.5, 2.5, 7.0);

	Triangle3D<double> base(f, br, bl);
	Tetrahedron<double> shape(apex, base);
	

	// Test Face apex, f, bl
	euc::EuclideanPoint<double,3> point(1.875, 2.5, 3.5);

	bool isInside = shape.isPointInside(point);

	BOOST_CHECK_EQUAL(isInside, true);
}

// Test 10: Point is on Triangular Face 2
BOOST_AUTO_TEST_CASE(isPointInside_test10, * utf::tolerance(0.00001))
{
	euc::EuclideanPoint<double, 3> f(2.5, 5.0, 0.0);
	euc::EuclideanPoint<double, 3> bl(0.0, 0.0, 0.0);
	euc::EuclideanPoint<double, 3> br(5.0, 0.0, 0.0);
	euc::EuclideanPoint<double, 3> apex(2.5, 2.5, 7.0);

	Triangle3D<double> base(f, br, bl);
	Tetrahedron<double> shape(apex, base);
	

	// Test Face apex, bl, br
	euc::EuclideanPoint<double,3> point(2.5, 1.25, 3.5);

	bool isInside = shape.isPointInside(point);

	BOOST_CHECK_EQUAL(isInside, true);
}

// === getVolume ===
// Test 1: Compute Volume of Tetrahedron
BOOST_AUTO_TEST_CASE(getVolume_test1, * utf::tolerance(0.00001))
{
	euc::EuclideanPoint<double, 3> f(2.5, 5.0, 0.0);
	euc::EuclideanPoint<double, 3> bl(0.0, 0.0, 0.0);
	euc::EuclideanPoint<double, 3> br(5.0, 0.0, 0.0);
	euc::EuclideanPoint<double, 3> apex(2.5, 2.5, 7.0);

	Triangle3D<double> base(f, br, bl);
	Tetrahedron<double> shape(apex, base);
	
	double volume = shape.getVolume();

	BOOST_TEST(volume == 29.166667);
}

// Test 2: Compute Volume of Oblique Tetrahedron
BOOST_AUTO_TEST_CASE(getVolume_test2, * utf::tolerance(0.00001))
{
	euc::EuclideanPoint<double, 3> f(2.5, 5.0, 0.0);
	euc::EuclideanPoint<double, 3> bl(0.0, 0.0, 0.0);
	euc::EuclideanPoint<double, 3> br(5.0, 0.0, 0.0);
	euc::EuclideanPoint<double, 3> apex(1.4, 3.2, 6.4);

	Triangle3D<double> base(f, br, bl);
	Tetrahedron<double> shape(apex, base);
	

	double volume = shape.getVolume();

	BOOST_TEST(volume == 26.666667);
}

// === getCentroid ===
// Test 1: Compute Centroid of Pyramid
BOOST_AUTO_TEST_CASE(getCentroid_test1, * utf::tolerance(0.00001))
{
	euc::EuclideanPoint<double, 3> f(2.5, 5.0, 0.0);
	euc::EuclideanPoint<double, 3> bl(0.0, 0.0, 0.0);
	euc::EuclideanPoint<double, 3> br(5.0, 0.0, 0.0);
	euc::EuclideanPoint<double, 3> apex(2.5, 2.5, 7.0);

	Triangle3D<double> base(f, br, bl);
	Tetrahedron<double> shape(apex, base);
	
	euc::EuclideanPoint<double, 3> centroid = shape.getCentroid();

	BOOST_TEST(centroid.cmp[0] == 2.5);
	BOOST_TEST(centroid.cmp[1] == 1.875000);
	BOOST_TEST(centroid.cmp[2] == 1.75);
}

// Test 2: Compute Centroid of Oblique Pyramid
BOOST_AUTO_TEST_CASE(getCentroid_test2, * utf::tolerance(0.00001))
{
	euc::EuclideanPoint<double, 3> f(2.5, 5.0, 0.0);
	euc::EuclideanPoint<double, 3> bl(0.0, 0.0, 0.0);
	euc::EuclideanPoint<double, 3> br(5.0, 0.0, 0.0);
	euc::EuclideanPoint<double, 3> apex(1.4, 3.2, 6.4);

	Triangle3D<double> base(f, br, bl);
	Tetrahedron<double> shape(apex, base);
	
	euc::EuclideanPoint<double, 3> centroid = shape.getCentroid();

	BOOST_TEST(centroid.cmp[0] == 2.225);
	BOOST_TEST(centroid.cmp[1] == 2.05000);
	BOOST_TEST(centroid.cmp[2] == 1.6);
}

// === isPointOnEdge ===
// Test 1: Test point that is not on an edge
BOOST_AUTO_TEST_CASE(isPointOnEdge_test1, * utf::tolerance(0.00001))
{
	euc::EuclideanPoint<double, 3> f(2.5, 5.0, 0.0);
	euc::EuclideanPoint<double, 3> bl(0.0, 0.0, 0.0);
	euc::EuclideanPoint<double, 3> br(5.0, 0.0, 0.0);
	euc::EuclideanPoint<double, 3> apex(2.5, 2.5, 7.0);

	Triangle3D<double> base(f, br, bl);
	Tetrahedron<double> shape(apex, base);
	
	euc::EuclideanPoint<double, 3> point(1.2, 1.2, 0.6);

	bool onEdge = shape.isPointOnEdge(point);
	BOOST_CHECK_EQUAL(onEdge, false);
}

// Test 2: Test point that is on an edge
BOOST_AUTO_TEST_CASE(isPointOnEdge_test2, * utf::tolerance(0.00001))
{
	euc::EuclideanPoint<double, 3> f(2.5, 5.0, 0.0);
	euc::EuclideanPoint<double, 3> bl(0.0, 0.0, 0.0);
	euc::EuclideanPoint<double, 3> br(5.0, 0.0, 0.0);
	euc::EuclideanPoint<double, 3> apex(2.5, 2.5, 7.0);

	Triangle3D<double> base(f, br, bl);
	Tetrahedron<double> shape(apex, base);
	
	euc::EuclideanPoint<double, 3> point(1.25, 1.25, 3.5);

	bool onEdge = shape.isPointOnEdge(point);
	BOOST_CHECK_EQUAL(onEdge, true);
}


// === isPointOnVertex ===
// Test 1: Test point that is not on a vertex
BOOST_AUTO_TEST_CASE(isPointOnVertex_test1, * utf::tolerance(0.00001))
{
	euc::EuclideanPoint<double, 3> f(2.5, 5.0, 0.0);
	euc::EuclideanPoint<double, 3> bl(0.0, 0.0, 0.0);
	euc::EuclideanPoint<double, 3> br(5.0, 0.0, 0.0);
	euc::EuclideanPoint<double, 3> apex(2.5, 2.5, 7.0);

	Triangle3D<double> base(f, br, bl);
	Tetrahedron<double> shape(apex, base);
	
	euc::EuclideanPoint<double, 3> point(1.25, 1.25, 3.5);

	bool onEdge = shape.isPointOnVertex(point);
	BOOST_CHECK_EQUAL(onEdge, false);
}


// Test 2: Test point that is on a vertex
BOOST_AUTO_TEST_CASE(isPointOnVertex_test2, * utf::tolerance(0.00001))
{
	euc::EuclideanPoint<double, 3> f(2.5, 5.0, 0.0);
	euc::EuclideanPoint<double, 3> bl(0.0, 0.0, 0.0);
	euc::EuclideanPoint<double, 3> br(5.0, 0.0, 0.0);
	euc::EuclideanPoint<double, 3> apex(2.5, 2.5, 7.0);

	Triangle3D<double> base(f, br, bl);
	Tetrahedron<double> shape(apex, base);
	
	euc::EuclideanPoint<double, 3> point(2.5, 2.5, 7.0);

	bool onEdge = shape.isPointOnVertex(point);
	BOOST_CHECK_EQUAL(onEdge, true);
}

