/*
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 */

#define BOOST_TEST_MODULE SparseMatrixHDF5File
#include <boost/test/unit_test.hpp>
#include <boost/test/output_test_stream.hpp>
#include "mpi.h"

#include "SparseMatrixHDF5File.h"
#include "Error.h"
#include "Communicator.h"
#include "SparseMatrixCOO.h"

using namespace cupcfd::fileformats::matrices;

// These tests require MPI
BOOST_AUTO_TEST_CASE(setup)
{
    int argc = boost::unit_test::framework::master_test_suite().argc;
    char ** argv = boost::unit_test::framework::master_test_suite().argv;
    MPI_Init(&argc, &argv);
}

// === Constructor ===
// Test 1:
BOOST_AUTO_TEST_CASE(constructor_test1)
{
	cupcfd::mpi::Communicator comm;
	cupcfd::mpi::drivers::set_comm(comm, MPI_COMM_WORLD);

	if(comm.rank == 0)
	{
		std::string fileName = "../tests/testdata/Matrix1SparseCOO.h5";
		SparseMatrixHDF5File<int ,double> file(fileName);
	}
}

// === getNNZ ===
// Test 1:
BOOST_AUTO_TEST_CASE(getNNZ_test1)
{
	cupcfd::mpi::Communicator comm;
	cupcfd::mpi::drivers::set_comm(comm, MPI_COMM_WORLD);
	cupcfd::error::eCodes status;

	if(comm.rank == 0)
	{
		std::string fileName = "../tests/testdata/Matrix1SparseCOO.h5";
		SparseMatrixHDF5File<int, double> file(fileName);

		int nnz;
		status = file.getNNZ(&nnz);
		BOOST_CHECK_EQUAL(nnz, 24);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	}
}

// === getNRows ===
// Test 1:
BOOST_AUTO_TEST_CASE(getNRows_test1)
{
	cupcfd::mpi::Communicator comm;
	cupcfd::mpi::drivers::set_comm(comm, MPI_COMM_WORLD);
	cupcfd::error::eCodes status;

	if(comm.rank == 0)
	{
		std::string fileName = "../tests/testdata/Matrix1SparseCOO.h5";
		SparseMatrixHDF5File<int, double> file(fileName);

		int nRows;
		status = file.getNRows(&nRows);
		BOOST_CHECK_EQUAL(nRows, 8);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	}
}

// === getNCols ===
// Test 1:
BOOST_AUTO_TEST_CASE(getNCols_test1)
{
	cupcfd::mpi::Communicator comm;
	cupcfd::mpi::drivers::set_comm(comm, MPI_COMM_WORLD);
	cupcfd::error::eCodes status;

	if(comm.rank == 0)
	{
		std::string fileName = "../tests/testdata/Matrix1SparseCOO.h5";
		SparseMatrixHDF5File<int, double> file(fileName);

		int nCols;
		status = file.getNCols(&nCols);
		BOOST_CHECK_EQUAL(nCols, 8);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	}
}

// === getMatrixIndicesBase ===
// Test 1:
BOOST_AUTO_TEST_CASE(getMatrixIndicesBase_test1)
{
	cupcfd::mpi::Communicator comm;
	cupcfd::mpi::drivers::set_comm(comm, MPI_COMM_WORLD);
	cupcfd::error::eCodes status;

	if(comm.rank == 0)
	{
		std::string fileName = "../tests/testdata/Matrix1SparseCOO.h5";
		SparseMatrixHDF5File<int, double> file(fileName);

		int base;
		status = file.getMatrixIndicesBase(&base);
		BOOST_CHECK_EQUAL(base, 1);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	}
}

// === getNNZRows ===
// Test 1:
BOOST_AUTO_TEST_CASE(getNNZRows_test1)
{
	cupcfd::mpi::Communicator comm;
	cupcfd::mpi::drivers::set_comm(comm, MPI_COMM_WORLD);
	cupcfd::error::eCodes status;

	if(comm.rank == 0)
	{
		std::string fileName = "../tests/testdata/Matrix1SparseCOO.h5";
		SparseMatrixHDF5File<int, double> file(fileName);

		// This test file is known in advance to have 8 rows
		int * nnzRows = (int *) malloc(sizeof(int) * 8);
		status = file.getNNZRows(nnzRows, 8);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

		int cmp[8] = {3, 3, 3, 3, 3, 3, 4, 2};
		BOOST_CHECK_EQUAL_COLLECTIONS(cmp, cmp + 8, nnzRows, nnzRows + 8);
	}
}

// === getSparseMatrix (All) ===
// Test 1: Test we can read into a matrix with the correct values for a base 0 Sparsematrix object
BOOST_AUTO_TEST_CASE(getSparseMatrix_full_test1)
{
	cupcfd::mpi::Communicator comm;
	cupcfd::mpi::drivers::set_comm(comm, MPI_COMM_WORLD);
	cupcfd::error::eCodes status;

	if(comm.rank == 0)
	{
		std::string fileName = "../tests/testdata/Matrix1SparseCOO.h5";
		SparseMatrixHDF5File<int, double> file(fileName);

		cupcfd::data_structures::matrices::SparseMatrixCOO<int, double> matrix(8, 8, 0);

		// Test and Check
		status = file.getSparseMatrix(matrix);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

		// Should probably use the matrix functions for this test, but can just check on
		// internal data structures for now
		int rowCmp[24] = {0, 0, 0, 1, 1, 1, 2, 2, 2, 3, 3, 3, 4, 4, 4, 5, 5, 5, 6, 6, 6, 6, 7, 7};
		int colCmp[24] = {0, 1, 2, 1, 2, 3, 2, 3, 4, 3, 4, 5, 4, 5, 6, 5, 6, 7, 4, 5, 6, 7, 6, 7};
		double valCmp[24] = {0.1, 0.2, 0.3, 0.4, 0.1, 0.2, 0.3, 0.4, 0.1, 0.2, 0.3, 0.4, 0.1, 0.2, 0.3, 0.4, 0.1, 0.2, 0.3, 0.4, 0.1, 0.2, 0.3, 0.4};

		BOOST_CHECK_EQUAL_COLLECTIONS(rowCmp, rowCmp + 24, &matrix.row[0], &matrix.row[0] + 24);
		BOOST_CHECK_EQUAL_COLLECTIONS(colCmp, colCmp + 24, &matrix.col[0], &matrix.col[0] + 24);
		BOOST_CHECK_EQUAL_COLLECTIONS(valCmp, valCmp + 24, &matrix.val[0], &matrix.val[0] + 24);
	}
}

// Test 1: Test we can read into a matrix with the correct values for a base 4 SparseMatrix object
BOOST_AUTO_TEST_CASE(getSparseMatrix_full_test2)
{
	cupcfd::mpi::Communicator comm;
	cupcfd::mpi::drivers::set_comm(comm, MPI_COMM_WORLD);
	cupcfd::error::eCodes status;

	if(comm.rank == 0)
	{
		std::string fileName = "../tests/testdata/Matrix1SparseCOO.h5";
		SparseMatrixHDF5File<int, double> file(fileName);

		cupcfd::data_structures::matrices::SparseMatrixCOO<int, double> matrix(8, 8, 4);

		// Test and Check
		status = file.getSparseMatrix(matrix);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

		// Should probably use the matrix functions for this test, but can just check on
		// internal data structures for now
		int rowCmp[24] = {4, 4, 4, 5, 5, 5, 6, 6, 6, 7, 7, 7, 8, 8, 8, 9, 9, 9, 10, 10, 10, 10, 11, 11};
		int colCmp[24] = {4, 5, 6, 5, 6, 7, 6, 7, 8, 7, 8, 9, 8, 9, 10, 9, 10, 11, 8, 9, 10, 11, 10, 11};
		double valCmp[24] = {0.1, 0.2, 0.3, 0.4, 0.1, 0.2, 0.3, 0.4, 0.1, 0.2, 0.3, 0.4, 0.1, 0.2, 0.3, 0.4, 0.1, 0.2, 0.3, 0.4, 0.1, 0.2, 0.3, 0.4};

		BOOST_CHECK_EQUAL_COLLECTIONS(rowCmp, rowCmp + 24, &matrix.row[0], &matrix.row[0] + 24);
		BOOST_CHECK_EQUAL_COLLECTIONS(colCmp, colCmp + 24, &matrix.col[0], &matrix.col[0] + 24);
		BOOST_CHECK_EQUAL_COLLECTIONS(valCmp, valCmp + 24, &matrix.val[0], &matrix.val[0] + 24);
	}
}

// === getSparseMatrix (Partial) ===
//ToDo: The multile pranges aspect has the potential for a lot of edge cases.
// This will probably need more expansive testing - this one test tries too many things at once
// Test 1:
BOOST_AUTO_TEST_CASE(getSparseMatrix_partial_test1)
{
	cupcfd::mpi::Communicator comm;
	cupcfd::mpi::drivers::set_comm(comm, MPI_COMM_WORLD);
	cupcfd::error::eCodes status;

	if(comm.rank == 0)
	{
		// === Setup ===
		std::string fileName = "../tests/testdata/Matrix1SparseCOO.h5";
		SparseMatrixHDF5File<int, double> file(fileName);
		cupcfd::data_structures::matrices::SparseMatrixCOO<int, double> matrix(8, 8, 4);

		// Speciy which rows we want
		int desiredRows[4] = {2, 3, 5, 7};

		// === Test and Check ===
		status = file.getSparseMatrix(matrix, desiredRows, 4, 1);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

		// Should probably use the matrix functions for this test, but can just check on
		// internal data structures for now
		int rowCmp[13] = {5, 5, 5, 6, 6, 6, 8, 8, 8, 10, 10, 10, 10};
		int colCmp[13] = {5, 6, 7, 6, 7, 8, 8, 9, 10, 8, 9, 10, 11};
		double valCmp[13] = {0.4, 0.1, 0.2, 0.3, 0.4, 0.1, 0.1, 0.2, 0.3, 0.3, 0.4, 0.1, 0.2};

		BOOST_CHECK_EQUAL_COLLECTIONS(rowCmp, rowCmp + 13, &matrix.row[0], &matrix.row[0] + 13);
		BOOST_CHECK_EQUAL_COLLECTIONS(colCmp, colCmp + 13, &matrix.col[0], &matrix.col[0] + 13);
		BOOST_CHECK_EQUAL_COLLECTIONS(valCmp, valCmp + 13, &matrix.val[0], &matrix.val[0] + 13);
	}
}

// Test 2: Single row
BOOST_AUTO_TEST_CASE(getSparseMatrix_partial_test2)
{
	cupcfd::mpi::Communicator comm;
	cupcfd::mpi::drivers::set_comm(comm, MPI_COMM_WORLD);
	cupcfd::error::eCodes status;

	if(comm.rank == 0)
	{
		// === Setup ===
		std::string fileName = "../tests/testdata/Matrix1SparseCOO.h5";
		SparseMatrixHDF5File<int, double> file(fileName);
		cupcfd::data_structures::matrices::SparseMatrixCOO<int, double> matrix(8, 8, 4);

		// Speciy which rows we want
		int desiredRows[1] = {5};

		// === Test and Check ===
		status = file.getSparseMatrix(matrix, desiredRows, 1, 2);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

		// Should probably use the matrix functions for this test, but can just check on
		// internal data structures for now
		int rowCmp[3] = {7, 7, 7};
		int colCmp[3] = {7, 8, 9};
		double valCmp[3] = {0.2, 0.3, 0.4};

		BOOST_CHECK_EQUAL_COLLECTIONS(rowCmp, rowCmp + 3, &matrix.row[0], &matrix.row[0] + 3);
		BOOST_CHECK_EQUAL_COLLECTIONS(colCmp, colCmp + 3, &matrix.col[0], &matrix.col[0] + 3);
		BOOST_CHECK_EQUAL_COLLECTIONS(valCmp, valCmp + 3, &matrix.val[0], &matrix.val[0] + 3);
	}
}

// Finalize MPI
BOOST_AUTO_TEST_CASE(cleanup)
{
    // Cleanup MPI Environment
    MPI_Finalize();
}
