# -Try to find PETSC Library
# Once done this will define
#PETSC_FOUND - System has libpetsc
#PETSC_INCLUDE_DIRS - The petsc include directories
#PETSC_LIBRARIES - The libraries needed to use petsc
#PETSC_DEFINITIONS - Compiler switches required for using libpetsc
#
# Variables that can be used by this module.
# They must be defined before calling find_package.
# PETSC_ROOT - Root Directory

# Petsc Include Directories
if(NOT PETSC_INCLUDE_DIRS)
	find_path(PETSC_INCLUDE_DIRS petsc.h
		HINTS ${PETSC_ROOT} ENV PETSC_ROOT				# Petsc Root Path Variable
			  ${PETSC_INCLUDE_DIR} ENV PETSC_INCLUDE_DIR	# Petsc Include Dir Variable
		PATH_SUFFIXES include
		DOC "Directory where the PETSC header files can be found"
	)
endif()


# Petsc Library Directories
if(NOT PETSC_LIBRARIES)
	# Look for static library first?
	set(CMAKE_FIND_LIBRARY_SUFFIXES .a;.so)

	find_library(PETSC_LIBRARIES
		NAMES petsc
		HINTS ${PETSC_ROOT} ENV PETSC_ROOT				# Petsc Root Path Variable
			  ${PETSC_LIB_DIR} ENV PETSC_LIB_DIR			# Petsc Lib Dir Variable
		PATH_SUFFIXES lib
		DOC "Directory where the petsc libraries can be found"
	)

	# Restore library suffix order
	set(CMAKE_FIND_LIBRARY_SUFFIXES .so;.a)

endif()


# Test build of simple program
# https://cmake.org/cmake/help/v3.0/module/CheckCSourceRuns.html
# Test: Include of petsc.h

# Test: Linking library for PETSC function.


# ToDo: Petsc Version Identification

# Handling of find_package arguments
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(PETSC DEFAULT_MSG PETSC_INCLUDE_DIRS PETSC_LIBRARIES)
