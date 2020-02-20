
This README will cover some details of note for the CupCfd Proxy app.

Areas that might be of import to address are:
(a) Testing/any issues with the build system at large.
(b) A code review of its components prior to any distribution to ensure that non-distributable parts are not present.
(c) A review of any licensing issues and/or IP usage. This may also include how to access/whether dependancies can be distributed.
(d) A list of known issues and/or future tasks is included in the file under docs/TODO. Some of these are merely wishlist items,
but some others might be considered of import - in particular how benchmarks are setup/parameters stored/results presented.
(e) A license should be decided upon, with the caveat that it must be compatible with the Apache License (see the file docs/IP).

#====================================================================================================================================================
#======= Directory Structure ========
#====================================================================================================================================================

doc - Documents
	: README - Detail Build, Run and General Information
	  IP - Details IP/licensing details that might be important about CupCfd during review/for later Ip documents
	  TODO - Known issues/future tasks

cmake - Custom Cmake Modules
include - Headers
src - src files
test - Unit tests
examples - Any example files (some may also be found in test folders such as example matrix formats)
scripts - Relevant scripts that might be of use

#====================================================================================================================================================
#======= Build Guide ========
#====================================================================================================================================================

#=================================================================================
(a) Running the build system 
#=================================================================================

CupCfd uses a cmake build system

To build, run the following commands:

mkdir build
cd build
cmake .. -DHDF5_ROOT=<Top Level HDF5 Root Directory> \
         -DMETIS_ROOT=<Top Level Metis Root Directory> \
         -DPARMETIS_ROOT=<Top Level Parmetis Root Directory> \
         -DPETSC_ROOT=<Top Level Petsc Root Directory>

A functional example is provided in the example-build-script in the top-level directory of cupcfd

The following parameters also need to be set (they are currently just set inside CMakeLists.txt, although perhaps setting them via command line
would be prefereable)

SQLITE_LIBS This is the full path to an SQLite shared library (i.e. <path>/libsqlite3.so) - dependancy for the treetimer library
TREETIMER_LIBS This is a full path to the TreeTimer Library (i.e. <path>/libtt.so)
TREETIMER_INCLUDE this is a full path to the interface include folder of the treetimer library (e.g. <toplevelpath>/include/timing_library/interface)

Note 2:
	There is a provision for disabling building with HDF5, Metis/Parmetis and/or PETSc via the USE_<Package> flags in CMakeLists.txt.
	However this setup is untested and likely to break compilation currently, since there are likely components that need wrapping with ifdefs
	(e.g. header includes, interface passthroughs). Expansion to make them optional is a future task.

#=================================================================================
(b) Header Override Values
#=================================================================================

The following values are defined in header files, but it may be desired for experimental purposes to override them:

# Pad/Increase the size of the CupCfdAoSMeshBoundary class in an AoS Mesh beyond that of the required values
# This is to measure the impact of the class size in an AoS setting.
CUPCFD_AOS_MESH_BOUNDARY_PADDING

# Pad/Increase the size of the CupCfdAoSMeshCell class in an AoS Mesh beyond that of the required values
# This is to measure the impact of the class size in an AoS setting.
CUPCFD_AOS_MESH_CELL_PADDING

# Pad/Increase the size of the CupCfdAoSMeshFace class in an AoS Mesh beyond that of the required values
# This is to measure the impact of the class size in an AoS setting.
CUPCFD_AOS_MESH_FACE_PADDING

# Pad/Increase the size of the CupCfdAoSMeshRegion class in an AoS Mesh beyond that of the required values
# This is to measure the impact of the class size in an AoS setting.
CUPCFD_AOS_MESH_REGION_PADDING

# Pad/Increase the size of the CupCfdAoSMeshVertex class in an AoS Mesh beyond that of the required values
# This is to measure the impact of the class size in an AoS setting.
CUPCFD_AOS_MESH_VERTEX_PADDING

# Set the delta threshold for whether two single-precision floating point numbers are considered equivalent. Used in ArithmeticKernels.ipp
CUPCFD_ZERO_COMP_TOL_F

#Set the delta threshold for whether two double precision floating point numbers are considered equivalent. Used in ArithmeticKernels.ipp
CUPCFD_ZERO_COMP_TOL_D

#=================================================================================
(c) Unit Tests
#=================================================================================

A number of units are provided for testing the code using Boost. The majority of the critical components should currently be covered (although there
remains some further development to be done). However building these unit tests can significantly inflate the build time, and building them requires
a dependancy on the boost unit tests library (libboost-test-dev package under debian).
As such they can be disabled via the use of the "USE_UNIT_TESTS" flag under the cmake system.

If they are built, they can be run using the command "ctest" in the build directory. Some tests test parallel functionality, and will run using
4 MPI processes. The mpi command used to run this is defined by the "MPIRUN_EXEC" flag in the cmake system, but defaults to srun in CMakeLists.txt 
(or could be changed in the CMakeLists.txt file).


#====================================================================================================================================================
# ======== Run Guide ==========
#====================================================================================================================================================

#================================
# ==== Cupcfd Operation ====
#================================

Cupcfd can be run simply with the command "<mpirun> -n <number of processes> cupcfd".

All options are taken from a json file called cupcfd.json, located in the same folder as the cupcfd binary. Access to any other files
is defined inside this configuration file.

#================================
# ==== JSON Parameters ====
#================================

This may not quite be comprehensive - if in doubt JSON options should be documented in comments or in the code itself, but here is a description of some expected formats.

TopLevel:

###
DataTypes
###

Fields are IntDataType, FloatDataType, MeshDataType

Field Options:
	IntDataType: ["int"]				- Use int datatype for indexing
	FloatDatType ["float"|"double"]		- Use float or double datatype for indexing
	MeshDataType ["MiniAoS"|"MiniSoA"]	- Use AoS or SoA for the mesh geometry format

#####
Mesh
#####

Fields are partitioner, MeshSource.

Field Options:

Partitioner: Accepts another JSON Field named from one of ["NaivePartitioner"|"MetisPartitioner"|"ParmetisPartitioner"]
None of these fields accepts further options currently.
	NaivePartitioner - Use a naive partitioner
	MetisPartitioner - Use metis to partition
	ParmetisPartitioner - Use parmetis to partition

MeshSource:
	Mesh Source accepts another set of subfields to define different mesh source types.

	Formats:
		"MeshSource" : {
		    "MeshSourceFile" : {						# The mesh is to be loaded from a file
        		"FileFormat" : "HDF5",			# The Mesh is to be loaded using HDF5
		        "FilePath" : "../tests/geometry/mesh/data/MeshHDF5.hdf5"		# The Mesh file location
    		}
		}

			This loads the HDF5 file at the FilePath, using the HDF5 format for HDF5. An simple example of the format can be found
			in the unit tests

		OR

        "MeshSource": {
            "MeshSourceStructGen": {		# The Mesh is generated as a regular grid
                "CellX" : 11,				# Number of cells in the x dimension
                "CellY" : 12,				# Number of cells in the Y dimension
                "CellZ" : 14,				# Number of cells in the Z dimension
                "SpatialXMin" : 0.0,		# Minimum value in the X spatial dimension
                "SpatialYMin" : 0.0,		# Minimum value in the Y spatial dimension
                "SpatialZMin" : 0.0,		# Minimum value in the Z spatial dimension
                "SpatialXMax" : 1.0,		# Maximum value in the X spatial dimension
                "SpatialYMax" : 1.0,		# Maximum value in the Y spatial dimension
                "SpatialZMax" : 1.0			# Maximum value in the Z spatial dimension
            }   
        }

Benchmarks:
	The Benchmark field can have any number of different types of benchmark jsons defined under it in an array.

         "BenchmarkKernels" : {							# Setup a benchmark for the kernels
                "BenchmarkName" : "KernelTest",			# Name of the benchmark (should be unique)
                "Repetitions"   : 1000					# Number of repetitions of the benchmark
            }

            "BenchmarkExchange" : {						# Setup a benchmark for comms exchange
                "BenchmarkName" : "ExchangeTest",		# Name of the benchmark (should be unique)
                "Repetitions"   : 10,					# Number of repetitions of the benchmark
                "ExchangePattern" : { "Method" : "NBTwoSided"}	# Exchange Pattern to use - Current options are NBOneSided (Non-Blocking one sided comms) 
																# or NBTwoSided (Non blocking two sided comms)
            }

			"BenchmarkLinearSolver" : {						# Benchmark a linear solver
                "BenchmarkName" : "LinearSolverTest",		# Name of the benchmark (should be unique)
                "Repetitions"   : 10,						# Number of repetitions of the benchmark
                "LinearSolver"  : {							# Linear Solver to use
                    "LinearSolverPETSc" : {					# Use PETSc (only current option)
                        "Algorithm" : "CGAMG",				# Algorithm type - currently "CGAMG" for CG with AMG preconditioning or "CommandLine" for parsing PETSc options from
															# the command line (untested)
                        "eTol"  : 1e-6,						# Set the etolerance
                        "rTol"  : 1e-6						# Set the rtolerance
                    }
                },
                "SparseMatrix"  : {							# Specify the sparsematrix source
                    "SparseMatrixFile" : {					# Load a sparse matrix form a file (current only option)
                        "FilePath" : "../tests/linearsolvers/data/SolverMatrixInput.h5",	# Path to Sparse Matrix file (see tests for example)
                        "FileFormat" : "HDF5"												# File Format (Currently only HDF5 accepted)
                    }
                },
                "RHSVector" : {								# Specify the right hand vector source
                    "VectorFile" : {						# Load a Vector from a file
                        "FilePath" : "../tests/linearsolvers/data/ZeroVector.h5",		# Path to file (see tests for example)
                        "FileFormat" : "HDF5"											# File Format (Currently only HDF5 accepted)
                    }
				},
                "SolVector" : {								# Specify the solution vector source
                    "VectorFile" : {						# Load a Vector from a file
                        "FilePath" : "../tests/linearsolvers/data/ZeroVector.h5",		# Path to file (see tests for example)
                        "FileFormat" : "HDF5"											# File Format (Currently only HDF5 accepted)
                    }
                },
                "DataDistribution"  : "Concurrent"			# Specify the type of solve to run
															# "Concurrent" - Load a full copy of the matrix on every process and run simultaniously
															# "Distributed" - Load part of the matrix on every process and run using a parallel solve.
            }


            "BenchmarkParticleSystem" : {							# Benchmark a particle system
                "BenchmarkName" : "ParticleSystemSimpleTest",		# Name of the benchmark (should be unique)
                "Repetitions"   : 1,    							# Number of repetitions
                "NTimesteps"    : 10,								# Number of timesteps to run for
                "DtDistribution" : {"FixedDistribution" : {"value" : 0.13171}},		# Specify the distribution for the time used for each timestep (see below)
                "ParticleSystemSimple" : {							# Use a ParticleSimple System (Only option for now)
                    "ParticleSourceSimple" : {						# Specify a source to load particle data from (optional)
                        "FilePath" : "../tests/particles/data/ParticleSourceSimpleExample.h5",	# Path to data file
                        "FileFormat" : "HDF5"													# Format (See tests for an example)
                    },
                    "Emitters": [							# Define an array of emitters to use for generating particles
                        {
                            "ParticleEmitterSimple" : { 	# Deines a Emitter of Simple Particles (currently only option)
                                "Position" : {"X": 0.05, "Y": 0.12, "Z": 0.16},					# Define position of emitter in mesh (X,Y,Z coordinates)
                                "Rate" : {"FixedDistribution" : {"value" : 0.1}},				# Define rate particles are generated at (distribution, see below)
                                "AngleXY" : {"FixedDistribution" : {"value" : 0.1}},			# Define angle on XY plane of velocity vector (distribution)
                                "AngleRotation" : {"FixedDistribution" : {"value" : 0.1}},		# Define angle about Z plane for velocity vector (distribution)
                                "Speed" : {"FixedDistribution" : {"value" : 0.1}},				# Define speed of generated particle (distribution)
                                "AccelerationX" : {"FixedDistribution" : {"value" : 0.1}},		# Define X acceleration of generated particle (distribution)
                                "AccelerationY" : {"FixedDistribution" : {"value" : 0.1}},		# Define Y acceleration of generated particle (distribution)
                                "AccelerationZ" : {"FixedDistribution" : {"value" : 0.1}},		# Define Z acceleration of generated particle (distribution)
                                "JerkX" : {"FixedDistribution" : {"value" : 0.1}},				# Define X Jerk of generated particle (distribution)
                                "JerkY" : {"FixedDistribution" : {"value" : 0.1}},				# Define Y jerk of generated particle (distribution)
                                "JerkZ" : {"FixedDistribution" : {"value" : 0.1}},				# Define Z jerk of generated particle (distribution)
                                "DecayRate" : {"FixedDistribution" : {"value" : 0.1}},			# Define rate of particle decay (cumulative towards threshold) (distribution)
                                "DecayThreshold" : {"FixedDistribution" : {"value" : 10.0}}		# Define threshold when particle has fully decay and will be removed (distribution)
                            }
                        },
                        {
                            "ParticleEmitterSimple" : { 
                                "Position" : {"X": 1.3, "Y": 1.72, "Z": -0.76},
                                "Rate" : {"FixedDistribution" : {"value" : 0.1}},
                                "AngleXY" : {"FixedDistribution" : {"value" : 0.1}},
                                "AngleRotation" : {"FixedDistribution" : {"value" : 0.1}},
                                "Speed" : {"FixedDistribution" : {"value" : 0.1}},
                                "AccelerationX" : {"FixedDistribution" : {"value" : 0.1}},
                                "AccelerationY" : {"FixedDistribution" : {"value" : 0.1}},
                                "AccelerationZ" : {"FixedDistribution" : {"value" : 0.1}},
                                "JerkX" : {"FixedDistribution" : {"value" : 0.1}},
                                "JerkY" : {"FixedDistribution" : {"value" : 0.1}},
                                "JerkZ" : {"FixedDistribution" : {"value" : 0.1}},
                                "DecayRate" : {"FixedDistribution" : {"value" : 0.1}},
                                "DecayThreshold" : {"FixedDistribution" : {"value" : 0.1}}
                            }
                        }
                    ]


		# Distributions:
		Where Distributions are used, any one of three current distributions can be defined:

		"FixedDistribution" : {
			"value" : 0.1				# Defines a distribution that always gives the same, fixed value specified here
		}

		"NormalDistribution" : {		# Defines a normal distribution
			"lbound" : 0.1,				# Lower bound of distribution
			"ubound" : 0.5,				# Upper bound of distribution
			"mean" : 0.25,				# Mean of distribution
			"stdev" : 0.02				# Standard Deviation of distribution
		}

		"UniformDistribution" : {		# Defines a uniform distribution
			"lbound" : 0.1,				# Lower bound of distribution
			"ubound" : 0.5				# Upper bound of distribution
		}


#================================
# ==== TreeTimer ====
#================================

The TreeTimer Library is a custom library intended to capture data about instrumented blocks in the code. It tracks the relationship
between codeblocks based on how they are nested in one another akin to a callpath tree. In each block, as well as timing data, it can also
store 'local' parameters that are associated with the block in which they are stored, and 'global' parameters that can be associated with an entire
run.

#================
Usage
#================

Postprocessing:
The library itself must be initialised with TreeTimerInit() prior to any blocks, and finalized with TreeTimerFinalize() to write out its results.
The results are currently written out to an SQLite Database for postprocessing, such as by the example python script provided at scripts/treetimer-postprocessing.

The script can be used by running it with the path to a SQlite database file output by the instrumentation library as an argument.
A small selection of post-processing methods can be selected by uncommenting the appropriate method.

Instrumentation:
Timer Blocks are started with a entry method call associated with a "BlockName", beginning a timing/parameter data capture.
Timer Blocks are ended with a exit method call associated with a "BlockName", ending the timing of the current block entry and storing either a new trace entry or
adding to the aggregate data. Please note that if the exit blockname differs from the name of the current block the library thinks it is in, a warning will be
printed since the data will be invalid (i.e. the entry/exits should match in a stacking order if they are nested)

Different types of entry/exit methods are used to tracked different types of blocks - e.g. method calls, compute, MPI etc.
The selection can be found in tt_interface_c.h and is currently as follows:

TreeTimer[Enter|Exit]BlockMethod(<Name>) : Enter/Exit a Method Call (typically used at start and end of methods)
TreeTimer[Enter|Exit]BlockLoop(<Name>) : Enter/Exit an arbitrary loop
TreeTimer[Enter|Exit]BlockComputeLoop(<Name>) : Enter/Exit a compute loop
TreeTimer[Enter|Exit]BlockUnspecified(<Name>) : Enter/Exit a custom block not specifically associated with a type of behaviour
TreeTimer[Enter|Exit]BlockNonMPICall(<Name>) : Enter/Exit a Non-MPI method call (similiar to BlockMethod, but might be used for third-party library calls)
TreeTimer[Enter|Exit]BlockMPICommCall(<Name>) : Enter/Exit a MPI Call that uses communications
TreeTimer[Enter|Exit]BlockMPINOnCommCall(<Name>) : Enter/Exit a MPI call that does not use communications
TreeTimer[Enter|Exit]BlockIOCall(<Name>) : Enter/exit an I/O Block.

These use of these is effectively arbitrary - these methods do not do anything special to detect the types of behaviours. It is just the case that which method is used
determines what type of behaviour is associated with the timer block in the output data/post-processing.

Local Parameters can be tracked using the following methods:

TreeTimerLogParameterInt(<"name">, value) : Stores an integer value associated with the parameter "name" and the block the library is currently in (i.e. the last entered but not exited)
TreeTimerLogParameterDouble(<"name">, value) : Stores an double value associated with the parameter "name" and the block the library is currently in (i.e. the last entered but not exited)
TreeTimerLogParameterBool(<"name">, value) : Stores an bool value associated with the parameter "name" and the block the library is currently in (i.e. the last entered but not exited)
TreeTimerLogParameterString(<"name">, value) : Stores an string associated with the parameter "name" and the block the library is currently in (i.e. the last entered but not exited)

Global equivalents can be stored in a similar fashion, but they are not associated with any particular block.

TreeTimerLogGlobalParameterInt
TreeTimerLogGlobalParameterDouble
TreeTimerLogGlobalParameterBool
TreeTimerLogGlobalParameterString

To use these operations the file tt_interface_c.h should be included.
Please see the src code in the benchmarks folder for examples of its usage.
Currently a Fortran and C interface exists (CupCfd uses the C interface)

The data storage layout is too lengthy to detail here, but the schemas used can be found in treetimer/src/database/sqlite3/ folder.

Provision is made to store machine data in a location in the database, but this data is not currently captured and thus these tables in the database remain empty.

#================
RunTime Options
#================

The TreeTimer Library has a number of configuration options that can be set at runtime via the use of enviromental parameters:

TT_CONFIG_APPNAME : String. This environment variable contains the application name that will be associated with the results in the database.

Aggregate Data: This will store aggregated data to minimise data output.
TT_CONFIG_ATIMERS : [0|1] If set to 1, this enables aggregate timing data for a block. 0 disables this option. By default it is on if unset.
TT_CONFIG_APARAM : [0:1] If set to 1, this enables aggregate parameter data for a block. 0 disables this option. By default it is on if unset.
TT_CONFIG_APAPI : [0:1] If set to 1, this enables aggregate papi data for a block. 0 disables this option. This is a placeholder - PAPI is not yet included in this version.

Trace Data: This will store a unique data entry for every block entry/exit, enabling traces to be reconstructed.
TT_CONFIG_TTIMERS : [0|1] If set to 1, this enables trace timing data for a block. 0 disables this option. By default it is on if unset.
TT_CONFIG_TPARAM : [0:1] If set to 1, this enables trace parameter data for a block. 0 disables this option. By default it is on if unset.
TT_CONFIG_TPAPI : [0:1] If set to 1, this enables trace papi data for a block. 0 disables this option. This is a placeholder - PAPI is not yet included in this version.

TT_CONFIG_MPI_HOOKS: [0|1] If set to 1, this enables the wrapping of all MPI function calls (at least, those implemented in the library) such that MPI specific
data including times, destinations, byte sizes, buffer sizes etc will be captured (depending on the type of call). 
0 disables this option. This option is also influenced by the aggreate/trace configuration options - i.e. if trace is enabled then trace data should be stored for MPI calls.

This particular options requires the use of LD_PRELOAD and a shared treetimer library to be effective, since it utilises PMPI to catch MPI calls. It may work without LD_PRELOAD
or a static library, but is not guaranteed or recommended since it will likely depend upon linking orders.

#====================================================================================================================================================
# ======== Doxygen Guide ==========
#====================================================================================================================================================

Most of the documentation for CupCfd can be found in the form of doxygen comments in the header files. This documentation can be generated using
the doxygen scripts under ./scripts, and the documentation can be navigated from docs/doxygen/html/index.html.
