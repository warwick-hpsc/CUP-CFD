#!/bin/bash

set -e

cd examples

## Currently, cup-cfd loads a hard-coded json file:
if [ ! -f cupcfd.json ]; then
	ln -s examplejson.json cupcfd.json
fi

../build/cupcfd
