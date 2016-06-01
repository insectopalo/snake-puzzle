## Python scripts


### `csv2pdb.py`: Convert comma-separated coordinate files to PDB files

#### Usage

```bash
csv2pdb.py <input_files>
```
The *input_files* can be a list of files, a pattern according to the rules used
by the Unix shell, or a multiple combination of both. For example:
```bash
csv2pdb.py output.* file01.csv 
```
is a valid syntax.


#### Format of input/output files

The coordinates in the fields of he input files have to be comma-separated: x,
y, and z. Lines differing from the 3-field format will be skipped. For each
input file, an output PDB file will be generated. The name of the file will be
the same as the input with a *.pdb* extension appended. PDB files can be
inspected using freely available molecular graphics visualisation programs such
as [pymol](http://www.pymol.org).

The `-o` option from *fold_snake* program generates the files in the format
accepted by this script, so no further format tinkering is needed.


### `translate.py`: Translate CSV coordinate file structure to origin

The script translates the coordinates of the folded structure to the
origin of the cartesian coordinates (0, 0, 0) and ensures that only
positive directions are taken. If a set of maximally compact structures
is processed by the script, all the resulting structures will occupy the same
coordinates.


#### Usage

```bash
translate.py <input_files>
```
The *input_files* can be a list of files, a pattern according to the rules used
by the Unix shell, or a multiple combination of both (like for the previous
script).


#### Format of input/output files
For each input file, an output comma-separated value file will be generated.
The name of the file will be the same as the input with a *.ori* extension
appended.


### `coords2contacts.py`: Generate contact matrix from coordinate file structure in CSV format

This script generates binary matrices from folded structures.


#### Usage

```bash
coords2contacts.py <input_files>
```


#### Format of input/output files

Same input format as the previous scripts. For each input, a symmetric contact
matrix will be output in comma-separated value format. Note that consecutive
units are not counted as "in contact". The name of the file will be the same as
the input with a *.contacts* extension appended.


### `rotate_structures.py`: Generate all the possible rotations of a structure

This script generates all the 24 non-superposable rotations in the discrete cartesian
coordinate of a given structure(s).


#### Usage

```bash
rotate_structures.py <input_files>
```

#### Format of input/output files

The input is specified as in the previous scripts. For each input filename, a
total of 24 files are output (also comma-separated value format) with the
coordinates of each rotated structure. The name of the output files are the
same as the input, with a *.rXX* appended, where *XX* is a number between 1 and
24 (zero-padded).

