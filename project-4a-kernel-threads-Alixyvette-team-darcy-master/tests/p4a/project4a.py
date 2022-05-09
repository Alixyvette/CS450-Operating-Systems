import sys, os, inspect

import toolspath
from testing import Xv6Test, Xv6Build

curdir = os.path.realpath(os.path.dirname(inspect.getfile(inspect.currentframe())))
def get_description(name):
  cfile = os.path.join(curdir, 'ctests', name+'.c')
  with open(cfile, 'r') as f:
    _ = f.readline()
    desc = f.readline()
  return desc[2:].strip()

all_tests = []
build_test = Xv6Build
for testname in '''clone_basic clone_parent_sleep clone_child_sleep clone_arguments clone_bad clone_return clone_wait fork_join
                   threads_basic threads_many threads_zombies threads_sbrk
                   lock_basic lock_setup'''.split():
  members = {
      'name': testname,
      'tester': 'ctests/' + testname + '.c',
      'description': get_description(testname),
      'timeout': 10 if testname not in ['threads_many', 'lock_basic', 'lock_setup'] else 30,
      'point_value': 10
  }
  newclass = type(testname, (Xv6Test,), members)
  all_tests.append(newclass)
  setattr(sys.modules[__name__], testname, newclass)

from testing.runtests import main
main(build_test, all_tests)
