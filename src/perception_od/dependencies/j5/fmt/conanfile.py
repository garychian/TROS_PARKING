from conans import ConanFile, tools


class FmtConan(ConanFile):
    name = "fmt"
    version = "9.0.0"
    settings = "os", "compiler", "build_type", "arch"

    def package(self):
        self.copy("*")

    def package_info(self):
        self.cpp_info.libs = tools.collect_libs(self)

    def package_id(self):
    	del self.info.settings.build_type

