/*
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Unit Tests for the methods of the Pyramid class
 */


#define BOOST_TEST_MODULE Pyramid
#include <boost/test/unit_test.hpp>
#include <boost/test/output_test_stream.hpp>
#include <stdexcept>

#include "Pyramid.h"
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

	Quadrilateral3D<double> base(p1, p2, p3, p4);
	Pyramid<Quadrilateral3D<double>, double> shape(apex, base);

	BOOST_CHECK_EQUAL(shape.apex.cmp[0], 5.0);
	BOOST_CHECK_EQUAL(shape.apex.cmp[1], 5.0);
	BOOST_CHECK_EQUAL(shape.apex.cmp[2], 5.0);

	BOOST_CHECK_EQUAL(shape.base.vertices[0].cmp[0], 0.0);
	BOOST_CHECK_EQUAL(shape.base.vertices[0].cmp[1], 0.0);
	BOOST_CHECK_EQUAL(shape.base.vertices[0].cmp[2], 0.0);

	BOOST_CHECK_EQUAL(shape.base.vertices[1].cmp[0], 0.0);
	BOOST_CHECK_EQUAL(shape.base.vertices[1].cmp[1], 10.0);
	BOOST_CHECK_EQUAL(shape.base.vertices[1].cmp[2], 0.0);

	BOOST_CHECK_EQUAL(shape.base.vertices[2].cmp[0], 10.0);
	BOOST_CHECK_EQUAL(shape.base.vertices[2].cmp[1], 10.0);
	BOOST_CHECK_EQUAL(shape.base.vertices[2].cmp[2], 0.0);

	BOOST_CHECK_EQUAL(shape.base.vertices[3].cmp[0], 10.0);
	BOOST_CHECK_EQUAL(shape.base.vertices[3].cmp[1], 0.0);
	BOOST_CHECK_EQUAL(shape.base.vertices[3].cmp[2], 0.0);
}

// === isPointInside ===
// Test 1:
BOOST_AUTO_TEST_CASE(isPointInside_test1, * utf::tolerance(0.00001))
{

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

	Quadrilateral3D<double> base(p1, p2, p3, p4);
	Pyramid<Quadrilateral3D<double>, double> shape(apex, base);

	double volume = shape.computeVolume();

	BOOST_TEST(volume == 166.666667);
}

// Test 2: Compute Volume of Oblique Pyramid
BOOST_AUTO_TEST_CASE(computeVolume_test2, * utf::tolerance(0.00001))
{
	euc::EuclideanPoint<double, 3> p1(0.0, 0.0, 0.0);
	euc::EuclideanPoint<double, 3> p2(0.0, 10.0, 0.0);
	euc::EuclideanPoint<double, 3> p3(10.0, 10.0, 0.0);
	euc::EuclideanPoint<double, 3> p4(10.0, 0.0, 0.0);
	euc::EuclideanPoint<double, 3> apex(2.7, 3.7, 8.9);

	Quadrilateral3D<double> base(p1, p2, p3, p4);
	Pyramid<Quadrilateral3D<double>, double> shape(apex, base);

	double volume = shape.computeVolume();

	BOOST_TEST(volume == 296.666667);
}
