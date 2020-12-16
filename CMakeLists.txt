# ========================================================
# ==================== CMake =============================
# ========================================================

cmake_minimum_required (VERSION 2.6)
project (cupcfd)
#enable_testing()
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

# ====================================================================================================================================
# The following sections can be edited if desired for build configuration purposes.
# ====================================================================================================================================

# ====================================================================================================================================
# ====================================================== Build Config ================================================================
# ====================================================================================================================================

# ======================================
# ======= Set Program Version ==========
# ======================================

# Version Number
set (cupcfd_VERSION_MAJOR 1)
set (cupcfd_VERSION_MINOR 0)

# ========================================
# ==== Override Header Defined Values ====
# ========================================

# Pad/Increase the size of the CupCfdAoSMeshBoundary class in an AoS Mesh beyond that of the required values
# This is to measure the impact of the class size in an AoS setting.
# CUPCFD_AOS_MESH_BOUNDARY_PADDING

# Pad/Increase the size of the CupCfdAoSMeshCell class in an AoS Mesh beyond that of the required values
# This is to measure the impact of the class size in an AoS setting.
# CUPCFD_AOS_MESH_CELL_PADDING

# Pad/Increase the size of the CupCfdAoSMeshFace class in an AoS Mesh beyond that of the required values
# This is to measure the impact of the class size in an AoS setting.
# CUPCFD_AOS_MESH_FACE_PADDING

# Pad/Increase the size of the CupCfdAoSMeshRegion class in an AoS Mesh beyond that of the required values
# This is to measure the impact of the class size in an AoS setting.
# CUPCFD_AOS_MESH_REGION_PADDING

# Pad/Increase the size of the CupCfdAoSMeshVertex class in an AoS Mesh beyond that of the required values
# This is to measure the impact of the class size in an AoS setting.
# CUPCFD_AOS_MESH_VERTEX_PADDING

# ======================================
# ======== Set Compile Flags ===========
# ======================================

if ( "${CMAKE_BUILD_TYPE}" STREQUAL "Debug" )
  if("${COMPILER}" STREQUAL "intel")
    set(CMAKE_CXX_FLAGS "-DDEBUG -O0 -g -pg -mkl")
  elseif ("${COMPILER}" STREQUAL "clang")
    #set(CMAKE_CXX_FLAGS "-DDEBUG -O0 -g -pg -Wfatal-errors -Wunused -Wno-narrowing -Wno-undefined-var-template -Wno-delete-abstract-non-virtual-dtor -Wno-pointer-bool-conversion -Wno-dynamic-class-memaccess -Werror")
    #set(CMAKE_CXX_FLAGS "-DDEBUG -O0 -g -pg -Wfatal-errors -Wunused -Wextra -Wno-narrowing -Wno-undefined-var-template -Wno-delete-abstract-non-virtual-dtor -Wno-pointer-bool-conversion -Wno-dynamic-class-memaccess -Werror")
    set(CMAKE_CXX_FLAGS "-DDEBUG -O0 -g -pg -Wfatal-errors -Wunused-result -Werror")
  elseif ("${COMPILER}" STREQUAL "gnu")
    #set(CMAKE_CXX_FLAGS "-DDEBUG -O0 -g -pg -Wfatal-errors -Werror -Wno-narrowing")
    #set(CMAKE_CXX_FLAGS "-DDEBUG -O0 -g -pg -Wfatal-errors -Werror -Wno-narrowing -Wunused")
    set(CMAKE_CXX_FLAGS "-DDEBUG -O0 -g -pg -Wfatal-errors -Werror -Wno-narrowing -Wunused -Wextra")
  else()
  	message(FATAL_ERROR "Unrecognised value for COMPILER - '${COMPILER}'" )
  endif()
elseif ( "${CMAKE_BUILD_TYPE}" STREQUAL "Release" )
  if("${COMPILER}" STREQUAL "intel")
    set(CMAKE_CXX_FLAGS "-O2 -g -mkl")
  elseif ("${COMPILER}" STREQUAL "clang")
    set(CMAKE_CXX_FLAGS "-O2 -g -Wfatal-errors")
  elseif ("${COMPILER}" STREQUAL "gnu")
    set(CMAKE_CXX_FLAGS "-O2 -g -Wfatal-errors")
  else()
  	message(FATAL_ERROR "Unrecognised value for COMPILER - '${COMPILER}'" )
  endif()
else()
  message(FATAL_ERROR "Unrecognised value for CMAKE_BUILD_TYPE - '${CMAKE_BUILD_TYPE}'" )
endif()

# ====================================================================================================================================
# ====================================================== Options =====================================================================
# ====================================================================================================================================

# === File Input/Output Options ===
option(USE_HDF5 "Enable HDF5 input/output" ON)
# Needs HDF5_ROOT

# === Partitioning Options ===
option(USE_METIS "Enable METIS Partioning" ON)
# Needs METIS_ROOT

option(USE_PARMETIS "Enable ParMETIS Partitioning" ON)
# Needs PARMETIS_ROOT

option(USE_PTSCOTCH "Enable PTScotch Partitioning" OFF)
# Needs PTSCOTCH_ROOT

# === Linear Solver Options ===
option(USE_PETSC "Enable use of PETSc Linear Solver" ON)
# Needs PETSC_ROOT

# === Build Options (Testing etc) ===
# option (USE_UNIT_TESTS "Enable Unit Tests" ON)

# Needs an MPI run command for parallel tests
# ToDo - Enable Serial and Parallel Unit Tests separately
set(MPIRUN_EXEC srun)

# ====================================================================================================================================
# ================================================= External Libraries ===============================================================
# ====================================================================================================================================

cmake_policy(SET CMP0074 NEW)

# === File Input/Output Options ===
if(USE_HDF5)
	set(HDF5_USE_STATIC_LIBRARIES 1)
	set(HDF5_PREFER_PARALLEL 1)
	find_package(HDF5 REQUIRED)
endif(USE_HDF5)

# === Partitioning Options ===
if(USE_METIS)
	find_package(METIS REQUIRED)
endif(USE_METIS)

if(USE_PARMETIS)
	find_package(PARMETIS REQUIRED)
endif(USE_PARMETIS)

# === Linear Solver Options ===
if(USE_PETSC)
	find_package(PETSC REQUIRED)
endif(USE_PETSC)

# === Timer Library ===
# Currently default to always needing this
# Should set via CMake Line but can be set here if needed
#set(SQLITE_LIBS /path/to/sqlite3/lib/libsqlite3.so)
#set(TREETIMER_LIBS /path/to/warwick-hpsc/treetimer/libtt.so)
#set(TREETIMER_INCLUDE /path/to/warwick-hpscetreetimer/include/timing_library/interface)

# ====================================================================================================================================
# ============================================== Third-Party Libraries ===============================================================
# ====================================================================================================================================

# === File Input/Output Options ===
if(USE_HDF5)
	include_directories(${HDF5_INCLUDE_DIRS})
	#set(CORE_INCLUDE ${CORE_INCLUDE} ${HDF5_INCLUDE_DIRS})
endif(USE_HDF5)
	
# === Partitioning Options ===
if(USE_METIS)
	#include_directories(${METIS_INCLUDE_DIRS})
	set(CORE_INCLUDE ${CORE_INCLUDE} ${METIS_INCLUDE_DIRS})
endif(USE_METIS)

if(USE_PARMETIS)
	#include_directories(${PARMETIS_INCLUDE_DIRS})
	set(CORE_INCLUDE ${CORE_INCLUDE} ${PARMETIS_INCLUDE_DIRS})
endif(USE_PARMETIS)

# === Linear Solver Options ===
if(USE_PETSC)
	include_directories(${PETSC_INCLUDE_DIRS})
	#set(CORE_INCLUDE ${CORE_INCLUDE} ${PETSC_INCLUDE_DIRS})
endif(USE_PETSC)

# === Timer Library Include ===
set(CORE_INCLUDE ${CORE_INCLUDE} ${TREETIMER_INCLUDE})

# ====================================================================================================================================
# The following sections should not be edited if only configuring for build purposes.
# ====================================================================================================================================

# ====================================================================================================================================
# ================================================= Internal Includes ================================================================
# ====================================================================================================================================

set(CORE_INCLUDE ${CORE_INCLUDE} ${PROJECT_BINARY_DIR}/src)

# ===================================================
# ================== Utility ========================
# Files pertaining to common functionality across multiple categories
# E.g. array manipulation etc
# ===================================================

set(CORE_INCLUDE ${CORE_INCLUDE} include/utility/implementation/component)
set(CORE_INCLUDE ${CORE_INCLUDE} include/utility/interface/component)

# ===================================================
# =============== Error Handling ====================
# ===================================================
# Currently this is just a header of error codes
# ===================================================

set(CORE_INCLUDE ${CORE_INCLUDE} include/error)

# ===================================================
# =============== Configuration =====================
# ===================================================

#set(CORE_INCLUDE ${CORE_INCLUDE} include/config/cmake)
#set(CORE_INCLUDE ${CORE_INCLUDE} include/config/run)

# ===================================================
# ============== Network Communication ==============
# ===================================================
# Files that pertain to network communication during 
# computationE.g. MPI operations, interfaces
# =================================

set(CORE_INCLUDE ${CORE_INCLUDE} include/comm/implementation/component)
set(CORE_INCLUDE ${CORE_INCLUDE} include/comm/implementation/config)
set(CORE_INCLUDE ${CORE_INCLUDE} include/comm/implementation/source)
set(CORE_INCLUDE ${CORE_INCLUDE} include/comm/interface/component)
set(CORE_INCLUDE ${CORE_INCLUDE} include/comm/interface/config)
set(CORE_INCLUDE ${CORE_INCLUDE} include/comm/interface/source)

# ===================================================
# ================= Distributions ===================
# ===================================================
# Files pertaining to generating values from
# probabilistic distributions
# ===================================================

set(CORE_INCLUDE ${CORE_INCLUDE} include/distributions/implementation/component)
set(CORE_INCLUDE ${CORE_INCLUDE} include/distributions/implementation/config)
set(CORE_INCLUDE ${CORE_INCLUDE} include/distributions/implementation/source)
set(CORE_INCLUDE ${CORE_INCLUDE} include/distributions/interface/component)
set(CORE_INCLUDE ${CORE_INCLUDE} include/distributions/interface/config)
set(CORE_INCLUDE ${CORE_INCLUDE} include/distributions/interface/source)

# ===================================================
# ================ Data Structures ==================
# ===================================================
# Abstract Data Structures
# ===================================================

set(CORE_INCLUDE ${CORE_INCLUDE} include/data_structures/implementation/component)
set(CORE_INCLUDE ${CORE_INCLUDE} include/data_structures/implementation/config)
set(CORE_INCLUDE ${CORE_INCLUDE} include/data_structures/implementation/source)
set(CORE_INCLUDE ${CORE_INCLUDE} include/data_structures/interface/component)
set(CORE_INCLUDE ${CORE_INCLUDE} include/data_structures/interface/config)
set(CORE_INCLUDE ${CORE_INCLUDE} include/data_structures/interface/source)

# ===================================================
# ================ I/O (Interfaces) =================
# ===================================================
# Files that pertain to generic I/O operations
# I.e. manage I/O output but are generic in some way
# ===================================================

set(CORE_INCLUDE ${CORE_INCLUDE} include/io/implementation/component)

# ===================================================
# =================== Geometry ======================
# ===================================================
# Files that pertain to simulation geometry
# E.g. Mesh data structures, Euclidean space etc.
# ===================================================

# ======== Euclidean Base Geometry (Point, Vectors) ========
set(CORE_INCLUDE ${CORE_INCLUDE} include/geometry/euclidean/implementation/component)

# ======== Mesh Geometry ========

set(CORE_INCLUDE ${CORE_INCLUDE} include/geometry/mesh/implementation/component)
set(CORE_INCLUDE ${CORE_INCLUDE} include/geometry/mesh/implementation/config)
set(CORE_INCLUDE ${CORE_INCLUDE} include/geometry/mesh/implementation/source)

set(CORE_INCLUDE ${CORE_INCLUDE} include/geometry/mesh/interface/component)
set(CORE_INCLUDE ${CORE_INCLUDE} include/geometry/mesh/interface/config)
set(CORE_INCLUDE ${CORE_INCLUDE} include/geometry/mesh/interface/source)

# ======== Shapes (2D, 3D) ========

set(CORE_INCLUDE ${CORE_INCLUDE} include/geometry/shapes/implementation/component)
set(CORE_INCLUDE ${CORE_INCLUDE} include/geometry/shapes/implementation/config)
set(CORE_INCLUDE ${CORE_INCLUDE} include/geometry/shapes/implementation/source)

set(CORE_INCLUDE ${CORE_INCLUDE} include/geometry/shapes/interface/component)
set(CORE_INCLUDE ${CORE_INCLUDE} include/geometry/shapes/interface/config)
set(CORE_INCLUDE ${CORE_INCLUDE} include/geometry/shapes/interface/source)

# ===================================================
# =================== Data ==========================
# ===================================================
# Files that pertain to accessing data from 
# non-internal sources (e.g. file I/O, command line 
# rather than data structures)
# ===================================================

# ===================================================
# ============= Finite Volume Method ================
# ===================================================
# Files that pertain to finite volume computation
# ===================================================

#set(CORE_INCLUDE ${CORE_INCLUDE} include/fvm/gradient)
#set(CORE_INCLUDE ${CORE_INCLUDE} include/fvm/mass)
#set(CORE_INCLUDE ${CORE_INCLUDE} include/fvm/scalar)
#set(CORE_INCLUDE ${CORE_INCLUDE} include/fvm/uvw)
#set(CORE_INCLUDE ${CORE_INCLUDE} include/fvm/viscosity)

set(CORE_INCLUDE ${CORE_INCLUDE} include/fvm/implementation/component)

# ===================================================
# ================= Partitioners ====================
# ===================================================
# Files that pertain to graph partitioning
# ===================================================

set(CORE_INCLUDE ${CORE_INCLUDE} include/partitioner/implementation/component)
set(CORE_INCLUDE ${CORE_INCLUDE} include/partitioner/implementation/config)
set(CORE_INCLUDE ${CORE_INCLUDE} include/partitioner/implementation/source)
set(CORE_INCLUDE ${CORE_INCLUDE} include/partitioner/interface/component)
set(CORE_INCLUDE ${CORE_INCLUDE} include/partitioner/interface/config)
set(CORE_INCLUDE ${CORE_INCLUDE} include/partitioner/interface/source)

# ===================================================
# ================ Linear Solvers ===================
# ===================================================
# Files that pertain to the use of linear solvers
# ===================================================

set(CORE_INCLUDE ${CORE_INCLUDE} include/linearsolvers/implementation/component)
set(CORE_INCLUDE ${CORE_INCLUDE} include/linearsolvers/implementation/config)
set(CORE_INCLUDE ${CORE_INCLUDE} include/linearsolvers/implementation/source)
set(CORE_INCLUDE ${CORE_INCLUDE} include/linearsolvers/interface/component)
set(CORE_INCLUDE ${CORE_INCLUDE} include/linearsolvers/interface/config)
set(CORE_INCLUDE ${CORE_INCLUDE} include/linearsolvers/interface/source)

# ===================================================
# =============== Particle Systems ==================
# ===================================================
# Files that pertain to particle systems
# ===================================================

set(CORE_INCLUDE ${CORE_INCLUDE} include/particles/implementation/component)
set(CORE_INCLUDE ${CORE_INCLUDE} include/particles/implementation/config)
set(CORE_INCLUDE ${CORE_INCLUDE} include/particles/implementation/source)
set(CORE_INCLUDE ${CORE_INCLUDE} include/particles/interface/component)
set(CORE_INCLUDE ${CORE_INCLUDE} include/particles/interface/config)
set(CORE_INCLUDE ${CORE_INCLUDE} include/particles/interface/source)

# ===================================================
# ================= Benchmarks ======================
# ===================================================
# Files pertaining to running benchmarks
# ===================================================

set(CORE_INCLUDE ${CORE_INCLUDE} include/benchmarks/interface/component)
set(CORE_INCLUDE ${CORE_INCLUDE} include/benchmarks/interface/config)
set(CORE_INCLUDE ${CORE_INCLUDE} include/benchmarks/interface/source)

set(CORE_INCLUDE ${CORE_INCLUDE} include/benchmarks/implementation/component)
set(CORE_INCLUDE ${CORE_INCLUDE} include/benchmarks/implementation/config)
set(CORE_INCLUDE ${CORE_INCLUDE} include/benchmarks/implementation/source)

# ===================================================
# ================== Structure ======================
# ===================================================

set(CORE_INCLUDE ${CORE_INCLUDE} include/structure/interface/component)
set(CORE_INCLUDE ${CORE_INCLUDE} include/structure/interface/config)
set(CORE_INCLUDE ${CORE_INCLUDE} include/structure/interface/source)

set(CORE_INCLUDE ${CORE_INCLUDE} include/structure/implementation/component)
set(CORE_INCLUDE ${CORE_INCLUDE} include/structure/implementation/config)
set(CORE_INCLUDE ${CORE_INCLUDE} include/structure/implementation/source)

# Include selected headers
include_directories(${CORE_INCLUDE})

# ====================================================================================================================================
# ================================================= Internal Source Files ============================================================
# ====================================================================================================================================

# ===================================================
# ================== Utility ========================
# ===================================================
# All functionality currently in header/tpp files
set(utility_source_files
)

# ===================================================
# =============== Configuration =====================
# ===================================================
set(config_source_files
#	src/config/cmake/BuildConfig.cpp
)

# ===================================================
# ============== Network Communication ==============
# ===================================================
set(comm_source_files
	src/comm/interface/component/CustomMPIType.cpp
	src/comm/implementation/component/MPIUtility.cpp
	src/comm/interface/component/ExchangePattern.cpp
	src/comm/implementation/component/ExchangePatternOneSidedNonBlocking.cpp
	src/comm/implementation/component/ExchangePatternTwoSidedNonBlocking.cpp	
	src/comm/implementation/config/ExchangePatternConfig.cpp
	src/comm/interface/source/ExchangePatternConfigSource.cpp
	src/comm/implementation/source/ExchangePatternConfigSourceJSON.cpp
	src/comm/interface/component/Communicator.cpp
)

# ===================================================
# ================= Distributions ===================
# ===================================================

set(distributions_source_files
	src/distributions/interface/component/Distribution.cpp
	src/distributions/implementation/component/DistributionUniform.cpp
	src/distributions/implementation/component/DistributionNormal.cpp
	src/distributions/implementation/component/DistributionFixed.cpp
)

# ===================================================
# ================ Data Structures ==================
# ===================================================
set(data_structures_source_files
	src/data_structures/interface/component/AdjacencyList.cpp
	src/data_structures/interface/component/SparseMatrix.cpp
	src/data_structures/interface/config/SparseMatrixSourceConfig.cpp
	src/data_structures/interface/source/SparseMatrixSource.cpp
	src/data_structures/interface/source/SparseMatrixSourceConfigSource.cpp
	src/data_structures/interface/source/SparseMatrixSourceFileConfigSource.cpp
	src/data_structures/interface/source/SparseMatrixSourceMeshGenConfigSource.cpp
	src/data_structures/interface/config/VectorSourceConfig.cpp
	src/data_structures/interface/source/VectorSource.cpp
	src/data_structures/interface/source/VectorSourceConfigSource.cpp
	src/data_structures/implementation/component/AdjacencyListCSR.cpp
	src/data_structures/implementation/component/AdjacencyListVector.cpp
	src/data_structures/implementation/component/DistributedAdjacencyList.cpp
	src/data_structures/implementation/component/SparseMatrixCOO.cpp
	src/data_structures/implementation/component/SparseMatrixCSR.cpp
	src/data_structures/implementation/config/SparseMatrixSourceFileConfig.cpp
	src/data_structures/implementation/config/SparseMatrixSourceMeshGenConfig.cpp
	src/data_structures/implementation/source/SparseMatrixSourceHDF5.cpp
	src/data_structures/implementation/source/SparseMatrixSourceMeshGen.cpp
	src/data_structures/implementation/source/SparseMatrixSourceFileConfigJSON.cpp
	src/data_structures/implementation/source/SparseMatrixSourceMeshGenConfigJSON.cpp
	src/data_structures/implementation/config/VectorSourceFileConfig.cpp
	src/data_structures/implementation/source/VectorSourceHDF5.cpp
	src/data_structures/implementation/source/VectorSourceFileConfigJSON.cpp
)

# ===================================================
# ================ I/O (Interfaces) =================
# ===================================================

set(io_source_files 
	src/io/implementation/component/CommandLine.cpp
	src/io/implementation/component/jsoncpp.cpp
	)
		
if(USE_HDF5)
	set(io_source_files 
		${io_source_files}
		src/io/implementation/component/HDF5Access.cpp
		src/io/implementation/component/HDF5Properties.cpp
		src/io/implementation/component/HDF5Record.cpp
	)
endif(USE_HDF5)

# ===================================================
# =================== Geometry ======================
# ===================================================
set(geometry_source_files
	src/geometry/euclidean/implementation/component/EuclideanPoint.cpp
	src/geometry/euclidean/implementation/component/EuclideanVector.cpp
	src/geometry/euclidean/implementation/component/EuclideanPlane3D.cpp
	src/geometry/mesh/interface/component/UnstructuredMeshProperties.cpp
	src/geometry/mesh/interface/component/UnstructuredMeshInterface.cpp
	src/geometry/mesh/interface/config/MeshSourceConfig.cpp
	src/geometry/mesh/interface/source/MeshSource.cpp
	src/geometry/mesh/interface/source/MeshSourceConfigSource.cpp
	src/geometry/mesh/implementation/component/CupCfdAoSMeshBoundary.cpp
	src/geometry/mesh/implementation/component/CupCfdAoSMeshCell.cpp
	src/geometry/mesh/implementation/component/CupCfdAoSMeshFace.cpp
	src/geometry/mesh/implementation/component/CupCfdAoSMeshRegion.cpp
	src/geometry/mesh/implementation/component/CupCfdAoSMeshVertex.cpp
	src/geometry/mesh/implementation/component/CupCfdAoSMesh.cpp
	src/geometry/mesh/implementation/component/CupCfdSoAMesh.cpp
	src/geometry/mesh/implementation/config/MeshSourceFileConfig.cpp
	src/geometry/mesh/implementation/config/MeshSourceStructGenConfig.cpp
	src/geometry/mesh/implementation/config/MeshConfig.cpp
	src/geometry/mesh/implementation/source/MeshHDF5Source.cpp
	src/geometry/mesh/implementation/source/MeshStructGenSource.cpp
	src/geometry/mesh/implementation/source/MeshSourceFileConfigJSON.cpp
	src/geometry/mesh/implementation/source/MeshSourceStructGenConfigJSON.cpp
	src/geometry/mesh/implementation/source/MeshConfigSourceJSON.cpp
	src/geometry/shapes/interface/component/Polygon2D.cpp
	src/geometry/shapes/interface/component/Polygon3D.cpp
	src/geometry/shapes/implementation/component/Triangle.cpp
	src/geometry/shapes/implementation/component/Triangle2D.cpp
	src/geometry/shapes/implementation/component/Triangle3D.cpp	
	src/geometry/shapes/implementation/component/Quadrilateral3D.cpp	
	src/geometry/shapes/interface/component/Polyhedron.cpp
	src/geometry/shapes/implementation/component/Pyramid.cpp
	src/geometry/shapes/implementation/component/Tetrahedron.cpp
	src/geometry/shapes/implementation/component/QuadPyramid.cpp
	src/geometry/shapes/implementation/component/Hexahedron.cpp
	src/geometry/shapes/implementation/component/TriPrism.cpp
)

# ===================================================
# ============= Finite Volume Method ================
# ===================================================
set(fvm_source_files
	# Kernel definitions are currently all in .ipp header definition files
)

# ===================================================
# ================= Partitioners ====================
# ===================================================
set(partitioner_source_files
	src/partitioner/interface/component/PartitionerInterface.cpp
	src/partitioner/interface/config/PartitionerConfig.cpp
	src/partitioner/interface/source/PartitionerConfigSource.cpp
	src/partitioner/interface/source/PartitionerNaiveConfigSource.cpp
	src/partitioner/implementation/component/PartitionerNaive.cpp
	src/partitioner/implementation/config/PartitionerNaiveConfig.cpp
	src/partitioner/implementation/source/PartitionerNaiveConfigSourceJSON.cpp
)

if(USE_PARMETIS)
	set(partitioner_source_files
			${partitioner_source_files}
			src/partitioner/interface/source/PartitionerParmetisConfigSource.cpp
			src/partitioner/interface/source/PartitionerMetisConfigSource.cpp
			src/partitioner/implementation/component/PartitionerMetis.cpp
			src/partitioner/implementation/component/PartitionerParmetis.cpp
			src/partitioner/implementation/config/PartitionerMetisConfig.cpp
			src/partitioner/implementation/config/PartitionerParmetisConfig.cpp
			src/partitioner/implementation/source/PartitionerParmetisConfigSourceJSON.cpp
			src/partitioner/implementation/source/PartitionerMetisConfigSourceJSON.cpp
		)
elseif(USE_METIS)
	set(partitioner_source_files
		${partitioner_source_files}
		src/partitioner/interface/source/PartitionerMetisConfigSource.cpp
		src/partitioner/implementation/component/PartitionerMetis.cpp
		src/partitioner/implementation/config/PartitionerMetisConfig.cpp
		src/partitioner/implementation/source/PartitionerMetisConfigSourceJSON.cpp
	)
endif(USE_PARMETIS)


# ===================================================
# ================ Linear Solvers ===================
# ===================================================
set(linearsolver_source_files
	src/linearsolvers/interface/component/LinearSolverInterface.cpp
	src/linearsolvers/interface/config/LinearSolverConfig.cpp
	src/linearsolvers/interface/source/LinearSolverConfigSource.cpp
)

if(USE_PETSC)
	set(linearsolver_source_files
		${linearsolver_source_files}
		src/linearsolvers/implementation/component/LinearSolverPETScAlgorithm.cpp
		src/linearsolvers/implementation/component/LinearSolverPETSc.cpp
		src/linearsolvers/implementation/config/LinearSolverConfigPETSc.cpp
		src/linearsolvers/implementation/source/LinearSolverConfigPETScJSON.cpp
	)
endif(USE_PETSC)

# ===================================================
# =============== Particle Systems ==================
# ===================================================
set(particle_source_files
	src/particles/interface/component/Particle.cpp
	src/particles/interface/component/ParticleEmitter.cpp
	src/particles/interface/config/ParticleEmitterConfig.cpp
	src/particles/interface/config/ParticleSystemConfig.cpp
	src/particles/interface/config/ParticleSourceConfig.cpp
	src/particles/interface/source/ParticleEmitterConfigSource.cpp
	src/particles/interface/source/ParticleSystemConfigSource.cpp
	src/particles/interface/source/ParticleSource.cpp
	src/particles/interface/source/ParticleSourceConfigSource.cpp
	src/particles/implementation/component/ParticleSimple.cpp
	src/particles/implementation/component/ParticleEmitterSimple.cpp
	src/particles/implementation/component/ParticleSystemSimple.cpp
	src/particles/implementation/config/ParticleEmitterSimpleConfig.cpp
	src/particles/implementation/config/ParticleSystemSimpleConfig.cpp
	src/particles/implementation/config/ParticleSimpleSourceFileConfig.cpp
	src/particles/implementation/source/ParticleEmitterSimpleConfigJSON.cpp
	src/particles/implementation/source/ParticleSystemSimpleConfigJSON.cpp
	src/particles/implementation/source/ParticleSimpleSourceHDF5.cpp
	src/particles/implementation/source/ParticleSimpleSourceFileConfigJSON.cpp
)

# ===================================================
# ================= Benchmarks ======================
# ===================================================
set(bench_source_files
	src/benchmarks/interface/component/Benchmark.cpp
	src/benchmarks/interface/config/BenchmarkConfig.cpp
	src/benchmarks/implementation/component/BenchmarkKernels.cpp
	src/benchmarks/implementation/component/BenchmarkExchange.cpp
	src/benchmarks/implementation/component/BenchmarkLinearSolver.cpp
	src/benchmarks/implementation/component/BenchmarkParticleSystemSimple.cpp
	src/benchmarks/implementation/config/BenchmarkConfigKernels.cpp
	src/benchmarks/implementation/config/BenchmarkConfigExchange.cpp
	src/benchmarks/implementation/config/BenchmarkConfigLinearSolver.cpp
	src/benchmarks/implementation/config/BenchmarkConfigParticleSystemSimple.cpp
	src/benchmarks/implementation/source/BenchmarkConfigKernelsJSON.cpp
	src/benchmarks/implementation/source/BenchmarkConfigExchangeJSON.cpp
	src/benchmarks/implementation/source/BenchmarkConfigLinearSolverJSON.cpp
	src/benchmarks/implementation/source/BenchmarkConfigParticleSystemSimpleJSON.cpp
)

# ===================================================
# ================== Structure ======================
# ===================================================
set(structure_source_files
	src/structure/implementation/source/SystemConfigJSON.cpp
	src/structure/implementation/component/CupCfd.cpp
)

# ===================================================
# ================== Error Checking =================
# ===================================================
set(error_source_files
	src/error/Error.cpp
)

# ===================================================
# ================= Set Libraries ===================
# ===================================================

add_library(cupcfd_lib ${bench_source_files}
					   ${comm_source_files}
					   ${distributions_source_files}
					   ${config_source_files}
					   ${data_structures_source_files}
					   ${fvm_source_files}
					   ${geometry_source_files}
					   ${io_source_files}
					   ${linearsolver_source_files}
					   ${particle_source_files}
					   ${partitioner_source_files}
					   ${structure_source_files}
					   ${error_source_files}
					   ${utility_source_files}
			)

if(USE_HDF5)
	set(CORE_LIBS ${HDF5_C_LIBRARIES} z ${CORE_LIBS})
else(true)
	set(CORE_LIBS ${CORE_LIBS})
endif(USE_HDF5)

if(USE_PARMETIS)
	# Parmetis also includes metis
	set(CORE_LIBS ${PARMETIS_LIBRARIES} ${METIS_LIBRARIES} ${CORE_LIBS})
elseif(USE_METIS)
	# Metis only
	set(CORE_LIBS ${METIS_LIBRARIES} ${CORE_LIBS})
else(true)
	# Neither Parmetis nor Metis
	set(CORE_LIBS ${CORE_LIBS})
endif(USE_PARMETIS)

if(USE_PETSC)
	#ToDo: We only want X11 if it is linked against PETSc.....
	set(CORE_LIBS -lX11 ${PETSC_LIBRARIES} ${CORE_LIBS})
else(true)
	set(CORE_LIBS ${CORE_LIBS})
endif(USE_PETSC)

# ===================================================
# ============== Timer Libraries ====================
# ===================================================
# Timer Libraries
set(ALL_LIBS ${CORE_LIBS} ${SQLITE_LIBS})
if(COMPILER STREQUAL "intel")
  set(ALL_LIBS ${CORE_LIBS} ${TREETIMER_LIBS} ifcore)
else()
  set(ALL_LIBS ${CORE_LIBS} ${TREETIMER_LIBS})
endif()

set(ALL_LIBS cupcfd_lib ${ALL_LIBS})

# ========================================================
# ========= Construct Build Settings Header ==============
# ========================================================

# Header File to pass CMake settings to source code
configure_file (
    "${PROJECT_SOURCE_DIR}/src/cmake_settings.h.in"
    "${PROJECT_BINARY_DIR}/src/cmake_settings.h"
)

# ========================================================
# ================= Main Executable =========================
# ========================================================

add_executable(cupcfd src/structure/main.cpp)

# ========================================================
# ================== Link Stage ==========================
# ========================================================
target_link_libraries (cupcfd ${ALL_LIBS})

# ====================================================================================================================================
# ================================================= Unit Tests =======================================================================
# ====================================================================================================================================

function(addCupCfdTest testName testSrcFile)
	# ========= Set Executable Name  ===========
	add_executable(${testName} ${testSrcFile})

	# === Includes ===
	target_include_directories(${testName} PRIVATE ${Boost_INCLUDE_DIRS})	# Boost is always added
	target_include_directories(${testName} PRIVATE ${CORE_INCLUDE})

	# === Libraries ===
	target_link_libraries(${testName} ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})
	target_link_libraries(${testName} ${ALL_LIBS})
	target_compile_definitions(${testName} PRIVATE "BOOST_TEST_DYN_LINK=1")		# Dynamic Library

	# === Add Test to CMake ===
	add_test(NAME ${testName} COMMAND ${testName})
endfunction(addCupCfdTest)


function(addCupCfdMPITest testName testSrcFile nRanks)
	# ========= Set Executable Name  ===========
	add_executable(${testName} ${testSrcFile})

	# === Includes ===
	target_include_directories(${testName} PRIVATE ${Boost_INCLUDE_DIRS})	# Boost is always added
	target_include_directories(${testName} PRIVATE ${CORE_INCLUDE})

	# === Libraries ===
	target_link_libraries(${testName} ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})
	target_link_libraries(${testName} ${ALL_LIBS})
	target_compile_definitions(${testName} PRIVATE "BOOST_TEST_DYN_LINK=1")		# Dynamic Library

	# === Add Test to CMake ===
	add_test(NAME ${testName} COMMAND ${MPIRUN_EXEC} -n ${nRanks} ${testName})
endfunction(addCupCfdMPITest)

if (USE_UNIT_TESTS)
	# Boost Library is needed for unit tests
	# Not required for full build, but will have to turn unit testing off
	find_package(Boost REQUIRED COMPONENTS unit_test_framework)

	# =====================================================================================================================
	# ================================================ Utility ============================================================
	# =====================================================================================================================

	# ===========================
	# ===== Implementations =====
	# ===========================
	
	# === Components ===	
	addCupCfdTest(utility_arithmetic_kernels_tests tests/utility/implementation/component/ArithmeticKernelTests.cpp)
	addCupCfdTest(utility_array_kernels_tests tests/utility/implementation/component/ArrayKernelTests.cpp)
	addCupCfdTest(utility_sort_kernels_tests tests/utility/implementation/component/SortKernelTests.cpp)
	addCupCfdTest(utility_search_kernels_tests tests/utility/implementation/component/SearchKernelTests.cpp)
	addCupCfdTest(utility_statistics_kernels_tests tests/utility/implementation/component/StatisticsKernelTests.cpp)
	
	# ======================
	# ===== Interfaces =====
	# ======================

	# === Components ===	
	addCupCfdTest(utility_array_drivers_tests tests/utility/interface/component/ArrayDriverTests.cpp)
	addCupCfdTest(utility_sort_drivers_tests tests/utility/interface/component/SortDriverTests.cpp)
	addCupCfdTest(utility_search_drivers_tests tests/utility/interface/component/SearchDriverTests.cpp)
	addCupCfdTest(utility_statistics_drivers_tests tests/utility/interface/component/StatisticsDriverTests.cpp)


	# =====================================================================================================================
	# ======================================= Network Communication =======================================================
	# =====================================================================================================================

	# ===========================
	# ===== Implementations =====
	# ===========================
	
	# === Config ===	
	addCupCfdTest(comm_exchangepattern_config_tests tests/comm/implementation/config/ExchangePatternConfigTests.cpp)	
		
	# === Source ===
	#addCupCfdTest(comm_exchangepattern_config_source_json_tests tests/comm/implementation/source/ExchangePatternConfigSourceJSONTests.cpp)
		
	# === Components ===	
	addCupCfdMPITest(comm_class_custommpitype_tests tests/comm/implementation/component/CustomMPITypeTests.cpp 4)
	addCupCfdMPITest(comm_mpi_utility_tests tests/comm/implementation/component/MPIUtilityTests.cpp 4)
	addCupCfdMPITest(comm_alltoallmpi_tests tests/comm/implementation/component/AllToAllMPITests.cpp 4)
	addCupCfdMPITest(comm_mpi_barrier_tests tests/comm/implementation/component/BarrierMPITests.cpp 4)
	addCupCfdMPITest(comm_mpi_broadcast_tests tests/comm/implementation/component/BroadcastMPITests.cpp 4)
	addCupCfdMPITest(comm_mpi_exchange_tests tests/comm/implementation/component/ExchangeMPITests.cpp 12)
	addCupCfdMPITest(comm_mpi_gather_tests tests/comm/implementation/component/GatherMPITests.cpp 4)
	addCupCfdMPITest(comm_mpi_reduce_tests tests/comm/implementation/component/ReduceMPITests.cpp 4)
	addCupCfdMPITest(comm_mpi_scatter_tests tests/comm/implementation/component/ScatterMPITests.cpp 4)
	addCupCfdMPITest(comm_mpi_waitall_tests tests/comm/implementation/component/WaitallMPITests.cpp 4)
	addCupCfdMPITest(comm_exchangepattern_nonblocking_onesided_tests tests/comm/implementation/component/ExchangePatternOneSidedNonBlockingTests.cpp 4)
	addCupCfdMPITest(comm_exchangepattern_nonblocking_twosided_tests tests/comm/implementation/component/ExchangePatternTwoSidedNonBlockingTests.cpp 4)
	
	# ======================
	# ===== Interfaces =====
	# ======================

	# === Config ===	
	
	# === Source ===
	addCupCfdTest(comm_exchangepattern_config_source_tests tests/comm/interface/source/ExchangePatternConfigSourceTests.cpp)
	
	# === Components ===	
	addCupCfdMPITest(comm_class_communicator_tests tests/comm/interface/component/CommunicatorTests.cpp 4)
	addCupCfdMPITest(comm_interface_alltoall_tests tests/comm/interface/component/AllToAllTests.cpp 4)
	addCupCfdMPITest(comm_interface_barrier_tests tests/comm/interface/component/BarrierTests.cpp 4)
	addCupCfdMPITest(comm_interface_broadcast_tests tests/comm/interface/component/BroadcastTests.cpp 4)
	addCupCfdMPITest(comm_interface_gather_tests tests/comm/interface/component/GatherTests.cpp 4)
	addCupCfdMPITest(comm_interface_reduce_tests tests/comm/interface/component/ReduceTests.cpp 4)
	addCupCfdMPITest(comm_interface_scatter_tests tests/comm/interface/component/ScatterTests.cpp 4)
	addCupCfdMPITest(comm_exchangepattern_tests tests/comm/interface/component/ExchangePatternTests.cpp 4)

	# === Classes ===
	#addCupCfdMPITest(comm_class_communicators_tests tests/comm/classes/CommunicatorsTests.cpp 4)
	
	# =====================================================================================================================
	# ================= Distributions ===================
	# =====================================================================================================================
	
	# ===========================
	# ===== Implementations =====
	# ===========================
	
	# === Components ===	
	addCupCfdTest(distributions_distribution_uniform_tests tests/distributions/implementation/component/DistributionUniformTests.cpp)
	addCupCfdTest(distributions_distribution_normal_tests tests/distributions/implementation/component/DistributionNormalTests.cpp)
	addCupCfdTest(distributions_distribution_fixed_tests tests/distributions/implementation/component/DistributionFixedTests.cpp)
	
	# === Config ===	
	addCupCfdTest(distributions_distribution_config_normal_tests tests/distributions/implementation/config/DistributionConfigNormalTests.cpp)
	addCupCfdTest(distributions_distribution_config_uniform_tests tests/distributions/implementation/config/DistributionConfigUniformTests.cpp)
	addCupCfdTest(distributions_distribution_config_fixed_tests tests/distributions/implementation/config/DistributionConfigFixedTests.cpp)
		
	# === Source ===
	addCupCfdTest(distributions_distribution_config_source_fixed_json_tests tests/distributions/implementation/source/DistributionConfigSourceFixedJSONTests.cpp)
	addCupCfdTest(distributions_distribution_config_source_normal_json_tests tests/distributions/implementation/source/DistributionConfigSourceNormalJSONTests.cpp)
	addCupCfdTest(distributions_distribution_config_source_uniform_json_tests tests/distributions/implementation/source/DistributionConfigSourceUniformJSONTests.cpp)

	# ===========================
	# ===== Interface =====
	# ===========================
	
	# === Components ===	
	addCupCfdTest(distributions_distribution_tests tests/distributions/interface/component/DistributionTests.cpp)

	# === Config ===	
	addCupCfdTest(distributions_distribution_config_tests tests/distributions/interface/config/DistributionConfigTests.cpp)
	
	# === Source ===


	# =====================================================================================================================
	# ================ Data Structures ==================
	# =====================================================================================================================
	
	# ===========================
	# ===== Implementations =====
	# ===========================
	
	# === Components ===	
	addCupCfdTest(data_structures_adjacency_list_csr_tests tests/data_structures/implementation/component/AdjacencyListCSRTests.cpp)
	addCupCfdTest(data_structures_adjacency_list_vector_tests tests/data_structures/implementation/component/AdjacencyListVectorTests.cpp)	
	addCupCfdMPITest(data_structures_distributed_adjacency_list_tests tests/data_structures/implementation/component/DistributedAdjacencyListTests.cpp 4)
	addCupCfdTest(data_structures_sparse_matrix_coo_tests tests/data_structures/implementation/component/SparseMatrixCOOTests.cpp) 
	addCupCfdTest(data_structures_sparse_matrix_csr_tests tests/data_structures/implementation/component/SparseMatrixCSRTests.cpp)
	
	# === Config ===	
	addCupCfdTest(data_structures_sparse_matrix_source_file_config_tests tests/data_structures/implementation/config/SparseMatrixSourceFileConfigTests.cpp)

	# === Source ===
	addCupCfdTest(data_structures_sparse_matrix_source_hdf5_tests tests/data_structures/implementation/source/SparseMatrixSourceHDF5Tests.cpp)
	
	# ===========================
	# ===== Interface =====
	# ===========================
	
	# === Components ===	
	addCupCfdTest(data_structures_adjacency_list_tests tests/data_structures/interface/component/AdjacencyListTests.cpp)
	addCupCfdTest(data_structures_sparse_matrix_abstract_tests tests/data_structures/interface/component/SparseMatrixTests.cpp)
	
	# === Config ===	
	addCupCfdTest(data_structures_sparse_matrix_source_config_tests tests/data_structures/interface/config/SparseMatrixSourceConfigTests.cpp)

	# === Source ===
	addCupCfdTest(data_structures_sparse_matrix_source_tests tests/data_structures/interface/source/SparseMatrixSourceTests.cpp)
	
	
	# === Sparse Matrices ===
	
	# Configs
	#addCupCfdTest(data_structures_sparse_matrix_config_tests tests/data_structures/SparseMatrix/config/SparseMatrixConfigTests.cpp)
	#addCupCfdTest(data_structures_sparse_matrix_config_from_file_tests tests/data_structures/SparseMatrix/config/SparseMatrixConfigFromFileTests.cpp)
	#addCupCfdTest(data_structures_sparse_matrix_config_gen_tests tests/data_structures/SparseMatrix/config/SparseMatrixConfigGenTests.cpp)
	#addCupCfdTest(data_structures_sparse_matrix_config_builder tests/data_structures/SparseMatrix/config/SparseMatrixConfigBuilderTests.cpp)
	
	# Sources
	#addCupCfdTest(data_structures_sparse_matrix_source_config_tests tests/data_structures/SparseMatrix/source/SparseMatrixConfigSourceTests.cpp)
	#addCupCfdTest(data_structures_sparse_matrix_source_config_json_tests tests/data_structures/SparseMatrix/source/SparseMatrixConfigSourceJSONTests.cpp)
	
	# === AdjacencyLists ===
	
	# Classes

	
	# =====================================================================================================================
	# ============================================= I/O (Interfaces) ======================================================
	# =====================================================================================================================
	
	# ===========================
	# ===== Interfaces =====
	# ===========================
	
	# === Components ===	
	
	# === Sources ===
		
	# === Configs ===
	
	
	# ===========================
	# ===== Implementations =====
	# ===========================
	
	# === Components ===	
	addCupCfdTest(io_cmdline_interface_tests tests/io/implementation/component/CommandLineTests.cpp)
	
	if(USE_HDF5)
		addCupCfdTest(io_hdf5_interface_tests tests/io/implementation/component/HDF5InterfaceTests.cpp)
	endif(USE_HDF5)
	
	# === Sources ===
		
	# === Configs ===


	# =====================================================================================================================
	# ================================================= Geometry ==========================================================
	# =====================================================================================================================

	# ========================================================
	# ====================== Euclidean =======================
	# ========================================================
		
	# ======================
	# ===== Interfaces =====
	# ======================
	
	# === Components ===

	# === Sources ===

	# === Configs ===

	# ===========================
	# ===== Implementations =====
	# ===========================
	
	# === Components ===	
	addCupCfdMPITest(geometry_euclidean_vector_tests tests/geometry/euclidean/implementation/component/EuclideanVectorTests.cpp 4)
	addCupCfdMPITest(geometry_euclidean_point_tests tests/geometry/euclidean/implementation/component/EuclideanPointTests.cpp 4)
	addCupCfdTest(geometry_euclidean_plane3d_tests tests/geometry/euclidean/implementation/component/EuclideanPlane3DTests.cpp)

	# === Sources ===
	
	# === Configs ===

	# ===================================================
	# ====================== Mesh =======================
	# ===================================================

	# ======================
	# ===== Interfaces =====
	# ======================
	
	# === Components ===
	addCupCfdTest(geometry_mesh_unstructured_mesh_properties_tests tests/geometry/mesh/interface/component/UnstructuredMeshPropertiesTests.cpp)
	addCupCfdMPITest(geometry_mesh_unstructured_mesh_tests tests/geometry/mesh/interface/component/UnstructuredMeshInterfaceTests.cpp 4)
	
	# === Sources ===
	addCupCfdTest(geometry_mesh_source_tests tests/geometry/mesh/interface/source/MeshSourceTests.cpp)
		
	# === Configs ===
	addCupCfdTest(geometry_mesh_source_config_tests tests/geometry/mesh/interface/config/MeshSourceConfigTests.cpp)
		
	# ===========================
	# ===== Implementations =====
	# ===========================
	
	# === Components ===		
	addCupCfdTest(geometry_mesh_aos_mesh_boundary_tests tests/geometry/mesh/implementation/component/CupCfdAoSMeshBoundaryTests.cpp)
	addCupCfdTest(geometry_mesh_aos_mesh_cell_tests tests/geometry/mesh/implementation/component/CupCfdAoSMeshCellTests.cpp)
	addCupCfdTest(geometry_mesh_aos_mesh_face_tests tests/geometry/mesh/implementation/component/CupCfdAoSMeshFaceTests.cpp)
	addCupCfdTest(geometry_mesh_aos_mesh_region_tests tests/geometry/mesh/implementation/component/CupCfdAoSMeshRegionTests.cpp)
	addCupCfdTest(geometry_mesh_aos_mesh_vertex_tests tests/geometry/mesh/implementation/component/CupCfdAoSMeshVertexTests.cpp)
	addCupCfdMPITest(geometry_mesh_aos_mesh_tests tests/geometry/mesh/implementation/component/CupCfdAoSMeshTests.cpp 4)
		
	# === Sources ===
	addCupCfdMPITest(geometry_mesh_struct_gen_source_tests tests/geometry/mesh/implementation/source/MeshStructGenSourceTests.cpp 4)
	
	if(USE_HDF5)
		addCupCfdMPITest(geometry_mesh_hdf5_source_tests tests/geometry/mesh/implementation/source/MeshHDF5SourceTests.cpp 4)
	endif(USE_HDF5)
	
	addCupCfdTest(geometry_mesh_source_file_config_json_tests tests/geometry/mesh/implementation/source/MeshSourceFileConfigJSONTests.cpp)
	addCupCfdTest(geometry_mesh_source_structgen_config_json_tests tests/geometry/mesh/implementation/source/MeshSourceStructGenConfigJSONTests.cpp)
	addCupCfdMPITest(geometry_mesh_config_source_json_tests tests/geometry/mesh/implementation/source/MeshConfigSourceJSONTests.cpp 4)
	
	# === Configs ===
	addCupCfdTest(geometry_mesh_source_file_config_tests tests/geometry/mesh/implementation/config/MeshSourceFileConfigTests.cpp)
	addCupCfdTest(geometry_mesh_source_struct_gen_config_tests tests/geometry/mesh/implementation/config/MeshSourceStructGenConfigTests.cpp)
	#addCupCfdMPITest(geometry_mesh_config_tests tests/geometry/mesh/implementation/config/MeshConfigTests.cpp 4)


	# =====================================================
	# ====================== Shapes =======================
	# =====================================================	
	
	# ======================
	# ===== Interfaces =====
	# ======================
	
	# === Components ===
	addCupCfdTest(geometry_shapes_polygon3d_tests tests/geometry/shapes/interface/component/Polygon3DTests.cpp)
	addCupCfdTest(geometry_shapes_polyhedron_tests tests/geometry/shapes/interface/component/PolyhedronTests.cpp)

	# === Sources ===

	# === Configs ===

	# ===========================
	# ===== Implementations =====
	# ===========================
	
	# === Components ===	
	addCupCfdTest(geometry_shapes_triangle_tests tests/geometry/shapes/implementation/component/TriangleTests.cpp)
	addCupCfdTest(geometry_shapes_triangle_2d_tests tests/geometry/shapes/implementation/component/Triangle2DTests.cpp)
	addCupCfdTest(geometry_shapes_triangle_3d_tests tests/geometry/shapes/implementation/component/Triangle3DTests.cpp)
	addCupCfdTest(geometry_shapes_quadrilateral_3d_tests tests/geometry/shapes/implementation/component/Quadrilateral3DTests.cpp)
	addCupCfdTest(geometry_shapes_pyramid_tests tests/geometry/shapes/implementation/component/PyramidTests.cpp)
	addCupCfdTest(geometry_shapes_tetrahedron_tests tests/geometry/shapes/implementation/component/TetrahedronTests.cpp)
	addCupCfdTest(geometry_shapes_quadpyramid_tests tests/geometry/shapes/implementation/component/QuadPyramidTests.cpp)
	addCupCfdTest(geometry_shapes_hexahedron_tests tests/geometry/shapes/implementation/component/HexahedronTests.cpp)
	addCupCfdTest(geometry_shapes_triprism_tests tests/geometry/shapes/implementation/component/TriPrismTests.cpp)
	
	# === Sources ===
	
	# === Configs ===


	# === Meshes ===
	
	# Sources
	#addCupCfdTest(geometry_mesh_config_source_tests tests/geometry/mesh/source/MeshConfigSourceTests.cpp)
	#addCupCfdMPITest(geometry_mesh_config_source_json_tests tests/geometry/mesh/source/MeshConfigSourceJSONTests.cpp 4)


	# =====================================================================================================================
	# ============================================== Partitioners =========================================================
	# =====================================================================================================================
	
	# ======================
	# ===== Interfaces =====
	# ======================
	
	# === Configs ===
	addCupCfdTest(partitioner_method_config_tests tests/partitioner/interface/config/PartitionerConfigTests.cpp)
		
	# === Sources ===
	
	# === Components ===
	addCupCfdMPITest(partitioner_interface_tests tests/partitioner/interface/component/PartitionerInterfaceTests.cpp 4)
		
	# ===========================
	# ===== Implementations =====
	# ===========================
	
	# === Configs ===
	addCupCfdTest(partitioner_naive_config_tests tests/partitioner/implementation/config/PartitionerNaiveConfigTests.cpp)
	
	if(USE_PARMETIS)
		addCupCfdTest(partitioner_parmetis_config_tests tests/partitioner/implementation/config/PartitionerParmetisConfigTests.cpp)
		addCupCfdTest(partitioner_metis_config_tests tests/partitioner/implementation/config/PartitionerMetisConfigTests.cpp)
	elseif(USE_METIS)	
		addCupCfdTest(partitioner_metis_config_tests tests/partitioner/implementation/config/PartitionerMetisConfigTests.cpp)
	endif(USE_PARMETIS)
	
	# === Sources ===
	
	# === Components ===	
	addCupCfdMPITest(partitioner_naive_tests tests/partitioner/implementation/component/PartitionerNaiveTests.cpp 4)
	
	if(USE_PARMETIS)
		addCupCfdMPITest(partitioner_parmetis_tests tests/partitioner/implementation/component/PartitionerParmetisTests.cpp 4)
		addCupCfdMPITest(partitioner_metis_tests tests/partitioner/implementation/component/PartitionerMetisTests.cpp 4)
	elseif(USE_METIS)
		addCupCfdMPITest(partitioner_metis_tests tests/partitioner/implementation/component/PartitionerMetisTests.cpp 4)
	endif(USE_PARMETIS)





	# === Sources ===
	#addCupCfdTest(partitioner_config_source_tests tests/partitioner/source/PartitionerConfigSourceTests.cpp)
	#addCupCfdTest(partitioner_config_source_json_tests tests/partitioner/source/PartitionerConfigSourceJSONTests.cpp)
	
	# === Classes ===
	
	
	# =====================================================================================================================
	# ============= Finite Volume Method ================
	# =====================================================================================================================
	
	# ======================
	# ===== Interfaces =====
	# ======================
	
	# === Configs ===

	# === Sources ===
	
	# === Components ===

	# ===========================
	# ===== Implementations =====
	# ===========================
	
	# === Configs ===

	# === Sources ===
	
	# === Components ===	
	
	addCupCfdMPITest(fvm_gradient_kernels tests/fvm/implementation/component/GradientKernelTests.cpp 4)
	addCupCfdMPITest(fvm_mass_kernels tests/fvm/implementation/component/MassKernelTests.cpp 4)
	addCupCfdMPITest(fvm_scalar_kernels tests/fvm/implementation/component/ScalarKernelTests.cpp 4)
	
	# =====================================================================================================================
	# ================ Linear Solvers ===================
	# =====================================================================================================================

	# ======================
	# ===== Interfaces =====
	# ======================
	
	# === Components ===
	addCupCfdTest(linearsolver_tests tests/linearsolvers/interface/component/LinearSolverInterfaceTests.cpp)
			
	# === Configs ===
	
	# === Sources ===
	

	
	# ===========================
	# ===== Implementations =====
	# ===========================
	
	# === Components ===	
	if(USE_PETSC)
		addCupCfdMPITest(linearsolver_petsc_algorithm_tests tests/linearsolvers/implementation/component/LinearSolverPETScAlgorithmTests.cpp 4)
		addCupCfdMPITest(linearsolver_petsc_tests tests/linearsolvers/implementation/component/LinearSolverPETScTests.cpp 4)
	endif(USE_PETSC)
	
	# === Configs ===
	
	# === Sources ===
	






	# Config
	#addCupCfdTest(linearsolver_config_tests tests/linearsolvers/config/LinearSolverConfigTests.cpp)
	
	if(USE_PETSC)
		#addCupCfdTest(linearsolver_config_petsc_tests tests/linearsolvers/config/LinearSolverConfigPETScTests.cpp)
	endif(USE_PETSC)
	
	# Sources
	#addCupCfdTest(linearsolver_config_source_tests tests/linearsolvers/source/LinearSolverConfigSourceTests.cpp)
	#addCupCfdTest(linearsolver_config_source_json_tests tests/linearsolvers/source/LinearSolverConfigSourceJSONTests.cpp)
	
	# Classes
	
	if(USE_PETSC)
		#addCupCfdTest(linearsolver_petsc_serial_tests tests/linearsolvers/classes/LinearSolverPETScSerialTests.cpp)
		#addCupCfdMPITest(linearsolver_petsc_parallel_tests tests/linearsolvers/classes/LinearSolverPETScParallelTests.cpp 4)
	endif(USE_PETSC)
		
	# =====================================================================================================================
	# =============== Particle Systems ==================
	# =====================================================================================================================
	
	# ======================
	# ===== Interfaces =====
	# ======================
	
	# === Components ===

	# === Configs ===
	
	# === Sources ===
	

	# ===========================
	# ===== Implementations =====
	# ===========================
	
	# === Components ===	
	addCupCfdMPITest(particles_particle_simple_tests tests/particles/implementation/component/ParticleSimpleTests.cpp 4)
	addCupCfdMPITest(particles_particle_emitter_simple_tests tests/particles/implementation/component/ParticleEmitterSimpleTests.cpp 4)
	addCupCfdMPITest(particles_particle_system_simple_tests tests/particles/implementation/component/ParticleSystemSimpleTests.cpp 4)
			
	# === Configs ===
	
	# === Sources ===
	
	
	# =====================================================================================================================
	# ================= Benchmarks ======================
	# =====================================================================================================================
	
	# ======================
	# ===== Interfaces =====
	# ======================
	
	# === Components ===
	addCupCfdMPITest(benchmarks_benchmark_tests tests/benchmarks/interface/component/BenchmarkTests.cpp 4)
	addCupCfdMPITest(benchmarks_benchmark_kernels_tests tests/benchmarks/implementation/component/BenchmarkKernelsTests.cpp 4)
				
	# === Configs ===
	
	# === Sources ===
	

	# ===========================
	# ===== Implementations =====
	# ===========================
	
	# === Components ===	
		
	# === Configs ===
	
	# === Sources ===
	
	
	
	
	# Config
	#addCupCfdMPITest(benchmarks_benchmark_config_tests tests/benchmarks/config/BenchmarkConfigTests.cpp 4)
	#addCupCfdMPITest(benchmarks_benchmark_config_exchange_tests tests/benchmarks/config/BenchmarkConfigExchangeTests.cpp 4)
	#addCupCfdMPITest(benchmarks_benchmark_config_kernels_tests tests/benchmarks/config/BenchmarkConfigKernelsTests.cpp 4)
	#addCupCfdMPITest(benchmarks_benchmark_config_linearsolvers_tests tests/benchmarks/config/BenchmarkConfigLinearSolversTests.cpp 4)
	#addCupCfdMPITest(benchmarks_benchmark_config_particles_tests tests/benchmarks/config/BenchmarkConfigParticlesTests.cpp 4)
	
	# Source
	#addCupCfdTest(benchmarks_benchmark_config_source_tests tests/benchmarks/source/BenchmarkConfigSourceTests.cpp 4)
	
	# Classes
	
	#addCupCfdMPITest(benchmarks_benchmark_exchange_tests tests/benchmarks/classes/BenchmarkExchangeTests.cpp 4)
	
	#addCupCfdMPITest(benchmarks_benchmark_linearsolvers_tests tests/benchmarks/classes/BenchmarkLinearSolversTests.cpp 4)
	#addCupCfdMPITest(benchmarks_benchmark_particles_tests tests/benchmarks/classes/BenchmarkParticlesTests.cpp 4)
	
	# =====================================================================================================================
	# ================== Structure ======================
	# =====================================================================================================================

	# ======================
	# ===== Interfaces =====
	# ======================
	
	# === Components ===
				
	# === Configs ===
	
	# === Sources ===
	
	# ===========================
	# ===== Implementations =====
	# ===========================
	
	# === Components ===	
		
	# === Configs ===
	
	# === Sources ===
	
endif (USE_UNIT_TESTS)
