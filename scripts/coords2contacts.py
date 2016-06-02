#!/usr/bin/env python
__author__     = "Gonzalo S. Nido"
__copyright__  = "Copyright 2016"
__license__    = "MIT"
__email__      = "gonzalo.s.nido@gmail.com"

"""Generate contact matrix from coordinate file structure in CSV format.

Usage:
    coords2contacts.py <input_files>

Format of input/output files:
    INPUT file(s) are comma-separated with 3 columns: x, y, and z. An arbitrary
    number of input files can be given, also bash glob expressions. Note that
    lines differing from the 3-field format will be skipped. For each CSV
    input, the contact binary matrix will be output in comma-separated format
    (CSV). Note that consecutive units are not counted as "in contact". The
    name of the file will be the same as the input with a .contacts extension
    appended.
"""

import getopt
import sys
import glob
import os

def usage():
    sys.stderr.write("USAGE: "+sys.argv[0]+" <inputfiles>\n")
    sys.stderr.write("---\n")
    sys.stderr.write(sys.argv[0]+" generates contact matrix from structure in " +
                     "cartesian coordinates\n")

def main(argv):
    try:
        opts, args = getopt.getopt(argv,"h")
    except getopt.GetoptError:
        usage()
        sys.exit(2)
    for opt, arg in opts:
        if opt == '-h':
            usage()
            sys.exit(2)
    if len(args) < 1:
        usage()
        sys.exit(2)
    
    files = list()
    for g in args:
        files.extend(glob.glob(g))

    for fname in files:
        f = open(fname)
        lines = f.readlines()
        f.close()

        coords = list()
        for line in lines:
            coords_tmp = map(float, line.split(','))
            if len(coords_tmp) != 3:
                sys.stderr.write("Line skipped!\n")
                sys.stderr.write(line)
                continue
            coords.append(coords_tmp)

        contacts = [[0 for i in range(len(coords))] for j in range(len(coords))]

        for i in range(len(coords)):
            for j in range(len(coords)):
                distances = list()
                for p_a, p_b in zip(coords[i],coords[j]):
                    distances.append(abs(p_a-p_b))
                if sum(distances) == 1 and abs(i-j) > 1:
                    contacts[i][j] = 1

        outfile = open(fname+".contacts", "w")

        for i in range(len(coords)):
            outfile.write(",".join(map(str, contacts[i]))+"\n")

        outfile.close()

if __name__ == "__main__":
    main(sys.argv[1:])

