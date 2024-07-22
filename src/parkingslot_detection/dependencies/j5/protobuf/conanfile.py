from conans import ConanFile, tools


class ProtobufConan(ConanFile):
    name = "protobuf"
    version = "3.19.3"
    settings = "os", "compiler", "build_type", "arch"

    def package(self):
        self.copy("*")

    def package_info(self):
        self.cpp_info.libs = tools.collect_libs(self)

    def package_id(self):
        del self.info.settings.build_type
        

