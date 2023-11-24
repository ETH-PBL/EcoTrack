#!/bin/env python

import argparse
import sys
from py_implementation.algorithm import Algorithm

# Constants
beta1 = -0.203
beta2 = 0.468
gamma = 0.67


if __name__ == "__main__":
    # Parse arguments
    parser = argparse.ArgumentParser(description="Validation of smart energy-aware algorithm")
    parser.add_argument("-i", help="Input file", required=True)
    parser.add_argument("-o", help="Output file - optional")
    args = parser.parse_args()

    # Open files
    try:
        infile =  open(args.i, 'r')
    except IOError:
        print("Unable to open file!", file=sys.stderr)
        exit(1)

    if args.o:
        try:
            outfile =  open(args.o, 'w')
            outfile.write("battery_level, k\n")
        except IOError:
            print("Unable to open output file!", file=sys.stderr)
            args.o = None

    if args.o is None:
        print("battery_level, k")

    
    # Instantiate algorithm
    algorithm = Algorithm(beta1, beta2, gamma, 11.4, 1)

    # Read lines of input file, update algorithm, print k
    for line in infile:
        try:
            number = float(line)
        except ValueError:
            # if line contains no number
            continue
        k = algorithm.update_algorithm(number)
        if args.o:
            outfile.write(f"{number:.3f}, {k}\n")
        else:
            print(f"{number:.3f}, {k}")

    infile.close()
    if args.o:
        outfile.close()
