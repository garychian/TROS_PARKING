from conans import ConanFile, tools


class J5dvbSystemConan(ConanFile):
    name = "j5dvb_system"
    version = "2022.08.10"
    settings = "os", "compiler", "build_type", "arch"

    def package(self):
        self.copy("*")

    def package_info(self):
        self.cpp_info.libs = tools.collect_libs(self)

    def package_id(self):
    	del self.info.settings.build_type

