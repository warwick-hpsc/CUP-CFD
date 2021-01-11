/*
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Unit Tests for the concrete methods of the abstract Partitioner class.
 */

#define BOOST_TEST_MODULE Partitioner
#include <boost/test/unit_test.hpp>
#include <boost/test/output_test_stream.hpp>
#include <stdexcept>

#include "PartitionerInterface.h"
#include "Error.h"

// Concrete Implementation of abstract class for testing purposes.
// We only wish to test the concrete methods, so abstract methods are
// implemented but left empty to fulfill overloading obligations

namespace cupcfd
{
	namespace partitioner
	{
		template <class I, class T>
		class PartitionerInterfaceTest : public PartitionerInterface<I, T>
		{
			public:
				PartitionerInterfaceTest(cupcfd::comm::Communicator& workComm)
				: PartitionerInterface<I,T>(workComm)
				{
					// Call parent constructor but do nothing else.
				}

				cupcfd::error::eCodes initialise(cupcfd::data_structures::DistributedAdjacencyList<I, T>& graph, I nParts)
				{
					return cupcfd::error::E_SUCCESS;
				}

				cupcfd::error::eCodes partition()
				{
					return cupcfd::error::E_SUCCESS;
				}
		};
	}
}

// ======= Setup ========
// Some tests require MPI
BOOST_AUTO_TEST_CASE(setup)
{
    int argc = boost::unit_test::framework::master_test_suite().argc;
    char ** argv = boost::unit_test::framework::master_test_suite().argv;
    MPI_Init(&argc, &argv);
}

using namespace cupcfd::partitioner;

// ========================================
// ============== Tests ===================
// ========================================

// ============== Constructors ===================
// Test 1: Empty constructor - Check default state
// Output:
// Result/nodes arrays should be nullptr, sizes should be 0
// nParts should be 0
BOOST_AUTO_TEST_CASE(constructor_test1)
{
	cupcfd::comm::Communicator comm(MPI_COMM_WORLD);
	PartitionerInterfaceTest<int,int> partitioner(comm);

	BOOST_CHECK_EQUAL(partitioner.nodes, static_cast<decltype(partitioner.nodes)>(nullptr));
	BOOST_CHECK_EQUAL(partitioner.nNodes, 0);
	BOOST_CHECK_EQUAL(partitioner.result, static_cast<decltype(partitioner.result)>(nullptr));
	BOOST_CHECK_EQUAL(partitioner.nResult, 0);
	BOOST_CHECK_EQUAL(partitioner.nParts, 0);
}

// ============== resetResultStorage ===================
// Test 1: Create a default object with no results setup
// When the results are nullptr, reset without error
// Output:
// Results storage is nullptr, size is 0
BOOST_AUTO_TEST_CASE(resetResultStorage_test1)
{
	cupcfd::comm::Communicator comm(MPI_COMM_WORLD);
	PartitionerInterfaceTest<int,int> partitioner(comm);
	cupcfd::error::eCodes status;

	// Test Method
	partitioner.resetResultStorage();
	BOOST_CHECK_EQUAL(partitioner.result, static_cast<decltype(partitioner.result)>(nullptr));
	BOOST_CHECK_EQUAL(partitioner.nResult, 0);
}

// Test 2: Manually set the results array to be non-nullptr and with a size
// Reset should clear this.
// Output:
// Results storage is nullptr, size is 0
BOOST_AUTO_TEST_CASE(resetResultStorage_test2)
{
	cupcfd::comm::Communicator comm(MPI_COMM_WORLD);
	PartitionerInterfaceTest<int,int> partitioner(comm);
	cupcfd::error::eCodes status;

	// Setup
	partitioner.result = (int *) malloc(sizeof(int) * 4);
	partitioner.nResult = 4;
	BOOST_CHECK_NE(partitioner.result, static_cast<decltype(partitioner.result)>(nullptr));
	BOOST_CHECK_EQUAL(partitioner.nResult, 4);

	// Test Method
	partitioner.resetResultStorage();
	BOOST_CHECK_EQUAL(partitioner.result, static_cast<decltype(partitioner.result)>(nullptr));
	BOOST_CHECK_EQUAL(partitioner.nResult, 0);
}

// === setNodeStorage ===
// Test 1: Copy the node data across correctly
BOOST_AUTO_TEST_CASE(setNodeStorage_test1)
{
	cupcfd::comm::Communicator comm(MPI_COMM_WORLD);
	PartitionerInterfaceTest<int,int> partitioner(comm);
	cupcfd::error::eCodes status;

	int testStore[5] = {10, 20, 30, 40, 50};

	partitioner.setNodeStorage(testStore, 5);
	// ToDo: This should use a getter function rather than internals
	BOOST_CHECK_EQUAL_COLLECTIONS(testStore, testStore + 5, partitioner.nodes, partitioner.nodes + 5);
}

// ============== resetNodeStorage ===================
// Test 1: Create a default object with no nodes setup
// When the nodes are nullptr, reset without error
// Output:
// Nodes storage is nullptr, size is 0
BOOST_AUTO_TEST_CASE(resetNodeStorage_test1)
{
	cupcfd::comm::Communicator comm(MPI_COMM_WORLD);
	PartitionerInterfaceTest<int,int> partitioner(comm);
	cupcfd::error::eCodes status;

	// Test Method
	partitioner.resetNodeStorage();
	BOOST_CHECK_EQUAL(partitioner.nodes, static_cast<decltype(partitioner.nodes)>(nullptr));
	BOOST_CHECK_EQUAL(partitioner.nNodes, 0);
}

// Test 2: Manually set the nodes array to be non-nullptr and with a size
// Reset should clear this.
// Output:
// Node storage is nullptr, size is 0
BOOST_AUTO_TEST_CASE(resetNodeStorage_test2)
{
	cupcfd::comm::Communicator comm(MPI_COMM_WORLD);
	PartitionerInterfaceTest<int,int> partitioner(comm);
	cupcfd::error::eCodes status;

	// Setup
	partitioner.nodes = (int *) malloc(sizeof(int) * 4);
	partitioner.nNodes = 4;
	BOOST_CHECK_NE(partitioner.nodes, static_cast<decltype(partitioner.nodes)>(nullptr));
	BOOST_CHECK_EQUAL(partitioner.nNodes, 4);

	// Test Method
	partitioner.resetNodeStorage();
	BOOST_CHECK_EQUAL(partitioner.nodes, static_cast<decltype(partitioner.nodes)>(nullptr));
	BOOST_CHECK_EQUAL(partitioner.nNodes, 0);
}

// === set/getNParts ===
// Test 1: Set Correctly
BOOST_AUTO_TEST_CASE(set_getNParts_test1)
{
	cupcfd::comm::Communicator comm(MPI_COMM_WORLD);
	PartitionerInterfaceTest<int,int> partitioner(comm);
	cupcfd::error::eCodes status;

	BOOST_CHECK_EQUAL(partitioner.getNParts(), 0);

	partitioner.setNParts(14);
	BOOST_CHECK_EQUAL(partitioner.getNParts(), 14);
}

// Test 2: Error Test: Set Negative partition Count
BOOST_AUTO_TEST_CASE(set_getNParts_test2)
{

}

// Test 3: Error Test: Set Zero Partition Count
BOOST_AUTO_TEST_CASE(set_getNParts_test3)
{

}

// === assignRankNodes ===
// Test 1: Test we can correctly distribute data across the distributeComm
BOOST_AUTO_TEST_CASE(assignRankNodes_test1)
{

}

// Test 2: Error Case: Called before there are results to distribute
BOOST_AUTO_TEST_CASE(assignRankNodes_test2)
{

}

// === reset ===
// Test 1: Check that the internal pointers are reset to nullptr
BOOST_AUTO_TEST_CASE(reset_test1)
{

}

// ======= Cleanup ========
// Finalize MPI
BOOST_AUTO_TEST_CASE(cleanup)
{
    // Cleanup MPI Environment
    MPI_Finalize();
}

