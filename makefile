# name: 		Hong Chen
# ONE Card number: 	1386152
# Unix id: 		chen1

CC := gcc

Factory: factory.o
	$(CC) -Wall -std=c99 -pthread -o Factory factory.o -lm

factory.o: factory.c factory.h
	$(CC) -Wall -std=c99 -pthread -c -g factory.c

clean:
	-rm -f *.o Factory core
