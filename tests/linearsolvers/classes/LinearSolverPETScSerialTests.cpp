/*
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Serial Unit Tests for the LinearSolverPETSc Class
 */

#define BOOST_TEST_MODULE LinearSolverPETScSerial
#include <boost/test/unit_test.hpp>
#include <boost/test/output_test_stream.hpp>
#include <stdexcept>

#include "LinearSolverPETScSerial.h"
#include "Error.h"
#include "SparseMatrixCOO.h"
#include <iostream>

using namespace cupcfd::linearsolvers;

// Setup
BOOST_AUTO_TEST_CASE(setup)
{
    int argc = boost::unit_test::framework::master_test_suite().argc;
    char ** argv = boost::unit_test::framework::master_test_suite().argv;
	PetscInitialize(&argc, &argv, NULL, NULL);
}

// ============== Constructors ===================
// Test 1: Check values are probably defaulted to their null states
BOOST_AUTO_TEST_CASE(constructor_test1)
{
	LinearSolverPETScSerial<double> solver;

	// Workaround - seems OK to compare these in C++,
	// but BOOST doesn't like making the comparison
	if(solver.a != PETSC_NULL)
	{
		BOOST_CHECK_EQUAL(true, false);
	}

	// Workaround - seems OK to compare these in C++,
	// but BOOST doesn't like making the comparison
	if(solver.x != PETSC_NULL)
	{
		BOOST_CHECK_EQUAL(true, false);
	}

	// Workaround - seems OK to compare these in C++,
	// but BOOST doesn't like making the comparison
	if(solver.b != PETSC_NULL)
	{
		BOOST_CHECK_EQUAL(true, false);
	}

	BOOST_CHECK_EQUAL(solver.mLocal, 0);
	BOOST_CHECK_EQUAL(solver.nLocal, 0);
	BOOST_CHECK_EQUAL(solver.mGlobal, 0);
	BOOST_CHECK_EQUAL(solver.nGlobal, 0);

	BOOST_CHECK_EQUAL(solver.xRanges, static_cast<decltype(solver.xRanges)>(nullptr));
	BOOST_CHECK_EQUAL(solver.bRanges, static_cast<decltype(solver.bRanges)>(nullptr));
	BOOST_CHECK_EQUAL(solver.aRanges, static_cast<decltype(solver.aRanges)>(nullptr));
}

// ============== setupVectorX ===================
// Test 1: Check Vector is created if a suitable row size is set
BOOST_AUTO_TEST_CASE(setupVectorX_test1)
{
	cupcfd::error::eCodes status;

	LinearSolverPETScSerial<double> solver;

	// Manually set row size so it is valid
	solver.mLocal = 20;
	solver.mGlobal = 20;

	// Test and Check
	status = solver.setupVectorX();

	// Workaround - seems OK to compare these in C++,
	// but BOOST doesn't like making the comparison
	if(solver.x == PETSC_NULL)
	{
		BOOST_CHECK_EQUAL(true, false);
	}

	PetscInt cmp[2] = {0, 20};
	BOOST_CHECK_EQUAL_COLLECTIONS(cmp, cmp + 2, solver.xRanges, solver.xRanges + 2);

	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
}

// Test 2: Check error if a suitable row size is not set - e.g. 0
BOOST_AUTO_TEST_CASE(setupVectorX_test2)
{
	cupcfd::error::eCodes status;

	LinearSolverPETScSerial<double> solver;

	// Manually set row size so it is valid
	solver.mLocal = 0;
	solver.mGlobal = 0;

	// Test and Check
	status = solver.setupVectorX();

	BOOST_CHECK_EQUAL(status, cupcfd::error::E_LINEARSOLVER_ROW_SIZE_UNSET);
}

// Test 3: Check error if the communicator is not set to PETSC_COMM_SELF
// ToDo
BOOST_AUTO_TEST_CASE(setupVectorX_test3)
{

}

// ============== resetVectorX ===================
// Test 1: Test the successful reset of a existing vector
BOOST_AUTO_TEST_CASE(resetVectorX_test1)
{
	cupcfd::error::eCodes status;

	LinearSolverPETScSerial<double> solver;

	//Setup

	// Manually set row size so it is valid
	solver.mLocal = 20;
	solver.mGlobal = 20;

	// Create the vector
	status = solver.setupVectorX();
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	// Test and Check
	status = solver.resetVectorX();
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	if(solver.x != PETSC_NULL)
	{
		BOOST_CHECK_EQUAL(true, false);
	}

	BOOST_CHECK_EQUAL(solver.xRanges, static_cast<decltype(solver.xRanges)>(nullptr));
}

// Test 2: Test the successful reset of a non-existing vector (i.e. no
// change, but no error either).
BOOST_AUTO_TEST_CASE(resetVectorX_test2)
{
	cupcfd::error::eCodes status;

	LinearSolverPETScSerial<double> solver;

	// === Setup ===

	// Check it is unset for the test
	if(solver.x != PETSC_NULL)
	{
		BOOST_CHECK_EQUAL(true, false);
	}

	// === Test and Check ===
	status = solver.resetVectorX();
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	if(solver.x != PETSC_NULL)
	{
		BOOST_CHECK_EQUAL(true, false);
	}
}

// ============== setupVectorB ===================
// Test 1: Check Vector is created if a suitable row size is set
BOOST_AUTO_TEST_CASE(setupVectorB_test1)
{
	cupcfd::error::eCodes status;

	LinearSolverPETScSerial<double> solver;

	// Manually set row size so it is valid
	solver.mLocal = 20;
	solver.mGlobal = 20;

	// Test and Check
	status = solver.setupVectorB();

	// Workaround - seems OK to compare these in C++,
	// but BOOST doesn't like making the comparison
	if(solver.b == PETSC_NULL)
	{
		BOOST_CHECK_EQUAL(true, false);
	}

	BOOST_CHECK_EQUAL(solver.bRanges, static_cast<decltype(solver.bRanges)>(nullptr));

	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
}

// Test 2: Check error if a suitable row size is not set - e.g. 0
BOOST_AUTO_TEST_CASE(setupVectorB_test2)
{
	cupcfd::error::eCodes status;

	LinearSolverPETScSerial<double> solver;

	// Manually set row size so it is valid
	solver.mLocal = 0;
	solver.mGlobal = 0;

	// Test and Check
	status = solver.setupVectorB();

	BOOST_CHECK_EQUAL(status, cupcfd::error::E_LINEARSOLVER_ROW_SIZE_UNSET);
}

// Test 3: Check error if the correct communicator is not set to PETSC_COMM_SELF
// ToDo
BOOST_AUTO_TEST_CASE(setupVectorB_test3)
{

}


// ============== resetVectorB ===================
// Test 1: Test the successful reset of a existing vector
BOOST_AUTO_TEST_CASE(resetVectorB_test1)
{
	cupcfd::error::eCodes status;

	LinearSolverPETScSerial<double> solver;

	//Setup

	// Manually set row size so it is valid
	solver.mLocal = 20;
	solver.mGlobal = 20;

	// Create the vector
	status = solver.setupVectorB();
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	// Test and Check
	status = solver.resetVectorB();
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	if(solver.b != PETSC_NULL)
	{
		BOOST_CHECK_EQUAL(true, false);
	}

	BOOST_CHECK_EQUAL(solver.bRanges, static_cast<decltype(solver.bRanges)>(nullptr));
}

// Test 2: Test the successful reset of a non-existing vector (i.e. no
// change, but no error either).
BOOST_AUTO_TEST_CASE(resetVectorB_test2)
{
	cupcfd::error::eCodes status;

	LinearSolverPETScSerial<double> solver;

	// === Setup ===

	// Check it is unset for the test
	if(solver.b != PETSC_NULL)
	{
		BOOST_CHECK_EQUAL(true, false);
	}

	// === Test and Check ===
	status = solver.resetVectorB();
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	if(solver.b != PETSC_NULL)
	{
		BOOST_CHECK_EQUAL(true, false);
	}
}

// ============== setupMatrixA ===================
// Test 1: Test setup of PETSc matrix from a SparseCOO matrix
BOOST_AUTO_TEST_CASE(setupMatrixA_test1)
{
	cupcfd::error::eCodes status;

	LinearSolverPETScSerial<double> solver;

	// === Setup ===
	// Create a simple SparseMatrix
	cupcfd::data_structures::SparseMatrixCOO<int, double> matrix(8, 8, 0);

	int rows[13] = {0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 6, 7};
	int cols[13] = {0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 7};
	double vals[13] = {0.1, 0.2, 0.1, 0.05, 0.07, 0.09, 0.06, 0.1, 0.15, 0.23, 0.11, 0.13, 0.09};

	for(int i = 0; i < 13; i++)
	{
		matrix.setElement(rows[i], cols[i], vals[i]);
	}

	// Set the row and col sizes
	solver.mLocal = 8;
	solver.nLocal = 8;
	solver.mGlobal = 8;
	solver.nGlobal = 8;

	// Test and Check
	status = solver.setupMatrixA(matrix);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	PetscInt cmp[2] = {0, 8};
	BOOST_CHECK_EQUAL_COLLECTIONS(cmp, cmp + 2, solver.aRanges, solver.aRanges + 2);

	// ToDo: Ideally would like to check PETSc internal nnz structure of matrix
}

// ============== resetMatrixA ===================
//Test 1: Check the matrix is correctly reset after being setup
BOOST_AUTO_TEST_CASE(resetMatrixA_test1)
{
	cupcfd::error::eCodes status;

	LinearSolverPETScSerial<double> solver;

	// === Setup ===
	// Create a simple SparseMatrix
	cupcfd::data_structures::matrices::SparseMatrixCOO<int, double> matrix(8, 8, 0);

	int rows[13] {0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 6, 7};
	int cols[13] = {0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 7};
	double vals[13] = {0.1, 0.2, 0.1, 0.05, 0.07, 0.09, 0.06, 0.1, 0.15, 0.23, 0.11, 0.13, 0.09};

	for(int i = 0; i < 13; i++)
	{
		matrix.setElement(rows[i], cols[i], vals[i]);
	}

	// Set the row and col sizes
	solver.mLocal = 8;
	solver.nLocal = 8;
	solver.mGlobal = 8;
	solver.nGlobal = 8;

	// Setup Matrix
	status = solver.setupMatrixA(matrix);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	// Test and Check Reset
	status = solver.resetMatrixA();
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	// Check range array has been reset
	BOOST_CHECK_EQUAL(solver.aRanges, static_cast<decltype(solver.aRanges)>(nullptr));
}

// Test 2: Check the matrix reset function does not error when
// no matrix is yet setup
BOOST_AUTO_TEST_CASE(resetMatrixA_test2)
{
	cupcfd::error::eCodes status;

	LinearSolverPETScSerial<double> solver;

	// Test and Check Reset
	status = solver.resetMatrixA();
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
}

// ToDo
// Test 3: There should be an error code for PETSc errors
BOOST_AUTO_TEST_CASE(resetMatrixA_test3)
{

}

// ============== setup ===========
// Test 1: Test the successful setup of the object from a matrix
BOOST_AUTO_TEST_CASE(setup_test1)
{
	cupcfd::error::eCodes status;

	LinearSolverPETScSerial<double> solver;

	// === Setup ===
	// Create a simple SparseMatrix
	cupcfd::data_structures::matrices::SparseMatrixCOO<int, double> matrix(8, 8, 0);

	int rows[13] = {0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 6, 7};
	int cols[13] = {0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 7};
	double vals[13] = {0.1, 0.2, 0.1, 0.05, 0.07, 0.09, 0.06, 0.1, 0.15, 0.23, 0.11, 0.13, 0.09};

	for(int i = 0; i < 13; i++)
	{
		matrix.setElement(rows[i], cols[i], vals[i]);
	}

	status = solver.setup(matrix);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
}

// ============== reset ===========
// Test 1: Test a successful reset after the object has been setup
BOOST_AUTO_TEST_CASE(reset_test1)
{
	cupcfd::error::eCodes status;

	LinearSolverPETScSerial<double> solver;

	// === Setup ===
	// Create a simple SparseMatrix
	cupcfd::data_structures::matrices::SparseMatrixCOO<int, double> matrix(8, 8, 0);

	int rows[13] = {0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 6, 7};
	int cols[13] = {0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 7};
	double vals[13] = {0.1, 0.2, 0.1, 0.05, 0.07, 0.09, 0.06, 0.1, 0.15, 0.23, 0.11, 0.13, 0.09};

	for(int i = 0; i < 13; i++)
	{
		matrix.setElement(rows[i], cols[i], vals[i]);
	}

	status = solver.setup(matrix);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	status = solver.reset();
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
}

// Test 2: Test a successful reset if the object is still in an unset state
BOOST_AUTO_TEST_CASE(reset_test2)
{
	cupcfd::error::eCodes status;

	LinearSolverPETScSerial<double> solver;

	status = solver.reset();
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
}

// ============== setValuesVectorX(T scalar) ===========
// Test 1: Test setting the entire vector to a specific scalar
// Ideally want to check value using getter, but will
// use PETSc function directly to keep function tests separate
BOOST_AUTO_TEST_CASE(setValuesVectorX_scalar_test1)
{
	cupcfd::error::eCodes status;

	LinearSolverPETScSerial<double> solver;

	// === Setup ===

	// Set the row and col sizes
	solver.mLocal = 8;
	solver.nLocal = 8;
	solver.mGlobal = 8;
	solver.nGlobal = 8;

	// Create Vector
	solver.setupVectorX();

	// Test and Check
	status = solver.setValuesVectorX(2.4);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	// Any position should hold the value 2.4
	PetscInt pos = 5;
	PetscScalar result;
	VecGetValues(solver.x, 1, &pos, &result);
	BOOST_CHECK_EQUAL(result, 2.4);

	// Second check in different position to ensure more than one was updated
	pos = 7;
	result = 0.0;
	VecGetValues(solver.x, 1, &pos, &result);
	BOOST_CHECK_EQUAL(result, 2.4);
}

// Test 2: Check for error when vector does not exist
BOOST_AUTO_TEST_CASE(setValuesVectorX_scalar_test2)
{
	cupcfd::error::eCodes status;

	LinearSolverPETScSerial<double> solver;

	// Test and Check
	status = solver.setValuesVectorX(2.4);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_LINEARSOLVER_INVALID_VECTOR);
}

// ============== setValuesVectorX(T * scalars, int nScalars) ===========
// Test 1: Test setting values at specific locations
BOOST_AUTO_TEST_CASE(setValuesVectorX_indexedscalars_test1)
{
	cupcfd::error::eCodes status;

	LinearSolverPETScSerial<double> solver;

	// === Setup ===

	// Set the row and col sizes
	solver.mLocal = 8;
	solver.nLocal = 8;
	solver.mGlobal = 8;
	solver.nGlobal = 8;

	// Create Vector
	solver.setupVectorX();

	// Test and Check
	int indices[3] = {0, 3, 7};
	double values[3] = {0.5, 0.7, 0.3};
	status = solver.setValuesVectorX(values, 3, indices, 3, 0);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	// Any position should hold the value 2.4
	PetscInt pos[8] = {0, 1, 2, 3, 4, 5, 6, 7};
	PetscScalar result[8];
	VecGetValues(solver.x, 8, pos, result);

	PetscScalar cmp[8] = {0.5, 0.0, 0.0, 0.7, 0.0, 0.0, 0.0, 0.3};
	BOOST_CHECK_EQUAL_COLLECTIONS(cmp, cmp + 8, result, result + 8);
}

// Test 2: Check Error for arrays of mismatched sizes
BOOST_AUTO_TEST_CASE(setValuesVectorX_indexedscalars_test2)
{
	cupcfd::error::eCodes status;

	LinearSolverPETScSerial<double> solver;

	// Set the row and col sizes
	solver.mLocal = 8;
	solver.nLocal = 8;
	solver.mGlobal = 8;
	solver.nGlobal = 8;

	// Create Vector
	solver.setupVectorX();

	// Test and Check
	int indices[3] = {0, 3, 7};
	double values[2] = {0.5, 0.7};
	status = solver.setValuesVectorX(values, 2, indices, 3, 0);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_ARRAY_MISMATCH_SIZE);
}

// Test 3: Check error for vector X not being setup
BOOST_AUTO_TEST_CASE(setValuesVectorX_indexedscalars_test3)
{
	cupcfd::error::eCodes status;

	LinearSolverPETScSerial<double> solver;

	// Set the row and col sizes
	solver.mLocal = 8;
	solver.nLocal = 8;
	solver.mGlobal = 8;
	solver.nGlobal = 8;

	// Test and Check
	int indices[3] = {0, 3, 7};
	double values[3] = {0.5, 0.7, 0.3};
	status = solver.setValuesVectorX(values, 3, indices, 3, 0);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_LINEARSOLVER_INVALID_VECTOR);
}

// Test 4: Check error for unset row size
BOOST_AUTO_TEST_CASE(setValuesVectorX_indexedscalars_test4)
{
	cupcfd::error::eCodes status;

	LinearSolverPETScSerial<double> solver;

	int indices[3] = {0, 3, 7};
	double values[3] = {0.5, 0.7, 0.3};
	status = solver.setValuesVectorX(values, 3, indices, 3, 0);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_LINEARSOLVER_ROW_SIZE_UNSET);
}

// Test 5: Check error for an index lower than minimum of global range
BOOST_AUTO_TEST_CASE(setValuesVectorX_indexedscalars_test5)
{
	cupcfd::error::eCodes status;

	LinearSolverPETScSerial<double> solver;

	// === Setup ===

	// Set the row and col sizes
	solver.mLocal = 8;
	solver.nLocal = 8;
	solver.mGlobal = 8;
	solver.nGlobal = 8;

	// Create Vector
	solver.setupVectorX();

	// Test and Check
	int indices[3] = {-1, 3, 7};
	double values[3] = {0.5, 0.7, 0.3};
	status = solver.setValuesVectorX(values, 3, indices, 3, 0);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_INVALID_INDEX);
}

// Test 6: Check error for an index higher than maximum of global range
BOOST_AUTO_TEST_CASE(setValuesVectorX_indexedscalars_test6)
{
	cupcfd::error::eCodes status;

	LinearSolverPETScSerial<double> solver;

	// === Setup ===

	// Set the row and col sizes
	solver.mLocal = 8;
	solver.nLocal = 8;
	solver.mGlobal = 8;
	solver.nGlobal = 8;

	// Create Vector
	solver.setupVectorX();

	// Test and Check
	int indices[3] = {0, 3, 8};
	double values[3] = {0.5, 0.7, 0.3};
	status = solver.setValuesVectorX(values, 3, indices, 3, 0);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_INVALID_INDEX);
}

// ============== setValuesVectorB(T scalar) ===========
// Test 1: Test setting the entire vector to a specific scalar
// Ideally want to check value using getter, but will
// use PETSc function directly to keep function tests separate
BOOST_AUTO_TEST_CASE(setValuesVectorB_scalar_test1)
{
	cupcfd::error::eCodes status;

	LinearSolverPETScSerial<double> solver;

	// === Setup ===

	// Set the row and col sizes
	solver.mLocal = 8;
	solver.nLocal = 8;
	solver.mGlobal = 8;
	solver.nGlobal = 8;

	// Create Vector
	solver.setupVectorB();

	// Test and Check
	status = solver.setValuesVectorB(2.4);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	// Any position should hold the value 2.4
	PetscInt pos = 5;
	PetscScalar result;
	VecGetValues(solver.b, 1, &pos, &result);
	BOOST_CHECK_EQUAL(result, 2.4);

	// Second check in different position to ensure more than one was updated
	pos = 7;
	result = 0.0;
	VecGetValues(solver.b, 1, &pos, &result);
	BOOST_CHECK_EQUAL(result, 2.4);
}

// Test 2: Check for error when vector does not exist
BOOST_AUTO_TEST_CASE(setValuesVectorB_scalar_test2)
{
	cupcfd::error::eCodes status;

	LinearSolverPETScSerial<double> solver;

	// Test and Check
	status = solver.setValuesVectorB(2.4);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_LINEARSOLVER_INVALID_VECTOR);
}

// ============== setValuesVectorB(T * scalars, int nScalars) ===========
// Test 1: Test setting values at specific locations
BOOST_AUTO_TEST_CASE(setValuesVectorB_indexedscalars_test1)
{
	cupcfd::error::eCodes status;

	LinearSolverPETScSerial<double> solver;

	// === Setup ===

	// Set the row and col sizes
	solver.mLocal = 8;
	solver.nLocal = 8;
	solver.mGlobal = 8;
	solver.nGlobal = 8;

	// Create Vector
	solver.setupVectorB();

	// Test and Check
	int indices[3] = {0, 3, 7};
	double values[3] = {0.5, 0.7, 0.3};
	status = solver.setValuesVectorB(values, 3, indices, 3, 0);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	// Any position should hold the value 2.4
	PetscInt pos[8] = {0, 1, 2, 3, 4, 5, 6, 7};
	PetscScalar result[8];
	VecGetValues(solver.b, 8, pos, result);

	PetscScalar cmp[8] = {0.5, 0.0, 0.0, 0.7, 0.0, 0.0, 0.0, 0.3};
	BOOST_CHECK_EQUAL_COLLECTIONS(cmp, cmp + 8, result, result + 8);
}

// Test 2: Check Error for arrays of mismatched sizes
BOOST_AUTO_TEST_CASE(setValuesVectorB_indexedscalars_test2)
{
	cupcfd::error::eCodes status;

	LinearSolverPETScSerial<double> solver;

	// Set the row and col sizes
	solver.mLocal = 8;
	solver.nLocal = 8;
	solver.mGlobal = 8;
	solver.nGlobal = 8;

	// Create Vector
	solver.setupVectorB();

	// Test and Check
	int indices[3] = {0, 3, 7};
	double values[2] = {0.5, 0.7};
	status = solver.setValuesVectorB(values, 2, indices, 3, 0);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_ARRAY_MISMATCH_SIZE);
}

// Test 3: Check error for vector B not being setup
BOOST_AUTO_TEST_CASE(setValuesVectorB_indexedscalars_test3)
{
	cupcfd::error::eCodes status;

	LinearSolverPETScSerial<double> solver;

	// Set the row and col sizes
	solver.mLocal = 8;
	solver.nLocal = 8;
	solver.mGlobal = 8;
	solver.nGlobal = 8;

	// Test and Check
	int indices[3] = {0, 3, 7};
	double values[3] = {0.5, 0.7, 0.3};
	status = solver.setValuesVectorB(values, 3, indices, 3, 0);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_LINEARSOLVER_INVALID_VECTOR);
}

// Test 4: Check error for unset row size
BOOST_AUTO_TEST_CASE(setValuesVectorB_indexedscalars_test4)
{
	cupcfd::error::eCodes status;

	LinearSolverPETScSerial<double> solver;

	int indices[3] = {0, 3, 7};
	double values[3] = {0.5, 0.7, 0.3};
	status = solver.setValuesVectorB(values, 3, indices, 3, 0);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_LINEARSOLVER_ROW_SIZE_UNSET);
}

// Test 5: Check error for an index lower than minimum of global range
BOOST_AUTO_TEST_CASE(setValuesVectorB_indexedscalars_test5)
{
	cupcfd::error::eCodes status;

	LinearSolverPETScSerial<double> solver;

	// === Setup ===

	// Set the row and col sizes
	solver.mLocal = 8;
	solver.nLocal = 8;
	solver.mGlobal = 8;
	solver.nGlobal = 8;

	// Create Vector
	solver.setupVectorB();

	// Test and Check
	int indices[3] = {-1, 3, 7};
	double values[3] = {0.5, 0.7, 0.3};
	status = solver.setValuesVectorB(values, 3, indices, 3, 0);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_INVALID_INDEX);
}

// Test 6: Check error for an index higher than maximum of global range
BOOST_AUTO_TEST_CASE(setValuesVectorB_indexedscalars_test6)
{
	cupcfd::error::eCodes status;
	LinearSolverPETScSerial<double> solver;

	// === Setup ===

	// Set the row and col sizes
	solver.mLocal = 8;
	solver.nLocal = 8;
	solver.mGlobal = 8;
	solver.nGlobal = 8;

	// Create Vector
	solver.setupVectorB();

	// Test and Check
	int indices[3] = {0, 3, 8};
	double values[3] = {0.5, 0.7, 0.3};
	status = solver.setValuesVectorB(values, 3, indices, 3, 0);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_INVALID_INDEX);
}

// ============== setValuesMatrixA ===========
// Test 1: Successfully set the matrix values from a Sparse Matrix
BOOST_AUTO_TEST_CASE(setValuesMatrixA_test1)
{
	cupcfd::error::eCodes status;
	LinearSolverPETScSerial<double> solver;

	// === Setup ===
	// Create a simple SparseMatrix
	cupcfd::data_structures::matrices::SparseMatrixCOO<int, double> matrix(8, 8, 0);

	int rows[13] = {0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 6, 7};
	int cols[13] = {0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 7};
	double vals[13] = {0.1, 0.2, 0.1, 0.05, 0.07, 0.09, 0.06, 0.1, 0.15, 0.23, 0.11, 0.13, 0.09};

	for(int i = 0; i < 13; i++)
	{
		matrix.setElement(rows[i], cols[i], vals[i]);
	}

	// Set the row and col sizes
	solver.mLocal = 8;
	solver.nLocal = 8;
	solver.mGlobal = 8;
	solver.nGlobal = 8;

	// Setup the Matrix
	status = solver.setupMatrixA(matrix);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	// Test and Check
	status = solver.setValuesMatrixA(matrix);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	// Check the values are correct - use PETSc function directly since we haven't tested
	// the get function yet
	PetscInt rowResult = 3;
	PetscInt colResult[2] = {3, 4};
	PetscScalar valResult[2];

	MatGetValues(solver.a, 1, &rowResult, 2, colResult, valResult);
	BOOST_CHECK_EQUAL(valResult[0], 0.06);
	BOOST_CHECK_EQUAL(valResult[1], 0.1);
}

// Test 2: Error: Check the row sizes are set
BOOST_AUTO_TEST_CASE(setValuesMatrixA_test2)
{
	cupcfd::error::eCodes status;
	LinearSolverPETScSerial<double> solver;

	// === Setup ===
	// Create a simple SparseMatrix
	cupcfd::data_structures::matrices::SparseMatrixCOO<int, double> matrix(8, 8, 0);

	int rows[13] = {0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 6, 7};
	int cols[13] = {0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 7};
	double vals[13] = {0.1, 0.2, 0.1, 0.05, 0.07, 0.09, 0.06, 0.1, 0.15, 0.23, 0.11, 0.13, 0.09};

	for(int i = 0; i < 13; i++)
	{
		matrix.setElement(rows[i], cols[i], vals[i]);
	}

	// Set the row and col sizes
	solver.mLocal = 8;
	solver.mGlobal = 8;
	solver.nLocal = 8;
	solver.nGlobal = 8;

	// Setup the Matrix
	status = solver.setupMatrixA(matrix);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	// Test and Check: Don't use solver setup/set the solver matrix row sizes
	solver.mLocal = 0;
	solver.mGlobal = 0;
	status = solver.setValuesMatrixA(matrix);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_LINEARSOLVER_ROW_SIZE_UNSET);
}

// Test 3: Error: Check the column sizes are set
BOOST_AUTO_TEST_CASE(setValuesMatrixA_test3)
{
	cupcfd::error::eCodes status;
	LinearSolverPETScSerial<double> solver;

	// === Setup ===
	// Create a simple SparseMatrix
	cupcfd::data_structures::matrices::SparseMatrixCOO<int, double> matrix(8, 8, 0);

	int rows[13] = {0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 6, 7};
	int cols[13] = {0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 7};
	double vals[13] = {0.1, 0.2, 0.1, 0.05, 0.07, 0.09, 0.06, 0.1, 0.15, 0.23, 0.11, 0.13, 0.09};

	for(int i = 0; i < 13; i++)
	{
		matrix.setElement(rows[i], cols[i], vals[i]);
	}

	// Set the row and col sizes
	solver.mLocal = 8;
	solver.mGlobal = 8;
	solver.nLocal = 8;
	solver.nGlobal = 8;

	// Setup the Matrix
	status = solver.setupMatrixA(matrix);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	// Test and Check: Don't use solver setup/set the solver matrix col sizes
	solver.nLocal = 0;
	solver.nGlobal = 0;
	status = solver.setValuesMatrixA(matrix);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_LINEARSOLVER_COL_SIZE_UNSET);
}

// Test 4: Error: Check the Matrix has been setup
BOOST_AUTO_TEST_CASE(setValuesMatrixA_test4)
{
	cupcfd::error::eCodes status;
	LinearSolverPETScSerial<double> solver;

	// === Setup ===
	// Create a simple SparseMatrix
	cupcfd::data_structures::matrices::SparseMatrixCOO<int, double> matrix(8, 8, 0);

	int rows[13] = {0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 6, 7};
	int cols[13] = {0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 7};
	double vals[13] = {0.1, 0.2, 0.1, 0.05, 0.07, 0.09, 0.06, 0.1, 0.15, 0.23, 0.11, 0.13, 0.09};

	for(int i = 0; i < 13; i++)
	{
		matrix.setElement(rows[i], cols[i], vals[i]);
	}

	// Set the row and col sizes
	solver.mLocal = 8;
	solver.nLocal = 8;
	solver.mGlobal = 8;
	solver.nGlobal = 8;

	// Test and Check: Matrix has not been setup
	status = solver.setValuesMatrixA(matrix);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_LINEARSOLVER_INVALID_MATRIX);
}

// ============== getValuesVectorX ===========
// Test 1: Check all values are the same when
// set to all same value.
BOOST_AUTO_TEST_CASE(getValuesVectorX_test1)
{
	cupcfd::error::eCodes status;

	LinearSolverPETScSerial<double> solver;

	// === Setup ===

	// Set the row and col sizes, else we will get error
	solver.mLocal = 8;
	solver.nLocal = 8;
	solver.mGlobal = 8;
	solver.nGlobal = 8;

	// Create Vector
	solver.setupVectorX();

	// Set the values
	status = solver.setValuesVectorX(2.4);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	// Test and Check
	double * result;
	int nResult;
	status = solver.getValuesVectorX(&result, &nResult);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	double cmp[8] = {2.4, 2.4, 2.4, 2.4, 2.4, 2.4, 2.4, 2.4};
	BOOST_CHECK_EQUAL_COLLECTIONS(result, result + 8, cmp, cmp + 8);

	free(result);
}

// Test 2: Check all values are different when
// set to different values
BOOST_AUTO_TEST_CASE(getValuesVectorX_test2)
{

}

// Test 3: Check error when row size unset
BOOST_AUTO_TEST_CASE(getValuesVectorX_test3)
{
	cupcfd::error::eCodes status;

	LinearSolverPETScSerial<double> solver;

	double * result;
	int nResult;
	status = solver.getValuesVectorX(&result, &nResult);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_LINEARSOLVER_ROW_SIZE_UNSET);

	// Since error, nothing to free
}

// Test 4: Check error when vector not set
BOOST_AUTO_TEST_CASE(getValuesVectorX_test4)
{
	cupcfd::error::eCodes status;

	LinearSolverPETScSerial<double> solver;

	solver.mLocal = 8;
	solver.nLocal = 8;
	solver.mGlobal = 8;
	solver.nGlobal = 8;

	double * result;
	int nResult;
	status = solver.getValuesVectorX(&result, &nResult);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_LINEARSOLVER_INVALID_VECTOR);

	// Since error, nothing to free
}

// ============== getValuesVectorXIndexes ===========
// Test 1: Retrieve specific indexes
BOOST_AUTO_TEST_CASE(getValuesVectorX_indexes_test1)
{
	cupcfd::error::eCodes status;

	LinearSolverPETScSerial<double> solver;

	// === Setup ===

	// Set the row and col sizes
	solver.mLocal = 8;
	solver.nLocal = 8;
	solver.mGlobal = 8;
	solver.nGlobal = 8;

	// Create Vector
	solver.setupVectorX();

	// Setup
	int indices[3] = {0, 3, 7};
	double values[3] = {0.5, 0.7, 0.3};
	status = solver.setValuesVectorX(values, 3, indices, 3, 0);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	// Test and Check
	int getIndices[4] = {7, 4, 0, 6};
	double * result;
	int nResult;
	status = solver.getValuesVectorX(&result, &nResult, getIndices, 4, 0);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	PetscScalar cmp[4] = {0.3, 0.0, 0.5, 0.0};
	BOOST_CHECK_EQUAL_COLLECTIONS(cmp, cmp + 4, result, result + 4);

	free(result);
}

// Test 2: Error Check for unset global row size
BOOST_AUTO_TEST_CASE(getValuesVectorX_indexes_test2)
{
	cupcfd::error::eCodes status;

	LinearSolverPETScSerial<double> solver;

	// === Setup ===

	// Create Vector
	solver.setupVectorX();

	// Test and Check
	int getIndices[4] = {7, 4, 0, 6};
	double * result;
	int nResult;
	status = solver.getValuesVectorX(&result, &nResult, getIndices, 4, 0);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_LINEARSOLVER_ROW_SIZE_UNSET);
}

// Test 3: Error Check for unset vector
BOOST_AUTO_TEST_CASE(getValuesVectorX_indexes_test3)
{
	cupcfd::error::eCodes status;

	LinearSolverPETScSerial<double> solver;

	// === Setup ===

	// Set the row and col sizes
	solver.mLocal = 8;
	solver.nLocal = 8;
	solver.mGlobal = 8;
	solver.nGlobal = 8;

	// Test and Check
	int getIndices[4] = {7, 4, 0, 6};
	double * result;
	int nResult;
	status = solver.getValuesVectorX(&result, &nResult, getIndices, 4, 0);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_LINEARSOLVER_INVALID_VECTOR);
}

// Test 4: Error Check for invalid lower bound index
BOOST_AUTO_TEST_CASE(getValuesVectorX_indexes_test4)
{
	cupcfd::error::eCodes status;

	LinearSolverPETScSerial<double> solver;

	// === Setup ===

	// Set the row and col sizes
	solver.mLocal = 8;
	solver.nLocal = 8;
	solver.mGlobal = 8;
	solver.nGlobal = 8;

	// Create Vector
	solver.setupVectorX();

	// Setup
	int indices[3] = {0, 3, 7};
	double values[3] = {0.5, 0.7, 0.3};
	status = solver.setValuesVectorX(values, 3, indices, 3, 0);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	// Test and Check
	int getIndices[4] = {-1, 4, 0, 6};
	double * result;
	int nResult;
	status = solver.getValuesVectorX(&result, &nResult, getIndices, 4, 0);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_INVALID_INDEX);
}

// Test 5: Error Check for invalid upper bound index
BOOST_AUTO_TEST_CASE(getValuesVectorX_indexes_test5)
{
	cupcfd::error::eCodes status;

	LinearSolverPETScSerial<double> solver;

	// === Setup ===

	// Set the row and col sizes
	solver.mLocal = 8;
	solver.nLocal = 8;
	solver.mGlobal = 8;
	solver.nGlobal = 8;

	// Create Vector
	solver.setupVectorX();

	// Setup
	int indices[3] = {0, 3, 7};
	double values[3] = {0.5, 0.7, 0.3};
	status = solver.setValuesVectorX(values, 3, indices, 3, 0);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	// Test and Check
	int getIndices[4] = {0, 4, 0, 8};
	double * result;
	int nResult;
	status = solver.getValuesVectorX(&result, &nResult, getIndices, 4, 0);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_INVALID_INDEX);
}

// ============== getValuesVectorB ===========
// Test 1: Check all values are the same when
// set to all same value.
BOOST_AUTO_TEST_CASE(getValuesVectorB_test1)
{
	cupcfd::error::eCodes status;

	LinearSolverPETScSerial<double> solver;

	// === Setup ===

	// Set the row and col sizes, else we will get error
	solver.mLocal = 8;
	solver.nLocal = 8;
	solver.mGlobal = 8;
	solver.nGlobal = 8;

	// Create Vector
	solver.setupVectorB();

	// Set the values
	status = solver.setValuesVectorB(2.4);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	// Test and Check
	double * result;
	int nResult;
	status = solver.getValuesVectorB(&result, &nResult);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	double cmp[8] = {2.4, 2.4, 2.4, 2.4, 2.4, 2.4, 2.4, 2.4};
	BOOST_CHECK_EQUAL_COLLECTIONS(result, result + 8, cmp, cmp + 8);

	free(result);
}

// Test 2: Check all values are different when
// set to all same value.
BOOST_AUTO_TEST_CASE(getValuesVectorB_test2)
{

}

// Test 3: Check error when row size unset
BOOST_AUTO_TEST_CASE(getValuesVectorB_test3)
{
	cupcfd::error::eCodes status;

	LinearSolverPETScSerial<double> solver;

	double * result;
	int nResult;
	status = solver.getValuesVectorB(&result, &nResult);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_LINEARSOLVER_ROW_SIZE_UNSET);

	// Since error, nothing to free
}

// Test 4: Check error when vector not set
BOOST_AUTO_TEST_CASE(getValuesVectorB_test4)
{
	cupcfd::error::eCodes status;

	LinearSolverPETScSerial<double> solver;

	solver.mLocal = 8;
	solver.nLocal = 8;
	solver.mGlobal = 8;
	solver.nGlobal = 8;

	double * result;
	int nResult;
	status = solver.getValuesVectorB(&result, &nResult);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_LINEARSOLVER_INVALID_VECTOR);

	// Since error, nothing to free
}



// ============== getValuesVectorBIndexes ===========
// Test 1: Retrieve specific indexes
BOOST_AUTO_TEST_CASE(getValuesVectorB_indexes_test1)
{
	cupcfd::error::eCodes status;

	LinearSolverPETScSerial<double> solver;

	// === Setup ===

	// Set the row and col sizes
	solver.mLocal = 8;
	solver.nLocal = 8;
	solver.mGlobal = 8;
	solver.nGlobal = 8;

	// Create Vector
	solver.setupVectorB();

	// Setup
	int indices[3] = {0, 3, 7};
	double values[3] = {0.5, 0.7, 0.3};
	status = solver.setValuesVectorB(values, 3, indices, 3, 0);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	// Test and Check
	int getIndices[4] = {7, 4, 0, 6};
	double * result;
	int nResult;
	status = solver.getValuesVectorB(&result, &nResult, getIndices, 4, 0);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	PetscScalar cmp[4] = {0.3, 0.0, 0.5, 0.0};
	BOOST_CHECK_EQUAL_COLLECTIONS(cmp, cmp + 4, result, result + 4);

	free(result);
}

// Test 2: Error Check for unset global row size
BOOST_AUTO_TEST_CASE(getValuesVectorB_indexes_test2)
{
	cupcfd::error::eCodes status;

	LinearSolverPETScSerial<double> solver;

	// === Setup ===

	// Create Vector
	solver.setupVectorB();

	// Test and Check
	int getIndices[4] = {7, 4, 0, 6};
	double * result;
	int nResult;
	status = solver.getValuesVectorB(&result, &nResult, getIndices, 4, 0);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_LINEARSOLVER_ROW_SIZE_UNSET);
}

// Test 3: Error Check for unset vector
BOOST_AUTO_TEST_CASE(getValuesVectorB_indexes_test3)
{
	cupcfd::error::eCodes status;

	LinearSolverPETScSerial<double> solver;

	// === Setup ===

	// Set the row and col sizes
	solver.mLocal = 8;
	solver.nLocal = 8;
	solver.mGlobal = 8;
	solver.nGlobal = 8;

	// Test and Check
	int getIndices[4] = {7, 4, 0, 6};
	double * result;
	int nResult;
	status = solver.getValuesVectorB(&result, &nResult, getIndices, 4, 0);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_LINEARSOLVER_INVALID_VECTOR);
}

// Test 4: Error Check for invalid lower bound index
BOOST_AUTO_TEST_CASE(getValuesVectorB_indexes_test4)
{
	cupcfd::error::eCodes status;

	LinearSolverPETScSerial<double> solver;

	// === Setup ===

	// Set the row and col sizes
	solver.mLocal = 8;
	solver.nLocal = 8;
	solver.mGlobal = 8;
	solver.nGlobal = 8;

	// Create Vector
	solver.setupVectorB();

	// Setup
	int indices[3] = {0, 3, 7};
	double values[3] = {0.5, 0.7, 0.3};
	status = solver.setValuesVectorB(values, 3, indices, 3, 0);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	// Test and Check
	int getIndices[4] = {-1, 4, 0, 6};
	double * result;
	int nResult;
	status = solver.getValuesVectorB(&result, &nResult, getIndices, 4, 0);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_INVALID_INDEX);
}

// Test 5: Error Check for invalid upper bound index
BOOST_AUTO_TEST_CASE(getValuesVectorB_indexes_test5)
{
	cupcfd::error::eCodes status;

	LinearSolverPETScSerial<double> solver;

	// === Setup ===

	// Set the row and col sizes
	solver.mLocal = 8;
	solver.nLocal = 8;
	solver.mGlobal = 8;
	solver.nGlobal = 8;

	// Create Vector
	solver.setupVectorB();

	// Setup
	int indices[3] = {0, 3, 7};
	double values[3] = {0.5, 0.7, 0.3};
	status = solver.setValuesVectorB(values, 3, indices, 3, 0);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	// Test and Check
	int getIndices[4] = {0, 4, 0, 8};
	double * result;
	int nResult;
	status = solver.getValuesVectorB(&result, &nResult, getIndices, 4, 0);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_INVALID_INDEX);
}



// ============== getValuesMatrixA ===========
// Test 1: Check we can retrieve values correctly from the matrix
BOOST_AUTO_TEST_CASE(getValuesMatrixA_test1)
{
	cupcfd::error::eCodes status;
	LinearSolverPETScSerial<double> solver;

	// === Setup ===
	// Create a simple SparseMatrix
	cupcfd::data_structures::matrices::SparseMatrixCOO<int, double> matrix(8, 8, 0);

	int rows[13] =    {0,   0,   1,   1,    2,    2,    3,    3,   4,    4,    5,    6,    7};
	int cols[13] =    {0,   1,   1,   2,    2,    3,    3,    4,   4,    5,    5,    6,    7};
	double vals[13] = {0.1, 0.2, 0.1, 0.05, 0.07, 0.09, 0.06, 0.1, 0.15, 0.23, 0.11, 0.13, 0.09};

	for(int i = 0; i < 13; i++)
	{
		matrix.setElement(rows[i], cols[i], vals[i]);
	}

	cupcfd::data_structures::matrices::SparseMatrixCOO<int, double> resultMatrix(8, 8, 0);

	int resultRows[9] = {0, 1, 2, 2, 4, 4, 5, 6, 6};
	int resultCols[9] = {1, 1, 2, 3, 4, 5, 5, 6, 7};
	double resultVals[9] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};

	for(int i = 0; i < 9; i++)
	{
		resultMatrix.setElement(resultRows[i], resultCols[i], resultVals[i]);
	}

	// Set the row and col sizes
	solver.mLocal = 8;
	solver.nLocal = 8;
	solver.mGlobal = 8;
	solver.nGlobal = 8;

	// Setup the Matrix
	status = solver.setupMatrixA(matrix);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	// Set the values inside the matrix
	status = solver.setValuesMatrixA(matrix);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	// Test and Check
	// Retrieve values via the function and check they are what we expect
	status = solver.getValuesMatrixA(resultMatrix);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	// Check contents of SparseMatrixCOO nnz to verify it has been updated correctly
	double cmp[9] = {0.2, 0.1, 0.07, 0.09, 0.15, 0.23, 0.11, 0.13, 0.0};

	BOOST_CHECK_EQUAL_COLLECTIONS(cmp, cmp + 9, &resultMatrix.val[0], &resultMatrix.val[0] + 9);
}

// Not Done
// Test 2: Error Check: Check PETSc matrix is setup
BOOST_AUTO_TEST_CASE(getValuesMatrixA_test2)
{
	cupcfd::error::eCodes status;
	LinearSolverPETScSerial<double> solver;

	// === Setup ===
	// Create a simple SparseMatrix
	cupcfd::data_structures::matrices::SparseMatrixCOO<int, double> resultMatrix(8, 8, 0);

	int resultRows[9] = {0, 1, 2, 2, 4, 4, 5, 6, 6};
	int resultCols[9] = {1, 1, 2, 3, 4, 5, 5, 6, 7};
	double resultVals[9] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};

	for(int i = 0; i < 9; i++)
	{
		resultMatrix.setElement(resultRows[i], resultCols[i], resultVals[i]);
	}

	// Set the row and col sizes
	solver.mLocal = 8;
	solver.nLocal = 8;
	solver.mGlobal = 8;
	solver.nGlobal = 8;

	// Skip setting up the matrix

	// Test and Check
	// Should be an error since the matrix is not setup
	status = solver.getValuesMatrixA(resultMatrix);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_LINEARSOLVER_INVALID_MATRIX);
}

// Test 3: Error Check: Check that the maximum indices of the matrix object are within the bounds
// of the PETSc matrix object
BOOST_AUTO_TEST_CASE(getValuesMatrixA_test3)
{

}

// ============== clearVectorX ===========
// Test 1: Check value is set to 0
BOOST_AUTO_TEST_CASE(clearVectorX_test1)
{
	cupcfd::error::eCodes status;

	LinearSolverPETScSerial<double> solver;

	// === Setup ===

	// Set the row and col sizes, else we will get error
	solver.mLocal = 8;
	solver.nLocal = 8;
	solver.mGlobal = 8;
	solver.nGlobal = 8;

	// Create Vector
	solver.setupVectorX();

	// Set the values
	// Will need to use setValueVector, but this should have been
	// tested by this point
	status = solver.setValuesVectorX(2.4);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	// Test and Check
	status = solver.clearVectorX();
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	// Get value to check
	// Will need to use getValuesVectorX, but should be tested by this point
	double * result;
	int nResult;
	status = solver.getValuesVectorX(&result, &nResult);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	double cmp[8] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
	BOOST_CHECK_EQUAL_COLLECTIONS(result, result + 8, cmp, cmp + 8);

	free(result);
}

// Test 2: Check for error when vector X is not setup
BOOST_AUTO_TEST_CASE(clearVectorX_test2)
{
	cupcfd::error::eCodes status;

	LinearSolverPETScSerial<double> solver;

	// === Setup ===

	// Set the row and col sizes, else we will get error
	solver.mLocal = 8;
	solver.nLocal = 8;
	solver.mGlobal = 8;
	solver.nGlobal = 8;

	// === Test and Check ===
	status = solver.clearVectorX();
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_LINEARSOLVER_INVALID_VECTOR);
}

// ============== clearVectorB ===========
BOOST_AUTO_TEST_CASE(clearVectorB_test1)
{
	cupcfd::error::eCodes status;

	LinearSolverPETScSerial<double> solver;

	// === Setup ===

	// Set the row and col sizes, else we will get error
	solver.mLocal = 8;
	solver.nLocal = 8;
	solver.mGlobal = 8;
	solver.nGlobal = 8;

	// Create Vector
	solver.setupVectorB();

	// Set the values
	// Will need to use setValueVector, but this should have been
	// tested by this point
	status = solver.setValuesVectorB(2.4);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	// Test and Check
	status = solver.clearVectorB();
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	// Get value to check
	// Will need to use getValuesVectorB, but should be tested by this point
	double * result;
	int nResult;
	status = solver.getValuesVectorB(&result, &nResult);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	double cmp[8] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
	BOOST_CHECK_EQUAL_COLLECTIONS(result, result + 8, cmp, cmp + 8);

	free(result);
}

BOOST_AUTO_TEST_CASE(clearVectorB_test2)
{
	cupcfd::error::eCodes status;

	LinearSolverPETScSerial<double> solver;

	// === Setup ===

	// Set the row and col sizes, else we will get error
	solver.mLocal = 8;
	solver.nLocal = 8;
	solver.mGlobal = 8;
	solver.nGlobal = 8;

	// === Test and Check ===
	status = solver.clearVectorB();
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_LINEARSOLVER_INVALID_VECTOR);
}

// ============== clearMatrixA ===========
BOOST_AUTO_TEST_CASE(clearMatrixA_test1)
{

}

// ============== resetSolverSelection ===========
BOOST_AUTO_TEST_CASE(resetSolverSelection_test1)
{

}

// ============== setSolverSelection ===========
BOOST_AUTO_TEST_CASE(setSolverSelection_test1)
{

}

// ============== resetTolerances ===========
BOOST_AUTO_TEST_CASE(resetTolerances_test1)
{

}

// ============== resetPETScSolver ===========
// Test 1: Test that we can reset when the KSP solver is not setup
BOOST_AUTO_TEST_CASE(resetPETScSolver_test1)
{

}

// Test 2: Test that we can reset when the KSP Solver is setup
BOOST_AUTO_TEST_CASE(resetPETScSolver_test2)
{

}

// ============== setupPETScSolver ===========
// Test 1: Test that we can setup the CG/AMG solver
BOOST_AUTO_TEST_CASE(setupPETScSolver_test1)
{
	cupcfd::error::eCodes status;

	LinearSolverPETScSerial<double> solver;

	// === Setup ===
	// Create a simple SparseMatrix
	cupcfd::data_structures::matrices::SparseMatrixCOO<int, double> matrix(8, 8, 0);

	int rows[13] = {0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 6, 7};
	int cols[13] = {0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 7};
	double vals[13] = {0.1, 0.2, 0.1, 0.05, 0.07, 0.09, 0.06, 0.1, 0.15, 0.23, 0.11, 0.13, 0.09};

	for(int i = 0; i < 13; i++)
	{
		matrix.setElement(rows[i], cols[i], vals[i]);
	}

	// Set the row and col sizes
	solver.mLocal = 8;
	solver.nLocal = 8;
	solver.mGlobal = 8;
	solver.nGlobal = 8;

	// Test and Check
	status = solver.setupMatrixA(matrix);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	solver.petscSolverSelection = PETSC_SOLVER_CGAMG;

	// Test and Check
	status = solver.setupPETScSolver();
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
}


// ============== solveMatrixA ===========
BOOST_AUTO_TEST_CASE(solveMatrixA_test1)
{
	cupcfd::error::eCodes status;

	LinearSolverPETScSerial<double> solver;

	// === Setup ===
	// Create a simple SparseMatrix
	cupcfd::data_structures::matrices::SparseMatrixCOO<int, double> matrix(8, 8, 0);

	//int rows[21] = {0, 1, 2, 4, 5, 6, 1, 2, 3, 4, 5, 6, 7, 0, 1, 2, 3, 4, 5, 6, 7};
	//int cols[21] = {1, 2, 3, 5, 6, 7, 0, 1, 2, 3, 4, 5, 6, 0, 1, 2, 3, 4, 5, 6, 7};
	//double vals[21] = {0.1, 0.2, 0.1, 0.05, 0.07, 0.09, 0.06, 0.1, 0.15, 0.23, 0.11, 0.13, 0.09, 0.4, 0.2, 0.6, 0.8, 0.7, 0.4, 0.3, 0.2};
	int rows[8] = {0, 1, 2, 3, 4, 5, 6, 7};
	int cols[8] = {0, 1, 2, 3, 4, 5, 6, 7};
	double vals[8] = {0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8};

	for(int i = 0; i < 8; i++)
	{
		matrix.setElement(rows[i], cols[i], vals[i]);
	}

	// Setup the data structures of the solver
	status = solver.setup(matrix);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	// Set some suitable values for a very small test solve
	status = solver.setValuesMatrixA(matrix);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	// Set a suitable value for the B Vector
	//double tmp[8] = {0.23, 0.598, 0.46, 0.345, 0.644, 0.414, 0.506, 0.207};
	//int tmp2[8] = {0, 1, 2, 3, 4, 5, 6, 7};

	//status = solver.setValuesVectorB(tmp, 8, tmp2, 8, 0);
	status = solver.setValuesVectorB(0.1);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	// Clear the X Vector
	status = solver.clearVectorX();
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	// Set the solver type
	status = solver.setSolverSelection(PETSC_SOLVER_CGAMG);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	solver.setTolerances(0.0, 0.0);

	// Test and Check
	// Run the solve
	status = solver.solve();
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	// Get the contents of Vector X
	double * vecX;
	int nVecX;

	status = solver.getValuesVectorX(&vecX, &nVecX);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	double cmp[8] = {1, 0.5, 0.33333333333333337, 0.25, 0.2, 0.16666666666666669, 0.14285714285714288, 0.125};
	BOOST_CHECK_EQUAL_COLLECTIONS(cmp, cmp + 8, vecX, vecX + 8);
}

BOOST_AUTO_TEST_CASE(cleanup)
{
	PetscFinalize();
}
