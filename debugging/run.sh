#!/bin/bash

set -e

BDIR=build-Debug

## Currently, cup-cfd loads a hard-coded json file:
ln -sf particles.json cupcfd.json

export TT_CONFIG_ATIMERS=1
export TT_CONFIG_APARAM=1
export TT_CONFIG_AGG_STEP_NODE=ParticleSystemTimestep
export TT_CONFIG_AGG_STEP_INTERVAL=2

# export TT_CONFIG_TTIMERS=1
# export TT_CONFIG_TPARAM=1
# export TT_CONFIG_TRACE_CONDUCTOR_NODE=ParticleSystemTimestep

# export TT_CONFIG_MPI_HOOKS=1
# export LD_LIBRARY_PATH="$HOME"/local/SPARTA/impi-classic-2021.3/treetimer.b.windowed-aggregation:"$LD_LIBRARY_PATH"
# export LD_PRELOAD="$HOME"/local/SPARTA/impi-classic-2021.3/treetimer.b.windowed-aggregation/libtt.so

if [ -d tt_results ]; then
	rm -r tt_results
fi

# _nranks=4
# _nranks=3
# _nranks=2
# _nranks=1
# _nranks=`nproc`
_nranks=$((`nproc` - 1))

run_valgrind=false
run_gdb=false
run_valgrind=true
# run_gdb=true
debug_rank=0
# debug_rank=1


if $run_gdb ; then
	AC=gdb.appcontext
	if [ -f "$AC" ]; then rm "$AC" ; fi
	if [ "$debug_rank" != "0" ]; then
		echo "-np $((debug_rank)) ../$BDIR/cupcfd" >> "$AC"
	fi
	echo "-np 1 gdb ../$BDIR/cupcfd" >> "$AC"
	if [ "$debug_rank" != "$((_nranks - 1))" ]; then
		echo "-np $((_nranks - debug_rank - 1)) ../$BDIR/cupcfd" >> "$AC"
	fi
	cat "$AC" ; sleep 2
	mpirun --app "$AC"

elif $run_valgrind ; then
	SUPPS=""
	# SUPPS+=" --suppressions=/opt/intel/oneapi/2021.3/itac/2021.3.0/lib/impi.supp"
	# SUPPS+=" --suppressions=../impi.supp"

	SUPPS+=" --suppressions=/opt/mpi/openmpi/4.0.4/gnu/9.3.0/share/pmix/pmix-valgrind.supp"
	SUPPS+=" --suppressions=/opt/mpi/openmpi/4.0.4/gnu/9.3.0/share/openmpi/openmpi-valgrind.supp"
	SUPPS+=" --suppressions=ValgrindSupps/ompi.supp"

	#SUPPS+=" --suppressions=ValgrindSupps/libipathverbs-rdmav2.supp"
	#SUPPS+=" --suppressions=ValgrindSupps/librdmacm.supp"
	#SUPPS+=" --suppressions=ValgrindSupps/mpi-misc.supp"

	SUPPS+=" --suppressions=ValgrindSupps/treetimer.supp"
	SUPPS+=" --suppressions=ValgrindSupps/cupcfd.supp"
	SUPPS+=" --suppressions=ValgrindSupps/main-ompi.supp"

	SUPPS+=" --suppressions=ValgrindSupps/stdlib.supp"
	SUPPS+=" --suppressions=ValgrindSupps/system.supp"
	
	SUPPS+=" --suppressions=ValgrindSupps/parmetis.supp"
	SUPPS+=" --suppressions=ValgrindSupps/sqlite.supp"
	SUPPS+=" --suppressions=ValgrindSupps/misc.supp"

	SUPPS+=" --gen-suppressions=yes"
	# SUPPS+=" --gen-suppressions=all"

	AC=valgrind.appcontext
	if [ -f "$AC" ]; then rm "$AC" ; fi
	if [ "$debug_rank" != "0" ]; then
		echo "-np $((debug_rank)) ../$BDIR/cupcfd" >> "$AC"
	fi
	if [ "$SUPPS" != "" ]; then
		echo "-np 1 valgrind -v $SUPPS --leak-check=full --show-leak-kinds=all --track-origins=yes ../$BDIR/cupcfd" >> "$AC"
	else
		echo "-np 1 valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ../$BDIR/cupcfd" >> "$AC"
	fi
	if [ "$debug_rank" != "$((_nranks - 1))" ]; then
		echo "-np $((_nranks - debug_rank - 1)) ../$BDIR/cupcfd" >> "$AC"
	fi
	cat "$AC" ; sleep 2
	mpirun --app "$AC" 2>&1 | tee ../valgrind.stdout

else
	mpirun -np $_nranks ../"$BDIR"/cupcfd
fi
