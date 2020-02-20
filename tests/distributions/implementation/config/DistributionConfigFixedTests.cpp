/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Tests for the concrete methods of the DistributionConfigFixed class.
 */

#define BOOST_TEST_MODULE DistributionConfigFixed

// Boost setup
#include <boost/test/unit_test.hpp>
#include <boost/test/output_test_stream.hpp>
#include <stdexcept>

#include "DistributionConfigFixed.h"
#include "Error.h"

// Namespaces
namespace utf = boost::unit_test;

using namespace cupcfd::distributions;

// === Constructor ===
// Test 1: Default Empty Constructor
BOOST_AUTO_TEST_CASE(constructor_test1, * utf::tolerance(0.00001))
{
	DistributionConfigFixed<int, double> config;

	BOOST_TEST(config.getFixedValue() == 0.0);
}

// Test 2: Parameter Constructor
BOOST_AUTO_TEST_CASE(constructor_test2, * utf::tolerance(0.00001))
{
	DistributionConfigFixed<int, double> config(5.6);

	BOOST_TEST(config.getFixedValue() == 5.6);
}

// Test 3: Configuration Copy Constructor
BOOST_AUTO_TEST_CASE(constructor_test3, * utf::tolerance(0.00001))
{
	DistributionConfigFixed<int, double> config2(6.7);
	DistributionConfigFixed<int, double> config(config2);

	BOOST_TEST(config.getFixedValue() == 6.7);
}

// === set/getFixedValue ===
// Test 1: Test correct set/get of method
BOOST_AUTO_TEST_CASE(set_getLBound_test1, * utf::tolerance(0.00001))
{
	DistributionConfigFixed<int, double> config;

	// Check current value is as expected
	BOOST_TEST(config.getFixedValue() == 0.0);

	// Set and check changed value can be retrieved.
	config.setFixedValue(2.451);
	BOOST_TEST(config.getFixedValue() == 2.451);
}

// === operator= ===
// Test 1: Test correct copy
BOOST_AUTO_TEST_CASE(operator_equals_test1, * utf::tolerance(0.00001))
{
	DistributionConfigFixed<int, double> config;
	DistributionConfigFixed<int, double> config2(7.9);

	// Check current values are as expected
	BOOST_TEST(config.getFixedValue() == 0.0);
	BOOST_TEST(config2.getFixedValue() == 7.9);

	// Operator Test
	config = config2;

	// Check all configuration parameters copied correctly
	BOOST_TEST(config.getFixedValue() == 7.9);
}

// === clone ===
BOOST_AUTO_TEST_CASE(clone_test1, * utf::tolerance(0.00001))
{

}

// === buildDistribution ===
BOOST_AUTO_TEST_CASE(buildDistribution_test1, * utf::tolerance(0.00001))
{

}
