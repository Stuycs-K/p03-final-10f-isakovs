# Dev Log:

## Samuel Isakov

### 2026-01-06 - Writing boilerplate and importing networking.
Class commit: I wrote the boilerplate for the codebase, including the makefile.

Home commits: I imported the server and client handshakes from lab 16. I intend to use these handshakes for more advanced use cases than that seen in lab 16.
Also I did some research and thinking about how a master computer can send commands to nodes, thinking about fork() and execvp().

### 2026-01-07 - Using fork() and execvp() to create nodes.
Class commit: I updated the proposal doc to include select().

Home commits: I made the amount of nodes made to be at the choice of the user, up to 30 nodes.
Each node is created by making a new ssh session and running the node program with the master's IP as an argument.
Also, I implemented select() instead of semaphores to read data from sockets in the correct order and efficiently wait for each socket.
