/*
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Unit Tests for the concrete methods of the Particlesimple class
 */

#define BOOST_TEST_MODULE ParticleSimple
#include <boost/test/unit_test.hpp>
#include <boost/test/output_test_stream.hpp>
#include <stdexcept>

#include "ParticleSimple.h"
#include "Broadcast.h"

#include "EuclideanPoint.h"
#include "PartitionerConfig.h"
#include "PartitionerNaiveConfig.h"
#include "MeshSourceStructGenConfig.h"
#include "MeshConfig.h"
#include "CupCfdAoSMesh.h"

namespace utf = boost::unit_test;

using namespace cupcfd::particles;

// Setup
BOOST_AUTO_TEST_CASE(setup)
{
    int argc = boost::unit_test::framework::master_test_suite().argc;
    char ** argv = boost::unit_test::framework::master_test_suite().argv;

    MPI_Init(&argc, &argv);

    cupcfd::error::eCodes status;

	// Need to register point, vector MPI datatype since the particle MPI datatype depends on them
	cupcfd::geometry::euclidean::EuclideanPoint<double, 3> point;
	status = point.registerMPIType();
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	cupcfd::geometry::euclidean::EuclideanVector<double,3> vector;
	status = vector.registerMPIType();
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
}

// === Constructor ===
// Test 1: Test values are set correctly
BOOST_AUTO_TEST_CASE(constructor_test1, * utf::tolerance(0.00001))
{
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> pos(0.12, 0.11, 0.14);
	cupcfd::geometry::euclidean::EuclideanVector<double,3> velocity(1.0, 1.1, 1.2);
	cupcfd::geometry::euclidean::EuclideanVector<double,3> acceleration(2.1, 2.2, 2.3);
	cupcfd::geometry::euclidean::EuclideanVector<double,3> jerk(3.4, 4.5, 5.6);
	uint pID=0, cellID=102, rank=5;
	ParticleSimple<int,double> particle(pos, velocity, acceleration, jerk, pID, cellID, rank, 16.7, 4.3, 0.01);

	BOOST_TEST(particle.pos.cmp[0] == 0.12);
	BOOST_TEST(particle.pos.cmp[1] == 0.11);
	BOOST_TEST(particle.pos.cmp[2] == 0.14);
	BOOST_TEST(particle.inflightPos.cmp[0] == 0.12);
	BOOST_TEST(particle.inflightPos.cmp[1] == 0.11);
	BOOST_TEST(particle.inflightPos.cmp[2] == 0.14);
	BOOST_TEST(particle.velocity.cmp[0] == 1.0);
	BOOST_TEST(particle.velocity.cmp[1] == 1.1);
	BOOST_TEST(particle.velocity.cmp[2] == 1.2);
	BOOST_TEST(particle.acceleration.cmp[0] == 2.1);
	BOOST_TEST(particle.acceleration.cmp[1] == 2.2);
	BOOST_TEST(particle.acceleration.cmp[2] == 2.3);
	BOOST_TEST(particle.jerk.cmp[0] == 3.4);
	BOOST_TEST(particle.jerk.cmp[1] == 4.5);
	BOOST_TEST(particle.jerk.cmp[2] == 5.6);
	BOOST_TEST(particle.getCellGlobalID() == 102);
	BOOST_TEST(particle.getRank() == 5);
	BOOST_TEST(particle.decayLevel == 16.7);
	BOOST_TEST(particle.decayRate == 4.3);
	BOOST_TEST(particle.travelDt == 0.01);
}

// === set/getPos ===
// Test 1:
BOOST_AUTO_TEST_CASE(set_getPos_test1, * utf::tolerance(0.00001))
{

}

// === set/getInFlightPos ===
// Test 1:
BOOST_AUTO_TEST_CASE(set_getInFlightPos_test1, * utf::tolerance(0.00001))
{

}

// === set/getVelocity ===
// Test 1:
BOOST_AUTO_TEST_CASE(set_getVelocity_test1, * utf::tolerance(0.00001))
{

}

// === set/getAcceleration ===
// Test 1:
BOOST_AUTO_TEST_CASE(set_getAcceleration_test1, * utf::tolerance(0.00001))
{

}

// === set/getJerk ===
// Test 1:
BOOST_AUTO_TEST_CASE(set_getJerk_test1, * utf::tolerance(0.00001))
{

}

// === set/getCellID ===
// Test 1:
BOOST_AUTO_TEST_CASE(set_getCellID_test1, * utf::tolerance(0.00001))
{

}

// === set/getTravelTime ===
// Test 1:
BOOST_AUTO_TEST_CASE(set_getTravelTime_test1, * utf::tolerance(0.00001))
{

}

// === set/getDecayLevel ===
// Test 1:
BOOST_AUTO_TEST_CASE(set_getDecayLevel_test1, * utf::tolerance(0.00001))
{

}

// === set/getDecayRate ===
// Test 1:
BOOST_AUTO_TEST_CASE(set_getDecayRate_test1, * utf::tolerance(0.00001))
{

}

// === updateVelocity ===
// Test 1:
BOOST_AUTO_TEST_CASE(updateVelocity_test1, * utf::tolerance(0.00001))
{

}

// === updateAcceleration ===
// Test 1:
BOOST_AUTO_TEST_CASE(updateAcceleration_test1, * utf::tolerance(0.00001))
{

}

// === updateDecayLevel ===
// Test 1:
BOOST_AUTO_TEST_CASE(updateDecayLevel_test1, * utf::tolerance(0.00001))
{

}

// === updatePositionAtomic ===
// Test 1: Correctly update the position of a particle that remains within a cell for the dt period
BOOST_AUTO_TEST_CASE(updatePosition_test1, * utf::tolerance(0.00001))
{
	// Don't care about testing MPI functionality, here, restrict to rank 0
	cupcfd::error::eCodes status;
	cupcfd::comm::Communicator comm(MPI_COMM_SELF);

	if(comm.rank == 0)
	{
		// === Create a small test mesh ===
		// Setup the configurations
		cupcfd::partitioner::PartitionerNaiveConfig<int,int> partConfig;
		cupcfd::geometry::mesh::MeshSourceStructGenConfig<int, double> meshSourceConfig(5, 5, 5, 0.0, 1.0, 0.0, 1.0, 0.0, 1.0);
		cupcfd::geometry::mesh::MeshConfig<int,double,int> meshConfig(partConfig, meshSourceConfig);

		// Build the mesh
		cupcfd::geometry::mesh::CupCfdAoSMesh<int,double,int> * mesh;
		status = meshConfig.buildUnstructuredMesh(&mesh, comm);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);


		// Create a new particle with a constant velocity that doesn't decay
		// Make a cell that just moves to the right in the X dimension
		cupcfd::geometry::euclidean::EuclideanPoint<double,3> pos(0.12, 0.11, 0.14);
		cupcfd::geometry::euclidean::EuclideanVector<double,3> velocity(1.0, 0.0, 0.0);
		cupcfd::geometry::euclidean::EuclideanVector<double,3> acceleration(0.0, 0.0, 0.0);
		cupcfd::geometry::euclidean::EuclideanVector<double,3> jerk(0.0, 0.0, 0.0);
		uint pID=0, cellID=0, rank=5;
		ParticleSimple<int,double> particle(pos, velocity, acceleration, jerk, pID, cellID, rank, 1.0, 0.0, 0.01);

		double dt;
		int localFaceID;
		status = particle.updatePositionAtomic(*mesh, &dt, &localFaceID, false);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

		// Check new position is correct
		// Should have moved to another location within the cell
		BOOST_TEST(particle.inflightPos.cmp[0] == 0.13);
		BOOST_TEST(particle.inflightPos.cmp[1] == 0.11);
		BOOST_TEST(particle.inflightPos.cmp[2] == 0.14);

		// Check the elapsed dt value is correct
		BOOST_TEST(dt == 0.01);

		// Check that the travel time was reduced
		BOOST_TEST(particle.travelDt == 0.0);

		// Check the localFaceID is -1, i.e. it did not reach a face/boundary
		BOOST_CHECK_EQUAL(localFaceID, -1);
	}
}

// Test 2: Correctly update the position of a particle that reaches/crosses a cell face but stays on node
BOOST_AUTO_TEST_CASE(updatePosition_test2, * utf::tolerance(0.00001))
{
	// Since we test on-node/off node particle status, need to use all ranks for this test
	cupcfd::error::eCodes status;
	cupcfd::comm::Communicator comm(MPI_COMM_WORLD);

	// === Create a small test mesh ===
	// Setup the configurations
	cupcfd::partitioner::PartitionerNaiveConfig<int,int> partConfig;
	cupcfd::geometry::mesh::MeshSourceStructGenConfig<int, double> meshSourceConfig(5, 5, 5, 0.0, 1.0, 0.0, 1.0, 0.0, 1.0);
	cupcfd::geometry::mesh::MeshConfig<int,double,int> meshConfig(partConfig, meshSourceConfig);

	// Build the mesh
	cupcfd::geometry::mesh::CupCfdAoSMesh<int,double,int> * mesh;
	status = meshConfig.buildUnstructuredMesh(&mesh, comm);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);


	// Create a new particle with a constant velocity that doesn't decay
	// Make a cell that just moves to the right in the X dimension

	if(comm.rank == 0)
	{
		cupcfd::geometry::euclidean::EuclideanPoint<double,3> pos(0.12, 0.11, 0.14);
		cupcfd::geometry::euclidean::EuclideanVector<double,3> velocity(1.0, 0.0, 0.0);
		cupcfd::geometry::euclidean::EuclideanVector<double,3> acceleration(0.0, 0.0, 0.0);
		cupcfd::geometry::euclidean::EuclideanVector<double,3> jerk(0.0, 0.0, 0.0);
		uint pID=0, cellID=0, rank=0;
		ParticleSimple<int,double> particle(pos, velocity, acceleration, jerk, pID, cellID, rank, 1.0, 0.0, 0.1);

		double dt;
		int localFaceID;
		status = particle.updatePositionAtomic(*mesh, &dt, &localFaceID, false);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

		// In 0.1 dt, should intend to cross a cell boundary from Global Cell 0 to Global Cell 1
		// Check particle is positioned at boundary
		BOOST_TEST(particle.inflightPos.cmp[0] == 0.20);
		BOOST_TEST(particle.inflightPos.cmp[1] == 0.11);
		BOOST_TEST(particle.inflightPos.cmp[2] == 0.14);

		// Check that the elapsed time was set correctly - i.e. we did not use the full 0.1 since it was
		// atomic across a cell
		BOOST_TEST(dt = 0.08);
		// Check that the ID of the face we reached is correct
		BOOST_CHECK_EQUAL(localFaceID, 1);
	}
}

// Test 3: Correctly update the position of a particle that is sitting on a face but should exit via another face
// (i.e. move off the current face without getting stuck)
BOOST_AUTO_TEST_CASE(updatePosition_test3, * utf::tolerance(0.00001))
{
	// Since we test on-node/off node particle status, need to use all ranks for this test
	cupcfd::error::eCodes status;
	cupcfd::comm::Communicator comm(MPI_COMM_WORLD);

	// === Create a small test mesh ===
	// Setup the configurations
	cupcfd::partitioner::PartitionerNaiveConfig<int,int> partConfig;
	cupcfd::geometry::mesh::MeshSourceStructGenConfig<int, double> meshSourceConfig(5, 5, 5, 0.0, 1.0, 0.0, 1.0, 0.0, 1.0);
	cupcfd::geometry::mesh::MeshConfig<int,double,int> meshConfig(partConfig, meshSourceConfig);

	// Build the mesh
	cupcfd::geometry::mesh::CupCfdAoSMesh<int,double,int> * mesh;
	status = meshConfig.buildUnstructuredMesh(&mesh, comm);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);


	// Create a new particle with a constant velocity that doesn't decay
	// Make a cell that just moves to the right in the X dimension

	if(comm.rank == 0)
	{
		cupcfd::geometry::euclidean::EuclideanPoint<double,3> pos(0.2, 0.11, 0.14);
		cupcfd::geometry::euclidean::EuclideanVector<double,3> velocity(1.0, 0.0, 0.0);
		cupcfd::geometry::euclidean::EuclideanVector<double,3> acceleration(0.0, 0.0, 0.0);
		cupcfd::geometry::euclidean::EuclideanVector<double,3> jerk(0.0, 0.0, 0.0);
		uint pID=0, cellID=0, rank=0;
		ParticleSimple<int,double> particle(pos, velocity, acceleration, jerk, pID, cellID, rank, 1.0, 0.0, 0.3);

		double dt;
		int localFaceID;
		status = particle.updatePositionAtomic(*mesh, &dt, &localFaceID, false);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

		// In 0.1 dt, should intend to cross a cell boundary from Global Cell 0 to Global Cell 1
		// Check particle is positioned at boundary
		BOOST_TEST(particle.inflightPos.cmp[0] == 0.40);
		BOOST_TEST(particle.inflightPos.cmp[1] == 0.11);
		BOOST_TEST(particle.inflightPos.cmp[2] == 0.14);

		// Check that the elapsed time was set correctly - i.e. we did not use the full 0.1 since it was
		// atomic across a cell
		BOOST_TEST(dt = 0.2);
		// Check that the ID of the face we reached is correct
		BOOST_CHECK_EQUAL(localFaceID, 2);

		// Call again, but without changing the CellID - it should not advance or give the wrong face ID
		status = particle.updatePositionAtomic(*mesh, &dt, &localFaceID, false);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
		BOOST_TEST(particle.inflightPos.cmp[0] == 0.40);
		BOOST_TEST(particle.inflightPos.cmp[1] == 0.11);
		BOOST_TEST(particle.inflightPos.cmp[2] == 0.14);
		BOOST_TEST(dt == 0.0);
		BOOST_CHECK_EQUAL(localFaceID, 2);
	}
}

// Test 4: Correctly update the position of a particle that is sitting on a face that it should exit (i.e. the current
// velocity would immediately move it into the next cell, so it can no longer be moved until its CellID is updated to
// the next cell along)
BOOST_AUTO_TEST_CASE(updatePosition_test4, * utf::tolerance(0.00001))
{
	// Since we test on-node/off node particle status, need to use all ranks for this test
	cupcfd::error::eCodes status;
	cupcfd::comm::Communicator comm(MPI_COMM_WORLD);

	// === Create a small test mesh ===
	// Setup the configurations
	cupcfd::partitioner::PartitionerNaiveConfig<int,int> partConfig;
	cupcfd::geometry::mesh::MeshSourceStructGenConfig<int, double> meshSourceConfig(5, 5, 5, 0.0, 1.0, 0.0, 1.0, 0.0, 1.0);
	cupcfd::geometry::mesh::MeshConfig<int,double,int> meshConfig(partConfig, meshSourceConfig);

	// Build the mesh
	cupcfd::geometry::mesh::CupCfdAoSMesh<int,double,int> * mesh;
	status = meshConfig.buildUnstructuredMesh(&mesh, comm);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);


	// Create a new particle with a constant velocity that doesn't decay
	// Make a cell that just moves to the right in the X dimension

	if(comm.rank == 0)
	{
		cupcfd::geometry::euclidean::EuclideanPoint<double,3> pos(0.12, 0.11, 0.14);
		cupcfd::geometry::euclidean::EuclideanVector<double,3> velocity(1.0, 0.0, 0.0);
		cupcfd::geometry::euclidean::EuclideanVector<double,3> acceleration(0.0, 0.0, 0.0);
		cupcfd::geometry::euclidean::EuclideanVector<double,3> jerk(0.0, 0.0, 0.0);
		uint pID=0, cellID=0, rank=0;
		ParticleSimple<int,double> particle(pos, velocity, acceleration, jerk, pID, cellID, rank, 1.0, 0.0, 0.1);

		double dt;
		int localFaceID;
		status = particle.updatePositionAtomic(*mesh, &dt, &localFaceID, false);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

		// In 0.1 dt, should intend to cross a cell boundary from Global Cell 0 to Global Cell 1
		// Check particle is positioned at boundary
		BOOST_TEST(particle.inflightPos.cmp[0] == 0.20);
		BOOST_TEST(particle.inflightPos.cmp[1] == 0.11);
		BOOST_TEST(particle.inflightPos.cmp[2] == 0.14);

		// Check that the elapsed time was set correctly - i.e. we did not use the full 0.1 since it was
		// atomic across a cell
		BOOST_TEST(dt = 0.08);
		// Check that the ID of the face we reached is correct
		BOOST_CHECK_EQUAL(localFaceID, 1);

		// call again, but without changing the CellID - it should not advance or give the wrong face ID
		status = particle.updatePositionAtomic(*mesh, &dt, &localFaceID, false);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
		BOOST_TEST(dt == 0.0);
		BOOST_CHECK_EQUAL(localFaceID, 1);
	}
}

// === updateVelocityAtomic ===
// Test 1: Test that without an acceleration or jerk, the velocity does not change
BOOST_AUTO_TEST_CASE(updateVelocityAtomic_test1, * utf::tolerance(0.00001))
{
	// Since we test on-node/off node particle status, need to use all ranks for this test
	cupcfd::error::eCodes status;
	cupcfd::comm::Communicator comm(MPI_COMM_WORLD);

	// === Create a small test mesh ===
	// Setup the configurations
	cupcfd::partitioner::PartitionerNaiveConfig<int,int> partConfig;
	cupcfd::geometry::mesh::MeshSourceStructGenConfig<int, double> meshSourceConfig(5, 5, 5, 0.0, 1.0, 0.0, 1.0, 0.0, 1.0);
	cupcfd::geometry::mesh::MeshConfig<int,double,int> meshConfig(partConfig, meshSourceConfig);

	// Build the mesh
	cupcfd::geometry::mesh::CupCfdAoSMesh<int,double,int> * mesh;
	status = meshConfig.buildUnstructuredMesh(&mesh, comm);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);


	// Create a new particle with a constant velocity that doesn't decay
	// Make a cell that just moves to the right in the X dimension

	if(comm.rank == 0)
	{
		cupcfd::geometry::euclidean::EuclideanPoint<double,3> pos(0.12, 0.11, 0.14);
		cupcfd::geometry::euclidean::EuclideanVector<double,3> velocity(1.0, 0.0, 0.0);
		cupcfd::geometry::euclidean::EuclideanVector<double,3> acceleration(0.0, 0.0, 0.0);
		cupcfd::geometry::euclidean::EuclideanVector<double,3> jerk(0.0, 0.0, 0.0);
		uint pID=0, cellID=0, rank=0;
		ParticleSimple<int,double> particle(pos, velocity, acceleration, jerk, pID, cellID, rank, 1.0, 0.0, 0.1);

		// double dt;
		// int localFaceID;
		status = particle.updateVelocityAtomic(*mesh, 0, 100.0);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

		BOOST_TEST(particle.velocity.cmp[0] == 1.0);
		BOOST_TEST(particle.velocity.cmp[1] == 0.0);
		BOOST_TEST(particle.velocity.cmp[2] == 0.0);

		BOOST_TEST(particle.acceleration.cmp[0] == 0.0);
		BOOST_TEST(particle.acceleration.cmp[1] == 0.0);
		BOOST_TEST(particle.acceleration.cmp[2] == 0.0);

		BOOST_TEST(particle.jerk.cmp[0] == 0.0);
		BOOST_TEST(particle.jerk.cmp[1] == 0.0);
		BOOST_TEST(particle.jerk.cmp[2] == 0.0);
	}
}

// Test 2: Test that with an acceleration but no jerk, the velocity changes but the acceleration does not
BOOST_AUTO_TEST_CASE(updateVelocityAtomic_test2, * utf::tolerance(0.00001))
{
	// Since we test on-node/off node particle status, need to use all ranks for this test
	cupcfd::error::eCodes status;
	cupcfd::comm::Communicator comm(MPI_COMM_WORLD);

	// === Create a small test mesh ===
	// Setup the configurations
	cupcfd::partitioner::PartitionerNaiveConfig<int,int> partConfig;
	cupcfd::geometry::mesh::MeshSourceStructGenConfig<int, double> meshSourceConfig(5, 5, 5, 0.0, 1.0, 0.0, 1.0, 0.0, 1.0);
	cupcfd::geometry::mesh::MeshConfig<int,double,int> meshConfig(partConfig, meshSourceConfig);

	// Build the mesh
	cupcfd::geometry::mesh::CupCfdAoSMesh<int,double,int> * mesh;
	status = meshConfig.buildUnstructuredMesh(&mesh, comm);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);


	// Create a new particle with a constant velocity that doesn't decay
	// Make a cell that just moves to the right in the X dimension

	if(comm.rank == 0)
	{
		cupcfd::geometry::euclidean::EuclideanPoint<double,3> pos(0.12, 0.11, 0.14);
		cupcfd::geometry::euclidean::EuclideanVector<double,3> velocity(1.0, 0.0, 0.0);
		cupcfd::geometry::euclidean::EuclideanVector<double,3> acceleration(0.0, 1.2, 0.0);
		cupcfd::geometry::euclidean::EuclideanVector<double,3> jerk(0.0, 0.0, 0.0);
		uint pID=0, cellID=0, rank=0;
		ParticleSimple<int,double> particle(pos, velocity, acceleration, jerk, pID, cellID, rank, 1.0, 0.0, 0.1);

		// double dt;
		// int localFaceID;
		status = particle.updateVelocityAtomic(*mesh, 0, 2.0);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

		BOOST_TEST(particle.velocity.cmp[0] == 1.0);
		BOOST_TEST(particle.velocity.cmp[1] == 2.4);
		BOOST_TEST(particle.velocity.cmp[2] == 0.0);

		BOOST_TEST(particle.acceleration.cmp[0] == 0.0);
		BOOST_TEST(particle.acceleration.cmp[1] == 1.2);
		BOOST_TEST(particle.acceleration.cmp[2] == 0.0);

		BOOST_TEST(particle.jerk.cmp[0] == 0.0);
		BOOST_TEST(particle.jerk.cmp[1] == 0.0);
		BOOST_TEST(particle.jerk.cmp[2] == 0.0);
	}
}

// Test 3: Test that with an acceleration and a jerk, both the velocity and acceleration change.
BOOST_AUTO_TEST_CASE(updateVelocityAtomic_test3, * utf::tolerance(0.00001))
{
	// Since we test on-node/off node particle status, need to use all ranks for this test
	cupcfd::error::eCodes status;
	cupcfd::comm::Communicator comm(MPI_COMM_WORLD);

	// === Create a small test mesh ===
	// Setup the configurations
	cupcfd::partitioner::PartitionerNaiveConfig<int,int> partConfig;
	cupcfd::geometry::mesh::MeshSourceStructGenConfig<int, double> meshSourceConfig(5, 5, 5, 0.0, 1.0, 0.0, 1.0, 0.0, 1.0);
	cupcfd::geometry::mesh::MeshConfig<int,double,int> meshConfig(partConfig, meshSourceConfig);

	// Build the mesh
	cupcfd::geometry::mesh::CupCfdAoSMesh<int,double,int> * mesh;
	status = meshConfig.buildUnstructuredMesh(&mesh, comm);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);


	// Create a new particle with a constant velocity that doesn't decay
	// Make a cell that just moves to the right in the X dimension

	if(comm.rank == 0)
	{
		cupcfd::geometry::euclidean::EuclideanPoint<double,3> pos(0.12, 0.11, 0.14);
		cupcfd::geometry::euclidean::EuclideanVector<double,3> velocity(1.0, 0.0, 0.0);
		cupcfd::geometry::euclidean::EuclideanVector<double,3> acceleration(0.0, 1.2, 0.0);
		cupcfd::geometry::euclidean::EuclideanVector<double,3> jerk(0.2, -0.1, 0.3);
		uint pID=0, cellID=0, rank=0;
		ParticleSimple<int,double> particle(pos, velocity, acceleration, jerk, pID, cellID, rank, 1.0, 0.0, 0.1);

		status = particle.updateVelocityAtomic(*mesh, 0, 2.0);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

		BOOST_TEST(particle.velocity.cmp[0] == 1.0);
		BOOST_TEST(particle.velocity.cmp[1] == 2.4);
		BOOST_TEST(particle.velocity.cmp[2] == 0.0);

		BOOST_TEST(particle.acceleration.cmp[0] == 0.4);
		BOOST_TEST(particle.acceleration.cmp[1] == 1.0);
		BOOST_TEST(particle.acceleration.cmp[2] == 0.6);

		BOOST_TEST(particle.jerk.cmp[0] == 0.2);
		BOOST_TEST(particle.jerk.cmp[1] == -0.1);
		BOOST_TEST(particle.jerk.cmp[2] == 0.3);

		status = particle.updateVelocityAtomic(*mesh, 0, 2.0);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

		BOOST_TEST(particle.velocity.cmp[0] == 1.8);
		BOOST_TEST(particle.velocity.cmp[1] == 4.4);
		BOOST_TEST(particle.velocity.cmp[2] == 1.2);

		BOOST_TEST(particle.acceleration.cmp[0] == 0.8);
		BOOST_TEST(particle.acceleration.cmp[1] == 0.8);
		BOOST_TEST(particle.acceleration.cmp[2] == 1.2);

		BOOST_TEST(particle.jerk.cmp[0] == 0.2);
		BOOST_TEST(particle.jerk.cmp[1] == -0.1);
		BOOST_TEST(particle.jerk.cmp[2] == 0.3);
	}
}

// === updateStateAtomic ===

// === updateNonBoundaryFace ===
// Test 1: Test that the particle cell ID is correctly updated
BOOST_AUTO_TEST_CASE(updateNonBoundaryFace_test1, * utf::tolerance(0.00001))
{
	// Since we test on-node/off node particle status, need to use all ranks for this test
	cupcfd::error::eCodes status;
	cupcfd::comm::Communicator comm(MPI_COMM_WORLD);

	// === Create a small test mesh ===
	// Setup the configurations
	cupcfd::partitioner::PartitionerNaiveConfig<int,int> partConfig;
	cupcfd::geometry::mesh::MeshSourceStructGenConfig<int, double> meshSourceConfig(5, 5, 5, 0.0, 1.0, 0.0, 1.0, 0.0, 1.0);
	cupcfd::geometry::mesh::MeshConfig<int,double,int> meshConfig(partConfig, meshSourceConfig);

	// Build the mesh
	cupcfd::geometry::mesh::CupCfdAoSMesh<int,double,int> * mesh;
	status = meshConfig.buildUnstructuredMesh(&mesh, comm);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);


	// Create a new particle with a constant velocity that doesn't decay
	// Make a cell that just moves to the right in the X dimension

	if(comm.rank == 0)
	{
		cupcfd::geometry::euclidean::EuclideanPoint<double,3> pos(0.12, 0.11, 0.14);
		cupcfd::geometry::euclidean::EuclideanVector<double,3> velocity(1.0, 0.0, 0.0);
		cupcfd::geometry::euclidean::EuclideanVector<double,3> acceleration(0.0, 0.0, 0.0);
		cupcfd::geometry::euclidean::EuclideanVector<double,3> jerk(0.0, 0.0, 0.0);
		uint pID=0, cellID=0, rank=0;
		ParticleSimple<int,double> particle(pos, velocity, acceleration, jerk, pID, cellID, rank, 1.0, 0.0, 0.1);

		double dt;
		int localFaceID;
		status = particle.updatePositionAtomic(*mesh, &dt, &localFaceID, false);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
		BOOST_CHECK_EQUAL(particle.getCellGlobalID(), 0);

		// Test and Check
		status = particle.updateNonBoundaryFace(*mesh, localFaceID);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
		BOOST_CHECK_EQUAL(particle.getCellGlobalID(), 1);
		BOOST_CHECK_EQUAL(particle.getRank(), 0);
	}
}

// Test 2: Test moving to a boundary, then updating the cell, then moving again
BOOST_AUTO_TEST_CASE(updateNonBoundaryFace_test2, * utf::tolerance(0.00001))
{
	// Since we test on-node/off node particle status, need to use all ranks for this test
	cupcfd::error::eCodes status;
	cupcfd::comm::Communicator comm(MPI_COMM_WORLD);

	// === Create a small test mesh ===
	// Setup the configurations
	cupcfd::partitioner::PartitionerNaiveConfig<int,int> partConfig;
	cupcfd::geometry::mesh::MeshSourceStructGenConfig<int, double> meshSourceConfig(5, 5, 5, 0.0, 1.0, 0.0, 1.0, 0.0, 1.0);
	cupcfd::geometry::mesh::MeshConfig<int,double,int> meshConfig(partConfig, meshSourceConfig);

	// Build the mesh
	cupcfd::geometry::mesh::CupCfdAoSMesh<int,double,int> * mesh;
	status = meshConfig.buildUnstructuredMesh(&mesh, comm);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);


	// Create a new particle with a constant velocity that doesn't decay
	// Make a cell that just moves to the right in the X dimension

	if(comm.rank == 0)
	{
		cupcfd::geometry::euclidean::EuclideanPoint<double,3> pos(0.12, 0.11, 0.14);
		cupcfd::geometry::euclidean::EuclideanVector<double,3> velocity(1.0, 0.0, 0.0);
		cupcfd::geometry::euclidean::EuclideanVector<double,3> acceleration(0.0, 0.0, 0.0);
		cupcfd::geometry::euclidean::EuclideanVector<double,3> jerk(0.0, 0.0, 0.0);
		uint pID=0, cellID=0, rank=0;
		ParticleSimple<int,double> particle(pos, velocity, acceleration, jerk, pID, cellID, rank, 1.0, 0.0, 0.3);

		double dt;
		int localFaceID;
		status = particle.updatePositionAtomic(*mesh, &dt, &localFaceID, false);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
		BOOST_CHECK_EQUAL(particle.getCellGlobalID(), 0);
		BOOST_TEST(particle.inflightPos.cmp[0] == 0.20);
		BOOST_TEST(particle.inflightPos.cmp[1] == 0.11);
		BOOST_TEST(particle.inflightPos.cmp[2] == 0.14);

		// Test and Check
		status = particle.updateNonBoundaryFace(*mesh, localFaceID);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
		BOOST_CHECK_EQUAL(particle.getCellGlobalID(), 1);
		BOOST_CHECK_EQUAL(particle.getRank(), 0);

		status = particle.updatePositionAtomic(*mesh, &dt, &localFaceID, false);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
		BOOST_TEST(particle.inflightPos.cmp[0] == 0.40);
		BOOST_TEST(particle.inflightPos.cmp[1] == 0.11);
		BOOST_TEST(particle.inflightPos.cmp[2] == 0.14);

		status = particle.updateNonBoundaryFace(*mesh, localFaceID);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
		BOOST_CHECK_EQUAL(particle.getCellGlobalID(), 2);
		BOOST_CHECK_EQUAL(particle.getRank(), 0);

		status = particle.updatePositionAtomic(*mesh, &dt, &localFaceID, false);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
		BOOST_TEST(particle.inflightPos.cmp[0] == 0.42);
		BOOST_TEST(particle.inflightPos.cmp[1] == 0.11);
		BOOST_TEST(particle.inflightPos.cmp[2] == 0.14);
	}
}

// Test 3: Move particle up to a rank boundary
BOOST_AUTO_TEST_CASE(updateNonBoundaryFace_test3, * utf::tolerance(0.00001))
{
	// Since we test on-node/off node particle status, need to use all ranks for this test
	cupcfd::error::eCodes status;
	cupcfd::comm::Communicator comm(MPI_COMM_WORLD);

	// === Create a small test mesh ===
	// Setup the configurations
	cupcfd::partitioner::PartitionerNaiveConfig<int,int> partConfig;
	cupcfd::geometry::mesh::MeshSourceStructGenConfig<int, double> meshSourceConfig(5, 5, 5, 0.0, 1.0, 0.0, 1.0, 0.0, 1.0);
	cupcfd::geometry::mesh::MeshConfig<int,double,int> meshConfig(partConfig, meshSourceConfig);

	// Build the mesh
	cupcfd::geometry::mesh::CupCfdAoSMesh<int,double,int> * mesh;
	status = meshConfig.buildUnstructuredMesh(&mesh, comm);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);


	// Create a new particle with a constant velocity that doesn't decay
	// Make a cell that just moves to the right in the X dimension

	if(comm.rank == 0)
	{
		cupcfd::geometry::euclidean::EuclideanPoint<double,3> pos(0.97, 0.86, 0.121);
		cupcfd::geometry::euclidean::EuclideanVector<double,3> velocity(0.0, 0.0, 1.0);
		cupcfd::geometry::euclidean::EuclideanVector<double,3> acceleration(0.0, 0.0, 0.0);
		cupcfd::geometry::euclidean::EuclideanVector<double,3> jerk(0.0, 0.0, 0.0);
		uint pID=0, cellID=24, rank=0;
		ParticleSimple<int,double> particle(pos, velocity, acceleration, jerk, pID, cellID, rank, 0.0, 1.0, 0.3);

		double dt;
		int localFaceID;
		status = particle.updatePositionAtomic(*mesh, &dt, &localFaceID, false);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
		BOOST_CHECK_EQUAL(particle.getCellGlobalID(), 24);
		BOOST_TEST(particle.inflightPos.cmp[0] == 0.97);
		BOOST_TEST(particle.inflightPos.cmp[1] == 0.86);
		BOOST_TEST(particle.inflightPos.cmp[2] == 0.2);

		// Test and Check that the correct cell/rank are identified
		status = particle.updateNonBoundaryFace(*mesh, localFaceID);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
		BOOST_CHECK_EQUAL(particle.getCellGlobalID(), 49);
		BOOST_CHECK_EQUAL(particle.getRank(), 1);
	}
}

// === updateBoundaryFaceWall ===
BOOST_AUTO_TEST_CASE(updateBoundaryFaceWall_test1, * utf::tolerance(0.00001))
{
	// Don't care about testing MPI functionality, here, restrict to rank 0
	cupcfd::error::eCodes status;
	cupcfd::comm::Communicator comm(MPI_COMM_SELF);

	if(comm.rank == 0)
	{
		// === Create a small test mesh ===
		// Setup the configurations
		cupcfd::partitioner::PartitionerNaiveConfig<int,int> partConfig;
		cupcfd::geometry::mesh::MeshSourceStructGenConfig<int, double> meshSourceConfig(5, 5, 5, 0.0, 1.0, 0.0, 1.0, 0.0, 1.0);
		cupcfd::geometry::mesh::MeshConfig<int,double,int> meshConfig(partConfig, meshSourceConfig);

		// Build the mesh
		cupcfd::geometry::mesh::CupCfdAoSMesh<int,double,int> * mesh;
		status = meshConfig.buildUnstructuredMesh(&mesh, comm);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);


		// Create a new particle and place it directly on a wall boundary
		cupcfd::geometry::euclidean::EuclideanVector<double,3> acceleration(0.0, 0.0, 0.0);
		cupcfd::geometry::euclidean::EuclideanVector<double,3> jerk(0.0, 0.0, 0.0);
		uint cellID=24, rank=0;

		// Test reflection in X-axis
		cupcfd::geometry::euclidean::EuclideanPoint<double,3> pos1(0.0, 0.12, 0.034);
		cupcfd::geometry::euclidean::EuclideanVector<double,3> velocity1(-5.6, 0.0, 0.0);
		ParticleSimple<int,double> particle1(pos1, velocity1, acceleration, jerk, 0, cellID, rank, 1.0, 0.0, 0.01);
		status = particle1.updateBoundaryFaceWall(*mesh, 0, 0);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
		BOOST_TEST(particle1.velocity.cmp[0] == 5.6);
		BOOST_TEST(particle1.velocity.cmp[1] == 0.0);
		BOOST_TEST(particle1.velocity.cmp[2] == 0.0);


		// Test reflection in Y-axis
		cupcfd::geometry::euclidean::EuclideanPoint<double,3> pos2(0.12, 0.0, 0.034);
		cupcfd::geometry::euclidean::EuclideanVector<double,3> velocity2(0.0, -3.4, 0.0);
		ParticleSimple<int,double> particle2(pos2, velocity2, acceleration, jerk, 1, cellID, rank, 1.0, 0.0, 0.01);
		status = particle2.updateBoundaryFaceWall(*mesh, 0, 150);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
		BOOST_TEST(particle2.velocity.cmp[0] == 0.0);
		BOOST_TEST(particle2.velocity.cmp[1] == 3.4);
		BOOST_TEST(particle2.velocity.cmp[2] == 0.0);

		// Test reflection in Z-axis
		cupcfd::geometry::euclidean::EuclideanPoint<double,3> pos3(0.12, 0.1131, 0.0);
		cupcfd::geometry::euclidean::EuclideanVector<double,3> velocity3(0.1, 1.2, -6.2);
		ParticleSimple<int,double> particle3(pos3, velocity3, acceleration, jerk, 2, cellID, rank, 1.0, 0.0, 0.01);
		status = particle3.updateBoundaryFaceWall(*mesh, 0, 300);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
		BOOST_TEST(particle3.velocity.cmp[0] == 0.1);
		BOOST_TEST(particle3.velocity.cmp[1] == 1.2);
		BOOST_TEST(particle3.velocity.cmp[2] == 6.2);

		// Test reflection in X-axis
		cupcfd::geometry::euclidean::EuclideanPoint<double,3> pos4(2.0, 0.12, 0.034);
		cupcfd::geometry::euclidean::EuclideanVector<double,3> velocity4(5.6, 0.0, 0.0);
		ParticleSimple<int,double> particle4(pos4, velocity4, acceleration, jerk, 3, cellID, rank, 1.0, 0.0, 0.01);
		status = particle4.updateBoundaryFaceWall(*mesh, 0, 1);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
		BOOST_TEST(particle4.velocity.cmp[0] == -5.6);
		BOOST_TEST(particle4.velocity.cmp[1] == 0.0);
		BOOST_TEST(particle4.velocity.cmp[2] == 0.0);

		// Test reflection in Y-axis
		cupcfd::geometry::euclidean::EuclideanPoint<double,3> pos5(0.12, 2.0, 0.034);
		cupcfd::geometry::euclidean::EuclideanVector<double,3> velocity5(0.0, 3.4, 0.0);
		ParticleSimple<int,double> particle5(pos5, velocity5, acceleration, jerk, 4, cellID, rank, 1.0, 0.0, 0.01);
		status = particle5.updateBoundaryFaceWall(*mesh, 0, 155);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
		BOOST_TEST(particle5.velocity.cmp[0] == 0.0);
		BOOST_TEST(particle5.velocity.cmp[1] == -3.4);
		BOOST_TEST(particle5.velocity.cmp[2] == 0.0);

		// Test reflection in Z-axis
		cupcfd::geometry::euclidean::EuclideanPoint<double,3> pos6(0.12, 0.1131, 2.0);
		cupcfd::geometry::euclidean::EuclideanVector<double,3> velocity6(0.1, 1.2, 6.2);
		ParticleSimple<int,double> particle6(pos6, velocity6, acceleration, jerk, 5, cellID, rank, 1.0, 0.0, 0.01);
		status = particle3.updateBoundaryFaceWall(*mesh, 0, 325);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
		BOOST_TEST(particle3.velocity.cmp[0] == 0.1);
		BOOST_TEST(particle3.velocity.cmp[1] == 1.2);
		BOOST_TEST(particle3.velocity.cmp[2] == -6.2);

		// ToDo: Should test faces that don't lie on XYZ planes + test that normal direction is handled correctly
	}
}


// === updateBoundaryFaceSymp ===

// === updateBoundaryFaceInlet ===

// === updateBoundaryFaceOutlet ===



// === isRegistered ===
// Not sure quite where to/when to place this test....
// Should use a setter/getter to set/get registered?

// === register and deregisterMPIType ===
// Test 1: Test that Type is registered without error, and can be deregistered without error
BOOST_AUTO_TEST_CASE(registerMPIType_test1, * utf::tolerance(0.00001))
{
	cupcfd::error::eCodes status;
	ParticleSimple<int, double> particle;

	status = particle.registerMPIType();
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	status = particle.deregisterMPIType();
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
}

// Test 2: Test that if it is already registered, it does not register again
BOOST_AUTO_TEST_CASE(registerMPIType_test2, * utf::tolerance(0.00001))
{
	cupcfd::error::eCodes status;
	ParticleSimple<int, double> particle;

	status = particle.registerMPIType();
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	status = particle.registerMPIType();
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_MPI_DATATYPE_REGISTERED);

	// Cleanup for future tests since we use a static variable!
	status = particle.deregisterMPIType();
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
}

// Test 3: Test that if unregistered, we cannot deregister again
BOOST_AUTO_TEST_CASE(registerMPIType_test3, * utf::tolerance(0.00001))
{
	cupcfd::error::eCodes status;
	ParticleSimple<int, double> particle;

	BOOST_CHECK_EQUAL(particle.isRegistered(), false);
	status = particle.deregisterMPIType();
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_MPI_DATATYPE_UNREGISTERED);
}

// === getMPIType ===
// Test 1: Test that corrected ID is returned for the registered type with error code
BOOST_AUTO_TEST_CASE(getMPIType_test1, * utf::tolerance(0.00001))
{
	cupcfd::error::eCodes status;
	ParticleSimple<int, double> particle;
	MPI_Datatype dType;

	status = particle.registerMPIType();
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	status = particle.getMPIType(&dType);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	// Cleanup for future tests since we use a static variable!
	status = particle.deregisterMPIType();
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
}

// Test 2: Test error case detected when type unregistered
BOOST_AUTO_TEST_CASE(getMPIType_test2, * utf::tolerance(0.00001))
{
	cupcfd::error::eCodes status;
	ParticleSimple<int, double> particle;
	MPI_Datatype dType;

	BOOST_CHECK_EQUAL(particle.isRegistered(), false);
	status = particle.getMPIType(&dType);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_MPI_DATATYPE_UNREGISTERED);
}

// Test 3: Test corrected ID is returned for method without error code
BOOST_AUTO_TEST_CASE(getMPIType_test3, * utf::tolerance(0.00001))
{

}


// === BroadcastParticles ===
// Test 1: Test the MPI Type by ensuring that we can broadcast two particles
BOOST_AUTO_TEST_CASE(BroadcastParticles_test1)
{
	cupcfd::error::eCodes status;
	cupcfd::comm::Communicator comm(MPI_COMM_WORLD);

	// Create Two Particles on Rank 0

	// Register the type (only once per all tests in file since static?)
	ParticleSimple<int, double> particle;

	// Should not yet be registered
	BOOST_CHECK_EQUAL(particle.isRegistered(), false);

	// Register
	status = particle.registerMPIType();
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	// Should now be registered
	BOOST_CHECK_EQUAL(particle.isRegistered(), true);

	if(comm.rank == 0)
	{
		uint pID=0, cellID=5, rank=2;
		cupcfd::geometry::euclidean::EuclideanPoint<double,3> a(1.0, 2.0, 3.0);
		cupcfd::geometry::euclidean::EuclideanVector<double,3> b(4.0, 5.0, 6.0);
		cupcfd::geometry::euclidean::EuclideanVector<double,3> c(7.0, 8.0, 9.0);
		cupcfd::geometry::euclidean::EuclideanVector<double,3> d(10.0, 11.0, 12.0);
		ParticleSimple<int, double> particle2(a, b, c, d, pID, cellID, rank, 7.9, 2.3, 0.0);

		pID=1, cellID=15, rank=8;
		cupcfd::geometry::euclidean::EuclideanPoint<double,3> f(11.0, 12.0, 13.0);
		cupcfd::geometry::euclidean::EuclideanVector<double,3> g(14.0, 15.0, 16.0);
		cupcfd::geometry::euclidean::EuclideanVector<double,3> h(17.0, 18.0, 19.0);
		cupcfd::geometry::euclidean::EuclideanVector<double,3> i(110.0, 111.0, 112.0);
		ParticleSimple<int, double> particle3(f, g, h, i, pID, cellID, rank, 8.7, 0.563, 0.0);

		ParticleSimple<int, double> particles[2] = {particle2, particle3};

		status = cupcfd::comm::Broadcast(particles, 2, 0, comm);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	}
	else
	{
		ParticleSimple<int, double> recvParticles[2];

		status = cupcfd::comm::Broadcast(recvParticles, 2, 0, comm);
		BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

		BOOST_CHECK_EQUAL(recvParticles[0].pos.cmp[0], 1.0);
		BOOST_CHECK_EQUAL(recvParticles[0].pos.cmp[1], 2.0);
		BOOST_CHECK_EQUAL(recvParticles[0].pos.cmp[2], 3.0);
		BOOST_CHECK_EQUAL(recvParticles[0].velocity.cmp[0], 4.0);
		BOOST_CHECK_EQUAL(recvParticles[0].velocity.cmp[1], 5.0);
		BOOST_CHECK_EQUAL(recvParticles[0].velocity.cmp[2], 6.0);
		BOOST_CHECK_EQUAL(recvParticles[0].acceleration.cmp[0], 7.0);
		BOOST_CHECK_EQUAL(recvParticles[0].acceleration.cmp[1], 8.0);
		BOOST_CHECK_EQUAL(recvParticles[0].acceleration.cmp[2], 9.0);
		BOOST_CHECK_EQUAL(recvParticles[0].jerk.cmp[0], 10.0);
		BOOST_CHECK_EQUAL(recvParticles[0].jerk.cmp[1], 11.0);
		BOOST_CHECK_EQUAL(recvParticles[0].jerk.cmp[2], 12.0);
		BOOST_CHECK_EQUAL(recvParticles[0].getCellGlobalID(), 5);
		BOOST_CHECK_EQUAL(recvParticles[0].getRank(), 2);
		BOOST_CHECK_EQUAL(recvParticles[0].decayLevel, 7.9);
		BOOST_CHECK_EQUAL(recvParticles[0].decayRate, 2.3);

		BOOST_CHECK_EQUAL(recvParticles[1].pos.cmp[0], 11.0);
		BOOST_CHECK_EQUAL(recvParticles[1].pos.cmp[1], 12.0);
		BOOST_CHECK_EQUAL(recvParticles[1].pos.cmp[2], 13.0);
		BOOST_CHECK_EQUAL(recvParticles[1].velocity.cmp[0], 14.0);
		BOOST_CHECK_EQUAL(recvParticles[1].velocity.cmp[1], 15.0);
		BOOST_CHECK_EQUAL(recvParticles[1].velocity.cmp[2], 16.0);
		BOOST_CHECK_EQUAL(recvParticles[1].acceleration.cmp[0], 17.0);
		BOOST_CHECK_EQUAL(recvParticles[1].acceleration.cmp[1], 18.0);
		BOOST_CHECK_EQUAL(recvParticles[1].acceleration.cmp[2], 19.0);
		BOOST_CHECK_EQUAL(recvParticles[1].jerk.cmp[0], 110.0);
		BOOST_CHECK_EQUAL(recvParticles[1].jerk.cmp[1], 111.0);
		BOOST_CHECK_EQUAL(recvParticles[1].jerk.cmp[2], 112.0);
		BOOST_CHECK_EQUAL(recvParticles[1].getCellGlobalID(), 15);
		BOOST_CHECK_EQUAL(recvParticles[1].getRank(), 8);
		BOOST_CHECK_EQUAL(recvParticles[1].decayLevel, 8.7);
		BOOST_CHECK_EQUAL(recvParticles[1].decayRate, 0.563);
	}

	// Cleanup for future tests since we use a static variable!
	status = particle.deregisterMPIType();
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);


}

BOOST_AUTO_TEST_CASE(cleanup)
{
	// Cleanup these MPI datatypes
	cupcfd::geometry::euclidean::EuclideanPoint<double, 3> point;
	cupcfd::geometry::euclidean::EuclideanVector<double,3> vector;
	cupcfd::error::eCodes status;
	status = point.deregisterMPIType();
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	status = vector.deregisterMPIType();
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

    MPI_Finalize();
}
