# Ziatest

Ziatest is intended to provide a realistic assessment of
both launch and wireup requirements of MPI applications.
Accordingly, it exercises both the launch system of the environment
and the interconnect subsystem in a specified pattern.

Specifically, the test consists of the following steps:

1. record a time stamp for when the test started -
   this is passed to rank=0 upon launch

2. launch a 100MB executable on a specified number of processes on each node

3. each process executes MPI_Init

4. each process on an odd-numbered node
   (designated the "originator" for purposes of this description)
   sends a one-byte message to the process with the same local rank
   on the even-numbered node above it -
   i.e., a process on node N sends to a process on node N+1, where N is odd.

5. the receiving process answers the message with a one-byte message
   sent back to the original sender. In other words, the test identifies
   pairs of nodes, and then the processes with the same local rank on each
   pair of nodes exchange a one-byte message.

6. each originator records the time that the reply is received,
   and then enters a call to MPI_Gather.
   This allows all the time stamps to be collected by the rank=0 process

7. once all the times stamps have been collected,
   the rank=0 process searches them to find the latest time.
   This marks the ending time of the benchmark.
   The start time is then subtracted from this
   to generate the final time to execute the benchmark.

Thus, the benchmark seeks to measure
not just the time required to spawn processes on remote nodes,
but also the time required by the interconnect
to form inter-process connections capable of communicating.


## Installation

Ziatest has no software prerequisites besides a  MPI library.
This test is included in the OpenMPI developers code base
and was distributed in the OpenMPI 1.5.0 release,
but there is no dependence on OpenMPI;
other MPI implementations can be used with little or no modification.

To install the benchmark,
you will need to compile both the ziatest.c and ziaprobe.c programs.
A very simple Makefile is provided.
The ziatest.c program simply obtains the initial time stamp,
and then executes the "mpirun" (or equivalent) command
to initiate the actual benchmark.


## Execution

With the code compiled, use the command:
```
./ziatest <N> "<mpirun_command> <mpirun_options> "
```
where `N` is the number of processes to be launched on each node,
and `mpirun_command` is the command used to launch parallel jobs (e.g. mpirun),
and `mpirun_options` describes the distribution of processes among nodes.
The syntax for `mpirun_options` differs between MPI implmentations
(or resource managers) and should be modified as needed.

The `run_ziatest.sh` script demonstates use of
the [SLURM](https://slurm.schedmd.com/) `srun` command
to launch 8 processes per node on 6 nodes (48 processes total).
```
./ziatest 8 "srun --ntasks 48 --ntasks-per-node "
```
Notice that tasks_per_node is provided as the first argument to ziatest,
but not within the srun command;
Ziatest appends the tasks_per_node value to the srun command.
Thus, the option that sets the number of tasks per node
(i.e. `--ntasks-per-node`) should be listed last in `mpirun_options`.

There is no requirement on the number of nodes,
nor that there be an even number of nodes.
In the case of an odd number of nodes,
the test will automatically "wrap" the test
by requiring the last node to communicate with node=0.
Note that this can invoke a penalty in performance
as the processes on node=0 will have to respond twice to messages.
Thus, the test does tend to favor even numbers of nodes.
The required behavior is to launch
a constant number of processes on each node.


## Output

The output will appear in the following format:
```
> ./ziatest 6 "srun --ntasks 36 --ntasks-per-node "
srun 	  --ntasks 36 	  --ntasks-per-node  6 ./ziaprobe 1682721242 912291 6
Time test was completed in   0:04 min:sec
Slowest rank: 11
```
The command used to launch `ziaprobe` is printed,
followed by the time required to execute the test,
then the rank that reported the slowest time.
The time will be in `milliseconds` if the test tool less than 1 second to execute,
or `min:sec` if the test took longer than 1 second.
The slowest rank information is provided
in the hopes it may prove of some diagnostic value.

## Required Results

The purpose of Ziatest is to measure the time needed to launch full-system jobs,
and should be run using at least 99.5% of the compute nodes,
and at least MPI rank per NIC.
For systems composed of multiple types of compute nodes,
results should be provided separately for each node type.
It is not necessary to provide results for Ziatest jobs that span multiple node types.


## Reporting

The purpose of Ziatest is to measure the time needed to launch full-system jobs,
and should be run using at least 99.5% of the compute nodes,
and at least MPI rank per NIC.
For systems composed of multiple types of compute nodes,
results should be provided separately for each node type.
It is not necessary to provide results for Ziatest jobs
that span multiple node types.


## Reporting

Benchmark results should include
the job configuration (node type, node count, processes per node)
and a projection of the "Time test was completed in".

For the electronic submission,
include all the source and makefiles used to build on the target platform
and input files and runscripts.
Include all standard output files.



## Copyright
Copyright (c) 2008 Los Alamos National Security, LLC.  All rights reserved.<br>
Modified by Sue Kelly, Sandia National Laboratories, January 2010, January 2012.<br>
Modified by Brian Austin, Lawerence Berkeley National Laboratory, April 2023.

