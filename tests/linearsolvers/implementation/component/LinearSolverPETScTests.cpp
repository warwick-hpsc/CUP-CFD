/*
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Unit Tests for the LinearSolverPETSc class
 */

#define BOOST_TEST_MODULE LinearSolverPETSc
#include <boost/test/unit_test.hpp>
#include <boost/test/output_test_stream.hpp>
#include <stdexcept>

#include "Communicator.h"
#include "LinearSolverPETSc.h"
#include "Error.h"
#include "SparseMatrixCOO.h"

// ========================================
// ============== Tests ===================
// ========================================

namespace utf = boost::unit_test;
using namespace cupcfd::linearsolvers;

// Setup
BOOST_AUTO_TEST_CASE(setup)
{
    int argc = boost::unit_test::framework::master_test_suite().argc;
    char ** argv = boost::unit_test::framework::master_test_suite().argv;

    MPI_Init(&argc, &argv);
	PetscInitialize(&argc, &argv, NULL, NULL);
}

// === Constructors ===
// Test 1: Create a Serial PETSc Linear Solver
BOOST_AUTO_TEST_CASE(constructor_test1)
{
	// This default to MPI_COMM_SELF
	cupcfd::comm::Communicator comm;

	// Create a simple SparseMatrix
	cupcfd::data_structures::SparseMatrixCOO<int, double> matrix(8, 8, 0);

	int rows[13] = {0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 6, 7};
	int cols[13] = {0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 7};
	double vals[13] = {0.1, 0.2, 0.1, 0.05, 0.07, 0.09, 0.06, 0.1, 0.15, 0.23, 0.11, 0.13, 0.09};

	for(int i = 0; i < 13; i++)
	{
		matrix.setElement(rows[i], cols[i], vals[i]);
	}

	LinearSolverPETSc<cupcfd::data_structures::SparseMatrixCOO<int, double>, int, double> solver(comm, PETSC_KSP_CGAMG, 1E-6, 1E-6, matrix);

	// Workaround - seems OK to compare these in C++,
	// but BOOST doesn't like making the comparison
	if(solver.a == PETSC_NULL)
	{
		BOOST_CHECK_EQUAL(true, false);
	}

	// Workaround - seems OK to compare these in C++,
	// but BOOST doesn't like making the comparison
	if(solver.x == PETSC_NULL)
	{
		BOOST_CHECK_EQUAL(true, false);
	}

	// Workaround - seems OK to compare these in C++,
	// but BOOST doesn't like making the comparison
	if(solver.b == PETSC_NULL)
	{
		BOOST_CHECK_EQUAL(true, false);
	}

	BOOST_CHECK_EQUAL(solver.mGlobal, 8);
	BOOST_CHECK_EQUAL(solver.nGlobal, 8);

	//BOOST_CHECK_EQUAL(solver.xRanges, static_cast<decltype(solver.xRanges)>(nullptr));
	//BOOST_CHECK_EQUAL(solver.bRanges, static_cast<decltype(solver.bRanges)>(nullptr));
	//BOOST_CHECK_EQUAL(solver.aRanges, static_cast<decltype(solver.aRanges)>(nullptr));
}

// Test 2: Create a Parallel PETSc Linear Solver
BOOST_AUTO_TEST_CASE(constructor_test2)
{
	cupcfd::comm::Communicator comm(MPI_COMM_WORLD);

	// Create a simple SparseMatrix
	cupcfd::data_structures::SparseMatrixCOO<int, double> matrix(8, 8, 0);

	if(comm.rank == 0)
	{
		int rows[5] = {0, 0, 1, 1, 2};
		int cols[5] = {0, 1, 1, 2, 2};
		double vals[5] = {0.1, 0.2, 0.1, 0.05, 0.07};

		for(int i = 0; i < 5; i++)
		{
			matrix.setElement(rows[i], cols[i], vals[i]);
		}
	}
	else if(comm.rank == 1)
	{
		int rows[3] = {2, 3, 3};
		int cols[3] = {3, 3, 4};
		double vals[3] = {0.09, 0.06, 0.1};

		for(int i = 0; i < 3; i++)
		{
			matrix.setElement(rows[i], cols[i], vals[i]);
		}
	}
	else if(comm.rank == 2)
	{
		int rows[3] = {4, 4, 5};
		int cols[3] = {4, 5, 5};
		double vals[3] = {0.15, 0.23, 0.11};

		for(int i = 0; i < 3; i++)
		{
			matrix.setElement(rows[i], cols[i], vals[i]);
		}
	}
	else if(comm.rank == 3)
	{
		int rows[2] = {6, 7};
		int cols[2] = {6, 7};
		double vals[2] = {0.13, 0.09};

		for(int i = 0; i < 2; i++)
		{
			matrix.setElement(rows[i], cols[i], vals[i]);
		}
	}

	LinearSolverPETSc<cupcfd::data_structures::SparseMatrixCOO<int, double>, int, double> solver(comm, PETSC_KSP_CGAMG, 1E-6, 1E-6, matrix);

	// Workaround - seems OK to compare these in C++,
	// but BOOST doesn't like making the comparison
	if(solver.a == PETSC_NULL)
	{
		BOOST_CHECK_EQUAL(true, false);
	}

	// Workaround - seems OK to compare these in C++,
	// but BOOST doesn't like making the comparison
	if(solver.x == PETSC_NULL)
	{
		BOOST_CHECK_EQUAL(true, false);
	}

	// Workaround - seems OK to compare these in C++,
	// but BOOST doesn't like making the comparison
	if(solver.b == PETSC_NULL)
	{
		BOOST_CHECK_EQUAL(true, false);
	}

	BOOST_CHECK_EQUAL(solver.mGlobal, 8);
	BOOST_CHECK_EQUAL(solver.nGlobal, 8);

	//BOOST_CHECK_EQUAL(solver.xRanges, static_cast<decltype(solver.xRanges)>(nullptr));
	//BOOST_CHECK_EQUAL(solver.bRanges, static_cast<decltype(solver.bRanges)>(nullptr));
	//BOOST_CHECK_EQUAL(solver.aRanges, static_cast<decltype(solver.aRanges)>(nullptr));
}

// ============== resetVectorX ===================
// Test 1: Test the successful reset of a existing vector
BOOST_AUTO_TEST_CASE(resetVectorX_test1)
{
	cupcfd::error::eCodes status;

	cupcfd::comm::Communicator comm;

	// Create a simple SparseMatrix
	cupcfd::data_structures::SparseMatrixCOO<int, double> matrix(8, 8, 0);

	int rows[13] = {0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 6, 7};
	int cols[13] = {0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 7};
	double vals[13] = {0.1, 0.2, 0.1, 0.05, 0.07, 0.09, 0.06, 0.1, 0.15, 0.23, 0.11, 0.13, 0.09};

	for(int i = 0; i < 13; i++)
	{
		matrix.setElement(rows[i], cols[i], vals[i]);
	}

	LinearSolverPETSc<cupcfd::data_structures::SparseMatrixCOO<int, double>, int, double> solver(comm, PETSC_KSP_CGAMG, 1E-6, 1E-6, matrix);

	//Setup

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

// ============== resetVectorB ===================
// Test 1: Test the successful reset of a existing vector
BOOST_AUTO_TEST_CASE(resetVectorB_test1)
{
	cupcfd::error::eCodes status;

	cupcfd::comm::Communicator comm;

	// Create a simple SparseMatrix
	cupcfd::data_structures::SparseMatrixCOO<int, double> matrix(8, 8, 0);

	int rows[13] = {0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 6, 7};
	int cols[13] = {0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 7};
	double vals[13] = {0.1, 0.2, 0.1, 0.05, 0.07, 0.09, 0.06, 0.1, 0.15, 0.23, 0.11, 0.13, 0.09};

	for(int i = 0; i < 13; i++)
	{
		matrix.setElement(rows[i], cols[i], vals[i]);
	}

	LinearSolverPETSc<cupcfd::data_structures::SparseMatrixCOO<int, double>, int, double> solver(comm, PETSC_KSP_CGAMG, 1E-6, 1E-6, matrix);

	//Setup

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

// ============== resetMatrixA ===================
// Test 1: Test the reset of an existing matrix
BOOST_AUTO_TEST_CASE(resetMatrixA_test1)
{
	cupcfd::error::eCodes status;
	cupcfd::comm::Communicator comm;

	// Create a simple SparseMatrix
	cupcfd::data_structures::SparseMatrixCOO<int, double> matrix(8, 8, 0);

	int rows[13] = {0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 6, 7};
	int cols[13] = {0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 7};
	double vals[13] = {0.1, 0.2, 0.1, 0.05, 0.07, 0.09, 0.06, 0.1, 0.15, 0.23, 0.11, 0.13, 0.09};

	for(int i = 0; i < 13; i++)
	{
		matrix.setElement(rows[i], cols[i], vals[i]);
	}

	LinearSolverPETSc<cupcfd::data_structures::SparseMatrixCOO<int, double>, int, double> solver(comm, PETSC_KSP_CGAMG, 1E-6, 1E-6, matrix);

	// === Setup ===

	// Test and Check
	status = solver.setup(matrix);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	status = solver.resetMatrixA();
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
}

// ============== setupVectorX ===================
// Test 1: Check Vector is created if a suitable row size is set - serial
BOOST_AUTO_TEST_CASE(setupVectorX_test1)
{
	cupcfd::error::eCodes status;
	cupcfd::comm::Communicator comm;

	// Create a simple SparseMatrix
	cupcfd::data_structures::SparseMatrixCOO<int, double> matrix(8, 8, 0);

	int rows[13] = {0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 6, 7};
	int cols[13] = {0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 7};
	double vals[13] = {0.1, 0.2, 0.1, 0.05, 0.07, 0.09, 0.06, 0.1, 0.15, 0.23, 0.11, 0.13, 0.09};

	for(int i = 0; i < 13; i++)
	{
		matrix.setElement(rows[i], cols[i], vals[i]);
	}

	LinearSolverPETSc<cupcfd::data_structures::SparseMatrixCOO<int, double>, int, double> solver(comm, PETSC_KSP_CGAMG, 1E-6, 1E-6, matrix);

	// Reset vector prior to setup
	solver.resetVectorX();

	// Test and Check
	status = solver.setupVectorX();

	// Workaround - seems OK to compare these in C++,
	// but BOOST doesn't like making the comparison
	if(solver.x == PETSC_NULL)
	{
		BOOST_CHECK_EQUAL(true, false);
	}

	PetscInt cmp[2] = {0, 8};
	BOOST_CHECK_EQUAL_COLLECTIONS(cmp, cmp + 2, solver.xRanges, solver.xRanges + 2);

	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
}

// Test 2: Check Vector is created if a suitable row size is set - parallel
BOOST_AUTO_TEST_CASE(setupVectorX_test2)
{
	cupcfd::error::eCodes status;
	cupcfd::comm::Communicator comm(MPI_COMM_WORLD);

	// Create a simple SparseMatrix
	cupcfd::data_structures::SparseMatrixCOO<int, double> matrix(8, 8, 0);

	if(comm.rank == 0)
	{
		int rows[5] = {0, 0, 1, 1, 2};
		int cols[5] = {0, 1, 1, 2, 2};
		double vals[5] = {0.1, 0.2, 0.1, 0.05, 0.07};

		for(int i = 0; i < 5; i++)
		{
			matrix.setElement(rows[i], cols[i], vals[i]);
		}
	}
	else if(comm.rank == 1)
	{
		int rows[3] = {2, 3, 3};
		int cols[3] = {3, 3, 4};
		double vals[3] = {0.09, 0.06, 0.1};

		for(int i = 0; i < 3; i++)
		{
			matrix.setElement(rows[i], cols[i], vals[i]);
		}
	}
	else if(comm.rank == 2)
	{
		int rows[3] = {4, 4, 5};
		int cols[3] = {4, 5, 5};
		double vals[3] = {0.15, 0.23, 0.11};

		for(int i = 0; i < 3; i++)
		{
			matrix.setElement(rows[i], cols[i], vals[i]);
		}
	}
	else if(comm.rank == 3)
	{
		int rows[2] = {6, 7};
		int cols[2] = {6, 7};
		double vals[2] = {0.13, 0.09};

		for(int i = 0; i < 2; i++)
		{
			matrix.setElement(rows[i], cols[i], vals[i]);
		}
	}

	LinearSolverPETSc<cupcfd::data_structures::SparseMatrixCOO<int, double>, int, double> solver(comm, PETSC_KSP_CGAMG, 1E-6, 1E-6, matrix);

	// Reset vector prior to setup
	solver.resetVectorX();

	// Test and Check
	status = solver.setupVectorX();

	// Workaround - seems OK to compare these in C++,
	// but BOOST doesn't like making the comparison
	if(solver.x == PETSC_NULL)
	{
		BOOST_CHECK_EQUAL(true, false);
	}

	// Is this always guaranteed? Assuming for now...
	PetscInt cmp[5] = {0, 2, 4, 6, 8};
	BOOST_CHECK_EQUAL_COLLECTIONS(cmp, cmp + 5, solver.xRanges, solver.xRanges + 5);

	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
}

// ============== setupVectorB ===================
// Test 1: Check Vector is created if a suitable row size is set - serial
BOOST_AUTO_TEST_CASE(setupVectorB_test1)
{
	cupcfd::error::eCodes status;
	cupcfd::comm::Communicator comm;

	// Create a simple SparseMatrix
	cupcfd::data_structures::SparseMatrixCOO<int, double> matrix(8, 8, 0);

	int rows[13] = {0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 6, 7};
	int cols[13] = {0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 7};
	double vals[13] = {0.1, 0.2, 0.1, 0.05, 0.07, 0.09, 0.06, 0.1, 0.15, 0.23, 0.11, 0.13, 0.09};

	for(int i = 0; i < 13; i++)
	{
		matrix.setElement(rows[i], cols[i], vals[i]);
	}

	LinearSolverPETSc<cupcfd::data_structures::SparseMatrixCOO<int, double>, int, double> solver(comm, PETSC_KSP_CGAMG, 1E-6, 1E-6, matrix);

	// Reset vector prior to setup
	solver.resetVectorB();

	// Test and Check
	status = solver.setupVectorB();
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	// Workaround - seems OK to compare these in C++,
	// but BOOST doesn't like making the comparison
	if(solver.b == PETSC_NULL)
	{
		BOOST_CHECK_EQUAL(true, false);
	}

	PetscInt cmp[2] = {0, 8};
	BOOST_CHECK_EQUAL_COLLECTIONS(cmp, cmp + 2, solver.bRanges, solver.bRanges + 2);

	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
}

// Test 2: Check Vector is created if a suitable row size is set - parallel
BOOST_AUTO_TEST_CASE(setupVectorB_test2)
{
	cupcfd::error::eCodes status;
	cupcfd::comm::Communicator comm(MPI_COMM_WORLD);

	// Create a simple SparseMatrix
	cupcfd::data_structures::SparseMatrixCOO<int, double> matrix(20, 20, 0);

	if(comm.rank == 0)
	{
		int rows[5] = {0, 0, 1, 1, 2};
		int cols[5] = {0, 1, 1, 2, 2};
		double vals[5] = {0.1, 0.2, 0.1, 0.05, 0.07};

		for(int i = 0; i < 5; i++)
		{
			matrix.setElement(rows[i], cols[i], vals[i]);
		}
	}
	else if(comm.rank == 1)
	{
		int rows[3] = {6, 6, 6};
		int cols[3] = {6, 7, 8};
		double vals[3] = {0.09, 0.06, 0.1};

		for(int i = 0; i < 3; i++)
		{
			matrix.setElement(rows[i], cols[i], vals[i]);
		}
	}
	else if(comm.rank == 2)
	{
		int rows[3] = {10, 10, 10};
		int cols[3] = {10, 11, 12};
		double vals[3] = {0.15, 0.23, 0.11};

		for(int i = 0; i < 3; i++)
		{
			matrix.setElement(rows[i], cols[i], vals[i]);
		}
	}
	else if(comm.rank == 3)
	{
		int rows[2] = {15, 16};
		int cols[2] = {15, 16};
		double vals[2] = {0.13, 0.09};

		for(int i = 0; i < 2; i++)
		{
			matrix.setElement(rows[i], cols[i], vals[i]);
		}
	}

	LinearSolverPETSc<cupcfd::data_structures::SparseMatrixCOO<int, double>, int, double> solver(comm, PETSC_KSP_CGAMG, 1E-6, 1E-6, matrix);

	// Reset vector prior to setup
	solver.resetVectorB();

	// Test and Check
	status = solver.setupVectorB();
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	// Workaround - seems OK to compare these in C++,
	// but BOOST doesn't like making the comparison
	if(solver.b == PETSC_NULL)
	{
		BOOST_CHECK_EQUAL(true, false);
	}

	// Is this always guaranteed? Assuming for now...
	PetscInt cmp[5] = {0, 5, 10, 15, 20};
	BOOST_CHECK_EQUAL_COLLECTIONS(cmp, cmp + 5, solver.bRanges, solver.bRanges + 5);

	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
}


// ============== setupMatrixA ===================
// Test 1: Setup a Matrix on a Serial Solver from a SparseCOO matrix
BOOST_AUTO_TEST_CASE(setupMatrixA_test1)
{
	cupcfd::error::eCodes status;
	cupcfd::comm::Communicator comm;

	// Create a simple SparseMatrix
	cupcfd::data_structures::SparseMatrixCOO<int, double> matrix(8, 8, 0);

	int rows[13] = {0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 6, 7};
	int cols[13] = {0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 7};
	double vals[13] = {0.1, 0.2, 0.1, 0.05, 0.07, 0.09, 0.06, 0.1, 0.15, 0.23, 0.11, 0.13, 0.09};

	for(int i = 0; i < 13; i++)
	{
		matrix.setElement(rows[i], cols[i], vals[i]);
	}

	LinearSolverPETSc<cupcfd::data_structures::SparseMatrixCOO<int, double>, int, double> solver(comm, PETSC_KSP_CGAMG, 1E-6, 1E-6, matrix);

	// Reset Matrix prior to setup
	solver.resetMatrixA();

	// === Setup ===

	// Test and Check
	status = solver.setupMatrixA(matrix);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	PetscInt cmp[2] = {0, 8};
	BOOST_CHECK_EQUAL_COLLECTIONS(cmp, cmp + 2, solver.aRanges, solver.aRanges + 2);

	// ToDo: Ideally would like to check PETSc internal nnz structure of matrix
}


// Test 2: Setup a Matrix on a Distributed Solver, where each rank holds a different set of rows
BOOST_AUTO_TEST_CASE(setupMatrixA_test2)
{
	cupcfd::error::eCodes status;
	cupcfd::comm::Communicator comm(MPI_COMM_WORLD);

	// Create a simple SparseMatrix
	cupcfd::data_structures::SparseMatrixCOO<int, double> matrix(8, 8, 0);

	if(comm.rank == 0)
	{
		int rows[5] = {0, 0, 1, 1, 2};
		int cols[5] = {0, 1, 1, 2, 2};
		double vals[5] = {0.1, 0.2, 0.1, 0.05, 0.07};

		for(int i = 0; i < 5; i++)
		{
			matrix.setElement(rows[i], cols[i], vals[i]);
		}
	}
	else if(comm.rank == 1)
	{
		int rows[3] = {2, 3, 3};
		int cols[3] = {3, 3, 4};
		double vals[3] = {0.09, 0.06, 0.1};

		for(int i = 0; i < 3; i++)
		{
			matrix.setElement(rows[i], cols[i], vals[i]);
		}
	}
	else if(comm.rank == 2)
	{
		int rows[3] = {4, 4, 5};
		int cols[3] = {4, 5, 5};
		double vals[3] = {0.15, 0.23, 0.11};

		for(int i = 0; i < 3; i++)
		{
			matrix.setElement(rows[i], cols[i], vals[i]);
		}
	}
	else if(comm.rank == 3)
	{
		int rows[2] = {6, 7};
		int cols[2] = {6, 7};
		double vals[2] = {0.13, 0.09};

		for(int i = 0; i < 2; i++)
		{
			matrix.setElement(rows[i], cols[i], vals[i]);
		}
	}

	LinearSolverPETSc<cupcfd::data_structures::SparseMatrixCOO<int, double>, int, double> solver(comm, PETSC_KSP_CGAMG, 1E-6, 1E-6, matrix);

	// Reset Matrix prior to setup
	solver.resetMatrixA();

	// === Setup ===

	// Test and Check
	status = solver.setupMatrixA(matrix);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	//PetscInt cmp[2] = {0, 8};
	//BOOST_CHECK_EQUAL_COLLECTIONS(cmp, cmp + 2, solver.aRanges, solver.aRanges + 2);

	// ToDo: Ideally would like to check PETSc internal nnz structure of matrix
}

// ============== setup ===================

// Test 1: Test setup on a Serial Solver
BOOST_AUTO_TEST_CASE(setup_test1)
{
	cupcfd::error::eCodes status;
	cupcfd::comm::Communicator comm;

	// Create a simple SparseMatrix
	cupcfd::data_structures::SparseMatrixCOO<int, double> matrix(8, 8, 0);

	int rows[13] = {0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 6, 7};
	int cols[13] = {0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 7};
	double vals[13] = {0.1, 0.2, 0.1, 0.05, 0.07, 0.09, 0.06, 0.1, 0.15, 0.23, 0.11, 0.13, 0.09};

	for(int i = 0; i < 13; i++)
	{
		matrix.setElement(rows[i], cols[i], vals[i]);
	}

	LinearSolverPETSc<cupcfd::data_structures::SparseMatrixCOO<int, double>, int, double> solver(comm, PETSC_KSP_CGAMG, 1E-6, 1E-6, matrix);

	// === Setup ===

	// Reset Vectors and Matrix
	solver.resetVectorX();
	solver.resetVectorB();
	solver.resetMatrixA();

	// Test and Check
	status = solver.setup(matrix);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
}

// Test 2: Test setup on a Distributed Solver
BOOST_AUTO_TEST_CASE(setup_test2)
{
	cupcfd::error::eCodes status;
	cupcfd::comm::Communicator comm(MPI_COMM_WORLD);

	// Create a simple SparseMatrix
	cupcfd::data_structures::SparseMatrixCOO<int, double> matrix(8, 8, 0);

	if(comm.rank == 0)
	{
		int rows[5] = {0, 0, 1, 1, 2};
		int cols[5] = {0, 1, 1, 2, 2};
		double vals[5] = {0.1, 0.2, 0.1, 0.05, 0.07};

		for(int i = 0; i < 5; i++)
		{
			matrix.setElement(rows[i], cols[i], vals[i]);
		}
	}
	else if(comm.rank == 1)
	{
		int rows[3] = {2, 3, 3};
		int cols[3] = {3, 3, 4};
		double vals[3] = {0.09, 0.06, 0.1};

		for(int i = 0; i < 3; i++)
		{
			matrix.setElement(rows[i], cols[i], vals[i]);
		}
	}
	else if(comm.rank == 2)
	{
		int rows[3] = {4, 4, 5};
		int cols[3] = {4, 5, 5};
		double vals[3] = {0.15, 0.23, 0.11};

		for(int i = 0; i < 3; i++)
		{
			matrix.setElement(rows[i], cols[i], vals[i]);
		}
	}
	else if(comm.rank == 3)
	{
		int rows[2] = {6, 7};
		int cols[2] = {6, 7};
		double vals[2] = {0.13, 0.09};

		for(int i = 0; i < 2; i++)
		{
			matrix.setElement(rows[i], cols[i], vals[i]);
		}
	}

	LinearSolverPETSc<cupcfd::data_structures::SparseMatrixCOO<int, double>, int, double> solver(comm, PETSC_KSP_CGAMG, 1E-6, 1E-6, matrix);

	// === Setup ===

	// Reset Vectors and Matrix
	solver.resetVectorX();
	solver.resetVectorB();
	solver.resetMatrixA();

	// Test and Check
	status = solver.setup(matrix);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
}


// ============== setValuesVectorX/getValuesVectorX ===================
// Note: For these tests we can only presume that the values are set correctly
// without error till the getters are tested....

// Test 1: Set all values to same scalar + get all values (serial)
BOOST_AUTO_TEST_CASE(set_getValuesVectorX_test1)
{
	cupcfd::error::eCodes status;
	cupcfd::comm::Communicator comm;

	// Create a simple SparseMatrix
	cupcfd::data_structures::SparseMatrixCOO<int, double> matrix(8, 8, 0);

	int rows[13] = {0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 6, 7};
	int cols[13] = {0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 7};
	double vals[13] = {0.1, 0.2, 0.1, 0.05, 0.07, 0.09, 0.06, 0.1, 0.15, 0.23, 0.11, 0.13, 0.09};

	for(int i = 0; i < 13; i++)
	{
		matrix.setElement(rows[i], cols[i], vals[i]);
	}

	LinearSolverPETSc<cupcfd::data_structures::SparseMatrixCOO<int, double>, int, double> solver(comm, PETSC_KSP_CGAMG, 1E-6, 1E-6, matrix);

	// Setup
	solver.setupVectorX();

	// Test and check
	status = solver.setValuesVectorX(2.5);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	double * result;
	int nResult;
	double resultCmp[8] = {2.5, 2.5, 2.5, 2.5, 2.5, 2.5, 2.5, 2.5};
	status = solver.getValuesVectorX(&result, &nResult);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	BOOST_CHECK_EQUAL(nResult, 8);
	BOOST_CHECK_EQUAL_COLLECTIONS(result, result + 8, resultCmp, resultCmp + 8);
}

// Test 2: Set all values to same scalar + get all values (parallel)
BOOST_AUTO_TEST_CASE(set_getValuesVectorX_test2)
{
	cupcfd::error::eCodes status;
	cupcfd::comm::Communicator comm(MPI_COMM_WORLD);

	// Create a simple SparseMatrix
	cupcfd::data_structures::SparseMatrixCOO<int, double> matrix(8, 8, 0);

	if(comm.rank == 0)
	{
		int rows[5] = {0, 0, 1, 1, 2};
		int cols[5] = {0, 1, 1, 2, 2};
		double vals[5] = {0.1, 0.2, 0.1, 0.05, 0.07};

		for(int i = 0; i < 5; i++)
		{
			matrix.setElement(rows[i], cols[i], vals[i]);
		}
	}
	else if(comm.rank == 1)
	{
		int rows[3] = {2, 3, 3};
		int cols[3] = {3, 3, 4};
		double vals[3] = {0.09, 0.06, 0.1};

		for(int i = 0; i < 3; i++)
		{
			matrix.setElement(rows[i], cols[i], vals[i]);
		}
	}
	else if(comm.rank == 2)
	{
		int rows[3] = {4, 4, 5};
		int cols[3] = {4, 5, 5};
		double vals[3] = {0.15, 0.23, 0.11};

		for(int i = 0; i < 3; i++)
		{
			matrix.setElement(rows[i], cols[i], vals[i]);
		}
	}
	else if(comm.rank == 3)
	{
		int rows[2] = {6, 7};
		int cols[2] = {6, 7};
		double vals[2] = {0.13, 0.09};

		for(int i = 0; i < 2; i++)
		{
			matrix.setElement(rows[i], cols[i], vals[i]);
		}
	}

	LinearSolverPETSc<cupcfd::data_structures::SparseMatrixCOO<int, double>, int, double> solver(comm, PETSC_KSP_CGAMG, 1E-6, 1E-6, matrix);

	// Setup
	solver.setupVectorX();

	// Test and check
	status = solver.setValuesVectorX(2.5);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	double * result;
	int nResult;
	double resultCmp[8] = {2.5, 2.5, 2.5, 2.5, 2.5, 2.5, 2.5, 2.5};

	status = solver.getValuesVectorX(&result, &nResult);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	BOOST_CHECK_EQUAL(nResult, 8);
	BOOST_CHECK_EQUAL_COLLECTIONS(result, result + 8, resultCmp, resultCmp + 8);
}

// Test 3: Set values to specific indices + retrieve those indices (serial) - all local rows
BOOST_AUTO_TEST_CASE(set_getValuesVectorX_test3)
{
	cupcfd::error::eCodes status;
	cupcfd::comm::Communicator comm;

	// Create a simple SparseMatrix
	cupcfd::data_structures::SparseMatrixCOO<int, double> matrix(8, 8, 0);

	int rows[13] = {0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 6, 7};
	int cols[13] = {0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 7};
	double matvals[13] = {0.1, 0.2, 0.1, 0.05, 0.07, 0.09, 0.06, 0.1, 0.15, 0.23, 0.11, 0.13, 0.09};

	for(int i = 0; i < 13; i++)
	{
		matrix.setElement(rows[i], cols[i], matvals[i]);
	}

	LinearSolverPETSc<cupcfd::data_structures::SparseMatrixCOO<int, double>, int, double> solver(comm, PETSC_KSP_CGAMG, 1E-6, 1E-6, matrix);

	// Setup
	solver.setupVectorX();

	int indices[4] = {2, 5, 3, 7};
	double vals[4] = {0.5, 0.3, 0.6, 0.1};

	double * result;
	int nResult;

	double * fullResult;
	int nFullResult;
	double fullResultCmp[8] = {0.0, 0.0, 0.5, 0.6, 0.0, 0.3, 0.0, 0.1};

	status = solver.setValuesVectorX(0.0);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	status = solver.setValuesVectorX(vals, 4, indices, 4, 0);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	status = solver.getValuesVectorX(&result, &nResult, indices, 4, 0);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	BOOST_CHECK_EQUAL(nResult, 4);
	BOOST_CHECK_EQUAL_COLLECTIONS(result, result + 4, vals, vals + 4);

	status = solver.getValuesVectorX(&fullResult, &nFullResult);
	BOOST_CHECK_EQUAL(nFullResult, 8);
	BOOST_CHECK_EQUAL_COLLECTIONS(fullResult, fullResult + 8, fullResultCmp, fullResultCmp + 8);
}

// Test 4: Set values to specific indices + retrieve those indices (parallel) - all local rows
BOOST_AUTO_TEST_CASE(set_getValuesVectorX_test4)
{
	cupcfd::error::eCodes status;
	cupcfd::comm::Communicator comm(MPI_COMM_WORLD);

	// Create a simple SparseMatrix
	cupcfd::data_structures::SparseMatrixCOO<int, double> matrix(8, 8, 0);

	if(comm.rank == 0)
	{
		int rows[5] = {0, 0, 1, 1, 2};
		int cols[5] = {0, 1, 1, 2, 2};
		double vals[5] = {0.1, 0.2, 0.1, 0.05, 0.07};

		for(int i = 0; i < 5; i++)
		{
			matrix.setElement(rows[i], cols[i], vals[i]);
		}
	}
	else if(comm.rank == 1)
	{
		int rows[3] = {2, 3, 3};
		int cols[3] = {3, 3, 4};
		double vals[3] = {0.09, 0.06, 0.1};

		for(int i = 0; i < 3; i++)
		{
			matrix.setElement(rows[i], cols[i], vals[i]);
		}
	}
	else if(comm.rank == 2)
	{
		int rows[3] = {4, 4, 5};
		int cols[3] = {4, 5, 5};
		double vals[3] = {0.15, 0.23, 0.11};

		for(int i = 0; i < 3; i++)
		{
			matrix.setElement(rows[i], cols[i], vals[i]);
		}
	}
	else if(comm.rank == 3)
	{
		int rows[2] = {6, 7};
		int cols[2] = {6, 7};
		double vals[2] = {0.13, 0.09};

		for(int i = 0; i < 2; i++)
		{
			matrix.setElement(rows[i], cols[i], vals[i]);
		}
	}

	LinearSolverPETSc<cupcfd::data_structures::SparseMatrixCOO<int, double>, int, double> solver(comm, PETSC_KSP_CGAMG, 1E-6, 1E-6, matrix);

	// Setup
	solver.setupVectorX();

	// Check they are actually local rows
	PetscInt rangeCmp[5] = {0, 2, 4, 6, 8};
	BOOST_CHECK_EQUAL_COLLECTIONS(rangeCmp, rangeCmp + 5, solver.xRanges, solver.xRanges + 5);

	double * fullResult;
	int nFullResult;
	double fullResultCmp[8] = {0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8};

	double * result;
	int nResult;

	status = solver.setValuesVectorX(0.0);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	if(comm.rank == 0)
	{
		int indices[2] = {0, 1};
		double vals[2] = {0.1, 0.2};

		status = solver.setValuesVectorX(vals, 2, indices, 2, 0);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

		status = solver.getValuesVectorX(&result, &nResult, indices, 2, 0);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
		BOOST_CHECK_EQUAL(nResult, 2);
		BOOST_CHECK_EQUAL_COLLECTIONS(result, result + 2, vals, vals + 2);
		free(result);
	}
	else if(comm.rank == 1)
	{
		int indices[2] = {2, 3};
		double vals[2] = {0.3, 0.4};

		status = solver.setValuesVectorX(vals, 2, indices, 2, 0);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

		status = solver.getValuesVectorX(&result, &nResult, indices, 2, 0);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
		BOOST_CHECK_EQUAL(nResult, 2);
		BOOST_CHECK_EQUAL_COLLECTIONS(result, result + 2, vals, vals + 2);
		free(result);
	}
	else if(comm.rank == 2)
	{
		int indices[2] = {4, 5};
		double vals[2] = {0.5, 0.6};

		status = solver.setValuesVectorX(vals, 2, indices, 2, 0);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

		status = solver.getValuesVectorX(&result, &nResult, indices, 2, 0);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
		BOOST_CHECK_EQUAL(nResult, 2);
		BOOST_CHECK_EQUAL_COLLECTIONS(result, result + 2, vals, vals + 2);
		free(result);
	}
	else if(comm.rank == 3)
	{
		int indices[2] = {6, 7};
		double vals[2] = {0.7, 0.8};

		status = solver.setValuesVectorX(vals, 2, indices, 2, 0);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

		status = solver.getValuesVectorX(&result, &nResult, indices, 2, 0);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
		BOOST_CHECK_EQUAL(nResult, 2);
		BOOST_CHECK_EQUAL_COLLECTIONS(result, result + 2, vals, vals + 2);
		free(result);
	}

	// Test all values set correctly too.
	status = solver.getValuesVectorX(&fullResult, &nFullResult);
	BOOST_CHECK_EQUAL(nFullResult, 8);
	BOOST_CHECK_EQUAL_COLLECTIONS(fullResult, fullResult + 8, fullResultCmp, fullResultCmp + 8);
	free(fullResult);
}

// Test 5: Set values to specific indices + retrieve those indices (parallel) - off-node rows
BOOST_AUTO_TEST_CASE(set_getValuesVectorX_test5)
{
	cupcfd::error::eCodes status;
	cupcfd::comm::Communicator comm(MPI_COMM_WORLD);

	// Create a simple SparseMatrix
	cupcfd::data_structures::SparseMatrixCOO<int, double> matrix(8, 8, 0);

	if(comm.rank == 0)
	{
		int rows[5] = {0, 0, 1, 1, 2};
		int cols[5] = {0, 1, 1, 2, 2};
		double vals[5] = {0.1, 0.2, 0.1, 0.05, 0.07};

		for(int i = 0; i < 5; i++)
		{
			matrix.setElement(rows[i], cols[i], vals[i]);
		}
	}
	else if(comm.rank == 1)
	{
		int rows[3] = {2, 3, 3};
		int cols[3] = {3, 3, 4};
		double vals[3] = {0.09, 0.06, 0.1};

		for(int i = 0; i < 3; i++)
		{
			matrix.setElement(rows[i], cols[i], vals[i]);
		}
	}
	else if(comm.rank == 2)
	{
		int rows[3] = {4, 4, 5};
		int cols[3] = {4, 5, 5};
		double vals[3] = {0.15, 0.23, 0.11};

		for(int i = 0; i < 3; i++)
		{
			matrix.setElement(rows[i], cols[i], vals[i]);
		}
	}
	else if(comm.rank == 3)
	{
		int rows[2] = {6, 7};
		int cols[2] = {6, 7};
		double vals[2] = {0.13, 0.09};

		for(int i = 0; i < 2; i++)
		{
			matrix.setElement(rows[i], cols[i], vals[i]);
		}
	}

	LinearSolverPETSc<cupcfd::data_structures::SparseMatrixCOO<int, double>, int, double> solver(comm, PETSC_KSP_CGAMG, 1E-6, 1E-6, matrix);

	// Setup
	solver.setupVectorX();

	// Check they are actually local rows
	PetscInt rangeCmp[5] = {0, 2, 4, 6, 8};
	BOOST_CHECK_EQUAL_COLLECTIONS(rangeCmp, rangeCmp + 5, solver.xRanges, solver.xRanges + 5);
	BOOST_CHECK_EQUAL_COLLECTIONS(rangeCmp, rangeCmp + 5, solver.bRanges, solver.bRanges + 5);
	BOOST_CHECK_EQUAL_COLLECTIONS(rangeCmp, rangeCmp + 5, solver.aRanges, solver.aRanges + 5);


	double * result;
	int nResult;

	status = solver.setValuesVectorX(0.0);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	if(comm.rank == 0)
	{
		int indices[4] = {0, 1, 2, 5};
		double vals[4] = {0.1, 0.2, 0.3, 0.6};

		status = solver.setValuesVectorX(vals, 4, indices, 4, 0);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

		status = solver.getValuesVectorX(&result, &nResult, indices, 4, 0);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
		BOOST_CHECK_EQUAL(nResult, 4);
		BOOST_CHECK_EQUAL_COLLECTIONS(result, result + 4, vals, vals + 4);
		//free(result);
	}
	else if(comm.rank == 1)
	{
		int indices[1] = {3};
		double vals[1] = {0.4};

		status = solver.setValuesVectorX(vals, 1, indices, 1, 0);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

		status = solver.getValuesVectorX(&result, &nResult, indices, 1, 0);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
		BOOST_CHECK_EQUAL(nResult, 1);
		BOOST_CHECK_EQUAL_COLLECTIONS(result, result + 1, vals, vals + 1);
		//free(result);
	}
	else if(comm.rank == 2)
	{
		int indices[2] = {4, 7};
		double vals[2] = {0.5, 0.8};

		status = solver.setValuesVectorX(vals, 2, indices, 2, 0);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

		status = solver.getValuesVectorX(&result, &nResult, indices, 2, 0);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
		BOOST_CHECK_EQUAL(nResult, 2);
		BOOST_CHECK_EQUAL_COLLECTIONS(result, result + 2, vals, vals + 2);
		//free(result);
	}
	else if(comm.rank == 3)
	{
		int indices[1] = {6};
		double vals[1] = {0.7};

		status = solver.setValuesVectorX(vals, 1, indices, 1, 0);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

		status = solver.getValuesVectorX(&result, &nResult, indices, 1, 0);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
		BOOST_CHECK_EQUAL(nResult, 1);
		BOOST_CHECK_EQUAL_COLLECTIONS(result, result + 1, vals, vals + 1);
		//free(result);
	}

	// Test all values set correctly too.
	double * fullResult;
	int nFullResult;
	double fullResultCmp[8] = {0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8};

	status = solver.getValuesVectorX(&fullResult, &nFullResult);
	BOOST_CHECK_EQUAL(nFullResult, 8);
	BOOST_CHECK_EQUAL_COLLECTIONS(fullResult, fullResult + 8, fullResultCmp, fullResultCmp + 8);
	free(fullResult);
}

// Test 6: Set values to specific indices + retrieve those indices (parallel) - off-node rows,
// using different bases for the indices
BOOST_AUTO_TEST_CASE(set_getValuesVectorX_test6)
{

}

// ============== setValuesVectorB/getValuesVectorB ===================
// Note: For these tests we can only presume that the values are set correctly
// without error till the getters are tested....

// Test 1: Set all values to same scalar + get all values (serial)
BOOST_AUTO_TEST_CASE(set_getValuesVectorB_test1)
{
	cupcfd::error::eCodes status;
	cupcfd::comm::Communicator comm;

	// Create a simple SparseMatrix
	cupcfd::data_structures::SparseMatrixCOO<int, double> matrix(8, 8, 0);

	int rows[13] = {0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 6, 7};
	int cols[13] = {0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 7};
	double vals[13] = {0.1, 0.2, 0.1, 0.05, 0.07, 0.09, 0.06, 0.1, 0.15, 0.23, 0.11, 0.13, 0.09};

	for(int i = 0; i < 13; i++)
	{
		matrix.setElement(rows[i], cols[i], vals[i]);
	}

	LinearSolverPETSc<cupcfd::data_structures::SparseMatrixCOO<int, double>, int, double> solver(comm, PETSC_KSP_CGAMG, 1E-6, 1E-6, matrix);

	// Setup
	solver.setupVectorB();

	// Test and check
	status = solver.setValuesVectorB(2.5);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	double * result;
	int nResult;
	double resultCmp[8] = {2.5, 2.5, 2.5, 2.5, 2.5, 2.5, 2.5, 2.5};
	status = solver.getValuesVectorB(&result, &nResult);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	BOOST_CHECK_EQUAL(nResult, 8);
	BOOST_CHECK_EQUAL_COLLECTIONS(result, result + 8, resultCmp, resultCmp + 8);
}

// Test 2: Set all values to same scalar + get all values (parallel)
BOOST_AUTO_TEST_CASE(set_getValuesVectorB_test2)
{
	cupcfd::error::eCodes status;
	cupcfd::comm::Communicator comm(MPI_COMM_WORLD);

	// Create a simple SparseMatrix
	cupcfd::data_structures::SparseMatrixCOO<int, double> matrix(8, 8, 0);

	if(comm.rank == 0)
	{
		int rows[5] = {0, 0, 1, 1, 2};
		int cols[5] = {0, 1, 1, 2, 2};
		double vals[5] = {0.1, 0.2, 0.1, 0.05, 0.07};

		for(int i = 0; i < 5; i++)
		{
			matrix.setElement(rows[i], cols[i], vals[i]);
		}
	}
	else if(comm.rank == 1)
	{
		int rows[3] = {2, 3, 3};
		int cols[3] = {3, 3, 4};
		double vals[3] = {0.09, 0.06, 0.1};

		for(int i = 0; i < 3; i++)
		{
			matrix.setElement(rows[i], cols[i], vals[i]);
		}
	}
	else if(comm.rank == 2)
	{
		int rows[3] = {4, 4, 5};
		int cols[3] = {4, 5, 5};
		double vals[3] = {0.15, 0.23, 0.11};

		for(int i = 0; i < 3; i++)
		{
			matrix.setElement(rows[i], cols[i], vals[i]);
		}
	}
	else if(comm.rank == 3)
	{
		int rows[2] = {6, 7};
		int cols[2] = {6, 7};
		double vals[2] = {0.13, 0.09};

		for(int i = 0; i < 2; i++)
		{
			matrix.setElement(rows[i], cols[i], vals[i]);
		}
	}

	LinearSolverPETSc<cupcfd::data_structures::SparseMatrixCOO<int, double>, int, double> solver(comm, PETSC_KSP_CGAMG, 1E-6, 1E-6, matrix);

	// Setup
	solver.setupVectorB();

	// Test and check
	status = solver.setValuesVectorB(2.5);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	double * result;
	int nResult;
	double resultCmp[8] = {2.5, 2.5, 2.5, 2.5, 2.5, 2.5, 2.5, 2.5};

	status = solver.getValuesVectorB(&result, &nResult);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	BOOST_CHECK_EQUAL(nResult, 8);
	BOOST_CHECK_EQUAL_COLLECTIONS(result, result + 8, resultCmp, resultCmp + 8);
}

// Test 3: Set values to specific indices + retrieve those indices (serial) - all local rows
BOOST_AUTO_TEST_CASE(set_getValuesVectorB_test3)
{
	cupcfd::error::eCodes status;
	cupcfd::comm::Communicator comm;

	// Create a simple SparseMatrix
	cupcfd::data_structures::SparseMatrixCOO<int, double> matrix(8, 8, 0);

	int rows[13] = {0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 6, 7};
	int cols[13] = {0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 7};
	double matvals[13] = {0.1, 0.2, 0.1, 0.05, 0.07, 0.09, 0.06, 0.1, 0.15, 0.23, 0.11, 0.13, 0.09};

	for(int i = 0; i < 13; i++)
	{
		matrix.setElement(rows[i], cols[i], matvals[i]);
	}

	LinearSolverPETSc<cupcfd::data_structures::SparseMatrixCOO<int, double>, int, double> solver(comm, PETSC_KSP_CGAMG, 1E-6, 1E-6, matrix);

	// Setup
	solver.setupVectorB();

	int indices[4] = {2, 5, 3, 7};
	double vals[4] = {0.5, 0.3, 0.6, 0.1};

	double * result;
	int nResult;

	double * fullResult;
	int nFullResult;
	double fullResultCmp[8] = {0.0, 0.0, 0.5, 0.6, 0.0, 0.3, 0.0, 0.1};

	status = solver.setValuesVectorB(0.0);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	status = solver.setValuesVectorB(vals, 4, indices, 4, 0);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	status = solver.getValuesVectorB(&result, &nResult, indices, 4, 0);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	BOOST_CHECK_EQUAL(nResult, 4);
	BOOST_CHECK_EQUAL_COLLECTIONS(result, result + 4, vals, vals + 4);

	status = solver.getValuesVectorB(&fullResult, &nFullResult);
	BOOST_CHECK_EQUAL(nFullResult, 8);
	BOOST_CHECK_EQUAL_COLLECTIONS(fullResult, fullResult + 8, fullResultCmp, fullResultCmp + 8);
}

// Test 4: Set values to specific indices + retrieve those indices (parallel) - all local rows
BOOST_AUTO_TEST_CASE(set_getValuesVectorB_test4)
{
	cupcfd::error::eCodes status;
	cupcfd::comm::Communicator comm(MPI_COMM_WORLD);

	// Create a simple SparseMatrix
	cupcfd::data_structures::SparseMatrixCOO<int, double> matrix(8, 8, 0);

	if(comm.rank == 0)
	{
		int rows[5] = {0, 0, 1, 1, 2};
		int cols[5] = {0, 1, 1, 2, 2};
		double vals[5] = {0.1, 0.2, 0.1, 0.05, 0.07};

		for(int i = 0; i < 5; i++)
		{
			matrix.setElement(rows[i], cols[i], vals[i]);
		}
	}
	else if(comm.rank == 1)
	{
		int rows[3] = {2, 3, 3};
		int cols[3] = {3, 3, 4};
		double vals[3] = {0.09, 0.06, 0.1};

		for(int i = 0; i < 3; i++)
		{
			matrix.setElement(rows[i], cols[i], vals[i]);
		}
	}
	else if(comm.rank == 2)
	{
		int rows[3] = {4, 4, 5};
		int cols[3] = {4, 5, 5};
		double vals[3] = {0.15, 0.23, 0.11};

		for(int i = 0; i < 3; i++)
		{
			matrix.setElement(rows[i], cols[i], vals[i]);
		}
	}
	else if(comm.rank == 3)
	{
		int rows[2] = {6, 7};
		int cols[2] = {6, 7};
		double vals[2] = {0.13, 0.09};

		for(int i = 0; i < 2; i++)
		{
			matrix.setElement(rows[i], cols[i], vals[i]);
		}
	}

	LinearSolverPETSc<cupcfd::data_structures::SparseMatrixCOO<int, double>, int, double> solver(comm, PETSC_KSP_CGAMG, 1E-6, 1E-6, matrix);

	// Setup
	solver.setupVectorB();

	// Check they are actually local rows
	PetscInt rangeCmp[5] = {0, 2, 4, 6, 8};
	BOOST_CHECK_EQUAL_COLLECTIONS(rangeCmp, rangeCmp + 5, solver.xRanges, solver.xRanges + 5);

	double * fullResult;
	int nFullResult;
	double fullResultCmp[8] = {0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8};

	double * result;
	int nResult;

	status = solver.setValuesVectorB(0.0);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	if(comm.rank == 0)
	{
		int indices[2] = {0, 1};
		double vals[2] = {0.1, 0.2};

		status = solver.setValuesVectorB(vals, 2, indices, 2, 0);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

		status = solver.getValuesVectorB(&result, &nResult, indices, 2, 0);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
		BOOST_CHECK_EQUAL(nResult, 2);
		BOOST_CHECK_EQUAL_COLLECTIONS(result, result + 2, vals, vals + 2);
		free(result);
	}
	else if(comm.rank == 1)
	{
		int indices[2] = {2, 3};
		double vals[2] = {0.3, 0.4};

		status = solver.setValuesVectorB(vals, 2, indices, 2, 0);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

		status = solver.getValuesVectorB(&result, &nResult, indices, 2, 0);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
		BOOST_CHECK_EQUAL(nResult, 2);
		BOOST_CHECK_EQUAL_COLLECTIONS(result, result + 2, vals, vals + 2);
		free(result);
	}
	else if(comm.rank == 2)
	{
		int indices[2] = {4, 5};
		double vals[2] = {0.5, 0.6};

		status = solver.setValuesVectorB(vals, 2, indices, 2, 0);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

		status = solver.getValuesVectorB(&result, &nResult, indices, 2, 0);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
		BOOST_CHECK_EQUAL(nResult, 2);
		BOOST_CHECK_EQUAL_COLLECTIONS(result, result + 2, vals, vals + 2);
		free(result);
	}
	else if(comm.rank == 3)
	{
		int indices[2] = {6, 7};
		double vals[2] = {0.7, 0.8};

		status = solver.setValuesVectorB(vals, 2, indices, 2, 0);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

		status = solver.getValuesVectorB(&result, &nResult, indices, 2, 0);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
		BOOST_CHECK_EQUAL(nResult, 2);
		BOOST_CHECK_EQUAL_COLLECTIONS(result, result + 2, vals, vals + 2);
		free(result);
	}

	// Test all values set correctly too.
	status = solver.getValuesVectorB(&fullResult, &nFullResult);
	BOOST_CHECK_EQUAL(nFullResult, 8);
	BOOST_CHECK_EQUAL_COLLECTIONS(fullResult, fullResult + 8, fullResultCmp, fullResultCmp + 8);
	free(fullResult);
}

// Test 5: Set values to specific indices + retrieve those indices (parallel) - off-node rows
BOOST_AUTO_TEST_CASE(set_getValuesVectorB_test5)
{
	cupcfd::error::eCodes status;
	cupcfd::comm::Communicator comm(MPI_COMM_WORLD);

	// Create a simple SparseMatrix
	cupcfd::data_structures::SparseMatrixCOO<int, double> matrix(8, 8, 0);

	if(comm.rank == 0)
	{
		int rows[5] = {0, 0, 1, 1, 2};
		int cols[5] = {0, 1, 1, 2, 2};
		double vals[5] = {0.1, 0.2, 0.1, 0.05, 0.07};

		for(int i = 0; i < 5; i++)
		{
			matrix.setElement(rows[i], cols[i], vals[i]);
		}
	}
	else if(comm.rank == 1)
	{
		int rows[3] = {2, 3, 3};
		int cols[3] = {3, 3, 4};
		double vals[3] = {0.09, 0.06, 0.1};

		for(int i = 0; i < 3; i++)
		{
			matrix.setElement(rows[i], cols[i], vals[i]);
		}
	}
	else if(comm.rank == 2)
	{
		int rows[3] = {4, 4, 5};
		int cols[3] = {4, 5, 5};
		double vals[3] = {0.15, 0.23, 0.11};

		for(int i = 0; i < 3; i++)
		{
			matrix.setElement(rows[i], cols[i], vals[i]);
		}
	}
	else if(comm.rank == 3)
	{
		int rows[2] = {6, 7};
		int cols[2] = {6, 7};
		double vals[2] = {0.13, 0.09};

		for(int i = 0; i < 2; i++)
		{
			matrix.setElement(rows[i], cols[i], vals[i]);
		}
	}

	LinearSolverPETSc<cupcfd::data_structures::SparseMatrixCOO<int, double>, int, double> solver(comm, PETSC_KSP_CGAMG, 1E-6, 1E-6, matrix);

	// Setup
	solver.setupVectorB();

	// Check they are actually local rows
	PetscInt rangeCmp[5] = {0, 2, 4, 6, 8};
	BOOST_CHECK_EQUAL_COLLECTIONS(rangeCmp, rangeCmp + 5, solver.xRanges, solver.xRanges + 5);
	BOOST_CHECK_EQUAL_COLLECTIONS(rangeCmp, rangeCmp + 5, solver.bRanges, solver.bRanges + 5);
	BOOST_CHECK_EQUAL_COLLECTIONS(rangeCmp, rangeCmp + 5, solver.aRanges, solver.aRanges + 5);


	double * result;
	int nResult;

	status = solver.setValuesVectorB(0.0);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	if(comm.rank == 0)
	{
		int indices[4] = {0, 1, 2, 5};
		double vals[4] = {0.1, 0.2, 0.3, 0.6};

		status = solver.setValuesVectorB(vals, 4, indices, 4, 0);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

		status = solver.getValuesVectorB(&result, &nResult, indices, 4, 0);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
		BOOST_CHECK_EQUAL(nResult, 4);
		BOOST_CHECK_EQUAL_COLLECTIONS(result, result + 4, vals, vals + 4);
		//free(result);
	}
	else if(comm.rank == 1)
	{
		int indices[1] = {3};
		double vals[1] = {0.4};

		status = solver.setValuesVectorB(vals, 1, indices, 1, 0);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

		status = solver.getValuesVectorB(&result, &nResult, indices, 1, 0);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
		BOOST_CHECK_EQUAL(nResult, 1);
		BOOST_CHECK_EQUAL_COLLECTIONS(result, result + 1, vals, vals + 1);
		//free(result);
	}
	else if(comm.rank == 2)
	{
		int indices[2] = {4, 7};
		double vals[2] = {0.5, 0.8};

		status = solver.setValuesVectorB(vals, 2, indices, 2, 0);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

		status = solver.getValuesVectorB(&result, &nResult, indices, 2, 0);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
		BOOST_CHECK_EQUAL(nResult, 2);
		BOOST_CHECK_EQUAL_COLLECTIONS(result, result + 2, vals, vals + 2);
		//free(result);
	}
	else if(comm.rank == 3)
	{
		int indices[1] = {6};
		double vals[1] = {0.7};

		status = solver.setValuesVectorB(vals, 1, indices, 1, 0);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

		status = solver.getValuesVectorB(&result, &nResult, indices, 1, 0);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
		BOOST_CHECK_EQUAL(nResult, 1);
		BOOST_CHECK_EQUAL_COLLECTIONS(result, result + 1, vals, vals + 1);
		//free(result);
	}

	// Test all values set correctly too.
	double * fullResult;
	int nFullResult;
	double fullResultCmp[8] = {0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8};

	status = solver.getValuesVectorB(&fullResult, &nFullResult);
	BOOST_CHECK_EQUAL(nFullResult, 8);
	BOOST_CHECK_EQUAL_COLLECTIONS(fullResult, fullResult + 8, fullResultCmp, fullResultCmp + 8);
	free(fullResult);
}

// Test 6: Set values to specific indices + retrieve those indices (parallel) - off-node rows,
// using different bases for the indices
BOOST_AUTO_TEST_CASE(set_getValuesVectorB_test6)
{

}

// ============== setValuesMatrixA/getValuesMatrixA ===================

// Test 1: Set/Get all non-zero values from provided matrix - serial
BOOST_AUTO_TEST_CASE(set_getValuesMatrixA_test1)
{
	cupcfd::error::eCodes status;
	cupcfd::comm::Communicator comm;

	// Create a simple SparseMatrix
	cupcfd::data_structures::SparseMatrixCOO<int, double> matrix(8, 8, 0);

	int rows[13] = {0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 6, 7};
	int cols[13] = {0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 7};
	double vals[13] = {0.1, 0.2, 0.1, 0.05, 0.07, 0.09, 0.06, 0.1, 0.15, 0.23, 0.11, 0.13, 0.09};

	for(int i = 0; i < 13; i++)
	{
		matrix.setElement(rows[i], cols[i], vals[i]);
	}

	LinearSolverPETSc<cupcfd::data_structures::SparseMatrixCOO<int, double>, int, double> solver(comm, PETSC_KSP_CGAMG, 1E-6, 1E-6, matrix);

	// === Setup ===

	cupcfd::data_structures::SparseMatrixCOO<int, double> resultMatrix(8, 8, 0);

	int resultRowsCmp[64] = {0, 0, 0, 0, 0, 0, 0, 0,
						 1, 1, 1, 1, 1, 1, 1, 1,
						 2, 2, 2, 2, 2, 2, 2, 2,
						 3, 3, 3, 3, 3, 3, 3, 3,
						 4, 4, 4, 4, 4, 4, 4, 4,
						 5, 5, 5, 5, 5, 5, 5, 5,
						 6, 6, 6, 6, 6, 6, 6, 6,
						 7, 7, 7, 7, 7, 7, 7, 7};


	int resultColsCmp[64] = {0, 1, 2, 3, 4, 5, 6, 7,
							 0, 1, 2, 3, 4, 5, 6, 7,
							 0, 1, 2, 3, 4, 5, 6, 7,
							 0, 1, 2, 3, 4, 5, 6, 7,
							 0, 1, 2, 3, 4, 5, 6, 7,
							 0, 1, 2, 3, 4, 5, 6, 7,
							 0, 1, 2, 3, 4, 5, 6, 7,
							 0, 1, 2, 3, 4, 5, 6, 7};

	// Note we don't set all values of the resultMatrix, so we only retrieve specific elements from those set by matrix
	int resultRows[8] =    {0,   1,   2,    3,    4,    5,    6,    7};
	int resultCols[8] =    {0,   1,   2,    3,    4,    5,    6,    7};

	double resultValsCmp[64] = {0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
							 0.0, 0.1, 0.00, 0.0, 0.0, 0.0, 0.0, 0.0,
							 0.0, 0.0, 0.07, 0.0, 0.0, 0.0, 0.0, 0.0,
							 0.0, 0.0, 0.0, 0.06, 0.0, 0.0, 0.0, 0.0,
							 0.0, 0.0, 0.0, 0.0, 0.15, 0.0, 0.0, 0.0,
							 0.0, 0.0, 0.0, 0.0, 0.0, 0.11, 0.0, 0.0,
							 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.13, 0.0,
							 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.09};

	for(int i = 0; i < 8; i++)
	{
		// Set the values so they register as 'non-zero' for copies
		resultMatrix.setElement(resultRows[i], resultCols[i], 0.0);
	}

	// Setup the Matrix
	status = solver.setupMatrixA(matrix);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	// Test and Check
	// Set the values inside the matrix
	status = solver.setValuesMatrixA(matrix);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	// Retrieve values via the function and check they are what we expect
	status = solver.getValuesMatrixA(resultMatrix);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	for(int i = 0; i < 64; i++)
	{
		double val;
		resultMatrix.getElement(resultRowsCmp[i], resultColsCmp[i], &val);
		BOOST_CHECK_EQUAL(resultValsCmp[i], val);
	}
}

// Test 2: Set all non-zero values from provided matrix - parallel
BOOST_AUTO_TEST_CASE(set_getValuesMatrixA_test2)
{

}

// ============== clearVectorX ===================
// Test 1: Set all values to same scalar and then clear
BOOST_AUTO_TEST_CASE(clearVectorX_test1)
{
	cupcfd::error::eCodes status;
	cupcfd::comm::Communicator comm;

	// Create a simple SparseMatrix
	cupcfd::data_structures::SparseMatrixCOO<int, double> matrix(8, 8, 0);

	int rows[13] = {0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 6, 7};
	int cols[13] = {0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 7};
	double vals[13] = {0.1, 0.2, 0.1, 0.05, 0.07, 0.09, 0.06, 0.1, 0.15, 0.23, 0.11, 0.13, 0.09};

	for(int i = 0; i < 13; i++)
	{
		matrix.setElement(rows[i], cols[i], vals[i]);
	}

	LinearSolverPETSc<cupcfd::data_structures::SparseMatrixCOO<int, double>, int, double> solver(comm, PETSC_KSP_CGAMG, 1E-6, 1E-6, matrix);

	// Setup
	solver.setupVectorX();

	// Test and check
	status = solver.setValuesVectorX(2.5);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	double * result;
	int nResult;
	double resultCmp[8] = {2.5, 2.5, 2.5, 2.5, 2.5, 2.5, 2.5, 2.5};
	status = solver.getValuesVectorX(&result, &nResult);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	BOOST_CHECK_EQUAL(nResult, 8);
	BOOST_CHECK_EQUAL_COLLECTIONS(result, result + 8, resultCmp, resultCmp + 8);
	free(result);

	double result2Cmp[8] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
	status = solver.clearVectorX();
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	status = solver.getValuesVectorX(&result, &nResult);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	BOOST_CHECK_EQUAL_COLLECTIONS(result, result + 8, result2Cmp, result2Cmp + 8);
	free(result);
}

// Test 2: Set all values to same scalar + get all values (parallel)
BOOST_AUTO_TEST_CASE(clearVectorX_test2)
{
	cupcfd::error::eCodes status;
	cupcfd::comm::Communicator comm(MPI_COMM_WORLD);

	// Create a simple SparseMatrix
	cupcfd::data_structures::SparseMatrixCOO<int, double> matrix(8, 8, 0);

	if(comm.rank == 0)
	{
		int rows[5] = {0, 0, 1, 1, 2};
		int cols[5] = {0, 1, 1, 2, 2};
		double vals[5] = {0.1, 0.2, 0.1, 0.05, 0.07};

		for(int i = 0; i < 5; i++)
		{
			matrix.setElement(rows[i], cols[i], vals[i]);
		}
	}
	else if(comm.rank == 1)
	{
		int rows[3] = {2, 3, 3};
		int cols[3] = {3, 3, 4};
		double vals[3] = {0.09, 0.06, 0.1};

		for(int i = 0; i < 3; i++)
		{
			matrix.setElement(rows[i], cols[i], vals[i]);
		}
	}
	else if(comm.rank == 2)
	{
		int rows[3] = {4, 4, 5};
		int cols[3] = {4, 5, 5};
		double vals[3] = {0.15, 0.23, 0.11};

		for(int i = 0; i < 3; i++)
		{
			matrix.setElement(rows[i], cols[i], vals[i]);
		}
	}
	else if(comm.rank == 3)
	{
		int rows[2] = {6, 7};
		int cols[2] = {6, 7};
		double vals[2] = {0.13, 0.09};

		for(int i = 0; i < 2; i++)
		{
			matrix.setElement(rows[i], cols[i], vals[i]);
		}
	}

	LinearSolverPETSc<cupcfd::data_structures::SparseMatrixCOO<int, double>, int, double> solver(comm, PETSC_KSP_CGAMG, 1E-6, 1E-6, matrix);

	// Setup
	solver.setupVectorX();

	// Test and check
	status = solver.setValuesVectorX(2.5);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	double * result;
	int nResult;
	double resultCmp[8] = {2.5, 2.5, 2.5, 2.5, 2.5, 2.5, 2.5, 2.5};

	status = solver.getValuesVectorX(&result, &nResult);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	BOOST_CHECK_EQUAL(nResult, 8);
	BOOST_CHECK_EQUAL_COLLECTIONS(result, result + 8, resultCmp, resultCmp + 8);
}

// ============== clearVectorB ===================

// ============== clearMatrixA ===================

// ============== solve ===================
// Test 1: Test that the solve runs without error on a serial setup
BOOST_AUTO_TEST_CASE(solve_test1, * utf::tolerance(0.00001))
{
	cupcfd::error::eCodes status;
	cupcfd::comm::Communicator comm;

	// Create a simple SparseMatrix
	cupcfd::data_structures::SparseMatrixCOO<int, double> matrix(8, 8, 0);

	int rows[8] = {0, 1, 2, 3, 4, 5, 6, 7};
	int cols[8] = {0, 1, 2, 3, 4, 5, 6, 7};
	double vals[8] = {0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8};

	for(int i = 0; i < 8; i++)
	{
		matrix.setElement(rows[i], cols[i], vals[i]);
	}

	LinearSolverPETSc<cupcfd::data_structures::SparseMatrixCOO<int, double>, int, double> solver(comm, PETSC_KSP_CGAMG, 1E-6, 1E-6, matrix);

	// Set some suitable values for a very small test solve
	status = solver.setValuesMatrixA(matrix);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	status = solver.setValuesVectorB(0.1);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	// Clear the X Vector
	status = solver.clearVectorX();
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

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

	for(int i = 0; i < 8; i++)
	{
		BOOST_TEST(cmp[i] == vecX[i]);
	}
}

// Test 2: Test that the solve runs without error on a parallel setup
BOOST_AUTO_TEST_CASE(solve_test2, * utf::tolerance(0.00001))
{
	cupcfd::error::eCodes status;
	cupcfd::comm::Communicator comm(MPI_COMM_WORLD);

	// Create a simple SparseMatrix
	cupcfd::data_structures::SparseMatrixCOO<int, double> matrix(8, 8, 0);

	if(comm.rank == 0)
	{
		int rows[2] = {0, 1};
		int cols[2] = {0, 1};
		double vals[2] = {0.1, 0.2};

		for(int i = 0; i < 2; i++)
		{
			matrix.setElement(rows[i], cols[i], vals[i]);
		}
	}
	else if(comm.rank == 1)
	{
		int rows[2] = {2, 3};
		int cols[2] = {2, 3};
		double vals[2] = {0.3, 0.4};

		for(int i = 0; i < 2; i++)
		{
			matrix.setElement(rows[i], cols[i], vals[i]);
		}
	}
	else if(comm.rank == 2)
	{
		int rows[2] = {4, 5};
		int cols[2] = {4, 5};
		double vals[2] = {0.5, 0.6};

		for(int i = 0; i < 2; i++)
		{
			matrix.setElement(rows[i], cols[i], vals[i]);
		}
	}
	else if(comm.rank == 3)
	{
		int rows[2] = {6, 7};
		int cols[2] = {6, 7};
		double vals[2] = {0.7, 0.8};

		for(int i = 0; i < 2; i++)
		{
			matrix.setElement(rows[i], cols[i], vals[i]);
		}
	}

	LinearSolverPETSc<cupcfd::data_structures::SparseMatrixCOO<int, double>, int, double> solver(comm, PETSC_KSP_CGAMG, 1E-6, 1E-6, matrix);

	// Set some suitable values for a very small test solve
	status = solver.setValuesMatrixA(matrix);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	status = solver.setValuesVectorB(0.1);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	// Clear the X Vector
	status = solver.clearVectorX();
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

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

	for(int i = 0; i < 8; i++)
	{
		BOOST_TEST(cmp[i] == vecX[i]);
	}
}

// Test 3: Test that the solve runs without error on a parallel setup even if all values from one process
// (PETSc should handle behind scenes, even if slower...)
BOOST_AUTO_TEST_CASE(solve_test3, * utf::tolerance(0.00001))
{
	cupcfd::error::eCodes status;
	cupcfd::comm::Communicator comm(MPI_COMM_WORLD);

	// Create a simple SparseMatrix
	cupcfd::data_structures::SparseMatrixCOO<int, double> matrix(8, 8, 0);

	if(comm.rank == 0)
	{
		int rows[8] = {0, 1, 2, 3, 4, 5, 6, 7};
		int cols[8] = {0, 1, 2, 3, 4, 5, 6, 7};
		double vals[8] = {0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8};

		for(int i = 0; i < 8; i++)
		{
			matrix.setElement(rows[i], cols[i], vals[i]);
		}
	}

	LinearSolverPETSc<cupcfd::data_structures::SparseMatrixCOO<int, double>, int, double> solver(comm, PETSC_KSP_CGAMG, 1E-6, 1E-6, matrix);

	PetscInt rangeCmp[5] = {0, 2, 4, 6, 8};
	BOOST_CHECK_EQUAL_COLLECTIONS(rangeCmp, rangeCmp + 5, solver.aRanges, solver.aRanges + 5);
	BOOST_CHECK_EQUAL_COLLECTIONS(rangeCmp, rangeCmp + 5, solver.xRanges, solver.xRanges + 5);
	BOOST_CHECK_EQUAL_COLLECTIONS(rangeCmp, rangeCmp + 5, solver.bRanges, solver.bRanges + 5);

	// Set some suitable values for a very small test solve
	status = solver.setValuesMatrixA(matrix);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	status = solver.setValuesVectorB(0.1);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	// Clear the X Vector
	status = solver.clearVectorX();
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

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

	for(int i = 0; i < 8; i++)
	{
		BOOST_TEST(cmp[i] == vecX[i]);
	}
}

BOOST_AUTO_TEST_CASE(cleanup)
{
	PetscFinalize();
    MPI_Finalize();
}
