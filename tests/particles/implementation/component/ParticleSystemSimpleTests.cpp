/*
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Unit Tests for the concrete methods of the ParticleSystemSimple class
 */

#define BOOST_TEST_MODULE ParticleSystemSimple
#include <boost/test/unit_test.hpp>
#include <boost/test/output_test_stream.hpp>
#include <stdexcept>

#include "ParticleEmitterSimple.h"
#include "EuclideanPoint.h"
#include "PartitionerConfig.h"
#include "PartitionerNaiveConfig.h"
#include "MeshSourceStructGenConfig.h"
#include "MeshConfig.h"
#include "CupCfdAoSMesh.h"
#include "Error.h"
#include "DistributionFixed.h"
#include "DistributionNormal.h"
#include "DistributionUniform.h"
#include <memory>
#include "ParticleSimple.h"
#include "ParticleSystemSimple.h"

namespace utf = boost::unit_test;
namespace euc = cupcfd::geometry::euclidean;
namespace meshgeo = cupcfd::geometry::mesh;
namespace dist = cupcfd::distributions;

using namespace cupcfd::particles;

// Setup
BOOST_AUTO_TEST_CASE(setup)
{
    int argc = boost::unit_test::framework::master_test_suite().argc;
    char ** argv = boost::unit_test::framework::master_test_suite().argv;

    MPI_Init(&argc, &argv);

	// Need to register point, vector MPI datatype since the particle MPI datatype depends on them
	cupcfd::geometry::euclidean::EuclideanPoint<double, 3> point;
	point.registerMPIType();

	cupcfd::geometry::euclidean::EuclideanVector<double,3> vector;
	vector.registerMPIType();

	ParticleSimple<int, double> particle;
	particle.registerMPIType();
}

// === Constructor ===
// Test 1:
BOOST_AUTO_TEST_CASE(constructor_test1)
{
	cupcfd::error::eCodes status;
	cupcfd::comm::Communicator comm(MPI_COMM_SELF);

	// === Create a small test mesh ===
	// Setup the configurations
	cupcfd::partitioner::PartitionerNaiveConfig<int,int> partConfig;
	cupcfd::geometry::mesh::MeshSourceStructGenConfig<int, double> meshSourceConfig(5, 5, 5, 0.0, 1.0, 0.0, 1.0, 0.0, 1.0);
	cupcfd::geometry::mesh::MeshConfig<int,double,int> meshConfig(partConfig, meshSourceConfig);

	// Build the mesh
	cupcfd::geometry::mesh::CupCfdAoSMesh<int,double,int> * mesh;
	status = meshConfig.buildUnstructuredMesh(&mesh, comm);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	std::shared_ptr<meshgeo::CupCfdAoSMesh<int,double,int>> meshPtr(mesh);

	// Create the particle system
	ParticleSystemSimple<meshgeo::CupCfdAoSMesh<int,double,int>, int, double, int> particleSystem(meshPtr);

	// ToDo: What to test?
	// Check mesh properties stored inside system are correct?
}

// === addParticle ===
BOOST_AUTO_TEST_CASE(addParticle_test1, * utf::tolerance(0.00001))
{
	cupcfd::error::eCodes status;
	cupcfd::comm::Communicator comm(MPI_COMM_SELF);

	// === Create a small test mesh ===
	// Setup the configurations
	cupcfd::partitioner::PartitionerNaiveConfig<int,int> partConfig;
	cupcfd::geometry::mesh::MeshSourceStructGenConfig<int, double> meshSourceConfig(5, 5, 5, 0.0, 1.0, 0.0, 1.0, 0.0, 1.0);
	cupcfd::geometry::mesh::MeshConfig<int,double,int> meshConfig(partConfig, meshSourceConfig);

	// Build the mesh
	cupcfd::geometry::mesh::CupCfdAoSMesh<int,double,int> * mesh;
	status = meshConfig.buildUnstructuredMesh(&mesh, comm);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	std::shared_ptr<meshgeo::CupCfdAoSMesh<int,double,int>> meshPtr(mesh);

	// Create the particle system
	ParticleSystemSimple<meshgeo::CupCfdAoSMesh<int,double,int>, int, double, int> system(meshPtr);

	// Add a particle
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> pos1(0.12, 0.11, 0.14);
	cupcfd::geometry::euclidean::EuclideanVector<double,3> velocity1(1.0, 1.1, 1.2);
	cupcfd::geometry::euclidean::EuclideanVector<double,3> acceleration1(0.0, 0.0, 0.0);
	cupcfd::geometry::euclidean::EuclideanVector<double,3> jerk1(0.0, 0.0, 0.0);
	ParticleSimple<int,double> particle1(pos1, velocity1, acceleration1, jerk1, 0, 0, 1000.0, 0.0, 0.0);

	system.addParticle(particle1);
	BOOST_CHECK_EQUAL(system.getNActiveParticles(), 1);	// 1 Active Particle
	BOOST_CHECK_EQUAL(system.getNTravelParticles(), 0); // Travel time was set to 0, so no travelling particles

	// ToDo: Further Checks
}

// === addParticleEmitter ===
// Test 1:
BOOST_AUTO_TEST_CASE(addParticleEmitter_test1, * utf::tolerance(0.00001))
{
	cupcfd::error::eCodes status;
	cupcfd::comm::Communicator comm(MPI_COMM_SELF);

	// === Create a small test mesh ===
	// Setup the configurations
	cupcfd::partitioner::PartitionerNaiveConfig<int,int> partConfig;
	cupcfd::geometry::mesh::MeshSourceStructGenConfig<int, double> meshSourceConfig(5, 5, 5, 0.0, 1.0, 0.0, 1.0, 0.0, 1.0);
	cupcfd::geometry::mesh::MeshConfig<int,double,int> meshConfig(partConfig, meshSourceConfig);

	// Build the mesh
	cupcfd::geometry::mesh::CupCfdAoSMesh<int,double,int> * mesh;
	status = meshConfig.buildUnstructuredMesh(&mesh, comm);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	std::shared_ptr<meshgeo::CupCfdAoSMesh<int,double,int>> meshPtr(mesh);

	// Create the particle system
	ParticleSystemSimple<meshgeo::CupCfdAoSMesh<int,double,int>, int, double, int> system(meshPtr);

	// Add an emitter to the system
	cupcfd::geometry::euclidean::EuclideanPoint<double, 3> position(2.0, 3.0, 4.0);
    dist::DistributionFixed<int,double> rate(2.3);
    dist::DistributionFixed<int,double> angleXY(78);
    dist::DistributionFixed<int,double> angleRotation(62);
    dist::DistributionFixed<int,double> speed(0.052);
    dist::DistributionFixed<int,double> accelerationX(0.012);
    dist::DistributionFixed<int,double> accelerationY(0.012);
    dist::DistributionFixed<int,double> accelerationZ(0.012);
    dist::DistributionFixed<int,double> jerkX(0.01);
    dist::DistributionFixed<int,double> jerkY(0.01);
    dist::DistributionFixed<int,double> jerkZ(0.01);
    dist::DistributionFixed<int,double> decayRate(0.1);
    dist::DistributionFixed<int,double> decayThreshold(10);

    ParticleEmitterSimple<int,double> emitter(0, 10, comm.rank, position, &rate, &angleXY, &angleRotation, &speed,
			  &accelerationX, &accelerationY, &accelerationZ,
			  &jerkX, &jerkY, &jerkZ,
			  &decayRate, &decayThreshold);

    system.addParticleEmitter(emitter);
}

// === setParticleInactive

// === removeInactiveParticles ===
BOOST_AUTO_TEST_CASE(removeInactiveParticles_test1, * utf::tolerance(0.00001))
{
	cupcfd::error::eCodes status;
	cupcfd::comm::Communicator comm(MPI_COMM_SELF);

	// === Create a small test mesh ===
	// Setup the configurations
	cupcfd::partitioner::PartitionerNaiveConfig<int,int> partConfig;
	cupcfd::geometry::mesh::MeshSourceStructGenConfig<int, double> meshSourceConfig(5, 5, 5, 0.0, 1.0, 0.0, 1.0, 0.0, 1.0);
	cupcfd::geometry::mesh::MeshConfig<int,double,int> meshConfig(partConfig, meshSourceConfig);

	// Build the mesh
	cupcfd::geometry::mesh::CupCfdAoSMesh<int,double,int> * mesh;
	status = meshConfig.buildUnstructuredMesh(&mesh, comm);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	std::shared_ptr<meshgeo::CupCfdAoSMesh<int,double,int>> meshPtr(mesh);

	// Create the particle system
	ParticleSystemSimple<meshgeo::CupCfdAoSMesh<int,double,int>, int, double, int> system(meshPtr);

	// Add a particle
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> pos1(0.12, 0.11, 0.14);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> pos2(0.13, 0.11, 0.14);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> pos3(0.14, 0.11, 0.14);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> pos4(0.15, 0.11, 0.14);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> pos5(0.16, 0.11, 0.14);

	cupcfd::geometry::euclidean::EuclideanVector<double,3> velocity1(1.0, 1.1, 1.2);
	cupcfd::geometry::euclidean::EuclideanVector<double,3> acceleration1(0.0, 0.0, 0.0);
	cupcfd::geometry::euclidean::EuclideanVector<double,3> jerk1(0.0, 0.0, 0.0);

	ParticleSimple<int,double> particle1(pos1, velocity1, acceleration1, jerk1, 0, 0, 1000.0, 0.0, 0.0);
	ParticleSimple<int,double> particle2(pos2, velocity1, acceleration1, jerk1, 0, 0, 1000.0, 0.0, 0.0);
	ParticleSimple<int,double> particle3(pos3, velocity1, acceleration1, jerk1, 0, 0, 1000.0, 0.0, 0.0);
	ParticleSimple<int,double> particle4(pos4, velocity1, acceleration1, jerk1, 0, 0, 1000.0, 0.0, 0.0);
	ParticleSimple<int,double> particle5(pos5, velocity1, acceleration1, jerk1, 0, 0, 1000.0, 0.0, 0.0);

	system.addParticle(particle1);
	system.addParticle(particle2);
	system.addParticle(particle3);
	system.addParticle(particle4);
	system.addParticle(particle5);

	// Currently 5 'active'
	// Set two to inactive and remove them
	system.setParticleInactive(2);
	system.setParticleInactive(3);

	BOOST_CHECK_EQUAL(system.getNParticles(), 5);	// 3 active, 2 inactive
	system.removeInactiveParticles();
	BOOST_CHECK_EQUAL(system.getNParticles(), 3);	// 3 active, 0 inactive

	// Check system properties
	BOOST_CHECK_EQUAL(system.getNActiveParticles(), 3);
	BOOST_CHECK_EQUAL(system.getNTravelParticles(), 0);
	BOOST_TEST(system.particles[0].pos.cmp[0] == 0.12);
	BOOST_TEST(system.particles[1].pos.cmp[0] == 0.13);
	BOOST_TEST(system.particles[2].pos.cmp[0] == 0.16);
}

// === generateEmitterParticles ===


// === exchangeParticles ===
BOOST_AUTO_TEST_CASE(exchangeParticles_test1)
{
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
	std::shared_ptr<meshgeo::CupCfdAoSMesh<int,double,int>> meshPtr(mesh);

	// Create the particle system
	ParticleSystemSimple<meshgeo::CupCfdAoSMesh<int,double,int>, int, double, int> system(meshPtr);

	// Add a particle
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> pos1(0.12, 0.11, 0.14);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> pos2(0.13, 0.11, 0.14);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> pos3(0.14, 0.11, 0.14);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> pos4(0.15, 0.11, 0.14);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> pos5(0.16, 0.11, 0.14);

	cupcfd::geometry::euclidean::EuclideanVector<double,3> velocity1(1.0, 1.1, 1.2);
	cupcfd::geometry::euclidean::EuclideanVector<double,3> acceleration1(0.0, 0.0, 0.0);
	cupcfd::geometry::euclidean::EuclideanVector<double,3> jerk1(0.0, 0.0, 0.0);

	if(comm.rank == 0)
	{
		// We will add these exclusively to Rank 0, but set their destination ranks to something different
		// Note: Technically the Cell IDs are incorrect for the destination ranks, but if we are just
		// testing the exchange it shouldn't be an issue
		// Note: For this test the process IDs have to be carefully selected, since the ranks must
		// be mesh neighbours if they are to send to one another
		ParticleSimple<int,double> particle2(pos2, velocity1, acceleration1, jerk1, 0, 0, 1000.0, 0.0, 0.0);
		ParticleSimple<int,double> particle3(pos3, velocity1, acceleration1, jerk1, 0, 1, 1000.0, 0.0, 0.0);
		system.addParticle(particle2);
		system.addParticle(particle3);
	}
	else if(comm.rank == 1)
	{
		ParticleSimple<int,double> particle4(pos4, velocity1, acceleration1, jerk1, 0, 2, 1000.0, 0.0, 13.3);
		system.addParticle(particle4);
	}
	else if(comm.rank == 2)
	{
		ParticleSimple<int,double> particle1(pos1, velocity1, acceleration1, jerk1, 0, 3, 1000.0, 0.0, 5.7);
		ParticleSimple<int,double> particle5(pos5, velocity1, acceleration1, jerk1, 0, 2, 1000.0, 0.0, 0.0);

		system.addParticle(particle1);
		system.addParticle(particle5);
	}

	// Test and Check
	system.exchangeParticles();
	system.removeInactiveParticles(); // The exchange process should have marked sent particles as inactive

	if(comm.rank == 0)
	{
		BOOST_CHECK_EQUAL(system.getNActiveParticles(), 1);
		BOOST_CHECK_EQUAL(system.getNTravelParticles(), 0);
		BOOST_TEST(system.particles[0].pos.cmp[0] == 0.13);
	}
	else if(comm.rank == 1)
	{
		BOOST_CHECK_EQUAL(system.getNActiveParticles(), 1);
		BOOST_CHECK_EQUAL(system.getNTravelParticles(), 0);
		BOOST_TEST(system.particles[0].pos.cmp[0] == 0.14);
	}
	else if(comm.rank == 2)
	{
		BOOST_CHECK_EQUAL(system.getNActiveParticles(), 2);
		BOOST_CHECK_EQUAL(system.getNTravelParticles(), 1);
		BOOST_TEST(system.particles[0].pos.cmp[0] == 0.16);
		BOOST_TEST(system.particles[1].pos.cmp[0] == 0.15);
	}
	else if(comm.rank == 3)
	{
		BOOST_CHECK_EQUAL(system.getNActiveParticles(), 1);
		BOOST_CHECK_EQUAL(system.getNTravelParticles(), 1);
		BOOST_TEST(system.particles[0].pos.cmp[0] == 0.12);
	}
}

// === updateSystem ===
// Test 1: Test correct movement of particles around system in a serial mesh
BOOST_AUTO_TEST_CASE(updateSystem_test1, * utf::tolerance(0.00001))
{
	cupcfd::error::eCodes status;
	cupcfd::comm::Communicator comm(MPI_COMM_SELF);

	// === Create a small test mesh ===
	// Setup the configurations
	cupcfd::partitioner::PartitionerNaiveConfig<int,int> partConfig;
	cupcfd::geometry::mesh::MeshSourceStructGenConfig<int, double> meshSourceConfig(5, 5, 5, 0.0, 1.0, 0.0, 1.0, 0.0, 1.0);
	cupcfd::geometry::mesh::MeshConfig<int,double,int> meshConfig(partConfig, meshSourceConfig);

	// Build the mesh
	cupcfd::geometry::mesh::CupCfdAoSMesh<int,double,int> * mesh;
	status = meshConfig.buildUnstructuredMesh(&mesh, comm);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
	std::shared_ptr<meshgeo::CupCfdAoSMesh<int,double,int>> meshPtr(mesh);

	// Create the particle system
	ParticleSystemSimple<meshgeo::CupCfdAoSMesh<int,double,int>, int, double, int> system(meshPtr);

	// Add a particle
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> pos1(0.12, 0.11, 0.14);
	cupcfd::geometry::euclidean::EuclideanVector<double,3> velocity1(1.0, 1.1, 1.2);
	cupcfd::geometry::euclidean::EuclideanVector<double,3> acceleration1(0.0, 0.0, 0.0);
	cupcfd::geometry::euclidean::EuclideanVector<double,3> jerk1(0.0, 0.0, 0.0);
	ParticleSimple<int,double> particle1(pos1, velocity1, acceleration1, jerk1, 0, 0, 1000.0, 0.0, 0.0);

	system.addParticle(particle1);

	// Advance system by 119 seconds/time units
	// This should lead to it bouncing off
	system.updateSystem(19);

	// Check that the particle has ended up in the correct position (no acceleration/jerk to make this easier
	// to manually compute)
	BOOST_TEST(system.particles[0].pos.cmp[0] == 0.88);
	BOOST_TEST(system.particles[0].pos.cmp[1] == 0.99);
	BOOST_TEST(system.particles[0].pos.cmp[2] == 0.94);
}

// Test 2: Test correct movement of particles around system, including across ranks
BOOST_AUTO_TEST_CASE(updateSystem_test2, * utf::tolerance(0.00001))
{
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
	std::shared_ptr<meshgeo::CupCfdAoSMesh<int,double,int>> meshPtr(mesh);

	// Create the particle system
	ParticleSystemSimple<meshgeo::CupCfdAoSMesh<int,double,int>, int, double, int> system(meshPtr);

	// Add a particle
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> pos1(0.12, 0.11, 0.14);
	cupcfd::geometry::euclidean::EuclideanVector<double,3> velocity1(1.0, 1.1, 1.2);
	cupcfd::geometry::euclidean::EuclideanVector<double,3> acceleration1(0.0, 0.0, 0.0);
	cupcfd::geometry::euclidean::EuclideanVector<double,3> jerk1(0.0, 0.0, 0.0);
	ParticleSimple<int,double> particle1(pos1, velocity1, acceleration1, jerk1, 0, 0, 1000.0, 0.0, 0.0);

	if(comm.rank == 0)
	{
		system.addParticle(particle1);
	}

	// Advance system by 19 seconds/time units
	// This should lead to it bouncing around the system
	system.updateSystem(19);

	// Check that the particle has ended up in the correct position (no acceleration/jerk to make this easier
	// to manually compute)
	if(comm.rank == 0)
	{
		BOOST_CHECK_EQUAL(system.particles.size(), 0);
	}
	else if(comm.rank == 1)
	{
		BOOST_CHECK_EQUAL(system.particles.size(), 0);
	}
	else if(comm.rank == 2)
	{
		BOOST_CHECK_EQUAL(system.particles.size(), 0);
	}
	else if(comm.rank == 3)
	{
		BOOST_CHECK_EQUAL(system.particles.size(), 1);
		BOOST_TEST(system.particles[0].pos.cmp[0] == 0.88);
		BOOST_TEST(system.particles[0].pos.cmp[1] == 0.99);
		BOOST_TEST(system.particles[0].pos.cmp[2] == 0.94);
	}
}

// Test 3: Test correct movement of particles when there is also an emitter (so new particles should be generated too)
BOOST_AUTO_TEST_CASE(updateSystem_test3, * utf::tolerance(0.00001))
{
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
	std::shared_ptr<meshgeo::CupCfdAoSMesh<int,double,int>> meshPtr(mesh);

	// Create the particle system
	ParticleSystemSimple<meshgeo::CupCfdAoSMesh<int,double,int>, int, double, int> system(meshPtr);

	// Add an emitter to the system
	cupcfd::geometry::euclidean::EuclideanPoint<double, 3> position(0.07, 0.12, 0.08);
    dist::DistributionFixed<int,double> rate(2.3);
    dist::DistributionFixed<int,double> angleXY(1.2);
    dist::DistributionFixed<int,double> angleRotation(1.3);
    dist::DistributionFixed<int,double> speed(0.052);
    dist::DistributionFixed<int,double> accelerationX(0.0);
    dist::DistributionFixed<int,double> accelerationY(0.0);
    dist::DistributionFixed<int,double> accelerationZ(0.0);
    dist::DistributionFixed<int,double> jerkX(0.0);
    dist::DistributionFixed<int,double> jerkY(0.0);
    dist::DistributionFixed<int,double> jerkZ(0.0);
    dist::DistributionFixed<int,double> decayRate(0.0);
    dist::DistributionFixed<int,double> decayThreshold(10);

    if(comm.rank == 0)
    {
    	ParticleEmitterSimple<int,double> emitter(0, 0, comm.rank, position, &rate, &angleXY, &angleRotation, &speed,
				  &accelerationX, &accelerationY, &accelerationZ,
				  &jerkX, &jerkY, &jerkZ,
				  &decayRate, &decayThreshold);
        system.addParticleEmitter(emitter);
    }

	// Add a particle
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> pos1(0.12, 0.11, 0.14);
	cupcfd::geometry::euclidean::EuclideanVector<double,3> velocity1(1.0, 1.1, 1.2);
	cupcfd::geometry::euclidean::EuclideanVector<double,3> acceleration1(0.0, 0.0, 0.0);
	cupcfd::geometry::euclidean::EuclideanVector<double,3> jerk1(0.0, 0.0, 0.0);
	ParticleSimple<int,double> particle1(pos1, velocity1, acceleration1, jerk1, 0, 0, 1000.0, 0.0, 0.0);

	if(comm.rank == 0)
	{
		system.addParticle(particle1);
	}

	// Advance system by 2.31 seconds/time units
	// This should lead to a second particle being created and mover ever so slightly
	system.updateSystem(2.31);

	// Check that the particle has ended up in the correct position (no acceleration/jerk to make this easier
	// to manually compute)
	if(comm.rank == 0)
	{
		// Note this particle was generated right at the end of the time period and had little time to advance
		BOOST_CHECK_EQUAL(system.getNParticles(), 1);
		BOOST_CHECK_EQUAL(system.particles[0].cellGlobalID, 0);
		BOOST_TEST(system.particles[0].pos.cmp[0] == 0.0700504);
		BOOST_TEST(system.particles[0].pos.cmp[1] == 0.1204846);
		BOOST_TEST(system.particles[0].pos.cmp[2] == 0.07981844);

		// Next particle generation time should be 2.29 into the next dt period
		BOOST_TEST(system.emitters[0].nextParticleTime == 2.29);
	}
	else if(comm.rank == 1)
	{
		BOOST_CHECK_EQUAL(system.getNParticles(), 0);
	}
	else if(comm.rank == 2)
	{
		BOOST_CHECK_EQUAL(system.getNParticles(), 0);
	}
	else if(comm.rank == 3)
	{
		BOOST_CHECK_EQUAL(system.getNParticles(), 1);
		BOOST_CHECK_EQUAL(system.particles[0].cellGlobalID, 117);
		BOOST_TEST(system.particles[0].pos.cmp[0] == 0.43);
		BOOST_TEST(system.particles[0].pos.cmp[1] == 0.651);
		BOOST_TEST(system.particles[0].pos.cmp[2] == 0.912);
	}
}

// Cleanup
BOOST_AUTO_TEST_CASE(cleanup)
{
	// Cleanup these MPI datatypes
	cupcfd::geometry::euclidean::EuclideanPoint<double, 3> point;
	cupcfd::geometry::euclidean::EuclideanVector<double,3> vector;
	ParticleSimple<int, double> particle;

	particle.deregisterMPIType();
	point.deregisterMPIType();
	vector.deregisterMPIType();

    MPI_Finalize();
}
