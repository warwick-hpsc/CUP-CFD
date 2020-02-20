/*
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Tests for the arithmetic kernel functions from the utility operations
 *
 */

#define BOOST_TEST_MODULE ArithmeticKernel
#include <boost/test/unit_test.hpp>
#include <boost/test/output_test_stream.hpp>

#include <stdexcept>
#include "ArithmeticKernels.h"

namespace utf = boost::unit_test;
using namespace cupcfd::utility::arithmetic::kernels;

// === isEqual ===
// Test 1: Integer Equal
BOOST_AUTO_TEST_CASE(isEqual_test1, * utf::tolerance(0.00001))
{

}

// Test 2: Integer Not Equal
BOOST_AUTO_TEST_CASE(isEqual_test2, * utf::tolerance(0.00001))
{

}

// Test 3: Float Equal Within header Tolerance
BOOST_AUTO_TEST_CASE(isEqual_test3, * utf::tolerance(0.00001))
{

}

// Test 4: Float Not Equal Within header Tolerance
BOOST_AUTO_TEST_CASE(isEqual_test4, * utf::tolerance(0.00001))
{

}

// Test 5: Double Equal Within Tolerance
BOOST_AUTO_TEST_CASE(isEqual_test5, * utf::tolerance(0.00001))
{

}

// Test 6: Double Not Equal Within Tolerance
BOOST_AUTO_TEST_CASE(isEqual_test6, * utf::tolerance(0.00001))
{

}

// === sqrtWr ===
// Test 1: Test double function
BOOST_AUTO_TEST_CASE(sqrt_test1, * utf::tolerance(0.00001))
{

}

// Test 2: Test float function
BOOST_AUTO_TEST_CASE(sqrt_test2, * utf::tolerance(0.00001))
{

}

// === sgn ===
// Test 1: Test negative signed integer
BOOST_AUTO_TEST_CASE(sgn_test1)
{
	int val = -4;
	int result = sgn(val);
	BOOST_CHECK_EQUAL(result, -1);
}

// Test 2: Test zero integer
BOOST_AUTO_TEST_CASE(sgn_test2)
{
	int val = 0;
	int result = sgn(val);
	BOOST_CHECK_EQUAL(result, 0);
}

// Test 3: Test positive integer
BOOST_AUTO_TEST_CASE(sgn_test3)
{
	int val = 4;
	int result = sgn(val);
	BOOST_CHECK_EQUAL(result, 1);
}

// Test 4: Test negative double
BOOST_AUTO_TEST_CASE(sgn_test4)
{
	int val = -4.3;
	int result = sgn(val);
	BOOST_CHECK_EQUAL(result, -1);
}

// Test 5: Test zero double
BOOST_AUTO_TEST_CASE(sgn_test5)
{
	int val = 0.0;
	int result = sgn(val);
	BOOST_CHECK_EQUAL(result, 0);
}

// Test 6: Test positive double
BOOST_AUTO_TEST_CASE(sgn_test6)
{
	int val = 4.3;
	int result = sgn(val);
	BOOST_CHECK_EQUAL(result, 1);
}
