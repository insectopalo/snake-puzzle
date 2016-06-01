CODE TO FOLD SNAKE PUZZLES IN 3D LATTICE
========================================

## Synopsis

This code accompanies the paper "Learning Protein Folding and Evolution with a
Snake Puzzle". It includes a program written entirely in C code to fold snake
puzzles through exhaustive search in a 3D lattice (`fold_snake.c`) together
with a number of other scripts to allow further analysis of resulting 3D
structures and visualisation written in Python and R.


## Requirements and Installation

The programs and scripts included in this repository are not intended to be
installed system-wide, but rather to be used as scripts, complementing the text
of the paper and allowing the reproduction of the exercises proposed in the
text.


### Downloading all the code

The code is hosted in a public repository in [github.com](https://github.com/insectopalo/snake-puzzle).
The simplest way to download the code is using `git`, which can be installed
on Debian-based systems with
```bash
sudo apt-get install git
```

If `git` is already installed, change to a directory of choice and clone the repository
```bash
git clone https://github.com/insectopalo/snake-puzzle.git
```


### Compiling the main folding program, `fold_snake.c`

In order to compile `fold_snake.c`, a C compiler should be installed in your
system. In Debian-based Linux distributions (such as Ubuntu), this can be
accomplished via `apt-get` command:
```bash
sudo apt-get install build-essential
```

Once gcc is installed, in order to compile `fold_snake.c`, change to the `src`
directory
```bash
cd src
```
and compile the source
```bash
gcc fold_snake.c -o fold_snake -lm
```

#### The sequence

Snake puzzles have rigid fragments of different lengths stacked one after each
other. This sequence of fragments is fundamental in the folding process since
it dramatically limits the size of the search space.

Within the `src` folder, the file [`config.h`](src/config.h) contains a line
with the sequence to fold. This correspons to the length of the consecutive
rigid (straight) fragments in the snake puzzle. The file contains some example
sequences (commented out) together with the sequence of the particular puzzle
described in the aforementioned publication and is meant to be edited prior to
compiling the source code. In order to fold a different sequence, the
[`fold_snake.c`](src/fold_snake.c) program has to be recompiled after saving
the changes to [`config.h`](src/config.h).

The original sequence for the snake puzzle can also be found in the file
[`input_sequence.txt`](data/input_sequence.txt) in the `data` folder. This
file, however, is only for reference and not used by any of the programs.

### Using `fold_snake`

A new executable file named `fold_snake` should now be present in the `src`
folder. In order to run an exhausive folding search and print the total number
of solutions found,
```bash
./fold_snake
```

The program can be run with a number of options in the command line. These options can be seen by
running
```bash
./fold_snake -h
```
Here is a brief overview thoug.

The `-o <prefix>` flag allows print the coordinates of all the solutions found
to text files. The flag requires an arguments which is used as a prefix for the
output files.
```bash
./fold_snake -o <prefix>
```
prints the coordinates in CSV format (plain text), one per file:
`*prefix*.0001, *prefix*.0002 ...`.

The option `-m` extends the search by including **m**irror solutions
(enantiomers). By default the `-m` flag is unset, so unless provided, the
mirror structures will not be counted/output.

Note that the program by default tries to fold the puzzle within a bounding box
**as smallest as possible**. While not recommended, you can extend the limits
of the bounding cube. This will significantly increase the number of possible
solutions and exponentially expand the search space, which may result in
really, really, (*really*), long runtimes. If you still want to expand the
search bounding **b**ox, use the option `-b <INT>`, where *INT* is a positive
integer (corresponding to the length of the side of the bounding cubic volume).
Providing a value of 0 will not use bounding box at all and allow the snake to
fold also in non-compact structures. Note that the condition of self-avoidance
is never allowed to be violated. The value of -1 as an argument to the `-b`
flag tries to fold the puzzle into the smallest volume that fits it.

It is possible to print the number of contacts between the units of the snake
along the folding process. Use the `-E <file>` option. The required argument
for the flag is the file name where the values are to be output. The output
file format is a CSV with two columns. The first column corresponds to the
number of contacts (non-consecutive units of the puzzle that are in contact in
the 3-dimensional lattice), the second column is the number of puzzle units
that have been already folded. This allows to easily normalise these values if
desired.

The `-n <file>` option prints the search tree in Newick format. The tags in the
leaves indicate the direction taken at each step. For a description of the
format, have a look
[here](http://evolution.genetics.washington.edu/phylip/newicktree.html). The
search trees generated by the search, however, are generally quite big. Keep in
mind that if you want to visualise the Newick text file output you may run into
memory problems.

The `-M <INT>` allows to specify a maximum number of solutions to return. When
the search finds the *INT*-th solution, the program terminates.


#### Examples

```bash
./fold_snake -m -b 4 -o sequence_01 -E energies.txt
```

will search for solutions within a 4x4x4 cube (`-b 4`), including in the
outputs the mirror structures (`-m`). For each solution, a file with the
coordinates of the solution will be created (`-o`) with file names
*sequence_01.0001*, *sequence_01.0002*, ...) and the number of contacts along
the search will be written in the file *energies.txt* (`-E`).

```bash
./fold_snake -b 0 -n tree.dat -M 1
```

will search the tree until the whole puzzle is folded for the first time. Only
that solution will be returned (`-M 1`). The search space will not be limited
to a particular cubic volume (`-b 0`), and hence the only solution returned
will probably be non maximally compact (actually, it will be the maximally
extended!). The number of contacts along the search will be written into the
file *contacts.dat* (`-E`). Since only maximally extended conformation is going
to be returned, the number of contacts will all be zero in the *contacts.dat*
file. Not much of a point but, hey. The coordinates for the solution will not
be written into a file (the `-o` option is not specified).


### Usage of other scripts

A number of other scripts are included in the repository in the `scripts`
folder. Their functions are described in the [README.md](scripts/README.md)
file contained in the `scripts` folder. These scripts are written in Python and
R, which means that the interpreters must be installed in your system for them
to work.


#### Python 2

Most Linux distributions come with Python installed by default. If yours don't,
have a look [here](https://www.python.org/downloads/). If you don't have Linux,
have a look [here](http://www.gnu.org/philosophy/philosophy.en.html).

To execute a Python script:
```bash
python script_name.py [OPTIONS]
```

In general the scripts come with a brief help that can be printed using the
`-h` flag:
```bash
python script_name.py -h
```

Again, have a look at the [README.md](scripts/README.md) file in the *scripts* folder.
#### R

To install R on Debian-based Linux distributions
```
sudo apt-get install r-base
```

If in trouble, have a look
[here](https://cran.r-project.org/doc/manuals/r-release/R-admin.html).


## Contributors

The present implementation has been written mostly by Gonzalo S. Nido, and the
code contained in the `ludo` folder has been written by Ludovica
Bachschmid-Romano. For an old-school Fortran implementation, talk to Alberto
Pascual-Garcia.


## License

This code is licensed by a MIT License (Copyright (c) 2016 Gonzalo S. Nido,
Ludovica Bachschmid-Romano, Ugo Bastolla, Alberto Pascual-Garcia). See the
[LICENSE](LICENSE) file.


