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

#define BOOST_TEST_MODULE ScatterMPI
#include <boost/test/unit_test.hpp>
#include <boost/test/output_test_stream.hpp>

#include <stdexcept>

#include "ScatterMPI.h"
#include "Communicator.h"

// Setup MPI
BOOST_AUTO_TEST_CASE(setup)
{
    int argc = boost::unit_test::framework::master_test_suite().argc;
    char ** argv = boost::unit_test::framework::master_test_suite().argv;
    MPI_Init(&argc, &argv);
}

using namespace cupcfd::comm::mpi;

// === ScatterMPI ===
// Test 1: Test correct scatter of integers from rank 0
BOOST_AUTO_TEST_CASE(ScatterMPI_test1)
{
    cupcfd::comm::Communicator comm(MPI_COMM_WORLD);
    cupcfd::error::eCodes status;

	// Test Intended for Communicator size of 4.
	int bufferSend[12] = {1, 2, 4, 3, 5, 7, 6, 8, 10, 9, 11, 12};
	int nSend = 3;
	int bufferRecv[3];
	int nRecv = 3;

	status = ScatterMPI(bufferSend, nSend, bufferRecv, nRecv, 0, comm.comm);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

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

// Test 2: Test correct scatter of CustomMPIType with registered type from rank 1
BOOST_AUTO_TEST_CASE(ScatterMPI_test2)
{

}

// Test 3: Error Test: Test CustomMPIType with Unregistered Type
BOOST_AUTO_TEST_CASE(ScatterMPI_test3)
{

}

// === ScatterVMPI ===
// Test 1: Test variable scatter from rank 0
BOOST_AUTO_TEST_CASE(ScatterVMPI_test1)
{
    cupcfd::comm::Communicator comm(MPI_COMM_WORLD);
    cupcfd::error::eCodes status;

	// Test Intended for Communicator size of 4.
	int bufferSend[12] = {19, 2, 4, 3, 5, 7, 6, 8, 10, 9, 11, 12};
	int nSend = 12;
	int * bufferRecv;
	int nRecv;
	int sendCounts[4] = {1, 3, 6, 2};

	nRecv = sendCounts[comm.rank];
	bufferRecv = (int *) malloc(sizeof(int) * nRecv);

	status = ScatterVMPI(bufferSend, nSend, bufferRecv, sendCounts[comm.rank], sendCounts, 4, 0, comm.comm);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

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

// Test 2: Test correct scatter of CustomMPIType with registered type from rank 1
BOOST_AUTO_TEST_CASE(ScatterVMPI_test2)
{

}

// Test 3: Error Test: Test CustomMPIType with Unregistered Type
BOOST_AUTO_TEST_CASE(ScatterVMPI_test3)
{

}

BOOST_AUTO_TEST_CASE(cleanup)
{
    // Cleanup MPI Environment
    MPI_Finalize();
}

