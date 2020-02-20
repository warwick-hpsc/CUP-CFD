/*
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Unit Tests for the LinearSolverPETScAlgorithm class
 */

#define BOOST_TEST_MODULE LinearSolverPETScAlgorithm
#include <boost/test/unit_test.hpp>
#include <boost/test/output_test_stream.hpp>
#include <stdexcept>

#include "LinearSolverPETScAlgorithm.h"
#include "Error.h"

// ========================================
// ============== Tests ===================
// ========================================

using namespace cupcfd::linearsolvers;

// Setup
BOOST_AUTO_TEST_CASE(setup)
{
    int argc = boost::unit_test::framework::master_test_suite().argc;
    char ** argv = boost::unit_test::framework::master_test_suite().argv;

    MPI_Init(&argc, &argv);
	PetscInitialize(&argc, &argv, NULL, NULL);
}

// === Constructors ===
// Test 1: Setup a Command Line Variant
BOOST_AUTO_TEST_CASE(constructor_test1)
{

}

// Test 2: Setup a CGAMG Variant
BOOST_AUTO_TEST_CASE(constructor_test2)
{

}

// === Solve ===
// Test 1: Solve a Command Line Variant
BOOST_AUTO_TEST_CASE(solve_test1)
{

}

// Test 2: Solve a CGAMG Variant
BOOST_AUTO_TEST_CASE(solve_test2)
{

}

BOOST_AUTO_TEST_CASE(cleanup)
{
	PetscFinalize();
    MPI_Finalize();
}
