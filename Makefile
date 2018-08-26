# Makefile



# Compiler options

cc = gcc
cflags = -Wall -std=c99 -v



# Recipes

all: ecaTerm

ecaTerm: ecaTerm.o eca.o ecaout.o utils.o
	$(cc) $(cflags) -o ecaTerm ecaTerm.o eca.o ecaout.o utils.o

ecaTerm.o: ecaTerm.c eca.h ecaout.h
	$(cc) $(cflags) -c ecaTerm.c

eca.o: eca.c eca.h utils.h
	$(cc) $(cflags) -c eca.c

ecaout.o: ecaout.c eca.h ecaout.h
	$(cc) $(cflags) -c ecaout.c

utils.o: utils.c utils.h
	$(cc) $(cflags) -c utils.c

clean:
	rm *.o ecaTerm
