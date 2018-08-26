# Makefile



# Compiler options

compiler = gcc
flags = -Wall -std=c99 -v



# Terminal simulation

termName = ecaTerm
termSrc = ecaTerm.c eca.c ecaout.c utils.c



# SDL visual simulation

visualName = ecaVisual
visualSrc = ecaVisual.c eca.c utils.c



# Compilation

all: $(termName) $(visualName)

$(termName):
	$(compiler) $(flags) -o $(termName) $(termSrc)

$(visualName):
	$(compiler) `sdl2-config --cflags --libs` $(flags) -o $(visualName) $(visualSrc) -lSDL
