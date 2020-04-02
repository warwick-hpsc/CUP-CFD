#!/bin/bash

set -e
set -u

if [ "$COMPILER" = "gnu" ]; then
	export CC=mpicc
	export CXX=mpic++
elif [ "$COMPILER" = "intel" ]; then
	export CC=mpiicc
	export CXX=mpicxx
fi

## 3rd-party libraries:
HDF5_ROOT=/path/to/hdf5-1.8.21-parallel
METIS_ROOT=/path/to/metis-5.1.0
PARMETIS_ROOT=/path/to/parmetis-4.0.3
PETSC_ROOT=/path/to/petsc-3.12.0
SQLITE_ROOT=/path/to/sqlite

## Link to treetimer library:
TREETIMER_ROOT=`pwd`/../treetimer

# ## Optional: clean previous build files:
# rm -rf build
# mkdir build

# Compile!
RTYPE=Release
# RTYPE=Debug
cd build
cmake .. -DHDF5_ROOT="$HDF5_ROOT" \
         -DCMAKE_BUILD_TYPE="$RTYPE" \
         -DMETIS_ROOT="$METIS_ROOT" \
         -DPARMETIS_ROOT="$PARMETIS_ROOT" \
		 -DSQLITE_LIBS="$SQLITE_ROOT"/lib/libsqlite3.so \
		 -DPETSC_ROOT="$PETSC_ROOT" \
		 -DTREETIMER_LIBS="$TREETIMER_ROOT"/libtt.so \
		 -DTREETIMER_INCLUDE="$TREETIMER_ROOT"/include/timing_library/interface \
		 -DUSE_UNIT_TESTS=OFF

# make VERBOSE=1 -j 8
make -j 8
