/*
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Unit Tests for the PartitionerNaive class
 */

#define BOOST_TEST_MODULE PartitionerNaive
#include <boost/test/unit_test.hpp>
#include <boost/test/output_test_stream.hpp>
#include <stdexcept>

#include "PartitionerNaive.h"
#include "Error.h"

#include "DistributedAdjacencyList.h"

#include <iostream>

// ========================================
// ============== Tests ===================
// ========================================

// ======= Setup ========
// Some tests require MPI
BOOST_AUTO_TEST_CASE(setup)
{
    int argc = boost::unit_test::framework::master_test_suite().argc;
    char ** argv = boost::unit_test::framework::master_test_suite().argv;
    MPI_Init(&argc, &argv);
}

using namespace cupcfd::partitioner;

// === Constructors ===
// Test 1:
BOOST_AUTO_TEST_CASE(constructor_test1)
{

}

// Test 2: Test that the Partitioner can be given a graph and nParts, and set up the internal
// data structures correctly
BOOST_AUTO_TEST_CASE(constructor_test2)
{

}

// === initialize ===
BOOST_AUTO_TEST_CASE(initialize_test1)
{

}

// === partition ===
// Test 1: Test that a correctly setup PartitionerNaive can partition the data without causing an error
BOOST_AUTO_TEST_CASE(partition_test1)
{
	cupcfd::error::eCodes status;
	cupcfd::comm::Communicator comm(MPI_COMM_WORLD);
	cupcfd::data_structures::DistributedAdjacencyList<int, int> graph(comm);

	// Build a test distributed graph to serve as input
	// Add Local Nodes
	if(comm.rank == 0)
	{
		int localNodes[20] = {1, 2, 3, 6, 7, 8, 9, 10, 11, 13, 18, 19,
							  21, 22, 28, 30, 31, 33, 36, 39};
		int nLocalNodes = 20;

		for(int i = 0; i < nLocalNodes; i ++)
		{
			status = graph.addLocalNode(localNodes[i]);
			BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
		}
	}
	else if(comm.rank == 1)
	{
		int localNodes[7] = {4, 5, 32, 34, 35, 26, 27};
		int nLocalNodes = 7;

		for(int i = 0; i < nLocalNodes; i ++)
		{
			status = graph.addLocalNode(localNodes[i]);
			BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
		}
	}
	else if(comm.rank == 2)
	{
		int localNodes[7] = {20, 12, 17, 38, 23, 24, 25};
		int nLocalNodes = 7;

		for(int i = 0; i < nLocalNodes; i ++)
		{
			status = graph.addLocalNode(localNodes[i]);
			BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
		}
	}
	else if(comm.rank == 3)
	{
		int localNodes[6] = {29, 37, 40, 14, 15, 16};
		int nLocalNodes = 6;

		for(int i = 0; i < nLocalNodes; i ++)
		{
			status = graph.addLocalNode(localNodes[i]);
			BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
		}
	}

	int nEdges;

	// Add Edges
	if(comm.rank == 0)
	{
		int edgeNode1[13] = {1, 2, 1, 6, 1, 28, 36, 7, 22, 32, 12, 28, 21};
		int edgeNode2[13] = {3, 8, 9, 29, 16, 5, 20, 11, 40, 18, 19, 23, 23};
		nEdges = 13;

		for(int i = 0; i < nEdges; i++)
		{
			status = graph.addUndirectedEdge(edgeNode1[i], edgeNode2[i]);
			BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
		}
	}
	else if(comm.rank == 1)
	{
		int edgeNode1[8] = {28, 18, 5, 32, 26, 5, 5, 32};
		int edgeNode2[8] = {5, 32, 27, 5, 27, 38, 37, 15};
		nEdges = 8;

		for(int i = 0; i < nEdges; i++)
		{
			status = graph.addUndirectedEdge(edgeNode1[i], edgeNode2[i]);
			BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
		}
	}
	else if(comm.rank == 2)
	{
		int edgeNode1[11] = {36, 12, 28, 20, 23, 24, 5, 23, 21, 14, 12};
		int edgeNode2[11] = {20, 19, 23, 23, 24, 25, 38, 40, 23, 23, 14};
		nEdges = 11;

		for(int i = 0; i < nEdges; i++)
		{
			status = graph.addUndirectedEdge(edgeNode1[i], edgeNode2[i]);
			BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
		}
	}
	else if(comm.rank == 3)
	{
		int edgeNode1[11] = {6, 1, 22, 37, 14, 16, 5, 32, 23, 14, 12};
		int edgeNode2[11] = {29, 16, 40, 40, 16, 14, 37, 15, 40, 23, 14};
		nEdges = 11;

		for(int i = 0; i < nEdges; i++)
		{
			status = graph.addUndirectedEdge(edgeNode1[i], edgeNode2[i]);
			BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
		}
	}

	// Finalize before use
	status = graph.finalize();
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	PartitionerNaive<int, int> * partitioner = new PartitionerNaive<int,int>(graph, 4);
	status = partitioner->partition();
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	int * results;
	int nResults;

	status = partitioner->assignRankNodes(&results, &nResults);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	//for(int i = 0; i < nResults; i++)
	//{
	//	std::cout << "Rank,i = (" << comm.rank << "," << results[i] << ")\n";
	//}

	delete(partitioner);
}

// Test 2: Test that a correctly setup PartitionerNaive can partition the data
// appropriately from a structured mesh source.
// This version has a remainder where it is not quite evenly distributed (1 extra cell on
// rank 0 and 1)
BOOST_AUTO_TEST_CASE(partition_test2)
{
	cupcfd::error::eCodes status;
	cupcfd::comm::Communicator comm(MPI_COMM_WORLD);
	cupcfd::data_structures::DistributedAdjacencyList<int, int> graph(comm);

	// Build a test distributed graph to serve as input
	// Add Local Nodes
	if(comm.rank == 0)
	{
		int localNodes[20] = {1, 2, 3, 6, 7, 8, 9, 10, 11, 13, 18, 19,
							  21, 22, 28, 30, 31, 33, 36, 39};
		int nLocalNodes = 20;

		for(int i = 0; i < nLocalNodes; i ++)
		{
			status = graph.addLocalNode(localNodes[i]);
			BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
		}
	}
	else if(comm.rank == 1)
	{
		int localNodes[7] = {4, 5, 26, 27, 32, 34, 35};
		int nLocalNodes = 7;

		for(int i = 0; i < nLocalNodes; i ++)
		{
			status = graph.addLocalNode(localNodes[i]);
			BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
		}
	}
	else if(comm.rank == 2)
	{
		int localNodes[7] = {12, 17, 20, 23, 24, 25, 38};
		int nLocalNodes = 7;

		for(int i = 0; i < nLocalNodes; i ++)
		{
			status = graph.addLocalNode(localNodes[i]);
			BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
		}
	}
	else if(comm.rank == 3)
	{
		int localNodes[8] = {14, 15, 16, 29, 37, 40, 101, 201};
		int nLocalNodes = 8;

		for(int i = 0; i < nLocalNodes; i ++)
		{
			status = graph.addLocalNode(localNodes[i]);
			BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
		}
	}

	// Edges don't matter for naive partitioner

	// Finalize before use
	status = graph.finalize();
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	PartitionerNaive<int, int> * partitioner = new PartitionerNaive<int,int>(graph, 4);

	status = partitioner->partition();
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	int * results;
	int nResults;

	status = partitioner->assignRankNodes(&results, &nResults);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	// Check Assignments are correct:

	if(comm.rank == 0)
	{
		int assignedNodeCmp[11] = {1, 2, 3, 6, 7, 8, 9, 10, 11, 13, 18};
		BOOST_CHECK_EQUAL(nResults, 11);
		BOOST_CHECK_EQUAL_COLLECTIONS(assignedNodeCmp, assignedNodeCmp + 11, results, results + 11);
	}
	else if(comm.rank == 1)
	{
		int assignedNodeCmp[11] = {19, 21, 22, 28, 30, 31, 33, 36, 39, 4, 5};
		BOOST_CHECK_EQUAL(nResults, 11);
		BOOST_CHECK_EQUAL_COLLECTIONS(assignedNodeCmp, assignedNodeCmp + 11, results, results + 11);
	}
	else if(comm.rank == 2)
	{
		int assignedNodeCmp[10] = {26, 27, 32, 34, 35, 12, 17, 20, 23, 24};
		BOOST_CHECK_EQUAL(nResults, 10);
		BOOST_CHECK_EQUAL_COLLECTIONS(assignedNodeCmp, assignedNodeCmp + 10, results, results + 10);
	}
	else if(comm.rank == 3)
	{
		int assignedNodeCmp[10] = {25, 38, 14, 15, 16, 29, 37, 40, 101, 201};
		BOOST_CHECK_EQUAL(nResults, 10);
		BOOST_CHECK_EQUAL_COLLECTIONS(assignedNodeCmp, assignedNodeCmp + 10, results, results + 10);
	}

	free(results);
	delete(partitioner);
}

// === reset ===
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
