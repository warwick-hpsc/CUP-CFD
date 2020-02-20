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

#define BOOST_TEST_MODULE ReduceMPI
#include <boost/test/unit_test.hpp>
#include <boost/test/output_test_stream.hpp>

#include <stdexcept>
#include "Communicator.h"
#include "ReduceMPI.h"

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

// === reduceMPISum ===
// Test 1: Perform a sum on integers
BOOST_AUTO_TEST_CASE(reduceMPISum_test1)
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
		cupcfd::error::eCodes err = reduceMPISum(bSend, nSend, rcv, nRecv, root, comm.comm);

		BOOST_CHECK_EQUAL(err, cupcfd::error::E_SUCCESS);
	    BOOST_CHECK_EQUAL_COLLECTIONS(rcv, rcv + 4, cmp, cmp + 4);// This is not the root rank, so the total should not have been overwritten
	}
	else if(comm.rank == 1)
	{
		int bSend[4] = {43, 52, 4, 82};
		int cmp[4] = {354, 358, 464, 676};
		cupcfd::error::eCodes err = reduceMPISum(bSend, nSend, rcv, nRecv, root, comm.comm);

		BOOST_CHECK_EQUAL(err, cupcfd::error::E_SUCCESS);
		BOOST_CHECK_EQUAL_COLLECTIONS(rcv, rcv + 4, cmp, cmp + 4); // This is the root rank, so total should be updated
	}
	else if(comm.rank == 2)
	{
		int bSend[4] = {2, 3, 4, 5};
		int cmp[4] = {0, 0, 0, 0};
		cupcfd::error::eCodes err = reduceMPISum(bSend, nSend, rcv, nRecv, root, comm.comm);

		BOOST_CHECK_EQUAL(err, cupcfd::error::E_SUCCESS);
		BOOST_CHECK_EQUAL_COLLECTIONS(rcv, rcv + 4, cmp, cmp + 4);// This is not the root rank, so the total should not have been overwritten
	}
	else if(comm.rank == 3)
	{
		int bSend[4] = {200, 300, 400, 500};
		int cmp[4] = {0, 0, 0, 0};
		cupcfd::error::eCodes err = reduceMPISum(bSend, nSend, rcv, nRecv, root, comm.comm);

		BOOST_CHECK_EQUAL(err, cupcfd::error::E_SUCCESS);
		BOOST_CHECK_EQUAL_COLLECTIONS(rcv, rcv + 4, cmp, cmp + 4); // This is not the root rank, so the total should not have been overwritten
	}
}

// Test 2: CustomMPIType
BOOST_AUTO_TEST_CASE(reduceMPISum_test2)
{

}

// Test 3: Error - CustomMPIType with Unregistered Type
BOOST_AUTO_TEST_CASE(reduceMPISum_test3)
{

}

// === reduceMPIMin ===
// Test 1: Perform a min on integers
BOOST_AUTO_TEST_CASE(reduceMPIMin_test1)
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
		cupcfd::error::eCodes err = reduceMPIMin(bSend, nSend, rcv, nRecv, root, comm.comm);

		BOOST_CHECK_EQUAL(err, cupcfd::error::E_SUCCESS);
	    BOOST_CHECK_EQUAL_COLLECTIONS(rcv, rcv + 4, cmp, cmp + 4);// This is not the root rank, so the total should not have been overwritten
	}
	else if(comm.rank == 1)
	{
		int bSend[4] = {43, 52, 4, 82};
		int cmp[4] = {2, 3, 4, 82};
		cupcfd::error::eCodes err = reduceMPIMin(bSend, nSend, rcv, nRecv, root, comm.comm);

		BOOST_CHECK_EQUAL(err, cupcfd::error::E_SUCCESS);
		BOOST_CHECK_EQUAL_COLLECTIONS(rcv, rcv + 4, cmp, cmp + 4); // This is the root rank, so total should be updated
	}
	else if(comm.rank == 2)
	{
		int bSend[4] = {2, 3, 906, 321};
		int cmp[4] = {0, 0, 0, 0};
		cupcfd::error::eCodes err = reduceMPIMin(bSend, nSend, rcv, nRecv, root, comm.comm);

		BOOST_CHECK_EQUAL(err, cupcfd::error::E_SUCCESS);
		BOOST_CHECK_EQUAL_COLLECTIONS(rcv, rcv + 4, cmp, cmp + 4);// This is not the root rank, so the total should not have been overwritten
	}
	else if(comm.rank == 3)
	{
		int bSend[4] = {200, 300, 400, 500};
		int cmp[4] = {0, 0, 0, 0};
		cupcfd::error::eCodes err = reduceMPIMin(bSend, nSend, rcv, nRecv, root, comm.comm);

		BOOST_CHECK_EQUAL(err, cupcfd::error::E_SUCCESS);
		BOOST_CHECK_EQUAL_COLLECTIONS(rcv, rcv + 4, cmp, cmp + 4); // This is not the root rank, so the total should not have been overwritten
	}
}

// Test 2: CustomMPIType
BOOST_AUTO_TEST_CASE(reduceMPIMin_test2)
{

}

// Test 3: Error - CustomMPIType with Unregistered Type
BOOST_AUTO_TEST_CASE(reduceMPIMin_test3)
{

}

// === reduceMPIMax ===
BOOST_AUTO_TEST_CASE(reduceMPIMax_test1)
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
		cupcfd::error::eCodes err = reduceMPIMax(bSend, nSend, rcv, nRecv, root, comm.comm);

		BOOST_CHECK_EQUAL(err, cupcfd::error::E_SUCCESS);
	    BOOST_CHECK_EQUAL_COLLECTIONS(rcv, rcv + 4, cmp, cmp + 4);// This is not the root rank, so the total should not have been overwritten
	}
	else if(comm.rank == 1)
	{
		int bSend[4] = {43, 52, 4, 82};
		int cmp[4] = {200, 300, 908, 1042};
		cupcfd::error::eCodes err = reduceMPIMax(bSend, nSend, rcv, nRecv, root, comm.comm);

		BOOST_CHECK_EQUAL(err, cupcfd::error::E_SUCCESS);
		BOOST_CHECK_EQUAL_COLLECTIONS(rcv, rcv + 4, cmp, cmp + 4); // This is the root rank, so total should be updated
	}
	else if(comm.rank == 2)
	{
		int bSend[4] = {2, 3, 4, 1042};
		int cmp[4] = {0, 0, 0, 0};
		cupcfd::error::eCodes err = reduceMPIMax(bSend, nSend, rcv, nRecv, root, comm.comm);

		BOOST_CHECK_EQUAL(err, cupcfd::error::E_SUCCESS);
		BOOST_CHECK_EQUAL_COLLECTIONS(rcv, rcv + 4, cmp, cmp + 4);// This is not the root rank, so the total should not have been overwritten
	}
	else if(comm.rank == 3)
	{
		int bSend[4] = {200, 300, 400, 500};
		int cmp[4] = {0, 0, 0, 0};
		cupcfd::error::eCodes err = reduceMPIMax(bSend, nSend, rcv, nRecv, root, comm.comm);

		BOOST_CHECK_EQUAL(err, cupcfd::error::E_SUCCESS);
		BOOST_CHECK_EQUAL_COLLECTIONS(rcv, rcv + 4, cmp, cmp + 4); // This is not the root rank, so the total should not have been overwritten
	}
}

// Test 2: CustomMPIType
BOOST_AUTO_TEST_CASE(reduceMPIMax_test2)
{

}

// Test 3: Error - CustomMPIType with Unregistered Type
BOOST_AUTO_TEST_CASE(reduceMPIMax_test3)
{

}

// === reduceMPIProduct ===
BOOST_AUTO_TEST_CASE(reduceMPIProduct_test1)
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
		cupcfd::error::eCodes err = reduceMPIProduct(bSend, nSend, rcv, nRecv, root, comm.comm);

		BOOST_CHECK_EQUAL(err, cupcfd::error::E_SUCCESS);
	    BOOST_CHECK_EQUAL_COLLECTIONS(rcv, rcv + 4, cmp, cmp + 4);// This is not the root rank, so the total should not have been overwritten
	}
	else if(comm.rank == 1)
	{
		int bSend[4] = {43, 52, 4, 82};
		int cmp[4] = {1874800, 140400, 358400, 18245000};
		cupcfd::error::eCodes err = reduceMPIProduct(bSend, nSend, rcv, nRecv, root, comm.comm);

		BOOST_CHECK_EQUAL(err, cupcfd::error::E_SUCCESS);
		BOOST_CHECK_EQUAL_COLLECTIONS(rcv, rcv + 4, cmp, cmp + 4); // This is the root rank, so total should be updated
	}
	else if(comm.rank == 2)
	{
		int bSend[4] = {2, 3, 4, 5};
		int cmp[4] = {0, 0, 0, 0};
		cupcfd::error::eCodes err = reduceMPIProduct(bSend, nSend, rcv, nRecv, root, comm.comm);

		BOOST_CHECK_EQUAL(err, cupcfd::error::E_SUCCESS);
		BOOST_CHECK_EQUAL_COLLECTIONS(rcv, rcv + 4, cmp, cmp + 4);// This is not the root rank, so the total should not have been overwritten
	}
	else if(comm.rank == 3)
	{
		int bSend[4] = {200, 300, 400, 500};
		int cmp[4] = {0, 0, 0, 0};
		cupcfd::error::eCodes err = reduceMPIProduct(bSend, nSend, rcv, nRecv, root, comm.comm);

		BOOST_CHECK_EQUAL(err, cupcfd::error::E_SUCCESS);
		BOOST_CHECK_EQUAL_COLLECTIONS(rcv, rcv + 4, cmp, cmp + 4); // This is not the root rank, so the total should not have been overwritten
	}
}

// Test 2: CustomMPIType
BOOST_AUTO_TEST_CASE(reduceMPIProduct_test2)
{

}

// Test 3: Error - CustomMPIType with Unregistered Type
BOOST_AUTO_TEST_CASE(reduceMPIProduct_test3)
{

}

// === reduceMPI ===
// This is currently tested via the use of the other tests (since they internally call this method)

// === allReduceMPISum ===
BOOST_AUTO_TEST_CASE(allReduceMPISum_test1)
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
		cupcfd::error::eCodes err = allReduceMPISum(bSend, nSend, rcv, nRecv, comm.comm);

		BOOST_CHECK_EQUAL(err, cupcfd::error::E_SUCCESS);
	    BOOST_CHECK_EQUAL_COLLECTIONS(rcv, rcv + 4, cmp, cmp + 4);
	}
	else if(comm.rank == 1)
	{
		int bSend[4] = {43, 52, 4, 82};
		cupcfd::error::eCodes err = allReduceMPISum(bSend, nSend, rcv, nRecv, comm.comm);

		BOOST_CHECK_EQUAL(err, cupcfd::error::E_SUCCESS);
		BOOST_CHECK_EQUAL_COLLECTIONS(rcv, rcv + 4, cmp, cmp + 4);
	}
	else if(comm.rank == 2)
	{
		int bSend[4] = {2, 3, 4, 5};
		cupcfd::error::eCodes err = allReduceMPISum(bSend, nSend, rcv, nRecv, comm.comm);

		BOOST_CHECK_EQUAL(err, cupcfd::error::E_SUCCESS);
		BOOST_CHECK_EQUAL_COLLECTIONS(rcv, rcv + 4, cmp, cmp + 4);
	}
	else if(comm.rank == 3)
	{
		int bSend[4] = {200, 300, 400, 500};
		cupcfd::error::eCodes err = allReduceMPISum(bSend, nSend, rcv, nRecv, comm.comm);

		BOOST_CHECK_EQUAL(err, cupcfd::error::E_SUCCESS);
		BOOST_CHECK_EQUAL_COLLECTIONS(rcv, rcv + 4, cmp, cmp + 4);
	}
}

// Test 2: CustomMPIType
BOOST_AUTO_TEST_CASE(allReduceMPISum_test2)
{

}

// Test 3: Error - CustomMPIType with Unregistered Type
BOOST_AUTO_TEST_CASE(allReduceMPISum_test3)
{

}

// === allReduceMPIMin ===
BOOST_AUTO_TEST_CASE(allReduceMPIMin_test1)
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
		cupcfd::error::eCodes err = allReduceMPIMin(bSend, nSend, rcv, nRecv, comm.comm);

		BOOST_CHECK_EQUAL(err, cupcfd::error::E_SUCCESS);
	    BOOST_CHECK_EQUAL_COLLECTIONS(rcv, rcv + 4, cmp, cmp + 4);// This is not the root rank, so the total should not have been overwritten
	}
	else if(comm.rank == 1)
	{
		int bSend[4] = {43, 52, 4, 82};
		cupcfd::error::eCodes err = allReduceMPIMin(bSend, nSend, rcv, nRecv, comm.comm);

		BOOST_CHECK_EQUAL(err, cupcfd::error::E_SUCCESS);
		BOOST_CHECK_EQUAL_COLLECTIONS(rcv, rcv + 4, cmp, cmp + 4); // This is the root rank, so total should be updated
	}
	else if(comm.rank == 2)
	{
		int bSend[4] = {2, 3, 906, 321};
		cupcfd::error::eCodes err = allReduceMPIMin(bSend, nSend, rcv, nRecv, comm.comm);

		BOOST_CHECK_EQUAL(err, cupcfd::error::E_SUCCESS);
		BOOST_CHECK_EQUAL_COLLECTIONS(rcv, rcv + 4, cmp, cmp + 4);// This is not the root rank, so the total should not have been overwritten
	}
	else if(comm.rank == 3)
	{
		int bSend[4] = {200, 300, 400, 500};
		cupcfd::error::eCodes err = allReduceMPIMin(bSend, nSend, rcv, nRecv, comm.comm);

		BOOST_CHECK_EQUAL(err, cupcfd::error::E_SUCCESS);
		BOOST_CHECK_EQUAL_COLLECTIONS(rcv, rcv + 4, cmp, cmp + 4); // This is not the root rank, so the total should not have been overwritten
	}
}

// Test 2: CustomMPIType
BOOST_AUTO_TEST_CASE(allReduceMPIMin_test2)
{

}

// Test 3: Error - CustomMPIType with Unregistered Type
BOOST_AUTO_TEST_CASE(allReduceMPIMin_test3)
{

}

// === allReduceMPIMax ===
BOOST_AUTO_TEST_CASE(allReduceMPIMax_test1)
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
		cupcfd::error::eCodes err = allReduceMPIMax(bSend, nSend, rcv, nRecv, comm.comm);

		BOOST_CHECK_EQUAL(err, cupcfd::error::E_SUCCESS);
	    BOOST_CHECK_EQUAL_COLLECTIONS(rcv, rcv + 4, cmp, cmp + 4);// This is not the root rank, so the total should not have been overwritten
	}
	else if(comm.rank == 1)
	{
		int bSend[4] = {43, 52, 4, 82};
		cupcfd::error::eCodes err = allReduceMPIMax(bSend, nSend, rcv, nRecv, comm.comm);

		BOOST_CHECK_EQUAL(err, cupcfd::error::E_SUCCESS);
		BOOST_CHECK_EQUAL_COLLECTIONS(rcv, rcv + 4, cmp, cmp + 4); // This is the root rank, so total should be updated
	}
	else if(comm.rank == 2)
	{
		int bSend[4] = {2, 3, 4, 1042};
		cupcfd::error::eCodes err = allReduceMPIMax(bSend, nSend, rcv, nRecv, comm.comm);

		BOOST_CHECK_EQUAL(err, cupcfd::error::E_SUCCESS);
		BOOST_CHECK_EQUAL_COLLECTIONS(rcv, rcv + 4, cmp, cmp + 4);// This is not the root rank, so the total should not have been overwritten
	}
	else if(comm.rank == 3)
	{
		int bSend[4] = {200, 300, 400, 500};
		cupcfd::error::eCodes err = allReduceMPIMax(bSend, nSend, rcv, nRecv, comm.comm);

		BOOST_CHECK_EQUAL(err, cupcfd::error::E_SUCCESS);
		BOOST_CHECK_EQUAL_COLLECTIONS(rcv, rcv + 4, cmp, cmp + 4); // This is not the root rank, so the total should not have been overwritten
	}
}

// Test 2: CustomMPIType
BOOST_AUTO_TEST_CASE(allReduceMPIMax_test2)
{

}

// Test 3: Error - CustomMPIType with Unregistered Type
BOOST_AUTO_TEST_CASE(allReduceMPIMax_test3)
{

}

// === allReduceMPIProduct ===
BOOST_AUTO_TEST_CASE(allReduceMPIProduct_test1)
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
		cupcfd::error::eCodes err = allReduceMPIProduct(bSend, nSend, rcv, nRecv, comm.comm);

		BOOST_CHECK_EQUAL(err, cupcfd::error::E_SUCCESS);
	    BOOST_CHECK_EQUAL_COLLECTIONS(rcv, rcv + 4, cmp, cmp + 4);// This is not the root rank, so the total should not have been overwritten
	}
	else if(comm.rank == 1)
	{
		int bSend[4] = {43, 52, 4, 82};
		cupcfd::error::eCodes err = allReduceMPIProduct(bSend, nSend, rcv, nRecv, comm.comm);

		BOOST_CHECK_EQUAL(err, cupcfd::error::E_SUCCESS);
		BOOST_CHECK_EQUAL_COLLECTIONS(rcv, rcv + 4, cmp, cmp + 4); // This is the root rank, so total should be updated
	}
	else if(comm.rank == 2)
	{
		int bSend[4] = {2, 3, 4, 5};
		cupcfd::error::eCodes err = allReduceMPIProduct(bSend, nSend, rcv, nRecv, comm.comm);

		BOOST_CHECK_EQUAL(err, cupcfd::error::E_SUCCESS);
		BOOST_CHECK_EQUAL_COLLECTIONS(rcv, rcv + 4, cmp, cmp + 4);// This is not the root rank, so the total should not have been overwritten
	}
	else if(comm.rank == 3)
	{
		int bSend[4] = {200, 300, 400, 500};
		cupcfd::error::eCodes err = allReduceMPIProduct(bSend, nSend, rcv, nRecv, comm.comm);

		BOOST_CHECK_EQUAL(err, cupcfd::error::E_SUCCESS);
		BOOST_CHECK_EQUAL_COLLECTIONS(rcv, rcv + 4, cmp, cmp + 4); // This is not the root rank, so the total should not have been overwritten
	}
}

// Test 2: CustomMPIType
BOOST_AUTO_TEST_CASE(allReduceMPIProduct_test2)
{

}

// Test 3: Error - CustomMPIType with Unregistered Type
BOOST_AUTO_TEST_CASE(allReduceMPIProduct_test3)
{

}

// === allReduceMPI ===
// This is currently tested via the use of the other tests (since they internally call this method)

// Cleanup MPI Environment
BOOST_AUTO_TEST_CASE(cleanup)
{
    MPI_Finalize();
}
