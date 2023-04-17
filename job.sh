#!/bin/bash 

# Job script for Ziatest for Cori.

#SBATCH -N 1612
#SBATCH -n 51584
#SBATCH --time 00:10:00
#SBATCH --partition=regular

#yunhe for usgweb
touch $SCRATCH/Cori_Perf/Ziatest-1612/`echo $SLURM_JOBID`

./ziatest 32 "srun -n ${SLURM_NPROCS} --ntasks-per-node "
