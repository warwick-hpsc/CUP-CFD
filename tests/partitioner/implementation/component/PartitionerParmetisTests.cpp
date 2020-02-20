/*
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Unit Tests for the PartitionerParmetis class
 */

#define BOOST_TEST_MODULE Partitioner
#include <boost/test/unit_test.hpp>
#include <boost/test/output_test_stream.hpp>
#include <stdexcept>
#include <cstdlib>
#include <iostream>

#include "PartitionerParmetis.h"
#include "Error.h"
#include "parmetis.h"

// ======= Setup ========
// These tests require MPI
BOOST_AUTO_TEST_CASE(setup)
{
    int argc = boost::unit_test::framework::master_test_suite().argc;
    char ** argv = boost::unit_test::framework::master_test_suite().argv;
    MPI_Init(&argc, &argv);
}

// ========================================
// ============== Tests ===================
// ========================================
using namespace cupcfd::partitioner;

// ========================================
// ============ Constructors ==============
// ========================================
// Test 1: Empty constructor - Check default state
// Output:
// Result/nodes arrays should be nullptr, sizes should be 0
// nParts should be 0
BOOST_AUTO_TEST_CASE(constructor_test1)
{

}

// ============== resetSubdomainWeights ===================
// Test 1: Reset on empty default object
BOOST_AUTO_TEST_CASE(resetSubdomainWeights_test1)
{
	cupcfd::comm::Communicator comm(MPI_COMM_WORLD);
	PartitionerParmetis<int,int> partitioner(comm);
	cupcfd::error::eCodes status;

	// Test Method
	status = partitioner.resetSubdomainWeights();
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	BOOST_CHECK_EQUAL(partitioner.tpwgts, static_cast<decltype(partitioner.tpwgts)>(nullptr));
	BOOST_CHECK_EQUAL(partitioner.nTpwgts, 0);
}

// Test 2: Reset on object with subdomain weights array set
BOOST_AUTO_TEST_CASE(resetSubdomainWeights_test2)
{
	cupcfd::comm::Communicator comm(MPI_COMM_WORLD);
	PartitionerParmetis<int,int> partitioner(comm);
	cupcfd::error::eCodes status;

	// Setup
	partitioner.tpwgts = (real_t *) malloc(sizeof(real_t) * 4);
	partitioner.nTpwgts = 4;
	BOOST_CHECK_NE(partitioner.tpwgts, static_cast<decltype(partitioner.tpwgts)>(nullptr));
	BOOST_CHECK_EQUAL(partitioner.nTpwgts, 4);

	// Test Method
	status = partitioner.resetSubdomainWeights();

	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	BOOST_CHECK_EQUAL(partitioner.tpwgts, static_cast<decltype(partitioner.tpwgts)>(nullptr));
	BOOST_CHECK_EQUAL(partitioner.nTpwgts, 0);
}

// ============== resetVertexEdgeWeights ===================
// Test 1: Reset on empty default object
BOOST_AUTO_TEST_CASE(resetVertexEdgeWeights_test1)
{
	cupcfd::comm::Communicator comm(MPI_COMM_WORLD);
	PartitionerParmetis<int,int> partitioner(comm);
	cupcfd::error::eCodes status;

	// Test Method
	status = partitioner.resetVertexEdgeWeights();

	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	BOOST_CHECK_EQUAL(partitioner.vwgt, static_cast<decltype(partitioner.vwgt)>(nullptr));
	BOOST_CHECK_EQUAL(partitioner.nVwgt, 0);

	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	BOOST_CHECK_EQUAL(partitioner.adjwgt, static_cast<decltype(partitioner.adjwgt)>(nullptr));
	BOOST_CHECK_EQUAL(partitioner.nAdjwgt, 0);
}

// Test 2: Reset on object with vertex edge weights arrays set
BOOST_AUTO_TEST_CASE(resetVertexEdgeWeights_test2)
{
	cupcfd::comm::Communicator comm(MPI_COMM_WORLD);
	PartitionerParmetis<int,int> partitioner(comm);
	cupcfd::error::eCodes status;

	// Setup
	partitioner.vwgt = (idx_t *) malloc(sizeof(idx_t) * 4);
	partitioner.nVwgt = 4;
	BOOST_CHECK_NE(partitioner.vwgt, static_cast<decltype(partitioner.vwgt)>(nullptr));
	BOOST_CHECK_EQUAL(partitioner.nVwgt, 4);

	partitioner.adjwgt = (idx_t *) malloc(sizeof(idx_t) * 4);
	partitioner.nAdjwgt = 4;
	BOOST_CHECK_NE(partitioner.adjwgt, static_cast<decltype(partitioner.adjwgt)>(nullptr));
	BOOST_CHECK_EQUAL(partitioner.nAdjwgt, 4);

	// Test Method
	status = partitioner.resetVertexEdgeWeights();

	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	BOOST_CHECK_EQUAL(partitioner.vwgt, static_cast<decltype(partitioner.vwgt)>(nullptr));
	BOOST_CHECK_EQUAL(partitioner.nVwgt, 0);

	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	BOOST_CHECK_EQUAL(partitioner.adjwgt, static_cast<decltype(partitioner.adjwgt)>(nullptr));
	BOOST_CHECK_EQUAL(partitioner.nAdjwgt, 0);
}

// ============== resetVertexImbalanceWeights ===================
// Test 1: Reset on empty default object
BOOST_AUTO_TEST_CASE(resetVertexImbalanceWeights_test1)
{
	cupcfd::comm::Communicator comm(MPI_COMM_WORLD);
	PartitionerParmetis<int,int> partitioner(comm);
	cupcfd::error::eCodes status;

	// Test Method
	status = partitioner.resetVertexImbalanceWeights();
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	BOOST_CHECK_EQUAL(partitioner.ubvec, static_cast<decltype(partitioner.ubvec)>(nullptr));
	BOOST_CHECK_EQUAL(partitioner.nUbvec, 0);
}

// Test 2: Reset on object with subdomain weights array set
BOOST_AUTO_TEST_CASE(resetVertexImbalanceWeights_test2)
{
	cupcfd::comm::Communicator comm(MPI_COMM_WORLD);
	PartitionerParmetis<int,int> partitioner(comm);
	cupcfd::error::eCodes status;

	// Setup
	partitioner.ubvec = (real_t *) malloc(sizeof(real_t) * 4);
	partitioner.nUbvec = 4;
	BOOST_CHECK_NE(partitioner.ubvec, static_cast<decltype(partitioner.ubvec)>(nullptr));
	BOOST_CHECK_EQUAL(partitioner.nUbvec, 4);

	// Test Method
	status = partitioner.resetVertexImbalanceWeights();

	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	BOOST_CHECK_EQUAL(partitioner.ubvec, static_cast<decltype(partitioner.ubvec)>(nullptr));
	BOOST_CHECK_EQUAL(partitioner.nUbvec, 0);
}

// ============== resetWorkArrays ===================
// Test 1: Reset on empty default object
BOOST_AUTO_TEST_CASE(resetWorkArrays_test1)
{
	cupcfd::comm::Communicator comm(MPI_COMM_WORLD);
	PartitionerParmetis<int,int> partitioner(comm);
	cupcfd::error::eCodes status;

	// Test Method and check status
	status = partitioner.resetWorkArrays();
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	BOOST_CHECK_EQUAL(partitioner.xadj, static_cast<decltype(partitioner.xadj)>(nullptr));
	BOOST_CHECK_EQUAL(partitioner.nXAdj, 0);

	BOOST_CHECK_EQUAL(partitioner.adjncy, static_cast<decltype(partitioner.adjncy)>(nullptr));
	BOOST_CHECK_EQUAL(partitioner.nAdjncy, 0);

	BOOST_CHECK_EQUAL(partitioner.vtxdist, static_cast<decltype(partitioner.vtxdist)>(nullptr));
	BOOST_CHECK_EQUAL(partitioner.nVtxdist, 0);
}

// Test 2: Reset on object with arrays set
BOOST_AUTO_TEST_CASE(resetWorkArrays_test2)
{
	cupcfd::comm::Communicator comm(MPI_COMM_WORLD);
	PartitionerParmetis<int,int> partitioner(comm);
	cupcfd::error::eCodes status;

	// Setup
	partitioner.xadj = (idx_t *) malloc(sizeof(idx_t) * 4);
	partitioner.nXAdj = 4;

	partitioner.adjncy = (idx_t *) malloc(sizeof(idx_t) * 4);
	partitioner.nAdjncy = 4;

	partitioner.vtxdist = (idx_t *) malloc(sizeof(idx_t) * 4);
	partitioner.nVtxdist = 4;

	// ToDo: Also set communicator for test?

	// Test Method and check status
	status = partitioner.resetWorkArrays();
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	BOOST_CHECK_EQUAL(partitioner.xadj, static_cast<decltype(partitioner.xadj)>(nullptr));
	BOOST_CHECK_EQUAL(partitioner.nXAdj, 0);

	BOOST_CHECK_EQUAL(partitioner.adjncy, static_cast<decltype(partitioner.adjncy)>(nullptr));
	BOOST_CHECK_EQUAL(partitioner.nAdjncy, 0);

	BOOST_CHECK_EQUAL(partitioner.vtxdist, static_cast<decltype(partitioner.vtxdist)>(nullptr));
	BOOST_CHECK_EQUAL(partitioner.nVtxdist, 0);
}

// ============== setWeightArrays ===================
// Currently this method does nothing in the PartitionerParmetis class, so no suitable test cases

// ============== setSubdomainWeightArrays ===================
// Test 1: Check that the array is set up correctly from a default empty class
BOOST_AUTO_TEST_CASE(setSubdomainWeightArrays_test1)
{
	cupcfd::comm::Communicator comm(MPI_COMM_WORLD);
	PartitionerParmetis<int,int> partitioner(comm);
	cupcfd::error::eCodes status;

	partitioner.nCon = 3;
	partitioner.nParts = 4;

	status = partitioner.setSubdomainWeightArrays();

	// Check correct status and array set correctly
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	BOOST_CHECK_NE(partitioner.tpwgts, static_cast<decltype(partitioner.tpwgts)>(nullptr));
	BOOST_CHECK_EQUAL(partitioner.nTpwgts, 12);

	real_t wgt = 1.0 / 4;
	real_t tpwgtsCmp[12] = {wgt, wgt, wgt, wgt, wgt, wgt, wgt, wgt, wgt, wgt, wgt, wgt};
	BOOST_CHECK_EQUAL_COLLECTIONS(partitioner.tpwgts, partitioner.tpwgts + 12, tpwgtsCmp, tpwgtsCmp + 12);
}

// Test 2: Check that the array is setup correctly if there already exists an array
BOOST_AUTO_TEST_CASE(setSubdomainWeightArrays_test2)
{
	// ToDo: Since method call is a blackbox, cannot check that the previous array
	// was correctly freed during this method (though it should be via resetSubdomainWeightArrays_test2)
	// Is it possible to add a check for this (e.g. by checking previous memory allocation)?

	cupcfd::comm::Communicator comm(MPI_COMM_WORLD);
	PartitionerParmetis<int,int> partitioner(comm);
	cupcfd::error::eCodes status;

	partitioner.setNCon(3);
	partitioner.setNParts(4);

	// Setup
	partitioner.nTpwgts = 5;
	partitioner.tpwgts = (real_t *) malloc(sizeof(real_t) * partitioner.nTpwgts);

	status = partitioner.setSubdomainWeightArrays();

	// Check correct status and array set correctly
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	BOOST_CHECK_NE(partitioner.tpwgts, static_cast<decltype(partitioner.tpwgts)>(nullptr));
	BOOST_CHECK_EQUAL(partitioner.nTpwgts, 12);

	real_t wgt = 1.0 / 4;
	real_t tpwgtsCmp[12] = {wgt, wgt, wgt, wgt, wgt, wgt, wgt, wgt, wgt, wgt, wgt, wgt};
	BOOST_CHECK_EQUAL_COLLECTIONS(partitioner.tpwgts, partitioner.tpwgts + 12, tpwgtsCmp, tpwgtsCmp + 12);
}

// Test 3: Error edge case - cannot create array if nCon value is 0
BOOST_AUTO_TEST_CASE(setSubdomainWeightArrays_test3)
{
	cupcfd::comm::Communicator comm(MPI_COMM_WORLD);
	PartitionerParmetis<int,int> partitioner(comm);
	cupcfd::error::eCodes status;

	partitioner.nCon = 0;
	partitioner.nParts = 4;

	status = partitioner.setSubdomainWeightArrays();

	// Check correct status and array still unset
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_PARMETIS_INVALID_NCON);
	BOOST_CHECK_EQUAL(partitioner.tpwgts, static_cast<decltype(partitioner.tpwgts)>(nullptr));
	BOOST_CHECK_EQUAL(partitioner.nTpwgts, 0);
}

// Test 4: Error edge case - cannot create array if nCon value is < 1
BOOST_AUTO_TEST_CASE(setSubdomainWeightArrays_test4)
{
	cupcfd::comm::Communicator comm(MPI_COMM_WORLD);
	PartitionerParmetis<int,int> partitioner(comm);
	cupcfd::error::eCodes status;

	partitioner.nCon = -1;
	partitioner.nParts = 4;

	status = partitioner.setSubdomainWeightArrays();

	// Check correct status and array still unset
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_PARMETIS_INVALID_NCON);
	BOOST_CHECK_EQUAL(partitioner.tpwgts, static_cast<decltype(partitioner.tpwgts)>(nullptr));
	BOOST_CHECK_EQUAL(partitioner.nTpwgts, 0);
}

// Test 5: Error edge case - cannot create array if nparts value is 0
BOOST_AUTO_TEST_CASE(setSubdomainWeightArrays_test5)
{
	cupcfd::comm::Communicator comm(MPI_COMM_WORLD);
	PartitionerParmetis<int,int> partitioner(comm);
	cupcfd::error::eCodes status;

	partitioner.nCon = 3;
	partitioner.nParts = 0;

	status = partitioner.setSubdomainWeightArrays();

	// Check correct status and array still unset
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_PARMETIS_INVALID_NPARTS);
	BOOST_CHECK_EQUAL(partitioner.tpwgts, static_cast<decltype(partitioner.tpwgts)>(nullptr));
	BOOST_CHECK_EQUAL(partitioner.nTpwgts, 0);
}

// Test 6: Error edge case - cannot create array if nparts value is < 1
BOOST_AUTO_TEST_CASE(setSubdomainWeightArrays_test6)
{
	cupcfd::comm::Communicator comm(MPI_COMM_WORLD);
	PartitionerParmetis<int,int> partitioner(comm);
	cupcfd::error::eCodes status;

	partitioner.nCon = 3;
	partitioner.nParts = -1;

	status = partitioner.setSubdomainWeightArrays();

	// Check correct status and array still unset
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_PARMETIS_INVALID_NPARTS);
	BOOST_CHECK_EQUAL(partitioner.tpwgts, static_cast<decltype(partitioner.tpwgts)>(nullptr));
	BOOST_CHECK_EQUAL(partitioner.nTpwgts, 0);
}

// ============== setVertexImbalanceWeightArrays ===================
// Test 1: Check that the array is set up correctly from a default empty class
BOOST_AUTO_TEST_CASE(setVertexImbalanceWeightArrays_test1)
{
	cupcfd::comm::Communicator comm(MPI_COMM_WORLD);
	PartitionerParmetis<int,int> partitioner(comm);
	cupcfd::error::eCodes status;

	partitioner.nCon = 3;

	status = partitioner.setVertexImbalanceWeightArrays();

	// Check correct status and array set correctly
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	BOOST_CHECK_NE(partitioner.ubvec, static_cast<decltype(partitioner.ubvec)>(nullptr));
	BOOST_CHECK_EQUAL(partitioner.nUbvec, 3);

	real_t ubvecCmp[3] = {1.05, 1.05, 1.05};
	BOOST_CHECK_EQUAL_COLLECTIONS(partitioner.ubvec, partitioner.ubvec + 3, ubvecCmp, ubvecCmp + 3);
}

// Test 2: Check that the array is setup correctly if there already exists an array
BOOST_AUTO_TEST_CASE(setVertexImbalanceWeightArrays_test2)
{
	cupcfd::comm::Communicator comm(MPI_COMM_WORLD);
	PartitionerParmetis<int,int> partitioner(comm);
	cupcfd::error::eCodes status;

	// Setup
	partitioner.nCon = 5;
	status = partitioner.setVertexImbalanceWeightArrays();
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	BOOST_CHECK_NE(partitioner.ubvec, static_cast<decltype(partitioner.ubvec)>(nullptr));
	BOOST_CHECK_EQUAL(partitioner.nUbvec, 5);

	// Test and Check
	partitioner.nCon = 3;
	status = partitioner.setVertexImbalanceWeightArrays();
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	BOOST_CHECK_NE(partitioner.ubvec, static_cast<decltype(partitioner.ubvec)>(nullptr));
	BOOST_CHECK_EQUAL(partitioner.nUbvec, 3);

	real_t ubvecCmp[3] = {1.05, 1.05, 1.05};
	BOOST_CHECK_EQUAL_COLLECTIONS(partitioner.ubvec, partitioner.ubvec + 3, ubvecCmp, ubvecCmp + 3);
}

// Test 3: Error edge case - cannot create array if nCon value is 0
BOOST_AUTO_TEST_CASE(setVertexImbalanceWeightArrays_test3)
{
	cupcfd::comm::Communicator comm(MPI_COMM_WORLD);
	PartitionerParmetis<int,int> partitioner(comm);
	cupcfd::error::eCodes status;

	partitioner.nCon = 0;

	status = partitioner.setVertexImbalanceWeightArrays();

	// Check correct status and array set correctly
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_PARMETIS_INVALID_NCON);
	BOOST_CHECK_EQUAL(partitioner.ubvec, static_cast<decltype(partitioner.ubvec)>(nullptr));
	BOOST_CHECK_EQUAL(partitioner.nUbvec, 0);
}

// Test 4: Error edge case - cannot create array if nCon value is < 0
BOOST_AUTO_TEST_CASE(setVertexImbalanceWeightArrays_test4)
{
	cupcfd::comm::Communicator comm(MPI_COMM_WORLD);
	PartitionerParmetis<int,int> partitioner(comm);
	cupcfd::error::eCodes status;

	partitioner.nCon = -1;

	status = partitioner.setVertexImbalanceWeightArrays();

	// Check correct status and array set correctly
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_PARMETIS_INVALID_NCON);
	BOOST_CHECK_EQUAL(partitioner.ubvec, static_cast<decltype(partitioner.ubvec)>(nullptr));
	BOOST_CHECK_EQUAL(partitioner.nUbvec, 0);
}

// ============== setNCon ===================
// Test 1: Ensure ncon is set correctly in a default object
BOOST_AUTO_TEST_CASE(setNCon_test1)
{
	cupcfd::comm::Communicator comm(MPI_COMM_WORLD);
	PartitionerParmetis<int,int> partitioner(comm);
	cupcfd::error::eCodes status;

	// Test and Check
	status = partitioner.setNCon(4);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	BOOST_CHECK_EQUAL(partitioner.tpwgts, static_cast<decltype(partitioner.tpwgts)>(nullptr));
	BOOST_CHECK_EQUAL(partitioner.nTpwgts, 0);

	BOOST_CHECK_EQUAL(partitioner.ubvec, static_cast<decltype(partitioner.ubvec)>(nullptr));
	BOOST_CHECK_EQUAL(partitioner.nUbvec, 0);
}

// Test 2: Ensure ncon is set correctly in an object where ncon was previously set
BOOST_AUTO_TEST_CASE(setNCon_test2)
{
	cupcfd::comm::Communicator comm(MPI_COMM_WORLD);
	PartitionerParmetis<int,int> partitioner(comm);
	cupcfd::error::eCodes status;

	// Test and Check
	status = partitioner.setNCon(4);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	status = partitioner.setNCon(8);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	BOOST_CHECK_EQUAL(partitioner.tpwgts, static_cast<decltype(partitioner.tpwgts)>(nullptr));
	BOOST_CHECK_EQUAL(partitioner.nTpwgts, 0);

	BOOST_CHECK_EQUAL(partitioner.ubvec, static_cast<decltype(partitioner.ubvec)>(nullptr));
	BOOST_CHECK_EQUAL(partitioner.nUbvec, 0);
}

// Test 3: Ensure appropriate arrays are correctly reset when ncon is changed
BOOST_AUTO_TEST_CASE(setNCon_test3)
{
	cupcfd::comm::Communicator comm(MPI_COMM_WORLD);
	PartitionerParmetis<int,int> partitioner(comm);
	cupcfd::error::eCodes status;

	// Setup
	// Sizes of these don't matter for this test, as long as they are set
	partitioner.tpwgts = (real_t *) malloc(sizeof(real_t) * 4);
	partitioner.nTpwgts = 4;

	partitioner.ubvec = (real_t *) malloc(sizeof(real_t) * 4);
	partitioner.nUbvec = 4;

	// Test and Check
	status = partitioner.setNCon(4);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	BOOST_CHECK_EQUAL(partitioner.tpwgts, static_cast<decltype(partitioner.tpwgts)>(nullptr));
	BOOST_CHECK_EQUAL(partitioner.nTpwgts, 0);

	BOOST_CHECK_EQUAL(partitioner.ubvec, static_cast<decltype(partitioner.ubvec)>(nullptr));
	BOOST_CHECK_EQUAL(partitioner.nUbvec, 0);
}

// ============== setNParts ===================
// Test 1: Ensure nparts is set correctly in a default object
BOOST_AUTO_TEST_CASE(setNparts_test1)
{
	cupcfd::comm::Communicator comm(MPI_COMM_WORLD);
	PartitionerParmetis<int,int> partitioner(comm);
	cupcfd::error::eCodes status;

	// Test and Check
	status = partitioner.setNParts(4);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	BOOST_CHECK_EQUAL(partitioner.tpwgts, static_cast<decltype(partitioner.tpwgts)>(nullptr));
	BOOST_CHECK_EQUAL(partitioner.nTpwgts, 0);
}

// Test 2: Ensure nparts is set correctly in an object where ncon was previously set
BOOST_AUTO_TEST_CASE(setNparts_test2)
{
	cupcfd::comm::Communicator comm(MPI_COMM_WORLD);
	PartitionerParmetis<int,int> partitioner(comm);
	cupcfd::error::eCodes status;

	// Test and Check
	status = partitioner.setNParts(4);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	status = partitioner.setNParts(8);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	BOOST_CHECK_EQUAL(partitioner.tpwgts, static_cast<decltype(partitioner.tpwgts)>(nullptr));
	BOOST_CHECK_EQUAL(partitioner.nTpwgts, 0);
}

// Test 3: Ensure appropriate arrays are correctly reset when nparts is changed
BOOST_AUTO_TEST_CASE(setNparts_test3)
{
	cupcfd::comm::Communicator comm(MPI_COMM_WORLD);
	PartitionerParmetis<int,int> partitioner(comm);
	cupcfd::error::eCodes status;

	// Setup
	// Sizes of these don't matter for this test, as long as they are set
	partitioner.tpwgts = (real_t *) malloc(sizeof(real_t) * 4);
	partitioner.nTpwgts = 4;

	// Test and Check
	status = partitioner.setNParts(4);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	BOOST_CHECK_EQUAL(partitioner.tpwgts, static_cast<decltype(partitioner.tpwgts)>(nullptr));
	BOOST_CHECK_EQUAL(partitioner.nTpwgts, 0);
}

// === initialise ===
// Test 1:
BOOST_AUTO_TEST_CASE(initialise_test1)
{

}

// === partition ===
// Test 1: Successful Partition
BOOST_AUTO_TEST_CASE(partition_test1)
{
	cupcfd::error::eCodes status;
	cupcfd::comm::Communicator comm(MPI_COMM_WORLD);
	PartitionerParmetis<int, int> * partitioner = new PartitionerParmetis<int, int>(comm);
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

	// Use the Builder to setup the Parmetis partitioner
	status = partitioner->initialise(graph, 4);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	status = partitioner->partition();
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	int * results;
	int nResults;

	status = partitioner->assignRankNodes(&results, &nResults);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	delete(partitioner);
}

// Test 2: Catch error when xadj work array is not set
BOOST_AUTO_TEST_CASE(partition_test2)
{
	cupcfd::comm::Communicator comm(MPI_COMM_WORLD);
	PartitionerParmetis<int,int> partitioner(comm);
	cupcfd::error::eCodes status;

	// Setup
	BOOST_CHECK_EQUAL(partitioner.xadj, static_cast<decltype(partitioner.xadj)>(nullptr));
	BOOST_CHECK_EQUAL(partitioner.nXAdj, 0);

	partitioner.adjncy = (idx_t *) malloc(sizeof(idx_t) * 4);
	partitioner.nAdjncy = 4;

	partitioner.vtxdist = (idx_t *) malloc(sizeof(idx_t) * 4);
	partitioner.nVtxdist = 4;

	status = partitioner.partition();
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_PARMETIS_INVALID_WORK_ARRAY);

	// Arrays are freed by the deconstructor if they are not null, so no need to free here
}

// Test 3: Catch error when adjncy work array is not set
BOOST_AUTO_TEST_CASE(partition_test3)
{
	cupcfd::comm::Communicator comm(MPI_COMM_WORLD);
	PartitionerParmetis<int,int> partitioner(comm);
	cupcfd::error::eCodes status;

	// Setup
	partitioner.xadj = (idx_t *) malloc(sizeof(idx_t) * 4);
	partitioner.nXAdj = 4;

	BOOST_CHECK_EQUAL(partitioner.adjncy, static_cast<decltype(partitioner.adjncy)>(nullptr));
	BOOST_CHECK_EQUAL(partitioner.nAdjncy, 0);

	partitioner.vtxdist = (idx_t *) malloc(sizeof(idx_t) * 4);
	partitioner.nVtxdist = 4;

	status = partitioner.partition();
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_PARMETIS_INVALID_WORK_ARRAY);

	// Arrays are freed by the deconstructor if they are not null, so no need to free here
}

// Test 4: Catch error when vtxdist work array is not set
BOOST_AUTO_TEST_CASE(partition_test4)
{
	cupcfd::comm::Communicator comm(MPI_COMM_WORLD);
	PartitionerParmetis<int,int> partitioner(comm);
	cupcfd::error::eCodes status;

	// Setup
	partitioner.xadj = (idx_t *) malloc(sizeof(idx_t) * 4);
	partitioner.nXAdj = 4;

	partitioner.adjncy = (idx_t *) malloc(sizeof(idx_t) * 4);
	partitioner.nAdjncy = 4;

	status = partitioner.partition();
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_PARMETIS_INVALID_WORK_ARRAY);

	// Arrays are freed by the deconstructor if they are not null, so no need to free here
}

// Test 5: Catch error when nparts not greater than 1
BOOST_AUTO_TEST_CASE(partition_test5)
{
	cupcfd::comm::Communicator comm(MPI_COMM_WORLD);
	PartitionerParmetis<int,int> partitioner(comm);
	cupcfd::error::eCodes status;

	// Setup
	partitioner.nCon = 1;

	partitioner.xadj = (idx_t *) malloc(sizeof(idx_t) * 4);
	partitioner.nXAdj = 4;

	partitioner.adjncy = (idx_t *) malloc(sizeof(idx_t) * 4);
	partitioner.nAdjncy = 4;

	partitioner.vtxdist = (idx_t *) malloc(sizeof(idx_t) * 4);
	partitioner.nVtxdist = 4;

	status = partitioner.partition();
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_PARMETIS_INVALID_NPARTS);

	// Arrays are freed by the deconstructor if they are not null, so no need to free here
}

// Further Test Cases
// Test 6: Catch error when nCon not 1 or greater
// Test 7: Catch error when tpwgts is nullptr
// Test 8: Catch error when ubvec is nullptr


/*
// ============== assignRankNodes ===================
// Test 1:
BOOST_AUTO_TEST_CASE(assignRankNodes_test1)
{
	cupcfd::mpi::Communicator comm;
	cupcfd::error::eCodes status;

	cupcfd::mpi::drivers::set_comm(comm, MPI_COMM_WORLD);

	cupcfd::partitioner::PartitionerParmetis<int,int> partitioner;
	cupcfd::data_structures::DistributedAdjacencyList<int, int> graph(comm);


	// Build a test distributed graph to serve as input

	// Add Local Nodes
	if(comm.rank == 0)
	{
		int localNodes[20] = {1, 2, 3, 6, 7, 8, 9, 10, 11, 13, 18, 19,
							  21, 22, 28, 30, 31, 33, 36, 39};

		for(int i = 0; i < 20; i ++)
		{
			graph.addLocalNode(localNodes[i]);
		}
	}
	else if(comm.rank == 1)
	{
		int localNodes[7] = {4, 5, 32, 34, 35, 26, 27};

		for(int i = 0; i < 7; i ++)
		{
			graph.addLocalNode(localNodes[i]);
		}
	}
	else if(comm.rank == 2)
	{
		int localNodes[7] = {20, 12, 17, 38, 23, 24, 25};

		for(int i = 0; i < 7; i ++)
		{
			graph.addLocalNode(localNodes[i]);
		}
	}
	else if(comm.rank == 3)
	{
		int localNodes[6] = {29, 37, 40, 14, 15, 16};

		for(int i = 0; i < 6; i ++)
		{
			graph.addLocalNode(localNodes[i]);
		}
	}

	// Add Edges
	if(comm.rank == 0)
	{
		int edgeNode1[13] = {1, 2, 1, 6, 1, 28, 36, 7, 22, 32, 12, 28, 21};
		int edgeNode2[13] = {3, 8, 9, 29, 16, 5, 20, 11, 40, 18, 19, 23, 23};

		for(int i = 0; i < 13; i++)
		{
			graph.addUndirectedEdge(edgeNode1[i], edgeNode2[i]);
		}
	}
	else if(comm.rank == 1)
	{
		int edgeNode1[8] = {28, 18, 5, 32, 26, 5, 5, 32};
		int edgeNode2[8] = {5, 32, 27, 5, 27, 38, 37, 15};

		for(int i = 0; i < 8; i++)
		{
			graph.addUndirectedEdge(edgeNode1[i], edgeNode2[i]);
		}
	}
	else if(comm.rank == 2)
	{
		int edgeNode1[11] = {36, 12, 28, 20, 23, 24, 5, 23, 21, 14, 12};
		int edgeNode2[11] = {20, 19, 23, 23, 24, 25, 38, 40, 23, 23, 14};

		for(int i = 0; i < 11; i++)
		{
			graph.addUndirectedEdge(edgeNode1[i], edgeNode2[i]);
		}
	}
	else if(comm.rank == 3)
	{
		int edgeNode1[11] = {6, 1, 22, 37, 14, 16, 5, 32, 23, 14, 12};
		int edgeNode2[11] = {29, 16, 40, 40, 16, 14, 37, 15, 40, 23, 14};

		for(int i = 0; i < 11; i++)
		{
			graph.addUndirectedEdge(edgeNode1[i], edgeNode2[i]);
		}
	}

	// Finalize before use
	graph.finalize();

	// Run partition method with target of 4 partitions
	status = partitioner.partition(graph, 4);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	// Test and Check
	int * rankNodes;
	int nRankNodes;
	status = partitioner.assignRankNodes(&rankNodes, &nRankNodes, comm);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	// ToDo: How to suitably check that the results are allocated correctly
	// Would need an all-to-all, but that's basically what the method itself does......
	// Need a means where the partition is known ahead of time, but can we rely on
	// the partitioner consistently producing the same results across machines, process counts etc?
	// For now just check that it runs without a major error
}

// Test 2: Error Case: Results array is not set
BOOST_AUTO_TEST_CASE(assignRankNodes_test2)
{
	PartitionerParmetis<int,int> partitioner(comm);
	cupcfd::error::eCodes status;
	cupcfd::mpi::Communicator comm;

	// Setup
	cupcfd::mpi::drivers::set_comm(comm, MPI_COMM_WORLD);
	BOOST_CHECK_EQUAL(partitioner.result, static_cast<decltype(partitioner.result)>(nullptr));
	BOOST_CHECK_EQUAL(partitioner.nResult, 0);

	// Test and Check
	int * rankNodes;
	int nRankNodes;
	status = partitioner.assignRankNodes(&rankNodes, &nRankNodes, comm);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_PARMETIS_INVALID_RESULT_ARRAY);
}

// Test 3: Error Case: Communicator for distribution is smaller than number of partitions
BOOST_AUTO_TEST_CASE(assignRankNodes_test3)
{
	PartitionerParmetis<int,int> partitioner(comm);
	cupcfd::error::eCodes status;
	cupcfd::mpi::Communicator comm;

	// Setup
	// Self is only of size 1 so should be less than 4

	cupcfd::mpi::drivers::set_comm(comm, MPI_COMM_SELF);
	partitioner.result = (int *) malloc(sizeof(int) * 4);
	partitioner.nResult = 4;
	partitioner.nParts = 4;

	// Test and Check
	int * rankNodes;
	int nRankNodes;
	status = partitioner.assignRankNodes(&rankNodes, &nRankNodes, comm);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_PARMETIS_UNDERSIZED_COMM);

	// Arrays are freed by the deconstructor if they are not null, so no need to free here
}
*/

// ======= Cleanup ========
// Finalize MPI
BOOST_AUTO_TEST_CASE(cleanup)
{
    // Cleanup MPI Environment
    MPI_Finalize();
}
