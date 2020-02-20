/*
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Tests for the Distribution class
 */

#define BOOST_TEST_MODULE Distribution
#include <boost/test/unit_test.hpp>
#include <boost/test/output_test_stream.hpp>
#include <stdexcept>

#include "Distribution.h"
#include "Error.h"

using namespace cupcfd::distributions;

// === Constructors ===
BOOST_AUTO_TEST_CASE(constructor_test1)
{
	// Since virtual class, currently no test here
}
