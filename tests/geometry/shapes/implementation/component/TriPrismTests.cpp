/*
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Unit Tests for the methods of the TriPrism class
 */


#define BOOST_TEST_MODULE TriPrism
#include <boost/test/unit_test.hpp>
#include <boost/test/output_test_stream.hpp>
#include <stdexcept>

#include "TriPrism.h"
#include "Triangle3D.h"
#include "EuclideanPoint.h"

using namespace cupcfd::geometry::shapes;
namespace euc = cupcfd::geometry::euclidean;
namespace utf = boost::unit_test;

// === Constructor ===
// Test 1:
BOOST_AUTO_TEST_CASE(constructor_test1, * utf::tolerance(0.00001))
{
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> tf(2.5, 5.0, 12.0);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> tlb(0.0, 0.0, 12.0);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> trb(5.0, 0.0, 12.0);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> bf(2.5, 5.0, 0.0);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> blb(0.0, 0.0, 0.0);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> brb(5.0, 0.0, 0.0);

	// TriPrism<double> shape(tf, tlb, trb, bf, blb, brb);
	Triangle3D<double> triTop(tf, trb, tlb);
	Triangle3D<double> triBottom(bf, blb, brb);
	TriPrism<double> shape(triTop, triBottom);

	BOOST_TEST(shape.top.vertices[0].cmp[0] == 2.5);
	BOOST_TEST(shape.top.vertices[0].cmp[0] == 2.5);
	BOOST_TEST(shape.top.vertices[0].cmp[1] == 5.0);
	BOOST_TEST(shape.top.vertices[0].cmp[2] == 12.0);
	BOOST_TEST(shape.top.vertices[2].cmp[0] == 0.0);
	BOOST_TEST(shape.top.vertices[2].cmp[1] == 0.0);
	BOOST_TEST(shape.top.vertices[2].cmp[2] == 12.0);
	BOOST_TEST(shape.top.vertices[1].cmp[0] == 5.0);
	BOOST_TEST(shape.top.vertices[1].cmp[1] == 0.0);
	BOOST_TEST(shape.top.vertices[1].cmp[2] == 12.0);

	BOOST_TEST(shape.bottom.vertices[0].cmp[0] == 2.5);
	BOOST_TEST(shape.bottom.vertices[0].cmp[1] == 5.0);
	BOOST_TEST(shape.bottom.vertices[0].cmp[2] == 0.0);
	BOOST_TEST(shape.bottom.vertices[1].cmp[0] == 0.0);
	BOOST_TEST(shape.bottom.vertices[1].cmp[1] == 0.0);
	BOOST_TEST(shape.bottom.vertices[1].cmp[2] == 0.0);
	BOOST_TEST(shape.bottom.vertices[2].cmp[0] == 5.0);
	BOOST_TEST(shape.bottom.vertices[2].cmp[1] == 0.0);
	BOOST_TEST(shape.bottom.vertices[2].cmp[2] == 0.0);
}

// === getCentroid ===
// Test 1: Correctly compute centroid
BOOST_AUTO_TEST_CASE(getCentroid_test1, * utf::tolerance(0.00001))
{

}

// === getVolume ===
// Test 1: Correctly compute volume
BOOST_AUTO_TEST_CASE(getVolume_test1, * utf::tolerance(0.00001))
{

}

// Test 2: Correctly compute volume of oblique triprism
BOOST_AUTO_TEST_CASE(getVolume_test2, * utf::tolerance(0.00001))
{

}

// === isPointInside ===
// Test 1: Point Inside TriPrism
BOOST_AUTO_TEST_CASE(isPointInside_test1, * utf::tolerance(0.00001))
{

}

// Test 2: Point Outside TriPrism
BOOST_AUTO_TEST_CASE(isPointInside_test2, * utf::tolerance(0.00001))
{

}

// Test 3: Point On Point a
BOOST_AUTO_TEST_CASE(isPointInside_test3, * utf::tolerance(0.00001))
{

}

// Test 4: Point On Point b
BOOST_AUTO_TEST_CASE(isPointInside_test4, * utf::tolerance(0.00001))
{

}

// Test 5: Point On Point c
BOOST_AUTO_TEST_CASE(isPointInside_test5, * utf::tolerance(0.00001))
{

}

// Test 6: Point On Point d
BOOST_AUTO_TEST_CASE(isPointInside_test6, * utf::tolerance(0.00001))
{

}

// Test 7: Point On Point e
BOOST_AUTO_TEST_CASE(isPointInside_test7, * utf::tolerance(0.00001))
{

}

// Test 8: Point On Point f
BOOST_AUTO_TEST_CASE(isPointInside_test8, * utf::tolerance(0.00001))
{

}

// Test 9: Point On Top Face
BOOST_AUTO_TEST_CASE(isPointInside_test9, * utf::tolerance(0.00001))
{

}

// Test 10: Point On Bottom Face
BOOST_AUTO_TEST_CASE(isPointInside_test10, * utf::tolerance(0.00001))
{

}

// Test 11: Point Side Face 1
BOOST_AUTO_TEST_CASE(isPointInside_test11, * utf::tolerance(0.00001))
{

}

// Test 12: Point Side Face 2
BOOST_AUTO_TEST_CASE(isPointInside_test12, * utf::tolerance(0.00001))
{

}

// Test 13: Point Side Face 3
BOOST_AUTO_TEST_CASE(isPointInside_test13, * utf::tolerance(0.00001))
{

}

// Test 14: Point On Centroid
BOOST_AUTO_TEST_CASE(isPointInside_test14, * utf::tolerance(0.00001))
{

}

// === isPointOnEdge ===
// Test 1: Test point that is not on an edge
BOOST_AUTO_TEST_CASE(isPointOnEdge_test1, * utf::tolerance(0.00001))
{
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> tf(2.5, 5.0, 12.0);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> tlb(0.0, 0.0, 12.0);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> trb(5.0, 0.0, 12.0);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> bf(2.5, 5.0, 0.0);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> blb(0.0, 0.0, 0.0);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> brb(5.0, 0.0, 0.0);

	Triangle3D<double> triTop(tf, trb, tlb);
	Triangle3D<double> triBottom(bf, blb, brb);
	TriPrism<double> shape(triTop, triBottom);
	euc::EuclideanPoint<double, 3> point(1.24, 4.9, 12.0);

	bool onEdge = shape.isPointOnEdge(point);
	BOOST_CHECK_EQUAL(onEdge, false);
}

// Test 2: Test point that is on an edge
BOOST_AUTO_TEST_CASE(isPointOnEdge_test2, * utf::tolerance(0.00001))
{
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> tf(2.5, 5.0, 12.0);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> tlb(0.0, 0.0, 12.0);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> trb(5.0, 0.0, 12.0);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> bf(2.5, 5.0, 0.0);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> blb(0.0, 0.0, 0.0);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> brb(5.0, 0.0, 0.0);

	Triangle3D<double> triTop(tf, trb, tlb);
	Triangle3D<double> triBottom(bf, blb, brb);
	TriPrism<double> shape(triTop, triBottom);
	euc::EuclideanPoint<double, 3> point(4.78, 0.0, 12.0);

	bool onEdge = shape.isPointOnEdge(point);
	BOOST_CHECK_EQUAL(onEdge, true);
}


// === isPointOnVertex ===
// Test 1: Test point that is not on a vertex
BOOST_AUTO_TEST_CASE(isPointOnVertex_test1, * utf::tolerance(0.00001))
{
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> tf(2.5, 5.0, 12.0);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> tlb(0.0, 0.0, 12.0);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> trb(5.0, 0.0, 12.0);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> bf(2.5, 5.0, 0.0);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> blb(0.0, 0.0, 0.0);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> brb(5.0, 0.0, 0.0);

	Triangle3D<double> triTop(tf, trb, tlb);
	Triangle3D<double> triBottom(bf, blb, brb);
	TriPrism<double> shape(triTop, triBottom);
	euc::EuclideanPoint<double, 3> point(4.78, 0.0, 12.0);

	bool onEdge = shape.isPointOnVertex(point);
	BOOST_CHECK_EQUAL(onEdge, false);
}


// Test 2: Test point that is on a vertex
BOOST_AUTO_TEST_CASE(isPointOnVertex_test2, * utf::tolerance(0.00001))
{
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> tf(2.5, 5.0, 12.0);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> tlb(0.0, 0.0, 12.0);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> trb(5.0, 0.0, 12.0);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> bf(2.5, 5.0, 0.0);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> blb(0.0, 0.0, 0.0);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> brb(5.0, 0.0, 0.0);

	Triangle3D<double> triTop(tf, trb, tlb);
	Triangle3D<double> triBottom(bf, blb, brb);
	TriPrism<double> shape(triTop, triBottom);
	euc::EuclideanPoint<double, 3> point(5.0, 0.0, 12.0);

	bool onEdge = shape.isPointOnVertex(point);
	BOOST_CHECK_EQUAL(onEdge, true);
}
