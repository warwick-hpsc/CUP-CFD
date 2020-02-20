/*
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Test the Reduce Interface Operations
 */

#define BOOST_TEST_MODULE Reduce
#include <boost/test/unit_test.hpp>
#include <boost/test/output_test_stream.hpp>

#include <stdexcept>

#include "Communicator.h"
#include "Reduce.h"

BOOST_AUTO_TEST_CASE(setup)
{
    int argc = boost::unit_test::framework::master_test_suite().argc;
    char ** argv = boost::unit_test::framework::master_test_suite().argv;
    MPI_Init(&argc, &argv);
}

using namespace cupcfd::comm;

// === reduceAdd ===
// Test 1: Test Correct Functionality using ints
BOOST_AUTO_TEST_CASE(reduceAdd_test1)
{
    cupcfd::comm::Communicator comm(MPI_COMM_WORLD);

    int root = 1;
	int nSend = 4;
	int nRecv = 4;
    int rcv[4] = {0, 0, 0, 0};

	if(comm.rank == 0)
	{
		int bSend[4] = {109, 3, 56, 89};
		int cmp[4] = {0, 0, 0, 0};
		cupcfd::error::eCodes err = reduceAdd(bSend, nSend, rcv, nRecv, root, comm);

		BOOST_CHECK_EQUAL(err, cupcfd::error::E_SUCCESS);
	    BOOST_CHECK_EQUAL_COLLECTIONS(rcv, rcv + 4, cmp, cmp + 4);// This is not the root rank, so the total should not have been overwritten
	}
	else if(comm.rank == 1)
	{
		int bSend[4] = {43, 52, 4, 82};
		int cmp[4] = {354, 358, 464, 676};
		cupcfd::error::eCodes err = reduceAdd(bSend, nSend, rcv, nRecv, root, comm);

		BOOST_CHECK_EQUAL(err, cupcfd::error::E_SUCCESS);
		BOOST_CHECK_EQUAL_COLLECTIONS(rcv, rcv + 4, cmp, cmp + 4); // This is the root rank, so total should be updated
	}
	else if(comm.rank == 2)
	{
		int bSend[4] = {2, 3, 4, 5};
		int cmp[4] = {0, 0, 0, 0};
		cupcfd::error::eCodes err = reduceAdd(bSend, nSend, rcv, nRecv, root, comm);

		BOOST_CHECK_EQUAL(err, cupcfd::error::E_SUCCESS);
		BOOST_CHECK_EQUAL_COLLECTIONS(rcv, rcv + 4, cmp, cmp + 4);// This is not the root rank, so the total should not have been overwritten
	}
	else if(comm.rank == 3)
	{
		int bSend[4] = {200, 300, 400, 500};
		int cmp[4] = {0, 0, 0, 0};
		cupcfd::error::eCodes err = reduceAdd(bSend, nSend, rcv, nRecv, root, comm);

		BOOST_CHECK_EQUAL(err, cupcfd::error::E_SUCCESS);
		BOOST_CHECK_EQUAL_COLLECTIONS(rcv, rcv + 4, cmp, cmp + 4); // This is not the root rank, so the total should not have been overwritten
	}
}

// === reduceMultiply ===
// Test 1: Test Correct Functionality using ints
BOOST_AUTO_TEST_CASE(reduceMultiply_test1)
{
    cupcfd::comm::Communicator comm(MPI_COMM_WORLD);

    int root = 1;
	int nSend = 4;
	int nRecv = 4;
    int rcv[4] = {0, 0, 0, 0};

	if(comm.rank == 0)
	{
		int bSend[4] = {109, 3, 56, 89};
		int cmp[4] = {0, 0, 0, 0};
		cupcfd::error::eCodes err = reduceMultiply(bSend, nSend, rcv, nRecv, root, comm);

		BOOST_CHECK_EQUAL(err, cupcfd::error::E_SUCCESS);
	    BOOST_CHECK_EQUAL_COLLECTIONS(rcv, rcv + 4, cmp, cmp + 4);// This is not the root rank, so the total should not have been overwritten
	}
	else if(comm.rank == 1)
	{
		int bSend[4] = {43, 52, 4, 82};
		int cmp[4] = {1874800, 140400, 358400, 18245000};
		cupcfd::error::eCodes err = reduceMultiply(bSend, nSend, rcv, nRecv, root, comm);

		BOOST_CHECK_EQUAL(err, cupcfd::error::E_SUCCESS);
		BOOST_CHECK_EQUAL_COLLECTIONS(rcv, rcv + 4, cmp, cmp + 4); // This is the root rank, so total should be updated
	}
	else if(comm.rank == 2)
	{
		int bSend[4] = {2, 3, 4, 5};
		int cmp[4] = {0, 0, 0, 0};
		cupcfd::error::eCodes err = reduceMultiply(bSend, nSend, rcv, nRecv, root, comm);

		BOOST_CHECK_EQUAL(err, cupcfd::error::E_SUCCESS);
		BOOST_CHECK_EQUAL_COLLECTIONS(rcv, rcv + 4, cmp, cmp + 4);// This is not the root rank, so the total should not have been overwritten
	}
	else if(comm.rank == 3)
	{
		int bSend[4] = {200, 300, 400, 500};
		int cmp[4] = {0, 0, 0, 0};
		cupcfd::error::eCodes err = reduceMultiply(bSend, nSend, rcv, nRecv, root, comm);

		BOOST_CHECK_EQUAL(err, cupcfd::error::E_SUCCESS);
		BOOST_CHECK_EQUAL_COLLECTIONS(rcv, rcv + 4, cmp, cmp + 4); // This is not the root rank, so the total should not have been overwritten
	}
}

// === reduceMin ===
// Test 1: Test Correct Functionality using ints
BOOST_AUTO_TEST_CASE(reduceMin_test1)
{
    cupcfd::comm::Communicator comm(MPI_COMM_WORLD);

    int root = 1;
	int nSend = 4;
	int nRecv = 4;
    int rcv[4] = {0, 0, 0, 0};

	if(comm.rank == 0)
	{
		int bSend[4] = {109, 3, 908, 89};
		int cmp[4] = {0, 0, 0, 0};
		cupcfd::error::eCodes err = reduceMin(bSend, nSend, rcv, nRecv, root, comm);

		BOOST_CHECK_EQUAL(err, cupcfd::error::E_SUCCESS);
	    BOOST_CHECK_EQUAL_COLLECTIONS(rcv, rcv + 4, cmp, cmp + 4);// This is not the root rank, so the total should not have been overwritten
	}
	else if(comm.rank == 1)
	{
		int bSend[4] = {43, 52, 4, 82};
		int cmp[4] = {2, 3, 4, 82};
		cupcfd::error::eCodes err = reduceMin(bSend, nSend, rcv, nRecv, root, comm);

		BOOST_CHECK_EQUAL(err, cupcfd::error::E_SUCCESS);
		BOOST_CHECK_EQUAL_COLLECTIONS(rcv, rcv + 4, cmp, cmp + 4); // This is the root rank, so total should be updated
	}
	else if(comm.rank == 2)
	{
		int bSend[4] = {2, 3, 4, 1042};
		int cmp[4] = {0, 0, 0, 0};
		cupcfd::error::eCodes err = reduceMin(bSend, nSend, rcv, nRecv, root, comm);

		BOOST_CHECK_EQUAL(err, cupcfd::error::E_SUCCESS);
		BOOST_CHECK_EQUAL_COLLECTIONS(rcv, rcv + 4, cmp, cmp + 4);// This is not the root rank, so the total should not have been overwritten
	}
	else if(comm.rank == 3)
	{
		int bSend[4] = {200, 300, 400, 500};
		int cmp[4] = {0, 0, 0, 0};
		cupcfd::error::eCodes err = reduceMin(bSend, nSend, rcv, nRecv, root, comm);

		BOOST_CHECK_EQUAL(err, cupcfd::error::E_SUCCESS);
		BOOST_CHECK_EQUAL_COLLECTIONS(rcv, rcv + 4, cmp, cmp + 4); // This is not the root rank, so the total should not have been overwritten
	}
}

// === reduceMax ===
// Test 1: Test Correct Functionality using ints
BOOST_AUTO_TEST_CASE(reduceMax_test1)
{
    cupcfd::comm::Communicator comm(MPI_COMM_WORLD);

    int root = 1;
	int nSend = 4;
	int nRecv = 4;
    int rcv[4] = {0, 0, 0, 0};

	if(comm.rank == 0)
	{
		int bSend[4] = {109, 3, 908, 89};
		int cmp[4] = {0, 0, 0, 0};
		cupcfd::error::eCodes err = reduceMax(bSend, nSend, rcv, nRecv, root, comm);

		BOOST_CHECK_EQUAL(err, cupcfd::error::E_SUCCESS);
	    BOOST_CHECK_EQUAL_COLLECTIONS(rcv, rcv + 4, cmp, cmp + 4);// This is not the root rank, so the total should not have been overwritten
	}
	else if(comm.rank == 1)
	{
		int bSend[4] = {43, 52, 4, 82};
		int cmp[4] = {200, 300, 908, 1042};
		cupcfd::error::eCodes err = reduceMax(bSend, nSend, rcv, nRecv, root, comm);

		BOOST_CHECK_EQUAL(err, cupcfd::error::E_SUCCESS);
		BOOST_CHECK_EQUAL_COLLECTIONS(rcv, rcv + 4, cmp, cmp + 4); // This is the root rank, so total should be updated
	}
	else if(comm.rank == 2)
	{
		int bSend[4] = {2, 3, 4, 1042};
		int cmp[4] = {0, 0, 0, 0};
		cupcfd::error::eCodes err = reduceMax(bSend, nSend, rcv, nRecv, root, comm);

		BOOST_CHECK_EQUAL(err, cupcfd::error::E_SUCCESS);
		BOOST_CHECK_EQUAL_COLLECTIONS(rcv, rcv + 4, cmp, cmp + 4);// This is not the root rank, so the total should not have been overwritten
	}
	else if(comm.rank == 3)
	{
		int bSend[4] = {200, 300, 400, 500};
		int cmp[4] = {0, 0, 0, 0};
		cupcfd::error::eCodes err = reduceMax(bSend, nSend, rcv, nRecv, root, comm);

		BOOST_CHECK_EQUAL(err, cupcfd::error::E_SUCCESS);
		BOOST_CHECK_EQUAL_COLLECTIONS(rcv, rcv + 4, cmp, cmp + 4); // This is not the root rank, so the total should not have been overwritten
	}
}

// === allReduceAdd ===
// Test 1: Test Correct Functionality using ints
BOOST_AUTO_TEST_CASE(allReduceAdd_test1)
{
    cupcfd::comm::Communicator comm(MPI_COMM_WORLD);

    int root = 1;
	int nSend = 4;
	int nRecv = 4;
    int rcv[4] = {0, 0, 0, 0};
	int cmp[4] = {354, 358, 464, 676};

	if(comm.rank == 0)
	{
		int bSend[4] = {109, 3, 56, 89};
		cupcfd::error::eCodes err = allReduceAdd(bSend, nSend, rcv, nRecv, comm);

		BOOST_CHECK_EQUAL(err, cupcfd::error::E_SUCCESS);
	    BOOST_CHECK_EQUAL_COLLECTIONS(rcv, rcv + 4, cmp, cmp + 4);// This is not the root rank, so the total should not have been overwritten
	}
	else if(comm.rank == 1)
	{
		int bSend[4] = {43, 52, 4, 82};
		cupcfd::error::eCodes err = allReduceAdd(bSend, nSend, rcv, nRecv, comm);

		BOOST_CHECK_EQUAL(err, cupcfd::error::E_SUCCESS);
		BOOST_CHECK_EQUAL_COLLECTIONS(rcv, rcv + 4, cmp, cmp + 4); // This is the root rank, so total should be updated
	}
	else if(comm.rank == 2)
	{
		int bSend[4] = {2, 3, 4, 5};
		cupcfd::error::eCodes err = allReduceAdd(bSend, nSend, rcv, nRecv, comm);

		BOOST_CHECK_EQUAL(err, cupcfd::error::E_SUCCESS);
		BOOST_CHECK_EQUAL_COLLECTIONS(rcv, rcv + 4, cmp, cmp + 4);// This is not the root rank, so the total should not have been overwritten
	}
	else if(comm.rank == 3)
	{
		int bSend[4] = {200, 300, 400, 500};
		cupcfd::error::eCodes err = allReduceAdd(bSend, nSend, rcv, nRecv, comm);

		BOOST_CHECK_EQUAL(err, cupcfd::error::E_SUCCESS);
		BOOST_CHECK_EQUAL_COLLECTIONS(rcv, rcv + 4, cmp, cmp + 4); // This is not the root rank, so the total should not have been overwritten
	}
}


// === allReduceMultiply ===
// Test 1: Test Correct Functionality using ints
BOOST_AUTO_TEST_CASE(allReduceMultiply_test1)
{
    cupcfd::comm::Communicator comm(MPI_COMM_WORLD);

    int root = 1;
	int nSend = 4;
	int nRecv = 4;
    int rcv[4] = {0, 0, 0, 0};
	int cmp[4] = {1874800, 140400, 358400, 18245000};

	if(comm.rank == 0)
	{
		int bSend[4] = {109, 3, 56, 89};
		cupcfd::error::eCodes err = allReduceMultiply(bSend, nSend, rcv, nRecv, comm);

		BOOST_CHECK_EQUAL(err, cupcfd::error::E_SUCCESS);
	    BOOST_CHECK_EQUAL_COLLECTIONS(rcv, rcv + 4, cmp, cmp + 4);// This is not the root rank, so the total should not have been overwritten
	}
	else if(comm.rank == 1)
	{
		int bSend[4] = {43, 52, 4, 82};
		cupcfd::error::eCodes err = allReduceMultiply(bSend, nSend, rcv, nRecv, comm);

		BOOST_CHECK_EQUAL(err, cupcfd::error::E_SUCCESS);
		BOOST_CHECK_EQUAL_COLLECTIONS(rcv, rcv + 4, cmp, cmp + 4); // This is the root rank, so total should be updated
	}
	else if(comm.rank == 2)
	{
		int bSend[4] = {2, 3, 4, 5};
		cupcfd::error::eCodes err = allReduceMultiply(bSend, nSend, rcv, nRecv, comm);

		BOOST_CHECK_EQUAL(err, cupcfd::error::E_SUCCESS);
		BOOST_CHECK_EQUAL_COLLECTIONS(rcv, rcv + 4, cmp, cmp + 4);// This is not the root rank, so the total should not have been overwritten
	}
	else if(comm.rank == 3)
	{
		int bSend[4] = {200, 300, 400, 500};
		cupcfd::error::eCodes err = allReduceMultiply(bSend, nSend, rcv, nRecv, comm);

		BOOST_CHECK_EQUAL(err, cupcfd::error::E_SUCCESS);
		BOOST_CHECK_EQUAL_COLLECTIONS(rcv, rcv + 4, cmp, cmp + 4); // This is not the root rank, so the total should not have been overwritten
	}
}

// === allReduceMin ===
// Test 1: Test Correct Functionality using ints
BOOST_AUTO_TEST_CASE(allReduceMin_test1)
{
    cupcfd::comm::Communicator comm(MPI_COMM_WORLD);

    int root = 1;
	int nSend = 4;
	int nRecv = 4;
    int rcv[4] = {0, 0, 0, 0};
	int cmp[4] = {2, 3, 4, 82};

	if(comm.rank == 0)
	{
		int bSend[4] = {109, 3, 56, 89};
		cupcfd::error::eCodes err = allReduceMin(bSend, nSend, rcv, nRecv, comm);

		BOOST_CHECK_EQUAL(err, cupcfd::error::E_SUCCESS);
	    BOOST_CHECK_EQUAL_COLLECTIONS(rcv, rcv + 4, cmp, cmp + 4);// This is not the root rank, so the total should not have been overwritten
	}
	else if(comm.rank == 1)
	{
		int bSend[4] = {43, 52, 4, 82};
		cupcfd::error::eCodes err = allReduceMin(bSend, nSend, rcv, nRecv, comm);

		BOOST_CHECK_EQUAL(err, cupcfd::error::E_SUCCESS);
		BOOST_CHECK_EQUAL_COLLECTIONS(rcv, rcv + 4, cmp, cmp + 4); // This is the root rank, so total should be updated
	}
	else if(comm.rank == 2)
	{
		int bSend[4] = {2, 3, 906, 321};
		cupcfd::error::eCodes err = allReduceMin(bSend, nSend, rcv, nRecv, comm);

		BOOST_CHECK_EQUAL(err, cupcfd::error::E_SUCCESS);
		BOOST_CHECK_EQUAL_COLLECTIONS(rcv, rcv + 4, cmp, cmp + 4);// This is not the root rank, so the total should not have been overwritten
	}
	else if(comm.rank == 3)
	{
		int bSend[4] = {200, 300, 400, 500};
		cupcfd::error::eCodes err = allReduceMin(bSend, nSend, rcv, nRecv, comm);

		BOOST_CHECK_EQUAL(err, cupcfd::error::E_SUCCESS);
		BOOST_CHECK_EQUAL_COLLECTIONS(rcv, rcv + 4, cmp, cmp + 4); // This is not the root rank, so the total should not have been overwritten
	}
}

// === allReduceMax ===
// Test 1: Test Correct Functionality using ints
BOOST_AUTO_TEST_CASE(allReduceMax_test1)
{
    cupcfd::comm::Communicator comm(MPI_COMM_WORLD);

    int root = 1;
	int nSend = 4;
	int nRecv = 4;
    int rcv[4] = {0, 0, 0, 0};
	int cmp[4] = {200, 300, 908, 1042};

	if(comm.rank == 0)
	{
		int bSend[4] = {109, 3, 908, 89};
		cupcfd::error::eCodes err = allReduceMax(bSend, nSend, rcv, nRecv, comm);

		BOOST_CHECK_EQUAL(err, cupcfd::error::E_SUCCESS);
	    BOOST_CHECK_EQUAL_COLLECTIONS(rcv, rcv + 4, cmp, cmp + 4);// This is not the root rank, so the total should not have been overwritten
	}
	else if(comm.rank == 1)
	{
		int bSend[4] = {43, 52, 4, 82};
		cupcfd::error::eCodes err = allReduceMax(bSend, nSend, rcv, nRecv, comm);

		BOOST_CHECK_EQUAL(err, cupcfd::error::E_SUCCESS);
		BOOST_CHECK_EQUAL_COLLECTIONS(rcv, rcv + 4, cmp, cmp + 4); // This is the root rank, so total should be updated
	}
	else if(comm.rank == 2)
	{
		int bSend[4] = {2, 3, 4, 1042};
		cupcfd::error::eCodes err = allReduceMax(bSend, nSend, rcv, nRecv, comm);

		BOOST_CHECK_EQUAL(err, cupcfd::error::E_SUCCESS);
		BOOST_CHECK_EQUAL_COLLECTIONS(rcv, rcv + 4, cmp, cmp + 4);// This is not the root rank, so the total should not have been overwritten
	}
	else if(comm.rank == 3)
	{
		int bSend[4] = {200, 300, 400, 500};
		cupcfd::error::eCodes err = allReduceMax(bSend, nSend, rcv, nRecv, comm);

		BOOST_CHECK_EQUAL(err, cupcfd::error::E_SUCCESS);
		BOOST_CHECK_EQUAL_COLLECTIONS(rcv, rcv + 4, cmp, cmp + 4); // This is not the root rank, so the total should not have been overwritten
	}
}

BOOST_AUTO_TEST_CASE(cleanup)
{
    // Cleanup MPI Environment
    MPI_Finalize();
}
