#!/usr/bin/env python

# this sets up the developer paths

import os, inspect, sys

devpaths = [
    os.path.realpath(os.path.join(os.path.abspath(os.path.split(
    inspect.getfile( inspect.currentframe() ))[0]),
    '..')),
]

for path in devpaths:
    if path not in sys.path:
        sys.path.insert(0,path)
