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

#define BOOST_TEST_MODULE EuclideanVector
#include <boost/test/unit_test.hpp>
#include <boost/test/output_test_stream.hpp>
#include <stdexcept>

#include "EuclideanVector.h"
#include "EuclideanVector3D.h"
#include "Error.h"
#include "Broadcast.h"
#include <iostream>

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
// Test 1: Check all values set to zero - 2D
BOOST_AUTO_TEST_CASE(constructor1_test1)
{
	EuclideanVector<double, 2> vec;

	BOOST_CHECK_EQUAL(vec.cmp[0], 0.0);
	BOOST_CHECK_EQUAL(vec.cmp[1], 0.0);
	BOOST_CHECK_EQUAL(vec.isRegistered(), false);
}

// Test 2: Check all values set to zero - 3D
BOOST_AUTO_TEST_CASE(constructor1_test2)
{
	EuclideanVector<double, 3> vec;

	BOOST_CHECK_EQUAL(vec.cmp[0], 0.0);
	BOOST_CHECK_EQUAL(vec.cmp[1], 0.0);
	BOOST_CHECK_EQUAL(vec.cmp[2], 0.0);
	BOOST_CHECK_EQUAL(vec.isRegistered(), false);
}

// === Constructor2 ===
// Test 1: Check values set to match parameters - 2D
BOOST_AUTO_TEST_CASE(constructor2_test1)
{
	EuclideanVector<double, 2> vec(2.0, 3.0);

	BOOST_CHECK_EQUAL(vec.cmp[0], 2.0);
	BOOST_CHECK_EQUAL(vec.cmp[1], 3.0);
	BOOST_CHECK_EQUAL(vec.isRegistered(), false);
}

// Test 2: Check values set to match parameters - 3D
BOOST_AUTO_TEST_CASE(constructor2_test2)
{
	EuclideanVector<double, 3> vec(2.0, 3.0, 4.0);

	BOOST_CHECK_EQUAL(vec.cmp[0], 2.0);
	BOOST_CHECK_EQUAL(vec.cmp[1], 3.0);
	BOOST_CHECK_EQUAL(vec.cmp[2], 4.0);
	BOOST_CHECK_EQUAL(vec.isRegistered(), false);
}


// === Operator= Vector ===
// Test 1: Check values are appropriately copied form src to dst - 2D
BOOST_AUTO_TEST_CASE(operator_assign_vector_test1)
{
	EuclideanVector<double, 2> srcVec(2.0, 3.0);
	EuclideanVector<double, 2> dstVec;

	dstVec = srcVec;

	srcVec.cmp[1] = -15.0;

	BOOST_CHECK_EQUAL(dstVec.cmp[0], 2.0);
	BOOST_CHECK_EQUAL(dstVec.cmp[1], 3.0);
}

// Test 2: Check values are appropriately copied form src to dst - 3D
BOOST_AUTO_TEST_CASE(operator_assign_vector_test2)
{
	EuclideanVector<double, 3> srcVec(2.0, 3.0, 4.0);
	EuclideanVector<double, 3> dstVec;

	dstVec = srcVec;

	srcVec.cmp[1] = -15.0;

	BOOST_CHECK_EQUAL(dstVec.cmp[0], 2.0);
	BOOST_CHECK_EQUAL(dstVec.cmp[1], 3.0);
	BOOST_CHECK_EQUAL(dstVec.cmp[2], 4.0);
}

// === Operator= Scalar ===
// Test 1: Check values are all set to same as scalar - 2D
BOOST_AUTO_TEST_CASE(operator_assign_scalar_test1)
{
	EuclideanVector<double, 2> vec;

	vec = 5.0;

	BOOST_CHECK_EQUAL(vec.cmp[0], 5.0);
	BOOST_CHECK_EQUAL(vec.cmp[1], 5.0);
}

// Test 2: Check values are all set to same as scalar - 3D
BOOST_AUTO_TEST_CASE(operator_assign_scalar_test2)
{
	EuclideanVector<double, 3> vec;

	vec = 5.0;

	BOOST_CHECK_EQUAL(vec.cmp[0], 5.0);
	BOOST_CHECK_EQUAL(vec.cmp[1], 5.0);
	BOOST_CHECK_EQUAL(vec.cmp[2], 5.0);
}

// === operator== ===
// Test 1: Test that vecs match
BOOST_AUTO_TEST_CASE(operator_equivalence_test1)
{
	EuclideanVector<double, 3> vec(1.0, 2.0, 3.0);
	EuclideanVector<double, 3> vec2(1.0, 2.0, 3.0);

	BOOST_CHECK_EQUAL(vec == vec2, true);
}

// Test 2: Test that vecs don't match
BOOST_AUTO_TEST_CASE(operator_equivalence_test2)
{
	EuclideanVector<double, 3> vec(1.0, 2.0, 3.0);
	EuclideanVector<double, 3> vec2(2.0, 2.0, 3.0);
	EuclideanVector<double, 3> vec3(1.0, 3.0, 3.0);
	EuclideanVector<double, 3> vec4(1.0, 2.0, 4.0);

	BOOST_CHECK_EQUAL(vec == vec2, false);
	BOOST_CHECK_EQUAL(vec == vec3, false);
	BOOST_CHECK_EQUAL(vec == vec4, false);
}

// === Operator+ Vector ===
// Test 1: Check values are appropriately summed - 2D
BOOST_AUTO_TEST_CASE(operator_add_vector_test1)
{
	EuclideanVector<double, 2> vec1(5.0, 8.0);
	EuclideanVector<double, 2> vec2(1.2, 4.5);

	EuclideanVector<double, 2> vec3 = vec1 + vec2;

	BOOST_CHECK_EQUAL(vec3.cmp[0], 6.2);
	BOOST_CHECK_EQUAL(vec3.cmp[1], 12.5);
}

// Test 2: Check values are appropriately summed - 3D
BOOST_AUTO_TEST_CASE(operator_add_vector_test2)
{
	EuclideanVector<double, 3> vec1(5.0, 8.0, 9.5);
	EuclideanVector<double, 3> vec2(1.2, 4.5, 19.3);

	EuclideanVector<double, 3> vec3 = vec1 + vec2;

	BOOST_CHECK_EQUAL(vec3.cmp[0], 6.2);
	BOOST_CHECK_EQUAL(vec3.cmp[1], 12.5);
	BOOST_CHECK_EQUAL(vec3.cmp[2], 28.8);
}

// === Operator+ Scalar ===
// Test 1: Check scalars are appropriately added when vector on left - 2D
BOOST_AUTO_TEST_CASE(operator_add_scalar_test1)
{
	EuclideanVector<double, 2> vec1(5.0, 8.0);
	double scalar = 5.0;

	EuclideanVector<double, 2> vec3 = vec1 + scalar;

	BOOST_CHECK_EQUAL(vec3.cmp[0], 10.0);
	BOOST_CHECK_EQUAL(vec3.cmp[1], 13.0);
}

// Test 2: Check scalars are appropriately added when vector on right - 2D
BOOST_AUTO_TEST_CASE(operator_add_scalar_test2)
{
	EuclideanVector<double, 2> vec1(5.0, 8.0);
	double scalar = 5.0;

	EuclideanVector<double, 2> vec3 = scalar + vec1;

	BOOST_CHECK_EQUAL(vec3.cmp[0], 10.0);
	BOOST_CHECK_EQUAL(vec3.cmp[1], 13.0);
}


// Test 3: Check scalars are appropriately added when vector on left - 3D
BOOST_AUTO_TEST_CASE(operator_add_scalar_test3)
{
	EuclideanVector<double, 3> vec1(5.0, 8.0, 9.5);
	double scalar = 5.0;

	EuclideanVector<double, 3> vec3 = vec1 + scalar;

	BOOST_CHECK_EQUAL(vec3.cmp[0], 10.0);
	BOOST_CHECK_EQUAL(vec3.cmp[1], 13.0);
	BOOST_CHECK_EQUAL(vec3.cmp[2], 14.5);
}

// Test 4: Check scalars are appropriately added when vector on right - 3D
BOOST_AUTO_TEST_CASE(operator_add_scalar_test4)
{
	EuclideanVector<double, 3> vec1(5.0, 8.0, 9.5);
	double scalar = 5.0;

	EuclideanVector<double, 3> vec3 = scalar + vec1;

	BOOST_CHECK_EQUAL(vec3.cmp[0], 10.0);
	BOOST_CHECK_EQUAL(vec3.cmp[1], 13.0);
	BOOST_CHECK_EQUAL(vec3.cmp[2], 14.5);
}


// === Operator- Vector ===
// Test 1: Check values are appropriately subtracted - 2D
BOOST_AUTO_TEST_CASE(operator_subtract_vector_test1)
{
	EuclideanVector<double,2> vec1(5.0, 8.0);
	EuclideanVector<double,2> vec2(1.2, 4.5);

	EuclideanVector<double,2> vec3 = vec1 - vec2;

	BOOST_CHECK_EQUAL(vec3.cmp[0], 3.8);
	BOOST_CHECK_EQUAL(vec3.cmp[1], 3.5);
}


// Test 2: Check values are appropriately subtracted - 3D
BOOST_AUTO_TEST_CASE(operator_subtract_vector_test2)
{
	EuclideanVector<double,3> vec1(5.0, 8.0, 9.5);
	EuclideanVector<double,3> vec2(1.2, 4.5, 19.3);

	EuclideanVector<double,3> vec3 = vec1 - vec2;

	BOOST_CHECK_EQUAL(vec3.cmp[0], 3.8);
	BOOST_CHECK_EQUAL(vec3.cmp[1], 3.5);
	BOOST_CHECK_EQUAL(vec3.cmp[2], -9.8);
}

// === Operator- Scalar ===
// Test 1: Check scalar values are subtracted when vector on left - 2D
BOOST_AUTO_TEST_CASE(operator_subtract_scalar_minus_test1,  * utf::tolerance(0.0000000001))
{
	EuclideanVector<double,2> vec1(5.0, 8.0);
	double scalar = 5.0;

	EuclideanVector<double,2> vec3 = vec1 - scalar;

	BOOST_TEST(vec3.cmp[0] == 0.0);
	BOOST_TEST(vec3.cmp[1] == 3.0);
}

// Test 2: Check scalar values are subtracted when vector on right - 2D
BOOST_AUTO_TEST_CASE(operator_subtract_scalar_minus_test2,  * utf::tolerance(0.0000000001))
{
	EuclideanVector<double,2> vec1(5.0, 8.0);
	double scalar = 5.0;

	EuclideanVector<double,2> vec3 = scalar - vec1;

	BOOST_TEST(vec3.cmp[0] == 0.0);
	BOOST_TEST(vec3.cmp[1] == -3.0);
}


// Test 3: Check scalar values are subtracted when vector on left - 3D
BOOST_AUTO_TEST_CASE(operator_subtract_scalar_minus_test3,  * utf::tolerance(0.0000000001))
{
	EuclideanVector<double,3> vec1(5.0, 8.0, 9.5);
	double scalar = 5.0;

	EuclideanVector<double,3> vec3 = vec1 - scalar;

	BOOST_TEST(vec3.cmp[0] == 0.0);
	BOOST_TEST(vec3.cmp[1] == 3.0);
	BOOST_TEST(vec3.cmp[2] == 4.5);
}

// Test 4: Check scalar values are subtracted when vector on right - 3D
BOOST_AUTO_TEST_CASE(operator_subtract_scalar_minus_test4,  * utf::tolerance(0.0000000001))
{
	EuclideanVector<double,3> vec1(5.0, 8.0, 9.5);
	double scalar = 5.0;

	EuclideanVector<double,3> vec3 = scalar - vec1;

	BOOST_TEST(vec3.cmp[0] == 0.0);
	BOOST_TEST(vec3.cmp[1] == -3.0);
	BOOST_TEST(vec3.cmp[2] == -4.5);
}

// === Operator* Scalar ===
// Test 1: Check scalar values are multiplied when vector on left - 2D
BOOST_AUTO_TEST_CASE(operator_subtract_scalar_multiply_test1,  * utf::tolerance(0.0000000001))
{
	EuclideanVector<double,2> vec1(5.0, 8.0);
	double scalar = 5.0;

	EuclideanVector<double,2> vec3 = vec1 * scalar;

	BOOST_TEST(vec3.cmp[0] == 25.0);
	BOOST_TEST(vec3.cmp[1] == 40.0);
}

// Test 2: Check scalar values are multiplied when vector on right - 2D
BOOST_AUTO_TEST_CASE(operator_subtract_scalar_multiply_test2,  * utf::tolerance(0.0000000001))
{
	EuclideanVector<double,2> vec1(5.0, 8.0);
	double scalar = 5.0;

	EuclideanVector<double,2> vec3 = scalar * vec1;

	BOOST_TEST(vec3.cmp[0] == 25.0);
	BOOST_TEST(vec3.cmp[1] == 40.0);
}

// Test 3: Check scalar values are multiplied when vector on left - 3D
BOOST_AUTO_TEST_CASE(operator_subtract_scalar_multiply_test3,  * utf::tolerance(0.0000000001))
{
	EuclideanVector<double,3> vec1(5.0, 8.0, 9.5);
	double scalar = 5.0;

	EuclideanVector<double,3> vec3 = vec1 * scalar;

	BOOST_TEST(vec3.cmp[0] == 25.0);
	BOOST_TEST(vec3.cmp[1] == 40.0);
	BOOST_TEST(vec3.cmp[2] == 47.5);
}

// Test 4: Check scalar values are multiplied when vector on right - 3D
BOOST_AUTO_TEST_CASE(operator_subtract_scalar_multiply_test4,  * utf::tolerance(0.0000000001))
{
	EuclideanVector<double,3> vec1(5.0, 8.0, 9.5);
	double scalar = 5.0;

	EuclideanVector<double,3> vec3 = scalar * vec1;

	BOOST_TEST(vec3.cmp[0] == 25.0);
	BOOST_TEST(vec3.cmp[1] == 40.0);
	BOOST_TEST(vec3.cmp[2] == 47.5);
}

// === Operator/ Scalar ===
// Test 1: Check scalar values are multiplied when vector on left - 2D
BOOST_AUTO_TEST_CASE(operator_subtract_scalar_divide_test1,  * utf::tolerance(0.0000000001))
{
	EuclideanVector<double,2> vec1(5.0, 10.0);
	double scalar = 5.0;

	EuclideanVector<double,2> vec3 = vec1 / scalar;

	BOOST_TEST(vec3.cmp[0] == 1.0);
	BOOST_TEST(vec3.cmp[1] == 2.0);
}

// Test 2: Check scalar values are multiplied when vector on right - 2D
BOOST_AUTO_TEST_CASE(operator_subtract_scalar_divide_test2,  * utf::tolerance(0.0000000001))
{
	EuclideanVector<double,2> vec1(5.0, 10.0);
	double scalar = 5.0;

	EuclideanVector<double,2> vec3 = scalar / vec1;

	BOOST_TEST(vec3.cmp[0] == 1.0);
	BOOST_TEST(vec3.cmp[1] == 0.5);
}

// Test 3: Check scalar values are multiplied when vector on left - 3D
BOOST_AUTO_TEST_CASE(operator_subtract_scalar_divide_test3,  * utf::tolerance(0.0000000001))
{
	EuclideanVector<double,3> vec1(5.0, 10.0, 9.5);
	double scalar = 5.0;

	EuclideanVector<double,3> vec3 = vec1 / scalar;

	BOOST_TEST(vec3.cmp[0] == 1.0);
	BOOST_TEST(vec3.cmp[1] == 2.0);
	BOOST_TEST(vec3.cmp[2] == 1.9);
}

// Test 4: Check scalar values are multiplied when vector on left - 3D
BOOST_AUTO_TEST_CASE(operator_subtract_scalar_divide_test4,  * utf::tolerance(0.0000000001))
{
	EuclideanVector<double,3> vec1(5.0, 10.0, 9.5);
	double scalar = 5.0;

	EuclideanVector<double,3> vec3 = scalar / vec1;

	BOOST_TEST(vec3.cmp[0] == 1.0);
	BOOST_TEST(vec3.cmp[1] == 0.5);
	BOOST_TEST(vec3.cmp[2] == 0.526315789473684);
}

// === CrossProduct ===

// Test 1: Check Cross-Product is computed correctly - 3D
BOOST_AUTO_TEST_CASE(operator_crossproduct_test1, * utf::tolerance(0.0000000001))
{
	EuclideanVector3D<double> vec1(2.0, 3.0, 4.0);
	EuclideanVector3D<double> vec2(3.5, 6.3, 19.7);
	EuclideanVector3D<double> result;

	result = vec1.crossProduct(vec2);

	BOOST_TEST(result.cmp[0] == 33.9);
	BOOST_TEST(result.cmp[1] == -25.4);
	BOOST_TEST(result.cmp[2] == 2.1);
}

// === rotateXAxisRadian ===
// Test 1: Rotate a vector 1 radian
BOOST_AUTO_TEST_CASE(rotateXAxisRadian_test1, * utf::tolerance(0.00001))
{
	EuclideanVector3D<double> vec(1.4, -3.2, 5.6);

	vec.rotateXAxisRadian(1.0);

	BOOST_TEST(vec.cmp[0] == 1.4);
	BOOST_TEST(vec.cmp[1] == -6.4412);
	BOOST_TEST(vec.cmp[2] == 0.332986);
}

// Test 2: Rotate a vector 90 degrees/1.5708 radians
BOOST_AUTO_TEST_CASE(rotateXAxisRadian_test2, * utf::tolerance(0.00001))
{
	EuclideanVector3D<double> vec(0.0,1.0,0.0);

	vec.rotateXAxisRadian(1.5708);

	BOOST_TEST(vec.cmp[0] == 0.0);
	BOOST_TEST(vec.cmp[1] == 0.0);
	BOOST_TEST(vec.cmp[2] == 1.0);
}

// Test 3: Rotate a vector -90 degrees/-1.5708 radians
BOOST_AUTO_TEST_CASE(rotateXAxisRadian_test3, * utf::tolerance(0.00001))
{
	EuclideanVector3D<double> vec(0.0,1.0,0.0);

	vec.rotateXAxisRadian(-1.5708);

	BOOST_TEST(vec.cmp[0] == 0.0);
	BOOST_TEST(vec.cmp[1] == 0.0);
	BOOST_TEST(vec.cmp[2] == -1.0);
}

// === rotateYAxisRadian ===
// Test 1:
BOOST_AUTO_TEST_CASE(rotateYAxisRadian_test1, * utf::tolerance(0.00001))
{
	EuclideanVector3D<double> vec(1.4, -3.2, 5.6);

	vec.rotateYAxisRadian(1.0);

	BOOST_TEST(vec.cmp[0] == 5.46866);
	BOOST_TEST(vec.cmp[1] == -3.2);
	BOOST_TEST(vec.cmp[2] == 1.84763);
}

// Test 2: Rotate a vector 90 degrees/1.5708 radians
BOOST_AUTO_TEST_CASE(rotateYAxisRadian_test2, * utf::tolerance(0.00001))
{
	EuclideanVector3D<double> vec(1.0,0.0,0.0);

	vec.rotateYAxisRadian(1.5708);

	BOOST_TEST(vec.cmp[0] == 0.0);
	BOOST_TEST(vec.cmp[1] == 0.0);
	BOOST_TEST(vec.cmp[2] == -1.0);
}

// Test 3: Rotate a vector -90 degrees/1.5708 radians
BOOST_AUTO_TEST_CASE(rotateYAxisRadian_test3, * utf::tolerance(0.00001))
{
	EuclideanVector3D<double> vec(1.0,0.0,0.0);

	vec.rotateYAxisRadian(-1.5708);

	BOOST_TEST(vec.cmp[0] == 0.0);
	BOOST_TEST(vec.cmp[1] == 0.0);
	BOOST_TEST(vec.cmp[2] == 1.0);
}

// === rotateZAxisRadian ===
// Test 1:
BOOST_AUTO_TEST_CASE(rotateZAxisRadian_test1, * utf::tolerance(0.00001))
{
	EuclideanVector3D<double> vec(1.4, 3.2, 5.6);

	vec.rotateZAxisRadian(1.0);

	BOOST_TEST(vec.cmp[0] == -1.93629);
	BOOST_TEST(vec.cmp[1] == 2.90703);
	BOOST_TEST(vec.cmp[2] == 5.6);
}

// Test 2: Rotate a unit vector
BOOST_AUTO_TEST_CASE(rotateZAxisRadian_test2, * utf::tolerance(0.00001))
{
	EuclideanVector3D<double> vec(1.0,0.0,0.0);

	vec.rotateZAxisRadian(1.5708);

	// Vector was pointing along X axis, should now be pointing up along Y-axis
	BOOST_TEST(vec.cmp[0] == 0.0);
	BOOST_TEST(vec.cmp[1] == 1.0);
	BOOST_TEST(vec.cmp[2] == 0.0);
}

// Test 3: Rotate a unit vector
BOOST_AUTO_TEST_CASE(rotateZAxisRadian_test3, * utf::tolerance(0.00001))
{
	EuclideanVector3D<double> vec(1.0,0.0,0.0);

	vec.rotateZAxisRadian(-1.5708);

	// Vector was pointing along X axis, should now be pointing up along Y-axis
	BOOST_TEST(vec.cmp[0] == 0.0);
	BOOST_TEST(vec.cmp[1] == -1.0);
	BOOST_TEST(vec.cmp[2] == 0.0);
}

// === isParallel ===
// Test 1: Test that parallel vectors detected - 2D
BOOST_AUTO_TEST_CASE(isParallel_test1, * utf::tolerance(0.0000000001))
{
	EuclideanVector<double,2> vec1(2.0, 3.0);
	EuclideanVector<double,2> vec2(4.0, 6.0);

	bool isParallel = vec1.isParallel(vec2);
	BOOST_CHECK_EQUAL(isParallel, true);
}

// Test 2: Test that parallel vectors detected even with inverted sign - 2D
BOOST_AUTO_TEST_CASE(isParallel_test2, * utf::tolerance(0.0000000001))
{
	EuclideanVector<double,2> vec1(2.0, 3.0);
	EuclideanVector<double,2> vec2(-4.0, -6.0);

	bool isParallel = vec1.isParallel(vec2);
	BOOST_CHECK_EQUAL(isParallel, true);
}

// Test 3: Test that parallel vectors detected - 3D
BOOST_AUTO_TEST_CASE(isParallel_test3, * utf::tolerance(0.0000000001))
{
	EuclideanVector<double,3> vec1(2.0, 3.0, 4.0);
	EuclideanVector<double,3> vec2(4.0, 6.0, 8.0);

	bool isParallel = vec1.isParallel(vec2);
	BOOST_CHECK_EQUAL(isParallel, true);
}

// Test 4: Test that non-parallel vectors identified correctly - 2D
BOOST_AUTO_TEST_CASE(isParallel_test4, * utf::tolerance(0.0000000001))
{
	EuclideanVector<double,2> vec1(2.0, 3.0);
	EuclideanVector<double,2> vec2(2.0, 3.5);

	bool isParallel = vec1.isParallel(vec2);
	BOOST_CHECK_EQUAL(isParallel, false);
}

// Test 5: Test that non-parallel vectors identified correctly - 3D
BOOST_AUTO_TEST_CASE(isParallel_test5, * utf::tolerance(0.0000000001))
{
	EuclideanVector<double,3> vec1(2.0, 3.0, 7.6);
	EuclideanVector<double,3> vec2(4.0, 6.0, 8.9);

	bool isParallel = vec1.isParallel(vec2);
	BOOST_CHECK_EQUAL(isParallel, false);
}

// Test 6: Test that parallel vectors detected even if first component is zero - 3D
BOOST_AUTO_TEST_CASE(isParallel_test6, * utf::tolerance(0.0000000001))
{
	EuclideanVector<double,3> vec1(0.0, 3.0, 4.0);
	EuclideanVector<double,3> vec2(0.0, 6.0, 8.0);

	bool isParallel = vec1.isParallel(vec2);
	BOOST_CHECK_EQUAL(isParallel, true);
}

// Test 7: Test that non-parallel vectors detected even if first component is zero - 3D
BOOST_AUTO_TEST_CASE(isParallel_test7, * utf::tolerance(0.0000000001))
{
	EuclideanVector<double,3> vec1(0.0, 3.0, 7.6);
	EuclideanVector<double,3> vec2(0.0, 6.0, 8.9);

	bool isParallel = vec1.isParallel(vec2);
	BOOST_CHECK_EQUAL(isParallel, false);
}

// Test 8: Test that parallel vectors detected even if non-first component is zero - 3D
BOOST_AUTO_TEST_CASE(isParallel_test8, * utf::tolerance(0.0000000001))
{
	EuclideanVector<double,3> vec1(2.0, 0.0, 4.0);
	EuclideanVector<double,3> vec2(4.0, 0.0, 8.0);

	bool isParallel = vec1.isParallel(vec2);
	BOOST_CHECK_EQUAL(isParallel, true);
}

// Test 9: Test that non-parallel vectors detected even if non-first component is zero - 3D
BOOST_AUTO_TEST_CASE(isParallel_test9, * utf::tolerance(0.0000000001))
{
	EuclideanVector<double,3> vec1(2.0, 0.0, 7.6);
	EuclideanVector<double,3> vec2(4.0, 0.0, 8.9);

	bool isParallel = vec1.isParallel(vec2);
	BOOST_CHECK_EQUAL(isParallel, false);
}

// === isPointOnLine ===
// Test 1: Test that point is detected on the line
BOOST_AUTO_TEST_CASE(isPointOnLine_test1, * utf::tolerance(0.0000000001))
{
	EuclideanPoint<double,3> x1(2.0, 3.0, 5.0);
	EuclideanPoint<double,3> x2(4.0, 6.0, 10.0);
	EuclideanPoint<double,3> point(3.0, 4.5, 7.5);

	bool isPointOnLine = cupcfd::geometry::euclidean::isPointOnLine(x1, x2, point);
	BOOST_CHECK_EQUAL(isPointOnLine, true);
}

// Test 2: Test that x1 is detected as on the line
BOOST_AUTO_TEST_CASE(isPointOnLine_test2, * utf::tolerance(0.0000000001))
{
	EuclideanPoint<double,3> x1(2.0, 3.0, 5.0);
	EuclideanPoint<double,3> x2(4.0, 6.0, 10.0);

	bool isPointOnLine = cupcfd::geometry::euclidean::isPointOnLine(x1, x2, x1);
	BOOST_CHECK_EQUAL(isPointOnLine, true);
}

// Test 3: Test that x2 is detected as on the line
BOOST_AUTO_TEST_CASE(isPointOnLine_test3, * utf::tolerance(0.0000000001))
{
	EuclideanPoint<double,3> x1(2.0, 3.0, 5.0);
	EuclideanPoint<double,3> x2(4.0, 6.0, 10.0);

	bool isPointOnLine = cupcfd::geometry::euclidean::isPointOnLine(x1, x2, x2);
	BOOST_CHECK_EQUAL(isPointOnLine, true);
}

// Test 4: Test that a point not on the line is detected as such
BOOST_AUTO_TEST_CASE(isPointOnLine_test4, * utf::tolerance(0.0000000001))
{
	EuclideanPoint<double,3> x1(2.0, 3.0, 5.0);
	EuclideanPoint<double,3> x2(4.0, 6.0, 10.0);
	EuclideanPoint<double,3> point(2.0, 3.0, 8.12);

	bool isPointOnLine = cupcfd::geometry::euclidean::isPointOnLine(x1, x2, point);
	BOOST_CHECK_EQUAL(isPointOnLine, false);
}

// Test 1: Test that point is detected on the line when there is a zero component to the vector (i.e. on a axis plane)
BOOST_AUTO_TEST_CASE(isPointOnLine_test5, * utf::tolerance(0.0000000001))
{
	EuclideanPoint<double,3> x1(2.0, 3.0, 5.0);
	EuclideanPoint<double,3> x2(2.0, 6.0, 10.0);
	EuclideanPoint<double,3> point(2.0, 4.5, 7.5);

	bool isPointOnLine = cupcfd::geometry::euclidean::isPointOnLine(x1, x2, point);
	BOOST_CHECK_EQUAL(isPointOnLine, true);
}

// === isVectorRangeIntersection (3D) ===
// Test 1: Test that two vectors are detected as intersecting - 3D
BOOST_AUTO_TEST_CASE(isVectorRangeIntersection_test1, * utf::tolerance(0.0000000001))
{
	EuclideanPoint<double,3> x1(3.0, 4.0, 5.0);
	EuclideanPoint<double,3> x2(7.0, 8.0, 6.0);
	EuclideanPoint<double,3> x3(7.0, 4.0, 5.0);
	EuclideanPoint<double,3> x4(3.0, 8.0, 6.0);

	// Should Intersect at 5.0, 6.0, 5.5
	bool intersect = cupcfd::geometry::euclidean::isVectorRangeIntersection(x1, x2, x3, x4);
	BOOST_CHECK_EQUAL(intersect, true);
}

// Test 2: Test two vectors that are not coplanar so don't intersect - 3D
BOOST_AUTO_TEST_CASE(isVectorRangeIntersection_test2, * utf::tolerance(0.0000000001))
{

	EuclideanPoint<double,3> x1(3.0, 4.0, 5.0);
	EuclideanPoint<double,3> x2(7.0, 8.0, 6.0);
	EuclideanPoint<double,3> x3(7.0, 4.0, 1.3);
	EuclideanPoint<double,3> x4(3.0, 8.0, 3.4);

	bool intersect = cupcfd::geometry::euclidean::isVectorRangeIntersection(x1, x2, x3, x4);

	BOOST_CHECK_EQUAL(intersect, false);
}

// Test 3: Test that two vectors would intersect, but don't because the range of vector 1 is too short - 3D
BOOST_AUTO_TEST_CASE(isVectorRangeIntersection_test3, * utf::tolerance(0.0000000001))
{
	EuclideanPoint<double,3> x1(3.0, 4.0, 5.0);
	EuclideanPoint<double,3> x2(4.0, 5.0, 5.25);
	EuclideanPoint<double,3> x3(7.0, 4.0, 5.0);
	EuclideanPoint<double,3> x4(3.0, 8.0, 6.0);

	bool intersect = cupcfd::geometry::euclidean::isVectorRangeIntersection(x1, x2, x3, x4);
	BOOST_CHECK_EQUAL(intersect, false);
}

// Test 4: Test that two vectors would intersect, but don't because the range of vector 2 is too short - 3D
BOOST_AUTO_TEST_CASE(isVectorRangeIntersection_test4, * utf::tolerance(0.0000000001))
{
	EuclideanPoint<double,3> x1(3.0, 4.0, 5.0);
	EuclideanPoint<double,3> x2(7.0, 8.0, 6.0);
	EuclideanPoint<double,3> x3(4.0, 7.0, 5.75);
	EuclideanPoint<double,3> x4(3.0, 8.0, 6.0);

	bool intersect = cupcfd::geometry::euclidean::isVectorRangeIntersection(x1, x2, x3, x4);
	BOOST_CHECK_EQUAL(intersect, false);
}

// Test 5: Test two vectors that are coplanar but the intersect point is outside both ranges
BOOST_AUTO_TEST_CASE(isVectorRangeIntersection_test5, * utf::tolerance(0.0000000001))
{
	EuclideanPoint<double,3> x1(3.0, 4.0, 5.0);
	EuclideanPoint<double,3> x2(4.0, 5.0, 5.25);
	EuclideanPoint<double,3> x3(4.0, 7.0, 5.75);
	EuclideanPoint<double,3> x4(3.0, 8.0, 6.0);

	bool intersect = cupcfd::geometry::euclidean::isVectorRangeIntersection(x1, x2, x3, x4);
	BOOST_CHECK_EQUAL(intersect, false);
}

// Test 6: Test that two lines that are colinear but don't overlap so don't intersect - 3D
BOOST_AUTO_TEST_CASE(isVectorRangeIntersection_test6, * utf::tolerance(0.0000000001))
{
	EuclideanPoint<double,3> x1(3.0, 4.0, 5.0);
	EuclideanPoint<double,3> x2(4.0, 5.0, 6.0);

	EuclideanPoint<double,3> x3(9.0, 10.0, 11.0);
	EuclideanPoint<double,3> x4(15.0, 16.0, 17.0);

	bool intersect = cupcfd::geometry::euclidean::isVectorRangeIntersection(x1, x2, x3, x4);
	BOOST_CHECK_EQUAL(intersect, false);
}

// Test 7: Test that two lines that are colinear and do overlap are considered as intersecting - 3D
BOOST_AUTO_TEST_CASE(isVectorRangeIntersection_test7, * utf::tolerance(0.0000000001))
{
	EuclideanPoint<double,3> x1(3.0, 4.0, 5.0);
	EuclideanPoint<double,3> x2(4.0, 5.0, 6.0);

	EuclideanPoint<double,3> x3(3.5, 4.5, 5.5);
	EuclideanPoint<double,3> x4(15.0, 16.0, 17.0);

	bool intersect = cupcfd::geometry::euclidean::isVectorRangeIntersection(x1, x2, x3, x4);
	BOOST_CHECK_EQUAL(intersect, true);
}

// Test 8: Test two vectors that are parallel but never overlap
BOOST_AUTO_TEST_CASE(isVectorRangeIntersection_test8, * utf::tolerance(0.0000000001))
{
	EuclideanPoint<double,3> x1(3.0, 4.0, 5.0);
	EuclideanPoint<double,3> x2(4.0, 5.0, 6.0);

	EuclideanPoint<double,3> x3(6.0, 7.8, 2.3);
	EuclideanPoint<double,3> x4(7.0, 8.8, 3.3);

	bool intersect = cupcfd::geometry::euclidean::isVectorRangeIntersection(x1, x2, x3, x4);
	BOOST_CHECK_EQUAL(intersect, false);
}

// Test 9: Test that two lines that are colinear but don't overlap so don't intersect (one is in opposite direction) - 3D
// ToDo: This test case fails: Detects as colinear but thinks they overlap?
BOOST_AUTO_TEST_CASE(isVectorRangeIntersection_test9, * utf::tolerance(0.0000000001))
{
	EuclideanPoint<double,3> x1(3.0, 4.0, 5.0);
	EuclideanPoint<double,3> x2(4.0, 5.0, 6.0);

	EuclideanPoint<double,3> x3(15.0, 16.0, 17.0);
	EuclideanPoint<double,3> x4(9.0, 10.0, 11.0);

	bool intersect = cupcfd::geometry::euclidean::isVectorRangeIntersection(x1, x2, x3, x4);
	BOOST_CHECK_EQUAL(intersect, false);
}

// Test 10: Test that two lines that are colinear and do overlap are considered as intersecting (one is in opposite direction) - 3D
BOOST_AUTO_TEST_CASE(isVectorRangeIntersection_test10, * utf::tolerance(0.0000000001))
{
	EuclideanPoint<double,3> x1(3.0, 4.0, 5.0);
	EuclideanPoint<double,3> x2(4.0, 5.0, 6.0);

	EuclideanPoint<double,3> x3(15.0, 16.0, 17.0);
	EuclideanPoint<double,3> x4(3.5, 4.5, 5.5);

	bool intersect = cupcfd::geometry::euclidean::isVectorRangeIntersection(x1, x2, x3, x4);
	BOOST_CHECK_EQUAL(intersect, true);
}

// Test 11: Both ranges are just the same point
BOOST_AUTO_TEST_CASE(isVectorRangeIntersection_test11, * utf::tolerance(0.0000000001))
{
	EuclideanPoint<double,3> x1(3.0, 4.0, 5.0);
	EuclideanPoint<double,3> x2(3.0, 4.0, 5.0);

	EuclideanPoint<double,3> x3(3.0, 4.0, 5.0);
	EuclideanPoint<double,3> x4(3.0, 4.0, 5.0);

	bool intersect = cupcfd::geometry::euclidean::isVectorRangeIntersection(x1, x2, x3, x4);
	BOOST_CHECK_EQUAL(intersect, true);
}

// Test 12: Both ranges are just points, but not the same point
BOOST_AUTO_TEST_CASE(isVectorRangeIntersection_test12, * utf::tolerance(0.0000000001))
{
	EuclideanPoint<double,3> x1(3.0, 4.0, 5.0);
	EuclideanPoint<double,3> x2(3.0, 4.0, 5.0);

	EuclideanPoint<double,3> x3(4.0, 5.0, 7.0);
	EuclideanPoint<double,3> x4(4.0, 5.0, 7.0);

	bool intersect = cupcfd::geometry::euclidean::isVectorRangeIntersection(x1, x2, x3, x4);
	BOOST_CHECK_EQUAL(intersect, false);
}

// Test 13: Range 1 is a point, Range 2 is a line, the point does not lie on the line
BOOST_AUTO_TEST_CASE(isVectorRangeIntersection_test13, * utf::tolerance(0.0000000001))
{

}

// Test 14: Range 1 is a point, Range 2 is a line, the point does lie on the line
BOOST_AUTO_TEST_CASE(isVectorRangeIntersection_test14, * utf::tolerance(0.0000000001))
{

}

// Test 15: Range 2 is a point, Range 1 is a line, the point does not lie on the line
BOOST_AUTO_TEST_CASE(isVectorRangeIntersection_test15, * utf::tolerance(0.0000000001))
{

}

// Test 16: Range 2 is a point, Range 1 is a line, the point does lie on the line
BOOST_AUTO_TEST_CASE(isVectorRangeIntersection_test16, * utf::tolerance(0.0000000001))
{

}

// === computeVectorRangeIntersection ===
// Test 1: Test that two vectors are detected as intersecting - 3D
BOOST_AUTO_TEST_CASE(computeVectorRangeIntersection_test1, * utf::tolerance(0.0000000001))
{
	EuclideanPoint<double,3> x1(3.0, 4.0, 5.0);
	EuclideanPoint<double,3> x2(7.0, 8.0, 6.0);
	EuclideanPoint<double,3> x3(7.0, 4.0, 5.0);
	EuclideanPoint<double,3> x4(3.0, 8.0, 6.0);
	EuclideanPoint<double,3> intersectPoint;

	// Should Intersect at 5.0, 6.0, 5.5
	cupcfd::error::eCodes status = cupcfd::geometry::euclidean::computeVectorRangeIntersection(x1, x2, x3, x4, intersectPoint);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	BOOST_TEST(intersectPoint.cmp[0] == 5.0);
	BOOST_TEST(intersectPoint.cmp[1] == 6.0);
	BOOST_TEST(intersectPoint.cmp[2] == 5.5);
}

// Test 2: Test two vectors that are not coplanar so don't intersect - 3D
BOOST_AUTO_TEST_CASE(computeVectorRangeIntersection_test2, * utf::tolerance(0.0000000001))
{

	EuclideanPoint<double,3> x1(3.0, 4.0, 5.0);
	EuclideanPoint<double,3> x2(7.0, 8.0, 6.0);
	EuclideanPoint<double,3> x3(7.0, 4.0, 1.3);
	EuclideanPoint<double,3> x4(3.0, 8.0, 3.4);
	EuclideanPoint<double,3> intersectPoint;

	cupcfd::error::eCodes status = cupcfd::geometry::euclidean::computeVectorRangeIntersection(x1, x2, x3, x4, intersectPoint);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_GEOMETRY_NO_INTERSECT);
}

// Test 3: Test that two vectors would intersect, but don't because the range of vector 1 is too short - 3D
BOOST_AUTO_TEST_CASE(computeVectorRangeIntersection_test3, * utf::tolerance(0.0000000001))
{
	EuclideanPoint<double,3> x1(3.0, 4.0, 5.0);
	EuclideanPoint<double,3> x2(4.0, 5.0, 5.25);
	EuclideanPoint<double,3> x3(7.0, 4.0, 5.0);
	EuclideanPoint<double,3> x4(3.0, 8.0, 6.0);
	EuclideanPoint<double,3> intersectPoint;

	cupcfd::error::eCodes status = cupcfd::geometry::euclidean::computeVectorRangeIntersection(x1, x2, x3, x4, intersectPoint);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_GEOMETRY_NO_INTERSECT);
}

// Test 4: Test that two vectors would intersect, but don't because the range of vector 2 is too short - 3D
BOOST_AUTO_TEST_CASE(computeVectorRangeIntersection_test4, * utf::tolerance(0.0000000001))
{
	EuclideanPoint<double,3> x1(3.0, 4.0, 5.0);
	EuclideanPoint<double,3> x2(7.0, 8.0, 6.0);
	EuclideanPoint<double,3> x3(4.0, 7.0, 5.75);
	EuclideanPoint<double,3> x4(3.0, 8.0, 6.0);
	EuclideanPoint<double,3> intersectPoint;

	cupcfd::error::eCodes status = cupcfd::geometry::euclidean::computeVectorRangeIntersection(x1, x2, x3, x4, intersectPoint);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_GEOMETRY_NO_INTERSECT);
}

// Test 5: Test two vectors that are coplanar but the intersect point is outside both ranges
BOOST_AUTO_TEST_CASE(computeVectorRangeIntersection_test5, * utf::tolerance(0.0000000001))
{
	EuclideanPoint<double,3> x1(3.0, 4.0, 5.0);
	EuclideanPoint<double,3> x2(4.0, 5.0, 5.25);
	EuclideanPoint<double,3> x3(4.0, 7.0, 5.75);
	EuclideanPoint<double,3> x4(3.0, 8.0, 6.0);
	EuclideanPoint<double,3> intersectPoint;

	cupcfd::error::eCodes status = cupcfd::geometry::euclidean::computeVectorRangeIntersection(x1, x2, x3, x4, intersectPoint);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_GEOMETRY_NO_INTERSECT);
}

// Test 6: Test that two lines that are colinear but don't overlap so don't intersect - 3D
BOOST_AUTO_TEST_CASE(computeVectorRangeIntersection_test6, * utf::tolerance(0.0000000001))
{
	EuclideanPoint<double,3> x1(3.0, 4.0, 5.0);
	EuclideanPoint<double,3> x2(4.0, 5.0, 6.0);
	EuclideanPoint<double,3> x3(9.0, 10.0, 11.0);
	EuclideanPoint<double,3> x4(15.0, 16.0, 17.0);
	EuclideanPoint<double,3> intersectPoint;

	cupcfd::error::eCodes status = cupcfd::geometry::euclidean::computeVectorRangeIntersection(x1, x2, x3, x4, intersectPoint);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_GEOMETRY_NO_INTERSECT);
}

// Test 7: Test that two lines that are colinear and do overlap are considered as intersecting - 3D
BOOST_AUTO_TEST_CASE(computeVectorRangeIntersection_test7, * utf::tolerance(0.0000000001))
{
	EuclideanPoint<double,3> x1(3.0, 4.0, 5.0);
	EuclideanPoint<double,3> x2(4.0, 5.0, 6.0);
	EuclideanPoint<double,3> x3(3.5, 4.5, 5.5);
	EuclideanPoint<double,3> x4(15.0, 16.0, 17.0);
	EuclideanPoint<double,3> intersectPoint;

	cupcfd::error::eCodes status = cupcfd::geometry::euclidean::computeVectorRangeIntersection(x1, x2, x3, x4, intersectPoint);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	BOOST_TEST(intersectPoint.cmp[0] == 3.5);
	BOOST_TEST(intersectPoint.cmp[1] == 4.5);
	BOOST_TEST(intersectPoint.cmp[2] == 5.5);
}

// Test 8: Test two vectors that are parallel but never overlap
BOOST_AUTO_TEST_CASE(computeVectorRangeIntersection_test8, * utf::tolerance(0.0000000001))
{
	EuclideanPoint<double,3> x1(3.0, 4.0, 5.0);
	EuclideanPoint<double,3> x2(4.0, 5.0, 6.0);
	EuclideanPoint<double,3> x3(6.0, 7.8, 2.3);
	EuclideanPoint<double,3> x4(7.0, 8.8, 3.3);
	EuclideanPoint<double,3> intersectPoint;

	cupcfd::error::eCodes status = cupcfd::geometry::euclidean::computeVectorRangeIntersection(x1, x2, x3, x4, intersectPoint);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_GEOMETRY_NO_INTERSECT);
}

// Test 9: Test that two lines that are colinear but don't overlap so don't intersect (one is in opposite direction) - 3D
// ToDo: This test case fails: Detects as colinear but thinks they overlap?
BOOST_AUTO_TEST_CASE(computeVectorRangeIntersection_test9, * utf::tolerance(0.0000000001))
{
	EuclideanPoint<double,3> x1(3.0, 4.0, 5.0);
	EuclideanPoint<double,3> x2(4.0, 5.0, 6.0);
	EuclideanPoint<double,3> x3(15.0, 16.0, 17.0);
	EuclideanPoint<double,3> x4(9.0, 10.0, 11.0);
	EuclideanPoint<double,3> intersectPoint;

	cupcfd::error::eCodes status = cupcfd::geometry::euclidean::computeVectorRangeIntersection(x1, x2, x3, x4, intersectPoint);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_GEOMETRY_NO_INTERSECT);
}

// Test 10: Test that two lines that are colinear and do overlap are considered as intersecting (one is in opposite direction) - 3D
BOOST_AUTO_TEST_CASE(computeVectorRangeIntersection_test10, * utf::tolerance(0.0000000001))
{
	EuclideanPoint<double,3> x1(3.0, 4.0, 5.0);
	EuclideanPoint<double,3> x2(4.0, 5.0, 6.0);
	EuclideanPoint<double,3> x3(15.0, 16.0, 17.0);
	EuclideanPoint<double,3> x4(3.5, 4.5, 5.5);
	EuclideanPoint<double,3> intersectPoint;

	cupcfd::error::eCodes status = cupcfd::geometry::euclidean::computeVectorRangeIntersection(x1, x2, x3, x4, intersectPoint);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	BOOST_TEST(intersectPoint.cmp[0] == 3.5);
	BOOST_TEST(intersectPoint.cmp[1] == 4.5);
	BOOST_TEST(intersectPoint.cmp[2] == 5.5);
}

// Test 11: Both ranges are just the same point
BOOST_AUTO_TEST_CASE(computeVectorRangeIntersection_test11, * utf::tolerance(0.0000000001))
{
	EuclideanPoint<double,3> x1(3.0, 4.0, 5.0);
	EuclideanPoint<double,3> x2(3.0, 4.0, 5.0);
	EuclideanPoint<double,3> x3(3.0, 4.0, 5.0);
	EuclideanPoint<double,3> x4(3.0, 4.0, 5.0);
	EuclideanPoint<double,3> intersectPoint;

	cupcfd::error::eCodes status = cupcfd::geometry::euclidean::computeVectorRangeIntersection(x1, x2, x3, x4, intersectPoint);
	BOOST_TEST(intersectPoint.cmp[0] == 3.0);
	BOOST_TEST(intersectPoint.cmp[1] == 4.0);
	BOOST_TEST(intersectPoint.cmp[2] == 5.0);
}

// Test 12: Both ranges are just points, but not the same point
BOOST_AUTO_TEST_CASE(computeVectorRangeIntersection_test12, * utf::tolerance(0.0000000001))
{
	EuclideanPoint<double,3> x1(3.0, 4.0, 5.0);
	EuclideanPoint<double,3> x2(3.0, 4.0, 5.0);
	EuclideanPoint<double,3> x3(4.0, 5.0, 7.0);
	EuclideanPoint<double,3> x4(4.0, 5.0, 7.0);
	EuclideanPoint<double,3> intersectPoint;

	cupcfd::error::eCodes status = cupcfd::geometry::euclidean::computeVectorRangeIntersection(x1, x2, x3, x4, intersectPoint);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_GEOMETRY_NO_INTERSECT);
}

// Test 13: Range 1 is a point, Range 2 is a line, the point does not lie on the line
BOOST_AUTO_TEST_CASE(computeVectorRangeIntersection_test13, * utf::tolerance(0.0000000001))
{

}

// Test 14: Range 1 is a point, Range 2 is a line, the point does lie on the line
BOOST_AUTO_TEST_CASE(computeVectorRangeIntersection_test14, * utf::tolerance(0.0000000001))
{

}

// Test 15: Range 2 is a point, Range 1 is a line, the point does not lie on the line
BOOST_AUTO_TEST_CASE(computeVectorRangeIntersection_test15, * utf::tolerance(0.0000000001))
{

}

// Test 16: Range 2 is a point, Range 1 is a line, the point does lie on the line
BOOST_AUTO_TEST_CASE(computeVectorRangeIntersection_test16, * utf::tolerance(0.0000000001))
{

}

// Test 17: Correct intersection when higher range is listed first in overlapping colinear setup of oposite directions
BOOST_AUTO_TEST_CASE(computeVectorRangeIntersection_test17, * utf::tolerance(0.0000000001))
{
	EuclideanPoint<double,3> x1(15.0, 16.0, 17.0);
	EuclideanPoint<double,3> x2(3.5, 4.5, 5.5);
	EuclideanPoint<double,3> x3(3.0, 4.0, 5.0);
	EuclideanPoint<double,3> x4(4.0, 5.0, 6.0);
	EuclideanPoint<double,3> intersectPoint;

	cupcfd::error::eCodes status = cupcfd::geometry::euclidean::computeVectorRangeIntersection(x1, x2, x3, x4, intersectPoint);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	BOOST_TEST(intersectPoint.cmp[0] == 3.5);
	BOOST_TEST(intersectPoint.cmp[1] == 4.5);
	BOOST_TEST(intersectPoint.cmp[2] == 5.5);
}

// Test 18: Correct intersection when higher range is listed first in overlapping colinear setup
BOOST_AUTO_TEST_CASE(computeVectorRangeIntersection_test18, * utf::tolerance(0.0000000001))
{
	EuclideanPoint<double,3> x1(3.5, 4.5, 5.5);
	EuclideanPoint<double,3> x2(15.0, 16.0, 17.0);
	EuclideanPoint<double,3> x3(3.0, 4.0, 5.0);
	EuclideanPoint<double,3> x4(4.0, 5.0, 6.0);
	EuclideanPoint<double,3> intersectPoint;

	cupcfd::error::eCodes status = cupcfd::geometry::euclidean::computeVectorRangeIntersection(x1, x2, x3, x4, intersectPoint);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	BOOST_TEST(intersectPoint.cmp[0] == 4.0);
	BOOST_TEST(intersectPoint.cmp[1] == 5.0);
	BOOST_TEST(intersectPoint.cmp[2] == 6.0);
}

// === DotProduct ===

// Test 1: Test correct dot product - 2D
BOOST_AUTO_TEST_CASE(dotproduct_test1, * utf::tolerance(0.01))
{
	EuclideanVector<double,2> vec1(2.0, 3.0);
	EuclideanVector<double,2> vec2(3.5, 6.3);
	double result = vec1.dotProduct(vec2);

	BOOST_TEST(result == 25.9);
}

// Test 2: Test correct dot product - 3D
BOOST_AUTO_TEST_CASE(dotproduct_test2, * utf::tolerance(0.01))
{
	EuclideanVector<double,3> vec1(2.0, 3.0, 4.0);
	EuclideanVector<double,3> vec2(3.5, 6.3, 19.7);
	double result = vec1.dotProduct(vec2);

	BOOST_TEST(result == 104.7);
}

// === length ===
// Test 1: Test correct length compute, double - 2D
BOOST_AUTO_TEST_CASE(length_test1, * utf::tolerance(0.00001))
{
	EuclideanVector<double,2> vec1(3.5, 6.3);
	double length = vec1.length();
	BOOST_TEST(length == 7.20694);
}

// Test 2: Test correct length compute, double - 3D
BOOST_AUTO_TEST_CASE(length_test2, * utf::tolerance(0.0001))
{
	EuclideanVector<double,3> vec1(3.5, 6.3, 19.7);
	double length = vec1.length();
	BOOST_TEST(length == 20.9769);
}

// === normalise ===
// Test 1: Test in-place normalisation - 2D
BOOST_AUTO_TEST_CASE(normalise_test1, * utf::tolerance(0.000001))
{
	// Setup
	EuclideanVector<double,2> vec(3.5, 6.3);
	vec.normalise();
	BOOST_TEST(vec.cmp[0] == 0.485643);
	BOOST_TEST(vec.cmp[1] == 0.874157);
}

// Test 2: Test in-place normalisation - 3D
BOOST_AUTO_TEST_CASE(normalise_test2, * utf::tolerance(0.00001))
{
	// Setup
	EuclideanVector<double,3> vec(3.5, 6.3, 19.7);
	vec.normalise();
	BOOST_TEST(vec.cmp[0] == 0.16685);
	BOOST_TEST(vec.cmp[1] == 0.30033);
	BOOST_TEST(vec.cmp[2] == 0.939129);
}

// === computeOrthagonalVector ===
// Test 1: Compute Orthagonal Vector
BOOST_AUTO_TEST_CASE(computeOrthagonalVector_test1, * utf::tolerance(0.00001))
{
	EuclideanVector<double,3> vec1(3.5, 6.3, 19.7);

	EuclideanVector<double,3> vec2 = vec1.computeOrthagonalVector();

	// Since the solution could technically be one of many things, test using dot product
	double dotProd = vec1.dotProduct(vec2);
	BOOST_TEST(dotProd == 0.0);
}

// Test 2: Compute Orthagonal Vector when first component of vector is Zero
// (I.e. should use another position as the unknown coefficient)
BOOST_AUTO_TEST_CASE(computeOrthagonalVector_test2, * utf::tolerance(0.00001))
{
	EuclideanVector<double,3> vec1(0.0, 6.3, 19.7);

	EuclideanVector<double,3> vec2 = vec1.computeOrthagonalVector();

	// Since the solution could technically be one of many things, test using dot product
	double dotProd = vec1.dotProduct(vec2);
	BOOST_TEST(dotProd == 0.0);
}

// === isRegistered ===
// Not sure quite where to/when to place this test....
// Should use a setter/getter to set/get registered?

// === register and deregisterMPIType ===
// Test 1: Test that Type is registered without error, and can be deregistered without error
BOOST_AUTO_TEST_CASE(registerMPIType_test1, * utf::tolerance(0.00001))
{
	cupcfd::error::eCodes status;
	EuclideanVector<double,3> vec;

	status = vec.registerMPIType();
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	status = vec.deregisterMPIType();
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
}

// Test 2: Test that if it is already registered, it does not register again
BOOST_AUTO_TEST_CASE(registerMPIType_test2, * utf::tolerance(0.00001))
{
	cupcfd::error::eCodes status;
	EuclideanVector<double,3> vec;

	status = vec.registerMPIType();
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	status = vec.registerMPIType();
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_MPI_DATATYPE_REGISTERED);

	// Cleanup for future tests since we use a static variable!
	status = vec.deregisterMPIType();
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
}

// Test 3: Test that if unregistered, we cannot deregister again
BOOST_AUTO_TEST_CASE(registerMPIType_test3, * utf::tolerance(0.00001))
{
	cupcfd::error::eCodes status;
	EuclideanVector<double,3> vec;

	BOOST_CHECK_EQUAL(vec.isRegistered(), false);
	status = vec.deregisterMPIType();
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_MPI_DATATYPE_UNREGISTERED);
}

// === getMPIType ===
// Test 1: Test that corrected ID is returned for the registered type with error code
BOOST_AUTO_TEST_CASE(getMPIType_test1, * utf::tolerance(0.00001))
{
	cupcfd::error::eCodes status;
	EuclideanVector<double,3> vec;
	MPI_Datatype dType;

	status = vec.registerMPIType();
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	status = vec.getMPIType(&dType);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	// Cleanup for future tests since we use a static variable!
	status = vec.deregisterMPIType();
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
}

// Test 2: Test error case detected when type unregistered
BOOST_AUTO_TEST_CASE(getMPIType_test2, * utf::tolerance(0.00001))
{
	cupcfd::error::eCodes status;
	EuclideanVector<double,3> vec;
	MPI_Datatype dType;

	BOOST_CHECK_EQUAL(vec.isRegistered(), false);
	status = vec.getMPIType(&dType);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_MPI_DATATYPE_UNREGISTERED);
}

// Test 3: Test corrected ID is returned for method without error code
BOOST_AUTO_TEST_CASE(getMPIType_test3, * utf::tolerance(0.00001))
{

}

// === MPI: Vector Broadcast ===
// Test 1: Testing the the MPI Type was setup correctly, and able to broadcast two vectors
// This should establish that the custom MPI type has the right sizes
BOOST_AUTO_TEST_CASE(MPIVectorBroadcast)
{
	cupcfd::error::eCodes status;
	cupcfd::comm::Communicator comm(MPI_COMM_WORLD);

	// Create Two Vectors on Rank 0

	// Register the type (only once per all tests in file since static?)
	EuclideanVector<double,3> vec;

	// Should not yet be registered
	BOOST_CHECK_EQUAL(vec.isRegistered(), false);

	// Register
	status = vec.registerMPIType();
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	// Should now be registered
	BOOST_CHECK_EQUAL(vec.isRegistered(), true);

	if(comm.rank == 0)
	{
		EuclideanVector<double,3> vecs[2] = {EuclideanVector<double,3>(3.5, 6.3, 19.7),
											 EuclideanVector<double,3>(3.0, 4.5, 6.7)};

		cupcfd::comm::Broadcast(vecs, 2, 0, comm);
	}
	else
	{
		EuclideanVector<double,3> recvVecs[2];

		cupcfd::comm::Broadcast(recvVecs, 2, 0, comm);

		BOOST_CHECK_EQUAL(recvVecs[0].cmp[0], 3.5);
		BOOST_CHECK_EQUAL(recvVecs[0].cmp[1], 6.3);
		BOOST_CHECK_EQUAL(recvVecs[0].cmp[2], 19.7);

		BOOST_CHECK_EQUAL(recvVecs[1].cmp[0], 3.0);
		BOOST_CHECK_EQUAL(recvVecs[1].cmp[1], 4.5);
		BOOST_CHECK_EQUAL(recvVecs[1].cmp[2], 6.7);
	}

	// Cleanup for future tests since we use a static variable!
	status = vec.deregisterMPIType();
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
}


BOOST_AUTO_TEST_CASE(cleanup)
{
    MPI_Finalize();
}

