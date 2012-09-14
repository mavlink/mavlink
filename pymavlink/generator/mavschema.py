#!/usr/bin/env python
'''
Xml schema validator for mavlink generator

For xsd implementation see: mavlink_schema.xsd

For information on the python xsd library, pyxsd, see:
    http://pyxsd.org/

Copyright David Goodman 2012
Released under GNU GPL version 3 or later
'''

from mavparse import MAVParseError

class MAVSchema(object):
    '''xml schema validator'''
    def __init__(self,xmlfile):

