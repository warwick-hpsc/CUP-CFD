/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Tests for the concrete methods of the MeshSourceConfig class.
 */

#define BOOST_TEST_MODULE MeshSourceConfig

// Boost setup
#include <boost/test/unit_test.hpp>
#include <boost/test/output_test_stream.hpp>
#include <stdexcept>

#include "Error.h"
#include "MeshSourceConfig.h"


// Namespaces
namespace utf = boost::unit_test;

using namespace cupcfd::geometry::mesh;

// === Constructor ===

// Test 1: Test Empty Constructor Config sets values correctly
BOOST_AUTO_TEST_CASE(constructor_test1, * utf::tolerance(0.00001))
{
	// Virtual Class....
}
