#!/bin/bash

#SBATCH --job-name=cup-cfd
#SBATCH --output=sbatch.stdout
#SBATCH --error=sbatch.stderr

#SBATCH --nodes=1
#SBATCH --ntasks=12
#SBATCH --exclusive
#SBATCH -m block:block

#SBATCH --time=01:00:00
#SBATCH --partition=cpu

RUN_CMD="srun --cpu_bind=cores"

export OMP_NUM_THREADS=$SLURM_CPUS_PER_TASK
export OMP_PROC_BIND=true

set -e

source ~/Profile/software-intel.sh

cd examples

## Currently, cup-cfd loads a hard-coded json file:
if [ ! -f cupcfd.json ]; then
	ln -s examplejson.json cupcfd.json
fi

srun ../build/cupcfd
