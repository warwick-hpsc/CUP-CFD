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

#define BOOST_TEST_MODULE BroadcastMPI
#include <boost/test/unit_test.hpp>
#include <boost/test/output_test_stream.hpp>

#include <stdexcept>

#include "mpi.h"

#include "Communicator.h"
#include "BroadcastMPI.h"
#include "EuclideanVector.h"

// Setup MPI
BOOST_AUTO_TEST_CASE(setup)
{
    int argc = boost::unit_test::framework::master_test_suite().argc;
    char ** argv = boost::unit_test::framework::master_test_suite().argv;
    MPI_Init(&argc, &argv);
}

using namespace cupcfd::comm::mpi;

// === BroadcastMPI ===
// Test 1: Test broadcasting integers across 4 ranks from rank 0
BOOST_AUTO_TEST_CASE(BroadcastMPI_test1)
{
    cupcfd::comm::Communicator comm(MPI_COMM_WORLD);

	int buf[6];
	int nBuf = 6;
	int cmp[6] = {102, 7, 9, 2, 11, 13};

	if(comm.rank == 0)
	{
		buf[0] = 102;
		buf[1] = 7;
		buf[2] = 9;
		buf[3] = 2;
		buf[4] = 11;
		buf[5] = 13;

		cupcfd::error::eCodes err = BroadcastMPI(buf, nBuf, 0, comm.comm);
		BOOST_CHECK_EQUAL(err, cupcfd::error::E_SUCCESS);
	    BOOST_CHECK_EQUAL_COLLECTIONS(buf, buf + nBuf, cmp, cmp + nBuf);
	}
	else if(comm.rank == 1)
	{
		buf[0] = 0;
		buf[1] = 0;
		buf[2] = 0;
		buf[3] = 0;
		buf[4] = 0;
		buf[5] = 0;

		cupcfd::error::eCodes err = BroadcastMPI(buf, nBuf, 0, comm.comm);
		BOOST_CHECK_EQUAL(err, cupcfd::error::E_SUCCESS);
	    BOOST_CHECK_EQUAL_COLLECTIONS(buf, buf + nBuf, cmp, cmp + nBuf);
	}
	else if(comm.rank == 2)
	{
		buf[0] = 0;
		buf[1] = 0;
		buf[2] = 0;
		buf[3] = 0;
		buf[4] = 0;
		buf[5] = 0;

		cupcfd::error::eCodes err = BroadcastMPI(buf, nBuf, 0, comm.comm);
		BOOST_CHECK_EQUAL(err, cupcfd::error::E_SUCCESS);
	    BOOST_CHECK_EQUAL_COLLECTIONS(buf, buf + nBuf, cmp, cmp + nBuf);
	}
	else if(comm.rank == 3)
	{
		buf[0] = 0;
		buf[1] = 0;
		buf[2] = 0;
		buf[3] = 0;
		buf[4] = 0;
		buf[5] = 0;

		cupcfd::error::eCodes err = BroadcastMPI(buf, nBuf, 0, comm.comm);
		BOOST_CHECK_EQUAL(err, cupcfd::error::E_SUCCESS);
	    BOOST_CHECK_EQUAL_COLLECTIONS(buf, buf + nBuf, cmp, cmp + nBuf);
	}
}

// Test 2: Test broadcasting doubles across 4 ranks from rank 1
BOOST_AUTO_TEST_CASE(BroadcastMPI_test2)
{
	cupcfd::comm::Communicator comm(MPI_COMM_WORLD);

	double buf[6];
	int nBuf = 6;
	double cmp[6] = {102.23, 7.10, 9.95, 2.682, 11.111, 13.73003};

	if(comm.rank == 0)
	{
		buf[0] = 0;
		buf[1] = 0;
		buf[2] = 0;
		buf[3] = 0;
		buf[4] = 0;
		buf[5] = 0;

		cupcfd::error::eCodes err = BroadcastMPI(buf, nBuf, 1, comm.comm);
		BOOST_CHECK_EQUAL(err, cupcfd::error::E_SUCCESS);
	    BOOST_CHECK_EQUAL_COLLECTIONS(buf, buf + nBuf, cmp, cmp + nBuf);
	}
	else if(comm.rank == 1)
	{
		buf[0] = 102.23;
		buf[1] = 7.10;
		buf[2] = 9.95;
		buf[3] = 2.682;
		buf[4] = 11.111;
		buf[5] = 13.73003;

		cupcfd::error::eCodes err = BroadcastMPI(buf, nBuf, 1, comm.comm);
		BOOST_CHECK_EQUAL(err, cupcfd::error::E_SUCCESS);
	    BOOST_CHECK_EQUAL_COLLECTIONS(buf, buf + nBuf, cmp, cmp + nBuf);
	}
	else if(comm.rank == 2)
	{
		buf[0] = 0;
		buf[1] = 0;
		buf[2] = 0;
		buf[3] = 0;
		buf[4] = 0;
		buf[5] = 0;

		cupcfd::error::eCodes err = BroadcastMPI(buf, nBuf, 1, comm.comm);
		BOOST_CHECK_EQUAL(err, cupcfd::error::E_SUCCESS);
	    BOOST_CHECK_EQUAL_COLLECTIONS(buf, buf + nBuf, cmp, cmp + nBuf);
	}
	else if(comm.rank == 3)
	{
		buf[0] = 0;
		buf[1] = 0;
		buf[2] = 0;
		buf[3] = 0;
		buf[4] = 0;
		buf[5] = 0;

		cupcfd::error::eCodes err = BroadcastMPI(buf, nBuf, 1, comm.comm);
		BOOST_CHECK_EQUAL(err, cupcfd::error::E_SUCCESS);
	    BOOST_CHECK_EQUAL_COLLECTIONS(buf, buf + nBuf, cmp, cmp + nBuf);
	}
}

// Test 3: Test Broadcast with a CustomMPIType that has a correctly registered datatype
// This is a duplicate of a test in EuclideanVectorTests
BOOST_AUTO_TEST_CASE(BroadcastMPI_test3)
{
	cupcfd::error::eCodes status;
	cupcfd::comm::Communicator comm(MPI_COMM_WORLD);

	// Create Two Vectors on Rank 0

	// Register the type (only once per all tests in file since static?)
	cupcfd::geometry::euclidean::EuclideanVector<double,3> vec;

	// Should not yet be registered
	BOOST_CHECK_EQUAL(vec.isRegistered(), false);

	// Register
	status = vec.registerMPIType();
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	// Should now be registered
	BOOST_CHECK_EQUAL(vec.isRegistered(), true);

	if(comm.rank == 0)
	{
		cupcfd::geometry::euclidean::EuclideanVector<double,3> vecs[2] = {
				cupcfd::geometry::euclidean::EuclideanVector<double,3>(3.5, 6.3, 19.7),
				cupcfd::geometry::euclidean::EuclideanVector<double,3>(3.0, 4.5, 6.7)};

		status = BroadcastMPI(vecs, 2, 0, comm.comm);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	}
	else
	{
		cupcfd::geometry::euclidean::EuclideanVector<double,3> recvVecs[2];

		status = BroadcastMPI(recvVecs, 2, 0, comm.comm);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

		BOOST_CHECK_EQUAL(recvVecs[0].cmp[0], 3.5);
		BOOST_CHECK_EQUAL(recvVecs[0].cmp[1], 6.3);
		BOOST_CHECK_EQUAL(recvVecs[0].cmp[2], 19.7);

		BOOST_CHECK_EQUAL(recvVecs[1].cmp[0], 3.0);
		BOOST_CHECK_EQUAL(recvVecs[1].cmp[1], 4.5);
		BOOST_CHECK_EQUAL(recvVecs[1].cmp[2], 6.7);
	}

	// Cleanup for future tests since we use a static variable!
	status = vec.deregisterMPIType();
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
}

// Test 4: Error Case: Test Broadcast with a CustomMPIType that has not registered a datatype
BOOST_AUTO_TEST_CASE(BroadcastMPI_test4)
{
	cupcfd::error::eCodes status;
	cupcfd::comm::Communicator comm(MPI_COMM_WORLD);

	// Create Two Vectors on Rank 0

	// Register the type (only once per all tests in file since static?)
	cupcfd::geometry::euclidean::EuclideanVector<double,3> vec;

	// Should not yet be registered
	BOOST_CHECK_EQUAL(vec.isRegistered(), false);

	if(comm.rank == 0)
	{
		cupcfd::geometry::euclidean::EuclideanVector<double,3> vecs[2] = {
				cupcfd::geometry::euclidean::EuclideanVector<double,3>(3.5, 6.3, 19.7),
				cupcfd::geometry::euclidean::EuclideanVector<double,3>(3.0, 4.5, 6.7)};

		status = BroadcastMPI(vecs, 2, 0, comm.comm);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_MPI_DATATYPE_UNREGISTERED);
	}
	else
	{
		cupcfd::geometry::euclidean::EuclideanVector<double,3> recvVecs[2];
		status = BroadcastMPI(recvVecs, 2, 0, comm.comm);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_MPI_DATATYPE_UNREGISTERED);
	}
}

BOOST_AUTO_TEST_CASE(cleanup)
{
    // Cleanup MPI Environment
    MPI_Finalize();
}
