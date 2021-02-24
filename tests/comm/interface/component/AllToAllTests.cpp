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

#define BOOST_TEST_MODULE AllToAll
#include <boost/test/unit_test.hpp>
#include <boost/test/output_test_stream.hpp>

#include <stdexcept>
#include "AllToAll.h"
#include "mpi.h"
#include <iostream>
#include "Communicator.h"

BOOST_AUTO_TEST_CASE(setup)
{
    int argc = boost::unit_test::framework::master_test_suite().argc;
    char ** argv = boost::unit_test::framework::master_test_suite().argv;
    MPI_Init(&argc, &argv);
}

using namespace cupcfd::comm;

// ToDo: CustomTypes/Errors

// === AllToAll1 ===
// Test 1: Test Fixed-Size Correct Functionality
BOOST_AUTO_TEST_CASE(AllToAll1_test1)
{
    cupcfd::comm::Communicator comm(MPI_COMM_WORLD);

    cupcfd::error::eCodes status;

	int nBuf = 8;
	int recvbuf[8];

	if(comm.rank == 0)
	{
		int sendbuf[8] = {1, 2, 3, 4, 5, 6, 7, 8};
		int cmp[8] = {1, 2, 9, 10, 17, 18, 25, 26};

		status = AllToAll(sendbuf, 8, recvbuf, 8, 2, comm);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
		BOOST_CHECK_EQUAL_COLLECTIONS(recvbuf, recvbuf + nBuf, cmp, cmp + nBuf);
	}
	else if(comm.rank == 1)
	{
		int sendbuf[8] = {9, 10, 11, 12, 13 ,14 ,15, 16};
		int cmp[8] = {3, 4, 11, 12, 19, 20, 27, 28};

		status = AllToAll(sendbuf, 8, recvbuf, 8, 2, comm);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
		BOOST_CHECK_EQUAL_COLLECTIONS(recvbuf, recvbuf + nBuf, cmp, cmp + nBuf);
	}
	else if(comm.rank == 2)
	{
		int sendbuf[8] = {17, 18, 19, 20, 21, 22, 23, 24};
		int cmp[8] = {5, 6, 13, 14, 21, 22, 29, 30};

		status = AllToAll(sendbuf, 8, recvbuf, 8, 2, comm);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
		BOOST_CHECK_EQUAL_COLLECTIONS(recvbuf, recvbuf + nBuf, cmp, cmp + nBuf);
	}
	else if(comm.rank == 3)
	{
		int sendbuf[8] = {25, 26, 27, 28, 29, 30, 31, 32};
		int cmp[8] = {7, 8, 15, 16, 23, 24, 31, 32};

		status = AllToAll(sendbuf, 8, recvbuf, 8, 2, comm);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
		BOOST_CHECK_EQUAL_COLLECTIONS(recvbuf, recvbuf + nBuf, cmp, cmp + nBuf);
	}
}

// === AllToAll2 ===
// Test 1: Test Variable Correct Functionality With Displacements specified
BOOST_AUTO_TEST_CASE(AllToAll2_test2)
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

		status = AllToAll(sendbuf, 12,
				  sendcounts, 4,
				  sdispls, 4,
				  recvbuf, 7,
				  recvcounts, 4,
				  rdispls, 4,
				  comm);

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

		status = AllToAll(sendbuf, 5,
				  sendcounts, 4,
				  sdispls, 4,
				  recvbuf, 5,
				  recvcounts, 4,
				  rdispls, 4,
				  comm);

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

		status = AllToAll(sendbuf, 7,
				  sendcounts, 4,
				  sdispls, 4,
				  recvbuf, 12,
				  recvcounts, 4,
				  rdispls, 4,
				  comm);

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

		status = AllToAll(sendbuf, 8,
				  sendcounts, 4,
				  sdispls, 4,
				  recvbuf, 8,
				  recvcounts, 4,
				  rdispls, 4,
				  comm);

		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
		BOOST_CHECK_EQUAL_COLLECTIONS(recvbuf, recvbuf + 8, cmp, cmp + 8);
	}
}


// === AllToAll3 ===
// Test 1: Test Variable Correct Functionality without Displacements specified
BOOST_AUTO_TEST_CASE(AllToAll3_test3)
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

		status = AllToAll(sendbuf, 12,
				  sendcounts, 4,
				  recvbuf, 7,
				  recvcounts, 4,
				  comm);

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

		status = AllToAll(sendbuf, 5,
				  sendcounts, 4,
				  recvbuf, 5,
				  recvcounts, 4,
				  comm);

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

		status = AllToAll(sendbuf, 7,
				  sendcounts, 4,
				  recvbuf, 12,
				  recvcounts, 4,
				  comm);

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

		status = AllToAll(sendbuf, 8,
				  sendcounts, 4,
				  recvbuf, 8,
				  recvcounts, 4,
				  comm);

		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
		BOOST_CHECK_EQUAL_COLLECTIONS(recvbuf, recvbuf + 8, cmp, cmp + 8);
	}
}

// === AllToAll4 ===
// Test 1: Test Variable Correct Functionality With Process/Element Pairing in
// unsorted order
BOOST_AUTO_TEST_CASE(AllToAll4_test1)
{
	cupcfd::comm::Communicator comm(MPI_COMM_WORLD);

    cupcfd::error::eCodes status;
    
	if(comm.rank == 0)
	{
		int sendbuf[12] =    {2, 4, 6, 7, 8, 3, 9, 5, 11, 12, 10, 1};
		int processIDs[12] = {0, 0, 1, 1, 1, 0, 2, 0, 2,  3,  2,  0};

		int cmp[7] = {2, 4, 3, 5, 1, 18, 25};

		int * recvbuf;
		int nRecvBuf;

		status = AllToAll(sendbuf, 12, processIDs, 12, &recvbuf, &nRecvBuf, comm);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
		BOOST_CHECK_EQUAL_COLLECTIONS(recvbuf, recvbuf + 7, cmp, cmp + 7);

		free(recvbuf);

	}
	else if(comm.rank == 1)
	{
		int sendbuf[5] =    {16, 14, 13, 15, 17};
		int processIDs[5] = {3,  2,  1,  2,  3};

		int cmp[5] = {6, 7, 8, 13, 19};

		int * recvbuf;
		int nRecvBuf;

		status = AllToAll(sendbuf, 5, processIDs, 5, &recvbuf, &nRecvBuf, comm);

		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
		BOOST_CHECK_EQUAL_COLLECTIONS(recvbuf, recvbuf + 5, cmp, cmp + 5);

		free(recvbuf);
	}
	else if(comm.rank == 2)
	{
		int sendbuf[7] =    {23, 19, 20, 22, 24, 21, 18};
		int processIDs[7] = {3,  1,  2,  2,  3,  2,  0};

		int cmp[12] = {9, 11, 10, 14, 15, 20, 22, 21, 26, 27, 28, 29};

		int * recvbuf;
		int nRecvBuf;

		status = AllToAll(sendbuf, 7, processIDs, 7, &recvbuf, &nRecvBuf, comm);

		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
		BOOST_CHECK_EQUAL_COLLECTIONS(recvbuf, recvbuf + 12, cmp, cmp + 12);

		free(recvbuf);
	}
	else if(comm.rank == 3)
	{
		int sendbuf[8] = {25, 26, 27, 28, 29, 30, 31 ,32};
		int processIDs[8] = {0, 2, 2, 2, 2, 3, 3, 3};
		int cmp[8] = {12, 16, 17, 23, 24, 30, 31, 32};

		int * recvbuf;
		int nRecvBuf;

		status = AllToAll(sendbuf, 8, processIDs, 8, &recvbuf, &nRecvBuf, comm);

		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
		BOOST_CHECK_EQUAL_COLLECTIONS(recvbuf, recvbuf + 8, cmp, cmp + 8);

		free(recvbuf);
	}
}

BOOST_AUTO_TEST_CASE(cleanup)
{
    // Cleanup MPI Environment
    MPI_Finalize();
}
