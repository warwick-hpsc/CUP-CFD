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

#define BOOST_TEST_MODULE Gather
#include <boost/test/unit_test.hpp>
#include <boost/test/output_test_stream.hpp>

#include <stdexcept>
#include "Gather.h"
#include "mpi.h"
#include "CommError.h"

BOOST_AUTO_TEST_CASE(setup)
{
    int argc = boost::unit_test::framework::master_test_suite().argc;
    char ** argv = boost::unit_test::framework::master_test_suite().argv;
    MPI_Init(&argc, &argv);
}

using namespace cupcfd::comm;

// === Gather ===
// Test 1: Test Gather of correct values using integers
BOOST_AUTO_TEST_CASE(Gather_test1)
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

		cupcfd::error::eCodes err = Gather(buf, 2, rcv, 8, 2, 0, comm);
	    BOOST_CHECK_EQUAL_COLLECTIONS(rcv, rcv + 8, cmp, cmp + 8);
	}
	else if(comm.rank == 1)
	{
		int buf[2];
		int * rcv = nullptr;

		buf[0] = 102;
		buf[1] = 7;

		cupcfd::error::eCodes err = Gather(buf, 2, rcv, 0, 2, 0, comm);
	}
	else if(comm.rank == 2)
	{
		int buf[2];
		int * rcv = nullptr;

		buf[0] = 9;
		buf[1] = 2;

		cupcfd::error::eCodes err = Gather(buf, 2, rcv, 0, 2, 0, comm);
	}
	else if(comm.rank == 3)
	{
		int buf[2];
		int * rcv = nullptr;

		buf[0] = 11;
		buf[1] = 13;

		cupcfd::error::eCodes err = Gather(buf, 2, rcv, 0, 2, 0, comm);
	}
}

// === AllGather ===
// Test 1: Test AllGather of Correct Value using integers
BOOST_AUTO_TEST_CASE(AllGather_test1)
{
    cupcfd::comm::Communicator comm(MPI_COMM_WORLD);

	int buf[2];
	int rcv[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	int cmp[8] = {65, 1054, 102, 7, 9, 2, 11, 13};

	if(comm.rank == 0)
	{
		buf[0] = 65;
		buf[1] = 1054;

		cupcfd::error::eCodes err = AllGather(buf, 2, rcv, 8, 2, comm);
	    BOOST_CHECK_EQUAL_COLLECTIONS(rcv, rcv + 8, cmp, cmp + 8);
	}
	else if(comm.rank == 1)
	{
		buf[0] = 102;
		buf[1] = 7;

		cupcfd::error::eCodes err = AllGather(buf, 2, rcv, 8, 2, comm);
	    BOOST_CHECK_EQUAL_COLLECTIONS(rcv, rcv + 8, cmp, cmp + 8);
	}
	else if(comm.rank == 2)
	{
		buf[0] = 9;
		buf[1] = 2;

		cupcfd::error::eCodes err = AllGather(buf, 2, rcv, 8, 2, comm);
	    BOOST_CHECK_EQUAL_COLLECTIONS(rcv, rcv + 8, cmp, cmp + 8);
	}
	else if(comm.rank == 3)
	{
		buf[0] = 11;
		buf[1] = 13;

		cupcfd::error::eCodes err = AllGather(buf, 2, rcv, 8, 2, comm);
	    BOOST_CHECK_EQUAL_COLLECTIONS(rcv, rcv + 8, cmp, cmp + 8);
	}
}

// === GatherV1 ===
// Test 1: Test Gather of Correct Values using integers
BOOST_AUTO_TEST_CASE(GatherV1_test1)
{
    cupcfd::comm::Communicator comm(MPI_COMM_WORLD);

	if(comm.rank == 0)
	{
		int buf[3] = {65, 1054, 16};
		int * rcv = nullptr;
		int * rcvCounts = nullptr;
		cupcfd::error::eCodes err = GatherV(buf, 3, rcv, 0, rcvCounts, 0, 1, comm);
	}
	else if(comm.rank == 1)
	{
		int buf[5] = {102, 7, 98, 76, 51};
		int rcvCounts[4] = {3,5,1,6};
		int rcv[15] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
		int cmp[15] = {65, 1054, 16, 102, 7, 98, 76, 51, 97, 76, 14, 56, 98, 90, 43};

		cupcfd::error::eCodes err = GatherV(buf, 5, rcv, 15, rcvCounts, 15, 1, comm);
	    BOOST_CHECK_EQUAL_COLLECTIONS(rcv, rcv + 15, cmp, cmp + 15);
	}
	else if(comm.rank == 2)
	{
		int buf[1] = {97};
		int * rcv = nullptr;
		int * rcvCounts = nullptr;
		cupcfd::error::eCodes err = GatherV(buf, 1, rcv, 0, rcvCounts, 0, 1, comm);
	}
	else if(comm.rank == 3)
	{
		int buf[6] = {76, 14, 56, 98, 90, 43};
		int * rcv = nullptr;
		int * rcvCounts = nullptr;
		cupcfd::error::eCodes err = GatherV(buf, 6, rcv, 0, rcvCounts, 0, 1, comm);
	}
}


// === GatherV2 ===
// Test 1: Test Gather of Correct Values using integers, as well as correct
// creation of suitably sized buffers
BOOST_AUTO_TEST_CASE(GatherV2_test1)
{
    cupcfd::comm::Communicator comm(MPI_COMM_WORLD);

	int * rcv = nullptr;
	int nRcv;

	int * rcvCounts = nullptr;
	int nRecvCounts;

	if(comm.rank == 0)
	{
		int buf[3] = {65, 1054, 16};
		cupcfd::error::eCodes err = GatherV(buf, 3, &rcv, &nRcv, &rcvCounts, &nRecvCounts, 1, comm);
	}
	else if(comm.rank == 1)
	{
		int buf[5] = {102, 7, 98, 76, 51};
		int cmp[15] = {65, 1054, 16, 102, 7, 98, 76, 51, 97, 76, 14, 56, 98, 90, 43};
		int cmp2[4] = {3, 5, 1, 6};

		cupcfd::error::eCodes err = GatherV(buf, 5, &rcv, &nRcv, &rcvCounts, &nRecvCounts, 1, comm);
		BOOST_CHECK_EQUAL(err, cupcfd::error::E_SUCCESS);
		BOOST_CHECK_EQUAL(nRcv, 15);
		BOOST_CHECK_EQUAL(nRecvCounts, 4);
	    BOOST_CHECK_EQUAL_COLLECTIONS(rcv, rcv + 15, cmp, cmp + 15);
	    BOOST_CHECK_EQUAL_COLLECTIONS(rcvCounts, rcvCounts + 4, cmp2, cmp2 + 4);
	}
	else if(comm.rank == 2)
	{
		int buf[1] = {97};
		cupcfd::error::eCodes err = GatherV(buf, 1, &rcv, &nRcv, &rcvCounts, &nRecvCounts, 1, comm);
	}
	else if(comm.rank == 3)
	{
		int buf[6] = {76, 14, 56, 98, 90, 43};
		cupcfd::error::eCodes err = GatherV(buf, 6, &rcv, &nRcv, &rcvCounts, &nRecvCounts, 1, comm);
	}

	if(rcv != NULL && rcv != nullptr)
	{
		free(rcv);
	}

	if(rcvCounts != NULL  && rcvCounts != nullptr)
	{
		free(rcvCounts);
	}
}

// === AllGatherV ===
// Test 1: Test correct allgatherv of values using ints.
BOOST_AUTO_TEST_CASE(AllGatherV_test1)
{
    cupcfd::comm::Communicator comm(MPI_COMM_WORLD);

	int rcvCounts[4] = {3,5,1,6};
	int rcv[15] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	int cmp[15] = {65, 1054, 16, 102, 7, 98, 76, 51, 97, 76, 14, 56, 98, 90, 43};

	if(comm.rank == 0)
	{
		int buf[3] = {65, 1054, 16};

		cupcfd::error::eCodes err = AllGatherV(buf, 3, rcv, 0, rcvCounts, 0, comm);
	    BOOST_CHECK_EQUAL_COLLECTIONS(rcv, rcv + 15, cmp, cmp + 15);
	}
	else if(comm.rank == 1)
	{
		int buf[5] = {102, 7, 98, 76, 51};

		cupcfd::error::eCodes err = AllGatherV(buf, 5, rcv, 15, rcvCounts, 15, comm);
	    BOOST_CHECK_EQUAL_COLLECTIONS(rcv, rcv + 15, cmp, cmp + 15);
	}
	else if(comm.rank == 2)
	{
		int buf[1] = {97};

		cupcfd::error::eCodes err = AllGatherV(buf, 1, rcv, 0, rcvCounts, 0, comm);
	    BOOST_CHECK_EQUAL_COLLECTIONS(rcv, rcv + 15, cmp, cmp + 15);
	}
	else if(comm.rank == 3)
	{
		int buf[6] = {76, 14, 56, 98, 90, 43};

		cupcfd::error::eCodes err = AllGatherV(buf, 6, rcv, 0, rcvCounts, 0, comm);
	    BOOST_CHECK_EQUAL_COLLECTIONS(rcv, rcv + 15, cmp, cmp + 15);
	}
}

// === AllGatherV2 ===
// Test 1: Test correct allgatherv of values using ints when number of expected data elements is unknown
BOOST_AUTO_TEST_CASE(AllGatherV2_test1)
{
    cupcfd::comm::Communicator comm(MPI_COMM_WORLD);

	int * rcv = nullptr;
	int nRcv;

	int * rcvCounts = nullptr;
	int nRcvCounts;

	int cmp[15] = {65, 1054, 16, 102, 7, 98, 76, 51, 97, 76, 14, 56, 98, 90, 43};
	int rcvCountsCmp[4] = {3,5,1,6};

	if(comm.rank == 0)
	{
		int buf[3] = {65, 1054, 16};

		cupcfd::error::eCodes err = AllGatherV(buf, 3, &rcv, &nRcv, &rcvCounts, &nRcvCounts, comm);
		BOOST_CHECK_EQUAL(err, cupcfd::error::E_SUCCESS);
		BOOST_CHECK_EQUAL(nRcv, 15);
		BOOST_CHECK_EQUAL(nRcvCounts, 4);
	    BOOST_CHECK_EQUAL_COLLECTIONS(rcv, rcv + 15, cmp, cmp + 15);
	    BOOST_CHECK_EQUAL_COLLECTIONS(rcvCounts, rcvCounts + 4, rcvCountsCmp, rcvCountsCmp + 4);
	}
	else if(comm.rank == 1)
	{
		int buf[5] = {102, 7, 98, 76, 51};

		cupcfd::error::eCodes err = AllGatherV(buf, 5, &rcv, &nRcv, &rcvCounts, &nRcvCounts, comm);
		BOOST_CHECK_EQUAL(err, cupcfd::error::E_SUCCESS);
		BOOST_CHECK_EQUAL(nRcv, 15);
		BOOST_CHECK_EQUAL(nRcvCounts, 4);
	    BOOST_CHECK_EQUAL_COLLECTIONS(rcv, rcv + 15, cmp, cmp + 15);
	    BOOST_CHECK_EQUAL_COLLECTIONS(rcvCounts, rcvCounts + 4, rcvCountsCmp, rcvCountsCmp + 4);
	}
	else if(comm.rank == 2)
	{
		int buf[1] = {97};

		cupcfd::error::eCodes err = AllGatherV(buf, 1, &rcv, &nRcv, &rcvCounts, &nRcvCounts, comm);
		BOOST_CHECK_EQUAL(err, cupcfd::error::E_SUCCESS);
		BOOST_CHECK_EQUAL(nRcv, 15);
		BOOST_CHECK_EQUAL(nRcvCounts, 4);
	    BOOST_CHECK_EQUAL_COLLECTIONS(rcv, rcv + 15, cmp, cmp + 15);
	    BOOST_CHECK_EQUAL_COLLECTIONS(rcvCounts, rcvCounts + 4, rcvCountsCmp, rcvCountsCmp + 4);
	}
	else if(comm.rank == 3)
	{
		int buf[6] = {76, 14, 56, 98, 90, 43};

		cupcfd::error::eCodes err = AllGatherV(buf, 6, &rcv, &nRcv, &rcvCounts, &nRcvCounts, comm);
		BOOST_CHECK_EQUAL(err, cupcfd::error::E_SUCCESS);
		BOOST_CHECK_EQUAL(nRcv, 15);
		BOOST_CHECK_EQUAL(nRcvCounts, 4);
	    BOOST_CHECK_EQUAL_COLLECTIONS(rcv, rcv + 15, cmp, cmp + 15);
	    BOOST_CHECK_EQUAL_COLLECTIONS(rcvCounts, rcvCounts + 4, rcvCountsCmp, rcvCountsCmp + 4);
	}
}

// Test 2: Test Error Check when pointers for setup are not provided as nullptrs

BOOST_AUTO_TEST_CASE(cleanup)
{
    // Cleanup MPI Environment
    MPI_Finalize();
}
