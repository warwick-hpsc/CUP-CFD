/*
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 */

#define BOOST_TEST_MODULE EuclideanPoint
#include <boost/test/unit_test.hpp>
#include <boost/test/output_test_stream.hpp>
#include <stdexcept>

#include "EuclideanPoint.h"
#include "EuclideanVector.h"
#include "Error.h"
#include "Communicator.h"
#include "Broadcast.h"

// Namespaces
namespace utf = boost::unit_test;

using namespace cupcfd::geometry::euclidean;

// Setup
BOOST_AUTO_TEST_CASE(setup)
{
    int argc = boost::unit_test::framework::master_test_suite().argc;
    char ** argv = boost::unit_test::framework::master_test_suite().argv;

    MPI_Init(&argc, &argv);
}

// === Constructor1 ===
// Test 1: Check values are set to 0.0 correctly - 2D
BOOST_AUTO_TEST_CASE(constructor1_test1, * utf::tolerance(0.00001))
{
	EuclideanPoint<double, 2> point;

	BOOST_CHECK_EQUAL(point.cmp[0], 0.0);
	BOOST_CHECK_EQUAL(point.cmp[1], 0.0);
}

// Test 2: Check values are set to 0.0 correctly - 3D
BOOST_AUTO_TEST_CASE(constructor1_test2, * utf::tolerance(0.00001))
{
	EuclideanPoint<double,3> point;

	BOOST_CHECK_EQUAL(point.cmp[0], 0.0);
	BOOST_CHECK_EQUAL(point.cmp[1], 0.0);
	BOOST_CHECK_EQUAL(point.cmp[2], 0.0);
}

// === Constructor2 ===
// Test 1: Check values are set to provided parameters correctly - 2D
BOOST_AUTO_TEST_CASE(constructor2_test1, * utf::tolerance(0.00001))
{
	EuclideanPoint<double, 2> point(2.0, -13.5);

	BOOST_CHECK_EQUAL(point.cmp[0], 2.0);
	BOOST_CHECK_EQUAL(point.cmp[1], -13.5);
}

// Test 2: Check values are set to provided parameters correctly - 3D
BOOST_AUTO_TEST_CASE(constructor2_test2, * utf::tolerance(0.00001))
{
	EuclideanPoint<double,3> point(2.0, -13.5, 18.9);

	BOOST_CHECK_EQUAL(point.cmp[0], 2.0);
	BOOST_CHECK_EQUAL(point.cmp[1], -13.5);
	BOOST_CHECK_EQUAL(point.cmp[2], 18.9);
}

// === Operation= Point ===
// Test 1: Check values are appropriately copied form src to dst - 2D
BOOST_AUTO_TEST_CASE(operation_assign_point_test1, * utf::tolerance(0.00001))
{
	EuclideanPoint<double, 2> pointsrc(2.0, 3.0);
	EuclideanPoint<double, 2> pointdst;

	pointdst = pointsrc;

	// Set this to verify the values are properly copied (i.e. no references)
	pointsrc.cmp[1] = -15.6;

	BOOST_CHECK_EQUAL(pointdst.cmp[0], 2.0);
	BOOST_CHECK_EQUAL(pointdst.cmp[1], 3.0);
}

// Test 2: Check values are appropriately copied form src to dst - 3D
BOOST_AUTO_TEST_CASE(operation_assign_point_test2, * utf::tolerance(0.00001))
{
	EuclideanPoint<double,3> pointsrc(2.0, 3.0, 4.0);
	EuclideanPoint<double,3> pointdst;

	pointdst = pointsrc;

	// Set this to verify the values are properly copied (i.e. no references)
	pointsrc.cmp[1] = -15.6;

	BOOST_CHECK_EQUAL(pointdst.cmp[0], 2.0);
	BOOST_CHECK_EQUAL(pointdst.cmp[1], 3.0);
	BOOST_CHECK_EQUAL(pointdst.cmp[2], 4.0);
}

// === Operation= Scalar ===
// Test 1: Check the three components of the point are all set to the same scalar - 2D
BOOST_AUTO_TEST_CASE(operation_assign_scalar_test1, * utf::tolerance(0.00001))
{
	EuclideanPoint<double, 2> pointdst;

	pointdst = 5.0;

	BOOST_CHECK_EQUAL(pointdst.cmp[0], 5.0);
	BOOST_CHECK_EQUAL(pointdst.cmp[1], 5.0);
}

// Test 2: Check the three components of the point are all set to the same scalar - 3D
BOOST_AUTO_TEST_CASE(operation_assign_scalar_test2, * utf::tolerance(0.00001))
{
	EuclideanPoint<double,3> pointdst;

	pointdst = 5.0;

	BOOST_CHECK_EQUAL(pointdst.cmp[0], 5.0);
	BOOST_CHECK_EQUAL(pointdst.cmp[1], 5.0);
	BOOST_CHECK_EQUAL(pointdst.cmp[2], 5.0);
}

// === Operation== ===
// Test 1: Test that the two points are identified as having the same values - 2D
BOOST_AUTO_TEST_CASE(operation_equals_test1, * utf::tolerance(0.00001))
{
	EuclideanPoint<double, 2> point1(2.0, 3.0);
	EuclideanPoint<double, 2> point2(2.0, 3.0);

	bool status = point1 == point2;

	BOOST_CHECK_EQUAL(status, true);
}

// Test 2: Test that the two points are identified as different when x is different - 2D
BOOST_AUTO_TEST_CASE(operation_equals_test2, * utf::tolerance(0.00001))
{
	EuclideanPoint<double,2> point1(2.0, 3.0);
	EuclideanPoint<double,2> point2(15.0, 3.0);

	bool status = point1 == point2;

	BOOST_CHECK_EQUAL(status, false);
}

// Test 3: Test that the two points are identified as different when y is different - 2D
BOOST_AUTO_TEST_CASE(operation_equals_test3, * utf::tolerance(0.00001))
{
	EuclideanPoint<double,2> point1(2.0, 3.0);
	EuclideanPoint<double,2> point2(2.0, 16.0);

	bool status = point1 == point2;

	BOOST_CHECK_EQUAL(status, false);
}

// Test 4: Test that the two points are identified as having the same values - 3D
BOOST_AUTO_TEST_CASE(operation_equals_test4, * utf::tolerance(0.00001))
{
	EuclideanPoint<double,3> point1(2.0, 3.0, 4.0);
	EuclideanPoint<double,3> point2(2.0, 3.0, 4.0);

	bool status = point1 == point2;

	BOOST_CHECK_EQUAL(status, true);
}

// Test 5: Test that the two points are identified as different when x is different - 3D
BOOST_AUTO_TEST_CASE(operation_equals_test5, * utf::tolerance(0.00001))
{
	EuclideanPoint<double,3> point1(2.0, 3.0, 4.0);
	EuclideanPoint<double,3> point2(15.0, 3.0, 4.0);

	bool status = point1 == point2;

	BOOST_CHECK_EQUAL(status, false);
}

// Test 6: Test that the two points are identified as different when y is different - 3D
BOOST_AUTO_TEST_CASE(operation_equals_test6, * utf::tolerance(0.00001))
{
	EuclideanPoint<double,3> point1(2.0, 3.0, 4.0);
	EuclideanPoint<double,3> point2(2.0, 16.0, 4.0);

	bool status = point1 == point2;

	BOOST_CHECK_EQUAL(status, false);
}

// Test 7: Test that the two points are identified as different when z is different
BOOST_AUTO_TEST_CASE(operation_equals_test7, * utf::tolerance(0.00001))
{
	EuclideanPoint<double,3> point1(2.0, 3.0, 4.0);
	EuclideanPoint<double,3> point2(2.0, 3.0, 17.0);

	bool status = point1 == point2;

	BOOST_CHECK_EQUAL(status, false);
}

// === Operation!= ===
// Test 1: Test that the two points are identified as having the same values - 2D
BOOST_AUTO_TEST_CASE(operation_notequals_test1, * utf::tolerance(0.00001))
{
	EuclideanPoint<double,2> point1(2.0, 3.0);
	EuclideanPoint<double,2> point2(2.0, 3.0);

	bool status = point1 != point2;

	BOOST_CHECK_EQUAL(status, false);
}

// Test 2: Test that the two points are identified as different when x is different - 2D
BOOST_AUTO_TEST_CASE(operation_notequals_test2, * utf::tolerance(0.00001))
{
	EuclideanPoint<double,2> point1(2.0, 3.0);
	EuclideanPoint<double,2> point2(15.0, 3.0);

	bool status = point1 != point2;

	BOOST_CHECK_EQUAL(status, true);
}

// Test 3: Test that the two points are identified as different when y is different - 2D
BOOST_AUTO_TEST_CASE(operation_notequals_test3, * utf::tolerance(0.00001))
{
	EuclideanPoint<double,2> point1(2.0, 3.0);
	EuclideanPoint<double,2> point2(2.0, 16.0);

	bool status = point1 != point2;

	BOOST_CHECK_EQUAL(status, true);
}

// Test 4: Test that the two points are identified as having the same values - 3D
BOOST_AUTO_TEST_CASE(operation_notequals_test4, * utf::tolerance(0.00001))
{
	EuclideanPoint<double,3> point1(2.0, 3.0, 4.0);
	EuclideanPoint<double,3> point2(2.0, 3.0, 4.0);

	bool status = point1 != point2;

	BOOST_CHECK_EQUAL(status, false);
}

// Test 5: Test that the two points are identified as different when x is different - 3D
BOOST_AUTO_TEST_CASE(operation_notequals_test5, * utf::tolerance(0.00001))
{
	EuclideanPoint<double,3> point1(2.0, 3.0, 4.0);
	EuclideanPoint<double,3> point2(15.0, 3.0, 4.0);

	bool status = point1 != point2;

	BOOST_CHECK_EQUAL(status, true);
}

// Test 6: Test that the two points are identified as different when y is different - 3D
BOOST_AUTO_TEST_CASE(operation_notequals_test6, * utf::tolerance(0.00001))
{
	EuclideanPoint<double,3> point1(2.0, 3.0, 4.0);
	EuclideanPoint<double,3> point2(2.0, 16.0, 4.0);

	bool status = point1 != point2;

	BOOST_CHECK_EQUAL(status, true);
}

// Test 7: Test that the two points are identified as different when z is different
BOOST_AUTO_TEST_CASE(operation_notequals_test7, * utf::tolerance(0.00001))
{
	EuclideanPoint<double,3> point1(2.0, 3.0, 4.0);
	EuclideanPoint<double,3> point2(2.0, 3.0, 17.0);

	bool status = point1 != point2;

	BOOST_CHECK_EQUAL(status, true);
}

// === Operation< ===
// Test 1: y is less - 2D
BOOST_AUTO_TEST_CASE(operation_lessthan_test1, * utf::tolerance(0.00001))
{
	EuclideanPoint<double,2> point1(2.0, 2.5);
	EuclideanPoint<double,2> point2(2.0, 3.0);

	bool status = point1 < point2;

	BOOST_CHECK_EQUAL(status, true);
}

// Test 2: y is greater - 2D
BOOST_AUTO_TEST_CASE(operation_lessthan_test2, * utf::tolerance(0.00001))
{
	EuclideanPoint<double,2> point1(2.0, 3.5);
	EuclideanPoint<double,2> point2(2.0, 3.0);

	bool status = point1 < point2;

	BOOST_CHECK_EQUAL(status, false);
}

// Test 3: y is equal, x is less - 2D
BOOST_AUTO_TEST_CASE(operation_lessthan_test3, * utf::tolerance(0.00001))
{
	EuclideanPoint<double,2> point1(1.5, 3.0);
	EuclideanPoint<double,2> point2(2.0, 3.0);

	bool status = point1 < point2;

	BOOST_CHECK_EQUAL(status, true);
}

// Test 4: y is equal, x is greater - 2D
BOOST_AUTO_TEST_CASE(operation_lessthan_test4, * utf::tolerance(0.00001))
{
	EuclideanPoint<double,2> point1(2.5, 3.0);
	EuclideanPoint<double,2> point2(2.0, 3.0);

	bool status = point1 < point2;

	BOOST_CHECK_EQUAL(status, false);
}

// Test 5: All components are equal - 2D
BOOST_AUTO_TEST_CASE(operation_lessthan_test5, * utf::tolerance(0.00001))
{
	EuclideanPoint<double,2> point1(2.0, 3.0);
	EuclideanPoint<double,2> point2(2.0, 3.0);

	bool status = point1 < point2;

	BOOST_CHECK_EQUAL(status, false);
}

// Test 6: z is less for point1
BOOST_AUTO_TEST_CASE(operation_lessthan_test6, * utf::tolerance(0.00001))
{
	EuclideanPoint<double,3> point1(2.0, 3.0, 3.5);
	EuclideanPoint<double,3> point2(2.0, 3.0, 4.0);

	bool status = point1 < point2;

	BOOST_CHECK_EQUAL(status, true);
}

// Test 7: z is greater for point 1
BOOST_AUTO_TEST_CASE(operation_lessthan_test7, * utf::tolerance(0.00001))
{
	EuclideanPoint<double,3> point1(2.0, 3.0, 5.0);
	EuclideanPoint<double,3> point2(2.0, 3.0, 4.0);

	bool status = point1 < point2;

	BOOST_CHECK_EQUAL(status, false);
}

// Test 8: z is equal, y is less
BOOST_AUTO_TEST_CASE(operation_lessthan_test8, * utf::tolerance(0.00001))
{
	EuclideanPoint<double,3> point1(2.0, 2.5, 4.0);
	EuclideanPoint<double,3> point2(2.0, 3.0, 4.0);

	bool status = point1 < point2;

	BOOST_CHECK_EQUAL(status, true);
}

// Test 9: z is equal, y is greater
BOOST_AUTO_TEST_CASE(operation_lessthan_test9, * utf::tolerance(0.00001))
{
	EuclideanPoint<double,3> point1(2.0, 3.5, 4.0);
	EuclideanPoint<double,3> point2(2.0, 3.0, 4.0);

	bool status = point1 < point2;

	BOOST_CHECK_EQUAL(status, false);
}

// Test 10: z is equal, y is equal, x is less
BOOST_AUTO_TEST_CASE(operation_lessthan_test10, * utf::tolerance(0.00001))
{
	EuclideanPoint<double,3> point1(1.5, 3.0, 4.0);
	EuclideanPoint<double,3> point2(2.0, 3.0, 4.0);

	bool status = point1 < point2;

	BOOST_CHECK_EQUAL(status, true);
}

// Test 11: z is equal, y is equal, x is greater
BOOST_AUTO_TEST_CASE(operation_lessthan_test11, * utf::tolerance(0.00001))
{
	EuclideanPoint<double,3> point1(2.5, 3.0, 4.0);
	EuclideanPoint<double,3> point2(2.0, 3.0, 4.0);

	bool status = point1 < point2;

	BOOST_CHECK_EQUAL(status, false);
}

// Test 12: All components are equal
BOOST_AUTO_TEST_CASE(operation_lessthan_test12, * utf::tolerance(0.00001))
{
	EuclideanPoint<double,3> point1(2.0, 3.0, 4.0);
	EuclideanPoint<double,3> point2(2.0, 3.0, 4.0);

	bool status = point1 < point2;

	BOOST_CHECK_EQUAL(status, false);
}

// === Operation<= ===
// Test 1: y is less
BOOST_AUTO_TEST_CASE(operation_lessthanorequals_test1, * utf::tolerance(0.00001))
{
	EuclideanPoint<double,2> point1(2.0, 2.5);
	EuclideanPoint<double,2> point2(2.0, 3.0);

	bool status = point1 <= point2;

	BOOST_CHECK_EQUAL(status, true);
}

// Test 2: y is greater
BOOST_AUTO_TEST_CASE(operation_lessthanorequals_test2, * utf::tolerance(0.00001))
{
	EuclideanPoint<double,2> point1(2.0, 3.5);
	EuclideanPoint<double,2> point2(2.0, 3.0);

	bool status = point1 <= point2;

	BOOST_CHECK_EQUAL(status, false);
}

// Test 3: y is equal, x is less
BOOST_AUTO_TEST_CASE(operation_lessthanorequals_test3, * utf::tolerance(0.00001))
{
	EuclideanPoint<double,2> point1(1.5, 3.0);
	EuclideanPoint<double,2> point2(2.0, 3.0);

	bool status = point1 <= point2;

	BOOST_CHECK_EQUAL(status, true);
}

// Test 4: y is equal, x is greater
BOOST_AUTO_TEST_CASE(operation_lessthanorequals_test4, * utf::tolerance(0.00001))
{
	EuclideanPoint<double,2> point1(2.5, 3.0);
	EuclideanPoint<double,2> point2(2.0, 3.0);

	bool status = point1 <= point2;

	BOOST_CHECK_EQUAL(status, false);
}

// Test 5: All components are equal
BOOST_AUTO_TEST_CASE(operation_lessthanorequals_test5, * utf::tolerance(0.00001))
{
	EuclideanPoint<double,2> point1(2.0, 3.0);
	EuclideanPoint<double,2> point2(2.0, 3.0);

	bool status = point1 <= point2;

	BOOST_CHECK_EQUAL(status, true);
}

// Test 6: z is less for point1
BOOST_AUTO_TEST_CASE(operation_lessthanorequals_test6, * utf::tolerance(0.00001))
{
	EuclideanPoint<double,3> point1(2.0, 3.0, 3.5);
	EuclideanPoint<double,3> point2(2.0, 3.0, 4.0);

	bool status = point1 <= point2;

	BOOST_CHECK_EQUAL(status, true);
}

// Test 7: z is greater for point 1
BOOST_AUTO_TEST_CASE(operation_lessthanorequals_test7, * utf::tolerance(0.00001))
{
	EuclideanPoint<double,3> point1(2.0, 3.0, 5.0);
	EuclideanPoint<double,3> point2(2.0, 3.0, 4.0);

	bool status = point1 <= point2;

	BOOST_CHECK_EQUAL(status, false);
}

// Test 8: z is equal, y is less
BOOST_AUTO_TEST_CASE(operation_lessthanorequals_test8, * utf::tolerance(0.00001))
{
	EuclideanPoint<double,3> point1(2.0, 2.5, 4.0);
	EuclideanPoint<double,3> point2(2.0, 3.0, 4.0);

	bool status = point1 <= point2;

	BOOST_CHECK_EQUAL(status, true);
}

// Test 9: z is equal, y is greater
BOOST_AUTO_TEST_CASE(operation_lessthanorequals_test9, * utf::tolerance(0.00001))
{
	EuclideanPoint<double,3> point1(2.0, 3.5, 4.0);
	EuclideanPoint<double,3> point2(2.0, 3.0, 4.0);

	bool status = point1 <= point2;

	BOOST_CHECK_EQUAL(status, false);
}

// Test 10: z is equal, y is equal, x is less
BOOST_AUTO_TEST_CASE(operation_lessthanorequals_test10, * utf::tolerance(0.00001))
{
	EuclideanPoint<double,3> point1(1.5, 3.0, 4.0);
	EuclideanPoint<double,3> point2(2.0, 3.0, 4.0);

	bool status = point1 <= point2;

	BOOST_CHECK_EQUAL(status, true);
}

// Test 11: z is equal, y is equal, x is greater
BOOST_AUTO_TEST_CASE(operation_lessthanorequals_test11, * utf::tolerance(0.00001))
{
	EuclideanPoint<double,3> point1(2.5, 3.0, 4.0);
	EuclideanPoint<double,3> point2(2.0, 3.0, 4.0);

	bool status = point1 <= point2;

	BOOST_CHECK_EQUAL(status, false);
}

// Test 12: All components are equal
BOOST_AUTO_TEST_CASE(operation_lessthanorequals_test12, * utf::tolerance(0.00001))
{
	EuclideanPoint<double,3> point1(2.0, 3.0, 4.0);
	EuclideanPoint<double,3> point2(2.0, 3.0, 4.0);

	bool status = point1 <= point2;

	BOOST_CHECK_EQUAL(status, true);
}

// === Operation> ===
// Test 1: y is less - 2D
BOOST_AUTO_TEST_CASE(operation_greaterthan_test1, * utf::tolerance(0.00001))
{
	EuclideanPoint<double,2> point1(2.0, 2.5);
	EuclideanPoint<double,2> point2(2.0, 3.0);

	bool status = point1 > point2;

	BOOST_CHECK_EQUAL(status, false);
}

// Test 2: y is greater - 2D
BOOST_AUTO_TEST_CASE(operation_greaterthan_test2, * utf::tolerance(0.00001))
{
	EuclideanPoint<double,2> point1(2.0, 3.5);
	EuclideanPoint<double,2> point2(2.0, 3.0);

	bool status = point1 > point2;

	BOOST_CHECK_EQUAL(status, true);
}

// Test 3: y is equal, x is less - 2D
BOOST_AUTO_TEST_CASE(operation_greaterthan_test3, * utf::tolerance(0.00001))
{
	EuclideanPoint<double,2> point1(1.5, 3.0);
	EuclideanPoint<double,2> point2(2.0, 3.0);

	bool status = point1 > point2;

	BOOST_CHECK_EQUAL(status, false);
}

// Test 4: y is equal, x is greater - 2D
BOOST_AUTO_TEST_CASE(operation_greaterthan_test4, * utf::tolerance(0.00001))
{
	EuclideanPoint<double,2> point1(2.5, 3.0);
	EuclideanPoint<double,2> point2(2.0, 3.0);

	bool status = point1 > point2;

	BOOST_CHECK_EQUAL(status, true);
}

// Test 5: All components are equal - 2D
BOOST_AUTO_TEST_CASE(operation_greaterthan_test5, * utf::tolerance(0.00001))
{
	EuclideanPoint<double,2> point1(2.0, 3.0);
	EuclideanPoint<double,2> point2(2.0, 3.0);

	bool status = point1 > point2;

	BOOST_CHECK_EQUAL(status, false);
}

// Test 6: z is less for point1
BOOST_AUTO_TEST_CASE(operation_greaterthan_test6, * utf::tolerance(0.00001))
{
	EuclideanPoint<double,3> point1(2.0, 3.0, 3.5);
	EuclideanPoint<double,3> point2(2.0, 3.0, 4.0);

	bool status = point1 > point2;

	BOOST_CHECK_EQUAL(status, false);
}

// Test 7: z is greater for point 1
BOOST_AUTO_TEST_CASE(operation_greaterthan_test7, * utf::tolerance(0.00001))
{
	EuclideanPoint<double,3> point1(2.0, 3.0, 5.0);
	EuclideanPoint<double,3> point2(2.0, 3.0, 4.0);

	bool status = point1 > point2;

	BOOST_CHECK_EQUAL(status, true);
}

// Test 8: z is equal, y is less
BOOST_AUTO_TEST_CASE(operation_greaterthan_test8, * utf::tolerance(0.00001))
{
	EuclideanPoint<double,3> point1(2.0, 2.5, 4.0);
	EuclideanPoint<double,3> point2(2.0, 3.0, 4.0);

	bool status = point1 > point2;

	BOOST_CHECK_EQUAL(status, false);
}

// Test 9: z is equal, y is greater
BOOST_AUTO_TEST_CASE(operation_greaterthan_test9, * utf::tolerance(0.00001))
{
	EuclideanPoint<double,3> point1(2.0, 3.5, 4.0);
	EuclideanPoint<double,3> point2(2.0, 3.0, 4.0);

	bool status = point1 > point2;

	BOOST_CHECK_EQUAL(status, true);
}

// Test 10: z is equal, y is equal, x is less
BOOST_AUTO_TEST_CASE(operation_greaterthan_test10, * utf::tolerance(0.00001))
{
	EuclideanPoint<double,3> point1(1.5, 3.0, 4.0);
	EuclideanPoint<double,3> point2(2.0, 3.0, 4.0);

	bool status = point1 > point2;

	BOOST_CHECK_EQUAL(status, false);
}

// Test 11: z is equal, y is equal, x is greater
BOOST_AUTO_TEST_CASE(operation_greaterthan_test11, * utf::tolerance(0.00001))
{
	EuclideanPoint<double,3> point1(2.5, 3.0, 4.0);
	EuclideanPoint<double,3> point2(2.0, 3.0, 4.0);

	bool status = point1 > point2;

	BOOST_CHECK_EQUAL(status, true);
}

// Test 12: All components are equal
BOOST_AUTO_TEST_CASE(operation_greaterthan_test12, * utf::tolerance(0.00001))
{
	EuclideanPoint<double,3> point1(2.0, 3.0, 4.0);
	EuclideanPoint<double,3> point2(2.0, 3.0, 4.0);

	bool status = point1 > point2;

	BOOST_CHECK_EQUAL(status, false);
}

// === Operation>= ===
// Test 1: y is less
BOOST_AUTO_TEST_CASE(operation_greaterthanorequal_test1, * utf::tolerance(0.00001))
{
	EuclideanPoint<double,2> point1(2.0, 2.5);
	EuclideanPoint<double,2> point2(2.0, 3.0);

	bool status = point1 >= point2;

	BOOST_CHECK_EQUAL(status, false);
}

// Test 2: z is equal, y is greater
BOOST_AUTO_TEST_CASE(operation_greaterthanorequal_test2, * utf::tolerance(0.00001))
{
	EuclideanPoint<double,2> point1(2.0, 3.5);
	EuclideanPoint<double,2> point2(2.0, 3.0);

	bool status = point1 >= point2;

	BOOST_CHECK_EQUAL(status, true);
}

// Test 3: y is equal, x is less
BOOST_AUTO_TEST_CASE(operation_greaterthanorequal_test3, * utf::tolerance(0.00001))
{
	EuclideanPoint<double,2> point1(1.5, 3.0);
	EuclideanPoint<double,2> point2(2.0, 3.0);

	bool status = point1 >= point2;

	BOOST_CHECK_EQUAL(status, false);
}

// Test 4: y is equal, x is greater
BOOST_AUTO_TEST_CASE(operation_greaterthanorequal_test4, * utf::tolerance(0.00001))
{
	EuclideanPoint<double,2> point1(2.5, 3.0);
	EuclideanPoint<double,2> point2(2.0, 3.0);

	bool status = point1 >= point2;

	BOOST_CHECK_EQUAL(status, true);
}

// Test 5: All components are equal
BOOST_AUTO_TEST_CASE(operation_greaterthanorequal_test5, * utf::tolerance(0.00001))
{
	EuclideanPoint<double,2> point1(2.0, 3.0);
	EuclideanPoint<double,2> point2(2.0, 3.0);

	bool status = point1 >= point2;

	BOOST_CHECK_EQUAL(status, true);
}

// Test 6: z is less for point1
BOOST_AUTO_TEST_CASE(operation_greaterthanorequal_test6, * utf::tolerance(0.00001))
{
	EuclideanPoint<double,3> point1(2.0, 3.0, 3.5);
	EuclideanPoint<double,3> point2(2.0, 3.0, 4.0);

	bool status = point1 >= point2;

	BOOST_CHECK_EQUAL(status, false);
}

// Test 7: z is greater for point 1
BOOST_AUTO_TEST_CASE(operation_greaterthanorequal_test7, * utf::tolerance(0.00001))
{
	EuclideanPoint<double,3> point1(2.0, 3.0, 5.0);
	EuclideanPoint<double,3> point2(2.0, 3.0, 4.0);

	bool status = point1 >= point2;

	BOOST_CHECK_EQUAL(status, true);
}

// Test 8: z is equal, y is less
BOOST_AUTO_TEST_CASE(operation_greaterthanorequal_test8, * utf::tolerance(0.00001))
{
	EuclideanPoint<double,3> point1(2.0, 2.5, 4.0);
	EuclideanPoint<double,3> point2(2.0, 3.0, 4.0);

	bool status = point1 >= point2;

	BOOST_CHECK_EQUAL(status, false);
}

// Test 9: z is equal, y is greater
BOOST_AUTO_TEST_CASE(operation_greaterthanorequal_test9, * utf::tolerance(0.00001))
{
	EuclideanPoint<double,3> point1(2.0, 3.5, 4.0);
	EuclideanPoint<double,3> point2(2.0, 3.0, 4.0);

	bool status = point1 >= point2;

	BOOST_CHECK_EQUAL(status, true);
}

// Test 10: z is equal, y is equal, x is less
BOOST_AUTO_TEST_CASE(operation_greaterthanorequal_test10, * utf::tolerance(0.00001))
{
	EuclideanPoint<double,3> point1(1.5, 3.0, 4.0);
	EuclideanPoint<double,3> point2(2.0, 3.0, 4.0);

	bool status = point1 >= point2;

	BOOST_CHECK_EQUAL(status, false);
}

// Test 11: z is equal, y is equal, x is greater
BOOST_AUTO_TEST_CASE(operation_greaterthanorequal_test11, * utf::tolerance(0.00001))
{
	EuclideanPoint<double,3> point1(2.5, 3.0, 4.0);
	EuclideanPoint<double,3> point2(2.0, 3.0, 4.0);

	bool status = point1 >= point2;

	BOOST_CHECK_EQUAL(status, true);
}

// Test 12: All components are equal
BOOST_AUTO_TEST_CASE(operation_greaterthanorequal_test12, * utf::tolerance(0.00001))
{
	EuclideanPoint<double,3> point1(2.0, 3.0, 4.0);
	EuclideanPoint<double,3> point2(2.0, 3.0, 4.0);

	bool status = point1 >= point2;

	BOOST_CHECK_EQUAL(status, true);
}

// === Operation+ Vector ===
// Test 1: Point On Left Hand Side
BOOST_AUTO_TEST_CASE(operation_add_vector_test1, * utf::tolerance(0.00001))
{
	EuclideanPoint<double,3> point1(2.5, 3.0, 4.0);
	EuclideanVector<double,3> vec1(3.0, 4.0, 5.0);

	EuclideanPoint<double,3> result = point1 + vec1;
	EuclideanPoint<double,3> compare(5.5, 7.0, 9.0);

	BOOST_TEST(result.cmp[0] == compare.cmp[0]);
	BOOST_TEST(result.cmp[1] == compare.cmp[1]);
	BOOST_TEST(result.cmp[2] == compare.cmp[2]);
}

// Test 2: Point on Right Hand Side
BOOST_AUTO_TEST_CASE(operation_add_vector_test2, * utf::tolerance(0.00001))
{
	EuclideanPoint<double,3> point1(2.5, 3.0, 4.0);
	EuclideanVector<double,3> vec1(3.0, 4.0, 5.0);

	EuclideanPoint<double,3> result = vec1 + point1;
	EuclideanPoint<double,3> compare(5.5, 7.0, 9.0);

	BOOST_TEST(result.cmp[0] == compare.cmp[0]);
	BOOST_TEST(result.cmp[1] == compare.cmp[1]);
	BOOST_TEST(result.cmp[2] == compare.cmp[2]);
}

// === Operation+ Scalar ===
// Test 1: Point on Left hand Side
BOOST_AUTO_TEST_CASE(operation_add_scalar_test1, * utf::tolerance(0.00001))
{
	EuclideanPoint<double,3> point1(2.5, 3.0, 4.0);

	EuclideanPoint<double,3> result = point1 + 3.4;
	EuclideanPoint<double,3> compare(5.9, 6.4, 7.4);

	BOOST_TEST(result.cmp[0] == compare.cmp[0]);
	BOOST_TEST(result.cmp[1] == compare.cmp[1]);
	BOOST_TEST(result.cmp[2] == compare.cmp[2]);
}

// Test 2: Point on Right Hand Side
BOOST_AUTO_TEST_CASE(operation_add_scalar_test2, * utf::tolerance(0.00001))
{
	EuclideanPoint<double,3> point1(2.5, 3.0, 4.0);

	EuclideanPoint<double,3> result = 3.4 + point1;
	EuclideanPoint<double,3> compare(5.9, 6.4, 7.4);

	BOOST_TEST(result.cmp[0] == compare.cmp[0]);
	BOOST_TEST(result.cmp[1] == compare.cmp[1]);
	BOOST_TEST(result.cmp[2] == compare.cmp[2]);
}

// === Operation- Point ===
// Test 1: Get the Vector from a Point - Point
BOOST_AUTO_TEST_CASE(operation_minus_point_test1, * utf::tolerance(0.00001))
{
	EuclideanPoint<double,3> point1(2.5, 3.0, 5.1);
	EuclideanPoint<double,3> point2(3.0, 4.0, 4.0);

	EuclideanVector<double,3> result = point2 - point1;
	EuclideanVector<double,3> compare(0.5, 1.0, -1.1);

	BOOST_TEST(result.cmp[0] == compare.cmp[0]);
	BOOST_TEST(result.cmp[1] == compare.cmp[1]);
	BOOST_TEST(result.cmp[2] == compare.cmp[2]);
}

// === Operation- Vector ===
// Test 1: Get the Source Point From a Dest Point - Vector
BOOST_AUTO_TEST_CASE(operation_minus_vector_test1, * utf::tolerance(0.00001))
{
	EuclideanPoint<double,3> point1(2.5, 3.0, 5.1);
	EuclideanVector<double,3> vec1(3.0, 4.0, 5.0);

	EuclideanPoint<double,3> result = point1 - vec1;
	EuclideanPoint<double,3> compare(-0.5, -1.0, 0.1);

	BOOST_TEST(result.cmp[0] == compare.cmp[0]);
	BOOST_TEST(result.cmp[1] == compare.cmp[1]);
	BOOST_TEST(result.cmp[2] == compare.cmp[2]);
}

// === Operation* Scalar ===
// Test 1: Point on Left Hand Side
BOOST_AUTO_TEST_CASE(operation_multiply_scalar_test1, * utf::tolerance(0.00001))
{
	EuclideanPoint<double,3> point1(2.5, 3.0, 4.0);

	EuclideanPoint<double,3> result = point1 * 3.4;
	EuclideanPoint<double,3> compare(8.5, 10.2, 13.6);

	BOOST_TEST(result.cmp[0] == compare.cmp[0]);
	BOOST_TEST(result.cmp[1] == compare.cmp[1]);
	BOOST_TEST(result.cmp[2] == compare.cmp[2]);
}

// Test 2: Point on Right Hand Side
BOOST_AUTO_TEST_CASE(operation_multiply_scalar_test2, * utf::tolerance(0.00001))
{
	EuclideanPoint<double,3> point1(2.5, 3.0, 4.0);

	EuclideanPoint<double,3> result = 3.4 * point1;
	EuclideanPoint<double,3> compare(8.5, 10.2, 13.6);

	BOOST_TEST(result.cmp[0] == compare.cmp[0]);
	BOOST_TEST(result.cmp[1] == compare.cmp[1]);
	BOOST_TEST(result.cmp[2] == compare.cmp[2]);
}

// === Operation/ Scalar ===
// Test 1: Point on Left Hand Side
BOOST_AUTO_TEST_CASE(operation_divide_scalar_test1, * utf::tolerance(0.00001))
{
	EuclideanPoint<double,3> point1(2.5, 3.0, 4.0);

	EuclideanPoint<double,3> result = point1 / 3.4;
	EuclideanPoint<double,3> compare(0.735294, 0.882353, 1.176471);

	BOOST_TEST(result.cmp[0] == compare.cmp[0]);
	BOOST_TEST(result.cmp[1] == compare.cmp[1]);
	BOOST_TEST(result.cmp[2] == compare.cmp[2]);
}

// === isRegistered ===
// Not sure quite where to/when to place this test....
// Should use a setter/getter to set/get registered?

// === register and deregisterMPIType ===
// Test 1: Test that Type is registered without error, and can be deregistered without error
BOOST_AUTO_TEST_CASE(registerMPIType_test1, * utf::tolerance(0.00001))
{
	cupcfd::error::eCodes status;
	EuclideanPoint<double,3> point;

	status = point.registerMPIType();
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	status = point.deregisterMPIType();
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
}

// Test 2: Test that if it is already registered, it does not register again
BOOST_AUTO_TEST_CASE(registerMPIType_test2, * utf::tolerance(0.00001))
{
	cupcfd::error::eCodes status;
	EuclideanPoint<double,3> point;

	status = point.registerMPIType();
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	status = point.registerMPIType();
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_MPI_DATATYPE_REGISTERED);

	// Cleanup for future tests since we use a static variable!
	status = point.deregisterMPIType();
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
}

// Test 3: Test that if unregistered, we cannot deregister again
BOOST_AUTO_TEST_CASE(registerMPIType_test3, * utf::tolerance(0.00001))
{
	cupcfd::error::eCodes status;
	EuclideanPoint<double,3> point;

	BOOST_CHECK_EQUAL(point.isRegistered(), false);
	status = point.deregisterMPIType();
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_MPI_DATATYPE_UNREGISTERED);
}

// === getMPIType ===
// Test 1: Test that corrected ID is returned for the registered type with error code
BOOST_AUTO_TEST_CASE(getMPIType_test1, * utf::tolerance(0.00001))
{
	cupcfd::error::eCodes status;
	EuclideanPoint<double,3> point;
	MPI_Datatype dType;

	status = point.registerMPIType();
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	status = point.getMPIType(&dType);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	// Cleanup for future tests since we use a static variable!
	status = point.deregisterMPIType();
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
}

// Test 2: Test error case detected when type unregistered
BOOST_AUTO_TEST_CASE(getMPIType_test2, * utf::tolerance(0.00001))
{
	cupcfd::error::eCodes status;
	EuclideanPoint<double,3> point;
	MPI_Datatype dType;

	BOOST_CHECK_EQUAL(point.isRegistered(), false);
	status = point.getMPIType(&dType);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_MPI_DATATYPE_UNREGISTERED);
}

// Test 3: Test corrected ID is returned for method without error code
BOOST_AUTO_TEST_CASE(getMPIType_test3, * utf::tolerance(0.00001))
{

}

// === MPI: Point Broadcast ===
// Test 1: Testing the the MPI Type was setup correctly, and able to broadcast two points
// This should establish that the custom MPI type has the right sizes
BOOST_AUTO_TEST_CASE(MPIPointBroadcast_test1)
{
	cupcfd::error::eCodes status;
	cupcfd::comm::Communicator comm(MPI_COMM_WORLD);

	// Create Two Vectors on Rank 0

	// Register the type (only once per all tests in file since static?)
	EuclideanPoint<double,3> point;

	// Should not yet be registered
	BOOST_CHECK_EQUAL(point.isRegistered(), false);

	// Register
	status = point.registerMPIType();
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	// Should now be registered
	BOOST_CHECK_EQUAL(point.isRegistered(), true);

	if(comm.rank == 0)
	{
		EuclideanPoint<double,3> points[2] = {EuclideanPoint<double,3>(3.5, 6.3, 19.7),
											   EuclideanPoint<double,3>(3.0, 4.5, 6.7)};

		status = cupcfd::comm::Broadcast(points, 2, 0, comm);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	}
	else
	{
		EuclideanPoint<double,3> recvPoints[2];

		status = cupcfd::comm::Broadcast(recvPoints, 2, 0, comm);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

		BOOST_CHECK_EQUAL(recvPoints[0].cmp[0], 3.5);
		BOOST_CHECK_EQUAL(recvPoints[0].cmp[1], 6.3);
		BOOST_CHECK_EQUAL(recvPoints[0].cmp[2], 19.7);

		BOOST_CHECK_EQUAL(recvPoints[1].cmp[0], 3.0);
		BOOST_CHECK_EQUAL(recvPoints[1].cmp[1], 4.5);
		BOOST_CHECK_EQUAL(recvPoints[1].cmp[2], 6.7);
	}

	// Cleanup for future tests since we use a static variable!
	status = point.deregisterMPIType();
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
}

BOOST_AUTO_TEST_CASE(cleanup)
{
    MPI_Finalize();
}

