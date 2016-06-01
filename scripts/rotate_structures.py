#!/usr/bin/env python
__author__     = "Gonzalo S. Nido"
__copyright__  = "Copyright 2016"
__license__    = "MIT"
__email__      = "gonzalo.s.nido@gmail.com"

"""Generate all the possible rotations of a structure.

Usage:
    rotate_structures.py <input_files>

Format of input/output files:
    INPUT file(s) are comma-separated with 3 columns: x, y, and z. An arbitrary
    number of input files can be given, also bash glob expressions. Note that
    lines differing from the 3-field format will be skipped. For each CSV
    input, a total of 24 files will be printed, each with a rotation of the
    structure. The name of the files will the input file name as a prefix, with a
    number appended.
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

        X = list()
        Y = list()
        Z = list()
        for line in lines:
            coords_tmp = map(float, line.split(','))
            if len(coords_tmp) != 3:
                sys.stderr.write("Line skipped!\n")
                sys.stderr.write(line)
                continue
            X.append(coords_tmp[0])
            Y.append(coords_tmp[1])
            Z.append(coords_tmp[2])

        # R1
        outfile = open(fname+".r01", "w")
        j = 0
        for x in X:
            outfile.write("{x},{y},{z}\n".format(x=X[j], y=Y[j], z=Z[j]))
            j += 1
        outfile.close()

        tmp = 0

        # R2, R3, R4
        for i in range(2,5):
            outfile = open(fname+".r{:02}".format(i), "w")
            j = 0
            for x in X:
                tmp  = X[j]
                X[j] = Y[j]*(-1)+3
                Y[j] = tmp
                outfile.write("{x},{y},{z}\n".format(x=X[j], y=Y[j], z=Z[j]))
                j += 1
            outfile.close()
            
        # R5, R6, R7
        for i in range(5,8):
            outfile = open(fname+".r{:02}".format(i), "w")
            j = 0
            for y in Y:
                tmp  = Y[j]
                Y[j] = Z[j]*(-1)+3
                Z[j] = tmp
                outfile.write("{x},{y},{z}\n".format(x=X[j], y=Y[j], z=Z[j]))
                j += 1
            outfile.close()
            
        # R8, R9, R10
        for i in range(8,11):
            outfile = open(fname+".r{:02}".format(i), "w")
            j = 0
            for x in X:
                tmp  = X[j]
                X[j] = Y[j]*(-1)+3
                Y[j] = tmp
                outfile.write("{x},{y},{z}\n".format(x=X[j], y=Y[j], z=Z[j]))
                j += 1
            outfile.close()

        # R11, R12, R13
        for i in range(11,14):
            outfile = open(fname+".r{:02}".format(i), "w")
            j = 0
            for y in Y:
                tmp  = Y[j]
                Y[j] = Z[j]*(-1)+3
                Z[j] = tmp
                outfile.write("{x},{y},{z}\n".format(x=X[j], y=Y[j], z=Z[j]))
                j += 1
            outfile.close()

        # R14, R15, R16
        for i in range(14,17):
            outfile = open(fname+".r{:02}".format(i), "w")
            j = 0
            for x in X:
                tmp  = X[j]
                X[j] = Y[j]*(-1)+3
                Y[j] = tmp
                outfile.write("{x},{y},{z}\n".format(x=X[j], y=Y[j], z=Z[j]))
                j += 1
            outfile.close()

        # R17, R18
        for i in range(17,19):
            outfile = open(fname+".r{:02}".format(i), "w")
            j = 0
            for y in Y:
                tmp  = Y[j]
                Y[j] = Z[j]*(-1)+3
                Z[j] = tmp
                outfile.write("{x},{y},{z}\n".format(x=X[j], y=Y[j], z=Z[j]))
                j += 1
            outfile.close()

        # R19, R20
        for i in range(19,21):
            outfile = open(fname+".r{:02}".format(i), "w")
            j = 0
            for x in X:
                tmp  = X[j]
                X[j] = Y[j]*(-1)+3
                Y[j] = tmp
                outfile.write("{x},{y},{z}\n".format(x=X[j], y=Y[j], z=Z[j]))
                j += 1
            outfile.close()

        # R21
        i = 21
        outfile = open(fname+".r{:02}".format(i), "w")
        j = 0
        for y in Y:
            tmp  = Y[j]
            Y[j] = Z[j]*(-1)+3
            Z[j] = tmp
            outfile.write("{x},{y},{z}\n".format(x=X[j], y=Y[j], z=Z[j]))
            j += 1
        outfile.close()

        # R22
        i = 22
        outfile = open(fname+".r{:02}".format(i), "w")
        j = 0
        for x in X:
            tmp  = X[j]
            X[j] = Y[j]*(-1)+3
            Y[j] = tmp
            outfile.write("{x},{y},{z}\n".format(x=X[j], y=Y[j], z=Z[j]))
            j += 1
        outfile.close()

        # R23
        i = 23
        outfile = open(fname+".r{:02}".format(i), "w")
        j = 0
        for x in X:
            tmp  = Y[j]
            Y[j]= Z[j]*(-1)+3
            Z[j]= tmp
            outfile.write("{x},{y},{z}\n".format(x=X[j], y=Y[j], z=Z[j]))
            j += 1
        outfile.close()

        # R24
        i = 24
        outfile = open(fname+".r{:02}".format(i), "w")
        j = 0
        for x in X:
            tmp = X[j]
            X[j]= Y[j]*(-1)+3
            Y[j]= tmp
            outfile.write("{x},{y},{z}\n".format(x=X[j], y=Y[j], z=Z[j]))
            j += 1
        outfile.close()


if __name__ == "__main__":
    main(sys.argv[1:])
