#!/bin/bash

set -e

cd examples

## Currently, cup-cfd loads a hard-coded json file:
# ln -sf benchmarks.json cupcfd.json
ln -sf particlesOnGen.simple.json cupcfd.json
# ln -sf exchange.json cupcfd.json

if [ -d tt_results ]; then
	rm -r tt_results
fi

# _nranks=3
_nranks=2
# _nranks=1

# mpirun -n ${_nranks} xterm -fa 'Monospace' -fs 14 -e gdb ../build/cupcfd
# mpirun -n ${_nranks} xterm -fa 'Monospace' -fs 12 -e gdb ../build/cupcfd
# mpirun -n ${_nranks} xterm -fa 'Monospace' -fs 10 -e gdb ../build/cupcfd
mpirun -n ${_nranks} ../build/cupcfd
# ../build/cupcfd

# valgrind --leak-check=yes ../build/cupcfd
# valgrind ../build/cupcfd
# gdb ../build/cupcfd
