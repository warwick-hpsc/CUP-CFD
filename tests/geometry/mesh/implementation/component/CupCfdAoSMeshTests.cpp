/*
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Unit Tests for the concrete methods of the CupCfdAoSMesh class
 */

#define BOOST_TEST_MODULE CupCfdAoSMesh
#include <boost/test/unit_test.hpp>
#include <boost/test/output_test_stream.hpp>
#include <stdexcept>
#include <string>

#include "CupCfdAoSMesh.h"
#include "Error.h"
#include "EuclideanPoint.h"
#include "EuclideanVector.h"

#include "EuclideanPoint.h"
#include "PartitionerConfig.h"
#include "PartitionerNaiveConfig.h"
#include "MeshSourceStructGenConfig.h"
#include "MeshConfig.h"

using namespace cupcfd::geometry::mesh;
namespace utf = boost::unit_test;
namespace euc = cupcfd::geometry::euclidean;

// Setup
BOOST_AUTO_TEST_CASE(setup)
{
    int argc = boost::unit_test::framework::master_test_suite().argc;
    char ** argv = boost::unit_test::framework::master_test_suite().argv;

    MPI_Init(&argc, &argv);
}

// ToDo: A lot of these tests check the internals (which is OK for now), but really should be
// more akin to black-box tests

// ToDo: Unit Tests for getters and setters

// === Constructors ===
// Test 1:
BOOST_AUTO_TEST_CASE(constructor_test1)
{

}

// === addVertex + Vertex getters + Vertex Setters ===
// Test 1: Add Vertex and retrieve correct points
BOOST_AUTO_TEST_CASE(addVertex_test1)
{
	// Setup
	cupcfd::comm::Communicator comm(MPI_COMM_WORLD);
	CupCfdAoSMesh<int,double,int> mesh(comm);
	cupcfd::error::eCodes status;
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> center;
	cupcfd::geometry::euclidean::EuclideanVector<double,3> norm;

	// Add Vertices
	int vertLabel[18] = {180, 170, 160, 150, 140, 130, 120, 110, 100, 90, 80, 70, 60, 50, 40, 30, 20, 10};
	double vertX[18] = {0.0, 0.5, 1.0, 1.5, 0.0, 0.5, 1.0, 1.5, 0.5, 1.0, 0.0, 0.5, 1.0, 0.0, 0.5, 1.0, 0.5, 1.0};
	double vertY[18] = {0.0, 0.0, 0.0, 0.0, 0.5, 0.5, 0.5, 0.5, 1.0, 1.0, 0.0, 0.0, 0.0, 0.5, 0.5, 0.5, 1.0, 1.0};
	double vertZ[18] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5};

	for(int i = 0; i < 18; i++)
	{
		cupcfd::geometry::euclidean::EuclideanPoint<double,3> point(vertX[i], vertY[i], vertZ[i]);
		status = mesh.addVertex(vertLabel[i], point);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	}

	// Test getters functions without error codes
	for(int i = 0; i < 18; i++)
	{
		int localID = mesh.getVertexID(vertLabel[i]);

		cupcfd::geometry::euclidean::EuclideanPoint<double,3> point = mesh.getVertexPos(localID);
		BOOST_CHECK_EQUAL(point.cmp[0], vertX[i]);
		BOOST_CHECK_EQUAL(point.cmp[1], vertY[i]);
		BOOST_CHECK_EQUAL(point.cmp[2], vertZ[i]);
	}

	// Test getter functions with error codes
	for(int i = 0; i < 18; i++)
	{
		int localID;
		status = mesh.getVertexID(vertLabel[i], &localID);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

		cupcfd::geometry::euclidean::EuclideanPoint<double,3> point;
		status = mesh.getVertexPos(localID, point);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
		BOOST_CHECK_EQUAL(point.cmp[0], vertX[i]);
		BOOST_CHECK_EQUAL(point.cmp[1], vertY[i]);
		BOOST_CHECK_EQUAL(point.cmp[2], vertZ[i]);
	}

	// Check Mesh Local Properties are Updated - Global can only be updated at finalize
	BOOST_CHECK_EQUAL(mesh.properties.lVertices, 18);
}

// Test 2: Test overwriting values
BOOST_AUTO_TEST_CASE(addVertex_test2)
{
	// Setup
	cupcfd::comm::Communicator comm(MPI_COMM_WORLD);
	CupCfdAoSMesh<int,double,int> mesh(comm);
	cupcfd::error::eCodes status;
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> center;
	cupcfd::geometry::euclidean::EuclideanVector<double,3> norm;

	// Add Vertices
	int vertLabel[18] = {180, 170, 160, 150, 140, 130, 120, 110, 100, 90, 80, 70, 60, 50, 40, 30, 20, 10};
	double vertX[18] = {0.0, 0.5, 1.0, 1.5, 0.0, 0.5, 1.0, 1.5, 0.5, 1.0, 0.0, 0.5, 1.0, 0.0, 0.5, 1.0, 0.5, 1.0};
	double vertY[18] = {0.0, 0.0, 0.0, 0.0, 0.5, 0.5, 0.5, 0.5, 1.0, 1.0, 0.0, 0.0, 0.0, 0.5, 0.5, 0.5, 1.0, 1.0};
	double vertZ[18] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5};

	double vertXNew[18] = {10.0, 10.5, 11.0, 11.5, 10.0, 10.5, 11.0, 11.5, 10.5, 11.0, 10.0, 10.5, 11.0, 10.0, 10.5, 11.0, 10.5, 11.0};
	double vertYNew[18] = {10.0, 10.0, 10.0, 10.0, 10.5, 10.5, 10.5, 10.5, 11.0, 11.0, 10.0, 10.0, 10.0, 10.5, 10.5, 10.5, 11.0, 11.0};
	double vertZNew[18] = {10.0, 10.0, 10.0, 10.0, 10.0, 10.0, 10.0, 10.0, 10.0, 10.0, 10.5, 10.5, 10.5, 10.5, 10.5, 10.5, 10.5, 10.5};

	for(int i = 0; i < 18; i++)
	{
		cupcfd::geometry::euclidean::EuclideanPoint<double,3> point(vertX[i], vertY[i], vertZ[i]);
		status = mesh.addVertex(vertLabel[i], point);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	}

	for(int i = 0; i < 18; i++)
	{
		int localID = mesh.getVertexID(vertLabel[i]);
		cupcfd::geometry::euclidean::EuclideanPoint<double,3> point = mesh.getVertexPos(localID);
		BOOST_CHECK_EQUAL(point.cmp[0], vertX[i]);
		BOOST_CHECK_EQUAL(point.cmp[1], vertY[i]);
		BOOST_CHECK_EQUAL(point.cmp[2], vertZ[i]);
	}

	// Overwrite values
	for(int i = 0; i < 18; i++)
	{
		int localID = mesh.getVertexID(vertLabel[i]);
		cupcfd::geometry::euclidean::EuclideanPoint<double,3> point(vertXNew[i], vertYNew[i], vertZNew[i]);
		status = mesh.setVertexPos(localID, point);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	}

	// Test values correct
	for(int i = 0; i < 18; i++)
	{
		int localID = mesh.getVertexID(vertLabel[i]);

		cupcfd::geometry::euclidean::EuclideanPoint<double,3> point = mesh.getVertexPos(localID);
		BOOST_CHECK_EQUAL(point.cmp[0], vertXNew[i]);
		BOOST_CHECK_EQUAL(point.cmp[1], vertYNew[i]);
		BOOST_CHECK_EQUAL(point.cmp[2], vertZNew[i]);
	}

	// Check Mesh Local Properties are Updated - Global can only be updated at finalize
	BOOST_CHECK_EQUAL(mesh.properties.lVertices, 18);
}

// Test 3: Error Check: Add a vertex label that already exists on this rank
BOOST_AUTO_TEST_CASE(addVertex_test3)
{
	// Setup
	cupcfd::comm::Communicator comm(MPI_COMM_WORLD);
	CupCfdAoSMesh<int,double,int> mesh(comm);
	cupcfd::error::eCodes status;
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> center;
	cupcfd::geometry::euclidean::EuclideanVector<double,3> norm;

	// Add Vertices
	int vertLabel[18] = {180, 180, 180, 180, 180, 180, 180, 180, 180, 180, 180, 180, 180, 180, 180, 180, 180, 200};
	double vertX[18] = {0.0, 0.5, 1.0, 1.5, 0.0, 0.5, 1.0, 1.5, 0.5, 1.0, 0.0, 0.5, 1.0, 0.0, 0.5, 1.0, 0.5, 1.0};
	double vertY[18] = {0.0, 0.0, 0.0, 0.0, 0.5, 0.5, 0.5, 0.5, 1.0, 1.0, 0.0, 0.0, 0.0, 0.5, 0.5, 0.5, 1.0, 1.0};
	double vertZ[18] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5};

	cupcfd::geometry::euclidean::EuclideanPoint<double,3> point(vertX[0], vertY[0], vertZ[0]);
	status = mesh.addVertex(vertLabel[0], point);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	for(int i = 1; i < 17; i++)
	{
		cupcfd::geometry::euclidean::EuclideanPoint<double,3> point(vertX[i], vertY[i], vertZ[i]);
		status = mesh.addVertex(vertLabel[i], point);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_MESH_EXISTING_VERTEX);
	}

	cupcfd::geometry::euclidean::EuclideanPoint<double,3> point2(vertX[17], vertY[17], vertZ[17]);
	status = mesh.addVertex(vertLabel[17], point2);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	// Check Mesh Local Properties are Updated - Global can only be updated at finalize
	BOOST_CHECK_EQUAL(mesh.properties.lVertices, 2);
}


// === addRegion + Region getters + Region Setters ===
// Test 1: Add Region and get correct values
BOOST_AUTO_TEST_CASE(addRegion_test1)
{
	// Setup
	cupcfd::comm::Communicator comm(MPI_COMM_WORLD);
	CupCfdAoSMesh<int,double,int> mesh(comm);
	cupcfd::error::eCodes status;
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> center;
	cupcfd::geometry::euclidean::EuclideanVector<double,3> norm;

	// Add Regions
	// First 7 have set values, last two are default comparisons
	int rLabel[9] = {90, 80, 70, 60, 50, 30, 40, 10, 20};
	RType rTypes[9] = {RTYPE_DEFAULT, RTYPE_WALL, RTYPE_OUTLET, RTYPE_INLET, RTYPE_OUTLET, RTYPE_DEFAULT, RTYPE_SYMP, RTYPE_DEFAULT, RTYPE_DEFAULT};
	std::string rName[9] = {"Region1", "Region2", "Region3", "Test4", "Name5", "Region6", "Region7", "Default1", "Default2"};
	bool std[9] = {true, false, false, false, true, true, false, false, false};
	bool flux[9] = {false, false, true, true, false, false, true, false, false};
	bool adiab[9] = {false, true, true, true, false, false, false, false, false};
	double ylog[9] = {0.7, 9.8, 2.6, 6.9, 10.2, 12.8, 5.6, 0.0, 0.0};
	double elog[9] = {-1.2, 3.5, 10.9, 4.5, 3.5, 1.7, 8.6, 0.0, 0.0};
	double density[9] = {0.0, 3.6, 2.4, 100.3, 102.3, -6.7, -10.5, 0.0, 0.0};
	double turbKE[9] = {6.7, 2.4, -1.6, 23.5, 2.5, 2.6, 2.7, 0.0, 0.0};
	double turbDiss[9] = {1.1, 1.2, 1.3, 1.4, 1.5, 1.6, 1.7, 0.0, 0.0};
	double splvl[9] = {8.9, 8.8, 8.7, 8.6, 8.5, 8.4, 8.3, 0.0, 0.0};
	double den[9] = {10.0, 10.1, 10.2, 10.3, 10.4, 10.5, 10.6, 0.0, 0.0};
	double r[9] = {10.0, 20.0, 30.0, 40.0, 50.0, 60.0, 70.0, 0.0, 0.0};
	double t[9] = {90.0, 80.0, 70.0, 60.0, 50.0, 40.0, 30.0, 0.0, 0.0};
	double forceTanX[9] = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 0.0, 0.0};
	double forceTanY[9] = {9.0, 8.0, 7.0, 6.0, 5.0, 4.0, 3.0, 0.0, 0.0};
	double forceTanZ[9] = {1.5, 2.5, 3.5, 4.5, 5.5, 6.5, 7.5, 0.0, 0.0};
	double uvwX[9] = {9.5, 8.5, 7.5, 6.5, 5.5, 4.5, 3.5, 0.0, 0.0};
	double uvwY[9] = {1.1, 2.2, 3.3, 4.4, 5.5, 6.6, 7.7, 0.0, 0.0};
	double uvwZ[9] = {9.9, 8.8, 7.7, 6.6, 5.5, 4.4, 3.3, 0.0, 0.0};

	// Add Regions with values
	for(int i = 0; i < 7; i++)
	{
		cupcfd::geometry::euclidean::EuclideanVector<double,3> forceTangent(forceTanX[i], forceTanY[i], forceTanZ[i]);
		cupcfd::geometry::euclidean::EuclideanVector<double,3> uvw(uvwX[i], uvwY[i], uvwZ[i]);

		status = mesh.addRegion(rLabel[i], rTypes[i], std[i], flux[i], adiab[i], ylog[i], elog[i],
					   density[i], turbKE[i], turbDiss[i], splvl[i], den[i], r[i], t[i],
					   forceTangent, uvw, rName[i]);

		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	}

	// Add Regions that are defaults
	for(int i = 7; i < 9; i++)
	{
		status = mesh.addRegion(rLabel[i], rName[i]);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	}

	// Test getters without error codes
	for(int i = 0; i < 9; i++)
	{
		int localID = mesh.getRegionID(rLabel[i]);

		BOOST_CHECK_EQUAL(mesh.getRegionType(localID), rTypes[i]);
		BOOST_CHECK_EQUAL(mesh.getRegionStd(localID), std[i]);
		BOOST_CHECK_EQUAL(mesh.getRegionYLog(localID), ylog[i]);
		BOOST_CHECK_EQUAL(mesh.getRegionELog(localID), elog[i]);
		BOOST_CHECK_EQUAL(mesh.getRegionDensity(localID), density[i]);
		BOOST_CHECK_EQUAL(mesh.getRegionTurbKE(localID), turbKE[i]);
		BOOST_CHECK_EQUAL(mesh.getRegionTurbDiss(localID), turbDiss[i]);
		BOOST_CHECK_EQUAL(mesh.getRegionSplvl(localID), splvl[i]);
		BOOST_CHECK_EQUAL(mesh.getRegionDen(localID), den[i]);
		BOOST_CHECK_EQUAL(mesh.getRegionName(localID), rName[i]);
		BOOST_CHECK_EQUAL(mesh.getRegionFlux(localID), flux[i]);
		BOOST_CHECK_EQUAL(mesh.getRegionAdiab(localID), adiab[i]);
		BOOST_CHECK_EQUAL(mesh.getRegionR(localID), r[i]);
		BOOST_CHECK_EQUAL(mesh.getRegionT(localID), t[i]);

		cupcfd::geometry::euclidean::EuclideanVector<double,3> forceTangent = mesh.getRegionForceTangent(localID);
		cupcfd::geometry::euclidean::EuclideanVector<double,3> uvw = mesh.getRegionUVW(localID);
		BOOST_CHECK_EQUAL(forceTangent.cmp[0], forceTanX[i]);
		BOOST_CHECK_EQUAL(forceTangent.cmp[1], forceTanY[i]);
		BOOST_CHECK_EQUAL(forceTangent.cmp[2], forceTanZ[i]);
		BOOST_CHECK_EQUAL(uvw.cmp[0], uvwX[i]);
		BOOST_CHECK_EQUAL(uvw.cmp[1], uvwY[i]);
		BOOST_CHECK_EQUAL(uvw.cmp[2], uvwZ[i]);
	}

	// Test getters with error codes
	for(int i = 0; i < 9; i++)
	{
		int localID = mesh.getRegionID(rLabel[i]);

		bool bTmp;
		double dTmp;
		RType typeTmp;
		std::string sTmp;

		status = mesh.getRegionType(localID, &typeTmp);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
		BOOST_CHECK_EQUAL(typeTmp, rTypes[i]);

		status = mesh.getRegionStd(localID, &bTmp);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
		BOOST_CHECK_EQUAL(bTmp, std[i]);

		status = mesh.getRegionFlux(localID, &bTmp);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
		BOOST_CHECK_EQUAL(bTmp, flux[i]);

		status = mesh.getRegionAdiab(localID, &bTmp);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
		BOOST_CHECK_EQUAL(bTmp, adiab[i]);

		status = mesh.getRegionYLog(localID, &dTmp);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
		BOOST_CHECK_EQUAL(dTmp, ylog[i]);

		status = mesh.getRegionELog(localID, &dTmp);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
		BOOST_CHECK_EQUAL(dTmp, elog[i]);

		status = mesh.getRegionDensity(localID, &dTmp);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
		BOOST_CHECK_EQUAL(dTmp, density[i]);

		status = mesh.getRegionTurbKE(localID, &dTmp);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
		BOOST_CHECK_EQUAL(dTmp, turbKE[i]);

		status = mesh.getRegionTurbDiss(localID, &dTmp);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
		BOOST_CHECK_EQUAL(dTmp, turbDiss[i]);

		status = mesh.getRegionSplvl(localID, &dTmp);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
		BOOST_CHECK_EQUAL(dTmp, splvl[i]);

		status = mesh.getRegionDen(localID, &dTmp);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
		BOOST_CHECK_EQUAL(dTmp, den[i]);

		//status = mesh.getRegionName(localID, sTmp);
		//BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
		//BOOST_CHECK_EQUAL(dTmp, rName[i]);

		status = mesh.getRegionR(localID, &dTmp);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
		BOOST_CHECK_EQUAL(dTmp, r[i]);

		status = mesh.getRegionT(localID, &dTmp);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
		BOOST_CHECK_EQUAL(dTmp, t[i]);

		cupcfd::geometry::euclidean::EuclideanVector<double,3> forceTangent;
		status = mesh.getRegionForceTangent(localID, forceTangent);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
		BOOST_CHECK_EQUAL(forceTangent.cmp[0], forceTanX[i]);
		BOOST_CHECK_EQUAL(forceTangent.cmp[1], forceTanY[i]);
		BOOST_CHECK_EQUAL(forceTangent.cmp[2], forceTanZ[i]);

		cupcfd::geometry::euclidean::EuclideanVector<double,3> uvw;
		status = mesh.getRegionUVW(localID, uvw);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
		BOOST_CHECK_EQUAL(uvw.cmp[0], uvwX[i]);
		BOOST_CHECK_EQUAL(uvw.cmp[1], uvwY[i]);
		BOOST_CHECK_EQUAL(uvw.cmp[2], uvwZ[i]);
	}

	// Check Mesh Local Properties are Updated - Global can only be updated at finalize
	BOOST_CHECK_EQUAL(mesh.properties.lRegions, 9);
}

// Test 2: Test overwriting values
BOOST_AUTO_TEST_CASE(addRegion_test2)
{
	// Setup
	cupcfd::comm::Communicator comm(MPI_COMM_WORLD);
	CupCfdAoSMesh<int,double,int> mesh(comm);
	cupcfd::error::eCodes status;
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> center;
	cupcfd::geometry::euclidean::EuclideanVector<double,3> norm;

	// Add Regions
	// First 7 have set values, last two are default comparisons
	int rLabel[9] = {90, 80, 70, 60, 50, 30, 40, 10, 20};
	RType rTypes[9] = {RTYPE_DEFAULT, RTYPE_WALL, RTYPE_OUTLET, RTYPE_INLET, RTYPE_OUTLET, RTYPE_DEFAULT, RTYPE_SYMP, RTYPE_DEFAULT, RTYPE_DEFAULT};
	std::string rName[9] = {"Region1", "Region2", "Region3", "Test4", "Name5", "Region6", "Region7", "Default1", "Default2"};
	bool std[9] = {true, false, false, false, true, true, false, false, false};
	bool flux[9] = {false, false, true, true, false, false, true, false, false};
	bool adiab[9] = {false, true, true, true, false, false, false, false, false};
	double ylog[9] = {0.7, 9.8, 2.6, 6.9, 10.2, 12.8, 5.6, 0.0, 0.0};
	double elog[9] = {-1.2, 3.5, 10.9, 4.5, 3.5, 1.7, 8.6, 0.0, 0.0};
	double density[9] = {0.0, 3.6, 2.4, 100.3, 102.3, -6.7, -10.5, 0.0, 0.0};
	double turbKE[9] = {6.7, 2.4, -1.6, 23.5, 2.5, 2.6, 2.7, 0.0, 0.0};
	double turbDiss[9] = {1.1, 1.2, 1.3, 1.4, 1.5, 1.6, 1.7, 0.0, 0.0};
	double splvl[9] = {8.9, 8.8, 8.7, 8.6, 8.5, 8.4, 8.3, 0.0, 0.0};
	double den[9] = {10.0, 10.1, 10.2, 10.3, 10.4, 10.5, 10.6, 0.0, 0.0};
	double r[9] = {10.0, 20.0, 30.0, 40.0, 50.0, 60.0, 70.0, 0.0, 0.0};
	double t[9] = {90.0, 80.0, 70.0, 60.0, 50.0, 40.0, 30.0, 0.0, 0.0};
	double forceTanX[9] = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 0.0, 0.0};
	double forceTanY[9] = {9.0, 8.0, 7.0, 6.0, 5.0, 4.0, 3.0, 0.0, 0.0};
	double forceTanZ[9] = {1.5, 2.5, 3.5, 4.5, 5.5, 6.5, 7.5, 0.0, 0.0};
	double uvwX[9] = {9.5, 8.5, 7.5, 6.5, 5.5, 4.5, 3.5, 0.0, 0.0};
	double uvwY[9] = {1.1, 2.2, 3.3, 4.4, 5.5, 6.6, 7.7, 0.0, 0.0};
	double uvwZ[9] = {9.9, 8.8, 7.7, 6.6, 5.5, 4.4, 3.3, 0.0, 0.0};

	RType rTypesNew[9] = {RTYPE_DEFAULT, RTYPE_WALL, RTYPE_OUTLET, RTYPE_INLET, RTYPE_OUTLET, RTYPE_DEFAULT, RTYPE_SYMP, RTYPE_DEFAULT, RTYPE_DEFAULT};
	std::string rNameNew[9] = {"A", "B", "C", "D", "E", "F", "G", "H", "I"};
	bool stdNew[9] = {false, false, false, false, true, false, false, false, false};
	bool fluxNew[9] = {true, true, true, true, false, true, true, true, true};
	bool adiabNew[9] = {true, false, true, false, true, false, true, false, true};
	double ylogNew[9] = {-1.0, -2.0, -3.0, -4.0, -5.0, -6.0, -7.0, -8.0, -9.0};
	double elogNew[9] = {-9.0, -8.0, -7.0, -6.0, -5.0, -4.0, -3.0, -2.0, -1.0};
	double densityNew[9] = {0.0, 0.0, 0.0, 2.0, 0.0, 0.0, 0.0, 0.0, 0.0};
	double turbKENew[9] = {1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0};
	double turbDissNew[9] = {2.0, 2.0, 2.0, 2.0, 2.0, 2.0, 2.0, 2.0, 2.0};
	double splvlNew[9] = {-1.0, 1.0, -1.0, 1.0, -1.0, 1.0, -1.0, 1.0, -1.0};
	double denNew[9] = {1.0, 2.0, 3.0, 4.0, 5.0, 4.0, 3.0, 2.0, 1.0};
	double rNew[9] = {5.0, 4.0, 3.0, 2.0, 1.0, 2.0, 3.0, 4.0, 5.0};
	double tNew[9] = {10.0, 9.0, 8.0, 7.0, 6.0, 7.0, 8.0, 9.0, 10.0};
	double forceTanXNew[9] = {6.0, 7.0, 8.0, 9.0, 10.0, 11.0, 12.0, 13.0, 14.0};
	double forceTanYNew[9] = {1.2, 1.3, 1.4, 1.4, 1.5, 1.6, 1.6, 1.7, 1.7};
	double forceTanZNew[9] = {2.0, 3.0, 2.0, 4.0, 2.0, 2.0, 2.0, 2.0, 2.0};
	double uvwXNew[9] = {9.9, 8.8, 7.7, 6.6, 7.7, 8.8, 9.9, 8.8, 7.7};
	double uvwYNew[9] = {9.9, 8.8, 7.7, 6.6, 7.7, 8.8, 6.8, 8.8, 7.7};
	double uvwZNew[9] = {9.9, 8.8, 7.7, 6.6, 7.7, 8.8, 9.9, 7.8, 7.7};

	// Add Regions with values
	for(int i = 0; i < 7; i++)
	{
		cupcfd::geometry::euclidean::EuclideanVector<double,3> forceTangent(forceTanX[i], forceTanY[i], forceTanZ[i]);
		cupcfd::geometry::euclidean::EuclideanVector<double,3> uvw(uvwX[i], uvwY[i], uvwZ[i]);

		status = mesh.addRegion(rLabel[i], rTypes[i], std[i], flux[i], adiab[i], ylog[i], elog[i],
					   density[i], turbKE[i], turbDiss[i], splvl[i], den[i], r[i], t[i],
					   forceTangent, uvw, rName[i]);

		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	}

	// Add Regions that are defaults
	for(int i = 7; i < 9; i++)
	{
		status = mesh.addRegion(rLabel[i], rName[i]);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	}

	for(int i = 0; i < 9; i++)
	{
		int localID = mesh.getRegionID(rLabel[i]);

		BOOST_CHECK_EQUAL(mesh.getRegionType(localID), rTypes[i]);
		BOOST_CHECK_EQUAL(mesh.getRegionStd(localID), std[i]);
		BOOST_CHECK_EQUAL(mesh.getRegionYLog(localID), ylog[i]);
		BOOST_CHECK_EQUAL(mesh.getRegionELog(localID), elog[i]);
		BOOST_CHECK_EQUAL(mesh.getRegionDensity(localID), density[i]);
		BOOST_CHECK_EQUAL(mesh.getRegionTurbKE(localID), turbKE[i]);
		BOOST_CHECK_EQUAL(mesh.getRegionTurbDiss(localID), turbDiss[i]);
		BOOST_CHECK_EQUAL(mesh.getRegionSplvl(localID), splvl[i]);
		BOOST_CHECK_EQUAL(mesh.getRegionDen(localID), den[i]);
		BOOST_CHECK_EQUAL(mesh.getRegionName(localID), rName[i]);
		BOOST_CHECK_EQUAL(mesh.getRegionFlux(localID), flux[i]);
		BOOST_CHECK_EQUAL(mesh.getRegionAdiab(localID), adiab[i]);
		BOOST_CHECK_EQUAL(mesh.getRegionR(localID), r[i]);
		BOOST_CHECK_EQUAL(mesh.getRegionT(localID), t[i]);

		cupcfd::geometry::euclidean::EuclideanVector<double,3> forceTangent = mesh.getRegionForceTangent(localID);
		cupcfd::geometry::euclidean::EuclideanVector<double,3> uvw = mesh.getRegionUVW(localID);
		BOOST_CHECK_EQUAL(forceTangent.cmp[0], forceTanX[i]);
		BOOST_CHECK_EQUAL(forceTangent.cmp[1], forceTanY[i]);
		BOOST_CHECK_EQUAL(forceTangent.cmp[2], forceTanZ[i]);
		BOOST_CHECK_EQUAL(uvw.cmp[0], uvwX[i]);
		BOOST_CHECK_EQUAL(uvw.cmp[1], uvwY[i]);
		BOOST_CHECK_EQUAL(uvw.cmp[2], uvwZ[i]);
	}

	// Overwrite Values
	for(int i = 0; i < 9; i++)
	{
		int localID = mesh.getRegionID(rLabel[i]);

		status = mesh.setRegionType(localID, rTypesNew[i]);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

		status = mesh.setRegionStd(localID, stdNew[i]);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

		status = mesh.setRegionYLog(localID, ylogNew[i]);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

		status = mesh.setRegionELog(localID, elogNew[i]);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

		status = mesh.setRegionDensity(localID, densityNew[i]);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

		status = mesh.setRegionTurbKE(localID, turbKENew[i]);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

		status = mesh.setRegionTurbDiss(localID, turbDissNew[i]);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

		status = mesh.setRegionSplvl(localID, splvlNew[i]);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

		status = mesh.setRegionDen(localID, denNew[i]);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

		status = mesh.setRegionName(localID, rNameNew[i]);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

		status = mesh.setRegionFlux(localID, fluxNew[i]);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

		status = mesh.setRegionAdiab(localID, adiabNew[i]);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

		status = mesh.setRegionR(localID, rNew[i]);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

		status = mesh.setRegionT(localID, tNew[i]);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

		cupcfd::geometry::euclidean::EuclideanVector<double,3> forceTangent(forceTanXNew[i], forceTanYNew[i], forceTanZNew[i]);
		cupcfd::geometry::euclidean::EuclideanVector<double,3> uvw(uvwXNew[i], uvwYNew[i], uvwZNew[i]);

		mesh.setRegionForceTangent(localID, forceTangent);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

		mesh.setRegionUVW(localID, uvw);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	}

	// Check Values Correctly Overwritten
	for(int i = 0; i < 9; i++)
	{
		int localID = mesh.getRegionID(rLabel[i]);

		BOOST_CHECK_EQUAL(mesh.getRegionType(localID), rTypesNew[i]);
		BOOST_CHECK_EQUAL(mesh.getRegionStd(localID), stdNew[i]);
		BOOST_CHECK_EQUAL(mesh.getRegionYLog(localID), ylogNew[i]);
		BOOST_CHECK_EQUAL(mesh.getRegionELog(localID), elogNew[i]);
		BOOST_CHECK_EQUAL(mesh.getRegionDensity(localID), densityNew[i]);
		BOOST_CHECK_EQUAL(mesh.getRegionTurbKE(localID), turbKENew[i]);
		BOOST_CHECK_EQUAL(mesh.getRegionTurbDiss(localID), turbDissNew[i]);
		BOOST_CHECK_EQUAL(mesh.getRegionSplvl(localID), splvlNew[i]);
		BOOST_CHECK_EQUAL(mesh.getRegionDen(localID), denNew[i]);
		BOOST_CHECK_EQUAL(mesh.getRegionName(localID), rNameNew[i]);
		BOOST_CHECK_EQUAL(mesh.getRegionFlux(localID), fluxNew[i]);
		BOOST_CHECK_EQUAL(mesh.getRegionAdiab(localID), adiabNew[i]);
		BOOST_CHECK_EQUAL(mesh.getRegionR(localID), rNew[i]);
		BOOST_CHECK_EQUAL(mesh.getRegionT(localID), tNew[i]);

		cupcfd::geometry::euclidean::EuclideanVector<double,3> forceTangent = mesh.getRegionForceTangent(localID);
		cupcfd::geometry::euclidean::EuclideanVector<double,3> uvw = mesh.getRegionUVW(localID);
		BOOST_CHECK_EQUAL(forceTangent.cmp[0], forceTanXNew[i]);
		BOOST_CHECK_EQUAL(forceTangent.cmp[1], forceTanYNew[i]);
		BOOST_CHECK_EQUAL(forceTangent.cmp[2], forceTanZNew[i]);
		BOOST_CHECK_EQUAL(uvw.cmp[0], uvwXNew[i]);
		BOOST_CHECK_EQUAL(uvw.cmp[1], uvwYNew[i]);
		BOOST_CHECK_EQUAL(uvw.cmp[2], uvwZNew[i]);
	}

	// Check Mesh Local Properties are Updated - Global can only be updated at finalize
	BOOST_CHECK_EQUAL(mesh.properties.lRegions, 9);
}

// Test 3: Error Check: Add a region label that already exists on this rank
BOOST_AUTO_TEST_CASE(addRegion_test3)
{
	// Setup
	cupcfd::comm::Communicator comm(MPI_COMM_WORLD);
	CupCfdAoSMesh<int,double,int> mesh(comm);
	cupcfd::error::eCodes status;
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> center;
	cupcfd::geometry::euclidean::EuclideanVector<double,3> norm;

	// Add Regions
	// First 7 have set values, last two are default comparisons
	int rLabel[9] = {90, 90, 90, 90, 90, 90, 90, 90, 90};
	RType rTypes[9] = {RTYPE_DEFAULT, RTYPE_WALL, RTYPE_OUTLET, RTYPE_INLET, RTYPE_OUTLET, RTYPE_DEFAULT, RTYPE_SYMP, RTYPE_DEFAULT, RTYPE_DEFAULT};
	std::string rName[9] = {"Region1", "Region2", "Region3", "Test4", "Name5", "Region6", "Region7", "Default1", "Default2"};
	bool std[9] = {true, false, false, false, true, true, false, false, false};
	bool flux[9] = {false, false, true, true, false, false, true, false, false};
	bool adiab[9] = {false, true, true, true, false, false, false, false, false};
	double ylog[9] = {0.7, 9.8, 2.6, 6.9, 10.2, 12.8, 5.6, 0.0, 0.0};
	double elog[9] = {-1.2, 3.5, 10.9, 4.5, 3.5, 1.7, 8.6, 0.0, 0.0};
	double density[9] = {0.0, 3.6, 2.4, 100.3, 102.3, -6.7, -10.5, 0.0, 0.0};
	double turbKE[9] = {6.7, 2.4, -1.6, 23.5, 2.5, 2.6, 2.7, 0.0, 0.0};
	double turbDiss[9] = {1.1, 1.2, 1.3, 1.4, 1.5, 1.6, 1.7, 0.0, 0.0};
	double splvl[9] = {8.9, 8.8, 8.7, 8.6, 8.5, 8.4, 8.3, 0.0, 0.0};
	double den[9] = {10.0, 10.1, 10.2, 10.3, 10.4, 10.5, 10.6, 0.0, 0.0};
	double r[9] = {10.0, 20.0, 30.0, 40.0, 50.0, 60.0, 70.0, 0.0, 0.0};
	double t[9] = {90.0, 80.0, 70.0, 60.0, 50.0, 40.0, 30.0, 0.0, 0.0};
	double forceTanX[9] = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 0.0, 0.0};
	double forceTanY[9] = {9.0, 8.0, 7.0, 6.0, 5.0, 4.0, 3.0, 0.0, 0.0};
	double forceTanZ[9] = {1.5, 2.5, 3.5, 4.5, 5.5, 6.5, 7.5, 0.0, 0.0};
	double uvwX[9] = {9.5, 8.5, 7.5, 6.5, 5.5, 4.5, 3.5, 0.0, 0.0};
	double uvwY[9] = {1.1, 2.2, 3.3, 4.4, 5.5, 6.6, 7.7, 0.0, 0.0};
	double uvwZ[9] = {9.9, 8.8, 7.7, 6.6, 5.5, 4.4, 3.3, 0.0, 0.0};

	// Add Regions with values

	cupcfd::geometry::euclidean::EuclideanVector<double,3> forceTangent(forceTanX[0], forceTanY[0], forceTanZ[0]);
	cupcfd::geometry::euclidean::EuclideanVector<double,3> uvw(uvwX[0], uvwY[0], uvwZ[0]);

	status = mesh.addRegion(rLabel[0], rTypes[0], std[0], flux[0], adiab[0], ylog[0], elog[0],
				   density[0], turbKE[0], turbDiss[0], splvl[0], den[0], r[0], t[0],
				   forceTangent, uvw, rName[0]);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	for(int i = 1; i < 9; i++)
	{
		cupcfd::geometry::euclidean::EuclideanVector<double,3> forceTangent(forceTanX[i], forceTanY[i], forceTanZ[i]);
		cupcfd::geometry::euclidean::EuclideanVector<double,3> uvw(uvwX[i], uvwY[i], uvwZ[i]);

		status = mesh.addRegion(rLabel[i], rTypes[i], std[i], flux[i], adiab[i], ylog[i], elog[i],
					   density[i], turbKE[i], turbDiss[i], splvl[i], den[i], r[i], t[i],
					   forceTangent, uvw, rName[i]);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_MESH_EXISTING_REGION);
	}

	// Check Mesh Local Properties are Updated - Global can only be updated at finalize
	BOOST_CHECK_EQUAL(mesh.properties.lRegions, 1);
}

/*
// === addBoundary ===
// Test 1: Add two boundaries
BOOST_AUTO_TEST_CASE(addBoundary_test1)
{
	// Setup
	cupcfd::mpi::Communicator comm;
	cupcfd::mpi::drivers::set_comm(comm, MPI_COMM_WORLD);
	CupCfdAoSMesh<int,double,int> mesh(comm);
	cupcfd::error::eCodes status;
	std::string name;

	// Add Regions
	name = "Default Region";
	status = mesh.addRegion(65, RTYPE_DEFAULT, name);
	name = "Region2";
	status = mesh.addRegion(101, RTYPE_SYMP, name);

	// Add Vertices
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> point1(1.0, 2.0, 3.0);
	status = mesh.addVertex(41, point1);

	cupcfd::geometry::euclidean::EuclideanPoint<double,3> point2(2.0, 3.0, 4.0);
	status = mesh.addVertex(42, point2);

	cupcfd::geometry::euclidean::EuclideanPoint<double,3> point3(12.0, 13.0, 14.0);
	status = mesh.addVertex(15, point3);

	cupcfd::geometry::euclidean::EuclideanPoint<double,3> point4(13.0, 14.0, 15.0);
	status = mesh.addVertex(65, point4);

	cupcfd::geometry::euclidean::EuclideanPoint<double,3> point5(14.0, 15.0, 16.0);
	status = mesh.addVertex(101, point5);

	cupcfd::geometry::euclidean::EuclideanPoint<double,3> point6(3.0, 4.0, 5.0);
	status = mesh.addVertex(43, point6);

	int vertexIDs1[3] = {41, 43, 42};
	int vertexIDs2[4] = {43, 15, 65, 101};

	status = mesh.addBoundary(17, 101, vertexIDs1, 3, 3.7);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	BOOST_CHECK_EQUAL(mesh.boundaries[0].faceID, -1);
	BOOST_CHECK_EQUAL(mesh.boundaries[0].verticesID[0], 0);
	BOOST_CHECK_EQUAL(mesh.boundaries[0].verticesID[1], 5);
	BOOST_CHECK_EQUAL(mesh.boundaries[0].verticesID[2], 1);
	BOOST_CHECK_EQUAL(mesh.boundaries[0].verticesID[3], -1);
	BOOST_CHECK_EQUAL(mesh.boundaries[0].regionID, 1);
	BOOST_CHECK_EQUAL(mesh.boundaries[0].distance, 3.7);
	BOOST_CHECK_EQUAL(mesh.boundaries[0].yplus, 0.0);
	BOOST_CHECK_EQUAL(mesh.boundaries[0].uplus, 0.0);
	BOOST_CHECK_EQUAL(mesh.boundaries[0].shear.cmp[0], 0.0);
	BOOST_CHECK_EQUAL(mesh.boundaries[0].shear.cmp[1], 0.0);
	BOOST_CHECK_EQUAL(mesh.boundaries[0].shear.cmp[2], 0.0);
	BOOST_CHECK_EQUAL(mesh.boundaries[0].q, 0.0);
	BOOST_CHECK_EQUAL(mesh.boundaries[0].h, 0.0);
	BOOST_CHECK_EQUAL(mesh.boundaries[0].t, 0.0);
	BOOST_CHECK_EQUAL(mesh.boundaries.size(), 1);
	BOOST_CHECK_EQUAL(mesh.properties.lBoundaries, 1);

	status = mesh.addBoundary(201, 65, vertexIDs2, 4, 2.1);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	BOOST_CHECK_EQUAL(mesh.boundaries[1].faceID, -1);
	BOOST_CHECK_EQUAL(mesh.boundaries[1].verticesID[0], 5);
	BOOST_CHECK_EQUAL(mesh.boundaries[1].verticesID[1], 2);
	BOOST_CHECK_EQUAL(mesh.boundaries[1].verticesID[2], 3);
	BOOST_CHECK_EQUAL(mesh.boundaries[1].verticesID[3], 4);
	BOOST_CHECK_EQUAL(mesh.boundaries[1].regionID, 0);
	BOOST_CHECK_EQUAL(mesh.boundaries[1].distance, 2.1);
	BOOST_CHECK_EQUAL(mesh.boundaries[1].yplus, 0.0);
	BOOST_CHECK_EQUAL(mesh.boundaries[1].uplus, 0.0);
	BOOST_CHECK_EQUAL(mesh.boundaries[1].shear.cmp[0], 0.0);
	BOOST_CHECK_EQUAL(mesh.boundaries[1].shear.cmp[1], 0.0);
	BOOST_CHECK_EQUAL(mesh.boundaries[1].shear.cmp[2], 0.0);
	BOOST_CHECK_EQUAL(mesh.boundaries[1].q, 0.0);
	BOOST_CHECK_EQUAL(mesh.boundaries[1].h, 0.0);
	BOOST_CHECK_EQUAL(mesh.boundaries[1].t, 0.0);
	BOOST_CHECK_EQUAL(mesh.boundaries.size(), 2);
	BOOST_CHECK_EQUAL(mesh.properties.lBoundaries, 2);
}

*/

// === addBoundary + boundary getters + boundary setters ===
// Test 1: Add Boundaries and retrieve correct points


// === addCell + cell getters + cell setters ===
// Test 2: Test overwriting boundary values

// Test 3: Add a boundary with a non-existant region
BOOST_AUTO_TEST_CASE(addBoundary_test3)
{
	// Setup
	cupcfd::comm::Communicator comm(MPI_COMM_WORLD);
	CupCfdAoSMesh<int,double,int> mesh(comm);
	cupcfd::error::eCodes status;
	std::string name;

	// Add a Region
	name = "Default Region";
	status = mesh.addRegion(65, name);

	// Add Vertices
	double vertX[3] = {1.0, 2.0, 3.0};
	double vertY[3] = {2.0, 3.0, 4.0};
	double vertZ[3] = {3.0, 4.0, 5.0};
	double vertLabels[3] = {41, 42, 43};

	for(int i = 0; i < 3; i++)
	{
		cupcfd::geometry::euclidean::EuclideanPoint<double,3> point(vertX[i], vertY[i], vertZ[i]);
		status = mesh.addVertex(vertLabels[i], point);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	}

	int vertexIDs1[3] = {41, 43, 42};

	// Test and Check
	status = mesh.addBoundary(17, 57, vertexIDs1, 3);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_MESH_INVALID_REGION_LABEL);
}

// Test 4: Add a boundary with a non-existant vertex
BOOST_AUTO_TEST_CASE(addBoundary_test4)
{
	// Setup
	cupcfd::comm::Communicator comm(MPI_COMM_WORLD);
	CupCfdAoSMesh<int,double,int> mesh(comm);
	cupcfd::error::eCodes status;
	std::string name;

	// Add a Region
	name = "Default Region";
	status = mesh.addRegion(65, name);

	// Add Vertices
	double vertX[3] = {1.0, 2.0, 3.0};
	double vertY[3] = {2.0, 3.0, 4.0};
	double vertZ[3] = {3.0, 4.0, 5.0};
	double vertLabels[3] = {41, 42, 43};

	for(int i = 0; i < 3; i++)
	{
		cupcfd::geometry::euclidean::EuclideanPoint<double,3> point(vertX[i], vertY[i], vertZ[i]);
		status = mesh.addVertex(vertLabels[i], point);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	}

	// One Vertex ID has not been used during an add
	int vertexIDs1[3] = {41, 43, 109};

	// Test and Check
	status = mesh.addBoundary(17, 65, vertexIDs1, 3);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_MESH_INVALID_VERTEX_LABEL);
}

// Test 5: Add a boundary with too few vertexes
BOOST_AUTO_TEST_CASE(addBoundary_test5)
{
	// Setup
	cupcfd::comm::Communicator comm(MPI_COMM_WORLD);
	CupCfdAoSMesh<int,double,int> mesh(comm);
	cupcfd::error::eCodes status;
	std::string name;

	name = "Default Region";
	status = mesh.addRegion(65, name);

	// Add Vertices
	double vertX[3] = {1.0, 2.0, 3.0};
	double vertY[3] = {2.0, 3.0, 4.0};
	double vertZ[3] = {3.0, 4.0, 5.0};
	double vertLabels[3] = {41, 42, 43};

	for(int i = 0; i < 3; i++)
	{
		cupcfd::geometry::euclidean::EuclideanPoint<double,3> point(vertX[i], vertY[i], vertZ[i]);
		status = mesh.addVertex(vertLabels[i], point);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	}

	// 2 is insufficient number of vertices
	int vertexIDs1[2] = {41, 43};

	// Test and Check
	status = mesh.addBoundary(17, 65, vertexIDs1, 2);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_MESH_INVALID_VERTEX_COUNT);
}

// Test 6: Add a boundary with too many vertices (currently capped at 4)
BOOST_AUTO_TEST_CASE(addBoundary_test6)
{
	// Setup
	cupcfd::comm::Communicator comm(MPI_COMM_WORLD);
	CupCfdAoSMesh<int,double,int> mesh(comm);
	cupcfd::error::eCodes status;
	std::string name;

	name = "Default Region";
	status = mesh.addRegion(65, name);

	// Add Vertices
	double vertX[5] = {1.0, 2.0, 3.0, 4.0, 5.0};
	double vertY[5] = {2.0, 3.0, 4.0, 5.0, 6.0};
	double vertZ[5] = {3.0, 4.0, 5.0, 6.0, 7.0};
	double vertLabels[5] = {41, 42, 43, 44, 45};

	for(int i = 0; i < 3; i++)
	{
		cupcfd::geometry::euclidean::EuclideanPoint<double,3> point(vertX[i], vertY[i], vertZ[i]);
		status = mesh.addVertex(vertLabels[i], point);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	}

	int vertexIDs1[5] = {41, 43, 42, 44, 45};

	// Test and Check
	status = mesh.addBoundary(17, 65, vertexIDs1, 5);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_MESH_INVALID_VERTEX_COUNT);
}

// Test 7: Error Test - Add a boundary that already exists
BOOST_AUTO_TEST_CASE(addBoundary_test7)
{
	// Setup
	cupcfd::comm::Communicator comm(MPI_COMM_WORLD);
	CupCfdAoSMesh<int,double,int> mesh(comm);
	cupcfd::error::eCodes status;
	std::string name;

	// Add Regions
	name = "Default Region";
	status = mesh.addRegion(65, name);

	// Add Vertices
	double vertX[3] = {1.0, 2.0, 3.0};
	double vertY[3] = {2.0, 3.0, 4.0};
	double vertZ[3] = {3.0, 4.0, 5.0};
	double vertLabels[3] = {41, 42, 43};

	for(int i = 0; i < 3; i++)
	{
		cupcfd::geometry::euclidean::EuclideanPoint<double,3> point(vertX[i], vertY[i], vertZ[i]);
		status = mesh.addVertex(vertLabels[i], point);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	}

	int vertexIDs1[3] = {41, 43, 42};

	// Add boundary 1
	status = mesh.addBoundary(17, 65, vertexIDs1, 3);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	// Reuse boundary label
	status = mesh.addBoundary(17, 65, vertexIDs1, 3);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_MESH_EXISTING_BOUNDARY);
}

// === addCell + cell getters + cell setters ===
// Test 1: Add Cells and retrieve correct points
BOOST_AUTO_TEST_CASE(addCell_test1)
{
	// Setup
	cupcfd::comm::Communicator comm(MPI_COMM_WORLD);
	CupCfdAoSMesh<int,double,int> mesh(comm);
	cupcfd::error::eCodes status;
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> center;
	cupcfd::geometry::euclidean::EuclideanVector<double,3> norm;

	// Add Cells
	double cellLabel[5] = {10, 20, 30, 40, 50};
	double cellVol[5] = {1.0, 2.0, 3.0, 4.0, 0.0};
	double cellCenterX[5] = {5.0, 10.0, 15.0, 25.0, 0.0};
	double cellCenterY[5] = {15.0, 110.0, 115.0, 125.0, 0.0};
	double cellCenterZ[5] = {25.0, 210.0, 215.0, 225.0, 0.0};
	double cellLocal[5] = {false, false, false, false, false};
	double cellGhost[5] = {true, true, true, true, true};

	// Set 1 cell for each rank to be local
	cellLocal[comm.rank] = true;
	cellGhost[comm.rank] = false;

	// Rank 1 can have cell 5 (the 'default' cell)
	if(comm.rank == 0)
	{
		cellLocal[4] = true;
		cellGhost[4] = false;
	}

	for(int i = 0; i < 4; i++)
	{
		cupcfd::geometry::euclidean::EuclideanPoint<double,3> center(cellCenterX[i], cellCenterY[i], cellCenterZ[i]);
		status = mesh.addCell(cellLabel[i], center, cellVol[i], cellLocal[i]);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	}

	// Add 'default' cell
	status = mesh.addCell(cellLabel[4], cellLocal[4]);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	// Test getters functions without error codes
	for(int i = 0; i < 5; i++)
	{
		int localID = mesh.getCellID(cellLabel[i]);

		cupcfd::geometry::euclidean::EuclideanPoint<double,3> center;
		center = mesh.getCellCenter(localID);

		BOOST_CHECK_EQUAL(center.cmp[0], cellCenterX[i]);
		BOOST_CHECK_EQUAL(center.cmp[1], cellCenterY[i]);
		BOOST_CHECK_EQUAL(center.cmp[2], cellCenterZ[i]);
		BOOST_CHECK_EQUAL(mesh.getCellVolume(localID), cellVol[i]);
	}

	// Test getters functions with error codes
	for(int i = 0; i < 5; i++)
	{
		int localID = mesh.getCellID(cellLabel[i]);

		cupcfd::geometry::euclidean::EuclideanPoint<double,3> center;
		status = mesh.getCellCenter(localID, center);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
		BOOST_CHECK_EQUAL(center.cmp[0], cellCenterX[i]);
		BOOST_CHECK_EQUAL(center.cmp[1], cellCenterY[i]);
		BOOST_CHECK_EQUAL(center.cmp[2], cellCenterZ[i]);

		double tTmp;
		status = mesh.getCellVolume(localID, &tTmp);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
		BOOST_CHECK_EQUAL(tTmp, cellVol[i]);

		// Check the ghost/local status of the cells is correct - done by cell label
		bool local, ghost;

		status = mesh.cellConnGraph->existsLocalNode(cellLabel[i], &local);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
		BOOST_CHECK_EQUAL(local, cellLocal[i]);

		status = mesh.cellConnGraph->existsGhostNode(cellLabel[i], &ghost);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
		BOOST_CHECK_EQUAL(ghost, cellGhost[i]);
	}

	// Check Mesh Local Properties are Updated - Global can only be updated at finalize
	BOOST_CHECK_EQUAL(mesh.properties.lTCells, 5);

	if(comm.rank == 0)
	{
		BOOST_CHECK_EQUAL(mesh.properties.lOCells, 2);
		BOOST_CHECK_EQUAL(mesh.properties.lGhCells, 3);
	}
	else
	{
		BOOST_CHECK_EQUAL(mesh.properties.lOCells, 1);
		BOOST_CHECK_EQUAL(mesh.properties.lGhCells, 4);
	}
}

// Test 2: Test overwriting values
BOOST_AUTO_TEST_CASE(addCell_test2)
{

}

// Test 3: Test adding a duplicate cell label on this rank
BOOST_AUTO_TEST_CASE(addCell_test3)
{
	// Setup
	cupcfd::comm::Communicator comm(MPI_COMM_WORLD);
	CupCfdAoSMesh<int,double,int> mesh(comm);
	cupcfd::error::eCodes status;

	// Add Cells
	double cellLabel[5] = {50, 50, 50, 50, 50};
	double cellVol[5] = {1.0, 2.0, 3.0, 4.0, 0.0};
	double cellCenterX[5] = {5.0, 10.0, 15.0, 25.0, 0.0};
	double cellCenterY[5] = {15.0, 110.0, 115.0, 125.0, 0.0};
	double cellCenterZ[5] = {25.0, 210.0, 215.0, 225.0, 0.0};
	double cellLocal[5] = {false, false, false, false, false};
	double cellGhost[5] = {true, true, true, true, true};

	// Set 1 cell for each rank to be local
	cellLocal[comm.rank] = true;
	cellGhost[comm.rank] = false;

	// Rank 1 can have cell 5 (the 'default' cell)
	if(comm.rank == 0)
	{
		cellLocal[4] = true;
		cellGhost[4] = false;
	}

	cupcfd::geometry::euclidean::EuclideanPoint<double,3> center1(cellCenterX[0], cellCenterY[0], cellCenterZ[0]);
	status = mesh.addCell(cellLabel[0], center1, cellVol[0], cellLocal[0]);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	for(int i = 1; i < 5; i++)
	{
		cupcfd::geometry::euclidean::EuclideanPoint<double,3> center(cellCenterX[i], cellCenterY[i], cellCenterZ[i]);
		status = mesh.addCell(cellLabel[i], center, cellVol[i], cellLocal[i]);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_MESH_EXISTING_CELL);
	}
}

// === addFace + face getters + face setters ===
// Test 1: Add Face and retrieve correct points
BOOST_AUTO_TEST_CASE(addFace_test1)
{
	// Setup
	cupcfd::comm::Communicator comm(MPI_COMM_WORLD);
	CupCfdAoSMesh<int,double,int> mesh(comm);
	cupcfd::error::eCodes status;

	// Add Regions
	std::string name = "Default";
	status = mesh.addRegion(0, name);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	// ToDo: Strictly speaking, not all vertices and boundaries are needed on all ranks for this test.
	// We can whittle them down.

	// Add Vertices
	int vertLabel[18] = {5, 10, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 65, 70, 75, 80, 85, 90};
	double vertX[18] = {0.0, 0.5, 1.0, 1.5, 0.0, 0.5, 1.0, 1.5, 0.5, 1.0, 0.0, 0.5, 1.0, 0.0, 0.5, 1.0, 0.5, 1.0};
	double vertY[18] = {0.0, 0.0, 0.0, 0.0, 0.5, 0.5, 0.5, 0.5, 1.0, 1.0, 0.0, 0.0, 0.0, 0.5, 0.5, 0.5, 1.0, 1.0};
	double vertZ[18] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5};

	for(int i = 0; i < 18; i++)
	{
		cupcfd::geometry::euclidean::EuclideanPoint<double,3> point(vertX[i], vertY[i], vertZ[i]);
		status = mesh.addVertex(vertLabel[i], point);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	}

	// Add Boundaries
	int bndLabel[17] = {11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27};
	int bndVert1[17] = {0,  0, 0,  4,  10, 1, 1,  11, 2, 2,  3,  6,  6,  8,  5,  5, 14};
	int bndVert2[17] = {4,  1, 1,  5,  11, 2, 2,  12, 3, 3,  7,  7,  9,  9,  8,  6, 15};
	int bndVert3[17] = {10, 4, 10, 13, 13, 5, 11, 14, 6, 12, 12, 15, 15, 16, 14, 8, 16};
	int bndVert4[17] = {13, 5, 11, 14, 14, 6, 12, 15, 7, -1, 15, -1, 17, 17, 16, 9, 17};

	for(int i = 0; i < 17; i++)
	{
		// Use region 0 for all
		int vert[4] = {vertLabel[bndVert1[i]], vertLabel[bndVert2[i]], vertLabel[bndVert3[i]], -1};
		int count = 3;

		// add the 4th vertex if it exists
		if(bndVert4[i] != - 1)
		{
			count = 4;
			vert[3] = vertLabel[bndVert4[i]];
		}

		status = mesh.addBoundary(bndLabel[i], 0, vert, count);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	}

	int cellLabel[4] = {100, 200, 300, 400};

	// Add Cells
	// Each Rank gets 1 local cell
	if(comm.rank == 0)
	{
		status = mesh.addCell(cellLabel[0], true);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

		status = mesh.addCell(cellLabel[1], false);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	}
	else if(comm.rank == 1)
	{
		status = mesh.addCell(cellLabel[0], false);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

		status = mesh.addCell(cellLabel[1], true);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

		status = mesh.addCell(cellLabel[2], false);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

		status = mesh.addCell(cellLabel[3], false);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	}
	else if(comm.rank == 2)
	{
		status = mesh.addCell(cellLabel[2], true);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

		status = mesh.addCell(cellLabel[1], false);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	}
	else if(comm.rank == 3)
	{
		status = mesh.addCell(cellLabel[1], false);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

		status = mesh.addCell(cellLabel[3], true);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	}

	// Add Faces
	// For convienience, same order as boundaries + 3 non-boundary faces at end
	// This means face index = boundary index where applicable
	int faceLabel[20] = {10, 20, 30, 40, 50, 60, 70, 80, 90, 100, 110, 120, 130, 140, 150, 160, 170, 180, 190, 200};
	int faceVert1[20] = {0,  0, 0,  4,  10, 1, 1,  11, 2, 2,  3,  6,  6,  8,  5,  5, 14, 1,  2,  5};
	int faceVert2[20] = {4,  1, 1,  5,  11, 2, 2,  12, 3, 3,  7,  7,  9,  9,  8,  6, 15, 5,  6,  6};
	int faceVert3[20] = {10, 4, 10, 13, 13, 5, 11, 14, 6, 12, 12, 15, 15, 16, 14, 8, 16, 11, 12, 14};
	int faceVert4[20] = {13, 5, 11, 14, 14, 6, 12, 15, 7, -1, 15, -1, 17, 17, 16, 9, 17, 14, 15, 15};
	int faceNVertices[20] = {4, 4, 4, 4, 4, 4, 4, 4, 4, 3, 4, 3, 4, 4, 4, 4, 4, 4, 4, 4};
	int faceCell1[20] = {0, 0, 0, 0, 0, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 3, 0, 1, 1};
	int faceCell2OrBoundary[20] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 1, 2, 3};
	int isBoundary[20] = {true, true, true, true, true, true, true, true, true, true, true, true, true,
						  true, true, true, true, false, false, false};

	int keepFace[20] = {false, false, false, false, false, false, false, false, false, false,
						false, false, false, false, false, false, false, false, false, false};

	// Leave the last face as a default value face
	double faceLambda[20] = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0, 11.0, 12.0, 13.0, 14.0, 15.0, 16.0, 17.0, 18.0, 19.0, 0.0};
	double faceRLencos[20] = {20.0, 19.0, 18.0, 17.0, 16.0, 15.0, 14.0, 13.0, 12.0, 11.0, 10.0, 9.0, 8.0, 7.0, 6.0, 5.0, 4.0, 3.0, 2.0, 0.0};
	double faceArea[20] = {0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0, 1.1, 1.2, 1.3, 1.4, 1.5, 1.6, 1.7, 1.8, 1.9, 0.0};
	double faceCenterX[20] = {1.0, 1.1, 1.2, 1.3, 1.4, 1.5, 1.6, 1.7, 1.8, 1.9, 2.0, 2.1, 2.2, 2.3, 2.4, 2.5, 2.6, 2.7, 2.8, 0.0};
	double faceCenterY[20] = {1.0, 1.1, 1.2, 1.3, 1.4, 1.5, 1.6, 1.7, 1.8, 2.87, 2.0, 2.1, 2.2, 2.3, 2.4, 2.5, 2.6, 2.7, 2.8, 0.0};
	double faceCenterZ[20] = {4.0, 1.1, 1.2, 1.3, 1.4, 1.5, 1.6, 1.7, 7.8, 1.9, 2.0, 2.1, 2.2, 2.3, 4.4, 2.5, 2.6, 2.7, 2.8, 0.0};
	double faceNormX[20] = {1.0, 6.3, 1.2, 1.3, 1.4, 1.5, 1.6, 1.7, 1.8, 1.9, 2.0, 15.7, 2.2, 2.3, 2.4, 2.5, 2.6, 2.7, 2.8, 0.0};
	double faceNormY[20] = {1.0, 1.1, 9.8, 1.3, 1.4, 1.5, 1.6, 1.7, 1.8, 1.9, 2.0, 2.1, 2.2, 2.3, 2.4, 2.5, 2.6, 2.7, 2.8, 0.0};
	double faceNormZ[20] = {1.0, 1.1, 1.2, 1.3, 1.4, 1.5, 1.6, 1.7, 1.8, 10.54, 2.0, 2.1, 2.2, 2.3, 2.4, 2.5, 2.6, 2.7, 2.8, 0.0};
	double faceXnacX[20] = {1.0, 1.1, 1.2, 7.6, 1.4, 1.5, 1.6, 1.7, 1.8, 1.9, 2.0, 2.1, 2.2, 2.3, 2.4, 2.5, 2.6, 2.7, 2.8, 0.0};
	double faceXnacY[20] = {1.0, 1.1, 1.2, 1.3, 1.4, 1.5, 1.6, 1.7, 1.8, 1.9, 2.0, 2.1, 2.2, 2.3, 2.4, 2.5, 2.6, 5.9, 2.8, 0.0};
	double faceXnacZ[20] = {1.0, 1.1, 1.2, 1.3, 180.9, 1.5, 1.6, 1.7, 1.8, 1.9, 2.0, 2.1, 2.2, 2.3, 2.4, 2.5, 2.6, 2.7, 2.8, 0.0};
	double faceXpacX[20] = {1.0, 1.1, 1.2, 1.3, 1.4, 1.5, 1.6, 1.7, 1.8, 180.7, 2.0, 2.1, 2.2, 2.3, 2.4, 2.5, 2.6, 2.7, 2.8, 0.0};
	double faceXpacY[20] = {1.0, 1.1, 1.2, 1.3, 14.6, 1.5, 1.6, 1.7, 1.8, 1.9, 2.0, 2.1, 2.2, 2.3, 2.4, 2.5, 2.6, 2.7, 2.8, 0.0};
	double faceXpacZ[20] = {1.0, 1.1, 1.2, 1.3, 1.4, 1.5, 1.6, 1.7, 1.8, 1.9, 17.89, 2.1, 2.2, 2.3, 2.4, 2.5, 2.6, 2.7, 2.8, 0.0};

	if(comm.rank == 0)
	{
		keepFace[0] = true;
		keepFace[1] = true;
		keepFace[2] = true;
		keepFace[3] = true;
		keepFace[4] = true;
		keepFace[17] = true;
	}
	else if(comm.rank == 1)
	{
		keepFace[5] = true;
		keepFace[6] = true;
		keepFace[7] = true;
		keepFace[17] = true;
		keepFace[18] = true;
		keepFace[19] = true;
	}
	else if(comm.rank == 2)
	{
		keepFace[18] = true;
		keepFace[8] = true;
		keepFace[9] = true;
		keepFace[10] = true;
		keepFace[11] = true;
	}
	else if(comm.rank == 3)
	{
		keepFace[19] = true;
		keepFace[12] = true;
		keepFace[13] = true;
		keepFace[14] = true;
		keepFace[15] = true;
		keepFace[16] = true;
	}

	for(int i = 0; i < 20; i++)
	{
		if(keepFace[i] == true)
		{
			// Use region 0 for all
			int vert[4] = {vertLabel[faceVert1[i]], vertLabel[faceVert2[i]], vertLabel[faceVert3[i]], -1};
			int count = 3;

			// add the 4th vertex if it exists
			if(faceVert4[i] != - 1)
			{
				count = 4;
				vert[3] = vertLabel[faceVert4[i]];
			}

			if(i == 19)
			{
				// Default add
				if(isBoundary[i])
				{
					status = mesh.addFace(faceLabel[i], cellLabel[faceCell1[i]], bndLabel[faceCell2OrBoundary[i]], isBoundary[i], vert, count);
					BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
				}
				else
				{
					status = mesh.addFace(faceLabel[i], cellLabel[faceCell1[i]], cellLabel[faceCell2OrBoundary[i]], isBoundary[i], vert, count);
					BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
				}
			}
			else
			{
				if(isBoundary[i])
				{
					cupcfd::geometry::euclidean::EuclideanVector<double,3> norm(faceNormX[i], faceNormY[i], faceNormZ[i]);
					cupcfd::geometry::euclidean::EuclideanPoint<double,3> center(faceCenterX[i], faceCenterY[i], faceCenterZ[i]);
					cupcfd::geometry::euclidean::EuclideanPoint<double,3> xpac(faceXpacX[i], faceXpacY[i], faceXpacZ[i]);
					cupcfd::geometry::euclidean::EuclideanPoint<double,3> xnac(faceXnacX[i], faceXnacY[i], faceXnacZ[i]);

					status = mesh.addFace(faceLabel[i], cellLabel[faceCell1[i]], bndLabel[faceCell2OrBoundary[i]], isBoundary[i],
										  faceLambda[i], norm, vert, count, center, xpac, xnac, faceRLencos[i], faceArea[i]);
					BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
				}
				else
				{
					cupcfd::geometry::euclidean::EuclideanVector<double,3> norm(faceNormX[i], faceNormY[i], faceNormZ[i]);
					cupcfd::geometry::euclidean::EuclideanPoint<double,3> center(faceCenterX[i], faceCenterY[i], faceCenterZ[i]);
					cupcfd::geometry::euclidean::EuclideanPoint<double,3> xpac(faceXpacX[i], faceXpacY[i], faceXpacZ[i]);
					cupcfd::geometry::euclidean::EuclideanPoint<double,3> xnac(faceXnacX[i], faceXnacY[i], faceXnacZ[i]);

					status = mesh.addFace(faceLabel[i], cellLabel[faceCell1[i]], cellLabel[faceCell2OrBoundary[i]], isBoundary[i],
										  faceLambda[i], norm, vert, count, center, xpac, xnac, faceRLencos[i], faceArea[i]);
					BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
				}
			}
		}
	}

	// Test getters functions without error codes
	for(int i = 0; i < 20; i++)
	{
		if(keepFace[i])
		{
			int localID = mesh.getFaceID(faceLabel[i]);

			// (1) Test Face Values
			BOOST_CHECK_EQUAL(mesh.getFaceLambda(localID), faceLambda[i]);
			BOOST_CHECK_EQUAL(mesh.getFaceRLencos(localID), faceRLencos[i]);
			BOOST_CHECK_EQUAL(mesh.getFaceArea(localID), faceArea[i]);

			cupcfd::geometry::euclidean::EuclideanVector<double,3> norm = mesh.getFaceNorm(localID);
			cupcfd::geometry::euclidean::EuclideanPoint<double,3> center = mesh.getFaceCenter(localID);
			cupcfd::geometry::euclidean::EuclideanPoint<double,3> xpac = mesh.getFaceXpac(localID);
			cupcfd::geometry::euclidean::EuclideanPoint<double,3> xnac = mesh.getFaceXnac(localID);
			int nVertices = mesh.getFaceNVertices(localID);

			BOOST_CHECK_EQUAL(norm.cmp[0], faceNormX[i]);
			BOOST_CHECK_EQUAL(norm.cmp[1], faceNormY[i]);
			BOOST_CHECK_EQUAL(norm.cmp[2], faceNormZ[i]);

			BOOST_CHECK_EQUAL(center.cmp[0], faceCenterX[i]);
			BOOST_CHECK_EQUAL(center.cmp[1], faceCenterY[i]);
			BOOST_CHECK_EQUAL(center.cmp[2], faceCenterZ[i]);

			BOOST_CHECK_EQUAL(xpac.cmp[0], faceXpacX[i]);
			BOOST_CHECK_EQUAL(xpac.cmp[1], faceXpacY[i]);
			BOOST_CHECK_EQUAL(xpac.cmp[2], faceXpacZ[i]);

			BOOST_CHECK_EQUAL(xnac.cmp[0], faceXnacX[i]);
			BOOST_CHECK_EQUAL(xnac.cmp[1], faceXnacY[i]);
			BOOST_CHECK_EQUAL(xnac.cmp[2], faceXnacZ[i]);

			BOOST_CHECK_EQUAL(nVertices, faceNVertices[i]);

			// (2) Test Face References - the internal references should be the same as the label->id mapping for a component
			// This will be face->cell1, face->boundary and face->cell2

			// Check the stored local cell ID is the same as the local ID for tcupcfd::geometry::euclidean::EuclideanPoint<T,3>(T(0), T(0), T(0));he cell label
			BOOST_CHECK_EQUAL(mesh.getFaceCell1ID(localID), mesh.getCellID(cellLabel[faceCell1[i]]));

			if(isBoundary[i])
			{
				BOOST_CHECK_EQUAL(mesh.getFaceCell2ID(localID), -1);
				BOOST_CHECK_EQUAL(mesh.getFaceBoundaryID(localID), mesh.getBoundaryID(bndLabel[faceCell2OrBoundary[i]]));
			}
			else
			{
				BOOST_CHECK_EQUAL(mesh.getFaceCell2ID(localID), mesh.getCellID(cellLabel[faceCell2OrBoundary[i]]));
				BOOST_CHECK_EQUAL(mesh.getFaceBoundaryID(localID), -1);
			}

			BOOST_CHECK_EQUAL(mesh.getFaceVertex(localID,0), mesh.getVertexID(vertLabel[faceVert1[i]]));
			BOOST_CHECK_EQUAL(mesh.getFaceVertex(localID,1), mesh.getVertexID(vertLabel[faceVert2[i]]));
			BOOST_CHECK_EQUAL(mesh.getFaceVertex(localID,2), mesh.getVertexID(vertLabel[faceVert3[i]]));

			if(faceVert4[i] != -1)
			{
				BOOST_CHECK_EQUAL(mesh.getFaceVertex(localID,3), mesh.getVertexID(vertLabel[faceVert4[i]]));
			}
			else
			{
				BOOST_CHECK_EQUAL(mesh.getFaceVertex(localID,3), -1);
			}
		}
	}

	// ToDo - Test getters functions with error codes

	// Check Mesh Local Properties are Updated - Global can only be updated at finalize
	switch(comm.rank)
	{
		case 0:	BOOST_CHECK_EQUAL(mesh.properties.lFaces, 6);
				break;

		case 1:	BOOST_CHECK_EQUAL(mesh.properties.lFaces, 6);
				break;

		case 2:	BOOST_CHECK_EQUAL(mesh.properties.lFaces, 5);
				break;

		case 3:	BOOST_CHECK_EQUAL(mesh.properties.lFaces, 6);
				break;
	}
}

// Test 2: Test overwriting values
BOOST_AUTO_TEST_CASE(addFace_test2)
{

}

// Test 3: Test adding a duplicate face label on this rank
BOOST_AUTO_TEST_CASE(addFace_test3)
{
	// Setup
	cupcfd::comm::Communicator comm(MPI_COMM_WORLD);
	CupCfdAoSMesh<int,double,int> mesh(comm);
	cupcfd::error::eCodes status;

	// Add Vertices
	int vertLabel[4] = {0, 1, 2, 3};
	double vertX[4] = {0.0, 0.5, 1.0, 1.5};
	double vertY[4] = {0.0, 0.0, 0.0, 0.0};
	double vertZ[4] = {0.0, 0.0, 0.0, 0.0};

	for(int i = 0; i < 4; i++)
	{
		cupcfd::geometry::euclidean::EuclideanPoint<double,3> point(vertX[i], vertY[i], vertZ[i]);
		status = mesh.addVertex(vertLabel[i], point);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	}

	// Add Three Cells
	status = mesh.addCell(0, true);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	status = mesh.addCell(1, true);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	status = mesh.addCell(2, true);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	// Add Face between cell 0 and cell 1
	int vert1[3] = {0, 1, 2};
	status = mesh.addFace(0, 0, 1, false, vert1, 3);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	// Add Face between cell 1 and cell2, but with same face label, use different vert set
	int vert2[3] = {0, 1, 3};
	status = mesh.addFace(0, 1, 2, false, vert2, 3);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_MESH_EXISTING_FACE);
}

// Test 4: Error Check: Using a non-existing cell build ID
BOOST_AUTO_TEST_CASE(addFace_test4)
{
	// Setup
	cupcfd::comm::Communicator comm(MPI_COMM_WORLD);
	CupCfdAoSMesh<int,double,int> mesh(comm);
	cupcfd::error::eCodes status;

	// Add Vertices
	int vertLabel[4] = {0, 1, 2, 3};
	double vertX[4] = {0.0, 0.5, 1.0, 1.5};
	double vertY[4] = {0.0, 0.0, 0.0, 0.0};
	double vertZ[4] = {0.0, 0.0, 0.0, 0.0};

	for(int i = 0; i < 4; i++)
	{
		cupcfd::geometry::euclidean::EuclideanPoint<double,3> point(vertX[i], vertY[i], vertZ[i]);
		status = mesh.addVertex(vertLabel[i], point);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	}

	// Add Two Cells
	status = mesh.addCell(0, true);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	status = mesh.addCell(1, true);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	// Add Face between cell 0 and non-existant cell 4
	int vert1[3] = {0, 1, 2};
	status = mesh.addFace(0, 0, 4, false, vert1, 3);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_MESH_INVALID_CELL_LABEL);
}

// Test 5: Error Check: Using a non-existing boundary build ID
BOOST_AUTO_TEST_CASE(addFace_test5)
{
	// Setup
	cupcfd::comm::Communicator comm(MPI_COMM_WORLD);
	CupCfdAoSMesh<int,double,int> mesh(comm);
	cupcfd::error::eCodes status;

	// Add Vertices
	int vertLabel[4] = {0, 1, 2, 3};
	double vertX[4] = {0.0, 0.5, 1.0, 1.5};
	double vertY[4] = {0.0, 0.0, 0.0, 0.0};
	double vertZ[4] = {0.0, 0.0, 0.0, 0.0};

	for(int i = 0; i < 4; i++)
	{
		cupcfd::geometry::euclidean::EuclideanPoint<double,3> point(vertX[i], vertY[i], vertZ[i]);
		status = mesh.addVertex(vertLabel[i], point);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	}

	// Add Two Cells
	status = mesh.addCell(0, true);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	status = mesh.addCell(1, true);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	// Add Face between cell 0 and non-existant boundary 5
	int vert1[3] = {0, 1, 2};
	status = mesh.addFace(0, 0, 5, true, vert1, 3);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_MESH_INVALID_BOUNDARY_LABEL);
}

// Test 6: Error Check: Using a non-existing vertex build ID
BOOST_AUTO_TEST_CASE(addFace_test6)
{
	// Setup
	cupcfd::comm::Communicator comm(MPI_COMM_WORLD);
	CupCfdAoSMesh<int,double,int> mesh(comm);
	cupcfd::error::eCodes status;

	// Add Vertices
	int vertLabel[4] = {0, 1, 2, 3};
	double vertX[4] = {0.0, 0.5, 1.0, 1.5};
	double vertY[4] = {0.0, 0.0, 0.0, 0.0};
	double vertZ[4] = {0.0, 0.0, 0.0, 0.0};

	for(int i = 0; i < 4; i++)
	{
		cupcfd::geometry::euclidean::EuclideanPoint<double,3> point(vertX[i], vertY[i], vertZ[i]);
		status = mesh.addVertex(vertLabel[i], point);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	}

	// Add Two Cells
	status = mesh.addCell(0, true);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	status = mesh.addCell(1, true);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	// Add Face between cell 0 and cell 1, but with a non-existant vertex label  8
	int vert1[3] = {0, 1, 8};
	status = mesh.addFace(0, 0, 1, false, vert1, 3);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_MESH_INVALID_VERTEX_LABEL);
}

// Test 7: Too few vertices
BOOST_AUTO_TEST_CASE(addFace_test7)
{
	// Setup
	cupcfd::comm::Communicator comm(MPI_COMM_WORLD);
	CupCfdAoSMesh<int,double,int> mesh(comm);
	cupcfd::error::eCodes status;

	// Add Vertices
	int vertLabel[4] = {0, 1, 2, 3};
	double vertX[4] = {0.0, 0.5, 1.0, 1.5};
	double vertY[4] = {0.0, 0.0, 0.0, 0.0};
	double vertZ[4] = {0.0, 0.0, 0.0, 0.0};

	for(int i = 0; i < 4; i++)
	{
		cupcfd::geometry::euclidean::EuclideanPoint<double,3> point(vertX[i], vertY[i], vertZ[i]);
		status = mesh.addVertex(vertLabel[i], point);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	}

	// Add Two Cells
	status = mesh.addCell(0, true);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	status = mesh.addCell(1, true);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	// Add Face between cell 0 and cell 1, but with only two vertices
	int vert1[2] = {0, 1};
	status = mesh.addFace(0, 0, 1, false, vert1, 2);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_MESH_INVALID_VERTEX_COUNT);
}

// Test 8: Too many vertices
BOOST_AUTO_TEST_CASE(addFace_test8)
{
	// Setup
	cupcfd::comm::Communicator comm(MPI_COMM_WORLD);
	CupCfdAoSMesh<int,double,int> mesh(comm);
	cupcfd::error::eCodes status;

	// Add Vertices
	int vertLabel[5] = {0, 1, 2, 3, 4};
	double vertX[5] = {0.0, 0.5, 1.0, 1.5, 10.0};
	double vertY[5] = {0.0, 0.0, 0.0, 0.0, 20.0};
	double vertZ[5] = {0.0, 0.0, 0.0, 0.0, 30.0};

	for(int i = 0; i < 5; i++)
	{
		cupcfd::geometry::euclidean::EuclideanPoint<double,3> point(vertX[i], vertY[i], vertZ[i]);
		status = mesh.addVertex(vertLabel[i], point);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	}

	// Add Two Cells
	status = mesh.addCell(0, true);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	status = mesh.addCell(1, true);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	// Add Face between cell 0 and cell 1, but with only two vertices
	int vert1[5] = {0, 1, 2, 3, 4};
	status = mesh.addFace(0, 0, 1, false, vert1, 5);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_MESH_INVALID_VERTEX_COUNT);
}

// Test 9: Error Case: Edge/Face Already Exists (i.e. cell1, cell2 combo exists)
BOOST_AUTO_TEST_CASE(addFace_test9)
{
	// Setup
	cupcfd::comm::Communicator comm(MPI_COMM_WORLD);
	CupCfdAoSMesh<int,double,int> mesh(comm);
	cupcfd::error::eCodes status;

	// Add Vertices
	int vertLabel[4] = {0, 1, 2, 3};
	double vertX[4] = {0.0, 0.5, 1.0, 1.5};
	double vertY[4] = {0.0, 0.0, 0.0, 0.0};
	double vertZ[4] = {0.0, 0.0, 0.0, 0.0};

	for(int i = 0; i < 4; i++)
	{
		cupcfd::geometry::euclidean::EuclideanPoint<double,3> point(vertX[i], vertY[i], vertZ[i]);
		status = mesh.addVertex(vertLabel[i], point);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	}

	// Add Three Cells
	status = mesh.addCell(0, true);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	status = mesh.addCell(1, true);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	status = mesh.addCell(2, true);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	// Add Face between cell 0 and cell 1
	int vert1[3] = {0, 1, 2};
	status = mesh.addFace(0, 0, 1, false, vert1, 3);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	// Add Face between cell 0 and cell 1 again, different face label but same edge
	status = mesh.addFace(1, 0, 1, false, vert1, 3);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_MESH_FACE_EDGE_EXISTS);
}

// === Finalize ===
// Test 1: Test successful build, check properties are correct, check we can build cell Polyhedron types
BOOST_AUTO_TEST_CASE(finalize_test1)
{
	// Setup - Create a very simple mesh for 4 cells
	// Properties of components - volume, norm, center etc can be arbitrary - we are only testing these for
	// positioning by checking they are attached to the correct components before and after reordering.
	// The only things that must be correct are the component relationships - i.e. face->boundary mappings,
	// face-> vertex etc etc.

	// Setup
	cupcfd::comm::Communicator comm(MPI_COMM_WORLD);
	CupCfdAoSMesh<int,double,int> mesh(comm);
	cupcfd::error::eCodes status;

	// Add Regions
	std::string name = "Default";
	status = mesh.addRegion(0, name);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	// ToDo: Strictly speaking, not all vertices and boundaries are needed on all ranks for this test.
	// We can whittle them down.

	// Add Vertices
	int vertLabel[18] = {5, 10, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 65, 70, 75, 80, 85, 90};
	double vertX[18] = {0.0, 0.5, 1.0, 1.5, 0.0, 0.5, 1.0, 1.5, 0.5, 1.0, 0.0, 0.5, 1.0, 0.0, 0.5, 1.0, 0.5, 1.0};
	double vertY[18] = {0.0, 0.0, 0.0, 0.0, 0.5, 0.5, 0.5, 0.5, 1.0, 1.0, 0.0, 0.0, 0.0, 0.5, 0.5, 0.5, 1.0, 1.0};
	double vertZ[18] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5};

	for(int i = 0; i < 18; i++)
	{
		cupcfd::geometry::euclidean::EuclideanPoint<double,3> point(vertX[i], vertY[i], vertZ[i]);
		status = mesh.addVertex(vertLabel[i], point);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	}

	// Add Boundaries
	int bndLabel[17] = {11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27};
	int bndVert1[17] = {0,  0, 0,  4,  10, 1, 1,  11, 2, 2,  3,  6,  6,  8,  5,  5, 14};
	int bndVert2[17] = {4,  1, 1,  5,  11, 2, 2,  12, 3, 3,  7,  7,  9,  9,  8,  6, 15};
	int bndVert3[17] = {10, 4, 10, 13, 13, 5, 11, 14, 6, 12, 12, 15, 15, 16, 14, 8, 16};
	int bndVert4[17] = {13, 5, 11, 14, 14, 6, 12, 15, 7, -1, 15, -1, 17, 17, 16, 9, 17};
	int bndFace[17] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};

	int keepBnd[17] = {false, false, false, false, false, false, false, false, false, false,
						false, false, false, false, false, false, false};

	if(comm.rank == 0)
	{
		keepBnd[0] = true;
		keepBnd[1] = true;
		keepBnd[2] = true;
		keepBnd[3] = true;
		keepBnd[4] = true;
	}
	else if(comm.rank == 1)
	{
		keepBnd[5] = true;
		keepBnd[6] = true;
		keepBnd[7] = true;
	}
	else if(comm.rank == 2)
	{
		keepBnd[8] = true;
		keepBnd[9] = true;
		keepBnd[10] = true;
		keepBnd[11] = true;
	}
	else if(comm.rank == 3)
	{
		keepBnd[12] = true;
		keepBnd[13] = true;
		keepBnd[14] = true;
		keepBnd[15] = true;
		keepBnd[16] = true;
	}

	for(int i = 0; i < 17; i++)
	{
		if(keepBnd[i])
		{
			// Use region 0 for all
			int vert[4] = {vertLabel[bndVert1[i]], vertLabel[bndVert2[i]], vertLabel[bndVert3[i]], -1};
			int count = 3;

			// add the 4th vertex if it exists
			if(bndVert4[i] != - 1)
			{
				count = 4;
				vert[3] = vertLabel[bndVert4[i]];
			}

			status = mesh.addBoundary(bndLabel[i], 0, vert, count);
			BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
		}
	}

	int cellLabel[4] = {100, 200, 300, 400};
	double cellVol[4] = {1.0, 2.0, 3.0, 4.0};
	double cellCenterX[4] = {5.0, 10.0, 15.0, 25.0};
	double cellCenterY[4] = {15.0, 110.0, 115.0, 125.0};
	double cellCenterZ[4] = {25.0, 210.0, 215.0, 225.0};
	double keepCell[4] = {false, false, false, false};

	// Add Cells
	// Each Rank gets 1 local cell
	if(comm.rank == 0)
	{
		cupcfd::geometry::euclidean::EuclideanPoint<double,3> center;

		center = cupcfd::geometry::euclidean::EuclideanPoint<double,3>(cellCenterX[0], cellCenterY[0], cellCenterZ[0]);
		status = mesh.addCell(cellLabel[0], center, cellVol[0], true);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
		keepCell[0] = true;

		center = cupcfd::geometry::euclidean::EuclideanPoint<double,3>(cellCenterX[1], cellCenterY[1], cellCenterZ[1]);
		status = mesh.addCell(cellLabel[1], center, cellVol[1], false);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
		keepCell[1] = true;
	}
	else if(comm.rank == 1)
	{
		cupcfd::geometry::euclidean::EuclideanPoint<double,3> center;

		center = cupcfd::geometry::euclidean::EuclideanPoint<double,3>(cellCenterX[0], cellCenterY[0], cellCenterZ[0]);
		status = mesh.addCell(cellLabel[0], center, cellVol[0], false);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
		keepCell[0] = true;

		center = cupcfd::geometry::euclidean::EuclideanPoint<double,3>(cellCenterX[3], cellCenterY[3], cellCenterZ[3]);
		status = mesh.addCell(cellLabel[3], center, cellVol[3], false);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
		keepCell[1] = true;

		center = cupcfd::geometry::euclidean::EuclideanPoint<double,3>(cellCenterX[1], cellCenterY[1], cellCenterZ[1]);
		status = mesh.addCell(cellLabel[1], center, cellVol[1], true);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
		keepCell[2] = true;

		center = cupcfd::geometry::euclidean::EuclideanPoint<double,3>(cellCenterX[2], cellCenterY[2], cellCenterZ[2]);
		status = mesh.addCell(cellLabel[2], center, cellVol[2], false);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
		keepCell[3] = true;
	}
	else if(comm.rank == 2)
	{
		cupcfd::geometry::euclidean::EuclideanPoint<double,3> center;

		center = cupcfd::geometry::euclidean::EuclideanPoint<double,3>(cellCenterX[2], cellCenterY[2], cellCenterZ[2]);
		status = mesh.addCell(cellLabel[2], center, cellVol[2], true);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
		keepCell[2] = true;

		center = cupcfd::geometry::euclidean::EuclideanPoint<double,3>(cellCenterX[1], cellCenterY[1], cellCenterZ[1]);
		status = mesh.addCell(cellLabel[1], center, cellVol[1], false);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
		keepCell[1] = true;
	}
	else if(comm.rank == 3)
	{
		cupcfd::geometry::euclidean::EuclideanPoint<double,3> center;

		// Add ghost cell first so we can check reordering after finalize.

		center = cupcfd::geometry::euclidean::EuclideanPoint<double,3>(cellCenterX[1], cellCenterY[1], cellCenterZ[1]);
		status = mesh.addCell(cellLabel[1], center, cellVol[1], false);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
		keepCell[1] = true;

		center = cupcfd::geometry::euclidean::EuclideanPoint<double,3>(cellCenterX[3], cellCenterY[3], cellCenterZ[3]);
		status = mesh.addCell(cellLabel[3], center, cellVol[3], true);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
		keepCell[3] = true;
	}

	// Add Faces
	// For convienience, same order as boundaries + 3 non-boundary faces at end
	// This means face index = boundary index where applicable
	int faceLabel[20] = {10, 20, 30, 40, 50, 60, 70, 80, 90, 100, 110, 120, 130, 140, 150, 160, 170, 180, 190, 200};
	int faceVert1[20] = {0,  0, 0,  4,  10, 1, 1,  11, 2, 2,  3,  6,  6,  8,  5,  5, 14, 1,  2,  5};
	int faceVert2[20] = {4,  1, 1,  5,  11, 2, 2,  12, 3, 3,  7,  7,  9,  9,  8,  6, 15, 5,  6,  6};
	int faceVert3[20] = {10, 4, 10, 13, 13, 5, 11, 14, 6, 12, 12, 15, 15, 16, 14, 8, 16, 11, 12, 14};
	int faceVert4[20] = {13, 5, 11, 14, 14, 6, 12, 15, 7, -1, 15, -1, 17, 17, 16, 9, 17, 14, 15, 15};
	int faceCell1[20] = {0, 0, 0, 0, 0, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 3, 0, 1, 1};
	int faceCell2OrBoundary[20] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 1, 2, 3};
	int isBoundary[20] = {true, true, true, true, true, true, true, true, true, true, true, true, true,
						  true, true, true, true, false, false, false};

	int keepFace[20] = {false, false, false, false, false, false, false, false, false, false,
						false, false, false, false, false, false, false, false, false, false};

	// Leave the last face as a default value face
	double faceLambda[20] = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0, 11.0, 12.0, 13.0, 14.0, 15.0, 16.0, 17.0, 18.0, 19.0, 0.0};
	double faceRLencos[20] = {20.0, 19.0, 18.0, 17.0, 16.0, 15.0, 14.0, 13.0, 12.0, 11.0, 10.0, 9.0, 8.0, 7.0, 6.0, 5.0, 4.0, 3.0, 2.0, 0.0};
	double faceArea[20] = {0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0, 1.1, 1.2, 1.3, 1.4, 1.5, 1.6, 1.7, 1.8, 1.9, 0.0};
	double faceCenterX[20] = {1.0, 1.1, 1.2, 1.3, 1.4, 1.5, 1.6, 1.7, 1.8, 1.9, 2.0, 2.1, 2.2, 2.3, 2.4, 2.5, 2.6, 2.7, 2.8, 0.0};
	double faceCenterY[20] = {1.0, 1.1, 1.2, 1.3, 1.4, 1.5, 1.6, 1.7, 1.8, 2.87, 2.0, 2.1, 2.2, 2.3, 2.4, 2.5, 2.6, 2.7, 2.8, 0.0};
	double faceCenterZ[20] = {4.0, 1.1, 1.2, 1.3, 1.4, 1.5, 1.6, 1.7, 7.8, 1.9, 2.0, 2.1, 2.2, 2.3, 4.4, 2.5, 2.6, 2.7, 2.8, 0.0};
	double faceNormX[20] = {1.0, 6.3, 1.2, 1.3, 1.4, 1.5, 1.6, 1.7, 1.8, 1.9, 2.0, 15.7, 2.2, 2.3, 2.4, 2.5, 2.6, 2.7, 2.8, 0.0};
	double faceNormY[20] = {1.0, 1.1, 9.8, 1.3, 1.4, 1.5, 1.6, 1.7, 1.8, 1.9, 2.0, 2.1, 2.2, 2.3, 2.4, 2.5, 2.6, 2.7, 2.8, 0.0};
	double faceNormZ[20] = {1.0, 1.1, 1.2, 1.3, 1.4, 1.5, 1.6, 1.7, 1.8, 10.54, 2.0, 2.1, 2.2, 2.3, 2.4, 2.5, 2.6, 2.7, 2.8, 0.0};
	double faceXnacX[20] = {1.0, 1.1, 1.2, 7.6, 1.4, 1.5, 1.6, 1.7, 1.8, 1.9, 2.0, 2.1, 2.2, 2.3, 2.4, 2.5, 2.6, 2.7, 2.8, 0.0};
	double faceXnacY[20] = {1.0, 1.1, 1.2, 1.3, 1.4, 1.5, 1.6, 1.7, 1.8, 1.9, 2.0, 2.1, 2.2, 2.3, 2.4, 2.5, 2.6, 5.9, 2.8, 0.0};
	double faceXnacZ[20] = {1.0, 1.1, 1.2, 1.3, 180.9, 1.5, 1.6, 1.7, 1.8, 1.9, 2.0, 2.1, 2.2, 2.3, 2.4, 2.5, 2.6, 2.7, 2.8, 0.0};
	double faceXpacX[20] = {1.0, 1.1, 1.2, 1.3, 1.4, 1.5, 1.6, 1.7, 1.8, 180.7, 2.0, 2.1, 2.2, 2.3, 2.4, 2.5, 2.6, 2.7, 2.8, 0.0};
	double faceXpacY[20] = {1.0, 1.1, 1.2, 1.3, 14.6, 1.5, 1.6, 1.7, 1.8, 1.9, 2.0, 2.1, 2.2, 2.3, 2.4, 2.5, 2.6, 2.7, 2.8, 0.0};
	double faceXpacZ[20] = {1.0, 1.1, 1.2, 1.3, 1.4, 1.5, 1.6, 1.7, 1.8, 1.9, 17.89, 2.1, 2.2, 2.3, 2.4, 2.5, 2.6, 2.7, 2.8, 0.0};

	if(comm.rank == 0)
	{
		keepFace[0] = true;
		keepFace[1] = true;
		keepFace[2] = true;
		keepFace[3] = true;
		keepFace[4] = true;
		keepFace[17] = true;
	}
	else if(comm.rank == 1)
	{
		keepFace[5] = true;
		keepFace[6] = true;
		keepFace[7] = true;
		keepFace[17] = true;
		keepFace[18] = true;
		keepFace[19] = true;
	}
	else if(comm.rank == 2)
	{
		keepFace[18] = true;
		keepFace[8] = true;
		keepFace[9] = true;
		keepFace[10] = true;
		keepFace[11] = true;
	}
	else if(comm.rank == 3)
	{
		keepFace[19] = true;
		keepFace[12] = true;
		keepFace[13] = true;
		keepFace[14] = true;
		keepFace[15] = true;
		keepFace[16] = true;
	}

	for(int i = 0; i < 20; i++)
	{
		if(keepFace[i] == true)
		{
			// Use region 0 for all
			int vert[4] = {vertLabel[faceVert1[i]], vertLabel[faceVert2[i]], vertLabel[faceVert3[i]], -1};
			int count = 3;

			// add the 4th vertex if it exists
			if(faceVert4[i] != - 1)
			{
				count = 4;
				vert[3] = vertLabel[faceVert4[i]];
			}

			if(isBoundary[i])
			{
				cupcfd::geometry::euclidean::EuclideanVector<double,3> norm(faceNormX[i], faceNormY[i], faceNormZ[i]);
				cupcfd::geometry::euclidean::EuclideanPoint<double,3> center(faceCenterX[i], faceCenterY[i], faceCenterZ[i]);
				cupcfd::geometry::euclidean::EuclideanPoint<double,3> xpac(faceXpacX[i], faceXpacY[i], faceXpacZ[i]);
				cupcfd::geometry::euclidean::EuclideanPoint<double,3> xnac(faceXnacX[i], faceXnacY[i], faceXnacZ[i]);

				status = mesh.addFace(faceLabel[i], cellLabel[faceCell1[i]], bndLabel[faceCell2OrBoundary[i]], isBoundary[i],
									  faceLambda[i], norm, vert, count, center, xpac, xnac, faceRLencos[i], faceArea[i]);
				BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
			}
			else
			{
				cupcfd::geometry::euclidean::EuclideanVector<double,3> norm(faceNormX[i], faceNormY[i], faceNormZ[i]);
				cupcfd::geometry::euclidean::EuclideanPoint<double,3> center(faceCenterX[i], faceCenterY[i], faceCenterZ[i]);
				cupcfd::geometry::euclidean::EuclideanPoint<double,3> xpac(faceXpacX[i], faceXpacY[i], faceXpacZ[i]);
				cupcfd::geometry::euclidean::EuclideanPoint<double,3> xnac(faceXnacX[i], faceXnacY[i], faceXnacZ[i]);

				status = mesh.addFace(faceLabel[i], cellLabel[faceCell1[i]], cellLabel[faceCell2OrBoundary[i]], isBoundary[i],
									  faceLambda[i], norm, vert, count, center, xpac, xnac, faceRLencos[i], faceArea[i]);
				BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
			}
		}
	}

	status = mesh.finalize();
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	// === Checks ===

	// === Check Mesh Properties are Updated - Global is only updated at finalize ===
	switch(comm.rank)
	{
		case 0:	BOOST_CHECK_EQUAL(mesh.properties.lTCells, 2);
				BOOST_CHECK_EQUAL(mesh.properties.lOCells, 1);
				BOOST_CHECK_EQUAL(mesh.properties.lGhCells, 1);
				BOOST_CHECK_EQUAL(mesh.properties.lFaces, 6);
				BOOST_CHECK_EQUAL(mesh.properties.lBoundaries, 5);

				// Note: For this test we haven't bothered to filter out unused vertices
				BOOST_CHECK_EQUAL(mesh.properties.lVertices, 18);
				break;

		case 1:	BOOST_CHECK_EQUAL(mesh.properties.lTCells, 4);
				BOOST_CHECK_EQUAL(mesh.properties.lOCells, 1);
				BOOST_CHECK_EQUAL(mesh.properties.lGhCells, 3);
				BOOST_CHECK_EQUAL(mesh.properties.lFaces, 6);
				BOOST_CHECK_EQUAL(mesh.properties.lBoundaries, 3);

				// Note: For this test we haven't bothered to filter out unused vertices
				BOOST_CHECK_EQUAL(mesh.properties.lVertices, 18);
				break;

		case 2:	BOOST_CHECK_EQUAL(mesh.properties.lTCells, 2);
				BOOST_CHECK_EQUAL(mesh.properties.lOCells, 1);
				BOOST_CHECK_EQUAL(mesh.properties.lGhCells, 1);
				BOOST_CHECK_EQUAL(mesh.properties.lFaces, 5);
				BOOST_CHECK_EQUAL(mesh.properties.lBoundaries, 4);

				// Note: For this test we haven't bothered to filter out unused vertices
				BOOST_CHECK_EQUAL(mesh.properties.lVertices, 18);
				break;

		case 3:	BOOST_CHECK_EQUAL(mesh.properties.lTCells, 2);
				BOOST_CHECK_EQUAL(mesh.properties.lOCells, 1);
				BOOST_CHECK_EQUAL(mesh.properties.lGhCells, 1);
				BOOST_CHECK_EQUAL(mesh.properties.lFaces, 6);
				BOOST_CHECK_EQUAL(mesh.properties.lBoundaries, 5);

				// Note: For this test we haven't bothered to filter out unused vertices
				BOOST_CHECK_EQUAL(mesh.properties.lVertices, 18);
				break;
	}

	// === Check Cell Local IDs in Mesh match the local IDs in the connectivity graph ===

	for(int i = 0; i < 4; i++)
	{
		if(keepCell[i])
		{
			int meshLocalID;
			int graphLocalID;

			meshLocalID = mesh.getCellID(cellLabel[i]);
			mesh.cellConnGraph->connGraph.getNodeLocalIndex(cellLabel[i], &graphLocalID);
			BOOST_CHECK_EQUAL(meshLocalID, graphLocalID);
		}
	}

	// === Check that the cell data still matches up correctly for each cell label (i.e. it was reordered correctly) ===

	for(int i = 0; i < 4; i++)
	{
		if(keepCell[i])
		{
			// Cell is on this rank, check the values
			// Check Cell Data
			int localID = mesh.getCellID(cellLabel[i]);
			BOOST_CHECK_EQUAL(mesh.getCellVolume(localID), cellVol[i]);

			cupcfd::geometry::euclidean::EuclideanPoint<double,3> center;
			center = mesh.getCellCenter(localID);

			BOOST_CHECK_EQUAL(center.cmp[0], cellCenterX[i]);
			BOOST_CHECK_EQUAL(center.cmp[1], cellCenterY[i]);
			BOOST_CHECK_EQUAL(center.cmp[2], cellCenterZ[i]);
		}
	}

	// === Check each boundary still maps to the correct face and vertices ===

	for(int i = 0; i < 17; i++)
	{
		if(keepBnd[i] && comm.rank == 3)
		{
			int bndID = mesh.getBoundaryID(bndLabel[i]);
			int faceID = mesh.getFaceID(faceLabel[bndFace[i]]);
			BOOST_CHECK_EQUAL(mesh.getBoundaryFaceID(bndID), faceID);

			int vert1ID = mesh.getVertexID(vertLabel[bndVert1[i]]);
			int vert2ID = mesh.getVertexID(vertLabel[bndVert2[i]]);
			int vert3ID = mesh.getVertexID(vertLabel[bndVert3[i]]);
			BOOST_CHECK_EQUAL(mesh.getBoundaryVertex(bndID, 0), vert1ID);
			BOOST_CHECK_EQUAL(mesh.getBoundaryVertex(bndID, 1), vert2ID);
			BOOST_CHECK_EQUAL(mesh.getBoundaryVertex(bndID, 2), vert3ID);

			if(bndVert4[i] != -1)
			{
				int vert4ID = mesh.getVertexID(vertLabel[bndVert4[i]]);
				BOOST_CHECK_EQUAL(mesh.getBoundaryVertex(bndID, 3), vert4ID);
			}
			else
			{
				BOOST_CHECK_EQUAL(mesh.getBoundaryVertex(bndID, 3), -1);
			}
		}
	}

	// === Check each face still maps to the correct cells, boundaries and vertices ===

	for(int i = 0; i < 20; i++)
	{
		if(keepFace[i] && comm.rank == 3)
		{
			int faceID = mesh.getFaceID(faceLabel[i]);
			int cell1ID = mesh.getCellID(cellLabel[faceCell1[i]]);

			// Check Cell 1 Mapping is to same cell ID as expected from source data
			BOOST_CHECK_EQUAL(mesh.getFaceCell1ID(faceID), cell1ID);

			if(isBoundary[i])
			{
				// Check Boundary Mapping (should be same as before)
				int bndID = mesh.getBoundaryID(bndLabel[faceCell2OrBoundary[i]]);
				BOOST_CHECK_EQUAL(mesh.getFaceBoundaryID(faceID), bndID);
				BOOST_CHECK_EQUAL(mesh.getFaceCell2ID(faceID), -1);
			}
			else
			{
				// Check Cell 2 Mapping
				int cell2ID = mesh.getCellID(cellLabel[faceCell2OrBoundary[i]]);
				BOOST_CHECK_EQUAL(mesh.getFaceCell2ID(faceID), cell2ID);
				BOOST_CHECK_EQUAL(mesh.getFaceBoundaryID(faceID), -1);
			}

			int vert1ID = mesh.getVertexID(vertLabel[faceVert1[i]]);
			int vert2ID = mesh.getVertexID(vertLabel[faceVert2[i]]);
			int vert3ID = mesh.getVertexID(vertLabel[faceVert3[i]]);
			BOOST_CHECK_EQUAL(mesh.getFaceVertex(faceID, 0), vert1ID);
			BOOST_CHECK_EQUAL(mesh.getFaceVertex(faceID, 1), vert2ID);
			BOOST_CHECK_EQUAL(mesh.getFaceVertex(faceID, 2), vert3ID);

			if(faceVert4[i] != -1)
			{
				int vert4ID = mesh.getVertexID(vertLabel[faceVert4[i]]);
				BOOST_CHECK_EQUAL(mesh.getFaceVertex(faceID, 3), vert4ID);
			}
			else
			{
				BOOST_CHECK_EQUAL(mesh.getFaceVertex(faceID, 3), -1);
			}

		}
	}

	// === Check each cell has mappings to the correct faces ===
	// Also check that each cell tracks the correct number of faces, vertices
	// both locally and globally (will be different for ghost cells)

	switch(comm.rank)
	{
		int localID;

		case 0:	BOOST_CHECK_EQUAL(mesh.properties.lFaces, 6);

				// Cell 0

				localID = mesh.getCellID(cellLabel[0]);

				// Check Cell Face Mappings are correct
				BOOST_CHECK_EQUAL(mesh.getCellFaceID(localID, 0), mesh.getFaceID(faceLabel[0]));
				BOOST_CHECK_EQUAL(mesh.getCellFaceID(localID, 1), mesh.getFaceID(faceLabel[1]));
				BOOST_CHECK_EQUAL(mesh.getCellFaceID(localID, 2), mesh.getFaceID(faceLabel[2]));
				BOOST_CHECK_EQUAL(mesh.getCellFaceID(localID, 3), mesh.getFaceID(faceLabel[3]));
				BOOST_CHECK_EQUAL(mesh.getCellFaceID(localID, 4), mesh.getFaceID(faceLabel[4]));
				BOOST_CHECK_EQUAL(mesh.getCellFaceID(localID, 5), mesh.getFaceID(faceLabel[17]));

				// Check Cell Properties Counts are correct
				BOOST_CHECK_EQUAL(mesh.getCellStoredNFaces(localID), 6);
				BOOST_CHECK_EQUAL(mesh.getCellNFaces(localID), 6);
				BOOST_CHECK_EQUAL(mesh.getCellStoredNVertices(localID), 8);
				BOOST_CHECK_EQUAL(mesh.getCellNVertices(localID), 8);

				// Cell 1
				localID = mesh.getCellID(cellLabel[1]);

				// Check Cell Face Mappings are correct
				BOOST_CHECK_EQUAL(mesh.getCellFaceID(localID, 0), mesh.getFaceID(faceLabel[17]));

				// Check Cell Properties Counts are correct
				BOOST_CHECK_EQUAL(mesh.getCellStoredNFaces(localID), 1);
				BOOST_CHECK_EQUAL(mesh.getCellNFaces(localID), 6);
				BOOST_CHECK_EQUAL(mesh.getCellStoredNVertices(localID), 4);
				BOOST_CHECK_EQUAL(mesh.getCellNVertices(localID), 8);

				break;

		case 1:	BOOST_CHECK_EQUAL(mesh.properties.lFaces, 6);

				// Test getter for cell->number of locally attached faces

				// Cell 0
				localID = mesh.getCellID(cellLabel[0]);

				// Check Cell Face Mappings are correct
				BOOST_CHECK_EQUAL(mesh.getCellFaceID(localID, 0), mesh.getFaceID(faceLabel[17]));


				// Check Cell Properties Counts are correct
				BOOST_CHECK_EQUAL(mesh.getCellStoredNFaces(localID), 1);
				BOOST_CHECK_EQUAL(mesh.getCellNFaces(localID), 6);
				BOOST_CHECK_EQUAL(mesh.getCellStoredNVertices(localID), 4);
				BOOST_CHECK_EQUAL(mesh.getCellNVertices(localID), 8);

				// Cell 1
				localID = mesh.getCellID(cellLabel[1]);

				// Check Cell Face Mappings are correct
				// Should technically sort getFaceID(faceLabel...) to ensure they are in correct order
				// For now ensure they are in order of that which they were added, but this could be prone to
				// breaking if internals of class change.
				BOOST_CHECK_EQUAL(mesh.getCellFaceID(localID, 0), mesh.getFaceID(faceLabel[5]));
				BOOST_CHECK_EQUAL(mesh.getCellFaceID(localID, 1), mesh.getFaceID(faceLabel[6]));
				BOOST_CHECK_EQUAL(mesh.getCellFaceID(localID, 2), mesh.getFaceID(faceLabel[7]));
				BOOST_CHECK_EQUAL(mesh.getCellFaceID(localID, 3), mesh.getFaceID(faceLabel[17]));
				BOOST_CHECK_EQUAL(mesh.getCellFaceID(localID, 4), mesh.getFaceID(faceLabel[18]));
				BOOST_CHECK_EQUAL(mesh.getCellFaceID(localID, 5), mesh.getFaceID(faceLabel[19]));

				// Check Cell Properties Counts are correct
				BOOST_CHECK_EQUAL(mesh.getCellStoredNFaces(localID), 6);
				BOOST_CHECK_EQUAL(mesh.getCellNFaces(localID), 6);
				BOOST_CHECK_EQUAL(mesh.getCellStoredNVertices(localID), 8);
				BOOST_CHECK_EQUAL(mesh.getCellNVertices(localID), 8);

				// Cell 2
				localID = mesh.getCellID(cellLabel[2]);

				// Check Cell Face Mappings are correct
				BOOST_CHECK_EQUAL(mesh.getCellFaceID(localID, 0), mesh.getFaceID(faceLabel[18]));

				// Check Cell Properties Counts are correct
				BOOST_CHECK_EQUAL(mesh.getCellStoredNFaces(localID), 1);
				BOOST_CHECK_EQUAL(mesh.getCellNFaces(localID), 5);
				BOOST_CHECK_EQUAL(mesh.getCellStoredNVertices(localID), 4);
				BOOST_CHECK_EQUAL(mesh.getCellNVertices(localID), 6);

				// Cell 3
				localID = mesh.getCellID(cellLabel[3]);

				// Check Cell Face Mappings are correct
				BOOST_CHECK_EQUAL(mesh.getCellFaceID(localID, 0), mesh.getFaceID(faceLabel[19]));

				// Check Cell Properties Counts are correct
				BOOST_CHECK_EQUAL(mesh.getCellStoredNFaces(localID), 1);
				BOOST_CHECK_EQUAL(mesh.getCellNFaces(localID), 6);
				BOOST_CHECK_EQUAL(mesh.getCellStoredNVertices(localID), 4);
				BOOST_CHECK_EQUAL(mesh.getCellNVertices(localID), 8);

				break;

		case 2:	BOOST_CHECK_EQUAL(mesh.properties.lFaces, 5);

				// Cell 1
				localID = mesh.getCellID(cellLabel[1]);

				// Check Cell Face Mappings are correct
				BOOST_CHECK_EQUAL(mesh.getCellFaceID(localID, 0), mesh.getFaceID(faceLabel[18]));

				// Check Cell Properties Counts are correct
				BOOST_CHECK_EQUAL(mesh.getCellStoredNFaces(localID), 1);
				BOOST_CHECK_EQUAL(mesh.getCellNFaces(localID), 6);
				BOOST_CHECK_EQUAL(mesh.getCellStoredNVertices(localID), 4);
				BOOST_CHECK_EQUAL(mesh.getCellNVertices(localID), 8);

				// Cell 2
				localID = mesh.getCellID(cellLabel[2]);

				// Check Cell Face Mappings are correct
				// Should technically sort getFaceID(faceLabel...) to ensure they are in correct order
				BOOST_CHECK_EQUAL(mesh.getCellFaceID(localID, 0), mesh.getFaceID(faceLabel[8]));
				BOOST_CHECK_EQUAL(mesh.getCellFaceID(localID, 1), mesh.getFaceID(faceLabel[9]));
				BOOST_CHECK_EQUAL(mesh.getCellFaceID(localID, 2), mesh.getFaceID(faceLabel[10]));
				BOOST_CHECK_EQUAL(mesh.getCellFaceID(localID, 3), mesh.getFaceID(faceLabel[11]));
				BOOST_CHECK_EQUAL(mesh.getCellFaceID(localID, 4), mesh.getFaceID(faceLabel[18]));

				// Check Cell Properties Counts are correct
				BOOST_CHECK_EQUAL(mesh.getCellStoredNFaces(localID), 5);
				BOOST_CHECK_EQUAL(mesh.getCellNFaces(localID), 5);
				BOOST_CHECK_EQUAL(mesh.getCellStoredNVertices(localID), 6);
				BOOST_CHECK_EQUAL(mesh.getCellNVertices(localID), 6);

				break;

		case 3:	BOOST_CHECK_EQUAL(mesh.properties.lFaces, 6);

				// Cell 1
				localID = mesh.getCellID(cellLabel[1]);

				// Check Cell Face Mappings are correct
				BOOST_CHECK_EQUAL(mesh.getCellFaceID(localID, 0), mesh.getFaceID(faceLabel[19]));

				// Check Cell Properties Counts are correct
				BOOST_CHECK_EQUAL(mesh.getCellStoredNFaces(localID), 1);
				BOOST_CHECK_EQUAL(mesh.getCellNFaces(localID), 6);
				BOOST_CHECK_EQUAL(mesh.getCellStoredNVertices(localID), 4);
				BOOST_CHECK_EQUAL(mesh.getCellNVertices(localID), 8);

				// Cell 3
				localID = mesh.getCellID(cellLabel[3]);

				// Check Cell Face Mappings are correct
				// Should technically sort getFaceID(faceLabel...) to ensure they are in correct order
				// For now ensure they are in order of that which they were added, but this could be prone to
				// breaking if internals of class change.
				BOOST_CHECK_EQUAL(mesh.getCellFaceID(localID, 0), mesh.getFaceID(faceLabel[12]));
				BOOST_CHECK_EQUAL(mesh.getCellFaceID(localID, 1), mesh.getFaceID(faceLabel[13]));
				BOOST_CHECK_EQUAL(mesh.getCellFaceID(localID, 2), mesh.getFaceID(faceLabel[14]));
				BOOST_CHECK_EQUAL(mesh.getCellFaceID(localID, 3), mesh.getFaceID(faceLabel[15]));
				BOOST_CHECK_EQUAL(mesh.getCellFaceID(localID, 4), mesh.getFaceID(faceLabel[16]));
				BOOST_CHECK_EQUAL(mesh.getCellFaceID(localID, 5), mesh.getFaceID(faceLabel[19]));

				// Check Cell Properties Counts are correct
				BOOST_CHECK_EQUAL(mesh.getCellStoredNFaces(localID), 6);
				BOOST_CHECK_EQUAL(mesh.getCellNFaces(localID), 6);
				BOOST_CHECK_EQUAL(mesh.getCellStoredNVertices(localID), 8);
				BOOST_CHECK_EQUAL(mesh.getCellNVertices(localID), 8);


				break;
	}
}

// === buildPolyhedron + getCellPolyhedronType===
// Test 1: TriPrism
BOOST_AUTO_TEST_CASE(buildPolyhedron_test1, * utf::tolerance(0.00001))
{
	// For simplicity, build a very simple mesh on one process with only one cell
	cupcfd::comm::Communicator comm(MPI_COMM_WORLD);
	CupCfdAoSMesh<int,double,int> mesh(comm);
	cupcfd::error::eCodes status;

	if(comm.rank == 0)
	{
		// Add Regions
		std::string name = "Default";
		status = mesh.addRegion(0, name);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

		// Add Vertices
		int vertLabel[6] = {0, 1, 2, 3, 4, 5};
		double vertX[6] = {0.0, 5.0, 2.5, 0.0, 5.0, 2.5};
		double vertY[6] = {0.0, 0.0, 5.0, 0.0, 0.0, 5.0};
		double vertZ[6] = {0.0, 0.0, 0.0, 5.0, 5.0, 5.0};

		for(int i = 0; i < 6; i++)
		{
			cupcfd::geometry::euclidean::EuclideanPoint<double,3> point(vertX[i], vertY[i], vertZ[i]);
			status = mesh.addVertex(vertLabel[i], point);
			BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
		}

		// Add Boundaries
		int bndLabel[5] = {0, 1, 2, 3, 4};
		int bndVert1[5] = {0,  3, 0, 1, 0};
		int bndVert2[5] = {1,  4, 1, 2, 2};
		int bndVert3[5] = {2,  5, 3, 4, 3};
		int bndVert4[5] = {-1, -1, 4, 5, 5};

		for(int i = 0; i < 5; i++)
		{
			// Use region 0 for all
			int vert[4] = {vertLabel[bndVert1[i]], vertLabel[bndVert2[i]], vertLabel[bndVert3[i]], -1};
			int count = 3;

			// add the 4th vertex if it exists
			if(bndVert4[i] != - 1)
			{
				count = 4;
				vert[3] = vertLabel[bndVert4[i]];
			}

			status = mesh.addBoundary(bndLabel[i], 0, vert, count);
			BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
		}

		int cellLabel[1] = {0};

		cupcfd::geometry::euclidean::EuclideanPoint<double,3> center(2.1428571, 1.42857142, 2.1428571);
		double vol = 62.5;

		status = mesh.addCell(cellLabel[0], center, vol, true);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

		// Add Faces
		// For convienience, same order as boundaries + 3 non-boundary faces at end
		// This means face index = boundary index where applicable
		int faceLabel[5] = {0, 1, 2, 3, 4};
		int faceVert1[5] = {0, 3, 0, 1, 0};
		int faceVert2[5] = {1, 4, 3, 2, 2};
		int faceVert3[5] = {2, 5, 4, 5, 5};
		int faceVert4[5] = {-1, -1, 1, 4, 3};
		int faceNVertices[5] = {3, 3, 4, 4, 4};
		int faceCell1[5] = {0 ,0 ,0 ,0, 0};
		int faceCell2OrBoundary[5] = {0, 1, 2, 3, 4};
		int isBoundary[5] = {true, true, true, true, true};

		for(int i = 0; i < 5; i++)
		{
			// Use region 0 for all
			int vert[4] = {vertLabel[faceVert1[i]], vertLabel[faceVert2[i]], vertLabel[faceVert3[i]], -1};
			int count = 3;

			// add the 4th vertex if it exists
			if(faceVert4[i] != - 1)
			{
				count = 4;
				vert[3] = vertLabel[faceVert4[i]];
			}

			// Default add
			if(isBoundary[i])
			{
				status = mesh.addFace(faceLabel[i], cellLabel[faceCell1[i]], bndLabel[faceCell2OrBoundary[i]], isBoundary[i], vert, count);
				BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
			}
			else
			{
				status = mesh.addFace(faceLabel[i], cellLabel[faceCell1[i]], cellLabel[faceCell2OrBoundary[i]], isBoundary[i], vert, count);
				BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
			}
		}
	}

	status = mesh.finalize();
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	if(comm.rank == 0)
	{
		// Test getPolyhedronType
		cupcfd::geometry::shapes::PolyhedronType type = mesh.getCellPolyhedronType(0);
		BOOST_CHECK_EQUAL(type, cupcfd::geometry::shapes::POLYHEDRON_TRIPRISM);

		// Test Building the polyhedron
		cupcfd::geometry::shapes::TriPrism<double> * shape;
		status = mesh.buildPolyhedron(0, &shape);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

		// Check Polyhedron Values to ensure it is correct
		BOOST_TEST(shape->verticesStore[0].cmp[0] == 0.0);
		BOOST_TEST(shape->verticesStore[0].cmp[1] == 0.0);
		BOOST_TEST(shape->verticesStore[0].cmp[2] == 0.0);

		BOOST_TEST(shape->verticesStore[1].cmp[0] == 2.5);
		BOOST_TEST(shape->verticesStore[1].cmp[1] == 5.0);
		BOOST_TEST(shape->verticesStore[1].cmp[2] == 0.0);

		BOOST_TEST(shape->verticesStore[2].cmp[0] == 5.0);
		BOOST_TEST(shape->verticesStore[2].cmp[1] == 0.0);
		BOOST_TEST(shape->verticesStore[2].cmp[2] == 0.0);

		BOOST_TEST(shape->verticesStore[3].cmp[0] == 0.0);
		BOOST_TEST(shape->verticesStore[3].cmp[1] == 0.0);
		BOOST_TEST(shape->verticesStore[3].cmp[2] == 5.0);

		BOOST_TEST(shape->verticesStore[4].cmp[0] == 2.5);
		BOOST_TEST(shape->verticesStore[4].cmp[1] == 5.0);
		BOOST_TEST(shape->verticesStore[4].cmp[2] == 5.0);

		BOOST_TEST(shape->verticesStore[5].cmp[0] == 5.0);
		BOOST_TEST(shape->verticesStore[5].cmp[1] == 0.0);
		BOOST_TEST(shape->verticesStore[5].cmp[2] == 5.0);

		BOOST_TEST(shape->computeVolume() == 62.5);
		BOOST_TEST(shape->computeCentroid().cmp[0] == 2.1428571);
		BOOST_TEST(shape->computeCentroid().cmp[1] == 1.42857142);
		BOOST_TEST(shape->computeCentroid().cmp[2] == 2.1428571);
	}
}

// Test 2: QuadPyramid
BOOST_AUTO_TEST_CASE(buildPolyhedron_test2, * utf::tolerance(0.00001))
{
	// For simplicity, buila very simple mesh on one process with only one cell
	cupcfd::comm::Communicator comm(MPI_COMM_WORLD);
	CupCfdAoSMesh<int,double,int> mesh(comm);
	cupcfd::error::eCodes status;

	if(comm.rank == 0)
	{
		// Add Regions
		std::string name = "Default";
		status = mesh.addRegion(0, name);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

		// Add Vertices
		int vertLabel[5] = {0, 1, 2, 3, 4};
		double vertX[5] = {0.0, 5.0, 5.0, 0.0, 2.5};
		double vertY[5] = {0.0, 0.0, 5.0, 5.0, 2.5};
		double vertZ[5] = {0.0, 0.0, 0.0, 0.0, 5.0};

		for(int i = 0; i < 5; i++)
		{
			cupcfd::geometry::euclidean::EuclideanPoint<double,3> point(vertX[i], vertY[i], vertZ[i]);
			status = mesh.addVertex(vertLabel[i], point);
			BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
		}

		// Add Boundaries
		int bndLabel[5] = {0, 1, 2, 3, 4};
		int bndVert1[5] = {0, 0, 1, 2, 3};
		int bndVert2[5] = {1, 1, 2, 3, 1};
		int bndVert3[5] = {2, 4, 4, 4, 4};
		int bndVert4[5] = {3, -1, -1, -1, -1};

		for(int i = 0; i < 5; i++)
		{
			// Use region 0 for all
			int vert[4] = {vertLabel[bndVert1[i]], vertLabel[bndVert2[i]], vertLabel[bndVert3[i]], -1};
			int count = 3;

			// add the 4th vertex if it exists
			if(bndVert4[i] != - 1)
			{
				count = 4;
				vert[3] = vertLabel[bndVert4[i]];
			}

			status = mesh.addBoundary(bndLabel[i], 0, vert, count);
			BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
		}

		int cellLabel[1] = {0};

		cupcfd::geometry::euclidean::EuclideanPoint<double,3> center(2.5, 2.5, 1.25);
		double vol = 41.6666667;

		status = mesh.addCell(cellLabel[0], center, vol, true);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

		// Add Faces
		// For convienience, same order as boundaries + 3 non-boundary faces at end
		// This means face index = boundary index where applicable
		int faceLabel[5] = {0, 1, 2, 3, 4};
		int faceVert1[5] = {0, 0, 1, 2, 3};
		int faceVert2[5] = {1, 1, 2, 3, 0};
		int faceVert3[5] = {2, 4, 4, 4, 4};
		int faceVert4[5] = {3, -1, -1, -1, -1};
		int faceNVertices[5] = {4, 3, 3, 3, 3};
		int faceCell1[5] = {0 ,0 ,0 ,0, 0};
		int faceCell2OrBoundary[5] = {0, 1, 2, 3, 4};
		int isBoundary[5] = {true, true, true, true, true};

		for(int i = 0; i < 5; i++)
		{
			// Use region 0 for all
			int vert[4] = {vertLabel[faceVert1[i]], vertLabel[faceVert2[i]], vertLabel[faceVert3[i]], -1};
			int count = 3;

			// add the 4th vertex if it exists
			if(faceVert4[i] != - 1)
			{
				count = 4;
				vert[3] = vertLabel[faceVert4[i]];
			}

			// Default add
			if(isBoundary[i])
			{
				status = mesh.addFace(faceLabel[i], cellLabel[faceCell1[i]], bndLabel[faceCell2OrBoundary[i]], isBoundary[i], vert, count);
				BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
			}
			else
			{
				status = mesh.addFace(faceLabel[i], cellLabel[faceCell1[i]], cellLabel[faceCell2OrBoundary[i]], isBoundary[i], vert, count);
				BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
			}
		}
	}

	status = mesh.finalize();
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	if(comm.rank == 0)
	{
		// Test getPolyhedronType
		cupcfd::geometry::shapes::PolyhedronType type = mesh.getCellPolyhedronType(0);
		BOOST_CHECK_EQUAL(type, cupcfd::geometry::shapes::POLYHEDRON_QUADPYRAMID);

		// Test Building the polyhedron
		cupcfd::geometry::shapes::QuadPyramid<double> * shape;
		status = mesh.buildPolyhedron(0, &shape);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

		BOOST_TEST(shape->base.vertices[0].cmp[0] == 0.0);
		BOOST_TEST(shape->base.vertices[0].cmp[1] == 0.0);
		BOOST_TEST(shape->base.vertices[0].cmp[2] == 0.0);

		BOOST_TEST(shape->base.vertices[1].cmp[0] == 0.0);
		BOOST_TEST(shape->base.vertices[1].cmp[1] == 5.0);
		BOOST_TEST(shape->base.vertices[1].cmp[2] == 0.0);

		BOOST_TEST(shape->base.vertices[2].cmp[0] == 5.0);
		BOOST_TEST(shape->base.vertices[2].cmp[1] == 5.0);
		BOOST_TEST(shape->base.vertices[2].cmp[2] == 0.0);

		BOOST_TEST(shape->base.vertices[3].cmp[0] == 5.0);
		BOOST_TEST(shape->base.vertices[3].cmp[1] == 0.0);
		BOOST_TEST(shape->base.vertices[3].cmp[2] == 0.0);

		BOOST_TEST(shape->apex.cmp[0] == 2.5);
		BOOST_TEST(shape->apex.cmp[1] == 2.5);
		BOOST_TEST(shape->apex.cmp[2] == 5.0);

		BOOST_TEST(shape->computeVolume() == 41.6666667);
		BOOST_TEST(shape->computeCentroid().cmp[0] == 2.5);
		BOOST_TEST(shape->computeCentroid().cmp[1] == 2.5);
		BOOST_TEST(shape->computeCentroid().cmp[2] == 1.25);
	}
}

// Test 3: Tetrahedron
BOOST_AUTO_TEST_CASE(buildPolyhedron_test3, * utf::tolerance(0.00001))
{
	// For simplicity, buila very simple mesh on one process with only one cell
	cupcfd::comm::Communicator comm(MPI_COMM_WORLD);
	CupCfdAoSMesh<int,double,int> mesh(comm);
	cupcfd::error::eCodes status;

	if(comm.rank == 0)
	{
		// Add Regions
		std::string name = "Default";
		status = mesh.addRegion(0, name);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

		// Add Vertices
		int vertLabel[4] = {0, 1, 2, 3};
		double vertX[4] = {0.0, 5.0, 5.0, 2.5};
		double vertY[4] = {0.0, 0.0, 5.0, 2.5};
		double vertZ[4] = {0.0, 0.0, 0.0, 5.0};

		for(int i = 0; i < 4; i++)
		{
			cupcfd::geometry::euclidean::EuclideanPoint<double,3> point(vertX[i], vertY[i], vertZ[i]);
			status = mesh.addVertex(vertLabel[i], point);
			BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
		}

		// Add Boundaries
		int bndLabel[4] = {0, 1, 2, 3};
		int bndVert1[4] = {0, 0, 1, 2};
		int bndVert2[4] = {1, 1, 2, 3};
		int bndVert3[4] = {2, 3, 3, 3};
		int bndVert4[4] = {3, -1, -1, -1};

		for(int i = 0; i < 4; i++)
		{
			// Use region 0 for all
			int vert[4] = {vertLabel[bndVert1[i]], vertLabel[bndVert2[i]], vertLabel[bndVert3[i]], -1};
			int count = 3;

			// add the 4th vertex if it exists
			if(bndVert4[i] != - 1)
			{
				count = 4;
				vert[3] = vertLabel[bndVert4[i]];
			}

			status = mesh.addBoundary(bndLabel[i], 0, vert, count);
			BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
		}

		int cellLabel[1] = {0};

		status = mesh.addCell(cellLabel[0], true);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

		// Add Faces
		// For convienience, same order as boundaries + 3 non-boundary faces at end
		// This means face index = boundary index where applicable
		int faceLabel[4] = {0, 1, 2, 3};
		int faceVert1[4] = {0, 0, 1, 2};
		int faceVert2[4] = {1, 1, 2, 3};
		int faceVert3[4] = {2, 3, 3, 3};
		int faceVert4[4] = {3, -1, -1, -1};
		int faceNVertices[4] = {4, 3, 3, 3};
		int faceCell1[4] = {0 ,0 ,0 ,0};
		int faceCell2OrBoundary[4] = {0, 1, 2, 3};
		int isBoundary[4] = {true, true, true, true};

		for(int i = 0; i < 4; i++)
		{
			// Use region 0 for all
			int vert[4] = {vertLabel[faceVert1[i]], vertLabel[faceVert2[i]], vertLabel[faceVert3[i]], -1};
			int count = 3;

			// add the 4th vertex if it exists
			if(faceVert4[i] != - 1)
			{
				count = 4;
				vert[3] = vertLabel[faceVert4[i]];
			}

			// Default add
			if(isBoundary[i])
			{
				status = mesh.addFace(faceLabel[i], cellLabel[faceCell1[i]], bndLabel[faceCell2OrBoundary[i]], isBoundary[i], vert, count);
				BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
			}
			else
			{
				status = mesh.addFace(faceLabel[i], cellLabel[faceCell1[i]], cellLabel[faceCell2OrBoundary[i]], isBoundary[i], vert, count);
				BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
			}
		}
	}

	status = mesh.finalize();
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	if(comm.rank == 0)
	{
		// Test getPolyhedronType
		cupcfd::geometry::shapes::PolyhedronType type = mesh.getCellPolyhedronType(0);
		BOOST_CHECK_EQUAL(type, cupcfd::geometry::shapes::POLYHEDRON_TETRAHEDRON);

		// Test Building the polyhedron
		cupcfd::geometry::shapes::Tetrahedron<double> * shape;
		status = mesh.buildPolyhedron(0, &shape);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

		BOOST_TEST(shape->base.vertices[0].cmp[0] == 0.0);
		BOOST_TEST(shape->base.vertices[0].cmp[1] == 0.0);
		BOOST_TEST(shape->base.vertices[0].cmp[2] == 0.0);

		BOOST_TEST(shape->base.vertices[1].cmp[0] == 5.0);
		BOOST_TEST(shape->base.vertices[1].cmp[1] == 5.0);
		BOOST_TEST(shape->base.vertices[1].cmp[2] == 0.0);

		BOOST_TEST(shape->base.vertices[2].cmp[0] == 5.0);
		BOOST_TEST(shape->base.vertices[2].cmp[1] == 0.0);
		BOOST_TEST(shape->base.vertices[2].cmp[2] == 0.0);

		BOOST_TEST(shape->apex.cmp[0] == 2.5);
		BOOST_TEST(shape->apex.cmp[1] == 2.5);
		BOOST_TEST(shape->apex.cmp[2] == 5.0);

		BOOST_TEST(shape->computeVolume() == 20.83333);
		BOOST_TEST(shape->computeCentroid().cmp[0] == 3.125);
		BOOST_TEST(shape->computeCentroid().cmp[1] == 1.875);
		BOOST_TEST(shape->computeCentroid().cmp[2] == 1.25);
	}
}

// Test 4: Hexahedral
BOOST_AUTO_TEST_CASE(buildPolyhedron_test4, * utf::tolerance(0.00001))
{
	// For simplicity, buila very simple mesh on one process with only one cell
	cupcfd::comm::Communicator comm(MPI_COMM_WORLD);
	CupCfdAoSMesh<int,double,int> mesh(comm);
	cupcfd::error::eCodes status;

	if(comm.rank == 0)
	{
		// Add Regions
		std::string name = "Default";
		status = mesh.addRegion(0, name);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

		// Add Vertices
		int vertLabel[8] = {0, 1, 2, 3, 4, 5, 6, 7};
		double vertX[8] = {0.0, 5.0, 5.0, 0.0, 0.0, 5.0, 5.0, 0.0};
		double vertY[8] = {0.0, 0.0, 5.0, 5.0, 0.0, 0.0, 5.0, 5.0};
		double vertZ[8] = {0.0, 0.0, 0.0, 0.0, 5.0, 5.0, 5.0, 5.0};

		for(int i = 0; i < 8; i++)
		{
			cupcfd::geometry::euclidean::EuclideanPoint<double,3> point(vertX[i], vertY[i], vertZ[i]);
			status = mesh.addVertex(vertLabel[i], point);
			BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
		}

		// Add Boundaries
		int bndLabel[6] = {0, 1, 2, 3, 4, 5};
		int bndVert1[6] = {0, 4, 0, 1, 2, 0};
		int bndVert2[6] = {1, 5, 1, 2, 3, 3};
		int bndVert3[6] = {2, 6, 5, 6, 7, 7};
		int bndVert4[6] = {3, 7, 4, 5, 6, 4};

		for(int i = 0; i < 6; i++)
		{
			// Use region 0 for all
			int vert[4] = {vertLabel[bndVert1[i]], vertLabel[bndVert2[i]], vertLabel[bndVert3[i]], -1};
			int count = 3;

			// add the 4th vertex if it exists
			if(bndVert4[i] != - 1)
			{
				count = 4;
				vert[3] = vertLabel[bndVert4[i]];
			}

			status = mesh.addBoundary(bndLabel[i], 0, vert, count);
			BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
		}

		int cellLabel[1] = {0};

		status = mesh.addCell(cellLabel[0], true);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

		// Add Faces
		// For convienience, same order as boundaries + 3 non-boundary faces at end
		// This means face index = boundary index where applicable
		int faceLabel[6] = {0, 1, 2, 3, 4, 5};
		int faceVert1[6] = {0, 4, 0, 1, 2, 0};
		int faceVert2[6] = {1, 5, 1, 2, 3, 3};
		int faceVert3[6] = {2, 6, 5, 6, 7, 7};
		int faceVert4[6] = {3, 7, 4, 5, 6, 4};
		int faceNVertices[6] = {4, 4, 4, 4, 4, 4};
		int faceCell1[6] = {0 ,0 ,0 ,0, 0, 0};
		int faceCell2OrBoundary[6] = {0, 1, 2, 3, 4, 5};
		int isBoundary[6] = {true, true, true, true, true, true};

		for(int i = 0; i < 6; i++)
		{
			// Use region 0 for all
			int vert[4] = {vertLabel[faceVert1[i]], vertLabel[faceVert2[i]], vertLabel[faceVert3[i]], -1};
			int count = 3;

			// add the 4th vertex if it exists
			if(faceVert4[i] != - 1)
			{
				count = 4;
				vert[3] = vertLabel[faceVert4[i]];
			}

			// Default add
			if(isBoundary[i])
			{
				status = mesh.addFace(faceLabel[i], cellLabel[faceCell1[i]], bndLabel[faceCell2OrBoundary[i]], isBoundary[i], vert, count);
				BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
			}
			else
			{
				status = mesh.addFace(faceLabel[i], cellLabel[faceCell1[i]], cellLabel[faceCell2OrBoundary[i]], isBoundary[i], vert, count);
				BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
			}
		}
	}

	status = mesh.finalize();
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	if(comm.rank == 0)
	{
		// Test getPolyhedronType
		cupcfd::geometry::shapes::PolyhedronType type = mesh.getCellPolyhedronType(0);
		BOOST_CHECK_EQUAL(type, cupcfd::geometry::shapes::POLYHEDRON_HEXAHEDRON);

		// Test Building the polyhedron
		cupcfd::geometry::shapes::Hexahedron<double> * shape;
		status = mesh.buildPolyhedron(0, &shape);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

		BOOST_TEST(shape->tlf.cmp[0] == 0.0);
		BOOST_TEST(shape->tlf.cmp[1] == 0.0);
		BOOST_TEST(shape->tlf.cmp[2] == 5.0);

		BOOST_TEST(shape->trf.cmp[0] == 0.0);
		BOOST_TEST(shape->trf.cmp[1] == 5.0);
		BOOST_TEST(shape->trf.cmp[2] == 5.0);

		BOOST_TEST(shape->blf.cmp[0] == 0.0);
		BOOST_TEST(shape->blf.cmp[1] == 0.0);
		BOOST_TEST(shape->blf.cmp[2] == 0.0);

		BOOST_TEST(shape->brf.cmp[0] == 0.0);
		BOOST_TEST(shape->brf.cmp[1] == 5.0);
		BOOST_TEST(shape->brf.cmp[2] == 0.0);

		BOOST_TEST(shape->tlb.cmp[0] == 5.0);
		BOOST_TEST(shape->tlb.cmp[1] == 0.0);
		BOOST_TEST(shape->tlb.cmp[2] == 5.0);

		BOOST_TEST(shape->trb.cmp[0] == 5.0);
		BOOST_TEST(shape->trb.cmp[1] == 5.0);
		BOOST_TEST(shape->trb.cmp[2] == 5.0);

		BOOST_TEST(shape->blb.cmp[0] == 5.0);
		BOOST_TEST(shape->blb.cmp[1] == 0.0);
		BOOST_TEST(shape->blb.cmp[2] == 0.0);

		BOOST_TEST(shape->brb.cmp[0] == 5.0);
		BOOST_TEST(shape->brb.cmp[1] == 5.0);
		BOOST_TEST(shape->brb.cmp[2] == 0.0);

		BOOST_TEST(shape->computeVolume() == 125.0);
		BOOST_TEST(shape->computeCentroid().cmp[0] == 2.5);
		BOOST_TEST(shape->computeCentroid().cmp[1] == 2.5);
		BOOST_TEST(shape->computeCentroid().cmp[2] == 2.5);
	}
}

// === findCellID ===
// Test 1: Test arbitrary point
BOOST_AUTO_TEST_CASE(findCellID_test1)
{
	cupcfd::error::eCodes status;
    cupcfd::comm::Communicator comm(MPI_COMM_WORLD);

	// === Create a small test mesh ===
	// Setup the configurations
    cupcfd::partitioner::PartitionerNaiveConfig<int,int> partConfig;
    MeshSourceStructGenConfig<int, double> meshSourceConfig(5, 5, 5, 0.0, 1.0, 0.0, 1.0, 0.0, 1.0);
    MeshConfig<int,double,int> meshConfig(partConfig, meshSourceConfig);

    // Build the mesh
    CupCfdAoSMesh<int,double,int> * mesh;
	status = meshConfig.buildUnstructuredMesh(&mesh, comm);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	cupcfd::geometry::euclidean::EuclideanPoint<double,3> point1(0.56, 0.24, 0.3);

	int localCellID;
	int globalCellID;
	status = mesh->findCellID(point1, &localCellID, &globalCellID);

	// Naive Partitioner splits each rank into 32, 31, 31, 31
	// Expected cell is Cell 32 (Zero-indexed)

	switch(comm.rank)
	{
		case 0:
				BOOST_CHECK_EQUAL(status, cupcfd::error::E_GEOMETRY_NO_VALID_CELL);
				break;
		case 1:
				BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
				BOOST_CHECK_EQUAL(localCellID, 0);
				BOOST_CHECK_EQUAL(globalCellID, 32);
				break;
		case 2:
				BOOST_CHECK_EQUAL(status, cupcfd::error::E_GEOMETRY_NO_VALID_CELL);
				break;
		case 3:
				BOOST_CHECK_EQUAL(status, cupcfd::error::E_GEOMETRY_NO_VALID_CELL);
				break;
	}

	delete mesh;
}

BOOST_AUTO_TEST_CASE(cleanup)
{
    MPI_Finalize();
}



