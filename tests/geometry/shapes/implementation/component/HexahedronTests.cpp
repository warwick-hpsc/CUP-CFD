/*
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Unit Tests for the methods of the Hexahedral class
 */


#define BOOST_TEST_MODULE Hexahedron
#include <boost/test/unit_test.hpp>
#include <boost/test/output_test_stream.hpp>
#include <stdexcept>

#include "Hexahedron.h"
#include "EuclideanPoint.h"

using namespace cupcfd::geometry::shapes;
namespace euc = cupcfd::geometry::euclidean;
namespace utf = boost::unit_test;

// === Constructor ===
// Test 1: Construct a regular hexahedron
BOOST_AUTO_TEST_CASE(constructor_test1, * utf::tolerance(0.00001))
{
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> tlf(0.0, 5.0, 12.0);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> trf(5.0, 5.0, 12.0);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> blf(0.0, 5.0, 0.0);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> brf(5.0, 5.0, 0.0);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> tlb(0.0, 0.0, 12.0);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> trb(5.0, 0.0, 12.0);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> blb(0.0, 0.0, 0.0);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> brb(5.0, 0.0, 0.0);

	Hexahedron<double> shape(tlf, trf, blf, brf, tlb, trb, blb, brb);

	BOOST_TEST(shape.tlf.cmp[0] == 0.0);
	BOOST_TEST(shape.tlf.cmp[1] == 5.0);
	BOOST_TEST(shape.tlf.cmp[2] == 12.0);
	BOOST_TEST(shape.trf.cmp[0] == 5.0);
	BOOST_TEST(shape.trf.cmp[1] == 5.0);
	BOOST_TEST(shape.trf.cmp[2] == 12.0);
	BOOST_TEST(shape.blf.cmp[0] == 0.0);
	BOOST_TEST(shape.blf.cmp[1] == 5.0);
	BOOST_TEST(shape.blf.cmp[2] == 0.0);
	BOOST_TEST(shape.brf.cmp[0] == 5.0);
	BOOST_TEST(shape.brf.cmp[1] == 5.0);
	BOOST_TEST(shape.brf.cmp[2] == 0.0);
	BOOST_TEST(shape.tlb.cmp[0] == 0.0);
	BOOST_TEST(shape.tlb.cmp[1] == 0.0);
	BOOST_TEST(shape.tlb.cmp[2] == 12.0);
	BOOST_TEST(shape.trb.cmp[0] == 5.0);
	BOOST_TEST(shape.trb.cmp[1] == 0.0);
	BOOST_TEST(shape.trb.cmp[2] == 12.0);
	BOOST_TEST(shape.blb.cmp[0] == 0.0);
	BOOST_TEST(shape.blb.cmp[1] == 0.0);
	BOOST_TEST(shape.blb.cmp[2] == 0.0);
	BOOST_TEST(shape.brb.cmp[0] == 5.0);
	BOOST_TEST(shape.brb.cmp[1] == 0.0);
	BOOST_TEST(shape.brb.cmp[2] == 0.0);
}

// === isPointInside ===
// Test 1: Point is inside Hexahedron
BOOST_AUTO_TEST_CASE(isPointInside_test1, * utf::tolerance(0.00001))
{
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> tlf(0.0, 5.0, 12.0);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> trf(5.0, 5.0, 12.0);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> blf(0.0, 5.0, 0.0);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> brf(5.0, 5.0, 0.0);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> tlb(0.0, 0.0, 12.0);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> trb(5.0, 0.0, 12.0);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> blb(0.0, 0.0, 0.0);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> brb(5.0, 0.0, 0.0);

	Hexahedron<double> shape(tlf, trf, blf, brf, tlb, trb, blb, brb);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> point(2.3, 4.6, 6.7);

	bool isPointInside = shape.isPointInside(point);
	BOOST_CHECK_EQUAL(isPointInside, true);
}

// Test 2: Point is outside Hexahedron
BOOST_AUTO_TEST_CASE(isPointInside_test2, * utf::tolerance(0.00001))
{
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> tlf(0.0, 5.0, 12.0);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> trf(5.0, 5.0, 12.0);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> blf(0.0, 5.0, 0.0);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> brf(5.0, 5.0, 0.0);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> tlb(0.0, 0.0, 12.0);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> trb(5.0, 0.0, 12.0);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> blb(0.0, 0.0, 0.0);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> brb(5.0, 0.0, 0.0);

	Hexahedron<double> shape(tlf, trf, blf, brf, tlb, trb, blb, brb);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> point(2.3, 4.6, 13.7);

	bool isPointInside = shape.isPointInside(point);
	BOOST_CHECK_EQUAL(isPointInside, false);
}

// Test 3: Point is on Vertex tlf
BOOST_AUTO_TEST_CASE(isPointInside_test3, * utf::tolerance(0.00001))
{
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> tlf(0.0, 5.0, 12.0);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> trf(5.0, 5.0, 12.0);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> blf(0.0, 5.0, 0.0);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> brf(5.0, 5.0, 0.0);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> tlb(0.0, 0.0, 12.0);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> trb(5.0, 0.0, 12.0);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> blb(0.0, 0.0, 0.0);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> brb(5.0, 0.0, 0.0);

	Hexahedron<double> shape(tlf, trf, blf, brf, tlb, trb, blb, brb);
	bool isPointInside = shape.isPointInside(tlf);
	BOOST_CHECK_EQUAL(isPointInside, true);
}

// Test 4: Point is on Vertex trf
BOOST_AUTO_TEST_CASE(isPointInside_test4, * utf::tolerance(0.00001))
{
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> tlf(0.0, 5.0, 12.0);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> trf(5.0, 5.0, 12.0);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> blf(0.0, 5.0, 0.0);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> brf(5.0, 5.0, 0.0);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> tlb(0.0, 0.0, 12.0);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> trb(5.0, 0.0, 12.0);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> blb(0.0, 0.0, 0.0);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> brb(5.0, 0.0, 0.0);

	Hexahedron<double> shape(tlf, trf, blf, brf, tlb, trb, blb, brb);
	bool isPointInside = shape.isPointInside(trf);
	BOOST_CHECK_EQUAL(isPointInside, true);
}

// Test 5: Point is on Vertex blf
BOOST_AUTO_TEST_CASE(isPointInside_test5, * utf::tolerance(0.00001))
{
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> tlf(0.0, 5.0, 12.0);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> trf(5.0, 5.0, 12.0);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> blf(0.0, 5.0, 0.0);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> brf(5.0, 5.0, 0.0);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> tlb(0.0, 0.0, 12.0);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> trb(5.0, 0.0, 12.0);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> blb(0.0, 0.0, 0.0);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> brb(5.0, 0.0, 0.0);

	Hexahedron<double> shape(tlf, trf, blf, brf, tlb, trb, blb, brb);
	bool isPointInside = shape.isPointInside(blf);
	BOOST_CHECK_EQUAL(isPointInside, true);
}

// Test 6: Point is on Vertex brf
BOOST_AUTO_TEST_CASE(isPointInside_test6, * utf::tolerance(0.00001))
{
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> tlf(0.0, 5.0, 12.0);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> trf(5.0, 5.0, 12.0);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> blf(0.0, 5.0, 0.0);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> brf(5.0, 5.0, 0.0);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> tlb(0.0, 0.0, 12.0);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> trb(5.0, 0.0, 12.0);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> blb(0.0, 0.0, 0.0);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> brb(5.0, 0.0, 0.0);

	Hexahedron<double> shape(tlf, trf, blf, brf, tlb, trb, blb, brb);
	bool isPointInside = shape.isPointInside(brf);
	BOOST_CHECK_EQUAL(isPointInside, true);
}

// Test 7: Point is on Vertex tlb
BOOST_AUTO_TEST_CASE(isPointInside_test7, * utf::tolerance(0.00001))
{
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> tlf(0.0, 5.0, 12.0);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> trf(5.0, 5.0, 12.0);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> blf(0.0, 5.0, 0.0);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> brf(5.0, 5.0, 0.0);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> tlb(0.0, 0.0, 12.0);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> trb(5.0, 0.0, 12.0);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> blb(0.0, 0.0, 0.0);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> brb(5.0, 0.0, 0.0);

	Hexahedron<double> shape(tlf, trf, blf, brf, tlb, trb, blb, brb);
	bool isPointInside = shape.isPointInside(tlb);
	BOOST_CHECK_EQUAL(isPointInside, true);
}

// Test 8: Point is on Vertex trb
BOOST_AUTO_TEST_CASE(isPointInside_test8, * utf::tolerance(0.00001))
{
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> tlf(0.0, 5.0, 12.0);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> trf(5.0, 5.0, 12.0);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> blf(0.0, 5.0, 0.0);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> brf(5.0, 5.0, 0.0);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> tlb(0.0, 0.0, 12.0);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> trb(5.0, 0.0, 12.0);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> blb(0.0, 0.0, 0.0);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> brb(5.0, 0.0, 0.0);

	Hexahedron<double> shape(tlf, trf, blf, brf, tlb, trb, blb, brb);
	bool isPointInside = shape.isPointInside(trb);
	BOOST_CHECK_EQUAL(isPointInside, true);
}

// Test 9: Point is on Vertex blb
BOOST_AUTO_TEST_CASE(isPointInside_test9, * utf::tolerance(0.00001))
{
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> tlf(0.0, 5.0, 12.0);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> trf(5.0, 5.0, 12.0);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> blf(0.0, 5.0, 0.0);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> brf(5.0, 5.0, 0.0);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> tlb(0.0, 0.0, 12.0);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> trb(5.0, 0.0, 12.0);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> blb(0.0, 0.0, 0.0);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> brb(5.0, 0.0, 0.0);

	Hexahedron<double> shape(tlf, trf, blf, brf, tlb, trb, blb, brb);
	bool isPointInside = shape.isPointInside(blb);
	BOOST_CHECK_EQUAL(isPointInside, true);
}

// Test 10: Point is on Vertex brb
BOOST_AUTO_TEST_CASE(isPointInside_test10, * utf::tolerance(0.00001))
{
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> tlf(0.0, 5.0, 12.0);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> trf(5.0, 5.0, 12.0);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> blf(0.0, 5.0, 0.0);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> brf(5.0, 5.0, 0.0);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> tlb(0.0, 0.0, 12.0);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> trb(5.0, 0.0, 12.0);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> blb(0.0, 0.0, 0.0);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> brb(5.0, 0.0, 0.0);

	Hexahedron<double> shape(tlf, trf, blf, brf, tlb, trb, blb, brb);
	bool isPointInside = shape.isPointInside(brb);
	BOOST_CHECK_EQUAL(isPointInside, true);
}

// Test 11: Point is on Top Face
BOOST_AUTO_TEST_CASE(isPointInside_test11, * utf::tolerance(0.00001))
{
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> tlf(0.0, 5.0, 12.0);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> trf(5.0, 5.0, 12.0);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> blf(0.0, 5.0, 0.0);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> brf(5.0, 5.0, 0.0);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> tlb(0.0, 0.0, 12.0);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> trb(5.0, 0.0, 12.0);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> blb(0.0, 0.0, 0.0);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> brb(5.0, 0.0, 0.0);

	Hexahedron<double> shape(tlf, trf, blf, brf, tlb, trb, blb, brb);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> point(2.3, 4.6, 12.0);

	bool isPointInside = shape.isPointInside(point);
	BOOST_CHECK_EQUAL(isPointInside, true);
}

// Test 12: Point is on Bottom Face
BOOST_AUTO_TEST_CASE(isPointInside_test12, * utf::tolerance(0.00001))
{
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> tlf(0.0, 5.0, 12.0);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> trf(5.0, 5.0, 12.0);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> blf(0.0, 5.0, 0.0);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> brf(5.0, 5.0, 0.0);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> tlb(0.0, 0.0, 12.0);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> trb(5.0, 0.0, 12.0);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> blb(0.0, 0.0, 0.0);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> brb(5.0, 0.0, 0.0);

	Hexahedron<double> shape(tlf, trf, blf, brf, tlb, trb, blb, brb);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> point(2.3, 3.853, 0.0);

	bool isPointInside = shape.isPointInside(point);
	BOOST_CHECK_EQUAL(isPointInside, true);
}

// Test 13: Point is on Left Face
BOOST_AUTO_TEST_CASE(isPointInside_test13, * utf::tolerance(0.00001))
{
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> tlf(0.0, 5.0, 12.0);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> trf(5.0, 5.0, 12.0);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> blf(0.0, 5.0, 0.0);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> brf(5.0, 5.0, 0.0);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> tlb(0.0, 0.0, 12.0);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> trb(5.0, 0.0, 12.0);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> blb(0.0, 0.0, 0.0);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> brb(5.0, 0.0, 0.0);

	Hexahedron<double> shape(tlf, trf, blf, brf, tlb, trb, blb, brb);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> point(0.0, 2.98, 5.2);

	bool isPointInside = shape.isPointInside(point);
	BOOST_CHECK_EQUAL(isPointInside, true);
}

// Test 14: Point is on Right Face
BOOST_AUTO_TEST_CASE(isPointInside_test14, * utf::tolerance(0.00001))
{
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> tlf(0.0, 5.0, 12.0);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> trf(5.0, 5.0, 12.0);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> blf(0.0, 5.0, 0.0);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> brf(5.0, 5.0, 0.0);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> tlb(0.0, 0.0, 12.0);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> trb(5.0, 0.0, 12.0);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> blb(0.0, 0.0, 0.0);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> brb(5.0, 0.0, 0.0);

	Hexahedron<double> shape(tlf, trf, blf, brf, tlb, trb, blb, brb);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> point(5.0, 2.98, 5.2);

	bool isPointInside = shape.isPointInside(point);
	BOOST_CHECK_EQUAL(isPointInside, true);
}

// Test 15: Point is on Front Face
BOOST_AUTO_TEST_CASE(isPointInside_test15, * utf::tolerance(0.00001))
{
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> tlf(0.0, 5.0, 12.0);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> trf(5.0, 5.0, 12.0);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> blf(0.0, 5.0, 0.0);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> brf(5.0, 5.0, 0.0);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> tlb(0.0, 0.0, 12.0);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> trb(5.0, 0.0, 12.0);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> blb(0.0, 0.0, 0.0);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> brb(5.0, 0.0, 0.0);

	Hexahedron<double> shape(tlf, trf, blf, brf, tlb, trb, blb, brb);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> point(1.63, 5.0, 5.2);

	bool isPointInside = shape.isPointInside(point);
	BOOST_CHECK_EQUAL(isPointInside, true);
}

// Test 16: Point is on Back Face
BOOST_AUTO_TEST_CASE(isPointInside_test16, * utf::tolerance(0.00001))
{
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> tlf(0.0, 5.0, 12.0);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> trf(5.0, 5.0, 12.0);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> blf(0.0, 5.0, 0.0);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> brf(5.0, 5.0, 0.0);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> tlb(0.0, 0.0, 12.0);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> trb(5.0, 0.0, 12.0);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> blb(0.0, 0.0, 0.0);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> brb(5.0, 0.0, 0.0);

	Hexahedron<double> shape(tlf, trf, blf, brf, tlb, trb, blb, brb);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> point(1.63, 0.0, 5.2);

	bool isPointInside = shape.isPointInside(point);
	BOOST_CHECK_EQUAL(isPointInside, true);
}

// Test 17: Point is Outside
BOOST_AUTO_TEST_CASE(isPointInside_test17, * utf::tolerance(0.00001))
{
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> tlf(0.0909091,0.5,0.454545);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> trf(0.0909091,0.5,0.545455);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> blf(0.0,0.5,0.454545);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> brf(0.0,0.5,0.545455);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> tlb(0.0909091,0.583333,0.454545);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> trb(0.0909091,0.583333,0.545455);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> blb(0.0,0.583333,0.454545);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> brb(0.0,0.583333,0.545455);

	Hexahedron<double> shape(tlf, trf, blf, brf, tlb, trb, blb, brb);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> point(0.23, 0.572, 0.5563);

	bool isPointInside = shape.isPointInside(point);
	BOOST_CHECK_EQUAL(isPointInside, false);
}

// Test 18: Point is on same plane as a face, but not in a face

// ToDo: Point on Edges Tests


// === computeVolume ===
// Test 1: Compute volume of cube
BOOST_AUTO_TEST_CASE(computeVolume_test1, * utf::tolerance(0.00001))
{
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> tlf(0.0, 5.0, 12.0);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> trf(5.0, 5.0, 12.0);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> blf(0.0, 5.0, 0.0);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> brf(5.0, 5.0, 0.0);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> tlb(0.0, 0.0, 12.0);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> trb(5.0, 0.0, 12.0);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> blb(0.0, 0.0, 0.0);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> brb(5.0, 0.0, 0.0);

	Hexahedron<double> shape(tlf, trf, blf, brf, tlb, trb, blb, brb);

	double volume = shape.computeVolume();

	BOOST_TEST(volume == 300.0);
}

// Test 2: Compute volume of more irregular hexahedron
// ToDo
BOOST_AUTO_TEST_CASE(computeVolume_test2, * utf::tolerance(0.00001))
{

}

// === computeCentroid ===
// Test 1: Compute correct centroid of a cube
BOOST_AUTO_TEST_CASE(computeCentroid_test1, * utf::tolerance(0.00001))
{
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> tlf(0.0, 5.0, 12.0);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> trf(5.0, 5.0, 12.0);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> blf(0.0, 5.0, 0.0);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> brf(5.0, 5.0, 0.0);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> tlb(0.0, 0.0, 12.0);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> trb(5.0, 0.0, 12.0);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> blb(0.0, 0.0, 0.0);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> brb(5.0, 0.0, 0.0);

	Hexahedron<double> shape(tlf, trf, blf, brf, tlb, trb, blb, brb);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> centroid = shape.computeCentroid();
	BOOST_TEST(centroid.cmp[0], 2.5);
	BOOST_TEST(centroid.cmp[1], 2.5);
	BOOST_TEST(centroid.cmp[2], 6.0);
}

// === isPointOnEdge ===
// Test 1: Test point that is not on an edge
BOOST_AUTO_TEST_CASE(isPointOnEdge_test1, * utf::tolerance(0.00001))
{
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> tlf(0.0, 5.0, 12.0);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> trf(5.0, 5.0, 12.0);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> blf(0.0, 5.0, 0.0);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> brf(5.0, 5.0, 0.0);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> tlb(0.0, 0.0, 12.0);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> trb(5.0, 0.0, 12.0);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> blb(0.0, 0.0, 0.0);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> brb(5.0, 0.0, 0.0);

	Hexahedron<double> shape(tlf, trf, blf, brf, tlb, trb, blb, brb);

	cupcfd::geometry::euclidean::EuclideanPoint<double,3> point(3.4, 2.3, 4.7);

	bool onEdge = shape.isPointOnEdge(point);

	BOOST_CHECK_EQUAL(onEdge, false);
}

// Test 2: Test point that is on an edge
BOOST_AUTO_TEST_CASE(isPointOnEdge_test2, * utf::tolerance(0.00001))
{
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> tlf(0.0, 5.0, 12.0);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> trf(5.0, 5.0, 12.0);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> blf(0.0, 5.0, 0.0);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> brf(5.0, 5.0, 0.0);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> tlb(0.0, 0.0, 12.0);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> trb(5.0, 0.0, 12.0);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> blb(0.0, 0.0, 0.0);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> brb(5.0, 0.0, 0.0);

	Hexahedron<double> shape(tlf, trf, blf, brf, tlb, trb, blb, brb);

	cupcfd::geometry::euclidean::EuclideanPoint<double,3> point(2.13, 5.0, 12.0);

	bool onEdge = shape.isPointOnEdge(point);

	BOOST_CHECK_EQUAL(onEdge, true);
}

// === isPointOnVertex ===
// Test 1: Test point that is not on a vertex
BOOST_AUTO_TEST_CASE(isPointOnVertex_test1, * utf::tolerance(0.00001))
{
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> tlf(0.0, 5.0, 12.0);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> trf(5.0, 5.0, 12.0);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> blf(0.0, 5.0, 0.0);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> brf(5.0, 5.0, 0.0);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> tlb(0.0, 0.0, 12.0);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> trb(5.0, 0.0, 12.0);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> blb(0.0, 0.0, 0.0);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> brb(5.0, 0.0, 0.0);

	Hexahedron<double> shape(tlf, trf, blf, brf, tlb, trb, blb, brb);

	cupcfd::geometry::euclidean::EuclideanPoint<double,3> point(2.3, 5.0, 12.0);

	bool onEdge = shape.isPointOnVertex(point);

	BOOST_CHECK_EQUAL(onEdge, false);
}

// Test 2: Test point that is on a vertex
BOOST_AUTO_TEST_CASE(isPointOnVertex_test2, * utf::tolerance(0.00001))
{
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> tlf(0.0, 5.0, 12.0);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> trf(5.0, 5.0, 12.0);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> blf(0.0, 5.0, 0.0);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> brf(5.0, 5.0, 0.0);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> tlb(0.0, 0.0, 12.0);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> trb(5.0, 0.0, 12.0);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> blb(0.0, 0.0, 0.0);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> brb(5.0, 0.0, 0.0);

	Hexahedron<double> shape(tlf, trf, blf, brf, tlb, trb, blb, brb);

	cupcfd::geometry::euclidean::EuclideanPoint<double,3> point(0.0, 0.0, 12.0);

	bool onEdge = shape.isPointOnVertex(point);

	BOOST_CHECK_EQUAL(onEdge, true);
}
