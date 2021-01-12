/*
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Tests for the SparseMatrixCOO class
 */

#define BOOST_TEST_MODULE SparseMatrixCOO
#include <boost/test/unit_test.hpp>
#include <boost/test/output_test_stream.hpp>
#include <stdexcept>

#include "SparseMatrixCOO.h"
#include "Error.h"

#include <iostream>

using namespace cupcfd::data_structures;

// =========================================================================
// === Constructor Tests ===
// =========================================================================
// Test 1: Default Constructor
BOOST_AUTO_TEST_CASE(constructor_test1)
{
	SparseMatrixCOO<int, int> matrix;

	BOOST_CHECK_EQUAL(matrix.m, 1);
	BOOST_CHECK_EQUAL(matrix.n, 1);
	BOOST_CHECK_EQUAL(matrix.baseIndex, 0);
	BOOST_CHECK_EQUAL(matrix.nnz, 0);
}

// Test 2: Set M, N Values
BOOST_AUTO_TEST_CASE(constructor_test2)
{
	SparseMatrixCOO<int, int> matrix(4, 7);

	BOOST_CHECK_EQUAL(matrix.m, 4);
	BOOST_CHECK_EQUAL(matrix.n, 7);
	BOOST_CHECK_EQUAL(matrix.baseIndex, 0);
	BOOST_CHECK_EQUAL(matrix.nnz, 0);
}

// Test 3: Set M, N, BaseIndex Values
BOOST_AUTO_TEST_CASE(constructor_test3)
{
	SparseMatrixCOO<int, int> matrix(6, 9, 5);

	BOOST_CHECK_EQUAL(matrix.m, 6);
	BOOST_CHECK_EQUAL(matrix.n, 9);
	BOOST_CHECK_EQUAL(matrix.baseIndex, 5);
	BOOST_CHECK_EQUAL(matrix.nnz, 0);
}

// === SetElement Tests ===
// Test 1: Add non-existing entry
BOOST_AUTO_TEST_CASE(setElement_test1)
{
	// Setup
	SparseMatrixCOO<int, int> matrix(2, 2, 0);

	int elements[4] = {2, 4, 3, 20};
	int rows[4] = {0, 1, 0, 1};
	int columns[4] = {0, 1, 1, 0};

	for(int i = 0; i < 4; i++)
	{
		cupcfd::error::eCodes status = matrix.setElement(rows[i], columns[i], elements[i]);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	}

	int rowsCmp[4] = {0, 0, 1, 1};
	int colsCmp[4] = {0, 1, 0, 1};
	int valCmp[4] = {2, 3, 20, 4};

	BOOST_CHECK_EQUAL_COLLECTIONS(&matrix.row[0], &matrix.row[0] + 4, rowsCmp, rowsCmp + 4);
	BOOST_CHECK_EQUAL_COLLECTIONS(&matrix.col[0], &matrix.col[0] + 4, colsCmp, colsCmp + 4);
	BOOST_CHECK_EQUAL_COLLECTIONS(&matrix.val[0], &matrix.val[0] + 4, valCmp, valCmp + 4);

	BOOST_CHECK_EQUAL(matrix.nnz, 4);
}

// Test 2: Overwrite existing non-zero entry
BOOST_AUTO_TEST_CASE(setElement_test2)
{
	// Setup
	SparseMatrixCOO<int, int> matrix(2, 2, 0);

	// Set the values such that (0, 0) = 20, (1, 0) = 3
	// but these are overwriting 2 and 4 respectively.
	int elements[4] = {2, 4, 3, 20};
	int rows[4] = {0, 1, 1, 0};
	int columns[4] = {0, 0, 0, 0};

	// Check nnz count is 4
	BOOST_CHECK_EQUAL(matrix.nnz, 0);

	for(int i = 0; i < 4; i++)
	{
		cupcfd::error::eCodes status = matrix.setElement(rows[i], columns[i], elements[i]);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	}

	int rowsCmp[2] = {0, 1};
	int colsCmp[2] = {0, 0};
	int valCmp[2] = {20, 3};

	BOOST_CHECK_EQUAL_COLLECTIONS(&matrix.row[0], &matrix.row[0] + 2, rowsCmp, rowsCmp + 2);
	BOOST_CHECK_EQUAL_COLLECTIONS(&matrix.col[0], &matrix.col[0] + 2, colsCmp, colsCmp + 2);
	BOOST_CHECK_EQUAL_COLLECTIONS(&matrix.val[0], &matrix.val[0] + 2, valCmp, valCmp + 2);

	// NNZ Count should be 2 since we added 2, and overwrote 2
	BOOST_CHECK_EQUAL(matrix.nnz, 2);
}

// Test 3: Attempt to set an element outside the lower row bounds
BOOST_AUTO_TEST_CASE(setElement_test3)
{
	// Setup
	SparseMatrixCOO<int, int> matrix(2, 2, 0);

	cupcfd::error::eCodes status = matrix.setElement(-1, 0, 2);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_MATRIX_ROW_OOB);
}

// Test 4: Attempt to set an element outside the lower column bounds
BOOST_AUTO_TEST_CASE(setElement_test4)
{
	// Setup
	SparseMatrixCOO<int, int> matrix(2, 2, 0);

	cupcfd::error::eCodes status = matrix.setElement(0, -1, 2);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_MATRIX_COL_OOB);
}

// Test 5: Attempt to set an element outside the upper row bounds
BOOST_AUTO_TEST_CASE(setElement_test5)
{
	// Setup
	SparseMatrixCOO<int, int> matrix(2, 2, 0);

	cupcfd::error::eCodes status = matrix.setElement(2, 0, 2);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_MATRIX_ROW_OOB);
}

// Test 6: Attempt to set an element outside the upper column bounds
BOOST_AUTO_TEST_CASE(setElement_test6)
{
	// Setup
	SparseMatrixCOO<int, int> matrix(2, 2, 0);

	cupcfd::error::eCodes status = matrix.setElement(0, 2, 2);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_MATRIX_COL_OOB);
}

// Test 7: Overwrite non-existing zero entry with a non-zero base index
BOOST_AUTO_TEST_CASE(setElement_test7)
{
	// Setup
	SparseMatrixCOO<int, int> matrix(2, 2, 4);

	int elements[4] = {2, 4, 3, 20};
	int rows[4] = {4, 5, 4, 5};
	int columns[4] = {4, 5, 5, 4};

	for(int i = 0; i < 4; i++)
	{
		cupcfd::error::eCodes status = matrix.setElement(rows[i], columns[i], elements[i]);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	}

	int rowsCmp[4] = {4, 4, 5, 5};
	int colsCmp[4] = {4, 5, 4, 5};
	int valCmp[4] = {2, 3, 20, 4};

	BOOST_CHECK_EQUAL_COLLECTIONS(&matrix.row[0], &matrix.row[0] + 4, rowsCmp, rowsCmp + 4);
	BOOST_CHECK_EQUAL_COLLECTIONS(&matrix.col[0], &matrix.col[0] + 4, colsCmp, colsCmp + 4);
	BOOST_CHECK_EQUAL_COLLECTIONS(&matrix.val[0], &matrix.val[0] + 4, valCmp, valCmp + 4);
}

// Test 8: Overwrite existing non-zero entry with a non-zero base matrix index
BOOST_AUTO_TEST_CASE(setElement_test8)
{
	// Setup
	SparseMatrixCOO<int, int> matrix(2, 2, 4);

	// Set the values such that (4, 4) = 20, (5, 4) = 3
	// but these are overwriting 2 and 4 respectively.
	int elements[4] = {2, 4, 3, 20};
	int rows[4] = {4, 5, 5, 4};
	int columns[4] = {4, 4, 4, 4};

	for(int i = 0; i < 4; i++)
	{
		cupcfd::error::eCodes status = matrix.setElement(rows[i], columns[i], elements[i]);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	}

	int rowsCmp[2] = {4, 5};
	int colsCmp[2] = {4, 4};
	int valCmp[2] = {20, 3};

	BOOST_CHECK_EQUAL_COLLECTIONS(&matrix.row[0], &matrix.row[0] + 2, rowsCmp, rowsCmp + 2);
	BOOST_CHECK_EQUAL_COLLECTIONS(&matrix.col[0], &matrix.col[0] + 2, colsCmp, colsCmp + 2);
	BOOST_CHECK_EQUAL_COLLECTIONS(&matrix.val[0], &matrix.val[0] + 2, valCmp, valCmp + 2);
}

// Test 9: Attempt to set an element outside the lower row bounds with a non-zero base matrix index
BOOST_AUTO_TEST_CASE(setElement_test9)
{
	// Setup
	SparseMatrixCOO<int, int> matrix(2, 2, 4);

	cupcfd::error::eCodes status = matrix.setElement(3, 4, 2);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_MATRIX_ROW_OOB);
}

// Test 10: Attempt to set an element outside the lower column bounds with a non-zero base matrix index
BOOST_AUTO_TEST_CASE(setElement_test10)
{
	// Setup
	SparseMatrixCOO<int, int> matrix(2, 2, 4);

	cupcfd::error::eCodes status = matrix.setElement(4, 3, 2);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_MATRIX_COL_OOB);
}

// Test 11: Attempt to set an element outside the upper row bounds with a non-zero base matrix index
BOOST_AUTO_TEST_CASE(setElement_test11)
{
	// Setup
	SparseMatrixCOO<int, int> matrix(2, 2, 4);

	cupcfd::error::eCodes status = matrix.setElement(6, 4, 2);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_MATRIX_ROW_OOB);
}

// Test 12: Attempt to set an element outside the upper column bounds with a non-zero base matrix index
BOOST_AUTO_TEST_CASE(setElement_test12)
{
	// Setup
	SparseMatrixCOO<int, int> matrix(2, 2, 4);

	cupcfd::error::eCodes status = matrix.setElement(4, 6, 2);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_MATRIX_COL_OOB);
}

// === getElement ===
// Test 1: Correctly get a value that is non-zero with a base index of 0
BOOST_AUTO_TEST_CASE(getElement_test1)
{
	// Setup
	SparseMatrixCOO<int, int> matrix(8, 8, 0);

	int rows[4] = {0, 4, 6, 7};
	int columns[4] = {0, 1, 5, 3};
	int elements[4] = {2, 4, 3, 20};

	for(int i = 0; i < 4; i++)
	{
		cupcfd::error::eCodes status = matrix.setElement(rows[i], columns[i], elements[i]);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	}

	int vals[4];

	for(int i = 0; i < 4; i++)
	{
		cupcfd::error::eCodes status = matrix.getElement(rows[i], columns[i], vals + i);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	}

	BOOST_CHECK_EQUAL_COLLECTIONS(vals, vals + 4, elements, elements + 4);
}

// Test 2: Correctly get a value that is not set (zero) with a base index of 0
BOOST_AUTO_TEST_CASE(getElement_test2)
{
	// Setup
	SparseMatrixCOO<int, int> matrix(8, 8, 0);

	int rows[4] = {0, 4, 6, 7};
	int columns[4] = {0, 1, 5, 3};
	int elements[4] = {2, 4, 3, 20};

	for(int i = 0; i < 4; i++)
	{
		cupcfd::error::eCodes status = matrix.setElement(rows[i], columns[i], elements[i]);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	}

	int val;
	cupcfd::error::eCodes status = matrix.getElement(3, 6, &val);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	BOOST_CHECK_EQUAL(val, 0);
}

// Test 3: Catch the error case where we exceed the row bounds
// of an index scheme where the base Index is 0
BOOST_AUTO_TEST_CASE(getElement_test3)
{
	// Setup
	SparseMatrixCOO<int, int> matrix(8, 8, 0);

	int val;
	cupcfd::error::eCodes status = matrix.getElement(-1, 4, &val);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_MATRIX_ROW_OOB);
}

// Test 4: Catch the error case where we exceed the column bounds
// of an index scheme where the base Index is 0
BOOST_AUTO_TEST_CASE(getElement_test4)
{
	// Setup
	SparseMatrixCOO<int, int> matrix(8, 8, 0);

	int val;
	cupcfd::error::eCodes status = matrix.getElement(4, -1, &val);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_MATRIX_COL_OOB);
}

// Test 5: Catch the error case where we exceed the upper row bounds
// of an index scheme where the base Index is 0
BOOST_AUTO_TEST_CASE(getElement_test5)
{
	// Setup
	SparseMatrixCOO<int, int> matrix(8, 8, 0);

	int val;
	cupcfd::error::eCodes status = matrix.getElement(8, 4, &val);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_MATRIX_ROW_OOB);
}

// Test 6: Catch the error case where we exceed the upper column bounds
// of an index scheme where the base Index is 0
BOOST_AUTO_TEST_CASE(getElement_test6)
{
	// Setup
	SparseMatrixCOO<int, int> matrix(8, 8, 0);

	int val;
	cupcfd::error::eCodes status = matrix.getElement(4, 8, &val);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_MATRIX_COL_OOB);
}

// Test 7: Correctly get a value that is non-zero with a non-zero base-index
BOOST_AUTO_TEST_CASE(getElement_test7)
{
	// Setup
	SparseMatrixCOO<int, int> matrix(8, 8, 7);

	int rows[4] = {7, 11, 13, 14};
	int columns[4] = {7, 8, 12, 10};
	int elements[4] = {2, 4, 3, 20};

	for(int i = 0; i < 4; i++)
	{
		cupcfd::error::eCodes status = matrix.setElement(rows[i], columns[i], elements[i]);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	}

	int vals[4];

	for(int i = 0; i < 4; i++)
	{
		cupcfd::error::eCodes status = matrix.getElement(rows[i], columns[i], vals + i);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	}

	BOOST_CHECK_EQUAL_COLLECTIONS(vals, vals + 4, elements, elements + 4);
}

// Test 8: Correctly set a value that is not set (zero) with a non-zero base-index
BOOST_AUTO_TEST_CASE(getElement_test8)
{
	// Setup
	SparseMatrixCOO<int, int> matrix(8, 8, 7);

	int rows[4] = {7, 11, 13, 14};
	int columns[4] = {7, 8, 12, 10};
	int elements[4] = {2, 4, 3, 20};

	for(int i = 0; i < 4; i++)
	{
		cupcfd::error::eCodes status = matrix.setElement(rows[i], columns[i], elements[i]);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	}

	int val;
	cupcfd::error::eCodes status = matrix.getElement(10, 13, &val);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	BOOST_CHECK_EQUAL(val, 0);
}

// Test 9: Catch the error case where we exceed the lower row bounds
// of an index scheme with a non-zero base-index
BOOST_AUTO_TEST_CASE(getElement_test9)
{
	// Setup
	SparseMatrixCOO<int, int> matrix(8, 8, 8);

	int val;
	cupcfd::error::eCodes status = matrix.getElement(7, 9, &val);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_MATRIX_ROW_OOB);
}

// Test 10: Catch the error case where we exceed the lower column bounds
// of an index scheme with a non-zero base-index
BOOST_AUTO_TEST_CASE(getElement_test10)
{
	// Setup
	SparseMatrixCOO<int, int> matrix(8, 8, 8);

	int val;
	cupcfd::error::eCodes status = matrix.getElement(9, 7, &val);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_MATRIX_COL_OOB);
}


// Test 11: Catch the error case where we exceed the upper row bounds
// of an index scheme with a non-zero base-index
BOOST_AUTO_TEST_CASE(getElement_test11)
{
	// Setup
	SparseMatrixCOO<int, int> matrix(8, 8, 8);

	int val;
	cupcfd::error::eCodes status = matrix.getElement(16, 9, &val);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_MATRIX_ROW_OOB);
}

// Test 12: Catch the error case where we exceed the upper column bounds
// of an index scheme with a non-zero base-index
BOOST_AUTO_TEST_CASE(getElement_test12)
{
	// Setup
	SparseMatrixCOO<int, int> matrix(8, 8, 8);

	int val;
	cupcfd::error::eCodes status = matrix.getElement(9, 16, &val);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_MATRIX_COL_OOB);
}

// === clear ===
// Test 1: Check that the matrix vectors are cleared after calling clear
BOOST_AUTO_TEST_CASE(clear_test1)
{
	cupcfd::error::eCodes status;

	// Setup
	SparseMatrixCOO<int, int> matrix(8, 8, 0);

	// Setup data so that there is at least two elements
	status = matrix.setElement(3, 4, 15);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	status = matrix.setElement(7, 7, 19);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	status = matrix.clear();
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	// Check sizes are 0
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	BOOST_CHECK_EQUAL(matrix.row.size(), 0);
	BOOST_CHECK_EQUAL(matrix.col.size(), 0);
	BOOST_CHECK_EQUAL(matrix.val.size(), 0);

	// Check that retrieving the previously set index now has a value of 0
	int val;
	status = matrix.getElement(3, 4, &val);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	BOOST_CHECK_EQUAL(val, 0);
}

// === resize ===
// Test 1: Check that an existing matrix resets to appropriate values after a resize
// Data should also be cleared
BOOST_AUTO_TEST_CASE(resize_test1)
{
	cupcfd::error::eCodes status;

	// Setup
	SparseMatrixCOO<int, int> matrix(8, 8, 2);

	// Setup data so that there is at least two elements
	status = matrix.setElement(4, 5, 15);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	status = matrix.setElement(8, 8, 19);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	BOOST_CHECK_EQUAL(matrix.m, 8);
	BOOST_CHECK_EQUAL(matrix.n, 8);
	BOOST_CHECK_EQUAL(matrix.nnz, 2);

	// Test and Check
	status = matrix.resize(4, 4);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	// Check that the values of m, n and nnz are reset
	BOOST_CHECK_EQUAL(matrix.m, 4);
	BOOST_CHECK_EQUAL(matrix.n, 4);
	BOOST_CHECK_EQUAL(matrix.nnz, 0);

	// Check that the baseIndex is still the same
	BOOST_CHECK_EQUAL(matrix.baseIndex, 2);

	// Check sizes are 0
	BOOST_CHECK_EQUAL(matrix.row.size(), 0);
	BOOST_CHECK_EQUAL(matrix.col.size(), 0);
	BOOST_CHECK_EQUAL(matrix.val.size(), 0);

	// Check that retrieving the previously set index now has a value of 0
	int val;
	status = matrix.getElement(4, 5, &val);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	BOOST_CHECK_EQUAL(val, 0);
}

// === getNonZeroRowIndexes ===
// Test 1: Get correct row indexes for rows with set values
BOOST_AUTO_TEST_CASE(getNonZeroRowIndexes_test1)
{
	cupcfd::error::eCodes status;

	SparseMatrixCOO<int, int> matrix(4, 4, 0);

	status = matrix.setElement(0, 0, 2);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	status = matrix.setElement(3, 1, 4);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	status = matrix.setElement(0, 1, 3);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	status = matrix.setElement(3, 0, 20);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	int * rowIndexes;
	int  nRowIndexes;

	status = matrix.getNonZeroRowIndexes(&rowIndexes, &nRowIndexes);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	BOOST_CHECK_EQUAL(nRowIndexes, 2);
	BOOST_CHECK_EQUAL(rowIndexes[0], 0);
	BOOST_CHECK_EQUAL(rowIndexes[1], 3);
	free(rowIndexes);
}

// === getRowColumnIndexes ===
// Test 1: Get correct column indexes for a row
BOOST_AUTO_TEST_CASE(getRowColumnIndexes_test1)
{
	cupcfd::error::eCodes status;

	SparseMatrixCOO<int, int> matrix(4, 4, 0);

	status = matrix.setElement(0, 3, 3);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	status = matrix.setElement(0, 0, 2);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	status = matrix.setElement(3, 1, 4);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	status = matrix.setElement(3, 0, 20);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	status = matrix.setElement(1, 1, 2);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	int * colIndexes;
	int  nColIndexes;

	status = matrix.getRowColumnIndexes(0, &colIndexes, &nColIndexes);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	BOOST_CHECK_EQUAL(nColIndexes, 2);
	BOOST_CHECK_EQUAL(colIndexes[0], 0);
	BOOST_CHECK_EQUAL(colIndexes[1], 3);
	free(colIndexes);

	status = matrix.getRowColumnIndexes(1, &colIndexes, &nColIndexes);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	BOOST_CHECK_EQUAL(nColIndexes, 1);
	BOOST_CHECK_EQUAL(colIndexes[0], 1);
	free(colIndexes);

	status = matrix.getRowColumnIndexes(3, &colIndexes, &nColIndexes);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	BOOST_CHECK_EQUAL(nColIndexes, 2);
	BOOST_CHECK_EQUAL(colIndexes[0], 0);
	BOOST_CHECK_EQUAL(colIndexes[1], 1);
	free(colIndexes);
}

// Test 2: Check for error if row index is lower than matrix index range
BOOST_AUTO_TEST_CASE(getRowColumnIndexes_test2)
{
	cupcfd::error::eCodes status;

	SparseMatrixCOO<int, int> matrix(4, 4, 3);

	status = matrix.setElement(3, 3, 4);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	int * colIndexes;
	int  nColIndexes;

	status = matrix.getRowColumnIndexes(2, &colIndexes, &nColIndexes);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_MATRIX_ROW_OOB);
}

// Test 3: Check for error if row index is higher than matrix index range
BOOST_AUTO_TEST_CASE(getRowColumnIndexes_test3)
{
	cupcfd::error::eCodes status;

	SparseMatrixCOO<int, int> matrix(4, 4, 3);

	status = matrix.setElement(3, 3, 4);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	int * colIndexes;
	int  nColIndexes;

	status = matrix.getRowColumnIndexes(8, &colIndexes, &nColIndexes);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_MATRIX_ROW_OOB);
}


// === getRowNNZValues  ===
// Test 1: Get correct values for a row
BOOST_AUTO_TEST_CASE(getRowNNZValues_test1)
{
	cupcfd::error::eCodes status;

	SparseMatrixCOO<int, int> matrix(4, 4, 0);

	status = matrix.setElement(0, 3, 3);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	status = matrix.setElement(0, 0, 2);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	status = matrix.setElement(3, 1, 4);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	status = matrix.setElement(3, 0, 20);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	status = matrix.setElement(1, 1, 2);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	int * nnzValues;
	int  nNNZValues;

	status = matrix.getRowNNZValues(0, &nnzValues, &nNNZValues);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	BOOST_CHECK_EQUAL(nNNZValues, 2);
	BOOST_CHECK_EQUAL(nnzValues[0], 2);
	BOOST_CHECK_EQUAL(nnzValues[1], 3);
	free(nnzValues);

	status = matrix.getRowNNZValues(1, &nnzValues, &nNNZValues);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	BOOST_CHECK_EQUAL(nNNZValues, 1);
	BOOST_CHECK_EQUAL(nnzValues[0], 2);
	free(nnzValues);

	status = matrix.getRowNNZValues(3, &nnzValues, &nNNZValues);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	BOOST_CHECK_EQUAL(nNNZValues, 2);
	BOOST_CHECK_EQUAL(nnzValues[0], 20);
	BOOST_CHECK_EQUAL(nnzValues[1], 4);
	free(nnzValues);
}


// Test 2: Check for error if row index is lower than matrix index range
BOOST_AUTO_TEST_CASE(getRowNNZValues_test2)
{
	cupcfd::error::eCodes status;

	SparseMatrixCOO<int, int> matrix(4, 4, 3);

	status = matrix.setElement(3, 3, 4);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	int * nnzValues;
	int  nNNZValues;

	status = matrix.getRowNNZValues(2, &nnzValues, &nNNZValues);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_MATRIX_ROW_OOB);
}

// Test 3: Check for error if row index is higher than matrix index range
BOOST_AUTO_TEST_CASE(getRowNNZValues_test3)
{
	cupcfd::error::eCodes status;

	SparseMatrixCOO<int, int> matrix(4, 4, 3);

	status = matrix.setElement(3, 3, 4);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	int * nnzValues;
	int  nNNZValues;

	status = matrix.getRowNNZValues(8, &nnzValues, &nNNZValues);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_MATRIX_ROW_OOB);
}
