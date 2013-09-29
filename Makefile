all: paramRiver.c
	gcc -Wall *.h *.c -o main -l pthread -lrt
