/*
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Tests for the array driver functions from the utility operations
 *
 */

#define BOOST_TEST_MODULE ArrayDrivers
#include <boost/test/unit_test.hpp>
#include <boost/test/output_test_stream.hpp>

#include <stdexcept>
#include "ArrayDrivers.h"
#include "ArrayKernels.h"

using namespace cupcfd::utility::drivers;

// ======================= Array Copy Tests ==================================
// Test 1: Successful copy
BOOST_AUTO_TEST_CASE(copy_test1)
{
	int source[6] = {1, 4, 3, 2, 6, 1};
	int result[6] = {0 ,0 ,0 ,0 ,0 ,0};
	int resultCmp[6] = {1, 4, 3, 2, 6, 1};

	cupcfd::error::eCodes err = copy(source, 6, result, 6);

	BOOST_CHECK_EQUAL(err, cupcfd::error::E_SUCCESS);
	BOOST_CHECK_EQUAL_COLLECTIONS(result, result + 5, resultCmp, resultCmp + 5);
}

// Test 2: Undersized destination array
BOOST_AUTO_TEST_CASE(copy_test2)
{
	int source[6] = {1, 4, 3, 2, 6, 1};
	int result[4] = {0 ,0 ,0 ,0};

	cupcfd::error::eCodes err = copy(source, 6, result, 4);

	BOOST_CHECK_EQUAL(err, cupcfd::error::E_ARRAY_SIZE_UNDERSIZED);
}

// ======================= Array Zero Tests ==================================
// Test 1: Zero the array
BOOST_AUTO_TEST_CASE(zero_test1)
{
	int source[6] = {1, 4, 3, 2, 6, 1};
	int resultCmp[6] = {0, 0, 0, 0, 0, 0};

	cupcfd::error::eCodes err = zero(source, 6);

	BOOST_CHECK_EQUAL(err, cupcfd::error::E_SUCCESS);
	BOOST_CHECK_EQUAL_COLLECTIONS(source, source + 5, resultCmp, resultCmp + 5);
}

// ==================== UniqueCount ==========================
// Test 1: Count the number of unique elements in an array
BOOST_AUTO_TEST_CASE(uniqueCount_test1)
{
	int source[11] = {1, 1, 1, 1, 2, 3, 3, 4, 4, 7, 325};
	int count;
	cupcfd::error::eCodes status;
	status = uniqueCount(source, 11, & count);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	BOOST_CHECK_EQUAL(count, 3);
}

// Test 2: Count zero unique elements in an array with only duplicates
BOOST_AUTO_TEST_CASE(uniqueCount_test2)
{
	int source[14] = {1, 1, 1, 1, 2, 2, 3, 3, 4, 4, 7, 7, 325, 325};
	int count;
	cupcfd::error::eCodes status;
	status = uniqueCount(source, 14, & count);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	BOOST_CHECK_EQUAL(count, 0);
}

// Test 3: Count the number of unique elements in an array that is unsorted
BOOST_AUTO_TEST_CASE(uniqueCount_test3)
{
	int source[11] = {7, 5, 3, 2, 15, 2, 7, 21, 100, 200, 150};
	int sourceCmp[11] = {7, 5, 3, 2, 15, 2, 7, 21, 100, 200, 150};
	int count;
	cupcfd::error::eCodes status;
	status = uniqueCount(source, 11, & count);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	// Source should remain unmodified (non-destructive)
	BOOST_CHECK_EQUAL_COLLECTIONS(source, source + 11, sourceCmp, sourceCmp + 11);
	BOOST_CHECK_EQUAL(count, 7);
}

// Test 4: Count the number of unique elements when all elements are unique
BOOST_AUTO_TEST_CASE(uniqueCount_test4)
{
	int source[6] = {1, 2, 3, 4, 7, 325};
	int count;
	cupcfd::error::eCodes status;
	status = uniqueCount(source, 6, &count);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	BOOST_CHECK_EQUAL(count, 6);
}

// Test 5: Count the number of unique elements when only the first is unique
BOOST_AUTO_TEST_CASE(uniqueCount_test5)
{
	int source[11] = {1, 2, 2, 3, 3, 4, 4, 7, 7, 325, 325};
	int count;
	cupcfd::error::eCodes status;
	status = uniqueCount(source, 11, &count);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	BOOST_CHECK_EQUAL(count, 1);
}

// Test 6: Count the number of unique elements when only the last is unique
BOOST_AUTO_TEST_CASE(uniqueCount_test6)
{
	int source[11] = {1, 1, 2, 2, 3, 3, 4, 4, 7, 7, 325};
	int count;
	cupcfd::error::eCodes status;
	status = uniqueCount(source, 11, &count);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	BOOST_CHECK_EQUAL(count, 1);
}

// Test 7: Count the number of unique elements when only the last two are unique
BOOST_AUTO_TEST_CASE(uniqueCount_test7)
{
	int source[10] = {1, 1, 2, 2, 3, 3, 4, 4, 7, 325};
	int count;
	cupcfd::error::eCodes status;
	status = uniqueCount(source, 10, &count);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	BOOST_CHECK_EQUAL(count, 2);
}

// ================= Unique Array =======================
// Test 1: Find the unique elements in an arbitrary array
BOOST_AUTO_TEST_CASE(uniqueArray_test1)
{
	// The kernel expects sorted arrays only
	// Isn't this tested already in the kernels tests?
	int source[11] = {1, 1, 1, 1, 2, 3, 3, 4, 4, 7, 325};
	int count = cupcfd::utility::kernels::uniqueCount(source, 11);
	BOOST_CHECK_EQUAL(count, 3);

	int result[3];
	int result_cmp[3] = {2, 7, 325};

	cupcfd::error::eCodes err = uniqueArray(source, 11, result, 3);

	BOOST_CHECK_EQUAL(err, cupcfd::error::E_SUCCESS);
	BOOST_CHECK_EQUAL_COLLECTIONS(result, result + 3, result_cmp, result_cmp + 3);
}

// Test 2: Find the unique elements in an unsorted array
BOOST_AUTO_TEST_CASE(uniqueArray_test2)
{
	// The kernel expects sorted arrays only
	int source[11] = {1, 1, 1, 1, 2, 3, 3, 4, 4, 7, 325};
	int count = cupcfd::utility::kernels::uniqueCount(source, 11);
	BOOST_CHECK_EQUAL(count, 3);

	int result[3];
	int result_cmp[3] = {2, 7, 325};

	cupcfd::error::eCodes err = uniqueArray(source, 11, result, 3);

	BOOST_CHECK_EQUAL(err, cupcfd::error::E_SUCCESS);
	BOOST_CHECK_EQUAL_COLLECTIONS(result, result + 3, result_cmp, result_cmp + 3);
}

// Test 3: Check that the method runs without error when there are no unique elements
BOOST_AUTO_TEST_CASE(uniqueArray_test3)
{
	// The kernel expects sorted arrays only
	int source[14] = {1, 1, 1, 1, 2, 2, 3, 3, 4, 4, 7, 7, 325, 325};
	int count = cupcfd::utility::kernels::uniqueCount(source, 14);
	BOOST_CHECK_EQUAL(count, 0);

	int result[0];
	// Theoretically, it should never copy. If it does, it will go out of bounds in memory.
	cupcfd::error::eCodes err = uniqueArray(source, 14, result, 0);

	BOOST_CHECK_EQUAL(err, cupcfd::error::E_SUCCESS);
}

// Test 4: Test when all elements are unique
BOOST_AUTO_TEST_CASE(uniqueArray_test4)
{
	// The kernel expects sorted arrays only
	int source[6] = {1, 2, 3, 4, 7, 325};
	int count = cupcfd::utility::kernels::uniqueCount(source, 6);
	BOOST_CHECK_EQUAL(count, 6);

	int result[6];
	int result_cmp[6] = {1, 2, 3, 4, 7, 325};
	cupcfd::error::eCodes err = uniqueArray(source, 6, result, 6);

	BOOST_CHECK_EQUAL_COLLECTIONS(result, result + 6, result_cmp, result_cmp + 6);
	BOOST_CHECK_EQUAL(err, cupcfd::error::E_SUCCESS);
}

// Test 5: Test when only the first element is unique
BOOST_AUTO_TEST_CASE(uniqueArray_test5)
{
	// The kernel expects sorted arrays only
	int source[11] = {1, 2, 2, 3, 3, 4, 4, 7, 7, 325, 325};
	int count = cupcfd::utility::kernels::uniqueCount(source, 11);
	BOOST_CHECK_EQUAL(count, 1);

	int result[1];
	int result_cmp[1] = {1};
	cupcfd::error::eCodes err = uniqueArray(source, 11, result, 1);

	BOOST_CHECK_EQUAL_COLLECTIONS(result, result + 1, result_cmp, result_cmp + 1);
	BOOST_CHECK_EQUAL(err, cupcfd::error::E_SUCCESS);
}

// Test 6: Test when the last element is unique
BOOST_AUTO_TEST_CASE(uniqueArray_test6)
{
	// The kernel expects sorted arrays only
	int source[11] = {1, 1, 2, 2, 3, 3, 4, 4, 7, 7, 325};
	int count = cupcfd::utility::kernels::uniqueCount(source, 11);
	BOOST_CHECK_EQUAL(count, 1);

	int result[1];
	int result_cmp[1] = {325};
	cupcfd::error::eCodes err = uniqueArray(source, 11, result, 1);

	BOOST_CHECK_EQUAL_COLLECTIONS(result, result + 1, result_cmp, result_cmp + 1);
	BOOST_CHECK_EQUAL(err, cupcfd::error::E_SUCCESS);
}

// Test 7: Test when the last two elements are unique
BOOST_AUTO_TEST_CASE(uniqueArray_test7)
{
	// The kernel expects sorted arrays only
	int source[10] = {1, 1, 2, 2, 3, 3, 4, 4, 7, 325};
	int count = cupcfd::utility::kernels::uniqueCount(source, 10);
	BOOST_CHECK_EQUAL(count, 2);

	int result[2];
	int result_cmp[2] = {7, 325};
	cupcfd::error::eCodes err = uniqueArray(source, 10, result, 2);

	BOOST_CHECK_EQUAL_COLLECTIONS(result, result + 2, result_cmp, result_cmp + 2);
	BOOST_CHECK_EQUAL(err, cupcfd::error::E_SUCCESS);
}

// ================= Unique Array + Array Creation =======================

// Test 8: Test creating an array automatically when finding unique elements
BOOST_AUTO_TEST_CASE(uniqueArray_test8)
{
	// The kernel expects sorted arrays only
	int source[11] = {1, 1, 1, 1, 2, 3, 3, 4, 4, 7, 325};
	int count;
	int * result;

	int result_cmp[3] = {2, 7, 325};

	cupcfd::error::eCodes err = uniqueArray(source, 11, &result, &count);

	BOOST_CHECK_EQUAL(err, cupcfd::error::E_SUCCESS);
	BOOST_CHECK_EQUAL(count, 3);
	BOOST_CHECK_EQUAL_COLLECTIONS(result, result + 3, result_cmp, result_cmp + 3);

	// Cleanup
	free(result);
}

// ======================= Array Add Tests ===================================
// Test 1: Test adding with destructive storage in source1
BOOST_AUTO_TEST_CASE(add_test1)
{
	int source1[5] = {1,2,3,4,5};
	int source2[5] = {1,2,18,4,7};
	int resultCmp[5] = {2, 4, 21, 8, 12};
	int source2Cmp[5] = {1,2,18,4,7};

	cupcfd::error::eCodes err = add(source1, 5, source2, 5);

	BOOST_CHECK_EQUAL(err, cupcfd::error::E_SUCCESS);
	BOOST_CHECK_EQUAL_COLLECTIONS(source1, source1 + 5, resultCmp, resultCmp + 5);
	BOOST_CHECK_EQUAL_COLLECTIONS(source2, source2 + 5, source2Cmp, source2Cmp + 5);
}

// ======================= Array Add Non-Destructive Tests ===================================
// Test 2: Test adding with non-destructive storage
BOOST_AUTO_TEST_CASE(add_test2)
{
	int dest[5];

	int source1[5] = {1,2,3,4,5};
	int source2[5] = {1,2,18,4,7};

	int source1Cmp[5] = {1,2,3,4,5};
	int source2Cmp[5] = {1,2,18,4,7};
	int resultCmp[5] = {2, 4, 21, 8, 12};

	cupcfd::error::eCodes err = add(source1, 5, source2, 5, dest, 5);

	BOOST_CHECK_EQUAL(err, cupcfd::error::E_SUCCESS);
	BOOST_CHECK_EQUAL_COLLECTIONS(dest, dest + 5, resultCmp, resultCmp + 5);
	BOOST_CHECK_EQUAL_COLLECTIONS(source1, source1 + 5, source1Cmp, source1Cmp + 5);
	BOOST_CHECK_EQUAL_COLLECTIONS(source2, source2 + 5, source2Cmp, source2Cmp + 5);
}

// ================= Distinct Count ===========================
// Test 1: Test counting distinct elements in arbitrary array
BOOST_AUTO_TEST_CASE(distinctCount_test1)
{
	// The kernel expects sorted arrays only
	int source[11] = {1, 1, 1, 1, 2, 3, 3, 4, 4, 7, 325};
	int count;
	cupcfd::error::eCodes err = distinctCount(source, 11, &count);

	BOOST_CHECK_EQUAL(err, cupcfd::error::E_SUCCESS);
	BOOST_CHECK_EQUAL(count, 6);
}

// Test 2: Test counting distinct elements with duplicates
BOOST_AUTO_TEST_CASE(distinctCount_test2)
{
	// The kernel expects sorted arrays only
	int source[11] = {1, 1, 2, 2, 2, 3, 3, 4, 4, 4, 4};
	int count;
	cupcfd::error::eCodes err = distinctCount(source, 11, &count);

	BOOST_CHECK_EQUAL(err, cupcfd::error::E_SUCCESS);
	BOOST_CHECK_EQUAL(count, 4);
}

// Test 3: Test counting with all elements being distinct
BOOST_AUTO_TEST_CASE(distinctCount_test3)
{
	// The kernel expects sorted arrays only
	int source[11] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 15};
	int count;
	cupcfd::error::eCodes err = distinctCount(source, 11, &count);

	BOOST_CHECK_EQUAL(err, cupcfd::error::E_SUCCESS);
	BOOST_CHECK_EQUAL(count, 11);
}

// Test 4: Test counting where only first element is non-duplicate
BOOST_AUTO_TEST_CASE(distinctCount_test4)
{
	// The kernel expects sorted arrays only
	int source[11] = {1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2};
	int count;
	cupcfd::error::eCodes err = distinctCount(source, 11, &count);

	BOOST_CHECK_EQUAL(err, cupcfd::error::E_SUCCESS);
	BOOST_CHECK_EQUAL(count, 2);
}

// Test 5: Test counting where only last element is non-duplicate
BOOST_AUTO_TEST_CASE(distinctCount_test5)
{
	// The kernel expects sorted arrays only
	int source[11] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2};
	int count;
	cupcfd::error::eCodes err = distinctCount(source, 11, &count);

	BOOST_CHECK_EQUAL(err, cupcfd::error::E_SUCCESS);
	BOOST_CHECK_EQUAL(count, 2);
}

// Test 6: Test counting where only first two elements are non-duplicate
BOOST_AUTO_TEST_CASE(distinctCount_test6)
{
	// The kernel expects sorted arrays only
	int source[11] = {1, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3};
	int count;
	cupcfd::error::eCodes err = distinctCount(source, 11, &count);

	BOOST_CHECK_EQUAL(err, cupcfd::error::E_SUCCESS);
	BOOST_CHECK_EQUAL(count, 3);
}

// Test 7: Test counting where only last two elements are non-duplicate
BOOST_AUTO_TEST_CASE(distinctCount_test7)
{
	// The kernel expects sorted arrays only
	int source[11] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 3};
	int count;
	cupcfd::error::eCodes err = distinctCount(source, 11, &count);

	BOOST_CHECK_EQUAL(err, cupcfd::error::E_SUCCESS);
	BOOST_CHECK_EQUAL(count, 3);
}

// ================= Distinct Array  ===========================
// Test 1: Test finding distinct elements in arbitrary array
BOOST_AUTO_TEST_CASE(distinctArray_test1)
{
	// The kernel expects sorted arrays only
	int source[11] = {1, 1, 1, 1, 2, 3, 3, 4, 4, 7, 325};
	int * dest = (int *) malloc(sizeof(int) * 6);
	int resultCmp[6] = {1,2,3,4,7,325};

	cupcfd::error::eCodes err = distinctArray(source, 11, dest, 6);

	BOOST_CHECK_EQUAL(err, cupcfd::error::E_SUCCESS);
	BOOST_CHECK_EQUAL_COLLECTIONS(dest, dest + 6, resultCmp, resultCmp + 6);
}

// Test 2: Test finding distinct elements in arbitrary unsorted array
BOOST_AUTO_TEST_CASE(distinctArray_test2)
{
	// The kernel expects sorted arrays only
	int source[11] = {325, 1, 4, 1, 2, 7, 3, 1, 4, 3, 1};
	int * dest = (int *) malloc(sizeof(int) * 6);
	int resultCmp[6] = {1,2,3,4,7,325};

	cupcfd::error::eCodes err = distinctArray(source, 11, dest, 6);

	BOOST_CHECK_EQUAL(err, cupcfd::error::E_SUCCESS);
	BOOST_CHECK_EQUAL_COLLECTIONS(dest, dest + 6, resultCmp, resultCmp + 6);
}

// Test 3: Test finding distinct elements where all elements have duplicates
BOOST_AUTO_TEST_CASE(distinctArray_test3)
{
	// The kernel expects sorted arrays only
	int source[11] = {1, 1, 2, 2, 2, 3, 3, 4, 4, 4, 4};
	int * dest = (int *) malloc(sizeof(int) * 4);
	int resultCmp[4] = {1,2,3,4};

	cupcfd::error::eCodes err = distinctArray(source, 11, dest, 4);

	BOOST_CHECK_EQUAL(err, cupcfd::error::E_SUCCESS);
	BOOST_CHECK_EQUAL_COLLECTIONS(dest, dest + 4, resultCmp, resultCmp + 4);
}

// Test 4: Test finding distinct elements when no element has a duplicate
BOOST_AUTO_TEST_CASE(distinctArray_test4)
{
	// The kernel expects sorted arrays only
	int source[11] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 15};
	int * dest = (int *) malloc(sizeof(int) * 11);
	int resultCmp[11] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 15};

	cupcfd::error::eCodes err = distinctArray(source, 11, dest, 11);

	BOOST_CHECK_EQUAL(err, cupcfd::error::E_SUCCESS);
	BOOST_CHECK_EQUAL_COLLECTIONS(dest, dest + 11, resultCmp, resultCmp + 11);
}

// Test 5: Test finding distinct elements when only first element has no duplicate
BOOST_AUTO_TEST_CASE(distinctArray_test5)
{
	// The kernel expects sorted arrays only
	int source[11] = {1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2};
	int * dest = (int *) malloc(sizeof(int) * 2);
	int resultCmp[2] = {1, 2};

	cupcfd::error::eCodes err = distinctArray(source, 11, dest, 2);

	BOOST_CHECK_EQUAL(err, cupcfd::error::E_SUCCESS);
	BOOST_CHECK_EQUAL_COLLECTIONS(dest, dest + 2, resultCmp, resultCmp + 2);
}

// Test 6: Test finding distinct elements when only last element has no duplicate
BOOST_AUTO_TEST_CASE(distinctArray_test6)
{
	// The kernel expects sorted arrays only
	int source[11] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2};
	int * dest = (int *) malloc(sizeof(int) * 2);
	int resultCmp[2] = {1, 2};

	cupcfd::error::eCodes err = distinctArray(source, 11, dest, 2);

	BOOST_CHECK_EQUAL(err, cupcfd::error::E_SUCCESS);
	BOOST_CHECK_EQUAL_COLLECTIONS(dest, dest + 2, resultCmp, resultCmp + 2);
}

// Test 7: Test finding distinct elements when only first two elements have no duplicate
BOOST_AUTO_TEST_CASE(distinctArray_test7)
{
	// The kernel expects sorted arrays only
	int source[11] = {1, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3};
	int * dest = (int *) malloc(sizeof(int) * 3);
	int resultCmp[6] = {1, 2, 3};

	cupcfd::error::eCodes err = distinctArray(source, 11, dest, 3);

	BOOST_CHECK_EQUAL(err, cupcfd::error::E_SUCCESS);
	BOOST_CHECK_EQUAL_COLLECTIONS(dest, dest + 3, resultCmp, resultCmp + 3);
}

// Test 8: Test finding distinct elements when only last two elements have no duplicate
BOOST_AUTO_TEST_CASE(distinctArray_test8)
{
	// The kernel expects sorted arrays only
	int source[11] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 7};
	int * dest = (int *) malloc(sizeof(int) * 3);
	int resultCmp[3] = {1, 2, 7};

	cupcfd::error::eCodes err = distinctArray(source, 11, dest, 3);

	BOOST_CHECK_EQUAL(err, cupcfd::error::E_SUCCESS);
	BOOST_CHECK_EQUAL_COLLECTIONS(dest, dest + 3, resultCmp, resultCmp + 3);
}

// ================= Distinct Array + Array Creation ===========================

// ================= Distinct Array + Count ===========================
// Test 1: Test finding distinct elements with count also in arbitrary array
BOOST_AUTO_TEST_CASE(distinctArrayWithCount_test1)
{
	// The kernel expects sorted arrays only
	int source[11] = {1, 1, 1, 1, 2, 3, 3, 4, 4, 7, 325};
	int * dest = (int *) malloc(sizeof(int) * 6);
	int * count = (int *) malloc(sizeof(int) * 6);

	int resultCmp[6] = {1,2,3,4,7,325};
	int countCmp[6] = {4,1,2,2,1,1};

	cupcfd::error::eCodes err = distinctArray(source, 11, dest, 6, count, 6);
	BOOST_CHECK_EQUAL(err, cupcfd::error::E_SUCCESS);
	BOOST_CHECK_EQUAL_COLLECTIONS(dest, dest + 6, resultCmp, resultCmp + 6);
	BOOST_CHECK_EQUAL_COLLECTIONS(count, count + 6, countCmp, countCmp + 6);
}

// Test 2: Test finding distinct elements with count also in arbitrary array where all elements have duplicates
BOOST_AUTO_TEST_CASE(distinctArrayWithCount_test2)
{
	// The kernel expects sorted arrays only
	int source[11] = {1, 1, 2, 2, 2, 3, 3, 4, 4, 4, 4};
	int * dest = (int *) malloc(sizeof(int) * 4);
	int * count = (int *) malloc(sizeof(int) * 4);

	int resultCmp[4] = {1,2,3,4};
	int countCmp[4] = {2, 3, 2, 4};

	cupcfd::error::eCodes err = distinctArray(source, 11, dest, 4, count, 4);
	BOOST_CHECK_EQUAL(err, cupcfd::error::E_SUCCESS);
	BOOST_CHECK_EQUAL_COLLECTIONS(dest, dest + 4, resultCmp, resultCmp + 4);
	BOOST_CHECK_EQUAL_COLLECTIONS(count, count + 4, countCmp, countCmp + 4);
}

// Test 3: Test finding distinct elements with count also in arbitrary array where no elements has a duplicate
BOOST_AUTO_TEST_CASE(distinctArrayWithCount_test3)
{
	// The kernel expects sorted arrays only
	int source[11] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 15};
	int * dest = (int *) malloc(sizeof(int) * 11);
	int * count = (int *) malloc(sizeof(int) * 11);

	int resultCmp[11] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 15};
	int countCmp[11] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};

	cupcfd::error::eCodes err = distinctArray(source, 11, dest, 11, count, 11);
	BOOST_CHECK_EQUAL(err, cupcfd::error::E_SUCCESS);
	BOOST_CHECK_EQUAL_COLLECTIONS(dest, dest + 11, resultCmp, resultCmp + 11);
	BOOST_CHECK_EQUAL_COLLECTIONS(count, count + 11, countCmp, countCmp + 11);
}

// Test 4: Test finding distinct elements with count where first element only has no duplicate
BOOST_AUTO_TEST_CASE(distinctArrayWithCount_test4)
{
	// The kernel expects sorted arrays only
	int source[11] = {1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2};
	int * dest = (int *) malloc(sizeof(int) * 2);
	int * count = (int *) malloc(sizeof(int) * 2);

	int resultCmp[2] = {1, 2};
	int countCmp[2] = {1, 10};

	cupcfd::error::eCodes err = distinctArray(source, 11, dest, 2, count, 2);
	BOOST_CHECK_EQUAL(err, cupcfd::error::E_SUCCESS);
	BOOST_CHECK_EQUAL_COLLECTIONS(dest, dest + 2, resultCmp, resultCmp + 2);
	BOOST_CHECK_EQUAL_COLLECTIONS(count, count + 2, countCmp, countCmp + 2);
}

// Test 5: Test finding distinct elements with count where last element only has no duplicate
BOOST_AUTO_TEST_CASE(distinctArrayWithCount_test5)
{
	// The kernel expects sorted arrays only
	int source[11] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2};
	int * dest = (int *) malloc(sizeof(int) * 2);
	int * count = (int *) malloc(sizeof(int) * 2);

	int resultCmp[2] = {1, 2};
	int countCmp[2] = {10, 1};

	cupcfd::error::eCodes err = distinctArray(source, 11, dest, 2, count, 2);
	BOOST_CHECK_EQUAL(err, cupcfd::error::E_SUCCESS);
	BOOST_CHECK_EQUAL_COLLECTIONS(dest, dest + 2, resultCmp, resultCmp + 2);
	BOOST_CHECK_EQUAL_COLLECTIONS(count, count + 2, countCmp, countCmp + 2);
}

// Test 6: Test finding distinct elements with count where first two elements only have no duplicate
BOOST_AUTO_TEST_CASE(distinctArrayWithCount_test6)
{
	// The kernel expects sorted arrays only
	int source[11] = {1, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3};
	int * dest = (int *) malloc(sizeof(int) * 3);
	int * count = (int *) malloc(sizeof(int) * 3);

	int resultCmp[6] = {1, 2, 3};
	int countCmp[3] = {1, 1, 9};

	cupcfd::error::eCodes err = distinctArray(source, 11, dest, 3, count, 3);
	BOOST_CHECK_EQUAL(err, cupcfd::error::E_SUCCESS);
	BOOST_CHECK_EQUAL_COLLECTIONS(dest, dest + 3, resultCmp, resultCmp + 3);
	BOOST_CHECK_EQUAL_COLLECTIONS(count, count + 3, countCmp, countCmp + 3);
}

// Test 7: Test finding distinct elements with count where last two elements only have no duplicate
BOOST_AUTO_TEST_CASE(distinctArrayWithCount_test7)
{
	// The kernel expects sorted arrays only
	int source[11] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 7};
	int * dest = (int *) malloc(sizeof(int) * 3);
	int * count = (int *) malloc(sizeof(int) * 3);

	int resultCmp[3] = {1, 2, 7};
	int countCmp[3] = {9, 1, 1};

	cupcfd::error::eCodes err = distinctArray(source, 11, dest, 3, count, 3);
	BOOST_CHECK_EQUAL(err, cupcfd::error::E_SUCCESS);
	BOOST_CHECK_EQUAL_COLLECTIONS(dest, dest + 3, resultCmp, resultCmp + 3);
	BOOST_CHECK_EQUAL_COLLECTIONS(count, count + 3, countCmp, countCmp + 3);
}

// ================= Distinct Array + Count + Array Creation ===========================


//====================== Minus Count ===========================
// Test 1: Test MinusCount when array1 is larger than array2
BOOST_AUTO_TEST_CASE(minusCount_test1)
{
	int source1[7] = {1, 2, 2, 4, 8, 10, 12};
	int source2[4] = {2, 8, 21, 22};
	int count;

	minusCount(source1, 7, source2, 4, &count);

	BOOST_CHECK_EQUAL(count, 4);
}

// Test 2: Test MinusCount when array2 is larger than array1
BOOST_AUTO_TEST_CASE(minusCount_test2)
{
	int source1[7] = {1, 2, 2, 4, 8, 10, 12};
	int source2[8] = {2, 8, 21, 22, 100, 102, 200, 400};
	int count;

	minusCount(source1, 7, source2, 8, &count);

	BOOST_CHECK_EQUAL(count, 4);
}

// Test 3: Test MinusCount when array1 is all the same element
BOOST_AUTO_TEST_CASE(minusCount_test3)
{
	int source1[7] = {1, 1, 1, 1, 1, 1, 1};
	int source2[4] = {2, 8, 21, 22};
	int count;

	minusCount(source1, 7, source2, 4, &count);

	BOOST_CHECK_EQUAL(count, 7);
}

// Test 4: Test MinusCount when arrays have the same contents
BOOST_AUTO_TEST_CASE(minusCount_test4)
{
	int source1[4] = {2, 8, 21, 22};
	int source2[4] = {2, 8, 21, 22};
	int count;

	minusCount(source1, 4, source2, 4, &count);

	BOOST_CHECK_EQUAL(count, 0);
}

//====================== Minus Array ===========================
// Test 1: Test MinusArray when array1 is larger than array2
BOOST_AUTO_TEST_CASE(minusArray_test1)
{
	int source1[7] = {1, 2, 2, 4, 8, 10, 12};
	int source2[4] = {2, 8, 21, 22};
	int * result;
	int nResult;
	int resultCmp[4] = {1, 4, 10, 12};

	cupcfd::error::eCodes err = minusArray(source1, 7, source2, 4, &result, &nResult);

	BOOST_CHECK_EQUAL(err, cupcfd::error::E_SUCCESS);
	BOOST_CHECK_EQUAL(nResult, 4);
	BOOST_CHECK_EQUAL_COLLECTIONS(result, result + 4, resultCmp, resultCmp + 4);
}

// Test 2: Test MinusArray when array2 is larger than array1
BOOST_AUTO_TEST_CASE(minusArray_test2)
{
	int source1[7] = {1, 2, 2, 4, 8, 10, 12};
	int source2[8] = {2, 8, 21, 22, 100, 102, 200, 400};
	int * result;
	int nResult;
	int resultCmp[4] = {1, 4, 10, 12};

	cupcfd::error::eCodes err = minusArray(source1, 7, source2, 8, &result, &nResult);

	BOOST_CHECK_EQUAL(err, cupcfd::error::E_SUCCESS);
	BOOST_CHECK_EQUAL(nResult, 4);
	BOOST_CHECK_EQUAL_COLLECTIONS(result, result + 4, resultCmp, resultCmp + 4);
}

// Test 3: Test MinusArray when array1 is unsorted
BOOST_AUTO_TEST_CASE(minusArray_test3)
{
	int source1[7] = {12, 2, 4, 1, 8, 2, 10};
	int source2[4] = {2, 8, 21, 22};
	int * result;
	int nResult;
	int resultCmp[4] = {1, 4, 10, 12};

	cupcfd::error::eCodes err = minusArray(source1, 7, source2, 4, &result, &nResult);

	BOOST_CHECK_EQUAL(err, cupcfd::error::E_SUCCESS);
	BOOST_CHECK_EQUAL(nResult, 4);
	BOOST_CHECK_EQUAL_COLLECTIONS(result, result + 4, resultCmp, resultCmp + 4);
}

// Test 4: Test MinusArray when array1 is all the same element
BOOST_AUTO_TEST_CASE(minusArray_test4)
{
	int source1[7] = {1, 1, 1, 1, 1, 1, 1};
	int source2[4] = {2, 8, 21, 22};
	int * result;
	int nResult;
	int resultCmp[7] = {1, 1, 1, 1, 1, 1, 1};

	cupcfd::error::eCodes err = minusArray(source1, 7, source2, 4, &result, &nResult);

	BOOST_CHECK_EQUAL(err, cupcfd::error::E_SUCCESS);
	BOOST_CHECK_EQUAL(nResult, 7);
	BOOST_CHECK_EQUAL_COLLECTIONS(result, result + 7, resultCmp, resultCmp + 7);
}

// Test 5: Test MinusArray when the arrays have the same contents
BOOST_AUTO_TEST_CASE(minusArray_test5)
{
	int source1[4] = {2, 8, 21, 22};
	int source2[4] = {2, 8, 21, 22};
	int * result;
	int nResult;
	int resultCmp[0];

	cupcfd::error::eCodes err = minusArray(source1, 4, source2, 4, &result, &nResult);

	BOOST_CHECK_EQUAL(err, cupcfd::error::E_SUCCESS);
	BOOST_CHECK_EQUAL(nResult, 0);
	BOOST_CHECK_EQUAL_COLLECTIONS(result, result + 0, resultCmp, resultCmp + 0);
}

//====================== Intersect Count ===========================
// Test 1: Test Intersect Count with array
BOOST_AUTO_TEST_CASE(intersectCount_test1)
{
	int source1[6] = {1, 2, 4, 8, 10, 12};
	int source2[4] = {2, 8, 21, 22};
	int count;
	cupcfd::error::eCodes status;
	status = intersectCount(source1, 6, source2, 4, &count);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	// Duplicates should not count, so only 2 and 8 are in both.
	BOOST_CHECK_EQUAL(count, 2);
}

// Test 2: Test Intersect Count with array
BOOST_AUTO_TEST_CASE(intersectCount_test2)
{
	int source1[6] = {1, 2, 4, 8, 10, 22};
	int source2[4] = {2, 8, 21, 22};
	int count;
	cupcfd::error::eCodes status;
	status = intersectCount(source1, 6, source2, 4, &count);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	// Duplicates should not count, so only 2 and 8 are in both.
	BOOST_CHECK_EQUAL(count, 3);
}

// Test 3: Test Intersect Count with array2 larger
BOOST_AUTO_TEST_CASE(intersectCount_test3)
{
	int source1[6] = {1, 2, 4, 8, 10, 200};
	int source2[8] = {2, 8, 21, 22, 100, 102, 200, 400};
	int count;
	cupcfd::error::eCodes status;
	status = intersectCount(source1, 7, source2, 8, &count);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	// Duplicates should not count, so only 2, 8 and 200 are in both.
	BOOST_CHECK_EQUAL(count, 3);
}

// Test 4: Test Intersect Count with no intersection elements
BOOST_AUTO_TEST_CASE(intersectCount_test4)
{
	int source1[1] = {1};
	int source2[4] = {2, 8, 21, 22};
	int count;
	cupcfd::error::eCodes status;
	status = intersectCount(source1, 1, source2, 4, &count);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	BOOST_CHECK_EQUAL(count, 0);
}

// Test 5: Test Intersect Count with same array contents
BOOST_AUTO_TEST_CASE(intersectCount_test5)
{
	int source1[4] = {2, 8, 21, 22};
	int source2[4] = {2, 8, 21, 22};
	int count;
	cupcfd::error::eCodes status;
	status = intersectCount(source1, 4, source2, 4, &count);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	// Arrays are same, so all elements are in intersect
	BOOST_CHECK_EQUAL(count, 4);
}

//====================== Intersect Array ===========================
// Test 1: Test Intersect Count with array
BOOST_AUTO_TEST_CASE(intersectArray_test1)
{
	int source1[6] = {1, 2, 4, 8, 10, 12};
	int source2[4] = {2, 8, 21, 22};

	int * result;
	int nResult;
	int cmp[2] = {2, 8};

	cupcfd::error::eCodes status;
	status = intersectArray(source1, 6, source2, 4, &result, &nResult);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	BOOST_CHECK_EQUAL(nResult, 2);
	BOOST_CHECK_EQUAL_COLLECTIONS(result, result + 2, cmp, cmp + 2);
}

// Test 2: Test Intersect Count with array
BOOST_AUTO_TEST_CASE(intersectArray_test2)
{
	int source1[6] = {1, 2, 4, 8, 10, 22};
	int source2[4] = {2, 8, 21, 22};

	int * result;
	int nResult;
	int cmp[3] = {2, 8, 22};

	cupcfd::error::eCodes status;
	status = intersectArray(source1, 6, source2, 4, &result, &nResult);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	BOOST_CHECK_EQUAL(nResult, 3);
	BOOST_CHECK_EQUAL_COLLECTIONS(result, result + 3, cmp, cmp + 3);
}

// Test 3: Test Intersect Count with array2 larger
BOOST_AUTO_TEST_CASE(intersectArray_test3)
{
	int source1[6] = {1, 2, 4, 8, 10, 200};
	int source2[8] = {2, 8, 21, 22, 100, 102, 200, 400};

	int * result;
	int nResult;
	int cmp[3] = {2, 8, 200};

	cupcfd::error::eCodes status;
	status = intersectArray(source1, 7, source2, 8, &result, &nResult);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	BOOST_CHECK_EQUAL(nResult, 3);
	BOOST_CHECK_EQUAL_COLLECTIONS(result, result + 3, cmp, cmp + 3);
}

// Test 4: Test Intersect Count with no intersection elements
BOOST_AUTO_TEST_CASE(intersectArray_test4)
{
	int source1[1] = {1};
	int source2[4] = {2, 8, 21, 22};

	int * result;
	int nResult;
	int cmp[0];

	cupcfd::error::eCodes status;
	status = intersectArray(source1, 1, source2, 4, &result, &nResult);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	BOOST_CHECK_EQUAL(nResult, 0);
	BOOST_CHECK_EQUAL_COLLECTIONS(result, result + 0, cmp, cmp + 0);
}

// Test 5: Test Intersect Count with same array contents
BOOST_AUTO_TEST_CASE(intersectArray_test5)
{
	int source1[4] = {2, 8, 21, 22};
	int source2[4] = {2, 8, 21, 22};

	int * result;
	int nResult;
	int cmp[4] = {2, 8, 21, 22};

	cupcfd::error::eCodes status;
	status = intersectArray(source1, 4, source2, 4, &result, &nResult);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	BOOST_CHECK_EQUAL(nResult, 4);
	BOOST_CHECK_EQUAL_COLLECTIONS(result, result + 4, cmp, cmp + 4);
}

// === randomUniform ===
// ToDo: Is it possible to generate a suitable test for this?
