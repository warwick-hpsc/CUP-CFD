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

#define BOOST_TEST_MODULE ExchangePattern
#include <boost/test/unit_test.hpp>
#include <boost/test/output_test_stream.hpp>

#include <stdexcept>
#include <iostream>

#include "mpi.h"

#include "Communicator.h"
#include "ExchangePattern.h"



// Create a temporary base class that is non-virtual for testing
// concrete methods

namespace cupcfd
{
	namespace comm
	{
		template <class T>
		class ExchangePatternTest : public ExchangePattern<T>
		{
			cupcfd::error::eCodes packSendBuffer(
				T * data __attribute__((unused)), 
				int nData __attribute__((unused))) {
				return cupcfd::error::E_SUCCESS;
			}

			cupcfd::error::eCodes unpackRecvBuffer(
				T * data __attribute__((unused)), 
				int nData __attribute__((unused))) {
				return cupcfd::error::E_SUCCESS;
			}

			cupcfd::error::eCodes exchangeStart(
				T * sourceData __attribute__((unused)), 
				int nData __attribute__((unused))) {
				return cupcfd::error::E_SUCCESS;
			}

			cupcfd::error::eCodes exchangeStop(
				T * sinkData __attribute__((unused)), 
				int nData __attribute__((unused))) {
				return cupcfd::error::E_SUCCESS;
			}
		};
	}
}

using namespace cupcfd::comm;

BOOST_AUTO_TEST_CASE(setup)
{
    int argc = boost::unit_test::framework::master_test_suite().argc;
    char ** argv = boost::unit_test::framework::master_test_suite().argv;
    MPI_Init(&argc, &argv);
}

// === Constructor ===
// Test 1:
BOOST_AUTO_TEST_CASE(constructor_test1)
{

}


// === init ===
// Test 1: Test that the send/recv data is setup and stored correctly
// after initialisation.
BOOST_AUTO_TEST_CASE(init_test1)
{
    cupcfd::comm::Communicator comm(MPI_COMM_WORLD);
    cupcfd::error::eCodes status;

    // Create a default empty pattern
    cupcfd::comm::ExchangePatternTest<int> pattern;

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
    }
}

BOOST_AUTO_TEST_CASE(cleanup)
{
    // Cleanup MPI Environment
    MPI_Finalize();
}
