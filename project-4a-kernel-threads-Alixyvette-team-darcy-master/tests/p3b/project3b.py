import sys, os, inspect

import toolspath
from testing import Xv6Test, Xv6Build

curdir = os.path.realpath(os.path.dirname(inspect.getfile(inspect.currentframe())))
def get_description(name):
  cfile = os.path.join(curdir, 'ctests', name+'.c')
  with open(cfile, 'r') as f:
    desc = f.readline()
  desc = desc.strip()
  desc = desc[2:]
  if desc[-2:] == '*/':
    desc = desc[:-2]
  return desc.strip()

all_tests = []
build_test = Xv6Build
for testname in '''null null2 bounds
                   protect_args protect_fail protect_basic protect_advanced'''.split():
  members = {
      'name': testname,
      'tester': 'ctests/' + testname + '.c',
      'description': get_description(testname),
      'timeout': 10 if testname != 'usertests' else 240,
      'point_value': 10 if testname in {
          'null', 'null2', 'bounds', 'protect_args',
          } else {}.get(testname, 20)
      }
  newclass = type(testname, (Xv6Test,), members)
  all_tests.append(newclass)
  setattr(sys.modules[__name__], testname, newclass)

from testing.runtests import main
main(build_test, all_tests)
