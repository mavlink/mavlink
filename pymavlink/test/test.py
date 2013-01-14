#!/usr/bin/env python

import unittest, os, inspect
import subprocess
from pymavlink import *

rootPath = os.path.realpath(os.path.join(os.path.abspath(os.path.split(inspect.getfile( inspect.currentframe() ))[0]),'..'))
exPath = os.path.join(rootPath, 'examples')
dataPath = os.path.join(rootPath, 'test', 'data')

class Test(unittest.TestCase):

    def setUp(self):
        pass

    def test_simple(self):
        print 'test simple'

    def test_logdumpv09(self):
        prog = os.path.join(exPath,'mavlogdump.py')
        data = os.path.join(dataPath,'mav09.log')
        try:
            log = subprocess.check_call([prog,data])
            #log = subprocess.check_call("exit 1", shell=True, stdout=subprocess.STDOUT)
        except subprocess.CalledProcessError:
            print log
