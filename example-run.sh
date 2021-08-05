#!/bin/bash

set -e

cd examples

## Currently, cup-cfd loads a hard-coded json file:
# ln -sf examplejson.json cupcfd.json
ln -sf particlesOnGen.simple.json cupcfd.json

if [ -d tt_results ]; then
	rm -r tt_results
fi

mpirun -n 4 ../build/cupcfd
