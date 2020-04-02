/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Main Method
 */

#include <string>
#include <iostream>

#include "MeshConfigSourceJSON.h"
#include "MeshConfig.h"
#include "CupCfdAoSMesh.h"
#include "CupCfdSoAMesh.h"

#include "Communicator.h"

#include "Benchmark.h"

#include "BenchmarkKernels.h"
#include "BenchmarkConfigKernels.h"
#include "BenchmarkConfigKernelsJSON.h"

#include "BenchmarkExchange.h"
#include "BenchmarkConfigExchange.h"
#include "BenchmarkConfigExchangeJSON.h"

#include "BenchmarkLinearSolver.h"
#include "BenchmarkConfigLinearSolver.h"
#include "BenchmarkConfigLinearSolverJSON.h"

#include "SparseMatrixCSR.h"

#include "ParticleSystemSimpleConfigJSON.h"
#include "ParticleSystemSimple.h"
#include "ParticleEmitterSimple.h"
#include "ParticleSimple.h"

#include "ParticleSystemSimple.h"

#include "BenchmarkConfigParticleSystemSimpleJSON.h"

#include <memory>

// Timer Interface
#include "tt_interface_c.h"

#include "petscsys.h"

// JsonCPP - Supplied as standalone in include/io/jsoncpp
#include "json.h"
#include "json-forwards.h"
#include <fstream>

#include "CupCfd.h"
#include "SystemConfigJSON.h"

#include "EuclideanPoint.h"
#include "EuclideanVector.h"
#include "ParticleSimple.h"


namespace mesh = cupcfd::geometry::mesh;

int main (int argc, char ** argv)
{
	cupcfd::error::eCodes status;

	MPI_Init(&argc, &argv);

	// Add ifdef for petsc?
	PetscInitialize(&argc, &argv, NULL, NULL);

	TreeTimerInit();

	// Use all processes
	cupcfd::comm::Communicator comm(MPI_COMM_WORLD);

	// Register the custom MPI types
	// ToDo: Would prefer to use static methods for these, but since we
	// setup the interface via inheritance its difficult to do so
	cupcfd::geometry::euclidean::EuclideanPoint<double, 3> point;
	point.registerMPIType();

	cupcfd::geometry::euclidean::EuclideanVector<double,3> vector;
	vector.registerMPIType();

	cupcfd::particles::ParticleSimple<int, double> particle;
	particle.registerMPIType();


	// === Main Configuration Setup ===

	// Identify where to source the run configuration from
	// Currently hard-coded to specificly named JSON file
	std::string configPath = "./cupcfd.json";

	// Verify configuration file exists


	// Identify which types to use for the run

	// Defaults
	cupcfd::intDataType iData = cupcfd::INT_DATATYPE_INT;
	cupcfd::floatingDataType fData = cupcfd::FLOAT_DATATYPE_FLOAT;
	cupcfd::meshDataType mData = cupcfd::MESH_DATATYPE_MINIAOS;

	Json::Value configData;
	std::ifstream source(configPath, std::ifstream::binary);
	source >> configData;

	if(configData.isMember("DataTypes"))
	{
		if(comm.rank == 0)
		{
			std::cout << "Reading DataTypes\n";
		}

		cupcfd::SystemConfigJSON dataTypeJSON(configData["DataTypes"]);

		status = dataTypeJSON.getIntegerDataType(&iData);
		if(status != cupcfd::error::E_SUCCESS)
		{
			if(comm.rank == 0)
			{
				std::cout << "Warning: Could not read Integer Datatype from JSON. Using Default Integer Type of int";
			}
		}

		status = dataTypeJSON.getFloatingPointDataType(&fData);
		if(status != cupcfd::error::E_SUCCESS)
		{
			if(comm.rank == 0)
			{
				std::cout << "Warning: Could not read Floating Datatype from JSON. Using Default Integer Type of float";
			}
		}

		status = dataTypeJSON.getMeshSpecialisationType(&mData);
		if(status != cupcfd::error::E_SUCCESS)
		{
			if(comm.rank == 0)
			{
				std::cout << "Warning: Could not read Mesh Datatype from JSON. Using Default Mesh Type of CupCfdAoS";
			}
		}
	}

	// === Mesh ====

	// Downside of CRTP - need to know types at compile time, so we're forced to have multiple branches here.
	// May figure out a while to do this in a more reusable fashion at another time
	if(iData == cupcfd::INT_DATATYPE_INT && fData == cupcfd::FLOAT_DATATYPE_DOUBLE && mData == cupcfd::MESH_DATATYPE_MINIAOS)
	{
		// Mesh is reused across multiple components, so it is loaded as its own configuration step
		// Mesh Config Source
		std::string topLevel[0] = {};
		cupcfd::geometry::mesh::MeshConfigSourceJSON<int, double, int> configFile(configPath, topLevel, 0);

		// Build Mesh Config
		cupcfd::geometry::mesh::MeshConfig<int,double,int> * meshConfig;
		status = configFile.buildMeshConfig(&meshConfig);
		if(status != cupcfd::error::E_SUCCESS)
		{
			std::cout << "Error Encountered: Cannot Parse a JSON Mesh Configuration at " << configPath << "\n";
			std::cout << "Ending Benchmarking\n";
			int ierr = -1;
			MPI_Abort(MPI_COMM_WORLD, ierr);
		}

		// Build Mesh
		if(comm.rank == 0)
		{
			std::cout << "Building Mesh\n";
		}

		mesh::CupCfdAoSMesh<int, double, int> * mesh;
		status = meshConfig->buildUnstructuredMesh(&mesh, comm);
		if(status != cupcfd::error::E_SUCCESS)
		{
			std::cout << "Error Encountered: Failed to build Mesh with current configuration. Please check the provided configuration is correct.\n";
			std::cout << "Ending Benchmarking\n";
			int ierr = -1;
			MPI_Abort(MPI_COMM_WORLD, ierr);
		}

		// ToDo: Should only ever exist as a shared pointer, not a raw pointer, but this is the only instance for now
		// so convert to a shared pointer and never use the raw pointer again (even for deleting)
		// This needs to be shared for safely passing around the mesh
		std::shared_ptr<mesh::CupCfdAoSMesh<int, double, int>> meshPtr(mesh);

		// === Benchmarking System ===
		// This should create and setup the top level structure that will contain the benchmarks, state etc.
		// ToDo: For now, it is hard-coded to read from a JSON file (by passing the path), but it should technically
		// be moved out to get input from a generic 'source' structure.

		cupcfd::CupCfd<cupcfd::geometry::mesh::CupCfdAoSMesh<int,double,int>,int,double,int> run(configPath, meshPtr);
	}
	else if(iData == cupcfd::INT_DATATYPE_INT && fData == cupcfd::FLOAT_DATATYPE_FLOAT && mData == cupcfd::MESH_DATATYPE_MINIAOS)
	{
		// Mesh is reused across multiple components, so it is loaded as its own configuration step
		// Mesh Config Source
		std::string topLevel[0] = {};
		cupcfd::geometry::mesh::MeshConfigSourceJSON<int, float, int> configFile(configPath, topLevel, 0);

		// Build Mesh Config
		cupcfd::geometry::mesh::MeshConfig<int,float,int> * meshConfig;
		status = configFile.buildMeshConfig(&meshConfig);
		if(status != cupcfd::error::E_SUCCESS)
		{
			std::cout << "Error Encountered: Cannot Parse a JSON Mesh Configuration at " << configPath << "\n";
			std::cout << "Ending Benchmarking\n";
			int ierr = -1;
			MPI_Abort(MPI_COMM_WORLD, ierr);
		}

		// Build Mesh
		if(comm.rank == 0)
		{
			std::cout << "Building Mesh\n";
		}

		mesh::CupCfdAoSMesh<int, float, int> * mesh;
		status = meshConfig->buildUnstructuredMesh(&mesh, comm);
		if(status != cupcfd::error::E_SUCCESS)
		{
			std::cout << "Error Encountered: Failed to build Mesh with current configuration. Please check the provided configuration is correct.\n";
			std::cout << "Ending Benchmarking\n";
			int ierr = -1;
			MPI_Abort(MPI_COMM_WORLD, ierr);
		}

		// ToDo: Should only ever exist as a shared pointer, not a raw pointer, but this is the only instance for now
		// so convert to a shared pointer and never use the raw pointer again (even for deleting)
		// This needs to be shared for safely passing around the mesh
		std::shared_ptr<mesh::CupCfdAoSMesh<int, float, int>> meshPtr(mesh);

		// === Benchmarking System ===
		// This should create and setup the top level structure that will contain the benchmarks, state etc.
		// ToDo: For now, it is hard-coded to read from a JSON file (by passing the path), but it should technically
		// be moved out to get input from a generic 'source' structure.

		cupcfd::CupCfd<cupcfd::geometry::mesh::CupCfdAoSMesh<int,float,int>,int,float,int> run(configPath, meshPtr);
	}
	else if(iData == cupcfd::INT_DATATYPE_INT && fData == cupcfd::FLOAT_DATATYPE_DOUBLE && mData == cupcfd::MESH_DATATYPE_MINISOA)
	{
		// Mesh is reused across multiple components, so it is loaded as its own configuration step
		// Mesh Config Source
		std::string topLevel[0] = {};
		cupcfd::geometry::mesh::MeshConfigSourceJSON<int, double, int> configFile(configPath, topLevel, 0);

		// Build Mesh Config
		cupcfd::geometry::mesh::MeshConfig<int,double,int> * meshConfig;
		status = configFile.buildMeshConfig(&meshConfig);
		if(status != cupcfd::error::E_SUCCESS)
		{
			std::cout << "Error Encountered: Cannot Parse a JSON Mesh Configuration at " << configPath << "\n";
			std::cout << "Ending Benchmarking\n";
			int ierr = -1;
			MPI_Abort(MPI_COMM_WORLD, ierr);
		}

		// Build Mesh
		if(comm.rank == 0)
		{
			std::cout << "Building Mesh\n";
		}

		mesh::CupCfdSoAMesh<int, double, int> * mesh;
		status = meshConfig->buildUnstructuredMesh(&mesh, comm);
		if(status != cupcfd::error::E_SUCCESS)
		{
			std::cout << "Error Encountered: Failed to build Mesh with current configuration. Please check the provided configuration is correct.\n";
			std::cout << "Ending Benchmarking\n";
			int ierr = -1;
			MPI_Abort(MPI_COMM_WORLD, ierr);
		}

		// ToDo: Should only ever exist as a shared pointer, not a raw pointer, but this is the only instance for now
		// so convert to a shared pointer and never use the raw pointer again (even for deleting)
		// This needs to be shared for safely passing around the mesh
		std::shared_ptr<mesh::CupCfdSoAMesh<int, double, int>> meshPtr(mesh);

		// === Benchmarking System ===
		// This should create and setup the top level structure that will contain the benchmarks, state etc.
		// ToDo: For now, it is hard-coded to read from a JSON file (by passing the path), but it should technically
		// be moved out to get input from a generic 'source' structure.

		cupcfd::CupCfd<cupcfd::geometry::mesh::CupCfdSoAMesh<int,double,int>,int,double,int> run(configPath, meshPtr);
	}
	else if(iData == cupcfd::INT_DATATYPE_INT && fData == cupcfd::FLOAT_DATATYPE_FLOAT && mData == cupcfd::MESH_DATATYPE_MINISOA)
	{
		// Mesh is reused across multiple components, so it is loaded as its own configuration step
		// Mesh Config Source
		std::string topLevel[0] = {};
		cupcfd::geometry::mesh::MeshConfigSourceJSON<int, float, int> configFile(configPath, topLevel, 0);

		// Build Mesh Config
		cupcfd::geometry::mesh::MeshConfig<int,float,int> * meshConfig;
		status = configFile.buildMeshConfig(&meshConfig);
		if(status != cupcfd::error::E_SUCCESS)
		{
			std::cout << "Error Encountered: Cannot Parse a JSON Mesh Configuration at " << configPath << "\n";
			std::cout << "Ending Benchmarking\n";
			int ierr = -1;
			MPI_Abort(MPI_COMM_WORLD, ierr);
		}

		// Build Mesh
		if(comm.rank == 0)
		{
			std::cout << "Building Mesh\n";
		}

		mesh::CupCfdSoAMesh<int, float, int> * mesh;
		status = meshConfig->buildUnstructuredMesh(&mesh, comm);
		if(status != cupcfd::error::E_SUCCESS)
		{
			std::cout << "Error Encountered: Failed to build Mesh with current configuration. Please check the provided configuration is correct.\n";
			std::cout << "Ending Benchmarking\n";
			int ierr = -1;
			MPI_Abort(MPI_COMM_WORLD, ierr);
		}

		// ToDo: Should only ever exist as a shared pointer, not a raw pointer, but this is the only instance for now
		// so convert to a shared pointer and never use the raw pointer again (even for deleting)
		// This needs to be shared for safely passing around the mesh
		std::shared_ptr<mesh::CupCfdSoAMesh<int, float, int>> meshPtr(mesh);

		// === Benchmarking System ===
		// This should create and setup the top level structure that will contain the benchmarks, state etc.
		// ToDo: For now, it is hard-coded to read from a JSON file (by passing the path), but it should technically
		// be moved out to get input from a generic 'source' structure.

		cupcfd::CupCfd<cupcfd::geometry::mesh::CupCfdSoAMesh<int,float,int>,int,float,int> run(configPath, meshPtr);
	}

	// Deregister the Custom MPI Types
	particle.deregisterMPIType();
	point.deregisterMPIType();
	vector.deregisterMPIType();

	// Add ifdef for petsc?
	PetscFinalize();

	TreeTimerFinalize();
	//MPI_Finalize();

	if(comm.rank == 0) {
		std::cout << "cup-cfd has completed" << std::endl;
	}
}
