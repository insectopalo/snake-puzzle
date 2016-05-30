CODE TO FOLD SNAKE PUZZLES IN 3D LATTICE
========================================

## Synopsis

This code accompanies the paper "Learning Protein Folding and Evolution with a
Snake Puzzle".  It includes a program written entirely in C code to fold snake
puzzles through exhaustive search in a 3D lattice (`fold_snake.c`) together
with a number of other scripts to allow further analysis of resulting 3D
structures and visualisation written in Perl and R.


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
folder. In order to run an exhausive folding search and print the resulting
solutions on screen
```bash
./fold_snake
```

The program has a number of options that can be provided in the command line.
The `-o` flag allows to provide a prefix that will be used as filename to print
the solutions
```bash
./fold_snake -o <file_prefix>
```
will output one solution per file: `<file_prefix>.0001, <file_prefix>.0002 ...`.
In order to print only the number of solutions and skip the coordinate output,
use `-c` (for *c*ount).

The option `-s` will extend the search to include *s*pecular solutions (mirror
images/enantiomers). By default the `-s` flag is unset, so unless provided, the
mirror structures will not be counted/output.

Note that the program by default tries to fold the puzzle within a bounding box
**as smallest as possible**. While not recommended, you can extend the limits
of the bounding box. This will significantly increase the number of possible
solutions and exponentially expand the search space, which may result in a
really, really, (*really*), long run.  If you still want to expand the search
bounding *b*ox, use the flag `-b` followed by a positive integer (corresponding
to the length of the side of the squared box). For an infinite bounding box (no
limits), use `-b 0`.

Finally, it is possible to print the number of contacts between the units of
the snake as the folding process takes place. Use the `-E` flag followed by the
name for the output file.


#### Examples

```bash
./fold_snake -s -b 4 -o sequence_01 -E energies.txt
```

will search for solutions within a 4x4x4 cube (`-b 4`), including in the
outputs the mirror structures (`-s`). For each solution, a file with the
coordinates of the solution will be created (`-o`) with file names
`sequence_01.0001`, `sequence_01.0002`, ...) and the number of contacts along
the search will be written in the file `energies.txt` (`-E`).

```bash
./fold_snake -c -b 0 -E contacts.dat
```

will search for all the possible non-overlapping paths and print the number of
solutions (`-c`). The search will not be limited to any cubic volume (`-b 0`).
The number of contacts along the search will be written into the file
`contacts.dat` (`-E`).  The coordinates for the solutions will not be written
into files nor stdout.


### Usage of other scripts

A number of other scripts are included in the repository in the `scripts`
folder. Their functions are described in the [README.md](scripts/README.md)
file contained in the `scripts` folder.  These scripts are written in Perl and
R, which means that the interpreters must be installed in your system for them
to work.


#### Perl

Most Linux distributions come with Perl installed by default. If yours don't,
have a look [here](http://learn.perl.org/installing/). If you don't have Linux,
have a look [here](http://www.gnu.org/philosophy/philosophy.en.html).

To execute a perl script:
```bash
perl script_name.pl [OPTIONS]
```

In general the scripts come with a brief help that can be printed using the
`-h` flag:
```bash
perl script_name.pl -h
```


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
Bachschmid-Romano.  For an old-school Fortran implementation, talk to Alberto
Pascual-Garcia.


## License

This code is licensed by a MIT License (Copyright (c) 2016 Gonzalo S. Nido,
Ludovica Bachschmid-Romano, Ugo Bastolla, Alberto Pascual-Garcia). See the
[LICENSE](LICENSE) file.


