#!/usr/bin/env python
__author__ = "Gonzalo S. Nido"
__copyright__  = "Copyright 2016"
__license__    = "MIT"
__email__      = "gonzalo.s.nido@gmail.com"

"""Convert comma-separated coordinate files to PDB files.

Usage:
    csv2pdb.py <input_files>

Format of input/output files:
    INPUT file(s) are comma-separated with 3 columns: x, y, and z. An arbitrary
    number of input files can be given, also bash glob expressions. Note that
    lines differing from the 3-field format will be skipped. For each CSV input
    an OUTPUT PDB file will be generated. The name of the file will be the same
    as the input with a .pdb extension appended. PDB files can be visualised in
    molecular graphics visualisation programs.
"""

import sys
import re
import getopt
import glob

def usage():
    sys.stderr.write("USAGE: "+sys.argv[0]+" <inputfiles/glob expressions>\n")
    sys.stderr.write("---\n")
    sys.stderr.write(sys.argv[0]+" converts files in comma-separated format " +
                     "(CSV) into PDB files for molecular graphics visualisation software.\n")

def main(argv):
    try:
        opts, args = getopt.getopt(argv,"h")
    except getopt.GetoptError:
        usage()
        sys.exit(2)
    for opt, arg in opts:
        if opt == '-h':
            usage()
            sys.exit()
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
        pdb = open(fname+'.pdb', "w")

        i = 1
        for line in lines:
            coords = map(float, line.split(','))
            if len(coords) != 3:
                sys.stderr.write("Line skipped!\n")
                sys.stderr.write(line)
                continue
            pdb.write( "ATOM     {0:2d}  BOX BOX     1       ".format(i) +
                       "{0:.3f}   {1:.3f}   {2:.3f}\n".format(*coords))
            i = i+1

        for j in range(1,i-1):
            pdb.write( "CONECT   {0:2d}   {1:2d}           \n".format(j,j+1))
        pdb.close()
        sys.stderr.write("CSV={} ".format(fname) +
                         "to PDB={} [ SUCCESS ]\n".format(fname+'.pdb'))

if __name__ == "__main__":
    main(sys.argv[1:])

