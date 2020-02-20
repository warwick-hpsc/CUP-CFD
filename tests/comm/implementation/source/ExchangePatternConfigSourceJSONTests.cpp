/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Tests for the concrete methods of the ExchangePatternConfigSourceJSON class.
 */

#define BOOST_TEST_MODULE ExchangePatternConfigSourceJSON

// Boost setup
#include <boost/test/unit_test.hpp>
#include <boost/test/output_test_stream.hpp>
#include <stdexcept>

#include "ExchangePatternConfigSourceJSON.h"
#include "Error.h"

#include <string>

// Namespaces
namespace utf = boost::unit_test;

using namespace cupcfd::comm;

// === Constructor ===
// Test 1: Check Values are set up correctly
BOOST_AUTO_TEST_CASE(constructor_test1, * utf::tolerance(0.00001))
{
	std::string levels[2] = {"TopLevel1", "TopLevel2"};
	ExchangePatternConfigSourceJSON configFile("../tests/comm/TestData/ExchangePatternConfigSourceJSONTestInput.json", levels, 2);
}

// === getExchangeMethod ===
// Test 1: Looking up a One Sided Method
BOOST_AUTO_TEST_CASE(getExchangeMethod_test1, * utf::tolerance(0.00001))
{
	cupcfd::error::eCodes status;
	ExchangeMethod methodID;

	std::string levels[2] = {"TopLevel1", "TopLevel2"};
	ExchangePatternConfigSourceJSON configFile("../tests/comm/TestData/ExchangePatternConfigSourceJSONTestInput.json", levels, 2);

	status = configFile.getExchangeMethod(&methodID);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	BOOST_CHECK_EQUAL(methodID, EXCHANGE_NONBLOCKING_ONE_SIDED);
}

// Test 2: Looking up a Two Sided Method
BOOST_AUTO_TEST_CASE(getExchangeMethod_test2, * utf::tolerance(0.00001))
{
	cupcfd::error::eCodes status;
	ExchangeMethod methodID;

	std::string levels[2] = {"TopLevel1", "TopLevel3"};
	ExchangePatternConfigSourceJSON configFile("../tests/comm/TestData/ExchangePatternConfigSourceJSONTestInput.json", levels, 2);

	status = configFile.getExchangeMethod(&methodID);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	BOOST_CHECK_EQUAL(methodID, EXCHANGE_NONBLOCKING_TWO_SIDED);
}

// Test 3: Error Test: Option is missing
BOOST_AUTO_TEST_CASE(getExchangeMethod_test3, * utf::tolerance(0.00001))
{
	std::string levels[1] = {"TopLevel4"};
	ExchangeMethod methodID;
	cupcfd::error::eCodes status;

	ExchangePatternConfigSourceJSON configFile("../tests/comm/TestData/ExchangePatternConfigSourceJSONTestInput.json", levels, 1);
	status = configFile.getExchangeMethod(&methodID);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_CONFIG_OPT_NOT_FOUND);
}

// Test 4: Error Test: Invalid value provided
BOOST_AUTO_TEST_CASE(getExchangeMethod_test4, * utf::tolerance(0.00001))
{
	std::string levels[1] = {"TopLevel5"};
	ExchangeMethod methodID;
	cupcfd::error::eCodes status;

	ExchangePatternConfigSourceJSON configFile("../tests/comm/TestData/ExchangePatternConfigSourceJSONTestInput.json", levels, 1);
	status = configFile.getExchangeMethod(&methodID);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_CONFIG_INVALID_VALUE);
}

// === buildExchangePatternConfig ===
// Test 1: Test the correct creation and setup of an ExchangePatternConfig for One-Sided
BOOST_AUTO_TEST_CASE(buildExchangePatternConfig2_test1, * utf::tolerance(0.00001))
{
	cupcfd::error::eCodes status;
	ExchangePatternConfig * config;
	std::string levels[2] = {"TopLevel1", "TopLevel2"};
	ExchangePatternConfigSourceJSON source("../tests/comm/TestData/ExchangePatternConfigSourceJSONTestInput.json", levels, 2);

	// Build Pattern - This should be a One Sided Method
	status = source.buildExchangePatternConfig(&config);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	BOOST_CHECK_EQUAL(config->getExchangeMethod(), EXCHANGE_NONBLOCKING_ONE_SIDED);
}

// Test 2: Test the correct creation and setup of an ExchangePatternConfig  for Two-Sided
BOOST_AUTO_TEST_CASE(buildExchangePatternConfig2_test2, * utf::tolerance(0.00001))
{
	cupcfd::error::eCodes status;
	ExchangePatternConfig * config;
	std::string levels[2] = {"TopLevel1", "TopLevel3"};
	ExchangePatternConfigSourceJSON source("../tests/comm/TestData/ExchangePatternConfigSourceJSONTestInput.json", levels, 2);

	// Build Pattern - This should be a Two Sided Method
	status = source.buildExchangePatternConfig(&config);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	BOOST_CHECK_EQUAL(config->getExchangeMethod(), EXCHANGE_NONBLOCKING_TWO_SIDED);
}
