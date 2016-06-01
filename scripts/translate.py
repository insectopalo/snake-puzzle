#!/usr/bin/env python
__author__ = "Gonzalo S. Nido"
__copyright__  = "Copyright 2016"
__license__    = "MIT"
__email__      = "gonzalo.s.nido@gmail.com"

"""Translate CSV coordinate file structure to origin of cartesian coordinates
(0,0,0).

Usage:
    translate.py <input_files>

Format of input/output files:
    INPUT file(s) are comma-separated with 3 columns: x, y, and z. An arbitrary
    number of input files can be given, also bash glob expressions. Note that
    lines differing from the 3-field format will be skipped. For each CSV input
    an OUTPUT CSV file will be generated. The name of the file will be the
    same as the input with a .ori extension appended.
"""

#import re
import getopt
import sys
import glob
import os

def usage():
    sys.stderr.write("USAGE: "+sys.argv[0]+" <inputfiles>\n")
    sys.stderr.write("---\n")
    sys.stderr.write(sys.argv[0]+" translates the coordinates to start in the " +
                     "cartesian coordinates origin and the elements only " +
                     "occupy positive values.\n")

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

        maxs = map(float, lines[0].split(','))
        mins = map(float, lines[0].split(','))

        for line in lines:
            coords_tmp = map(float, line.split(','))
            if len(coords_tmp) != 3:
                sys.stderr.write("Line skipped!\n")
                sys.stderr.write(line)
                continue
            for i in range(3):
                if coords_tmp[i] > maxs[i]:
                    maxs[i] = coords_tmp[i]
                if coords_tmp[i] < mins[i]:
                    mins[i] = coords_tmp[i]

        outfile = open(fname+".ori", "w")
        for line in lines:
            coords_tmp = map(float, line.split(','))
            if len(coords_tmp) != 3:
                continue
            new_coords = list()
            for c,m in zip(coords_tmp,mins):
                new_coords.append(c-m)
            outfile.write(",".join(map(str, new_coords))+"\n")

        outfile.close()

if __name__ == "__main__":
    main(sys.argv[1:])
