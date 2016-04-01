#!/usr/bin/env python
'''
mavlink expression evaluation functions

Copyright Andrew Tridgell 2011
Released under GNU GPL version 3 or later
'''

import os

# these imports allow for mavgraph and mavlogdump to use maths expressions more easily
from math import *
from .mavextra import *

'''
Support having a $HOME/.pymavlink/mavextra.py for extra graphing functions
'''
home = os.getenv('HOME')
if home is not None:
    extra = os.path.join(home, '.pymavlink', 'mavextra.py')
    if os.path.exists(extra):
        import imp
        mavuser = imp.load_source('pymavlink.mavuser', extra)
        from pymavlink.mavuser import *

def evaluate_expression(expression, vars):
    '''evaluation an expression'''
    try:
        v = eval(expression, globals(), vars)
    except NameError:
        return None
    except ZeroDivisionError:
        return None
    return v
