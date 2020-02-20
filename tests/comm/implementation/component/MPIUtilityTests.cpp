/*
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 */

#define BOOST_TEST_MODULE MPIUtilityTests
#include <boost/test/unit_test.hpp>
#include <boost/test/output_test_stream.hpp>

#include <stdexcept>

#include "Communicator.h"
#include "MPIUtility.h"
#include "Error.h"
#include "CustomMPIType.h"

using namespace cupcfd::comm::mpi;

// Setup MPI
BOOST_AUTO_TEST_CASE(setup)
{
    int argc = boost::unit_test::framework::master_test_suite().argc;
    char ** argv = boost::unit_test::framework::master_test_suite().argv;
    MPI_Init(&argc, &argv);
}

// === getMPIType ===
// Test 1: Get MPI Int Type
BOOST_AUTO_TEST_CASE(getMPIType_test1)
{
	int dummy;
	cupcfd::error::eCodes status;
	MPI_Datatype dataType;

	status = getMPIType(dummy, &dataType);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	BOOST_CHECK_EQUAL(dataType, MPI_INT);
}

// Test 2: Get MPI Float Type
BOOST_AUTO_TEST_CASE(getMPIType_test2)
{
	float dummy;
	cupcfd::error::eCodes status;
	MPI_Datatype dataType;

	status = getMPIType(dummy, &dataType);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	BOOST_CHECK_EQUAL(dataType, MPI_FLOAT);
}

// Test 3: Get MPI Double Type
BOOST_AUTO_TEST_CASE(getMPIType_test3)
{
	double dummy;
	cupcfd::error::eCodes status;
	MPI_Datatype dataType;

	status = getMPIType(dummy, &dataType);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	BOOST_CHECK_EQUAL(dataType, MPI_DOUBLE);
}

// Test 4: Get MPI Long Type
BOOST_AUTO_TEST_CASE(getMPIType_test4)
{
	long dummy;
	cupcfd::error::eCodes status;
	MPI_Datatype dataType;

	status = getMPIType(dummy, &dataType);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	BOOST_CHECK_EQUAL(dataType, MPI_LONG);
}

// Test 5: Get MPI of a CustomMPIType when registered
// Unsure of best way to test this
// Theoretically, it just passes through to CustomType methods so it
// should just work without further testing - don't want to have to test
// this for every custommpitype since we would want to test the method for
// each class directly, not via this interface method....

// Cleanup MPI Environment
BOOST_AUTO_TEST_CASE(cleanup)
{
    MPI_Finalize();
}
