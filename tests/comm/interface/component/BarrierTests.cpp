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

#define BOOST_TEST_MODULE Barrier
#include <boost/test/unit_test.hpp>
#include <boost/test/output_test_stream.hpp>

#include <stdexcept>

#include "Communicator.h"
#include "Barrier.h"

BOOST_AUTO_TEST_CASE(setup)
{
    int argc = boost::unit_test::framework::master_test_suite().argc;
    char ** argv = boost::unit_test::framework::master_test_suite().argv;
    MPI_Init(&argc, &argv);
}

// === Barrier ===

BOOST_AUTO_TEST_CASE(Barrier_test1)
{
	 cupcfd::comm::Communicator comm(MPI_COMM_WORLD);
	 cupcfd::error::eCodes status;

	 status = Barrier(comm);
	 BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
}

BOOST_AUTO_TEST_CASE(cleanup)
{
    // Cleanup MPI Environment
    MPI_Finalize();
}
