#!/bin/bash

set -e

cd examples

## Currently, cup-cfd loads a hard-coded json file:
# ln -sf examplejson.json cupcfd.json
ln -sf particlesOnGen.json cupcfd.json

if [ -d tt_results ]; then
	rm -r tt_results
fi

mpirun -n 2 xterm -fa 'Monospace' -fs 14 -e gdb ../build/cupcfd

