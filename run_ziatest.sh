#!/bin/bash
#SBATCH -A nstaff
#SBATCH -N 6
#SBATCH -C cpu
#SBATCH -t 00:05:00

tasks_per_node=6
total_tasks=$(( SLURM_JOB_NUM_NODES * tasks_per_node ))

#Notice that tasks_per_node is provided as the first argument to ziatest,
#but not within the srun command;
#Ziatest will append the tasks_per_node value to the srun command

./ziatest $tasks_per_node  \
	  "srun \
	  --ntasks ${total_tasks} \
	  --ntasks-per-node "

