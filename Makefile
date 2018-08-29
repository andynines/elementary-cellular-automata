# Makefile



# Compiler options

cc = gcc
cflags = -Wall -std=c99



# Recipes

all: ecaterm ecavisual

ecaterm: ecaterm.o eca.o ecaout.o ecaargs.o fracs.o utils.o
	$(cc) $(cflags) -o ecaterm ecaterm.o eca.o ecaout.o ecaargs.o fracs.o utils.o

ecavisual: ecavisual.o eca.o ecaargs.o fracs.o utils.o
	$(cc) $(cflags) -o ecavisual ecavisual.o eca.o ecaargs.o fracs.o utils.o -lSDL2

ecaterm.o: ecaterm.c
	$(cc) $(cflags) -c ecaterm.c

ecavisual.o: ecavisual.c
	$(cc) $(cflags) -c `sdl2-config --cflags --libs` ecavisual.c

eca.o: eca.c eca.h
	$(cc) $(cflags) -c eca.c

ecaout.o: ecaout.c ecaout.h
	$(cc) $(cflags) -c ecaout.c

ecaargs.o: ecaargs.c ecaargs.h
	$(cc) $(cflags) -c ecaargs.c

fracs.o: fracs.c fracs.h
	$(cc) $(cflags) -c fracs.c

utils.o: utils.c utils.h
	$(cc) $(cflags) -c utils.c

.PHONY: clean
clean:
	-rm *.o ecaterm ecavisual
