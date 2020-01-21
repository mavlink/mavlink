from conans import ConanFile

__version__ = "1.0.0"

class MavlinkConan(ConanFile):
    name = "mavlink"
    version = __version__
    license = "Tevel"
    author = "Gal gal@tevel-tech.com"
    description = "micro air vehicle communication protocol"
    topics = ("mavlink", "communication")
    settings = "os", "compiler", "build_type", "arch"
    exports_sources = "*"

    def source(self):
        self.run("git clone https://github.com/ardupilot/pymavlink.git")

    def build(self):
        self.run("python -m pymavlink.tools.mavgen --lang=C++11 --wire-protocol=2.0 --output=build message_definitions/v1.0/ardupilotmega.xml")

    def package(self):
        self.copy("*.h*", dst="include", keep_path=True)

    def package_info(self):
        self.cpp_info.libs = ["mavlink"]
