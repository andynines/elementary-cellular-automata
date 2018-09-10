# Makefile



# Compiler options

cc = gcc
cflags = -Wall -std=c99

.PHONY: all clean purge



# Compilation recipes

all: ecaterm ecavisual

ecaterm: ecaterm.o eca.o ecaio.o allocs.o fracs.o utils.o
	$(cc) $(cflags) -o ecaterm ecaterm.o eca.o ecaio.o allocs.o fracs.o utils.o

ecavisual: ecavisual.o eca.o ecaio.o allocs.o fracs.o utils.o
	$(cc) $(cflags) -o ecavisual ecavisual.o eca.o ecaio.o allocs.o fracs.o utils.o -lSDL2

ecaterm.o: ecaterm.c
	$(cc) $(cflags) -c ecaterm.c

ecavisual.o: ecavisual.c
	$(cc) $(cflags) -c `sdl2-config --cflags --libs` ecavisual.c

eca.o: eca.c eca.h
	$(cc) $(cflags) -c eca.c

ecaio.o: ecaio.c ecaio.h
	$(cc) $(cflags) -c ecaio.c

allocs.o: allocs.c allocs.h
	$(cc) $(cflags) -c allocs.c

fracs.o: fracs.c fracs.h
	$(cc) $(cflags) -c fracs.c

utils.o: utils.c utils.h
	$(cc) $(cflags) -c utils.c



# Utility recipies

test: ecaterm
	./ecaterm 30 80 25 wrap 1 even # Attempting to draw rule 30 to terminal

clean:
	-rm *.o

purge: clean
	rm ecaterm ecavisual
