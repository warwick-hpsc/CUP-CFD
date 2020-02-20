/*
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Unit Tests for the concrete methods of the UnstructuredMeshInterface class
 */

#define BOOST_TEST_MODULE UnstructuredMeshInterface
#include <boost/test/unit_test.hpp>
#include <boost/test/output_test_stream.hpp>
#include <stdexcept>

#include "UnstructuredMeshInterface.h"

using namespace cupcfd::geometry::mesh;


// Setup
BOOST_AUTO_TEST_CASE(setup)
{
    int argc = boost::unit_test::framework::master_test_suite().argc;
    char ** argv = boost::unit_test::framework::master_test_suite().argv;

    MPI_Init(&argc, &argv);
}

BOOST_AUTO_TEST_CASE(cleanup)
{
    MPI_Finalize();
}

// ToDo: Concrete functions are tested by implementing classes for this interface
// However, we need to test that the passthrough works correctly, so we could use
// a duplicate of the tests for those classes here but operating on a
// UnstructuredMeshInterface class
