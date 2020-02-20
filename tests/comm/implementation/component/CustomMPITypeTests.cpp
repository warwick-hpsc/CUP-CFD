/*
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Test non-pure virtual methods of CustomMPIType
 *
 */

#define BOOST_TEST_MODULE CustomMPIType
#include <boost/test/unit_test.hpp>
#include <boost/test/output_test_stream.hpp>

#include <stdexcept>

#include "CustomMPIType.h"

using namespace cupcfd::comm::mpi;

// Setup
BOOST_AUTO_TEST_CASE(setup)
{
    int argc = boost::unit_test::framework::master_test_suite().argc;
    char ** argv = boost::unit_test::framework::master_test_suite().argv;
    MPI_Init(&argc, &argv);
}

// Finalize
BOOST_AUTO_TEST_CASE(cleanup)
{
    // Cleanup MPI Environment
    MPI_Finalize();
}
