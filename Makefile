all: river.c
	gcc -Wall river.c -o main -l pthread -lrt
