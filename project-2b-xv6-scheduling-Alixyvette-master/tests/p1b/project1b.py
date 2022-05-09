#! /bin/env python

import toolspath
from testing import Xv6Build, Xv6Test

class Readcount1(Xv6Test):
   name = "readcount1"
   description = "call getreadcount(), value > 0"
   tester = "ctests/" + name + ".c"
   make_qemu_args = "CPUS=1"
   point_value = 1

class Readcount2(Xv6Test):
   name = "readcount2"
   description = "call readcount() twice, no read() between"
   tester = "ctests/" + name + ".c"
   make_qemu_args = "CPUS=1"
   point_value = 1

class Readcount3(Xv6Test):
   name = "readcount3"
   description = "called readcount() with 5 reads in between"
   tester = "ctests/" + name + ".c"
   make_qemu_args = "CPUS=1"
   point_value = 1

import toolspath
from testing.runtests import main
main(Xv6Build, [Readcount1, Readcount2, Readcount3])
