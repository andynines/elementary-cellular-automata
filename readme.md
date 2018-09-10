# Elementary Cellular Automata

This repository consists of libraries making up an elementary cellular automaton simulation engine, and applications that allow you to explore these automata. Read more about them on [Wikipedia](https://en.wikipedia.org/wiki/Elementary_cellular_automaton "Go to Wikipedia.com").



## License

The contents of this repository are made available under the [MIT License](../blob/master/license.txt "Go to full license text").



## Dependencies

The `ecavisual` application requires SDL2. On Debian-like systems, this can be installed using the command:
```
sudo apt install libsdl2-2.0-0
```



## Compilation

Use the included `Makefile` to build the two included applications.
+ `make`, `make all`: Build both applications.
+ `make ecaterm`, `make ecavisual`: Build specific application.
+ `make test`: Do a test run with `ecaterm`. A successful test looks like this:
![Successful test](media/successful-test.png "Successful test")
+ `make clean`: Remove all object files created during the compilation process.
+ `make purge`: Deconstruct the project; remove all object files and executable files.



## Usage

### Argument format

Pass arguments to either executable from the command line. Call one of them without arguments to see this usage message:
```
Usage: ./exec-name rule habitat-width buffer-height boundary-type cells-alive spacing

rule:              integer from 0 to 255
habitat-width:     integer greater than 0
buffer-height:     integer greater than 1
boundary-type:     wrap, dead, alive
cells-alive:       integer from 0 to habitat-width
spacing:           even, random
```

### Examples

![Rule 73](media/example1.png "Rule 73")
The `ecaterm` application draws simulations right to the terminal. Because we specified live borders, the rightmost cell of every generation will act as if there is a live cell to its right, and likewise the leftmost cell of every generation will act as if there is a live cell to its left. We also asked for 20 live cells in the initial configuration, which were positioned chaotically.

![Rule 110](media/example2.png "Rule 110 - Turing complete!")
The `ecavisual` application draws simulations in SDL. Press the spacebar to iterate the simulation by a single time step. Because we asked for even spacing, the simulator will attempt to center and evenly space out the cells of the initial generation as much as possible. For best spacing, use live cell requirements and habitat widths that create reduceable ratios, like 20/100. Ratios such as 37/80 will result in a large clump of cells centered in the initial generation.

![Rule 117](media/example3.png "Rule 117")

![Role 105](media/example4.png "Rule 105")

*Note: The window size and colors used in `ecavisual` simulations are determined by constants inside `ecavisual.c`. The window size has been increased for these screenshots. If you attempt to create a simulation that will not fit inside the window, the program will manually resize the simulation. `ecavisual` has to store all of the `SDL_Rect`s that make up its drawings, and this takes up __a lot of memory__!*



## Todo

+ `ecavisual` is currently a hulking mess in terms of its code and memory consumption. Look into ways to improve this.
+ Document all of the public methods of each library in this readme.
+ Look into procedural music generation; start working on new `ecatunes` app.
