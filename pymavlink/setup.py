from distutils.core import setup, Extension
 
setup (name = 'pymavlink',
       version = '1.0',
       description = 'Python mavlink code - see http://www.qgroundcontrol.org/mavlink/mavproxy_startpage',
       packages = ['pymavlink', 'pymavlink.generator'],
       scripts = [ 'pymavlink/tools/magfit_delta.py', 'pymavlink/tools/mavextract.py',
                   'pymavlink/tools/mavgraph.py', 'pymavlink/tools/mavparmdiff.py',
                   'pymavlink/tools/mavtogpx.py', 'pymavlink/tools/magfit_gps.py',
                   'pymavlink/tools/mavflightmodes.py', 'pymavlink/tools/mavlogdump.py',
                   'pymavlink/tools/mavparms.py', 'pymavlink/tools/magfit_motors.py',
                   'pymavlink/tools/mavflighttime.py', 'pymavlink/tools/mavloss.py',
                   'pymavlink/tools/mavplayback.py', 'pymavlink/tools/magfit.py',
                   'pymavlink/tools/mavgpslock.py',
                   'pymavlink/tools/mavmission.py',
                   'pymavlink/tools/mavsigloss.py' ]
       )
