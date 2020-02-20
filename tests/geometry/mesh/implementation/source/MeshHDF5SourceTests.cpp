/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Tests for the MeshHDF5Source Class
 */

#define BOOST_TEST_MODULE MeshHDF5Source
#include <boost/test/unit_test.hpp>
#include <boost/test/output_test_stream.hpp>

#include <stdexcept>
#include <string>

#include "MeshHDF5Source.h"
#include "EuclideanPoint.h"
#include "EuclideanVector.h"

using namespace cupcfd::geometry::mesh;

namespace utf = boost::unit_test;

// Setup
BOOST_AUTO_TEST_CASE(setup)
{
    int argc = boost::unit_test::framework::master_test_suite().argc;
    char ** argv = boost::unit_test::framework::master_test_suite().argv;

    MPI_Init(&argc, &argv);
}

// === Constructors ===
// Test 1:
BOOST_AUTO_TEST_CASE(constructor_test1)
{

}

// === getAttribute int ===

// === getAttribute float ===

// === getAttribute double ===

// === getCellLabels ===
BOOST_AUTO_TEST_CASE(getCellLabels)
{
	cupcfd::comm::Communicator comm(MPI_COMM_WORLD);

	std::string filePath = "../tests/geometry/mesh/data/MeshHDF5.hdf5";
	MeshHDF5Source<int, double> file(filePath);
	cupcfd::error::eCodes status;

	int indices[4] = {0, 1, 2, 3};

	int cellLabels[4];
	int cellLabelsCmp[4] = {1, 2, 3, 4};

	status = file.getCellLabels(cellLabels, 4, indices, 4);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	BOOST_CHECK_EQUAL_COLLECTIONS(cellLabels, cellLabels + 4, cellLabelsCmp, cellLabelsCmp + 4);
}

// === getFaceLabels ===

// === getVertexLabels ===

// === getBoundaryLabels ===

// === getRegionLabels ===

// === getFaceArea ===

// === getFaceNVertices ===
BOOST_AUTO_TEST_CASE(getFaceNVertices_test1)
{
	cupcfd::comm::Communicator comm(MPI_COMM_WORLD);

	std::string filePath = "../tests/geometry/mesh/data/MeshHDF5.hdf5";
	MeshHDF5Source<int, double> file(filePath);
	cupcfd::error::eCodes status;

	int faceLabels[15] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};

	int nFaceVertices[15];
	int nFaceVerticesCmp[15] = {4, 4, 4, 4, 4, 4, 4, 4, 4, 3, 4, 3, 4, 4, 4};

	status = file.getFaceNVertices(nFaceVertices, 15, faceLabels, 15);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	BOOST_CHECK_EQUAL_COLLECTIONS(nFaceVertices, nFaceVertices + 15, nFaceVerticesCmp, nFaceVerticesCmp + 15);
}

// === getFaceVerticesLabelsCSR ===
// Test 1: Partial Read 1
BOOST_AUTO_TEST_CASE(getFaceVerticesLabelsCSR_test1)
{
	cupcfd::comm::Communicator comm(MPI_COMM_WORLD);

	std::string filePath = "../tests/geometry/mesh/data/MeshHDF5.hdf5";
	MeshHDF5Source<int, double> file(filePath);
	cupcfd::error::eCodes status;

	int faceLabels[15] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};

	int csrIndices[16];
	int csrData[58];

	int csrIndicesCmp[16] = {0, 4, 8, 12, 16, 20, 24, 28, 32, 36, 39, 43, 46, 50, 54, 58};

	int csrDataCmp[58] = {1, 5, 14, 11,
					  1, 2, 6, 5,
					  1, 2, 12, 11,
					  5, 6, 15, 14,
					  11, 12, 15, 14,
					  2, 3, 7, 6,
					  2, 3, 13, 12,
					  12, 13, 16, 15,
					  3, 4, 8, 7,
					  3, 4, 13,
					  4, 8, 16, 13,
					  7, 8, 16,
					  7, 10, 18, 16,
					  9, 10, 18, 17,
					  6, 9, 17, 15
	};

	status = file.getFaceVerticesLabelsCSR(csrIndices, 16, csrData, 58, faceLabels, 15);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	BOOST_CHECK_EQUAL_COLLECTIONS(csrIndices, csrIndices + 16, csrIndicesCmp, csrIndicesCmp + 16);
	BOOST_CHECK_EQUAL_COLLECTIONS(csrData, csrData + 58, csrDataCmp, csrDataCmp + 58);
}

// Test 2: Partial Read 2
BOOST_AUTO_TEST_CASE(getFaceVerticesLabelsCSR_test2)
{
	cupcfd::comm::Communicator comm(MPI_COMM_WORLD);

	std::string filePath = "../tests/geometry/mesh/data/MeshHDF5.hdf5";
	MeshHDF5Source<int, double> file(filePath);
	cupcfd::error::eCodes status;

	int faceLabels[12] = {15, 1, 2, 4, 5, 6, 8, 10, 11, 13, 14, 7};

	int csrIndices[13];
	int csrData[47];

	int csrIndicesCmp[16] = {0, 4, 8, 12, 16, 20, 24, 28, 31, 35, 39, 43, 47};

	int csrDataCmp[47] = { 6, 9, 17, 15,
					  1, 5, 14, 11,
					  1, 2, 6, 5,
					  5, 6, 15, 14,
					  11, 12, 15, 14,
					  2, 3, 7, 6,
					  12, 13, 16, 15,
					  3, 4, 13,
					  4, 8, 16, 13,
					  7, 10, 18, 16,
					  9, 10, 18, 17,
					  2, 3, 13, 12
	};

	status = file.getFaceVerticesLabelsCSR(csrIndices, 13, csrData, 48, faceLabels, 12);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	BOOST_CHECK_EQUAL_COLLECTIONS(csrIndices, csrIndices + 13, csrIndicesCmp, csrIndicesCmp + 13);
	BOOST_CHECK_EQUAL_COLLECTIONS(csrData, csrData + 47, csrDataCmp, csrDataCmp + 47);
}

// === getBoundaryVerticesLabelsCSR ===
// Partial Read 1
BOOST_AUTO_TEST_CASE(getBoundaryVerticesLabelsCSR_test1)
{
	cupcfd::comm::Communicator comm(MPI_COMM_WORLD);

	std::string filePath = "../tests/geometry/mesh/data/MeshHDF5.hdf5";
	MeshHDF5Source<int, double> file(filePath);
	cupcfd::error::eCodes status;

	int boundaryLabels[15] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};

	int csrIndices[16];
	int csrData[58];

	int csrIndicesCmp[16] = {0, 4, 8, 12, 16, 20, 24, 28, 32, 36, 39, 43, 47, 50, 54, 58};

	int csrDataCmp[58] = {1, 5, 14, 11,
					  1, 2, 6, 5,
					  1, 2, 12, 11,
					  5, 6, 15, 14,
					  11, 12, 15, 14,
					  2, 3, 7, 6,
					  2, 3, 13, 12,
					  12, 13, 16, 15,
					  3, 4, 8, 7,
					  3, 4, 13,
					  4, 8, 16, 13,
					  7, 10, 18, 16,
					  7, 8, 16,
					  9, 10, 18, 17,
					  15, 16, 18, 17
	};

	status = file.getBoundaryVerticesLabelsCSR(csrIndices, 16, csrData, 58, boundaryLabels, 15);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	BOOST_CHECK_EQUAL_COLLECTIONS(csrIndices, csrIndices + 16, csrIndicesCmp, csrIndicesCmp + 16);
	BOOST_CHECK_EQUAL_COLLECTIONS(csrData, csrData + 58, csrDataCmp, csrDataCmp + 58);
}

// === getCellCount ===
// Test 1:
BOOST_AUTO_TEST_CASE(getCellCount_test1)
{
	cupcfd::comm::Communicator comm(MPI_COMM_WORLD);

	std::string filePath = "../tests/geometry/mesh/data/MeshHDF5.hdf5";
	MeshHDF5Source<int, double> file(filePath);
	int cells;
	cupcfd::error::eCodes status;

	status = file.getCellCount(&cells);
	BOOST_CHECK_EQUAL(cells, 4);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
}

// === getFaceCount ===
// Test 1:
BOOST_AUTO_TEST_CASE(getFaceCount_test1)
{
	cupcfd::comm::Communicator comm(MPI_COMM_WORLD);

	std::string filePath = "../tests/geometry/mesh/data/MeshHDF5.hdf5";
	MeshHDF5Source<int, double> file(filePath);
	int faces;
	cupcfd::error::eCodes status;

	status = file.getFaceCount(&faces);
	BOOST_CHECK_EQUAL(faces, 20);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
}

// === getBoundaryCount ===
// Test 1:
BOOST_AUTO_TEST_CASE(getBoundaryCount_test1)
{
	cupcfd::comm::Communicator comm(MPI_COMM_WORLD);

	std::string filePath = "../tests/geometry/mesh/data/MeshHDF5.hdf5";
	MeshHDF5Source<int, double> file(filePath);
	int boundaries;
	cupcfd::error::eCodes status;

	status = file.getBoundaryCount(&boundaries);
	BOOST_CHECK_EQUAL(boundaries, 17);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
}

// === getRegionCount ===
// Test 1:
BOOST_AUTO_TEST_CASE(getRegionCount_test1)
{
	cupcfd::comm::Communicator comm(MPI_COMM_WORLD);

	std::string filePath = "../tests/geometry/mesh/data/MeshHDF5.hdf5";
	MeshHDF5Source<int, double> file(filePath);
	int regions;
	cupcfd::error::eCodes status;

	status = file.getRegionCount(&regions);
	BOOST_CHECK_EQUAL(regions, 1);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
}

// === getVertexCount ===
// Test 1:
BOOST_AUTO_TEST_CASE(getVertexCount_test1)
{
	cupcfd::comm::Communicator comm(MPI_COMM_WORLD);

	std::string filePath = "../tests/geometry/mesh/data/MeshHDF5.hdf5";
	MeshHDF5Source<int, double> file(filePath);
	int vertices;
	cupcfd::error::eCodes status;

	status = file.getVertexCount(&vertices);
	BOOST_CHECK_EQUAL(vertices, 18);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
}

// === getMaxFaceCount ===
// Test 1:
BOOST_AUTO_TEST_CASE(getMaxFaceCount_test1)
{
	cupcfd::comm::Communicator comm(MPI_COMM_WORLD);

	std::string filePath = "../tests/geometry/mesh/data/MeshHDF5.hdf5";
	MeshHDF5Source<int, double> file(filePath);
	int maxFaceCount;
	cupcfd::error::eCodes status;

	status = file.getMaxFaceCount(&maxFaceCount);
	BOOST_CHECK_EQUAL(maxFaceCount, 6);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
}

// === getMaxVertexCount ===
BOOST_AUTO_TEST_CASE(getMaxVertexCount_test1)
{
	cupcfd::comm::Communicator comm(MPI_COMM_WORLD);

	std::string filePath = "../tests/geometry/mesh/data/MeshHDF5.hdf5";
	MeshHDF5Source<int, double> file(filePath);
	int maxVertexCount;
	cupcfd::error::eCodes status;

	status = file.getMaxVertexCount(&maxVertexCount);
	BOOST_CHECK_EQUAL(maxVertexCount, 4);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
}

// === getCellNFaces ===
BOOST_AUTO_TEST_CASE(getCellNFaces_test1)
{
	cupcfd::comm::Communicator comm(MPI_COMM_WORLD);

	std::string filePath = "../tests/geometry/mesh/data/MeshHDF5.hdf5";
	MeshHDF5Source<int, double> file(filePath);
	cupcfd::error::eCodes status;

	// Cell Labels - Indexed from base of 1 for this format
	int nLabels[3] = {1,3,4};

	int nFaces[3];
	int nFacesCmp[3] = {6, 5, 6};

	status = file.getCellNFaces(nFaces, 3, nLabels, 3);
	BOOST_CHECK_EQUAL_COLLECTIONS(nFaces, nFaces + 3, nFacesCmp, nFacesCmp + 3);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
}

// === getCellVolume ===
BOOST_AUTO_TEST_CASE(getCellVolume_test1)
{
	cupcfd::comm::Communicator comm(MPI_COMM_WORLD);

	std::string filePath = "../tests/geometry/mesh/data/MeshHDF5.hdf5";
	MeshHDF5Source<int, double> file(filePath);
	cupcfd::error::eCodes status;

	int nLabels[3] = {1,3,4};

	double vol[3];
	int nVol = 3;
	double volCmp[4] = {1.0, 3.0, 4.0};

	status = file.getCellVolume(vol, nVol, nLabels, 3);
	BOOST_CHECK_EQUAL_COLLECTIONS(vol, vol + 4, volCmp, volCmp + 4);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
}

// === getCellCenter ===
BOOST_AUTO_TEST_CASE(getCellCenter_test1)
{

}

// === getCellFaceLabels ===
BOOST_AUTO_TEST_CASE(getCellFaceLabels_test1)
{
	cupcfd::comm::Communicator comm(MPI_COMM_WORLD);

	std::string filePath = "../tests/geometry/mesh/data/MeshHDF5.hdf5";
	MeshHDF5Source<int, double> file(filePath);
	cupcfd::error::eCodes status;

	int cellLabels[3] = {1, 3, 4};

	int faceLabelsInd[4];
	int faceLabelsIndCmp[4] = {0, 6, 11, 17};

	int faceLabelsData[17];
	int faceLabelsDataCmp[17] = {1, 2, 3, 4, 5, 18, 9, 10, 11, 12, 19, 13, 14, 15, 16, 17, 20};

	status = file.getCellFaceLabels(faceLabelsInd, 4, faceLabelsData, 17, cellLabels, 3);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	BOOST_CHECK_EQUAL_COLLECTIONS(faceLabelsInd, faceLabelsInd + 4, faceLabelsIndCmp, faceLabelsIndCmp + 4);
	BOOST_CHECK_EQUAL_COLLECTIONS(faceLabelsData, faceLabelsData + 17, faceLabelsDataCmp, faceLabelsDataCmp + 17);
}

// === getFaceIsBoundary ===
BOOST_AUTO_TEST_CASE(getFaceIsBoundary_test1)
{
	cupcfd::comm::Communicator comm(MPI_COMM_WORLD);

	std::string filePath = "../tests/geometry/mesh/data/MeshHDF5.hdf5";
	MeshHDF5Source<int, double> file(filePath);
	cupcfd::error::eCodes status;

	int faceLabels[20] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20};
	bool isBoundary[20];
	bool isBoundaryCmp[20] = {true, true, true, true, true, true, true, true, true, true,
							 true, true, true, true, true, true, true, false, false, false};

	status = file.getFaceIsBoundary(isBoundary, 20, faceLabels, 20);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	BOOST_CHECK_EQUAL_COLLECTIONS(isBoundary, isBoundary + 20, isBoundaryCmp, isBoundaryCmp + 20);
}

// === getFaceBoundaryLabels ===
// Test 1: Get Correct Boundary Labels for all valid faces
BOOST_AUTO_TEST_CASE(getFaceBoundaryLabels_test1)
{
	cupcfd::comm::Communicator comm(MPI_COMM_WORLD);

	std::string filePath = "../tests/geometry/mesh/data/MeshHDF5.hdf5";
	MeshHDF5Source<int, double> file(filePath);
	cupcfd::error::eCodes status;

	int faceLabels[17] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17};

	int boundaryLabels[17];
	int boundaryLabelsCmp[17] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 13, 12, 14, 17, 16, 15};

	status = file.getFaceBoundaryLabels(boundaryLabels, 17, faceLabels, 17);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	BOOST_CHECK_EQUAL_COLLECTIONS(boundaryLabels, boundaryLabels + 17, boundaryLabelsCmp, boundaryLabelsCmp + 17);
}

// Test 2: Error: One or more faces are not boundary faces
BOOST_AUTO_TEST_CASE(getFaceBoundaryLabels_test2)
{
	cupcfd::comm::Communicator comm(MPI_COMM_WORLD);

	std::string filePath = "../tests/geometry/mesh/data/MeshHDF5.hdf5";
	MeshHDF5Source<int, double> file(filePath);
	cupcfd::error::eCodes status;

	int faceLabels[20] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20};
	int boundaryLabels[20];

	status = file.getFaceBoundaryLabels(boundaryLabels, 20, faceLabels, 20);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_ERROR);
}

// === getFaceCell1Labels ===
BOOST_AUTO_TEST_CASE(getFaceCell1Labels_test1)
{
	cupcfd::comm::Communicator comm(MPI_COMM_WORLD);

	std::string filePath = "../tests/geometry/mesh/data/MeshHDF5.hdf5";
	MeshHDF5Source<int, double> file(filePath);
	cupcfd::error::eCodes status;

	int faceLabels[20] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20};
	int cellLabels[20];
	int cellLabelsCmp[20] = {1, 1, 1, 1, 1, 2, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4, 4, 1, 2, 2};

	status = file.getFaceCell1Labels(cellLabels, 20, faceLabels, 20);
	BOOST_CHECK_EQUAL_COLLECTIONS(cellLabels, cellLabels + 20, cellLabelsCmp, cellLabelsCmp + 20);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
}

// === getFaceCell2Labels ===
BOOST_AUTO_TEST_CASE(getFaceCell2Labels_test1)
{
	cupcfd::comm::Communicator comm(MPI_COMM_WORLD);

	std::string filePath = "../tests/geometry/mesh/data/MeshHDF5.hdf5";
	MeshHDF5Source<int, double> file(filePath);
	cupcfd::error::eCodes status;

	int faceLabels[20] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20};
	int cellLabels[20];
	int cellLabelsCmp[20] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 2, 3, 4};

	status = file.getFaceCell2Labels(cellLabels, 20, faceLabels, 20);
	BOOST_CHECK_EQUAL_COLLECTIONS(cellLabels, cellLabels + 20, cellLabelsCmp, cellLabelsCmp + 20);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
}

// === getFaceLambda ===
BOOST_AUTO_TEST_CASE(getFaceLambda2_test1)
{
	/*
	cupcfd::comm::Communicator comm(MPI_COMM_WORLD);

	std::string filePath = "../tests/geometry/mesh/data/MeshHDF5.hdf5";
	MeshHDF5Source<int, double> file(filePath);
	cupcfd::error::eCodes status;

	double faceLambda[20];
	double faceLambdaCmp[20] = {12.0, 3.2, 0.53, 634.0, -431.3, 23.351, 54.6352, 4246.2,
							 421.4, 2.53, 351.4, 23.0, 41.0, 0.0, 426.2, 42111.5, 2462.2,
							 14.3, 15.0, -252.2};

	status = file.getFaceLambda(faceLambda, 20);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	for(int i = 0; i < 20; i++)
	{
		// Allow some leeway for floating point tolerance
		BOOST_TEST(faceLambda[i] == faceLambdaCmp[i]);
	}
	*/
}

// === getFaceNormal ===
BOOST_AUTO_TEST_CASE(getFaceNormal_test1)
{
	/*
	cupcfd::comm::Communicator comm(MPI_COMM_WORLD);

	std::string filePath = "../tests/geometry/mesh/data/MeshHDF5.hdf5";
	MeshHDF5Source<int, double> file(filePath);
	cupcfd::error::eCodes status;

	cupcfd::geometry::euclidean::EuclideanVector<double,3> normal[20];
	cupcfd::geometry::euclidean::EuclideanVector<double,3> normalCmp[20];
	double normalCmpX[20] = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0, 11.0, 12.0, 13.0, 14.0, 15.0, 16.0, 17.0, 18.0, 19.0, 20.0};
	double normalCmpY[20] = {100.0, 101.0, 102.0, 103.0, 104.0, 105.5, 106.0, 107.0, 108.0, 109.0, 110.0, 111.0, 112.3, 113.0, 114.0, 115.0, 116.0, 117.0, 118.0, 119.0};
	double normalCmpZ[20] = {200.8, 201.0, 202.0, 203.0, 204.5, 205.0, 206.0, 207.0, 208.0, 209.0, 210.0, 211.1, 212.0, 213.0, 214.0, 215.0, 216.0, 217.0, 218.9, 219.0};

	for(int i = 0; i < 20; i++)
	{
		normalCmp[i] = cupcfd::geometry::euclidean::EuclideanVector<double,3>(normalCmpX[i], normalCmpY[i], normalCmpZ[i]);
	}

	status = file.getFaceNormal(normal, 20);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	for(int i = 0; i < 20; i++)
	{
		// Boost does not handle comparing EuclieanPoints in it's macro
		BOOST_TEST(normal[i].cmp[0] == normalCmp[i].cmp[0]);
		BOOST_TEST(normal[i].cmp[1] == normalCmp[i].cmp[1]);
		BOOST_TEST(normal[i].cmp[2] == normalCmp[i].cmp[2]);
	}
	*/
}

// === getFaceCenter ===
BOOST_AUTO_TEST_CASE(getFaceCenter_test1)
{

}

// === getVertexCoords ===
BOOST_AUTO_TEST_CASE(getVertexCoords_test1)
{

}

// === getBoundaryFaceLabels ===
BOOST_AUTO_TEST_CASE(getBoundaryFaceLabels_test1)
{

}

// === getBoundaryNVertices ===
BOOST_AUTO_TEST_CASE(getBoundaryNVertices_test1)
{
	cupcfd::comm::Communicator comm(MPI_COMM_WORLD);

	std::string filePath = "../tests/geometry/mesh/data/MeshHDF5.hdf5";
	MeshHDF5Source<int, double> file(filePath);
	cupcfd::error::eCodes status;

	// Note: Face 13 - > Boundary 12 and Face 12 -> Boundary 13.
	// This is to test that it is boundary labels being used to do lookup rather
	// than face labels since this will give values in different order
	int boundaryLabels[17] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17};

	int nVertices[17];
	int nVerticesCmp[17] = {4, 4, 4, 4, 4, 4, 4, 4, 4, 3, 4, 4, 3, 4, 4, 4, 4};

	status = file.getBoundaryNVertices(nVertices, 15, boundaryLabels, 15);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	BOOST_CHECK_EQUAL_COLLECTIONS(nVertices, nVertices + 15, nVerticesCmp, nVerticesCmp + 15);
}

// === getBoundaryRegionLabels ===
// Test 1:
BOOST_AUTO_TEST_CASE(getBoundaryRegionLabels_test1)
{

}

// === getBoundaryDistance ===
// Test 1:
BOOST_AUTO_TEST_CASE(getBoundaryDistance_test1)
{

}

// === getRegionName ===
// Test 1:
BOOST_AUTO_TEST_CASE(getRegionName_test1)
{

}


// === buildDistributedAdjacencyList1 ===
// Test: Build a arbitrary graph based on manually assigned cells
BOOST_AUTO_TEST_CASE(buildDistributedAdjacencyList1_test1)
{

}

// === buildDistributedAdjacencyList2 ===
// Test: Build a graph that has a naive cell allocation
BOOST_AUTO_TEST_CASE(buildDistributedAdjacencyList2_test1)
{

}

BOOST_AUTO_TEST_CASE(cleanup)
{
    MPI_Finalize();
}
