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

#define BOOST_TEST_MODULE LinearSolverPETScParallel
#include <boost/test/unit_test.hpp>
#include <boost/test/output_test_stream.hpp>
#include <stdexcept>

#include "LinearSolverPETScParallel.h"
#include "Error.h"
#include "SparseMatrixCOO.h"
#include <iostream>

using namespace cupcfd::linearsolvers;

// Setup
BOOST_AUTO_TEST_CASE(setup)
{
    int argc = boost::unit_test::framework::master_test_suite().argc;
    char ** argv = boost::unit_test::framework::master_test_suite().argv;

    MPI_Init(&argc, &argv);
	PetscInitialize(&argc, &argv, NULL, NULL);
}

// ============== Constructors ===================
// Test 1: Check values are probably defaulted to their null states
BOOST_AUTO_TEST_CASE(constructor_test1)
{
	LinearSolverPETScParallel<double> solver;

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

	cupcfd::mpi::Communicator comm;
	cupcfd::mpi::drivers::set_comm(comm, MPI_COMM_WORLD);

	LinearSolverPETScParallel<double> solver;

	// Since we are not using setup, manually set the petsc communicator for now
	cupcfd::mpi::drivers::set_comm(solver.comm, MPI_COMM_WORLD);

	if(comm.rank == 0)
	{
		// Manually set row size so it is valid
		solver.mLocal = 5;
		solver.mGlobal = 20;

		// Test and Check
		status = solver.setupVectorX();
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	}
	else if(comm.rank == 1)
	{
		// Manually set row size so it is valid
		solver.mLocal = 5;
		solver.mGlobal = 20;

		// Test and Check
		status = solver.setupVectorX();
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	}
	else if(comm.rank == 2)
	{
		// Manually set row size so it is valid
		solver.mLocal = 5;
		solver.mGlobal = 20;

		// Test and Check
		status = solver.setupVectorX();
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	}
	else if(comm.rank == 3)
	{
		// Manually set row size so it is valid
		solver.mLocal = 5;
		solver.mGlobal = 20;

		// Test and Check
		status = solver.setupVectorX();
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	}

	// Workaround - seems OK to compare these in C++,
	// but BOOST doesn't like making the comparison
	if(solver.x == PETSC_NULL)
	{
		BOOST_CHECK_EQUAL(true, false);
	}

	PetscInt cmp[5] = {0, 5, 10, 15, 20};
	BOOST_CHECK_EQUAL_COLLECTIONS(cmp, cmp + 5, solver.xRanges, solver.xRanges + 5);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
}

// Test 2: Check error if a suitable row size is not set - e.g. 0
BOOST_AUTO_TEST_CASE(setupVectorX_test2)
{
	cupcfd::error::eCodes status;

	cupcfd::mpi::Communicator comm;
	cupcfd::mpi::drivers::set_comm(comm, MPI_COMM_WORLD);

	LinearSolverPETScParallel<double> solver;

	// Since we are not using setup, manually set the petsc communicator for now
	cupcfd::mpi::drivers::set_comm(solver.comm, MPI_COMM_WORLD);

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

	cupcfd::mpi::Communicator comm;
	cupcfd::mpi::drivers::set_comm(comm, MPI_COMM_WORLD);

	LinearSolverPETScParallel<double> solver;

	// Since we are not using setup, manually set the petsc communicator for now
	cupcfd::mpi::drivers::set_comm(solver.comm, MPI_COMM_WORLD);

	//Setup

	// Manually set row size so it is valid
	solver.mLocal = 5;
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

	cupcfd::mpi::Communicator comm;
	cupcfd::mpi::drivers::set_comm(comm, MPI_COMM_WORLD);

	LinearSolverPETScParallel<double> solver;

	// Since we are not using setup, manually set the petsc communicator for now
	cupcfd::mpi::drivers::set_comm(solver.comm, MPI_COMM_WORLD);

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

	cupcfd::mpi::Communicator comm;
	cupcfd::mpi::drivers::set_comm(comm, MPI_COMM_WORLD);

	LinearSolverPETScParallel<double> solver;

	// Since we are not using setup, manually set the petsc communicator for now
	cupcfd::mpi::drivers::set_comm(solver.comm, MPI_COMM_WORLD);

	if(comm.rank == 0)
	{
		// Manually set row size so it is valid
		solver.mLocal = 5;
		solver.mGlobal = 20;

		// Test and Check
		status = solver.setupVectorB();
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	}
	else if(comm.rank == 1)
	{
		// Manually set row size so it is valid
		solver.mLocal = 5;
		solver.mGlobal = 20;

		// Test and Check
		status = solver.setupVectorB();
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	}
	else if(comm.rank == 2)
	{
		// Manually set row size so it is valid
		solver.mLocal = 5;
		solver.mGlobal = 20;

		// Test and Check
		status = solver.setupVectorB();
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	}
	else if(comm.rank == 3)
	{
		// Manually set row size so it is valid
		solver.mLocal = 5;
		solver.mGlobal = 20;

		// Test and Check
		status = solver.setupVectorB();
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	}

	// Workaround - seems OK to compare these in C++,
	// but BOOST doesn't like making the comparison
	if(solver.b == PETSC_NULL)
	{
		BOOST_CHECK_EQUAL(true, false);
	}

	PetscInt cmp[5] = {0, 5, 10, 15, 20};
	BOOST_CHECK_EQUAL_COLLECTIONS(cmp, cmp + 5, solver.bRanges, solver.bRanges + 5);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
}

// Test 2: Check error if a suitable row size is not set - e.g. 0
BOOST_AUTO_TEST_CASE(setupVectorB_test2)
{
	cupcfd::error::eCodes status;

	cupcfd::mpi::Communicator comm;
	cupcfd::mpi::drivers::set_comm(comm, MPI_COMM_WORLD);

	LinearSolverPETScParallel<double> solver;

	// Since we are not using setup, manually set the petsc communicator for now
	cupcfd::mpi::drivers::set_comm(solver.comm, MPI_COMM_WORLD);

	// Manually set row size so it is valid
	solver.mLocal = 0;
	solver.mGlobal = 0;

	// Test and Check
	status = solver.setupVectorB();

	BOOST_CHECK_EQUAL(status, cupcfd::error::E_LINEARSOLVER_ROW_SIZE_UNSET);
}

// Test 3: Check error if the communicator is not set to PETSC_COMM_SELF
// ToDo
BOOST_AUTO_TEST_CASE(setupVectorB_test3)
{

}

// ============== resetVectorB ===================
// Test 1: Test the successful reset of a existing vector
BOOST_AUTO_TEST_CASE(resetVectorB_test1)
{
	cupcfd::error::eCodes status;

	cupcfd::mpi::Communicator comm;
	cupcfd::mpi::drivers::set_comm(comm, MPI_COMM_WORLD);

	LinearSolverPETScParallel<double> solver;

	// Since we are not using setup, manually set the petsc communicator for now
	cupcfd::mpi::drivers::set_comm(solver.comm, MPI_COMM_WORLD);

	//Setup

	// Manually set row size so it is valid
	solver.mLocal = 5;
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

	cupcfd::mpi::Communicator comm;
	cupcfd::mpi::drivers::set_comm(comm, MPI_COMM_WORLD);

	LinearSolverPETScParallel<double> solver;

	// Since we are not using setup, manually set the petsc communicator for now
	cupcfd::mpi::drivers::set_comm(solver.comm, MPI_COMM_WORLD);

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
	cupcfd::mpi::Communicator comm;
	cupcfd::mpi::drivers::set_comm(comm, MPI_COMM_WORLD);

	LinearSolverPETScParallel<double> solver;

	// === Setup ===
	// Create a simple SparseMatrix, with data distributed across the four ranks
	cupcfd::data_structures::matrices::SparseMatrixCOO<int, double> matrix(8, 8, 0);

	int rows[13] = {0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 6, 7};
	int cols[13] = {0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 7};
	double vals[13] = {0.1, 0.2, 0.1, 0.05, 0.07, 0.09, 0.06, 0.1, 0.15, 0.23, 0.11, 0.13, 0.09};

	if(comm.rank == 0)
	{
		for(int i = 0; i < 4; i++)
		{
			matrix.setElement(rows[i], cols[i], vals[i]);
		}

		solver.mLocal = 2;
		solver.nLocal = 2;
	}
	else if(comm.rank == 1)
	{
		for(int i = 4; i < 8; i++)
		{
			matrix.setElement(rows[i], cols[i], vals[i]);
		}

		solver.mLocal = 2;
		solver.nLocal = 2;
	}
	else if(comm.rank == 2)
	{
		for(int i = 8; i < 10; i++)
		{
			matrix.setElement(rows[i], cols[i], vals[i]);
		}

		solver.mLocal = 2;
		solver.nLocal = 2;
	}
	else if(comm.rank == 3)
	{
		for(int i = 11; i < 13; i++)
		{
			matrix.setElement(rows[i], cols[i], vals[i]);
		}

		solver.mLocal = 2;
		solver.nLocal = 2;
	}

	solver.mGlobal = 8;
	solver.nGlobal = 8;

	// Test and Check
	status = solver.setupMatrixA(matrix);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	// ToDo: Technically, we let PETSc decide this so it could change.....
	// We're just assuming that 2 is a fair distribution
	//PetscInt cmp[5] = {0, 2, 4, 6, 8};
	//BOOST_CHECK_EQUAL_COLLECTIONS(cmp, cmp + 5, solver.aRanges, solver.aRanges + 5);

	// ToDo: Ideally would like to check PETSc internal nnz structure of matrix
}

// ============== resetMatrixA ===================
//Test 1: Check the matrix is correctly reset after being setup
BOOST_AUTO_TEST_CASE(resetMatrixA_test1)
{
	cupcfd::error::eCodes status;
	cupcfd::mpi::Communicator comm;
	cupcfd::mpi::drivers::set_comm(comm, MPI_COMM_WORLD);

	LinearSolverPETScParallel<double> solver;

	// === Setup ===
	// Create a simple SparseMatrix
	cupcfd::data_structures::matrices::SparseMatrixCOO<int, double> matrix(8, 8, 0);

	int rows[13] {0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 6, 7};
	int cols[13] = {0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 7};
	double vals[13] = {0.1, 0.2, 0.1, 0.05, 0.07, 0.09, 0.06, 0.1, 0.15, 0.23, 0.11, 0.13, 0.09};

	if(comm.rank == 0)
	{
		for(int i = 0; i < 4; i++)
		{
			matrix.setElement(rows[i], cols[i], vals[i]);
		}

		solver.mLocal = 2;
		solver.nLocal = 2;
	}
	else if(comm.rank == 1)
	{
		for(int i = 4; i < 8; i++)
		{
			matrix.setElement(rows[i], cols[i], vals[i]);
		}

		solver.mLocal = 2;
		solver.nLocal = 2;
	}
	else if(comm.rank == 2)
	{
		for(int i = 8; i < 10; i++)
		{
			matrix.setElement(rows[i], cols[i], vals[i]);
		}

		solver.mLocal = 2;
		solver.nLocal = 2;
	}
	else if(comm.rank == 3)
	{
		for(int i = 11; i < 13; i++)
		{
			matrix.setElement(rows[i], cols[i], vals[i]);
		}

		solver.mLocal = 2;
		solver.nLocal = 2;
	}

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
	cupcfd::mpi::Communicator comm;
	cupcfd::mpi::drivers::set_comm(comm, MPI_COMM_WORLD);

	LinearSolverPETScParallel<double> solver;

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
	cupcfd::mpi::Communicator comm;
	cupcfd::mpi::drivers::set_comm(comm, MPI_COMM_WORLD);

	LinearSolverPETScParallel<double> solver;

	// === Setup ===
	// Create a simple SparseMatrix
	cupcfd::data_structures::matrices::SparseMatrixCOO<int, double> matrix(8, 8, 0);

	int rows[13] {0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 6, 7};
	int cols[13] = {0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 7};
	double vals[13] = {0.1, 0.2, 0.1, 0.05, 0.07, 0.09, 0.06, 0.1, 0.15, 0.23, 0.11, 0.13, 0.09};

	if(comm.rank == 0)
	{
		for(int i = 0; i < 4; i++)
		{
			matrix.setElement(rows[i], cols[i], vals[i]);
		}
	}
	else if(comm.rank == 1)
	{
		for(int i = 4; i < 8; i++)
		{
			matrix.setElement(rows[i], cols[i], vals[i]);
		}
	}
	else if(comm.rank == 2)
	{
		for(int i = 8; i < 10; i++)
		{
			matrix.setElement(rows[i], cols[i], vals[i]);
		}
	}
	else if(comm.rank == 3)
	{
		for(int i = 11; i < 13; i++)
		{
			matrix.setElement(rows[i], cols[i], vals[i]);
		}
	}

	status = solver.setup(matrix);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
}

// ============== reset ===========
// Test 1: Test a successful reset after the object has been setup
BOOST_AUTO_TEST_CASE(reset_test1)
{
	cupcfd::error::eCodes status;
	cupcfd::mpi::Communicator comm;
	cupcfd::mpi::drivers::set_comm(comm, MPI_COMM_WORLD);

	LinearSolverPETScParallel<double> solver;

	// === Setup ===
	// Create a simple SparseMatrix
	cupcfd::data_structures::matrices::SparseMatrixCOO<int, double> matrix(8, 8, 0);

	int rows[13] = {0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 6, 7};
	int cols[13] = {0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 7};
	double vals[13] = {0.1, 0.2, 0.1, 0.05, 0.07, 0.09, 0.06, 0.1, 0.15, 0.23, 0.11, 0.13, 0.09};

	if(comm.rank == 0)
	{
		for(int i = 0; i < 4; i++)
		{
			matrix.setElement(rows[i], cols[i], vals[i]);
		}
	}
	else if(comm.rank == 1)
	{
		for(int i = 4; i < 8; i++)
		{
			matrix.setElement(rows[i], cols[i], vals[i]);
		}
	}
	else if(comm.rank == 2)
	{
		for(int i = 8; i < 10; i++)
		{
			matrix.setElement(rows[i], cols[i], vals[i]);
		}
	}
	else if(comm.rank == 3)
	{
		for(int i = 11; i < 13; i++)
		{
			matrix.setElement(rows[i], cols[i], vals[i]);
		}
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
	cupcfd::mpi::Communicator comm;
	cupcfd::mpi::drivers::set_comm(comm, MPI_COMM_WORLD);

	LinearSolverPETScParallel<double> solver;

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
	cupcfd::mpi::Communicator comm;
	cupcfd::mpi::drivers::set_comm(comm, MPI_COMM_WORLD);

	LinearSolverPETScParallel<double> solver;

	// === Setup ===

	// Set the row and col sizes
	solver.mLocal = 2;
	solver.nLocal = 2;
	solver.mGlobal = 8;
	solver.nGlobal = 8;
	solver.comm = comm;

	// Create Vector
	status = solver.setupVectorX();
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	// Test and Check
	status = solver.setValuesVectorX(2.4);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	// Any position should hold the value 2.4
	if(comm.rank == 2)
	{
		PetscInt pos = 5;
		PetscScalar result;
		VecGetValues(solver.x, 1, &pos, &result);
		BOOST_CHECK_EQUAL(result, 2.4);
	}

	// Second check in different position to ensure more than one was updated
	if(comm.rank == 3)
	{
		PetscInt pos = 7;
		PetscScalar result = 0.0;
		VecGetValues(solver.x, 1, &pos, &result);
		BOOST_CHECK_EQUAL(result, 2.4);
	}
}

// Test 2: Check for error when vector does not exist
BOOST_AUTO_TEST_CASE(setValuesVectorX_scalar_test2)
{
	cupcfd::error::eCodes status;

	LinearSolverPETScParallel<double> solver;

	// Test and Check
	status = solver.setValuesVectorX(2.4);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_LINEARSOLVER_INVALID_VECTOR);
}

// ============== setValuesVectorX(T * scalars, int nScalars) ===========
// Test 1: Test setting values at specific locations, across different processors
BOOST_AUTO_TEST_CASE(setValuesVectorX_indexedscalars_test1)
{
	cupcfd::error::eCodes status;
	cupcfd::mpi::Communicator comm;
	cupcfd::mpi::drivers::set_comm(comm, MPI_COMM_WORLD);

	LinearSolverPETScParallel<double> solver;

	// === Setup ===

	// Set the row and col sizes
	solver.mLocal = 2;
	solver.nLocal = 2;
	solver.mGlobal = 8;
	solver.nGlobal = 8;
	solver.comm = comm;

	// Create Vector
	status = solver.setupVectorX();
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	// Test and Check
	int indices[4] = {0, 3, 4, 7};
	double values[4] = {0.5, 0.7, 0.2, 0.3};

	if(comm.rank == 0)
	{
		status = solver.setValuesVectorX(values, 1, indices, 1, 0);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	}
	else if(comm.rank == 1)
	{
		status = solver.setValuesVectorX(values + 1, 1, indices + 1, 1, 0);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	}
	else if(comm.rank == 2)
	{
		status = solver.setValuesVectorX(values + 2, 1, indices + 2, 1, 0);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	}
	else if(comm.rank == 3)
	{
		status = solver.setValuesVectorX(values + 3, 1, indices + 3, 1, 0);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	}

	// Assuming each process is allocated two rows each here...
	if(comm.rank == 0)
	{
		PetscInt pos[2] = {0, 1};
		PetscScalar result[2];
		VecGetValues(solver.x, 2, pos, result);

		PetscScalar cmp[2] = {0.5, 0.0};
		BOOST_CHECK_EQUAL_COLLECTIONS(cmp, cmp + 2, result, result + 2);
	}
	else if(comm.rank == 1)
	{
		PetscInt pos[2] = {2, 3};
		PetscScalar result[2];
		VecGetValues(solver.x, 2, pos, result);

		PetscScalar cmp[2] = {0.0, 0.7};
		BOOST_CHECK_EQUAL_COLLECTIONS(cmp, cmp + 2, result, result + 2);
	}
	else if(comm.rank == 2)
	{
		PetscInt pos[2] = {4, 5};
		PetscScalar result[2];
		VecGetValues(solver.x, 2, pos, result);

		PetscScalar cmp[2] = {0.2, 0.0};
		BOOST_CHECK_EQUAL_COLLECTIONS(cmp, cmp + 2, result, result + 2);
	}
	else if(comm.rank == 3)
	{
		PetscInt pos[2] = {6, 7};
		PetscScalar result[2];
		VecGetValues(solver.x, 2, pos, result);

		PetscScalar cmp[2] = {0.0, 0.3};
		BOOST_CHECK_EQUAL_COLLECTIONS(cmp, cmp + 2, result, result + 2);
	}
}

// ============== setValuesVectorB(T scalar) ===========
// Test 1: Test setting the entire vector to a specific scalar
// Ideally want to check value using getter, but will
// use PETSc function directly to keep function tests separate
BOOST_AUTO_TEST_CASE(setValuesVectorB_scalar_test1)
{
	cupcfd::error::eCodes status;
	cupcfd::mpi::Communicator comm;
	cupcfd::mpi::drivers::set_comm(comm, MPI_COMM_WORLD);

	LinearSolverPETScParallel<double> solver;

	// === Setup ===

	// Set the row and col sizes
	solver.mLocal = 2;
	solver.nLocal = 2;
	solver.mGlobal = 8;
	solver.nGlobal = 8;
	solver.comm = comm;

	// Create Vector
	status = solver.setupVectorB();
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	// Test and Check
	status = solver.setValuesVectorB(2.4);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	// Any position should hold the value 2.4
	if(comm.rank == 2)
	{
		PetscInt pos = 5;
		PetscScalar result;
		VecGetValues(solver.b, 1, &pos, &result);
		BOOST_CHECK_EQUAL(result, 2.4);
	}

	// Second check in different position to ensure more than one was updated
	if(comm.rank == 3)
	{
		PetscInt pos = 7;
		PetscScalar result = 0.0;
		VecGetValues(solver.b, 1, &pos, &result);
		BOOST_CHECK_EQUAL(result, 2.4);
	}
}

// Test 2: Check for error when vector does not exist
BOOST_AUTO_TEST_CASE(setValuesVectorB_scalar_test2)
{
	cupcfd::error::eCodes status;

	LinearSolverPETScParallel<double> solver;

	// Test and Check
	status = solver.setValuesVectorB(2.4);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_LINEARSOLVER_INVALID_VECTOR);
}


// ============== setValuesVectorB(T * scalars, int nScalars) ===========
// Test 1: Test setting values at specific locations, across different processors
BOOST_AUTO_TEST_CASE(setValuesVectorB_indexedscalars_test1)
{
	cupcfd::error::eCodes status;
	cupcfd::mpi::Communicator comm;
	cupcfd::mpi::drivers::set_comm(comm, MPI_COMM_WORLD);

	LinearSolverPETScParallel<double> solver;

	// === Setup ===

	// Set the row and col sizes
	solver.mLocal = 2;
	solver.nLocal = 2;
	solver.mGlobal = 8;
	solver.nGlobal = 8;
	solver.comm = comm;

	// Create Vector
	status = solver.setupVectorB();
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	// Test and Check
	int indices[4] = {0, 3, 4, 7};
	double values[4] = {0.5, 0.7, 0.2, 0.3};

	if(comm.rank == 0)
	{
		status = solver.setValuesVectorB(values, 1, indices, 1, 0);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	}
	else if(comm.rank == 1)
	{
		status = solver.setValuesVectorB(values + 1, 1, indices + 1, 1, 0);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	}
	else if(comm.rank == 2)
	{
		status = solver.setValuesVectorB(values + 2, 1, indices + 2, 1, 0);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	}
	else if(comm.rank == 3)
	{
		status = solver.setValuesVectorB(values + 3, 1, indices + 3, 1, 0);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	}

	// Assuming each process is allocated two rows each here...
	if(comm.rank == 0)
	{
		PetscInt pos[2] = {0, 1};
		PetscScalar result[2];
		VecGetValues(solver.b, 2, pos, result);

		PetscScalar cmp[2] = {0.5, 0.0};
		BOOST_CHECK_EQUAL_COLLECTIONS(cmp, cmp + 2, result, result + 2);
	}
	else if(comm.rank == 1)
	{
		PetscInt pos[2] = {2, 3};
		PetscScalar result[2];
		VecGetValues(solver.b, 2, pos, result);

		PetscScalar cmp[2] = {0.0, 0.7};
		BOOST_CHECK_EQUAL_COLLECTIONS(cmp, cmp + 2, result, result + 2);
	}
	else if(comm.rank == 2)
	{
		PetscInt pos[2] = {4, 5};
		PetscScalar result[2];
		VecGetValues(solver.b, 2, pos, result);

		PetscScalar cmp[2] = {0.2, 0.0};
		BOOST_CHECK_EQUAL_COLLECTIONS(cmp, cmp + 2, result, result + 2);
	}
	else if(comm.rank == 3)
	{
		PetscInt pos[2] = {6, 7};
		PetscScalar result[2];
		VecGetValues(solver.b, 2, pos, result);

		PetscScalar cmp[2] = {0.0, 0.3};
		BOOST_CHECK_EQUAL_COLLECTIONS(cmp, cmp + 2, result, result + 2);
	}
}

// ============== setValuesMatrixA ===========
// Test 1: Test the successful setup of the object from a matrix
BOOST_AUTO_TEST_CASE(setValuesMatrixA_test1)
{
	cupcfd::error::eCodes status;
	cupcfd::mpi::Communicator comm;
	cupcfd::mpi::drivers::set_comm(comm, MPI_COMM_WORLD);

	LinearSolverPETScParallel<double> solver;

	// === Setup ===
	// Create a simple SparseMatrix
	cupcfd::data_structures::matrices::SparseMatrixCOO<int, double> matrix(8, 8, 0);

	int rows[13] {0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 6, 7};
	int cols[13] = {0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 7};
	double vals[13] = {0.1, 0.2, 0.1, 0.05, 0.07, 0.09, 0.06, 0.1, 0.15, 0.23, 0.11, 0.13, 0.09};

	if(comm.rank == 0)
	{
		for(int i = 0; i < 4; i++)
		{
			matrix.setElement(rows[i], cols[i], vals[i]);
		}
	}
	else if(comm.rank == 1)
	{
		for(int i = 4; i < 8; i++)
		{
			matrix.setElement(rows[i], cols[i], vals[i]);
		}
	}
	else if(comm.rank == 2)
	{
		for(int i = 8; i < 10; i++)
		{
			matrix.setElement(rows[i], cols[i], vals[i]);
		}
	}
	else if(comm.rank == 3)
	{
		for(int i = 11; i < 13; i++)
		{
			matrix.setElement(rows[i], cols[i], vals[i]);
		}
	}

	status = solver.setup(matrix);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	status = solver.setValuesMatrixA(matrix);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	// ToDo: Best way to check contents of matrix in PETSc?
	// Could get local matrix contents for each rank
}



// ============== getValuesVectorX ===========
BOOST_AUTO_TEST_CASE(getValuesVectorX_test1)
{
	cupcfd::error::eCodes status;
	cupcfd::mpi::Communicator comm;
	cupcfd::mpi::drivers::set_comm(comm, MPI_COMM_WORLD);

	LinearSolverPETScParallel<double> solver;

	// === Setup ===

	// Set the row and col sizes
	solver.mLocal = 2;
	solver.nLocal = 2;
	solver.mGlobal = 8;
	solver.nGlobal = 8;
	solver.comm = comm;

	// Create Vector
	status = solver.setupVectorX();
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	// Test and Check
	int indices[4] = {0, 3, 4, 7};
	double values[4] = {0.5, 0.7, 0.2, 0.3};

	if(comm.rank == 0)
	{
		status = solver.setValuesVectorX(values, 1, indices, 1, 0);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	}
	else if(comm.rank == 1)
	{
		status = solver.setValuesVectorX(values + 1, 1, indices + 1, 1, 0);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	}
	else if(comm.rank == 2)
	{
		status = solver.setValuesVectorX(values + 2, 1, indices + 2, 1, 0);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	}
	else if(comm.rank == 3)
	{
		status = solver.setValuesVectorX(values + 3, 1, indices + 3, 1, 0);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	}

	// Test and Check
	// Everyone get's a copy of the entire vector
	double * result;
	int nResult;

	status = solver.getValuesVectorX(&result, &nResult);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	BOOST_CHECK_EQUAL(nResult, 8);

	// Assuming each process is allocated two rows each here...
	if(comm.rank == 0)
	{
		PetscScalar cmp[8] = {0.5, 0.0, 0.0, 0.7,0.2, 0.0, 0.0, 0.3};
		BOOST_CHECK_EQUAL_COLLECTIONS(cmp, cmp + 8, result, result + 8);
	}
	else if(comm.rank == 1)
	{
		PetscScalar cmp[8] = {0.5, 0.0, 0.0, 0.7,0.2, 0.0, 0.0, 0.3};
		BOOST_CHECK_EQUAL_COLLECTIONS(cmp, cmp + 8, result, result + 8);
	}
	else if(comm.rank == 2)
	{
		PetscScalar cmp[8] = {0.5, 0.0, 0.0, 0.7,0.2, 0.0, 0.0, 0.3};
		BOOST_CHECK_EQUAL_COLLECTIONS(cmp, cmp + 8, result, result + 8);
	}
	else if(comm.rank == 3)
	{
		PetscScalar cmp[8] = {0.5, 0.0, 0.0, 0.7,0.2, 0.0, 0.0, 0.3};
		BOOST_CHECK_EQUAL_COLLECTIONS(cmp, cmp + 8, result, result + 8);
	}
}


// ============== getValuesVectorXIndexes ===========
// ToDo

// ============== getValuesVectorB ===========
BOOST_AUTO_TEST_CASE(getValuesVectorB_test1)
{
	cupcfd::error::eCodes status;
	cupcfd::mpi::Communicator comm;
	cupcfd::mpi::drivers::set_comm(comm, MPI_COMM_WORLD);

	LinearSolverPETScParallel<double> solver;

	// === Setup ===

	// Set the row and col sizes
	solver.mLocal = 2;
	solver.nLocal = 2;
	solver.mGlobal = 8;
	solver.nGlobal = 8;
	solver.comm = comm;

	// Create Vector
	status = solver.setupVectorB();
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	// Test and Check
	int indices[4] = {0, 3, 4, 7};
	double values[4] = {0.5, 0.7, 0.2, 0.3};

	if(comm.rank == 0)
	{
		status = solver.setValuesVectorB(values, 1, indices, 1, 0);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	}
	else if(comm.rank == 1)
	{
		status = solver.setValuesVectorB(values + 1, 1, indices + 1, 1, 0);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	}
	else if(comm.rank == 2)
	{
		status = solver.setValuesVectorB(values + 2, 1, indices + 2, 1, 0);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	}
	else if(comm.rank == 3)
	{
		status = solver.setValuesVectorB(values + 3, 1, indices + 3, 1, 0);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	}

	// Test and Check
	// Everyone get's a copy of the entire vector
	double * result;
	int nResult;

	status = solver.getValuesVectorB(&result, &nResult);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	BOOST_CHECK_EQUAL(nResult, 8);

	// Assuming each process is allocated two rows each here...
	if(comm.rank == 0)
	{
		PetscScalar cmp[8] = {0.5, 0.0, 0.0, 0.7,0.2, 0.0, 0.0, 0.3};
		BOOST_CHECK_EQUAL_COLLECTIONS(cmp, cmp + 8, result, result + 8);
	}
	else if(comm.rank == 1)
	{
		PetscScalar cmp[8] = {0.5, 0.0, 0.0, 0.7,0.2, 0.0, 0.0, 0.3};
		BOOST_CHECK_EQUAL_COLLECTIONS(cmp, cmp + 8, result, result + 8);
	}
	else if(comm.rank == 2)
	{
		PetscScalar cmp[8] = {0.5, 0.0, 0.0, 0.7,0.2, 0.0, 0.0, 0.3};
		BOOST_CHECK_EQUAL_COLLECTIONS(cmp, cmp + 8, result, result + 8);
	}
	else if(comm.rank == 3)
	{
		PetscScalar cmp[8] = {0.5, 0.0, 0.0, 0.7,0.2, 0.0, 0.0, 0.3};
		BOOST_CHECK_EQUAL_COLLECTIONS(cmp, cmp + 8, result, result + 8);
	}
}

// ============== getValuesVectorBIndexes ===========
// ToDo

// ============== getValuesMatrixA ===========
// ToDo

// ============== clearVectorX ===========
// ToDo

// ============== clearVectorB ===========
// ToDo

// ============== clearMatrixA ===========
BOOST_AUTO_TEST_CASE(clearMatrixA_test1)
{
	// ToDo
}

// ============== resetSolverSelection ===========
BOOST_AUTO_TEST_CASE(resetSolverSelection_test1)
{
	// ToDo
}

// ============== setSolverSelection ===========
BOOST_AUTO_TEST_CASE(setSolverSelection_test1)
{
	// ToDo
}

// ============== resetTolerances ===========
BOOST_AUTO_TEST_CASE(resetTolerances_test1)
{
	// ToDo
}

// ============== resetPETScSolver ===========
// Test 1: Test that we can reset when the KSP solver is not setup
BOOST_AUTO_TEST_CASE(resetPETScSolver_test1)
{
	// ToDo
}

// Test 2: Test that we can reset when the KSP Solver is setup
BOOST_AUTO_TEST_CASE(resetPETScSolver_test2)
{
	// ToDo
}

// ============== setupPETScSolver ===========
BOOST_AUTO_TEST_CASE(setupPETScSolver_test1)
{
	cupcfd::error::eCodes status;
	cupcfd::mpi::Communicator comm;
	cupcfd::mpi::drivers::set_comm(comm, MPI_COMM_WORLD);

	LinearSolverPETScParallel<double> solver;

	// === Setup ===
	// Create a simple SparseMatrix
	cupcfd::data_structures::matrices::SparseMatrixCOO<int, double> matrix(8, 8, 0);

	int rows[13] {0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 6, 7};
	int cols[13] = {0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 7};
	double vals[13] = {0.1, 0.2, 0.1, 0.05, 0.07, 0.09, 0.06, 0.1, 0.15, 0.23, 0.11, 0.13, 0.09};

	if(comm.rank == 0)
	{
		for(int i = 0; i < 4; i++)
		{
			matrix.setElement(rows[i], cols[i], vals[i]);
		}

		solver.mLocal = 2;
		solver.nLocal = 2;
	}
	else if(comm.rank == 1)
	{
		for(int i = 4; i < 8; i++)
		{
			matrix.setElement(rows[i], cols[i], vals[i]);
		}

		solver.mLocal = 2;
		solver.nLocal = 2;
	}
	else if(comm.rank == 2)
	{
		for(int i = 8; i < 10; i++)
		{
			matrix.setElement(rows[i], cols[i], vals[i]);
		}

		solver.mLocal = 2;
		solver.nLocal = 2;
	}
	else if(comm.rank == 3)
	{
		for(int i = 11; i < 13; i++)
		{
			matrix.setElement(rows[i], cols[i], vals[i]);
		}

		solver.mLocal = 2;
		solver.nLocal = 2;
	}

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
	cupcfd::mpi::Communicator comm;
	cupcfd::mpi::drivers::set_comm(comm, MPI_COMM_WORLD);

	LinearSolverPETScParallel<double> solver;

	// === Setup ===
	// Create a simple SparseMatrix
	cupcfd::data_structures::matrices::SparseMatrixCOO<int, double> matrix(8, 8, 0);

	int rows[8] = {0, 1, 2, 3, 4, 5, 6, 7};
	int cols[8] = {0, 1, 2, 3, 4, 5, 6, 7};
	double vals[8] = {0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8};

	if(comm.rank == 0)
	{
		for(int i = 0; i < 2; i++)
		{
			matrix.setElement(rows[i], cols[i], vals[i]);
		}
	}
	else if(comm.rank == 1)
	{
		for(int i = 2; i < 4; i++)
		{
			matrix.setElement(rows[i], cols[i], vals[i]);
		}
	}
	else if(comm.rank == 2)
	{
		for(int i = 4; i < 6; i++)
		{
			matrix.setElement(rows[i], cols[i], vals[i]);
		}
	}
	else if(comm.rank == 3)
	{
		for(int i = 6; i < 8; i++)
		{
			matrix.setElement(rows[i], cols[i], vals[i]);
		}
	}

	// Setup the data structures of the solver
	status = solver.setup(matrix);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	// Set some suitable values for a very small test solve
	status = solver.setValuesMatrixA(matrix);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

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
    MPI_Finalize();
}
