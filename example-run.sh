#!/bin/bash

set -e

cd examples

## Currently, cup-cfd loads a hard-coded json file:
# ln -sf examplejson.json cupcfd.json
# ln -sf particlesOnGen.json cupcfd.json
ln -sf particlesOnGen-large.json cupcfd.json

if [ -d tt_results ]; then
	rm -r tt_results
fi

# _n=3
_n=4

# mpirun -n ${_n} xterm -fa 'Monospace' -fs 14 -e gdb ../build/cupcfd
# mpirun -n ${_n} xterm -fa 'Monospace' -fs 12 -e gdb ../build/cupcfd
mpirun -n ${_n} ../build/cupcfd

if [ -d "run-n${_n}-tt_results" ]; then
	rm -r "run-n${_n}-tt_results"
fi
mv tt_results "run-n${_n}-tt_results"
