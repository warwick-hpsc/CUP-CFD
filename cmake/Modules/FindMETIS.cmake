# -Try to find METIS Library
# Once done this will define
#METIS_FOUND - System has libmetis
#METIS_INCLUDE_DIRS - The metis include directories
#METIS_LIBRARIES - The libraries needed to use metis
#METIS_DEFINITIONS - Compiler switches required for using libmetis
#
# Variables that can be used by this module.
# They must be defined before calling find_package.
# METIS_ROOT - Root Directory


# Metis Include Directories
if(NOT METIS_INCLUDE_DIRS)
	find_path(METIS_INCLUDE_DIRS metis.h
		HINTS ${METIS_ROOT} ENV METIS_ROOT					# Metis Root Path Variable
			  ${METIS_INCLUDE_DIR} ENV METIS_INCLUDE_DIR	# Metis Include Dir Variable
		PATH_SUFFIXES include
		DOC "Directory where the METIS header files can be found"
	)
endif()


# Metis Library Directories
if(NOT METIS_LIBRARIES)
	# Look for static library first?
	set(CMAKE_FIND_LIBRARY_SUFFIXES .a;.so)

	find_library(METIS_LIBRARIES
		NAMES metis
		HINTS ${METIS_ROOT} ENV METIS_ROOT					# Metis Root Path Variable
			  ${METIS_LIB_DIR} ENV METIS_LIB_DIR			# Metis Lib Dir Variable
		PATH_SUFFIXES lib
		DOC "Directory where the metis libraries can be found"
	)

	# Restore library suffix order
	set(CMAKE_FIND_LIBRARY_SUFFIXES .so;.a)

endif()


# Test build of simple program
# https://cmake.org/cmake/help/v3.0/module/CheckCSourceRuns.html
# Test: Include of metis.h

# Test: Linking library for METIS function.


# ToDo: Metis Version Identification
# Defined in metis.h
# METIS_VER_MAJOR
# METIS_VER_MINOR
# METIS_VER_SUBMINOR


# Handling of find_package arguments
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(METIS DEFAULT_MSG METIS_INCLUDE_DIRS METIS_LIBRARIES)
