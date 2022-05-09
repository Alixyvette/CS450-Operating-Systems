#!/usr/bin/env python
import os
import sys
import string
import struct

from itertools import repeat

fibs  = [1, 2, 3, 5, 8, 13, 21, 34, 55, 89, 144, 233, 377, 610, 987, 1597,
        2584, 4181, 6765, 10946, 17711, 28657, 46368, 75025, 121393, 196418]
alphs = ["q","w","e","r","t","y","u","i","o","p","a","s","d","f","m","n","b","g","h","k","j","l","z","x","c","v"]

def dump_into_file(filename):
    i = 0
    while i < 26:
        s = "".join(repeat(alphs[i], fibs[i]))
        filename.write(s)
        i += 1

if __name__ == '__main__':
    if len(sys.argv) != 3:
        sys.exit('usage: generator.py <file_size in MB> <output_location>\n example: python generator.py 100 1.in')

    output_size = int(sys.argv[1])
    output      = sys.argv[2]
    filepath    = output
    filename    = open(filepath, "a")

if os.path.exists(filepath):
    while os.path.getsize(filepath) < output_size*1024*1024:
        dump_into_file(filename)
