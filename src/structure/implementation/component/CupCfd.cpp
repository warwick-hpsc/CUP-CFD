/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * This file contains the definitions for the CupCfd class
 */

#include "CupCfd.h"
#include "CupCfdAoSMesh.h"
#include "CupCfdSoAMesh.h"

// JsonCPP - Supplied as standalone in include/io/jsoncpp
#include "json.h"
#include "json-forwards.h"

#include <fstream>

#include "BenchmarkKernels.h"
#include "BenchmarkConfigKernels.h"
#include "BenchmarkConfigKernelsJSON.h"

#include "BenchmarkExchange.h"
#include "BenchmarkConfigExchange.h"
#include "BenchmarkConfigExchangeJSON.h"

#include "BenchmarkLinearSolver.h"
#include "BenchmarkConfigLinearSolver.h"
#include "BenchmarkConfigLinearSolverJSON.h"

#include "BenchmarkParticleSystemSimple.h"
#include "BenchmarkConfigParticleSystemSimple.h"
#include "BenchmarkConfigParticleSystemSimpleJSON.h"

#include <iostream>
#include "Communicator.h"
#include "SparseMatrixCSR.h"

namespace cupcfd
{
	template <class M, class I, class T, class L>
	CupCfd<M,I,T,L>::CupCfd(std::string jsonFilePath, std::shared_ptr<M> meshPtr)
	: meshPtr(meshPtr)
	{
		cupcfd::error::eCodes status;

		// ToDo: The JSON processing should be moved to a 'source' class.
		// This will suffice for now however.

		// Setup an initial system state (e.g. data arrays etc).
		// For now, will use random values.

		cupcfd::comm::Communicator comm(MPI_COMM_WORLD);

		// === Search for benchmark configurations ===
		Json::Value configData;
		std::ifstream source(jsonFilePath, std::ifstream::binary);
		source >> configData;

		// Store all benchmarks under "Benchmarks" field name
		if(configData.isMember("Benchmarks")) {
			I size;
			size = cupcfd::utility::drivers::safeConvertSizeT<I>(configData["Benchmarks"].size());
			for(I i = 0; i < size; i++) {
				Json::Value benchmarkConfigData = configData["Benchmarks"][i];

				// === Finite Volume Kernel Benchmarks ===
				if(benchmarkConfigData.isMember("BenchmarkKernels")) {
					cupcfd::benchmark::BenchmarkConfigKernelsJSON<I,T> fvmBenchJSON(benchmarkConfigData["BenchmarkKernels"]);

					// Build Finite Volume Kernel Benchmark Config
					cupcfd::benchmark::BenchmarkConfigKernels<I, T> * fvmBenchConfig;
					status = fvmBenchJSON.buildBenchmarkConfig(&fvmBenchConfig);

					if(status != cupcfd::error::E_SUCCESS) {
						std::cout << "Cannot Parse a Kernel Benchmark Config at " << jsonFilePath << ". Skipping.\n";
					}
					else {
						// Build Finite Volume Kernel Benchmark
						if(comm.rank == 0) {
							std::cout << "Building Kernel Benchmark\n";
						}

						cupcfd::benchmark::BenchmarkKernels<M,I,T,L> * fvmBench;
						status = fvmBenchConfig->buildBenchmark(&fvmBench, meshPtr);

						if(status != cupcfd::error::E_SUCCESS) {
							std::cout << "Error Encountered: Failed to build Kernel Benchmark with current configuration. Please check the provided configuration is correct.\n";
						}
						else {
							// Run Benchmark
							status = fvmBench->runBenchmark();
							HARD_CHECK_ECODE(status);
							delete(fvmBench);
						}

						delete(fvmBenchConfig);
					}
				}

				// === Exchange Benchmarks ===
				if(benchmarkConfigData.isMember("BenchmarkExchange")) {
					cupcfd::benchmark::BenchmarkConfigExchangeJSON<I,T> exchangeBenchJSON(benchmarkConfigData["BenchmarkExchange"]);
					cupcfd::benchmark::BenchmarkConfigExchange<I,T> * exchangeBenchConfig;
					status = exchangeBenchJSON.buildBenchmarkConfig(&exchangeBenchConfig);

					if(status != cupcfd::error::E_SUCCESS) {
						std::cout << "Cannot Parse a Exchange Benchmark Config at " << jsonFilePath << ". Skipping.\n";
					}
					else {
						// Build Exchange Benchmark Based on Mesh Connectivity Graph
						if(comm.rank == 0) {
							std::cout << "Building Exchange Benchmark\n";
						}

						cupcfd::benchmark::BenchmarkExchange<I, T> * exchangeBench;
						status = exchangeBenchConfig->buildBenchmark(&exchangeBench, *(meshPtr->cellConnGraph));

						if(status != cupcfd::error::E_SUCCESS) {
							std::cout << "Error Encountered: Failed to build Exchange Benchmark with current configuration. Please check the provided configuration is correct.\n";
						}
						else {
							status = exchangeBench->runBenchmark();
							HARD_CHECK_ECODE(status)
						}
						delete(exchangeBench);
					}

					delete(exchangeBenchConfig);
				}

				// === Linear Solver Benchmarks ===
				// Hard-coded to Matrix CSR for now....
				// Could make it an option in a config? (Would need to know it before building the object however)
				if(benchmarkConfigData.isMember("BenchmarkLinearSolver")) {
					if(comm.rank == 0) {
						std::cout << "Building Linear Solver Benchmark\n";
					}

					cupcfd::benchmark::BenchmarkConfigLinearSolverJSON<cupcfd::data_structures::SparseMatrixCSR<I,T>,I,T> linearSolverBenchJSON(benchmarkConfigData["BenchmarkLinearSolver"]);
					cupcfd::benchmark::BenchmarkConfigLinearSolver<cupcfd::data_structures::SparseMatrixCSR<I,T>,I,T> * linearSolverBenchConfig;
					status = linearSolverBenchJSON.buildBenchmarkConfig(&linearSolverBenchConfig);

					if(status != cupcfd::error::E_SUCCESS) {
						std::cout << "Cannot Parse a Linear Solver Benchmark Config at " << jsonFilePath << ". Skipping.\n";
					}
					else {
						cupcfd::benchmark::BenchmarkLinearSolver<cupcfd::data_structures::SparseMatrixCSR<I,T>,I,T> * linearSolverBench;
						status = linearSolverBenchConfig->buildBenchmark(&linearSolverBench);

						if(status != cupcfd::error::E_SUCCESS) {
							std::cout << "Error Encountered: Failed to build Linear Solver Benchmark with current configuration. Please check the provided configuration is correct.\n";
						}
						else {
							status = linearSolverBench->runBenchmark();
							HARD_CHECK_ECODE(status)
							delete(linearSolverBench);
						}

						delete(linearSolverBenchConfig);
					}
				}

				// === Particle Benchmarks ===
				if(benchmarkConfigData.isMember("BenchmarkParticleSystem")) {
					if(comm.rank == 0) {
						std::cout << "Building Simple Particle Benchmark\n";
					}

					// Test for a Particle Simple System Benchmark
					cupcfd::benchmark::BenchmarkConfigParticleSystemSimpleJSON<M,I,T,L> particleSystemJSON(benchmarkConfigData["BenchmarkParticleSystem"]);

					// Build Config
					cupcfd::benchmark::BenchmarkConfigParticleSystemSimple<M,I,T,L> * particleSystemConfig;
					status = particleSystemJSON.buildBenchmarkConfig(&particleSystemConfig);

					if(status != cupcfd::error::E_SUCCESS) {
						std::cout << "Cannot Parse a Particle Benchmark Config at " << jsonFilePath << ". Skipping.\n";
					}
					else {
						cupcfd::benchmark::BenchmarkParticleSystemSimple<M,I,T,L> * benchmarkParticleSystem;
						status = particleSystemConfig->buildBenchmark(&benchmarkParticleSystem, meshPtr);

						if(status != cupcfd::error::E_SUCCESS) {
							std::cout << "Error Encountered: Failed to build Simple Particle Benchmark with current configuration. Please check the provided configuration is correct.\n";
						}
						else {
							status = benchmarkParticleSystem->runBenchmark();
							HARD_CHECK_ECODE(status)
							delete(benchmarkParticleSystem);
						}

						delete(particleSystemConfig);
					}
				}

			}
		}
	}

	template <class M, class I, class T, class L>
	CupCfd<M,I,T,L>::~CupCfd()
	{

	}
}

// Explicit Instantiation
template class cupcfd::CupCfd<cupcfd::geometry::mesh::CupCfdAoSMesh<int,float,int>, int, float, int>;
template class cupcfd::CupCfd<cupcfd::geometry::mesh::CupCfdAoSMesh<int,double,int>, int, double, int>;

template class cupcfd::CupCfd<cupcfd::geometry::mesh::CupCfdSoAMesh<int,float,int>, int, float, int>;
template class cupcfd::CupCfd<cupcfd::geometry::mesh::CupCfdSoAMesh<int,double,int>, int, double, int>;
