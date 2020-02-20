# -Try to find PARMETIS Library
# Once done this will define
#PARMETIS_FOUND - System has libparmetis
#PARMETIS_INCLUDE_DIRS - The parmetis include directories
#PARMETIS_LIBRARIES - The libraries needed to use parmetis
#PARMETIS_DEFINITIONS - Compiler switches required for using libparmetis
#
# Variables that can be used by this module.
# They must be defined before calling find_package.
# PARMETIS_ROOT - Root Directory
# PARMETIS_INCLUDE_DIR
# PARMETIS_LIB_DIR

# ParMetis Include Directories
if(NOT PARMETIS_INCLUDE_DIRS)
	find_path(PARMETIS_INCLUDE_DIRS parmetis.h
		HINTS ${PARMETIS_ROOT} ENV PARMETIS_ROOT					# Parmetis Root Path Variable
			  ${PARMETIS_INCLUDE_DIR} ENV PARMETIS_INCLUDE_DIR		# Parmetis Include Dir Variable
		PATH_SUFFIXES include
		DOC "Directory where the PARMETIS header files can be found"
	)
endif()


# ParMetis Library Directories
if(NOT PARMETIS_LIBRARIES)
	# Look for static library first?
	set(CMAKE_FIND_LIBRARY_SUFFIXES .a;.so)

	find_library(PARMETIS_LIBRARIES
		NAMES parmetis
		HINTS ${PARMETIS_ROOT} ENV PARMETIS_ROOT					# Parmetis Root Path Variable
			  ${PARMETIS_LIB_DIR} ENV PARMETIS_LIB_DIR				# Parmetis Lib Dir Variable
		PATH_SUFFIXES lib
		DOC "Directory where the metis libraries can be found"
	)

	# Restore library suffix order
	set(CMAKE_FIND_LIBRARY_SUFFIXES .so;.a)

endif()


# Test build of simple program
# https://cmake.org/cmake/help/v3.0/module/CheckCSourceRuns.html
# Test: Include of parmetis.h

# Test: Linking library for PARMETIS function.


# ToDo: Parmetis Version Identification

# Handling of find_package arguments
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(PARMETIS DEFAULT_MSG PARMETIS_INCLUDE_DIRS PARMETIS_LIBRARIES)
