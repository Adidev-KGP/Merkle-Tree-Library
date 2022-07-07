# pylint: disable=missing-module-docstring
import importlib
import types
from inspect import getmembers, isfunction
import sys
import os
import colors

class Runner:

    '''
      This class contains the following methods:
      1)load_test_files() --> this methods checks for file
                            names that start with test
                            under a certain given directory.
 
      2)load_tests() --> this method returns the files starting
                        with test under the given directory

      3)load_module() --> this method loads the modules specific
                          to tests.

      4)run_single_file() --> this methods runs single file whoose
                              name starts with test.

      5)run() --> this method runs all the files starting with test
                  with the iteration of run_single_file() method.

    '''

    def __init__(self, path):
        self.test_files = []
        self.success = True
        self.load_test_files(path)

    def load_test_files(self, path)->None:
        if path.endswith("__pycache__"):
            return

        if os.path.isfile(path):
            self.test_files.append(path)
        elif os.path.isdir(path):
            for nested_path in os.listdir(path):
                self.load_test_files(path + "/" + nested_path)

    def load_tests(self, mod):
        return [m for m in getmembers(mod) if isfunction(m[1]) and m[0].startswith("test_")]

    def load_module(self, file):
        '''
          in order to have this module available to us we need two elements :
          first is the variable, a place that will be a module ;
          second is a loader :
          it is some function  that will parse the whole file as a one long string
          it will execute it and it will pack it into this  module variable
        '''
        loader = importlib.machinery.SourceFileLoader("testmod", file)
        mod = types.ModuleType("testmod")
        loader.exec_module(mod)
        return mod

    def run_single_file(self, file):

        '''
          This fucntion fetches the modules
          inside which there are functions
          which start with test.
        '''

        mod = self.load_module(file)
        tests = self.load_tests(mod)
        for test in tests:
            (test_name, test_function) = test
            try:
                print(f"\n{colors.PURPLE}running-{test_name}{colors.RESET}")
                test_function()
                print(f"{test_name} - {colors.GREEN}success{colors.RESET}")
            except AssertionError:
                print(f"{test_name} - {colors.RED}failure{colors.RESET}")
                self.success = False

    def run(self):
        '''
          we have to turn this string with file path to the file
          into a python module that is considered a real
          code and that can be  executed.
        '''

        for test_file in self.test_files:
            self.run_single_file(test_file)
        if self.success:
            print(f"\n{colors.GREEN}Overall tests succeeeded{colors.RESET}")
        else:
            print(f"\n{colors.RED}Overall tests failed{colors.RESET}")

if __name__ == "__main__":
    Runner(sys.argv[1]).run()
