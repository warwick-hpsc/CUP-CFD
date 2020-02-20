/*
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Unit Tests for the SparseMatrixSource class
 */

#define BOOST_TEST_MODULE SparseMatrixSource
#include <boost/test/unit_test.hpp>
#include <boost/test/output_test_stream.hpp>
#include <stdexcept>

#include "SparseMatrixSource.h"
#include "Error.h"

using namespace cupcfd::data_structures;

// === Constructors ===
BOOST_AUTO_TEST_CASE(constructor_test1)
{
	// Nothing to test
}

// Since most are virtual methods, we do no further tests here....
