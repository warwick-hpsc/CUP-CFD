/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Tests for the Communicator Class
 */

#define BOOST_TEST_MODULE Communicator
#include <boost/test/unit_test.hpp>
#include <boost/test/output_test_stream.hpp>

#include <stdexcept>

#include "mpi.h"

#include "Communicator.h"

using namespace cupcfd::comm;

// Setup
BOOST_AUTO_TEST_CASE(setup)
{
    int argc = boost::unit_test::framework::master_test_suite().argc;
    char ** argv = boost::unit_test::framework::master_test_suite().argv;
    MPI_Init(&argc, &argv);
}

// === Constructor ===
// Test 1: Test Empty Constructor is Size 1 Self
BOOST_AUTO_TEST_CASE(constructor_test1)
{
	Communicator comm;

	BOOST_CHECK_EQUAL(comm.size, 1);
}

// Test 2: Test World Constructor is correct size
BOOST_AUTO_TEST_CASE(constructor_test2)
{
	Communicator comm(MPI_COMM_WORLD);
	int rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	// This presumes the tests are run with the standard 4 ranks!
	BOOST_CHECK_EQUAL(comm.size, 4);

	if(rank == 0)
	{
		BOOST_CHECK_EQUAL(comm.rank, 0);
	}
	else if(rank == 1)
	{
		BOOST_CHECK_EQUAL(comm.rank, 1);
	}
	else if(rank == 2)
	{
		BOOST_CHECK_EQUAL(comm.rank, 2);
	}
	else if(rank == 3)
	{
		BOOST_CHECK_EQUAL(comm.rank, 3);
	}
}

// === operator= ===
// Test 1: Test values are correctly copied, and that
// the communicators are not the same ID (it should be a duplicate)
BOOST_AUTO_TEST_CASE(copy_test1)
{
	Communicator comm1;
	Communicator comm2(MPI_COMM_WORLD);

	BOOST_CHECK_EQUAL(comm1.size, 1);
	BOOST_CHECK_EQUAL(comm2.size, 4);

	comm1 = comm2;
	BOOST_CHECK_EQUAL(comm1.size, 4);

	// The MPI comms are duplicated, they should NOT be equal
	BOOST_TEST(comm1.comm != comm2.comm);
}

// Finalize
BOOST_AUTO_TEST_CASE(cleanup)
{
    // Cleanup MPI Environment
    MPI_Finalize();
}
