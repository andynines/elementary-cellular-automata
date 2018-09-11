# Elementary Cellular Automata



This project simulates and creates visual representations of the elementary cellular automata. Read more about these cellular automata on [Wikipedia](https://en.wikipedia.org/wiki/Elementary_cellular_automaton "Go to Wikipedia.com").



## License

The contents of this repository are made available under the [MIT License](../license.txt "Go to full license text").



## Dependencies

The `ecavisual` application requires SDL2. All other dependencies are part of the C standard library.



## Compilation

Use the included `Makefile` to build the two applications.
+ `make`: Build both applications.
+ `make ecaterm`, `make ecavisual`: Build specific application.
+ `make test`: Do a test run with `ecaterm`. A successful test looks like this:
![alt text](../blob/master/media/successful-test.png "Successful test")
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
+ **Rule**: A number in the range from 0 to 255 in Wolfram's numbering scheme. Determines Determines the state of a cell during the next time step based on the states of itself and its two immediate neighbors.
+ **Habitat width**: The number of cell spaces available in a single generation.
+ **Buffer height**: The maximum number of generations stored in memory at a given time.
+ **Boundary type**: Determines how the leftmost and rightmost cells in a generation react to their left and right neighbors, respectively. For instance, when this option is set to "dead," the leftmost cell will behave as if there is a dead cell to its left. "Wrap" is, however, the most natural option. This is because the elementary cellular automata would ideally be simulated in an infinitely wide habitat. During the first instant that an automaton wraps, the design it creates will become unnaturally periodic. (*However, this period could still be very long.*)
+ **Cells alive**: The number of cells in the initial generation that will be alive.
+ **Spacing**: How the alive cells of the initial generation are spaced. If set to even, the alive cells will have an even distance between them and be centered as much as possible. This option works best when *cells alive* and *habitat width* create a reduceable ratio. If set to "random," the spacing will instead be chaotic.

### Examples

![alt text](../blob/master/media/example1.png "Rule 73")

![alt text](../blob/master/media/example2.png "Rule 110 - Turing complete!")

![alt text](../blob/master/media/example3.png "Rule 117")

![alt text](../blob/master/media/example4.png "Rule 105")



## Documentation

The following is a brief documentation of the public methods in each of this repository's libraries.

### eca.c

```c
Simulation\* createSim(int rule, int habitatSize, int genBufferSize, BoundaryCode borderType, ConfigCode initCode)
```
Given a rule from Wolfram's numbering scheme, a number of cells belonging to a 
single generation, a number of generations to store on memory, a method of
interpreting the neighborhoods of edge cells, and a code specifying how the
initial generation should be populated, build a simulation according to these
attributes and return a pointer to it.

```c
bool getCellState(CellBlock* intlBlockPtr, int cellIndex)
```
Locate a particular cell bit based off of a pointer to its generation's memory
block and its index relative to that entire generation. Return a boolean
representing the cell's state; live cells return true and dead cells return
false.

```c
void iterateSim(Simulation* simPtr, int iterations)
```
Knowing the latest states of each cell in a simulation, calculate their next
state. Place the resulting new generation in the buffer, deleting the oldest
generation in the buffer if necassary in order to make room.
   
```c
void destroySim(Simulation* simPtr)
```
Remove a simulation's data from memory using its pointer.



## Todo

+ `ecavisual` is currently a hulking mess in terms of its code and memory consumption. Look into ways to improve this.
+ Finish the documentation.
+ Organize the source code into folders and spiffy up the Makefile.
