from conans import ConanFile, CMake, tools
from pathlib import Path
import os

hlog_version = "1.4.4"

class HlogConan(ConanFile):
    name = "hlog"
    version = hlog_version
    settings = "os", "compiler", "build_type", "arch"
    options = {}
    requires = [("fmt/9.0.0@thirdparty/stable")]
    exports_sources = "example/*"

    generators = "cmake_find_package"
    _cmake = None

    @property
    def _source_subfolder(self):
        return "source_subfolder"

    @property
    def _build_subfolder(self):
        return "build_subfolder"

    def requirements(self):
        if self.settings.arch == "armv8" and self.settings.os != "Android":
            self.requires("unwind/1.6.0@thirdparty/stable")
            self.requires("lzma/5.2.5@thirdparty/stable")

    def source(self):
        git = tools.Git(folder=self._source_subfolder)
        git.clone("git@gitlab.hobot.cc:ptd/ap/middleware/log.git", "dev-v2")

    def _configure_cmake(self):
        if self._cmake :
            return self._cmake
        self._cmake = CMake(self)
        self._cmake.definitions["CMAKE_BUILD_TYPE"] = self.settings.build_type
        if 'distro' in \
           self.settings.compiler[self.settings.compiler.value].fields \
           and self.settings.compiler.distro == "fsl":
            self._cmake.definitions["S32G"] = True  
        self._cmake.configure(build_folder=self._build_subfolder,defs={"HLOG_VERSION":hlog_version})   
        return self._cmake

    def build(self):
        cmake = self._configure_cmake()
        cmake.build()

    def package(self):
        cmake = self._configure_cmake()
        cmake.install()
        self.copy("*", src= os.path.join(self.source_folder, "output", self.name),  keep_path=True)


    def package_info(self):
        self.cpp_info.libs = [self.name]
        self.cpp_info.names["cmake_find_package"] = self.name
        self.cpp_info.set_property("cmake_find_mode", "both")
        self.cpp_info.set_property("cmake_file_name", self.name)
