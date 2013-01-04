#!/usr/bin/env python

import inspect
import os
import subprocess
import unittest

from pymavlink import *


# Real, absolute path of directory above the script location
root_path = os.path.realpath(os.path.join(
    os.path.abspath(os.path.split(inspect.getfile(inspect.currentframe()))[0]),
    os.path.pardir))
ex_path = os.path.join(root_path, 'examples')
data_path = os.path.join(root_path, 'test', 'data')


def run_cmd(cmd, input=None):
    """
    cmd (iterable): program and arguments to execute
    input (string): input for STDIN
    returns 3-tuple: (returncode, stdout, stderr)
    """
    PIPE = subprocess.PIPE
    proc = subprocess.Popen(cmd, stdin=PIPE, stdout=PIPE, stderr=PIPE)
    out, err = proc.communicate(input)
    return proc.returncode, out, err


class Test(unittest.TestCase):

    def setUp(self):
        pass

    def test_simple(self):
        print 'test simple'

    def test_logdumpv09(self):
        prog = os.path.join(ex_path, 'mavlogdump.py')
        data = os.path.join(data_path, 'mav09.log')
        input = None
        retcode, out, err = run_cmd([prog, data], input)
        if retcode != 0:
            print err
