# Makefile



# Compiler options

cc = gcc
cflags = -Wall -std=c99



# Recipes

all: ecaTerm ecaVisual

ecaTerm: ecaTerm.o eca.o ecaout.o utils.o
	$(cc) $(cflags) -o ecaTerm ecaTerm.o eca.o ecaout.o utils.o

ecaVisual: ecaVisual.o eca.o utils.o
	$(cc) $(cflags) -o ecaVisual ecaVisual.o eca.o utils.o -lSDL2

ecaTerm.o: ecaTerm.c
	$(cc) $(cflags) -c ecaTerm.c

ecaVisual.o: ecaVisual.c
	$(cc) $(cflags) -c `sdl2-config --cflags --libs` ecaVisual.c

eca.o: eca.c eca.h
	$(cc) $(cflags) -c eca.c

ecaout.o: ecaout.c ecaout.h
	$(cc) $(cflags) -c ecaout.c

utils.o: utils.c utils.h
	$(cc) $(cflags) -c utils.c

.PHONY: clean
clean:
	-rm *.o ecaTerm ecaVisual
