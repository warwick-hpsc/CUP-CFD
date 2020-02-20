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

#define BOOST_TEST_MODULE BarrierMPI
#include <boost/test/unit_test.hpp>
#include <boost/test/output_test_stream.hpp>

#include <stdexcept>

#include "BarrierMPI.h"
#include "Communicator.h"

using namespace cupcfd::comm::mpi;

// Setup MPI
BOOST_AUTO_TEST_CASE(setup)
{
    int argc = boost::unit_test::framework::master_test_suite().argc;
    char ** argv = boost::unit_test::framework::master_test_suite().argv;
    MPI_Init(&argc, &argv);
}

// === BarrierMPI ===
// Test 1: Test call returns correctly
BOOST_AUTO_TEST_CASE(BarrierMPI_test1)
{
	 cupcfd::comm::Communicator comm(MPI_COMM_WORLD);
	 cupcfd::error::eCodes status;

	 status = BarrierMPI(comm.comm);
	 BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
}

// Cleanup MPI
BOOST_AUTO_TEST_CASE(cleanup)
{
    // Cleanup MPI Environment
    MPI_Finalize();
}
