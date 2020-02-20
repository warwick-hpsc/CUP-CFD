/*
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Unit Tests for the LinearSolverConfigSourceJSON class
 */

#define BOOST_TEST_MODULE LinearSolverConfigSourceJSON
#include <boost/test/unit_test.hpp>
#include <boost/test/output_test_stream.hpp>
#include <stdexcept>

#include "LinearSolverConfigSourceJSON.h"
#include "Error.h"

using namespace cupcfd::linearsolvers;

// === Constructors ===
BOOST_AUTO_TEST_CASE(constructor_test1)
{
	LinearSolverConfigSourceJSON config;

	// Check Defaults
}
