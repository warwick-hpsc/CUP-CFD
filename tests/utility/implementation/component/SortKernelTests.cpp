/*
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Tests for the sort kernels of the utility functions
 */

#define BOOST_TEST_MODULE Sort
#include <boost/test/unit_test.hpp>
#include <boost/test/output_test_stream.hpp>

#include <stdexcept>

#include "SortKernels.h"

using namespace cupcfd::utility::kernels;

// === Merge Sort 1 (Destructive) ===
// Test 1: Test for negative array size
BOOST_AUTO_TEST_CASE(merge_sort_1_test1)
{
	int testData[5] = {78, 14, 3, 9, 16};
	int err = merge_sort(testData, -1);

	BOOST_CHECK_EQUAL(err, -1);
}

// Test 2: Test with array of minimum size
BOOST_AUTO_TEST_CASE(merge_sort_1_test2)
{
	int testData[1] = {78};
	int err = merge_sort(testData, 1);

	BOOST_CHECK_EQUAL(err, 0);
	BOOST_CHECK_EQUAL(testData[0], 78);
}

// Test 3: Test unsorted array of size 2
BOOST_AUTO_TEST_CASE(merge_sort_1_test3)
{
	int testData[2] = {78, 64};
	int err = merge_sort(testData, 2);

	BOOST_CHECK_EQUAL(err, 0);
	BOOST_CHECK_EQUAL(testData[0], 64);
	BOOST_CHECK_EQUAL(testData[1], 78);
}

// Test 4: Test sorted array of size 2
BOOST_AUTO_TEST_CASE(merge_sort_1_test4)
{
	int testData[2] = {64, 78};
	int err = merge_sort(testData, 2);

	BOOST_CHECK_EQUAL(err, 0);
	BOOST_CHECK_EQUAL(testData[0], 64);
	BOOST_CHECK_EQUAL(testData[1], 78);
}

// Test 5: Test larger array, odd size, unsorted
BOOST_AUTO_TEST_CASE(merge_sort_1_test5)
{
	int arr1[15] = {15, 19, 2, 3, 7, 13, 12, 10, 2003, 142, 16, 27, 43, 21, 9};
	int arr1_cmp[15] = {2, 3, 7, 9, 10, 12, 13, 15, 16, 19, 21, 27, 43, 142, 2003};

	int result = merge_sort(arr1, 15);
	BOOST_CHECK_EQUAL(result, 0);

	BOOST_CHECK_EQUAL_COLLECTIONS(arr1, arr1 + 15, arr1_cmp, arr1_cmp + 15);
}

// Test 6: Test larger array, odd size, unsorted with duplicates
BOOST_AUTO_TEST_CASE(merge_sort_1_test6)
{
	int arr1[15] = {15, 19, 2, 3, 7, 13, 10, 10, 2003, 142, 2, 27, 43, 2003, 9};
	int arr1_cmp[15] = {2, 2, 3, 7, 9, 10, 10, 13, 15, 19, 27, 43, 142, 2003, 2003};

	int result = merge_sort(arr1, 15);
	BOOST_CHECK_EQUAL(result, 0);

	BOOST_CHECK_EQUAL_COLLECTIONS(arr1, arr1 + 15, arr1_cmp, arr1_cmp + 15);
}

// Test 7: Test larger array, even size, unsorted
BOOST_AUTO_TEST_CASE(merge_sort_1_test7)
{
	int arr1[10] = {10,9,8,7,6,5,4,3,2,1};
	int arr1_cmp[10] = {1,2,3,4,5,6,7,8,9,10};

	int result = merge_sort(arr1, 10);
	BOOST_CHECK_EQUAL(result, 0);

	BOOST_CHECK_EQUAL_COLLECTIONS(arr1, arr1 + 10, arr1_cmp, arr1_cmp + 10);
}

// Test 8: Test larger array, even size, unsorted with duplicates
BOOST_AUTO_TEST_CASE(merge_sort_1_test8)
{
	int arr1[10] = {10,9,8,6,6,5,4,3,3,10};
	int arr1_cmp[10] = {3,3,4,5,6,6,8,9,10,10};

	int result = merge_sort(arr1, 10);
	BOOST_CHECK_EQUAL(result, 0);

	BOOST_CHECK_EQUAL_COLLECTIONS(arr1, arr1 + 10, arr1_cmp, arr1_cmp + 10);
}

// === Merge Sort 2 (Destructive with indexes) ===

// === is_sorted ===
// Test 1: Array sorted, no duplicates
BOOST_AUTO_TEST_CASE(is_sorted_test1)
{
	int arr1[9] = {2, 3, 4, 6, 7, 10, 12, 13, 2003};
	bool result = false;

	result = is_sorted(arr1, 9);
	BOOST_CHECK_EQUAL(result, true);
}

// Test 2: Array sorted, duplicates
BOOST_AUTO_TEST_CASE(is_sorted_test2)
{
	int arr1[9] = {2, 2, 3, 4, 7, 10, 10, 13, 2003};
	bool result = false;

	result = is_sorted(arr1, 9);
	BOOST_CHECK_EQUAL(result, true);
}

// Test 3: First element out of order
BOOST_AUTO_TEST_CASE(is_sorted_test3)
{
	int arr1[9] = {2003, 3, 4, 6, 7, 10, 12, 13, 14};
	bool result = true;

	result = is_sorted(arr1, 9);
	BOOST_CHECK_EQUAL(result, false);
}

// Test 4: Last element out of order
BOOST_AUTO_TEST_CASE(is_sorted_test4)
{
	int arr1[9] = {2, 3, 4, 6, 7, 10, 12, 2003, 14};
	bool result = true;

	result = is_sorted(arr1, 9);
	BOOST_CHECK_EQUAL(result, false);
}

// Test 5: Array generally not sorted
BOOST_AUTO_TEST_CASE(is_sorted_test5)
{
	int arr1[9] = {16, 32, 3, 4, 12, 2013, 231, 5135, 32};
	bool result = true;

	result = is_sorted(arr1, 9);
	BOOST_CHECK_EQUAL(result, false);
}

// === sourceIndexReorder ===
// Test 1: Test correct reordering for arbitrary array
BOOST_AUTO_TEST_CASE(sourceIndexReorder_test1)
{
	int testData[5] = {78, 14, 3, 9, 16};
	int indexes[5] = {3, 4, 1, 2, 0};
	int testDataCmp[5] = {9, 16, 14, 3, 78};

	cupcfd::error::eCodes status;
	status = sourceIndexReorder(testData, 5, indexes, 5);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	BOOST_CHECK_EQUAL_COLLECTIONS(testData, testData + 5, testDataCmp, testDataCmp + 5);
}

// === destIndexReorder ===
// Test 1: Test cirrect reordering for arbitrary array
BOOST_AUTO_TEST_CASE(destIndexReorder_test1)
{
	int testData[5] = {78, 14, 3, 9, 16};
	int indexes[5] = {3, 4, 1, 2, 0};
	int testDataCmp[5] = {16, 3, 9, 78, 14};

	cupcfd::error::eCodes status;
	status = destIndexReorder(testData, 5, indexes, 5);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	BOOST_CHECK_EQUAL_COLLECTIONS(testData, testData + 5, testDataCmp, testDataCmp + 5);
}
