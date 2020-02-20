/*
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Tests for the drivers of the utility search operations
 */

#define BOOST_TEST_MODULE SearchDrivers
#include <boost/test/unit_test.hpp>
#include <boost/test/output_test_stream.hpp>

#include <stdexcept>

#include "SearchDrivers.h"

// ============================================================
// ==================== Driver Tests ==========================
// ============================================================
using namespace cupcfd::utility::drivers;


// ================== Binary Search ===========================

BOOST_AUTO_TEST_CASE(driver_binarySearch_index_first)
{
	int source[8] = {1, 4, 7, 8, 9, 11, 13, 20};
	int index;
	cupcfd::error::eCodes result;

	result = binarySearch(source, 8, 1, &index);
	BOOST_CHECK_EQUAL(result, cupcfd::error::E_SEARCH_SUCCESS);
}

BOOST_AUTO_TEST_CASE(driver_binarySearch_index_last)
{
	int source[8] = {1, 4, 7, 8, 9, 11, 13, 20};
	int index;
	cupcfd::error::eCodes result;

	result = binarySearch(source, 8, 20, &index);
	BOOST_CHECK_EQUAL(result, cupcfd::error::E_SEARCH_SUCCESS);
}

BOOST_AUTO_TEST_CASE(driver_binarySearch_index_mid)
{
	int source[8] = {1, 4, 7, 8, 9, 11, 13, 20};
	int index;
	cupcfd::error::eCodes result;

	result = binarySearch(source, 8, 9, &index);
	BOOST_CHECK_EQUAL(result, cupcfd::error::E_SEARCH_SUCCESS);
}

BOOST_AUTO_TEST_CASE(driver_binarySearch_index_error_unsorted)
{
	int source[8] = {4, 1, 7, 9, 7, 11, 13, 20};
	int index;
	cupcfd::error::eCodes result;

	result = binarySearch(source, 8, 9, &index);
	BOOST_CHECK_EQUAL(result, cupcfd::error::E_SEARCH_UNSORTED);
}

BOOST_AUTO_TEST_CASE(driver_binarySearch_index_error_notfound)
{
	int source[8] = {1, 4, 7, 8, 9, 11, 13, 20};
	int index;
	cupcfd::error::eCodes result;

	result = binarySearch(source, 8, 1024, &index);
	BOOST_CHECK_EQUAL(result, cupcfd::error::E_SEARCH_NOT_FOUND);
}

BOOST_AUTO_TEST_CASE(driver_binarySearch_bool_first)
{
	int source[8] = {1, 4, 7, 8, 9, 11, 13, 20};
	bool found;
	cupcfd::error::eCodes result;

	result = binarySearch(source, 8, 1, &found);
	BOOST_CHECK_EQUAL(result, cupcfd::error::E_SEARCH_SUCCESS);
}


BOOST_AUTO_TEST_CASE(driver_binarySearch_bool_last)
{
	int source[8] = {1, 4, 7, 8, 9, 11, 13, 20};
	bool found;
	cupcfd::error::eCodes result;

	result = binarySearch(source, 8, 20, &found);
	BOOST_CHECK_EQUAL(result, cupcfd::error::E_SEARCH_SUCCESS);
}

BOOST_AUTO_TEST_CASE(driver_binarySearch_bool_mid)
{
	int source[8] = {1, 4, 7, 8, 9, 11, 13, 20};
	bool found;
	cupcfd::error::eCodes result;

	result = binarySearch(source, 8, 9, &found);
	BOOST_CHECK_EQUAL(result, cupcfd::error::E_SEARCH_SUCCESS);
}

BOOST_AUTO_TEST_CASE(driver_binarySearch_bool_error_unsorted)
{
	int source[8] = {4, 1, 7, 9, 7, 11, 13, 20};
	bool found;
	cupcfd::error::eCodes result;

	result = binarySearch(source, 8, 9, &found);
	BOOST_CHECK_EQUAL(result, cupcfd::error::E_SEARCH_UNSORTED);
}

BOOST_AUTO_TEST_CASE(driver_binarySearch_bool_error_notfound)
{
	int source[8] = {1, 4, 7, 8, 9, 11, 13, 20};
	bool found;
	cupcfd::error::eCodes result;

	result = binarySearch(source, 8, 1024, &found);
	BOOST_CHECK_EQUAL(result, cupcfd::error::E_SEARCH_NOT_FOUND);
}

// ================== Linear Search ===========================


BOOST_AUTO_TEST_CASE(driver_linearSearch_index_first)
{
	int source[8] = {1, 4, 7, 8, 9, 11, 13, 20};
	int index;
	cupcfd::error::eCodes result;

	result = linearSearch(source, 8, 1, &index);
	BOOST_CHECK_EQUAL(result, cupcfd::error::E_SEARCH_SUCCESS);
}

BOOST_AUTO_TEST_CASE(driver_linearSearch_index_last)
{
	int source[8] = {1, 4, 7, 8, 9, 11, 13, 20};
	int index;
	cupcfd::error::eCodes result;

	result = linearSearch(source, 8, 20, &index);
	BOOST_CHECK_EQUAL(result, cupcfd::error::E_SEARCH_SUCCESS);
}

BOOST_AUTO_TEST_CASE(driver_linearSearch_index_mid)
{
	int source[8] = {1, 4, 7, 8, 9, 11, 13, 20};
	int index;
	cupcfd::error::eCodes result;

	result = linearSearch(source, 8, 9, &index);
	BOOST_CHECK_EQUAL(result, cupcfd::error::E_SEARCH_SUCCESS);
}

BOOST_AUTO_TEST_CASE(driver_linearSearch_index_error_notfound)
{
	int source[8] = {1, 4, 7, 8, 9, 11, 13, 20};
	int index;
	cupcfd::error::eCodes result;

	result = linearSearch(source, 8, 1024, &index);
	BOOST_CHECK_EQUAL(result, cupcfd::error::E_SEARCH_NOT_FOUND);
}


BOOST_AUTO_TEST_CASE(driver_linearSearch_bool_first)
{
	int source[8] = {1, 4, 7, 8, 9, 11, 13, 20};
	bool found;
	cupcfd::error::eCodes result;

	result = linearSearch(source, 8, 1, &found);
	BOOST_CHECK_EQUAL(result, cupcfd::error::E_SEARCH_SUCCESS);
}

BOOST_AUTO_TEST_CASE(driver_linearSearch_bool_last)
{
	int source[8] = {1, 4, 7, 8, 9, 11, 13, 20};
	bool found;
	cupcfd::error::eCodes result;

	result = linearSearch(source, 8, 20, &found);
	BOOST_CHECK_EQUAL(result, cupcfd::error::E_SEARCH_SUCCESS);
}

BOOST_AUTO_TEST_CASE(driver_linearSearch_bool_mid)
{
	int source[8] = {1, 4, 7, 8, 9, 11, 13, 20};
	bool found;
	cupcfd::error::eCodes result;

	result = linearSearch(source, 8, 9, &found);
	BOOST_CHECK_EQUAL(result, cupcfd::error::E_SEARCH_SUCCESS);
}

BOOST_AUTO_TEST_CASE(driver_linearSearch_bool_error_notfound)
{
	int source[8] = {1, 4, 7, 8, 9, 11, 13, 20};
	bool found;
	cupcfd::error::eCodes result;

	result = linearSearch(source, 8, 1024, &found);
	BOOST_CHECK_EQUAL(result, cupcfd::error::E_SEARCH_NOT_FOUND);
}

