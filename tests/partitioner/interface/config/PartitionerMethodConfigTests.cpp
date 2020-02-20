/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Tests for the concrete methods of the PartitionerMethodConfig class.
 */

#define BOOST_TEST_MODULE PartitionerMethodConfig

// Boost setup
#include <boost/test/unit_test.hpp>
#include <boost/test/output_test_stream.hpp>
#include <stdexcept>

#include "PartitionerMethodConfig.h"
#include "Error.h"

// Namespaces
namespace utf = boost::unit_test;

using namespace cupcfd::partitioner;

// ======= Setup ========
// Some tests require MPI
BOOST_AUTO_TEST_CASE(setup)
{
    int argc = boost::unit_test::framework::master_test_suite().argc;
    char ** argv = boost::unit_test::framework::master_test_suite().argv;
    MPI_Init(&argc, &argv);
}

// === Constructor ===
// Test 1:
BOOST_AUTO_TEST_CASE(constructor_test1, * utf::tolerance(0.00001))
{
	// Virtual Class
}


// ======= Cleanup ========
// Finalize MPI
BOOST_AUTO_TEST_CASE(cleanup)
{
    // Cleanup MPI Environment
    MPI_Finalize();
}
