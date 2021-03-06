/*
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Unit Tests for the DistributedAdjacencyList class
 */

#define BOOST_TEST_MODULE DistributedAdjacencyList
#include <boost/test/unit_test.hpp>
#include <boost/test/output_test_stream.hpp>
#include <stdexcept>

#include "DistributedAdjacencyList.h"
#include "Communicator.h"
#include "Error.h"

#include "SortDrivers.h"

// ToDo: These tests need tidying up

using namespace cupcfd::data_structures;

// These tests require MPI
BOOST_AUTO_TEST_CASE(setup)
{
    int argc = boost::unit_test::framework::master_test_suite().argc;
    char ** argv = boost::unit_test::framework::master_test_suite().argv;
    MPI_Init(&argc, &argv);
}

// === Constructors/Deconstructors ===
// Test 1:
BOOST_AUTO_TEST_CASE(constructor_test1)
{

}


// === reset ===
// Test 1: Test a successful reset
BOOST_AUTO_TEST_CASE(reset_test1)
{
	// Setup a Graph and add a few elements

	// Test and Check - reset and check contents are gone.
}

// === operator= ===
// Test 1: Test the contents are correctly copied over to an empty graph

// Test 2: Test the contents are correctly copied over the top of a graph
// with existing data (i.e. the destination graph is reset prior)


// === existsLocalNode ===
// Test 1: Test non-existant node is identified as missing in empty graph
BOOST_AUTO_TEST_CASE(existsLocalNode_test1)
{
	cupcfd::comm::Communicator comm(MPI_COMM_WORLD);
	DistributedAdjacencyList<int, int> list(comm);

	bool found;

	if(comm.rank == 0)
	{
		found = list.existsLocalNode(5);
		BOOST_CHECK_EQUAL(found, false);
	}
}

// Test 2: Test non-existant node is identified as missing in non-empty graph
BOOST_AUTO_TEST_CASE(existsLocalNode_test2)
{
	cupcfd::comm::Communicator comm(MPI_COMM_WORLD);
	DistributedAdjacencyList<int, int> list(comm);
	bool found;

	if(comm.rank == 0)
	{
		// Manually add a node directly
		list.nodeDistType[10] = LOCAL;
		found = list.existsLocalNode(5);
		BOOST_CHECK_EQUAL(found, false);
	}
}

// Test 3: Test that existing node but not LOCAL type is identified as not present as
// a local node (false)
BOOST_AUTO_TEST_CASE(existsLocalNode_test3)
{
	cupcfd::comm::Communicator comm(MPI_COMM_WORLD);
	DistributedAdjacencyList<int, int> list(comm);
	bool found;

	if(comm.rank == 0)
	{
		// Manually add a node directly
		list.nodeDistType[5] = GHOST;
		found = list.existsLocalNode(5);
		BOOST_CHECK_EQUAL(found, false);
	}
}

// Test 4: Test that existing node of correct LOCAL type is identified as present (true)
BOOST_AUTO_TEST_CASE(existsLocalNode_test4)
{
	cupcfd::comm::Communicator comm(MPI_COMM_WORLD);
	DistributedAdjacencyList<int, int> list(comm);
	bool found;

	if(comm.rank == 0)
	{
		// Manually add a node directly
		list.nodeDistType[5] = LOCAL;
		found = list.existsLocalNode(5);
		BOOST_CHECK_EQUAL(found, true);
	}
}

// === existsGhostNode ===
// Test 1:
BOOST_AUTO_TEST_CASE(driver_existsGhostNode_test_empty)
{
	cupcfd::comm::Communicator comm(MPI_COMM_WORLD);
	DistributedAdjacencyList<int, int> list(comm);

	bool found;

	if(comm.rank == 0)
	{
		found = list.existsGhostNode(5);
		BOOST_CHECK_EQUAL(found, false);
	}
}

BOOST_AUTO_TEST_CASE(driver_existsGhostNode_nonEmpty_notFound)
{
	cupcfd::comm::Communicator comm(MPI_COMM_WORLD);
	DistributedAdjacencyList<int, int> list(comm);
	bool found;

	if(comm.rank == 0)
	{
		// Manually add a node directly
		list.nodeDistType[10] = LOCAL;
		found = list.existsGhostNode(5);
		BOOST_CHECK_EQUAL(found, false);
	}
}

BOOST_AUTO_TEST_CASE(driver_existsGhostNode_nonEmpty_wrongType)
{
	cupcfd::comm::Communicator comm(MPI_COMM_WORLD);
	DistributedAdjacencyList<int, int> list(comm);
	bool found;

	if(comm.rank == 0)
	{
		// Manually add a node directly
		list.nodeDistType[5] = LOCAL;
		found = list.existsGhostNode(5);
		BOOST_CHECK_EQUAL(found, false);
	}
}

BOOST_AUTO_TEST_CASE(driver_existsGhostNode_nonEmpty_rightType)
{
	cupcfd::comm::Communicator comm(MPI_COMM_WORLD);
	DistributedAdjacencyList<int, int> list(comm);
	bool found;

	if(comm.rank == 0)
	{
		// Manually add a node directly
		list.nodeDistType[5] = GHOST;
		found = list.existsGhostNode(5);
		BOOST_CHECK_EQUAL(found, true);
	}
}

// === existsNode ===
// Test 1:
BOOST_AUTO_TEST_CASE(driver_existsNode_test_empty)
{
	cupcfd::comm::Communicator comm(MPI_COMM_WORLD);
	DistributedAdjacencyList<int, int> list(comm);

	bool found;

	if(comm.rank == 0)
	{
		found = list.existsNode(5);
		BOOST_CHECK_EQUAL(found, false);
	}
}

BOOST_AUTO_TEST_CASE(driver_existsNode_nonEmpty_notFound)
{
	cupcfd::comm::Communicator comm(MPI_COMM_WORLD);
	DistributedAdjacencyList<int, int> list(comm);
	bool found;

	if(comm.rank == 0)
	{
		// Manually add a node directly
		list.nodeDistType[10] = LOCAL;
		found = list.existsNode(5);
		BOOST_CHECK_EQUAL(found, false);
	}
}

BOOST_AUTO_TEST_CASE(driver_existsNode_nonEmpty_found_local)
{
	cupcfd::comm::Communicator comm(MPI_COMM_WORLD);
	DistributedAdjacencyList<int, int> list(comm);
	bool found;

	if(comm.rank == 0)
	{
		// Manually add a node directly
		list.nodeDistType[5] = LOCAL;
		found = list.existsNode(5);
		BOOST_CHECK_EQUAL(found, true);
	}
}

BOOST_AUTO_TEST_CASE(driver_existsNode_nonEmpty_found_ghost)
{
	cupcfd::comm::Communicator comm(MPI_COMM_WORLD);
	DistributedAdjacencyList<int, int> list(comm);
	bool found;

	if(comm.rank == 0)
	{
		// Manually add a node directly
		list.nodeDistType[5] = GHOST;
		found = list.existsNode(5);
		BOOST_CHECK_EQUAL(found, true);
	}
}


// === addLocalNode ===
// Test 1:
BOOST_AUTO_TEST_CASE(driver_addLocalNode_test_empty)
{
	cupcfd::comm::Communicator comm(MPI_COMM_WORLD);
	DistributedAdjacencyList<int, int> list(comm);

	bool found;

	if(comm.rank == 0)
	{
		// Check the node does not yet exist
		found = list.existsNode(5);
		BOOST_CHECK_EQUAL(found, false);

		cupcfd::error::eCodes status = list.addLocalNode(5);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

		// Check Local Owned Node Count
		BOOST_CHECK_EQUAL(list.nLONodes, 1);

		// Check Ghost Count has not changed
		BOOST_CHECK_EQUAL(list.nLGhNodes, 0);

		// Check the node now exists
		found = list.existsNode(5);
		BOOST_CHECK_EQUAL(found, true);

		// Check it exists as a local node and not a ghost node
		found = list.existsLocalNode(5);
		BOOST_CHECK_EQUAL(found, true);

		found = list.existsGhostNode(5);
		BOOST_CHECK_EQUAL(found, false);
	}
}

BOOST_AUTO_TEST_CASE(driver_addLocalNode_test_addNodeTwice)
{
	cupcfd::comm::Communicator comm(MPI_COMM_WORLD);
	DistributedAdjacencyList<int, int> list(comm);

	bool found;

	if(comm.rank == 0)
	{
		// Check the node does not yet exist
		found = list.existsNode(5);
		BOOST_CHECK_EQUAL(found, false);

		// First add should be successful
		cupcfd::error::eCodes status = list.addLocalNode(5);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

		// Second add should leave list unmodified and report correct error code.
		status = list.addLocalNode(5);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_ADJACENCY_LIST_NODE_EXISTS);

		// Check Local Owned Node Count
		// Should only be 1 - i.e. not added twice but still exists once
		BOOST_CHECK_EQUAL(list.nLONodes, 1);

		// Check Ghost Count has not changed
		BOOST_CHECK_EQUAL(list.nLGhNodes, 0);

		// Check the node now exists
		found = list.existsNode(5);
		BOOST_CHECK_EQUAL(found, true);

		// Check it exists as a local node and not a ghost node
		found = list.existsLocalNode(5);
		BOOST_CHECK_EQUAL(found, true);

		found = list.existsGhostNode(5);
		BOOST_CHECK_EQUAL(found, false);
	}
}


BOOST_AUTO_TEST_CASE(driver_addLocalNode_test_addMultiple)
{
	cupcfd::comm::Communicator comm(MPI_COMM_WORLD);
	DistributedAdjacencyList<int, int> list(comm);

	bool found;

	if(comm.rank == 0)
	{
		// Check the node does not yet exist
		found = list.existsNode(5);
		BOOST_CHECK_EQUAL(found, false);

		cupcfd::error::eCodes status = list.addLocalNode(5);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

		status = list.addLocalNode(10);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

		status = list.addLocalNode(12);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

		status = list.addLocalNode(2);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

		// Check Local Owned Node Count
		BOOST_CHECK_EQUAL(list.nLONodes, 4);

		// Check Ghost Count has not changed
		BOOST_CHECK_EQUAL(list.nLGhNodes, 0);

		// Check the node now exists
		found = list.existsNode(5);
		BOOST_CHECK_EQUAL(found, true);

		// Check it exists as a local node and not a ghost node
		found = list.existsLocalNode(5);
		BOOST_CHECK_EQUAL(found, true);

		found = list.existsGhostNode(5);
		BOOST_CHECK_EQUAL(found, false);

		found = list.existsNode(10);
		BOOST_CHECK_EQUAL(found, true);

		// Check it exists as a local node and not a ghost node
		found = list.existsLocalNode(10);
		BOOST_CHECK_EQUAL(found, true);

		found = list.existsGhostNode(10);
		BOOST_CHECK_EQUAL(found, false);

		found = list.existsNode(12);
		BOOST_CHECK_EQUAL(found, true);

		// Check it exists as a local node and not a ghost node
		found = list.existsLocalNode(12);
		BOOST_CHECK_EQUAL(found, true);

		found = list.existsGhostNode(12);
		BOOST_CHECK_EQUAL(found, false);

		found = list.existsNode(2);
		BOOST_CHECK_EQUAL(found, true);

		// Check it exists as a local node and not a ghost node
		found = list.existsLocalNode(2);
		BOOST_CHECK_EQUAL(found, true);

		found = list.existsGhostNode(2);
		BOOST_CHECK_EQUAL(found, false);

		// Check a random non-existant node does not exist
		found = list.existsNode(7);
		BOOST_CHECK_EQUAL(found, false);

		// Check it exists as a local node and not a ghost node
		found = list.existsLocalNode(7);
		BOOST_CHECK_EQUAL(found, false);

		found = list.existsGhostNode(7);
		BOOST_CHECK_EQUAL(found, false);

		// Test after trying to add a duplicate that a random node does not exist
		status = list.addLocalNode(8);

		found = list.existsNode(7);
		BOOST_CHECK_EQUAL(found, false);

		// Check it exists as a local node and not a ghost node
		found = list.existsLocalNode(7);
		BOOST_CHECK_EQUAL(found, false);

		found = list.existsGhostNode(7);
		BOOST_CHECK_EQUAL(found, false);
	}
}


// === addGhostNode ===
// Test 1:
BOOST_AUTO_TEST_CASE(driver_addGhostNode_test_empty)
{
	cupcfd::comm::Communicator comm(MPI_COMM_WORLD);
	DistributedAdjacencyList<int, int> list(comm);

	bool found;

	if(comm.rank == 0)
	{
		// Check the node does not yet exist
		found = list.existsNode(5);
		BOOST_CHECK_EQUAL(found, false);

		cupcfd::error::eCodes status = list.addGhostNode(5);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

		// Check Local Owned Node Count
		BOOST_CHECK_EQUAL(list.nLONodes, 0);

		// Check Ghost Count has not changed
		BOOST_CHECK_EQUAL(list.nLGhNodes, 1);

		// Check the node now exists
		found = list.existsNode(5);
		BOOST_CHECK_EQUAL(found, true);

		// Check it exists as a local node and not a ghost node
		found = list.existsLocalNode(5);
		BOOST_CHECK_EQUAL(found, false);

		found = list.existsGhostNode(5);
		BOOST_CHECK_EQUAL(found, true);
	}
}

BOOST_AUTO_TEST_CASE(driver_addGhostNode_test_addNodeTwice)
{
	cupcfd::comm::Communicator comm(MPI_COMM_WORLD);
	DistributedAdjacencyList<int, int> list(comm);

	bool found;

	if(comm.rank == 0)
	{
		// Check the node does not yet exist
		found = list.existsNode(5);
		BOOST_CHECK_EQUAL(found, false);

		// First add should be successful
		cupcfd::error::eCodes status = list.addGhostNode(5);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

		// Second add should leave list unmodified and report correct error code.
		status = list.addGhostNode(5);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_ADJACENCY_LIST_NODE_EXISTS);

		// Check Local Owned Node Count
		// Should only be 1 - i.e. not added twice but still exists once
		BOOST_CHECK_EQUAL(list.nLONodes, 0);

		// Check Ghost Count has not changed
		BOOST_CHECK_EQUAL(list.nLGhNodes, 1);

		// Check the node now exists
		found = list.existsNode(5);
		BOOST_CHECK_EQUAL(found, true);

		// Check it exists as a local node and not a ghost node
		found = list.existsLocalNode(5);
		BOOST_CHECK_EQUAL(found, false);

		found = list.existsGhostNode(5);
		BOOST_CHECK_EQUAL(found, true);
	}
}


BOOST_AUTO_TEST_CASE(driver_addGhostNode_test_addMultiple)
{
	cupcfd::comm::Communicator comm(MPI_COMM_WORLD);
	DistributedAdjacencyList<int, int> list(comm);

	bool found;

	if(comm.rank == 0)
	{
		// Check the node does not yet exist
		found = list.existsNode(5);
		BOOST_CHECK_EQUAL(found, false);

		cupcfd::error::eCodes status = list.addGhostNode(5);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

		status = list.addGhostNode(10);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

		status = list.addGhostNode(12);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

		status = list.addGhostNode(2);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

		// Check Local Owned Node Count
		BOOST_CHECK_EQUAL(list.nLONodes, 0);

		// Check Ghost Count has not changed
		BOOST_CHECK_EQUAL(list.nLGhNodes, 4);

		// Check the node now exists
		found = list.existsNode(5);
		BOOST_CHECK_EQUAL(found, true);

		// Check it exists as a local node and not a ghost node
		found = list.existsLocalNode(5);
		BOOST_CHECK_EQUAL(found, false);

		found = list.existsGhostNode(5);
		BOOST_CHECK_EQUAL(found, true);

		found = list.existsNode(10);
		BOOST_CHECK_EQUAL(found, true);

		// Check it exists as a local node and not a ghost node
		found = list.existsLocalNode(10);
		BOOST_CHECK_EQUAL(found, false);

		found = list.existsGhostNode(10);
		BOOST_CHECK_EQUAL(found, true);

		found = list.existsNode(12);
		BOOST_CHECK_EQUAL(found, true);

		// Check it exists as a local node and not a ghost node
		found = list.existsLocalNode(12);
		BOOST_CHECK_EQUAL(found, false);

		found = list.existsGhostNode(12);
		BOOST_CHECK_EQUAL(found, true);

		found = list.existsNode(2);
		BOOST_CHECK_EQUAL(found, true);

		// Check it exists as a local node and not a ghost node
		found = list.existsLocalNode(2);
		BOOST_CHECK_EQUAL(found, false);

		found = list.existsGhostNode(2);
		BOOST_CHECK_EQUAL(found, true);

		// Check a random non-existant node does not exist
		found = list.existsNode(7);
		BOOST_CHECK_EQUAL(found, false);

		// Check it exists as a local node and not a ghost node
		found = list.existsLocalNode(7);
		BOOST_CHECK_EQUAL(found, false);

		found = list.existsGhostNode(7);
		BOOST_CHECK_EQUAL(found, false);

		// Test after trying to add a duplicate that a random node does not exist
		status = list.addGhostNode(8);

		found = list.existsNode(7);
		BOOST_CHECK_EQUAL(found, false);

		// Check it exists as a local node and not a ghost node
		found = list.existsLocalNode(7);
		BOOST_CHECK_EQUAL(found, false);

		found = list.existsGhostNode(7);
		BOOST_CHECK_EQUAL(found, false);
	}
}


// === addNode ===
// Test 1:
BOOST_AUTO_TEST_CASE(driver_addNode_test_empty)
{
	cupcfd::comm::Communicator comm(MPI_COMM_WORLD);
	DistributedAdjacencyList<int, int> list(comm);

	bool found;

	if(comm.rank == 0)
	{
		// Check the node does not yet exist
		found = list.existsNode(5);
		BOOST_CHECK_EQUAL(found, false);

		cupcfd::error::eCodes status = list.addNode(5);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

		// Check Local Owned Node Count
		BOOST_CHECK_EQUAL(list.nLONodes, 0);

		// Check Ghost Count has not changed
		BOOST_CHECK_EQUAL(list.nLGhNodes, 1);

		// Check the node now exists
		found = list.existsNode(5);
		BOOST_CHECK_EQUAL(found, true);

		// Check it exists as a local node and not a ghost node
		found = list.existsLocalNode(5);
		BOOST_CHECK_EQUAL(found, false);

		found = list.existsGhostNode(5);
		BOOST_CHECK_EQUAL(found, true);
	}
}

BOOST_AUTO_TEST_CASE(driver_addNode_test_addNodeTwice)
{
	cupcfd::comm::Communicator comm(MPI_COMM_WORLD);
	DistributedAdjacencyList<int, int> list(comm);

	bool found;

	if(comm.rank == 0)
	{
		// Check the node does not yet exist
		found = list.existsNode(5);
		BOOST_CHECK_EQUAL(found, false);

		// First add should be successful
		cupcfd::error::eCodes status = list.addNode(5);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

		// Second add should leave list unmodified and report correct error code.
		status = list.addNode(5);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_ADJACENCY_LIST_NODE_EXISTS);

		// Check Local Owned Node Count
		// Should only be 1 - i.e. not added twice but still exists once
		BOOST_CHECK_EQUAL(list.nLONodes, 0);

		// Check Ghost Count has not changed
		BOOST_CHECK_EQUAL(list.nLGhNodes, 1);

		// Check the node now exists
		found = list.existsNode(5);
		BOOST_CHECK_EQUAL(found, true);

		// Check it exists as a local node and not a ghost node
		found = list.existsLocalNode(5);
		BOOST_CHECK_EQUAL(found, false);

		found = list.existsGhostNode(5);
		BOOST_CHECK_EQUAL(found, true);
	}
}


BOOST_AUTO_TEST_CASE(driver_addNode_test_addMultiple)
{
	cupcfd::comm::Communicator comm(MPI_COMM_WORLD);
	DistributedAdjacencyList<int, int> list(comm);

	bool found;

	if(comm.rank == 0)
	{
		// Check the node does not yet exist
		found = list.existsNode(5);
		BOOST_CHECK_EQUAL(found, false);

		cupcfd::error::eCodes status = list.addNode(5);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

		status = list.addNode(10);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

		status = list.addNode(12);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

		status = list.addNode(2);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

		// Check Local Owned Node Count
		BOOST_CHECK_EQUAL(list.nLONodes, 0);

		// Check Ghost Count has not changed
		BOOST_CHECK_EQUAL(list.nLGhNodes, 4);

		// Check the node now exists
		found = list.existsNode(5);
		BOOST_CHECK_EQUAL(found, true);

		// Check it exists as a local node and not a ghost node
		found = list.existsLocalNode(5);
		BOOST_CHECK_EQUAL(found, false);

		found = list.existsGhostNode(5);
		BOOST_CHECK_EQUAL(found, true);

		found = list.existsNode(10);
		BOOST_CHECK_EQUAL(found, true);

		// Check it exists as a local node and not a ghost node
		found = list.existsLocalNode(10);
		BOOST_CHECK_EQUAL(found, false);

		found = list.existsGhostNode(10);
		BOOST_CHECK_EQUAL(found, true);

		found = list.existsNode(12);
		BOOST_CHECK_EQUAL(found, true);

		// Check it exists as a local node and not a ghost node
		found = list.existsLocalNode(12);
		BOOST_CHECK_EQUAL(found, false);

		found = list.existsGhostNode(12);
		BOOST_CHECK_EQUAL(found, true);

		found = list.existsNode(2);
		BOOST_CHECK_EQUAL(found, true);

		// Check it exists as a local node and not a ghost node
		found = list.existsLocalNode(2);
		BOOST_CHECK_EQUAL(found, false);

		found = list.existsGhostNode(2);
		BOOST_CHECK_EQUAL(found, true);

		// Check a random non-existant node does not exist
		found = list.existsNode(7);
		BOOST_CHECK_EQUAL(found, false);

		// Check it exists as a local node and not a ghost node
		found = list.existsLocalNode(7);
		BOOST_CHECK_EQUAL(found, false);

		found = list.existsGhostNode(7);
		BOOST_CHECK_EQUAL(found, false);

		// Test after trying to add a duplicate that a random node does not exist
		status = list.addNode(8);

		found = list.existsNode(7);
		BOOST_CHECK_EQUAL(found, false);

		// Check it exists as a local node and not a ghost node
		found = list.existsLocalNode(7);
		BOOST_CHECK_EQUAL(found, false);

		found = list.existsGhostNode(7);
		BOOST_CHECK_EQUAL(found, false);
	}
}


BOOST_AUTO_TEST_CASE(driver_addNode_test_addNodeTwice_mixture)
{
	cupcfd::comm::Communicator comm(MPI_COMM_WORLD);
	DistributedAdjacencyList<int, int> list(comm);

	bool found;

	if(comm.rank == 0)
	{
		// Check the node does not yet exist
		found = list.existsNode(5);
		BOOST_CHECK_EQUAL(found, false);

		// First add should be successful
		cupcfd::error::eCodes status = list.addLocalNode(5);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

		// Second add should leave list unmodified and report correct error code, even if it is of a different type
		status = list.addGhostNode(5);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_ADJACENCY_LIST_NODE_EXISTS);

		// Check Local Owned Node Count
		// Should only be 1 - i.e. not added twice but still exists once
		BOOST_CHECK_EQUAL(list.nLONodes, 1);

		// Check Ghost Count has not changed
		BOOST_CHECK_EQUAL(list.nLGhNodes, 0);

		// Check the node now exists
		found = list.existsNode(5);
		BOOST_CHECK_EQUAL(found, true);

		// Check it exists as a local node and not a ghost node
		found = list.existsLocalNode(5);
		BOOST_CHECK_EQUAL(found, true);

		found = list.existsGhostNode(5);
		BOOST_CHECK_EQUAL(found, false);
	}
}


BOOST_AUTO_TEST_CASE(driver_addNode_test_addMultiple_mixture)
{
	cupcfd::comm::Communicator comm(MPI_COMM_WORLD);
	DistributedAdjacencyList<int, int> list(comm);

	bool found;

	if(comm.rank == 0)
	{
		// Check the node does not yet exist
		found = list.existsNode(5);
		BOOST_CHECK_EQUAL(found, false);

		cupcfd::error::eCodes status = list.addLocalNode(5);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

		status = list.addGhostNode(10);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

		status = list.addGhostNode(12);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

		status = list.addLocalNode(2);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

		// Check Local Owned Node Count
		BOOST_CHECK_EQUAL(list.nLONodes, 2);

		// Check Ghost Count has not changed
		BOOST_CHECK_EQUAL(list.nLGhNodes, 2);

		// Check the node now exists
		found = list.existsNode(5);
		BOOST_CHECK_EQUAL(found, true);

		// Check it exists as a local node and not a ghost node
		found = list.existsLocalNode(5);
		BOOST_CHECK_EQUAL(found, true);

		found = list.existsGhostNode(5);
		BOOST_CHECK_EQUAL(found, false);

		found = list.existsNode(10);
		BOOST_CHECK_EQUAL(found, true);

		// Check it exists as a local node and not a ghost node
		found = list.existsLocalNode(10);
		BOOST_CHECK_EQUAL(found, false);

		found = list.existsGhostNode(10);
		BOOST_CHECK_EQUAL(found, true);

		found = list.existsNode(12);
		BOOST_CHECK_EQUAL(found, true);

		// Check it exists as a local node and not a ghost node
		found = list.existsLocalNode(12);
		BOOST_CHECK_EQUAL(found, false);

		found = list.existsGhostNode(12);
		BOOST_CHECK_EQUAL(found, true);

		found = list.existsNode(2);
		BOOST_CHECK_EQUAL(found, true);

		// Check it exists as a local node and not a ghost node
		found = list.existsLocalNode(2);
		BOOST_CHECK_EQUAL(found, true);

		found = list.existsGhostNode(2);
		BOOST_CHECK_EQUAL(found, false);

		// Check a random non-existant node does not exist
		found = list.existsNode(7);
		BOOST_CHECK_EQUAL(found, false);

		// Check it exists as a local node and not a ghost node
		found = list.existsLocalNode(7);
		BOOST_CHECK_EQUAL(found, false);

		found = list.existsGhostNode(7);
		BOOST_CHECK_EQUAL(found, false);

		// Test after trying to add a duplicate that a random node does not exist
		status = list.addLocalNode(8);

		found = list.existsNode(7);
		BOOST_CHECK_EQUAL(found, false);

		// Check it exists as a local node and not a ghost node
		found = list.existsLocalNode(7);
		BOOST_CHECK_EQUAL(found, false);

		found = list.existsGhostNode(7);
		BOOST_CHECK_EQUAL(found, false);
	}
}

// === existsEdge ===
// Test 1:
BOOST_AUTO_TEST_CASE(driver_existsEdge_srcNode_notfound)
{
	cupcfd::comm::Communicator comm(MPI_COMM_WORLD);
	DistributedAdjacencyList<int, int> list(comm);
	cupcfd::error::eCodes status;

	bool found;

	if(comm.rank == 0)
	{
		status = list.addLocalNode(1);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
		status = list.addLocalNode(2);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

		// Add an edge directly to the underlying data structure
		status = list.buildGraph.addEdge(1, 2);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

		// Test for the edges existance via the distributed layer on top
		cupcfd::error::eCodes status = list.existsEdge(3, 2, &found);

		BOOST_CHECK_EQUAL(status, cupcfd::error::E_ADJACENCY_LIST_NODE_MISSING);
	}
}

BOOST_AUTO_TEST_CASE(driver_existsEdge_dstNode_notfound)
{
	cupcfd::comm::Communicator comm(MPI_COMM_WORLD);
	DistributedAdjacencyList<int, int> list(comm);
	cupcfd::error::eCodes status;

	bool found;

	if(comm.rank == 0)
	{
		status = list.addLocalNode(1);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
		status = list.addLocalNode(2);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

		// Add an edge directly to the underlying data structure
		status = list.buildGraph.addEdge(1, 2);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

		// Test for the edges existance via the distributed layer on top
		cupcfd::error::eCodes status = list.existsEdge(1, 102, &found);

		BOOST_CHECK_EQUAL(status, cupcfd::error::E_ADJACENCY_LIST_NODE_MISSING);
	}
}

BOOST_AUTO_TEST_CASE(driver_existsEdge_notfound)
{
	cupcfd::comm::Communicator comm(MPI_COMM_WORLD);
	DistributedAdjacencyList<int, int> list(comm);
	cupcfd::error::eCodes status;

	bool found;

	if(comm.rank == 0)
	{
		status = list.addLocalNode(1);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
		status = list.addLocalNode(2);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

		// Add an edge directly to the underlying data structure
		status = list.buildGraph.addEdge(1, 2);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

		// Test for the edges existance via the distributed layer on top
		cupcfd::error::eCodes status = list.existsEdge(2, 1, &found);

		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
		BOOST_CHECK_EQUAL(found, false);
	}
}

BOOST_AUTO_TEST_CASE(driver_existsEdge_found)
{
	cupcfd::comm::Communicator comm(MPI_COMM_WORLD);
	DistributedAdjacencyList<int, int> list(comm);
	cupcfd::error::eCodes status;

	bool found;

	if(comm.rank == 0)
	{
		status = list.addLocalNode(1);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
		status = list.addLocalNode(2);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

		// Add an edge directly to the underlying data structure
		status = list.buildGraph.addEdge(1, 2);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

		// Test for the edges existance via the distributed layer on top
		cupcfd::error::eCodes status = list.existsEdge(1, 2, &found);

		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
		BOOST_CHECK_EQUAL(found, true);
	}
}


// === addEdge ===
// Test 1:
BOOST_AUTO_TEST_CASE(driver_addEdge)
{
	cupcfd::comm::Communicator comm(MPI_COMM_WORLD);
	DistributedAdjacencyList<int, int> list(comm);
	cupcfd::error::eCodes status;

	bool found;

	if(comm.rank == 0)
	{
		status = list.addLocalNode(1);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
		status = list.addLocalNode(2);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

		status = list.addEdge(1, 2);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

		status = list.existsEdge(1, 2, &found);
		BOOST_CHECK_EQUAL(found, true);

		// Edge should only exist in one direction
		status = list.existsEdge(2, 1, &found);
		BOOST_CHECK_EQUAL(found, false);
	}
}

BOOST_AUTO_TEST_CASE(driver_addEdge_src_missing)
{
	cupcfd::comm::Communicator comm(MPI_COMM_WORLD);
	DistributedAdjacencyList<int, int> list(comm);
	cupcfd::error::eCodes status;

	// bool found;

	if(comm.rank == 0)
	{
		status = list.addLocalNode(1);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
		status = list.addLocalNode(2);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

		status = list.addEdge(3, 2);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

		bool exists;
		exists = list.existsGhostNode(3);
		BOOST_CHECK_EQUAL(exists, true);

		exists = list.existsLocalNode(3);
		BOOST_CHECK_EQUAL(exists, false);

		status = list.existsEdge(3, 2, &exists);
		BOOST_CHECK_EQUAL(exists, true);
	}
}

BOOST_AUTO_TEST_CASE(driver_addEdge_dst_missing)
{
	cupcfd::comm::Communicator comm(MPI_COMM_WORLD);
	DistributedAdjacencyList<int, int> list(comm);
	cupcfd::error::eCodes status;

	bool exists;

	if(comm.rank == 0)
	{
		status = list.addLocalNode(1);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
		status = list.addLocalNode(2);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

		status = list.addEdge(1, 4);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

		exists = list.existsGhostNode(4);
		BOOST_CHECK_EQUAL(exists, true);

		exists = list.existsLocalNode(4);
		BOOST_CHECK_EQUAL(exists, false);

		status = list.existsEdge(1, 4, &exists);
		BOOST_CHECK_EQUAL(exists, true);
	}
}

BOOST_AUTO_TEST_CASE(driver_addEdge_alreadyExists)
{
	cupcfd::comm::Communicator comm(MPI_COMM_WORLD);
	DistributedAdjacencyList<int, int> list(comm);
	cupcfd::error::eCodes status;

	bool exists;

	if(comm.rank == 0)
	{
		status = list.addLocalNode(1);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
		status = list.addLocalNode(2);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

		status = list.addEdge(1, 2);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

		status = list.existsEdge(1, 2, &exists);
		BOOST_CHECK_EQUAL(exists, true);

		// Edge should only exist in one direction
		status = list.existsEdge(2, 1, &exists);
		BOOST_CHECK_EQUAL(exists, false);

		status = list.addEdge(1, 2);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_ADJACENCY_LIST_EDGE_EXISTS);
	}
}


// === addUndirectedEdge ===
// Test 1:
BOOST_AUTO_TEST_CASE(driver_addUndirectedEdge)
{
	cupcfd::comm::Communicator comm(MPI_COMM_WORLD);
	DistributedAdjacencyList<int, int> list(comm);
	cupcfd::error::eCodes status;

	bool exists;

	if(comm.rank == 0)
	{
		status = list.addLocalNode(1);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
		status = list.addLocalNode(2);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

		status = list.addUndirectedEdge(1, 2);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

		status = list.existsEdge(1, 2, &exists);
		BOOST_CHECK_EQUAL(exists, true);

		// Edge should only exist in one direction
		status = list.existsEdge(2, 1, &exists);
		BOOST_CHECK_EQUAL(exists, true);
	}
}

BOOST_AUTO_TEST_CASE(driver_addUndirectedEdge_opposite_direction)
{
	cupcfd::comm::Communicator comm(MPI_COMM_WORLD);
	DistributedAdjacencyList<int, int> list(comm);
	cupcfd::error::eCodes status;

	bool exists;

	if(comm.rank == 0)
	{
		status = list.addLocalNode(1);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
		status = list.addLocalNode(2);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

		// Add the edge with src and dst reversed to prior test - should not change outcome.
		status = list.addUndirectedEdge(2, 1);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

		status = list.existsEdge(1, 2, &exists);
		BOOST_CHECK_EQUAL(exists, true);

		// Edge should only exist in one direction
		status = list.existsEdge(2, 1, &exists);
		BOOST_CHECK_EQUAL(exists, true);
	}
}

// === finalize ===
// Test 1:
BOOST_AUTO_TEST_CASE(driver_finalize)
{
	cupcfd::comm::Communicator comm(MPI_COMM_WORLD);
	DistributedAdjacencyList<int, int> graph(comm);
	cupcfd::error::eCodes status;

	// This function tests whether the final graph is constructed correctly
	// A set of Nodes 1->40 are distributed across 4 processes for this test

	// Add Local Nodes
	if(comm.rank == 0)
	{
		status = graph.addLocalNode(1);
		status = graph.addLocalNode(2);
		status = graph.addLocalNode(3);
		status = graph.addLocalNode(6);
		status = graph.addLocalNode(7);
		status = graph.addLocalNode(8);
		status = graph.addLocalNode(9);
		status = graph.addLocalNode(10);
		status = graph.addLocalNode(11);
		status = graph.addLocalNode(13);
		status = graph.addLocalNode(18);
		status = graph.addLocalNode(19);
		status = graph.addLocalNode(21);
		status = graph.addLocalNode(22);
		status = graph.addLocalNode(28);
		status = graph.addLocalNode(30);
		status = graph.addLocalNode(31);
		status = graph.addLocalNode(33);
		status = graph.addLocalNode(36);
		status = graph.addLocalNode(39);
	}
	else if(comm.rank == 1)
	{
		status = graph.addLocalNode(4);
		status = graph.addLocalNode(5);
		status = graph.addLocalNode(32);
		status = graph.addLocalNode(34);
		status = graph.addLocalNode(35);
		status = graph.addLocalNode(26);
		status = graph.addLocalNode(27);
	}
	else if(comm.rank == 2)
	{
		status = graph.addLocalNode(20);
		status = graph.addLocalNode(12);
		status = graph.addLocalNode(17);
		status = graph.addLocalNode(38);
		status = graph.addLocalNode(23);
		status = graph.addLocalNode(24);
		status = graph.addLocalNode(25);
	}
	else if(comm.rank == 3)
	{
		status = graph.addLocalNode(29);
		status = graph.addLocalNode(37);
		status = graph.addLocalNode(40);
		status = graph.addLocalNode(14);
		status = graph.addLocalNode(15);
		status = graph.addLocalNode(16);
	}
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	// Add Edges
	if(comm.rank == 0)
	{
		status = graph.addUndirectedEdge(1, 3);	// Local
		status = graph.addUndirectedEdge(2, 8);	// Local
		status = graph.addUndirectedEdge(1, 9);	// Local
		status = graph.addUndirectedEdge(6, 29);	// Process 0, 3
		status = graph.addUndirectedEdge(1, 16);	// Process 0, 3
		status = graph.addUndirectedEdge(28, 5);	// Process 0, 1
		status = graph.addUndirectedEdge(36, 20);	// Process 0, 2
		status = graph.addUndirectedEdge(7, 11);	// Local
		status = graph.addUndirectedEdge(22, 40);	// Process 0, 3
		status = graph.addUndirectedEdge(32, 18);	// Process 0, 1
		status = graph.addUndirectedEdge(12, 19);	// Process 0, 2
		status = graph.addUndirectedEdge(28, 23);	// Process 0, 2
		status = graph.addUndirectedEdge(21, 23);	// Process 0, 2
	}
	else if(comm.rank == 1)
	{
		status = graph.addUndirectedEdge(28, 5);	// Process 0, 1
		status = graph.addUndirectedEdge(18, 32);	// Process 0, 1
		status = graph.addUndirectedEdge(5, 27);	// Local
		status = graph.addUndirectedEdge(32, 5);	// Local
		status = graph.addUndirectedEdge(26, 27);	// Local
		status = graph.addUndirectedEdge(5, 38);	// Process 1, 2
		status = graph.addUndirectedEdge(5, 37);	// Process 1, 3
		status = graph.addUndirectedEdge(32, 15);	// Process 1, 3
	}
	else if(comm.rank == 2)
	{
		status = graph.addUndirectedEdge(36, 20);	// Process 0, 2
		status = graph.addUndirectedEdge(12, 19);	// Process 0, 2
		status = graph.addUndirectedEdge(28, 23);	// Process 0, 2
		status = graph.addUndirectedEdge(20, 23);	// Local
		status = graph.addUndirectedEdge(23, 24);	// Local
		status = graph.addUndirectedEdge(24, 25);	// Local
		status = graph.addUndirectedEdge(5, 38);	// Process 1, 2
		status = graph.addUndirectedEdge(23, 40);	// Process 2, 3
		status = graph.addUndirectedEdge(21, 23);	// Process 0, 2
		status = graph.addUndirectedEdge(14, 23);	// Process 2, 3
		status = graph.addUndirectedEdge(12, 14);	// Process 2, 3
	}
	else if(comm.rank == 3)
	{
		status = graph.addUndirectedEdge(6, 29);	// Process 0, 3
		status = graph.addUndirectedEdge(1, 16);	// Process 0, 3
		status = graph.addUndirectedEdge(22, 40);	// Process 0, 3
		status = graph.addUndirectedEdge(37, 40);  // Local
		status = graph.addUndirectedEdge(14, 16);	// Local
		status = graph.addUndirectedEdge(16, 14);	// Local
		status = graph.addUndirectedEdge(5, 37);	// Process 1, 3
		status = graph.addUndirectedEdge(32, 15);	// Process 1, 3
		status = graph.addUndirectedEdge(23, 40);	// Process 2, 3
		status = graph.addUndirectedEdge(14, 23);	// Process 2, 3
		status = graph.addUndirectedEdge(12, 14);	// Process 2, 3
	}
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	status = graph.finalize();
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	if(comm.rank == 0)
	{
		BOOST_CHECK_EQUAL(graph.nLONodes, 20);
		BOOST_CHECK_EQUAL(graph.nLGhNodes, 8);
		BOOST_CHECK_EQUAL(graph.nGNodes, 40);
		BOOST_CHECK_EQUAL(graph.nGGhNodes, 27);

		int * ghostNodes = (int *) malloc(sizeof(int) * graph.nLGhNodes);
		status = graph.getGhostNodes(ghostNodes, graph.nLGhNodes);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

		int cmp[8] = {5, 12, 16, 20, 23, 29, 32, 40};
		BOOST_CHECK_EQUAL_COLLECTIONS(ghostNodes, ghostNodes + 8, cmp, cmp + 8);
		free(ghostNodes);

		// Check Global ID Assignment is correct
		// Local Nodes
		BOOST_CHECK_EQUAL(graph.nodeToGlobal[1], 0);
		BOOST_CHECK_EQUAL(graph.nodeToGlobal[2], 1);
		BOOST_CHECK_EQUAL(graph.nodeToGlobal[3], 2);
		BOOST_CHECK_EQUAL(graph.nodeToGlobal[6], 3);
		BOOST_CHECK_EQUAL(graph.nodeToGlobal[7], 4);
		BOOST_CHECK_EQUAL(graph.nodeToGlobal[8], 5);
		BOOST_CHECK_EQUAL(graph.nodeToGlobal[9], 6);
		BOOST_CHECK_EQUAL(graph.nodeToGlobal[10], 7);
		BOOST_CHECK_EQUAL(graph.nodeToGlobal[11], 8);
		BOOST_CHECK_EQUAL(graph.nodeToGlobal[13], 9);
		BOOST_CHECK_EQUAL(graph.nodeToGlobal[18], 10);
		BOOST_CHECK_EQUAL(graph.nodeToGlobal[19], 11);
		BOOST_CHECK_EQUAL(graph.nodeToGlobal[21], 12);
		BOOST_CHECK_EQUAL(graph.nodeToGlobal[22], 13);
		BOOST_CHECK_EQUAL(graph.nodeToGlobal[28], 14);
		BOOST_CHECK_EQUAL(graph.nodeToGlobal[30], 15);
		BOOST_CHECK_EQUAL(graph.nodeToGlobal[31], 16);
		BOOST_CHECK_EQUAL(graph.nodeToGlobal[33], 17);
		BOOST_CHECK_EQUAL(graph.nodeToGlobal[36], 18);
		BOOST_CHECK_EQUAL(graph.nodeToGlobal[39], 19);

		// Ghost Nodes
		BOOST_CHECK_EQUAL(graph.nodeToGlobal[5], 21);
		BOOST_CHECK_EQUAL(graph.nodeToGlobal[12], 27);
		BOOST_CHECK_EQUAL(graph.nodeToGlobal[16], 36);
		BOOST_CHECK_EQUAL(graph.nodeToGlobal[20], 29);
		BOOST_CHECK_EQUAL(graph.nodeToGlobal[23], 30);
		BOOST_CHECK_EQUAL(graph.nodeToGlobal[29], 37);
		BOOST_CHECK_EQUAL(graph.nodeToGlobal[32], 24);
		BOOST_CHECK_EQUAL(graph.nodeToGlobal[40], 39);

		// Check Process Ownership
		BOOST_CHECK_EQUAL(graph.nodeOwner[5], 1);
		BOOST_CHECK_EQUAL(graph.nodeOwner[12], 2);
		BOOST_CHECK_EQUAL(graph.nodeOwner[16], 3);
		BOOST_CHECK_EQUAL(graph.nodeOwner[20], 2);
		BOOST_CHECK_EQUAL(graph.nodeOwner[23], 2);
		BOOST_CHECK_EQUAL(graph.nodeOwner[29], 3);
		BOOST_CHECK_EQUAL(graph.nodeOwner[32], 1);
		BOOST_CHECK_EQUAL(graph.nodeOwner[40], 3);

		// Check Build Graph and CSR Graph have the same data
		// I.e. CSR was built correctly and without changing indexes

	}
	else if(comm.rank == 1)
	{
		BOOST_CHECK_EQUAL(graph.nLONodes, 7);
		BOOST_CHECK_EQUAL(graph.nLGhNodes, 5);
		BOOST_CHECK_EQUAL(graph.nGNodes, 40);
		BOOST_CHECK_EQUAL(graph.nGGhNodes, 27);

		int * ghostNodes = (int *) malloc(sizeof(int) * graph.nLGhNodes);
		status = graph.getGhostNodes(ghostNodes, graph.nLGhNodes);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

		int cmp[5] = {15, 18, 28, 37, 38};
		BOOST_CHECK_EQUAL_COLLECTIONS(ghostNodes, ghostNodes + 5, cmp, cmp + 5);
		free(ghostNodes);

		// Check Global ID Assignment is correct
		// Local Nodes (note they have been assigned ids in sorted order)
		BOOST_CHECK_EQUAL(graph.nodeToGlobal[4], 20);
		BOOST_CHECK_EQUAL(graph.nodeToGlobal[5], 21);
		BOOST_CHECK_EQUAL(graph.nodeToGlobal[32], 24);
		BOOST_CHECK_EQUAL(graph.nodeToGlobal[34], 25);
		BOOST_CHECK_EQUAL(graph.nodeToGlobal[35], 26);
		BOOST_CHECK_EQUAL(graph.nodeToGlobal[26], 22);
		BOOST_CHECK_EQUAL(graph.nodeToGlobal[27], 23);

		// Check Process Ownership
		BOOST_CHECK_EQUAL(graph.nodeOwner[15], 3);
		BOOST_CHECK_EQUAL(graph.nodeOwner[18], 0);
		BOOST_CHECK_EQUAL(graph.nodeOwner[28], 0);
		BOOST_CHECK_EQUAL(graph.nodeOwner[37], 3);
		BOOST_CHECK_EQUAL(graph.nodeOwner[38], 2);

		// Check Build Graph and CSR Graph have the same data
		// I.e. CSR was built correctly and without changing indexes
	}
	else if(comm.rank == 2)
	{
		BOOST_CHECK_EQUAL(graph.nLONodes, 7);
		BOOST_CHECK_EQUAL(graph.nLGhNodes, 7);
		BOOST_CHECK_EQUAL(graph.nGNodes, 40);
		BOOST_CHECK_EQUAL(graph.nGGhNodes, 27);

		int * ghostNodes = (int *) malloc(sizeof(int) * graph.nLGhNodes);
		status = graph.getGhostNodes(ghostNodes, graph.nLGhNodes);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

		int cmp[7] = {5, 14, 19, 21, 28, 36, 40};
		BOOST_CHECK_EQUAL_COLLECTIONS(ghostNodes, ghostNodes + 7, cmp, cmp + 7);
		free(ghostNodes);

		// Check Global ID Assignment is correct
		// Local Nodes (note they have been assigned ids in sorted order)
		BOOST_CHECK_EQUAL(graph.nodeToGlobal[12], 27);
		BOOST_CHECK_EQUAL(graph.nodeToGlobal[17], 28);
		BOOST_CHECK_EQUAL(graph.nodeToGlobal[20], 29);
		BOOST_CHECK_EQUAL(graph.nodeToGlobal[23], 30);
		BOOST_CHECK_EQUAL(graph.nodeToGlobal[24], 31);
		BOOST_CHECK_EQUAL(graph.nodeToGlobal[25], 32);
		BOOST_CHECK_EQUAL(graph.nodeToGlobal[38], 33);

		// Check Process Ownership
		BOOST_CHECK_EQUAL(graph.nodeOwner[5], 1);
		BOOST_CHECK_EQUAL(graph.nodeOwner[14], 3);
		BOOST_CHECK_EQUAL(graph.nodeOwner[19], 0);
		BOOST_CHECK_EQUAL(graph.nodeOwner[21], 0);
		BOOST_CHECK_EQUAL(graph.nodeOwner[28], 0);
		BOOST_CHECK_EQUAL(graph.nodeOwner[36], 0);
		BOOST_CHECK_EQUAL(graph.nodeOwner[40], 3);

		// Check Build Graph and CSR Graph have the same data
		// I.e. CSR was built correctly and without changing indexes
	}
	else if(comm.rank == 3)
	{
		BOOST_CHECK_EQUAL(graph.nLONodes, 6);
		BOOST_CHECK_EQUAL(graph.nLGhNodes, 7);
		BOOST_CHECK_EQUAL(graph.nGNodes, 40);
		BOOST_CHECK_EQUAL(graph.nGGhNodes, 27);

		int * ghostNodes = (int *) malloc(sizeof(int) * graph.nLGhNodes);
		status = graph.getGhostNodes(ghostNodes, graph.nLGhNodes);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

		int cmp[7] = {1, 5, 6, 12, 22, 23, 32};
		BOOST_CHECK_EQUAL_COLLECTIONS(ghostNodes, ghostNodes + 7, cmp, cmp + 7);
		free(ghostNodes);

		// Check Global ID Assignment is correct
		// Local Nodes (note they have been assigned ids in sorted order)
		BOOST_CHECK_EQUAL(graph.nodeToGlobal[29], 37);
		BOOST_CHECK_EQUAL(graph.nodeToGlobal[37], 38);
		BOOST_CHECK_EQUAL(graph.nodeToGlobal[40], 39);
		BOOST_CHECK_EQUAL(graph.nodeToGlobal[14], 34);
		BOOST_CHECK_EQUAL(graph.nodeToGlobal[15], 35);
		BOOST_CHECK_EQUAL(graph.nodeToGlobal[16], 36);

		BOOST_CHECK_EQUAL(graph.globalToNode[37], 29);
		BOOST_CHECK_EQUAL(graph.globalToNode[38], 37);
		BOOST_CHECK_EQUAL(graph.globalToNode[39], 40);
		BOOST_CHECK_EQUAL(graph.globalToNode[34], 14);
		BOOST_CHECK_EQUAL(graph.globalToNode[35], 15);
		BOOST_CHECK_EQUAL(graph.globalToNode[36], 16);

		// Check Process Ownership
		BOOST_CHECK_EQUAL(graph.nodeOwner[1], 0);
		BOOST_CHECK_EQUAL(graph.nodeOwner[5], 1);
		BOOST_CHECK_EQUAL(graph.nodeOwner[6], 0);
		BOOST_CHECK_EQUAL(graph.nodeOwner[12], 2);
		BOOST_CHECK_EQUAL(graph.nodeOwner[22], 0);
		BOOST_CHECK_EQUAL(graph.nodeOwner[23], 2);
		BOOST_CHECK_EQUAL(graph.nodeOwner[32], 1);

		// Check Build Graph and CSR Graph have the same data
		// I.e. CSR was built correctly and without changing indexes
	}

}

// === sortNodesByLocal ===
// ToDo: Add Tests (although indirectly tested in finalize)

// === getGhostNodes ===
// ToDo: Add Tests (although indirectly tested in finalize)

// === getLocalNodes ===
// ToDo: Add Tests (although indirectly tested in finalize)

// === buildSerialAdjacencyList ===
BOOST_AUTO_TEST_CASE(buildSerialAdjacencyList_test1)
{
	cupcfd::comm::Communicator comm(MPI_COMM_WORLD);
	DistributedAdjacencyList<int, int> graph(comm);
	cupcfd::error::eCodes status;

	// This function tests whether the final graph is constructed correctly
	// A set of Nodes 1->40 are distributed across 4 processes for this test

	// Add Local Nodes
	if(comm.rank == 0)
	{
		status = graph.addLocalNode(1);
		status = graph.addLocalNode(2);
		status = graph.addLocalNode(3);
		status = graph.addLocalNode(6);
		status = graph.addLocalNode(7);
		status = graph.addLocalNode(8);
		status = graph.addLocalNode(9);
		status = graph.addLocalNode(10);
		status = graph.addLocalNode(11);
		status = graph.addLocalNode(13);
		status = graph.addLocalNode(18);
		status = graph.addLocalNode(19);
		status = graph.addLocalNode(21);
		status = graph.addLocalNode(22);
		status = graph.addLocalNode(28);
		status = graph.addLocalNode(30);
		status = graph.addLocalNode(31);
		status = graph.addLocalNode(33);
		status = graph.addLocalNode(36);
		status = graph.addLocalNode(39);
	}
	else if(comm.rank == 1)
	{
		status = graph.addLocalNode(4);
		status = graph.addLocalNode(5);
		status = graph.addLocalNode(32);
		status = graph.addLocalNode(34);
		status = graph.addLocalNode(35);
		status = graph.addLocalNode(26);
		status = graph.addLocalNode(27);
	}
	else if(comm.rank == 2)
	{
		status = graph.addLocalNode(20);
		status = graph.addLocalNode(12);
		status = graph.addLocalNode(17);
		status = graph.addLocalNode(38);
		status = graph.addLocalNode(23);
		status = graph.addLocalNode(24);
		status = graph.addLocalNode(25);
	}
	else if(comm.rank == 3)
	{
		status = graph.addLocalNode(29);
		status = graph.addLocalNode(37);
		status = graph.addLocalNode(40);
		status = graph.addLocalNode(14);
		status = graph.addLocalNode(15);
		status = graph.addLocalNode(16);
	}
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	// Add Edges
	if(comm.rank == 0)
	{
		status = graph.addUndirectedEdge(1, 3);	// Local
		status = graph.addUndirectedEdge(2, 8);	// Local
		status = graph.addUndirectedEdge(1, 9);	// Local
		status = graph.addUndirectedEdge(6, 29);	// Process 0, 3
		status = graph.addUndirectedEdge(1, 16);	// Process 0, 3
		status = graph.addUndirectedEdge(28, 5);	// Process 0, 1
		status = graph.addUndirectedEdge(36, 20);	// Process 0, 2
		status = graph.addUndirectedEdge(7, 11);	// Local
		status = graph.addUndirectedEdge(22, 40);	// Process 0, 3
		status = graph.addUndirectedEdge(32, 18);	// Process 0, 1
		status = graph.addUndirectedEdge(12, 19);	// Process 0, 2
		status = graph.addUndirectedEdge(28, 23);	// Process 0, 2
		status = graph.addUndirectedEdge(21, 23);	// Process 0, 2
	}
	else if(comm.rank == 1)
	{
		status = graph.addUndirectedEdge(28, 5);	// Process 0, 1
		status = graph.addUndirectedEdge(18, 32);	// Process 0, 1
		status = graph.addUndirectedEdge(5, 27);	// Local
		status = graph.addUndirectedEdge(32, 5);	// Local
		status = graph.addUndirectedEdge(26, 27);	// Local
		status = graph.addUndirectedEdge(5, 38);	// Process 1, 2
		status = graph.addUndirectedEdge(5, 37);	// Process 1, 3
		status = graph.addUndirectedEdge(32, 15);	// Process 1, 3
	}
	else if(comm.rank == 2)
	{
		status = graph.addUndirectedEdge(36, 20);	// Process 0, 2
		status = graph.addUndirectedEdge(12, 19);	// Process 0, 2
		status = graph.addUndirectedEdge(28, 23);	// Process 0, 2
		status = graph.addUndirectedEdge(20, 23);	// Local
		status = graph.addUndirectedEdge(23, 24);	// Local
		status = graph.addUndirectedEdge(24, 25);	// Local
		status = graph.addUndirectedEdge(5, 38);	// Process 1, 2
		status = graph.addUndirectedEdge(23, 40);	// Process 2, 3
		status = graph.addUndirectedEdge(21, 23);	// Process 0, 2
		status = graph.addUndirectedEdge(14, 23);	// Process 2, 3
		status = graph.addUndirectedEdge(12, 14);	// Process 2, 3
	}
	else if(comm.rank == 3)
	{
		status = graph.addUndirectedEdge(6, 29);	// Process 0, 3
		status = graph.addUndirectedEdge(1, 16);	// Process 0, 3
		status = graph.addUndirectedEdge(22, 40);	// Process 0, 3
		status = graph.addUndirectedEdge(37, 40);  // Local
		status = graph.addUndirectedEdge(14, 16);	// Local
		status = graph.addUndirectedEdge(16, 14);	// Local
		status = graph.addUndirectedEdge(5, 37);	// Process 1, 3
		status = graph.addUndirectedEdge(32, 15);	// Process 1, 3
		status = graph.addUndirectedEdge(23, 40);	// Process 2, 3
		status = graph.addUndirectedEdge(14, 23);	// Process 2, 3
		status = graph.addUndirectedEdge(12, 14);	// Process 2, 3
	}
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	status = graph.finalize();
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	AdjacencyListCSR<int, int> * serialGraph = nullptr;

	if(comm.rank == 1)
	{
		serialGraph = new AdjacencyListCSR<int, int>();
	}

	status = graph.buildSerialAdjacencyList(serialGraph, 1);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	if(graph.comm->rank == 1)
	{
		int nNodes;
		int nEdges;

		nNodes = serialGraph->getNodeCount();
		nEdges = serialGraph->getEdgeCount();

		BOOST_CHECK_EQUAL(nNodes, 40);
		BOOST_CHECK_EQUAL(nEdges, 54);

		// Check Values are correct in serial graph
		int nodeCmp[40] = {1, 2, 3, 4, 5, 6, 7, 8, 9,
						   10, 11, 12, 13, 14, 15, 16 ,17, 18, 19,
						   20, 21, 22, 23, 24, 25, 26, 27, 28, 29,
						   30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40};

		int nodes[40];
		status = serialGraph->getNodes(nodes, 40);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
		status = cupcfd::utility::drivers::merge_sort(nodes, 40);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

		BOOST_CHECK_EQUAL_COLLECTIONS(nodes, nodes + 40, nodeCmp, nodeCmp + 40);

		int nodeEdge1Cmp[54] = {1, 1, 1,  2, 3, 5,  5,  5,  5,  5, 6,  7,  8, 9, 11, 12, 12, 14, 14, 14, 15, 16, 16, 18,
							 19, 20, 20, 21, 22, 23, 23, 23, 23, 23, 23, 24, 24, 25, 26, 27, 27, 28, 28,
							 29, 32, 32, 32, 36, 37, 37, 38, 40, 40, 40};

		int nodeEdge2Cmp[54] = {3, 9, 16, 8, 1, 27, 28, 32, 37, 38, 29, 11, 2, 1, 7, 14, 19, 12, 16, 23, 32, 1,  14, 32,
							 12, 23, 36, 23, 40, 14, 20, 21, 24, 28, 40, 23, 25, 24, 27, 5,  26, 5,  23,
							 6,  5,  15, 18, 20, 5,  40, 5,  22, 23, 37};

		int nodeEdge1[54];
		int nodeEdge2[54];
		int groupCount[40] = {3, 1, 1, 0, 5, 1, 1, 1, 1, 1, 2, 3, 1, 2, 1, 1, 2,
							  1, 1, 6, 2, 1, 1, 2, 2, 1, 0, 0, 3, 0, 0, 0, 1, 2, 1, 0, 3};

		status = serialGraph->getEdges(nodeEdge1, 54, nodeEdge2, 54);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

		int indexes[54];

		// Reorder the edges so we can verify in a consistent order
		status = cupcfd::utility::drivers::merge_sort_index(nodeEdge1, 54, indexes, 54);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
		status = cupcfd::utility::drivers::merge_sort(nodeEdge1, 54);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
		status = cupcfd::utility::drivers::sourceIndexReorder(nodeEdge2, 54, indexes, 54);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

		// This gets them grouped, but not neccessarily in the same order (since merge sort may not have reordered
		// edges if node1 was the same for multiple.
		// Sort in group
		int ptr = 0;
		for(int i = 0; i < 40; i++)
		{
			status = cupcfd::utility::drivers::merge_sort(nodeEdge2 + ptr, groupCount[i]);
			BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
			ptr = ptr + groupCount[i];
		}

		BOOST_CHECK_EQUAL_COLLECTIONS(nodeEdge1, nodeEdge1 + 54, nodeEdge1Cmp, nodeEdge1Cmp + 54);
		BOOST_CHECK_EQUAL_COLLECTIONS(nodeEdge2, nodeEdge2 + 54, nodeEdge2Cmp, nodeEdge2Cmp + 54);
	}
}


// Finalize MPI
BOOST_AUTO_TEST_CASE(cleanup)
{
    // Cleanup MPI Environment
    MPI_Finalize();
}
