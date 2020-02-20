/*
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Tests for the array kernel functions from the utility operations
 *
 */

#define BOOST_TEST_MODULE ArrayKernel
#include <boost/test/unit_test.hpp>
#include <boost/test/output_test_stream.hpp>

#include <stdexcept>
#include "ArrayKernels.h"

// ============================================================
// ==================== Kernel Tests ==========================
// ============================================================
using namespace cupcfd::utility::kernels;

// ==================== add ============================
// Test 1: Test correction addition of two arrays
BOOST_AUTO_TEST_CASE(add_test1)
{
	int dest[5];

	int source1[5] = {1,2,3,4,5};
	int source2[5] = {1,2,18,4,7};

	int source1Cmp[5] = {1,2,3,4,5};
	int source2Cmp[5] = {1,2,18,4,7};
	int resultCmp[5] = {2, 4, 21, 8, 12};

	add(source1, source2, dest, 5);

	BOOST_CHECK_EQUAL_COLLECTIONS(dest, dest + 5, resultCmp, resultCmp + 5);
	BOOST_CHECK_EQUAL_COLLECTIONS(source1, source1 + 5, source1Cmp, source1Cmp + 5);
	BOOST_CHECK_EQUAL_COLLECTIONS(source2, source2 + 5, source2Cmp, source2Cmp + 5);
}

// ==================== uniqueCount ==========================
// Test 1: Test correct count of unique elements in arbitrary array
BOOST_AUTO_TEST_CASE(uniqueCount_test1)
{
	// The kernel expects sorted arrays only
	int source[11] = {1, 1, 1, 1, 2, 3, 3, 4, 4, 7, 325};
	int count = uniqueCount(source, 11);

	BOOST_CHECK_EQUAL(count, 3);
}

// Test 2: Test correct count of zero in array with no unique elements
BOOST_AUTO_TEST_CASE(uniqueCount_test2)
{
	// The kernel expects sorted arrays only
	int source[14] = {1, 1, 1, 1, 2, 2, 3, 3, 4, 4, 7, 7, 325, 325};

	int count = uniqueCount(source, 14);

	BOOST_CHECK_EQUAL(count, 0);
}

// Test 3: Test correct count of unique elements when all elements are unique
BOOST_AUTO_TEST_CASE(uniqueCount_test3)
{
	// The kernel expects sorted arrays only
	int source[6] = {1, 2, 3, 4, 7, 325};

	int count = uniqueCount(source, 6);

	BOOST_CHECK_EQUAL(count, 6);
}

// Test 4: Test correct count when only first element is unique
BOOST_AUTO_TEST_CASE(uniqueCount_test4)
{
	// The kernel expects sorted arrays only
	int source[11] = {1, 2, 2, 3, 3, 4, 4, 7, 7, 325, 325};

	int count = uniqueCount(source, 11);

	BOOST_CHECK_EQUAL(count, 1);
}

// Test 5: Test correct count when only last element is unique
BOOST_AUTO_TEST_CASE(uniqueCount_test5)
{
	// The kernel expects sorted arrays only
	int source[11] = {1, 1, 2, 2, 3, 3, 4, 4, 7, 7, 325};

	int count = uniqueCount(source, 11);

	BOOST_CHECK_EQUAL(count, 1);
}

// Test 6: Test correct count when last two elements are unique
BOOST_AUTO_TEST_CASE(uniqueCount_test6)
{
	// The kernel expects sorted arrays only
	int source[10] = {1, 1, 2, 2, 3, 3, 4, 4, 7, 325};

	int count = uniqueCount(source, 10);

	BOOST_CHECK_EQUAL(count, 2);
}

// ================= uniqueArray ===========================

// Test 1: Find the unique elements for an arbitrary array
BOOST_AUTO_TEST_CASE(uniqueArray_test1)
{
	// The kernel expects sorted arrays only
	int source[11] = {1, 1, 1, 1, 2, 3, 3, 4, 4, 7, 325};
	int count = uniqueCount(source, 11);
	BOOST_CHECK_EQUAL(count, 3);

	int result[3];
	int result_cmp[3] = {2, 7, 325};

	uniqueArray(source, result, 11);
	BOOST_CHECK_EQUAL_COLLECTIONS(result, result + 3, result_cmp, result_cmp + 3);
}

// Test 2: Check that the function runs to completion without error when there are
// no unique elements
BOOST_AUTO_TEST_CASE(uniqueArray_test2)
{
	// The kernel expects sorted arrays only
	int source[14] = {1, 1, 1, 1, 2, 2, 3, 3, 4, 4, 7, 7, 325, 325};
	int count = uniqueCount(source, 14);
	BOOST_CHECK_EQUAL(count, 0);

	int result[0];
	// Theoretically, it should never copy. If it does, it will go out of bounds in memory.
	uniqueArray(source, result, 14);
}

// Test 3: Check that the result is correct when the entire source is unique elements
BOOST_AUTO_TEST_CASE(uniqueArray_test3)
{
	// The kernel expects sorted arrays only
	int source[6] = {1, 2, 3, 4, 7, 325};
	int count = uniqueCount(source, 6);
	BOOST_CHECK_EQUAL(count, 6);

	int result[6];
	int result_cmp[6] = {1, 2, 3, 4, 7, 325};
	uniqueArray(source, result, 6);
	BOOST_CHECK_EQUAL_COLLECTIONS(result, result + 6, result_cmp, result_cmp + 6);
}

// Test 4: Check that the result is correct when only the first element is unique
BOOST_AUTO_TEST_CASE(uniqueArray_test4)
{
	// The kernel expects sorted arrays only
	int source[11] = {1, 2, 2, 3, 3, 4, 4, 7, 7, 325, 325};
	int count = uniqueCount(source, 11);
	BOOST_CHECK_EQUAL(count, 1);

	int result[1];
	int result_cmp[1] = {1};
	uniqueArray(source, result, 11);
	BOOST_CHECK_EQUAL_COLLECTIONS(result, result + 1, result_cmp, result_cmp + 1);
}

// Test 5: Check that the result is correct when only the last element is unique
BOOST_AUTO_TEST_CASE(uniqueArray_test5)
{
	// The kernel expects sorted arrays only
	int source[11] = {1, 1, 2, 2, 3, 3, 4, 4, 7, 7, 325};
	int count = uniqueCount(source, 11);
	BOOST_CHECK_EQUAL(count, 1);

	int result[1];
	int result_cmp[1] = {325};
	uniqueArray(source, result, 11);
	BOOST_CHECK_EQUAL_COLLECTIONS(result, result + 1, result_cmp, result_cmp + 1);
}

// Test 6: Check that the result is correct when the last two elements are unique
BOOST_AUTO_TEST_CASE(uniqueArray_test6)
{
	// The kernel expects sorted arrays only
	int source[10] = {1, 1, 2, 2, 3, 3, 4, 4, 7, 325};
	int count = uniqueCount(source, 10);
	BOOST_CHECK_EQUAL(count, 2);

	int result[2];
	int result_cmp[2] = {7, 325};
	uniqueArray(source, result, 10);
	BOOST_CHECK_EQUAL_COLLECTIONS(result, result + 2, result_cmp, result_cmp + 2);
}


// ==================== distinctCount ==========================

// Test 1: Find the correct number of distinct elements
BOOST_AUTO_TEST_CASE(distinctCount_test1)
{
	// The kernel expects sorted arrays only
	int source[11] = {1, 1, 1, 1, 2, 3, 3, 4, 4, 7, 325};
	int count = distinctCount(source, 11);

	BOOST_CHECK_EQUAL(count, 6);
}

// Test 2: Find correct number of distinct elements with no unique elements
BOOST_AUTO_TEST_CASE(distinctCount_test2)
{
	// The kernel expects sorted arrays only
	int source[11] = {1, 1, 2, 2, 2, 3, 3, 4, 4, 4, 4};
	int count = distinctCount(source, 11);

	BOOST_CHECK_EQUAL(count, 4);
}

// Test 3: Find correct number of distinct elements with all unique elements
BOOST_AUTO_TEST_CASE(distinctCount_test3)
{
	// The kernel expects sorted arrays only
	int source[11] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 15};
	int count = distinctCount(source, 11);

	BOOST_CHECK_EQUAL(count, 11);
}

// Test 4: Find correct number of distinct elements when only first is unique
BOOST_AUTO_TEST_CASE(distinctCount_test4)
{
	// The kernel expects sorted arrays only
	int source[11] = {1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2};
	int count = distinctCount(source, 11);

	BOOST_CHECK_EQUAL(count, 2);
}

// Test 5: Find correct number of distinct elements when only last is distinct
BOOST_AUTO_TEST_CASE(distinctCount_test5)
{
	// The kernel expects sorted arrays only
	int source[11] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2};
	int count = distinctCount(source, 11);

	BOOST_CHECK_EQUAL(count, 2);
}

// Test 6: Find correct number of distinct elements when first two are unique
BOOST_AUTO_TEST_CASE(distinctCount_test6)
{
	// The kernel expects sorted arrays only
	int source[11] = {1, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3};
	int count = distinctCount(source, 11);

	BOOST_CHECK_EQUAL(count, 3);
}

// Test 7: Find correct number of distinct elements when last two are unique
BOOST_AUTO_TEST_CASE(distinctCount_int_lastTwoDistinct)
{
	// The kernel expects sorted arrays only
	int source[11] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 3};
	int count = distinctCount(source, 11);

	BOOST_CHECK_EQUAL(count, 3);
}

// ==================== DistinctArray ==========================

// Test 1: Find Array of Distinct Elements
BOOST_AUTO_TEST_CASE(distinctArray_test1)
{
	// The kernel expects sorted arrays only
	int source[11] = {1, 1, 1, 1, 2, 3, 3, 4, 4, 7, 325};
	int * dest = (int *) malloc(sizeof(int) * 6);
	int resultCmp[6] = {1,2,3,4,7,325};

	distinctArray(source, dest, 11);
	BOOST_CHECK_EQUAL_COLLECTIONS(dest, dest + 6, resultCmp, resultCmp + 6);
}

// Test 2: Find Array of Distinct Elements when no unique elements
BOOST_AUTO_TEST_CASE(distinctArray_test2)
{
	// The kernel expects sorted arrays only
	int source[11] = {1, 1, 2, 2, 2, 3, 3, 4, 4, 4, 4};
	int * dest = (int *) malloc(sizeof(int) * 4);
	int resultCmp[4] = {1,2,3,4};

	distinctArray(source, dest, 11);
	BOOST_CHECK_EQUAL_COLLECTIONS(dest, dest + 4, resultCmp, resultCmp + 4);

}

// Test 3: Find Array of Distinct Elements when all are unique
BOOST_AUTO_TEST_CASE(distinctArray_test3)
{
	// The kernel expects sorted arrays only
	int source[11] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 15};
	int * dest = (int *) malloc(sizeof(int) * 11);
	int resultCmp[11] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 15};

	distinctArray(source, dest, 11);
	BOOST_CHECK_EQUAL_COLLECTIONS(dest, dest + 11, resultCmp, resultCmp + 11);
}

// Test 4: Find array of distinct elements when only first is unique
BOOST_AUTO_TEST_CASE(distinctArray_test4)
{
	// The kernel expects sorted arrays only
	int source[11] = {1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2};
	int * dest = (int *) malloc(sizeof(int) * 2);
	int resultCmp[2] = {1, 2};

	distinctArray(source, dest, 11);
	BOOST_CHECK_EQUAL_COLLECTIONS(dest, dest + 2, resultCmp, resultCmp + 2);
}

// Test 5: Find array of distinct elements when only last is unique
BOOST_AUTO_TEST_CASE(distinctArray_test5)
{
	// The kernel expects sorted arrays only
	int source[11] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2};
	int * dest = (int *) malloc(sizeof(int) * 2);
	int resultCmp[2] = {1, 2};

	distinctArray(source, dest, 11);
	BOOST_CHECK_EQUAL_COLLECTIONS(dest, dest + 2, resultCmp, resultCmp + 2);
}

// Test 6: Find array of distinct elements when only first two are unique
BOOST_AUTO_TEST_CASE(distinctArray_test6)
{
	// The kernel expects sorted arrays only
	int source[11] = {1, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3};
	int * dest = (int *) malloc(sizeof(int) * 3);
	int resultCmp[6] = {1, 2, 3};

	distinctArray(source, dest, 11);
	BOOST_CHECK_EQUAL_COLLECTIONS(dest, dest + 3, resultCmp, resultCmp + 3);
}

// Test 7: Find array of distinct elements when only last two are unique
BOOST_AUTO_TEST_CASE(distinctArray_test7)
{
	// The kernel expects sorted arrays only
	int source[11] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 7};
	int * dest = (int *) malloc(sizeof(int) * 3);
	int resultCmp[3] = {1, 2, 7};

	distinctArray(source, dest, 11);
	BOOST_CHECK_EQUAL_COLLECTIONS(dest, dest + 3, resultCmp, resultCmp + 3);
}


// ==================== DistinctWithCount ======================

// Test 1: Find Array of Distinct Elements with correct number of instances of each element
BOOST_AUTO_TEST_CASE(distinctArrayWithCount_test1)
{
	// The kernel expects sorted arrays only
	int source[11] = {1, 1, 1, 1, 2, 3, 3, 4, 4, 7, 325};
	int * dest = (int *) malloc(sizeof(int) * 6);
	int * count = (int *) malloc(sizeof(int) * 6);

	int resultCmp[6] = {1,2,3,4,7,325};
	int countCmp[6] = {4,1,2,2,1,1};

	distinctArray(source, dest, count, 11);
	BOOST_CHECK_EQUAL_COLLECTIONS(dest, dest + 6, resultCmp, resultCmp + 6);
	BOOST_CHECK_EQUAL_COLLECTIONS(count, count + 6, countCmp, countCmp + 6);
}

// Test 2: Find Array of Distinct Elements with correct number of instances of each element with no unique elements
BOOST_AUTO_TEST_CASE(distinctArrayWithCount_test2)
{
	// The kernel expects sorted arrays only
	int source[11] = {1, 1, 2, 2, 2, 3, 3, 4, 4, 4, 4};
	int * dest = (int *) malloc(sizeof(int) * 4);
	int * count = (int *) malloc(sizeof(int) * 4);

	int resultCmp[4] = {1,2,3,4};
	int countCmp[4] = {2, 3, 2, 4};

	distinctArray(source, dest, count, 11);
	BOOST_CHECK_EQUAL_COLLECTIONS(dest, dest + 4, resultCmp, resultCmp + 4);
	BOOST_CHECK_EQUAL_COLLECTIONS(count, count + 4, countCmp, countCmp + 4);
}

// Test 3: Find Array of Distinct Elements with correct number of instances of each element with all unique elements
BOOST_AUTO_TEST_CASE(distinctArrayWithCount_test3)
{
	// The kernel expects sorted arrays only
	int source[11] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 15};
	int * dest = (int *) malloc(sizeof(int) * 11);
	int * count = (int *) malloc(sizeof(int) * 11);

	int resultCmp[11] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 15};
	int countCmp[11] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};

	distinctArray(source, dest, count, 11);
	BOOST_CHECK_EQUAL_COLLECTIONS(dest, dest + 11, resultCmp, resultCmp + 11);
	BOOST_CHECK_EQUAL_COLLECTIONS(count, count + 11, countCmp, countCmp + 11);
}

// Test 4: Find Array of Distinct Elements with correct number of instances of each element when only first is unique
BOOST_AUTO_TEST_CASE(distinctArrayWithCount_test4)
{
	// The kernel expects sorted arrays only
	int source[11] = {1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2};
	int * dest = (int *) malloc(sizeof(int) * 2);
	int * count = (int *) malloc(sizeof(int) * 2);

	int resultCmp[2] = {1, 2};
	int countCmp[2] = {1, 10};

	distinctArray(source, dest, count, 11);
	BOOST_CHECK_EQUAL_COLLECTIONS(dest, dest + 2, resultCmp, resultCmp + 2);
	BOOST_CHECK_EQUAL_COLLECTIONS(count, count + 2, countCmp, countCmp + 2);
}

// Test 5: Find Array of Distinct Elements with correct number of instances of each element when only last is unique
BOOST_AUTO_TEST_CASE(distinctArrayWithCount_test5)
{
	// The kernel expects sorted arrays only
	int source[11] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2};
	int * dest = (int *) malloc(sizeof(int) * 2);
	int * count = (int *) malloc(sizeof(int) * 2);

	int resultCmp[2] = {1, 2};
	int countCmp[2] = {10, 1};

	distinctArray(source, dest, count, 11);
	BOOST_CHECK_EQUAL_COLLECTIONS(dest, dest + 2, resultCmp, resultCmp + 2);
	BOOST_CHECK_EQUAL_COLLECTIONS(count, count + 2, countCmp, countCmp + 2);
}

// Test 6: Find Array of Distinct Elements with correct number of instances of each element when only first two are unique
BOOST_AUTO_TEST_CASE(distinctArrayWithCount_test6)
{
	// The kernel expects sorted arrays only
	int source[11] = {1, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3};
	int * dest = (int *) malloc(sizeof(int) * 3);
	int * count = (int *) malloc(sizeof(int) * 3);

	int resultCmp[6] = {1, 2, 3};
	int countCmp[3] = {1, 1, 9};

	distinctArray(source, dest, count, 11);
	BOOST_CHECK_EQUAL_COLLECTIONS(dest, dest + 3, resultCmp, resultCmp + 3);
	BOOST_CHECK_EQUAL_COLLECTIONS(count, count + 3, countCmp, countCmp + 3);
}

// Test 7: Find Array of Distinct Elements with correct number of instances of each element when only last tow are unique
BOOST_AUTO_TEST_CASE(distinctArrayWithCount_test7)
{
	// The kernel expects sorted arrays only
	int source[11] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 7};
	int * dest = (int *) malloc(sizeof(int) * 3);
	int * count = (int *) malloc(sizeof(int) * 3);

	int resultCmp[3] = {1, 2, 7};
	int countCmp[3] = {9, 1, 1};

	distinctArray(source, dest, count, 11);
	BOOST_CHECK_EQUAL_COLLECTIONS(dest, dest + 3, resultCmp, resultCmp + 3);
	BOOST_CHECK_EQUAL_COLLECTIONS(count, count + 3, countCmp, countCmp + 3);
}

//====================== Minus Count ===========================

// Test 1: Get correct number of elements left in minus array when first array is larger
BOOST_AUTO_TEST_CASE(minusCount_test1)
{
	int source1[7] = {1, 2, 2, 4, 8, 10, 12};
	int source2[4] = {2, 8, 21, 22};

	int count = minusCount(source1, 7, source2, 4);

	BOOST_CHECK_EQUAL(count, 4);
}

// Test 2: Get correct number of elements left in minus array when second array is larger
BOOST_AUTO_TEST_CASE(minusCount_test2)
{
	int source1[7] = {1, 2, 2, 4, 8, 10, 12};
	int source2[8] = {2, 8, 21, 22, 100, 102, 200, 400};

	int count = minusCount(source1, 7, source2, 8);

	BOOST_CHECK_EQUAL(count, 4);
}

// Test 3: Get correct number of elements left in minus array when first array is all same element
BOOST_AUTO_TEST_CASE(minusCount_test3)
{
	int source1[7] = {1, 1, 1, 1, 1, 1, 1};
	int source2[4] = {2, 8, 21, 22};

	int count = minusCount(source1, 7, source2, 4);

	BOOST_CHECK_EQUAL(count, 7);
}

// Test 4: Get correct number of elements left in minus array when arrays are the same
BOOST_AUTO_TEST_CASE(minusCount_test4)
{
	int source1[4] = {2, 8, 21, 22};
	int source2[4] = {2, 8, 21, 22};

	int count = minusCount(source1, 4, source2, 4);

	BOOST_CHECK_EQUAL(count, 0);
}

//====================== Minus Array ===========================

// Test 1: Get correct set minus array when first array is larger
BOOST_AUTO_TEST_CASE(minusArray_test1)
{
	int source1[7] = {1, 2, 2, 4, 8, 10, 12};
	int source2[4] = {2, 8, 21, 22};
	int result[4];
	int resultCmp[4] = {1, 4, 10, 12};

	minusArray(source1, 7, source2, 4, result, 4);

	BOOST_CHECK_EQUAL_COLLECTIONS(result, result + 4, resultCmp, resultCmp + 4);
}

// Test 2: Get correct set minus array when second array is larger
BOOST_AUTO_TEST_CASE(minusArray_test2)
{
	int source1[7] = {1, 2, 2, 4, 8, 10, 12};
	int source2[8] = {2, 8, 21, 22, 100, 102, 200, 400};
	int result[4];
	int resultCmp[4] = {1, 4, 10, 12};

	minusArray(source1, 7, source2, 8, result, 4);

	BOOST_CHECK_EQUAL_COLLECTIONS(result, result + 4, resultCmp, resultCmp + 4);
}

// Test 3: Get correct set minus array when first array is all same element
BOOST_AUTO_TEST_CASE(minusArray_test3)
{
	int source1[7] = {1, 1, 1, 1, 1, 1, 1};
	int source2[4] = {2, 8, 21, 22};
	int result[7];
	int resultCmp[7] = {1, 1, 1, 1, 1, 1, 1};

	minusArray(source1, 7, source2, 4, result, 7);

	BOOST_CHECK_EQUAL_COLLECTIONS(result, result + 7, resultCmp, resultCmp + 7);
}

// Test 4: Get correct set minus array when arrays have same contents
BOOST_AUTO_TEST_CASE(minusArray_test4)
{
	int source1[4] = {2, 8, 21, 22};
	int source2[4] = {2, 8, 21, 22};
	int result[0];
	int resultCmp[0];

	minusArray(source1, 4, source2, 4, result, 0);

	BOOST_CHECK_EQUAL_COLLECTIONS(result, result + 0, resultCmp, resultCmp + 0);
}


//====================== Intersect Count ===========================

// Test 1: Get correct set intersect count when first array is larger and
// first + last elements to be discarded
BOOST_AUTO_TEST_CASE(intersectCount_test1)
{
	int source1[6] = {1, 2, 4, 8, 10, 12};
	int source2[4] = {2, 8, 21, 22};

	int count = intersectCount(source1, 6, source2, 4);

	// Duplicates should not count, so only 2 and 8 are in both.
	BOOST_CHECK_EQUAL(count, 2);
}

// Test 2: Get correct set intersect count when first array is larger and
// first to be discarded
BOOST_AUTO_TEST_CASE(intersectCount_test2)
{
	int source1[6] = {1, 2, 4, 8, 10, 22};
	int source2[4] = {2, 8, 21, 22};

	int count = intersectCount(source1, 6, source2, 4);

	// Duplicates should not count, so only 2 and 8 are in both.
	BOOST_CHECK_EQUAL(count, 3);
}

// Test 3: Get correct set intersect count when second array is larger
BOOST_AUTO_TEST_CASE(intersectCount_test3)
{
	int source1[6] = {1, 2, 4, 8, 10, 200};
	int source2[8] = {2, 8, 21, 22, 100, 102, 200, 400};

	int count = intersectCount(source1, 7, source2, 8);

	// Duplicates should not count, so only 2, 8 and 200 are in both.
	BOOST_CHECK_EQUAL(count, 3);
}

// Test 4: Get correct set intersect count when no elements are in both arrays
BOOST_AUTO_TEST_CASE(intersectCount_test4)
{
	int source1[1] = {1};
	int source2[4] = {2, 8, 21, 22};

	int count = intersectCount(source1, 1, source2, 4);

	BOOST_CHECK_EQUAL(count, 0);
}

// Test 5: Get correct set intersect count when all elements are in both arrays
BOOST_AUTO_TEST_CASE(intersectCount_test5)
{
	int source1[4] = {2, 8, 21, 22};
	int source2[4] = {2, 8, 21, 22};

	int count = intersectCount(source1, 4, source2, 4);

	// Arrays are same, so all elements are in intersect
	BOOST_CHECK_EQUAL(count, 4);
}


//====================== Intersect Array ===========================

// Test 1: Get correct set intersect array when first array is larger and
// first + last elements to be discarded
BOOST_AUTO_TEST_CASE(intersectArray_test1)
{
	int source1[6] = {1, 2, 4, 8, 10, 12};
	int source2[4] = {2, 8, 21, 22};

	int result[2];
	int nResult = 2;
	int cmp[2] = {2, 8};

	intersectArray(source1, 6, source2, 4, result, nResult);
	BOOST_CHECK_EQUAL_COLLECTIONS(result, result + 2, cmp, cmp + 2);
}

// Test 2: Get correct set intersect array when first array is larger and
// first to be discarded
BOOST_AUTO_TEST_CASE(intersectArray_test2)
{
	int source1[6] = {1, 2, 4, 8, 10, 22};
	int source2[4] = {2, 8, 21, 22};

	int result[3];
	int nResult = 3;
	int cmp[3] = {2, 8, 22};

	intersectArray(source1, 6, source2, 4, result, nResult);
	BOOST_CHECK_EQUAL_COLLECTIONS(result, result + 3, cmp, cmp + 3);
}

// Test 3: Get correct set intersect array when second array is larger
BOOST_AUTO_TEST_CASE(intersectArray_test3)
{
	int source1[6] = {1, 2, 4, 8, 10, 200};
	int source2[8] = {2, 8, 21, 22, 100, 102, 200, 400};

	int result[3];
	int nResult = 3;
	int cmp[3] = {2, 8, 200};

	intersectArray(source1, 7, source2, 8, result, nResult);
	BOOST_CHECK_EQUAL_COLLECTIONS(result, result + 3, cmp, cmp + 3);
}

// Test 4: Get correct set intersect array when no elements are in both arrays
BOOST_AUTO_TEST_CASE(intersectArray_test4)
{
	int source1[1] = {1};
	int source2[4] = {2, 8, 21, 22};

	int result[0];
	int nResult = 0;
	int cmp[0];

	intersectArray(source1, 1, source2, 4, result, nResult);
	BOOST_CHECK_EQUAL_COLLECTIONS(result, result + 0, cmp, cmp + 0);
}

// Test 5: Get correct set intersect array when all elements are in both arrays
BOOST_AUTO_TEST_CASE(intersectArray_test5)
{
	int source1[4] = {2, 8, 21, 22};
	int source2[4] = {2, 8, 21, 22};

	int result[4];
	int nResult = 4;
	int cmp[4] = {2, 8, 21, 22};

	intersectArray(source1, 4, source2, 4, result, nResult);
	BOOST_CHECK_EQUAL_COLLECTIONS(result, result + 4, cmp, cmp + 4);
}

// === randomUniform ===
// ToDo: Is it possible to generate a suitable test for this?
