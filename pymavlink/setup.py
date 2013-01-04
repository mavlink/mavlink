#!/usr/bin/env python
from setuptools import setup, find_packages
import sys
import os

version = '1.0.10'

setup(name='pymavlink',
      version=version,
      description="An interface to the MAVLink protocol.",
      long_description="""\
      An interface to the MAVLink protocol.
      """,
      classifiers=[
          'Development Status :: 4 - Beta',
          'Environment :: Console',
          'Intended Audience :: Science/Research',
          'License :: OSI Approved :: GNU Lesser General Public License v3 (LGPLv3)',
          'Operating System :: OS Independent',
          'Programming Language :: Python :: 2.7',
          'Topic :: Scientific/Engineering',
          'Topic :: Text Processing :: General',
      ],
      # Get strings from
      # http://pypi.python.org/pypi?%3Aaction=list_classifiers
      keywords='',
      author='MAVLink Dev Team',
      author_email='james.goppert@gmail.com',
      url='https://github.com/mavlink/pymavlink',
      license='LPLv3',
      packages=find_packages(exclude=['ez_setup', 'examples', 'tests']),
      include_package_data=True,
      install_requires=['pyserial'],
      package_dir={'pymavlink': 'pymavlink'},
      #package_data={'pymavlink': ['templates/*']},
      #entry_points={
        #'console_scripts': [
            #'cmd_name = package:Module.func'
        #]},
      )
