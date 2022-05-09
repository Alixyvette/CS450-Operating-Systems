import shutil, os, subprocess
from testing import Test, BuildTest, pexpect

class Xv6Test(BuildTest, Test):
   name = None
   description = None
   timeout = 30
   tester = "tester.c"
   make_qemu_args = ""
   point_value=0

   def run(self):
      tester_path = self.test_path + "/" + self.tester
      self.log("Running xv6 user progam " + str(tester_path))
      self.log("Project path is: " + str(self.project_path))
      #shutil.copy(tester_path, self.project_path + "/user/tester.c")
      shutil.copy(tester_path, self.project_path + "/tester.c")


      is_success = self.make(["xv6.img", "fs.img"])
      if not is_success:
         return # stop test on if make fails

      target = "qemu-nox " + self.make_qemu_args
      if self.use_gdb:
         target = "qemu-gdb " + self.make_qemu_args
      self.log("make " + target)
      child = pexpect.spawn("make " + target,
            cwd=self.project_path,
            logfile=self.logfd,
            timeout=None)
      self.children.append(child)

      if self.use_gdb:
         gdb_child = subprocess.Popen(
               ["xterm", "-title", "\"gdb\"", "-e", "gdb"],
               cwd=self.project_path)
         self.children.append(gdb_child)

      child.expect_exact("init: starting sh")
      child.expect_exact("$ ")
      child.sendline("tester")

      # check if test passed
      patterns = ["failed", "cpu\d: panic: .*\n", "Should print 1 then 2", "[$][ ]"]
      index = child.expect(patterns)
      if index == 0:
         self.fail("tester failed")
      elif index == 1:
         self.fail("xv6 kernel panic")
      elif index == 3:
         self.fail("tester failed")

      if self.use_gdb:
         child.wait()
      else:
         child.close()
      self.done()

class Xv6Build(BuildTest):
   name = "build"
   description = "build xv6 using make"
   timeout = 60
   targets = ["xv6.img", "fs.img"]


