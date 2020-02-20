/*
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Unit Tests for the methods of the Polyhedron class
 */


#define BOOST_TEST_MODULE Polyhedron
#include <boost/test/unit_test.hpp>
#include <boost/test/output_test_stream.hpp>
#include <stdexcept>

#include "Polyhedron.h"

using namespace cupcfd::geometry::shapes;
namespace euc = cupcfd::geometry::euclidean;
namespace utf = boost::unit_test;

// Note we only test static methods in this tester since many methods just passthrough
// to an inheriting class (CRTP)

// === findPolyhedronType ===
// Test 1: Tetrahedron
BOOST_AUTO_TEST_CASE(findPolyhedronType_test1, * utf::tolerance(0.00001))
{
	PolyhedronType type = findPolyhedronType(4,4);
	BOOST_CHECK_EQUAL(type, POLYHEDRON_TETRAHEDRON);
}

// Test 2: Quad Pyramid
BOOST_AUTO_TEST_CASE(findPolyhedronType_test2, * utf::tolerance(0.00001))
{
	PolyhedronType type = findPolyhedronType(5,5);
	BOOST_CHECK_EQUAL(type, POLYHEDRON_QUADPYRAMID);
}

// Test 3: TriPrism
BOOST_AUTO_TEST_CASE(findPolyhedronType_test3, * utf::tolerance(0.00001))
{
	PolyhedronType type = findPolyhedronType(6,5);
	BOOST_CHECK_EQUAL(type, POLYHEDRON_TRIPRISM);
}

// Test 4: TriPrism
BOOST_AUTO_TEST_CASE(findPolyhedronType_test4, * utf::tolerance(0.00001))
{
	PolyhedronType type = findPolyhedronType(8,6);
	BOOST_CHECK_EQUAL(type, POLYHEDRON_HEXAHEDRON);
}

// Test 5: Unknown Polyhedron
BOOST_AUTO_TEST_CASE(findPolyhedronType_test5, * utf::tolerance(0.00001))
{
	PolyhedronType type = findPolyhedronType(14,23);
	BOOST_CHECK_EQUAL(type, POLYHEDRON_UNKNOWN);
}
