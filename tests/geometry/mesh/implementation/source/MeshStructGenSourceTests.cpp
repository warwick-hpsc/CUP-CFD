/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Tests for the MeshStructGenSource Class
 */

#define BOOST_TEST_MODULE MeshStructGenSource
#include <boost/test/unit_test.hpp>
#include <boost/test/output_test_stream.hpp>

#include <stdexcept>
#include <string>

#include "MeshStructGenSource.h"
#include "EuclideanPoint.h"
#include "EuclideanVector.h"
#include "EuclideanPoint.h"

#include "DistributedAdjacencyList.h"

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
	MeshStructGenSource<int, double> source(5, 5, 5, -1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
}

// === getCellLabels ===
// Test 1: Test Labels are correct
BOOST_AUTO_TEST_CASE(getCellLabels_test1)
{
	MeshStructGenSource<int, double> source(3, 3, 3, -1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
	cupcfd::error::eCodes status;

	int indices[27] = {0, 1, 2, 3, 4, 5, 6, 7, 8,
					 9, 10, 11, 12, 13, 14, 15, 16, 17,
					 18, 19, 20, 21, 22, 23, 24, 25, 26};
	int result[27];
	int cmp[27] = {0, 1, 2, 3, 4, 5, 6, 7, 8,
				 9, 10, 11, 12, 13, 14, 15, 16, 17,
				 18, 19, 20, 21, 22, 23, 24, 25, 26};

	status = source.getCellLabels(result, 27, indices, 27);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	BOOST_CHECK_EQUAL_COLLECTIONS(result, result+27, cmp, cmp+27);
}


// === getFaceLabels ===
// Test 1:
BOOST_AUTO_TEST_CASE(getFaceLabels_test1)
{

}

// === getVertexLabels ===
// Test 1:
BOOST_AUTO_TEST_CASE(getVertexLabels_test1)
{

}

// === getBoundaryLabels ===
// Test 1:
BOOST_AUTO_TEST_CASE(getBoundaryLabels_test1)
{

}

// === getRegionLabels ===
// Test 1:
BOOST_AUTO_TEST_CASE(getRegionLabels_test1)
{

}

// === calculateLabel ===
// Test 1:
BOOST_AUTO_TEST_CASE(calculateLabel_test1)
{

}

// === calculateXCoord ===
// Test 1: Compute the correct coordinate from a 0 minimum
BOOST_AUTO_TEST_CASE(calculateXCoord_test1)
{

}

// Test 2: Compute the correct coordinate from a negative minimum
BOOST_AUTO_TEST_CASE(calculateXCoord_test2)
{

}

// Test 3: Compute the correct coordinate from a positive minimum
BOOST_AUTO_TEST_CASE(calculateXCoord_test3)
{

}

// Test 4: Compute the correct coordinate for all cellLabels
BOOST_AUTO_TEST_CASE(calculateXCoord_test4)
{
	MeshStructGenSource<int, double> source(3, 4, 5, -1.0, 1.0, -1.0, 1.0, -1.0, 1.0);

	int cellLabels[60] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16,
				 	      17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32,
						  33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48,
						  49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59};

	int result[60];
	int cmp[60] = {0, 1, 2, 0, 1, 2, 0, 1, 2, 0, 1, 2, 0, 1, 2, 0, 1, 2, 0, 1, 2,
				   0, 1, 2, 0, 1, 2, 0, 1, 2, 0, 1, 2, 0, 1, 2, 0, 1, 2, 0, 1, 2,
				   0, 1, 2, 0, 1, 2, 0, 1, 2, 0, 1, 2, 0, 1, 2, 0, 1, 2};

	for(int i = 0; i < 60; i++)
	{
		result[i] = source.calculateXCoord(cellLabels[i], 0, 2);
	}

	BOOST_CHECK_EQUAL_COLLECTIONS(cmp, cmp + 60, result, result + 60);
}


// === calculateYCoord ===
// Test 1:
BOOST_AUTO_TEST_CASE(calculateYCoord_test1)
{

}

// Test 2: Compute the correct coordinate from a negative minimum
BOOST_AUTO_TEST_CASE(calculateYCoord_test2)
{

}

// Test 3: Compute the correct coordinate from a positive minimum
BOOST_AUTO_TEST_CASE(calculateYCoord_test3)
{

}

// Test 4: Compute the correct coordinate for all cellLabels
BOOST_AUTO_TEST_CASE(calculateYCoord_test4)
{
	MeshStructGenSource<int, double> source(3, 4, 5, -1.0, 1.0, -1.0, 1.0, -1.0, 1.0);

	int cellLabels[60] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16,
				 	      17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32,
						  33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48,
						  49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59};

	int result[60];
	int cmp[60] = {0, 0, 0, 1, 1, 1, 2, 2, 2, 3, 3, 3,
				   0, 0, 0, 1, 1, 1, 2, 2, 2, 3, 3, 3,
				   0, 0, 0, 1, 1, 1, 2, 2, 2, 3, 3, 3,
				   0, 0, 0, 1, 1, 1, 2, 2, 2, 3, 3, 3,
				   0, 0, 0, 1, 1, 1, 2, 2, 2, 3, 3, 3};

	for(int i = 0; i < 60; i++)
	{
		result[i] = source.calculateYCoord(cellLabels[i], 0, 2, 0, 3);
	}

	BOOST_CHECK_EQUAL_COLLECTIONS(cmp, cmp + 60, result, result + 60);
}

// === calculateZCoord ===
// Test 1:
BOOST_AUTO_TEST_CASE(calculateZCoord_test1)
{

}

// Test 2: Compute the correct coordinate from a negative minimum
BOOST_AUTO_TEST_CASE(calculateZCoord_test2)
{

}

// Test 3: Compute the correct coordinate from a positive minimum
BOOST_AUTO_TEST_CASE(calculateZCoord_test3)
{

}

// Test 4: Compute the correct coordinate for all cellLabels
BOOST_AUTO_TEST_CASE(calculateZCoord_test4)
{
	MeshStructGenSource<int, double> source(3, 4, 5, -1.0, 1.0, -1.0, 1.0, -1.0, 1.0);

	int cellLabels[60] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16,
				 	      17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32,
						  33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48,
						  49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59};

	int result[60];
	int cmp[60] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				   1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
				   2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
				   3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
				   4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4};

	for(int i = 0; i < 60; i++)
	{
		result[i] = source.calculateZCoord(cellLabels[i], 0, 2, 0, 3, 0, 4);
	}

	BOOST_CHECK_EQUAL_COLLECTIONS(cmp, cmp + 60, result, result + 60);
}

// === getCellCount ===
// Test 1: Get Correct Value
BOOST_AUTO_TEST_CASE(getCellCount_test1)
{
	MeshStructGenSource<int, double> source(5, 5, 5, -1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
	cupcfd::error::eCodes status;

	int cellCount;
	status = source.getCellCount(&cellCount);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	BOOST_CHECK_EQUAL(cellCount, 125);
}

// === getFaceCount ===
// Test 1: Get Correct Value
BOOST_AUTO_TEST_CASE(getFaceCount_test1)
{
	MeshStructGenSource<int, double> source(3, 4, 5, -1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
	cupcfd::error::eCodes status;

	int faceCount;
	status = source.getFaceCount(&faceCount);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	BOOST_CHECK_EQUAL(faceCount, 227);
}

// === getBoundaryCount ===
// Test 1: Get Correct Value
BOOST_AUTO_TEST_CASE(getBoundaryCount_test1)
{
	MeshStructGenSource<int, double> source(3, 4, 5, -1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
	cupcfd::error::eCodes status;

	int boundaryCount;
	status = source.getBoundaryCount(&boundaryCount);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	BOOST_CHECK_EQUAL(boundaryCount, 94);
}

// === getRegionCount ===
// Test 1: Get Correct Value
BOOST_AUTO_TEST_CASE(getRegionCount_test1)
{
	MeshStructGenSource<int, double> source(5, 5, 5, -1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
	cupcfd::error::eCodes status;

	int regionCount;
	status = source.getRegionCount(&regionCount);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	BOOST_CHECK_EQUAL(regionCount, 1);
}

// === getVertexCount ===
// Test 1: Get Correct Value
BOOST_AUTO_TEST_CASE(getVertexCount_test1)
{
	MeshStructGenSource<int, double> source(5, 5, 5, -1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
	cupcfd::error::eCodes status;

	int vertexCount;
	status = source.getVertexCount(&vertexCount);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	BOOST_CHECK_EQUAL(vertexCount, 216);
}

// === getMaxFaceCount ===
// Test 1: Get Correct Value
BOOST_AUTO_TEST_CASE(getMaxFaceCount_test1)
{
	MeshStructGenSource<int, double> source(5, 5, 5, -1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
	cupcfd::error::eCodes status;

	int maxFaceCount;
	status = source.getMaxFaceCount(&maxFaceCount);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	BOOST_CHECK_EQUAL(maxFaceCount, 6);
}

// === getMaxVertexCount ===
// Test 1: Get Correct Value
BOOST_AUTO_TEST_CASE(getMaxVertexCount_test1)
{
	MeshStructGenSource<int, double> source(5, 5, 5, -1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
	cupcfd::error::eCodes status;

	int maxVertexCount;
	status = source.getMaxVertexCount(&maxVertexCount);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	BOOST_CHECK_EQUAL(maxVertexCount, 4);
}

// === getCellNFaces ===
// Test 1:
BOOST_AUTO_TEST_CASE(getCellNFaces_test1)
{
	MeshStructGenSource<int, double> source(3, 4, 5, -1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
	cupcfd::error::eCodes status;

	int cellLabels[60] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16,
				 	      17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32,
						  33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48,
						  49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59};

	int result[60];
	int cmp[60] = {6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
				   6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
				   6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
				   6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
				   6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6};

	status = source.getCellNFaces(result, 60, cellLabels, 60);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	BOOST_CHECK_EQUAL_COLLECTIONS(cmp, cmp + 60, result, result + 60);
}

// === getCellVolume ===
// Test 1:
BOOST_AUTO_TEST_CASE(getCellVolume_test1)
{

}

// === getCellCenter ===
// Test 1:
BOOST_AUTO_TEST_CASE(getCellCenter_test1)
{
	MeshStructGenSource<int, double> source(2, 3, 2, -1.0, 1.0, -1.0, 2.0, 0.0, 2.0);
	cupcfd::error::eCodes status;

	int cellLabels[12] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};

	cupcfd::geometry::euclidean::EuclideanPoint<double, 3> result[12];
	cupcfd::geometry::euclidean::EuclideanPoint<double, 3> cmp[12] =
	{
		cupcfd::geometry::euclidean::EuclideanPoint<double, 3>(-0.5, -0.5 , 0.5),
		cupcfd::geometry::euclidean::EuclideanPoint<double, 3>(0.5 , -0.5 , 0.5),
		cupcfd::geometry::euclidean::EuclideanPoint<double, 3>(-0.5, 0.5  , 0.5),
		cupcfd::geometry::euclidean::EuclideanPoint<double, 3>(0.5 , 0.5  , 0.5),
		cupcfd::geometry::euclidean::EuclideanPoint<double, 3>(-0.5, 1.5  , 0.5),
		cupcfd::geometry::euclidean::EuclideanPoint<double, 3>(0.5 , 1.5  , 0.5),
		cupcfd::geometry::euclidean::EuclideanPoint<double, 3>(-0.5, -0.5 , 1.5),
		cupcfd::geometry::euclidean::EuclideanPoint<double, 3>(0.5 , -0.5 , 1.5),
		cupcfd::geometry::euclidean::EuclideanPoint<double, 3>(-0.5, 0.5  , 1.5),
		cupcfd::geometry::euclidean::EuclideanPoint<double, 3>(0.5 , 0.5  , 1.5),
		cupcfd::geometry::euclidean::EuclideanPoint<double, 3>(-0.5, 1.5  , 1.5),
		cupcfd::geometry::euclidean::EuclideanPoint<double, 3>(0.5 , 1.5  , 1.5),
	};

	status = source.getCellCenter(result, 12, cellLabels, 12);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	for(int i = 0; i < 12; i++)
	{
		BOOST_TEST(cmp[i].cmp[0] == result[i].cmp[0]);
		BOOST_TEST(cmp[i].cmp[1] == result[i].cmp[1]);
		BOOST_TEST(cmp[i].cmp[2] == result[i].cmp[2]);
	}
}

// === getCellFaceLabels ===
// Test 1:
BOOST_AUTO_TEST_CASE(getCellFaceLabels_test1)
{
	MeshStructGenSource<int, double> source(2, 3, 4, -1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
	cupcfd::error::eCodes status;

	int cellLabels[24] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23};

	// Expected number of face labels = 24 * 6
	int csrIndices[25];
	int csrData[144];

	int cmpIndices[25] = {0, 6, 12, 18, 24, 30, 36, 42, 48, 54, 60, 66, 72, 78, 84, 90, 96,
						  102, 108, 114, 120, 126, 132, 138, 144};

	// Result is face labels grouped by cell (ordered XLow, XHigh, YLow, YHigh, ZLow, ZHigh)
	int cmpData[144] = {0, 1, 36, 38, 68, 74,
					1, 2, 37, 39, 69, 75,
					3, 4, 38, 40, 70, 76,
					4, 5, 39, 41, 71, 77,
					6, 7, 40, 42, 72, 78,
					7, 8, 41, 43, 73, 79,
					9, 10, 44, 46, 74, 80,
					10, 11, 45, 47, 75, 81,
					12, 13, 46, 48, 76, 82,
					13, 14, 47, 49, 77, 83,
					15, 16, 48, 50, 78, 84,
					16, 17, 49, 51, 79, 85,
					18, 19, 52, 54, 80, 86,
					19, 20, 53, 55, 81, 87,
					21, 22, 54, 56, 82, 88,
					22, 23, 55, 57, 83, 89,
					24, 25, 56, 58, 84, 90,
					25, 26, 57, 59, 85, 91,
					27, 28, 60, 62, 86, 92,
					28, 29, 61, 63, 87, 93,
					30, 31, 62, 64, 88, 94,
					31, 32, 63, 65, 89, 95,
					33, 34, 64, 66, 90, 96,
					34, 35, 65, 67, 91, 97};

	status = source.getCellFaceLabels(csrIndices, 25, csrData, 144, cellLabels, 24);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	BOOST_CHECK_EQUAL_COLLECTIONS(csrIndices, csrIndices + 25, cmpIndices, cmpIndices + 25);
	BOOST_CHECK_EQUAL_COLLECTIONS(cmpData, cmpData + 144, csrData, csrData + 144);
}

// === getFaceIsBoundary ===
// Test 1:
BOOST_AUTO_TEST_CASE(getFaceIsBoundary_test1)
{
	MeshStructGenSource<int, double> source(2, 3, 4, -1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
	cupcfd::error::eCodes status;

	int faceLabels[98] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
						  10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
						  20, 21, 22, 23, 24, 25, 26, 27, 28, 29,
						  30, 31, 32, 33, 34, 35, 36, 37, 38, 39,
						  40, 41, 42, 43, 44, 45, 46, 47, 48, 49,
						  50, 51, 52, 53, 54, 55, 56, 57, 58, 59,
						  60, 61, 62, 63, 64, 65, 66, 67, 68, 69,
						  70, 71, 72, 73, 74, 75, 76, 77, 78, 79,
						  80, 81, 82, 83, 84, 85, 86, 87, 88, 89,
						  90, 91, 92, 93, 94, 95, 96, 97};

	bool result[98];

	bool cmp[98] = {true, false, true, true, false, true, true, false, true, true,
					false, true, true, false, true, true, false, true, true, false,
					true, true, false, true, true, false, true, true, false, true,
					true, false, true, true, false, true, true, true, false, false,
					false, false, true, true, true, true, false, false, false, false,
					true, true, true, true, false, false, false, false, true, true,
					true, true, false, false, false, false, true, true, true, true,
					true, true, true, true, false, false, false, false, false, false,
					false, false, false, false, false, false, false, false, false, false,
					false, false, true, true, true, true, true, true
	};

	status = source.getFaceIsBoundary(result, 98, faceLabels, 98);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	BOOST_CHECK_EQUAL_COLLECTIONS(cmp, cmp + 98, result, result + 98);
}

// === getFaceNVertices ===
// Test 1:
BOOST_AUTO_TEST_CASE(getFaceNVertices_test1)
{
	MeshStructGenSource<int, double> source(2, 3, 4, -1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
	cupcfd::error::eCodes status;

	int faceLabels[98] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
						  10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
						  20, 21, 22, 23, 24, 25, 26, 27, 28, 29,
						  30, 31, 32, 33, 34, 35, 36, 37, 38, 39,
						  40, 41, 42, 43, 44, 45, 46, 47, 48, 49,
						  50, 51, 52, 53, 54, 55, 56, 57, 58, 59,
						  60, 61, 62, 63, 64, 65, 66, 67, 68, 69,
						  70, 71, 72, 73, 74, 75, 76, 77, 78, 79,
						  80, 81, 82, 83, 84, 85, 86, 87, 88, 89,
						  90, 91, 92, 93, 94, 95, 96, 97};

	int result[98];

	int cmp[98] = {4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
				   4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
				   4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
				   4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
				   4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
				   4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
				   4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
				   4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
				   4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
				   4, 4, 4, 4, 4, 4, 4, 4};

	status = source.getFaceNVertices(result, 98, faceLabels, 98);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	BOOST_CHECK_EQUAL_COLLECTIONS(cmp, cmp + 98, result, result + 98);
}

// === getFaceBoundaryLabels ===
// Test 1: Get correct values
BOOST_AUTO_TEST_CASE(getFaceBoundaryLabels_test1)
{
	MeshStructGenSource<int, double> source(2, 3, 4, -1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
	cupcfd::error::eCodes status;

	int faceLabels[52] = {0, 2, 3, 5, 6, 8, 9, 11, 12, 14, 15, 17, 18, 20, 21, 23, 24,
						  26, 27, 29, 30, 32, 33, 35, 36, 37, 42, 43, 44, 45, 50, 51,
						  52, 53, 58, 59, 60, 61, 66, 67, 68, 69, 70, 71, 72, 73, 92,
						  93, 94, 95, 96, 97};

	int result[52];

	int cmp[52] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
				   11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26,
				   27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42,
				   43, 44, 45, 46, 47, 48, 49, 50, 51};

	status = source.getFaceBoundaryLabels(result, 52, faceLabels, 52);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	BOOST_CHECK_EQUAL_COLLECTIONS(cmp, cmp + 52, result, result + 52);
}

// Test 2: Error Case: One or more faces are not boundary faces
BOOST_AUTO_TEST_CASE(getFaceBoundaryLabels_test2)
{
	MeshStructGenSource<int, double> source(2, 3, 4, -1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
	cupcfd::error::eCodes status;

	int faceLabels[41] = {0, 1, 2, 3, 4, 5, 27, 30, 31, 32, 33, 34, 35,
						  36, 37, 42, 43, 44, 45, 50, 51, 52, 53, 58, 59, 60, 61, 66, 67,
						  68, 69, 70, 71, 72, 73, 92, 93, 94, 95, 96, 97};

	int result[41];

	status = source.getFaceBoundaryLabels(result, 41, faceLabels, 41);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_ERROR);
}

// === getFaceCell1Labels ===
// Test 1:
BOOST_AUTO_TEST_CASE(getFaceCell1Labels_test1)
{
	MeshStructGenSource<int, double> source(2, 3, 4, -1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
	cupcfd::error::eCodes status;

	int faceLabels[98] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
						  10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
						  20, 21, 22, 23, 24, 25, 26, 27, 28, 29,
						  30, 31, 32, 33, 34, 35, 36, 37, 38, 39,
						  40, 41, 42, 43, 44, 45, 46, 47, 48, 49,
						  50, 51, 52, 53, 54, 55, 56, 57, 58, 59,
						  60, 61, 62, 63, 64, 65, 66, 67, 68, 69,
						  70, 71, 72, 73, 74, 75, 76, 77, 78, 79,
						  80, 81, 82, 83, 84, 85, 86, 87, 88, 89,
						  90, 91, 92, 93, 94, 95, 96, 97};

	int result[98];

	int cmp[98] = {0, 0, 1, 2, 2, 3, 4, 4, 5, 6, 6, 7, 8, 8, 9,
				   10, 10, 11, 12, 12, 13, 14, 14, 15, 16, 16, 17,
				   18, 18, 19, 20, 20, 21, 22, 22, 23,
				   0, 1, 0, 1, 2, 3, 4, 5,
				   6, 7, 6, 7, 8, 9, 10, 11,
				   12, 13, 12, 13, 14, 15, 16, 17,
				   18, 19, 18, 19, 20, 21, 22, 23,
				   0, 1, 2, 3, 4, 5,
				   0, 1, 2, 3, 4, 5,
				   6, 7, 8, 9, 10, 11,
				   12, 13, 14, 15, 16, 17,
				   18, 19, 20, 21, 22, 23};

	status = source.getFaceCell1Labels(result, 98, faceLabels, 98);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	BOOST_CHECK_EQUAL_COLLECTIONS(cmp, cmp + 98, result, result + 98);
}

// === getFaceCell2Labels ===
// Test 1:
BOOST_AUTO_TEST_CASE(getFaceCell2Labels_test1)
{
	MeshStructGenSource<int, double> source(2, 3, 4, -1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
	cupcfd::error::eCodes status;

	int faceLabels[98] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
						  10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
						  20, 21, 22, 23, 24, 25, 26, 27, 28, 29,
						  30, 31, 32, 33, 34, 35, 36, 37, 38, 39,
						  40, 41, 42, 43, 44, 45, 46, 47, 48, 49,
						  50, 51, 52, 53, 54, 55, 56, 57, 58, 59,
						  60, 61, 62, 63, 64, 65, 66, 67, 68, 69,
						  70, 71, 72, 73, 74, 75, 76, 77, 78, 79,
						  80, 81, 82, 83, 84, 85, 86, 87, 88, 89,
						  90, 91, 92, 93, 94, 95, 96, 97};

	int result[98];

	int cmp[98] = {-1, 1, -1, -1, 3, -1, -1, 5, -1, -1, 7, -1, -1, 9, -1,
				   -1, 11, -1, -1, 13, -1, -1, 15, -1, -1, 17, -1,
				   -1, 19, -1, -1, 21, -1, -1, 23, -1,
				   -1, -1, 2, 3, 4, 5, -1, -1,
				   -1, -1, 8, 9, 10, 11, -1, -1,
				   -1, -1, 14, 15, 16, 17, -1, -1,
				   -1, -1, 20, 21, 22, 23, -1, -1,
				   -1, -1, -1, -1, -1, -1,
				   6, 7, 8, 9, 10, 11,
				   12, 13, 14, 15, 16, 17,
				   18, 19, 20, 21, 22, 23,
				   -1, -1, -1, -1, -1, -1};

	status = source.getFaceCell2Labels(result, 98, faceLabels, 98);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	BOOST_CHECK_EQUAL_COLLECTIONS(cmp, cmp + 98, result, result + 98);
}

// === getFaceLambda ===
// Test 1:
BOOST_AUTO_TEST_CASE(getFaceLambda_test1)
{

}

// === getFaceArea ===
// Test 1:
BOOST_AUTO_TEST_CASE(getFaceArea_test1)
{

}

// === getFaceNormal ===
// Test 1:
BOOST_AUTO_TEST_CASE(getFaceNormal_test1)
{

}

// === getFaceCenter ===
// Test 1:
BOOST_AUTO_TEST_CASE(getFaceCenter_test1)
{

}

// === getFaceVerticesLabels ===
// Test 1:
BOOST_AUTO_TEST_CASE(getFaceVerticesLabels_test1)
{

}

// === getFaceVerticesLabelsCSR ===
// Test 1:
BOOST_AUTO_TEST_CASE(getFaceVerticesLabelsCSR_test1)
{
	MeshStructGenSource<int, double> source(2, 3, 2, -1.0, 1.0, -1.0, 2.0, 0.0, 1.0);
	cupcfd::error::eCodes status;

	int faceLabels[52] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17,
						  18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33,
						  34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49,
						  50, 51};
	int csrIndices[53];
	int csrData[208];

	int cmpIndices[53] = {0, 4, 8, 12, 16, 20, 24, 28, 32, 36, 40, 44, 48, 52, 56, 60, 64,
						  68, 72, 76, 80, 84, 88, 92, 96, 100, 104, 108, 112, 116, 120,
						  124, 128, 132, 136, 140, 144, 148, 152, 156, 160, 164, 168, 172,
						  176, 180, 184, 188, 192, 196, 200, 204, 208};

	int cmpData[208] = {
			0, 3, 15, 12,
			1, 4, 16, 13,
			2, 5, 17, 14,
			3, 6, 18, 15,
			4, 7, 19, 16,
			5, 8, 20, 17,
			6, 9, 21, 18,
			7, 10, 22, 19,
			8, 11, 23, 20,
			12, 15, 27, 24,
			13, 16, 28, 25,
			14, 17, 29, 26,
			15, 18, 30, 27,
			16, 19, 31, 28,
			17, 20, 32, 29,
			18, 21, 33, 30,
			19, 22, 34, 31,
			20, 23, 35,	32, // End of YZ Faces
			0, 1, 13, 12,
			1, 2, 14, 13,
			3, 4, 16, 15,
			4, 5, 17, 16,
			6, 7, 19, 18,
			7, 8, 20, 19,
			9, 10, 22, 21,
			10, 11, 23, 22,
			12, 13, 25, 24,
			13, 14, 26, 25,
			15, 16, 28, 27,
			16, 17, 29, 28,
			18, 19, 31, 30,
			19, 20, 32, 31,
			21, 22, 34, 33,
			22, 23, 35, 34, // End of XZ faces
			0, 1, 4, 3,
			1, 2, 5, 4,
			3, 4, 7, 6,
			4, 5, 8, 7,
			6, 7, 10, 9,
			7, 8, 11, 10,
			12, 13, 16, 15,
			13, 14, 17, 16,
			15, 16, 19, 18,
			16, 17, 20, 19,
			18, 19, 22, 21,
			19, 20, 23, 22,
			24, 25, 28, 27,
			25, 26, 29, 28,
			27, 28, 31, 30,
			28, 29, 32, 31,
			30, 31, 34, 33,
			31, 32, 35, 34 // End of XY faces
	};

	status = source.getFaceVerticesLabelsCSR(csrIndices, 53, csrData, 208, faceLabels, 52);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	BOOST_CHECK_EQUAL_COLLECTIONS(csrIndices, csrIndices + 53, cmpIndices, cmpIndices + 53);
	BOOST_CHECK_EQUAL_COLLECTIONS(cmpData, cmpData + 208, csrData, csrData + 208);
}

// === getVertexCoords ===
// Test 1:
BOOST_AUTO_TEST_CASE(getVertexCoords_test1)
{
	MeshStructGenSource<int, double> source(2, 3, 2, -1.0, 1.0, -1.0, 2.0, 0.0, 1.0);
	cupcfd::error::eCodes status;

	int vertexLabels[36] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11,
							12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23,
							24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35};

	cupcfd::geometry::euclidean::EuclideanPoint<double, 3> result[36];
	cupcfd::geometry::euclidean::EuclideanPoint<double, 3> cmp[36] =
	{
		cupcfd::geometry::euclidean::EuclideanPoint<double, 3>(-1.0, -1.0 , 0.0),	// 0,0,0
		cupcfd::geometry::euclidean::EuclideanPoint<double, 3>(0.0, -1.0 , 0.0),	// 1,0,0
		cupcfd::geometry::euclidean::EuclideanPoint<double, 3>(1.0, -1.0 , 0.0),	// 2,0,0
		cupcfd::geometry::euclidean::EuclideanPoint<double, 3>(-1.0, 0.0 , 0.0),	// 0,1,0
		cupcfd::geometry::euclidean::EuclideanPoint<double, 3>(0.0, 0.0 , 0.0),	// 1,1,0
		cupcfd::geometry::euclidean::EuclideanPoint<double, 3>(1.0, 0.0 , 0.0),	// 2,1,0
		cupcfd::geometry::euclidean::EuclideanPoint<double, 3>(-1.0, 1.0 , 0.0),	// 0,2,0
		cupcfd::geometry::euclidean::EuclideanPoint<double, 3>(0.0, 1.0 , 0.0),	// 1,2,0
		cupcfd::geometry::euclidean::EuclideanPoint<double, 3>(1.0, 1.0 , 0.0),	// 2,2,0
		cupcfd::geometry::euclidean::EuclideanPoint<double, 3>(-1.0, 2.0 , 0.0),	// 0,3,0
		cupcfd::geometry::euclidean::EuclideanPoint<double, 3>(0.0, 2.0 , 0.0),	// 1,3,0
		cupcfd::geometry::euclidean::EuclideanPoint<double, 3>(1.0, 2.0 , 0.0),	// 2,3,0
		cupcfd::geometry::euclidean::EuclideanPoint<double, 3>(-1.0, -1.0 , 0.5),	// 0,0,1
		cupcfd::geometry::euclidean::EuclideanPoint<double, 3>(0.0, -1.0 , 0.5),	// 1,0,1
		cupcfd::geometry::euclidean::EuclideanPoint<double, 3>(1.0, -1.0 , 0.5),	// 2,0,1
		cupcfd::geometry::euclidean::EuclideanPoint<double, 3>(-1.0, 0.0 , 0.5),	// 0,1,1
		cupcfd::geometry::euclidean::EuclideanPoint<double, 3>(0.0, 0.0 , 0.5),	// 1,1,1
		cupcfd::geometry::euclidean::EuclideanPoint<double, 3>(1.0, 0.0 , 0.5),	// 2,1,1
		cupcfd::geometry::euclidean::EuclideanPoint<double, 3>(-1.0, 1.0 , 0.5),	// 0,2,1
		cupcfd::geometry::euclidean::EuclideanPoint<double, 3>(0.0, 1.0 , 0.5),	// 1,2,1
		cupcfd::geometry::euclidean::EuclideanPoint<double, 3>(1.0, 1.0 , 0.5),	// 2,2,1
		cupcfd::geometry::euclidean::EuclideanPoint<double, 3>(-1.0, 2.0 , 0.5),	// 0,3,1
		cupcfd::geometry::euclidean::EuclideanPoint<double, 3>(0.0, 2.0 , 0.5),	// 1,3,1
		cupcfd::geometry::euclidean::EuclideanPoint<double, 3>(1.0, 2.0 , 0.5),	// 2,3,1
		cupcfd::geometry::euclidean::EuclideanPoint<double, 3>(-1.0, -1.0 , 1.0),	// 0,0,2
		cupcfd::geometry::euclidean::EuclideanPoint<double, 3>(0.0, -1.0 , 1.0),	// 1,0,2
		cupcfd::geometry::euclidean::EuclideanPoint<double, 3>(1.0, -1.0 , 1.0),	// 2,0,2
		cupcfd::geometry::euclidean::EuclideanPoint<double, 3>(-1.0, 0.0 , 1.0),	// 0,1,2
		cupcfd::geometry::euclidean::EuclideanPoint<double, 3>(0.0, 0.0 , 1.0),	// 1,1,2
		cupcfd::geometry::euclidean::EuclideanPoint<double, 3>(1.0, 0.0 , 1.0),	// 2,1,2
		cupcfd::geometry::euclidean::EuclideanPoint<double, 3>(-1.0, 1.0 , 1.0),	// 0,2,2
		cupcfd::geometry::euclidean::EuclideanPoint<double, 3>(0.0, 1.0 , 1.0),	// 1,2,2
		cupcfd::geometry::euclidean::EuclideanPoint<double, 3>(1.0, 1.0 , 1.0),	// 2,2,2
		cupcfd::geometry::euclidean::EuclideanPoint<double, 3>(-1.0, 2.0 , 1.0),	// 0,3,2
		cupcfd::geometry::euclidean::EuclideanPoint<double, 3>(0.0, 2.0 , 1.0),	// 1,3,2
		cupcfd::geometry::euclidean::EuclideanPoint<double, 3>(1.0, 2.0 , 1.0),	// 2,3,2
	};

	status = source.getVertexCoords(result, 36, vertexLabels, 36);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	for(int i = 0; i < 36; i++)
	{
		BOOST_TEST(cmp[i].cmp[0] == result[i].cmp[0]);
		BOOST_TEST(cmp[i].cmp[1] == result[i].cmp[1]);
		BOOST_TEST(cmp[i].cmp[2] == result[i].cmp[2]);
	}
}

// === getBoundaryFaceLabels ===
// Test 1:
BOOST_AUTO_TEST_CASE(getBoundaryFaceLabels_test1)
{
	MeshStructGenSource<int, double> source(2, 3, 4, -1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
	cupcfd::error::eCodes status;

	int boundaryLabels[52] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
				   11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26,
				   27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42,
				   43, 44, 45, 46, 47, 48, 49, 50, 51};

	int result[52];

	int cmp[52] = {0, 2, 3, 5, 6, 8, 9, 11, 12, 14, 15, 17, 18, 20, 21, 23, 24,
						  26, 27, 29, 30, 32, 33, 35, 36, 37, 42, 43, 44, 45, 50, 51,
						  52, 53, 58, 59, 60, 61, 66, 67, 68, 69, 70, 71, 72, 73, 92,
						  93, 94, 95, 96, 97};

	status = source.getBoundaryFaceLabels(result, 52, boundaryLabels, 52);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	BOOST_CHECK_EQUAL_COLLECTIONS(cmp, cmp + 52, result, result + 52);
}

// === getBoundaryNVertices ===
// Test 1:
BOOST_AUTO_TEST_CASE(getBoundaryNVertices_test1)
{
	MeshStructGenSource<int, double> source(2, 3, 4, -1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
	cupcfd::error::eCodes status;

	int boundaryLabels[40] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
						  10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
						  20, 21, 22, 23, 24, 25, 26, 27, 28, 29,
						  30, 31, 32, 33, 34, 35, 36, 37, 38, 39};

	int result[40];

	int cmp[40] = {4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
				   4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
				   4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
				   4, 4, 4, 4, 4, 4, 4, 4, 4, 4};

	status = source.getBoundaryNVertices(result, 40, boundaryLabels, 40);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	BOOST_CHECK_EQUAL_COLLECTIONS(cmp, cmp + 40, result, result + 40);
}

// === getBoundaryRegionLabels ===
// Test 1:
BOOST_AUTO_TEST_CASE(getBoundaryRegionLabels_test1)
{
	MeshStructGenSource<int, double> source(2, 3, 4, -1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
	cupcfd::error::eCodes status;

	int boundaryLabels[40] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
						  10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
						  20, 21, 22, 23, 24, 25, 26, 27, 28, 29,
						  30, 31, 32, 33, 34, 35, 36, 37, 38, 39};

	int result[40];

	int cmp[40] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				   0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				   0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				   0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

	status = source.getBoundaryRegionLabels(result, 40, boundaryLabels, 40);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	BOOST_CHECK_EQUAL_COLLECTIONS(cmp, cmp + 40, result, result + 40);
}

// === getBoundaryVerticesLabelsCSR ===
// Test 1:
BOOST_AUTO_TEST_CASE(getBoundaryVerticesLabelsCSR_test1)
{
	MeshStructGenSource<int, double> source(2, 3, 2, -1.0, 1.0, -1.0, 2.0, 0.0, 1.0);
	cupcfd::error::eCodes status;

	int boundaryLabels[32] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17,
						  18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31};
	int csrIndices[33];
	int csrData[128];

	int cmpIndices[33] = {0, 4, 8, 12, 16, 20, 24, 28, 32, 36, 40, 44, 48, 52, 56, 60, 64,
						  68, 72, 76, 80, 84, 88, 92, 96, 100, 104, 108, 112, 116, 120,
						  124, 128};

	int cmpData[128] = {
			0, 3, 15, 12,
			2, 5, 17, 14,
			3, 6, 18, 15,
			5, 8, 20, 17,
			6, 9, 21, 18,
			8, 11, 23, 20,
			12, 15, 27, 24,
			14, 17, 29, 26,
			15, 18, 30, 27,
			17, 20, 32, 29,
			18, 21, 33, 30,
			20, 23, 35, 32,	// End of YZ Boundaries
			0, 1, 13, 12,
			1, 2, 14, 13,
			9, 10, 22, 21,
			10, 11, 23, 22,
			12, 13, 25, 24,
			13, 14, 26, 25,
			21, 22, 34, 33,
			22, 23, 35, 34, // End of XZ Boundaries
			0, 1, 4, 3,
			1, 2, 5, 4,
			3, 4, 7, 6,
			4, 5, 8, 7,
			6, 7, 10, 9,
			7, 8, 11, 10,
			24, 25, 28, 27,
			25, 26, 29, 28,
			27, 28, 31, 30,
			28, 29, 32, 31,
			30, 31, 34, 33,
			31, 32, 35, 34 // End of XY Boundaries
	};

	status = source.getBoundaryVerticesLabelsCSR(csrIndices, 33, csrData, 128, boundaryLabels, 32);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	BOOST_CHECK_EQUAL_COLLECTIONS(csrIndices, csrIndices + 33, cmpIndices, cmpIndices + 33);
	BOOST_CHECK_EQUAL_COLLECTIONS(cmpData, cmpData + 128, csrData, csrData + 128);
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

	MeshStructGenSource<int, double> source(2, 3, 4, -1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
	cupcfd::error::eCodes status;

	int regionLabels[1] = {0};
	std::string regionNames[1];
	std::string cmp[1] = {"Default"};

	status = source.getRegionName(regionNames, 1, regionLabels, 1);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	BOOST_TEST(cmp[0] == regionNames[0]);
}



// === buildDistributedAdjacencyList1 ===
// Test: Build a arbitrary graph based on manually assigned cells using a Structured Grid Source
BOOST_AUTO_TEST_CASE(buildDistributedAdjacencyList1_test1)
{
	cupcfd::comm::Communicator comm(MPI_COMM_WORLD);
	cupcfd::geometry::mesh::MeshStructGenSource<int, double> source(2, 3, 4, -1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
	cupcfd::error::eCodes status;

	// Assign arbitrary range of cells to each process
	// 24/4 = 6 each
	int indices[6];

	if(comm.rank == 0)
	{
		indices[0] = 0; indices[1] = 1; indices[2] = 2; indices[3] = 3; indices[4] = 4; indices[5] = 5;
	}
	else if(comm.rank == 1)
	{
		indices[0] = 6; indices[1] = 7; indices[2] = 8; indices[3] = 9; indices[4] = 10; indices[5] = 11;
	}
	else if(comm.rank == 2)
	{
		indices[0] = 12; indices[1] = 13; indices[2] = 14; indices[3] = 15; indices[4] = 16; indices[5] = 17;
	}
	else if(comm.rank == 3)
	{
		indices[0] = 18; indices[1] = 19; indices[2] = 20; indices[3] = 21; indices[4] = 22; indices[5] = 23;
	}

	// Get Labels from the source
	int cellLabels[6];
	status = source.getCellLabels(cellLabels, 6, indices, 6);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	cupcfd::data_structures::DistributedAdjacencyList<int,int> * graph;
	status = source.buildDistributedAdjacencyList(&graph, comm, cellLabels, 6);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	// Check Local Nodes
	if(comm.rank == 0)
	{
		int localNodes[6];
		int localNodesCmp[6] = {0, 1, 2, 3, 4, 5};
		status = graph->getLocalNodes(localNodes, 6);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
		BOOST_CHECK_EQUAL_COLLECTIONS(localNodes, localNodes + 6, localNodesCmp, localNodesCmp + 6);
	}
	else if(comm.rank == 1)
	{
		int localNodes[6];
		int localNodesCmp[6] = {6, 7, 8, 9, 10, 11};
		status = graph->getLocalNodes(localNodes, 6);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
		BOOST_CHECK_EQUAL_COLLECTIONS(localNodes, localNodes + 6, localNodesCmp, localNodesCmp + 6);
	}
	else if(comm.rank == 2)
	{
		int localNodes[6];
		int localNodesCmp[6] = {12, 13, 14, 15, 16, 17};
		status = graph->getLocalNodes(localNodes, 6);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
		BOOST_CHECK_EQUAL_COLLECTIONS(localNodes, localNodes + 6, localNodesCmp, localNodesCmp + 6);
	}
	else if(comm.rank == 3)
	{
		int localNodes[6];
		int localNodesCmp[6] = {18, 19, 20, 21, 22, 23};
		status = graph->getLocalNodes(localNodes, 6);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
		BOOST_CHECK_EQUAL_COLLECTIONS(localNodes, localNodes + 6, localNodesCmp, localNodesCmp + 6);
	}

	// Check Ghost Nodes
	if(comm.rank == 0)
	{
		int ghostNodes[6];
		int ghostNodesCmp[6] = {6, 7, 8, 9, 10, 11};
		status = graph->getGhostNodes(ghostNodes, 6);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
		BOOST_CHECK_EQUAL_COLLECTIONS(ghostNodes, ghostNodes + 6, ghostNodesCmp, ghostNodesCmp + 6);
	}
	else if(comm.rank == 1)
	{
		int ghostNodes[12];
		int ghostNodesCmp[12] = {0, 1, 2, 3, 4, 5, 12, 13, 14, 15, 16, 17};
		status = graph->getGhostNodes(ghostNodes, 12);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
		BOOST_CHECK_EQUAL_COLLECTIONS(ghostNodes, ghostNodes + 12, ghostNodesCmp, ghostNodesCmp + 12);
	}
	else if(comm.rank == 2)
	{
		int ghostNodes[12];
		int ghostNodesCmp[12] = {6, 7, 8, 9, 10, 11, 18, 19, 20, 21, 22, 23};
		status = graph->getGhostNodes(ghostNodes, 12);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
		BOOST_CHECK_EQUAL_COLLECTIONS(ghostNodes, ghostNodes + 12, ghostNodesCmp, ghostNodesCmp + 12);
	}
	else if(comm.rank == 3)
	{
		int ghostNodes[6];
		int ghostNodesCmp[6] = {12, 13, 14, 15, 16, 17};
		status = graph->getGhostNodes(ghostNodes, 6);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
		BOOST_CHECK_EQUAL_COLLECTIONS(ghostNodes, ghostNodes + 6, ghostNodesCmp, ghostNodesCmp + 6);
	}

	// Check Edges
	if(comm.rank == 0)
	{
		int edgeCell1[20] = {0, 0, 0, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 4, 4, 4, 5, 5, 5};
		int edgeCell2[20] = {1, 2, 6, 0, 3, 7, 0, 3, 4, 8, 1, 2, 5, 9, 2, 5, 10,3, 4, 11};

		for(int i = 0; i < 20; i++)
		{
			bool check = false;
			status = graph->existsEdge(edgeCell1[i], edgeCell2[i], &check);
			BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
			BOOST_CHECK_EQUAL(check, true);
		}
	}
	else if(comm.rank == 1)
	{
		int edgeCell1[26] = {6, 6, 6, 6, 7, 7, 7, 7, 8, 8, 8, 8, 8, 9, 9, 9, 9, 9, 10, 10, 10, 10, 11, 11, 11, 11};
		int edgeCell2[26] = {7, 8, 0, 12, 6, 9, 1, 13, 6, 9, 10, 2, 14, 7, 8, 11, 3, 15, 8, 11, 4, 16, 9, 10, 5, 17};

		for(int i = 0; i < 26; i++)
		{
			bool check = false;
			status = graph->existsEdge(edgeCell1[i], edgeCell2[i], &check);
			BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
			BOOST_CHECK_EQUAL(check, true);
		}
	}
	else if(comm.rank == 2)
	{
		int edgeCell1[26] = {12, 12, 12, 12, 13, 13, 13, 13, 14, 14, 14, 14, 14, 15, 15, 15, 15, 15, 16, 16, 16, 16, 17, 17, 17, 17};
		int edgeCell2[26] = {13, 14, 6, 18, 12, 15, 7, 19, 12, 15, 16, 8, 20, 13, 14, 17, 9, 21, 14, 17, 10, 22, 15, 16, 11, 23};

		for(int i = 0; i < 26; i++)
		{
			bool check = false;
			status = graph->existsEdge(edgeCell1[i], edgeCell2[i], &check);
			BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
			BOOST_CHECK_EQUAL(check, true);
		}
	}
	else if(comm.rank == 3)
	{
		int edgeCell1[20] = {18, 18, 18, 19, 19, 19, 20, 20, 20, 20, 21, 21, 21, 21, 22, 22, 22, 23, 23, 23};
		int edgeCell2[20] = {19, 20, 12, 18, 21, 13, 18, 21, 22, 14, 19, 20, 23, 15, 20, 23, 16, 21, 22, 17};

		for(int i = 0; i < 20; i++)
		{
			bool check = false;
			status = graph->existsEdge(edgeCell1[i], edgeCell2[i], &check);
			BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
			BOOST_CHECK_EQUAL(check, true);
		}
	}
}



// === buildDistributedAdjacencyList2 ===
// Test 1: Build a graph based on the naive cell allocation method using a Structured Grid Source
BOOST_AUTO_TEST_CASE(buildDistributedAdjacencyList2_test1)
{
	cupcfd::comm::Communicator comm(MPI_COMM_WORLD);
	cupcfd::geometry::mesh::MeshStructGenSource<int, double> source(2, 3, 4, -1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
	cupcfd::error::eCodes status;

	cupcfd::data_structures::DistributedAdjacencyList<int,int> * graph;
	status = source.buildDistributedAdjacencyList(&graph, comm);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	// Check Local Nodes
	if(comm.rank == 0)
	{
		int localNodes[6];
		int localNodesCmp[6] = {0, 1, 2, 3, 4, 5};
		status = graph->getLocalNodes(localNodes, 6);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
		BOOST_CHECK_EQUAL_COLLECTIONS(localNodes, localNodes + 6, localNodesCmp, localNodesCmp + 6);
	}
	else if(comm.rank == 1)
	{
		int localNodes[6];
		int localNodesCmp[6] = {6, 7, 8, 9, 10, 11};
		status = graph->getLocalNodes(localNodes, 6);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
		BOOST_CHECK_EQUAL_COLLECTIONS(localNodes, localNodes + 6, localNodesCmp, localNodesCmp + 6);
	}
	else if(comm.rank == 2)
	{
		int localNodes[6];
		int localNodesCmp[6] = {12, 13, 14, 15, 16, 17};
		status = graph->getLocalNodes(localNodes, 6);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
		BOOST_CHECK_EQUAL_COLLECTIONS(localNodes, localNodes + 6, localNodesCmp, localNodesCmp + 6);
	}
	else if(comm.rank == 3)
	{
		int localNodes[6];
		int localNodesCmp[6] = {18, 19, 20, 21, 22, 23};
		status = graph->getLocalNodes(localNodes, 6);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
		BOOST_CHECK_EQUAL_COLLECTIONS(localNodes, localNodes + 6, localNodesCmp, localNodesCmp + 6);
	}

	// Check Ghost Nodes
	if(comm.rank == 0)
	{
		int ghostNodes[6];
		int ghostNodesCmp[6] = {6, 7, 8, 9, 10, 11};
		status = graph->getGhostNodes(ghostNodes, 6);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
		BOOST_CHECK_EQUAL_COLLECTIONS(ghostNodes, ghostNodes + 6, ghostNodesCmp, ghostNodesCmp + 6);
	}
	else if(comm.rank == 1)
	{
		int ghostNodes[12];
		int ghostNodesCmp[12] = {0, 1, 2, 3, 4, 5, 12, 13, 14, 15, 16, 17};
		status = graph->getGhostNodes(ghostNodes, 12);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
		BOOST_CHECK_EQUAL_COLLECTIONS(ghostNodes, ghostNodes + 12, ghostNodesCmp, ghostNodesCmp + 12);
	}
	else if(comm.rank == 2)
	{
		int ghostNodes[12];
		int ghostNodesCmp[12] = {6, 7, 8, 9, 10, 11, 18, 19, 20, 21, 22, 23};
		status = graph->getGhostNodes(ghostNodes, 12);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
		BOOST_CHECK_EQUAL_COLLECTIONS(ghostNodes, ghostNodes + 12, ghostNodesCmp, ghostNodesCmp + 12);
	}
	else if(comm.rank == 3)
	{
		int ghostNodes[6];
		int ghostNodesCmp[6] = {12, 13, 14, 15, 16, 17};
		status = graph->getGhostNodes(ghostNodes, 6);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
		BOOST_CHECK_EQUAL_COLLECTIONS(ghostNodes, ghostNodes + 6, ghostNodesCmp, ghostNodesCmp + 6);
	}

	// Check Edges
	if(comm.rank == 0)
	{
		int edgeCell1[20] = {0, 0, 0, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 4, 4, 4, 5, 5, 5};
		int edgeCell2[20] = {1, 2, 6, 0, 3, 7, 0, 3, 4, 8, 1, 2, 5, 9, 2, 5, 10,3, 4, 11};

		for(int i = 0; i < 20; i++)
		{
			bool check = false;
			status = graph->existsEdge(edgeCell1[i], edgeCell2[i], &check);
			BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
			BOOST_CHECK_EQUAL(check, true);
		}
	}
	else if(comm.rank == 1)
	{
		int edgeCell1[26] = {6, 6, 6, 6, 7, 7, 7, 7, 8, 8, 8, 8, 8, 9, 9, 9, 9, 9, 10, 10, 10, 10, 11, 11, 11, 11};
		int edgeCell2[26] = {7, 8, 0, 12, 6, 9, 1, 13, 6, 9, 10, 2, 14, 7, 8, 11, 3, 15, 8, 11, 4, 16, 9, 10, 5, 17};

		for(int i = 0; i < 26; i++)
		{
			bool check = false;
			status = graph->existsEdge(edgeCell1[i], edgeCell2[i], &check);
			BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
			BOOST_CHECK_EQUAL(check, true);
		}
	}
	else if(comm.rank == 2)
	{
		int edgeCell1[26] = {12, 12, 12, 12, 13, 13, 13, 13, 14, 14, 14, 14, 14, 15, 15, 15, 15, 15, 16, 16, 16, 16, 17, 17, 17, 17};
		int edgeCell2[26] = {13, 14, 6, 18, 12, 15, 7, 19, 12, 15, 16, 8, 20, 13, 14, 17, 9, 21, 14, 17, 10, 22, 15, 16, 11, 23};

		for(int i = 0; i < 26; i++)
		{
			bool check = false;
			status = graph->existsEdge(edgeCell1[i], edgeCell2[i], &check);
			BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
			BOOST_CHECK_EQUAL(check, true);
		}
	}
	else if(comm.rank == 3)
	{
		int edgeCell1[20] = {18, 18, 18, 19, 19, 19, 20, 20, 20, 20, 21, 21, 21, 21, 22, 22, 22, 23, 23, 23};
		int edgeCell2[20] = {19, 20, 12, 18, 21, 13, 18, 21, 22, 14, 19, 20, 23, 15, 20, 23, 16, 21, 22, 17};

		for(int i = 0; i < 20; i++)
		{
			bool check = false;
			status = graph->existsEdge(edgeCell1[i], edgeCell2[i], &check);
			BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
			BOOST_CHECK_EQUAL(check, true);
		}
	}
}

/*
// Test 2: Build a graph based on the naive cell allocation method using a Structured Grid Source
BOOST_AUTO_TEST_CASE(buildDistributedAdjacencyList2_test2)
{
	cupcfd::comm::Communicator comm(MPI_COMM_WORLD);
	cupcfd::geometry::mesh::MeshStructGenSource<int, double> source(5, 5, 5, -1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
	cupcfd::error::eCodes status;

	cupcfd::data_structures::DistributedAdjacencyList<int,int> * graph;
	status = source.buildDistributedAdjacencyList(&graph, comm);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	std::cout << "Number of local nodes:" << graph->nLONodes << "\n";

	// Check Local Nodes
	if(comm.rank == 0)
	{
		int localNodes[6];
		int localNodesCmp[6] = {0, 1, 2, 3, 4, 5};
		status = graph->getLocalNodes(localNodes, 6);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
		BOOST_CHECK_EQUAL_COLLECTIONS(localNodes, localNodes + 6, localNodesCmp, localNodesCmp + 6);
	}
	else if(comm.rank == 1)
	{
		int localNodes[6];
		int localNodesCmp[6] = {6, 7, 8, 9, 10, 11};
		status = graph->getLocalNodes(localNodes, 6);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
		BOOST_CHECK_EQUAL_COLLECTIONS(localNodes, localNodes + 6, localNodesCmp, localNodesCmp + 6);
	}
	else if(comm.rank == 2)
	{
		int localNodes[6];
		int localNodesCmp[6] = {12, 13, 14, 15, 16, 17};
		status = graph->getLocalNodes(localNodes, 6);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
		BOOST_CHECK_EQUAL_COLLECTIONS(localNodes, localNodes + 6, localNodesCmp, localNodesCmp + 6);
	}
	else if(comm.rank == 3)
	{
		int localNodes[6];
		int localNodesCmp[6] = {18, 19, 20, 21, 22, 23};
		status = graph->getLocalNodes(localNodes, 6);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
		BOOST_CHECK_EQUAL_COLLECTIONS(localNodes, localNodes + 6, localNodesCmp, localNodesCmp + 6);
	}

	// Check Ghost Nodes
	if(comm.rank == 0)
	{
		int ghostNodes[6];
		int ghostNodesCmp[6] = {6, 7, 8, 9, 10, 11};
		status = graph->getGhostNodes(ghostNodes, 6);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
		BOOST_CHECK_EQUAL_COLLECTIONS(ghostNodes, ghostNodes + 6, ghostNodesCmp, ghostNodesCmp + 6);
	}
	else if(comm.rank == 1)
	{
		int ghostNodes[12];
		int ghostNodesCmp[12] = {0, 1, 2, 3, 4, 5, 12, 13, 14, 15, 16, 17};
		status = graph->getGhostNodes(ghostNodes, 12);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
		BOOST_CHECK_EQUAL_COLLECTIONS(ghostNodes, ghostNodes + 12, ghostNodesCmp, ghostNodesCmp + 12);
	}
	else if(comm.rank == 2)
	{
		int ghostNodes[12];
		int ghostNodesCmp[12] = {6, 7, 8, 9, 10, 11, 18, 19, 20, 21, 22, 23};
		status = graph->getGhostNodes(ghostNodes, 12);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
		BOOST_CHECK_EQUAL_COLLECTIONS(ghostNodes, ghostNodes + 12, ghostNodesCmp, ghostNodesCmp + 12);
	}
	else if(comm.rank == 3)
	{
		int ghostNodes[6];
		int ghostNodesCmp[6] = {12, 13, 14, 15, 16, 17};
		status = graph->getGhostNodes(ghostNodes, 6);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
		BOOST_CHECK_EQUAL_COLLECTIONS(ghostNodes, ghostNodes + 6, ghostNodesCmp, ghostNodesCmp + 6);
	}

	// Check Edges
	if(comm.rank == 0)
	{
		int edgeCell1[20] = {0, 0, 0, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 4, 4, 4, 5, 5, 5};
		int edgeCell2[20] = {1, 2, 6, 0, 3, 7, 0, 3, 4, 8, 1, 2, 5, 9, 2, 5, 10,3, 4, 11};

		for(int i = 0; i < 20; i++)
		{
			bool check = false;
			status = graph->existsEdge(edgeCell1[i], edgeCell2[i], &check);
			BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
			BOOST_CHECK_EQUAL(check, true);
		}
	}
	else if(comm.rank == 1)
	{
		int edgeCell1[26] = {6, 6, 6, 6, 7, 7, 7, 7, 8, 8, 8, 8, 8, 9, 9, 9, 9, 9, 10, 10, 10, 10, 11, 11, 11, 11};
		int edgeCell2[26] = {7, 8, 0, 12, 6, 9, 1, 13, 6, 9, 10, 2, 14, 7, 8, 11, 3, 15, 8, 11, 4, 16, 9, 10, 5, 17};

		for(int i = 0; i < 26; i++)
		{
			bool check = false;
			status = graph->existsEdge(edgeCell1[i], edgeCell2[i], &check);
			BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
			BOOST_CHECK_EQUAL(check, true);
		}
	}
	else if(comm.rank == 2)
	{
		int edgeCell1[26] = {12, 12, 12, 12, 13, 13, 13, 13, 14, 14, 14, 14, 14, 15, 15, 15, 15, 15, 16, 16, 16, 16, 17, 17, 17, 17};
		int edgeCell2[26] = {13, 14, 6, 18, 12, 15, 7, 19, 12, 15, 16, 8, 20, 13, 14, 17, 9, 21, 14, 17, 10, 22, 15, 16, 11, 23};

		for(int i = 0; i < 26; i++)
		{
			bool check = false;
			status = graph->existsEdge(edgeCell1[i], edgeCell2[i], &check);
			BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
			BOOST_CHECK_EQUAL(check, true);
		}
	}
	else if(comm.rank == 3)
	{
		int edgeCell1[20] = {18, 18, 18, 19, 19, 19, 20, 20, 20, 20, 21, 21, 21, 21, 22, 22, 22, 23, 23, 23};
		int edgeCell2[20] = {19, 20, 12, 18, 21, 13, 18, 21, 22, 14, 19, 20, 23, 15, 20, 23, 16, 21, 22, 17};

		for(int i = 0; i < 20; i++)
		{
			bool check = false;
			status = graph->existsEdge(edgeCell1[i], edgeCell2[i], &check);
			BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
			BOOST_CHECK_EQUAL(check, true);
		}
	}
}
	*/

BOOST_AUTO_TEST_CASE(cleanup)
{
    MPI_Finalize();
}
