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

#define BOOST_TEST_MODULE Statistics
#include <boost/test/unit_test.hpp>
#include <boost/test/output_test_stream.hpp>

#include <stdexcept>
#include "Scatter.h"
#include "mpi.h"

// Setup MPI
BOOST_AUTO_TEST_CASE(setup)
{
    int argc = boost::unit_test::framework::master_test_suite().argc;
    char ** argv = boost::unit_test::framework::master_test_suite().argv;
    MPI_Init(&argc, &argv);
}

using namespace cupcfd::comm;

// === Scatter1 ===
// Test 1: Test correct functionality when scattering ints
// for fixed size chunks.
BOOST_AUTO_TEST_CASE(Scatter1_test1)
{
    cupcfd::comm::Communicator comm(MPI_COMM_WORLD);

	// Test Intended for Communicator size of 4.
	int bufferSend[12] = {1, 2, 4, 3, 5, 7, 6, 8, 10, 9, 11, 12};
	int nSend = 3;
	int bufferRecv[3];
	int nRecv = 3;

	cupcfd::error::eCodes err = Scatter(&bufferSend[0], nSend, &bufferRecv[0], nRecv, nRecv, comm, 0);

	if(comm.rank == 0)
	{
		int recvCmp[3] = {1, 2, 4};
	    BOOST_CHECK_EQUAL_COLLECTIONS(bufferRecv, bufferRecv + 3, recvCmp, recvCmp + 3);
	}
	else if(comm.rank == 1)
	{
		int recvCmp[3] = {3, 5, 7};
	    BOOST_CHECK_EQUAL_COLLECTIONS(bufferRecv, bufferRecv + 3, recvCmp, recvCmp + 3);
	}
	else if(comm.rank == 2)
	{
		int recvCmp[3] = {6, 8, 10};
	    BOOST_CHECK_EQUAL_COLLECTIONS(bufferRecv, bufferRecv + 3, recvCmp, recvCmp + 3);
	}
	else if(comm.rank == 3)
	{
		int recvCmp[3] = {9, 11, 12};
	    BOOST_CHECK_EQUAL_COLLECTIONS(bufferRecv, bufferRecv + 3, recvCmp, recvCmp + 3);
	}
}

// === Scatter2 ===
// Test 1: Test correct functionality when scattering ints
BOOST_AUTO_TEST_CASE(Scatter2_test1)
{
    cupcfd::comm::Communicator comm(MPI_COMM_WORLD);

	// Test Intended for Communicator size of 4.
	int bufferSend[12] = {19, 2, 4, 3, 5, 7, 6, 8, 10, 9, 11, 12};
	int nSend = 12;
	int * bufferRecv;
	int nRecv;
	int sendCounts[4] = {1, 3, 6, 2};

	nRecv = sendCounts[comm.rank];
	bufferRecv = (int *) malloc(sizeof(int) * nRecv);

	for(int i = 0; i < sendCounts[comm.rank]; i++)
	{
		bufferRecv[i] = 0;
	}

	cupcfd::error::eCodes err = Scatter(bufferSend, nSend, bufferRecv, nRecv, sendCounts, 4, comm, 0);

	if(comm.rank == 0)
	{
		int recvCmp[1] = {19};
	    BOOST_CHECK_EQUAL_COLLECTIONS(bufferRecv, bufferRecv + 1, recvCmp, recvCmp + 1);
	}
	else if(comm.rank == 1)
	{
		int recvCmp[3] = {2, 4, 3};
	    BOOST_CHECK_EQUAL_COLLECTIONS(bufferRecv, bufferRecv + 3, recvCmp, recvCmp + 3);
	}
	else if(comm.rank == 2)
	{
		int recvCmp[6] = {5, 7, 6, 8, 10, 9};
	    BOOST_CHECK_EQUAL_COLLECTIONS(bufferRecv, bufferRecv + 6, recvCmp, recvCmp + 6);
	}
	else if(comm.rank == 3)
	{
		int recvCmp[2] = {11, 12};
	    BOOST_CHECK_EQUAL_COLLECTIONS(bufferRecv, bufferRecv + 2, recvCmp, recvCmp + 2);
	}

	free(bufferRecv);
}

// Test 2: Test correct functionality one of the destination processes receives no elements
// (but is specified as size 0).
BOOST_AUTO_TEST_CASE(Scatter2_test2)
{

}

// === Scatter3 ===
// Test 1: Test correct functionality when scattering ints
BOOST_AUTO_TEST_CASE(Scatter3_test1)
{
    cupcfd::comm::Communicator comm(MPI_COMM_WORLD);

	// Test Intended for Communicator size of 4.
	int bufferSend[20] 	= {5, 7, 123, 25, 91, 5, 5, 2, 6, 5, 27, 19, 78, 54, 71, 1052, 2562, 1523, 19, 111};
	int pSend[20] 		= {3, 1, 0,   3,  2,  1, 0, 0, 2, 1, 3,  1,  1,  0,  2,  1,    3,    3,    3,  3};
	int nSend = 20;

	int * bufferRecv;
	int nRecv;

	cupcfd::error::eCodes err = Scatter(bufferSend, nSend, &bufferRecv, &nRecv, pSend, 20, comm, 0);

	if(comm.rank == 0)
	{
		int recvCmp[4] = {123, 5, 2, 54};

		BOOST_CHECK_EQUAL(nRecv, 4);
	    BOOST_CHECK_EQUAL_COLLECTIONS(bufferRecv, bufferRecv + 4, recvCmp, recvCmp + 4);
	}
	else if(comm.rank == 1)
	{
		int recvCmp[6] = {7, 5, 5, 19, 78, 1052};
		BOOST_CHECK_EQUAL(nRecv, 6);
	    BOOST_CHECK_EQUAL_COLLECTIONS(bufferRecv, bufferRecv + 6, recvCmp, recvCmp + 6);
	}
	else if(comm.rank == 2)
	{
		int recvCmp[3] = {91, 6, 71};
		BOOST_CHECK_EQUAL(nRecv, 3);
	    BOOST_CHECK_EQUAL_COLLECTIONS(bufferRecv, bufferRecv + 3, recvCmp, recvCmp + 3);
	}
	else if(comm.rank == 3)
	{
		int recvCmp[7] = {5, 25, 27, 2562, 1523, 19, 111};
		BOOST_CHECK_EQUAL(nRecv, 7);
	    BOOST_CHECK_EQUAL_COLLECTIONS(bufferRecv, bufferRecv + 7, recvCmp, recvCmp + 7);
	}

	free(bufferRecv);
}

// Test 2: Test correct functionality one of the destination processes receives no elements
BOOST_AUTO_TEST_CASE(Scatter3_test2)
{

}

BOOST_AUTO_TEST_CASE(cleanup)
{
    // Cleanup MPI Environment
    MPI_Finalize();
}

