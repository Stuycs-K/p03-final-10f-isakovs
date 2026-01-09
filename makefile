.PHONY: compile master node clean
master: progm
	@./progm
intgen: progr
	@./progintgen
clean:
	rm -f *.o prog* random_numbers.txt
compile progm progn: node.o master.o networking.o randintgen.o max.o
	@gcc -o progm master.o networking.o
	@gcc -o progn node.o networking.o
	@gcc -o progintgen randintgen.o
	@gcc -o progmax max.o 
node.o: node.c networking.h
	@gcc -Wall -c node.c
master.o: master.c networking.h
	@gcc -Wall -c master.c
networking.o: networking.c networking.h
	@gcc -Wall -c networking.c
randintgen.o: randintgen.c
	@gcc -Wall -c randintgen.c
max.o: max.c
	@gcc -Wall -c max.c
