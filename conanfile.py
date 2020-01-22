from conans import ConanFile
import os
import re


def get_version():
    patch = 0
    minor = 0
    major = 1
    try:
        with open("CMakeLists.txt", "r") as cmake_file:
            for line in cmake_file:
                if 'set(PROJECT_VERSION_PATCH' in line:
                    patch = re.search(r'\d+', line)[0]
                if 'set(PROJECT_VERSION_MINOR' in line:
                    minor = re.search(r'\d+', line)[0]
                if 'set(PROJECT_VERSION_MAJOR' in line:
                    major = re.search(r'\d+', line)[0]
    except:
        pass
    version = str(major) + '.' + str(minor) + '.' + str(patch)
    return version


class MavlinkConan(ConanFile):
    name = "mavlink"
    version = get_version()
    license = "Tevel"
    author = "Gal gal@tevel-tech.com"
    description = "micro air vehicle communication protocol"
    topics = ("mavlink", "communication")
    exports_sources = "*"
    options = {"msg_dialect": ["ardupilotmega", "ASLUAV", "autoquad", "common", "icarous", "matrixpilot",
                                    "minimal", "paparazzi", "python_array_test", "slugs", "standard", "test",
                                   "ualberta", "uAvionix"]}
    default_options = ("msg_dialect = ardupilotmega")
    settings = "os", "compiler", "build_type", "arch"

    def source(self):
        if not os.path.isdir('./pymavlink'):
            self.run("git clone https://github.com/ardupilot/pymavlink.git")

    def build(self):
        cmd = "python -m pymavlink.tools.mavgen --lang=C++11 --wire-protocol=2.0 --output=build message_definitions/v1.0/"\
              + str(self.options.msg_dialect) + ".xml"
        self.run(cmd)

    def package(self):
        self.copy("*.h*", dst="include", keep_path=True)

    def package_info(self):
        self.cpp_info.name = "mavlink"
