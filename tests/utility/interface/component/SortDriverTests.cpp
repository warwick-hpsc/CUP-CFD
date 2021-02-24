/*
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Tests for the sort drivers of the utility functions
 */

#define BOOST_TEST_MODULE SortDrivers
#include <boost/test/unit_test.hpp>
#include <boost/test/output_test_stream.hpp>

#include <stdexcept>

#include "SortDrivers.h"

#include "Error.h"

// ============================================================
// ==================== Driver Tests ==========================
// ============================================================
using namespace cupcfd::utility::drivers;

BOOST_AUTO_TEST_CASE(merge_sort_destructive_driver)
{
	int arr1[15] = {15, 19, 2, 3, 7, 13, 10, 10, 2003, 142, 2, 27, 43, 2003, 9};
	int arr1_cmp[15] = {2, 2, 3, 7, 9, 10, 10, 13, 15, 19, 27, 43, 142, 2003, 2003};
	cupcfd::error::eCodes status;

	status = merge_sort(arr1, 15);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	BOOST_CHECK_EQUAL_COLLECTIONS(arr1, arr1 + 15, arr1_cmp, arr1_cmp + 15);
}

BOOST_AUTO_TEST_CASE(merge_sort_nondestructive_driver)
{

}

BOOST_AUTO_TEST_CASE(driver_is_sorted_noduplicates)
{
	int arr1[9] = {2, 3, 4, 6, 7, 10, 12, 13, 2003};
	bool result = false;

	result = is_sorted(arr1, 9);
	BOOST_CHECK_EQUAL(result, true);
}

BOOST_AUTO_TEST_CASE(driver_is_sorted_duplicates)
{
	int arr1[9] = {2, 2, 3, 4, 7, 10, 10, 13, 2003};
	bool result = false;

	result = is_sorted(arr1, 9);
	BOOST_CHECK_EQUAL(result, true);
}

BOOST_AUTO_TEST_CASE(driver_is_not_sorted_first)
{
	//int arr1[15] = {15, 19, 2, 3, 7, 13, 10, 10, 2003, 142, 2, 27, 43, 2003, 9};
	int arr1[9] = {2003, 3, 4, 6, 7, 10, 12, 13, 14};
	bool result = true;

	result = is_sorted(arr1, 9);
	BOOST_CHECK_EQUAL(result, false);
}

BOOST_AUTO_TEST_CASE(driver_is_not_sorted_last)
{
	//int arr1[15] = {15, 19, 2, 3, 7, 13, 10, 10, 2003, 142, 2, 27, 43, 2003, 9};
	int arr1[9] = {2, 3, 4, 6, 7, 10, 12, 2003, 14};
	bool result = true;

	result = is_sorted(arr1, 9);
	BOOST_CHECK_EQUAL(result, false);
}

BOOST_AUTO_TEST_CASE(driver_is_not_sorted)
{
	//int arr1[15] = {15, 19, 2, 3, 7, 13, 10, 10, 2003, 142, 2, 27, 43, 2003, 9};
	int arr1[9] = {16, 32, 3, 4, 12, 2013, 231, 5135, 32};
	bool result = true;

	result = is_sorted(arr1, 9);
	BOOST_CHECK_EQUAL(result, false);
}

BOOST_AUTO_TEST_CASE(driver_merge_sort_index)
{
	int arr1[15] = {15, 19, 2, 3, 7, 13, 10, 10, 2003, 142, 2, 27, 43, 2003, 9};
	int arr1_cmp[15] = {2, 2, 3, 7, 9, 10, 10, 13, 15, 19, 27, 43, 142, 2003, 2003};

	int indexes[15];
	int indexes_cmp[15] = {2, 10, 3, 4, 14, 6, 7, 5, 0, 1, 11, 12, 9, 8, 13};

	cupcfd::error::eCodes status;
	status = merge_sort_index(arr1, 15, indexes, 15);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	BOOST_CHECK_EQUAL_COLLECTIONS(arr1, arr1 + 15, arr1_cmp, arr1_cmp + 15);
	BOOST_CHECK_EQUAL_COLLECTIONS(indexes, indexes + 15, indexes_cmp, indexes_cmp + 15);
}
