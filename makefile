.PHONY: compile master node clean
master: progm
	@./progm
node: progn
	@./progn
clean:
	rm -f *.o progm progn
compile progm progn: node.o master.o networking.o
	@gcc -o progm master.o networking.o
	@gcc -o progn node.o networking.o
node.o: node.c networking.h
	@gcc -Wall -c node.c
master.o: master.c networking.h
	@gcc -Wall -c master.c
networking.o: networking.c networking.h
	@gcc -Wall -c networking.c
