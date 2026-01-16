.PHONY: compile master node clean
run: progm
	@./progm
master: progm
	@./progm
intgen: progintgen
	@./progintgen
clean:
	rm -f *.o prog* *.ppm
compile progm progn progintgen progmax: node.o master.o networking.o intgen.o max.o
	@gcc -o progm master.o networking.o
	@gcc -o progn node.o networking.o
	@gcc -o progintgen intgen.o
	@gcc -o progmax max.o 
node.o: node.c networking.h
	@gcc -Wall -c node.c
master.o: master.c networking.h
	@gcc -Wall -c master.c
networking.o: networking.c networking.h
	@gcc -Wall -c networking.c
intgen.o: intgen.c
	@gcc -Wall -c intgen.c
max.o: max.c
	@gcc -Wall -c max.c
