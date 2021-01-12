/*
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Tests for the AdjacencyListVector class
 */

#define BOOST_TEST_MODULE AdjacencyListVector
#include <boost/test/unit_test.hpp>
#include <boost/test/output_test_stream.hpp>
#include <stdexcept>

#include "AdjacencyListVector.h"
#include "AdjacencyListCSR.h"
#include "Error.h"

using namespace cupcfd::data_structures;

// === Constructors/Deconstructors ===
// Test 1: Empty Constructor
BOOST_AUTO_TEST_CASE(constructor_test1)
{
	AdjacencyListVector<int, int> graph;
}

// === Test 2: Copy Constructor ===
BOOST_AUTO_TEST_CASE(constructor_test2)
{

}

// === getNodeLocalIndex ===
// Test 1: Get the correct local IDs from node IDs/labels
BOOST_AUTO_TEST_CASE(getNodeLocalIndex_test1)
{
	AdjacencyListVector<int, int> graph;
	cupcfd::error::eCodes status;
	int idx;

	status = graph.addNode(20);
	status = graph.addNode(40);
	status = graph.addNode(60);
	status = graph.addNode(80);

	status = graph.getNodeLocalIndex(20, &idx);
	BOOST_CHECK_EQUAL(idx, 0);

	status = graph.getNodeLocalIndex(40, &idx);
	BOOST_CHECK_EQUAL(idx, 1);

	status = graph.getNodeLocalIndex(60, &idx);
	BOOST_CHECK_EQUAL(idx, 2);

	status = graph.getNodeLocalIndex(80, &idx);
	BOOST_CHECK_EQUAL(idx, 3);
}

// Test 2: Error Code Searching on a node that does not exist
BOOST_AUTO_TEST_CASE(getNodeLocalIndex_test2)
{
	AdjacencyListVector<int, int> graph;
	cupcfd::error::eCodes status;
	int idx;

	status = graph.addNode(20);

	status = graph.getNodeLocalIndex(40, &idx);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_ADJACENCY_LIST_NODE_MISSING);
}

// === getLocalIndexNode ===
// Test 1:


// === getNodeCount ===
// Test 1: Get correct node count
BOOST_AUTO_TEST_CASE(getNodeCount_test1)
{
	AdjacencyListVector<int, int> graph;
	cupcfd::error::eCodes status;
	// int idx;
	// bool exists;

	status = graph.addNode(20);
	status = graph.addNode(40);
	status = graph.addEdge(20, 40);
	status = graph.addEdge(20, 40);

	int count = graph.getNodeCount();
	BOOST_CHECK_EQUAL(count, 2);
}

// === getNodes ===
// Test 1: Retrieve all nodes
BOOST_AUTO_TEST_CASE(getNodes_test1)
{
	AdjacencyListVector<int, int> graph;
	cupcfd::error::eCodes status;
	// int idx;
	// bool exists;

	status = graph.addNode(20);
	status = graph.addNode(40);
	status = graph.addNode(80);
	status = graph.addNode(30);

	int nNodes = 4;
	int nodes[4];
	int cmpNodes[4] = {20, 40, 80, 30};

	status = graph.getNodes(nodes, nNodes);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
    BOOST_CHECK_EQUAL_COLLECTIONS(nodes , nodes + 4, cmpNodes, cmpNodes + 4);
}

// === getEdgeCount ===
// Test 1: Get the edge count of an empty graph
BOOST_AUTO_TEST_CASE(getEdgeCount_test1)
{
	AdjacencyListVector<int, int> graph;

	int count = graph.getEdgeCount();
	BOOST_CHECK_EQUAL(count, 0);
}

// Test 2: Get the correct edge count of a single directed edge
BOOST_AUTO_TEST_CASE(getEdgeCount_test2)
{
	AdjacencyListVector<int, int> graph;
	cupcfd::error::eCodes status;
	int count;

	status = graph.addNode(20);
	status = graph.addNode(40);
	status = graph.addEdge(20, 40);

	count = graph.getEdgeCount();
	BOOST_CHECK_EQUAL(count, 1);
}

// Test 3: Get the correct edge count of an edge in both directions
BOOST_AUTO_TEST_CASE(getEdgeCount_test3)
{
	AdjacencyListVector<int, int> graph;
	cupcfd::error::eCodes status;
	int count;

	status = graph.addNode(20);
	status = graph.addNode(40);
	status = graph.addEdge(20, 40);
	status = graph.addEdge(40, 20);

	count = graph.getEdgeCount();
	BOOST_CHECK_EQUAL(count, 2);
}

// Test 4: Get the correct edge count with multiple edges
BOOST_AUTO_TEST_CASE(getEdgeCount_test4)
{
	AdjacencyListVector<int, int> graph;
	cupcfd::error::eCodes status;
	int count;

	status = graph.addNode(20);
	status = graph.addNode(40);
	status = graph.addNode(60);
	status = graph.addNode(80);
	status = graph.addEdge(20, 40);
	status = graph.addEdge(40, 20);
	status = graph.addEdge(40, 80);
	status = graph.addEdge(80, 20);
	status = graph.addEdge(20, 60);

	count = graph.getEdgeCount();
	BOOST_CHECK_EQUAL(count, 5);
}


// Test 5: Check adding duplicate edges doesn't increase the returned count
BOOST_AUTO_TEST_CASE(getEdgeCount_test5)
{
	AdjacencyListVector<int, int> graph;
	cupcfd::error::eCodes status;
	int count;

	status = graph.addNode(20);
	status = graph.addNode(40);
	status = graph.addNode(60);
	status = graph.addNode(80);
	status = graph.addEdge(20, 40);
	status = graph.addEdge(40, 20);
	status = graph.addEdge(40, 80);
	status = graph.addEdge(80, 20);
	status = graph.addEdge(20, 60);
	status = graph.addEdge(20, 60);
	status = graph.addEdge(80, 20);

	count = graph.getEdgeCount();
	BOOST_CHECK_EQUAL(count, 5);
}

// === getEdges ===
// Test 1:
BOOST_AUTO_TEST_CASE(getEdges_test1)
{
	AdjacencyListVector<int, int> graph;
	cupcfd::error::eCodes status;

	// Setup
	int nodes[5] = {20, 40, 60, 80, 100};

	// Will  keep these sorted since for now getEdges will return in sorted order due
	// to use of map
	int edges1[6] = {20, 20, 20, 20, 60, 80};
	int edges2[6] = {40, 60, 80, 100, 80, 100};

	for(int i = 0; i < 5; i++)
	{
		status = graph.addNode(nodes[i]);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	}

	for(int i = 0; i < 6; i++)
	{
		status = graph.addEdge(edges1[i], edges2[i]);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	}

	// Test and Check
	int rEdges1[6];
	int rEdges2[6];
	status = graph.getEdges(rEdges1, 6, rEdges2, 6);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	BOOST_CHECK_EQUAL_COLLECTIONS(edges1, edges1 + 6, rEdges1, rEdges1 + 6);
	BOOST_CHECK_EQUAL_COLLECTIONS(edges2, edges2 + 6, rEdges2, rEdges2 + 6);
}

// === addNode ===
// Test 1: Test add previously non-existant node
BOOST_AUTO_TEST_CASE(addNode_test1)
{
	AdjacencyListVector<int, int> graph;
	cupcfd::error::eCodes status;

	status = graph.addNode(20);

	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
}

// Test 2: Error Code when node already exists
BOOST_AUTO_TEST_CASE(addNode_test2)
{
	AdjacencyListVector<int, int> graph;
	cupcfd::error::eCodes status;

	status = graph.addNode(20);
	status = graph.addNode(20);

	BOOST_CHECK_EQUAL(status, cupcfd::error::E_ADJACENCY_LIST_NODE_EXISTS);
}

// === existsNode ===
// Test 1: Identify node exists
BOOST_AUTO_TEST_CASE(existsNode_test1)
{
	AdjacencyListVector<int, int> graph;
	cupcfd::error::eCodes status;
	bool exists;

	status = graph.addNode(20);
	exists = graph.existsNode(20);
	BOOST_CHECK_EQUAL(exists, true);

}

// Test 2: Identify that node does not exist
BOOST_AUTO_TEST_CASE(existsNode_test2)
{
	AdjacencyListVector<int, int> graph;
	bool exists;

	exists = graph.existsNode(20);
	BOOST_CHECK_EQUAL(exists, false);
}

// === addEdge ===
// Test 1: Test an edge can be added between two existing nodes without issue
BOOST_AUTO_TEST_CASE(addEdge_test1)
{
	AdjacencyListVector<int, int> graph;
	cupcfd::error::eCodes status;
	// int idx;
	// bool exists;

	status = graph.addNode(20);
	status = graph.addNode(40);
	status = graph.addEdge(20, 40);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
}

// Test 2: Test an existing edge is detected when adding duplicates
BOOST_AUTO_TEST_CASE(addEdge_test2)
{
	AdjacencyListVector<int, int> graph;
	cupcfd::error::eCodes status;
	// int idx;
	// bool exists;

	status = graph.addNode(20);
	status = graph.addNode(40);
	status = graph.addEdge(20, 40);
	status = graph.addEdge(20, 40);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_ADJACENCY_LIST_EDGE_EXISTS);
}

// Test 3: Check the source missing node is detected
BOOST_AUTO_TEST_CASE(addEdge_test3)
{
	AdjacencyListVector<int, int> graph;
	cupcfd::error::eCodes status;
	// int idx;
	// bool exists;

	// Setup
	status = graph.addNode(20);


	// Test and Check
	// Check edge was added successfully
	status = graph.addEdge(20, 40);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_ADJACENCY_LIST_NODE_MISSING);
}


// Test 4: Check the destination missing node is detected
BOOST_AUTO_TEST_CASE(addEdge_test4)
{
	AdjacencyListVector<int, int> graph;
	cupcfd::error::eCodes status;
	// int idx;
	// bool exists;

	// Setup
	status = graph.addNode(20);


	// Test and Check
	// Check edge was added successfully
	status = graph.addEdge(40, 20);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_ADJACENCY_LIST_NODE_MISSING);
}

// === existsEdge ===
// Test 1: Determine that edge exists in one direction
BOOST_AUTO_TEST_CASE(existsEdge_test1)
{
	AdjacencyListVector<int, int> graph;
	cupcfd::error::eCodes status;
	// int idx;
	bool exists;

	status = graph.addNode(20);
	status = graph.addNode(40);
	status = graph.addEdge(20, 40);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	status = graph.existsEdge(20, 40, &exists);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	BOOST_CHECK_EQUAL(exists, true);

	status = graph.existsEdge(40, 20, &exists);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	BOOST_CHECK_EQUAL(exists, false);
}

// Test 2: Determine that edge is missing
BOOST_AUTO_TEST_CASE(existsEdge_test2)
{
	AdjacencyListVector<int, int> graph;
	cupcfd::error::eCodes status;
	// int idx;
	bool exists;

	status = graph.addNode(20);
	status = graph.addNode(40);

	status = graph.existsEdge(20, 40, &exists);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	BOOST_CHECK_EQUAL(exists, false);
}

// Test 3: Test that edge exists in both directions
BOOST_AUTO_TEST_CASE(existsEdge_test3)
{
	AdjacencyListVector<int, int> graph;
	cupcfd::error::eCodes status;
	// int idx;
	bool exists;

	status = graph.addNode(20);
	status = graph.addNode(40);
	status = graph.addEdge(20, 40);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	status = graph.addEdge(40, 20);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	status = graph.existsEdge(20, 40, &exists);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	BOOST_CHECK_EQUAL(exists, true);

	status = graph.existsEdge(40, 20, &exists);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	BOOST_CHECK_EQUAL(exists, true);
}

// === getAdjacentNodeCount ===
// Test 1: Get correct adjacenct node count
BOOST_AUTO_TEST_CASE(getAdjacentNodeCount_test1)
{
	AdjacencyListVector<int, int> graph;
	cupcfd::error::eCodes status;
	// int idx;
	// bool exists;
	int count;

	status = graph.addNode(20);
	status = graph.addNode(40);
	status = graph.addNode(60);
	status = graph.addNode(80);
	status = graph.addNode(100);

	status = graph.getAdjacentNodeCount(20, &count);
	BOOST_CHECK_EQUAL(count, 0);

	status = graph.addEdge(20, 40);
	status = graph.getAdjacentNodeCount(20, &count);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	BOOST_CHECK_EQUAL(count, 1);
	status = graph.getAdjacentNodeCount(40, &count);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	BOOST_CHECK_EQUAL(count, 0);

	status = graph.addEdge(20, 60);
	status = graph.getAdjacentNodeCount(20, &count);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	BOOST_CHECK_EQUAL(count, 2);
	status = graph.getAdjacentNodeCount(60, &count);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	BOOST_CHECK_EQUAL(count, 0);

	status = graph.addEdge(20, 80);
	status = graph.getAdjacentNodeCount(20, &count);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	BOOST_CHECK_EQUAL(count, 3);
	status = graph.getAdjacentNodeCount(80, &count);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	BOOST_CHECK_EQUAL(count, 0);

	status = graph.addEdge(20, 100);
	status = graph.getAdjacentNodeCount(20, &count);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	BOOST_CHECK_EQUAL(count, 4);
	status = graph.getAdjacentNodeCount(100, &count);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	BOOST_CHECK_EQUAL(count, 0);
}

// Test 2: Error Code: Search on a node that is missing
BOOST_AUTO_TEST_CASE(getAdjacentNodeCount_test2)
{
	AdjacencyListVector<int, int> graph;
	cupcfd::error::eCodes status;
	// int idx;
	// bool exists;
	int count;

	status = graph.addNode(20);
	status = graph.addNode(40);
	status = graph.addEdge(20, 40);
	status = graph.getAdjacentNodeCount(80, &count);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_ADJACENCY_LIST_NODE_MISSING);
}

// === getAdjacentNodes ===
// Test 1: Check correct nodes are retrieved
BOOST_AUTO_TEST_CASE(getAdjacentNodes_test1)
{
	AdjacencyListVector<int, int> graph;
	cupcfd::error::eCodes status;
	// int idx;
	// bool exists;
	// int count;

	// Setup
	int nodes[4] = {20, 40, 60, 80};
	int edges1[5] = {20, 20, 20, 40, 40};
	int edges2[5] = {40, 60, 80, 60, 80};

	for(int i = 0; i < 4; i++)
	{
		status = graph.addNode(nodes[i]);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	}

	for(int i = 0; i < 5; i++)
	{
		status = graph.addEdge(edges1[i], edges2[i]);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	}

	// Test and Check
	int cmp1[3] = {40, 60, 80};
	int store1[3];
	status = graph.getAdjacentNodes(20, store1, 3);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	BOOST_CHECK_EQUAL_COLLECTIONS(cmp1, cmp1 + 3, store1, store1 + 3);

	int cmp2[2] = {60, 80};
	int store2[2];
	status = graph.getAdjacentNodes(40, store2, 3);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	BOOST_CHECK_EQUAL_COLLECTIONS(cmp2, cmp2 + 2, store2, store2 + 2);
}


// === reset ===
// Test 1: Test edges and nodes no longer exist after reset
BOOST_AUTO_TEST_CASE(reset_test1)
{
	AdjacencyListVector<int, int> graph;
	cupcfd::error::eCodes status;
	// int idx;
	bool exists;
	// int count;

	// Setup
	int nodes[4] = {20, 40, 60, 80};
	int edges1[5] = {20, 20, 20, 40, 40};
	int edges2[5] = {40, 60, 80, 60, 80};

	for(int i = 0; i < 4; i++)
	{
		status = graph.addNode(nodes[i]);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	}

	for(int i = 0; i < 5; i++)
	{
		status = graph.addEdge(edges1[i], edges2[i]);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	}

	int cmp1[3] = {40, 60, 80};
	int store1[3];
	status = graph.getAdjacentNodes(20, store1, 3);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	BOOST_CHECK_EQUAL_COLLECTIONS(cmp1, cmp1 + 3, store1, store1 + 3);

	int cmp2[2] = {60, 80};
	int store2[2];
	status = graph.getAdjacentNodes(40, store2, 3);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	BOOST_CHECK_EQUAL_COLLECTIONS(cmp2, cmp2 + 2, store2, store2 + 2);

	// Test and Check
	graph.reset();

	BOOST_CHECK_EQUAL(graph.nNodes, 0);
	BOOST_CHECK_EQUAL(graph.nEdges, 0);

	exists = graph.existsNode(20);
	BOOST_CHECK_EQUAL(exists, false);

	exists = graph.existsNode(40);
	BOOST_CHECK_EQUAL(exists, false);

	exists = graph.existsNode(60);
	BOOST_CHECK_EQUAL(exists, false);

	exists = graph.existsNode(80);
	BOOST_CHECK_EQUAL(exists, false);

	status = graph.existsEdge(20, 40, &exists);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_ADJACENCY_LIST_NODE_MISSING);

	status = graph.existsEdge(20, 60, &exists);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_ADJACENCY_LIST_NODE_MISSING);

	status = graph.existsEdge(20, 80, &exists);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_ADJACENCY_LIST_NODE_MISSING);

	status = graph.existsEdge(40, 60, &exists);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_ADJACENCY_LIST_NODE_MISSING);

	status = graph.existsEdge(40, 80, &exists);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_ADJACENCY_LIST_NODE_MISSING);
}


// === operator= ===
// Test 1: Test copy and initialisation from a AdjacencyCSR
BOOST_AUTO_TEST_CASE(operator_copy_test1)
{
	AdjacencyListCSR<int, int> graph;
	AdjacencyListVector<int, int> graph2;
	cupcfd::error::eCodes status;

	int nodes[5] = {20, 40, 60, 80, 100};
	int edges1[6] = {20, 20, 20, 20, 80, 60};
	int edges2[6] = {40, 60, 80, 100, 100, 80};

	for(int i = 0; i < 5; i++)
	{
		status = graph.addNode(nodes[i]);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	}

	for(int i = 0; i < 6; i++)
	{
		status = graph.addEdge(edges1[i], edges2[i]);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	}

	graph2 = graph;
	BOOST_CHECK_EQUAL(graph2.nNodes, 5);
	BOOST_CHECK_EQUAL(graph2.nEdges, 6);

	int nodesDest[5];
	int edgeNode1Dest[6];
	int edgeNode2Dest[6];

	status = graph2.getNodes(nodesDest, 5);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	status = graph2.getEdges(edgeNode1Dest, 6, edgeNode2Dest, 6);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	int edges1Cmp[6] = {20, 20, 20, 20, 60, 80};
	int edges2Cmp[6] = {40, 60, 80, 100, 80, 100};

	BOOST_CHECK_EQUAL_COLLECTIONS(nodes, nodes + 5, nodesDest, nodesDest + 5);
	BOOST_CHECK_EQUAL_COLLECTIONS(edges1Cmp, edges1Cmp + 6, edgeNode1Dest, edgeNode1Dest + 6);
	BOOST_CHECK_EQUAL_COLLECTIONS(edges2Cmp, edges2Cmp + 6, edgeNode2Dest, edgeNode2Dest + 6);
}

// Test 1: Test copy and initialisation from a AdjacencyVector
BOOST_AUTO_TEST_CASE(operator_copy_test2)
{
	AdjacencyListCSR<int, int> graph;
	AdjacencyListVector<int, int> graph2;
	cupcfd::error::eCodes status;

	int nodes[5] = {20, 40, 60, 80, 100};
	int edges1[6] = {20, 20, 20, 20, 80, 60};
	int edges2[6] = {40, 60, 80, 100, 100, 80};

	for(int i = 0; i < 5; i++)
	{
		status = graph.addNode(nodes[i]);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	}

	for(int i = 0; i < 6; i++)
	{
		status = graph.addEdge(edges1[i], edges2[i]);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	}

	graph2 = graph;
	BOOST_CHECK_EQUAL(graph2.nNodes, 5);
	BOOST_CHECK_EQUAL(graph2.nEdges, 6);

	int nodesDest[5];
	int edgeNode1Dest[6];
	int edgeNode2Dest[6];

	status = graph2.getNodes(nodesDest, 5);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	status = graph2.getEdges(edgeNode1Dest, 6, edgeNode2Dest, 6);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	int edges1Cmp[6] = {20, 20, 20, 20, 60, 80};
	int edges2Cmp[6] = {40, 60, 80, 100, 80, 100};

	BOOST_CHECK_EQUAL_COLLECTIONS(nodes, nodes + 5, nodesDest, nodesDest + 5);
	BOOST_CHECK_EQUAL_COLLECTIONS(edges1Cmp, edges1Cmp + 6, edgeNode1Dest, edgeNode1Dest + 6);
	BOOST_CHECK_EQUAL_COLLECTIONS(edges2Cmp, edges2Cmp + 6, edgeNode2Dest, edgeNode2Dest + 6);
}
