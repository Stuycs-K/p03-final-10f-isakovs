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

### 2026-01-08 - Creating a means to test the capability of parallel processing via nodes.
Class commit: I finished the process of spawning nodes on different physical machines. 35 are available.

Home comitts: I made a program to generate a large random integer file of 8 gigabytes in order to use that as testing material.
I also made a program that reads the generated file and prints out the max integer it finds. I also created a means to segment that program into the active nodes. Still experiencing some issues with reading from sockets without blocking execution.

### 2026-01-09 -- 2026-01-11
Overall, I attempted to fix an issue that was ultimately no fault of mine:

The bottleneck to scanning a file appears to be the school network, so the minimal working product is completed, but I need to work on other problems for the computer to solve now.

### 2026-01-12
I have decided that for my next program to test, I will implement Bogosort. I implemented the necessary boilerplate to get Bogosort set up on the serverside.

I am thinking about passing the entire unsorted array through the command line rather than writing to the socket, the server will just read the first socket that is ready.

### 2026-01-13
I spent the day writing Bogosort in node.c, which is where the actual algorithm is placed.

### 2026-01-14
I spent the day writing the part of the code that select()'s every node to find if one has sorted the array correctly, and then causes all other nodes to close.

Bogosort now works. Machine .117 refused ssh, for some reason, but for the most part everything works.

### 2026-01-15
I spent today making two new programs to test

A smaller program is the approximation of PI using the Monte Carlo method, which is not sequential and thus perfect for distribution.

The most complex program I made today is the Mandelbrot Set plotter. It distributes the creation of a mandelbrot set into a .ppm image across nodes.

When the program is distributed, there appear to be minor artifacts in the image, but that is probably the fault of the Network File System. All nodes write to one image file, which may be the culprit.
