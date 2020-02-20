/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Tests for the concrete methods of the DistributionConfigUniform class.
 */

#define BOOST_TEST_MODULE DistributionConfigUniform

// Boost setup
#include <boost/test/unit_test.hpp>
#include <boost/test/output_test_stream.hpp>
#include <stdexcept>

#include "DistributionConfigUniform.h"
#include "Error.h"

// Namespaces
namespace utf = boost::unit_test;

using namespace cupcfd::distributions;

// === Constructor ===
// Test 1: Default Empty Constructor
BOOST_AUTO_TEST_CASE(constructor_test1, * utf::tolerance(0.00001))
{
	DistributionConfigUniform<int, double> config;

	BOOST_TEST(config.getLBound() == 0.0);
	BOOST_TEST(config.getUBound() == 0.0);
}

// Test 2: Parameter Constructor
BOOST_AUTO_TEST_CASE(constructor_test2, * utf::tolerance(0.00001))
{
	DistributionConfigUniform<int, double> config(2.0, 4.0);

	BOOST_TEST(config.getLBound() == 2.0);
	BOOST_TEST(config.getUBound() == 4.0);
}

// Test 3: Configuration Copy Constructor
BOOST_AUTO_TEST_CASE(constructor_test3, * utf::tolerance(0.00001))
{
	DistributionConfigUniform<int, double> config2(2.0, 4.0);
	DistributionConfigUniform<int, double> config(config2);

	BOOST_TEST(config.getLBound() == 2.0);
	BOOST_TEST(config.getUBound() == 4.0);
}

// === set/getLBound ===
// Test 1: Test correct set/get of method
BOOST_AUTO_TEST_CASE(set_getLBound_test1, * utf::tolerance(0.00001))
{
	DistributionConfigUniform<int, double> config;

	// Check current value is as expected
	BOOST_TEST(config.getLBound() == 0.0);

	// Set and check changed value can be retrieved.
	config.setLBound(12.3);
	BOOST_TEST(config.getLBound() == 12.3);
}

// === set/getUBound ===
// Test 1: Test correct set/get of method
BOOST_AUTO_TEST_CASE(set_getUBound_test1, * utf::tolerance(0.00001))
{
	DistributionConfigUniform<int, double> config;

	// Check current value is as expected
	BOOST_TEST(config.getUBound() == 0.0);

	// Set and check changed value can be retrieved.
	config.setUBound(14.6);
	BOOST_TEST(config.getUBound() == 14.6);
}

// === operator= ===
BOOST_AUTO_TEST_CASE(operator_equals_test1, * utf::tolerance(0.00001))
{
	DistributionConfigUniform<int, double> config;
	DistributionConfigUniform<int, double> config2(2.0, 4.0);

	// Check current values are as expected
	BOOST_TEST(config.getLBound() == 0.0);
	BOOST_TEST(config.getUBound() == 0.0);

	BOOST_TEST(config2.getLBound() == 2.0);
	BOOST_TEST(config2.getUBound() == 4.0);

	// Operator Test
	config = config2;

	// Check all configuration parameters copied correctly
	BOOST_TEST(config.getLBound() == 2.0);
	BOOST_TEST(config.getUBound() == 4.0);
}

// === clone ===
BOOST_AUTO_TEST_CASE(clone_test1, * utf::tolerance(0.00001))
{

}

// === buildDistribution ===
BOOST_AUTO_TEST_CASE(buildDistribution_test1, * utf::tolerance(0.00001))
{

}
