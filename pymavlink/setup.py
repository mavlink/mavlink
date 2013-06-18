from distutils.core import setup, Extension
import glob, os

version = '1.1.0'

from generator import mavgen, mavparse

# path to message_definitions directory
mdef_path = os.path.join(os.path.dirname(os.path.realpath(__file__)), '..', 'message_definitions')

v09_dialects = glob.glob(os.path.join(mdef_path, 'v0.9', '*.xml'))
v10_dialects = glob.glob(os.path.join(mdef_path, 'v1.0', '*.xml'))

# build all the dialects in the dialects subpackage
class Opts:
    def __init__(self, wire_protocol, output):
        self.wire_protocol = wire_protocol
        self.error_limit = 200
        self.language = 'Python'
        self.output = output

if not "NOGEN" in os.environ:
    for xml in v09_dialects:
        py = os.path.join('dialects', 'v09', os.path.basename(xml)[:-4] + '.py')
        opts = Opts(mavparse.PROTOCOL_0_9, py)
        mavgen.mavgen( opts, [xml] )

    for xml in v10_dialects:
        py = os.path.join('dialects', 'v10', os.path.basename(xml)[:-4] + '.py')
        opts = Opts(mavparse.PROTOCOL_1_0, py)
        mavgen.mavgen( opts, [xml] )

setup (name = 'pymavlink',
       version = version,
       description = 'Python MAVLink code',
       url = 'http://github.com/mavlink/mavlink',
       package_dir = { 'pymavlink' : '.' },
       packages = ['pymavlink', 'pymavlink.generator',
                   'pymavlink.dialects', 'pymavlink.dialects.v09', 'pymavlink.dialects.v10'],
       scripts = [ 'tools/magfit_delta.py', 'tools/mavextract.py',
                   'tools/mavgraph.py', 'tools/mavparmdiff.py',
                   'tools/mavtogpx.py', 'tools/magfit_gps.py',
                   'tools/mavflightmodes.py', 'tools/mavlogdump.py',
                   'tools/mavparms.py', 'tools/magfit_motors.py',
                   'tools/mavflighttime.py', 'tools/mavloss.py',
                   'tools/mavplayback.py', 'tools/magfit.py',
                   'tools/mavgpslock.py',
                   'tools/mavmission.py',
                   'tools/mavsigloss.py' ]
       )
