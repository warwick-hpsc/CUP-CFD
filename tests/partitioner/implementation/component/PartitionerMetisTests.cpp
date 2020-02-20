/*
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Unit Tests for the PartitionerMetis class
 */

#define BOOST_TEST_MODULE Partitioner
#include <boost/test/unit_test.hpp>
#include <boost/test/output_test_stream.hpp>
#include <stdexcept>

#include "PartitionerMetis.h"
#include "Error.h"

// ========================================
// ============== Tests ===================
// ========================================

using namespace cupcfd::partitioner;

// ======= Setup ========
// Some tests require MPI
BOOST_AUTO_TEST_CASE(setup)
{
    int argc = boost::unit_test::framework::master_test_suite().argc;
    char ** argv = boost::unit_test::framework::master_test_suite().argv;
    MPI_Init(&argc, &argv);
}

// === Constructors ===
BOOST_AUTO_TEST_CASE(constructor_test1)
{

}


// === setWorkArrays ===
BOOST_AUTO_TEST_CASE(setWorkArrays_test1)
{

}

// === setNCon ===
BOOST_AUTO_TEST_CASE(setNCon_test1)
{

}

// === initialise ===
BOOST_AUTO_TEST_CASE(initialise_test1)
{

}

// === partition ===
// Test 1: Test a successful partition
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
			graph.addLocalNode(localNodes[i]);
		}
	}
	else if(comm.rank == 1)
	{
		int localNodes[7] = {4, 5, 32, 34, 35, 26, 27};
		int nLocalNodes = 7;

		for(int i = 0; i < nLocalNodes; i ++)
		{
			graph.addLocalNode(localNodes[i]);
		}
	}
	else if(comm.rank == 2)
	{
		int localNodes[7] = {20, 12, 17, 38, 23, 24, 25};
		int nLocalNodes = 7;

		for(int i = 0; i < nLocalNodes; i ++)
		{
			graph.addLocalNode(localNodes[i]);
		}
	}
	else if(comm.rank == 3)
	{
		int localNodes[6] = {29, 37, 40, 14, 15, 16};
		int nLocalNodes = 6;

		for(int i = 0; i < nLocalNodes; i ++)
		{
			graph.addLocalNode(localNodes[i]);
		}
	}

	int nEdges;

	// Add Edges
	if(comm.rank == 0)
	{
		int edgeNode1[13] = {1, 2, 1, 6, 1, 28, 36, 7, 22, 32, 12, 28, 21};
		int edgeNode2[13] = {3, 8, 9, 29, 16, 5, 20, 11, 40, 18, 19, 23, 23};
		int nEdges = 13;

		for(int i = 0; i < nEdges; i++)
		{
			graph.addUndirectedEdge(edgeNode1[i], edgeNode2[i]);
		}
	}
	else if(comm.rank == 1)
	{
		int edgeNode1[8] = {28, 18, 5, 32, 26, 5, 5, 32};
		int edgeNode2[8] = {5, 32, 27, 5, 27, 38, 37, 15};
		int nEdges = 8;

		for(int i = 0; i < nEdges; i++)
		{
			graph.addUndirectedEdge(edgeNode1[i], edgeNode2[i]);
		}
	}
	else if(comm.rank == 2)
	{
		int edgeNode1[11] = {36, 12, 28, 20, 23, 24, 5, 23, 21, 14, 12};
		int edgeNode2[11] = {20, 19, 23, 23, 24, 25, 38, 40, 23, 23, 14};
		int nEdges = 11;

		for(int i = 0; i < nEdges; i++)
		{
			graph.addUndirectedEdge(edgeNode1[i], edgeNode2[i]);
		}
	}
	else if(comm.rank == 3)
	{
		int edgeNode1[11] = {6, 1, 22, 37, 14, 16, 5, 32, 23, 14, 12};
		int edgeNode2[11] = {29, 16, 40, 40, 16, 14, 37, 15, 40, 23, 14};
		int nEdges = 11;

		for(int i = 0; i < nEdges; i++)
		{
			graph.addUndirectedEdge(edgeNode1[i], edgeNode2[i]);
		}
	}

	// Finalize before use
	graph.finalize();

	PartitionerMetis<int, int> * partitioner = new PartitionerMetis<int, int>(graph, 4, 1);
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

// === assignRankNodes ===
BOOST_AUTO_TEST_CASE(assignRankNodes_test1)
{

}

// ======= Cleanup ========
// Finalize MPI
BOOST_AUTO_TEST_CASE(cleanup)
{
    // Cleanup MPI Environment
    MPI_Finalize();
}
