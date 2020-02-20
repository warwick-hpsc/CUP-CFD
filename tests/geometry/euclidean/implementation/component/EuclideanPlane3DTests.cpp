/*
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 */

#define BOOST_TEST_MODULE EuclideanPlane3D
#include <boost/test/unit_test.hpp>
#include <boost/test/output_test_stream.hpp>
#include <stdexcept>

#include "EuclideanPlane3D.h"
#include "Error.h"

namespace utf = boost::unit_test;
using namespace cupcfd::geometry::euclidean;

// === Constructor ===
// Test 1: Test the correct values are setup by the constructor
BOOST_AUTO_TEST_CASE(constructor_test1, * utf::tolerance(0.00001))
{
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> p1(2.0, 3.0, 4.0);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> p2(18.6, 17.4, 33.9);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> p3(15.3, 6.7, 41.7);
	cupcfd::geometry::euclidean::EuclideanPlane3D<double> plane(p1, p2, p3);

	// Check Points have been copied
	BOOST_CHECK_EQUAL(plane.p1.cmp[0], p1.cmp[0]);
	BOOST_CHECK_EQUAL(plane.p1.cmp[1], p1.cmp[1]);
	BOOST_CHECK_EQUAL(plane.p1.cmp[2], p1.cmp[2]);

	BOOST_CHECK_EQUAL(plane.p2.cmp[0], p2.cmp[0]);
	BOOST_CHECK_EQUAL(plane.p2.cmp[1], p2.cmp[1]);
	BOOST_CHECK_EQUAL(plane.p2.cmp[2], p2.cmp[2]);

	BOOST_CHECK_EQUAL(plane.p3.cmp[0], p3.cmp[0]);
	BOOST_CHECK_EQUAL(plane.p3.cmp[1], p3.cmp[1]);
	BOOST_CHECK_EQUAL(plane.p3.cmp[2], p3.cmp[2]);

	// Check Plane Equation (These are the Non GCD Versions)
	BOOST_TEST(plane.a == 432.25);
	BOOST_TEST(plane.b == -228.15);
	BOOST_TEST(plane.c == -130.0999);
	BOOST_TEST(plane.d == 340.35);
}

// === operator = ===
// Test 1: Check copy works successfully
BOOST_AUTO_TEST_CASE(operator_single_equals_test1, * utf::tolerance(0.00001))
{

}

// === operator == ===
// Test 1: Test that true is obtained when the stored values are the same
BOOST_AUTO_TEST_CASE(operator_double_equals_test1, * utf::tolerance(0.00001))
{

}

// Test 2: Test that false is obtained when p1 is different
BOOST_AUTO_TEST_CASE(operator_double_equals_test2, * utf::tolerance(0.00001))
{

}

// Test 3: Test that false is obtained when p2 is different
BOOST_AUTO_TEST_CASE(operator_double_equals_test3, * utf::tolerance(0.00001))
{

}

// Test 4: Test that false is obtained when p3 is different
BOOST_AUTO_TEST_CASE(operator_double_equals_test4, * utf::tolerance(0.00001))
{

}

// === operator != ===
// Test 1: Test that false is obtained when the stored values are the same
BOOST_AUTO_TEST_CASE(operator_double_notequals_test1, * utf::tolerance(0.00001))
{

}

// Test 2: Test that true is obtained when p1 is different and p2, p3 are the same
BOOST_AUTO_TEST_CASE(operator_double_notequals_test2, * utf::tolerance(0.00001))
{

}

// Test 3: Test that true is obtained when p2 is different and p1, p3 are the same
BOOST_AUTO_TEST_CASE(operator_double_notequals_test3, * utf::tolerance(0.00001))
{

}

// Test 4: Test that true is obtained when p3 is different and p1, p2 are the same
BOOST_AUTO_TEST_CASE(operator_double_notequals_test4, * utf::tolerance(0.00001))
{

}

// === operator < ===
// ToDo
BOOST_AUTO_TEST_CASE(operator_less_than_test1, * utf::tolerance(0.00001))
{

}

// === operator <= ===
// ToDo
BOOST_AUTO_TEST_CASE(operator_less_than_or_equals_test1, * utf::tolerance(0.00001))
{

}

// === operator > ===
// ToDo
BOOST_AUTO_TEST_CASE(operator_greater_than_test1, * utf::tolerance(0.00001))
{

}

// === operator >= ===
// ToDo
BOOST_AUTO_TEST_CASE(operator_greater_than_or_equals_test1, * utf::tolerance(0.00001))
{

}

// === getNormal ===
// Test 1: Get the correct normal from a plane object
// ToDo
BOOST_AUTO_TEST_CASE(getNormal_test1, * utf::tolerance(0.00001))
{

}

// === normal ===
// Test 1: Test correct normal
BOOST_AUTO_TEST_CASE(normal_test2, * utf::tolerance(0.00001))
{
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> p1(2.0, 3.0, 4.0);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> p2(18.6, 17.4, 33.9);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> p3(15.3, 6.7, 41.7);
	cupcfd::geometry::euclidean::EuclideanVector<double,3> norm;
	cupcfd::error::eCodes status;

	norm = cupcfd::geometry::euclidean::EuclideanPlane3D<double>::normal(p1, p2, p3);

	// Wolfram Alpha gives these, but these are the GCD version
	//BOOST_TEST(norm.x == 0.854612);
	//BOOST_TEST(norm.y == -0.451081);
	//BOOST_TEST(norm.z == -0.257224);

	BOOST_TEST(norm.cmp[0] == 432.25);
	BOOST_TEST(norm.cmp[1] == -228.15);
	BOOST_TEST(norm.cmp[2] == -130.0999);
}

// === computeScalarPlaneEquation ===
// Test 1: Test correct equation is calculated
BOOST_AUTO_TEST_CASE(computeScalarPlaneEquation_test1, * utf::tolerance(0.00001))
{
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> p1(2.0, 3.0, 4.0);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> p2(18.6, 17.4, 33.9);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> p3(15.3, 6.7, 41.7);
	cupcfd::geometry::euclidean::EuclideanPlane3D<double> plane(p1, p2, p3);
	cupcfd::error::eCodes status;
	double a, b, c, d;

	status = plane.computeScalarPlaneEquation(&a, &b, &c, &d);

	BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);

	// Wolfram Alpha gives these, but these are the GCD version - factor ~ -131
	//BOOST_TEST(a == -3.32244);
	//BOOST_TEST(b == 1.75365);
	//BOOST_TEST(c == 1.0);
	//BOOST_TEST(d == -2.61606);

	BOOST_TEST(a == 432.25);
	BOOST_TEST(b == -228.15);
	BOOST_TEST(c == -130.0999);
	BOOST_TEST(d == 340.35);
}

// === isPointOnPlane ===
// Test 1: Test true is returned when the point lies on the plane - Test point p1
BOOST_AUTO_TEST_CASE(isPointOnPlane_test1, * utf::tolerance(0.00001))
{
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> p1(2.0, 3.0, 4.0);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> p2(18.6, 17.4, 33.9);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> p3(15.3, 6.7, 41.7);
	cupcfd::geometry::euclidean::EuclideanPlane3D<double> plane(p1, p2, p3);

	bool check = plane.isPointOnPlane(p1);
	BOOST_CHECK_EQUAL(check, true);
}

// Test 2: Test true is returned when the point lies on the plane - Test point p2
BOOST_AUTO_TEST_CASE(isPointOnPlane_test2, * utf::tolerance(0.00001))
{
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> p1(2.0, 3.0, 4.0);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> p2(18.6, 17.4, 33.9);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> p3(15.3, 6.7, 41.7);
	cupcfd::geometry::euclidean::EuclideanPlane3D<double> plane(p1, p2, p3);

	bool check = plane.isPointOnPlane(p2);
	BOOST_CHECK_EQUAL(check, true);
}

// Test 3: Test true is returned when the point lies on the plane - Test point p3
BOOST_AUTO_TEST_CASE(isPointOnPlane_test3, * utf::tolerance(0.00001))
{
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> p1(2.0, 3.0, 4.0);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> p2(18.6, 17.4, 33.9);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> p3(15.3, 6.7, 41.7);
	cupcfd::geometry::euclidean::EuclideanPlane3D<double> plane(p1, p2, p3);

	bool check = plane.isPointOnPlane(p3);
	BOOST_CHECK_EQUAL(check, true);
}

// Test 4: Test true is returned when the point lies on the plane - Arbitrary point
BOOST_AUTO_TEST_CASE(isPointOnPlane_test4, * utf::tolerance(0.00001))
{

}

// Test 5: Test false is returned when the point does not lie on the plane
BOOST_AUTO_TEST_CASE(isPointOnPlane_test5, * utf::tolerance(0.00001))
{
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> p1(2.0, 3.0, 4.0);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> p2(18.6, 17.4, 33.9);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> p3(15.3, 6.7, 41.7);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> p4(-2000.0, -2000.0, -2000.0);
	cupcfd::geometry::euclidean::EuclideanPlane3D<double> plane(p1, p2, p3);

	bool check = plane.isPointOnPlane(p4);
	BOOST_CHECK_EQUAL(check, false);
}

// === computeProjectedPoint ===
// Test 1: Test the correct projection of a point that lies above a flat plane in the z-axis
BOOST_AUTO_TEST_CASE(computeProjectedPoint_test1, * utf::tolerance(0.00001))
{
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> p1(2.8, 4.0, 2.0);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> p2(21.7, 6.0, 2.0);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> p3(14.3, 1.0, 2.0);
	cupcfd::geometry::euclidean::EuclideanPlane3D<double> plane(p1, p2, p3);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> testPoint(21.7, 6.0, 21.3);

	cupcfd::geometry::euclidean::EuclideanPoint<double,3> compare(21.7, 6.0, 2.0);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> result;

	result = plane.computeProjectedPoint(testPoint);

	BOOST_TEST(compare.cmp[0] == result.cmp[0]);
	BOOST_TEST(compare.cmp[1] == result.cmp[1]);
	BOOST_TEST(compare.cmp[2] == result.cmp[2]);
}

// Test 2: Test the correct projection of a point that lies below a flat plane in the z-axis
BOOST_AUTO_TEST_CASE(computeProjectedPoint_test2, * utf::tolerance(0.00001))
{

}

// Test 3: Test the correct projection of a point that lies on a flat plane in the z-axis (i.e. should be same point)
BOOST_AUTO_TEST_CASE(computeProjectedPoint_test3, * utf::tolerance(0.00001))
{

}

// === isVectorParallel ===
// Test 1: Test that a vector is correctly identified as being in parallel
BOOST_AUTO_TEST_CASE(isVectorParallel_test1, * utf::tolerance(0.00001))
{
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> p1(2.0, 3.0, 4.0);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> p2(18.6, 17.4, 33.9);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> p3(15.3, 6.7, 41.7);
	cupcfd::geometry::euclidean::EuclideanPlane3D<double> plane(p1, p2, p3);

	// Vector from p1 to p2 should be in parallel
	cupcfd::geometry::euclidean::EuclideanVector<double,3> vec = p2 - p1;
	bool parallel = plane.isVectorParallel(vec);
	BOOST_CHECK_EQUAL(parallel, true);
}

// Test 2: Test that a vector is correctly identified as not being in parallel
BOOST_AUTO_TEST_CASE(isVectorParallel_test2, * utf::tolerance(0.00001))
{
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> p1(2.0, 3.0, 4.0);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> p2(18.6, 17.4, 33.9);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> p3(15.3, 6.7, 41.7);
	cupcfd::geometry::euclidean::EuclideanPlane3D<double> plane(p1, p2, p3);

	// Normal vector should not be in parallel by definition
	cupcfd::geometry::euclidean::EuclideanVector<double,3> vec(432.25, -228.15, -130.0999);
	bool parallel = plane.isVectorParallel(vec);
	BOOST_CHECK_EQUAL(parallel, false);
}

// === isVectorParallelInPlane ===
// Test 1: Test true is returned when a parallel vector in the plane is detected
BOOST_AUTO_TEST_CASE(isVectorParallelInPlane_test1, * utf::tolerance(0.00001))
{
	// Setup
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> p1(2.0, 3.0, 4.0);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> p2(18.6, 17.4, 33.9);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> p3(15.3, 6.7, 41.7);
	cupcfd::geometry::euclidean::EuclideanPlane3D<double> plane(p1, p2, p3);

	// Generate a vector that is parallel to the plane
	cupcfd::geometry::euclidean::EuclideanVector<double,3> vec = p2 - p1;

	// Test and Check - Select a point on the plane with the parallel vector
	bool result = plane.isVectorParallelInPlane(vec, p1);
	BOOST_CHECK_EQUAL(result, true);
}

// Test 2: Test false is returned when a vector is parallel but does not lie on the plane
BOOST_AUTO_TEST_CASE(isVectorParallelInPlane_test2, * utf::tolerance(0.00001))
{
	// Setup
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> p1(2.0, 3.0, 4.0);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> p2(18.6, 17.4, 33.9);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> p3(15.3, 6.7, 41.7);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> origin(0.0, 0.0, 0.0);
	cupcfd::geometry::euclidean::EuclideanPlane3D<double> plane(p1, p2, p3);

	// Generate a vector that is parallel to the plane
	cupcfd::geometry::euclidean::EuclideanVector<double,3> vec = p2 - p1;

	// Test and Check - Select a point on the plane with the parallel vector
	bool result = plane.isVectorParallelInPlane(vec, origin);
	BOOST_CHECK_EQUAL(result, false);
}

// Test 3: Test false is returned when a vector is not parallel
BOOST_AUTO_TEST_CASE(isVectorParallelInPlane_test3, * utf::tolerance(0.00001))
{
	// Setup
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> p1(2.0, 3.0, 4.0);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> p2(18.6, 17.4, 33.9);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> p3(15.3, 6.7, 41.7);
	cupcfd::geometry::euclidean::EuclideanPlane3D<double> plane(p1, p2, p3);

	// Generate a vector that is parallel to the plane
	cupcfd::geometry::euclidean::EuclideanVector<double,3> vec(0.0, 0.0, 4.0);

	// Test and Check - Select a point on the plane with the parallel vector
	bool result = plane.isVectorParallelInPlane(vec, p1);
	BOOST_CHECK_EQUAL(result, false);
}

// === linePlaneIntersection ===
// Test 1: Find intersection of non-parallel vector with plane
BOOST_AUTO_TEST_CASE(linePlaneIntersection_test1, * utf::tolerance(0.00001))
{

}

// Test 2: Error code when vector is parallel in plane
BOOST_AUTO_TEST_CASE(linePlaneIntersection_test2, * utf::tolerance(0.00001))
{

}

// Test 3: Error code when vector is parallel but not in plane
BOOST_AUTO_TEST_CASE(linePlaneIntersection_test3, * utf::tolerance(0.00001))
{

}

// === linePlaneIntersection (static) ===
// Test 1: Find intersection of non-parallel vector with plane
BOOST_AUTO_TEST_CASE(linePlaneIntersection_static_test1, * utf::tolerance(0.00001))
{

}

// Test 2: Error code when vector is parallel in plane
BOOST_AUTO_TEST_CASE(linePlaneIntersection_static_test2, * utf::tolerance(0.00001))
{
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> p1(2.0, 3.0, 4.0);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> p2(18.6, 17.4, 33.9);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> p3(15.3, 6.7, 41.7);
	cupcfd::geometry::euclidean::EuclideanPlane3D<double> plane(p1, p2, p3);

	cupcfd::geometry::euclidean::EuclideanVector<double,3> normal = plane.getNormal();
	cupcfd::geometry::euclidean::EuclideanVector<double,3> parallelVec = p2-p1;
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> result;

	cupcfd::error::eCodes status = cupcfd::geometry::euclidean::EuclideanPlane3D<double>::linePlaneIntersection(normal, p3, parallelVec, p1, result);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_EUC_VEC_PARALLEL);
}

// Test 3: Error code when vector is parallel but not in plane
BOOST_AUTO_TEST_CASE(linePlaneIntersection_static_test3, * utf::tolerance(0.00001))
{
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> p1(2.0, 3.0, 4.0);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> p2(18.6, 17.4, 33.9);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> p3(15.3, 6.7, 41.7);
	cupcfd::geometry::euclidean::EuclideanPlane3D<double> plane(p1, p2, p3);

	cupcfd::geometry::euclidean::EuclideanVector<double,3> normal = plane.getNormal();
	cupcfd::geometry::euclidean::EuclideanVector<double,3> parallelVec = p2-p1;
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> l0(0.0, 0.0, 0.0);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> result;

	cupcfd::error::eCodes status = cupcfd::geometry::euclidean::EuclideanPlane3D<double>::linePlaneIntersection(normal, p3, parallelVec, l0, result);
	BOOST_CHECK_EQUAL(status, cupcfd::error::E_EUC_VEC_PARALLEL);
}

// === shortestDistance ===
// Test 1: Correctly compute the distance to a point from the plane where both are on the positive side of the origin
BOOST_AUTO_TEST_CASE(shortestDistance_test1, * utf::tolerance(0.00001))
{
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> p1(2.0, 3.0, 4.0);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> p2(18.6, 17.4, 33.9);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> p3(15.3, 6.7, 41.7);
	cupcfd::geometry::euclidean::EuclideanPlane3D<double> plane(p1, p2, p3);

	cupcfd::geometry::euclidean::EuclideanPoint<double,3> point(15.3,8.9, 43.5);

	double distance = plane.shortestDistance(point);

	BOOST_TEST(distance == 1.45538);
}

// Test 2: Correctly compute the distance to a point from the plane where the planes are on different sides of the origin
BOOST_AUTO_TEST_CASE(shortestDistance_test2, * utf::tolerance(0.00001))
{
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> p1(2.0, 3.0, 4.0);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> p2(18.6, 17.4, 33.9);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> p3(15.3, 6.7, 41.7);
	cupcfd::geometry::euclidean::EuclideanPlane3D<double> plane(p1, p2, p3);

	cupcfd::geometry::euclidean::EuclideanPoint<double,3> point(201.7,165.3,198.4);

	double distance = plane.shortestDistance(point);

	BOOST_TEST(distance == 47.4512);
}

// Test 3: Correctly compute the distance to a point on the plane
BOOST_AUTO_TEST_CASE(shortestDistance_test3, * utf::tolerance(0.00001))
{
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> p1(2.0, 3.0, 4.0);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> p2(18.6, 17.4, 33.9);
	cupcfd::geometry::euclidean::EuclideanPoint<double,3> p3(15.3, 6.7, 41.7);
	cupcfd::geometry::euclidean::EuclideanPlane3D<double> plane(p1, p2, p3);

	cupcfd::geometry::euclidean::EuclideanPoint<double,3> point(18.6, 17.4, 33.9);

	double distance = plane.shortestDistance(point);

	BOOST_TEST(distance == 0.0);
}
