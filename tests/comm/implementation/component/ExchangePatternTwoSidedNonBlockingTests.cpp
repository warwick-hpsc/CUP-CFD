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

#define BOOST_TEST_MODULE ExchangePatternTwoSidedNonBlocking
#include <boost/test/unit_test.hpp>
#include <boost/test/output_test_stream.hpp>

#include <stdexcept>
#include <iostream>

#include "mpi.h"

#include "Communicator.h"
#include "ExchangePatternTwoSidedNonBlocking.h"

using namespace cupcfd::comm;

// Setup
BOOST_AUTO_TEST_CASE(setup)
{
    int argc = boost::unit_test::framework::master_test_suite().argc;
    char ** argv = boost::unit_test::framework::master_test_suite().argv;
    MPI_Init(&argc, &argv);
}

// === Constructors ===

// === init ===
// Test 1:
BOOST_AUTO_TEST_CASE(init_test1)
{
    cupcfd::comm::Communicator comm(MPI_COMM_WORLD);

    cupcfd::error::eCodes status;

    // Create a default empty pattern
    ExchangePatternTwoSidedNonBlocking<int> pattern;

    // Test the init function

    // ExchangePattern is all about defining an exchange pattern based
    // on the indexes in a 1D array.
    // For this test, we need to associate a mapping scheme, and which
    // values are intended to go from one rank to another.
    // We then need to verify that the setup phase has correctly stored
    // the information regarding the send and recv data sources.

    if(comm.rank == 0)
    {
    	// Rank 0 can 'own' exchange ids 1-5, with space for 6 and 7 also
    	int exchangeIDX[7] = {1, 2, 3, 4, 5, 6, 7};

    	// We will send 6 and 7 to rank 1
    	int exchangeIDXSend[2] = {4, 5};
    	int rankSend[2] = {1, 1};

    	status = pattern.init(comm, exchangeIDX, 7, exchangeIDXSend, 2, rankSend, 2);
    	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

    	// Check the data has been set up correctly
	    BOOST_CHECK_EQUAL(pattern.nSProc, 1);
	    BOOST_CHECK_EQUAL(pattern.nSXAdj, 2);
	    BOOST_CHECK_EQUAL(pattern.nSAdjncy, 2);

	    BOOST_CHECK_EQUAL(pattern.nRProc, 1);
	    BOOST_CHECK_EQUAL(pattern.nRXAdj, 2);
	    BOOST_CHECK_EQUAL(pattern.nRAdjncy, 2);

	    int sProcCmp[1] = {1};
	    BOOST_CHECK_EQUAL_COLLECTIONS(pattern.sProc , pattern.sProc + 1, sProcCmp, sProcCmp + 1);

	    int sXAdjCmp[2] = {0, 2};
	    BOOST_CHECK_EQUAL_COLLECTIONS(pattern.sXAdj , pattern.sXAdj + 2, sXAdjCmp, sXAdjCmp + 2);

	    int sAdjncyCmp[2] = {4, 5};
	    BOOST_CHECK_EQUAL_COLLECTIONS(pattern.sAdjncy , pattern.sAdjncy + 2, sAdjncyCmp, sAdjncyCmp + 2);

	    int rProcCmp[1] = {1};
	    BOOST_CHECK_EQUAL_COLLECTIONS(pattern.rProc , pattern.rProc + 1, rProcCmp, rProcCmp + 1);

	    int rXAdjCmp[2] = {0, 2};
	    BOOST_CHECK_EQUAL_COLLECTIONS(pattern.rXAdj , pattern.rXAdj + 2, rXAdjCmp, rXAdjCmp + 2);

	    int rAdjncyCmp[2] = {6, 7};
	    BOOST_CHECK_EQUAL_COLLECTIONS(pattern.rAdjncy , pattern.rAdjncy + 2, rAdjncyCmp, rAdjncyCmp + 2);

	    // 1 for send, 1 for recv
	    BOOST_CHECK_EQUAL(pattern.nRequests, 2);
	    BOOST_CHECK(pattern.requests);

	    // 1 for send, 1 for recv
	    BOOST_CHECK_EQUAL(pattern.nStatuses, 2);
	    BOOST_CHECK(pattern.statuses);

    }
    else if(comm.rank == 1)
    {
    	// Rank 1 can 'own' exchange ids 6-10, with space also for 4, 5, 11 and 12
    	int exchangeIDX[9] = {6, 7, 8, 9, 10, 4, 5, 11, 12};

    	int exchangeIDXSend[4] = {6, 7, 9, 10};
    	int rankSend[4] = {0, 0, 2, 2};

    	status = pattern.init(comm, exchangeIDX, 9, exchangeIDXSend, 4, rankSend, 4);
    	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

    	// Check the data has been set up correctly
	    BOOST_CHECK_EQUAL(pattern.nSProc, 2);
	    BOOST_CHECK_EQUAL(pattern.nSXAdj, 3);
	    BOOST_CHECK_EQUAL(pattern.nSAdjncy, 4);

	    BOOST_CHECK_EQUAL(pattern.nRProc, 2);
	    BOOST_CHECK_EQUAL(pattern.nRXAdj, 3);
	    BOOST_CHECK_EQUAL(pattern.nRAdjncy, 4);

	    int sProcCmp[2] = {0, 2};
	    BOOST_CHECK_EQUAL_COLLECTIONS(pattern.sProc , pattern.sProc + 2, sProcCmp, sProcCmp + 2);

	    int sXAdjCmp[3] = {0, 2, 4};
	    BOOST_CHECK_EQUAL_COLLECTIONS(pattern.sXAdj , pattern.sXAdj + 3, sXAdjCmp, sXAdjCmp + 3);

	    int sAdjncyCmp[4] = {6, 7, 9, 10};
	    BOOST_CHECK_EQUAL_COLLECTIONS(pattern.sAdjncy , pattern.sAdjncy + 4, sAdjncyCmp, sAdjncyCmp + 4);

	    int rProcCmp[2] = {0, 2};
	    BOOST_CHECK_EQUAL_COLLECTIONS(pattern.rProc , pattern.rProc + 2, rProcCmp, rProcCmp + 2);

	    int rXAdjCmp[3] = {0, 2, 4};
	    BOOST_CHECK_EQUAL_COLLECTIONS(pattern.rXAdj , pattern.rXAdj + 3, rXAdjCmp, rXAdjCmp + 3);

	    // Note order received will be based on sendee (e.g. rank 2 sends 12, then 11)
	    int rAdjncyCmp[4] = {4, 5, 12, 11};
	    BOOST_CHECK_EQUAL_COLLECTIONS(pattern.rAdjncy , pattern.rAdjncy + 4, rAdjncyCmp, rAdjncyCmp + 4);

	    // 2 for send, 2 for recv (process count)
	    BOOST_CHECK_EQUAL(pattern.nRequests, 4);
	    BOOST_CHECK(pattern.requests);

	    // 2 for send, 2 for recv  (process count)
	    BOOST_CHECK_EQUAL(pattern.nStatuses, 4);
	    BOOST_CHECK(pattern.statuses);

    }
    else if(comm.rank == 2)
    {
    	// Rank 2 can 'own' exchange ids 11-15, with space for 9, 10, 16 and 17
    	// We will also mix up the order a bit to ensure that mappings are correct
    	int exchangeIDX[9] = {15, 13, 11, 12, 14, 10, 9, 17, 16};

    	// Note, these will be grouped by process, so ultimately the send array will be
    	// 12, 11, 14, 15, 13, but use of stable sort will ensure that elements within process
    	// groups stay in the same order (i.e. 12 before 11)
    	int exchangeIDXSend[5] = {12, 14, 11, 15, 13};
    	int rankSend[5] = {1, 3, 1, 3, 3};

    	status = pattern.init(comm, exchangeIDX, 9, exchangeIDXSend, 5, rankSend, 5);
    	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

    	// Check the data has been set up correctly
	    BOOST_CHECK_EQUAL(pattern.nSProc, 2);
	    BOOST_CHECK_EQUAL(pattern.nSXAdj, 3);
	    BOOST_CHECK_EQUAL(pattern.nSAdjncy, 5);

	    BOOST_CHECK_EQUAL(pattern.nRProc, 2);
	    BOOST_CHECK_EQUAL(pattern.nRXAdj, 3);
	    BOOST_CHECK_EQUAL(pattern.nRAdjncy, 4);

	    int sProcCmp[2] = {1, 3};
	    BOOST_CHECK_EQUAL_COLLECTIONS(pattern.sProc , pattern.sProc + 2, sProcCmp, sProcCmp + 2);

	    int sXAdjCmp[3] = {0, 2, 5};
	    BOOST_CHECK_EQUAL_COLLECTIONS(pattern.sXAdj , pattern.sXAdj + 3, sXAdjCmp, sXAdjCmp + 3);

	    int sAdjncyCmp[5] = {12, 11, 14, 15, 13};
	    BOOST_CHECK_EQUAL_COLLECTIONS(pattern.sAdjncy , pattern.sAdjncy + 5, sAdjncyCmp, sAdjncyCmp + 5);

	    int rProcCmp[2] = {1, 3};
	    BOOST_CHECK_EQUAL_COLLECTIONS(pattern.rProc , pattern.rProc + 2, rProcCmp, rProcCmp + 2);

	    int rXAdjCmp[3] = {0, 2, 4};
	    BOOST_CHECK_EQUAL_COLLECTIONS(pattern.rXAdj , pattern.rXAdj + 3, rXAdjCmp, rXAdjCmp + 3);

	    // Note order received will be based on sendee (e.g. rank 2 sends 10, then 9)
	    int rAdjncyCmp[4] = {9, 10, 16, 17};
	    BOOST_CHECK_EQUAL_COLLECTIONS(pattern.rAdjncy , pattern.rAdjncy + 4, rAdjncyCmp, rAdjncyCmp + 4);

	    // 1 for send, 1 for recv (process count)
	    BOOST_CHECK_EQUAL(pattern.nRequests, 4);
	    BOOST_CHECK(pattern.requests);

	    // 1 for send, 1 for recv  (process count)
	    BOOST_CHECK_EQUAL(pattern.nStatuses, 4);
	    BOOST_CHECK(pattern.statuses);
    }
    else if(comm.rank == 3)
    {
    	// Rank 3 can 'own' exchange ids 16-20, with space for 13, 14 and 15
    	int exchangeIDX[8] = {14, 15, 16, 17, 18, 19, 20, 13};
    	int exchangeIDXSend[2] = {16, 17};
    	int rankSend[2] = {2, 2};

    	status = status = pattern.init(comm, exchangeIDX, 7, exchangeIDXSend, 2, rankSend, 2);
    	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
    	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

    	// Check the data has been set up correctly
	    BOOST_CHECK_EQUAL(pattern.nSProc, 1);
	    BOOST_CHECK_EQUAL(pattern.nSXAdj, 2);
	    BOOST_CHECK_EQUAL(pattern.nSAdjncy, 2);

	    BOOST_CHECK_EQUAL(pattern.nRProc, 1);
	    BOOST_CHECK_EQUAL(pattern.nRXAdj, 2);
	    BOOST_CHECK_EQUAL(pattern.nRAdjncy, 3);

	    int sProcCmp[1] = {2};
	    BOOST_CHECK_EQUAL_COLLECTIONS(pattern.sProc , pattern.sProc + 1, sProcCmp, sProcCmp + 1);

	    int sXAdjCmp[2] = {0, 2};
	    BOOST_CHECK_EQUAL_COLLECTIONS(pattern.sXAdj , pattern.sXAdj + 2, sXAdjCmp, sXAdjCmp + 2);

	    int sAdjncyCmp[2] = {16, 17};
	    BOOST_CHECK_EQUAL_COLLECTIONS(pattern.sAdjncy , pattern.sAdjncy + 2, sAdjncyCmp, sAdjncyCmp + 2);

	    int rProcCmp[1] = {2};
	    BOOST_CHECK_EQUAL_COLLECTIONS(pattern.rProc , pattern.rProc + 1, rProcCmp, rProcCmp + 1);

	    int rXAdjCmp[2] = {0, 3};
	    BOOST_CHECK_EQUAL_COLLECTIONS(pattern.rXAdj , pattern.rXAdj + 2, rXAdjCmp, rXAdjCmp + 2);

	    int rAdjncyCmp[3] = {14, 15, 13};
	    BOOST_CHECK_EQUAL_COLLECTIONS(pattern.rAdjncy , pattern.rAdjncy + 3, rAdjncyCmp, rAdjncyCmp + 3);

	    // 1 for send, 1 for recv (process count)
	    BOOST_CHECK_EQUAL(pattern.nRequests, 2);
	    BOOST_CHECK(pattern.requests);

	    // 1 for send, 1 for recv  (process count)
	    BOOST_CHECK_EQUAL(pattern.nStatuses, 2);
	    BOOST_CHECK(pattern.statuses);
    }
}

// ToDo: Init Driver Error Condition Checks



// === packsendBuffer ===
// ToDo: Replace this by setting xadj and index arrays directly and test pack method
// Test 1:
BOOST_AUTO_TEST_CASE(packSendBuffer_test1)
{
    cupcfd::comm::Communicator comm(MPI_COMM_WORLD);

    cupcfd::error::eCodes status;

    // Create a default empty pattern
    cupcfd::comm::ExchangePatternTwoSidedNonBlocking<int> pattern;

    // Test the init function

    // ExchangePattern is all about defining an exchange pattern based
    // on the indexes in a 1D array.
    // For this test, we need to associate a mapping scheme, and which
    // values are intended to go from one rank to another.
    // We then need to verify that the setup phase has correctly stored
    // the information regarding the send and recv data sources.

    if(comm.rank == 0)
    {
    	// Rank 0 can 'own' exchange ids 1-5, with space for 6 and 7 also
    	int exchangeIDX[7] = {1, 2, 3, 4, 5, 6, 7};

    	// We will send 6 and 7 to rank 1
    	int exchangeIDXSend[2] = {4, 5};
    	int rankSend[2] = {1, 1};

    	status = pattern.init(comm, exchangeIDX, 7, exchangeIDXSend, 2, rankSend, 2);
    	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

    	// Check the data has been set up correctly
	    BOOST_CHECK_EQUAL(pattern.nSProc, 1);
	    BOOST_CHECK_EQUAL(pattern.nSXAdj, 2);
	    BOOST_CHECK_EQUAL(pattern.nSAdjncy, 2);

	    BOOST_CHECK_EQUAL(pattern.nRProc, 1);
	    BOOST_CHECK_EQUAL(pattern.nRXAdj, 2);
	    BOOST_CHECK_EQUAL(pattern.nRAdjncy, 2);

	    int sProcCmp[1] = {1};
	    BOOST_CHECK_EQUAL_COLLECTIONS(pattern.sProc , pattern.sProc + 1, sProcCmp, sProcCmp + 1);

	    int sXAdjCmp[2] = {0, 2};
	    BOOST_CHECK_EQUAL_COLLECTIONS(pattern.sXAdj , pattern.sXAdj + 2, sXAdjCmp, sXAdjCmp + 2);

	    int sAdjncyCmp[2] = {4, 5};
	    BOOST_CHECK_EQUAL_COLLECTIONS(pattern.sAdjncy , pattern.sAdjncy + 2, sAdjncyCmp, sAdjncyCmp + 2);

	    int rProcCmp[1] = {1};
	    BOOST_CHECK_EQUAL_COLLECTIONS(pattern.rProc , pattern.rProc + 1, rProcCmp, rProcCmp + 1);

	    int rXAdjCmp[2] = {0, 2};
	    BOOST_CHECK_EQUAL_COLLECTIONS(pattern.rXAdj , pattern.rXAdj + 2, rXAdjCmp, rXAdjCmp + 2);

	    int rAdjncyCmp[2] = {6, 7};
	    BOOST_CHECK_EQUAL_COLLECTIONS(pattern.rAdjncy , pattern.rAdjncy + 2, rAdjncyCmp, rAdjncyCmp + 2);

	    // 1 for send, 1 for recv
	    BOOST_CHECK_EQUAL(pattern.nRequests, 2);
	    BOOST_CHECK(pattern.requests);

	    // 1 for send, 1 for recv
	    BOOST_CHECK_EQUAL(pattern.nStatuses, 2);
	    BOOST_CHECK(pattern.statuses);

	    int data[7] = {42, 52, 12, 32, 1032, 201, 876};
	    status = pattern.packSendBuffer(data, 7);
	    BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	    int sendBufferCmp[2] = {32, 1032};
	    BOOST_CHECK_EQUAL_COLLECTIONS(pattern.sendBuffer , pattern.sendBuffer + 2, sendBufferCmp, sendBufferCmp + 2);
    }
    else if(comm.rank == 1)
    {
    	// Rank 1 can 'own' exchange ids 6-10, with space also for 4, 5, 11 and 12
    	int exchangeIDX[9] = {6, 7, 8, 9, 10, 4, 5, 11, 12};

    	int exchangeIDXSend[4] = {6, 7, 9, 10};
    	int rankSend[4] = {0, 0, 2, 2};

    	status = pattern.init(comm, exchangeIDX, 9, exchangeIDXSend, 4, rankSend, 4);
    	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

    	// Check the data has been set up correctly
	    BOOST_CHECK_EQUAL(pattern.nSProc, 2);
	    BOOST_CHECK_EQUAL(pattern.nSXAdj, 3);
	    BOOST_CHECK_EQUAL(pattern.nSAdjncy, 4);

	    BOOST_CHECK_EQUAL(pattern.nRProc, 2);
	    BOOST_CHECK_EQUAL(pattern.nRXAdj, 3);
	    BOOST_CHECK_EQUAL(pattern.nRAdjncy, 4);

	    int sProcCmp[2] = {0, 2};
	    BOOST_CHECK_EQUAL_COLLECTIONS(pattern.sProc , pattern.sProc + 2, sProcCmp, sProcCmp + 2);

	    int sXAdjCmp[3] = {0, 2, 4};
	    BOOST_CHECK_EQUAL_COLLECTIONS(pattern.sXAdj , pattern.sXAdj + 3, sXAdjCmp, sXAdjCmp + 3);

	    int sAdjncyCmp[4] = {6, 7, 9, 10};
	    BOOST_CHECK_EQUAL_COLLECTIONS(pattern.sAdjncy , pattern.sAdjncy + 4, sAdjncyCmp, sAdjncyCmp + 4);

	    int rProcCmp[2] = {0, 2};
	    BOOST_CHECK_EQUAL_COLLECTIONS(pattern.rProc , pattern.rProc + 2, rProcCmp, rProcCmp + 2);

	    int rXAdjCmp[3] = {0, 2, 4};
	    BOOST_CHECK_EQUAL_COLLECTIONS(pattern.rXAdj , pattern.rXAdj + 3, rXAdjCmp, rXAdjCmp + 3);

	    // Note order received will be based on sendee (e.g. rank 2 sends 12, then 11)
	    int rAdjncyCmp[4] = {4, 5, 12, 11};
	    BOOST_CHECK_EQUAL_COLLECTIONS(pattern.rAdjncy , pattern.rAdjncy + 4, rAdjncyCmp, rAdjncyCmp + 4);

	    // 2 for send, 2 for recv (process count)
	    BOOST_CHECK_EQUAL(pattern.nRequests, 4);
	    BOOST_CHECK(pattern.requests);

	    // 2 for send, 2 for recv  (process count)
	    BOOST_CHECK_EQUAL(pattern.nStatuses, 4);
	    BOOST_CHECK(pattern.statuses);

	    int data[9] = {20, 30, 40 ,50 ,60 ,70 ,80, 90, 100};
	    status = pattern.packSendBuffer(data, 9);
	    BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	    int sendBufferCmp[4] = {20, 30, 50, 60};
	    BOOST_CHECK_EQUAL_COLLECTIONS(pattern.sendBuffer , pattern.sendBuffer + 4, sendBufferCmp, sendBufferCmp + 4);
    }
    else if(comm.rank == 2)
    {
    	// Rank 2 can 'own' exchange ids 11-15, with space for 9, 10, 16 and 17
    	// We will also mix up the order a bit to ensure that mappings are correct
    	int exchangeIDX[9] = {15, 13, 11, 12, 14, 10, 9, 17, 16};

    	// Note, these will be grouped by process, so ultimately the send array will be
    	// 12, 11, 14, 15, 13, but use of stable sort will ensure that elements within process
    	// groups stay in the same order (i.e. 12 before 11)
    	int exchangeIDXSend[5] = {12, 14, 11, 15, 13};
    	int rankSend[5] = {1, 3, 1, 3, 3};

    	status = pattern.init(comm, exchangeIDX, 9, exchangeIDXSend, 5, rankSend, 5);
    	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

    	// Check the data has been set up correctly
	    BOOST_CHECK_EQUAL(pattern.nSProc, 2);
	    BOOST_CHECK_EQUAL(pattern.nSXAdj, 3);
	    BOOST_CHECK_EQUAL(pattern.nSAdjncy, 5);

	    BOOST_CHECK_EQUAL(pattern.nRProc, 2);
	    BOOST_CHECK_EQUAL(pattern.nRXAdj, 3);
	    BOOST_CHECK_EQUAL(pattern.nRAdjncy, 4);

	    int sProcCmp[2] = {1, 3};
	    BOOST_CHECK_EQUAL_COLLECTIONS(pattern.sProc , pattern.sProc + 2, sProcCmp, sProcCmp + 2);

	    int sXAdjCmp[3] = {0, 2, 5};
	    BOOST_CHECK_EQUAL_COLLECTIONS(pattern.sXAdj , pattern.sXAdj + 3, sXAdjCmp, sXAdjCmp + 3);

	    int sAdjncyCmp[5] = {12, 11, 14, 15, 13};
	    BOOST_CHECK_EQUAL_COLLECTIONS(pattern.sAdjncy , pattern.sAdjncy + 5, sAdjncyCmp, sAdjncyCmp + 5);

	    int rProcCmp[2] = {1, 3};
	    BOOST_CHECK_EQUAL_COLLECTIONS(pattern.rProc , pattern.rProc + 2, rProcCmp, rProcCmp + 2);

	    int rXAdjCmp[3] = {0, 2, 4};
	    BOOST_CHECK_EQUAL_COLLECTIONS(pattern.rXAdj , pattern.rXAdj + 3, rXAdjCmp, rXAdjCmp + 3);

	    // Note order received will be based on sendee (e.g. rank 2 sends 10, then 9)
	    int rAdjncyCmp[4] = {9, 10, 16, 17};
	    BOOST_CHECK_EQUAL_COLLECTIONS(pattern.rAdjncy , pattern.rAdjncy + 4, rAdjncyCmp, rAdjncyCmp + 4);

	    // 1 for send, 1 for recv (process count)
	    BOOST_CHECK_EQUAL(pattern.nRequests, 4);
	    BOOST_CHECK(pattern.requests);

	    // 1 for send, 1 for recv  (process count)
	    BOOST_CHECK_EQUAL(pattern.nStatuses, 4);
	    BOOST_CHECK(pattern.statuses);


	    int data[9] = {120, 130, 140 ,150 ,160 ,170 ,180, 190, 200};
	    status = pattern.packSendBuffer(data, 9);
	    BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	    int sendBufferCmp[5] = {150, 140, 160, 120, 130};
	    BOOST_CHECK_EQUAL_COLLECTIONS(pattern.sendBuffer , pattern.sendBuffer + 5, sendBufferCmp, sendBufferCmp + 5);

    }
    else if(comm.rank == 3)
    {
    	// Rank 3 can 'own' exchange ids 16-20, with space for 13, 14 and 15
    	int exchangeIDX[8] = {14, 15, 16, 17, 18, 19, 20, 13};
    	int exchangeIDXSend[2] = {16, 17};
    	int rankSend[2] = {2, 2};

    	status = pattern.init(comm, exchangeIDX, 7, exchangeIDXSend, 2, rankSend, 2);
    	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

    	// Check the data has been set up correctly
	    BOOST_CHECK_EQUAL(pattern.nSProc, 1);
	    BOOST_CHECK_EQUAL(pattern.nSXAdj, 2);
	    BOOST_CHECK_EQUAL(pattern.nSAdjncy, 2);

	    BOOST_CHECK_EQUAL(pattern.nRProc, 1);
	    BOOST_CHECK_EQUAL(pattern.nRXAdj, 2);
	    BOOST_CHECK_EQUAL(pattern.nRAdjncy, 3);

	    int sProcCmp[1] = {2};
	    BOOST_CHECK_EQUAL_COLLECTIONS(pattern.sProc , pattern.sProc + 1, sProcCmp, sProcCmp + 1);

	    int sXAdjCmp[2] = {0, 2};
	    BOOST_CHECK_EQUAL_COLLECTIONS(pattern.sXAdj , pattern.sXAdj + 2, sXAdjCmp, sXAdjCmp + 2);

	    int sAdjncyCmp[2] = {16, 17};
	    BOOST_CHECK_EQUAL_COLLECTIONS(pattern.sAdjncy , pattern.sAdjncy + 2, sAdjncyCmp, sAdjncyCmp + 2);

	    int rProcCmp[1] = {2};
	    BOOST_CHECK_EQUAL_COLLECTIONS(pattern.rProc , pattern.rProc + 1, rProcCmp, rProcCmp + 1);

	    int rXAdjCmp[2] = {0, 3};
	    BOOST_CHECK_EQUAL_COLLECTIONS(pattern.rXAdj , pattern.rXAdj + 2, rXAdjCmp, rXAdjCmp + 2);

	    int rAdjncyCmp[3] = {14, 15, 13};
	    BOOST_CHECK_EQUAL_COLLECTIONS(pattern.rAdjncy , pattern.rAdjncy + 3, rAdjncyCmp, rAdjncyCmp + 3);

	    // 1 for send, 1 for recv (process count)
	    BOOST_CHECK_EQUAL(pattern.nRequests, 2);
	    BOOST_CHECK(pattern.requests);

	    // 1 for send, 1 for recv  (process count)
	    BOOST_CHECK_EQUAL(pattern.nStatuses, 2);
	    BOOST_CHECK(pattern.statuses);

	    int data[8] = {220, 230, 240 ,250 ,260 ,270 ,280, 290};
	    status = pattern.packSendBuffer(data, 8);
	    BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	    int sendBufferCmp[2] = {240, 250};
	    BOOST_CHECK_EQUAL_COLLECTIONS(pattern.sendBuffer , pattern.sendBuffer + 2, sendBufferCmp, sendBufferCmp + 2);
    }
}

// === unpackRecvBuffer ===
BOOST_AUTO_TEST_CASE(unpackRecvBuffer_test1)
{
	// Can't really test this without testing the exchange, so easier to test as one package below
}

// === exchangeStart/exchangeStop/unpackRecvBuffer ===
// Test 1: Test full exchange and correct retrieval of data
BOOST_AUTO_TEST_CASE(exchange_test1)
{
    cupcfd::comm::Communicator comm(MPI_COMM_WORLD);

    cupcfd::error::eCodes status;

    // Create a default empty pattern
    cupcfd::comm::ExchangePatternTwoSidedNonBlocking<int> pattern;

    // Test the init function

    // ExchangePattern is all about defining an exchange pattern based
    // on the indexes in a 1D array.
    // For this test, we need to associate a mapping scheme, and which
    // values are intended to go from one rank to another.
    // We then need to verify that the setup phase has correctly stored
    // the information regarding the send and recv data sources.

    if(comm.rank == 0)
    {
    	// Rank 0 can 'own' exchange ids 1-5, with space for 6 and 7 also
    	int exchangeIDX[7] = {1, 2, 3, 4, 5, 6, 7};

    	// We will send 6 and 7 to rank 1
    	int exchangeIDXSend[2] = {4, 5};
    	int rankSend[2] = {1, 1};

    	status = pattern.init(comm, exchangeIDX, 7, exchangeIDXSend, 2, rankSend, 2);
    	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

    	// Check the data has been set up correctly
	    BOOST_CHECK_EQUAL(pattern.nSProc, 1);
	    BOOST_CHECK_EQUAL(pattern.nSXAdj, 2);
	    BOOST_CHECK_EQUAL(pattern.nSAdjncy, 2);

	    BOOST_CHECK_EQUAL(pattern.nRProc, 1);
	    BOOST_CHECK_EQUAL(pattern.nRXAdj, 2);
	    BOOST_CHECK_EQUAL(pattern.nRAdjncy, 2);

	    int sProcCmp[1] = {1};
	    BOOST_CHECK_EQUAL_COLLECTIONS(pattern.sProc , pattern.sProc + 1, sProcCmp, sProcCmp + 1);

	    int sXAdjCmp[2] = {0, 2};
	    BOOST_CHECK_EQUAL_COLLECTIONS(pattern.sXAdj , pattern.sXAdj + 2, sXAdjCmp, sXAdjCmp + 2);

	    int sAdjncyCmp[2] = {4, 5};
	    BOOST_CHECK_EQUAL_COLLECTIONS(pattern.sAdjncy , pattern.sAdjncy + 2, sAdjncyCmp, sAdjncyCmp + 2);

	    int rProcCmp[1] = {1};
	    BOOST_CHECK_EQUAL_COLLECTIONS(pattern.rProc , pattern.rProc + 1, rProcCmp, rProcCmp + 1);

	    int rXAdjCmp[2] = {0, 2};
	    BOOST_CHECK_EQUAL_COLLECTIONS(pattern.rXAdj , pattern.rXAdj + 2, rXAdjCmp, rXAdjCmp + 2);

	    int rAdjncyCmp[2] = {6, 7};
	    BOOST_CHECK_EQUAL_COLLECTIONS(pattern.rAdjncy , pattern.rAdjncy + 2, rAdjncyCmp, rAdjncyCmp + 2);

	    // 1 for send, 1 for recv
	    BOOST_CHECK_EQUAL(pattern.nRequests, 2);
	    BOOST_CHECK(pattern.requests);

	    // 1 for send, 1 for recv
	    BOOST_CHECK_EQUAL(pattern.nStatuses, 2);
	    BOOST_CHECK(pattern.statuses);

	    int data[7] = {42, 52, 12, 32, 1032, 201, 876};
	    status = pattern.packSendBuffer(data, 7);
	    BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	    int sendBufferCmp[2] = {32, 1032};
	    BOOST_CHECK_EQUAL_COLLECTIONS(pattern.sendBuffer , pattern.sendBuffer + 2, sendBufferCmp, sendBufferCmp + 2);

	    status = pattern.exchangeStart(data, 7);
	    BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	    status = pattern.exchangeStop(data, 7);
	    BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	    // Check that the data buffer has been updated with the contents of the recvBuffer in the correct locations
	    int dataCmp[7] = {42, 52, 12, 32, 1032, 20, 30};
	    BOOST_CHECK_EQUAL_COLLECTIONS(data, data + 7, dataCmp, dataCmp+7);


    }
    else if(comm.rank == 1)
    {
    	// Rank 1 can 'own' exchange ids 6-10, with space also for 4, 5, 11 and 12
    	int exchangeIDX[9] = {6, 7, 8, 9, 10, 4, 5, 11, 12};

    	int exchangeIDXSend[4] = {6, 7, 9, 10};
    	int rankSend[4] = {0, 0, 2, 2};

    	status = pattern.init(comm, exchangeIDX, 9, exchangeIDXSend, 4, rankSend, 4);
    	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

    	// Check the data has been set up correctly
	    BOOST_CHECK_EQUAL(pattern.nSProc, 2);
	    BOOST_CHECK_EQUAL(pattern.nSXAdj, 3);
	    BOOST_CHECK_EQUAL(pattern.nSAdjncy, 4);

	    BOOST_CHECK_EQUAL(pattern.nRProc, 2);
	    BOOST_CHECK_EQUAL(pattern.nRXAdj, 3);
	    BOOST_CHECK_EQUAL(pattern.nRAdjncy, 4);

	    int sProcCmp[2] = {0, 2};
	    BOOST_CHECK_EQUAL_COLLECTIONS(pattern.sProc , pattern.sProc + 2, sProcCmp, sProcCmp + 2);

	    int sXAdjCmp[3] = {0, 2, 4};
	    BOOST_CHECK_EQUAL_COLLECTIONS(pattern.sXAdj , pattern.sXAdj + 3, sXAdjCmp, sXAdjCmp + 3);

	    int sAdjncyCmp[4] = {6, 7, 9, 10};
	    BOOST_CHECK_EQUAL_COLLECTIONS(pattern.sAdjncy , pattern.sAdjncy + 4, sAdjncyCmp, sAdjncyCmp + 4);

	    int rProcCmp[2] = {0, 2};
	    BOOST_CHECK_EQUAL_COLLECTIONS(pattern.rProc , pattern.rProc + 2, rProcCmp, rProcCmp + 2);

	    int rXAdjCmp[3] = {0, 2, 4};
	    BOOST_CHECK_EQUAL_COLLECTIONS(pattern.rXAdj , pattern.rXAdj + 3, rXAdjCmp, rXAdjCmp + 3);

	    // Note order received will be based on sendee (e.g. rank 2 sends 12, then 11)
	    int rAdjncyCmp[4] = {4, 5, 12, 11};
	    BOOST_CHECK_EQUAL_COLLECTIONS(pattern.rAdjncy , pattern.rAdjncy + 4, rAdjncyCmp, rAdjncyCmp + 4);

	    // 2 for send, 2 for recv (process count)
	    BOOST_CHECK_EQUAL(pattern.nRequests, 4);
	    BOOST_CHECK(pattern.requests);

	    // 2 for send, 2 for recv  (process count)
	    BOOST_CHECK_EQUAL(pattern.nStatuses, 4);
	    BOOST_CHECK(pattern.statuses);

	    int data[9] = {20, 30, 40 ,50 ,60 ,70 ,80, 90, 100};
	    status = pattern.packSendBuffer(data, 9);
	    BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	    int sendBufferCmp[4] = {20, 30, 50, 60};
	    BOOST_CHECK_EQUAL_COLLECTIONS(pattern.sendBuffer , pattern.sendBuffer + 4, sendBufferCmp, sendBufferCmp + 4);

	    status = pattern.exchangeStart(data, 9);
	    BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	    status = pattern.exchangeStop(data, 9);
	    BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	    // Check that the data buffer has been updated with the contents of the recvBuffer in the correct locations
	    // Receiving values for exchange indexes 4, 5, 12, 11
	    int dataCmp[9] = {20, 30, 40 ,50 ,60 ,32 ,1032, 140, 150};
	    BOOST_CHECK_EQUAL_COLLECTIONS(data, data + 9, dataCmp, dataCmp+9);
    }
    else if(comm.rank == 2)
    {
    	// Rank 2 can 'own' exchange ids 11-15, with space for 9, 10, 16 and 17
    	// We will also mix up the order a bit to ensure that mappings are correct
    	int exchangeIDX[9] = {15, 13, 11, 12, 14, 10, 9, 17, 16};

    	// Note, these will be grouped by process, so ultimately the send array will be
    	// 12, 11, 14, 15, 13, but use of stable sort will ensure that elements within process
    	// groups stay in the same order (i.e. 12 before 11)
    	int exchangeIDXSend[5] = {12, 14, 11, 15, 13};
    	int rankSend[5] = {1, 3, 1, 3, 3};

    	status = pattern.init(comm, exchangeIDX, 9, exchangeIDXSend, 5, rankSend, 5);
    	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

    	// Check the data has been set up correctly
	    BOOST_CHECK_EQUAL(pattern.nSProc, 2);
	    BOOST_CHECK_EQUAL(pattern.nSXAdj, 3);
	    BOOST_CHECK_EQUAL(pattern.nSAdjncy, 5);

	    BOOST_CHECK_EQUAL(pattern.nRProc, 2);
	    BOOST_CHECK_EQUAL(pattern.nRXAdj, 3);
	    BOOST_CHECK_EQUAL(pattern.nRAdjncy, 4);

	    int sProcCmp[2] = {1, 3};
	    BOOST_CHECK_EQUAL_COLLECTIONS(pattern.sProc , pattern.sProc + 2, sProcCmp, sProcCmp + 2);

	    int sXAdjCmp[3] = {0, 2, 5};
	    BOOST_CHECK_EQUAL_COLLECTIONS(pattern.sXAdj , pattern.sXAdj + 3, sXAdjCmp, sXAdjCmp + 3);

	    int sAdjncyCmp[5] = {12, 11, 14, 15, 13};
	    BOOST_CHECK_EQUAL_COLLECTIONS(pattern.sAdjncy , pattern.sAdjncy + 5, sAdjncyCmp, sAdjncyCmp + 5);

	    int rProcCmp[2] = {1, 3};
	    BOOST_CHECK_EQUAL_COLLECTIONS(pattern.rProc , pattern.rProc + 2, rProcCmp, rProcCmp + 2);

	    int rXAdjCmp[3] = {0, 2, 4};
	    BOOST_CHECK_EQUAL_COLLECTIONS(pattern.rXAdj , pattern.rXAdj + 3, rXAdjCmp, rXAdjCmp + 3);

	    // Note order received will be based on sendee (e.g. rank 2 sends 10, then 9)
	    int rAdjncyCmp[4] = {9, 10, 16, 17};
	    BOOST_CHECK_EQUAL_COLLECTIONS(pattern.rAdjncy , pattern.rAdjncy + 4, rAdjncyCmp, rAdjncyCmp + 4);

	    // 1 for send, 1 for recv (process count)
	    BOOST_CHECK_EQUAL(pattern.nRequests, 4);
	    BOOST_CHECK(pattern.requests);

	    // 1 for send, 1 for recv  (process count)
	    BOOST_CHECK_EQUAL(pattern.nStatuses, 4);
	    BOOST_CHECK(pattern.statuses);

	    int data[9] = {120, 130, 140 ,150 ,160 ,170 ,180, 190, 200};
	    status = pattern.packSendBuffer(data, 9);
	    BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	    int sendBufferCmp[5] = {150, 140, 160, 120, 130};
	    BOOST_CHECK_EQUAL_COLLECTIONS(pattern.sendBuffer , pattern.sendBuffer + 5, sendBufferCmp, sendBufferCmp + 5);

	    status = pattern.exchangeStart(data, 9);
	    BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	    status = pattern.exchangeStop(data, 9);
	    BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	    // Check that the data buffer has been updated with the contents of the recvBuffer in the correct locations
	    // Receiving values for exchange indexes 9, 10, 16, 17
	    int dataCmp[9] = {120, 130, 140 ,150 ,160 ,60 ,50, 250, 240};
	    BOOST_CHECK_EQUAL_COLLECTIONS(data, data + 9, dataCmp, dataCmp+9);

    }
    else if(comm.rank == 3)
    {
    	// Rank 3 can 'own' exchange ids 16-20, with space for 13, 14 and 15
    	int exchangeIDX[8] = {14, 15, 16, 17, 18, 19, 20, 13};
    	int exchangeIDXSend[2] = {16, 17};
    	int rankSend[2] = {2, 2};

    	status = pattern.init(comm, exchangeIDX, 8, exchangeIDXSend, 2, rankSend, 2);
    	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

    	// Check the data has been set up correctly
	    BOOST_CHECK_EQUAL(pattern.nSProc, 1);
	    BOOST_CHECK_EQUAL(pattern.nSXAdj, 2);
	    BOOST_CHECK_EQUAL(pattern.nSAdjncy, 2);

	    BOOST_CHECK_EQUAL(pattern.nRProc, 1);
	    BOOST_CHECK_EQUAL(pattern.nRXAdj, 2);
	    BOOST_CHECK_EQUAL(pattern.nRAdjncy, 3);

	    int sProcCmp[1] = {2};
	    BOOST_CHECK_EQUAL_COLLECTIONS(pattern.sProc , pattern.sProc + 1, sProcCmp, sProcCmp + 1);

	    int sXAdjCmp[2] = {0, 2};
	    BOOST_CHECK_EQUAL_COLLECTIONS(pattern.sXAdj , pattern.sXAdj + 2, sXAdjCmp, sXAdjCmp + 2);

	    int sAdjncyCmp[2] = {16, 17};
	    BOOST_CHECK_EQUAL_COLLECTIONS(pattern.sAdjncy , pattern.sAdjncy + 2, sAdjncyCmp, sAdjncyCmp + 2);

	    int rProcCmp[1] = {2};
	    BOOST_CHECK_EQUAL_COLLECTIONS(pattern.rProc , pattern.rProc + 1, rProcCmp, rProcCmp + 1);

	    int rXAdjCmp[2] = {0, 3};
	    BOOST_CHECK_EQUAL_COLLECTIONS(pattern.rXAdj , pattern.rXAdj + 2, rXAdjCmp, rXAdjCmp + 2);

	    int rAdjncyCmp[3] = {14, 15, 13};
	    BOOST_CHECK_EQUAL_COLLECTIONS(pattern.rAdjncy , pattern.rAdjncy + 3, rAdjncyCmp, rAdjncyCmp + 3);

	    // 1 for send, 1 for recv (process count)
	    BOOST_CHECK_EQUAL(pattern.nRequests, 2);
	    BOOST_CHECK(pattern.requests);

	    // 1 for send, 1 for recv  (process count)
	    BOOST_CHECK_EQUAL(pattern.nStatuses, 2);
	    BOOST_CHECK(pattern.statuses);

	    int data[8] = {220, 230, 240 ,250 ,260 ,270 ,280, 290};
	    status = pattern.packSendBuffer(data, 8);
	    BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	    int sendBufferCmp[2] = {240, 250};
	    BOOST_CHECK_EQUAL_COLLECTIONS(pattern.sendBuffer , pattern.sendBuffer + 2, sendBufferCmp, sendBufferCmp + 2);

	    status = pattern.exchangeStart(data, 8);
	    BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	    status = pattern.exchangeStop(data, 8);
	    BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	    // Check that the data buffer has been updated with the contents of the recvBuffer in the correct locations
	    // Receiving values for exchange indexes 13, 14, 15

	    int recvCmp[3] = {160, 120, 130};
	    BOOST_CHECK_EQUAL_COLLECTIONS(pattern.recvBuffer, pattern.recvBuffer + 3, recvCmp, recvCmp + 3);

	    int dataCmp[8] = {160, 120, 240 ,250 ,260 ,270 ,280, 130};
	    BOOST_CHECK_EQUAL_COLLECTIONS(data, data + 8, dataCmp, dataCmp+8);
    }
}

// Test 2: Test full exchange and correct retrieval of data where some data elements may be sent to
// more than one process
BOOST_AUTO_TEST_CASE(exchange_test2)
{
    cupcfd::comm::Communicator comm(MPI_COMM_WORLD);

    cupcfd::error::eCodes status;

    // Create a default empty pattern
    cupcfd::comm::ExchangePatternTwoSidedNonBlocking<int> pattern;

    // Test the init function

    // ExchangePattern is all about defining an exchange pattern based
    // on the indexes in a 1D array.
    // For this test, we need to associate a mapping scheme, and which
    // values are intended to go from one rank to another.
    // We then need to verify that the setup phase has correctly stored
    // the information regarding the send and recv data sources.

    if(comm.rank == 0)
    {
    	// Rank 0 can 'own' exchange ids 1-5, with space for 6 and 7 also
    	int exchangeIDX[7] = {1, 2, 3, 4, 5, 6, 7};

    	// We will send 6 and 7 to rank 1
    	int exchangeIDXSend[2] = {4, 5};
    	int rankSend[2] = {1, 1};

    	status = pattern.init(comm, exchangeIDX, 7, exchangeIDXSend, 2, rankSend, 2);
    	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

    	// Check the data has been set up correctly
	    BOOST_CHECK_EQUAL(pattern.nSProc, 1);
	    BOOST_CHECK_EQUAL(pattern.nSXAdj, 2);
	    BOOST_CHECK_EQUAL(pattern.nSAdjncy, 2);

	    BOOST_CHECK_EQUAL(pattern.nRProc, 1);
	    BOOST_CHECK_EQUAL(pattern.nRXAdj, 2);
	    BOOST_CHECK_EQUAL(pattern.nRAdjncy, 2);

	    int sProcCmp[1] = {1};
	    BOOST_CHECK_EQUAL_COLLECTIONS(pattern.sProc , pattern.sProc + 1, sProcCmp, sProcCmp + 1);

	    int sXAdjCmp[2] = {0, 2};
	    BOOST_CHECK_EQUAL_COLLECTIONS(pattern.sXAdj , pattern.sXAdj + 2, sXAdjCmp, sXAdjCmp + 2);

	    int sAdjncyCmp[2] = {4, 5};
	    BOOST_CHECK_EQUAL_COLLECTIONS(pattern.sAdjncy , pattern.sAdjncy + 2, sAdjncyCmp, sAdjncyCmp + 2);

	    int rProcCmp[1] = {1};
	    BOOST_CHECK_EQUAL_COLLECTIONS(pattern.rProc , pattern.rProc + 1, rProcCmp, rProcCmp + 1);

	    int rXAdjCmp[2] = {0, 2};
	    BOOST_CHECK_EQUAL_COLLECTIONS(pattern.rXAdj , pattern.rXAdj + 2, rXAdjCmp, rXAdjCmp + 2);

	    int rAdjncyCmp[2] = {6, 7};
	    BOOST_CHECK_EQUAL_COLLECTIONS(pattern.rAdjncy , pattern.rAdjncy + 2, rAdjncyCmp, rAdjncyCmp + 2);

	    int data[7] = {42, 52, 12, 32, 1032, 201, 876};
	    status = pattern.packSendBuffer(data, 7);
	    BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	    int sendBufferCmp[2] = {32, 1032};
	    BOOST_CHECK_EQUAL_COLLECTIONS(pattern.sendBuffer , pattern.sendBuffer + 2, sendBufferCmp, sendBufferCmp + 2);

	    status = pattern.exchangeStart(data, 7);
	    BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	    status = pattern.exchangeStop(data, 7);
	    BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	    // Check that the data buffer has been updated with the contents of the recvBuffer in the correct locations
	    int dataCmp[7] = {42, 52, 12, 32, 1032, 20, 30};
	    BOOST_CHECK_EQUAL_COLLECTIONS(data, data + 7, dataCmp, dataCmp+7);
    }
    else if(comm.rank == 1)
    {
    	// Rank 1 can 'own' exchange ids 6-10, with space also for 4, 5, 11 and 12
    	int exchangeIDX[9] = {6, 7, 8, 9, 10, 4, 5, 11, 12};

    	// Note: We are sending 6 and 7 to more than one process
    	int exchangeIDXSend[6] = {6, 7, 9, 10, 6, 7};
    	int rankSend[6] = {0, 0, 2, 2, 3, 3};

    	status = pattern.init(comm, exchangeIDX, 9, exchangeIDXSend, 6, rankSend, 6);
    	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

    	// Check the data has been set up correctly
	    BOOST_CHECK_EQUAL(pattern.nSProc, 3);
	    BOOST_CHECK_EQUAL(pattern.nSXAdj, 4);
	    BOOST_CHECK_EQUAL(pattern.nSAdjncy, 6);

	    BOOST_CHECK_EQUAL(pattern.nRProc, 2);
	    BOOST_CHECK_EQUAL(pattern.nRXAdj, 3);
	    BOOST_CHECK_EQUAL(pattern.nRAdjncy, 4);

	    int sProcCmp[3] = {0, 2, 3};
	    BOOST_CHECK_EQUAL_COLLECTIONS(pattern.sProc , pattern.sProc + 3, sProcCmp, sProcCmp + 3);

	    int sXAdjCmp[4] = {0, 2, 4, 6};
	    BOOST_CHECK_EQUAL_COLLECTIONS(pattern.sXAdj , pattern.sXAdj + 4, sXAdjCmp, sXAdjCmp + 4);

	    int sAdjncyCmp[6] = {6, 7, 9, 10, 6, 7};
	    BOOST_CHECK_EQUAL_COLLECTIONS(pattern.sAdjncy , pattern.sAdjncy + 6, sAdjncyCmp, sAdjncyCmp + 6);

	    int rProcCmp[2] = {0, 2};
	    BOOST_CHECK_EQUAL_COLLECTIONS(pattern.rProc , pattern.rProc + 2, rProcCmp, rProcCmp + 2);

	    int rXAdjCmp[3] = {0, 2, 4};
	    BOOST_CHECK_EQUAL_COLLECTIONS(pattern.rXAdj , pattern.rXAdj + 3, rXAdjCmp, rXAdjCmp + 3);

	    // Note order received will be based on sendee (e.g. rank 2 sends 12, then 11)
	    int rAdjncyCmp[4] = {4, 5, 12, 11};
	    BOOST_CHECK_EQUAL_COLLECTIONS(pattern.rAdjncy , pattern.rAdjncy + 4, rAdjncyCmp, rAdjncyCmp + 4);

	    int data[9] = {20, 30, 40 ,50 ,60 ,70 ,80, 90, 100};
	    status = pattern.packSendBuffer(data, 9);
	    BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	    int sendBufferCmp[6] = {20, 30, 50, 60, 20, 30};
	    BOOST_CHECK_EQUAL_COLLECTIONS(pattern.sendBuffer , pattern.sendBuffer + 6, sendBufferCmp, sendBufferCmp + 6);

	    status = pattern.exchangeStart(data, 9);
	    BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	    status = pattern.exchangeStop(data, 9);
	    BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	    // Check that the data buffer has been updated with the contents of the recvBuffer in the correct locations
	    // Receiving values for exchange indexes 4, 5, 12, 11
	    int dataCmp[9] = {20, 30, 40 ,50 ,60 ,32 ,1032, 140, 150};
	    BOOST_CHECK_EQUAL_COLLECTIONS(data, data + 9, dataCmp, dataCmp+9);
    }
    else if(comm.rank == 2)
    {
    	// Rank 2 can 'own' exchange ids 11-15, with space for 9, 10, 16 and 17
    	// We will also mix up the order a bit to ensure that mappings are correct
    	int exchangeIDX[9] = {15, 13, 11, 12, 14, 10, 9, 17, 16};

    	// Note, these will be grouped by process, so ultimately the send array will be
    	// 12, 11, 14, 15, 13, but use of stable sort will ensure that elements within process
    	// groups stay in the same order (i.e. 12 before 11)
    	int exchangeIDXSend[5] = {12, 14, 11, 15, 13};
    	int rankSend[5] = {1, 3, 1, 3, 3};

    	status = pattern.init(comm, exchangeIDX, 9, exchangeIDXSend, 5, rankSend, 5);
    	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

    	// Check the data has been set up correctly
	    BOOST_CHECK_EQUAL(pattern.nSProc, 2);
	    BOOST_CHECK_EQUAL(pattern.nSXAdj, 3);
	    BOOST_CHECK_EQUAL(pattern.nSAdjncy, 5);

	    BOOST_CHECK_EQUAL(pattern.nRProc, 2);
	    BOOST_CHECK_EQUAL(pattern.nRXAdj, 3);
	    BOOST_CHECK_EQUAL(pattern.nRAdjncy, 4);

	    int sProcCmp[2] = {1, 3};
	    BOOST_CHECK_EQUAL_COLLECTIONS(pattern.sProc , pattern.sProc + 2, sProcCmp, sProcCmp + 2);

	    int sXAdjCmp[3] = {0, 2, 5};
	    BOOST_CHECK_EQUAL_COLLECTIONS(pattern.sXAdj , pattern.sXAdj + 3, sXAdjCmp, sXAdjCmp + 3);

	    int sAdjncyCmp[5] = {12, 11, 14, 15, 13};
	    BOOST_CHECK_EQUAL_COLLECTIONS(pattern.sAdjncy , pattern.sAdjncy + 5, sAdjncyCmp, sAdjncyCmp + 5);

	    int rProcCmp[2] = {1, 3};
	    BOOST_CHECK_EQUAL_COLLECTIONS(pattern.rProc , pattern.rProc + 2, rProcCmp, rProcCmp + 2);

	    int rXAdjCmp[3] = {0, 2, 4};
	    BOOST_CHECK_EQUAL_COLLECTIONS(pattern.rXAdj , pattern.rXAdj + 3, rXAdjCmp, rXAdjCmp + 3);

	    // Note order received will be based on sendee (e.g. rank 2 sends 10, then 9)
	    int rAdjncyCmp[4] = {9, 10, 16, 17};
	    BOOST_CHECK_EQUAL_COLLECTIONS(pattern.rAdjncy , pattern.rAdjncy + 4, rAdjncyCmp, rAdjncyCmp + 4);

	    int data[9] = {120, 130, 140 ,150 ,160 ,170 ,180, 190, 200};
	    status = pattern.packSendBuffer(data, 9);
	    BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	    int sendBufferCmp[5] = {150, 140, 160, 120, 130};
	    BOOST_CHECK_EQUAL_COLLECTIONS(pattern.sendBuffer , pattern.sendBuffer + 5, sendBufferCmp, sendBufferCmp + 5);

	    status = pattern.exchangeStart(data, 9);
	    BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	    status = pattern.exchangeStop(data, 9);
	    BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	    // Check that the data buffer has been updated with the contents of the recvBuffer in the correct locations
	    // Receiving values for exchange indexes 9, 10, 16, 17
	    int dataCmp[9] = {120, 130, 140 ,150 ,160 ,60 ,50, 250, 240};
	    BOOST_CHECK_EQUAL_COLLECTIONS(data, data + 9, dataCmp, dataCmp+9);

    }
    else if(comm.rank == 3)
    {
    	// Rank 3 can 'own' exchange ids 16-20, with space for 13, 14 and 15, 6 and 7
    	int exchangeIDX[10] = {14, 15, 16, 17, 18, 19, 20, 13, 6, 7};
    	int exchangeIDXSend[2] = {16, 17};
    	int rankSend[2] = {2, 2};

    	status = pattern.init(comm, exchangeIDX, 10, exchangeIDXSend, 2, rankSend, 2);
    	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

    	// Check the data has been set up correctly
	    BOOST_CHECK_EQUAL(pattern.nSProc, 1);
	    BOOST_CHECK_EQUAL(pattern.nSXAdj, 2);
	    BOOST_CHECK_EQUAL(pattern.nSAdjncy, 2);

	    BOOST_CHECK_EQUAL(pattern.nRProc, 2);
	    BOOST_CHECK_EQUAL(pattern.nRXAdj, 3);
	    BOOST_CHECK_EQUAL(pattern.nRAdjncy, 5);

	    int sProcCmp[1] = {2};
	    BOOST_CHECK_EQUAL_COLLECTIONS(pattern.sProc , pattern.sProc + 1, sProcCmp, sProcCmp + 1);

	    int sXAdjCmp[2] = {0, 2};
	    BOOST_CHECK_EQUAL_COLLECTIONS(pattern.sXAdj , pattern.sXAdj + 2, sXAdjCmp, sXAdjCmp + 2);

	    int sAdjncyCmp[2] = {16, 17};
	    BOOST_CHECK_EQUAL_COLLECTIONS(pattern.sAdjncy , pattern.sAdjncy + 2, sAdjncyCmp, sAdjncyCmp + 2);

	    int rProcCmp[2] = {1, 2};
	    BOOST_CHECK_EQUAL_COLLECTIONS(pattern.rProc , pattern.rProc + 2, rProcCmp, rProcCmp + 2);

	    int rXAdjCmp[3] = {0, 2, 5};
	    BOOST_CHECK_EQUAL_COLLECTIONS(pattern.rXAdj , pattern.rXAdj + 3, rXAdjCmp, rXAdjCmp + 3);

	    int rAdjncyCmp[5] = {6, 7, 14, 15, 13};
	    BOOST_CHECK_EQUAL_COLLECTIONS(pattern.rAdjncy , pattern.rAdjncy + 5, rAdjncyCmp, rAdjncyCmp + 5);

	    int data[8] = {220, 230, 240 ,250 ,260 ,270 ,280, 290};
	    status = pattern.packSendBuffer(data, 8);
	    BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	    int sendBufferCmp[2] = {240, 250};
	    BOOST_CHECK_EQUAL_COLLECTIONS(pattern.sendBuffer , pattern.sendBuffer + 2, sendBufferCmp, sendBufferCmp + 2);

	    status = pattern.exchangeStart(data, 8);
	    BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	    status = pattern.exchangeStop(data, 8);
	    BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	    // Check that the data buffer has been updated with the contents of the recvBuffer in the correct locations
	    // Receiving values for exchange indexes 13, 14, 15

	    int dataCmp[10] = {160, 120, 240 ,250 ,260 ,270 ,280, 130, 20, 30};
	    BOOST_CHECK_EQUAL_COLLECTIONS(data, data + 10, dataCmp, dataCmp+10);
    }
}

// Test 3: Test all process not receiving from ranks they are sending to


BOOST_AUTO_TEST_CASE(cleanup)
{
    // Cleanup MPI Environment
    MPI_Finalize();
}
