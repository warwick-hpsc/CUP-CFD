/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Tests for the concrete methods of the DistributionConfigSourceUniformJSON class.
 */

#define BOOST_TEST_MODULE DistributionConfigSourceUniformJSON

// Boost setup
#include <boost/test/unit_test.hpp>
#include <boost/test/output_test_stream.hpp>
#include <stdexcept>

#include "DistributionConfigSourceUniformJSON.h"
#include "Error.h"

// Namespaces
namespace utf = boost::unit_test;

using namespace cupcfd::distributions;

// === Constructor ===
// Test 1: Parameter Constructor
BOOST_AUTO_TEST_CASE(constructor_test1, * utf::tolerance(0.00001))
{
	std::string topLevels[1] = {"Level1"};
	DistributionConfigSourceUniformJSON<int,double> jsonConfigSource("../tests/distributions/data/DistributionConfigSourceUniformTest.json", topLevels, 1);
}

// Test 2: Configuration Copy Constructor
BOOST_AUTO_TEST_CASE(constructor_test2, * utf::tolerance(0.00001))
{

}

// === operator= ===
// Test 1: Test correct copy
BOOST_AUTO_TEST_CASE(operator_equals_test1, * utf::tolerance(0.00001))
{

}

// === clone ===
BOOST_AUTO_TEST_CASE(clone_test1, * utf::tolerance(0.00001))
{

}

// === getLBound ===
BOOST_AUTO_TEST_CASE(getLBound_test1, * utf::tolerance(0.00001))
{
	std::string topLevels[1] = {"Level1"};
	DistributionConfigSourceUniformJSON<int,double> jsonConfigSource("../tests/distributions/data/DistributionConfigSourceUniformTest.json", topLevels, 1);

	double lBound;
	cupcfd::error::eCodes status;
	status = jsonConfigSource.getLBound(&lBound);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	BOOST_TEST(lBound == 12.0);
}

// === getUBound ===
BOOST_AUTO_TEST_CASE(getUBound_test1, * utf::tolerance(0.00001))
{
	std::string topLevels[1] = {"Level1"};
	DistributionConfigSourceUniformJSON<int,double> jsonConfigSource("../tests/distributions/data/DistributionConfigSourceUniformTest.json", topLevels, 1);

	double uBound;
	cupcfd::error::eCodes status;
	status = jsonConfigSource.getUBound(&uBound);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	BOOST_TEST(uBound == 14.7);
}

// === buildDistributionConfig ===
BOOST_AUTO_TEST_CASE(buildDistribution_test1, * utf::tolerance(0.00001))
{
	cupcfd::error::eCodes status;

	std::string topLevels[1] = {"Level1"};
	DistributionConfigSourceUniformJSON<int,double> jsonConfigSource("../tests/distributions/data/DistributionConfigSourceUniformTest.json", topLevels, 1);

	DistributionConfig<int,double> * distConfig;
	status = jsonConfigSource.buildDistributionConfig(&distConfig);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
}
