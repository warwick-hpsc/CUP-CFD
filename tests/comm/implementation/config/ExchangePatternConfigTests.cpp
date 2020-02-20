/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Tests for the concrete methods of the ExchangePatternConfig class.
 */

#define BOOST_TEST_MODULE ExchangePatternConfig

// Boost setup
#include <boost/test/unit_test.hpp>
#include <boost/test/output_test_stream.hpp>
#include <stdexcept>

#include "ExchangePatternConfig.h"
#include "Error.h"

// Namespaces
namespace utf = boost::unit_test;

using namespace cupcfd::comm;

// === Constructor ===
// Test 1: Default Empty Constructor
BOOST_AUTO_TEST_CASE(constructor_test1, * utf::tolerance(0.00001))
{
	ExchangePatternConfig config;

	BOOST_CHECK_EQUAL(config.getExchangeMethod(), EXCHANGE_NONBLOCKING_TWO_SIDED);
}

// Test 2: Parameter Constructor
BOOST_AUTO_TEST_CASE(constructor_test2, * utf::tolerance(0.00001))
{
	ExchangePatternConfig config(EXCHANGE_NONBLOCKING_ONE_SIDED);

	BOOST_CHECK_EQUAL(config.getExchangeMethod(), EXCHANGE_NONBLOCKING_ONE_SIDED);
}

// Test 3: Configuration Copy Constructor
BOOST_AUTO_TEST_CASE(constructor_test3, * utf::tolerance(0.00001))
{
	ExchangePatternConfig config2(EXCHANGE_NONBLOCKING_ONE_SIDED);
	ExchangePatternConfig config(config2);

	BOOST_CHECK_EQUAL(config.getExchangeMethod(), EXCHANGE_NONBLOCKING_ONE_SIDED);
}

// === set/getExchangeMethod ===
// Test 1: Test correct set/get of method
BOOST_AUTO_TEST_CASE(setgetExchangeMethod_test1, * utf::tolerance(0.00001))
{
	ExchangePatternConfig config;

	// Check current value is as expected
	BOOST_CHECK_EQUAL(config.getExchangeMethod(), EXCHANGE_NONBLOCKING_TWO_SIDED);

	// Set and check changed value can be retrieved.
	config.setExchangeMethod(EXCHANGE_NONBLOCKING_ONE_SIDED);
	BOOST_CHECK_EQUAL(config.getExchangeMethod(), EXCHANGE_NONBLOCKING_ONE_SIDED);
}

// === operator= ===
BOOST_AUTO_TEST_CASE(operator_equals_test1, * utf::tolerance(0.00001))
{
	ExchangePatternConfig config;
	ExchangePatternConfig config2(EXCHANGE_NONBLOCKING_ONE_SIDED);

	// Check current values are as expected
	BOOST_CHECK_EQUAL(config.getExchangeMethod(), EXCHANGE_NONBLOCKING_TWO_SIDED);
	BOOST_CHECK_EQUAL(config2.getExchangeMethod(), EXCHANGE_NONBLOCKING_ONE_SIDED);

	// Operator Test
	config = config2;

	// Check all configuration parameters copied correctly
	BOOST_CHECK_EQUAL(config.getExchangeMethod(), EXCHANGE_NONBLOCKING_ONE_SIDED);
}

// === buildExchangePattern ===
// Test 1: Create a OneSidedPattern and populate it with data
BOOST_AUTO_TEST_CASE(buildExchangePattern_test1, * utf::tolerance(0.00001))
{

}

// Test 2: Create a TwoSidedPattern and populate it with data
BOOST_AUTO_TEST_CASE(buildExchangePattern_test2, * utf::tolerance(0.00001))
{

}

