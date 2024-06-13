from conans import ConanFile, CMake, tools
from pathlib import Path
import os


class ProtocolConan(ConanFile):
    name = "fanya-protocol"
    version = "1.0.0"
    settings = "os", "compiler", "build_type", "arch"
    options = {"accelerate_type": ["none",
                                   "pb_pb",
                                   "pb_cyclonedds",
                                   "pb_fastdds"]}
    default_options = {"accelerate_type": "none"}
    requires = [("protobuf/3.19.6@thirdparty/stable")]

    def requirements(self):
        if self.options.accelerate_type == "pb_pb":
            self.requires("message/[1.7.x]@bole/stable")
        if self.options.accelerate_type == "pb_fastdds":
            self.requires("message/[1.7.x]@bole/stable")
            self.requires("fast-dds/2.10.1@thirdparty/stable")
        if self.options.accelerate_type == "pb_cyclonedds":
            self.requires("message/[1.7.x]@bole/stable")
            self.requires("cyclonedds/0.10.0@thirdparty/stable")


    generators = "cmake_find_package"
    _cmake = None

    @property
    def _build_subfolder(self):
        return "build_subfolder"


    def source(self):
        git = tools.Git(folder=self.source_folder)


    def _configure_cmake(self):
        if self._cmake :
            return self._cmake
        self._cmake = CMake(self)
        if self.options.accelerate_type == "none":
            self._cmake.definitions["ACCELERATE"] = "none"
        if self.options.accelerate_type == "pb_pb":
            self._cmake.definitions["ACCELERATE"] = "pb"
        if self.options.accelerate_type == "pb_cyclonedds":
            self._cmake.definitions["ACCELERATE"] = "cyclonedds"
        if self.options.accelerate_type == "pb_fastdds":
            self._cmake.definitions["ACCELERATE"] = "fastdds"
        if self.settings.os == "Android":
            self._cmake.definitions["Android"] = "True"
            self._cmake.definitions["CONAN_CMAKE_TOOLCHAIN_FILE"] = \
                os.getenv("CONAN_CMAKE_TOOLCHAIN_FILE")
            self._cmake.definitions["ANDROID_ABI"] = os.getenv("ANDROID_ABI")
            self._cmake.definitions["ANDROID_NATIVE_API_LEVEL"] = \
                os.getenv("ANDROID_NATIVE_API_LEVEL")

        if self.settings.os == "Macos":
            self._cmake.definitions["python_cmd"] = "python3"

        self._cmake.definitions["CMAKE_BUILD_TYPE"] = self.settings.build_type

        self._cmake.definitions["arch"] = "NONE"
        if self.settings.os == "Linux":
            self._cmake.definitions["arch"] = "LINUX_X64"
            if self.settings.compiler.version == "6.5":
                self._cmake.definitions["arch"] = "AARCH64_J3"
            if self.settings.compiler.version == "9.3":
                self._cmake.definitions["arch"] = "AARCH64_J5"
        else:
            self._cmake.definitions["arch"] = "NONE"
        self._cmake.configure(build_folder=self._build_subfolder)
        return self._cmake

    def build(self):
        cmake = self._configure_cmake()
        cmake.build()

    def package(self):
        cmake = self._configure_cmake()
        cmake.install()
        self.copy(
            "*",
            src= os.path.join(self.source_folder, "output", self.name),
            keep_path=True
        )


    def package_info(self):
        self.cpp_info.libs = [self.name]
        self.cpp_info.names["cmake_find_package"] = self.name
        self.cpp_info.set_property("cmake_find_mode", "both")
        self.cpp_info.set_property("cmake_find_name", self.name)
