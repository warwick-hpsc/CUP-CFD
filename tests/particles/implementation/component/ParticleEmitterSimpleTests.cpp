/*
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Unit Tests for the concrete methods of the Particle class
 */

#define BOOST_TEST_MODULE ParticleEmitterSimple
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
}


// === Constructor ===
// Test 1: Check the data values are setup correctly
BOOST_AUTO_TEST_CASE(constructor_test1, * utf::tolerance(0.00001))
{
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

    ParticleEmitterSimple<int,double> emitter(0, 10, 4, 0, position, &rate, &angleXY, &angleRotation, &speed,
    										&accelerationX, &accelerationY, &accelerationZ,
											&jerkX, &jerkY, &jerkZ,
											&decayRate, &decayThreshold);

    BOOST_CHECK_EQUAL(emitter.localCellID, 0);
    BOOST_CHECK_EQUAL(emitter.globalCellID, 10);
    BOOST_TEST(emitter.position.cmp[0] == 2.0);
    BOOST_TEST(emitter.position.cmp[1] == 3.0);
    BOOST_TEST(emitter.position.cmp[2] == 4.0);
    BOOST_CHECK_EQUAL(emitter.rank, 4);
}

// Test 2: Check the copy constructor copies values correctly
BOOST_AUTO_TEST_CASE(constructor_test2, * utf::tolerance(0.00001))
{

}

// === generateParticles ===
// Test 1: Generate some particles with predictable values for testing
BOOST_AUTO_TEST_CASE(generateParticles_test1, * utf::tolerance(0.00001))
{
	cupcfd::geometry::euclidean::EuclideanPoint<double, 3> position(2.0, 3.0, 4.0);
    dist::DistributionFixed<int,double> rate(2.3);
    dist::DistributionFixed<int,double> angleXY(-0.6);
    dist::DistributionFixed<int,double> angleRotation(1.3);
    dist::DistributionFixed<int,double> speed(0.052);
    dist::DistributionFixed<int,double> accelerationX(0.012);
    dist::DistributionFixed<int,double> accelerationY(0.013);
    dist::DistributionFixed<int,double> accelerationZ(0.014);
    dist::DistributionFixed<int,double> jerkX(0.01);
    dist::DistributionFixed<int,double> jerkY(0.02);
    dist::DistributionFixed<int,double> jerkZ(0.03);
    dist::DistributionFixed<int,double> decayRate(0.1);
    dist::DistributionFixed<int,double> decayThreshold(10);

    // Rank is set to silly arbitrary value just to check it copies since we have no communicator
    ParticleEmitterSimple<int,double> emitter(0, 10, 721, 0, position, &rate, &angleXY, &angleRotation, &speed,
			  &accelerationX, &accelerationY, &accelerationZ,
			  &jerkX, &jerkY, &jerkZ,
			  &decayRate, &decayThreshold);

    ParticleSimple<int,double> * particles;
    int nParticles;

    emitter.generateParticles(&particles, &nParticles, 10.0);

    // In 10 seconds, at rate of 2.3, expected 4 particles
    BOOST_CHECK_EQUAL(nParticles, 4);

    cupcfd::geometry::euclidean::EuclideanVector<double,3> posCmp[4] = {
    		cupcfd::geometry::euclidean::EuclideanVector<double,3>(2.0,3.0,4.0),
			cupcfd::geometry::euclidean::EuclideanVector<double,3>(2.0,3.0,4.0),
			cupcfd::geometry::euclidean::EuclideanVector<double,3>(2.0,3.0,4.0),
			cupcfd::geometry::euclidean::EuclideanVector<double,3>(2.0,3.0,4.0)
    };

    cupcfd::geometry::euclidean::EuclideanVector<double,3> velCmp[4] = {
    		cupcfd::geometry::euclidean::EuclideanVector<double,3>(0.011480352, -0.029361384, -0.041353468),
			cupcfd::geometry::euclidean::EuclideanVector<double,3>(0.011480352, -0.029361384, -0.041353468),
			cupcfd::geometry::euclidean::EuclideanVector<double,3>(0.011480352, -0.029361384, -0.041353468),
			cupcfd::geometry::euclidean::EuclideanVector<double,3>(0.011480352, -0.029361384, -0.041353468)
    };


    cupcfd::geometry::euclidean::EuclideanVector<double,3> accelCmp[4] = {
    		cupcfd::geometry::euclidean::EuclideanVector<double,3>(0.012,0.013,0.014),
			cupcfd::geometry::euclidean::EuclideanVector<double,3>(0.012,0.013,0.014),
			cupcfd::geometry::euclidean::EuclideanVector<double,3>(0.012,0.013,0.014),
			cupcfd::geometry::euclidean::EuclideanVector<double,3>(0.012,0.013,0.014)
    };

    cupcfd::geometry::euclidean::EuclideanVector<double,3> jerkCmp[4] = {
    		cupcfd::geometry::euclidean::EuclideanVector<double,3>(0.01,0.02,0.03),
			cupcfd::geometry::euclidean::EuclideanVector<double,3>(0.01,0.02,0.03),
			cupcfd::geometry::euclidean::EuclideanVector<double,3>(0.01,0.02,0.03),
			cupcfd::geometry::euclidean::EuclideanVector<double,3>(0.01,0.02,0.03)};
    double decayRateCmp[4] = {0.1, 0.1, 0.1, 0.1};
    double decayThresholdCmp[4] = {10.0, 10.0, 10.0, 10.0};

    double travelTimes[4] = {7.7, 5.4, 3.1, 0.8};

    for(int i = 0; i < 4; i++)
    {
    	BOOST_TEST(particles[i].getPos().cmp[0] == posCmp[i].cmp[0]);
    	BOOST_TEST(particles[i].getPos().cmp[1] == posCmp[i].cmp[1]);
    	BOOST_TEST(particles[i].getPos().cmp[2] == posCmp[i].cmp[2]);

    	BOOST_TEST(particles[i].getInFlightPos().cmp[0] == posCmp[i].cmp[0]);
    	BOOST_TEST(particles[i].getInFlightPos().cmp[1] == posCmp[i].cmp[1]);
    	BOOST_TEST(particles[i].getInFlightPos().cmp[2] == posCmp[i].cmp[2]);

    	BOOST_TEST(particles[i].getVelocity().cmp[0] == velCmp[i].cmp[0]);
    	BOOST_TEST(particles[i].getVelocity().cmp[1] == velCmp[i].cmp[1]);
    	BOOST_TEST(particles[i].getVelocity().cmp[2] == velCmp[i].cmp[2]);

    	BOOST_TEST(particles[i].getAcceleration().cmp[0] == accelCmp[i].cmp[0]);
    	BOOST_TEST(particles[i].getAcceleration().cmp[1] == accelCmp[i].cmp[1]);
    	BOOST_TEST(particles[i].getAcceleration().cmp[2] == accelCmp[i].cmp[2]);

    	BOOST_TEST(particles[i].getJerk().cmp[0] == jerkCmp[i].cmp[0]);
    	BOOST_TEST(particles[i].getJerk().cmp[1] == jerkCmp[i].cmp[1]);
    	BOOST_TEST(particles[i].getJerk().cmp[2] == jerkCmp[i].cmp[2]);

    	BOOST_TEST(particles[i].getCellGlobalID() == 10);

    	BOOST_TEST(particles[i].getTravelTime() == travelTimes[i]);

    	BOOST_TEST(particles[i].getDecayLevel() == decayThresholdCmp[i]);

    	BOOST_TEST(particles[i].getDecayRate() == decayRateCmp[i]);

    	BOOST_CHECK_EQUAL(particles[i].getRank(), 721);
    }

    free(particles);

    // Do one more round to make sure the correct time is used from the previous run that overran
    emitter.generateParticles(&particles, &nParticles, 1.6);

    // Particle should be generated at 11.5, so expected travel time of 0.1
    // (Previous dt of 10 + this time period of 1.6 = 11.6 seconds elapsed)
    BOOST_CHECK_EQUAL(nParticles, 1);
    BOOST_TEST(particles[0].getTravelTime() == 0.1);

}


BOOST_AUTO_TEST_CASE(cleanup)
{
    MPI_Finalize();
}
