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

#define BOOST_TEST_MODULE Broadcast
#include <boost/test/unit_test.hpp>
#include <boost/test/output_test_stream.hpp>

#include <stdexcept>
#include "Broadcast.h"
#include "mpi.h"
#include <iostream>
#include <cstring>

// Setup MPI
BOOST_AUTO_TEST_CASE(setup)
{
    int argc = boost::unit_test::framework::master_test_suite().argc;
    char ** argv = boost::unit_test::framework::master_test_suite().argv;
    MPI_Init(&argc, &argv);
}

// Namespace
using namespace cupcfd::comm;

// === Broadcast1 ===
// Broadcast and storage of buffer, passing send pointer only

// Test1: Test an integer broadcast from rank 0 of a buffer
BOOST_AUTO_TEST_CASE(broadcast1_test1)
{
    Communicator comm(MPI_COMM_WORLD);

    int buf[6] = {0, 0, 0, 0, 0, 0};
    int nBuf = 6;

	int cmp[6] = {102, 7, 9, 2, 11, 13};

	if(comm.rank == 0)
	{
		std::memcpy(buf, cmp, sizeof(int) * 6);
	}

	cupcfd::error::eCodes status = Broadcast(buf, nBuf, 0, comm);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
    BOOST_CHECK_EQUAL_COLLECTIONS(buf, buf + nBuf, cmp, cmp + nBuf);
}

// Test2: Test a double broadcast from rank 0 of a buffer
BOOST_AUTO_TEST_CASE(broadcast1_test2)
{
    Communicator comm(MPI_COMM_WORLD);

    double buf[6] = {0, 0, 0, 0, 0, 0};
    int nBuf = 6;

    double cmp[6] = {102.23, 7.10, 9.95, 2.682, 11.111, 13.73003};

	if(comm.rank == 0)
	{
		std::memcpy(buf, cmp, sizeof(double) * 6);
	}

	cupcfd::error::eCodes status = Broadcast(buf, nBuf, 0, comm);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
    BOOST_CHECK_EQUAL_COLLECTIONS(buf, buf + nBuf, cmp, cmp + nBuf);
}

// === Broadcast2 ===
// Broadcast and storage of buffer, copying to recv buffer

// Test1: Test an integer broadcast from rank 0 of a buffer
BOOST_AUTO_TEST_CASE(broadcast2_test1)
{
    Communicator comm(MPI_COMM_WORLD);

    int send[6] = {0, 0, 0, 0, 0, 0};
    int nSend = 6;

    int recv[6] = {0, 0, 0, 0, 0, 0};
    int nRecv = 6;

	int cmp[6] = {102, 7, 9, 2, 11, 13};

	if(comm.rank == 0)
	{
		std::memcpy(send, cmp, sizeof(int) * 6);
	}

	cupcfd::error::eCodes status = Broadcast(send, nSend, recv, nRecv, 0, comm);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
    BOOST_CHECK_EQUAL_COLLECTIONS(recv, recv + nRecv, cmp, cmp + 6);
}

// Test2: Test a double broadcast from rank 0 of a buffer
BOOST_AUTO_TEST_CASE(broadcast2_test2)
{
    Communicator comm(MPI_COMM_WORLD);

    double send[6] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
    int nSend = 6;

    double recv[6] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
    int nRecv = 6;

    double cmp[6] = {102.23, 7.10, 9.95, 2.682, 11.111, 13.73003};

	if(comm.rank == 0)
	{
		std::memcpy(send, cmp, sizeof(double) * 6);
	}

	cupcfd::error::eCodes status = Broadcast(send, nSend, recv, nRecv, 0, comm);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
    BOOST_CHECK_EQUAL_COLLECTIONS(recv, recv + nRecv, cmp, cmp + 6);
}

// === Broadcast3 ===
// Broadcast and storage of buffer, creating the recv buffer of
// an appropriate size

// Test1: Test an integer broadcast from rank 0 of a buffer
BOOST_AUTO_TEST_CASE(broadcast3_test1)
{
    Communicator comm(MPI_COMM_WORLD);

    int send[6] = {0, 0, 0, 0, 0, 0};
    int nSend = 6;

    int * recv;
    int nRecv;

	int cmp[6] = {102, 7, 9, 2, 11, 13};

	if(comm.rank == 0)
	{
		std::memcpy(send, cmp, sizeof(int) * 6);
	}

	cupcfd::error::eCodes status = Broadcast(send, nSend, &recv, &nRecv, 0, comm);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	BOOST_CHECK_EQUAL(nRecv, 6);
    BOOST_CHECK_EQUAL_COLLECTIONS(recv, recv + nRecv, cmp, cmp + 6);
    free(recv);
}

// Test2: Test a double broadcast from rank 0 of a buffer
BOOST_AUTO_TEST_CASE(broadcast3_test2)
{
    Communicator comm(MPI_COMM_WORLD);

    double send[6] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
    int nSend = 6;

    double * recv;
    int nRecv;

    double cmp[6] = {102.23, 7.10, 9.95, 2.682, 11.111, 13.73003};

	if(comm.rank == 0)
	{
		std::memcpy(send, cmp, sizeof(double) * 6);
	}

	cupcfd::error::eCodes status = Broadcast(send, nSend, &recv, &nRecv, 0, comm);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	BOOST_CHECK_EQUAL(nRecv, 6);
    BOOST_CHECK_EQUAL_COLLECTIONS(recv, recv + nRecv, cmp, cmp + 6);
    free(recv);
}

BOOST_AUTO_TEST_CASE(cleanup)
{
    // Cleanup MPI Environment
    MPI_Finalize();
}
