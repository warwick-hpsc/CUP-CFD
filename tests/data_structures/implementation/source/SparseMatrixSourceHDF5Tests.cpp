/*
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Tests for the SparseMatrixSourceHDF5 class
 */

#define BOOST_TEST_MODULE SparseMatrixSourceHDF5
#include <boost/test/unit_test.hpp>
#include <boost/test/output_test_stream.hpp>
#include <stdexcept>

#include "SparseMatrixSourceHDF5.h"
#include "Error.h"

#include "Communicator.h"

using namespace cupcfd::data_structures;

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
	cupcfd::comm::Communicator comm(MPI_COMM_WORLD);

	if(comm.rank == 0)
	{
		std::string fileName = "../tests/data_structures/data/Matrix1Sparse.h5";
		SparseMatrixSourceHDF5<int ,double> file(fileName);
	}
}

// === getNNZ ===
// Test 1:
BOOST_AUTO_TEST_CASE(getNNZ_test1)
{
	cupcfd::comm::Communicator comm(MPI_COMM_WORLD);
	cupcfd::error::eCodes status;

	if(comm.rank == 0)
	{
		std::string fileName = "../tests/data_structures/data/Matrix1Sparse.h5";
		SparseMatrixSourceHDF5<int, double> file(fileName);

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
	cupcfd::comm::Communicator comm(MPI_COMM_WORLD);
	cupcfd::error::eCodes status;

	if(comm.rank == 0)
	{
		std::string fileName = "../tests/data_structures/data/Matrix1Sparse.h5";
		SparseMatrixSourceHDF5<int, double> file(fileName);

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
	cupcfd::comm::Communicator comm(MPI_COMM_WORLD);
	cupcfd::error::eCodes status;

	if(comm.rank == 0)
	{
		std::string fileName = "../tests/data_structures/data/Matrix1Sparse.h5";
		SparseMatrixSourceHDF5<int, double> file(fileName);

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
	cupcfd::comm::Communicator comm(MPI_COMM_WORLD);
	cupcfd::error::eCodes status;

	if(comm.rank == 0)
	{
		std::string fileName = "../tests/data_structures/data/Matrix1Sparse.h5";
		SparseMatrixSourceHDF5<int, double> file(fileName);

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
	cupcfd::comm::Communicator comm(MPI_COMM_WORLD);
	cupcfd::error::eCodes status;

	if(comm.rank == 0)
	{
		std::string fileName = "../tests/data_structures/data/Matrix1Sparse.h5";
		SparseMatrixSourceHDF5<int, double> file(fileName);

		// This test file is known in advance to have 8 rows
		int * nnzRows = (int *) malloc(sizeof(int) * 8);
		status = file.getNNZRows(nnzRows, 8);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

		int cmp[8] = {3, 0, 6, 3, 3, 3, 4, 2};
		BOOST_CHECK_EQUAL_COLLECTIONS(cmp, cmp + 8, nnzRows, nnzRows + 8);
	}
}

// === getRowColumnIndexes ===
// Test 1: Test for a valid row
BOOST_AUTO_TEST_CASE(getRowColumnIndexes_test1)
{
	cupcfd::comm::Communicator comm(MPI_COMM_WORLD);
	cupcfd::error::eCodes status;

	if(comm.rank == 0)
	{
		std::string fileName = "../tests/data_structures/data/Matrix1Sparse.h5";
		SparseMatrixSourceHDF5<int, double> file(fileName);

		int * columnIndexes;
		int nColumnIndexes;

		// Get the columns for row 3 (base 1)
		status = file.getRowColumnIndexes(3, &columnIndexes, &nColumnIndexes);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

		int cmp[6] = {1, 2, 3, 4, 5, 6};
		BOOST_CHECK_EQUAL(nColumnIndexes, 6);
		BOOST_CHECK_EQUAL_COLLECTIONS(cmp, cmp + 6, columnIndexes, columnIndexes + 6);
		free(columnIndexes);
	}
}

// Test 2: Test a row with no non-zero values
BOOST_AUTO_TEST_CASE(getRowColumnIndexes_test2)
{
	cupcfd::comm::Communicator comm(MPI_COMM_WORLD);
	cupcfd::error::eCodes status;

	if(comm.rank == 0)
	{
		std::string fileName = "../tests/data_structures/data/Matrix1Sparse.h5";
		SparseMatrixSourceHDF5<int, double> file(fileName);

		int * columnIndexes;
		int nColumnIndexes;

		// Get the columns for row 2 (base 1)
		status = file.getRowColumnIndexes(2, &columnIndexes, &nColumnIndexes);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
		BOOST_CHECK_EQUAL(nColumnIndexes, 0);
		free(columnIndexes);
	}
}

// Test 3: Test last row
BOOST_AUTO_TEST_CASE(getRowColumnIndexes_test3)
{
	cupcfd::comm::Communicator comm(MPI_COMM_WORLD);
	cupcfd::error::eCodes status;

	if(comm.rank == 0)
	{
		std::string fileName = "../tests/data_structures/data/Matrix1Sparse.h5";
		SparseMatrixSourceHDF5<int, double> file(fileName);

		int * columnIndexes;
		int nColumnIndexes;

		// Get the columns for row 8 (base 1)
		status = file.getRowColumnIndexes(8, &columnIndexes, &nColumnIndexes);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

		int cmp[2] = {7, 8};
		BOOST_CHECK_EQUAL(nColumnIndexes, 2);
		BOOST_CHECK_EQUAL_COLLECTIONS(cmp, cmp + 1, columnIndexes, columnIndexes + 1);
		free(columnIndexes);
	}
}

// Test 4: Test first row
BOOST_AUTO_TEST_CASE(getRowColumnIndexes_test4)
{
	cupcfd::comm::Communicator comm(MPI_COMM_WORLD);
	cupcfd::error::eCodes status;

	if(comm.rank == 0)
	{
		std::string fileName = "../tests/data_structures/data/Matrix1Sparse.h5";
		SparseMatrixSourceHDF5<int, double> file(fileName);

		int * columnIndexes;
		int nColumnIndexes;

		// Get the columns for row 1 (base 1)
		status = file.getRowColumnIndexes(1, &columnIndexes, &nColumnIndexes);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

		int cmp[3] = {1, 2, 3};
		BOOST_CHECK_EQUAL(nColumnIndexes, 3);
		BOOST_CHECK_EQUAL_COLLECTIONS(cmp, cmp + 3, columnIndexes, columnIndexes + 3);
		free(columnIndexes);
	}
}

// === getRowNNZValues ===
// Test 1: Lookup Row Values - use different bases to test correct index lookup
BOOST_AUTO_TEST_CASE(getRowNNZValues_test1)
{
	cupcfd::comm::Communicator comm(MPI_COMM_WORLD);
	cupcfd::error::eCodes status;

	// Test Data - Base Index is 1
	std::string fileName = "../tests/data_structures/data/Matrix1Sparse.h5";
	SparseMatrixSourceHDF5<int, double> file(fileName);

	int row;

	// ToDo: Get a rank to load row 2 since it has zero elements

	if(comm.rank == 0)
	{
		// Row 5
		row = 6;
	}
	else if(comm.rank == 1)
	{
		// Row 2
		row = 3;
	}
	else if(comm.rank == 2)
	{
		// Rows 0
		row = 1;
	}
	else if(comm.rank == 3)
	{
		// Row 6
		row = 7;
	}

	double * rowNNZ;
	int nRowNNZ;

	status = file.getRowNNZValues(row, &rowNNZ, &nRowNNZ);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	if(comm.rank == 0)
	{
		double nnzCmp[3] = {0.4, 0.1, 0.2};
		BOOST_CHECK_EQUAL(nRowNNZ, 3);
		BOOST_CHECK_EQUAL_COLLECTIONS(nnzCmp, nnzCmp + 3, rowNNZ, rowNNZ + 3);
	}
	else if(comm.rank == 1)
	{
		double nnzCmp[6] = {0.4, 0.1, 0.2, 0.3, 0.4, 0.1};
		BOOST_CHECK_EQUAL(nRowNNZ, 6);
		BOOST_CHECK_EQUAL_COLLECTIONS(nnzCmp, nnzCmp + 6, rowNNZ, rowNNZ + 6);
	}
	else if(comm.rank == 2)
	{
		double nnzCmp[3] = {0.1, 0.2, 0.3};
		BOOST_CHECK_EQUAL(nRowNNZ, 3);
		BOOST_CHECK_EQUAL_COLLECTIONS(nnzCmp, nnzCmp + 3, rowNNZ, rowNNZ + 3);
	}
	else if(comm.rank == 3)
	{
		double nnzCmp[4] = {0.3, 0.4, 0.1, 0.2};
		BOOST_CHECK_EQUAL(nRowNNZ, 4);
		BOOST_CHECK_EQUAL_COLLECTIONS(nnzCmp, nnzCmp + 4, rowNNZ, rowNNZ + 4);
	}

	free(rowNNZ);
}

// Finalize MPI
BOOST_AUTO_TEST_CASE(cleanup)
{
    // Cleanup MPI Environment
    MPI_Finalize();
}
