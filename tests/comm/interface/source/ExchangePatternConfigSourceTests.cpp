/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Tests for the concrete methods of the ExchangePatternConfigSource class.
 */

#define BOOST_TEST_MODULE ExchangePatternConfigSource

// Boost setup
#include <boost/test/unit_test.hpp>
#include <boost/test/output_test_stream.hpp>
#include <stdexcept>

#include "ExchangePatternConfigSource.h"
#include "Error.h"

// Namespaces
namespace utf = boost::unit_test;

using namespace cupcfd::comm;

// === Constructor ===
// Test 1: Default Empty Constructor
BOOST_AUTO_TEST_CASE(constructor_test1, * utf::tolerance(0.00001))
{
	// Need a concrete implementation of the class to test here.
}
