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

#define BOOST_TEST_MODULE ExchangeMPI
#include <boost/test/unit_test.hpp>
#include <boost/test/output_test_stream.hpp>

#include <stdexcept>
#include "ExchangeMPI.h"
#include "mpi.h"
#include <iostream>
#include "Communicator.h"
#include "WaitallMPI.h"

// Setup MPI
BOOST_AUTO_TEST_CASE(setup)
{
    int argc = boost::unit_test::framework::master_test_suite().argc;
    char ** argv = boost::unit_test::framework::master_test_suite().argv;
    MPI_Init(&argc, &argv);
}

// ToDo: Further tests of different configurations needed


using namespace cupcfd::comm::mpi;


// === ExchangeMPIIsendIrecv ===
// Test Correct Functionality for fixed size exchanges
BOOST_AUTO_TEST_CASE(ExchangeMPIIsendIrecv_test1)
{
    cupcfd::comm::Communicator comm(MPI_COMM_WORLD);
    cupcfd::error::eCodes status;

    MPI_Request * requests;
    int nRequests;

	if(comm.rank == 0)
	{
		int sendBuf[1] = {1};
		int tRanks[1] = {1};
		int recvBuf[1];


		status = ExchangeMPIIsendIrecv(sendBuf, 1,
							  recvBuf, 1,
							  tRanks, 1,
							  1, comm.comm, &requests, &nRequests);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
		status = WaitallMPI(requests, nRequests);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

		int cmp[1] = {10};
	    BOOST_CHECK_EQUAL_COLLECTIONS(recvBuf, recvBuf + 1, cmp, cmp + 1);
	}
	else if(comm.rank == 1)
	{

		int sendBuf[2] = {10, 20};
		int tRanks[2] = {0, 2};
		int recvBuf[2];


		status = ExchangeMPIIsendIrecv(sendBuf, 2,
							  recvBuf, 2,
							  tRanks, 2,
							  1, comm.comm, &requests, &nRequests);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
		status = WaitallMPI(requests, nRequests);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

		int cmp[2] = {1, 30};
	    BOOST_CHECK_EQUAL_COLLECTIONS(recvBuf, recvBuf + 2, cmp, cmp + 2);
	}
	else if(comm.rank == 2)
	{

		int sendBuf[2] = {30, 40};
		int tRanks[2] = {1, 3};
		int recvBuf[2];


		status = ExchangeMPIIsendIrecv(sendBuf, 2,
							  recvBuf, 2,
							  tRanks, 2,
							  1, comm.comm, &requests, &nRequests);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
		status = WaitallMPI(requests, nRequests);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

		int cmp[2] = {20, 50};
	    BOOST_CHECK_EQUAL_COLLECTIONS(recvBuf, recvBuf + 2, cmp, cmp + 2);
	}
	else if(comm.rank == 3)
	{
		int sendBuf[1] = {50};
		int tRanks[1] = {2};
		int recvBuf[2];


		status = ExchangeMPIIsendIrecv(sendBuf, 1,
							  recvBuf, 1,
							  tRanks, 1,
							  1, comm.comm, &requests, &nRequests);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
		status = WaitallMPI(requests, nRequests);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

		int cmp[1] = {40};
	    BOOST_CHECK_EQUAL_COLLECTIONS(recvBuf, recvBuf + 1, cmp, cmp + 1);
	}
}

// === ExchangeVMPIIsendIrecv1 ===
// Variable ExchangeVMPIIsendIrecv that creates the requests array.
// Test 1: Test correct functionality for exchanges given correct parameters.
BOOST_AUTO_TEST_CASE(ExchangeVMPIIsendIrecv1_test1)
{
    cupcfd::comm::Communicator comm(MPI_COMM_WORLD);
    cupcfd::error::eCodes status;

    MPI_Request * requests;
    int nRequests;

	if(comm.rank == 0)
	{
		int sRanks[3] = {11, 4, 7};
		int rRanks[0];

		int sendCount[3] = {279, 279, 279};
		int recvCount[3] = {0, 0, 0};

		int sendBuf[837];

		// Set recv size of 1, but we don't expect to actually receive any data.
		int recvBuf[1];

		for(int i = 0; i < 837; i++)
		{
			sendBuf[i] = i;
		}

		status = ExchangeVMPIIsendIrecv(sendBuf, 837, sendCount, 3,
							  recvBuf, 1, recvCount, 3,
							  sRanks, 3, rRanks, 0, comm.comm, &requests, &nRequests);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

		status = WaitallMPI(requests, nRequests);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
		free(requests);
	}
	else if(comm.rank == 4)
	{
		int sRanks[0];
		int rRanks[1] = {0};

		int sendCount[1] = {0};
		int recvCount[1] = {279};

		int sendBuf[1];
		int recvBuf[279];
		int recvBufCmp[279];

		for(int i = 279; i < 558; i++)
		{
			recvBufCmp[i-279] = i;
		}

		status = ExchangeVMPIIsendIrecv(sendBuf, 0, sendCount, 1,
							  recvBuf, 279, recvCount, 1,
							  sRanks, 0, rRanks, 1, comm.comm, &requests, &nRequests);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

		status = WaitallMPI(requests, nRequests);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
		BOOST_CHECK_EQUAL_COLLECTIONS(recvBufCmp, recvBufCmp + 2, recvBuf, recvBuf + 2);
		free(requests);
	}
	else if(comm.rank == 7)
	{
		int sRanks[0];
		int rRanks[1] = {0};

		int sendCount[1] = {0};
		int recvCount[1] = {279};

		int sendBuf[1];
		int recvBuf[279];
		int recvBufCmp[279];

		for(int i = 558; i < 837; i++)
		{
			recvBufCmp[i-558] = i;
		}

		status = ExchangeVMPIIsendIrecv(sendBuf, 0, sendCount, 1,
							  recvBuf, 279, recvCount, 1,
							  sRanks, 0, rRanks, 1, comm.comm, &requests, &nRequests);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

		status = WaitallMPI(requests, nRequests);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
		BOOST_CHECK_EQUAL_COLLECTIONS(recvBufCmp, recvBufCmp + 279, recvBuf, recvBuf + 279);
		free(requests);
	}
	else if(comm.rank == 11)
	{
		int sRanks[0];
		int rRanks[1] = {0};

		int sendCount[1] = {0};
		int recvCount[1] = {279};

		int sendBuf[1];
		int recvBuf[279];
		int recvBufCmp[279];

		for(int i = 0; i < 279; i++)
		{
			recvBufCmp[i] = i;
		}

		status = ExchangeVMPIIsendIrecv(sendBuf, 0, sendCount, 1,
							  recvBuf, 279, recvCount, 1,
							  sRanks, 0, rRanks, 1, comm.comm, &requests, &nRequests);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

		status = WaitallMPI(requests, nRequests);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
		BOOST_CHECK_EQUAL_COLLECTIONS(recvBufCmp, recvBufCmp + 279, recvBuf, recvBuf + 279);
		free(requests);
	}
}

// === ExchangeVMPIIsendIrecv2 ===
// Variable ExchangeVMPIIsendIrecv that requires a requests array to be precreated.
// Test 1: Test correct functionality for exchanges given correct parameters.
BOOST_AUTO_TEST_CASE(ExchangeVMPIIsendIrecv2_test1)
{
    cupcfd::comm::Communicator comm(MPI_COMM_WORLD);
    cupcfd::error::eCodes status;

    MPI_Request * requests;
    int nRequests;

	if(comm.rank == 0)
	{
		int sRanks[3] = {11, 4, 7};
		int rRanks[0];

		int sendCount[3] = {279, 279, 279};
		int recvCount[3] = {0, 0, 0};

		int sendBuf[837];

		// Set recv size of 1, but we don't expect to actually receive any data.
		int recvBuf[1];

		nRequests = 3;
		requests = (MPI_Request *) malloc(sizeof(MPI_Request) * nRequests);

		for(int i = 0; i < 837; i++)
		{
			sendBuf[i] = i;
		}

		status = ExchangeVMPIIsendIrecv(sendBuf, 837, sendCount, 3,
							  recvBuf, 1, recvCount, 3,
							  sRanks, 3, rRanks, 0, comm.comm, requests, nRequests);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

		status = WaitallMPI(requests, nRequests);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
		free(requests);
	}
	else if(comm.rank == 4)
	{
		int sRanks[0];
		int rRanks[1] = {0};

		int sendCount[1] = {0};
		int recvCount[1] = {279};

		int sendBuf[1];
		int recvBuf[279];
		int recvBufCmp[279];

		for(int i = 279; i < 558; i++)
		{
			recvBufCmp[i-279] = i;
		}

		nRequests = 1;
		requests = (MPI_Request *) malloc(sizeof(MPI_Request) * nRequests);

		status = ExchangeVMPIIsendIrecv(sendBuf, 0, sendCount, 1,
							  recvBuf, 279, recvCount, 1,
							  sRanks, 0, rRanks, 1, comm.comm, requests, nRequests);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

		status = WaitallMPI(requests, nRequests);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

		free(requests);
		BOOST_CHECK_EQUAL_COLLECTIONS(recvBufCmp, recvBufCmp + 279, recvBuf, recvBuf + 279);
	}
	else if(comm.rank == 7)
	{
		int sRanks[0];
		int rRanks[1] = {0};

		int sendCount[1] = {0};
		int recvCount[1] = {279};

		int sendBuf[1];
		int recvBuf[279];
		int recvBufCmp[279];

		for(int i = 558; i < 837; i++)
		{
			recvBufCmp[i-558] = i;
		}

		nRequests = 1;
		requests = (MPI_Request *) malloc(sizeof(MPI_Request) * nRequests);

		status = ExchangeVMPIIsendIrecv(sendBuf, 0, sendCount, 1,
							  recvBuf, 279, recvCount, 1,
							  sRanks, 0, rRanks, 1, comm.comm, requests, nRequests);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

		status = WaitallMPI(requests, nRequests);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

		free(requests);
		BOOST_CHECK_EQUAL_COLLECTIONS(recvBufCmp, recvBufCmp + 279, recvBuf, recvBuf + 279);
	}
	else if(comm.rank == 11)
	{
		int sRanks[0];
		int rRanks[1] = {0};

		int sendCount[1] = {0};
		int recvCount[1] = {279};

		int sendBuf[1];
		int recvBuf[279];
		int recvBufCmp[279];

		for(int i = 0; i < 279; i++)
		{
			recvBufCmp[i] = i;
		}

		nRequests = 1;
		requests = (MPI_Request *) malloc(sizeof(MPI_Request) * nRequests);

		status = ExchangeVMPIIsendIrecv(sendBuf, 0, sendCount, 1,
							  recvBuf, 279, recvCount, 1,
							  sRanks, 0, rRanks, 1, comm.comm, requests, nRequests);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

		status = WaitallMPI(requests, nRequests);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

		free(requests);
		BOOST_CHECK_EQUAL_COLLECTIONS(recvBufCmp, recvBufCmp + 279, recvBuf, recvBuf + 279);
	}
}

// Cleanup MPI Environment
BOOST_AUTO_TEST_CASE(cleanup)
{
    MPI_Finalize();
}
