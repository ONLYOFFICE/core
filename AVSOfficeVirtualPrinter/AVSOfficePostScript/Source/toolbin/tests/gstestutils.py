#    Copyright (C) 2002 Artifex Software, Inc.
#    All Rights Reserved.
#
# This software is provided AS-IS with no warranty, either express or
# implied.
#
# This software is distributed under license and may not be copied, modified
# or distributed except as expressly authorized under the terms of that
# license.  Refer to licensing information at http://www.artifex.com/
# or contact Artifex Software, Inc.,  7 Mt. Lassen Drive - Suite A-134,
# San Rafael, CA  94903, U.S.A., +1(415)492-9861, for further information.

# $Id: gstestutils.py 8409 2007-11-27 20:43:09Z giles $

# Utilities and documentation for Ghostscript testing using the Python
# 'unittest' framework.

# ------------------------ Documentation ------------------------ #

# Ghostscript provides a framework for writing test scripts that can be run
# either individually or as part of a regression test suite.  The purpose of
# this documentation is to help you write new tests within this framework.

# This framework is based on the Python 'unittest' framework, documented at
#	http://www.python.org/doc/current/lib/module-unittest.html

# Each test (or set of closely related tests) is in a separate file named
# check_xxx.py (if it does not run Ghostscript) or gscheck_xxx.py (if it
# does run Ghostscript).  Each test file must include the statement
#
#	from gstestutils import GSTestCase, gsRunTestsMain
#
# The main body of the file must define a 'test case' class derived from
# GSTestCase, e.g.,
#
#	class GSCheckGravity(GSTestCase):
#
# If instances of the test case require any additional parameters (e.g.,
# a directory name), the class must define an __init__ method that stores
# such parameters in instance variables, e.g.,
#
#	def __init__(self, planet):
#	    self.planet = planet
#	    GSTestCase.__init__(self)
#
# The last line above is required as the last line of any such __init__
# method.
#
# The test case class must provide a one-line description of its function.
# This description will be printed as part of any failure message.  If the
# description does not depend on any parameters, it can be included as a
# literal string as the first thing in the runTest method, e.g.,
#
#	def runTest(self):
#	    """The law of gravity must be operating."""
#
# If the description does depend on parameters provided to __init__, a
# separate shortDescription method is required, e.g.,
#
#	def shortDescription(self):
#	    return "Planet %s must have gravity." % self.planet
#
# The main body of the test must be a method called runTest.  To indicate
# a failure, it can call any of the self.failxxx messages defined for class
# unittest.TestCase.  The failure message can be either a string (possibly
# including embedded \n characters) or a list of strings, which will be
# printed on individual lines.  One useful method added in GSTestCase is
# failIfMessages: this takes a list of strings as a parameter, and fails
# if the list is not empty.  You can use it like this:
#
#	def runTest(self):
#	    messages = []
#	    if not self.feathersFall():
#		messages.append("Feathers don't fall.")
#	    if not self.leadWeightsFall():
#		messages.append("Lead weights don't fall.")
#	    self.failIfMessages(messages)
#
# At the end of the file there must be an addTests function (not a class
# method) that adds all the tests defined in the file to a suite, e.g.,
#
#	def addTests(suite, **args):
#	    for planet in ['Earth', 'Mars', 'Dune']:
#		suite.addTest(GSCheckGravity(planet))
#
# addTests may take additional parameters that a test harness will provide,
# which may be either required or optional.  In particular, any test that
# examines the Ghostscript source code must take a parameter named gsroot
# that is the prefix (not the directory) for the top-level Ghostscript
# directory.  The way to define addTests in this case is
#
#	def addTests(suite, gsroot, **args):
#
# The very last thing in the file must be the following lines to allow the
# file to be run stand-alone:
#
#	if __name__ == "__main__":
#	    gsRunTestsMain(addTests)
#
# Among its other functions, gsRunTestsMain parses the command line and
# provides default argument values: see its documentation below.

# ------------------------ End documentation ------------------------ #

import string, types, unittest
# gsRunTestsMain() imports additional modules, see below

# Define the exception that Ghostscript tests will raise when a failure
# occurs.  The result class will recognize this exception and print only
# the failure message rather than a traceback.

class GSTestFailure(AssertionError):
    pass

# Define a TestCase subclass that raises GSTestFailure for failures,
# and provides a method that fails iff an accumulated message string
# is not empty.

class GSTestCase(unittest.TestCase):

    def __init__(self):
        unittest.TestCase.__init__(self)

    failureException = GSTestFailure

    def failIfMessages(self, messages):
        self.failIf(len(messages) > 0, messages)

# Define a TestResult class that recognizes the GSTestFailure exception
# as described above, and a TestRunner class that uses it.
# The TestResult class also accepts a list or tuple of strings as the
# error message.

class _GSTextTestResult(unittest._TextTestResult):

    def addFailure(self, test, err):
        self.failures.append((test, err))
        if self.showAll:
	    lines = err[1].args[0]
	    if (len(lines) > 18) & (lines[0:18] == "non-zero exit code"):
		self.stream.writeln("ERROR")
	    else:
		self.stream.writeln("DIFFER")
        elif self.dots:
            self.stream.write("D")
    
    def printErrorList(self, flavor, errors):
        handoff = []
        for test, err in errors:
            if isinstance(err[1], GSTestFailure):
                self.stream.writeln(self.separator1)
                self.stream.writeln("%s: %s" % (flavor, self.getDescription(test)))
                self.stream.writeln(self.separator2)
                lines = err[1].args[0]
                if type(lines) == types.StringType:
                    lines = string.split(lines, '\n')
                    if lines[-1] == '':
                        del lines[-1]
                for line in lines:
                    self.stream.writeln(line)
            else:
                handoff.append((test, err))
            self.stream.writeln("")
        if len(handoff) > 0:
            unittest._TextTestResult.printErrorList(self, flavor, handoff)

class GSTestRunner(unittest.TextTestRunner):

    def _makeResult(self):
        return _GSTextTestResult(self.stream, self.descriptions, self.verbosity)

# Define a class to make it unnecessary for importers of gstestutils
# to import unittest directly.

class GSTestSuite(unittest.TestSuite):
    pass

# Run a set of tests specified by an addTests procedure, invoked from the
# command line.  Switches of the form --var=value provide arguments for
# addTests.  Also, default gsroot to the value in gsconf.py, gsconf.gsroot,
# if not specified on the command line.

def gsRunTestsMain(addTests):
    import sys
    import gsconf
    try:
      args = {'gsroot': gsconf.gsroot}
    except AttributeError:
      args = {}
    gsTestParseArgv(args, sys.argv)
    suite = GSTestSuite()
    addTests(suite, **args)
    results = GSTestRunner().run(suite)
    failures = len(results.failures)
    if failures:
      sys.exit(failures)

# Parse sys.argv to extract test args.

def gsTestParseArgv(args, argv):
    import re
    arg_re = re.compile(r'\-\-([a-zA-Z0-9_]+)=(.*)$')
    for arg in argv:
        match = arg_re.match(arg)
        if match:
            args[match.group(1)] = match.group(2)
