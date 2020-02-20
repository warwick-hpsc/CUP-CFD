/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Tests for the concrete methods of the MeshConfig class.
 */

#define BOOST_TEST_MODULE MeshConfig

// Boost setup
#include <boost/test/unit_test.hpp>
#include <boost/test/output_test_stream.hpp>
#include <stdexcept>

#include "Error.h"
#include "MeshConfig.h"
#include "PartitionerConfig.h"
#include "PartitionerNaiveConfig.h"
#include "MeshSourceStructGenConfig.h"
#include "UnstructuredMeshInterface.h"
#include "MeshSourceFileConfig.h"
#include "CupCfdAoSMesh.h"

// Setup
BOOST_AUTO_TEST_CASE(setup)
{
    int argc = boost::unit_test::framework::master_test_suite().argc;
    char ** argv = boost::unit_test::framework::master_test_suite().argv;

    MPI_Init(&argc, &argv);
}

// Namespaces
namespace utf = boost::unit_test;

using namespace cupcfd::geometry::mesh;

// === Constructor ===

// Test 1: Test Constructor with provided PartitionerConfigNaive is built correctly.
BOOST_AUTO_TEST_CASE(constructor_test1, * utf::tolerance(0.00001))
{
	// Setup PartitionerConfig

	// Constructor

	// Delete PartitionerConfig

	// Check Constructor Copy still exists
}

// Test 2: Test Constructor with provided PartitionerConfigParmetis is built correctly.
BOOST_AUTO_TEST_CASE(constructor_test2, * utf::tolerance(0.00001))
{
	// Setup PartitionerConfig

	// Constructor

	// Delete PartitionerConfig

	// Check Constructor Copy still exists
}

// Test 3: Test Constructor with Referenced Config copies values correctly
BOOST_AUTO_TEST_CASE(constructor_test3, * utf::tolerance(0.00001))
{

}

// === set/getPartitionerConfig
BOOST_AUTO_TEST_CASE(getPartitionerConfig_test1, * utf::tolerance(0.00001))
{

}

// === operator= ===
// Test 1: Test values are correctly copied from one config to another
BOOST_AUTO_TEST_CASE(operator_equals_test1, * utf::tolerance(0.00001))
{

}

// === buildUnstructuredMesh ===
// Test 1: Test correct build of a mesh from a HDF5 file format
BOOST_AUTO_TEST_CASE(buildUnstructuredMesh_test1, * utf::tolerance(0.00001))
{
	cupcfd::error::eCodes status;
    cupcfd::comm::Communicator comm(MPI_COMM_WORLD);

	// Setup the partitioner config
	// Use naive so the outcome is predictable
    cupcfd::partitioner::PartitionerNaiveConfig<int,int> naiveConfig;
    cupcfd::partitioner::PartitionerConfig<int,int> partConfig(naiveConfig);

	// Setup the source config
	MeshSourceFileConfig<int, double> meshSourceConfig(MESH_FILE_FORMAT_HDF5, "../tests/geometry/mesh/data/MeshHDF5.hdf5");

	// Setup the config to use for building
	MeshConfig<int, double, int> meshConfig(partConfig, meshSourceConfig);

	CupCfdAoSMesh<int,double,int> * mesh;
	status = meshConfig.buildUnstructuredMesh(&mesh, comm);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	// Naive means we should have 1 cell per rank
	if(comm.rank == 0)
	{
		// The graphs will use labels, not internal indexes
		// The local indexes they map to should still be zero-indexed however
		int localCellCmp[1] = {1};
		int localNodes[1];

		int ghostCellCmp[1] = {2};
		int ghostNodes[1];

		// === Check Local and Ghost Cells ===
		BOOST_CHECK_EQUAL(mesh->cellConnGraph->nLONodes, 1);
		BOOST_CHECK_EQUAL(mesh->cellConnGraph->nLGhNodes, 1);

		mesh->cellConnGraph->getLocalNodes(localNodes, 1);
		BOOST_CHECK_EQUAL_COLLECTIONS(localNodes, localNodes + 1, localCellCmp, localCellCmp + 1);

		mesh->cellConnGraph->getGhostNodes(ghostNodes, 1);
		BOOST_CHECK_EQUAL_COLLECTIONS(ghostNodes, ghostNodes + 1, ghostCellCmp, ghostCellCmp + 1);

		// === Check Internal IDs ===
		BOOST_CHECK_EQUAL(mesh->getCellID(1), 0);
		BOOST_CHECK_EQUAL(mesh->getCellID(2), 1);

		// === Check Cell Data ===
		// Local Cell Data
		int nLocalGVerticesCmp[1] = {8};		// Global Vertex Counts per cell
		int nLocalSVerticesCmp[1] = {8};		// Stored Vertex Counts per cell
		int nLocalGFaceCmp[1] = {6};			// Global Face Counts per cell
		int nLocalSFaceCmp[1] = {6};			// Stored Face Counts per cell

		for(int i = 0; i < 1; i++)
		{
			BOOST_CHECK_EQUAL(mesh->getCellNVertices(mesh->getCellID(localCellCmp[i])),  nLocalGVerticesCmp[i]);
			BOOST_CHECK_EQUAL(mesh->getCellStoredNVertices(mesh->getCellID(localCellCmp[i])),  nLocalSVerticesCmp[i]);

			BOOST_CHECK_EQUAL(mesh->getCellNFaces(mesh->getCellID(localCellCmp[i])),  nLocalGFaceCmp[i]);
			BOOST_CHECK_EQUAL(mesh->getCellStoredNFaces(mesh->getCellID(localCellCmp[i])),  nLocalSFaceCmp[i]);
		}

		// Ghost Cell Data
		int nGhostGVerticesCmp[1] = {8};		// Global Vertex Counts
		int nGhostSVerticesCmp[1] = {4};		// Stored Vertex Counts
		int nGhostGFaceCmp[1] = {6};			// Global Face Counts per cell
		int nGhostSFaceCmp[1] = {1};			// Stored Face Counts per cell

		for(int i = 0; i < 1; i++)
		{
			BOOST_CHECK_EQUAL(mesh->getCellNVertices(mesh->getCellID(ghostCellCmp[i])),  nGhostGVerticesCmp[i]);
			BOOST_CHECK_EQUAL(mesh->getCellStoredNVertices(mesh->getCellID(ghostCellCmp[i])),  nGhostSVerticesCmp[i]);

			BOOST_CHECK_EQUAL(mesh->getCellNFaces(mesh->getCellID(ghostCellCmp[i])),  nGhostGFaceCmp[i]);
			BOOST_CHECK_EQUAL(mesh->getCellStoredNFaces(mesh->getCellID(ghostCellCmp[i])),  nGhostSFaceCmp[i]);
		}

		// Check Face Data

		// Check Vertex Data

		// Check Boundary Data

		// Check Region Data
	}
	else if(comm.rank == 1)
	{
		int localCellCmp[1] = {2};
		int localNodes[1];

		int ghostCellCmp[3] = {1, 3, 4};
		int ghostNodes[3];

		BOOST_CHECK_EQUAL(mesh->cellConnGraph->nLONodes, 1);
		BOOST_CHECK_EQUAL(mesh->cellConnGraph->nLGhNodes, 3);

		mesh->cellConnGraph->getLocalNodes(localNodes, 1);
		BOOST_CHECK_EQUAL_COLLECTIONS(localNodes, localNodes + 1, localCellCmp, localCellCmp + 1);

		mesh->cellConnGraph->getGhostNodes(ghostNodes, 3);
		BOOST_CHECK_EQUAL_COLLECTIONS(ghostNodes, ghostNodes + 3, ghostCellCmp, ghostCellCmp + 3);

		// === Check Internal IDs ===
		BOOST_CHECK_EQUAL(mesh->getCellID(2), 0);
		BOOST_CHECK_EQUAL(mesh->getCellID(1), 1);
		BOOST_CHECK_EQUAL(mesh->getCellID(3), 2);
		BOOST_CHECK_EQUAL(mesh->getCellID(4), 3);

		// === Check Cell Data ===
		// Local Cell Data
		int nLocalGVerticesCmp[1] = {8};		// Global Vertex Counts
		int nLocalSVerticesCmp[1] = {8};		// Stored Vertex Counts
		int nLocalGFaceCmp[1] = {6};			// Global Face Counts per cell
		int nLocalSFaceCmp[1] = {6};			// Stored Face Counts per cell

		for(int i = 0; i < 1; i++)
		{
			BOOST_CHECK_EQUAL(mesh->getCellNVertices(mesh->getCellID(localCellCmp[i])),  nLocalGVerticesCmp[i]);
			BOOST_CHECK_EQUAL(mesh->getCellStoredNVertices(mesh->getCellID(localCellCmp[i])),  nLocalSVerticesCmp[i]);

			BOOST_CHECK_EQUAL(mesh->getCellNFaces(mesh->getCellID(localCellCmp[i])),  nLocalGFaceCmp[i]);
			BOOST_CHECK_EQUAL(mesh->getCellStoredNFaces(mesh->getCellID(localCellCmp[i])),  nLocalSFaceCmp[i]);
		}

		// Ghost Cell Data
		int nGhostGVerticesCmp[3] = {8, 6, 8};		// Global Vertex Counts
		int nGhostSVerticesCmp[3] = {4, 4, 4};		// Stored Vertex Counts
		int nGhostGFaceCmp[3] = {6, 5, 6};			// Global Face Counts per cell
		int nGhostSFaceCmp[3] = {1, 1, 1};			// Stored Face Counts per cell


		for(int i = 0; i < 1; i++)
		{
			BOOST_CHECK_EQUAL(mesh->getCellNVertices(mesh->getCellID(ghostCellCmp[i])),  nGhostGVerticesCmp[i]);
			BOOST_CHECK_EQUAL(mesh->getCellStoredNVertices(mesh->getCellID(ghostCellCmp[i])),  nGhostSVerticesCmp[i]);

			BOOST_CHECK_EQUAL(mesh->getCellNFaces(mesh->getCellID(ghostCellCmp[i])),  nGhostGFaceCmp[i]);
			BOOST_CHECK_EQUAL(mesh->getCellStoredNFaces(mesh->getCellID(ghostCellCmp[i])),  nGhostSFaceCmp[i]);
		}

		// Check Face Data

		// Check Vertex Data

		// Check Boundary Data

		// Check Region Data
	}
	else if(comm.rank == 2)
	{
		int localCellCmp[1] = {3};
		int localNodes[1];

		int ghostCellCmp[1] = {2};
		int ghostNodes[1];

		BOOST_CHECK_EQUAL(mesh->cellConnGraph->nLONodes, 1);
		BOOST_CHECK_EQUAL(mesh->cellConnGraph->nLGhNodes, 1);

		mesh->cellConnGraph->getLocalNodes(localNodes, 1);
		BOOST_CHECK_EQUAL_COLLECTIONS(localNodes, localNodes + 1, localCellCmp, localCellCmp + 1);

		mesh->cellConnGraph->getGhostNodes(ghostNodes, 1);
		BOOST_CHECK_EQUAL_COLLECTIONS(ghostNodes, ghostNodes + 1, ghostCellCmp, ghostCellCmp + 1);

		// === Check Internal IDs ===
		BOOST_CHECK_EQUAL(mesh->getCellID(3), 0);
		BOOST_CHECK_EQUAL(mesh->getCellID(2), 1);

		// === Check Cell Data ===
		// Local Cell Data
		int nLocalGVerticesCmp[1] = {6};		// Global Vertex Counts
		int nLocalSVerticesCmp[1] = {6};		// Stored Vertex Counts
		int nLocalGFaceCmp[1] = {5};			// Global Face Counts per cell
		int nLocalSFaceCmp[1] = {5};			// Stored Face Counts per cell

		for(int i = 0; i < 1; i++)
		{
			BOOST_CHECK_EQUAL(mesh->getCellNVertices(mesh->getCellID(localCellCmp[i])),  nLocalGVerticesCmp[i]);
			BOOST_CHECK_EQUAL(mesh->getCellStoredNVertices(mesh->getCellID(localCellCmp[i])),  nLocalSVerticesCmp[i]);

			BOOST_CHECK_EQUAL(mesh->getCellNFaces(mesh->getCellID(localCellCmp[i])),  nLocalGFaceCmp[i]);
			BOOST_CHECK_EQUAL(mesh->getCellStoredNFaces(mesh->getCellID(localCellCmp[i])),  nLocalSFaceCmp[i]);
		}

		// Ghost Cell Data
		int nGhostGVerticesCmp[1] = {8};		// Global Vertex Counts
		int nGhostSVerticesCmp[1] = {4};		// Stored Vertex Counts
		int nGhostGFaceCmp[1] = {6};			// Global Face Counts per cell
		int nGhostSFaceCmp[1] = {1};			// Stored Face Counts per cell

		for(int i = 0; i < 1; i++)
		{
			BOOST_CHECK_EQUAL(mesh->getCellNVertices(mesh->getCellID(ghostCellCmp[i])),  nGhostGVerticesCmp[i]);
			BOOST_CHECK_EQUAL(mesh->getCellStoredNVertices(mesh->getCellID(ghostCellCmp[i])),  nGhostSVerticesCmp[i]);

			BOOST_CHECK_EQUAL(mesh->getCellNFaces(mesh->getCellID(ghostCellCmp[i])),  nGhostGFaceCmp[i]);
			BOOST_CHECK_EQUAL(mesh->getCellStoredNFaces(mesh->getCellID(ghostCellCmp[i])),  nGhostSFaceCmp[i]);
		}

		// Check Face Data

		// Check Vertex Data

		// Check Boundary Data

		// Check Region Data
	}
	else if(comm.rank == 3)
	{
		int localCellCmp[1] = {4};
		int localNodes[1];

		int ghostCellCmp[1] = {2};
		int ghostNodes[1];

		BOOST_CHECK_EQUAL(mesh->cellConnGraph->nLONodes, 1);
		BOOST_CHECK_EQUAL(mesh->cellConnGraph->nLGhNodes, 1);

		mesh->cellConnGraph->getLocalNodes(localNodes, 1);
		BOOST_CHECK_EQUAL_COLLECTIONS(localNodes, localNodes + 1, localCellCmp, localCellCmp + 1);

		mesh->cellConnGraph->getGhostNodes(ghostNodes, 1);
		BOOST_CHECK_EQUAL_COLLECTIONS(ghostNodes, ghostNodes + 1, ghostCellCmp, ghostCellCmp + 1);

		// === Check Internal IDs ===
		BOOST_CHECK_EQUAL(mesh->getCellID(4), 0);
		BOOST_CHECK_EQUAL(mesh->getCellID(2), 1);

		// === Check Cell Data ===
		// Local Cell Data
		int nLocalGVerticesCmp[1] = {8};		// Global Vertex Counts
		int nLocalSVerticesCmp[1] = {8};		// Stored Vertex Counts
		int nLocalGFaceCmp[1] = {6};			// Global Face Counts per cell
		int nLocalSFaceCmp[1] = {6};			// Stored Face Counts per cell

		for(int i = 0; i < 1; i++)
		{
			BOOST_CHECK_EQUAL(mesh->getCellNVertices(mesh->getCellID(localCellCmp[i])),  nLocalGVerticesCmp[i]);
			BOOST_CHECK_EQUAL(mesh->getCellStoredNVertices(mesh->getCellID(localCellCmp[i])),  nLocalSVerticesCmp[i]);

			BOOST_CHECK_EQUAL(mesh->getCellNFaces(mesh->getCellID(localCellCmp[i])),  nLocalGFaceCmp[i]);
			BOOST_CHECK_EQUAL(mesh->getCellStoredNFaces(mesh->getCellID(localCellCmp[i])),  nLocalSFaceCmp[i]);
		}

		// Ghost Cell Data
		int nGhostGVerticesCmp[1] = {8};		// Global Vertex Counts
		int nGhostSVerticesCmp[1] = {4};		// Stored Vertex Counts
		int nGhostGFaceCmp[1] = {6};			// Global Face Counts per cell
		int nGhostSFaceCmp[1] = {1};			// Stored Face Counts per cell

		for(int i = 0; i < 1; i++)
		{
			BOOST_CHECK_EQUAL(mesh->getCellNVertices(mesh->getCellID(ghostCellCmp[i])),  nGhostGVerticesCmp[i]);
			BOOST_CHECK_EQUAL(mesh->getCellStoredNVertices(mesh->getCellID(ghostCellCmp[i])),  nGhostSVerticesCmp[i]);

			BOOST_CHECK_EQUAL(mesh->getCellNFaces(mesh->getCellID(ghostCellCmp[i])),  nGhostGFaceCmp[i]);
			BOOST_CHECK_EQUAL(mesh->getCellStoredNFaces(mesh->getCellID(ghostCellCmp[i])),  nGhostSFaceCmp[i]);
		}

		// Check Face Data

		// Check Vertex Data

		// Check Boundary Data

		// Check Region Data
	}
}

// Test 2: Test correct build of a mesh from a structured gen format
BOOST_AUTO_TEST_CASE(buildUnstructuredMesh_test2, * utf::tolerance(0.00001))
{
	cupcfd::error::eCodes status;
    cupcfd::comm::Communicator comm(MPI_COMM_WORLD);

	// Setup the partitioner config
	// Use naive so the outcome is predictable
    cupcfd::partitioner::PartitionerNaiveConfig<int,int> naiveConfig;
	cupcfd::partitioner::PartitionerConfig<int,int> partConfig(naiveConfig);

	// Setup the source config
	MeshSourceStructGenConfig<int, double> meshSourceConfig(20, 20, 20, -1.0, 1.0, -1.0, 1.0, -1.0, 1.0);

	// Setup the config to use for building
	MeshConfig<int, double,int> meshConfig(partConfig, meshSourceConfig);

	CupCfdAoSMesh<int,double,int> * mesh;
	status = meshConfig.buildUnstructuredMesh(&mesh, comm);

	// ToDo: Check Mesh Values

	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
}

BOOST_AUTO_TEST_CASE(cleanup)
{
    MPI_Finalize();
}
