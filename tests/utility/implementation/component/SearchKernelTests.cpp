/*
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Tests for the kernels of the utility search operations
 *
 */

#define BOOST_TEST_MODULE SearchKernels
#include <boost/test/unit_test.hpp>
#include <boost/test/output_test_stream.hpp>

#include <stdexcept>

#include "SearchKernels.h"

using namespace cupcfd::utility::kernels;

// ================== Binary Search ===========================
// Test 1: Test Element is in first position, even sized array
BOOST_AUTO_TEST_CASE(binarySearch_test1)
{
	int source[8] = {1, 4, 7, 8, 9, 11, 13, 20};
	int result;

	result = binarySearch(source, 8, 1);
	BOOST_CHECK_EQUAL(result, 0);
}

// Test 2: Test element in last position, even sized array
BOOST_AUTO_TEST_CASE(binarySearch_test2)
{

	int source[8] = {1, 4, 7, 8, 9, 11, 13, 20};
	int result;

	result = binarySearch(source, 8, 20);
	BOOST_CHECK_EQUAL(result, 7);
}

// Test 3: Test element in middle position, even sized array
BOOST_AUTO_TEST_CASE(binarySearch_test3)
{

	int source[8] = {1, 4, 7, 8, 9, 11, 13, 20};
	int result;

	result = binarySearch(source, 8, 9);
	BOOST_CHECK_EQUAL(result, 4);
}

// Test 4: Test searching for each element present in array, even sized array
BOOST_AUTO_TEST_CASE(binarySearch_test4)
{
	int source[8] = {1, 4, 7, 8, 9, 11, 13, 20};
	int result;

	result = binarySearch(source, 8, 1);
	BOOST_CHECK_EQUAL(result, 0);

	result = binarySearch(source, 8, 4);
	BOOST_CHECK_EQUAL(result, 1);

	result = binarySearch(source, 8, 7);
	BOOST_CHECK_EQUAL(result, 2);

	result = binarySearch(source, 8, 8);
	BOOST_CHECK_EQUAL(result, 3);

	result = binarySearch(source, 8, 9);
	BOOST_CHECK_EQUAL(result, 4);

	result = binarySearch(source, 8, 11);
	BOOST_CHECK_EQUAL(result, 5);

	result = binarySearch(source, 8, 13);
	BOOST_CHECK_EQUAL(result, 6);

	result = binarySearch(source, 8, 20);
	BOOST_CHECK_EQUAL(result, 7);
}

// Test 5: Test element not present in array, even sized array
BOOST_AUTO_TEST_CASE(binarySearch_test5)
{
	int source[8] = {1, 4, 7, 8, 9, 11, 12, 13};
	int result;

	result = binarySearch(source, 8, 1024);
	BOOST_CHECK_EQUAL(result, -1);
}

// Test 6: Test element in first position, odd sized array
BOOST_AUTO_TEST_CASE(binarySearch_test6)
{

	int source[7] = {1, 4, 7, 8, 9, 11, 13};
	int result;

	result = binarySearch(source, 7, 1);
	BOOST_CHECK_EQUAL(result, 0);
}

// Test 7: Test element in last position, odd sized array
BOOST_AUTO_TEST_CASE(binarySearch_test7)
{

	int source[7] = {1, 4, 7, 8, 9, 11, 13};
	int result;

	result = binarySearch(source, 7, 13);
	BOOST_CHECK_EQUAL(result, 6);
}

// Test 8: Test element in middle position, odd sized array
BOOST_AUTO_TEST_CASE(binarySearch_test8)
{

	int source[7] = {1, 4, 7, 8, 9, 11, 13};
	int result;

	result = binarySearch(source, 7, 9);
	BOOST_CHECK_EQUAL(result, 4);
}

// Test 9: Test search for each element in array, odd sized array
BOOST_AUTO_TEST_CASE(binarySearch_test9)
{
	int source[7] = {1, 4, 7, 8, 9, 11, 13};
	int result;

	result = binarySearch(source, 8, 1);
	BOOST_CHECK_EQUAL(result, 0);

	result = binarySearch(source, 8, 4);
	BOOST_CHECK_EQUAL(result, 1);

	result = binarySearch(source, 8, 7);
	BOOST_CHECK_EQUAL(result, 2);

	result = binarySearch(source, 8, 8);
	BOOST_CHECK_EQUAL(result, 3);

	result = binarySearch(source, 8, 9);
	BOOST_CHECK_EQUAL(result, 4);

	result = binarySearch(source, 8, 11);
	BOOST_CHECK_EQUAL(result, 5);

	result = binarySearch(source, 8, 13);
	BOOST_CHECK_EQUAL(result, 6);
}

// Test 10: Test when element is not present
BOOST_AUTO_TEST_CASE(binarySearch_test10)
{
	int source[7] = {1, 4, 7, 8, 9, 11, 12};
	int result;

	result = binarySearch(source, 8, 1024);
	BOOST_CHECK_EQUAL(result, -1);
}

// ================== Linear Search ===========================
// Test 1: Test element in first position, even sized array
BOOST_AUTO_TEST_CASE(linearSearch_test1)
{

	int source[8] = {1, 4, 7, 8, 9, 11, 13, 20};
	int result;

	result = linearSearch(source, 8, 1);
	BOOST_CHECK_EQUAL(result, 0);
}

// Test 2: Test element in last position, even sized array
BOOST_AUTO_TEST_CASE(linearSearch_test2)
{

	int source[8] = {1, 4, 7, 8, 9, 11, 13, 20};
	int result;

	result = linearSearch(source, 8, 20);
	BOOST_CHECK_EQUAL(result, 7);
}

// Test 3: Test element in middle position, even sized array
BOOST_AUTO_TEST_CASE(linearSearch_test3)
{

	int source[8] = {1, 4, 7, 8, 9, 11, 13, 20};
	int result;

	result = linearSearch(source, 8, 9);
	BOOST_CHECK_EQUAL(result, 4);
}

// Test 4: Test search for each element in array, even sized array
BOOST_AUTO_TEST_CASE(linearSearch_test4)
{
	int source[8] = {1, 4, 7, 8, 9, 11, 13, 20};
	int result;

	result = linearSearch(source, 8, 1);
	BOOST_CHECK_EQUAL(result, 0);

	result = linearSearch(source, 8, 4);
	BOOST_CHECK_EQUAL(result, 1);

	result = linearSearch(source, 8, 7);
	BOOST_CHECK_EQUAL(result, 2);

	result = linearSearch(source, 8, 8);
	BOOST_CHECK_EQUAL(result, 3);

	result = linearSearch(source, 8, 9);
	BOOST_CHECK_EQUAL(result, 4);

	result = linearSearch(source, 8, 11);
	BOOST_CHECK_EQUAL(result, 5);

	result = linearSearch(source, 8, 13);
	BOOST_CHECK_EQUAL(result, 6);

	result = linearSearch(source, 8, 20);
	BOOST_CHECK_EQUAL(result, 7);
}

// Test 5: Test when element not present, even sized array
BOOST_AUTO_TEST_CASE(linearSearch_test5)
{
	int source[8] = {1, 4, 7, 8, 9, 11, 12, 13};
	int result;

	result = linearSearch(source, 8, 1024);
	BOOST_CHECK_EQUAL(result, -1);
}

// Test 6: Test element in first position, odd sized array
BOOST_AUTO_TEST_CASE(linearSearch_test6)
{

	int source[7] = {1, 4, 7, 8, 9, 11, 13};
	int result;

	result = linearSearch(source, 7, 1);
	BOOST_CHECK_EQUAL(result, 0);
}

// Test 7: Test element in last position, odd sized array
BOOST_AUTO_TEST_CASE(linearSearch_test7)
{

	int source[7] = {1, 4, 7, 8, 9, 11, 13};
	int result;

	result = linearSearch(source, 7, 13);
	BOOST_CHECK_EQUAL(result, 6);
}

// Test 8: Test element in middle position, odd sized array
BOOST_AUTO_TEST_CASE(linearSearch_test8)
{

	int source[7] = {1, 4, 7, 8, 9, 11, 13};
	int result;

	result = linearSearch(source, 7, 9);
	BOOST_CHECK_EQUAL(result, 4);
}

// Test 9: Test search for each element in array, odd sized array
BOOST_AUTO_TEST_CASE(linearSearch_test9)
{
	int source[7] = {1, 4, 7, 8, 9, 11, 13};
	int result;

	result = linearSearch(source, 8, 1);
	BOOST_CHECK_EQUAL(result, 0);

	result = linearSearch(source, 8, 4);
	BOOST_CHECK_EQUAL(result, 1);

	result = linearSearch(source, 8, 7);
	BOOST_CHECK_EQUAL(result, 2);

	result = linearSearch(source, 8, 8);
	BOOST_CHECK_EQUAL(result, 3);

	result = linearSearch(source, 8, 9);
	BOOST_CHECK_EQUAL(result, 4);

	result = linearSearch(source, 8, 11);
	BOOST_CHECK_EQUAL(result, 5);

	result = linearSearch(source, 8, 13);
	BOOST_CHECK_EQUAL(result, 6);
}

// Test 10: Test element not present, odd sized array
BOOST_AUTO_TEST_CASE(linearSearch_test10)
{
	int source[7] = {1, 4, 7, 8, 9, 11, 12};
	int result;

	result = linearSearch(source, 8, 1024);
	BOOST_CHECK_EQUAL(result, -1);
}
