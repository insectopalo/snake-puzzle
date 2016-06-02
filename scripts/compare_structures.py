#!/usr/bin/env python
__author__ = "Gonzalo S. Nido"
__copyright__  = "Copyright 2016"
__license__    = "MIT"
__email__      = "gonzalo.s.nido@gmail.com"

"""Check whether CSV coordinate structures are superimposable.

Usage:
    program.py <input_files> 

Format of input/output files:
    INPUT file(s) can are comma-separated value files with 3 columns: x, y, and
    z.  An arbitrary number of input files can bie provided, also linux shell
    glob expression and any combination of both. The input files will be
    compared pair-wise.
    The OUTPUT...

"""

import getopt
import sys
import glob
import os

def usage():
    sys.stderr.write("USAGE: "+sys.argv[0]+" <inputfiles>\n")
    sys.stderr.write("---\n")
    sys.stderr.write(sys.argv[0]+" checks if input files correspond to " +
                     "identical superimposable structures. Any number of inputs " +
                     "or glob linux shell expressions allowed. For example:\n" + 
                     sys.argv[0]+" *.csv /home/gon/structures/file01.csv " +
                     "anotherfile.csv /tmp/[0-9].csv\n")

def check_structures(str1, str2):
    for p1, p2 in zip(str1, str2):
        for c1, c2 in zip(p1, p2):
            if c1 != c2:
                return 0
    return 1

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

    for i in range(len(files)):
        for j in range(i+1,len(files)):
            f1 = open(files[i])
            f2 = open(files[j])
            lines1 = f1.readlines()
            lines2 = f2.readlines()
            f1.close()
            f2.close()

            coords1 = list()
            coords2 = list()
            maxs1 = map(float, lines1[0].split(','))
            mins1 = map(float, lines1[0].split(','))
            maxs2 = map(float, lines2[0].split(','))
            mins2 = map(float, lines2[0].split(','))
            for line in lines1:
                coords_tmp = map(float, line.split(','))
                if len(coords_tmp) != 3:
                    sys.stderr.write("Line skipped!\n")
                    sys.stderr.write(line)
                    continue
                coords1.append(coords_tmp)
                for p in range(3):
                    if coords_tmp[p] > maxs1[p]:
                        maxs1[p] = coords_tmp[p]
                    if coords_tmp[p] < mins1[p]:
                        mins1[p] = coords_tmp[p]
            for line in lines2:
                coords_tmp = map(float, line.split(','))
                if len(coords_tmp) != 3:
                    sys.stderr.write("Line skipped!\n")
                    sys.stderr.write(line)
                    continue
                coords2.append(coords_tmp)
                for p in range(3):
                    if coords_tmp[p] > maxs2[p]:
                        maxs2[p] = coords_tmp[p]
                    if coords_tmp[p] < mins2[p]:
                        mins2[p] = coords_tmp[p]

            # Check lengths
            if len(coords1) != len(coords2):
                continue

            # Check volumes of boxes
            r1 = list()
            r2 = list()
            for m,M in zip(mins1,maxs1):
                r1.append(M-m)
            for m,M in zip(mins2,maxs2):
                r2.append(M-m)
            if r1[0]*r1[1]*r1[2] != r2[0]*r2[1]*r2[2]:
                continue
            
            # Translate to 0,0,0
            new_coords1 = list()
            new_coords2 = list()
            for coord in coords1:
                new_c = list()
                for c,m in zip(coord,mins1):
                    new_c.append(c-m)
                new_coords1.append(new_c)
            for coord in coords2:
                new_c = list()
                for c,m in zip(coord,mins2):
                    new_c.append(c-m)
                new_coords2.append(new_c)

            # Generate all rotations for structure 1 and compare to structure 2

            # R1
            if check_structures(new_coords1, new_coords2):
                sys.stdout.write("{file1} and {file2} ARE EQUAL\n".format(file1=files[i], file2=files[j]))
                continue

            equal = 0
            #rot_coords1 = list()
            
            # R2, R3, R4
            for p in range(3):
                q = 0
                for c in new_coords1:
                    tmp  = new_coords1[q][0]
                    new_coords1[q][0] = new_coords1[q][1]*(-1)+3
                    new_coords1[q][1] = tmp
                    if q == 0:
                        mins1 = c
                    else:
                        if new_coords1[0] < mins1[0]: mins1[0] = new_coords1[0]
                        if new_coords1[1] < mins1[1]: mins1[1] = new_coords1[1]
                        if new_coords1[2] < mins1[2]: mins1[2] = new_coords1[2]
                    q += 1
                # Translate to 0,0,0
                q = 0
                for c in new_coords1:
                    new_coords1[q][0] - mins1[0]
                    new_coords1[q][1] - mins1[1]
                    new_coords1[q][2] - mins1[2]
                    q += 1
                if check_structures(new_coords1, new_coords2):
                    sys.stdout.write("{file1} and {file2} ARE EQUAL\n".format(file1=files[i], file2=files[j]))
                    equal = 1
                    break
            if equal == 1:
                continue
                
            # R5, R6, R7
            for p in range(3):
                q = 0
                for c in new_coords1:
                    tmp  = new_coords1[q][1]
                    new_coords1[q][1] = new_coords1[q][2]*(-1)+3
                    new_coords1[q][2] = tmp
                    if q == 0:
                        mins1 = c
                    else:
                        if new_coords1[0] < mins1[0]: mins1[0] = new_coords1[0]
                        if new_coords1[1] < mins1[1]: mins1[1] = new_coords1[1]
                        if new_coords1[2] < mins1[2]: mins1[2] = new_coords1[2]
                    q += 1
                # Translate to 0,0,0
                q = 0
                for c in new_coords1:
                    new_coords1[q][0] - mins1[0]
                    new_coords1[q][1] - mins1[1]
                    new_coords1[q][2] - mins1[2]
                    q += 1
                if check_structures(new_coords1, new_coords2):
                    sys.stdout.write("{file1} and {file2} ARE EQUAL\n".format(file1=files[i], file2=files[j]))
                    equal = 1
                    break
            if equal == 1:
                continue
                
            # R8, R9, R10
            for p in range(3):
                q = 0
                for c in new_coords1:
                    tmp  = new_coords1[q][0]
                    new_coords1[q][0] = new_coords1[q][1]*(-1)+3
                    new_coords1[q][1] = tmp
                    if q == 0:
                        mins1 = c
                    else:
                        if new_coords1[0] < mins1[0]: mins1[0] = new_coords1[0]
                        if new_coords1[1] < mins1[1]: mins1[1] = new_coords1[1]
                        if new_coords1[2] < mins1[2]: mins1[2] = new_coords1[2]
                    q += 1
                # Translate to 0,0,0
                q = 0
                for c in new_coords1:
                    new_coords1[q][0] - mins1[0]
                    new_coords1[q][1] - mins1[1]
                    new_coords1[q][2] - mins1[2]
                    q += 1
                if check_structures(new_coords1, new_coords2):
                    sys.stdout.write("{file1} and {file2} ARE EQUAL\n".format(file1=files[i], file2=files[j]))
                    equal = 1
                    break
            if equal == 1:
                continue

            # R11, R12, R13
            for p in range(3):
                q = 0
                for c in new_coords1:
                    tmp  = new_coords1[q][1]
                    new_coords1[q][1] = new_coords1[q][2]*(-1)+3
                    new_coords1[q][2] = tmp
                    if q == 0:
                        mins1 = c
                    else:
                        if new_coords1[0] < mins1[0]: mins1[0] = new_coords1[0]
                        if new_coords1[1] < mins1[1]: mins1[1] = new_coords1[1]
                        if new_coords1[2] < mins1[2]: mins1[2] = new_coords1[2]
                    q += 1
                # Translate to 0,0,0
                q = 0
                for c in new_coords1:
                    new_coords1[q][0] - mins1[0]
                    new_coords1[q][1] - mins1[1]
                    new_coords1[q][2] - mins1[2]
                    q += 1
                if check_structures(new_coords1, new_coords2):
                    sys.stdout.write("{file1} and {file2} ARE EQUAL\n".format(file1=files[i], file2=files[j]))
                    equal = 1
                    break
            if equal == 1:
                continue

            # R14, R15, R16
            for p in range(2):
                q = 0
                for c in new_coords1:
                    tmp  = new_coords1[q][0]
                    new_coords1[q][0] = new_coords1[q][1]*(-1)+3
                    new_coords1[q][1] = tmp
                    if q == 0:
                        mins1 = c
                    else:
                        if new_coords1[0] < mins1[0]: mins1[0] = new_coords1[0]
                        if new_coords1[1] < mins1[1]: mins1[1] = new_coords1[1]
                        if new_coords1[2] < mins1[2]: mins1[2] = new_coords1[2]
                    q += 1
                # Translate to 0,0,0
                q = 0
                for c in new_coords1:
                    new_coords1[q][0] - mins1[0]
                    new_coords1[q][1] - mins1[1]
                    new_coords1[q][2] - mins1[2]
                    q += 1
                if check_structures(new_coords1, new_coords2):
                    sys.stdout.write("{file1} and {file2} ARE EQUAL\n".format(file1=files[i], file2=files[j]))
                    equal = 1
                    break
            if equal == 1:
                continue

            # R17, R18
            for p in range(2):
                q = 0
                for c in new_coords1:
                    tmp  = new_coords1[q][1]
                    new_coords1[q][1] = new_coords1[q][2]*(-1)+3
                    new_coords1[q][2] = tmp
                    if q == 0:
                        mins1 = c
                    else:
                        if new_coords1[0] < mins1[0]: mins1[0] = new_coords1[0]
                        if new_coords1[1] < mins1[1]: mins1[1] = new_coords1[1]
                        if new_coords1[2] < mins1[2]: mins1[2] = new_coords1[2]
                    q += 1
                # Translate to 0,0,0
                q = 0
                for c in new_coords1:
                    new_coords1[q][0] - mins1[0]
                    new_coords1[q][1] - mins1[1]
                    new_coords1[q][2] - mins1[2]
                    q += 1
                if check_structures(new_coords1, new_coords2):
                    sys.stdout.write("{file1} and {file2} ARE EQUAL\n".format(file1=files[i], file2=files[j]))
                    equal = 1
                    break
            if equal == 1:
                continue

            # R19, R20
            for p in range(2):
                q = 0
                for c in new_coords1:
                    tmp  = new_coords1[q][0]
                    new_coords1[q][0] = new_coords1[q][1]*(-1)+3
                    new_coords1[q][1] = tmp
                    if q == 0:
                        mins1 = c
                    else:
                        if new_coords1[0] < mins1[0]: mins1[0] = new_coords1[0]
                        if new_coords1[1] < mins1[1]: mins1[1] = new_coords1[1]
                        if new_coords1[2] < mins1[2]: mins1[2] = new_coords1[2]
                    q += 1
                # Translate to 0,0,0
                q = 0
                for c in new_coords1:
                    new_coords1[q][0] - mins1[0]
                    new_coords1[q][1] - mins1[1]
                    new_coords1[q][2] - mins1[2]
                    q += 1
                if check_structures(new_coords1, new_coords2):
                    sys.stdout.write("{file1} and {file2} ARE EQUAL\n".format(file1=files[i], file2=files[j]))
                    equal = 1
                    break
            if equal == 1:
                continue

            # R21
            p = 21
            q = 0
            for c in new_coords1:
                tmp  = new_coords1[q][1]
                new_coords1[q][1] = new_coords1[q][2]*(-1)+3
                new_coords1[q][2] = tmp
                if q == 0:
                    mins1 = c
                else:
                    if new_coords1[0] < mins1[0]: mins1[0] = new_coords1[0]
                    if new_coords1[1] < mins1[1]: mins1[1] = new_coords1[1]
                    if new_coords1[2] < mins1[2]: mins1[2] = new_coords1[2]
                q += 1
            # Translate to 0,0,0
            q = 0
            for c in new_coords1:
                new_coords1[q][0] - mins1[0]
                new_coords1[q][1] - mins1[1]
                new_coords1[q][2] - mins1[2]
                q += 1
            if check_structures(new_coords1, new_coords2):
                sys.stdout.write("{file1} and {file2} ARE EQUAL\n".format(file1=files[i], file2=files[j]))
                continue

            # R22
            p = 22
            q = 0
            for c in new_coords1:
                tmp  = new_coords1[q][0]
                new_coords1[q][0] = new_coords1[q][1]*(-1)+3
                new_coords1[q][1] = tmp
                if q == 0:
                    mins1 = c
                else:
                    if new_coords1[0] < mins1[0]: mins1[0] = new_coords1[0]
                    if new_coords1[1] < mins1[1]: mins1[1] = new_coords1[1]
                    if new_coords1[2] < mins1[2]: mins1[2] = new_coords1[2]
                q += 1
            # Translate to 0,0,0
            q = 0
            for c in new_coords1:
                new_coords1[q][0] - mins1[0]
                new_coords1[q][1] - mins1[1]
                new_coords1[q][2] - mins1[2]
                q += 1
            if check_structures(new_coords1, new_coords2):
                sys.stdout.write("{file1} and {file2} ARE EQUAL\n".format(file1=files[i], file2=files[j]))
                continue

            # R23
            p = 23
            q = 0
            for c in new_coords1:
                tmp  = new_coords1[q][1]
                new_coords1[q][1] = new_coords1[q][2]*(-1)+3
                new_coords1[q][2] = tmp
                if q == 0:
                    mins1 = c
                else:
                    if new_coords1[0] < mins1[0]: mins1[0] = new_coords1[0]
                    if new_coords1[1] < mins1[1]: mins1[1] = new_coords1[1]
                    if new_coords1[2] < mins1[2]: mins1[2] = new_coords1[2]
                q += 1
            # Translate to 0,0,0
            q = 0
            for c in new_coords1:
                new_coords1[q][0] - mins1[0]
                new_coords1[q][1] - mins1[1]
                new_coords1[q][2] - mins1[2]
                q += 1
            if check_structures(new_coords1, new_coords2):
                sys.stdout.write("{file1} and {file2} ARE EQUAL\n".format(file1=files[i], file2=files[j]))
                continue

            # R24
            p = 24
            q = 0
            for c in new_coords1:
                tmp = new_coords1[q][0]
                new_coords1[q][0]= new_coords1[q][1]*(-1)+3
                new_coords1[q][1]= tmp
                if q == 0:
                    mins1 = c
                else:
                    if new_coords1[0] < mins1[0]: mins1[0] = new_coords1[0]
                    if new_coords1[1] < mins1[1]: mins1[1] = new_coords1[1]
                    if new_coords1[2] < mins1[2]: mins1[2] = new_coords1[2]
                q += 1
            # Translate to 0,0,0
            q = 0
            for c in new_coords1:
                new_coords1[q][0] - mins1[0]
                new_coords1[q][1] - mins1[1]
                new_coords1[q][2] - mins1[2]
                q += 1
            if check_structures(new_coords1, new_coords2):
                sys.stdout.write("{file1} and {file2} ARE EQUAL\n".format(file1=files[i], file2=files[j]))
                continue
                


if __name__ == "__main__":
    main(sys.argv[1:])
