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

#define BOOST_TEST_MODULE AllToAllMPI
#include <boost/test/unit_test.hpp>
#include <boost/test/output_test_stream.hpp>

#include <stdexcept>
#include "AllToAllMPI.h"
#include "mpi.h"
#include <iostream>
#include "Communicator.h"
#include "mpi.h"

// Setup MPI
BOOST_AUTO_TEST_CASE(setup)
{
    int argc = boost::unit_test::framework::master_test_suite().argc;
    char ** argv = boost::unit_test::framework::master_test_suite().argv;
    MPI_Init(&argc, &argv);
}

// ============================================================
// ==================== Driver Tests ==========================
// ============================================================
using namespace cupcfd::comm::mpi;

// === AllToAllMPI ===
// Test 1: Test Correct Communication of Data in All To All using ints.
BOOST_AUTO_TEST_CASE(AllToAllMPI_test1)
{
    cupcfd::comm::Communicator comm(MPI_COMM_WORLD);
    cupcfd::error::eCodes status;

	int nBuf = 8;
	int recvbuf[8];

	if(comm.rank == 0)
	{
		int sendbuf[8] = {1, 2, 3, 4, 5, 6, 7, 8};
		int cmp[8] = {1, 2, 9, 10, 17, 18, 25, 26};

		status = AllToAllMPI(sendbuf, 2, recvbuf, 2, comm.comm);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
		BOOST_CHECK_EQUAL_COLLECTIONS(recvbuf, recvbuf + nBuf, cmp, cmp + nBuf);
	}
	else if(comm.rank == 1)
	{
		int sendbuf[8] = {9, 10, 11, 12, 13 ,14 ,15, 16};
		int cmp[8] = {3, 4, 11, 12, 19, 20, 27, 28};

		status = AllToAllMPI(sendbuf, 2, recvbuf, 2, comm.comm);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
		BOOST_CHECK_EQUAL_COLLECTIONS(recvbuf, recvbuf + nBuf, cmp, cmp + nBuf);
	}
	else if(comm.rank == 2)
	{
		int sendbuf[8] = {17, 18, 19, 20, 21, 22, 23, 24};
		int cmp[8] = {5, 6, 13, 14, 21, 22, 29, 30};

		status = AllToAllMPI(sendbuf, 2, recvbuf, 2, comm.comm);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
		BOOST_CHECK_EQUAL_COLLECTIONS(recvbuf, recvbuf + nBuf, cmp, cmp + nBuf);
	}
	else if(comm.rank == 3)
	{
		int sendbuf[8] = {25, 26, 27, 28, 29, 30, 31, 32};
		int cmp[8] = {7, 8, 15, 16, 23, 24, 31, 32};

		status = AllToAllMPI(sendbuf, 2, recvbuf, 2, comm.comm);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
		BOOST_CHECK_EQUAL_COLLECTIONS(recvbuf, recvbuf + nBuf, cmp, cmp + nBuf);
	}
}

// Test 2: Test Correct Communication for a CustomMPIType with a registered type
BOOST_AUTO_TEST_CASE(AllToAllMPI_test2)
{

}

// Test 3: Error: Test Error Code for CustomMPIType when type is unregistered
BOOST_AUTO_TEST_CASE(AllToAllMPI_test3)
{

}

// === AllToAllVMPI1 ===
// Test 1: Test Correct Communication of Data in All To AllV using ints.
BOOST_AUTO_TEST_CASE(AllToAllVMPI1_test1)
{
    cupcfd::comm::Communicator comm(MPI_COMM_WORLD);
    cupcfd::error::eCodes status;

	if(comm.rank == 0)
	{
		int sendbuf[12] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
		int sendcounts[4] = {5, 3, 3, 1};
		int sdispls[4] = {0, 5, 8, 11};

		int recvbuf[7];
		int recvcounts[4] = {5, 0, 1, 1};
		int rdispls[4] = {0, 5, 5, 6};

		int cmp[7] = {1, 2, 3, 4, 5, 18, 25};

		status = AllToAllVMPI(sendbuf, sendcounts, sdispls,
					 recvbuf, recvcounts, rdispls,
					 comm.comm);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

		BOOST_CHECK_EQUAL_COLLECTIONS(recvbuf, recvbuf + 7, cmp, cmp + 7);

	}
	else if(comm.rank == 1)
	{
		int sendbuf[5] = {13, 14, 15, 16, 17};
		int sendcounts[4] = {0, 1, 2, 2};
		int sdispls[4] = {0, 0, 1, 3};

		int recvbuf[5];
		int recvcounts[4] = {3, 1, 1, 0};
		int rdispls[4] = {0, 3, 4, 5};

		int cmp[5] = {6, 7, 8, 13, 19};

		status = AllToAllVMPI(sendbuf, sendcounts, sdispls,
					 recvbuf, recvcounts, rdispls,
					 comm.comm);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

		BOOST_CHECK_EQUAL_COLLECTIONS(recvbuf, recvbuf + 5, cmp, cmp + 5);

	}
	else if(comm.rank == 2)
	{
		int sendbuf[7] = {18, 19, 20, 21, 22, 23, 24};
		int sendcounts[4] = {1, 1, 3, 2};
		int sdispls[4] = {0, 1, 2, 5};

		int recvbuf[12];
		int recvcounts[4] = {3, 2, 3, 4};
		int rdispls[4] = {0, 3, 5, 8};

		int cmp[12] = {9, 10, 11, 14, 15, 20, 21, 22, 26, 27, 28, 29};

		status = AllToAllVMPI(sendbuf, sendcounts, sdispls,
					 recvbuf, recvcounts, rdispls,
					 comm.comm);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

		BOOST_CHECK_EQUAL_COLLECTIONS(recvbuf, recvbuf + 12, cmp, cmp + 12);
	}
	else if(comm.rank == 3)
	{
		int sendbuf[8] = {25, 26, 27, 28, 29, 30, 31 ,32};
		int sendcounts[4] = {1, 0, 4, 3};
		int sdispls[4] = {0, 1, 1, 5};

		int recvbuf[8];
		int recvcounts[4] = {1, 2, 2, 3};
		int rdispls[4] = {0, 1, 3, 5};

		int cmp[8] = {12, 16, 17, 23, 24, 30, 31, 32};

		status = AllToAllVMPI(sendbuf, sendcounts, sdispls,
					 recvbuf, recvcounts, rdispls,
					 comm.comm);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

		BOOST_CHECK_EQUAL_COLLECTIONS(recvbuf, recvbuf + 8, cmp, cmp + 8);
	}
}

// Test 2: Test Correct Communication for a CustomMPIType with a registered type
BOOST_AUTO_TEST_CASE(AllToAllVMPI1_test2)
{

}

// Test 3: Error: Test Error Code for CustomMPIType when type is unregistered
BOOST_AUTO_TEST_CASE(AllToAllVMPI1_test3)
{

}

// === AllToAllVMPI2 ===
// Test 1: Test Correct Communication of Data in All To AllV using ints.
BOOST_AUTO_TEST_CASE(AllToAllVMPI2_test1)
{
    cupcfd::comm::Communicator comm(MPI_COMM_WORLD);
    cupcfd::error::eCodes status;

	if(comm.rank == 0)
	{
		int sendbuf[12] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
		int sendcounts[4] = {5, 3, 3, 1};

		int recvbuf[7];
		int recvcounts[4] = {5, 0, 1, 1};

		int cmp[7] = {1, 2, 3, 4, 5, 18, 25};

		status = AllToAllVMPI(sendbuf, sendcounts,
					 recvbuf, recvcounts,
					 comm.comm);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

		BOOST_CHECK_EQUAL_COLLECTIONS(recvbuf, recvbuf + 7, cmp, cmp + 7);

	}
	else if(comm.rank == 1)
	{
		int sendbuf[5] = {13, 14, 15, 16, 17};
		int sendcounts[4] = {0, 1, 2, 2};

		int recvbuf[5];
		int recvcounts[4] = {3, 1, 1, 0};

		int cmp[5] = {6, 7, 8, 13, 19};

		status = AllToAllVMPI(sendbuf, sendcounts,
					 recvbuf, recvcounts,
					 comm.comm);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

		BOOST_CHECK_EQUAL_COLLECTIONS(recvbuf, recvbuf + 5, cmp, cmp + 5);

	}
	else if(comm.rank == 2)
	{
		int sendbuf[7] = {18, 19, 20, 21, 22, 23, 24};
		int sendcounts[4] = {1, 1, 3, 2};

		int recvbuf[12];
		int recvcounts[4] = {3, 2, 3, 4};

		int cmp[12] = {9, 10, 11, 14, 15, 20, 21, 22, 26, 27, 28, 29};

		status = AllToAllVMPI(sendbuf, sendcounts,
					 recvbuf, recvcounts,
					 comm.comm);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

		BOOST_CHECK_EQUAL_COLLECTIONS(recvbuf, recvbuf + 12, cmp, cmp + 12);
	}
	else if(comm.rank == 3)
	{
		int sendbuf[8] = {25, 26, 27, 28, 29, 30, 31 ,32};
		int sendcounts[4] = {1, 0, 4, 3};

		int recvbuf[8];
		int recvcounts[4] = {1, 2, 2, 3};

		int cmp[8] = {12, 16, 17, 23, 24, 30, 31, 32};

		status = AllToAllVMPI(sendbuf, sendcounts,
					 recvbuf, recvcounts,
					 comm.comm);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

		BOOST_CHECK_EQUAL_COLLECTIONS(recvbuf, recvbuf + 8, cmp, cmp + 8);
	}
}

// Test 2: Test Correct Communication for a CustomMPIType with a registered type
BOOST_AUTO_TEST_CASE(AllToAllVMPI2_test2)
{

}

// Test 3: Error: Test Error Code for CustomMPIType when type is unregistered
BOOST_AUTO_TEST_CASE(AllToAllVMPI2_test3)
{

}

BOOST_AUTO_TEST_CASE(cleanup)
{
    // Cleanup MPI Environment
    MPI_Finalize();
}
