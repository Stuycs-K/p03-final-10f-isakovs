# Final Project Proposal

## Group Members:

Samuel Isakov

# Intentions:

I am trying to find a solution to distributed computing and how to split processing across computers in a network.

# Intended usage:

The network will consist of one "Master" computer and one or more "nodes".
The master will be in charge of tasking the nodes with programs to execute and distrubting the workload.
The user will be able to give instructions and view the process on the master computer, and also choose the number of nodes to use.
The goal is to make the distributed workload provide faster processing than on one machine.

# Technical Details:

The master will prompt the user for instructions in the terminal.
The nodes will only display what programs the master tells them to run do on the terminal, for debugging purposes.
Communication will be done through TCP.
Output will be stored in files or printed in terminal.
It is especially important to make use of select() for distributed workloads that will have multiple outputs written to one file.
Nodes will use forking and execvp to run programs tasked by the master.

# Intended Pacing:

(The dates are when the task will be completed)

Jan 7, Wednesday
Confirm that existing infrastructure for simple server-client TCP communication works.
Make the master send a command to the node for them to run a program and its output to be sent back to the master.

Jan 8, Thursday
Make the master send execution instructions to multiple nodes, and their outputs to flow to a file in the correct order using select().
Think about what a program that can be distributed may look like.

Jan 9, Friday
Make a program whose workload can be distributed.
Make the master distribute execution of that program across nodes.

Jan 10-12, Sunday-Monday
See what else has to be done in order to make a working product

Other ideas:
Master is able to distribute any program designed to be distributed.
