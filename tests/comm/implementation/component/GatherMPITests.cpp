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

#define BOOST_TEST_MODULE GatherMPI
#include <boost/test/unit_test.hpp>
#include <boost/test/output_test_stream.hpp>

#include <stdexcept>
#include "Communicator.h"
#include "GatherMPI.h"
#include "mpi.h"

BOOST_AUTO_TEST_CASE(setup)
{
    int argc = boost::unit_test::framework::master_test_suite().argc;
    char ** argv = boost::unit_test::framework::master_test_suite().argv;
    MPI_Init(&argc, &argv);
}

using namespace cupcfd::comm::mpi;

// === GatherMPI ===
// Test 1: Test Correct functionality with ints
BOOST_AUTO_TEST_CASE(GatherMPI_test1)
{
	cupcfd::comm::Communicator comm(MPI_COMM_WORLD);

	if(comm.rank == 0)
	{
		int buf[2];
		int rcv[8];
		int cmp[8] = {65, 1054, 102, 7, 9, 2, 11, 13};

		buf[0] = 65;
		buf[1] = 1054;

		rcv[0] = 0;
		rcv[1] = 0;
		rcv[2] = 0;
		rcv[3] = 0;
		rcv[4] = 0;
		rcv[5] = 0;
		rcv[6] = 0;
		rcv[7] = 0;

		cupcfd::error::eCodes err = GatherMPI(buf, rcv, 2, 0, comm.comm);
		BOOST_CHECK_EQUAL(err, cupcfd::error::E_SUCCESS);
	    BOOST_CHECK_EQUAL_COLLECTIONS(rcv, rcv + 8, cmp, cmp + 8);
	}
	else if(comm.rank == 1)
	{
		int buf[2];
		int * rcv = nullptr;

		buf[0] = 102;
		buf[1] = 7;

		cupcfd::error::eCodes err = GatherMPI(buf, rcv, 2, 0, comm.comm);
		BOOST_CHECK_EQUAL(err, cupcfd::error::E_SUCCESS);
	}
	else if(comm.rank == 2)
	{
		int buf[2];
		int * rcv = nullptr;

		buf[0] = 9;
		buf[1] = 2;

		cupcfd::error::eCodes err = GatherMPI(buf, rcv, 2, 0, comm.comm);
		BOOST_CHECK_EQUAL(err, cupcfd::error::E_SUCCESS);
	}
	else if(comm.rank == 3)
	{
		int buf[2];
		int * rcv = nullptr;

		buf[0] = 11;
		buf[1] = 13;

		cupcfd::error::eCodes err = GatherMPI(buf, rcv, 2, 0, comm.comm);
		BOOST_CHECK_EQUAL(err, cupcfd::error::E_SUCCESS);
	}
}

// Test 2: Test with a CustomMPIType that has a correctly registered datatype
// Will use EuclideanVector for now.
BOOST_AUTO_TEST_CASE(GatherMPI_test2)
{

}

// Test 3: Error Case: Test Broadcast with a CustomMPIType that has not registered a datatype
// Will use EuclideanVector for now.
BOOST_AUTO_TEST_CASE(GatherMPI_test3)
{

}

// === AllGatherMPI ===
// Test 1: Test Correct functionality with ints
BOOST_AUTO_TEST_CASE(AllGatherMPI_test1)
{
	cupcfd::comm::Communicator comm(MPI_COMM_WORLD);

	int buf[2];
	int rcv[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	int cmp[8] = {65, 1054, 102, 7, 9, 2, 11, 13};

	if(comm.rank == 0)
	{
		buf[0] = 65;
		buf[1] = 1054;

		cupcfd::error::eCodes err = AllGatherMPI(buf, rcv, 2, comm.comm);
		BOOST_CHECK_EQUAL(err, cupcfd::error::E_SUCCESS);
	    BOOST_CHECK_EQUAL_COLLECTIONS(rcv, rcv + 8, cmp, cmp + 8);
	}
	else if(comm.rank == 1)
	{
		int buf[2];
		int rcv[8];
		int cmp[8] = {65, 1054, 102, 7, 9, 2, 11, 13};

		buf[0] = 102;
		buf[1] = 7;

		cupcfd::error::eCodes err = AllGatherMPI(buf, rcv, 2, comm.comm);
		BOOST_CHECK_EQUAL(err, cupcfd::error::E_SUCCESS);
	    BOOST_CHECK_EQUAL_COLLECTIONS(rcv, rcv + 8, cmp, cmp + 8);
	}
	else if(comm.rank == 2)
	{
		int buf[2];
		int rcv[8];
		int cmp[8] = {65, 1054, 102, 7, 9, 2, 11, 13};

		buf[0] = 9;
		buf[1] = 2;

		cupcfd::error::eCodes err = AllGatherMPI(buf, rcv, 2, comm.comm);
		BOOST_CHECK_EQUAL(err, cupcfd::error::E_SUCCESS);
	    BOOST_CHECK_EQUAL_COLLECTIONS(rcv, rcv + 8, cmp, cmp + 8);
	}
	else if(comm.rank == 3)
	{
		int buf[2];
		int rcv[8];
		int cmp[8] = {65, 1054, 102, 7, 9, 2, 11, 13};

		buf[0] = 11;
		buf[1] = 13;

		cupcfd::error::eCodes err = AllGatherMPI(buf, rcv, 2, comm.comm);
		BOOST_CHECK_EQUAL(err, cupcfd::error::E_SUCCESS);
	    BOOST_CHECK_EQUAL_COLLECTIONS(rcv, rcv + 8, cmp, cmp + 8);
	}
}

// Test 2: Test with a CustomMPIType that has a correctly registered datatype
// Will use EuclideanVector for now.
BOOST_AUTO_TEST_CASE(AllGatherMPI_test2)
{

}

// Test 3: Error Case: Test Broadcast with a CustomMPIType that has not registered a datatype
// Will use EuclideanVector for now.
BOOST_AUTO_TEST_CASE(AllGatherMPI_test3)
{

}

// === GatherVMPI ===
// Test 1: Test Correct functionality with ints
BOOST_AUTO_TEST_CASE(GatherVMPI_test1)
{
	cupcfd::comm::Communicator comm(MPI_COMM_WORLD);

	if(comm.rank == 0)
	{
		int buf[3] = {65, 1054, 16};
		int * rcv = nullptr;
		int * rcvCounts = nullptr;
		cupcfd::error::eCodes err = GatherVMPI(buf, 3, rcv, rcvCounts, 1, comm.comm);
		BOOST_CHECK_EQUAL(err, cupcfd::error::E_SUCCESS);
	}
	else if(comm.rank == 1)
	{
		int buf[5] = {102, 7, 98, 76, 51};
		int rcvCounts[4] = {3,5,1,6};
		int rcv[15] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
		int cmp[15] = {65, 1054, 16, 102, 7, 98, 76, 51, 97, 76, 14, 56, 98, 90, 43};

		cupcfd::error::eCodes err = GatherVMPI(buf, 5, rcv, rcvCounts, 1, comm.comm);
		BOOST_CHECK_EQUAL(err, cupcfd::error::E_SUCCESS);
	    BOOST_CHECK_EQUAL_COLLECTIONS(rcv, rcv + 15, cmp, cmp + 15);
	}
	else if(comm.rank == 2)
	{
		int buf[1] = {97};
		int * rcv = nullptr;
		int * rcvCounts = nullptr;
		cupcfd::error::eCodes err = GatherVMPI(buf, 1, rcv, rcvCounts, 1, comm.comm);
		BOOST_CHECK_EQUAL(err, cupcfd::error::E_SUCCESS);
	}
	else if(comm.rank == 3)
	{
		int buf[6] = {76, 14, 56, 98, 90, 43};
		int * rcv = nullptr;
		int * rcvCounts = nullptr;
		cupcfd::error::eCodes err = GatherVMPI(buf, 6, rcv, rcvCounts, 1, comm.comm);
		BOOST_CHECK_EQUAL(err, cupcfd::error::E_SUCCESS);
	}
}

// Test 2: Test with a CustomMPIType that has a correctly registered datatype
// Will use EuclideanVector for now.
BOOST_AUTO_TEST_CASE(GatherVMPI_test2)
{

}

// Test 3: Error Case: Test Broadcast with a CustomMPIType that has not registered a datatype
// Will use EuclideanVector for now.
BOOST_AUTO_TEST_CASE(GatherVMPI_test3)
{

}

// === AllGatherVMPI ===
// Test 1: Test Correct functionality with ints
BOOST_AUTO_TEST_CASE(AllGatherVMPI_test1)
{
    cupcfd::comm::Communicator comm(MPI_COMM_WORLD);

	if(comm.rank == 0)
	{
		int buf[3] = {65, 1054, 16};
		int rcvCounts[4] = {3,5,1,6};
		int rcv[15] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
		int cmp[15] = {65, 1054, 16, 102, 7, 98, 76, 51, 97, 76, 14, 56, 98, 90, 43};

		cupcfd::error::eCodes err = AllGatherVMPI(buf, 3, rcv, rcvCounts, comm.comm);
		BOOST_CHECK_EQUAL(err, cupcfd::error::E_SUCCESS);
	    BOOST_CHECK_EQUAL_COLLECTIONS(rcv, rcv + 15, cmp, cmp + 15);
	}
	else if(comm.rank == 1)
	{
		int buf[5] = {102, 7, 98, 76, 51};
		int rcvCounts[4] = {3,5,1,6};
		int rcv[15] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
		int cmp[15] = {65, 1054, 16, 102, 7, 98, 76, 51, 97, 76, 14, 56, 98, 90, 43};

		cupcfd::error::eCodes err = AllGatherVMPI(buf, 5, rcv, rcvCounts, comm.comm);
		BOOST_CHECK_EQUAL(err, cupcfd::error::E_SUCCESS);
	    BOOST_CHECK_EQUAL_COLLECTIONS(rcv, rcv + 15, cmp, cmp + 15);
	}
	else if(comm.rank == 2)
	{
		int buf[1] = {97};
		int rcvCounts[4] = {3,5,1,6};
		int rcv[15] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
		int cmp[15] = {65, 1054, 16, 102, 7, 98, 76, 51, 97, 76, 14, 56, 98, 90, 43};

		cupcfd::error::eCodes err = AllGatherVMPI(buf, 1, rcv, rcvCounts, comm.comm);
		BOOST_CHECK_EQUAL(err, cupcfd::error::E_SUCCESS);
	    BOOST_CHECK_EQUAL_COLLECTIONS(rcv, rcv + 15, cmp, cmp + 15);
	}
	else if(comm.rank == 3)
	{
		int buf[6] = {76, 14, 56, 98, 90, 43};
		int rcvCounts[4] = {3,5,1,6};
		int rcv[15] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
		int cmp[15] = {65, 1054, 16, 102, 7, 98, 76, 51, 97, 76, 14, 56, 98, 90, 43};

		cupcfd::error::eCodes err = AllGatherVMPI(buf, 6, rcv, rcvCounts, comm.comm);
		BOOST_CHECK_EQUAL(err, cupcfd::error::E_SUCCESS);
	    BOOST_CHECK_EQUAL_COLLECTIONS(rcv, rcv + 15, cmp, cmp + 15);
	}
}

// Test 2: Test with a CustomMPIType that has a correctly registered datatype
// Will use EuclideanVector for now.
BOOST_AUTO_TEST_CASE(AllGatherVMPI_test2)
{

}

// Test 3: Error Case: Test Broadcast with a CustomMPIType that has not registered a datatype
// Will use EuclideanVector for now.
BOOST_AUTO_TEST_CASE(AllGatherVMPI_test3)
{

}

BOOST_AUTO_TEST_CASE(cleanup)
{
    // Cleanup MPI Environment
    MPI_Finalize();
}
