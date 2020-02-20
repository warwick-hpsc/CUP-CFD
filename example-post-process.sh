#!/bin/bash

set -e
set -u

python ./scripts/treetimer-postprocessing.py examples/tt_results/results.0.db | head -n10 | column -s',' -t
