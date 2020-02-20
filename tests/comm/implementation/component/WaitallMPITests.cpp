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

#define BOOST_TEST_MODULE WaitAllMPI
#include <boost/test/unit_test.hpp>
#include <boost/test/output_test_stream.hpp>

#include <stdexcept>
#include <iostream>

#include "mpi.h"

#include "WaitallMPI.h"
#include "Communicator.h"

// Setup MPI
BOOST_AUTO_TEST_CASE(setup)
{
    int argc = boost::unit_test::framework::master_test_suite().argc;
    char ** argv = boost::unit_test::framework::master_test_suite().argv;
    MPI_Init(&argc, &argv);
}

// === WaitallMPI ===
// Test 1: Unsure of what to use as the test here for now since it's just a wrapper
// Probably best to test as part of exchange instead.
BOOST_AUTO_TEST_CASE(WaitallMPI_test1)
{

}

// Cleanup MPI Environment
BOOST_AUTO_TEST_CASE(cleanup)
{
    MPI_Finalize();
}
